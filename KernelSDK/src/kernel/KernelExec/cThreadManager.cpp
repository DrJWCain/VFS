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
#include "cThreadManager.h"
#include "cThreadImpl.h"
#include "cMsgQueue.h"
#include "LogCategories.h"

using namespace vfs;




// =======================================================================================
// === m_MainThreadPtr ===================================================================
// =======================================================================================

const cThreadImpl * cThreadManager::m_MainThreadPtr = 0;




// =======================================================================================
// === Non-IThread Helper ================================================================
// =======================================================================================

namespace vfs
{
class cNonIThread : public iBasicThread, public cRefCount
{
  public:
    cNonIThread (HANDLE threadHandle, const String & name) :
      m_Pimpl(new cThreadImpl(this, this, threadHandle, name))
    {
    }
    // iBasicThread
    void QAPI runLoop() {}
    virtual void QAPI signalLoopStop() {}
    virtual iWaitable::eWakeupReason QAPI waitLoopComplete(const unsigned long timeoutMs) { return m_Pimpl->waitLoopComplete(timeoutMs); }
    virtual void QAPI waitLoopCompleteForever() { m_Pimpl->waitLoopCompleteForever(); }
    virtual void QAPI stopLoop() { m_Pimpl->stopLoop(); }
    virtual void QAPI startLoop() { m_Pimpl->startLoop(); }
    virtual String QAPI getName() const { return m_Pimpl->getName(); }
    virtual unsigned long QAPI getThreadID() const { return m_Pimpl->getThreadID(); }
    virtual void QAPI setPriority(const ePriority NewPriority) { return m_Pimpl->setPriority(NewPriority); }
    virtual ePriority QAPI getPriority() const { return m_Pimpl->getPriority(); }
    virtual eExitState QAPI getExitState(String * const desc) const { return m_Pimpl->getExitState(desc); }
  private :
    iBasicThread::Ptr m_Pimpl;
};
}




// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

cThreadManager::cThreadManager() :
  m_BackgroundActivitySuspendCount(0),
  m_IsSettingMSVCThreadName(true)
{
  try
  {
    (void)iSystemInfo::singleton().getCmdLineOption(L"nothreadnames");
    m_IsSettingMSVCThreadName = false;
  }
  catch (cRecoverable &)
  {
  }
}




// =======================================================================================
// === sleep =============================================================================
// =======================================================================================

void QAPI cThreadManager::sleep(const unsigned int delayMs)
{
#if defined(_WIN32)
  Sleep(delayMs);
#elif defined(_OSX64)
  gCarbon->delay(minMdelayMsillisecs);
#endif
}




// =======================================================================================
// === INTERNAL_USE_ONLY ==================================================================
// =======================================================================================

iMsgQueue::Ptr QAPI cThreadManager::INTERNAL_USE_ONLY_createMsgQueue()
{
  return new cMsgQueue;
}

iBasicThread::Ptr QAPI cThreadManager::INTERNAL_USE_ONLY_createBasicThreadImpl(iBasicThread * const IfaceObject, cRefCount * const IfaceObjectRefCount, const String & Name, const iBasicThread::ePriority WhichPriority)
{
  return new cThreadImpl(IfaceObject, IfaceObjectRefCount, Name, WhichPriority, 0 /* default stack size */, eThreadLogging_Collapsed);
}

iBasicThread::Ptr QAPI cThreadManager::INTERNAL_USE_ONLY_createBasicThreadImpl(iBasicThread * const IfaceObject, cRefCount * const IfaceObjectRefCount, const String & Name, const iBasicThread::ePriority WhichPriority, const unsigned int InitialStackSize, const eThreadLogging Logging)
{
  return new cThreadImpl(IfaceObject, IfaceObjectRefCount, Name, WhichPriority, InitialStackSize, Logging);
}




// =======================================================================================
// === getCurrentThreadId ================================================================
// =======================================================================================

namespace
{

__forceinline unsigned long _getCurrentThreadId()
{
#ifdef _M_X64
  return GetCurrentThreadId();
#else
  unsigned long result;
  __asm
  {
    mov eax, fs:[0x24] // as documented by Matt Pietrik in MSJ - assumes win NT or W2K
    mov result, eax
  }
  return result;
#endif
}

}

unsigned int QAPI cThreadManager::getCurrentThreadId()
{
  return _getCurrentThreadId();
}




// =======================================================================================
// === addThreadCallback =================================================================
// =======================================================================================

cGuard::ConstPtr QAPI cThreadManager::addThreadCallback(const iThreadCallback::Ptr & NewCallback)
{
  cLockGuard ThreadSafety(&m_ThreadCallbacksThreadSafety);
  m_ThreadCallbacks.push_back(NewCallback);
  return new cGuard(this, NewCallback);
}

// ---------------------------------------------------------------------------------------

