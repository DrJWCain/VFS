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
#include "cDebugDriver.h"
#include "..\..\..\kernel\KernelExec\cThreadImpl.h" // So we can peek thread name
#include "..\..\..\kernel\KernelExec\cThreadManager.h"
#include "..\..\..\kernel\KernelExec\cExceptionManager.h"

using namespace vfs;


namespace
{

// =======================================================================================
// === Unhandled Exception ===============================================================
// =======================================================================================

  //{{{
  // =======================================================================================
  // === _onUnhandledException =============================================================
  // =======================================================================================

  LONG __stdcall _onUnhandledException(EXCEPTION_POINTERS* ExceptionPtrs)
  {
    // IMPORTANT - This can be called from _any_ thread!
    cThreadManagerEx& tm = cThreadManagerEx::singleton();
    return tm.unhandledException(ExceptionPtrs);
  }

  //}}}
  //#define MS_VISUALC_EXCEPTION ((DWORD)0xE06D7363L)
}

//{{{
LONG __stdcall cThreadManagerEx::unhandledException( const EXCEPTION_POINTERS* const ExceptionPtrs )
{
  //QTRACE((L"cThreadManagerEx::unhandledException"));
  if (ExceptionPtrs->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT)
  {
    QSOS((L"Debug break (int3) trapped - passing exception on for \"Just-In-Time Debugging\""));
    return EXCEPTION_CONTINUE_SEARCH;
  }
  else
  {
    iExceptionManager::cFatalDiagnostics fatalDiagnostics( EXIT_FAILURE, "<UNKNOWN>", 0, L"Fatal Error", L"cThreadManagerEx::unhandledException" );
    iDebugDriver::Ptr debugDriver = cExceptionManager::singleton().debugDriver();
    if (debugDriver.isValid())
      debugDriver->getFaultReason( GetCurrentThreadId(), ExceptionPtrs, fatalDiagnostics);

    iLogManager::bufferError(L"%s trapped in thread %d", fatalDiagnostics.m_Msg.c_str(), GetCurrentThreadId());
    //QTRACE((L"File: %s", fatalDiagnostics.m_SrcFilename.c_str()));
    //QTRACE((L"Line: %d", fatalDiagnostics.m_SrcLineNumber));

    // Info out to debugger ASAP.
    //OutputDebugString(L"*** Fatal Error ***\n{\n  ");
    //OutputDebugString(message);
    //OutputDebugString(L"\n");
    if (fatalDiagnostics.m_SrcLineNumber)
    {
      //OutputDebugString(L"  File: ");
      //OutputDebugString(file);
      //OutputDebugString(L"\n");
      Char lineStr[32];
      wsprintf(lineStr, L"  Line: %d\n", fatalDiagnostics.m_SrcLineNumber);
      //OutputDebugString(lineStr);
    }
    //OutputDebugString(L"}\n");

    // Breakpoint
    if (IsDebuggerPresent()) DebugBreak();

    if(stackDumpThread->getThreadID() != GetCurrentThreadId())
    {
      //QTRACE((L"Sending exception to stack dump thread"));
      stackDumpThread->pushException(GetCurrentThreadId(), ExceptionPtrs); // Never returns
    }
    else
    {
      static bool HasAlreadyCrashedInCrashHandler = false;
      if (HasAlreadyCrashedInCrashHandler)
      {
        QSOS((L"Crashed in unhandled exception handler again!"));
        Char TmpBuffer[1024];
        (void)_snwprintf(
          TmpBuffer,
          sizeof(TmpBuffer) / sizeof(Char),
          fatalDiagnostics.m_SrcLineNumber ?
            L"Unhandled Exception whilst handling Unhandled Exception!\n\n%s\n\nFile: %s\nLine: %d" :
            L"Unhandled Exception whilst handling Unhandled Exception!",
          fatalDiagnostics.m_Msg, fatalDiagnostics.m_SrcFilename, fatalDiagnostics.m_SrcLineNumber);
        (void)MessageBox(NULL, TmpBuffer, cExceptionManager::singleton().getDialogTitle().c_str(), MB_ICONERROR | MB_TOPMOST | MB_SETFOREGROUND | MB_APPLMODAL);
        (void)TerminateProcess(GetCurrentProcess(), 0);
      }
      else
      {
        HasAlreadyCrashedInCrashHandler = true;
        QSOS((L"Crashed in unhandled exception handler"));
        // SJSJSJ Can't use pushException() here because we probably crashed with the message queue lock already taken.
        cThreadManagerEx::singleton().handleException(GetCurrentThreadId(), ExceptionPtrs); // Never returns
      }
    }

    return EXCEPTION_EXECUTE_HANDLER;
  }
}
//}}}
//{{{
void cThreadManagerEx::handleException(DWORD ThreadID, const EXCEPTION_POINTERS* const ExceptionPtrs)
{
  //QTRACE((L"cThreadManagerEx::handleException"));

  iExceptionManager::cFatalDiagnostics fatalDiagnostics( EXIT_FAILURE, "<UNKNOWN>", 0, L"Fatal Error", L"cThreadManagerEx::handleException" );
  iDebugDriver::Ptr debugDriver = cExceptionManager::singleton().debugDriver();
  if (debugDriver.isValid())
    debugDriver->getFaultReason( ThreadID, ExceptionPtrs, fatalDiagnostics);

  HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, ThreadID);
  if (hThread)
  {
    //QTRACE((L"Opened thread"));

    cLogIndentSOSGuard IndentGuard(L"*** Call Stack for thread %d (%08x) ***", ThreadID, ThreadID);

    //{
      //cLogIndentGuard IndentGuard(0, L"Bytes on stack", ThreadID);
      //StackBytes(hThread,ExceptionPtrs);
    //}

    {
      //cLogIndentGuard IndentGuard(0, L"Calls on stack", ThreadID);
      if (ExceptionPtrs->ContextRecord == 0)
      {
        QMSG((0, L"Unable to proceed - ContextRecord is missing in EXCEPTION_POINTERS!"));
      }
      else
      {
        //QTRACE((L"cThreadManager.cpp line 280"));
        cDebugDriver::Ptr  debugDriver = ptr_cast<cDebugDriver::Ptr>(cExceptionManager::singleton().debugDriver());
        if (debugDriver.isValid())
          debugDriver->logCallStack (ThreadID, ExceptionPtrs);

      }
    }
    CloseHandle(hThread);
   }
  else
  {
    QSOS((L"Failed to OpenThread() - GetLastError() returned %d", GetLastError()));
  }

  iExceptionManager::singleton().fatal( fFatal_IsAnError | (fFatal_ShowAll & ~fFatal_ShowStack), // show everything except stack
                                        fatalDiagnostics );
}
//}}}

