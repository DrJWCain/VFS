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

//cSMBRequest.cpp
//
// References:
// [#1] SNIA CIFS Technical Reference Revision 1.0

#include "stdafx.h"

#if CIFS 

#include "mycifs.h"
#include "cSMBRequest.h"
#include "cSMBResponse.h"
#include "cSMBSession.h"
#include "iOpLockManager.h"
#include "iShareManager.h"
#include "gss.h"
#include "align.h"
#include "socket_routines.h"
#include "cSMB2Request.h"
#include "LogCategories.h"
#include "cQCIFSPacketProcessor.h"

#include "..\iTreeResource.h"
#include "..\SocketServer\iPacket.h"
#include "..\QCIFSIPC\iQCIFSIPC.h"
//#include "..\..\MiscPlugins\QOptions\QOptions.h"

using namespace std;

//static bool checkOption(const String& optionToTest) 
//{
//  try
//  {
//    iOptionManager::singleton().checkOut (optionToTest);
//    return true;
//  }
//  catch (cRecoverable&)
//  {
//    return false;
//  }
//}

bool useSMB2()
{
  return true;
  //static bool tested = false;
  //static bool UseSMB2 = false;
  //if(!tested)
  //{
  //  UseSMB2 = checkOption(L"SMB2");;
  //  tested = true;
  //}
  //return UseSMB2;
}

// Turn off DFS for now until we properly implement it, caused problems with linux CIFS driver versions greater than 1.52
//#define DFS 
//globals
extern tActivityMap ActivityQCIFSProcessors;

cSMBRequest::tProcessFuncPtr cSMBRequest::Dispatch[UCHAR_MAX] = {0};

static USHORT sessID=2;

typedef std::map<int, cPtr<cSMBSession> > tSessionMap;
static tSessionMap gSessionMap;
static cLock gSessionLock;

//{{{
class cMicroTimer
{
  LARGE_INTEGER StartTime;
public:
  void start()
  {
    QueryPerformanceCounter(&StartTime);
  }
  int stop()
  {
    LARGE_INTEGER stopTime;
    QueryPerformanceCounter(&stopTime);
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);

    LARGE_INTEGER result;
    result.QuadPart = ((stopTime.QuadPart - StartTime.QuadPart)*1000000)/freq.QuadPart;
    return result.LowPart;
  }
};
//}}}
//{{{
class cMicroTimerTor : private cMicroTimer
{
  String Message;
public:
  cMicroTimerTor(const String& message) : Message(message)
  {
    start();
  }
  ~cMicroTimerTor()
  {
    int time = stop();
    QTRACE((L"%s %dus",Message.c_str(),time));
  }
};
//}}}

template <typename T, typename U>
T readSMBBufferString(U& rBuffer)
{
  T retString(rBuffer);
  rBuffer += retString.size() + 1;
  return retString;
}

template <typename T, typename U>
void getUserStrings(String& rAccount, const U pBufferData, const String& formatChar)
{
  U pBufferStrings = pBufferData;

  const T sAccount(readSMBBufferString<T, U>(pBufferStrings));
  rAccount = widen(sAccount);
  QTRACE((String(L"Account name: " + formatChar).c_str(), sAccount.c_str()));

  const T sPrimaryDomain(readSMBBufferString<T, U>(pBufferStrings));
  QTRACE((String(L"Primary Domain is: " + formatChar).c_str(), sPrimaryDomain.c_str()));

  const T sNativeOS(readSMBBufferString<T, U>(pBufferStrings));
  QTRACE((String(L"Native OS is: " + formatChar).c_str(), sNativeOS.c_str()));

  const T sNativeLANMan(readSMBBufferString<T, U>(pBufferStrings));
  QTRACE((String(L"Native LAN Manager is: " + formatChar).c_str(), sNativeLANMan.c_str()));
}

///////////////////////////////////////////////////////////////////////////////////////
// cSMBRequest ////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

cSMBRequest::cSMBRequest(const cPtr<iRXPacket> pRXPacket
  , const cPtr<iShareManager> pShareManager
  , const cPtr<iOpLockManager> pOpLockManager
  , cQCIFSPacketProcessor* pQCIFSPacketProcessor) 
: cSMBBuffer(getSmbBufferFromNetBIOS(reinterpret_cast<unsigned char*>(pRXPacket->getRXBuffer()->getBytes()), m_nDataLength), m_nDataLength)
, m_pContextPacket(pRXPacket)
, m_pShareManager(pShareManager)
, m_pOpLockManager(pOpLockManager)
, m_pQCIFSPacketProcessor(pQCIFSPacketProcessor)
{
}

cSMBRequest::~cSMBRequest() 
{
}

