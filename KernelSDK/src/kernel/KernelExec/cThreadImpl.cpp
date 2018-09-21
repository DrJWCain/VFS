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

// cThreadImpl.cpp
//{{{  includes, using namespaces
// 

#include "stdafx.h"

#include "cThreadImpl.h"

#include "LogCategories.h"
#include "cLogOutputThread.h"
#include "cLogManager.h"
#include "cExceptionManager.h"

using namespace vfs;
//}}}

//{{{
static int _Win32Priorities[] =
{
  THREAD_PRIORITY_IDLE,                // eIdle
  THREAD_PRIORITY_LOWEST,              // eLowest
  THREAD_PRIORITY_BELOW_NORMAL,        // eBelowNormal
  THREAD_PRIORITY_NORMAL,              // eNormal
  THREAD_PRIORITY_ABOVE_NORMAL,        // eAboveNormal
  THREAD_PRIORITY_HIGHEST,             // eHighest
  THREAD_PRIORITY_TIME_CRITICAL        // eTimeCritical
};
//}}}

#define MS_VC_EXCEPTION 0x406d1388
//{{{
typedef struct tagTHREADNAME_INFO
{
    DWORD     dwType;         // must be 0x1000
    LPCSTR    szName;         // pointer to name (in same addr space)
    DWORD     dwThreadID;     // thread ID (-1 caller thread)
    DWORD     dwFlags;        // reserved for future use, most be zero
} THREADNAME_INFO;
//}}}
//{{{
static void _setMSVCThreadName(const DWORD ThreadID, const char * const ThreadName)
{
  if (IsDebuggerPresent() &&
      cThreadManager::singleton().isSettingMSVCThreadName() /* may need use this switch to avoid RaiseException below causing non-MSVC debuggers to trip up! */)
  {
    THREADNAME_INFO info;
    info.dwType     = 0x1000;
    info.szName     = ThreadName;
    info.dwThreadID = ThreadID;
    info.dwFlags    = 0;

    RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(DWORD),  (const ULONG_PTR *)&info);
  }
}
//}}}

//{{{
cThreadImpl::cThreadImpl (iBasicThread* const  IfaceObject,
                          cRefCount* const     IfaceObjectRefCount,
                          const String&        Name,
                          const iBasicThread::ePriority WhichPriority,
                          const unsigned int   InitialStackSize,
                          const eThreadLogging Logging) :
    m_Thread(IfaceObject),
    m_ThreadRefCount(IfaceObjectRefCount),
    m_Name(Name.c_str() /* Thread safe deep copy */),
    m_ThreadHandle(NULL),
    m_Priority(WhichPriority),
    m_InitialStackSize(InitialStackSize),
    m_IsLoopRunning(false),
    m_Logging(Logging),
    m_SignalThreadStopped(true /* initially set? */),
    m_IsCreatingOwnThread(true),
    m_ExitState(eExitState_Ok),
    m_Tib14(IfaceObject, this)
{
}
//}}}
//{{{
cThreadImpl::cThreadImpl (iBasicThread* const IfaceObject,
                          cRefCount* const    IfaceObjectRefCount,
                          const HANDLE        ThreadHandle,
                          const String&       Name) :
    m_Thread(IfaceObject),
    m_ThreadRefCount(IfaceObjectRefCount),
    m_Name(Name.c_str() /* Thread safe deep copy */),
    m_ThreadHandle(ThreadHandle),
    m_Priority(iBasicThread::ePriority_Idle),
    m_InitialStackSize(0),
    m_Logging(eThreadLogging_Collapsed),
    m_SignalThreadStopped(true /* initially set? */),
    m_IsCreatingOwnThread(false),
    m_ExitState(eExitState_Ok),
    m_Tib14(IfaceObject, this)
{

  m_ThreadID = GetCurrentThreadId();
  if (m_ThreadID == 0)
  {
    QFATAL((__FILE__, __LINE__, L"Tried to GetThreadId() - %d", GetLastError()));
  }

  const int Win32Priority = GetThreadPriority (m_ThreadHandle);
  if (Win32Priority == THREAD_PRIORITY_ERROR_RETURN)
  {
    QFATAL((__FILE__, __LINE__, L"Tried to GetThreadPriority() - %d", GetLastError()));
  }

  switch (Win32Priority)
  {
    case THREAD_PRIORITY_IDLE: m_Priority = iBasicThread::ePriority_Idle; break;
    case THREAD_PRIORITY_LOWEST: m_Priority = iBasicThread::ePriority_Lowest; break;
    case THREAD_PRIORITY_BELOW_NORMAL: m_Priority = iBasicThread::ePriority_BelowNormal; break;
    case THREAD_PRIORITY_NORMAL: m_Priority = iBasicThread::ePriority_Normal; break;
    case THREAD_PRIORITY_ABOVE_NORMAL: m_Priority = iBasicThread::ePriority_AboveNormal; break;
    case THREAD_PRIORITY_HIGHEST: m_Priority = iBasicThread::ePriority_Highest; break;
    case THREAD_PRIORITY_TIME_CRITICAL: m_Priority = iBasicThread::ePriority_TimeCritical; break;
    default:
      QFATAL((__FILE__, __LINE__, L"Unexpected Win32Priority %d", Win32Priority));
  }

  // Set the thread with the task pointer.
  const void * const Val = reinterpret_cast<const void * const>(&m_Tib14);
#ifdef _M_X64
  __writegsqword(0x28, (UInt64)Val);
#else
  __asm
  {
    mov eax, Val
    mov fs:[0x14], eax
  }
#endif

  // Tell Visual C debugger about our thread name!
  _setMSVCThreadName(m_ThreadID, narrow(m_Name).c_str());
}
//}}}
//{{{
// ---------------------------------------------------------------------------------------
cThreadImpl::~cThreadImpl()
{
  if (m_IsCreatingOwnThread && m_ThreadHandle) 
    (void)CloseHandle(m_ThreadHandle);
}
//}}}

