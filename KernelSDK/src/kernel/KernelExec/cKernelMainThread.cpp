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
#include "cKernelMainThread.h"
#include "cCmdConsoleManager.h"
#include "cClientServer.h"
#include "cDLLManager.h"
#include "cExceptionManager.h"
#include "cDeviceManager.h"
#include "cBootManager.h"
#include "LogCategories.h"
#include "cSystemInfo.h"
#include "cThreadManager.h"
#include "cLogManager.h"

using namespace vfs;




// =======================================================================================
// === gKernelMainThread =================================================================
// =======================================================================================

iKernelMainThread::Ptr vfs::gKernelMainThread;




// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

cKernelMainThread::cKernelMainThread() :
  cBasicThread(L"KrnlMain",iBasicThread::ePriority_Normal, 0x400000, eThreadLogging_Expanded), // request 4MB stack for main thread to avoid crash loading clips
  m_SignalBooted(false /* initially set? */),
  m_SignalDevicesReady(false /* initially set? */)
{
}




// =======================================================================================
// === onGuardDestroyed ==================================================================
// =======================================================================================

void QAPI cKernelMainThread::onGuardDestroyed(iGuarded::Ptr Guarded)
{
  m_ThreadExeGuard.invalidate(); // Signals loop to stop
  waitLoopCompleteForever();
}




// =======================================================================================
// === start =============================================================================
// =======================================================================================

cGuard::ConstPtr cKernelMainThread::start()
{
  QVERIFY(!m_ThreadExeGuard.isValid()); // Make sure not already running
  m_ThreadExeGuard = iThreadManager::singleton().startThread(this);
  return new cGuard(this, iGuarded::Ptr());
}




// =======================================================================================
// === waitDevicesReady/waitBooted =======================================================
// =======================================================================================

void cKernelMainThread::waitDevicesReady()
{
  m_SignalDevicesReady.waitForever();
}

// ---------------------------------------------------------------------------------------

void cKernelMainThread::waitBootedForever()
{
  m_SignalBooted.waitForever();
}

// ---------------------------------------------------------------------------------------

void cKernelMainThread::waitBooted(const unsigned int timeoutMs)
{
  m_SignalBooted.wait(timeoutMs);
}




// =======================================================================================
// === runLoop ===========================================================================
// =======================================================================================