void cSMBRequest::log() const
{
  if(SMB_COM_NT_CREATE_ANDX == Header->Command)
  {
    PREQ_NT_CREATE_ANDX pReq = (PREQ_NT_CREATE_ANDX)getParams(sizeof(REQ_NT_CREATE_ANDX));
    String sPath(reinterpret_cast<PWCHAR>(pReq->Buffer + getAlignment<WORD>(pReq->Buffer, reinterpret_cast<PUCHAR>(Header))));
    QMSG((kCtgLogFID, L"%s %s", desc().c_str(), sPath.c_str()));
    return;
  }
  if(SMB_COM_OPEN_ANDX == Header->Command)
  {
    PREQ_OPEN_ANDX pReq = (PREQ_OPEN_ANDX)getParams(static_cast<USHORT>(sizeof(REQ_OPEN_ANDX)));
    String sPath(reinterpret_cast<PWCHAR>(pReq->Buffer + getAlignment<WORD>(pReq->Buffer, reinterpret_cast<PUCHAR>(Header))));
    QMSG((kCtgLogFID, L"%s %s", desc().c_str(), sPath.c_str()));
    return;
  }
  if(SMB_COM_CLOSE == Header->Command)
  {
    PREQ_CLOSE pReq = (PREQ_CLOSE)getParams(sizeof(REQ_CLOSE));
    QMSG((kCtgLogFID, L"%s F%x", desc().c_str(), pReq->Fid));
    return;
  }

  if(SMB_COM_WRITE_ANDX == Header->Command)
  {
    PREQ_NT_WRITE_ANDX pReq = (PREQ_NT_WRITE_ANDX)getParams(sizeof(REQ_NT_WRITE_ANDX));
    QMSG((kCtgLogIO, L"%s F%x W%x%x:%x%x", desc().c_str(), pReq->Fid, pReq->OffsetHigh, pReq->Offset, pReq->DataLengthHigh, pReq->DataLength));
    return;
  }
  if(SMB_COM_WRITE == Header->Command)
  {
    PREQ_WRITE pReq = (PREQ_WRITE)getParams(5);
    QMSG((kCtgLogIO, L"%s F%x W%x:%x", desc().c_str(), pReq->Fid, pReq->Offset, pReq->Count));
    return;
  }
  if(SMB_COM_READ_ANDX == Header->Command)
  {
    PREQ_NT_READ_ANDX pReq = (PREQ_NT_READ_ANDX)getParams(sizeof(REQ_NT_READ_ANDX));
    QMSG((kCtgLogIO, L"%s F%x R%x%x:%x%x", desc().c_str(), pReq->Fid, pReq->OffsetHigh, pReq->Offset, pReq->MaxCountHigh, pReq->MaxCount));
    return;
  }

  if(SMB_COM_TRANSACTION2 == Header->Command)
  {
    PREQ_TRANSACTION pReq = (PREQ_TRANSACTION)getParams(sizeof(REQ_TRANSACTION));
    if(TRANS2_QUERY_PATH_INFORMATION == pReq->Buffer[0])
    {
      PREQ_QUERY_PATH_INFORMATION pReqPath = reinterpret_cast<PREQ_QUERY_PATH_INFORMATION>(reinterpret_cast<const PUCHAR>(Header) + pReq->ParameterOffset);
      String sPath(reinterpret_cast<PWCHAR>(pReqPath->Buffer));
      QMSG((kCtgLogSMB, L"%s I%x P:%s", desc().c_str(), pReqPath->InformationLevel, sPath.c_str()));
      return;
    }
    if(TRANS2_QUERY_FILE_INFORMATION == pReq->Buffer[0])
    {
      PREQ_QUERY_FILE_INFORMATION pReqFile = reinterpret_cast<PREQ_QUERY_FILE_INFORMATION>(reinterpret_cast<const PUCHAR>(Header) + pReq->ParameterOffset);
      QMSG((kCtgLogSMB, L"%s QI%x F%x", desc().c_str(), pReqFile->InformationLevel, pReqFile->Fid));
      return;
    }
    if(TRANS2_SET_FILE_INFORMATION == pReq->Buffer[0])
    {
      PREQ_SET_FILE_INFORMATION pReqFile = reinterpret_cast<PREQ_SET_FILE_INFORMATION>(reinterpret_cast<const PUCHAR>(Header) + pReq->ParameterOffset);
      QMSG((kCtgLogSMB, L"%s SI%x F%x ", desc().c_str(), pReqFile->InformationLevel, pReqFile->Fid));
      return;
    }
    QMSG((kCtgLogSMB, L"%s T%x", desc().c_str(), pReq->Buffer[0]));
    return;
  }

  if(SMB_COM_NT_TRANSACT == Header->Command)
  {
    PREQ_NT_TRANSACTION pReq
      = (PREQ_NT_TRANSACTION)getParams(sizeof(REQ_NT_TRANSACTION));

    if(NT_TRANSACT_IOCTL == pReq->Function)
    {
      PREQ_NT_IO_CONTROL pReqIOCTL = (PREQ_NT_IO_CONTROL)(pReq->Buffer);
      QMSG((kCtgLogSMB, L"%s F%x IOCTL%x", desc().c_str(), pReqIOCTL->Fid, pReqIOCTL->FunctionCode));
      return;
    }
    if(NT_TRANSACT_NOTIFY_CHANGE == pReq->Function)
    {
      PREQ_NOTIFY_CHANGE pReqNotify = (PREQ_NOTIFY_CHANGE)(pReq->Buffer);//Special case - not normal for SMB_COM_NT_TRANSACT
      QMSG((kCtgLogSMB, L"%s F%x CF%x", desc().c_str(), pReqNotify->Fid, pReqNotify->CompletionFilter));
      return;
    }
    if(NT_TRANSACT_QUERY_SECURITY_DESC == pReq->Function)
    {
      PREQ_QUERY_SECURITY_DESCRIPTOR pReqQuery = (PREQ_QUERY_SECURITY_DESCRIPTOR)(reinterpret_cast<const PUCHAR>(Header) + pReq->ParameterOffset);
      QMSG((kCtgLogSMB, L"%s F%x S%x", desc().c_str(), pReqQuery->Fid, pReqQuery->SecurityInformation));
      return;
    }

    QMSG((kCtgLogSMB, L"%s T%x", desc().c_str(), pReq->Buffer[0]));
    return;
  }

  if(SMB_COM_TRANSACTION == Header->Command)
  {
    PREQ_TRANSACTION pReq = (PREQ_TRANSACTION)getParams(sizeof(REQ_TRANSACTION));

    if(pReq->ParameterCount == 0x13 && !pReq->DataCount)
    {
      String NamedPipe = (wchar_t*)&(pReq->Buffer[3]);
      QMSG((kCtgLogSMB, L"%s %x %s", desc().c_str(), *((USHORT*)(pReq->Buffer)), NamedPipe.c_str()));
      if(L"\\PIPE\\LANMAN" == NamedPipe)
      {
        QMSG((kCtgLogSMB, L"Found RAP!!! JCJCJC to do .... - but not yet!!!"));
      }
      return;
    }


    QMSG((kCtgLogSMB, L"%s P%x D%x", desc().c_str(), pReq->ParameterCount, pReq->DataCount));
    return;
  }

  if(SMB_COM_TREE_CONNECT_ANDX == Header->Command)
  {
    PREQ_TREE_CONNECT_ANDX pReq = (PREQ_TREE_CONNECT_ANDX)getParams(sizeof(REQ_TREE_CONNECT_ANDX));

    USHORT offset = sizeof(USHORT); // size of ByteCount field
    PUCHAR pBufferData = getData(offset);
    //pBufferData += wordAlign(pBufferData, reinterpret_cast<PUCHAR>(Header));

    //if (pReq->PasswordLength > 0)
    //{
    //  PUCHAR pPWBuffer = new UCHAR[pReq->PasswordLength+1];
    //  memcpy(pPWBuffer, pBufferData, pReq->PasswordLength);  // use memcpy not strcpy
    //  pPWBuffer[pReq->PasswordLength] = '\0';
    //  //QTRACE((L"Password: %s", pPWBuffer));
    //  delete[] pPWBuffer;
    //}

    wchar_t* pBufferStrings = (wchar_t*)(pBufferData+pReq->PasswordLength);

    const String szPath(readSMBBufferString<String, wchar_t*>(pBufferStrings));
    QMSG((kCtgLogSMB, L"%s P:%s", desc().c_str(), szPath.c_str()));
    return;
  }

  QMSG((kCtgLogSMB, L"%s", desc().c_str()));
}


///////////////////////////////////////////////////////////////////////////////////////
// blocks /////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

void cSMBRequest::nextBlock() const
{
  if(size() > BufferSize)
    throw exception();

  PGENERIC_ANDX pAndX = getAndX();
  unsigned int offset = pAndX->AndXOffset;
  if(offset > BufferSize)
    throw exception();

  cSMBBlock block;
  //unsigned int blockLength = block.read(Buffer+offset,BufferSize-offset);
  block.read(Buffer+offset,BufferSize-offset);
  Blocks.push_back(block);

  if(size() > BufferSize)
    throw exception();
}

///////////////////////////////////////////////////////////////////////////////////////
// Generic Handlers ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

void cSMBRequest::getResponses(cPtr<cMemoryView> pMem, tResponseList& responses, vfs::iActivity::Ptr activity)
{
  // check request header
  Header = getSmb(Buffer,BufferSize);

  try
  {
    unsigned int offset = sizeof(NT_SMB_HEADER);
    if(offset > BufferSize)
      throw exception();

    cSMBBlock block;
    //unsigned int blockLength = block.read(Buffer+offset,BufferSize-offset);
    block.read(Buffer+offset,BufferSize-offset);
    Blocks.push_back(block);

    if(cSMBBuffer::size() > BufferSize)
      throw exception();

    //cPtr<cMemoryView> pMem;
    //{
    //  cActivityScope as1(getActivity(),kCreateBuffer);
    //  pMem = new cMemoryView(new cMemory((size_t)gkBufferSize, cMemory::eVirtual));
    //}

    cPtr<cSMBResponse> pResponse;
    {
      cActivityScope as1(getActivity(),kCreateResponse);
      pResponse = new cSMBResponse(*this, pMem);
    }

    log();
    {
      cActivityScope as1(ActivityQCIFSProcessors.find(1)->second,kProcessClose);
      responses = dispatchCommand(Header->Command, pResponse);
    }
    pResponse->log(this);
  }
  catch (const exception &err)
  {
    QSOS((L"cSMBRequest::getResponses() - caught exception %S", err.what()));
    cPtr<cSMBResponse> pResponse = new cSMBResponse(*this
      , new cMemoryView(new cMemory((size_t)gkBufferSize, cMemory::eVirtual), 0, checked_static_cast<size_t>(gkBufferSize)));
    pResponse->setDosError(SMB_ERR_CLASS_HARDWARE, SMB_ERR_GENERAL);
    responses.push_back(pResponse);
  }
}

