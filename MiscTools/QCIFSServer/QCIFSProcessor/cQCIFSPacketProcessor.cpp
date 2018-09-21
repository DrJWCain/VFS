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

//cQCIFSPacketProcessor.cpp

#include "StdAfx.h"
#include ".\cqcifspacketprocessor.h"
#include "cProcessPacketMsg.h"
#include "logging.h"
#include "iShareManager.h"
#include "iOpLockManager.h"
#include "cSMBResponse.h"
#include "cSMB2Response.h"
#include "iOpLockManager.h"

#include "..\Common\iIOCompletionPort.h"

using namespace vfs;
extern iModuleContext::Ptr gModuleContext;
extern tActivityMap ActivityQCIFSProcessors;

static const unsigned char SMBHeaderDelimiter[4] = {0xff, 'S', 'M', 'B'};
static const unsigned char SMB2HeaderDelimiter[4] = {0xfe, 'S', 'M', 'B'};

cQCIFSPacketProcessor::cQCIFSPacketProcessor(const cPtr<iIOCompletionPort> pCompletionPort
  , const cPtr<iShareManager> pShareManager
  , const cPtr<iOpLockManager> pOpLockManager)
: m_pCompletionPort(pCompletionPort)
, m_pShareManager(pShareManager)
, m_pOpLockManager(pOpLockManager)
, NumPacketsToKeep(20)
, NumPathsToKeep(50)
{
  try
  {
    NumPacketsToKeep = gModuleContext->getCustomSettingNumber(L"", L"NumPacketsToKeep");
  }
  catch(cRecoverable&)
  {
  }

  try
  {
    NumPathsToKeep = gModuleContext->getCustomSettingNumber(L"", L"NumPathsToKeep");
  }
  catch(cRecoverable&)
  {
  }

  QTRACE((L"cQCIFSPacketProcessor::cQCIFSPacketProcessor - NumPacketsToKeep %d, NumPathsToKeep %d", NumPacketsToKeep, NumPathsToKeep));
}

cQCIFSPacketProcessor::~cQCIFSPacketProcessor(void)
{
}

//iPacketProcessor
void cQCIFSPacketProcessor::AddPacket(const cPtr<iRXPacket> pPacket)
{
  const unsigned char* const pHeader = reinterpret_cast<const unsigned char* const>(pPacket->getRXBuffer()->getBytes());
  if(0x81 == *pHeader)//silently send NBT OK and continue ...
  {
    //QTRACE((L"cQCIFSPacketProcessor::AddPacket() - NBT session start"));
    //send NBT POSITIVE SESSION RESPONSE
    static const unsigned char NBTRespSize(4);
    cPtr<cMemoryView> pMem = new cMemoryView(new cMemory((size_t)NBTRespSize, cMemory::eVirtual));
    unsigned char* const buffer = reinterpret_cast<unsigned char* const>(pMem->getBytes());

    SMART_TPE smart_tpe;
    smart_tpe.pMem = pMem;
    ZeroMemory(buffer, NBTRespSize);
    buffer[0] = 0x82;
    smart_tpe.tpe.cLength = NBTRespSize;
    smart_tpe.tpe.dwElFlags = TP_ELEMENT_MEMORY;
    smart_tpe.tpe.pBuffer = buffer;

    tTransmitList transmitList;
    transmitList.push_back(smart_tpe);

    const cPtr<iTXPacket> pTXPacket = pPacket->getResponsePacket();
    pTXPacket->setReply(transmitList);
    pTXPacket->Send();
  }
  else if (m_pCompletionPort.isValid())
  {
    if (isSMB2Packet(pHeader+sizeof(int)))
    {
      m_pCompletionPort->PostMessage(new cProcessPacketMsg<cSMB2Request>(GetTXBufferSMB2()
        , pPacket
        , m_pShareManager
        , m_pOpLockManager
        , this));
    }
    else if (isSMBPacket(pHeader+sizeof(int)))
    {
#if CIFS 
      m_pCompletionPort->PostMessage(new cProcessPacketMsg<cSMBRequest>(GetTXBufferSMB()
        , pPacket
        , m_pShareManager
        , m_pOpLockManager
        , this));
#else 
      m_pCompletionPort->PostMessage(new cProcessPacketMsg<cSMB2Request>(GetTXBufferSMB2()
        , pPacket
        , m_pShareManager
        , m_pOpLockManager
        , this));
#endif // CIFS 
    }
    else
    {
      __debugbreak();
    }
  }
}

