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
#include "cFactory.h"
#include "cFileManager.h"
#include "Win32Resources.h"
#include "cExceptionManager.h"
#include "cLogManager.h"
#include "cCmdConsoleManager.h"
#include "cTimerManager.h"
#include "cMemoryManager.h"
#include "cDLLManager.h"
#include "cBootManager.h"
#include "cDeviceManager.h"
#include "cKernelMainThread.h"
#include "cFileManager.h"
#include "cActivityLog.h"
#include "cSystemInfo.h"
#include "cvfsEnvironment.h"
#include "cLogManager.h"
#include <cfenv>

using namespace vfs;
using namespace std;




// =======================================================================================
// === cKernelImpl =======================================================================
// =======================================================================================

namespace vfs
{

class cKernelImpl : public cRefCount
{
  public :
    vector<cGuard::ConstPtr>  m_Unwind;
    cKernelMainThread::Ptr    m_MainThread;
    cGuard::ConstPtr          m_MainThreadExeGuard;
};

}




// =======================================================================================
// === Backwards compatibility ===========================================================
// =======================================================================================

namespace vfs
{

iFactory::Ptr gFactory;

void QAPI kernelCreate(const String & CmdLine)
{
  gKernel = new cKernel(CmdLine);
}

bool QAPI kernelIsCreated()
{
  return gKernel != 0;
}

void QAPI kernelDestroy()
{
  delete gKernel;
}

}




// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

namespace vfs
{

cKernel * gKernel;

}

iFactory::Ptr cKernel::m_Factory;

QEXPORT cKernel::cKernel(const String & cmdLine) /* throw cRecoverable */ :
  m_Pimpl(new cKernelImpl)
{
  if (m_Factory.isValid())
  {
    throw cNotSupported(__FILE__, __LINE__, L"Only one Kernel can be created at a time");
  }

  gKernel = this;

  m_Factory = new cFactory;
  gFactory = m_Factory; // for backwards compatibilty

  m_Pimpl->m_Unwind.push_back(gFactory->registerClass(new cSystemInfo::class_ctor));
  m_Pimpl->m_Unwind.push_back(gFactory->registerClass(new cvfsEnvironment::class_ctor));
  m_Pimpl->m_Unwind.push_back(gFactory->registerClass(new cExceptionManager::class_ctor));
  #if defined(_DEBUG)
    QVERIFY(LoadLibrary(L"vcruntime140d.dll"));
    QVERIFY(LoadLibrary(L"ucrtbased.dll"));
    QVERIFY(LoadLibrary(L"msvcp140d.dll"));
  #else
    QVERIFY(LoadLibrary(L"vcruntime140.dll"));
    QVERIFY(LoadLibrary(L"ucrtbase.dll"));
    QVERIFY(LoadLibrary(L"msvcp140.dll"));
  #endif
  m_Pimpl->m_Unwind.push_back(gFactory->registerClass(new cFileManager::class_ctor));
  m_Pimpl->m_Unwind.push_back(gFactory->registerClass(new cLogManager::class_ctor));
  m_Pimpl->m_Unwind.push_back(gFactory->registerClass(new cActivityLog::class_ctor));
  m_Pimpl->m_Unwind.push_back(gFactory->registerClass(new cMemoryManager::class_ctor));
  m_Pimpl->m_Unwind.push_back(gFactory->registerClass(new cThreadManager::class_ctor));
  m_Pimpl->m_Unwind.push_back(gFactory->registerClass(new cCmdConsoleManager::class_ctor));
  m_Pimpl->m_Unwind.push_back(gFactory->registerClass(new cTimerManager::class_ctor));
  m_Pimpl->m_Unwind.push_back(gFactory->registerClass(new cDLLManager::class_ctor));
  m_Pimpl->m_Unwind.push_back(gFactory->registerClass(new cBootManager::class_ctor));
  m_Pimpl->m_Unwind.push_back(gFactory->registerClass(new cDeviceManager::class_ctor));

  cSystemInfo::singleton().init(cmdLine);
  cThreadManager::singleton().setOrigWinThreadId(GetCurrentThreadId());
  cSystemInfo::singleton().initBuiltInModules();
  try
  {
    cDLLManager::singleton().installDLLsNoFileScan(iModuleContext::eBuiltIn, iModuleContext::eCore - 1);
    cDLLManager::singleton().initDLLsNoFileScan(iModuleContext::eBuiltIn, iModuleContext::eCore - 1);
  }
  catch (cRecoverable & Reason)
  {
    QRECOVERABLE2FATAL(Reason);
  }
  cSystemInfo::singleton().initUsingRegistrySettings();

  m_Pimpl->m_MainThread = new cKernelMainThread;
  gKernelMainThread = m_Pimpl->m_MainThread; // for backwards compatibilty

  cBootManager::singleton().enableProgressMsgs(true);

  std::fesetround(FE_TONEAREST); 
  QVERIFY(FLT_ROUNDS == 1 || !"Floating point support has not been tested without rounding to the nearest!");
}

// ---------------------------------------------------------------------------------------

cKernel::~cKernel()
{
  m_Pimpl.invalidate();
  m_Factory.invalidate();
  gKernel = 0;
}




// =======================================================================================
// === installApp ========================================================================
// =======================================================================================

void cKernel::installApp() /* throw cRecoverable */
{
  (void)m_Pimpl->m_MainThread->start();
  m_Pimpl->m_MainThread->waitLoopCompleteForever();
}




// =======================================================================================
// === uninstallApp ======================================================================
// =======================================================================================

void cKernel::uninstallApp() /* throw cRecoverable */
{
  (void)m_Pimpl->m_MainThread->start();
  m_Pimpl->m_MainThread->waitLoopCompleteForever();
}




// =======================================================================================
// === startApp ==========================================================================
// =======================================================================================

void cKernel::startApp() /* throw cRecoverable */
{
//  m_Pimpl->m_Unwind.push_back(iThreadManager::singleton().startThread(new cKernelBootCommsThread));
  m_Pimpl->m_MainThreadExeGuard = m_Pimpl->m_MainThread->start();
}

// ---------------------------------------------------------------------------------------

void cKernel::waitAppStartedForever()
{
  m_Pimpl->m_MainThread->waitBootedForever();
}

// ---------------------------------------------------------------------------------------

void cKernel::waitAppStarted(const unsigned int timeoutMs)
{
  m_Pimpl->m_MainThread->waitBooted(timeoutMs);
}




// =======================================================================================
// === exitApp ===========================================================================
// =======================================================================================

void cKernel::exitApp() /* throw cRecoverable */
{
}

// ---------------------------------------------------------------------------------------

void cKernel::waitAppExitForever() /* throw cRecoverable */
{
  m_Pimpl->m_MainThreadExeGuard.invalidate(); // signal main thread to complete
  m_Pimpl->m_MainThread->waitLoopCompleteForever();
}

// ---------------------------------------------------------------------------------------

void cKernel::waitAppExit(const unsigned int timeoutMs) /* throw cRecoverable */
{
  m_Pimpl->m_MainThreadExeGuard.invalidate(); // signal main thread to complete
  m_Pimpl->m_MainThread->waitLoopComplete(timeoutMs);
}