tResponseList cSMBRequest::dispatchCommand(unsigned char command, cPtr<cSMBResponse> pResponse) const
{
  vfs::iActivity::Ptr activity = getActivity();
  if(!Dispatch[0])
  {
    for(int i = 0; i < UCHAR_MAX; ++i)
    {
      Dispatch[i] = &cSMBRequest::processUnhandled;
    }
    Dispatch[SMB_COM_OPEN_ANDX] = &cSMBRequest::processOpenAndX;
    Dispatch[SMB_COM_TREE_DISCONNECT] = &cSMBRequest::processTreeDisconnect;
    Dispatch[SMB_COM_NEGOTIATE] = &cSMBRequest::processNegotiate;
    Dispatch[SMB_COM_SESSION_SETUP_ANDX] = &cSMBRequest::processSessionSetupAndX;
    Dispatch[SMB_COM_TREE_CONNECT_ANDX] = &cSMBRequest::processTreeConnectAndX;
    Dispatch[SMB_COM_ECHO] = &cSMBRequest::processEcho;
    Dispatch[SMB_COM_NT_CREATE_ANDX] = &cSMBRequest::processNTCreateAndX;
    Dispatch[SMB_COM_TRANSACTION2] = &cSMBRequest::processTransaction2;
    Dispatch[SMB_COM_TRANSACTION2_SECONDARY] = &cSMBRequest::processTransaction2Secondary;
    Dispatch[SMB_COM_LOGOFF_ANDX] = &cSMBRequest::processLogoffAndX;
    Dispatch[SMB_COM_CLOSE] = &cSMBRequest::processClose;
    Dispatch[SMB_COM_NT_TRANSACT] = &cSMBRequest::processNtTrans;
    Dispatch[SMB_COM_READ_ANDX] = &cSMBRequest::processReadAndX;
    Dispatch[SMB_COM_WRITE_ANDX] = &cSMBRequest::processWriteAndX;
    Dispatch[SMB_COM_RENAME] = &cSMBRequest::processRename;
    Dispatch[SMB_COM_DELETE] = &cSMBRequest::processDelete;
    Dispatch[SMB_COM_LOCKING_ANDX] = &cSMBRequest::processLockingAndX;
    Dispatch[SMB_COM_WRITE] = &cSMBRequest::processWrite;
    Dispatch[SMB_COM_DELETE_DIRECTORY] = &cSMBRequest::processDeleteDirectory;
    Dispatch[SMB_COM_FIND_CLOSE2] = &cSMBRequest::processFindClose2;
    Dispatch[SMB_COM_CREATE_DIRECTORY] = &cSMBRequest::processCreateDirectory;
    Dispatch[SMB_COM_PROCESS_EXIT] = &cSMBRequest::processProcessExit;
    Dispatch[SMB_COM_FLUSH] = &cSMBRequest::processFlush;
    Dispatch[SMB_COM_SET_INFORMATION2] = &cSMBRequest::processSetInformation2;
    Dispatch[SMB_COM_QUERY_INFORMATION2] = &cSMBRequest::processQueryInformation2;
    Dispatch[SMB_COM_NT_CANCEL] = &cSMBRequest::processNTCancel;
    Dispatch[SMB_COM_CHECK_DIRECTORY] = &cSMBRequest::processCheckDirectory;
    Dispatch[SMB_COM_SET_INFORMATION] = &cSMBRequest::processSetInformation;
    Dispatch[SMB_COM_TRANSACTION] = &cSMBRequest::processTransaction;
  }

  cActivityScope as1(activity,kGetDispatchCommand);
  return (this->*Dispatch[command])(pResponse);
}

tResponseList cSMBRequest::processAndX(cPtr<cSMBResponse> pResponse) const
{
  PGENERIC_ANDX pRequestAndX = getAndX();
  PGENERIC_ANDX pResponseAndX = pResponse->getAndX();
  pResponseAndX->AndXCommand = pRequestAndX->AndXCommand;
  pResponseAndX->AndXReserved = 0;
  pResponseAndX->AndXOffset = static_cast<USHORT>(pResponse->size());

  if(SMB_COM_NO_ANDX_COMMAND == pRequestAndX->AndXCommand)
  {
    tResponseList retList;
    retList.push_back(pResponse);
    return retList;
  }

  pResponse->pushBlock();
  nextBlock();
  return dispatchCommand(pRequestAndX->AndXCommand,pResponse);
}


///////////////////////////////////////////////////////////////////////////////////////
// Command Handlers ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

tResponseList cSMBRequest::processUnhandled(cPtr<cSMBResponse> pResponse) const
{
  return errorResponse(pResponse, SMB_ERR_CLASS_DOS, SMB_ERR_BAD_FUNCTION);
}

tResponseList cSMBRequest::processNegotiate(cPtr<cSMBResponse> pResponse) const
{
  const char* SUPPORTED_DIALECT_STRINGS_SMB2[] = {SMB2_PROTOCOL_02_FF, SMB2_PROTOCOL_02_02};//When offering SMB2, ONLY offer SMB2 - useful for QTube.
  const char* SUPPORTED_DIALECT_STRINGS_SMB1[1] = {NTLANMAN};
  const char** SUPPORTED_DIALECT_STRINGS = SUPPORTED_DIALECT_STRINGS_SMB1;
  if(useSMB2())
    SUPPORTED_DIALECT_STRINGS = SUPPORTED_DIALECT_STRINGS_SMB2;

  const USHORT lenDIALECTS = sizeof(SUPPORTED_DIALECT_STRINGS) / sizeof(SUPPORTED_DIALECT_STRINGS[0]);
  for (USHORT i=0; i<lenDIALECTS; ++i)
  {
    USHORT count = 0;
    USHORT offset = sizeof(USHORT); // size of ByteCount field
    USHORT dialect_index = USHRT_MAX;
    offset += getAlignment<WORD>(getData(0), reinterpret_cast<PUCHAR>(Header));
    while(offset < numData())
    {
      char* dialect = (char*)getData(offset);
      //QTRACE((L"%d:%S",count,dialect));
      if (!strcmp(dialect, SUPPORTED_DIALECT_STRINGS[i])) 
      {
        dialect_index = i;
        break;
      }
      ++count;
      offset += static_cast<USHORT>(strlen(dialect)) + 1 + sizeof(unsigned char);
    }

    if (USHRT_MAX != dialect_index && useSMB2())
    {
      //found SMB2 so get out of here and return an SMB2 response
      cSMB2Request request(m_pContextPacket, m_pShareManager, m_pOpLockManager, m_pQCIFSPacketProcessor);
      tResponseList respList;
      cPtr<cMemoryView> pMem = new cMemoryView(new cMemory((size_t)gkSMB2BufferSize*2, cMemory::eVirtual));
      request.getResponses(pMem, respList, getActivity());
      return respList;
    }
    else if(!useSMB2())
    {
      if (offset < numData() && !useSMB2())
      {
        #ifdef DFS
            const bool bDFS(true);// = m_pServer->getReferralServer();
        #else //!DFS
            const bool bDFS(false);// = m_pServer->getReferralServer();
        #endif
        PRESP_NT_NEGOTIATE pResp = (PRESP_NT_NEGOTIATE)pResponse->allocParams(static_cast<USHORT>(sizeof(RESP_NT_NEGOTIATE)));

        pResp->DialectIndex = count;
        pResp->SecurityMode = NEGOTIATE_USER_SECURITY | NEGOTIATE_ENCRYPT_PASSWORDS;
        pResp->MaxMpxCount = 50;//(USHORT)min(kNumThreads, 0xFFFF);
        pResp->MaxNumberVcs = 1; // pulled 64 out of AFS...but ???
        pResp->MaxBufferSize = 0xffff;// + 0x10000;//61440+64;// needs to be 4 bytes shorter than max packet size because of NetBios header
        pResp->MaxRawSize = 65536;//0; // do not support raw
        pResp->SessionKey = 0; // ??
        pResp->Capabilities = CAP_RAW_MODE | CAP_UNICODE | CAP_NT_SMBS
                              | CAP_LARGE_FILES | CAP_NT_STATUS | CAP_NT_FIND
                              | CAP_LOCK_AND_READ | CAP_LEVEL_II_OPLOCKS
  #ifdef JC_EXPERIMENTAL_RPC_TEST
                              | CAP_RPC_REMOTE_APIS
  #endif
                              | CAP_LARGE_READX // | CAP_LARGE_WRITEX
                              | (bDFS ? CAP_DFS : 0);
                              //| CAP_UNIX;
        SYSTEMTIME st;
        GetSystemTime(&st);
        FILETIME ft;
        SystemTimeToFileTime(&st, &ft);
        pResp->SystemTimeLow = ft.dwLowDateTime;
        pResp->SystemTimeHigh = ft.dwHighDateTime;
        pResp->ServerTimeZone = 0; // ??

        std::string encyKey("MY_KEY_8");
        pResp->EncryptionKeyLength = 8;
        pResp->ByteCount = pResp->EncryptionKeyLength;
        memcpy(pResp->Buffer, encyKey.c_str(), pResp->EncryptionKeyLength);
        
        //if (pDFSReferral.isValid())
        //  pDFSReferral->resetReferralBase();

        return singleResponse(pResponse);
      }
      else if (dialect_index < lenDIALECTS)
      {
        PRESP_OLD_NEGOTIATE pResp = (PRESP_OLD_NEGOTIATE)pResponse->allocParams(sizeof(RESP_OLD_NEGOTIATE));
        pResp->DialectIndex = 0xFFFF;
      }
    }
  }

  return singleResponse(pResponse);
}

tResponseList cSMBRequest::processSessionSetupAndX(cPtr<cSMBResponse> pResponse) const
{
  //QTRACE((L"Set up session..."));

  try
  {
    pResponse = handleNTSession(pResponse);
  }
  catch (const exception&)
  {
    pResponse = handleSession(pResponse);
  }

  return processAndX(pResponse);
}

