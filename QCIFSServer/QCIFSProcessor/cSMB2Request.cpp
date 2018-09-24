// Copyright 2018 Grass Valley, A Belden Brand

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//cSMB2Request.cpp

#include "stdafx.h"
#include "main.h"
#include "cSMB2Request.h"
#include "socket_routines.h"
#include "cSMB2Response.h"
#include "align.h"
#include "iTreeResourceFactory.h"
#include "iShareManager.h"
#include "cSMB2Session.h"
#include "cSMB2Tree.h"
#include "SMB2FsInfoLevel.h"
#include "SMB2FileInfoLevel.h"
#include "SMB2SecurityDescriptor.h"
#include "iOplockManager.h"
#include <time.h>
#include "safeName.h"
#include "cSMB2NotificationReceiver.h"
#include "iUniqueNotifyID.h"
#include "cGuardedUniqueNotify.h"
#include "cQCIFSPacketProcessor.h"

#include "..\iComposite.h"
#include "..\QCIFSFwk\iChildLoaderVisitor.h"

//#define kUSE_SMB3
//#define kUSE_SMB_NETWORK_INTERFACE_INFO

#ifdef kUSE_SMB_NETWORK_INTERFACE_INFO
typedef struct _NETWORK_INTERFACE_INFO
{
  ULONG IpAddress;
  ULONG Capability;
  ULONGLONG LinkSpeed;
} tNETWORK_INTERFACE_INFO;

tNETWORK_INTERFACE_INFO NetworkInfos[] = 
{ 
//  {0x0aa20101, 0, 1000000000} //10.162.01.01
//  {0x0aa20101, RSS_CAPABLE, 1000000000} //10.162.01.01
//, {0x0aa2010a, RSS_CAPABLE, 1000000000} //10.162.01.10
//  {0x7f000001, 0, 1000000000} //127.0.0.1
  {0xc0a88201, RSS_CAPABLE, 1000000000} //192.168.130.1
 ,{0xc0a8d601, RSS_CAPABLE, 1000000000} //192.168.214.1
};
#endif kUSE_SMB_NETWORK_INTERFACE_INFO

using namespace std;
using namespace vfs;
extern tActivityMap ActivityQCIFSProcessors;
extern tActivityMap ActivityQCIFSProcessors2;

static ULONGLONG sessID = USHRT_MAX + 1;  //don't let SMB1 and SMB2 session Ids overlap for notification IDS
cSMB2Request::tProcessFuncPtr cSMB2Request::Dispatch[UCHAR_MAX] = {0};

typedef std::map<ULONGLONG, cPtr<cSMB2Session> > tSessionMap;
static tSessionMap gSessionMap;
static cLock gSessionLock;

bool requestRequireSign(const PSMB2_HEADER pReqHeader)
{
  return pReqHeader->Flags &  SMB2_FLAGS_SIGNED;
}

///////////////////////////////////////////////////////////////////////////////////////

static USHORT HomePort = 0;
int getColour(USHORT port)
{
  if(!HomePort)
    HomePort = port;

  int colour = (port - HomePort);
  return colour;
}

static ULONG HomeAddress = 0;
int getNicNumber(ULONG ipAddress)
{
  if(!HomeAddress)
    HomeAddress = ipAddress;

  bool homeAddressTest = (ipAddress == HomeAddress);
  return (homeAddressTest?0:1);
}

///////////////////////////////////////////////////////////////////////////////////////
// cSMB2Request ///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

cSMB2Request::cSMB2Request(const cPtr<iRXPacket> pRXPacket
  , const cPtr<iShareManager> pShareManager
  , const cPtr<iOpLockManager> pOpLockManager
  , cQCIFSPacketProcessor* pQCIFSPacketProcessor) 
: cSMB2Buffer(getSmbBufferFromNetBIOS(reinterpret_cast<unsigned char*>(pRXPacket->getRXBuffer()->getBytes())))
, m_pContextPacket(pRXPacket)
, m_pShareManager(pShareManager)
, m_pOpLockManager(pOpLockManager)
, m_pQCIFSPacketProcessor(pQCIFSPacketProcessor)
{
}

cSMB2Request::~cSMB2Request() 
{
}

///////////////////////////////////////////////////////////////////////////////////////
// Generic Handlers ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

void cSMB2Request::getResponses(cPtr<cMemoryView> pMem, tResponseList& responses, vfs::iActivity::Ptr activity)
{
  // check request header
  Header = getSmb2(Buffer,BufferSize);
  PSMB_HEADER oldHeader = getSmb(Buffer,BufferSize);

  try
  {
    unsigned int offset = sizeof(SMB2_HEADER);
    if(offset > BufferSize)
      throw exception();

    cPtr<cSMB2Response> pResponse;
    {
      activity->activityStart(kCreateResponse);
      pResponse = new cSMB2Response(this, pMem->getBytes(), pMem);
      activity->activityStart(kGetBlocks);
    }

    if(!Header && oldHeader && SMB_COM_NEGOTIATE == oldHeader->Command)
    {
      responses = processNegotiate(pResponse);
    }
    else
    {
      USHORT command = static_cast<USHORT>(Header->Command);
      cActivityScope as1(activity, kGetDispatchCommand, command);
      responses = dispatchCommand(command, pResponse);
    }
  }
  catch (const exception & err)
  {
    QSOS((L"cSMB2Request::getResponses exception: %S", err.what()));
    cPtr<cMemoryView> pMem = new cMemoryView(
      new cMemory(checked_static_cast<size_t>(gkSMB2BufferSize), cMemory::eVirtual)
      , 0
      , checked_static_cast<size_t>(gkSMB2BufferSize));
    cPtr<cSMB2Response> pResponse = new cSMB2Response(this
      , pMem->getBytes()
      , pMem);
    pResponse->setDosError(SMB_ERR_CLASS_HARDWARE, SMB_ERR_GENERAL);
    responses.push_back(pResponse);
  }
  catch(cRecoverable& err)
  {
    QSOS((L"cSMB2Request::getResponses exception: %s", err.getMsg().c_str()));
    cPtr<cMemoryView> pMem = new cMemoryView(
      new cMemory(checked_static_cast<size_t>(gkSMB2BufferSize), cMemory::eVirtual)
      , 0
      , checked_static_cast<size_t>(gkSMB2BufferSize));
    cPtr<cSMB2Response> pResponse = new cSMB2Response(this
      , pMem->getBytes()
      , pMem);
    pResponse->setDosError(SMB_ERR_CLASS_HARDWARE, SMB_ERR_GENERAL);
    responses.push_back(pResponse);
  }
}

cPtr<cSMB2Session> cSMB2Request::getSession(const cPtr<cSMB2Response>& pResponse) const
{
  {
    cLockGuard lg(&gSessionLock);
    const tSessionMap::const_iterator finder = gSessionMap.find(SessionID());
    if (gSessionMap.end() != finder)
    {
      return finder->second;
    }
  }

  QSOS((L"Invalid Session ID"));
  pResponse->setDosError(SMB_ERR_CLASS_SERVER, SMB_ERR_BAD_UID);
  return cPtr<cSMB2Session>();
}

cPtr<cSMB2Tree> cSMB2Request::getTree(const cPtr<cSMB2Response>& response) const
{
  const cPtr<cSMB2Session> pSession(getSession(response));
  if (pSession.isValid())
    return pSession->getTree(TreeID());

  response->setDosError(SMB_ERR_CLASS_SERVER, SMB_ERR_BAD_TID);
  return cPtr<cSMB2Tree>();
}

template <typename ReqType>
cPtr<iComposite> cSMB2Request::getComposite(ReqType*& pReq
  , const cPtr<cSMB2Response>& response) const
{
  const cPtr<cSMB2Tree> pTree = getTree(response);
  if (pTree.isNull())
  {
    response->setNTError(static_cast<ULONG>(STATUS_INVALID_HANDLE));
    return cPtr<iComposite>();
  }

  pReq = getParams<ReqType*>();
  return pTree->getComposite(Fid(pReq), response);
}

inline tResponseList cSMB2Request::singleResponse(const cPtr<cSMB2Response>& pResponse) const
{
  tResponseList retList;
  retList.push_back(pResponse);
  return retList;
}

inline tResponseList cSMB2Request::errorResponse(const cPtr<cSMB2Response>& pResponse) const
{
  return processChain(pResponse, sizeof(RESP_SMB2_ERROR));
}

inline tResponseList cSMB2Request::errorResponse(const cPtr<cSMB2Response>& pResponse
                                                 , const DWORD ntError
                                                 , UCHAR errorData) const
{
  pResponse->setNTError(ntError, errorData);
  return errorResponse(pResponse);
}

inline tResponseList cSMB2Request::errorResponse(const cPtr<cSMB2Response>& pResponse
  , const UCHAR errorClass
  , const DWORD error) const
{
  pResponse->setDosError(errorClass, error);
  return errorResponse(pResponse);
}

tResponseList cSMB2Request::dispatchCommand(USHORT command, cPtr<cSMB2Response> pResponse) const
{
  if(!Dispatch[0])
  {
    for(int i = 0; i < UCHAR_MAX; ++i)
    {
      Dispatch[i] = &cSMB2Request::processUnhandled;
    }
    Dispatch[SMB2_NegotiateProtocol] = &cSMB2Request::processNegotiate2;
    Dispatch[SMB2_SessionSetup] = &cSMB2Request::processSessionSetup;
    Dispatch[SMB2_SessionLogoff] = &cSMB2Request::processSessionLogoff;
    Dispatch[SMB2_TreeConnect] = &cSMB2Request::processTreeConnect;
    Dispatch[SMB2_TreeDisconnect] = &cSMB2Request::processTreeDisconnect;
    Dispatch[SMB2_Create] = &cSMB2Request::processCreate;
    Dispatch[SMB2_Close] = &cSMB2Request::processClose;
    Dispatch[SMB2_Flush] = &cSMB2Request::processFlush;
    Dispatch[SMB2_Read] = &cSMB2Request::processRead;
    Dispatch[SMB2_Write] = &cSMB2Request::processWrite;
    Dispatch[SMB2_Lock] = &cSMB2Request::processLock;
    Dispatch[SMB2_Ioctl] = &cSMB2Request::processIoctl;
    Dispatch[SMB2_Cancel] = &cSMB2Request::processCancel;
    Dispatch[SMB2_KeepAlive] = &cSMB2Request::processKeepAlive;
    Dispatch[SMB2_Find] = &cSMB2Request::processFind;
    Dispatch[SMB2_Notify] = &cSMB2Request::processNotify;
    Dispatch[SMB2_GetInfo] = &cSMB2Request::processGetInfo;
    Dispatch[SMB2_SetInfo] = &cSMB2Request::processSetInfo;
    Dispatch[SMB2_Break] = &cSMB2Request::processBreak;
  }

  return (this->*Dispatch[command])(pResponse);
}

tResponseList cSMB2Request::processChain(cPtr<cSMB2Response> pResponse, const ULONG curResponseLength) const
{
  if (haveChainedHeader())
  {
    if (curResponseLength)
    {
      const PUCHAR pCurResponseHeader(reinterpret_cast<PUCHAR>(pResponse->header()));
      const PUCHAR pNextResponseStartUnaligned(pCurResponseHeader + curResponseLength + sizeof(SMB2_HEADER));
      PUCHAR pNextResponseStart(pNextResponseStartUnaligned);
      align<ULONGLONG>(pNextResponseStart, pCurResponseHeader);
      pResponse->header()->NextCommand = checked_static_cast<ULONG>(pNextResponseStart - pCurResponseHeader);

      //if(SMB2_FLAGS_SIGNED & pResponse->header()->Flags)
      //{
      //  cPtr<cSMB2Session> session(getSession(pResponse));
      //  pResponse->sign(session);
      //}

      pResponse->haveChainedHeader();
      makeReplyHeader(header(), pResponse->header());
      pResponse->addData(sizeof(SMB2_HEADER) + (pNextResponseStart - pNextResponseStartUnaligned));
    }
    return dispatchCommand(header()->Command, pResponse);
  }

  //if(SMB2_FLAGS_SIGNED & pResponse->header()->Flags)
  //{
  //  cPtr<cSMB2Session> session(getSession(pResponse));
  //  pResponse->sign(session);
  //}

  return singleResponse(pResponse);
}

///////////////////////////////////////////////////////////////////////////////////////
// Command Handlers ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