void QAPI cKernelMainThread::runLoop()
{
  try
  {
    cThreadManager::m_MainThreadPtr = ptr_cast<cThreadImpl::Ptr>(m_Pimpl).getRaw();

    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) throw cWin32Error(__FILE__, __LINE__, L"Tried to CoInitialize(NULL) - returned %x", hr);

    QMSG((0, L"Started Kernel"));

    bool IsUninstalling = false, IsRunningNormally = true, IsRemovingSettingsAndDataOnUninstall = false, IsLoggingChangedSettings = false;
    try { (void)iSystemInfo::singleton().getCmdLineOption(L"install"); IsUninstalling = false; IsRunningNormally = false; } catch (cRecoverable &) {}
    try { (void)iSystemInfo::singleton().getCmdLineOption(L"uninstall"); IsUninstalling = true; IsRunningNormally = false; } catch (cRecoverable &) {}
    try { (void)iSystemInfo::singleton().getCmdLineOption(L"uninstalldeep"); IsUninstalling = true; IsRunningNormally = false; IsRemovingSettingsAndDataOnUninstall = true; } catch (cRecoverable &) {}
    try { (void)iSystemInfo::singleton().getCmdLineOption(L"registrydiff"); IsLoggingChangedSettings = true; } catch (cRecoverable &) {}

    cProgressMsgGuard::Ptr headlineProgress;
    if(IsRunningNormally && !cSystemInfo::singleton().isUserAnAdministrator())
    {
      headlineProgress = new cProgressMsgGuard(L"Scanning DLLs");
      if(iDLLManager::singleton().isInstallDllsNecessary())
      {
        iBootManager::singleton().setState(iBootManager::eBootState_InstallDllsNecessary);
        m_SignalBooted.set();
        throw cAborted(__FILE__, __LINE__, L"Re-install required");
      }
    }

    if (IsUninstalling) headlineProgress = new cProgressMsgGuard(L"Uninstalling");
    else if(IsRunningNormally) headlineProgress = new cProgressMsgGuard(L"Starting");
    else headlineProgress = new cProgressMsgGuard(L"Installing");


    // --- Uninstall -----------------------------------------------------------------------

    if (IsUninstalling)
    {
      try
      {
        iDLLManager::singleton().uninstallDLLs(IsRemovingSettingsAndDataOnUninstall);
      }
      catch (cRecoverable & Reason)
      {
        QFATAL((__FILE__, __LINE__, L"During uninstall: %s", Reason.getMsg().c_str()));
      }
    }

    if (iBootManager::singleton().getState() >= iBootManager::eBootState_CleanExiting) throw cAborted(__FILE__, __LINE__, L"Boot abandoned");

    // --- Install core & logging ----------------------------------------------------------


    if (!IsUninstalling)
    {
      iLogManager::singleton();
      iLogManager::bufferBreak(L"Core");

      try
      {
        iDLLManager::singleton().installDLLs(iModuleContext::eCore, iModuleContext::eDevices - 1);
      }
      catch (cRecoverable & Reason)
      {
        QRECOVERABLE2FATAL(Reason);
      }
    }

    if (iBootManager::singleton().getState() >= iBootManager::eBootState_CleanExiting) throw cAborted(__FILE__, __LINE__, L"Boot abandoned");

    // --- Init core -----------------------------------------------------------------------

    if (!IsUninstalling)
    {
      {
        cLogIndentGuard IndentGuard(kCtgLoadModule, L"Loading core modules");
        iDLLManager::singleton().initDLLs(iModuleInstance::eCore, iModuleInstance::eMonitoring - 1);
      }
      {
        cLogIndentGuard IndentGuard(kCtgInitDriver, L"Initialising exception drivers");
        cExceptionManager::singleton().startDrivers();
      }

      iMemoryManager::singleton();

      // Summaries
      {
        cLogIndentGuard IndentGuard(0, L"Summary");
        iSystemInfo::singleton().logSummary();
      }
      {
        cLogIndentGuard IndentGuard(kCtgSummaryDetail, L"Detailed summary");
      }
    }

    if (iBootManager::singleton().getState() >= iBootManager::eBootState_CleanExiting) throw cAborted(__FILE__, __LINE__, L"Boot abandoned");

    // --- Init logging --------------------------------------------------------------------

    if (!IsUninstalling)
    {
      if (iDLLManager::singleton().countDLLs(iModuleInstance::eMonitoring, iModuleInstance::eDevices - 1) > 0)
      {
        iLogManager::bufferBreak(L"Monitoring");

        {
          cLogIndentGuard IndentGuard(kCtgLoadModule, L"Loading monitoring modules");
          iDLLManager::singleton().initDLLs(iModuleInstance::eMonitoring, iModuleInstance::eDevices - 1);
        }
        {
          cLogIndentGuard IndentGuard(kCtgInitDriver, L"Initialising log drivers");
          cLogManager::singleton().startDrivers();
        }
        cBootManager::singleton().doPhase(iBootPhaseDriver::eMonitoring);

        // Summaries
        {
          cLogIndentGuard IndentGuard(0, L"Summary");
          iLogManager::singleton().logSummary();
        }
        {
          cLogIndentGuard IndentGuard(kCtgSummaryDetail, L"Detailed summary");
          iLogManager::singleton().logDetailedSummary();
        }
      }
    }

    if (iBootManager::singleton().getState() >= iBootManager::eBootState_CleanExiting) throw cAborted(__FILE__, __LINE__, L"Boot abandoned");

    // --- Install remaining ---------------------------------------------------------------

    if (!IsUninstalling)
    {
      try
      {
        iDLLManager::singleton().installDLLs(iModuleContext::eDevices, iModuleContext::eLowestPriority + 99);
      }
      catch (cRecoverable & Reason)
      {
        QRECOVERABLE2FATAL(Reason);
      }
    }

    if (iBootManager::singleton().getState() >= iBootManager::eBootState_CleanExiting) throw cAborted(__FILE__, __LINE__, L"Boot abandoned");

    // --- Init devices --------------------------------------------------------------------

    if (IsRunningNormally )
    {
      if (iDLLManager::singleton().countDLLs(iModuleInstance::eDevices, iModuleInstance::eSharedHighestPriority - 1) > 0)
      {
        iLogManager::bufferBreak(L"Devices");

        cBootManager::singleton().doPhase(iBootPhaseDriver::eDevices);

        {
          cProgressMsgGuard progress(L"Acquiring devices");
          cLogIndentGuard IndentGuard(0, L"Acquiring devices");
      /*
          iDLLManager::singleton().initDLLs(iModuleInstance::eDevices, iModuleInstance::eSharedHighestPriority - 1);
          static_cast<cDeviceManager &>(iDeviceManager::singleton()).acquireAllDevices();
      */
          for (unsigned int i = iModuleInstance::eDevices; i <= iModuleInstance::eSharedHighestPriority - 1; ++i)
          {
            iDLLManager::singleton().initDLLs(i, i);
            static_cast<cDeviceManager &>(iDeviceManager::singleton()).acquireAllDevices();
          }

        }

        // Summaries
        {
          cLogIndentGuard IndentGuard(0, L"Summary");
          iDeviceManager::singleton().logSummary();
        }
        {
          cLogIndentGuard IndentGuard(kCtgSummaryDetail, L"Detailed summary");
          iDeviceManager::singleton().logDetailedSummary();
        }
      }

      m_SignalDevicesReady.set();
    }

    if (iBootManager::singleton().getState() >= iBootManager::eBootState_CleanExiting) throw cAborted(__FILE__, __LINE__, L"Boot abandoned");

    // --- Init display --------------------------------------------------------------------

    if (IsRunningNormally)
    {
      if (cBootManager::singleton().hasPhase(iBootPhaseDriver::eDisplay))
      {
        iLogManager::bufferBreak(L"Display");
        cProgressMsgGuard progress(L"Initialising display");

        cBootManager::singleton().doPhase(iBootPhaseDriver::eDisplay);
      }
    }

    if (iBootManager::singleton().getState() >= iBootManager::eBootState_CleanExiting) throw cAborted(__FILE__, __LINE__, L"Boot abandoned");

    // --- Init remaining ------------------------------------------------------------------

    if (IsRunningNormally)
    {
      if (iDLLManager::singleton().countDLLs(iModuleInstance::eSharedHighestPriority, iModuleInstance::eLowestPriority + 99) > 0)
      {
        iLogManager::bufferBreak(L"Remaining Modules");
        {
          cLogIndentGuard IndentGuard(kCtgLoadModule, L"Loading modules");
          iDLLManager::singleton().initDLLs(iModuleInstance::eSharedHighestPriority, iModuleInstance::eLowestPriority + 99);
        }
        cBootManager::singleton().doPhase(iBootPhaseDriver::ePlugins);
      }
    }

    if (iBootManager::singleton().getState() >= iBootManager::eBootState_CleanExiting) throw cAborted(__FILE__, __LINE__, L"Boot abandoned");

    // --- DLL summary ---------------------------------------------------------------------

    if (!IsUninstalling)
    {
      iLogManager::bufferBreak(L"Summary");
      {
        cLogIndentGuard IndentGuard(0, L"DLL");
        iDLLManager::singleton().logSummary();
      }
      {
        cLogIndentGuard IndentGuard(kCtgSummaryDetail, L"DLL Detail");
        iDLLManager::singleton().logDetailedSummary();
      }
      if (IsLoggingChangedSettings)
      {
        cLogIndentGuard IndentGuard(0, L"Registry Differences");
        iDLLManager::singleton().logChangedSettings();
      }
    }

    if (iBootManager::singleton().getState() >= iBootManager::eBootState_CleanExiting) throw cAborted(__FILE__, __LINE__, L"Boot abandoned");

    // --- Create self-creating items --------------------------------------------------------------

    if (IsRunningNormally)
    {
      // [RFD-148/AJS] Make the factory a part of the support info gathering
      gFactory->checkAndRegisterDump();

      {
        cLogIndentGuard IndentGuard(0, L"Factory");
        gFactory->logSummary();
      }
      {
        cLogIndentGuard IndentGuard(kCtgSummaryDetail, L"Factory Detail");
        gFactory->logDetailedSummary();
      }
    }

    if (iBootManager::singleton().getState() >= iBootManager::eBootState_CleanExiting) throw cAborted(__FILE__, __LINE__, L"Boot abandoned");

    // --- DevTest Toggle summary and check we are T version ---------------------------------------

    if (IsRunningNormally)
    {
      cDevTestToggle::logAll();
    }

    if (iBootManager::singleton().getState() >= iBootManager::eBootState_CleanExiting) throw cAborted(__FILE__, __LINE__, L"Boot abandoned");

    // --- Booted ------------------------------------------------------------------------

    if (IsRunningNormally)
    {
      if (cBootManager::singleton().hasPhase(iBootPhaseDriver::eBooted))
      {
        iLogManager::bufferBreak(L"Finalising desktop");
        cProgressMsgGuard progress(L"Finalising desktop");

        {
          cLogIndentGuard IndentGuard(kCtgBootPhaseBooted, L"Finalising desktop");
          while (true)
          {
            cBootManager::singleton().doPhase(iBootPhaseDriver::eBooted);
            m_SignalBooted.set();
          }
        }
      }
    }

    m_SignalBooted.set();

    cLogManager::singleton().flushBuffer();
    Sleep(2000);

    cThreadManager::m_MainThreadPtr = 0;

    // If KernelServer has called us and we're installing, signal that a restart is now required
    if(!IsRunningNormally && !IsUninstalling) iBootManager::singleton().setState(iBootManager::eBootState_RestartNecessary);
  }
  catch (cAborted & Reason)
  {
    QSOS((L"Kernel main loop aborted (%s)", Reason.getMsg().c_str()));
  }
}