// =======================================================================================
// === _instance =========================================================================
// =======================================================================================

cThreadManagerEx* cThreadManagerEx::_instance = 0;

LPTOP_LEVEL_EXCEPTION_FILTER cThreadManagerEx::PrevFilter = 0;


// =======================================================================================
// === singleton =========================================================================
// =======================================================================================

#pragma warning (disable: 4786)

//{{{
cThreadManagerEx::cThreadManagerEx()
: stackDumpThread(new cDumpStacksThread)
{
  stackDumpThread->startLoop();
}

//}}}
//{{{
bool QAPI cThreadManagerEx::resetUnhandledExceptionFilter()
{
  const LPTOP_LEVEL_EXCEPTION_FILTER prevFilter = SetUnhandledExceptionFilter(_onUnhandledException);
  //return prevFilter != _onUnhandledException;
  if (prevFilter != _onUnhandledException)
  {
    //QSOS ((L"prevFilter != _onUnhandledException, %p != %p", prevFilter, _onUnhandledException));
    //return true;
  }

  return false;
}
//}}}

//{{{
// =======================================================================================
// === addNewThread ======================================================================
// =======================================================================================

void cThreadManagerEx::addNewThread()
{
  // Protect member variables from changes by another thread.
  cLockGuard MemberGuard(&m_ThreadListLock);

  thread_lookup.push_back(GetCurrentThreadId());
}