cPtr<cSMBResponse> cSMBRequest::handleNTSession(cPtr<cSMBResponse> pResponse) const
{
  // read request paramaters
  PREQ_NT_SESSION_SETUP_ANDX pReq = (PREQ_NT_SESSION_SETUP_ANDX)getParams(sizeof(REQ_NT_SESSION_SETUP_ANDX));

  // read request buffer

  // get passwords
  PUCHAR pBufferData = getData(sizeof(USHORT));

  std::string sANSIpw;
  std::string sUNICODEpw;
  if (pReq->CaseInsensitivePasswordLength != 0 && strlen((LPCSTR)pBufferData)) // ANSI password is present
  {
    sANSIpw.assign((LPCSTR)pBufferData, pReq->CaseInsensitivePasswordLength);
    //QTRACE((L"ANSI Password: %S {%d}", sANSIpw.c_str(), pReq->CaseInsensitivePasswordLength));
  }
  pBufferData += pReq->CaseInsensitivePasswordLength;

  if (pReq->CaseSensitivePasswordLength != 0 && strlen((LPCSTR)pBufferData)) // UNICODE password is present
  {
    sUNICODEpw.assign((LPCSTR)pBufferData, pReq->CaseSensitivePasswordLength);
    //QTRACE((L"UNICODE Password: %S {%d}", sUNICODEpw.c_str(), pReq->CaseSensitivePasswordLength));
  }
  pBufferData += pReq->CaseSensitivePasswordLength;

  // account name, primary domain, native OS and Native LAN manager
  // switch to wide-chars
  String sAccount;
  if (SMB_FLAGS2_UNICODE == (pResponse->Header->Flags2 & SMB_FLAGS2_UNICODE))
  {
    align<WCHAR>(pBufferData, reinterpret_cast<PUCHAR>(pReq));
    getUserStrings<String, PWCHAR>(sAccount, reinterpret_cast<PWCHAR>(pBufferData), L"%s");
  }
  else
    getUserStrings<std::string, PCHAR>(sAccount, reinterpret_cast<PCHAR>(pBufferData), L"%S");

  // set response parameters
  PRESP_SESSION_SETUP_ANDX pResp = (PRESP_SESSION_SETUP_ANDX)pResponse->allocParams(sizeof(RESP_SESSION_SETUP_ANDX));

  pResp->WordCount = 3;
  pResp->Action = 0;
  pResp->ByteCount = 0;

  {
    cLockGuard lg(&gSessionLock);
    // make sure we don't return a session ID of 0
    if (sessID == USHRT_MAX)
      sessID = 1;
    pResponse->Header->Uid = sessID++;

    // create a new session
    gSessionMap[pResponse->Header->Uid] = new cSMBSession(sAccount, sANSIpw, sUNICODEpw, m_pOpLockManager);
  }

  return pResponse;
}

cPtr<cSMBResponse> cSMBRequest::handleSession(cPtr<cSMBResponse> pResponse) const
{
  //PREQ_SESSION_SETUP_ANDX pReq = (PREQ_SESSION_SETUP_ANDX)getParams(sizeof(REQ_SESSION_SETUP_ANDX));
  getParams(sizeof(REQ_SESSION_SETUP_ANDX));

  // read request buffer
  // get passwords
  //PUCHAR pBufferData = getData(sizeof(USHORT));
  getData(sizeof(USHORT));

  // set response parameters
  PRESP_SESSION_SETUP_ANDX pResp = (PRESP_SESSION_SETUP_ANDX)pResponse->allocParams(sizeof(RESP_SESSION_SETUP_ANDX));

  pResp->WordCount = 4;
  pResp->Action = 0;

  //cifs header doesn't define security blob length field
  DWORD secBlobSize(304);
  pResponse->addSetupData(reinterpret_cast<PUCHAR>(&secBlobSize), 1);
  PUSHORT pData = reinterpret_cast<PUSHORT>(pResponse->getData(0));
  static const SHORT sesSecBlobLength(378);
  *pData = sesSecBlobLength;
  pData++;

  {
    cLockGuard lg(&gSessionLock);
    // make sure we don't return a session ID of 0
    if (sessID == USHRT_MAX)
      sessID = 1;
    pResponse->Header->Uid = sessID++;

    // create a new session
    const std::string sKereberosLogin("kerberos");
    const String sKereberosLoginW(widen(sKereberosLogin));
    gSessionMap[pResponse->Header->Uid] = new cSMBSession(sKereberosLoginW
      , sKereberosLogin
      , sKereberosLogin
      , m_pOpLockManager);
  }

  return pResponse;
}

tResponseList cSMBRequest::processLogoffAndX(cPtr<cSMBResponse> pResponse) const
{
  PREQ_LOGOFF_ANDX pReq = (PREQ_LOGOFF_ANDX)getParams(sizeof(REQ_LOGOFF_ANDX));

  // close the session
  cPtr<cSMBSession> pSession = getSession(Header->Uid, pResponse);
  if (pSession.isValid())
  {
    {
      cLockGuard lg(&gSessionLock);
      gSessionMap.erase(Header->Uid);
    }

    if (m_pShareManager.isValid())
      m_pShareManager->forgetListener(pSession);

    // set the reply header for this command
    PRESP_LOGOFF_ANDX pResp = (PRESP_LOGOFF_ANDX)pResponse->allocParams(sizeof(RESP_LOGOFF_ANDX));

    pResp->WordCount = 2;

    if (pReq->AndXCommand == SMB_COM_SESSION_SETUP_ANDX)
    {
      // session setup is the only valid ANDX Command for log off [#1] p.55
      return processAndX(pResponse);
    }

    pResp->AndXCommand = 0xff;
    pResp->AndXReserved = 0;
    pResp->ByteCount = 0;
    pResp->AndXOffset = static_cast<USHORT>(pResponse->size());
  }

  return singleResponse(pResponse);
}