void cThreadManager::doThreadStartedCallbacks(const iBasicThread::Ptr & Thread)
{
  list<iThreadCallback::Ptr> CopyOfCallbacks;
  {
    cLockGuard ThreadSafety(&m_ThreadCallbacksThreadSafety);
    CopyOfCallbacks = m_ThreadCallbacks;
  }
  for (list<iThreadCallback::Ptr>::const_iterator i = CopyOfCallbacks.begin(); i != CopyOfCallbacks.end(); ++i)
  {
    (*i)->onThreadStarted(Thread);
  }
}

// ---------------------------------------------------------------------------------------

void cThreadManager::doThreadStoppingCallbacks(const iBasicThread::Ptr & Thread)
{
  list<iThreadCallback::Ptr> CopyOfCallbacks;
  {
    cLockGuard ThreadSafety(&m_ThreadCallbacksThreadSafety);
    CopyOfCallbacks = m_ThreadCallbacks;
  }
  for (list<iThreadCallback::Ptr>::const_iterator i = CopyOfCallbacks.begin(); i != CopyOfCallbacks.end(); ++i)
  {
    (*i)->onThreadStopping(Thread);
  }
}




// =======================================================================================
// === addBackgroundActivityCallback =====================================================
// =======================================================================================

cGuard::ConstPtr QAPI cThreadManager::addBackgroundActivityCallback(iBackgroundActivityCallback::Ptr NewCallback)
{
  cLockGuard ThreadSafety(&m_BackgroundActivityThreadSafety);
  m_BackgroundActivityCallbacks.push_back(NewCallback);
  return new cGuard(this, NewCallback);
}




// =======================================================================================
// === suspendBackgroundActivity =========================================================
// =======================================================================================

cGuard::ConstPtr QAPI cThreadManager::suspendBackgroundActivity()
{
  cLockGuard ThreadSafety(&m_BackgroundActivityThreadSafety);

  m_BackgroundActivitySuspendCount++;
  if (m_BackgroundActivitySuspendCount == 1)
  {
    cLogIndentGuard Indent(kCtgBkgndActivitySuspension, L"cThreadManager::suspendBackgroundActivity: suspending");
    for (list<iBackgroundActivityCallback::Ptr>::const_iterator i = m_BackgroundActivityCallbacks.begin(); i != m_BackgroundActivityCallbacks.end(); ++i)
    {
      (*i)->suspend();
    }
  }
  return new cGuard(this, new cSuspendBackgroundActivityGuard);
}

// ---------------------------------------------------------------------------------------

void cThreadManager::_unsuspendBackgroundActivity()
{
  cLockGuard ThreadSafety(&m_BackgroundActivityThreadSafety);

  m_BackgroundActivitySuspendCount--;
  if (m_BackgroundActivitySuspendCount == 0)
  {
    cLogIndentGuard Indent(kCtgBkgndActivitySuspension, L"cThreadManager::suspendBackgroundActivity: unsuspending");
    for (list<iBackgroundActivityCallback::Ptr>::const_iterator i = m_BackgroundActivityCallbacks.begin(); i != m_BackgroundActivityCallbacks.end(); ++i)
    {
      (*i)->unsuspend();
    }
  }
}




// =======================================================================================
// === getCurrentThread ==================================================================
// =======================================================================================

iBasicThread * QAPI cThreadManager::getCurrentThread()
{
  // get the task pointer from the TIB
  void * fs14 = 0;
#ifdef _M_X64
  fs14 = (void *)__readgsqword(0x28);
#else
  __asm
  {
      mov eax, fs:[0x14]
      mov fs14, eax
  }
#endif
  cTib14 * const tib = reinterpret_cast<cTib14 *>(fs14);
  if (tib)
  {
    QVERIFY(tib->isValid());
    return tib->m_ThreadIface;
  }
  else
  {
    return 0;
  }
}



// =======================================================================================
// === createAsyncMsgQueue ===============================================================
// =======================================================================================

class cAsyncMsgQueue : public cBasicThread
{
  public :
    QDEFINE_SMARTPTR(cAsyncMsgQueue);
    cAsyncMsgQueue(const String & name, const iBasicThread::ePriority priority) :
      cBasicThread(name, priority),
      m_MsgQueue(new cMsgQueue)
    {
    }
    const iMsgQueue::Ptr & getMsgQueue() const { return m_MsgQueue; }
  private:
    iMsgQueue::Ptr m_MsgQueue;
    void QAPI signalLoopStop() { /* do nothing - have to post quit msg to stop */ }
    void QAPI runLoop()
    {
      bool isRunning = true;
      do
      {
        m_MsgQueue->waitMsgForever();
        iMsgQueue::iMsg::Ptr msg;
        while (m_MsgQueue->getMsg(&msg))
        {
          msg->deliver2();
          if (msg->isQuitMsg()) { isRunning = false; break; }
          msg.invalidate();
        }
      } while (isRunning);
    }
};

iMsgQueue::Ptr QAPI cThreadManager::createAsyncMsgQueue(const String & name, const iBasicThread::ePriority priority)
{
  cAsyncMsgQueue::Ptr thread = new cAsyncMsgQueue(name, priority);
  thread->startLoop();
  return thread->getMsgQueue();
  // "thread" goes out of scope here but is not destroyed because it is running. Post a quit msg
  // to the queue to kill it.
}




