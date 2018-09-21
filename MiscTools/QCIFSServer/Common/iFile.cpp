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

#include "stdafx.h"
#include "iFile.h"
#include "iThreadHeader.h"
#include "cMemory.h"
#include "iQueue.h"

#pragma comment(lib, "Ws2_32")

unsigned int getNumProcessors()
{
  SYSTEM_INFO sysInfo;
  GetSystemInfo(&sysInfo);
  return sysInfo.dwNumberOfProcessors;
}

///////////////////////////////////////////////////////////////////////////////

struct cOverlapped : public OVERLAPPED
{
  cOverlapped(unsigned int count, unsigned __int64 offset, cPtr<cMemory> memory);
  cOverlapped(unsigned int count, unsigned __int64 offset, std::list<cPtr<cMemory> > memory);

  unsigned __int64 offset();
  void setOffset(unsigned __int64 offset);

  cPtr<cMemory> getBuffer();
  LPWSABUF getWSABuffer();
  unsigned int numWSABufferEntries();

  void readBytes(cPtr<iHandle> handle);
  void writeBytes(cPtr<iHandle> handle);
  void dispatch(cPtr<iCompletionQueue> completion, unsigned int numBytes);

private:
  enum eOperationType {eNotSet, eReadOp, eWriteOp} OperationType;
  //cPtr<cMemory> Memory;
  std::list<cPtr<cMemory> > MemoryArray;
  WSABUF WSABuffer[10];
  unsigned int Count;
};

///////////////////////////////////////////////////////////////////////////////

iFile::iFile(const std::string& filename)
{
  hFile = CreateFile(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, 
    NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);
}

iFile::~iFile()
{
  CloseHandle(hFile);
}

HANDLE iFile::getHandle()
{
  return hFile;
}

void iFile::readBytes(struct cOverlapped* overlapped)
{
  cPtr<cMemory> memory = overlapped->getBuffer();
  ReadFile(hFile,memory->getBytes(),memory->getSize(),NULL,overlapped);
}

void iFile::writeBytes(struct cOverlapped* overlapped)
{
  cPtr<cMemory> memory = overlapped->getBuffer();
  WriteFile(hFile,memory->getBytes(),memory->getSize(),NULL,overlapped);
}

///////////////////////////////////////////////////////////////////////////////

iSocket::iSocket(unsigned short listenPort, unsigned int sendAddress, unsigned short sendPort)
: SendAddress(sendAddress), SendPort(sendPort)
{
  hSocket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);

	sockaddr_in addr;
	addr.sin_family      = AF_INET;
	addr.sin_port        = htons(listenPort);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(hSocket, reinterpret_cast<SOCKADDR*>(&addr), sizeof(addr)) == SOCKET_ERROR) {
		printf("bind() failed with error %d\n", WSAGetLastError());
	}
}

iSocket::~iSocket()
{
  closesocket(hSocket);
}

HANDLE iSocket::getHandle()
{
  return (HANDLE)hSocket;
}

void iSocket::readBytes(struct cOverlapped* overlapped)
{
  DWORD RecvBytes = 0;
  DWORD Flags = 0;
  SOCKADDR_IN IpAddress;
  int Addrlen = sizeof(SOCKADDR_IN);
  memset(&IpAddress, 0, Addrlen);

  if(WSARecvFrom(hSocket, 
    overlapped->getWSABuffer(), overlapped->numWSABufferEntries(), 
    &RecvBytes, &Flags, 
    reinterpret_cast<SOCKADDR*>(&IpAddress), &Addrlen, 
    overlapped, NULL) == SOCKET_ERROR) 
  {
    int err = WSAGetLastError();
    if(ERROR_IO_PENDING != err)
    {
      printf("WSARecvFrom() failed with error %d\n", err);
    }
  }
}

