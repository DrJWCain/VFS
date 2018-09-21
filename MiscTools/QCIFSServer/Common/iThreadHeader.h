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

//iThreadHeader.h

#include "iPointer.h"

#pragma once

///////////////////////////////////////////////////////////////////////////////
// Base type for all messages
class iMsg
{
public :
  virtual ~iMsg() {}
  virtual bool isQuitMessage() const { return false; }
  virtual void deliver() = 0;
};

// Concrete message to quit 
class cQuitMessage : public iMsg, public cRefCount
{
public :
  void deliver() {}
  bool isQuitMessage() const { return true; }
};

///////////////////////////////////////////////////////////////////////////////
// Basic queue of messages 
class iMsgQueue
{
public:
  iMsgQueue();
  virtual ~iMsgQueue();

  virtual void postMessage(cPtr<iMsg> NewMessage);
  virtual void postMessageFront(cPtr<iMsg> NewMessage);

  virtual cPtr<iMsg> getMessage();
  virtual cPtr<iMsg> peekMessage();

private:
  class cMsgQueueImpl* __MsgQueuePimpl;
  friend class iThreadHasAMsgQueue;
};

///////////////////////////////////////////////////////////////////////////////
// Limited queue of messages. 
// The caller of postMessage* will block until the queue length drops.
class iLimitedMsgQueue : public iMsgQueue
{
public:
  iLimitedMsgQueue(unsigned int maxMessages);
  virtual ~iLimitedMsgQueue();

  virtual void postMessage(cPtr<iMsg> NewMessage);
  virtual void postMessageFront(cPtr<iMsg> NewMessage);
  virtual cPtr<iMsg> getMessage();

private:
  class cLimitedMsgQueueImpl* __LimitedMsgQueuePimpl;
};

///////////////////////////////////////////////////////////////////////////////
// Basic thread wrapper
class iThread : public cRefCount
{
public:
  iThread();
  virtual ~iThread();
  unsigned int threadID();
  virtual void waitLoopComplete() ;

protected:
  virtual void runLoop() = 0;

private:
  class cThreadImpl* __ThreadPimpl;
  friend class cThreadImpl;
};

///////////////////////////////////////////////////////////////////////////////
// A thread that dispatches messages
// The queue can grow in an unlimited manner (subject to resources limits!)
class iThreadMsgQueue : public iThread, public iMsgQueue
{
protected:
  virtual void dispatchMessage(cPtr<iMsg> msg);
  void runLoop();
};

///////////////////////////////////////////////////////////////////////////////
// A thread that dispatches messages (queue length limited)
// Useful when the messages hold a limited resource (such as allocated RAM)
class iThreadLimitedMsgQueue : public iThread, public iLimitedMsgQueue
{
public:
  iThreadLimitedMsgQueue(unsigned int maxMessages);

protected:
  virtual void dispatchMessage(cPtr<iMsg> msg);
  void runLoop();
};

///////////////////////////////////////////////////////////////////////////////
// A pool of worker threads that compete for work on a single queue
class iThreadPoolMsgQueue : public iMsgQueue, public cRefCount
{
public:
  iThreadPoolMsgQueue(unsigned int numThreads);
  virtual ~iThreadPoolMsgQueue();

private:
  class cThreadPoolMsgQueueImpl* __ThreadPoolMsgQueuePimpl;
};

///////////////////////////////////////////////////////////////////////////////
// A pool of worker threads that compete for work on a single queue
class iThreadPoolLimitedMsgQueue : public iLimitedMsgQueue, public cRefCount
{
public:
  iThreadPoolLimitedMsgQueue(unsigned int numThreads, unsigned int maxMessages);
  virtual ~iThreadPoolLimitedMsgQueue();

private:
  class cThreadPoolMsgQueueImpl* __ThreadPoolMsgQueuePimpl;
};



