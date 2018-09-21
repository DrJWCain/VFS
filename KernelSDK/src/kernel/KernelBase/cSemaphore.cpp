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

cSemaphore::cSemaphore() :
  m_hSemaphore(NULL),
  m_InitialNumThreadsPermitted(0),
  m_MaxNumThreadsPermitted(kMaxSemaphoreThreadsPermitted)
{
  reset();
}


cSemaphore::cSemaphore(const unsigned int InitialNumThreadsPermitted, const unsigned int MaxNumThreadsPermitted) throw(cOSError) :
  m_hSemaphore(NULL),
  m_InitialNumThreadsPermitted(InitialNumThreadsPermitted),
  m_MaxNumThreadsPermitted(MaxNumThreadsPermitted)
{
  QKERNELBASEVERIFY(m_InitialNumThreadsPermitted <= kMaxSemaphoreThreadsPermitted);
  QKERNELBASEVERIFY(m_InitialNumThreadsPermitted <= MaxNumThreadsPermitted);
  QKERNELBASEVERIFY(m_MaxNumThreadsPermitted > 0);
  QKERNELBASEVERIFY(m_MaxNumThreadsPermitted <= kMaxSemaphoreThreadsPermitted);
  reset();
}

// ---------------------------------------------------------------------------------------

cSemaphore::cSemaphore(
  const unsigned int InitialNumThreadsPermitted,
  const unsigned int MaxNumThreadsPermitted,
  const String &     Name) throw(cOSError) :
    m_hSemaphore(NULL),
    m_Name(Name),
    m_InitialNumThreadsPermitted(InitialNumThreadsPermitted),
    m_MaxNumThreadsPermitted(MaxNumThreadsPermitted)
{
  QKERNELBASEVERIFY(m_InitialNumThreadsPermitted <= kMaxSemaphoreThreadsPermitted);
  QKERNELBASEVERIFY(m_InitialNumThreadsPermitted <= MaxNumThreadsPermitted);
  QKERNELBASEVERIFY(m_MaxNumThreadsPermitted > 0);
  QKERNELBASEVERIFY(m_MaxNumThreadsPermitted <= kMaxSemaphoreThreadsPermitted);
  reset();
}

// ---------------------------------------------------------------------------------------

cSemaphore::~cSemaphore()
{
  if (m_hSemaphore != NULL) CloseHandle (m_hSemaphore);
}




// =======================================================================================
// === getWin32EventHandle ===============================================================
// =======================================================================================

HANDLE QAPI cSemaphore::getWin32EventHandle() const
{
  return m_hSemaphore;
}




// =======================================================================================
// === reset =============================================================================
// =======================================================================================

inline UInt32 /* previous dst */ __cdecl atomicSet32(const UInt32 src, volatile UInt32 * const dst)
{
#ifdef _M_X64
  return InterlockedExchange((LONG volatile *)dst, src);
#else
  // Don't need to preserve the EAX, EBX, ECX, EDX, ESI, or EDI registers
  UInt32 result;
  __asm
  {
    mov eax, src
    mov ebx, dst
    lock xchg dword ptr [ebx], eax // swap [ebx] with eax
    mov result, eax // result is what was at [ebx]
  }
  return result;
#endif
}

void QAPI cSemaphore::reset()
{
  // The CreateSemaphore function creates or opens a named or unnamed semaphore object.
  const HANDLE newSemaphore = (void *)CreateSemaphore(
    NULL,             // Pointer to a SECURITY_ATTRIBUTES structure that determines whether
                      //  the returned m_hSemaphore can be inherited by child processes. Not used.
    m_InitialNumThreadsPermitted,
                      // Specifies an initial count for the semaphore object.
    m_MaxNumThreadsPermitted,
                      // Specifies the maximum count for the semaphore object.
    NULL);            // Pointer to a null-terminated string specifying the name of the
                      //  semaphore object. Not used.
  QKERNELBASEVERIFY(newSemaphore);

#if 0
  const HANDLE oldSemaphore = (HANDLE)atomicSet32((UInt32)newSemaphore, (UInt32 *)&m_hSemaphore);
  if (oldSemaphore != NULL)
  {
    CloseHandle(oldSemaphore);
  }
#else
  if (m_hSemaphore != NULL)
  {
    CloseHandle(m_hSemaphore);
  }
  m_hSemaphore = newSemaphore;
#endif
}




// =======================================================================================
// === wait ==============================================================================
// =======================================================================================

iWaitable::eWaitResult QAPI cSemaphore::wait(const unsigned int MillisecTimeout) const
{
  iWaitable::eWaitResult Result = eWaitResult_TimedOut;
  const DWORD Win32Result = WaitForSingleObject(m_hSemaphore, MillisecTimeout);
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
      Result = iWaitable::eWaitResult_Set;
      break;
    }
    case WAIT_TIMEOUT :
    {
      // The time-out interval elapsed, and the object's state is nonsignaled.
      break;
    }
    case WAIT_FAILED :
    {
      // Wait failed trying to WaitForSingleObject()
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
// === waitForever =======================================================================
// =======================================================================================

void cSemaphore::waitForever() const
{
  wait(INFINITE);
}




// =======================================================================================
// === set ===============================================================================
// =======================================================================================

void QAPI cSemaphore::set()
{
  // The ReleaseSemaphore function increases the count of the specified semaphore object
  // by a specified amount.
  (void)ReleaseSemaphore(
    m_hSemaphore,      // Handle to semaphore.
    1,                 // Count increment amount.
    NULL);             // Previous count - NULL because we don't require previous count.
  // Yes, we know that sometimes there will be 'too many posts to a semaphore' but we
  // don't care!
  SetLastError(NULL);
}




// =======================================================================================
// === getName ===========================================================================
// =======================================================================================

String QAPI cSemaphore::getName() const throw(cNotFound)
{
  if (m_Name == L"") throw cNotFound(__FILE__, __LINE__, L"Semaphore has no name");
  return m_Name;
}




// =======================================================================================
// === cSemaphoreGuard ===================================================================
// =======================================================================================

cSemaphoreGuard::cSemaphoreGuard(cSemaphore * const Semaphore) :
  m_Semaphore(Semaphore)
{
  m_Semaphore->waitForever();
}

// ---------------------------------------------------------------------------------------

cSemaphoreGuard::cSemaphoreGuard(const cSemaphoreGuard & Other)
{
  m_Semaphore = Other.m_Semaphore;
}

// ---------------------------------------------------------------------------------------

cSemaphoreGuard::~cSemaphoreGuard()
{
  m_Semaphore->set();
}

// ---------------------------------------------------------------------------------------

void cSemaphoreGuard::operator=(const cSemaphoreGuard & Other)
{
  Other.m_Semaphore->waitForever();
  m_Semaphore->set();
  m_Semaphore = Other.m_Semaphore;
}
