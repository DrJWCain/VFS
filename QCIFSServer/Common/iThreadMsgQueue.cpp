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

//iThreadWithQueue.cpp

#include "stdafx.h"
#include "iThreadHeader.h"

///////////////////////////////////////////////////////////////////////////////

void iThreadMsgQueue::runLoop()
{
  while(1)
  {
    cPtr<iMsg> msg = getMessage();
    if(msg->isQuitMessage())
      break;
    dispatchMessage(msg);
  }
}

void iThreadMsgQueue::dispatchMessage(cPtr<iMsg> msg)
{
  msg->deliver();
}

///////////////////////////////////////////////////////////////////////////////

iThreadLimitedMsgQueue::iThreadLimitedMsgQueue(unsigned int maxMessages)
: iLimitedMsgQueue(maxMessages)
{
}

void iThreadLimitedMsgQueue::runLoop()
{
  while(1)
  {
    cPtr<iMsg> msg = getMessage();
    if(msg->isQuitMessage())
      break;
    dispatchMessage(msg);
  }
}

void iThreadLimitedMsgQueue::dispatchMessage(cPtr<iMsg> msg)
{
  msg->deliver();
}

///////////////////////////////////////////////////////////////////////////////

class iThreadHasAMsgQueue : public iThread
{
  iMsgQueue* MessageQueue;
public:
  iThreadHasAMsgQueue(iMsgQueue* messageQueue) : MessageQueue(messageQueue)
  {
  }
  void runLoop()
  {
    while(1)
    {
      cPtr<iMsg> msg = MessageQueue->getMessage();
      if(msg->isQuitMessage())
        break;
      dispatchMessage(msg);
    }
  }
  void dispatchMessage(cPtr<iMsg> msg)
  {
    msg->deliver();
  }
};

///////////////////////////////////////////////////////////////////////////////

class cThreadPoolMsgQueueImpl
{
  friend iThreadPoolMsgQueue;
  friend iThreadPoolLimitedMsgQueue;
  std::list<cPtr<iThread> > ThreadPool;
};

iThreadPoolMsgQueue::iThreadPoolMsgQueue(unsigned int numThreads)
{
  __ThreadPoolMsgQueuePimpl = new cThreadPoolMsgQueueImpl;
  for(unsigned int i = 0; i < numThreads; ++i)
  {
    __ThreadPoolMsgQueuePimpl->ThreadPool.push_back(new iThreadHasAMsgQueue(this));
  }
}

iThreadPoolMsgQueue::~iThreadPoolMsgQueue()
{
  //tell the theads to die 
  for(std::list<cPtr<iThread> >::iterator it = __ThreadPoolMsgQueuePimpl->ThreadPool.begin(); it != __ThreadPoolMsgQueuePimpl->ThreadPool.end(); ++it)
  {
    postMessageFront(new cQuitMessage);
  }

  //wait until they do
  for(std::list<cPtr<iThread> >::iterator it = __ThreadPoolMsgQueuePimpl->ThreadPool.begin(); it != __ThreadPoolMsgQueuePimpl->ThreadPool.end(); ++it)
  {
    (*it)->waitLoopComplete();
  }


  delete __ThreadPoolMsgQueuePimpl;
}

///////////////////////////////////////////////////////////////////////////////

iThreadPoolLimitedMsgQueue::iThreadPoolLimitedMsgQueue(unsigned int numThreads, unsigned int maxMessages) : iLimitedMsgQueue(maxMessages)
{
  __ThreadPoolMsgQueuePimpl = new cThreadPoolMsgQueueImpl;
  for(unsigned int i = 0; i < numThreads; ++i)
  {
    __ThreadPoolMsgQueuePimpl->ThreadPool.push_back(new iThreadHasAMsgQueue(this));
  }
}

iThreadPoolLimitedMsgQueue::~iThreadPoolLimitedMsgQueue()
{
  //tell the theads to die
  for(std::list<cPtr<iThread> >::iterator it = __ThreadPoolMsgQueuePimpl->ThreadPool.begin(); it != __ThreadPoolMsgQueuePimpl->ThreadPool.end(); ++it)
  {
    postMessageFront(new cQuitMessage);
  }

  //wait until they do
  for(std::list<cPtr<iThread> >::iterator it = __ThreadPoolMsgQueuePimpl->ThreadPool.begin(); it != __ThreadPoolMsgQueuePimpl->ThreadPool.end(); ++it)
  {
    (*it)->waitLoopComplete();
  }

  delete __ThreadPoolMsgQueuePimpl;
}
