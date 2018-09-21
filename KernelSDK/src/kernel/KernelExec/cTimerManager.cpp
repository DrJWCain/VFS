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
#include "cTimerManager.h"
#include "LogCategories.h"

using namespace vfs;



// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

void CALLBACK _MultiMediaTimeCallback(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
  cTimerManager::singleton().serviceOneSecondTimerCallbacks();
}

cTimerManager::cTimerManager() :
  m_OneSecondTimerThreadTopLevelLogging(0)
{
  cLogIndentGuard IndentGuard(kCtgCreateManagerDetail, L"Creating cTimerManager");

  TIMECAPS tc;
  MMRESULT MMResult = timeGetDevCaps(&tc, sizeof(TIMECAPS));
  if (MMResult != TIMERR_NOERROR) QFATAL((__FILE__, __LINE__, L"Tried to timeGetDevCaps() - returned %d", MMResult));

  const unsigned int PeriodMillisecs = 1000 /* 1 second */;
  const unsigned int ResMillisecs    = PeriodMillisecs / 1;

  m_TimerRes = Min<UINT>(Max<UINT>(tc.wPeriodMin, ResMillisecs), tc.wPeriodMax);
  MMResult = timeBeginPeriod(m_TimerRes);
  if (MMResult != TIMERR_NOERROR) QFATAL((__FILE__, __LINE__, L"Tried to timeBeginPeriod() - returned %d", MMResult));

  MMResult = timeSetEvent(
    PeriodMillisecs,               // delay
    m_TimerRes,                    // resolution (global variable)
    (LPTIMECALLBACK)_MultiMediaTimeCallback,       // callback function
    (DWORD)0,                      // user data
    TIME_PERIODIC);                // single timer event
  if (MMResult == 0) QFATAL((__FILE__, __LINE__, L"Tried to timeSetEvent() - returned %d", MMResult));
  m_OneSecondTimerId = MMResult;
}

// ---------------------------------------------------------------------------------------

cTimerManager::~cTimerManager()
{
  cLogIndentGuard IndentGuard(0, L"Deleting cTimerManager");

  (void)timeKillEvent(m_OneSecondTimerId);
  (void)timeEndPeriod(m_TimerRes);

  if (m_OneSecondTimerThreadTopLevelLogging) delete m_OneSecondTimerThreadTopLevelLogging;
}




// =======================================================================================
// === serviceOneSecondTimerCallbacks ====================================================
// =======================================================================================

void cTimerManager::serviceOneSecondTimerCallbacks()
{
  if (!m_OneSecondTimerKernelThread.isValid())
  {
    m_OneSecondTimerKernelThread = iThreadManager::singleton().promoteCurrentThread (L"One second timer");
    m_OneSecondTimerThreadTopLevelLogging = new cLogIndentGuard(kCtgThreadRunLoop, L"Starting timer");
  }

  m_OneSecondPulse.pulse();

  const Int64 MillisecsElapsedSinceStart = iSystemInfo::singleton().getMillisecsSinceStart();

  list<iOneSecondTimerCallback::Ptr> CopyOfCallbacks;
  {
    cLockGuard ThreadSafety(&m_OneSecondTimerCallbacksThreadSafety);
    CopyOfCallbacks = m_OneSecondTimerCallbacks;
  }
  for (OneSecondTimerCallbackList::iterator i = CopyOfCallbacks.begin(); i != CopyOfCallbacks.end(); ++i)
  {
    (*i)->onOneSecondElapsed(MillisecsElapsedSinceStart);
  }
}




// =======================================================================================
// === addOneSecondTimerCallback =========================================================
// =======================================================================================

cGuard::ConstPtr QAPI cTimerManager::addOneSecondTimerCallback(const iOneSecondTimerCallback::Ptr & newCallback)
{
  cLockGuard ThreadSafety(&m_OneSecondTimerCallbacksThreadSafety);
  m_OneSecondTimerCallbacks.push_back(newCallback);
  return new cGuard(this, newCallback);
}




// =======================================================================================
// === waitNextSecondTick ================================================================
// =======================================================================================

void QAPI cTimerManager::waitNextSecondTick()
{
  m_OneSecondPulse.waitForever();
}




// =======================================================================================
// === onGuardDestroyed ==================================================================
// =======================================================================================

void QAPI cTimerManager::onGuardDestroyed(iGuarded::Ptr Guarded)
{
  iOneSecondTimerCallback::Ptr Candidate = ptr_cast<iOneSecondTimerCallback::Ptr>(Guarded);
  if (Candidate.isValid())
  {
    cLockGuard ThreadSafety(&m_OneSecondTimerCallbacksThreadSafety);
    m_OneSecondTimerCallbacks.remove(Candidate);
  }
}