tResponseList cSMBRequest::processTreeConnectAndX(cPtr<cSMBResponse> pResponse) const
{
  //read request paramaters
  PREQ_TREE_CONNECT_ANDX pReq = (PREQ_TREE_CONNECT_ANDX)getParams(sizeof(REQ_TREE_CONNECT_ANDX));

  USHORT offset = sizeof(USHORT); // size of ByteCount field

  PUCHAR pBufferData = getData(offset);
  //pBufferData += wordAlign(pBufferData, reinterpret_cast<PUCHAR>(Header));

  if (pReq->PasswordLength > 0)
  {
    PUCHAR pPWBuffer = new UCHAR[pReq->PasswordLength+1];
    memcpy(pPWBuffer, pBufferData, pReq->PasswordLength);  // use memcpy not strcpy
    pPWBuffer[pReq->PasswordLength] = '\0';
    //QTRACE((L"Password: %s", pPWBuffer));
    delete[] pPWBuffer;
  }

  String szPath;
  std::string szService;
  bool isUnicode = Header->Flags2 & SMB_FLAGS2_UNICODE;
  if(!isUnicode)
  {
    PCHAR pBufferStrings = (PCHAR)(pBufferData+pReq->PasswordLength);
    szPath = widen(readSMBBufferString<std::string, PCHAR>(pBufferStrings));
    szService = (char *)getData(static_cast<USHORT>((PUCHAR)(pBufferStrings) - getData(0)));
  }
  else
  {
    wchar_t* pBufferStrings = (wchar_t*)(pBufferData+pReq->PasswordLength);//SNIA 2009 JCJCJC this might not be WORD aligned - Open Solaris - Gorgon Ross
    //const UCHAR wordAlign = static_cast<UCHAR>((reinterpret_cast<PUCHAR>(&(pReq->Buffer)) - pHeader) % sizeof(USHORT));
    //if(((PCHAR)(pBufferStrings) & 1) == 1)
      //++pBufferStrings;//Aligned strings in Unicode
    szPath = readSMBBufferString<String, wchar_t*>(pBufferStrings);//SNIA 2009 JCJCJC this might be ASCII!!! - SwiftTest - Peter Murray.
    szService = (char *)getData(static_cast<USHORT>((PUCHAR)(pBufferStrings) - getData(0)));  // service never in UNICODE
  }
  //QTRACE((L"Tree Connect Path: %s", szPath.c_str()));
  //QTRACE((L"Tree Connect Service: %s", szService.c_str()));

  size_t pos = szPath.find_last_of(L"\\");
  const String sShareName(toUpper(szPath.substr(static_cast<USHORT>(pos+1))));

  static const String sIPC(L"IPC$");
  bool isIPC = (0 == sShareName.compare(sIPC));
  if (isIPC)
  {
    try
    {
      if (!m_pShareManager->HasShare(sIPC))
        return errorResponse(pResponse, SMB_ERR_CLASS_DOS, IPC_NOT_SUPPORTED);
    }
    catch (const cNotFound&)
    {
      return errorResponse(pResponse, SMB_ERR_CLASS_DOS, static_cast<USHORT>(IPC_NOT_SUPPORTED));
    }
  }

  //The response for clients that are LAN Manager 2.1 or better
  PRESP_21_TREE_CONNECT_ANDX pResp
    = (PRESP_21_TREE_CONNECT_ANDX)pResponse->allocParams(sizeof(RESP_21_TREE_CONNECT_ANDX));

  // set response parameters
  pResp->WordCount = 3;

  // connect a tree to the corresponding session
  const cPtr<cSMBSession> pSMBSession = getSession(pResponse->Header->Uid, pResponse);
  if (!pSMBSession.isValid())
    return singleResponse(pResponse);

  const cPtr<iTreeResourceFactory> pTreeResourceFactory(m_pShareManager->Get(sShareName, pSMBSession));
  if (pTreeResourceFactory.isNull())
    return errorResponse(pResponse, SMB_ERR_CLASS_SERVER, SMB_ERR_BAD_NET_NAME);

  pResponse->Header->Tid = pSMBSession->ConnectTree(sShareName, pTreeResourceFactory);

  // set response buffer
#ifdef DFS
  const bool bDFS(true);// = m_pServer->getReferralServer();
#else //!DFS
  const bool bDFS(false);// = m_pServer->getReferralServer();
#endif
  pResp->OptionalSupport = SMB_SUPPORT_SEARCH_BITS | (bDFS ? SMB_SHARE_IS_IN_DFS : 0);
  //pResp->OptionalSupport = SMB_SUPPORT_SEARCH_BITS | SMB_SHARE_IS_IN_DFS;

  offset = sizeof(USHORT); // size of ByteCount field
  PUCHAR bufferPtr = pResponse->getData(offset);
  if(isIPC)
  {
    strcpy(reinterpret_cast<char*>(bufferPtr), SHARE_TYPE_NAME_PIPE);  // disk system
    pResp->ByteCount = static_cast<USHORT>(strlen(SHARE_TYPE_NAME_PIPE) + 1); // +1 for string terminator
  }
  else
  {
    strcpy(reinterpret_cast<char*>(bufferPtr), SHARE_TYPE_NAME_DISK);  // disk system
    pResp->ByteCount = static_cast<USHORT>(strlen(SHARE_TYPE_NAME_DISK) + 1); // +1 for string terminator
  }

  bufferPtr = pResponse->getData(pResp->ByteCount+offset);
  const PUCHAR unaligned(bufferPtr);
  align<WORD>(bufferPtr, reinterpret_cast<PUCHAR>(pResponse->Header));
  pResp->ByteCount += bufferPtr - unaligned;
  if(isIPC)
  {
    wcscpy(reinterpret_cast<PWCHAR>(bufferPtr), L""); // disk system type
  }
  else
  {
    wcscpy(reinterpret_cast<PWCHAR>(bufferPtr), L"QCIFS"); // disk system type
  }
  pResp->ByteCount += (USHORT)wcslen(reinterpret_cast<PWCHAR>(bufferPtr))*sizeof(WCHAR);

  return processAndX(pResponse);
}

tResponseList cSMBRequest::processTreeDisconnect(cPtr<cSMBResponse> pResponse) const
{
  // Don't need to read parameters, all the info we need is in the header
  cPtr<cSMBSession> pSession = getSession(Header->Uid, pResponse);
  if (pSession.isValid())
  {
    pSession->DisconnectTree(Header->Tid);
    PRESP_CLOSE_AND_TREE_DISC pResp = (PRESP_CLOSE_AND_TREE_DISC)pResponse->allocParams(4);
    pResp->WordCount = 0;
    pResp->ByteCount = 0;
  }

  return singleResponse(pResponse);
}

tResponseList cSMBRequest::processOpenAndX(cPtr<cSMBResponse> pResponse) const
{
  PREQ_OPEN_ANDX pReq = (PREQ_OPEN_ANDX)getParams(static_cast<USHORT>(sizeof(REQ_OPEN_ANDX)));

  String sPath(reinterpret_cast<PWCHAR>(pReq->Buffer + getAlignment<WORD>(pReq->Buffer, reinterpret_cast<PUCHAR>(Header))));
  m_pQCIFSPacketProcessor->addCreateFileData(Header->Uid, sPath, 0);
  if (pResponse->Header->Flags2 & SMB_FLAGS2_DFS)
  {
    try
    {
      sPath =  iQCIFSIPC::singleton().removeDFSPart(sPath);
    }
    catch (const cNotFound&)
    {
    }
  }

  m_pOpLockManager->Check(sPath, *this);

  const cPtr<cSMBSession> pSMBSession(getSession(Header->Uid, pResponse));
  if (!pSMBSession.isValid())
    return singleResponse(pResponse);

  pSMBSession->processOpenAndX(pReq, reinterpret_cast<PUCHAR>(Header), pResponse);

  return processAndX(pResponse);
}

tResponseList cSMBRequest::processEcho(cPtr<cSMBResponse> pResponse) const
{
  const PREQ_ECHO pReq = (PREQ_ECHO)getParams(sizeof(REQ_ECHO));

  tResponseList retList;

  //use existing response object for the first response
  const PRESP_ECHO pResp = (PRESP_ECHO)pResponse->allocParams(sizeof(RESP_ECHO));
  pResp->WordCount = 1;
  pResp->SequenceNumber = 0;
  pResp->ByteCount = pReq->ByteCount;
  memcpy(pResp->Buffer, pReq->Buffer, pReq->ByteCount);
  retList.push_back(pResponse);

  for (USHORT i = 1; i<pReq->EchoCount; i++)
  {
    //create new responses for any extra exho requests
    const cPtr<cSMBResponse> pNextResponse = new cSMBResponse(*this
      , new cMemoryView(new cMemory((size_t)gkBufferSize, cMemory::eVirtual), 0, checked_static_cast<size_t>(gkBufferSize)));
    const PRESP_ECHO pNextResp = (PRESP_ECHO)pNextResponse->allocParams(sizeof(RESP_ECHO));
    pNextResp->WordCount = 1;
    pNextResp->SequenceNumber = i;
    pNextResp->ByteCount = pReq->ByteCount;
    memcpy(pNextResp->Buffer, pNextResp->Buffer, pNextResp->ByteCount);
    retList.push_back(pNextResponse);
  }

  return retList;
}

tResponseList cSMBRequest::processNTCreateAndX(cPtr<cSMBResponse> pResponse) const
{
  //cMicroTimerTor(L"processNTCreateAndX");
  PREQ_NT_CREATE_ANDX pReq = (PREQ_NT_CREATE_ANDX)getParams(sizeof(REQ_NT_CREATE_ANDX));

  String sPath(reinterpret_cast<PWCHAR>(pReq->Buffer + getAlignment<WORD>(pReq->Buffer, reinterpret_cast<PUCHAR>(Header))));
  m_pQCIFSPacketProcessor->addCreateFileData(Header->Uid, sPath, 0);
  if (pResponse->Header->Flags2 & SMB_FLAGS2_DFS)
  {
    try
    {
      sPath =  iQCIFSIPC::singleton().removeDFSPart(sPath);
    }
    catch (const cNotFound&)
    {
    }
  }

  m_pOpLockManager->Check(sPath, *this);

  cPtr<cSMBSession> pSMBSession = getSession(Header->Uid, pResponse);
  if (!pSMBSession.isValid())
    return singleResponse(pResponse);

  pSMBSession->processNTCreateAndX(pReq, reinterpret_cast<PUCHAR>(Header), pResponse);

  // if no error was set, store the oplock
  if (pResponse->Header->ErrorClass == 0)
  {
    //PRESP_NT_CREATE_ANDX pResp = reinterpret_cast<PRESP_NT_CREATE_ANDX>(pResponse->getParams(sizeof(RESP_NT_CREATE_ANDX)));
    //TODO properly sort out oplocks with a callback
    cPtr<cOpLock> pOpLock;// = m_pServer->SetOpLock(sPath, Header, pResp->Fid, m_socket);

    if (pOpLock.isValid())
    {
      pSMBSession->AddOplock(pOpLock);
    }
  }
  else
    return singleResponse(pResponse);

  return processAndX(pResponse);
}

