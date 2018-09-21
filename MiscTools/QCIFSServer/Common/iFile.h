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

//iFile.h

#include "iPointer.h"
#include "iThreadHeader.h"

class cMemory;

///////////////////////////////////////////////////////////////////////////////
// Base type that models file handles

class iHandle
{
public:
  virtual ~iHandle() {}
  virtual HANDLE getHandle() = 0;
  virtual void readBytes(struct cOverlapped* overlapped) = 0;
  virtual void writeBytes(struct cOverlapped* overlapped) = 0;
};

class iFile : public iHandle, public cRefCount
{
public:
  iFile(const std::string& filename);
  virtual ~iFile();

  virtual HANDLE getHandle();
  virtual void readBytes(struct cOverlapped* overlapped);
  virtual void writeBytes(struct cOverlapped* overlapped);

private:
  HANDLE hFile;
};

class iSocket : public iHandle, public cRefCount
{
public:
  iSocket(unsigned short listenPort, unsigned int sendAddress, unsigned short sendPort);
  ~iSocket();

  virtual HANDLE getHandle();
  virtual void readBytes(struct cOverlapped* overlapped);
  virtual void writeBytes(struct cOverlapped* overlapped);

private:
  SOCKET hSocket;
  unsigned int SendAddress;
  unsigned short SendPort;
};

///////////////////////////////////////////////////////////////////////////////
// Wrap an IO Completion Port and worker threads

class iCompletionQueue : public cRefCount
{
public:
  iCompletionQueue(cPtr<iHandle> handle);
  ~iCompletionQueue();

  virtual void readBytes(unsigned __int64 offset, unsigned int length);
  virtual void readByteRanges(unsigned __int64 offset, std::list<unsigned int> lengths);
  virtual void readMemories(unsigned __int64 offset, std::list<cPtr<cMemory> > memories);
  virtual void writeBytes(unsigned __int64 offset, cPtr<cMemory> memory);
  virtual void waitComplete();

protected:
  virtual void onReadBytes(unsigned int count, std::list<cPtr<cMemory> > memory, unsigned __int64 where, unsigned int numBytes) = 0;
  virtual void onWriteBytes(unsigned int count, std::list<cPtr<cMemory> > memory, unsigned __int64 where, unsigned int numBytes) = 0;
  virtual void complete(bool ok, DWORD err, struct cOverlapped* overlapped);

private:
  void readBytesInternal(unsigned __int64 offset, const std::list<cPtr<cMemory> >& memoryArray);
  void writeBytesInternal(unsigned __int64 offset, const std::list<cPtr<cMemory> >& memoryArray);
  void postQuitMsg();

private:
  class cCompletionQueueImpl* __CompletionQueuePimpl;

  friend struct cOverlapped;
  friend class cReadMessage;
  friend class iCompletionThread;
};

///////////////////////////////////////////////////////////////////////////////
// Wrap a limited length queue around an IO Completion Port and worker threads

class iLimitedCompletionQueue : public iCompletionQueue
{
public:
  iLimitedCompletionQueue(cPtr<iHandle> handle, unsigned int maxMessages);
  virtual ~iLimitedCompletionQueue();

  virtual void readBytes(unsigned __int64 offset, unsigned int length);

protected:
  void complete(bool ok, DWORD err, struct cOverlapped* overlapped);

private:
  class cLimitedCompletionQueueImpl* __LimitedCompletionQueuePimpl;
};

