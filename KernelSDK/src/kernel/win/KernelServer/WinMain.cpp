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
#include "resource.h"
#include <QKernelExec.h>
#include "cKernelBootCommsThread.h"

using namespace std;
using namespace vfs;




// =======================================================================================
// === ATL ===============================================================================
// =======================================================================================

static class cExeModule : public ATL::CAtlExeModuleT<cExeModule>
{
public:

  DECLARE_REGISTRY_APPID_RESOURCEID(IDR_KernelServer, "{5E95F91F-B013-4DD3-A31E-DB02743D81DD}")

  void RunMessageLoop()
  {
    MSG msg;
    while (GetMessage(&msg, 0, 0, 0) > 0)
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);

      if (msg.hwnd == NULL && msg.message == WM_USER)
      {
        ((void (*)(UIntPtr)) msg.wParam)(msg.lParam);  // call the function wParam passing lParam in this thread (the original windows thread)
      }
    }
  }

} _Module;

// ---------------------------------------------------------------------------------------

namespace
{

HRESULT _atlUpdateROT() throw(cNotAvailable)
{
  HRESULT hr = S_OK;

  // Add class factories to running object table (ROT)
  vector<_ATL_OBJMAP_ENTRY> Objects = iDLLManager::singleton().getATLObjects();
  for (vector<_ATL_OBJMAP_ENTRY>::iterator j = Objects.begin(); j != Objects.end(); ++j)
  {
    j->pfnObjectMain(true);
    hr = j->RegisterClassObject(CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE);
    if (FAILED(hr)) break;
  }

  return hr;
}

}

// ---------------------------------------------------------------------------------------

namespace
{

HRESULT _atlUpdateRegistry(const bool IsInstalling) throw(cWin32Error)
{
  HRESULT hr = S_OK;

  hr = _Module.UpdateRegistryAppId(IsInstalling);
  if (FAILED(hr)) throw cNotAvailable(__FILE__, __LINE__, L"Failed to UpdateRegistryAppId (0x%8x)", hr);

  // Write type libraries to registry
  {
    const multimap<HINSTANCE, unsigned int> TypeLibraries = iDLLManager::singleton().getATLTypeLibraries();
    for (multimap<HINSTANCE, unsigned int>::const_iterator j = TypeLibraries.begin(); j != TypeLibraries.end(); ++j)
    {
      const Char * ResourceIndexAsString = 0;
      StringStream Tmp;
      if (j->second /* resource index */)
      {
        Tmp << L"\\" << j->second /* resource index */;
        ResourceIndexAsString = Tmp.str().c_str();
      }
      if (IsInstalling)
      {
        hr = ATL::AtlRegisterTypeLib(j->first /* dll instance handle */, ResourceIndexAsString);
        if (FAILED(hr)) throw cWin32Error(__FILE__, __LINE__, L"Failed to AtlRegisterTypeLib (0x%8x)", hr);
      }
      else
      {
        hr = ATL::AtlUnRegisterTypeLib(j->first /* dll instance handle */, ResourceIndexAsString);
        if (FAILED(hr)) throw cWin32Error(__FILE__, __LINE__, L"Failed to AtlUnRegisterTypeLib (0x%8x)", hr);
      }
    }
  }

  // Write classes to registry
  {
    const vector<_ATL_OBJMAP_ENTRY> Objects = iDLLManager::singleton().getATLObjects();
    for (vector<_ATL_OBJMAP_ENTRY>::const_iterator j = Objects.begin(); j != Objects.end(); ++j)
    {
      hr = AtlRegisterClassCategoriesHelper( *j->pclsid, j->pfnGetCategoryMap(), IsInstalling);
      if (FAILED(hr)) throw cWin32Error(__FILE__, __LINE__, L"Failed to AtlRegisterClassCategoriesHelper (0x%8x)", hr);

      hr = j->pfnUpdateRegistry(IsInstalling);
      if (FAILED(hr)) throw cWin32Error(__FILE__, __LINE__, L"Failed to pfnUpdateRegistry (0x%8x)", hr);
    }
  }

  return hr;
}

}




// =======================================================================================
// === _parseCommandLine =================================================================
// =======================================================================================

namespace
{

enum eHowToRun
{
  eHowToRun_Normal,
  eHowToRun_NormalNoMutex,
  eHowToRun_Uninstall,
  eHowToRun_Install
};

eHowToRun _parseCommandLine(LPCTSTR lpCmdLine) throw(cNotAvailable)
{
  eHowToRun Result = eHowToRun_Normal;

  TCHAR szTokens[] = _T("-/");
  LPCTSTR lpszToken = ATL::CAtlModule::FindOneOf(lpCmdLine, szTokens);
  while (lpszToken != NULL)
  {
    if (ATL::CAtlModule::WordCmpI(lpszToken, L"uninstall") == 0)
    {
      Result = eHowToRun_Uninstall;
    }
    if (ATL::CAtlModule::WordCmpI(lpszToken, L"uninstalldeep") == 0)
    {
      Result = eHowToRun_Uninstall;
    }
    if (ATL::CAtlModule::WordCmpI(lpszToken, L"install") == 0)
    {
      Result = eHowToRun_Install;
    }
    if (ATL::CAtlModule::WordCmpI(lpszToken, L"nomutex") == 0)
    {
      Result = eHowToRun_NormalNoMutex;
    }
    lpszToken = ATL::CAtlModule::FindOneOf(lpszToken, szTokens);
  }

  return Result;
}

}