#define Uint8 unsigned char
#define Uint32 unsigned long
void dumpBuffer(Uint8* ptr, Uint32 lengthToPrint)
{
  const Uint8* lastLine = 0;
  Uint32 byte=0;
  Uint32 i=0;
  wchar_t line[80], *linePtr;

  linePtr = line;

  QTRACE((L"memory %p, lengthToPrint 0x%08x", ptr, lengthToPrint));

  while(i<lengthToPrint)
  {
    if(!(i & 15)) // line break every mod 16
    {
      if(lastLine)
      {
        QTRACE((L"%04x %s", i-16, line));
      }

      //printf("\n ");
      lastLine = ptr;
      linePtr = line;
      *line = 0;
    }

    wsprintf(linePtr, L"%02x ", *ptr++);
    i++; 
    byte++; 
    linePtr += 3;
  }

  if(lastLine)
  {
    QTRACE((L"%04x %s", i-(i%16), line));
  }
}
//}}}

DWORD cQCIFSPacketProcessor::PacketSize(UINT_PTR SessionID, const cPtr<cMemoryView> pBuffer, const DWORD dwReceivedBytes)
{
  if(dwReceivedBytes >= 8)
  {
    const unsigned char* const pHeader = reinterpret_cast<const unsigned char* const>(pBuffer->getBytes());

    if (isAcceptedSMBPacket(pHeader + sizeof(int)))
      return ntohl(*reinterpret_cast<const unsigned int* const>(pHeader)) + sizeof(int); //+NetBios size field
    else if(*pHeader == 0x81)
      return ntohs(*(reinterpret_cast<const unsigned short* const>(pHeader) + 1)) + sizeof(int); //+NetBios header field

    QSOS((L"cQCIFSProcessor::HavePacket() FAILED!!! - bytes %d, buffer size is %Iu", dwReceivedBytes, pBuffer->getSize()));
    dumpBuffer((Uint8*)pBuffer->getBytes(), min(dwReceivedBytes, pBuffer->getSize())); 

    QSOS((L"************** PRIVATE MEMEORY ************************"));
    cMemory::ConstPtr mem = pBuffer->getPrivateMemory();
    dumpBuffer((Uint8*)mem->getConstBytes(), mem->getSize()); 

    throw unknown_protocol();
  }
  return 0;
}

bool useSMB2();
static const int gkSMB2AllocationSize = gkSMB2BufferSize*2;
cPtr<cMemoryView> cQCIFSPacketProcessor::GetRXBuffer(const unsigned int minLength)
{
  cActivityScope as1(getActivity(),kRXBuffer);
  if(useSMB2())
  {
    if (minLength > gkSMB2AllocationSize)
      return new cMemoryView(new cMemory(size_t(minLength), cMemory::eHeap));

    LPVOID pMem = 0;
    {
      cLockGuard g(&FreeAllocationsLock);
      if(FreeAllocations.size() > NumPacketsToKeep)
      {
        pMem = FreeAllocations.front();
        FreeAllocations.pop_front();
        memset(pMem,0,gkSMB2AllocationSize);
      }
    }

    if(!pMem)
    {
      pMem = VirtualAlloc(0, gkSMB2AllocationSize, MEM_COMMIT, PAGE_READWRITE);
    }
    return new cMemoryView(new cMemory((char* const)pMem, max((size_t)minLength,(size_t)gkBufferSize), 1, this));//report this is smaller than it is on purpose ...  }
  }
  else
  {
    return new cMemoryView(new cMemory((size_t)gkBufferSize, cMemory::eVirtual));
  }
}

cPtr<cMemoryView> cQCIFSPacketProcessor::GetTXBufferSMB()
{
  cActivityScope as1(getActivity(),kTXBuffer);
  return new cMemoryView(new cMemory((size_t)gkBufferSize, cMemory::eVirtual));
}

cPtr<cMemoryView> cQCIFSPacketProcessor::GetTXBufferSMB2()
{
  cActivityScope as1(getActivity(),kTXBuffer);
  LPVOID pMem = 0;
  {
    cLockGuard g(&FreeAllocationsLock);
    if(FreeAllocations.size() > NumPacketsToKeep)
    {
      pMem = FreeAllocations.front();
      FreeAllocations.pop_front();
      memset(pMem,0,gkSMB2AllocationSize);
    }
  }

  if(!pMem)
  {
    pMem = VirtualAlloc(0, gkSMB2AllocationSize, MEM_COMMIT, PAGE_READWRITE);
  }
  return new cMemoryView(new cMemory((char* const)pMem, (size_t)gkSMB2AllocationSize, 1, this));
}