void iSocket::writeBytes(struct cOverlapped* overlapped)
{
	sockaddr_in recvAddr;
	recvAddr.sin_family      = AF_INET;
	recvAddr.sin_port        = htons(SendPort);
	recvAddr.sin_addr.s_addr = htonl(SendAddress);

  DWORD BytesSent = 0;
	if(WSASendTo(hSocket, 
    overlapped->getWSABuffer(), overlapped->numWSABufferEntries(), 
    &BytesSent, 0, 
    reinterpret_cast<SOCKADDR*>(&recvAddr), sizeof(recvAddr), 
    overlapped, NULL) == SOCKET_ERROR) 
  {
    int err = WSAGetLastError();
    if(ERROR_IO_PENDING != err)
    {
      printf("WSASendTo() failed with error %d\n", err);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////

cOverlapped::cOverlapped(unsigned int count, unsigned __int64 offset, cPtr<cMemory> memory)
  : OperationType(eNotSet), Count(count)
{
  Internal = 0;
  InternalHigh = 0;
  hEvent = 0;
  setOffset(offset);

  MemoryArray.push_back(memory);
}

cOverlapped::cOverlapped(unsigned int count, unsigned __int64 offset, std::list<cPtr<cMemory> > memory)
  : OperationType(eNotSet), MemoryArray(memory), Count(count)
{
  Internal = 0;
  InternalHigh = 0;
  hEvent = 0;
  setOffset(offset);
}

unsigned __int64 cOverlapped::offset()
{
  ULARGE_INTEGER offsetTemp;
  offsetTemp.LowPart = Offset;
  offsetTemp.HighPart = OffsetHigh;
  return offsetTemp.QuadPart;
}

void cOverlapped::setOffset(unsigned __int64 offset)
{
  ULARGE_INTEGER offsetTemp;
  offsetTemp.QuadPart = offset;
  Offset = offsetTemp.LowPart;
  OffsetHigh = offsetTemp.HighPart;
}

cPtr<cMemory> cOverlapped::getBuffer()
{
  return MemoryArray.front();
}

LPWSABUF cOverlapped::getWSABuffer()
{
  int i = 0;
  for(std::list<cPtr<cMemory> >::iterator it = MemoryArray.begin(); it != MemoryArray.end(); ++it)
  {
    WSABuffer[i].len = (*it)->getSize();
    WSABuffer[i].buf = (char*)(*it)->getBytes();
    ++i;
  }
  return WSABuffer;
}

unsigned int cOverlapped::numWSABufferEntries()
{
  return (unsigned int)MemoryArray.size();
}

void cOverlapped::readBytes(cPtr<iHandle> handle)
{
  OperationType = eReadOp;
  handle->readBytes(this);
}

void cOverlapped::writeBytes(cPtr<iHandle> handle)
{
  OperationType = eWriteOp;
  handle->writeBytes(this);
}

void cOverlapped::dispatch(cPtr<iCompletionQueue> completion, unsigned int numBytes)
{
  switch(OperationType)
  {
  case eReadOp:
    completion->onReadBytes(Count,MemoryArray,offset(),numBytes);
    break;
  case eWriteOp:
    completion->onWriteBytes(Count,MemoryArray,offset(),numBytes);
    break;
  default:
    break;
  }
}


///////////////////////////////////////////////////////////////////////////////
// Thread that waits upon a completion port

class iCompletionThread : public iThread
{
  iCompletionQueue* Completion;
public:
  iCompletionThread(iCompletionQueue* completion);

protected:
  void runLoop();
};

///////////////////////////////////////////////////////////////////////////////

class cCompletionQueueImpl : private cRefCount
{
  HANDLE hPort;
  cPtr<iHandle> Handle;
  HANDLE hEmptyEvent;
  unsigned int NumWorkerThreads;
  unsigned int Count;

  friend class iCompletionThread;
  friend class iCompletionQueue;
};

const int kKillCompletionKey = 0xdeadbeef;
const int kFileCompletionKey = 0x2468abcd;
iCompletionQueue::iCompletionQueue(cPtr<iHandle> handle)
{
  __CompletionQueuePimpl = new cCompletionQueueImpl;
  __CompletionQueuePimpl->Handle = handle;

  __CompletionQueuePimpl->hPort = CreateIoCompletionPort(handle->getHandle(), NULL, kFileCompletionKey, 0);  
  __CompletionQueuePimpl->hEmptyEvent = CreateEvent(0, TRUE, 0, 0);
  __CompletionQueuePimpl->Count = 0;

  __CompletionQueuePimpl->NumWorkerThreads = getNumProcessors() * 2;//See Jeffery Richter
  for(unsigned int i = 0; i < __CompletionQueuePimpl->NumWorkerThreads; ++i)
  {
    new iCompletionThread(this);
  }
}

iCompletionQueue::~iCompletionQueue()
{
  for(unsigned int i = 0; i < __CompletionQueuePimpl->NumWorkerThreads; ++i)
  {
    postQuitMsg();
  }
  CloseHandle(__CompletionQueuePimpl->hEmptyEvent);
  CloseHandle(__CompletionQueuePimpl->hPort);

  delete __CompletionQueuePimpl;
}

void iCompletionQueue::readBytes(unsigned __int64 offset, unsigned int length)
{
  std::list<cPtr<cMemory> > memoryArray;
  memoryArray.push_back(new cAllocatedMemory(length));
  readBytesInternal(offset, memoryArray);
}

void iCompletionQueue::readByteRanges(unsigned __int64 offset, std::list<unsigned int> lengths)
{
  std::list<cPtr<cMemory> > memoryArray;
  for(std::list<unsigned int>::iterator it = lengths.begin(); it != lengths.end(); ++it)
  {
    memoryArray.push_back(new cAllocatedMemory(*it));
  }
  readBytesInternal(offset, memoryArray);
}

void iCompletionQueue::readMemories(unsigned __int64 offset, std::list<cPtr<cMemory> > memories)
{
  readBytesInternal(offset, memories);
}

void iCompletionQueue::writeBytes(unsigned __int64 offset, cPtr<cMemory> memory)
{
  std::list<cPtr<cMemory> > memoryArray;
  memoryArray.push_back(memory);
  writeBytesInternal(offset, memoryArray);
}

void iCompletionQueue::readBytesInternal(unsigned __int64 offset, const std::list<cPtr<cMemory> >& memoryArray)
{
  ResetEvent(__CompletionQueuePimpl->hEmptyEvent);
  __CompletionQueuePimpl->addRef();
  cOverlapped* overlapped = new cOverlapped(__CompletionQueuePimpl->Count++, offset, memoryArray);
  overlapped->readBytes(__CompletionQueuePimpl->Handle);
}

void iCompletionQueue::writeBytesInternal(unsigned __int64 offset, const std::list<cPtr<cMemory> >& memoryArray)
{
  ResetEvent(__CompletionQueuePimpl->hEmptyEvent);
  __CompletionQueuePimpl->addRef();
  cOverlapped* overlapped = new cOverlapped(__CompletionQueuePimpl->Count++, offset, memoryArray);
  overlapped->writeBytes(__CompletionQueuePimpl->Handle);
}

void iCompletionQueue::postQuitMsg()
{
  ResetEvent(__CompletionQueuePimpl->hEmptyEvent);
  __CompletionQueuePimpl->addRef();
  PostQueuedCompletionStatus(__CompletionQueuePimpl->hPort,0,kKillCompletionKey,0);
}

void iCompletionQueue::complete(bool ok, DWORD err, cOverlapped* overlapped)
{
  if(overlapped)
  {
    delete overlapped;
  }
  if(__CompletionQueuePimpl->removeRef())
  {
    SetEvent(__CompletionQueuePimpl->hEmptyEvent);
  }
}

void iCompletionQueue::waitComplete()
{
  WaitForSingleObject(__CompletionQueuePimpl->hEmptyEvent, INFINITE);
  __CompletionQueuePimpl->Count = 0;
}

///////////////////////////////////////////////////////////////////////////////

iCompletionThread::iCompletionThread(iCompletionQueue* completion)
: Completion(completion)
{
}

void iCompletionThread::runLoop()
{
  while(1)
  {
    DWORD numBytes;
    ULONG_PTR key;
    cOverlapped* overlaped = 0;
    bool ok = (0 != GetQueuedCompletionStatus(Completion->__CompletionQueuePimpl->hPort,&numBytes,&key,(OVERLAPPED**)&overlaped,INFINITE));
    DWORD err = GetLastError();
    if(ok)
    {
      if(kKillCompletionKey == key)
        break;
      if(overlaped)
      {
        if(kFileCompletionKey == key)
        {
          overlaped->dispatch(Completion, numBytes);
        }
      }
    }
    Completion->complete(ok,err,overlaped);
  }
}

///////////////////////////////////////////////////////////////////////////////

class cReadMessage : public iMsg, public cRefCount
{
  iCompletionQueue* CompletionQueue;
  unsigned __int64 Offset;
  unsigned int Length;
public :
  cReadMessage(iCompletionQueue* completionQueue, unsigned __int64 offset, unsigned int length)
    : CompletionQueue(completionQueue), Offset(offset), Length(length)
  {
  }
  void deliver()
  {
    std::list<cPtr<cMemory> > memoryArray;
    memoryArray.push_back(new cAllocatedMemory(Length));
    CompletionQueue->readBytesInternal(Offset, memoryArray);
  }
};

class cMyThreadMsgQueue : public iThreadMsgQueue
{
  iQueue<unsigned int>& LimitedQueue;
public:
  cMyThreadMsgQueue(iQueue<unsigned int>& limitedQueue) : LimitedQueue(limitedQueue)
  {
  }
  void dispatchMessage(cPtr<iMsg> msg)
  {
    LimitedQueue.deQElem(eBlocking);
    iThreadMsgQueue::dispatchMessage(msg);
  }
};

class cLimitedCompletionQueueImpl
{
  friend class iLimitedCompletionQueue;
  iQueue<unsigned int> LimitedQueue;
  cMyThreadMsgQueue EntryQueue;

  cLimitedCompletionQueueImpl() : EntryQueue(LimitedQueue)
  {
  }
};

const unsigned int kSeedVal = 0xdeadbeef;
iLimitedCompletionQueue::iLimitedCompletionQueue(cPtr<iHandle> handle, unsigned int maxMessages)
  : iCompletionQueue(handle)
{
  __LimitedCompletionQueuePimpl = new cLimitedCompletionQueueImpl;

  for(unsigned int i = 0; i < maxMessages; ++i)
  {
    __LimitedCompletionQueuePimpl->LimitedQueue.enQElem(kSeedVal,eBack);
  }
}

iLimitedCompletionQueue::~iLimitedCompletionQueue()
{
  delete __LimitedCompletionQueuePimpl;
}

void iLimitedCompletionQueue::readBytes(unsigned __int64 offset, unsigned int length)
{
  __LimitedCompletionQueuePimpl->EntryQueue.postMessage(new cReadMessage(this,offset,length));
}

void iLimitedCompletionQueue::complete(bool ok, DWORD err, cOverlapped* overlapped)
{
  __LimitedCompletionQueuePimpl->LimitedQueue.enQElem(kSeedVal,eBack);
  iCompletionQueue::complete(ok, err, overlapped);
}