tResponseList cSMB2Request::processUnhandled(cPtr<cSMB2Response> pResponse) const
{
  return errorResponse(pResponse, SMB_ERR_CLASS_DOS, SMB_ERR_BAD_FUNCTION);
}

extern FILETIME BootFileTime;
tResponseList cSMB2Request::processNegotiate(cPtr<cSMB2Response> pResponse) const
{
  const char* SUPPORTED_DIALECT_STRINGS[] = {SMB2_PROTOCOL_02_FF, SMB2_PROTOCOL_02_02};//When offering SMB2, ONLY offer SMB2 - useful for QTube.
  //const char* SUPPORTED_DIALECT_STRING = SMB2_PROTOCOL_02_02;

  //SNIA SDC 2011
  char* data = reinterpret_cast<char*>(header()) + sizeof(SMB_HEADER);

  const USHORT lenDIALECTS = sizeof(SUPPORTED_DIALECT_STRINGS) / sizeof(SUPPORTED_DIALECT_STRINGS[0]);
  for (USHORT i=0; i<lenDIALECTS; ++i)
  {
    int count = 0;
    unsigned int offset = sizeof(UCHAR) + sizeof(USHORT) + 1; // size of WordCount & ByteCount fields plus skip the 0x02 at the start!!!
    offset += getAlignment<WORD>(reinterpret_cast<PUCHAR>(data), reinterpret_cast<PUCHAR>(header()));

    while(offset < BufferSize)
    {
      char* dialect = (char*)(data + offset);
      QTRACE((L"cSMB2Request - %d:%S",count,dialect));
      if (!strcmp(dialect, SUPPORTED_DIALECT_STRINGS[i])) 
      {
        break;
      }
      ++count;
      offset += checked_static_cast<unsigned int>(strlen(dialect)) + 1 + sizeof(unsigned char);
    }

    if(offset < BufferSize)//we've found SMB2 !!!
    {
      if(i == 0)
        return processNegotiate2Any(pResponse);
      return processNegotiate2(pResponse);
    }
  }
  return singleResponse(pResponse);
}

namespace {
USHORT getDialectRevision()
{
#ifdef kUSE_SMB3
  return 0x0300;      //SMB3 in Win8 & Win2k12 - JCJCJC SNIA SMB Plugfest 2012 & 2013!!!
#endif
  //return 0x0210; //JCJCJC SNIA SMB Plugfest 2013!!!
  return 0x0202;
}

USHORT getSecurity()
{
  return SMB2_NEGOTIATE_SIGNING_ENABLED;
  //return SMB2_NEGOTIATE_SIGNING_ENABLED | SMB2_NEGOTIATE_SIGNING_REQUIRED;
}

cGuid getGuid(ULONG nicNumber)
{
  //QTRACE((L"getGuid nicNumber %d", nicNumber));
  //should be persistent - in LanManServer registry key on Windows Server 2008.

  //if(nicNumber)
  //  return cGuid(L"{1B1472F8-C0A1-49da-8427-5503949C630E}");
  return cGuid (L"{04C132C4-E81D-4c69-93A1-1A4BF85BFFF1}");
}

ULONG getCapabilities()
{
#ifdef kUSE_SMB3
  //return SMB2_GLOBAL_CAP_DFS;
  //return SMB2_GLOBAL_CAP_DFS | SMB2_GLOBAL_CAP_MULTI_CHANNEL | SMB2_GLOBAL_CAP_PERSISTENT_HANDLES;//JCJCJC SNIA SMB Plugfest 2012 - experimental ...
  //return SMB2_GLOBAL_CAP_DFS | SMB2_GLOBAL_CAP_MULTI_CHANNEL;//JCJCJC SNIA SMB Plugfest 2012 - experimental ...
  //return SMB2_GLOBAL_CAP_MULTI_CHANNEL | SMB2_GLOBAL_CAP_PERSISTENT_HANDLES;//JCJCJC SNIA SMB Plugfest 2013 - experimental ...
  //return 0x127;//JCJCJC SNIA SMB Plugfest 2013 - random test ... noooooo!
  //return SMB2_GLOBAL_CAP_DFS;
#ifdef kUSE_SMB_NETWORK_INTERFACE_INFO
  return SMB2_GLOBAL_CAP_MULTI_CHANNEL;//JCJCJC SNIA SMB Plugfest 2013 - experimental ...
//  return SMB2_GLOBAL_CAP_DFS | SMB2_GLOBAL_CAP_MULTI_CHANNEL;//JCJCJC SNIA SMB Plugfest 2013 - experimental ...
#else
  return SMB2_GLOBAL_CAP_DFS;
  //return SMB2_GLOBAL_CAP_DFS | SMB2_GLOBAL_CAP_LARGE_MTU; // MWMWMW SNIA SMB Plugfest 2014 - experimental
#endif //kUSE_SMB_NETWORK_INTERFACE_INFO
#endif //kUSE_SMB3
  return 0;//SMB2_GLOBAL_CAP_DFS;
}
} //end of namespace 

tResponseList cSMB2Request::processNegotiate2Any(cPtr<cSMB2Response> pResponse) const
{
  const PRESP_SMB2_NEGOTIATE pResp = pResponse->getResponseType<RESP_SMB2_NEGOTIATE>();
  pResp->StructureSize = 65;  //[#2] p.29
  pResp->SecurityMode = getSecurity();

  pResp->DialectRevision = 0x02FF;
  pResp->Reserved = 0;

  std::pair<ULONG, USHORT> addressAndPort = m_pContextPacket->getIpAddressAndPort();
  int nicNumber = getNicNumber(addressAndPort.first);
  cGuid guid = getGuid(nicNumber);
  pResp->ServerGuid[0] = guid.getData64_0();
  pResp->ServerGuid[1] = guid.getData64_1();

  pResp->Capabilities = getCapabilities();

  pResp->MaxTransactSize = gkSMB2BufferSize;
  pResp->MaxReadSize = gkSMB2BufferSize;
  pResp->MaxWriteSize = gkSMB2BufferSize;

  SYSTEMTIME st;
  GetSystemTime(&st);
  FILETIME ft;
  SystemTimeToFileTime(&st, &ft);

  pResp->SystemTime[0] = ft.dwLowDateTime;
  pResp->SystemTime[1] = ft.dwHighDateTime;

  pResp->ServerStartTime[0] = BootFileTime.dwLowDateTime;
  pResp->ServerStartTime[1] = BootFileTime.dwHighDateTime;

  const PUCHAR pSecurityBlob = reinterpret_cast<PUCHAR>(pResp+1);
  pResp->SecurityBufferOffset = static_cast<USHORT>(pSecurityBlob
    - reinterpret_cast<PUCHAR>(pResponse->header()));
  const PUCHAR pSecurityBlobEnd = writeSPNEGOInit(pSecurityBlob);
  pResp->SecurityBufferLength = static_cast<USHORT>(pSecurityBlobEnd - pSecurityBlob);
  pResponse->addData(pResp->SecurityBufferLength);
  pResp->Reserved2 = 0x204d4c20;  //from netmon - " ML " ??? why ???

  return singleResponse(pResponse);
}

tResponseList cSMB2Request::processNegotiate2(cPtr<cSMB2Response> pResponse) const
{
  const PREQ_SMB2_NEGOTIATE pReq = getParams<PREQ_SMB2_NEGOTIATE>();
  QTRACE((L"cSMB2Request::processNegotiate2 DialectCount:%d", pReq->DialectCount));
  USHORT* dialects = (USHORT*)(((char*)pReq) + sizeof(REQ_SMB2_NEGOTIATE));

  bool found0202 = false;
  bool found0210 = false;
  bool found0300 = false;
  bool found0302 = false;
  for(int i = 0; i < pReq->DialectCount; ++i)
  {
    if(!found0202)
      found0202 = (0x0202 == dialects[i]);
    if(!found0210)
      found0210 = (0x0210 == dialects[i]);
    if(!found0300)
      found0300 = (0x0300 == dialects[i]);
    if(!found0302)
      found0302 = (0x0302 == dialects[i]);
    QTRACE((L"Dialect: 0x%04x", dialects[i]));
  }

  QTRACE((L"%d %d %d %d", found0202, found0210, found0300, found0302));

  const PRESP_SMB2_NEGOTIATE pResp = pResponse->getResponseType<RESP_SMB2_NEGOTIATE>();
  pResp->StructureSize = 65;  //[#2] p.29
  pResp->SecurityMode = getSecurity();

  pResp->DialectRevision = getDialectRevision();
  QTRACE((L"Using Dialect: 0x%04x", pResp->DialectRevision));
  pResp->Reserved = 0;

  std::pair<ULONG, USHORT> addressAndPort = m_pContextPacket->getIpAddressAndPort();
  int nicNumber = getNicNumber(addressAndPort.first);
  cGuid guid = getGuid(nicNumber);
  pResp->ServerGuid[0] = guid.getData64_0();
  pResp->ServerGuid[1] = guid.getData64_1();

  pResp->Capabilities = getCapabilities();

  pResp->MaxTransactSize = gkSMB2BufferSize;
  pResp->MaxReadSize = gkSMB2BufferSize;
  pResp->MaxWriteSize = gkSMB2BufferSize;

  SYSTEMTIME st;
  GetSystemTime(&st);
  FILETIME ft;
  SystemTimeToFileTime(&st, &ft);

  pResp->SystemTime[0] = ft.dwLowDateTime;
  pResp->SystemTime[1] = ft.dwHighDateTime;

  pResp->ServerStartTime[0] = BootFileTime.dwLowDateTime;
  pResp->ServerStartTime[1] = BootFileTime.dwHighDateTime;

  const PUCHAR pSecurityBlob = reinterpret_cast<PUCHAR>(pResp+1);
  pResp->SecurityBufferOffset = static_cast<USHORT>(pSecurityBlob
    - reinterpret_cast<PUCHAR>(pResponse->header()));
  const PUCHAR pSecurityBlobEnd = writeSPNEGOInit(pSecurityBlob);
  pResp->SecurityBufferLength = static_cast<USHORT>(pSecurityBlobEnd - pSecurityBlob);
  pResponse->addData(pResp->SecurityBufferLength);
  pResp->Reserved2 = 0x204d4c20;  //from netmon - " ML " ??? why ???

  return singleResponse(pResponse);
}

void dump(cConstPtr<cMemoryView> data, int lineCount)
{
  const int kLineLength = 16;
  unsigned char* bytes = (unsigned char*)data->getConstBytes();
  int sizeLeft = data->getSize();
  for(int count = 0; count < lineCount && sizeLeft > 0; ++count)
  {
    StringStream str;
    str << std::hex << std::setw(4) << std::setfill(L'0') << count << L":";
    for(int i = 0; i < kLineLength; ++i)
    {
      if(i >= sizeLeft)
        break;
      str << std::hex << std::setw(2) << std::setfill(L'0') << bytes[i] << L" ";
    }
    QTRACE((L"%s", str.str().c_str()));
    bytes += kLineLength;
    sizeLeft -= kLineLength;
  }
}

void dump(std::vector<unsigned char> data, int lineCount)
{
  const int kLineLength = 16;
  unsigned char* bytes = (unsigned char*)&(data.front());
  int sizeLeft = data.size();
  for(int count = 0; count < lineCount && sizeLeft > 0; ++count)
  {
    StringStream str;
    str << std::hex << std::setw(4) << std::setfill(L'0') << count << L":";
    std::stringstream str2;
    for(int i = 0; i < kLineLength; ++i)
    {
      if(i >= sizeLeft)
        break;
      char buff[4];
      _itoa(bytes[i], buff, 16);
      str << std::hex << std::setw(2) << std::setfill(L'0') << widen(buff);
      unsigned char val = bytes[i];
      if(val > 0x20 && val < 127)
        str2 << val;
      else
        str2 << ".";
    }
    QTRACE((L"%s %S", str.str().c_str(), str2.str().c_str()));
    bytes += kLineLength;
    sizeLeft -= kLineLength;
  }
}

void PrintHexDump(DWORD length, PBYTE buffer)
{
  if(length)
  {
    std::vector<unsigned char> data(&buffer[0], &buffer[length]);
    dump(data, 0x20);
  }
}

