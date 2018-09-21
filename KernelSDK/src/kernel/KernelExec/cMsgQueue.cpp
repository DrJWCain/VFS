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

/*
**  
*/

#include "stdafx.h"
#include "cMsgQueue.h"
#include <algorithm>

using namespace vfs;



// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

const unsigned int kMaxNumMsgs = 1000;

cMsgQueue::cMsgQueue() :
  m_SignalMsgQueueNotEmpty(new cManualResetEvent(false /* initially set? */))
#if MSGQUEUE_USING_SEMAPHORE
  ,m_HasSpace(kMaxNumMsgs /* initial count */, kMaxNumMsgs /* max count */)
#endif
{
}




// =======================================================================================
// === postMsg/postMsgFront ==============================================================
// =======================================================================================

void cMsgQueue::postMsg(const iMsgQueue::iMsg::Ptr & message)
{
#if MSGQUEUE_USING_SEMAPHORE
  QVERIFY(m_HasSpace.waitForever() == iWaitable::eWaitResult_Set);
#endif
  cLockGuard threadSafety(&m_MsgQueueThreadSafety);
  m_MsgQueue.push_back(message);
  m_SignalMsgQueueNotEmpty->set();
}

// ---------------------------------------------------------------------------------------

void cMsgQueue::postMsgFront(const iMsgQueue::iMsg::Ptr & message)
{
#if MSGQUEUE_USING_SEMAPHORE
  QVERIFY(m_HasSpace.waitForever() == iWaitable::eWaitResult_Set);
#endif
  cLockGuard threadSafety(&m_MsgQueueThreadSafety);
  m_MsgQueue.push_front(message);
  m_SignalMsgQueueNotEmpty->set();
}




// =======================================================================================
// === countMsgs =========================================================================
// =======================================================================================

unsigned int cMsgQueue::countMsgs() const
{
  cLockGuard threadSafety(&m_MsgQueueThreadSafety);
  return (unsigned int)m_MsgQueue.size();
}




// =======================================================================================
// === visitMsgsOldestFirst/visitMsgsNewestFirst =========================================
// =======================================================================================

bool /* visited all? see fMsgVisitor_Abort */ cMsgQueue::visitMsgsOldestFirst(iMsgVisitor & visitor)
{
  cLockGuard threadSafety(&m_MsgQueueThreadSafety);
  unsigned int msgPos = 0;
  unsigned int numMsgs = (unsigned int)m_MsgQueue.size();
  
  list<iMsgQueue::iMsg::Ptr>::iterator i;
  for (i = m_MsgQueue.begin(); i != m_MsgQueue.end();)
  {
    const int visitResultFlags = visitor.onMsgVisit(msgPos, numMsgs, *i);
    if (visitResultFlags & fMsgVisitor_RemoveMsgFromQueue)
    {
      // remove this msg
      i = m_MsgQueue.erase(i);
      --numMsgs;
#if MSGQUEUE_USING_SEMAPHORE
      m_HasSpace.set();
#endif
    }
    else
    {
      // next msg
      ++i;
      ++msgPos;
    }
    if (visitResultFlags & fMsgVisitor_Abort) break;
  }
  if (m_MsgQueue.empty()) m_SignalMsgQueueNotEmpty->reset();
  return i == m_MsgQueue.end(); // has visited all?
}

bool /* visited all? see fMsgVisitor_Abort */ cMsgQueue::visitMsgsNewestFirst(iMsgVisitor & visitor)
{
  cLockGuard threadSafety(&m_MsgQueueThreadSafety);

  unsigned int numMsgs = m_MsgQueue.size();
  unsigned int msgPos = numMsgs;
  
  if ( numMsgs > 0 )
  {
    list<iMsgQueue::iMsg::Ptr>::iterator i = m_MsgQueue.end();
    do
    {
      // prev msg in the list
      --i;
      --msgPos;

      const int visitResultFlags = visitor.onMsgVisit(msgPos, numMsgs, *i);
      if (visitResultFlags & fMsgVisitor_RemoveMsgFromQueue)
      {
        // remove this msg
        i = m_MsgQueue.erase(i); // i left on msg we did last time around or end()
        --numMsgs;

        #if MSGQUEUE_USING_SEMAPHORE
        m_HasSpace.set();
        #endif
      }
    
      if (visitResultFlags & fMsgVisitor_Abort)
        break;
    }
    while (msgPos > 0);
  }
  return msgPos == 0; // has visited all?
}


// =======================================================================================
// === clearMsgs =========================================================================
// =======================================================================================

unsigned int /* num culled */ cMsgQueue::clearMsgs()
{
  cLockGuard threadSafety(&m_MsgQueueThreadSafety);
  const unsigned int numMsgsCulled = (unsigned int)m_MsgQueue.size();
  m_MsgQueue.clear();
  m_SignalMsgQueueNotEmpty->reset();
#if MSGQUEUE_USING_SEMAPHORE
  m_HasSpace.reset();
#endif
  return numMsgsCulled;
}