//}}}
//{{{
// =======================================================================================
// === removeOldThread ===================================================================
// =======================================================================================

void cThreadManagerEx::removeOldThread()
{
  // Protect member variables from changes by another thread.
  cLockGuard MemberGuard(&m_ThreadListLock);

  THREADS::iterator it = std::find(thread_lookup.begin(),thread_lookup.end(),GetCurrentThreadId());
  if(it != thread_lookup.end())
  {
    thread_lookup.erase(it);
  }
}


//}}}

//{{{
// =======================================================================================
// === getAllThreads =====================================================================
// =======================================================================================

std::list<DWORD> cThreadManagerEx::getAllThreads() const
{
  THREADS copy_of_thread_lookup;
  {
    cLockGuard MemberGuard(&m_ThreadListLock);
    copy_of_thread_lookup = thread_lookup;
  }
  return copy_of_thread_lookup;
}



//}}}

//{{{
// =======================================================================================
// === logAllThreadsStacks ===============================================================
// =======================================================================================

void cThreadManagerEx::logAllThreadsStacks()
{
  THREADS copy_of_thread_lookup;
  {
    cLockGuard MemberGuard(&m_ThreadListLock);
    copy_of_thread_lookup = thread_lookup;
  }

  for(THREADS::iterator it = copy_of_thread_lookup.begin(); it != copy_of_thread_lookup.end(); ++it)
  {
    logCallStack(*it);
  }
}




//}}}
//{{{
// =======================================================================================
// === logAllThreadsTiming ===============================================================
// =======================================================================================

void cThreadManagerEx::logAllThreadsTiming(const bool IsPercentages)
{
  THREADS copy_of_thread_lookup;
  {
    cLockGuard MemberGuard(&m_ThreadListLock);
    copy_of_thread_lookup = thread_lookup;
  }

  multimap<unsigned int, cAboutThreadTiming::ConstPtr> SortedByCpuUsage;

  unsigned int TotalDiffUserTime = 0;
  unsigned int TotalDiffKernelTime = 0;
  for(THREADS::iterator it = copy_of_thread_lookup.begin(); it != copy_of_thread_lookup.end(); ++it)
  {
    {
      ThreadToTimeMap::iterator i(KernelTimesByThread.find(*it));
      if (i == KernelTimesByThread.end()) KernelTimesByThread.insert(std::make_pair(*it, cTime()));
    }
    {
      ThreadToTimeMap::iterator i(UserTimesByThread.find(*it));
      if (i == UserTimesByThread.end()) UserTimesByThread.insert(std::make_pair(*it, cTime()));
    }

    cAboutThreadTiming::ConstPtr ThisThreadsTiming = getThreadTiming(*it, &KernelTimesByThread.find(*it)->second, &UserTimesByThread.find(*it)->second);

    if (ThisThreadsTiming.isValid())
    {
      TotalDiffKernelTime += ThisThreadsTiming->m_DiffKernelTimeMs;
      TotalDiffUserTime += ThisThreadsTiming->m_DiffUserTimeMs;

      SortedByCpuUsage.insert(make_pair((unsigned int)(ThisThreadsTiming->m_DiffKernelTimeMs + ThisThreadsTiming->m_DiffUserTimeMs), ThisThreadsTiming));
    }
  }

  QMSG((0, L"Name---------------------------------------------------  ThreadID  Pri  KernelChange--  UserChange----  TotalChange---"));
  QMSG((0, L""));
  for (multimap<unsigned int, cAboutThreadTiming::ConstPtr>::reverse_iterator k = SortedByCpuUsage.rbegin(); k != SortedByCpuUsage.rend(); ++k)
  {
    if (IsPercentages)
    {
      QMSG((
        0,
        L"%-55s    %4d    % 3d      %3d%%            %3d%%            %3d%%",
        k->second->m_ThreadName.c_str(), k->second->m_ThreadID, k->second->m_ThreadPriority,
        (TotalDiffKernelTime > 0) ? (k->second->m_DiffKernelTimeMs * 100) / TotalDiffKernelTime : 0,
        (TotalDiffUserTime > 0) ? (k->second->m_DiffUserTimeMs * 100) / TotalDiffUserTime : 0,
        TotalDiffKernelTime + TotalDiffUserTime > 0 ? (((k->second->m_DiffKernelTimeMs + k->second->m_DiffUserTimeMs) * (Int64)100) / (TotalDiffKernelTime + TotalDiffUserTime)) : 0));
    }
    else
    {
      QMSG((
        0,
        L"%-55s    %4d    % 3d      %4d            %4d            %4d",
        k->second->m_ThreadName.c_str(), k->second->m_ThreadID, k->second->m_ThreadPriority,
        k->second->m_DiffKernelTimeMs,
        k->second->m_DiffUserTimeMs,
        k->second->m_DiffKernelTimeMs + k->second->m_DiffUserTimeMs));
    }
  }
}





