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
#include "QKernelBase.h"
#include "fatal.h"

using namespace vfs;




// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

cManualResetEvent::cManualResetEvent(const bool IsInitiallySet) throw(cOSError)
{
  m_hEvent = (void *)CreateEvent(NULL /* attribute */, TRUE /* manual reset? */, IsInitiallySet /* initial state */, NULL /* name */);
  const DWORD LastError = GetLastError();
  QKERNELBASEVERIFY(m_hEvent != 0);
  if (m_hEvent == 0) throw cOSError(__FILE__, __LINE__, L"CreateEvent() failed - %d", LastError);
}

// ---------------------------------------------------------------------------------------

/*
IMPORTANT - Name should be unique unless you want to open an existing event!
*/
cManualResetEvent::cManualResetEvent(const bool IsInitiallySet, const String & Name) throw(cOSError) :
  m_Name(Name)
{
  m_hEvent = (void *)CreateEvent(NULL /* attribute */, TRUE /* manual reset? */, IsInitiallySet /* initial state */, m_Name.c_str() /* name */);
  const DWORD LastError = GetLastError();
  QKERNELBASEVERIFY(m_hEvent != 0);
  if (m_hEvent == 0) throw cOSError(__FILE__, __LINE__, L"CreateEvent() failed - %d", LastError);
}

// ---------------------------------------------------------------------------------------

cManualResetEvent::~cManualResetEvent()
{
  if (m_hEvent != NULL) CloseHandle(m_hEvent);
}




// =======================================================================================
// === getWin32EventHandle ================================================================
// =======================================================================================

HANDLE QAPI cManualResetEvent::getWin32EventHandle() const
{
  return m_hEvent;
}




// =======================================================================================
// === wait/waitForever ==================================================================
// =======================================================================================

cManualResetEvent::eWaitResult QAPI cManualResetEvent::wait(const unsigned int timeoutMs) const
{
  eWaitResult Result = eWaitResult_TimedOut;

  DWORD Win32Result;
  Win32Result = WaitForSingleObject(m_hEvent, timeoutMs);
  switch (Win32Result)
  {
    case WAIT_ABANDONED :
    {
      // The specified object is a mutex object that was not released by the thread that
      // owned the mutex object before the owning thread terminated. Ownership of the mutex
      // object is granted to the calling thread, and the mutex is set to nonsignaled.

      // Abandoned WaitForSingleObject() because thread exited prematurely
      QKERNELBASEVERIFY(false);
    }
    case WAIT_OBJECT_0 :
    {
      // The state of the specified object is signaled.
      Result = eSignalled;
      break;
    }
    case WAIT_TIMEOUT :
    {
      // The time-out interval elapsed, and the object's state is nonsignaled.
      break;
    }
    case WAIT_FAILED :
    {
      // Tried to WaitForSingleObject()
      QKERNELBASEVERIFY(false);
    }
    default :
    {
      // Unexpected result
      QKERNELBASEVERIFY(false);
    }
  }

  return Result;
}

// ---------------------------------------------------------------------------------------

void QAPI cManualResetEvent::waitForever() const
{
  wait(INFINITE);
}




// =======================================================================================
// === set/reset =========================================================================
// =======================================================================================

void QAPI cManualResetEvent::set()
{
  (void)SetEvent(m_hEvent);
}

// ---------------------------------------------------------------------------------------

void QAPI cManualResetEvent::reset()
{
  (void)ResetEvent(m_hEvent);
}




// =======================================================================================
// === getName ===========================================================================
// =======================================================================================

String QAPI cManualResetEvent::getName() const throw(cNotFound)
{
  if (m_Name == L"") throw cNotFound(__FILE__, __LINE__, L"Event has no name");
  return m_Name;
}