//{{{
// =======================================================================================
void cThreadImpl::suspend()
{
  cLockGuard threadSafety(&m_ThreadHandleThreadSafety);

  SuspendThread(m_ThreadHandle);
}
//}}}
//{{{
// ---------------------------------------------------------------------------------------
void cThreadImpl::resume()
{
  cLockGuard threadSafety(&m_ThreadHandleThreadSafety);

  ResumeThread(m_ThreadHandle);
}
//}}}

//{{{
void cThreadImpl::_doThreadLoop(iBasicThread * const threadIface, cThreadImpl * const threadImpl)
{
  if (threadImpl->m_Logging == eThreadLogging_Expanded ||
      threadImpl->m_Logging == eThreadLogging_Collapsed)
  {
    const int logCtg = (threadImpl->m_Logging == eThreadLogging_Expanded ? 0 : kCtgThreadRunLoop);
    {
      cLogIndentGuard IndentGuard(logCtg, L"Started thread");
      threadIface->runLoop();
    }
    cLogIndentGuard IndentGuard(logCtg, L"Stopped thread");
  }
  else
  {
    cLogSuppressGuard SuppressGuard;
    threadIface->runLoop();
  }
}
//}}}
//{{{
void QARGS_STACK cThreadImpl::_staticThreadFunction(cThreadImpl * threadImpl)
{
  iBasicThread::Ptr KeepAliveForLoopRunning = threadImpl->m_KeepAliveForLoopRunning;
  threadImpl->m_KeepAliveForLoopRunning.invalidate();

  // Set the thread with the task pointer.
  cTib14 * const tibPtr = &threadImpl->m_Tib14;
#ifdef _M_X64
  __writegsqword(0x28, (UInt64)tibPtr);
#else
  __asm
  {
    mov eax, tibPtr
    mov fs:[0x14], eax
  }
#endif

  // Tell Visual C debugger about our thread name!
  _setMSVCThreadName(threadImpl->m_ThreadID, narrow(threadImpl->m_Name).c_str());

  cThreadManager::singleton().doThreadStartedCallbacks(KeepAliveForLoopRunning);

  _doThreadLoop(threadImpl->m_Thread, threadImpl);

  cThreadManager::singleton().doThreadStoppingCallbacks(KeepAliveForLoopRunning);

  {
    cLockGuard threadSafety(&threadImpl->m_ThreadHandleThreadSafety);
    if (threadImpl->m_ThreadHandle != 0)
    {
      (void)CloseHandle(threadImpl->m_ThreadHandle);
      threadImpl->m_ThreadHandle = NULL;
      threadImpl->m_ThreadID = 0;
    }
  }

  threadImpl->m_SignalThreadStopped.set();

  // Object may be destroyed by this if it is the last pointer.
  KeepAliveForLoopRunning.invalidate();

  // Set thread pointer to zero
#ifdef _M_X64
  __writegsqword(0x28, 0);
#else
  __asm
  {
    mov eax, 0
    mov fs:[0x14], eax
  }
#endif
}
//}}}