//}}}
//{{{
// =======================================================================================
// === logThreadStack ====================================================================
// =======================================================================================

void cThreadManagerEx::logCallStack(DWORD ThreadID)
{
  logCallStack(ThreadID, L"");
}

void cThreadManagerEx::logCallStack(DWORD ThreadID, const String & AfterFunc)
{
   QSOS((L"cThreadManagerEx::logCallStack(DWORD ThreadID, const String & AfterFunc) - not available"));
}



//}}}

//{{{
// =======================================================================================
// === logThreadTiming ===================================================================
// =======================================================================================

cThreadManagerEx::cAboutThreadTiming::Ptr cThreadManagerEx::getThreadTiming(DWORD ThreadID, cTime * const PrevKernelTime, cTime * const PrevUserTime)
{
  QSOS((L"Stubbed %S", __FUNCTION__));

  cThreadManagerEx::cAboutThreadTiming::Ptr Result;
  return Result;
}



//}}}

//{{{
// =======================================================================================
// === cDumpStacksThread =================================================================
// =======================================================================================

cDumpStacksThread::cDumpStacksThread()
: cThread(kSpecialThread,eHighest), SignalDelivered(false /* initially set? */)
{
}
//}}}

//{{{
class cException : public iMsgQueue::iMsg, public cRefCount
{
  const DWORD ThreadID;
  const EXCEPTION_POINTERS* const Ptrs;
public:
  cException( DWORD threadID, const EXCEPTION_POINTERS* const ptrs)
    : ThreadID(threadID), Ptrs(ptrs)
  {
  }
  void QAPI deliver2()
  {
    cThreadManagerEx::singleton().handleException(ThreadID,Ptrs);
  }
};

//}}}
//{{{
void cDumpStacksThread::pushException (DWORD ThreadID, const EXCEPTION_POINTERS* const ExceptionPtrs)
{
  cPtr<cException> p = new cException(ThreadID,ExceptionPtrs);
  postMessage(p);
  SignalDelivered.waitForever();
}

//}}}
//{{{
void QAPI cDumpStacksThread::signalLoopStop()
{
}

//}}}
//{{{
void QAPI cDumpStacksThread::runLoop()
{
  while(1)
  {
    waitMessageForever();
    if(isMessageWaiting())
    {
      iMsgQueue::iMsg::Ptr msg = getMessage();
      if(msg->isQuitMsg())
      {
        break;
      }
      msg->deliver2();
      SignalDelivered.set();
    }
  }
}
//}}}