DWORD getSize(PUCHAR pGSSdata)
{
  PUCHAR pDataLen = pGSSdata;
  int i = 1;
  while(i <= 4)
  {
    char val = *(pDataLen - i);
    //QTRACE((L"%d", val));
    if(4 == val)
    {
      if(i == 2)
        return *(pDataLen - 1);
      else if(i == 4)
      {
        SHORT val;
        val = *(pDataLen - 1) + (*(pDataLen - 2))*256;
        return val;
      }
    }
    ++i;
  }
  return 0;
}

tResponseList cSMB2Request::processSessionSetup(cPtr<cSMB2Response> pResponse) const
{
  if(!useClassicSMBHandler())
  {
    const PREQ_SMB2_SESSION_SETUP pReq = getParams<PREQ_SMB2_SESSION_SETUP>();
    const PUCHAR pSecBuf = reinterpret_cast<PUCHAR>(header()) + pReq->SecurityBufferOffset;
    DWORD respLength(0);

    QTRACE((L"cSMB2Request::processSessionSetup PreviousSessionId:%I64d", pReq->PreviousSessionId));
    try
    {
      const PUCHAR pGSSdata = getGSSWrappedData(pSecBuf);
      bool haveGSSAPI = (pSecBuf != pGSSdata);
      //QTRACE((L"if(%d != %d) we have SPENEGO!", pGSSdata, pSecBuf));
      DWORD size = 0;
      if (haveGSSAPI)
        size = getSize(pGSSdata);
      else
        size = BufferSize - (pGSSdata - Buffer);
      //QTRACE((L"GSSData size %d", size));
      //PrintHexDump(size, pGSSdata);

      if (0 == memcmp(pGSSdata, sNTLMSSP, sizeof(sNTLMSSP)))
      {
        const PNTLM_MSG pNTLMMsg = reinterpret_cast<PNTLM_MSG>(pGSSdata);

        //QTRACE((L"processSessionSetup NTLMSSP %d", BufferSize));
        //PrintHexDump(BufferSize, Buffer);

        const PRESP_SMB2_SESSION_SETUP pResp = pResponse->getResponseType<RESP_SMB2_SESSION_SETUP>();
        pResp->StructureSize = 9;  //[#2] p.32
        pResp->SessionFlags = 0;//SMB2_SESSION_FLAG_IS_NULL;

        if (NT_LM_NEGOTIATE == pNTLMMsg->MessageType)
        {
          QTRACE((L"cSMB2Request::processSessionSetup NT_LM_NEGOTIATE"));
          const PNTLM_MSG_NEGOTIATE pNTLMMsgNeg = reinterpret_cast<PNTLM_MSG_NEGOTIATE>(pNTLMMsg);
          QTRACE((L"Flags: 0x%08x", pNTLMMsgNeg->Flags));
          //if(NTLMSSP_NEGOTIATE_OEM_DOMAIN_SUPPLIED & pNTLMMsgNeg->Flags)
          //Confirmed - these are NOT unicode.
          String sDomain;
          if (NTLMSSP_NEGOTIATE_OEM_DOMAIN_SUPPLIED & pNTLMMsgNeg->Flags)
            sDomain = makeSafeNameFromNarrow(pGSSdata + pNTLMMsgNeg->Domain.Offset
              , pNTLMMsgNeg->Domain.NameLen);
          String sWorkstation;
          if (NTLMSSP_NEGOTIATE_OEM_WORKSTATION_SUPPLIED & pNTLMMsgNeg->Flags)
            sWorkstation = makeSafeNameFromNarrow(pGSSdata + pNTLMMsgNeg->Workstation.Offset
              , pNTLMMsgNeg->Workstation.NameLen);
          QTRACE((L"sDomain %s, sWorkstation %s", sDomain.c_str(), sWorkstation.c_str()));

  #ifdef kUSE_SMB3
          cPtr<cSMB2Session> session = new cSMB2Session(true);
  #else
          cPtr<cSMB2Session> session = new cSMB2Session(false);
  #endif

          ULONGLONG previousSessionId = 0;
          if (SMB2_SESSION_FLAG_BINDING & pReq->Flags)
          {
            cPtr<cSMB2Session> sessionBinding;
            QTRACE((L"Found session reconnect %I64d 0x%08x %d", header()->SessionId, pReq->Capabilities, pReq->Channel));
            {
              cLockGuard lg(&gSessionLock);
              sessionBinding = gSessionMap[pResponse->SessionID()];
            }
            if (sessionBinding.isValid())
            {
              session->addChannelKey(sessionBinding);
              QTRACE((L"Found previous session for %s", sessionBinding->loginName().c_str()));
              check(sessionBinding);
              previousSessionId = pResponse->SessionID();
            }
          }

          DWORD challengeLength = 0;
          bool fNewConversation = (NT_LM_NEGOTIATE == pNTLMMsg->MessageType);
          PBYTE pOutBuf = session->getNTLMServerResponce(fNewConversation, pGSSdata, size, challengeLength);

          ULONGLONG newSessionID;
          {
            cLockGuard lg(&gSessionLock);
            sessID++;
            if (sessID == ULLONG_MAX)  //don't let SMB1 and SMB2 session Ids overlap for notification IDS
              sessID = USHRT_MAX + 1;
            newSessionID = sessID;
          }
          pResponse->header()->SessionId = newSessionID;
          if (previousSessionId)
            Sleep(200 * (newSessionID - previousSessionId));

          pResp->SecurityBufferOffset = static_cast<USHORT>(reinterpret_cast<PUCHAR>(pResp + 1) - reinterpret_cast<PUCHAR>(pResponse->header()));

          PUCHAR pEndOfData = 0;
          //write GSS-API / SPNEGO
          if (haveGSSAPI)
          {
            const PNTLM_MSG_CHALLENGE pNTLMChg = reinterpret_cast<PNTLM_MSG_CHALLENGE>(
              writeSPNEGOChallenge(reinterpret_cast<PUCHAR>(pResp + 1), challengeLength));
            memcpy(pNTLMChg, pOutBuf, challengeLength);
            pEndOfData = ((PUCHAR)(pNTLMChg)) + challengeLength;

            session->setChallenge(pNTLMChg->ServerChallenge);
            session->setDomainAndWorkstation(sDomain, sWorkstation);
            session->setTargetInfo(pNTLMChg->TargetInfo.NameLen, ((PBYTE)pNTLMChg) + pNTLMChg->TargetInfo.Offset);

          }
          else
          {
            memcpy(reinterpret_cast<PUCHAR>(pResp + 1), pOutBuf, challengeLength);
            pEndOfData = reinterpret_cast<PUCHAR>(pResp + 1) + challengeLength;
          }

          // set size
          respLength = static_cast<USHORT>(pEndOfData - reinterpret_cast<PUCHAR>(pResp + 1));

          pResp->SecurityBufferLength = respLength;
          pResponse->addData(respLength);

          pResponse->header()->Status = static_cast<ULONG>(STATUS_MORE_PROCESSING_REQUIRED);
          QTRACE((L"cSMB2Request::processSessionSetup() - Challenge [%d] - len %d", pResponse->SessionID(), respLength));

          {
            cLockGuard lg(&gSessionLock);
            gSessionMap[pResponse->SessionID()] = session;
          }
          if (requestRequireSign(header()))
          {
            pResponse->sign(session);
          }
        }
        else if (NT_LM_AUTHENTICATE == pNTLMMsg->MessageType)
        {
          QTRACE((L"cSMB2Request::processSessionSetup NT_LM_AUTHENTICATE"));
          QTRACE((L"Found session %I64d 0x%08x 0x%08x %d", header()->SessionId, pReq->Flags, pReq->Capabilities, pReq->Channel));

          cPtr<cSMB2Session> session;
          {
            cLockGuard lg(&gSessionLock);
            session = gSessionMap[pResponse->SessionID()];
          }
          if (session.isValid())
          {
            const PNTLM_MSG_AUTHENTICATE pNTLMMsgAuth = reinterpret_cast<PNTLM_MSG_AUTHENTICATE>(pNTLMMsg);
            //Confirmed - these are unicode.
            const String sLoginName = makeSafeName(pGSSdata + pNTLMMsgAuth->UserName.Offset
              , pNTLMMsgAuth->UserName.NameLen);
            const String sWorkstationName = makeSafeName(pGSSdata + pNTLMMsgAuth->Workstation.Offset
              , pNTLMMsgAuth->Workstation.NameLen);
            const String sDomainName = makeSafeName(pGSSdata + pNTLMMsgAuth->Domain.Offset
              , pNTLMMsgAuth->Domain.NameLen);

            session->setDomainAndWorkstation(sDomainName, sWorkstationName);
            session->setLoginName(sLoginName);

            session->setLmChallengeResponse(pNTLMMsgAuth->LmChallengeResponse.NameLen, pGSSdata + pNTLMMsgAuth->LmChallengeResponse.Offset);
            session->setNtChallengeResponse(pNTLMMsgAuth->NtChallengeResponse.NameLen, pGSSdata + pNTLMMsgAuth->NtChallengeResponse.Offset);
            session->setEncryptedRandomSessionKey(pNTLMMsgAuth->EncryptedRandomSessionKey.NameLen, pGSSdata + pNTLMMsgAuth->EncryptedRandomSessionKey.Offset);


            DWORD challengeLength = 0;
            bool fNewConversation = (NT_LM_NEGOTIATE == pNTLMMsg->MessageType);

            PBYTE pOutBuf = 0;
            bool authSuccessed = false;
            try
            {
              pOutBuf = session->getNTLMServerResponce(fNewConversation, pGSSdata, size, challengeLength);
              authSuccessed = true;
            }
            catch (cAccessDeniedErrorValue& err)
            {
              QTRACE((L"Authentication Failed: value:%d %s. Will try to enable guest access.", err.ErrorValue, err.getMsg().c_str()));
              //pResponse->setNTError(static_cast<ULONG>(err.ErrorValue));
              //pResponse->setNTError(STATUS_LOGON_FAILURE);
              //return errorResponse(pResponse);
            }

            if (haveGSSAPI || !authSuccessed)
            {
              const PUCHAR pNTLMAccept = reinterpret_cast<PUCHAR>(pResp + 1);
              pResp->SecurityBufferOffset
                = static_cast<USHORT>(pNTLMAccept - reinterpret_cast<PUCHAR>(pResponse->header()));
              const PUCHAR pMsgTail = writeSPNEGOAccept(pNTLMAccept);
              respLength = static_cast<USHORT>(pMsgTail - pNTLMAccept);
            }
            else
            {
              memcpy(reinterpret_cast<PUCHAR>(pResp + 1), pOutBuf, challengeLength);
              PUCHAR pEndOfData = reinterpret_cast<PUCHAR>(pResp + 1) + challengeLength;
              respLength = static_cast<USHORT>(pEndOfData - reinterpret_cast<PUCHAR>(pResp + 1));
            }
            pResp->SecurityBufferLength = respLength;
            pResponse->addData(respLength);

            if (authSuccessed)
            {
              if (requestRequireSign(pResponse->header()))
              {
                session->clearChannelKey();
                pResponse->sign(session);
              }
              else
              {
                pResponse->header()->Flags |= SMB2_FLAGS_SIGNED;
                pResponse->sign(session);
              }
            }
          }
        }

        //QTRACE((L"processSessionSetup reply ... %d", pResponse->size()));
        //PrintHexDump(pResponse->size(), pResponse->getResponseBuffer());
      }
      else if (0 == memcmp(pGSSdata, sNEGOEXTS, sizeof(sNEGOEXTS)))
      {
        QTRACE((L"processSessionSetup NEGOEXTS %d", BufferSize));
        PrintHexDump(BufferSize, Buffer);

        //JCJCJC we should handle this ...

        pResponse->setNTError(static_cast<ULONG>(STATUS_INVALID_PARAMETER));
        return errorResponse(pResponse);
      }
      else
      {
        QSOS((L"cSMB2Request::processSessionSetup() - received request for unknown security protocol"));
        PrintHexDump(BufferSize, Buffer);
        pResponse->setNTError(static_cast<ULONG>(STATUS_INVALID_PARAMETER));
        return errorResponse(pResponse);
      }
    }
    catch (cAccessDenied& err)
    {
      QSOS((L"cSMB2Request::processSessionSetup() - cAccessDenied"));
      pResponse->setNTError(static_cast<ULONG>(STATUS_ACCESS_DENIED));
      return errorResponse(pResponse);
    }
    catch (exception& e)
    {
      QSOS((L"cSMB2Request::processSessionSetup() - caught exception: %S", e.what()));
      pResponse->setNTError(static_cast<ULONG>(STATUS_ACCESS_DENIED));
      return errorResponse(pResponse);
    }

    return processChain(pResponse, sizeof(RESP_SMB2_SESSION_SETUP) + respLength);
  }
  else
  {
    const PREQ_SMB2_SESSION_SETUP pReq = getParams<PREQ_SMB2_SESSION_SETUP>();
    const PUCHAR pSecBuf = reinterpret_cast<PUCHAR>(header()) + pReq->SecurityBufferOffset;
    USHORT respLength(0);

    try
    {
      const PUCHAR pGSSdata = getGSSWrappedData(pSecBuf);

      if (0 == memcmp(pGSSdata, sNTLMSSP, sizeof(sNTLMSSP)))
      {
        //QTRACE((L"processSessionSetup %d", BufferSize));
        //PrintHexDump(BufferSize, Buffer);

        const PNTLM_MSG pNTLMMsg = reinterpret_cast<PNTLM_MSG>(pGSSdata);

        const PRESP_SMB2_SESSION_SETUP pResp = pResponse->getResponseType<RESP_SMB2_SESSION_SETUP>();
        pResp->StructureSize = 9;  //[#2] p.32
        pResp->SessionFlags = 0;//SMB2_SESSION_FLAG_IS_NULL;

        if (NT_LM_NEGOTIATE == pNTLMMsg->MessageType)
        {
          const PNTLM_MSG_NEGOTIATE pNTLMMsgNeg = reinterpret_cast<PNTLM_MSG_NEGOTIATE>(pNTLMMsg);
          ULONGLONG newSessionID;
          {
            cLockGuard lg(&gSessionLock);
            sessID++;
            if (sessID == ULLONG_MAX)  //don't let SMB1 and SMB2 session Ids overlap for notification IDS
              sessID = USHRT_MAX + 1;
            newSessionID = sessID;
          }
          pResponse->header()->SessionId = newSessionID;

          static const String sTargetName(L"SAM");
          const USHORT NTLM_reply_size(sizeof(NTLM_MSG_CHALLENGE) + sTargetName.size() * sizeof(WCHAR) + getAVPairsLen(sTargetName));

          pResp->SecurityBufferOffset = static_cast<USHORT>(reinterpret_cast<PUCHAR>(pResp + 1) - reinterpret_cast<PUCHAR>(pResponse->header()));

          //write GSS-API / SPNEGO
          const PNTLM_MSG_CHALLENGE pNTLMChg = reinterpret_cast<PNTLM_MSG_CHALLENGE>(
            writeSPNEGOChallenge(reinterpret_cast<PUCHAR>(pResp + 1), NTLM_reply_size));

          //write NTLM
          ZeroMemory(pNTLMChg, NTLM_reply_size);
          memcpy(reinterpret_cast<PUCHAR>(&pNTLMChg->Signature), sNTLMSSP, sizeof(pNTLMChg->Signature));
          pNTLMChg->MessageType = NT_LM_CHALLENGE;
          pNTLMChg->Flags = pNTLMMsgNeg->Flags
            | NTLMSSP_NEGOTIATE_ACCEPT
            | NTLMSSP_TARGET_INFO;
          pNTLMChg->Flags &= ~(NTLMSSP_NEGOTIATE_OEM | NTLMSSP_LAN_MANAGER_KEY);
          time_t tt;
          srand(static_cast<UINT>(time(&tt)));
          pNTLMChg->ServerChallenge = static_cast<ULONGLONG>(rand())
            + (static_cast<ULONGLONG>(rand()) << 0x10)
            + (static_cast<ULONGLONG>(rand()) << 0x20)
            + (static_cast<ULONGLONG>(rand()) << 0x30);

          pNTLMChg->Version.Major = 6;  //[#3] p.35
                                        //pNTLMChg->Version.Minor = 0;
          pNTLMChg->Version.Build = 6001;
          pNTLMChg->Version.Revision = 0xf;

          //write Target Name
          const PUCHAR pTargetName = reinterpret_cast<PUCHAR>(pNTLMChg) + sizeof(NTLM_MSG_CHALLENGE);
          pNTLMChg->TargetName.NameLen = (sTargetName.size()) * sizeof(WCHAR);
          pNTLMChg->TargetName.NameMaxLen = pNTLMChg->TargetName.NameLen;
          pNTLMChg->TargetName.Offset = checked_static_cast<ULONG>(
            reinterpret_cast<const PUCHAR>(pTargetName) - reinterpret_cast<const PUCHAR>(pNTLMChg));
          memcpy(pTargetName, sTargetName.c_str(), (sTargetName.size()) * sizeof(WCHAR));

          //write Target Info
          const PUCHAR pNTLMAvPair = pTargetName + (sTargetName.size()) * sizeof(WCHAR);
          pNTLMChg->TargetInfo.Offset = checked_static_cast<ULONG>(
            reinterpret_cast<const PUCHAR>(pNTLMAvPair) - reinterpret_cast<const PUCHAR>(pNTLMChg));
          pNTLMChg->TargetInfo.NameLen = getAVPairsLen(sTargetName);
          pNTLMChg->TargetInfo.NameMaxLen = pNTLMChg->TargetInfo.NameLen;
          const PUCHAR pEndOfData = writeAVPairs(sTargetName, pNTLMAvPair);

          // set size
          respLength = static_cast<USHORT>(pEndOfData - reinterpret_cast<PUCHAR>(pResp + 1));
          pResp->SecurityBufferLength = respLength;
          pResponse->addData(respLength);

          pResponse->header()->Status = static_cast<ULONG>(STATUS_MORE_PROCESSING_REQUIRED);
          QTRACE((L"cSMB2Request::processSessionSetup() - Challenge [%d]", pResponse->SessionID()));

          {
            cLockGuard lg(&gSessionLock);
            gSessionMap[pResponse->SessionID()] = new cSMB2Session();
            gSessionMap[pResponse->SessionID()]->setChallenge(pNTLMChg->ServerChallenge);
          }
        }
        else if (NT_LM_AUTHENTICATE == pNTLMMsg->MessageType)
        {
          const PNTLM_MSG_AUTHENTICATE pNTLMMsgAuth = reinterpret_cast<PNTLM_MSG_AUTHENTICATE>(pNTLMMsg);

          const PUCHAR pNTLMAccept = reinterpret_cast<PUCHAR>(pResp + 1);
          pResp->SecurityBufferOffset
            = static_cast<USHORT>(pNTLMAccept - reinterpret_cast<PUCHAR>(pResponse->header()));
          const PUCHAR pMsgTail = writeSPNEGOAccept(pNTLMAccept);
          respLength = static_cast<USHORT>(pMsgTail - pNTLMAccept);
          pResp->SecurityBufferLength = respLength;
          pResponse->addData(respLength);

          const String sLoginName = makeSafeName(pGSSdata + pNTLMMsgAuth->UserName.Offset
            , pNTLMMsgAuth->UserName.NameLen);
          const String sWorkstationName = makeSafeName(pGSSdata + pNTLMMsgAuth->Workstation.Offset
            , pNTLMMsgAuth->Workstation.NameLen);
          const String sDomainName = makeSafeName(pGSSdata + pNTLMMsgAuth->Domain.Offset
            , pNTLMMsgAuth->Domain.NameLen);

          const String sDomainUser = sDomainName + L"\\" + sLoginName;

          QTRACE((L"cSMB2Request::processSessionSetup() - Accept [U:%s, W:%s, D:%s] => %s", sLoginName.c_str(), sWorkstationName.c_str(), sDomainName.c_str(), sDomainUser.c_str()));

          //iWebClient::singleton().registerDomainUser(sDomainUser, true);

          const cPtr<cSMB2Session> session = getSession(pResponse);
          if (session.isValid())
          {
            session->setLoginName(sDomainUser);
          }
        }
      }
      else
      {
        QSOS((L"cSMB2Request::processSessionSetup() - received request for unknown security protocol"));
        PrintHexDump(BufferSize, Buffer);
        pResponse->setNTError(static_cast<ULONG>(STATUS_INVALID_PARAMETER));
      }
    }
    catch (exception& e)
    {
      QSOS((L"cSMB2Request::processSessionSetup() - caught exception: %S", e.what()));
    }

    return processChain(pResponse, sizeof(RESP_SMB2_SESSION_SETUP) + respLength);
  }
}

