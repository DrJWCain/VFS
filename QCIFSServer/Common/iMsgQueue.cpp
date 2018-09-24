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

//iMsgQueue.cpp

#include "stdafx.h"
#include "iThreadHeader.h"
#include "iQueue.h"

///////////////////////////////////////////////////////////////////////////////

class cMsgQueueImpl
{
  friend iMsgQueue;
  iQueue<cPtr<iMsg> > ThreadQueue;
};

iMsgQueue::iMsgQueue()
{
  __MsgQueuePimpl = new cMsgQueueImpl;
}

iMsgQueue::~iMsgQueue()
{
  delete __MsgQueuePimpl;
}

void iMsgQueue::postMessage(cPtr<iMsg> NewMessage)
{
  __MsgQueuePimpl->ThreadQueue.enQElem(NewMessage, eBack);
}

void iMsgQueue::postMessageFront(cPtr<iMsg> NewMessage)
{
  __MsgQueuePimpl->ThreadQueue.enQElem(NewMessage, eFront);
}

cPtr<iMsg> iMsgQueue::getMessage()
{
  return __MsgQueuePimpl->ThreadQueue.deQElem(eBlocking);
}

cPtr<iMsg> iMsgQueue::peekMessage()
{
  return __MsgQueuePimpl->ThreadQueue.deQElem(eNonBlocking);
}

///////////////////////////////////////////////////////////////////////////////

class cLimitedMsgQueueImpl
{
  friend iLimitedMsgQueue;
  iQueue<unsigned int> ReverseThreadQueue;
};

const unsigned int kSeedValue = 0xdeadbeef;
iLimitedMsgQueue::iLimitedMsgQueue(unsigned int maxMessages)
{
  __LimitedMsgQueuePimpl = new cLimitedMsgQueueImpl;
  for(unsigned int i = 0; i < maxMessages; ++i)
  {
    __LimitedMsgQueuePimpl->ReverseThreadQueue.enQElem(kSeedValue,eBack);
  }
}

iLimitedMsgQueue::~iLimitedMsgQueue()
{
  delete __LimitedMsgQueuePimpl;
}

void iLimitedMsgQueue::postMessage(cPtr<iMsg> NewMessage)
{
  __LimitedMsgQueuePimpl->ReverseThreadQueue.deQElem(eBlocking);
  return iMsgQueue::postMessage(NewMessage);
}

void iLimitedMsgQueue::postMessageFront(cPtr<iMsg> NewMessage)
{
  __LimitedMsgQueuePimpl->ReverseThreadQueue.deQElem(eBlocking);
  return iMsgQueue::postMessageFront(NewMessage);
}

cPtr<iMsg> iLimitedMsgQueue::getMessage()
{
  cPtr<iMsg> msg = iMsgQueue::getMessage();
  __LimitedMsgQueuePimpl->ReverseThreadQueue.enQElem(kSeedValue,eBack);
  return msg;
}

