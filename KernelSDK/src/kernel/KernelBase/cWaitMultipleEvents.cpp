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
#include <QKernelBase.h>
#include "fatal.h"

using namespace vfs;
using namespace std;




// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

cWaitMultipleEvents::cWaitMultipleEvents(const vector<String> & EventIds) throw(cNotFound, cOSError) :
  m_EventIds(EventIds)
{
  for (vector<String>::const_iterator i = EventIds.begin(); i != EventIds.end(); ++i)
  {
    QKERNELBASEVERIFY(*i != L""); // Event must have a name!
    if (*i == L"") throw cOSError(__FILE__, __LINE__, L"Can't OpenEvent() on event with no name");
    const HANDLE Handle = OpenEvent(EVENT_ALL_ACCESS /* dwDesiredAccess */, FALSE /* bInheritHandle */, i->c_str() /* name */);
    if (Handle == NULL) throw cNotFound(__FILE__, __LINE__, L"OpenEvent() failed for \"%s\" - %d", i->c_str(), GetLastError());
    m_Handles.push_back(Handle);
  }
}

// ---------------------------------------------------------------------------------------

cWaitMultipleEvents::~cWaitMultipleEvents()
{
  for (vector<HANDLE>::const_iterator i = m_Handles.begin(); i != m_Handles.end(); ++i)
  {
    (void)CloseHandle(*i);
  }
}




// =======================================================================================
// === wait ==============================================================================
// =======================================================================================

iWaitable::eWaitResult QAPI cWaitMultipleEvents::wait(const unsigned int timeoutMs, String * const EventId) const
{
  *EventId = L"";
  iWaitable::eWaitResult Result = iWaitable::eWaitResult_TimedOut;
  const DWORD WaitResult = WaitForMultipleObjects(
    (DWORD)m_Handles.size(),          // number of handles in array
    &m_Handles.at(0),                 // object-handle array
    FALSE,                            // wait all?
    timeoutMs);                       // time-out interval
  if (WaitResult >= WAIT_OBJECT_0 && WaitResult < WAIT_OBJECT_0 + m_Handles.size())
  {
    *EventId = m_EventIds.at(WaitResult - WAIT_OBJECT_0);
    Result = iWaitable::eWaitResult_Set;
  }
  else if (WaitResult >= WAIT_ABANDONED_0 && WaitResult < WAIT_ABANDONED_0 + m_Handles.size())
  {
    // The specified object is a mutex object that was not released by the thread that
    // owned the mutex object before the owning thread terminated. Ownership of the mutex
    // object is granted to the calling thread, and the mutex is set to nonsignaled.

    // Abandoned WaitForSingleObject() because thread exited prematurely
    QKERNELBASEVERIFY(false);
  }
  else if (WaitResult == WAIT_TIMEOUT)
  {
    /* Nop */
  }
  else
  {
    // Unexpected result
    QKERNELBASEVERIFY(false);
  }
  
  return Result;
}




// =======================================================================================
// === waitForever =======================================================================
// =======================================================================================

String QAPI cWaitMultipleEvents::waitForever() const
{
  String EventId;
  (void)wait(INFINITE, &EventId);
  return EventId;
}