tResponseList cSMB2Request::processSessionLogoff(cPtr<cSMB2Response> response) const
{
  const PRESP_SMB2_LOGOFF pResp = response->getResponseType<RESP_SMB2_LOGOFF>();

  const cPtr<cSMB2Session> pSession = getSession(response);
  if (pSession.isNull())
    return errorResponse(response);

  QTRACE((L"cSMB2Request::processSessionLogoff %s", pSession->loginName().c_str()));

  {
    cLockGuard lg(&gSessionLock);
    gSessionMap.erase(SessionID());
  }

  if (m_pShareManager.isValid())
    m_pShareManager->forgetListener(pSession);

  pResp->StructureSize = 4; //[#2] p.33
  pResp->Reserved = 0;

  return processChain(response, sizeof(RESP_SMB2_LOGOFF));
}

tResponseList cSMB2Request::processTreeConnect(cPtr<cSMB2Response> response) const
{
  const cPtr<cSMB2Session> pSession = getSession(response);
  const PREQ_SMB2_TREE_CONNECT pReq = getParams<PREQ_SMB2_TREE_CONNECT>();
  if (pSession.isNull())
    return errorResponse(response);

  const String sConnectionPath = makeSafeName(reinterpret_cast<PUCHAR>(header()) + pReq->PathOffset
    , pReq->PathLength);
  
  const size_t pos = sConnectionPath.find_last_of(L"\\");
  const String sShare(toUpper(sConnectionPath.substr(static_cast<USHORT>(pos+1))));

  const PRESP_SMB2_TREE_CONNECT pResp = response->getResponseType<RESP_SMB2_TREE_CONNECT>();

  const cPtr<iTreeResourceFactory> pTreeResourceFactory(m_pShareManager->Get(sShare, pSession));
  if (pTreeResourceFactory.isNull())
  {
    QTRACE((L"cSMB2Request::processTreeConnect() failed to get %s", sShare.c_str()));
    return errorResponse(response, SMB_ERR_CLASS_SERVER, SMB_ERR_BAD_NET_NAME);
  }

  response->header()->TreeId = pSession->ConnectTree(sShare, response, pTreeResourceFactory);
  if (0 == response->header()->TreeId)
    return errorResponse(response);

  QTRACE((L"cSMB2Request::processTreeConnect() - %s:%d", sShare.c_str(), response->header()->TreeId));

  const bool bIPCShare(String::npos != sShare.find(L"IPC"));

  pResp->StructureSize = 16; //[#2] p.35
  pResp->ShareType = bIPCShare ? SMB2_SHARE_TYPE_PIPE : SMB2_SHARE_TYPE_DISK;
  pResp->Reserved = 0;
  pResp->ShareFlags = SMB2_SHAREFLAG_NO_CACHING | (bIPCShare ? SHI1005_FLAGS_ACCESS_BASED_DIRECTORY_ENUM : 0);
#ifdef kUSE_SMB3
  if(bIPCShare)
  {
    pResp->Capabilities = 0;//JCJCJC SNIA SDC 2013: IPC$ - does not have these flags set
    pResp->Capabilities = SMB2_SHARE_CAP_CONTINUOUS_AVAILABILITY | SMB2_SHARE_CAP_CLUSTER | SMB2_SHARE_CAP_SCALEOUT;//JCJCJC SNIA SDC 2013: Requires multichannel & two nics to be provoked ...
  }
  else
  {
    //pResp->Capabilities = SMB2_SHARE_CAP_CONTINUOUS_AVAILABILITY | SMB2_SHARE_CAP_SCALEOUT | SMB2_SHARE_CAP_CLUSTER;//JCJCJC SNIA SDC 2012 // 0;
    pResp->Capabilities = SMB2_SHARE_CAP_CONTINUOUS_AVAILABILITY | SMB2_SHARE_CAP_CLUSTER | SMB2_SHARE_CAP_SCALEOUT;//JCJCJC SNIA SDC 2013: Requires multichannel & two nics to be provoked ...
  }
#else
  pResp->Capabilities = 0;
#endif
  pResp->MaximalAccess = 0x1f01ff;//MAXIMAL_ACCESS;

  return processChain(response, sizeof(RESP_SMB2_SESSION_SETUP));
}