void cQCIFSPacketProcessor::onRelease(void* ObjectPtr)
{
  cActivityScope as1(getActivity(),kBufferFree);
  cLockGuard g(&FreeAllocationsLock);
  FreeAllocations.push_back(ObjectPtr);
  //VirtualFree(ObjectPtr, 0, MEM_RELEASE);
}

inline bool cQCIFSPacketProcessor::isSMBPacket(const unsigned char* const pBuffer)
{
  return 0 == memcmp(pBuffer, SMBHeaderDelimiter, sizeof(SMBHeaderDelimiter));
}

inline bool cQCIFSPacketProcessor::isSMB2Packet(const unsigned char* const pBuffer)
{
  return 0 == memcmp(pBuffer, SMB2HeaderDelimiter, sizeof(SMB2HeaderDelimiter));
}

inline bool cQCIFSPacketProcessor::isAcceptedSMBPacket(const unsigned char* const pBuffer)
{
  return isSMBPacket(pBuffer) || isSMB2Packet(pBuffer);
}

void PrintHexDump(DWORD length, PBYTE buffer);


String getCommand(short command)
{
  switch(command)
  {
    case SMB2_NegotiateProtocol: return L"SMB2_NegotiateProtocol";
    case SMB2_SessionSetup: return L"SMB2_SessionSetup";
    case SMB2_SessionLogoff: return L"SMB2_SessionLogoff";
    case SMB2_TreeConnect: return L"SMB2_TreeConnect";
    case SMB2_TreeDisconnect: return L"SMB2_TreeDisconnect";
    case SMB2_Create: return L"SMB2_Create";
    case SMB2_Close: return L"SMB2_Close";
    case SMB2_Flush: return L"SMB2_Flush";
    case SMB2_Read: return L"SMB2_Read";
    case SMB2_Write: return L"SMB2_Write";
    case SMB2_Lock: return L"SMB2_Lock";
    case SMB2_Ioctl: return L"SMB2_Ioctl";
    case SMB2_Cancel: return L"SMB2_Cancel";
    case SMB2_KeepAlive: return L"SMB2_KeepAlive";
    case SMB2_Find: return L"SMB2_Find";
    case SMB2_Notify: return L"SMB2_Notify";
    case SMB2_GetInfo: return L"SMB2_GetInfo";
    case SMB2_SetInfo: return L"SMB2_SetInfo";
    case SMB2_Break: return L"SMB2_Break";
    default: break;
  }
  return L"SMB2_UNKNOWN:" + lexical_cast<String>(command);
}

void cQCIFSPacketProcessor::addCreateFileData(const unsigned __int64& sessionID, const vfs::String& filePath, const unsigned __int64& Fid)
{
  cLockGuard g(&CreateFilePathsLock);
  while(CreateFilePaths.size() > NumPathsToKeep)
    CreateFilePaths.pop_front();

  CreateFilePaths.push_back(tCreateFilePath(tCreateFileInfo(sessionID,Fid),filePath));
}

void cQCIFSPacketProcessor::dumpBuffers()
{
  {
    cLockGuard g(&FreeAllocationsLock);
    for(std::list<void*>::const_iterator it = FreeAllocations.begin(); it != FreeAllocations.end(); ++it)
    {
      const unsigned char* const pHeader = reinterpret_cast<const unsigned char* const>(*it);

      int size = 0;
      if (isAcceptedSMBPacket(pHeader + sizeof(int)))
        size = ntohl(*reinterpret_cast<const unsigned int* const>(pHeader)) + sizeof(int); //+NetBios size field
      else if(*pHeader == 0x81)
        size = ntohs(*(reinterpret_cast<const unsigned short* const>(pHeader) + 1)) + sizeof(int); //+NetBios header field
      if(size)
      {
        PBYTE p = (PBYTE)(pHeader + sizeof(int));
        PSMB2_HEADER header = (PSMB2_HEADER)p;
        if(header->Flags & SMB2_FLAGS_SERVER_TO_REDIR)
          QTRACE((L"S %d %s %I64d Credit:%d Flags:0x%x Status:0x%08x", size, getCommand(header->Command).c_str(), header->MessageId, header->CreditRequest, header->Flags, header->Status));
        else
          QTRACE((L"C %d %s %I64d Credit:%d Flags:0x%x", size, getCommand(header->Command).c_str(), header->MessageId, header->CreditRequest, header->Flags));
        PrintHexDump(size, p);        
      }
    }
  }
  {
    cLockGuard g(&CreateFilePathsLock);
    for(std::list<tCreateFilePath>::const_iterator it = CreateFilePaths.begin(); it != CreateFilePaths.end(); ++it)
    {
      QTRACE((L"Session:%I64d: %s => %I64d", it->first.first, it->second.c_str(), it->first.second));
    }
  }
}