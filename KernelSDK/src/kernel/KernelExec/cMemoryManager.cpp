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
#include "cMemoryManager.h"
#include "cFixedSizeAllocator.h"
#include "cSystemInfo.h"
#include "LogCategories.h"

using namespace vfs;




// =======================================================================================
// === cMemWatchThread ===================================================================
// =======================================================================================

class cMemWatchThread : public cBasicThread
{
  public :

    QDEFINE_SMARTPTR(cMemWatchThread);

    cMemWatchThread(const unsigned int LowMemLimit, const unsigned int VeryLowMemLimit) :
      cBasicThread(L"Memory watch", iBasicThread::ePriority_Idle),
      m_HasSignalled(false),
      m_LowMemLimit(LowMemLimit),
      m_VeryLowMemLimit(VeryLowMemLimit)
    {
    }

    void QAPI signalLoopStop()
    {
      // Signal loop to exit.
      m_HasSignalled = true;
    }

    void QAPI runLoop()
    {
      unsigned int Delay = 3000;

      while (!m_HasSignalled)
      {
        Sleep(Delay);

        MEMORYSTATUS stat;
        GlobalMemoryStatus(&stat);
        if (stat.dwAvailPhys < m_VeryLowMemLimit && m_VeryLowMemLimit != 0)
        {
#if 0        
          iExceptionManager::singleton().presentWarning(
            cNoRoom(__FILE__, __LINE__, L"Available physical memory less than very low limit of %dMB", m_VeryLowMemLimit / (1024 * 1024)),
            L"Low memory - only %dMB left!", stat.dwAvailPhys / (1024 * 1024));
#else
          QSOS((L"Available physical memory less than very low limit of %dMB", m_VeryLowMemLimit / (1024 * 1024)));
#endif
          Delay = 30000;
        }
        else if (stat.dwAvailPhys < m_LowMemLimit && m_LowMemLimit != 0)
        {
          QSOS((L"Performance warning - low memory - only %IuMB left!", stat.dwAvailPhys / (1024 * 1024)));
          Delay = 15000;
        }
      }
    }

  private :

    bool          m_HasSignalled;
    unsigned int  m_LowMemLimit;
    unsigned int  m_VeryLowMemLimit;
};



// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

typedef WINBASEAPI BOOL (WINAPI * GetProcessHandleCountFuncPtr)(IN HANDLE hProcess, OUT PDWORD pdwHandleCount);

class cResourceLeakDetector : public iTimerManager::iOneSecondTimerCallback, public cRefCount
{
  public :
    cResourceLeakDetector(GetProcessHandleCountFuncPtr funcPtr) : m_FuncPtr(funcPtr), m_LastTime(0) {}
    virtual void onOneSecondElapsed(const Int64 millisecsElapsedSinceStart)
    {
      // every 5 seconds
      if (millisecsElapsedSinceStart - m_LastTime > 5000)
      {
        DWORD count = 0;
        if (!m_FuncPtr(GetCurrentProcess(), &count))
        {
          const DWORD lastError = GetLastError();
          QFATAL((__FILE__, __LINE__, L"Tried to GetProcessHandleCount - returned %d (%s)", lastError, win32ErrorToString(lastError).c_str()));
        }
        if (count > 10000)
        {
          QMSG((kCtgHandleLeakWarning, L"MemoryManager: Possible resource leak detected! (%d handles allocated)", count));
        }
        m_LastTime = millisecsElapsedSinceStart;
      }
    }
    GetProcessHandleCountFuncPtr m_FuncPtr;
    Int64 m_LastTime;
};

cMemoryManager::cMemoryManager()
{
  _initHeap();

  iRegistryHelp::Ptr rh = createRegistryHelp();
  const unsigned int LowMemLimit     = rh->getItemNumber(cSystemInfo::singleton().getKernelExecRegistryPath() + L"Custom Settings\\Memory\\", L"Low memory limit (MB, 0 = no warning)") * 1024 * 1024;
  const unsigned int VeryLowMemLimit = rh->getItemNumber(cSystemInfo::singleton().getKernelExecRegistryPath() + L"Custom Settings\\Memory\\", L"Very low memory limit (MB, 0 = no warning)") * 1024 * 1024;
  QVERIFY(VeryLowMemLimit <= LowMemLimit);

  if (VeryLowMemLimit != 0 && LowMemLimit != 0)
  {
     m_Unwind.push_back(iThreadManager::singleton().startThread(new cMemWatchThread(LowMemLimit, VeryLowMemLimit)));
  }

  // WinXP or better!
  GetProcessHandleCountFuncPtr funcPtr = (GetProcessHandleCountFuncPtr)GetProcAddress(LoadLibrary(L"Kernel32.dll"), "GetProcessHandleCount");
  if (funcPtr)
  {
    m_Unwind.push_back(iTimerManager::singleton().addOneSecondTimerCallback(new cResourceLeakDetector(funcPtr)));
  }
}