tResponseList cSMB2Request::processTreeDisconnect(cPtr<cSMB2Response> response) const
{
  const cPtr<cSMB2Session> pSession = getSession(response);
  const PRESP_SMB2_TREE_DISCONNECT pResp = response->getResponseType<RESP_SMB2_TREE_DISCONNECT>();

  if (pSession.isNull())
    return errorResponse(response);

  pSession->DisconnectTree(TreeID());

  pResp->StructureSize = 4; //[#2] p.37
  pResp->Reserved = 0;

  return processChain(response, sizeof(RESP_SMB2_TREE_DISCONNECT));
}

void fixupPath(String& sFilename)
{
  //String::size_type off = sFilename.find(L"@%!^"); 
  //String::size_type off2 = sFilename.find(L"^!%@", off);
  //if(off != String::npos && off2 != String::npos)
  //{
  //  String root(sFilename, 0, off);
  //  String params(sFilename, off+4, off2-(off+4));
  //  String rest(sFilename, off2+4);

  //  sFilename = root + rest;
  //  QTRACE((L"cSMB2Request::processCreate real sFilename \"%s\". Params \"%s\"", sFilename.c_str(), params.c_str()));
  //}
}

tResponseList cSMB2Request::processCreate(cPtr<cSMB2Response> response) const
{
  const cPtr<cSMB2Tree> pTree = getTree(response);
  const PRESP_SMB2_CREATE pResp = response->getResponseType<RESP_SMB2_CREATE>();

  if (pTree.isNull())
    return errorResponse(response);

  const PREQ_SMB2_CREATE pReq = getParams<PREQ_SMB2_CREATE>();
  BY_HANDLE_FILE_INFORMATION bhfi;
  DWORD dwErr = 0;
  String sFilename = L"\\" + makeSafeName(reinterpret_cast<PUCHAR>(header()) + pReq->NameOffset
    , pReq->NameLength);
  fixupPath(sFilename);

  const cPtr<cMemoryView> pNotifyMem = new cMemoryView(new cMemory(sizeof(PRESP_SMB2_OPLOCK_BREAK_NOTIFY), cMemory::eVirtual));
  const cPtr<cSMB2Response> pResponse = new cSMB2Response(this, pNotifyMem->getBytes(), pNotifyMem);

  ULONGLONG durableFid = 0;

  const ULONGLONG Fid = pTree->Create(sFilename, pReq, &bhfi, pResponse, durableFid, dwErr);
  m_pQCIFSPacketProcessor->addCreateFileData(SessionID(), sFilename, Fid);
  if (0 == Fid)
  {
    //QSOS((L"cSMB2Request::processCreate() - CREATE ERROR {%s} 0x%08x", sFilename.c_str(), dwErr));
    return errorResponse(response, SMB_ERR_CLASS_DOS, dwErr);
  }

  saveCompoundFid(Fid);

  //QTRACE((L"cSMB2Request::Create() - [%I64x] {%s} (%p) %s"
  //  , Fid
  //  , sFilename.c_str()
  //  , response.getRaw()
  //  , pTree->name().c_str()));

  const cPtr<iComposite> pComposite = pTree->getComposite(Fid, response);
  if (pComposite.isNull())
  {
    //QSOS((L"cSMB2Request::processCreate() - GET COMPOSITE ERROR {%s}", sFilename.c_str()));
    return errorResponse(response);
  }

  UCHAR oplockLevel = pComposite->hasBatchOpLock()?SMB2_OPLOCK_LEVEL_BATCH:SMB2_OPLOCK_LEVEL_NONE;
  //if(SMB2_OPLOCK_LEVEL_NONE == oplockLevel)
  //{
  //  QTRACE((L"No oplock for %s", sFilename.c_str()));
  //}

  pResp->StructureSize = 89;  //[#2]p.50
  pResp->OplockLevel = oplockLevel;
  pResp->Flags = 0;  //RH said: "9 got from wireshark", JC says: naaa... this 9 must the SMB2_OPLOCK_LEVEL_BATCH above! Only used with SMB3 to mean SMB2_CREATE_FLAG_REPARSEPOINT 0x01
  pResp->CreateAction = FILE_OPENED;
  writeAttributesAndSize(bhfi, pResp);
  pResp->Reserved2 = 0;
  pResp->FileId.Persistent = durableFid;//0xffffffff00000000 | Fid;
  pResp->FileId.Volatile = Fid;
  pResp->CreateContextsLength = 0;
  pResp->CreateContextsOffset = 0;

  return processChain(response, sizeof(RESP_SMB2_CREATE));
}

tResponseList cSMB2Request::processClose(cPtr<cSMB2Response> response) const
{
  const cPtr<cSMB2Tree> pTree = getTree(response);
  const PRESP_SMB2_CLOSE pResp = response->getResponseType<RESP_SMB2_CLOSE>();

  if (pTree.isNull())
    return errorResponse(response);

  const PREQ_SMB2_CLOSE pReq = getParams<PREQ_SMB2_CLOSE>();

  if (pReq->Flags & SMB2_CLOSE_FLAG_POSTQUERY_ATTRIB)
  {
    const cPtr<iComposite> pComposite = pTree->getComposite(Fid(pReq), response);
    if (pComposite.isNull())
      return errorResponse(response);

    BY_HANDLE_FILE_INFORMATION bhfi;
    pComposite->writeInfo(&bhfi, Fid(pReq));
    pResp->Flags = SMB2_CLOSE_FLAG_POSTQUERY_ATTRIB;
    writeAttributesAndSize(bhfi, pResp);
  }

  pTree->closeComposite(Fid(pReq), pReq->FileId.Persistent);
  pResp->StructureSize = 60;  //[#2] p.55

  return processChain(response, sizeof(RESP_SMB2_CLOSE));
}

tResponseList cSMB2Request::processFlush(cPtr<cSMB2Response> response) const
{
  return processUnhandled(response);
}

typedef std::list< cPtr<cSMB2Response> > tAsyncReadDelayedResponces;
tAsyncReadDelayedResponces AsyncReadDelayedResponces;
tAsyncReadDelayedResponces AsyncReadHoldingResponces;

cLock AsyncReadDelayedResponcesLock;
void addAsyncReadDelayedResponce(const cPtr<cSMB2Response>& resp)
{
  cLockGuard g(&AsyncReadDelayedResponcesLock);
  QSOS((L"cSMB2Request::processRead - addAsyncReadDelayedResponce %I64d", resp->async_header()->AsyncId));
  AsyncReadDelayedResponces.push_back(resp);
}

void addAsyncReadHoldingResponce(const cPtr<cSMB2Response>& resp)
{
  cLockGuard g(&AsyncReadDelayedResponcesLock);
  QSOS((L"cSMB2Request::processRead - addAsyncReadHoldingResponce %I64d", resp->async_header()->AsyncId));
  AsyncReadHoldingResponces.push_back(resp);
}

void sendQueuedHoldingResponces()
{
  cLockGuard g(&AsyncReadDelayedResponcesLock);
  for(tAsyncReadDelayedResponces::const_iterator it = AsyncReadHoldingResponces.begin(); it != AsyncReadHoldingResponces.end(); ++it)
  {
    QSOS((L"sendQueuedHoldingResponces %I64d", (*it)->async_header()->AsyncId));

    const cPtr<iRXPacket> pRXPacket = (*it)->getContextPacket();
    const cPtr<iTXPacket> pTXPacket = pRXPacket->getResponsePacket();
    (*it)->WritePacket(pTXPacket);
    pTXPacket->Send();
  }
}

void sendQueuedReadRequests()
{
  cLockGuard g(&AsyncReadDelayedResponcesLock);
  for(tAsyncReadDelayedResponces::const_iterator it = AsyncReadDelayedResponces.begin(); it != AsyncReadDelayedResponces.end(); ++it)
  {
    QSOS((L"sendQueuedReadRequests %I64d", (*it)->async_header()->AsyncId));

    const cPtr<iRXPacket> pRXPacket = (*it)->getContextPacket();
    const cPtr<iTXPacket> pTXPacket = pRXPacket->getResponsePacket();
    (*it)->WritePacket(pTXPacket);
    pTXPacket->Send();
  }
  AsyncReadDelayedResponces.clear();
  AsyncReadHoldingResponces.clear();
}

bool kAsync = false;
unsigned int kMsDelay = 0;
tResponseList cSMB2Request::processRead(cPtr<cSMB2Response> response) const
{
  bool asyncCommand = kAsync;//Read this once - atomically ...
  unsigned int msDelay = kMsDelay;//Read this once - atomically ...

  if(msDelay)
    Sleep(msDelay);

  std::pair<ULONG, USHORT> addressAndPort = m_pContextPacket->getIpAddressAndPort();
  int nicNumber = getNicNumber(addressAndPort.first);
  int colour = getColour(addressAndPort.second);
  //QTRACE((L"IP:%08x P:%04x %d:%d", addressAndPort.first, addressAndPort.second, nicNumber, colour));
  cActivityScope as1(ActivityQCIFSProcessors2.find(nicNumber)->second,colour);

  PREQ_SMB2_READ pReq;
  const cPtr<iComposite> pComposite = getComposite<REQ_SMB2_READ>(pReq, response);
  if (pComposite.isNull())
    return errorResponse(response);

  cPtr<cSMB2Response> initialResponse = response;
  cPtr<cSMB2Response> responseToProcess = response;
  if(asyncCommand)
  {
    const cPtr<cMemoryView> pMem = new cMemoryView(new cMemory((size_t)gkBufferSize, cMemory::eVirtual));
    const cPtr<cSMB2Response> pAsyncDelayedResponse = new cSMB2Response(this, pMem->getBytes(), pMem);
    const PSMB2_ASYNC_Header pAsyncDelayedHeader = pAsyncDelayedResponse->async_header();
    pAsyncDelayedHeader->AsyncId = header()->MessageId;
    pAsyncDelayedHeader->Flags |= SMB2_FLAGS_ASYNC_COMMAND;
    responseToProcess = pAsyncDelayedResponse;
  }

  DWORD nBytes(pReq->Length);
  LARGE_INTEGER lg;
  lg.QuadPart = pReq->Offset;

  const PRESP_SMB2_READ pResp = responseToProcess->getResponseType<RESP_SMB2_READ>();
  //cActivityScope as2(ActivityQCIFSProcessors.find(1)->second,kCompositeRead);
  DWORD nRet = pComposite->Read(responseToProcess->getTransmitList(), nBytes, lg, SessionID(), Fid(pReq));

  if (ERROR_SUCCESS != nRet)
    return errorResponse(response, SMB_ERR_CLASS_SERVER, nRet);
  if (0 == nBytes)
    return errorResponse(response, static_cast<DWORD>(STATUS_END_OF_FILE));

  pResp->StructureSize = 17;  //[#2] p.60
  pResp->DataOffset = static_cast<UCHAR>(reinterpret_cast<PUCHAR>(pResp+1) - reinterpret_cast<PUCHAR>(responseToProcess->header()));
  pResp->Reserved = 0;
  pResp->DataLength = nBytes;
  pResp->DataRemaining = 0;
  pResp->Reserved2 = 0;
  responseToProcess->addData(nBytes);

  if(asyncCommand)
  {
    responseToProcess->async_header()->CreditRequest = 0;
    addAsyncReadDelayedResponce(responseToProcess);

    const PSMB2_ASYNC_Header pAsyncReplyHeader = initialResponse->async_header();
    pAsyncReplyHeader->Flags |= SMB2_FLAGS_ASYNC_COMMAND;
    pAsyncReplyHeader->Status = STATUS_PENDING;
    pAsyncReplyHeader->AsyncId = header()->MessageId;

    const cPtr<cMemoryView> pMem = new cMemoryView(new cMemory((size_t)gkBufferSize, cMemory::eVirtual));
    const cPtr<cSMB2Response> pAsyncHoldingResponse = new cSMB2Response(this, pMem->getBytes(), pMem);
    const PSMB2_ASYNC_Header pAsyncHoldingHeader = pAsyncHoldingResponse->async_header();
    pAsyncHoldingHeader->AsyncId = header()->MessageId;
    pAsyncHoldingHeader->Flags |= SMB2_FLAGS_ASYNC_COMMAND;
    pAsyncHoldingHeader->Status = STATUS_PENDING;
    pAsyncHoldingHeader->CreditRequest = 0;
    {
      const PRESP_SMB2_ERROR pResp = pAsyncHoldingResponse->getResponseType<RESP_SMB2_ERROR>();
      pResp->StructureSize = 9; //[#2] p.24
      pResp->Reserved = 0;
      pResp->ByteCount = 0;
      pResp->ErrorData = 0;
    }
    addAsyncReadHoldingResponce(pAsyncHoldingResponse);

    const PRESP_SMB2_ERROR pResp = initialResponse->getResponseType<RESP_SMB2_ERROR>();
    pResp->StructureSize = 9; //[#2] p.24
    pResp->Reserved = 0;
    pResp->ByteCount = 0;
    pResp->ErrorData = 0;

    return processChain(initialResponse, 0);
  }
  else
  {
    return processChain(initialResponse, sizeof(RESP_SMB2_READ) + nBytes);
  }
}