// =======================================================================================
// === kernelBaseFatalHandler ============================================================
// =======================================================================================
/*
Redirect KernelBase fatals to KernelExec
*/
extern "C" _declspec(dllexport) bool /* was handled? */ QARGS_STACK kernelBaseFatalHandler(
  const char * const srcFilename,
  const unsigned long srcLineNumber,
  const Char * const msg)
{
  if (iExceptionManager::isSingletonConstructed())
  {
    QFATAL((srcFilename, srcLineNumber, L"%s", msg));
    return true; // never get here
  }
  else
  {
    return false;
  }
}




// =======================================================================================
// === _tWinMain =========================================================================
// =======================================================================================

extern "C" int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpCmdLine, int nShowCmd)
{
  HANDLE hThisProcessRunningMutex = 0;

  try
  {
    const eHowToRun howToRun = _parseCommandLine(lpCmdLine);
    if (howToRun == eHowToRun_Normal)
    {
      // Check to see if we're the first instance (by looking to see if a mutex named as our app already exists).

      // PLEASE NOTE: If you change the name of this mutex (L"Global\\vfs KernelServer Running") then you'll
      // need to advise PVG about this change and update the IsKernelRunning() function within iQCmdConsole.
      // Mount the Quentin VOB and see \Quentin\QuentinTest\iQCmdConsole_src\iQCmdConsole.cpp. Quentin Test
      // uses this mutex to check if GENQ is running. If you change it without informing anyone, it will
      // cause all of PVG's QPython regression tests to fail. Please also see GENQ-13221.

      hThisProcessRunningMutex = CreateMutex(NULL, TRUE, L"Global\\vfs KernelServer Running");

      const bool IsThisProcessAlreadyRunning = (hThisProcessRunningMutex == NULL) || (GetLastError() == ERROR_ALREADY_EXISTS);
      if (IsThisProcessAlreadyRunning)
      {
        throw cNotSupported(__FILE__, __LINE__, L"There can only be one instance of the vfs \"KernelServer\" running at a time. Please close the other instance first.");
      }
    }

    _Module.InitializeCom();

    cKernel::Ptr kernel = new cKernel(String(lpCmdLine) + L" /hostname \"KernelServer\" /hostreg=\"KernelServer.reg\" /hostversion=\"1.0.0.0\" /allowplugins /allowpatching");

    try
    {
      // Remove garbage left by setup process
      iFileManager::singleton().deleteTemporaryFolders(L"GenQ_KernelSetup", fDeleteFileOrFolder_DeleteReadOnly | fDeleteFileOrFolder_IgnoreErrorsWhenRecursing);
    }
    catch (cRecoverable &) {}

    if (howToRun == eHowToRun_Install)
    {
      _atlUpdateRegistry(true /* installing? */);
      _atlUpdateROT();
      kernel->installApp();
    }
    else if (howToRun == eHowToRun_Uninstall)
    {
      try
      {
        _atlUpdateRegistry(false /* installing? */);
      }
      catch (cRecoverable &)
      {
        // May have already unregistered ATL objects
      }
      kernel->uninstallApp();
    }
    else if ((howToRun == eHowToRun_Normal) || (howToRun == eHowToRun_NormalNoMutex))
    {
      try
      {
        _atlUpdateRegistry(true /* installing? */);
      }
      catch (cRecoverable &)
      {
        // Probably just don't have the privileges, but that's ok because we should have updated the registry
        // during install anyway. We are just doing it again in case the user is a developer and they are
        // relying on JIT install. i.e. just invented a new COM object and trying it out for first time.
      }
      _atlUpdateROT();

      cGuard::ConstPtr unwindThreadExe = iThreadManager::singleton().startThread(new cKernelBootCommsThread);

      kernel->startApp();
      kernel->waitAppStartedForever();
      
      if(iBootManager::singleton().getState() == iBootManager::eBootState_InstallDllsNecessary) throw cAborted(__FILE__, __LINE__, L"Re-install requires Administrator privilege");

      (void)_Module.StartMonitor();
      _Module.RunMessageLoop();

      kernel->waitAppExitForever();
    }
  }
  catch (cRecoverable & Reason)
  {
    (void)MessageBox(
      NULL,
      Reason.getMsg().c_str(),
      L"KernelServer",
      MB_ICONEXCLAMATION | MB_TOPMOST | MB_APPLMODAL);
  }

  if (hThisProcessRunningMutex) (void)CloseHandle(hThisProcessRunningMutex);

  // Need this because sometimes SmartHeap dies when trying to clean exit!
  (void)TerminateProcess(GetCurrentProcess(), EXIT_SUCCESS);
  return EXIT_SUCCESS;
}

