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

#include <process.h>
#include "QKernelExec.h"
#include "cLogThreadData.h"

using namespace std;
using namespace vfs;




// =======================================================================================
// === cTib14 ============================================================================
// =======================================================================================

#define TIB14_ID 0xc001c0de

class cThreadImpl;
class cTib14
{
  public :
    cTib14(iBasicThread * const threadIface, cThreadImpl * const threadImpl) :
      m_Id(TIB14_ID),
      m_ThreadIface(threadIface),
      m_ThreadImpl(threadImpl)
    {
    }
    bool isValid() const { return m_Id == TIB14_ID; }
    const unsigned long m_Id;            // Set to TIB14_ID
    iBasicThread * const m_ThreadIface;
    cThreadImpl * const m_ThreadImpl;
    cLogThreadData m_LogData;
};




// =======================================================================================
// === cThreadImpl =======================================================================
// =======================================================================================

class cThreadImpl : public iBasicThread, public cRefCount
{
  public :

    QDEFINE_SMARTPTR(cThreadImpl);

    // iBasicThread
    virtual void QAPI startLoop();
    virtual void QAPI runLoop() {} // Implemented by client
    virtual iWaitable::eWakeupReason QAPI waitLoopComplete(const unsigned long timeoutMs);
    virtual void QAPI waitLoopCompleteForever() { (void)waitLoopComplete(INFINITE); }
    virtual void QAPI stopLoop();
    virtual void QAPI signalLoopStop() {} // Implemented by client
    virtual String QAPI getName() const;
    virtual unsigned long QAPI getThreadID() const;
    virtual void QAPI setPriority(const iBasicThread::ePriority NewPriority);
    virtual iBasicThread::ePriority QAPI getPriority() const;
    virtual eExitState QAPI getExitState(String * const desc) const;

    // cThreadImpl
    cThreadImpl(iBasicThread * const IfaceObject, cRefCount * const IfaceObjectRefCount, const String & Name, const iBasicThread::ePriority WhichPriority, const unsigned int InitialStackSize, const eThreadLogging Logging);
    cThreadImpl(iBasicThread * const IfaceObject, cRefCount * const IfaceObjectRefCount, const HANDLE ThreadHandle, const String & Name);
    ~cThreadImpl();
    void suspend();
    void resume();
    iBasicThread * getIfaceObject() { return m_Thread; }

    const String     m_Name;
    cTib14           m_Tib14;

  private :

    mutable cLock             m_ThreadHandleThreadSafety;

    iBasicThread *            m_Thread;
    cRefCount *               m_ThreadRefCount;

    // As long as this pointer is valid the object will be alive and kicking. Only when
    // stopLoop() is called explicitly will it the pointer be invalidated and the object free
    // to destory.
    iBasicThread::Ptr         m_KeepAliveForLoopRunning;

    unsigned long             m_ThreadID;
    HANDLE                    m_ThreadHandle;

    iBasicThread::ePriority   m_Priority;
    unsigned int              m_InitialStackSize;
    bool                      m_IsLoopRunning;
    eThreadLogging            m_Logging;

    cManualResetEvent         m_SignalThreadStopped;

    const bool                m_IsCreatingOwnThread;
    
    mutable cLock             m_ExitStateThreadSafety;
    eExitState                m_ExitState;
    String                    m_ExitDesc;

    typedef unsigned (__stdcall * FUNCPTR)(void *);
    static void QARGS_STACK _staticThreadFunction(cThreadImpl * threadImpl);
    static void _doThreadLoop(iBasicThread * const threadIface, cThreadImpl * const threadImpl);

    // iGuardCreator
    void QAPI onGuardDestroyed(iGuarded::Ptr Guarded);
};