tResponseList cSMB2Request::processWrite(cPtr<cSMB2Response> response) const
{
  PREQ_SMB2_WRITE pReq;
  const cPtr<iComposite> pComposite = getComposite<REQ_SMB2_WRITE>(pReq, response);
  if (pComposite.isNull())
    return errorResponse(response);

  DWORD nBytes(pReq->Length);
  LARGE_INTEGER lg;
  lg.QuadPart = pReq->Offset;

  const PRESP_SMB2_WRITE pResp = response->getResponseType<RESP_SMB2_WRITE>();
  static const unsigned int kSmbBufferFromNetBIOS = 4;
  DWORD nRet = pComposite->Write(m_pContextPacket->getRXBuffer()->trimmedConst((size_t)pReq->DataOffset + kSmbBufferFromNetBIOS, 0)
    , nBytes
    , lg
    , SessionID()
    , Fid(pReq));
  
  if (ERROR_SUCCESS != nRet || 0 == nBytes)
    return errorResponse(response, SMB_ERR_CLASS_SERVER, nRet);

  pResp->StructureSize = 17;  //[#2] p.60
  pResp->Reserved = 0;
  pResp->Count = nBytes;
  pResp->Remaining = 0;
  pResp->WriteChannelInfoOffset = 0;
  pResp->WriteChannelInfoLength = 0;

  return processChain(response, sizeof(RESP_SMB2_WRITE));
}

tResponseList cSMB2Request::processLock(cPtr<cSMB2Response> response) const
{
  return processUnhandled(response);
}

tResponseList cSMB2Request::processIoctl(cPtr<cSMB2Response> response) const
{
  ULONG treeID = TreeID();
  //QTRACE((L"cSMB2Request::processIoctl %d", treeID));

  if (requestRequireSign(header()) && !useClassicSMBHandler())
  {
    cPtr<cSMB2Session> session(getSession(response));
    check(session);
  }

  const PREQ_SMB2_IOCTL pReq = getParams<PREQ_SMB2_IOCTL>();

  const PRESP_SMB2_IOCTL pResp = response->getResponseType<RESP_SMB2_IOCTL>();
  pResp->StructureSize = 4;
  pResp->Reserved = 0;

  tResponseList ret;
  if (0x00140198 == pReq->CtlCode)//Maybe OBO - added by Win 8 beta???
    ret = errorResponse(response, static_cast<ULONG>(STATUS_FILE_CLOSED));
  else if (FSCTL_VALIDATE_NEGOTIATE_INFO == pReq->CtlCode)//Added by Windows 8 era OSes
  {
    QTRACE((L"FSCTL_VALIDATE_NEGOTIATE_INFO"));
    if (!useClassicSMBHandler() && requestRequireSign(header()))
    {
      BYTE* pInfoBytes = (BYTE*)(pResp + 1);
      int offset = pInfoBytes - (BYTE*)response->header();

      pResp->StructureSize = 0x31;
      pResp->CtlCode = pReq->CtlCode;
      pResp->FileId = pReq->FileId;
      pResp->InputOffset = offset;
      pResp->InputCount = 0;
      pResp->OutputOffset = offset;
      pResp->OutputCount = sizeof(RESP_VALIDATE_NEGOTIATE_INFO);
      pResp->Flags = 0;
      pResp->Reserved2 = 0;

      std::pair<ULONG, USHORT> addressAndPort = m_pContextPacket->getIpAddressAndPort();
      int nicNumber = getNicNumber(addressAndPort.first);
      cGuid guid = getGuid(nicNumber);

      RESP_VALIDATE_NEGOTIATE_INFO* pInfo = (RESP_VALIDATE_NEGOTIATE_INFO*)pInfoBytes;
      pInfo->Capabilities = getCapabilities();
      pInfo->ServerGuid[0] = guid.getData64_0();
      pInfo->ServerGuid[1] = guid.getData64_1();
      pInfo->SecurityMode = getSecurity();
      pInfo->DialectRevision = getDialectRevision();
      response->addData(sizeof(RESP_VALIDATE_NEGOTIATE_INFO));

      ret = processChain(response, sizeof(RESP_SMB2_IOCTL) + sizeof(RESP_VALIDATE_NEGOTIATE_INFO));
    }
    else
    {
      ret = errorResponse(response, static_cast<ULONG>(STATUS_FILE_CLOSED), 0xff);//0xff from wireshark of Windows 7 client

    }
  }
  else if(FSCTL_QUERY_NETWORK_INTERFACE_INFO == pReq->CtlCode)//Added by Windows 8 era OSes
  {
#ifdef kUSE_SMB3 

#ifdef kUSE_SMB_NETWORK_INTERFACE_INFO
    int numInfos = sizeof(NetworkInfos) / sizeof(tNETWORK_INTERFACE_INFO);

    QTRACE((L"FSCTL_QUERY_NETWORK_INTERFACE_INFO %d", numInfos));

    BYTE* pInfoBytes = (BYTE*)(pResp+1);
    int offset = pInfoBytes - (BYTE*)response->header();

    pResp->StructureSize = 0x31;
    pResp->CtlCode = pReq->CtlCode;
    pResp->FileId = pReq->FileId;
    pResp->InputOffset = offset;
    pResp->InputCount = 0;
    pResp->OutputOffset = offset;
    pResp->OutputCount = sizeof(RESP_NETWORK_INTERFACE_INFO) * numInfos;
    pResp->Flags = 0;
    pResp->Reserved2 = 0;

    for(int i = 0; i < numInfos; ++i)
    {
      RESP_NETWORK_INTERFACE_INFO* pInfo = (RESP_NETWORK_INTERFACE_INFO*)(pInfoBytes + (sizeof(RESP_NETWORK_INTERFACE_INFO)*i));
      if(i < (numInfos-1))
        pInfo->Next = sizeof(RESP_NETWORK_INTERFACE_INFO);
      else
        pInfo->Next = 0;
      pInfo->Ifindex = i;
      //RSS_CAPABLE seems to make the client make 4 alternate session channels - once the connection criteria are reached. See [MS-SMB2].pdf: Behavior note <139> in Section 3.2.5.5.
      pInfo->Capability = NetworkInfos[i].Capability;
      pInfo->Reservered = 0;
      pInfo->LinkSpeed = NetworkInfos[i].LinkSpeed;

      RESP_NETWORK_INTERFACE_INFO_SOCKADDR_IN* pInfoSock = (RESP_NETWORK_INTERFACE_INFO_SOCKADDR_IN*)pInfo->SockAddress;
      pInfoSock->Family = InterNetwork;
      pInfoSock->Port = 0;//htons(445);//SNAI SDC 2012 - Makes no difference
      pInfoSock->IPAddress = htonl(NetworkInfos[i].IpAddress);
      pInfoSock->Resesrved = 0;
    }
    response->addData(sizeof(RESP_NETWORK_INTERFACE_INFO) * numInfos);

    ret = processChain(response, sizeof(RESP_SMB2_IOCTL) + sizeof(RESP_NETWORK_INTERFACE_INFO));
#endif //kUSE_SMB_NETWORK_INTERFACE_INFO

#else
    ret = errorResponse(response, static_cast<ULONG>(STATUS_FILE_CLOSED));
#endif //kUSE_SMB3 
  }
  else if (0x00140078 == pReq->CtlCode)
  {
    QTRACE((L"Do not support FSCTL_SRV_REQUEST_RESUME_KEY"));
    PrintHexDump(pReq->InputCount + sizeof(REQ_SMB2_IOCTL), (PBYTE)pReq);
    ret = errorResponse(response, static_cast<ULONG>(STATUS_NOT_SUPPORTED));
  }
  else if(FSCTL_CREATE_OR_GET_OBJECT_ID == pReq->CtlCode)
  {
    QTRACE((L"Do not support FSCTL_CREATE_OR_GET_OBJECT_ID"));
    ret = errorResponse(response, static_cast<ULONG>(STATUS_FS_DRIVER_REQUIRED));
  }
  else if(FSCTL_PIPE_WAIT == pReq->CtlCode)
  {
    PBYTE input = (PBYTE)(header()) + pReq->InputOffset;
    PREQ_FSCTL_PIPE_WAIT pPipeWait = (PREQ_FSCTL_PIPE_WAIT)input;
    String name(pPipeWait->Name, pPipeWait->NameLength/2);
    QTRACE((L"Pipe Wait %s", name.c_str()));
    ret = errorResponse(response, static_cast<ULONG>(STATUS_FS_DRIVER_REQUIRED));
  }
  else
  {
    bool isFSCTL = ((SMB2_0_IOCTL_IS_FSCTL & header()->Flags) == SMB2_0_IOCTL_IS_FSCTL);
    QTRACE((L"Unknown %s 0x%08x", isFSCTL?L"FSCTL":L"IOCTL", pReq->CtlCode));
    PrintHexDump(pReq->InputCount + sizeof(REQ_SMB2_IOCTL), (PBYTE)pReq);
    ret = errorResponse(response, static_cast<ULONG>(STATUS_FS_DRIVER_REQUIRED));
  }

  if(requestRequireSign(header()) && !useClassicSMBHandler())
  {
    cPtr<cSMB2Session> session(getSession(response));
    response->sign(session);
  }

  return ret;
}

void cSMB2Request::check(cPtr<cSMB2Session> session) const
{
  //QTRACE((L"cSMB2Request::check required."));

  BYTE sig[16];
  PSMB2_HEADER head = header();
  memcpy(sig, head->Signature, 16);
  memset(head->Signature, 0, 16);

  session->VerifyThis((PBYTE)Buffer, BufferSize, sig, 16);
}

tResponseList cSMB2Request::processCancel(cPtr<cSMB2Response> response) const
{
  const cPtr<cSMB2Session> pSession(getSession(response));
  if (pSession.isValid())
  {
    if (header()->Flags & SMB2_FLAGS_ASYNC_COMMAND) // Cancel async request
    {
      pSession->CancelNotifications(async_header()->AsyncId, false);
    }
    else if (header()->MessageId != 0) // Cancel sync request
    {
      QSOS((L"cSMB2Request::processCancel - Received synchronous cancel request! ID:%I64d", header()->MessageId));
      pSession->CancelNotifications(header()->MessageId, true);
    }
  }

  //QTRACE((L"cSMB2Request::processCancel - return empty list"));
  return tResponseList();
}

tResponseList cSMB2Request::processKeepAlive(cPtr<cSMB2Response> response) const
{
  const PRESP_SMB2_ECHO pResp = response->getResponseType<RESP_SMB2_ECHO>();
  pResp->StructureSize = 4;
  pResp->Reserved = 0;
  return processChain(response, sizeof(RESP_SMB2_ECHO));
}

template <typename T>
static void setLengthAndOffset(const cPtr<cSMB2Response>& response
                               , const T pResp
                               , const ULONG bufferLength
                               , const PUCHAR pBuf)
{
  pResp->OutputBufferOffset = static_cast<USHORT>(pBuf - reinterpret_cast<PUCHAR>(response->header()));
  pResp->OutputBufferLength = bufferLength;
  response->addData(bufferLength);
}