//{{{
// =======================================================================================
void QAPI cThreadImpl::startLoop()
{
  if (m_ThreadHandle != 0) // Is already running?
  {
    m_SignalThreadStopped.wait(5000); // Perhaps in process of stopping.... wait a bit to see....
    if (m_ThreadHandle != 0)
    {
      // Perhaps never going to stop!
      QFATAL((__FILE__, __LINE__, L"Thread \"%s\" - loop already started", m_Name.c_str()));
    }
  }

  {
    cLockGuard threadSafety(&m_ExitStateThreadSafety);
    m_ExitState = eExitState_StillRunning;
    m_ExitDesc  = L"";
  }

  cLockGuard threadSafety(&m_ThreadHandleThreadSafety);

  m_SignalThreadStopped.reset();

  // As long as this pointer is valid the object will be alive and kicking. Only when
  // stopLoop() is called explicitly will the pointer be invalidated and the object free
  // to destroy.
  m_KeepAliveForLoopRunning = iBasicThread::Ptr(m_Thread, m_ThreadRefCount);

  // Start up the thread - use _beginthreadex in order to get thread id.
  m_ThreadID = 0;
  m_ThreadHandle = CreateThread(
    0,
    m_InitialStackSize,
    (LPTHREAD_START_ROUTINE)_staticThreadFunction,
    (void *)this,
    CREATE_SUSPENDED,
    (unsigned long *)&m_ThreadID);
  if (m_ThreadHandle == 0)
  {
    QFATAL((__FILE__, __LINE__, L"Tried to CreateThread() - %d", GetLastError()));
  }

  if (!SetThreadPriority(m_ThreadHandle, _Win32Priorities[m_Priority]))
  {
    QFATAL((__FILE__, __LINE__, L"Tried to SetThreadPriority() - %d", GetLastError()));
  }

  if (ResumeThread(m_ThreadHandle) == -1)
  {
    QFATAL((__FILE__, __LINE__, L"Tried to ResumeThread() - %d", GetLastError()));
  }
}
//}}}
//{{{
// ---------------------------------------------------------------------------------------
void QAPI cThreadImpl::stopLoop()
{
  HANDLE CopyOfThreadHandle;
  {
    cLockGuard threadSafety(&m_ThreadHandleThreadSafety);
    CopyOfThreadHandle = m_ThreadHandle;
  }

  if (CopyOfThreadHandle)
  {
    m_Thread->signalLoopStop();
    m_SignalThreadStopped.waitForever();
  }
}
//}}}

//{{{
// =======================================================================================
iWaitable::eWakeupReason QAPI cThreadImpl::waitLoopComplete(const unsigned long TimeoutMs)
{
  HANDLE CopyOfThreadHandle;
  {
    cLockGuard threadSafety(&m_ThreadHandleThreadSafety);
    CopyOfThreadHandle = m_ThreadHandle;
  }

  iWaitable::eWakeupReason Reason = iWaitable::eWaitResult_Set;
  if (CopyOfThreadHandle)
  {
    if (m_SignalThreadStopped.wait(TimeoutMs) == cManualResetEvent::eTimedOut) 
      Reason = iWaitable::eWaitResult_TimedOut;
  }

  return Reason;
}
//}}}

//{{{
// =======================================================================================
void cThreadImpl::setPriority(const iBasicThread::ePriority NewPriority)
{
  cLockGuard threadSafety(&m_ThreadHandleThreadSafety);

  if (SetThreadPriority(m_ThreadHandle, _Win32Priorities[NewPriority]) == 0)
  {
    QFATAL((__FILE__, __LINE__, L"Tried to SetThreadPriority()"));
  }

  m_Priority = NewPriority;
}
//}}}
//{{{
iBasicThread::ePriority cThreadImpl::getPriority() const
{
  return m_Priority;
}
//}}}

//{{{
// =======================================================================================
String cThreadImpl::getName() const
{
  return m_Name;
}
//}}}
//{{{
// =======================================================================================
unsigned long cThreadImpl::getThreadID() const
{
  return m_ThreadID;
}
//}}}
//{{{
// =======================================================================================
iBasicThread::eExitState QAPI cThreadImpl::getExitState(String * const desc) const
{
  cLockGuard threadSafety(&m_ExitStateThreadSafety);

  *desc = m_ExitDesc;
  return m_ExitState;
}
//}}}
