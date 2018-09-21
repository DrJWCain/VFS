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

#pragma once
/*
**  
*/

#include "QKernelExec.h"

using namespace std;
using namespace vfs;




// =======================================================================================
// === cThreadManager ====================================================================
// =======================================================================================

class cThreadImpl;
class cThreadManager : public iThreadManager, public cRefCount, public iGuardCreator
{
  public:

    QDEFINE_SINGLETON_IMPL(cThreadManager, L"{43312554-2459-406C-AEEC-3D5E83A84BC0} // Thread manager impl", iThreadManager);

    // iManager
    void QAPI logSummary() {}
    void QAPI logDetailedSummary() {}

    // iThreadManager
    virtual iBasicThread * QAPI getCurrentThread();
    virtual cGuard::ConstPtr QAPI startThread(const iBasicThread::Ptr & thread);
    virtual bool QAPI isMainThread();
    virtual unsigned int QAPI getCurrentThreadId();
    virtual cGuard::ConstPtr QAPI addBackgroundActivityCallback(iBackgroundActivityCallback::Ptr NewCallback);
    virtual cGuard::ConstPtr QAPI suspendBackgroundActivity();
    virtual iBasicThread::Ptr QAPI promoteCurrentThread(const String & name);
    virtual cGuard::ConstPtr QAPI addThreadCallback(const iThreadCallback::Ptr & NewCallback);
    virtual void QAPI sleep(const unsigned int delayMs);
    virtual iMsgQueue::Ptr QAPI createAsyncMsgQueue(const String & name, const iBasicThread::ePriority priority);
    virtual iCircularMsgQueue::Ptr QAPI createCircularMsgQueueThread(const String & name, const iBasicThread::ePriority priority);
    virtual unsigned int getOrigWinThreadId() { return m_OrigWindowsThreadId; }
#if kKernelSDKIfaceVersion_Int < 0400000
    virtual iThread::Ptr QAPI createMsgQueueThread(const String & Name, const iThread::ePriority WhichPriority);
#endif
    virtual iMsgQueue::Ptr QAPI INTERNAL_USE_ONLY_createMsgQueue();
    virtual iBasicThread::Ptr QAPI INTERNAL_USE_ONLY_createBasicThreadImpl(iBasicThread * const IfaceObject, cRefCount * const IfaceObjectRefCount, const String & Name, const iBasicThread::ePriority WhichPriority);
    virtual iBasicThread::Ptr QAPI INTERNAL_USE_ONLY_createBasicThreadImpl(iBasicThread * const IfaceObject, cRefCount * const IfaceObjectRefCount, const String & Name, const iBasicThread::ePriority WhichPriority, const unsigned int InitialStackSize, const eThreadLogging Logging);
    virtual void deprecated0() {}

    // cThreadManager
    cThreadManager();
    virtual void doThreadStartedCallbacks(const iBasicThread::Ptr & Thread);
    virtual void doThreadStoppingCallbacks(const iBasicThread::Ptr & Thread);
    bool isSettingMSVCThreadName() const { return m_IsSettingMSVCThreadName; }
    void setOrigWinThreadId(const unsigned int threadId) { m_OrigWindowsThreadId = threadId; }

    static const cThreadImpl * m_MainThreadPtr;

  private :

    bool m_IsSettingMSVCThreadName;

    cLock                   m_BackgroundActivityThreadSafety;
    int                     m_BackgroundActivitySuspendCount; // <= 0 not suspended
    list<iBackgroundActivityCallback::Ptr> m_BackgroundActivityCallbacks;

    cLock                   m_ThreadCallbacksThreadSafety;
    list<iThreadCallback::Ptr> m_ThreadCallbacks;

    UInt32 m_OrigWindowsThreadId;

    class cSuspendBackgroundActivityGuard : public iGuarded, public cRefCount
    {
      public :
        QDEFINE_SMARTPTR(cSuspendBackgroundActivityGuard);
    };

    // iGuardCreator
    void QAPI onGuardDestroyed(iGuarded::Ptr Guarded) throw();

    void _unsuspendBackgroundActivity();
};