tResponseList cSMB2Request::processFind(cPtr<cSMB2Response> response) const
{
  const PREQ_SMB2_QUERY_DIRECTORY pReq = getParams<PREQ_SMB2_QUERY_DIRECTORY>();
  //QTRACE((L"cSMB2Request::processFind %I64x, FileInformationClass %d, Flags %x, Index %d, OutputBufferLength %d", Fid(pReq), (DWORD)pReq->FileInformationClass, (DWORD)pReq->Flags, pReq->FileIndex, pReq->OutputBufferLength));

  const PRESP_SMB2_QUERY_DIRECTORY pResp = response->getResponseType<RESP_SMB2_QUERY_DIRECTORY>();
  const cPtr<cSMB2Tree> pTree = getTree(response);
  if (pTree.isNull())
    return errorResponse(response);

  String sSearchPath = makeSafeName(reinterpret_cast<PUCHAR>(header()) + pReq->FileNameOffset
    , pReq->FileNameLength);
  fixupPath(sSearchPath);

  //QTRACE((L"cSMB2Request::processFind %s", sSearchPath.c_str()));

  tFileInfoList searchResults;
  ULONG respLength(0);
  const PUCHAR pRespStart(reinterpret_cast<PUCHAR>(pResp+1));
  DWORD dwRet = pTree->Find(Fid(pReq)
    , pReq
    , sSearchPath
    , reinterpret_cast<PUCHAR>(pResp)
    , pRespStart
    , respLength
    , response);
  pResp->StructureSize = 9;

  if (STATUS_SUCCESS != dwRet)
  {
    response->header()->Status = dwRet;
    pResp->OutputBufferLength = 0;
    pResp->OutputBufferOffset = 0;
    // data culled from Wiresharking
    const PULONGLONG pUnexplainedLong = reinterpret_cast<PULONGLONG>(pResp+1);
    *pUnexplainedLong = 5;
    response->addData(sizeof(ULONGLONG));
    return processChain(response, sizeof(RESP_SMB2_QUERY_DIRECTORY) + sizeof(ULONGLONG));
  }

  setLengthAndOffset(response, pResp, respLength, pRespStart);
  return processChain(response, sizeof(RESP_SMB2_QUERY_DIRECTORY) + respLength);
}

tResponseList cSMB2Request::processNotify(cPtr<cSMB2Response> response) const
{
  PREQ_SMB2_CHANGE_NOTIFY pReq;
  const cPtr<iComposite> pComposite = getComposite<REQ_SMB2_CHANGE_NOTIFY>(pReq, response);
  if (pComposite.isNull())
    return errorResponse(response);
  
  const cPtr<cMemoryView> pNotifyMem = new cMemoryView(new cMemory((size_t)pReq->OutputBufferLength, cMemory::eVirtual));

  const cPtr<cSMB2Response> pAsyncResponse = new cSMB2Response(this, pNotifyMem->getBytes(), pNotifyMem);
  {
    const PSMB2_ASYNC_Header pAsyncHeader = pAsyncResponse->async_header();
    pAsyncHeader->AsyncId = header()->MessageId;
    pAsyncHeader->Flags |= SMB2_FLAGS_ASYNC_COMMAND;
  }

  //QTRACE((L"cSMB2Request::processNotify add request:%I64d", header()->MessageId));
  cPtr<cSMB2NotificationReceiver> pSMB2NotificationReceiver = new cSMB2NotificationReceiver(pAsyncResponse, pReq->CompletionFilter, cPtr<cGuardedUniqueNotify>());

  DWORD dwRet = pComposite->registerNotification(header()->MessageId
    , pSMB2NotificationReceiver);

  if (ERROR_SUCCESS != dwRet)
  {
    return errorResponse(response, SMB_ERR_CLASS_DOS, dwRet);
  }

  const PSMB2_ASYNC_Header pAsyncHeader = response->async_header();

  pAsyncHeader->Flags |= SMB2_FLAGS_ASYNC_COMMAND;
  pAsyncHeader->Status = STATUS_PENDING;
  pAsyncHeader->AsyncId = header()->MessageId;

  //QSOS((L"cSMB2Request::processNotify - credit allocation: async:%d, sync:%d", response->async_header()->CreditRequest, response->header()->CreditRequest));
  //pAsyncHeader->CreditRequest = 1;//JCJCJC WRONG - used for testing

  const PRESP_SMB2_ERROR pResp = response->getResponseType<RESP_SMB2_ERROR>();
  pResp->StructureSize = 9; //[#2] p.24
  pResp->Reserved = 0;
  pResp->ByteCount = 0;
  pResp->ErrorData = 0;

  return processChain(response, 0);
}

template <typename T>
static void writeAllocationPacket(const T pResp, const cPtr<cSMB2Tree> pTree)
{
  DISK_GEOMETRY_EX disk_geometry;
  ULARGE_INTEGER free_space;
  DWORD sectorPerCluster;

  pTree->QueryInfoAllocation(disk_geometry, &free_space, &sectorPerCluster);

  pResp->TotalAllocationUnits = disk_geometry.DiskSize.QuadPart;
  pResp->ActualAvailableAllocationUnits = free_space.QuadPart;
  pResp->SectorsPerAllocationUnit = sectorPerCluster;
  pResp->BytesPerSector = disk_geometry.Geometry.BytesPerSector;
}

