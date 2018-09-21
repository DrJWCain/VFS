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
#include "cKernelBootCommsThread.h"
#include "LogCategories.h"

using namespace vfs;




// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

cKernelBootCommsThread::cKernelBootCommsThread() :
  cThread(L"KernelBootComms", cThread::eTimeCritical),
  m_IsRunning(false),
  m_Client(iSystemInfo::singleton().getProductName() + L" {DAF32FC1-6D5C-4770-A434-05EB6B29A4A5} // KernelBoot key")
{
}




// =======================================================================================
// === signalLoopStop ====================================================================
// =======================================================================================

void QAPI cKernelBootCommsThread::signalLoopStop()
{
  m_IsRunning = false;
  m_Client.spike();
}




// =======================================================================================
// === runLoop ===========================================================================
// =======================================================================================

void QAPI cKernelBootCommsThread::runLoop()
{
  m_IsRunning = true;

  while (m_IsRunning)
  {
    const String MsgBuffer = m_Client.getStringWaitForever();
    if (MsgBuffer == L"Emergency cleanup")
    {
      cEmergencyProgressMsgGuard progress(L"Closing");

      cLogIndentGuard IndentGuard(kCtgKernelBootComms, L"Emergency cleanup request received");
      cleanupEmergency();

      // Byeee!
      QMSG((0, L"Attempting TerminateProcess(EXIT_SUCCESS)"));
      Sleep(1000);
      TerminateProcess(GetCurrentProcess(), EXIT_SUCCESS);
    }
    else if (MsgBuffer == L"Standard cleanup")
    {
      cEmergencyProgressMsgGuard progress(L"Closing");

      cLogIndentGuard IndentGuard(kCtgKernelBootComms, L"Standard cleanup request received");
//      iExceptionManager::singleton().presentWarning(L"Closing %s", iSystemInfo::singleton().getProductName().c_str());
      cleanupStandard();

      // Byeee!
      QMSG((0, L"Attempting TerminateProcess(EXIT_SUCCESS)"));
      Sleep(1000);
      TerminateProcess(GetCurrentProcess(), EXIT_SUCCESS);
    }
    else
    {
      QSOS((L"KernelBootComms : \"%s\" request not recognised!", MsgBuffer.c_str()));
    }
  }
}




// =======================================================================================
// === cleanupEmergency/cleanupStandard ==================================================
// =======================================================================================

void QAPI cKernelBootCommsThread::cleanupEmergency()
{
  cEmergencyProgressMsgGuard progress(L"Emergency clean up");

  // Warn threads that they are about to be suspended/killed.
  if (iExceptionManager::isSingletonConstructed())
  {
    iExceptionManager::singleton().fatal(
      0, /* flags */
      iExceptionManager::cFatalDiagnostics(EXIT_SUCCESS, "" /* src file */, 0 /* src line */, L"" /* heading */, L"Cleaning up"));
  }
}

// ---------------------------------------------------------------------------------------

void QAPI cKernelBootCommsThread::cleanupStandard()
{
  cEmergencyProgressMsgGuard progress(L"Standard clean up");

  // Flush msg log.
  QMSG((0, L"Flushing msg log"));
  if (iLogManager::isSingletonConstructed())
  {
    iLogManager::singleton().flushBuffer();
  }

  cleanupEmergency();

  // Flush msg log again.
  QMSG((0, L"Flushing msg log"));
  if (iLogManager::isSingletonConstructed())
  {
    iLogManager::singleton().flushBuffer();
  }
}
