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

#include <QKernelExec.h>

using namespace vfs;




// =======================================================================================
// === cThreadManagerEx ====================================================================
// =======================================================================================

const String kSpecialThread = L"DEBUGDRIVER";
class cDumpStacksThread : public cThread
{
public:
  cDumpStacksThread();
  void pushException(DWORD ThreadID,const EXCEPTION_POINTERS* const ExceptionPtrs);
  void QAPI signalLoopStop() throw();

protected:
  void QAPI runLoop();

private:
  cAutoResetEvent SignalDelivered;
};

class cThreadManagerEx : public iManager, public cRefCount
{
  public:

    QDEFINE_SINGLETON_IMPL(cThreadManagerEx, L"{2EABCB2F-E8FD-4B20-A2E2-BE35095A4C59} // cThreadManagerEx", iManager);

    cThreadManagerEx();

    // iManager
    void QAPI logSummary() {};
    void QAPI logDetailedSummary() {};

    void addNewThread();
    void removeOldThread();
    void logAllThreadsStacks();
    void logAllThreadsTiming(const bool IsPercentages);
    void logCallStack(unsigned long ThreadID);
    void logCallStack(unsigned long ThreadID, const String & AfterFunc);
    class cAboutThreadTiming : public cRefCount
    {
      public :
        QDEFINE_SMARTPTR(cAboutThreadTiming);
        String m_ThreadName;
        DWORD  m_ThreadID;
        int    m_ThreadPriority;
        int    m_SuspendCount;
        unsigned int  m_DiffKernelTimeMs;
        unsigned int  m_DiffUserTimeMs;
        cAboutThreadTiming(
          const String & ThreadName,
          const DWORD    ThreadID,
          const int      ThreadPriority,
          const int      SuspendCount,
          const cTime &  DiffKernelTime,
          const cTime &  DiffUserTime) :
            m_ThreadName(ThreadName),
            m_ThreadID(ThreadID),
            m_ThreadPriority(ThreadPriority),
            m_SuspendCount(SuspendCount),
            m_DiffKernelTimeMs((unsigned int)DiffKernelTime.getDurationInMillisecs()),
            m_DiffUserTimeMs((unsigned int)DiffUserTime.getDurationInMillisecs())
        {
        }
    };
    cAboutThreadTiming::Ptr getThreadTiming(DWORD ThreadID, cTime * const PrevKernelTime, cTime * const PrevUserTime);
    std::list<DWORD> getAllThreads() const;
  
    LONG __stdcall unhandledException(const EXCEPTION_POINTERS* const ExceptionPtrs);
    void handleException(DWORD ThreadID,const EXCEPTION_POINTERS* const ExceptionPtrs);

    bool QAPI resetUnhandledExceptionFilter();

  private:

    mutable vfs::cLock    m_ThreadListLock;

    typedef std::map<unsigned long, cTime> ThreadToTimeMap;
    ThreadToTimeMap           KernelTimesByThread;
    ThreadToTimeMap           UserTimesByThread;

    typedef std::list<DWORD>  THREADS;
    THREADS                   thread_lookup;
    static cThreadManagerEx *   _instance;
    static LPTOP_LEVEL_EXCEPTION_FILTER PrevFilter;

    bool m_IsDumpingStackOnException;

    cPtr<cDumpStacksThread> stackDumpThread;
};