tResponseList cSMBRequest::processTransaction2(cPtr<cSMBResponse> pResponse) const
{
  PREQ_TRANSACTION pReq = (PREQ_TRANSACTION)getParams(sizeof(REQ_TRANSACTION));

  cPtr<cSMBSession>pSession = getSession(Header->Uid, pResponse);
  if (pSession.isValid())
  {
    if(pReq->ParameterCount == 0x13 && !pReq->DataCount)
    {
      String NamedPipe = (wchar_t*)&(pReq->Buffer[3]);
      if(L"\\PIPE\\LANMAN" == NamedPipe)
      {
        //JCJCJC to do ....
      }
    }

    if (pReq->TotalParameterCount == pReq->ParameterCount
      && pReq->TotalDataCount == pReq->DataCount)
    {
      // we have received all the data in a single packet so service the response
      // add USHORT to param size, since RESP_TRANSACTION doesn't define ByteCount field
      PRESP_TRANSACTION pResp
        = (PRESP_TRANSACTION)pResponse->allocParams(sizeof(RESP_TRANSACTION) + sizeof(USHORT));

      pResponse = pSession->processTrans2SubCommand(pReq->MaxDataCount
        , reinterpret_cast<const PUCHAR>(pReq->Buffer)
        , pReq->SetupCount
        , reinterpret_cast<const PUCHAR>(Header) + pReq->ParameterOffset
        , reinterpret_cast<const PUCHAR>(Header) + pReq->DataOffset
        , pResponse);

      pResp->WordCount = 10 + pResponse->getSetupCount();
      pResp->TotalParameterCount = static_cast<USHORT>(pResponse->getTotalParamCount());
      pResp->TotalDataCount = static_cast<USHORT>(pResponse->getTotalDataCount());
      pResp->Reserved = 0;
      pResp->ParameterCount = static_cast<USHORT>(pResponse->getParamCount());
      pResp->ParameterOffset = static_cast<USHORT>(pResponse->getParamOffset());
      pResp->ParameterDisplacement = static_cast<USHORT>(pResponse->getParamDisplacement());
      pResp->DataCount = static_cast<USHORT>(pResponse->getDataCount());
      pResp->DataOffset = static_cast<USHORT>(pResponse->getDataOffset());
      pResp->DataDisplacement = static_cast<USHORT>(pResponse->getDataDisplacement());
      pResp->SetupCount = 0;
      pResp->Reserved2 = 0;
    }
    else
    {
      PRESP_TRANSACTION_INTERIM pResp
        = (PRESP_TRANSACTION_INTERIM)pResponse->allocParams(sizeof(RESP_TRANSACTION_INTERIM));
      pResp->WordCount = 0;
      pResp->ByteCount = 0;
      pResponse->setDosError(SMB_ERR_CLASS_SERVER, SMB_ERR_ERROR);

      // RHRHRH - to do: buffer the requested parameters/data received so far
      //                  then 
    }
  }

  return singleResponse(pResponse);
}

tResponseList cSMBRequest::processTransaction2Secondary(cPtr<cSMBResponse> pResponse) const
{
  // send a reply to say we are ready to receive the secondary transaction request
  //PREQ_TRANSACTION_SECONDARY pReq
  //  = (PREQ_TRANSACTION_SECONDARY)getParams(sizeof(REQ_TRANSACTION_SECONDARY) + sizeof(USHORT));
  getParams(sizeof(REQ_TRANSACTION_SECONDARY) + sizeof(USHORT));

  cPtr<cSMBSession>pSession = getSession(Header->Uid, pResponse);
  if (pSession.isValid())
  {
    //PRESP_TRANSACTION pResp = (PRESP_TRANSACTION)pResponse->allocParams(sizeof(RESP_TRANSACTION));
    pResponse->allocParams(sizeof(RESP_TRANSACTION));

    pResponse->setDosError(SMB_ERR_CLASS_SERVER, SMB_ERR_ERROR);
  }

  return singleResponse(pResponse);
}

tResponseList cSMBRequest::processClose(cPtr<cSMBResponse> pResponse) const
{
  //cMicroTimerTor(L"processClose");
  cActivityScope as1(ActivityQCIFSProcessors.find(1)->second,kProcessClose);
  PREQ_CLOSE pReq
    = (PREQ_CLOSE)getParams(sizeof(REQ_CLOSE));

  cPtr<cSMBSession>pSession = getSession(Header->Uid, pResponse);
  if (pSession.isValid())
  {
    PRESP_CLOSE pResp = (PRESP_CLOSE)pResponse->allocParams(sizeof(RESP_CLOSE));
    pResp->ByteCount = 0;
    pResp->WordCount = 0;

    pResponse = pSession->Close(pReq->Fid, pResponse);
    m_pOpLockManager->Release(Header, pReq->Fid);
  }

  return singleResponse(pResponse);
}

tResponseList cSMBRequest::processNtTrans(cPtr<cSMBResponse> pResponse) const
{
  PREQ_NT_TRANSACTION pReq
    = (PREQ_NT_TRANSACTION)getParams(sizeof(REQ_NT_TRANSACTION));

  cPtr<cSMBSession>pSession = getSession(Header->Uid, pResponse);
  if (pSession.isValid())
  {
    if (pReq->TotalParameterCount == pReq->ParameterCount
      && pReq->TotalDataCount == pReq->DataCount)
    {
      // we have received all the data in a single packet so service the response
      // add USHORT to param size, since RESP_TRANSACTION doesn't define ByteCount field
      PRESP_NT_TRANSACTION pResp
        = (PRESP_NT_TRANSACTION)pResponse->allocParams(sizeof(RESP_NT_TRANSACTION) + sizeof(USHORT));

      pResponse = pSession->processNTTransSubCommand(pReq->Function
        , reinterpret_cast<const PUCHAR>(pReq->Buffer)
        , pReq->SetupCount
        , reinterpret_cast<const PUCHAR>(Header) + pReq->ParameterOffset
        , reinterpret_cast<const PUCHAR>(Header) + pReq->DataOffset
        , pResponse);

      if (pResponse.isValid())
      {
        pResp->WordCount = 18 + pResponse->getSetupCount();
        pResp->TotalParameterCount = pResponse->getTotalParamCount();
        pResp->TotalDataCount = pResponse->getTotalDataCount();
        pResp->ParameterCount = pResponse->getParamCount();
        pResp->ParameterOffset = pResponse->getParamOffset();
        pResp->ParameterDisplacement = pResponse->getParamDisplacement();
        pResp->DataCount = pResponse->getDataCount();
        pResp->DataOffset = pResponse->getDataOffset();
        pResp->DataDisplacement = pResponse->getDataDisplacement();
        pResp->SetupCount = pResponse->getSetupCount();
        pResp->Reserved1 = 0;
        pResp->Reserved2 = 0;
      }
    }
    else
    {
      PRESP_NT_TRANSACTION_INTERIM pResp
        = (PRESP_NT_TRANSACTION_INTERIM)pResponse->allocParams(sizeof(RESP_NT_TRANSACTION_INTERIM));
      pResp->WordCount = 0;
      pResp->ByteCount = 0;
      pResponse->setDosError(SMB_ERR_CLASS_SERVER, SMB_ERR_ERROR);

      // RHRHRH - to do: buffer the requested parameters/data received so far
    }
  }

  return singleResponse(pResponse);
}

tResponseList cSMBRequest::processReadAndX(cPtr<cSMBResponse> pResponse) const
{
  PREQ_READ_ANDX pReq
    = (PREQ_READ_ANDX)getParams(sizeof(REQ_READ_ANDX));

  //QTRACE((L"cSMBRequest::processReadAndX() - start [%d]", pReq->Fid));

  cPtr<cSMBSession>pSession = getSession(Header->Uid, pResponse);
  if (!pSession.isValid())
    return singleResponse(pResponse);

  // assign response before processing AndX since we won't know some of the values
  // but need to reserve enough space in the reply buffer
  PRESP_READ_ANDX pResp = (PRESP_READ_ANDX)pResponse->allocParams(sizeof(RESP_READ_ANDX));

  try
  {
    pSession->processReadAndX(pReq, pResponse);
  }
  catch(cRecoverable& err)
  {
    QSOS((L"cSMBRequest::processReadAndX %s",err.getMsg().c_str()));
    pResponse->setNTError(STATUS_END_OF_FILE);

    pResp->WordCount = 12;
    pResp->Remaining = static_cast<USHORT>(-1);
    pResp->DataCompactionMode = 0;
    pResp->Reserved = 0;
    pResp->DataLength = static_cast<USHORT>(pResponse->getDataCount());
    pResp->DataOffset = static_cast<USHORT>(pResponse->getDataOffset());
    pResp->DataLengthHigh = static_cast<USHORT>((pResponse->getDataCount() & 0xff00) >> 16);
    pResp->Reserved2 = 0;
    pResp->Reserved3[0] = 0;
    pResp->Reserved3[1] = 0;

    return singleResponse(pResponse);
  }

  pResp->WordCount = 12;
  pResp->Remaining = static_cast<USHORT>(-1);
  pResp->DataCompactionMode = 0;
  pResp->Reserved = 0;
  pResp->DataLength = static_cast<USHORT>(pResponse->getDataCount());
  pResp->DataOffset = static_cast<USHORT>(pResponse->getDataOffset());
  pResp->DataLengthHigh = static_cast<USHORT>((pResponse->getDataCount() & 0xff00) >> 16);
  pResp->Reserved2 = 0;
  pResp->Reserved3[0] = 0;
  pResp->Reserved3[1] = 0;

  //QTRACE((L"cSMBRequest::processReadAndX() - end [%d]", pReq->Fid));
  return processAndX(pResponse);
}