// =======================================================================================
// === isMsgWaiting ======================================================================
// =======================================================================================

bool cMsgQueue::isMsgWaiting() const
{
  cLockGuard threadSafety(&m_MsgQueueThreadSafety);
  return !m_MsgQueue.empty();
}




// =======================================================================================
// === waitMsgForever/waitMsg ============================================================
// =======================================================================================

void cMsgQueue::waitMsgForever() const
{
  m_SignalMsgQueueNotEmpty->waitForever();
}

// ---------------------------------------------------------------------------------------

bool /* signalled? */ cMsgQueue::waitMsg(const unsigned int timeoutMs) const
{
  return m_SignalMsgQueueNotEmpty->wait(timeoutMs) == iWaitable::eWaitResult_Set;
}




// =======================================================================================
// === getMsg ============================================================================
// =======================================================================================

bool cMsgQueue::getMsg(iMsgQueue::iMsg::Ptr * const optionalResult)
{
  bool messageWaiting = false;
  optionalResult->invalidate();
  {
    cLockGuard threadSafety(&m_MsgQueueThreadSafety);
    if (!m_MsgQueue.empty())
    {
      *optionalResult = m_MsgQueue.front();
      m_MsgQueue.pop_front();
      messageWaiting = true;
      if (m_MsgQueue.empty()) m_SignalMsgQueueNotEmpty->reset();
#if MSGQUEUE_USING_SEMAPHORE
      m_HasSpace.set();
#endif
    }
  }
  return messageWaiting;
}




// =======================================================================================
// === clear =============================================================================
// =======================================================================================

#if kKernelSDKIfaceVersion_Int < 0400000

unsigned int /* num culled */ cMsgQueue::clearOldestMessagesIfMoreThan(const unsigned int maxNumMsgs)
{
  unsigned int numMsgsCulled = 0;
  list<cThread::iMsg::Ptr> toBeDestroyedOutsideOfCriticalSection;
  {
    cLockGuard ThreadSafety(&m_MsgQueueThreadSafety);
    while (m_MsgQueue.size() > maxNumMsgs)
    {
      toBeDestroyedOutsideOfCriticalSection.push_back(m_MsgQueue.front());
      m_MsgQueue.pop_front();
      numMsgsCulled++;
    }
    if (m_MsgQueue.empty()) m_SignalMsgQueueNotEmpty->reset();
  }
  return numMsgsCulled;
}

unsigned int /* num culled */ cMsgQueue::clearNewestMessagesIfMoreThan(const unsigned int maxNumMsgs)
{
  unsigned int numMsgsCulled = 0;
  list<cThread::iMsg::Ptr> toBeDestroyedOutsideOfCriticalSection;
  {
    cLockGuard ThreadSafety(&m_MsgQueueThreadSafety);
    while (m_MsgQueue.size() > maxNumMsgs)
    {
      toBeDestroyedOutsideOfCriticalSection.push_back(m_MsgQueue.back());
      m_MsgQueue.pop_back();
      numMsgsCulled++;
    }
    if (m_MsgQueue.empty()) m_SignalMsgQueueNotEmpty->reset();
  }
  return numMsgsCulled;
}

unsigned int /* num culled */ cMsgQueue::clearMessages(const type_info& messageTypeToRemove)
{
  return clearMessages(messageTypeToRemove, 0 /* num to leave intact */);
}

unsigned int /* num culled */ cMsgQueue::clearMessages(const type_info& messageType, const unsigned int leavingMinNumMsgsIntact)
{
  unsigned int SkipCount = 0;
  list<cThread::iMsg::Ptr> toBeDestroyedOutsideOfCriticalSection;
  {
    cLockGuard ThreadSafety(&m_MsgQueueThreadSafety);

    if (leavingMinNumMsgsIntact > 0)
    {
      unsigned int MsgCount = 0;
      for (list<cThread::iMsg::Ptr>::iterator i = m_MsgQueue.begin(); i != m_MsgQueue.end(); ++i)
      {
        const type_info & typeOfMessage = typeid(*(i->getRaw()));
        if (typeOfMessage == messageType) MsgCount++;
      }
      SkipCount = Min<unsigned int>(leavingMinNumMsgsIntact, MsgCount);
    }
    {
      list<cThread::iMsg::Ptr>::iterator i = m_MsgQueue.begin();
      while (i != m_MsgQueue.end())
      {
        const type_info & typeOfMessage = typeid(*(i->getRaw()));
        if (typeOfMessage == messageType)
        {
          if (SkipCount > 0)
          {
            SkipCount--;
            ++i;
          }
          else
          {
            toBeDestroyedOutsideOfCriticalSection.push_back(*i);
            i = m_MsgQueue.erase(i);
          }
        }
        else
        {
          ++i;
        }
      }
    }

    if (m_MsgQueue.empty()) m_SignalMsgQueueNotEmpty->reset();
  }
  return SkipCount;
}

#endif
