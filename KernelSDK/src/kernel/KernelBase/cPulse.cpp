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
**/

#include "stdafx.h"
#include "QKernelBase.h"
#include "fatal.h"

using namespace vfs;




// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

cPulse::cPulse() throw(cOSError)
{
  m_hEvent = (void *)CreateEvent(NULL /* attribute */, TRUE /* manual reset? */, FALSE /* initial state */, NULL /* name */);
  const DWORD LastError = GetLastError();
  QKERNELBASEVERIFY(m_hEvent != 0);
  if (m_hEvent == 0) throw cOSError(__FILE__, __LINE__, L"CreateEvent() failed - %d", LastError);
}

// ---------------------------------------------------------------------------------------

cPulse::cPulse(const String & Name) throw(cOSError) :
  m_Name(Name)
{
  m_hEvent = (void *)CreateEvent(NULL /* attribute */, TRUE /* manual reset? */, FALSE /* initial state */, m_Name.c_str() /* name */);
  const DWORD LastError = GetLastError();
  QKERNELBASEVERIFY(m_hEvent != 0);
  if (m_hEvent == 0) throw cOSError(__FILE__, __LINE__, L"CreateEvent() failed - %d", LastError);
}

// ---------------------------------------------------------------------------------------

cPulse::~cPulse()
{
  if (m_hEvent != NULL) CloseHandle ((HANDLE) m_hEvent);
}




// =======================================================================================
// === wait ==============================================================================
// =======================================================================================

cPulse::eWaitResult QAPI cPulse::wait(const unsigned int timeoutMs) const
{
  eWaitResult Result = eWaitResult_TimedOut;

  DWORD Win32Result;
  Win32Result = WaitForSingleObject((HANDLE)m_hEvent, timeoutMs);
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




// =======================================================================================
// === getWin32EventHandle ===============================================================
// =======================================================================================

HANDLE QAPI cPulse::getWin32EventHandle() const
{
  return m_hEvent;
}




// =======================================================================================
// === waitForever =======================================================================
// =======================================================================================

void QAPI cPulse::waitForever() const
{
  wait(INFINITE);
}



// =======================================================================================
// === set ===============================================================================
// =======================================================================================

void QAPI cPulse::set()
{
  (void)PulseEvent((HANDLE)m_hEvent);
}

// ---------------------------------------------------------------------------------------

void QAPI cPulse::reset()
{
  // nop
}




// =======================================================================================
// === getName ===========================================================================
// =======================================================================================

String QAPI cPulse::getName() const throw(cNotFound)
{
  if (m_Name == L"") throw cNotFound(__FILE__, __LINE__, L"Pulse has no name");
  return m_Name;
}