tResponseList cSMBRequest::processRename(cPtr<cSMBResponse> pResponse) const
{
  PREQ_RENAME pReq = (PREQ_RENAME)getParams(sizeof(REQ_RENAME));

  cPtr<cSMBSession>pSession = getSession(Header->Uid, pResponse);
  if (pSession.isValid())
  {
    PRESP_RENAME pResp = (PRESP_RENAME)pResponse->allocParams(sizeof(RESP_RENAME));
    
    pResponse = pSession->Rename(pReq, pResponse);

    pResp->WordCount = 0;
    pResp->ByteCount = 0;
  }

  return singleResponse(pResponse);
}

tResponseList cSMBRequest::processDelete(cPtr<cSMBResponse> pResponse) const
{
  PREQ_DELETE pReq = (PREQ_DELETE)getParams(sizeof(REQ_DELETE));

  cPtr<cSMBSession>pSession = getSession(Header->Uid, pResponse);
  if (pSession.isValid())
  {
    PRESP_DELETE pResp = (PRESP_DELETE)pResponse->allocParams(sizeof(RESP_DELETE));

    pResponse = pSession->Delete(pReq, pResponse);

    pResp->WordCount = 0;
    pResp->ByteCount = 0;
  }

  return singleResponse(pResponse);
}

//RHRHRH thsi isn't working right now
tResponseList cSMBRequest::processLockingAndX(cPtr<cSMBResponse> pResponse) const
{
  PREQ_LOCKING_ANDX pReq = (PREQ_LOCKING_ANDX)getParams(sizeof(REQ_LOCKING_ANDX));

  if (pReq->LockType & LOCKING_ANDX_OPLOCK_RELEASE)
  {
    m_pOpLockManager->Release(Header, pReq->Fid);
  }
  else
  {
    cPtr<cSMBSession>pSession = getSession(Header->Uid, pResponse);
    if (pSession.isValid())
    {
      pSession->Lock(pReq, pResponse);
    }
  }

  return singleResponse(pResponse);
}

const unsigned int kSmbBufferFromNetBIOS = 4;
tResponseList cSMBRequest::processWriteAndX(cPtr<cSMBResponse> pResponse) const
{
  PREQ_NT_WRITE_ANDX pReq = (PREQ_NT_WRITE_ANDX)getParams(sizeof(REQ_NT_WRITE_ANDX));

  cPtr<cSMBSession>pSession = getSession(Header->Uid, pResponse);
  if (!pSession.isValid())
    return singleResponse(pResponse);

  PRESP_WRITE_ANDX pResp = (PRESP_WRITE_ANDX)pResponse->allocParams(sizeof(RESP_WRITE_ANDX));

  DWORD nBytes = static_cast<DWORD>((static_cast<DWORD>(pReq->DataLengthHigh) << 0x10) + pReq->DataLength);
  LARGE_INTEGER lg;
  lg.LowPart = pReq->Offset;
  lg.HighPart = pReq->OffsetHigh;
  pSession->processWrite(pReq->Fid
    , m_pContextPacket->getRXBuffer()->trimmedConst((size_t)pReq->DataOffset + kSmbBufferFromNetBIOS, 0)
    , lg
    , nBytes
    , pResponse);

  pResp->WordCount = 6;
  pResp->Count = static_cast<USHORT>(nBytes);
  pResp->CountHigh = static_cast<USHORT>((nBytes >> 0x10) & 0xffff);
  pResp->Remaining = 0;
  if (!pResp->CountHigh)
    pResp->Reserved = 0;

  return processAndX(pResponse);
}

tResponseList cSMBRequest::processWrite(cPtr<cSMBResponse> pResponse) const
{
  PREQ_WRITE pReq = (PREQ_WRITE)getParams(5);

  cPtr<cSMBSession>pSession = getSession(Header->Uid, pResponse);
  if (pSession.isValid())
  {
    PRESP_WRITE pResp = (PRESP_WRITE)pResponse->allocParams(sizeof(RESP_WRITE));
    DWORD nBytes = pReq->Count;
    LARGE_INTEGER lg;
    lg.LowPart = pReq->Offset;
    lg.HighPart = 0;

    pSession->processWrite(pReq->Fid
      , m_pContextPacket->getRXBuffer()->trimmedConst(((char*)(pReq->Buffer) - (char*)(Header)) + (size_t)kSmbBufferFromNetBIOS, 0)
      , lg
      , nBytes
      , pResponse);

    pResp->WordCount = 1;
    pResp->Count = static_cast<USHORT>(nBytes);
    pResp->ByteCount = 0;
  }

  return singleResponse(pResponse);
}

tResponseList cSMBRequest::processDeleteDirectory(cPtr<cSMBResponse> pResponse) const
{
  PREQ_DELETE_DIRECTORY pReq = (PREQ_DELETE_DIRECTORY)getParams(sizeof(REQ_DELETE_DIRECTORY));

  cPtr<cSMBSession> pSession = getSession(Header->Uid, pResponse);
  if (pSession.isValid())
  {
    PRESP_DELETE_DIRECTORY pResp = (PRESP_DELETE_DIRECTORY)pResponse->allocParams(sizeof(RESP_DELETE_DIRECTORY));

    PUCHAR pBuffer = pReq->Buffer;

    pBuffer++;
    const std::wstring sPath(reinterpret_cast<PWCHAR>(pBuffer));
    pResponse = pSession->DeleteDirectory(sPath, pResponse);

    pResp->ByteCount = 0;
    pResp->WordCount = 0;
  }

  return singleResponse(pResponse);
}

tResponseList cSMBRequest::processFindClose2(cPtr<cSMBResponse> pResponse) const
{
  PREQ_FIND_CLOSE2 pReq = (PREQ_FIND_CLOSE2)getParams(sizeof(REQ_FIND_CLOSE2));

  cPtr<cSMBSession> pSession = getSession(Header->Uid, pResponse);
  if (pSession.isValid())
  {
    pSession->closeSearch(pReq->Sid);

    PRESP_FIND_CLOSE2 pResp = (PRESP_FIND_CLOSE2)pResponse->allocParams(sizeof(RESP_FIND_CLOSE2));
    pResp->ByteCount = 0;
    pResp->WordCount = 0;
  }

  return singleResponse(pResponse);
}

tResponseList cSMBRequest::processCreateDirectory(cPtr<cSMBResponse> pResponse) const
{
  PREQ_CREATE_DIRECTORY pReq = (PREQ_CREATE_DIRECTORY)getParams(sizeof(REQ_CREATE_DIRECTORY));

  cPtr<cSMBSession> pSession = getSession(Header->Uid, pResponse);
  if (pSession.isValid())
  {
    PRESP_CREATE_DIRECTORY pResp = (PRESP_CREATE_DIRECTORY)pResponse->allocParams(sizeof(RESP_CREATE_DIRECTORY));

    PUCHAR pBuffer = pReq->Buffer;

    pBuffer++;
    const std::wstring sPath(reinterpret_cast<PWCHAR>(pBuffer));
    m_pQCIFSPacketProcessor->addCreateFileData(Header->Uid, sPath, 0);
    pResponse = pSession->CreateDirectory(sPath, pResponse);

    pResp->ByteCount = 0;
    pResp->WordCount = 0;
  }

  return singleResponse(pResponse);
}