// =======================================================================================
// === getFastIOAlignment ================================================================
// =======================================================================================

unsigned int QAPI cMemoryManager::getFastIOAlignment() const
{
  return 4096;
}




// =======================================================================================
// === createFixedSizeAllocator ==========================================================
// =======================================================================================

iMemoryAllocator::Ptr QAPI cMemoryManager::createFixedSizeAllocator(
  const unsigned int NumBytesPerObject,
  const unsigned int NumObjectsPerPool,
  const unsigned int Alignment) throw(cNoRoom)
{
  return new cFixedSizeAllocator2(NumBytesPerObject, NumObjectsPerPool, Alignment);
}

// ---------------------------------------------------------------------------------------

iMemoryAllocator::Ptr QAPI cMemoryManager::createFixedSizeAllocator(
  const unsigned int NumBytesPerObject,
  const unsigned int NumObjectsPerPool) throw(cNoRoom)
{
  return new cFixedSizeAllocator2(NumBytesPerObject, NumObjectsPerPool, 8);
}




// =======================================================================================
// === _initHeap =========================================================================
// =======================================================================================

void cMemoryManager::_initHeap()
{
  #if defined(_DEBUG)

    // Hook in our client-defined reporting function.
    // Every time a _CrtDbgReport is called to generate a debug report, our function
    // will get called first.
//    _CrtSetReportHook(_CrtReportingFunction);

    // _CRTDBG_ALLOC_MEM_DF
    // * Enable debug heap allocations and use of memory block type identifiers, such
    //   as _CLIENT_BLOCK
    // _CRTDBG_DELAY_FREE_MEM_DF
    // * Keep freed memory blocks in the heap's linked list, assign them the _FREE_BLOCK
    //   type, and fill them with the byte value 0xDD.
    // _CRTDBG_LEAK_CHECK_DF
    // * Perform automatic leak checking at program exit via a call to
    //   _CrtDumpMemoryLeaks and generate an error report if the application failed to
    //   free all the memory it allocated.
    // _CRTDBG_CHECK_CRT_DF
    // * Include _CRT_BLOCK types in leak detection and memory state difference
    //   operations.
    // _CRTDBG_CHECK_ALWAYS_DF
    // * Call _CrtCheckMemory at every allocation and deallocation request.
    (void)_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);// | _CRTDBG_CHECK_ALWAYS_DF);

//    _CrtMemCheckpoint(&m_InitialHeapState);

  #endif
}




// =======================================================================================
// === _initEmptiness ====================================================================
// =======================================================================================

cMemoryView::ConstPtr cMemoryManager::_initEmptiness(const size_t numBytes) const
{
  QVERIFYMSG(numBytes < 4 * 1024 * 1024, (L"Limited to 4MB of emptiness - %Iu bytes requested, is this correct?", numBytes));

  cLockGuard ThreadSafety(&m_EmptinessThreadSafety);

  if (!m_Emptiness.isValid())
  {
    m_Emptiness = new cMemoryView(new cMemory(numBytes, cMemory::eHeap, 16));
    (void)memset(m_Emptiness->getBytes(), 0, m_Emptiness->getSize());
  }
  else
  {
    if (m_Emptiness->getSize() < numBytes)
    {
      // DO NOT REALLOC m_Emptiness - someone could be DMAing async with it!
      m_Emptiness = new cMemoryView(new cMemory(numBytes, cMemory::eHeap, 16));
      (void)memset(m_Emptiness->getBytes(), 0, m_Emptiness->getSize());
    }
  }

  return m_Emptiness;
}




// =======================================================================================
// === getEmptiness ======================================================================
// =======================================================================================

cMemoryView::ConstPtr QAPI cMemoryManager::getEmptiness(const size_t numBytes) const
{
  cMemoryView::ConstPtr result = _initEmptiness(numBytes)->firstConst(numBytes);
#if 0
  // Test to make sure the emptiness really is empty
  {
    const int * p = (const int*)result->getConstBytes();
    unsigned int c = result->getSize() / 4;
    while (c > 0)
    {
      QVERIFY(*p == 0);
      p++;
      c--;
    }
  }
#endif
  return result;
}