// =======================================================================================
// === createCircularMsgQueueThread ======================================================
// =======================================================================================

class cAsyncCircularMsgQueue : public cBasicThread
{
  public :
    QDEFINE_SMARTPTR(cAsyncCircularMsgQueue);
    cAsyncCircularMsgQueue(const String & name, const iBasicThread::ePriority priority) :
      cBasicThread(name, priority)/*,
      m_MsgQueue(new cMsgQueue)*/
    {
    }
    const iCircularMsgQueue::Ptr & getMsgQueue() const { return m_MsgQueue; }
  private:
    iCircularMsgQueue::Ptr m_MsgQueue;
    void QAPI signalLoopStop() { /* do nothing - have to post quit msg to stop */ }
    void QAPI runLoop()
    {
      bool isRunning = true;
      do
      {
        m_MsgQueue->waitMsgForever();
        iCircularMsgQueue::iMsg::Ptr msg;
        while (m_MsgQueue->getMsg(&msg))
        {
          msg->deliver();
          if (msg->isQuitMsg()) { isRunning = false; break; }
          msg.invalidate();
        }
      } while (isRunning);
    }
};

iCircularMsgQueue::Ptr QAPI cThreadManager::createCircularMsgQueueThread(const String & name, const iBasicThread::ePriority priority)
{
  cAsyncCircularMsgQueue::Ptr thread = new cAsyncCircularMsgQueue(name, priority);
  thread->startLoop();
  return thread->getMsgQueue();
  // "thread" goes out of scope here but is not destroyed because it is running. Post a quit msg
  // to the queue to kill it.
}




// =======================================================================================
// === promoteThread =====================================================================
// =======================================================================================

iBasicThread::Ptr QAPI cThreadManager::promoteCurrentThread(const String & name)
{
  return new cNonIThread(GetCurrentThread(), name);
}



// =======================================================================================
// === startThread =======================================================================
// =======================================================================================

cGuard::ConstPtr QAPI cThreadManager::startThread(const iBasicThread::Ptr & thread)
{
  thread->startLoop();
  return new cGuard(this, thread);
}




// =======================================================================================
// === onGuardDestroyed ==================================================================
// =======================================================================================

void QAPI cThreadManager::onGuardDestroyed(iGuarded::Ptr Guarded) throw()
{
  iBasicThread::Ptr candidate = ptr_cast<iBasicThread::Ptr>(Guarded);
  if (candidate.isValid())
  {
    candidate->signalLoopStop();
  }
  else
  {
    cSuspendBackgroundActivityGuard::Ptr candidate = ptr_cast<cSuspendBackgroundActivityGuard::Ptr>(Guarded);
    if (candidate.isValid())
    {
      _unsuspendBackgroundActivity();
    }
    else
    {
      iBackgroundActivityCallback::Ptr candidate =  ptr_cast<iBackgroundActivityCallback::Ptr>(Guarded);
      if (candidate.isValid())
      {
        m_BackgroundActivityCallbacks.remove(candidate);
      }
    }
  }
}




// =======================================================================================
// === isMainThread ======================================================================
// =======================================================================================

bool QAPI cThreadManager::isMainThread()
{
  const cTib14 * const mainThreadTibPtr = &m_MainThreadPtr->m_Tib14;
#ifdef _M_X64
  return mainThreadTibPtr == (cTib14 *)__readgsqword(0x28);
#else
  void * result = 0;
  __asm
  {
    mov eax, fs:[0x14] // as documented in winnt.h
    cmp eax, mainThreadTibPtr
    je  End
    xor eax,eax
End:
    mov result, eax
  }
  return result != 0;
#endif
}




// =======================================================================================
// === createMsgQueueThread ==============================================================
// =======================================================================================

#if kKernelSDKIfaceVersion_Int < 0400000
class cMsgQueueThread : public cThread
{
  public :

    QDEFINE_SMARTPTR(cMsgQueueThread);

    cMsgQueueThread(const String & Name, const cThread::ePriority WhichPriority) :
      cThread(Name, WhichPriority)
    {
    }

  private:

    void QAPI signalLoopStop()
    {
      postMessage(new iThread::cQuitMessage);
    }

    void QAPI runLoop()
    {
      bool IsRunning = true;
      do
      {
        waitMessageForever();

        // Must use this form of getMessage() which is atomic as far as the thread message
        // queue is concerned. iThread::clearMessage() may be called at any time.
        iMsg::Ptr Msg;
        while (getMessage(&Msg))
        {
          if (Msg->isQuitMessage()) { IsRunning = false; break; }
          Msg->deliver2();
          Msg.invalidate();
        }
      } while (IsRunning);
    }
};
iThread::Ptr QAPI cThreadManager::createMsgQueueThread(const String & Name, const iThread::ePriority WhichPriority)
{
  return new cMsgQueueThread(Name, WhichPriority);
}
#endif