tResponseList cSMBRequest::processProcessExit(cPtr<cSMBResponse> pResponse) const
{
  //PREQ_PROCESS_EXIT pReq = (PREQ_PROCESS_EXIT)getParams(sizeof(REQ_PROCESS_EXIT));
  getParams(sizeof(REQ_PROCESS_EXIT));

  cPtr<cSMBSession> pSession = getSession(Header->Uid, pResponse);
  if (pSession.isValid())
  {
    PRESP_PROCESS_EXIT pResp = (PRESP_PROCESS_EXIT)pResponse->allocParams(sizeof(RESP_PROCESS_EXIT));

    //pResponse = pSession->ProcessExit(pResponse);

    pResp->ByteCount = 0;
    pResp->WordCount = 0;
  }

  return singleResponse(pResponse);
}

tResponseList cSMBRequest::processFlush(cPtr<cSMBResponse> pResponse) const
{
  //PREQ_FLUSH pReq = (PREQ_FLUSH)getParams(sizeof(REQ_FLUSH));
  getParams(sizeof(REQ_FLUSH));

  cPtr<cSMBSession> pSession = getSession(Header->Uid, pResponse);
  if (pSession.isValid())
  {
    PRESP_FLUSH pResp = (PRESP_FLUSH)pResponse->allocParams(sizeof(RESP_FLUSH));

    //pResponse = pSession->Flush(pResp->Fid, pResponse); RHRHRH ToDo

    pResp->ByteCount = 0;
    pResp->WordCount = 0;
  }

  return singleResponse(pResponse);
}

tResponseList cSMBRequest::processSetInformation(cPtr<cSMBResponse> pResponse) const
{
  PREQ_SET_INFORMATION pReq = (PREQ_SET_INFORMATION)getParams(sizeof(REQ_SET_INFORMATION));

  cPtr<cSMBSession> pSession = getSession(Header->Uid, pResponse);
  if (pSession.isValid())
  {
    PRESP_SET_INFORMATION pResp = (PRESP_SET_INFORMATION)pResponse->allocParams(sizeof(RESP_SET_INFORMATION));

    pResponse = pSession->SetInformation(pReq, pResponse);

    pResp->ByteCount = 0;
    pResp->WordCount = 0;
  }

  return singleResponse(pResponse);
}

tResponseList cSMBRequest::processSetInformation2(cPtr<cSMBResponse> pResponse) const
{
  PREQ_SET_INFORMATION2 pReq = (PREQ_SET_INFORMATION2)getParams(sizeof(REQ_SET_INFORMATION2));

  cPtr<cSMBSession> pSession = getSession(Header->Uid, pResponse);
  if (pSession.isValid())
  {
    PRESP_SET_INFORMATION2 pResp = (PRESP_SET_INFORMATION2)pResponse->allocParams(sizeof(RESP_SET_INFORMATION2));

    pResponse = pSession->SetInformation2(pReq, pResponse);

    pResp->ByteCount = 0;
    pResp->WordCount = 0;
  }

  return singleResponse(pResponse);
}

tResponseList cSMBRequest::processQueryInformation2(cPtr<cSMBResponse> pResponse) const
{
  PREQ_QUERY_INFORMATION2 pReq = (PREQ_QUERY_INFORMATION2)getParams(sizeof(REQ_QUERY_INFORMATION2));

  cPtr<cSMBSession> pSession = getSession(Header->Uid, pResponse);
  if (pSession.isValid())
  {
    pResponse = pSession->QueryInformation2(pReq, pResponse);
  }

  return singleResponse(pResponse);
}

tResponseList cSMBRequest::processNTCancel(cPtr<cSMBResponse> pResponse) const
{
  //PREQ_NT_CANCEL pReq = (PREQ_NT_CANCEL)getParams(sizeof(REQ_NT_CANCEL));
  getParams(sizeof(REQ_NT_CANCEL));

  cPtr<cSMBSession> pSession = getSession(Header->Uid, pResponse);
  if (pSession.isValid())
  {
    PRESP_NT_CANCEL pResp = (PRESP_NT_CANCEL)pResponse->allocParams(sizeof(RESP_NT_CANCEL));

    pResponse = pSession->NTCancel(pResponse);

    pResp->ByteCount = 0;
    pResp->WordCount = 0;
  }

  return singleResponse(pResponse);
}

tResponseList cSMBRequest::processCheckDirectory(cPtr<cSMBResponse> pResponse) const
{
  const PREQ_CHECK_DIRECTORY pReq = (PREQ_CHECK_DIRECTORY)getParams(sizeof(REQ_CHECK_DIRECTORY));

  cPtr<cSMBSession> pSession = getSession(Header->Uid, pResponse);
  if (pSession.isValid())
  {
    PRESP_CHECK_DIRECTORY pResp = (PRESP_CHECK_DIRECTORY)pResponse->allocParams(sizeof(RESP_CHECK_DIRECTORY));

    pResponse = pSession->CheckDirectory(pReq, pResponse);

    pResp->ByteCount = 0;
    pResp->WordCount = 0;
  }

  return singleResponse(pResponse);
}

tResponseList cSMBRequest::processTransaction(cPtr<cSMBResponse> pResponse) const
{
  const PREQ_TRANSACTION pReq = (PREQ_TRANSACTION)getParams(sizeof(REQ_TRANSACTION));

  cPtr<cSMBSession> pSession = getSession(Header->Uid, pResponse);
  if (pSession.isValid())
  {
    if (pReq->TotalParameterCount == pReq->ParameterCount
      && pReq->TotalDataCount == pReq->DataCount)
    {
      // we have received all the data in a single packet so service the response
      // add USHORT to param size, since RESP_TRANSACTION doesn't define ByteCount field
      const PRESP_TRANSACTION pResp
        = (PRESP_TRANSACTION)pResponse->allocParams(sizeof(RESP_TRANSACTION) + sizeof(USHORT));

      pResponse = pSession->processTransSubCommand(reinterpret_cast<const PUCHAR>(header())
        , reinterpret_cast<const PUCHAR>(pReq->Buffer)
        , pReq->SetupCount
        , reinterpret_cast<const PUCHAR>(Header) + pReq->ParameterOffset
        , reinterpret_cast<const PUCHAR>(Header) + pReq->DataOffset
        , reinterpret_cast<PUSHORT>(pResp->Buffer)
        , pResponse);

      if (pResponse.isValid())
      {
        pResp->WordCount = 10 + pResponse->getSetupCount();
        pResp->TotalParameterCount = pResponse->getTotalParamCount();
        pResp->TotalDataCount = pResponse->getTotalDataCount();
        pResp->ParameterCount = pResponse->getParamCount();
        pResp->ParameterOffset = pResponse->getParamOffset();
        pResp->ParameterDisplacement = pResponse->getParamDisplacement();
        pResp->DataCount = pResponse->getDataCount();
        pResp->DataOffset = pResponse->getDataOffset();
        pResp->DataDisplacement = pResponse->getDataDisplacement();
        pResp->SetupCount = pResponse->getSetupCount();
        pResp->Reserved = 0;
        pResp->Reserved2 = 0;
      }
    }
    else
    {
      PRESP_NT_TRANSACTION_INTERIM pResp
        = (PRESP_NT_TRANSACTION_INTERIM)pResponse->allocParams(sizeof(RESP_NT_TRANSACTION_INTERIM));
      pResp->WordCount = 0;
      pResp->ByteCount = 0;
      pResponse->setDosError(SMB_ERR_CLASS_SERVER, SMB_ERR_ERROR);

      // RHRHRH - to do: buffer the requested parameters/data received so far
    }
  }

  return singleResponse(pResponse);
}

cPtr<cSMBSession> cSMBRequest::getSession(const SHORT Uid, cPtr<cSMBResponse> pResponse) const
{
  {
    cLockGuard lg(&gSessionLock);
    const tSessionMap::const_iterator finder = gSessionMap.find(Uid);
    if (gSessionMap.end() != finder)
      return finder->second;
  }

  QSOS((L"Invalid Session ID"));
  pResponse->setDosError(SMB_ERR_CLASS_SERVER, SMB_ERR_BAD_UID);
  return cPtr<cSMBSession>();
}

inline tResponseList cSMBRequest::singleResponse(const vfs::cPtr<cSMBResponse> pResponse) const
{
  tResponseList retList;
  retList.push_back(pResponse);
  return retList;
}

inline tResponseList cSMBRequest::errorResponse(const vfs::cPtr<cSMBResponse> pResponse
  , const DWORD ntError) const
{
  pResponse->setNTError(ntError);
  return singleResponse(pResponse);
}

inline tResponseList cSMBRequest::errorResponse(const vfs::cPtr<cSMBResponse> pResponse
  , const UCHAR errorClass
  , const DWORD error) const
{
  pResponse->setDosError(errorClass, error);
  return singleResponse(pResponse);
}

#else

bool useSMB2()
{
  return true;
}


#endif // CIFS 