tResponseList cSMB2Request::processGetInfo(cPtr<cSMB2Response> response) const
{
  const cPtr<cSMB2Tree> pTree = getTree(response);
  const PRESP_SMB2_QUERY_INFO pResp = response->getResponseType<RESP_SMB2_QUERY_INFO>();

  if (pTree.isNull())
    return errorResponse(response);

  PREQ_SMB2_QUERY_INFO pReq = getParams<PREQ_SMB2_QUERY_INFO>();
  const PUCHAR pRespBuf(reinterpret_cast<PUCHAR>(pResp+1));
  ULONG respLength(0);
  switch(pReq->InfoType)
  {
  case SMB2_0_INFO_FILE:
    {
    const cPtr<iComposite> pComposite = pTree->getComposite(Fid(pReq), response);
    if (pComposite.isNull())
      return errorResponse(response);

    switch (pReq->FileInfoClass)
    {
    case kFileBothDirectoryInformation: // MWMWMW not sure about this -probably wrong - JCJCJC yes - different return structures required!
    case kFileDirectoryInformation:
      {
        fileInfo fi;
        pComposite->writeInfo(&fi, Fid(pReq));
        respLength = writeFileAttributeInformation<FileDirectoryInformation>(pRespBuf, fi);
      }
      break;
    case kFileStandardInformation:
      {
        FileStandardInformation* const pFileStandardInformation =
          reinterpret_cast<FileStandardInformation* >(pRespBuf);

        BY_HANDLE_FILE_INFORMATION bhfi;
        pComposite->writeInfo(&bhfi, Fid(pReq));

        hiAndLowToLG(bhfi.nFileSizeHigh, bhfi.nFileSizeLow, pFileStandardInformation->EndOfFile);
        pFileStandardInformation->AllocationSize = ((pFileStandardInformation->EndOfFile + DEFAULT_SECTOR_SIZE-1)/DEFAULT_SECTOR_SIZE)*DEFAULT_SECTOR_SIZE;

        pFileStandardInformation->NumberOfLinks = 1;
        pFileStandardInformation->DeletePending = pTree->deletePending(Fid(pReq));
        pFileStandardInformation->Directory = (FILE_ATTRIBUTE_DIRECTORY & bhfi.dwFileAttributes)?1:0;
        respLength = sizeof(FileStandardInformation);
      }
      break;
    case kFileInternalInformation:
      {
        FileInternalInformation* const pFileInteralInformation =
          reinterpret_cast<FileInternalInformation* >(pRespBuf);
        pFileInteralInformation->IndexNumber = 0;//MUST be zero for fiflsystems that do not support this feature
        respLength = sizeof(FileInternalInformation);
      }
      break;
    case kFileEaInformation:
      {
        FileEaInformation* const pFileEAInformation = reinterpret_cast<FileEaInformation* >(pRespBuf);
        pFileEAInformation->Size = 0;
        respLength = sizeof(FileEaInformation);
      }
      break;
    case kFileBasicInformation:
      {
        BY_HANDLE_FILE_INFORMATION bhfi;
        pComposite->writeInfo(&bhfi, Fid(pReq));
        FileBasicInformation* const pFileBasicInformation = reinterpret_cast<FileBasicInformation* >(pRespBuf);
        writeAttributes(bhfi, pFileBasicInformation);
        respLength = sizeof(FileBasicInformation);
      }
      break;
    case kFileNetworkOpenInformation:
      {
        FileNetworkOpenInformation* const pFileNetworkOpenInformation = reinterpret_cast<FileNetworkOpenInformation* >(pRespBuf);
        pFileNetworkOpenInformation->Reserved = 0;

        BY_HANDLE_FILE_INFORMATION bhfi;
        pComposite->writeInfo(&bhfi, Fid(pReq));
        hiAndLowToLG(bhfi.nFileSizeHigh, bhfi.nFileSizeLow, pFileNetworkOpenInformation->EndOfFile);
        pFileNetworkOpenInformation->AllocationSize = ((pFileNetworkOpenInformation->EndOfFile + DEFAULT_SECTOR_SIZE-1)/DEFAULT_SECTOR_SIZE)*DEFAULT_SECTOR_SIZE;

        writeAttributes(bhfi, pFileNetworkOpenInformation);
        respLength = sizeof(FileNetworkOpenInformation);
      }
      break;
    case kFileAllInformation:
      {       
//Apple call this during connect ...
        FileAllInformation* const pFileAllInformation = reinterpret_cast<FileAllInformation* >(pRespBuf);
        memset(pFileAllInformation, 0, sizeof(FileAllInformation));
        respLength = sizeof(FileAllInformation);

//	FileBasicInformation Basic;
        FileBasicInformation* pFileBasicInformation = &(pFileAllInformation->Basic);
        BY_HANDLE_FILE_INFORMATION bhfi;
        pComposite->writeInfo(&bhfi, Fid(pReq));
        writeAttributes(bhfi, pFileBasicInformation);

//	FileStandardInformation Standard;
        FileStandardInformation* const pFileStandardInformation = &(pFileAllInformation->Standard);
        hiAndLowToLG(bhfi.nFileSizeHigh, bhfi.nFileSizeLow, pFileStandardInformation->EndOfFile);
        pFileStandardInformation->AllocationSize = ((pFileStandardInformation->EndOfFile + DEFAULT_SECTOR_SIZE-1)/DEFAULT_SECTOR_SIZE)*DEFAULT_SECTOR_SIZE;

        pFileStandardInformation->NumberOfLinks = 1;
        pFileStandardInformation->DeletePending = pTree->deletePending(Fid(pReq));
        pFileStandardInformation->Directory = (FILE_ATTRIBUTE_DIRECTORY & bhfi.dwFileAttributes)?1:0;

//	FileInternalInformation Internal;
        FileInternalInformation* const pFileInteralInformation = &(pFileAllInformation->Internal);
        pFileInteralInformation->IndexNumber = 0;//MUST be zero for fiflsystems that do not support this feature

//	FileEaInformation Ea;
        FileEaInformation* const pFileEAInformation = &(pFileAllInformation->Ea);
        pFileEAInformation->Size = 0;

//	FileAccessInformation Access;
//	FilePositionInformation Position;
//	FileModeInformation Mode;
//	FileAlignmentInformation Alignment;
//	FileNameInformation Name;
      }
      break;
    default:
      return processUnhandled(response);
      break;
    }
    break;
    }
  case SMB2_0_INFO_FILESYSTEM:
    switch (pReq->FileInfoClass)
    {
    case kFileFsVolumeInformation:
      {
        FileFsVolumeInformation* const pFileFsVolumeInformation = reinterpret_cast<FileFsVolumeInformation* >(pRespBuf);
        GetSystemTimeAsFileTime(reinterpret_cast<LPFILETIME>(&(pFileFsVolumeInformation->VolumeCreationTime)));
        pFileFsVolumeInformation->VolumeSerialNumber = rand();

        static const String sVolLabel(L"");
        if (sVolLabel.empty())
          pFileFsVolumeInformation->VolumeLabelLength = 0;
        else
        {
          pFileFsVolumeInformation->VolumeLabelLength = (sVolLabel.size())* sizeof(WCHAR);
          memcpy(reinterpret_cast<PUCHAR>(pFileFsVolumeInformation+1)
            , sVolLabel.c_str()
            , pFileFsVolumeInformation->VolumeLabelLength);
        }

        pFileFsVolumeInformation->SupportsObjects = 0;
        pFileFsVolumeInformation->Reserved = 0;
        respLength = sizeof(FileFsVolumeInformation) + pFileFsVolumeInformation->VolumeLabelLength;
      }
      break;
    case kFileFsSizeInformation:
      {
        writeAllocationPacket(reinterpret_cast<FileFsSizeInformation* >(pRespBuf), pTree);
        respLength = sizeof(FileFsSizeInformation);
      }
      break;
    case kFileFsAttributeInformation:
      {
        FileFsAttributeInformation* const pFileFsAttributeInformation =
          reinterpret_cast<FileFsAttributeInformation* >(pRespBuf);
        pFileFsAttributeInformation->FileSystemAttributes = FILE_CASE_PRESERVED_NAMES;
        pFileFsAttributeInformation->MaximumComponentNameLength = UCHAR_MAX;
        static String sFileSysName(L"QCIFS");
        pFileFsAttributeInformation->FileSystemNameLength = sFileSysName.size()* sizeof(WCHAR);
        memcpy(reinterpret_cast<PUCHAR>(pFileFsAttributeInformation+1)
          , sFileSysName.c_str()
          , pFileFsAttributeInformation->FileSystemNameLength);
        respLength = sizeof(FileFsAttributeInformation) + pFileFsAttributeInformation->FileSystemNameLength;
      }
      break;
    case kFileFsFullSizeInformation:
      {
        FileFsFullSizeInformation* const pFileFsAttributeInformation =
          reinterpret_cast<FileFsFullSizeInformation* >(pRespBuf);
        writeAllocationPacket(pFileFsAttributeInformation, pTree);
        pFileFsAttributeInformation->CallerAvailableAllocationUnits = pFileFsAttributeInformation->ActualAvailableAllocationUnits;
        respLength = sizeof(FileFsFullSizeInformation);
      }
      break;
    default:
      return processUnhandled(response);
      break;
    }
    break;
  case SMB2_0_INFO_SECURITY:
    {  
      const cPtr<iComposite> pComposite = pTree->getComposite(Fid(pReq), response);
      if (pComposite.isNull())
      {
        return errorResponse(response);
      }

      // disregard security descriptor for win 2012.
      /*
      if (!pComposite->hasSecurityDescriptor())
      {
        QSOS((L"no security descriptor"));
        return processUnhandled(response);
      }
      */
      PACL pDacl = NULL;
      PSID pEveryoneSid = NULL;   
      PTOKEN_USER pTokenUser = NULL;  

      try
      {
        HANDLE hToken = NULL;   
        DWORD dwBufferSize = 0;

        // Open the access token associated with the calling process.
        if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken) == FALSE)
          throw cNotSupported(__FILE__, __LINE__, L"OpenProcessToken failed");

        // Retrieve the token information in a TOKEN_USER structure.
        GetTokenInformation(hToken, TokenUser, NULL, 0, &dwBufferSize);
        pTokenUser = (PTOKEN_USER) new BYTE[dwBufferSize];
        memset(pTokenUser, 0, dwBufferSize);
        if (GetTokenInformation(hToken, TokenUser, pTokenUser, dwBufferSize, &dwBufferSize))
          CloseHandle(hToken);
        else
          throw cNotSupported(__FILE__, __LINE__, L"GetTokenInformation failed");

        // Validate the user SID
        if (IsValidSid(pTokenUser->User.Sid) == FALSE)
          throw cNotSupported(__FILE__, __LINE__, L"The owner SID is invalid");

        // Retrieve the SID of the Everyone group.
        SID_IDENTIFIER_AUTHORITY WorldAuth = SECURITY_WORLD_SID_AUTHORITY;
        if (AllocateAndInitializeSid(&WorldAuth, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &pEveryoneSid) == FALSE)
          throw cNotSupported(__FILE__, __LINE__, L"AllocateAndInitializeSid failed");

        // Calculate the amount of memory that must be allocated for the DACL.  
        DWORD cbDacl = sizeof(ACL) + sizeof(ACCESS_ALLOWED_ACE) - sizeof(DWORD);
        cbDacl += GetLengthSid(pEveryoneSid);

        // Allocate memory for the Dacl
        pDacl = (PACL) new BYTE[cbDacl];  
        if (pDacl == NULL)
          throw cNotSupported(__FILE__, __LINE__, L"Allocation of Dacl failed");
        memset(pDacl, 0, cbDacl);
        if (InitializeAcl(pDacl, cbDacl, ACL_REVISION) == FALSE)
          throw cNotSupported(__FILE__, __LINE__, L"InitializeAcl failed");

        // Add an allowed access Ace for Everyone group to the dacl
        if (AddAccessAllowedAce(pDacl, ACL_REVISION, FILE_GENERIC_READ || FILE_GENERIC_EXECUTE, pEveryoneSid) == FALSE)
          throw cNotSupported(__FILE__, __LINE__, L"AddAccessAllowedAce failed");

        // Setup the security descriptor object which we return
        SECURITY_DESCRIPTOR SecurityDescriptor;
        if(InitializeSecurityDescriptor(&SecurityDescriptor, SECURITY_DESCRIPTOR_REVISION) == FALSE)
          throw cNotSupported(__FILE__, __LINE__, L"InitializeSecurityDescriptor failed");

        if((pReq->AdditionalInformation & REQUEST_OWNER) == REQUEST_OWNER)
        {
          if(SetSecurityDescriptorOwner(&SecurityDescriptor, pTokenUser->User.Sid, TRUE) == FALSE)
            throw cNotSupported(__FILE__, __LINE__, L"SetSecurityDescriptorOwner failed");
        }

        if((pReq->AdditionalInformation & REQUEST_GROUP) == REQUEST_GROUP)
        {
          if(SetSecurityDescriptorGroup(&SecurityDescriptor, pEveryoneSid, TRUE) == FALSE)
            throw cNotSupported(__FILE__, __LINE__, L"SetSecurityDescriptorGroup failed");
        }

        if((pReq->AdditionalInformation & REQUEST_DACL) == REQUEST_DACL)
        {
          if(SetSecurityDescriptorDacl(&SecurityDescriptor, TRUE, pDacl, FALSE) == FALSE)
            throw cNotSupported(__FILE__, __LINE__, L"SetSecurityDescriptorDacl failed");
        }

        if((pReq->AdditionalInformation & REQUEST_SACL) == REQUEST_SACL)
          throw cNotSupported(__FILE__, __LINE__, L"Security Descriptor SACL requests are currently unsupported");

        // We require the security descriptor to be in self-relative format for sending over a wire
        PUCHAR const pSRSecurityDescriptor = reinterpret_cast<PUCHAR>(pRespBuf);
        DWORD bufSize = pReq->OutputBufferLength;
        if(MakeSelfRelativeSD(&SecurityDescriptor, pRespBuf, &bufSize) == FALSE)
        {
          DWORD dwErrorCode = GetLastError();
          if( (bufSize > pReq->OutputBufferLength) || (dwErrorCode == 122) )
          {
            response->setNTError(STATUS_BUFFER_TOO_SMALL);
            BufferFullError* const pError = reinterpret_cast<BufferFullError*>(pRespBuf);
            pError->RequiredBufferSize = bufSize;
            setLengthAndOffset(response
              , pResp
              , sizeof(BufferFullError)
              , pRespBuf);
            return processChain(response, sizeof(RESP_SMB2_ERROR) + sizeof(BufferFullError));
          }
         
          throw cNotSupported(__FILE__, __LINE__, L"MakeSelfRelativeSD failed");
        }

        respLength = bufSize;
      }
      catch(cNotSupported& err)
      {
        DWORD dwErrorCode = GetLastError();
        QSOS((L"%s. GetLastError returned: %d", err.getMsg().c_str(), dwErrorCode));
        if (pTokenUser)
          delete [] pTokenUser;
        if (pEveryoneSid)
          FreeSid(pEveryoneSid);
        if (pDacl)
          delete [] pDacl;
        
        return processUnhandled(response);
      }

      delete [] pTokenUser;
      FreeSid(pEveryoneSid);
      delete [] pDacl;
    }
    break;
  //case SMB2_0_INFO_QUOTA:
  //  break;
  default:
    return processUnhandled(response);
    break;
  }

  setLengthAndOffset(response
    , pResp
    , respLength
    , pRespBuf);
  pResp->StructureSize = 9;
  return processChain(response, sizeof(RESP_SMB2_QUERY_INFO) + respLength);
}

FILETIME convert(ULONGLONG ull)
{
  ULARGE_INTEGER ulg;
  ulg.QuadPart = ull;
  FILETIME ft;
  ft.dwHighDateTime = ulg.HighPart;
  ft.dwLowDateTime = ulg.LowPart;
  return ft;
}

tResponseList cSMB2Request::processSetInfo(cPtr<cSMB2Response> response) const
{
  PREQ_SMB2_SET_INFO pReq;
  const cPtr<iComposite> pComposite = getComposite<REQ_SMB2_SET_INFO>(pReq, response);
  if (pComposite.isNull())
    return errorResponse(response);

  switch (pReq->InfoType)
  {
  case SMB2_0_INFO_FILE:
    switch (pReq->FileInfoClass)
    {
    case kFileRenameInformation:
      {
      FileRenameInformation* pFileRenameInformation
        = reinterpret_cast<FileRenameInformation*>(pReq+1);
      const String sNewName = L"\\" + makeSafeName(reinterpret_cast<PUCHAR>(pFileRenameInformation+1)
        , pFileRenameInformation->FileNameLength);
      cPtr<cSMB2Tree> pTree = getTree(response);
      if(pTree.isNull())
        return errorResponse(response);
      const DWORD dwRet = pTree->Rename(Fid(pReq), sNewName);
      if (ERROR_SUCCESS != dwRet)
        return errorResponse(response, SMB_ERR_CLASS_DOS, dwRet);
      }
      break;
    case kFileDispositionInformation:
      {
      const FileDispositionInformation* const pFileDispositionInformation
        = reinterpret_cast<FileDispositionInformation*>(pReq+1);
      if (pFileDispositionInformation->DeletePending)
      {
        const DWORD dwRet = pComposite->Delete();
        if (ERROR_SUCCESS != dwRet)
          return errorResponse(response, SMB_ERR_CLASS_DOS, dwRet);
      }
      }
      break;
    case kFileBasicInformation:
      {
      const FileBasicInformation* const pFileBasicInformation
        = reinterpret_cast<FileBasicInformation*>(pReq+1);
      pComposite->SetFileInfo(convert(pFileBasicInformation->CreationTime)
        , convert(pFileBasicInformation->LastWriteTime)
        , convert(pFileBasicInformation->LastAccessTime)
        , pFileBasicInformation->FileAttributes);
      }
      break;
    case kFileAllocationInformation:
      // MWMWMW This operation is used to query the allocation size for a file
      // It's just returns a status code to specify if the file system has room for the file
      // Should we be checking if the system's low on memory?
      break;
    case kFileEndOfFileInformation:
      {
      const FileEndOfFileInformation* const pFileEndOfFileInformation
        = reinterpret_cast<FileEndOfFileInformation*>(pReq+1);
      PLARGE_INTEGER temp = new LARGE_INTEGER();
      temp->QuadPart = (LONGLONG)pFileEndOfFileInformation->EndOfFile;
      pComposite->SetFileSize(temp);
      }
      break;
    default:
      return processUnhandled(response);
      break;
    }
    break;
  default:
    return processUnhandled(response);
    break;
  }

  PRESP_SMB2_SET_INFO pResp = response->getResponseType<RESP_SMB2_SET_INFO>();
  pResp->StructureSize = 2; //[#2]p.92

  return processChain(response, sizeof(RESP_SMB2_SET_INFO));
}

tResponseList cSMB2Request::processBreak(cPtr<cSMB2Response> response) const
{
  const PRESP_SMB2_OPLOCK_BREAK_ACK pReq = getParams<PRESP_SMB2_OPLOCK_BREAK_ACK>();
  const PRESP_SMB2_OPLOCK_BREAK_RESP pResp = response->getResponseType<RESP_SMB2_OPLOCK_BREAK_RESP>();

  pResp->StructureSize = pReq->StructureSize;
  pResp->OplockLevel = pReq->OplockLevel;
  pResp->FileId = pReq->FileId;
  
  return processChain(response, sizeof(RESP_SMB2_OPLOCK_BREAK_RESP));
}

///////////////////////////////////////////////////////////////////////////////////////
