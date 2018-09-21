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

//{{{  headers
#include "stdafx.h"
#include "cDLLManager.h"

#include "QKernelSDKVersion.h"
#include "cExceptionManager.h"
#include "cModuleInstallHelp.h"
#include "LogCategories.h"
#include "cModuleRegistrar.h"
#include "cSystemInfo.h"
#include "cDLLIterator.h"
#include <shlobj.h>
#include "cFileManager.h"
//}}}

using namespace vfs;
using namespace std;

//{{{
//BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
//{
  //Char dllPath[_MAX_PATH];
  //(void)GetModuleFileName(hinstDLL, dllPath, _MAX_PATH);
  //switch (fdwReason)
  //{
    //case DLL_PROCESS_ATTACH : { QMSG((kCtgDllMain, L"Attached DLL (%s)", dllPath)); break; }
    //case DLL_PROCESS_DETACH : { QMSG((kCtgDllMain, L"Detached DLL (%s)", dllPath)); break; }
    //default : { break; }
  //}
  //return TRUE;
//}
//}}}

namespace
{
  #if defined(_DEBUG)
  #define kDllCodeFolder L"DLL_Code_Debug\\"
  #else
  #define kDllCodeFolder L"DLL_Code_Release\\"
  #endif

  //{{{
  String _expandPathEnvVars(const String & Path)
  {
    vector<Char> Result;
    Result.resize(MAX_PATH);
    if (_wfullpath(&Result[0], cFileManager::singleton().expandPathEnvVars(Path).c_str(), Result.size()) ==  NULL)
    {
      QFATAL((__FILE__, __LINE__, L"Tried to _wfullpath \"%s\"", Path.c_str()));
    }
    return &Result[0];
  }
  //}}}
  //{{{
  String _tidyPath(const String & ToFormat)
  //!< Add trailing '\' if it needs it
  {
    String Result(_expandPathEnvVars(ToFormat));
    if (Result.size() > 0)
    {
      const Char LastChar = *Result.rbegin();
      if (LastChar != L'\\' && LastChar != L'/') Result += L"\\";
    }
    return Result;
  }
  //}}}
  //{{{
  void _replaceChar(String * const TheString, const String & ToReplace, const Char * const Replacement)
  {
    bool isReplacing = true;
    do
    {
      const String::size_type i = TheString->find(ToReplace);
      if (i == String::npos)
      {
        isReplacing = false;
      }
      else
      {
        TheString->replace(i, ToReplace.size(), Replacement);
      }
    } while (isReplacing);
  }
  //}}}
}

//----------------------------------------------------------------------------------------------------------------------
// CTOR/DTOR
//{{{
cDLLManager::cDLLManager()
  : m_HasBuiltRegistrarsList(false),
    m_HasScannedDLLs(false),
    m_HasPatchedDLLs(false),
    m_Rh(createRegistryHelp()),
    m_Urh(createCurrentUserRegistryHelp())
{
}
//}}}
//{{{
cDLLManager::~cDLLManager()
{
}
//}}}
//----------------------------------------------------------------------------------------------------------------------
// iManager
//{{{
void QAPI cDLLManager::logSummary()
{
  {
    cLogIndentGuard Indent(0, L"Intialised modules");
    _logDLLs(false /* IsOnlyShowingProblemDlls */);
  }

  if (m_HasPatchedDLLs)
  {
    cLogIndentSOSGuard Indent(L"Patched modules");
    cLockGuard ThreadSafety(&m_DLLsThreadSafety);

    QMSG((0, L"The following DLLs have been patched:", kKernelSDKVersion));
    QMSG((0, L""));

    for (multiset<cDLL>::const_iterator i = m_DLLs.begin(); i != m_DLLs.end(); ++i)
    {
      const cDLL & Entry = *i;
      if (!Entry.m_PatchedDLLLocation.empty())
      {
        QMSG((0, L"%-10s %-32s %s",
          Entry.m_DeveloperName.c_str(),
          Entry.m_ModuleName.c_str(),
          Entry.m_CodeFilename.c_str()));
        QMSG((0, L"%43s %s",
          L"overrides",
          Entry.m_PatchedDLLLocation.c_str()));
      }
    }
  }
}
//}}}
//{{{
void QAPI cDLLManager::logDetailedSummary()
{
}
//}}}
//----------------------------------------------------------------------------------------------------------------------
// iDLLManager
//{{{
void QAPI cDLLManager::installDLLs(const unsigned long MinGroup, const unsigned long MaxGroup) throw(cRecoverable)
{
  _scanDLLs();
  installDLLsNoFileScan(MinGroup, MaxGroup);
}
//}}}
//{{{
void QAPI cDLLManager::uninstallDLLs(const bool IsRemovingSettingsAndData)
{
  cLogIndentGuard Indent(0, L"Uninstalling DLLs");

  _scanDLLs();

  {
    cLockGuard ThreadSafety(&m_DLLsThreadSafety);
    for (
      multiset<cDLL>::iterator i = m_DLLs.begin();
      i != m_DLLs.end() && iBootManager::singleton().getState() < iBootManager::eBootState_CleanExiting;
      ++i)
    {

      // multiset<>::iterator was changed since this code was originially written - it is now a const iterator
      // to prevent element changes affecting multiset ordering.
      // This workaround is safe so long as 'dll' is not modified in a way that affects its ordering
      auto &Entry = const_cast<cDLL &>(*i);

      if (i->m_LMInstallAction == eInstallAction_Nothing) _uninstallDLL(Entry, IsRemovingSettingsAndData);
    }
  }
}
//}}}
//{{{
unsigned int cDLLManager::countDLLs(const unsigned long MinGroup, const unsigned long MaxGroup)
{
  unsigned int Result = 0;

  _scanDLLs();

  {
    cLockGuard ThreadSafety(&m_DLLsThreadSafety);
    for (multiset<cDLL>::const_iterator i = m_DLLs.begin(); i != m_DLLs.end(); ++i)
    {
      if (i->m_LoadOrderGroup >= (signed)MinGroup && i->m_LoadOrderGroup <= (signed)MaxGroup)
      {
        Result++;
      }
    }
  }

  return Result;
}
//}}}
//{{{
void cDLLManager::initDLLs(const unsigned long MinGroup, const unsigned long MaxGroup)
{
  _scanDLLs();
  initDLLsNoFileScan(MinGroup, MaxGroup);
}
//}}}
//{{{
vector<_ATL_OBJMAP_ENTRY> QAPI cDLLManager::getATLObjects() throw(cNotAvailable)
{
  vector<_ATL_OBJMAP_ENTRY> Result;

  const vector<cModuleRegistrar::Ptr> & Registrars = _getRegistrars();
  for (vector<cModuleRegistrar::Ptr>::const_iterator i = Registrars.begin(); i != Registrars.end(); ++i)
  {
    vector<_ATL_OBJMAP_ENTRY> Objects = (*i)->getATLObjects();
    for (vector<_ATL_OBJMAP_ENTRY>::const_iterator j = Objects.begin(); j != Objects.end(); ++j)
    {
      Result.push_back(*j);
    }
  }

  return Result;
}
//}}}
//{{{
multimap<HINSTANCE, unsigned int> cDLLManager::getATLTypeLibraries() throw(cNotAvailable)
{
  multimap<HINSTANCE, unsigned int> Result;

  const vector<cModuleRegistrar::Ptr> & Registrars = _getRegistrars();
  for (vector<cModuleRegistrar::Ptr>::const_iterator i = Registrars.begin(); i != Registrars.end(); ++i)
  {
    vector<unsigned int> Indices = (*i)->getATLTypeLibraryResourceIndices();
    for (vector<unsigned int>::const_iterator j = Indices.begin(); j != Indices.end(); ++j)
    {
      Result.insert(make_pair((*i)->getDllInstanceHandle(), *j));
    }
  }

  return Result;
}
//}}}
//{{{
iModuleInstance::Ptr QAPI cDLLManager::getInstance(const String & DeveloperName, const String & ModuleName) throw(cNotFound)
{
  cLockGuard ThreadSafety(&m_DLLsThreadSafety);
  for (multiset<cDLL>::iterator i = m_DLLs.begin(); i != m_DLLs.end(); ++i)
  {
    if (i->m_DeveloperName == DeveloperName && i->m_ModuleName == ModuleName)
    {
      if (!i->getInstance().isValid())
      {
        throw cNotFound(__FILE__, __LINE__, L"Module \"%s - %s\" not instantiated yet for getInstance()", DeveloperName.c_str(), ModuleName.c_str());
      }
      return i->getInstance();
    }
  }
  throw cNotFound(__FILE__, __LINE__, L"Module \"%s - %s\" not found by getInstance()", DeveloperName.c_str(), ModuleName.c_str());
}
//}}}
//{{{
void QAPI cDLLManager::registerDLLs()
{
  _getRegistrars();
}
//}}}
//{{{
bool QAPI cDLLManager::isInstallDllsNecessary()
{
  cLogIndentGuard Indent(0, L"Scanning DLLs for Install");

  _scanDLLs();

  {
    cLockGuard ThreadSafety(&m_DLLsThreadSafety);
    for(multiset<cDLL>::iterator i = m_DLLs.begin();
      i != m_DLLs.end() && iBootManager::singleton().getState() < iBootManager::eBootState_CleanExiting;  ++i)
    {
      if (i->m_LMInstallAction == eInstallAction_Install)
        return true;
    }
  }

  return false;
}
//}}}
//{{{
void QAPI cDLLManager::logChangedSettings()
{
  for (multiset<cDLL>::const_iterator i = m_DLLs.begin(); i != m_DLLs.end(); ++i)
    i->logChangedSettings();
}
//}}}
//{{{
bool QAPI cDLLManager::hasPatchedDLLs()
{
  return m_HasPatchedDLLs;
}
//}}}
//----------------------------------------------------------------------------------------------------------------------
// cDLLManager
//{{{
void cDLLManager::updateSharing(const String & DeveloperName, const String & ModuleName, const cVersion & ModuleVersion, const bool IsAdding)
{
  const String ModuleRegistryPath = cSystemInfo::singleton().getRegistryPathToModules() + DeveloperName + L"\\" + ModuleName + L"\\";

  const list<String> Versions = m_Rh->getSubKeys(ModuleRegistryPath);
  for (list<String>::const_iterator i = Versions.begin(); i != Versions.end(); ++i)
  {
    String Version(*i);
    _replaceChar(&Version, L"x", L"0");
    try
    {
      _removeSharing(true, DeveloperName, ModuleName, Version);
      _removeSharing(false, DeveloperName, ModuleName, Version);
    }
    catch (cBadFormat &)
    {
      // Probably old style "Security Passwords" caused cVersion ctor to fail
      QMSG((0, L"cDLLManager::updateSharing: Ignoring \"%s\"", Version.c_str()));
    }
  }
  if (IsAdding) _addSharing(DeveloperName, ModuleName, ModuleVersion);
}
//}}}
//{{{
HINSTANCE cDLLManager::loadRawDLL(
  const String &    filename,
  const bool        isResolvingSymbols) /* throw cWin32Error */
{
  HINSTANCE hDllInstance = 0;

  // --- Load the DLL suppressing error dialogs ----------------------------------------

  DWORD ExecFlags = LOAD_WITH_ALTERED_SEARCH_PATH;
  if (!isResolvingSymbols) ExecFlags |= DONT_RESOLVE_DLL_REFERENCES;

  {
// SJSJSJ WIP: XP only
//    cGuard::ConstPtr VEHGuard = cExceptionManager::singleton().addVectoredExceptionHandler();

    UINT PrevErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
    SetLastError(0);
    QMSG((0, L"LoadLibraryEx \"%s\"", filename.c_str()));

    /*
    Search order:
    The directory specified by the Filename path. (DLL_Code_Release folder)
    The current directory. (DLL_Data folder)
    The system directory. Use the GetSystemDirectory function to get the path of this directory.
    The 16-bit system directory. There is no function that obtains the path of this directory, but it is searched.
    The Windows directory. Use the GetWindowsDirectory function to get the path of this directory.
    The directories that are listed in the PATH environment variable.
    */
    hDllInstance = LoadLibraryEx(
      filename.c_str(),                // Filename of module
      NULL,                            // Reserved, must be NULL
      ExecFlags);                      // Entry-point execution flag
    SetErrorMode(PrevErrorMode);
  }

  if (!hDllInstance)
  {
    const DWORD LastError = GetLastError();
    throw cWin32Error(__FILE__, __LINE__, L"Tried to LoadLibraryEx() on \"%s\" - returned %d (0x%x, %s)", filename.c_str(), LastError, LastError, win32ErrorToString(LastError).c_str());
  }

  QMSG((0, L"Succeeded"));

  // --- Watch out for statically linked c runtimes --------------------------------------

  if (iExceptionManager::isSingletonConstructed())
  {
    if (cExceptionManager::singleton().resetUnhandledExceptionFilter()) // Has to reset?
    {
      throw cWin32Error(__FILE__, __LINE__, L"Danger! Static linkage to MSVC runtime libraries detected");
    }
  }

  return hDllInstance;
}
//}}}
//{{{
void cDLLManager::registerBuiltInDLL(
  const String & CodeFilename, // without extension
  const String & RegFilename,
  const String & DataPath,
  const String & DeveloperName,
  const String & ModuleName,
  const cVersion & ModuleVersion)
{
  cLockGuard ThreadSafety(&m_DLLsThreadSafety);
  m_DLLs.insert(
    cDLL(
      CodeFilename,        // (without extension)
      RegFilename,
      DataPath,
      DeveloperName,
      ModuleName,
      iModuleContext::eBuiltIn, // LoadOrderGroup
      ModuleVersion,
      kKernelSDKVersion,
      false,               // HasModuleRegisterFunc
      false,               // HasModuleInstallFunc
      false));             // HasModuleUninstallFunc
}
//}}}
//{{{
void cDLLManager::installDLLsNoFileScan(const unsigned long MinGroup, const unsigned long MaxGroup) throw(cRecoverable)
{
  cLogIndentGuard Indent(0, L"Installing DLLs");

  try
  {
    cLockGuard ThreadSafety(&m_DLLsThreadSafety);
    for (multiset<cDLL>::iterator i = m_DLLs.begin();
         i != m_DLLs.end() && iBootManager::singleton().getState() < iBootManager::eBootState_CleanExiting;
         ++i)
    {
      if (i->m_LoadOrderGroup >= (signed)MinGroup && i->m_LoadOrderGroup <= (signed)MaxGroup)
      {
        // multiset<>::iterator was changed since this code was originially written - it is now a const iterator
        // to prevent element changes affecting multiset ordering.
        // This workaround is safe so long as 'Entry' is not modified in a way that affects its ordering
        auto &Entry = const_cast<cDLL &>(*i);

        if ((i->m_LMInstallAction == eInstallAction_Install) || (i->m_CUInstallAction == eInstallAction_Install)) _installDLL(Entry);
      }
    }
  }
  catch (cInstallRequiresReboot & Reason)
  {
    iExceptionManager::singleton().reboot(L"%s", Reason.getMsg().c_str());
  }
}
//}}}
//{{{
void cDLLManager::initDLLsNoFileScan(const unsigned long MinGroup, const unsigned long MaxGroup)
{
  cLockGuard ThreadSafety(&m_DLLsThreadSafety);

  bool HasSucceeded = true;
  for (multiset<cDLL>::iterator i = m_DLLs.begin();
    i != m_DLLs.end() && iBootManager::singleton().getState() < iBootManager::eBootState_CleanExiting;
    ++i)
  {
    // multiset<>::iterator was changed since this code was originially written - it is now a const iterator
    // to prevent element changes affecting multiset ordering.
    // This workaround is safe so long as 'dll' is not modified in a way that affects its ordering
    cDLL & Entry = const_cast<cDLL &>(*i);

    if ((Entry.m_LMInstallAction == eInstallAction_Nothing) || (Entry.m_CUInstallAction == eInstallAction_Nothing))
    {
      if (Entry.m_LoadOrderGroup >= (signed)MinGroup && Entry.m_LoadOrderGroup <= (signed)MaxGroup)
      {
        cLogIndentGuard IndentGuard(kCtgLoadModuleDetail, L"Loading module \"%s\"", Entry.m_ModuleName.c_str());

        if (!Entry.isInitialised())
        {
          try
          {
            Entry.init();
          }
          catch (cRecoverable &)
          {
            HasSucceeded = false;
            Entry.m_HasProblem = true;
          }
        }
      }
    }
  }

  if (!HasSucceeded)
  {
    cLogIndentSOSGuard IndentGuard(L"*** Summary of load module results ***");
    _logDLLs(true /* IsOnlyShowingProblemDlls */);
  }
}
//}}}
//{{{
String cDLLManager::getDLLSearchPath() const
{
  return m_DLLSearchPath;
}
//}}}
//----------------------------------------------------------------------------------------------------------------------
// privates
//{{{
void cDLLManager::_installDLL(cDLL & AboutDLL) throw(cRecoverable, cInstallRequiresReboot)
{
  QVERIFY((AboutDLL.m_LMInstallAction == eInstallAction_Install) || (AboutDLL.m_CUInstallAction == eInstallAction_Install));

  cLogIndentGuard Indent(0, L"Installing \"%s - %s\"", AboutDLL.m_DeveloperName.c_str(), AboutDLL.m_ModuleName.c_str());

  StringStream InstallReason;
  {
    if (AboutDLL.isLMFirstTimeInstall()) InstallReason << L"First time install, ";
    if (AboutDLL.isLMUpgrading())        InstallReason << L"Upgrading, ";
    if (AboutDLL.isLMDowngrading())      InstallReason << L"Downgrading, ";
    if (AboutDLL.isRelocating())       InstallReason << L"Relocating, ";
    if (AboutDLL.isLMMigrating())        InstallReason << L"Migrating, ";
    QMSG((0, L"Reason: %s", InstallReason.str().c_str()));
  }

  if (AboutDLL.isLMFirstTimeInstall())
  {
    if (!cSystemInfo::singleton().isUserAnAdministrator())
    {
      QFATAL((__FILE__, __LINE__, L"Administrator privileges required for install procedure"));
    }
  }

  AboutDLL.pushPaths();

  try
  {
    cProgressMsgGuard progress(L"Examining \"%s - %s\"", AboutDLL.m_DeveloperName.c_str(), AboutDLL.m_ModuleName.c_str());

    // Write registry as default if missing
    if (!m_Rh->isKeyCreated(AboutDLL.m_CurrentVersionRegPath))
    {
      AboutDLL.restoreSettings(false);
    }
    unsigned int NewInstallState = 2; // 0=uninstalled, 1=ignored, 2=installed
    m_Rh->setItemNumber(AboutDLL.m_RegPath, L"Install state (0=uninstalled, 1=ignored, 2=installed)", 0);

    try
    {
      AboutDLL.load(true /* resolve symbols? */);
    }
    catch (cWin32Error & reason)
    {
      AboutDLL.m_Status += L"Ignore because LoadLibrary failed (" + reason.getMsg() + String(L"), ");
      QMSG((0, L"%s", reason.getMsg().c_str()));
      // if we can't load the dll then we're missing a dependency so it will never work
      if (GetLastError() == 0x7e)
        NewInstallState = 1;
      else
        NewInstallState = 0; // 0=uninstalled, 1=ignored, 2=installed
    }

    cModuleInstance::Ptr OptionalOutgoingInstance;
    if (!AboutDLL.m_PreviousVersion.empty())
      OptionalOutgoingInstance = new cModuleInstance(AboutDLL.m_hInstance, AboutDLL.m_DataPath, AboutDLL.m_DeveloperName, AboutDLL.m_ModuleName, AboutDLL.m_PreviousVersion, AboutDLL.m_PreviousVersionRegistry);

    // Migrate settings if required
    if (AboutDLL.isLMMigrating())
    {
      AboutDLL.migrateLMSettings();
      _removeSharing(false, AboutDLL.m_DeveloperName, AboutDLL.m_ModuleName, AboutDLL.m_PreviousVersion);
    }

    // having migrated the Local Machine values we can sort out the Current User values
    if (!m_Urh->isKeyCreated(AboutDLL.m_CurrentVersionRegPath))
    {
      AboutDLL.restoreSettings(true);
    }

    if (AboutDLL.isCUMigrating())
    {
      AboutDLL.migrateCUSettings();
      _removeSharing(true, AboutDLL.m_DeveloperName, AboutDLL.m_ModuleName, AboutDLL.m_PreviousUserVersion);
    }

    if ((AboutDLL.m_LMInstallAction == eInstallAction_Install) && NewInstallState == 2 && AboutDLL.m_HasModuleInstallFunc)
    {
      moduleInstallFuncPtr funcptr = (moduleInstallFuncPtr)_findFuncInDll(AboutDLL.m_hInstance, L"moduleInstall");
      QVERIFY(funcptr);

      cModuleInstallHelp::Ptr Help = new cModuleInstallHelp;

      cLogIndentGuard IndentGuard(0, L"Installing");
      cProgressMsgGuard progress(L"Installing \"%s - %s\"", AboutDLL.m_DeveloperName.c_str(), AboutDLL.m_ModuleName.c_str());

      try
      {
        funcptr(AboutDLL.getFlags(), AboutDLL.getInstance(), OptionalOutgoingInstance, Help);
      }
      catch (cInstallRequiresReboot & Reason)
      {
        AboutDLL.m_Status += L"Reboot required (" + Reason.getMsg() + L"), ";
        QMSG((0, L"Exception: %s", AboutDLL.m_Status.c_str()));
        throw;
      }
      catch (cIgnoreInstallFailed & Reason)
      {
        AboutDLL.m_Status += L"Ignore because install failed (" + Reason.getMsg() + String(L"), ");
        QMSG((0, L"Exception: %s", AboutDLL.m_Status.c_str()));
        NewInstallState = 1; // 0=uninstalled, 1=ignored, 2=installed
      }
      catch (cRecoverable & Reason)
      {
        AboutDLL.m_Status += L"\"" + AboutDLL.m_DeveloperName + L" " + AboutDLL.m_ModuleName + L"\" install failed (" + Reason.getMsg() + L"), ";
        QMSG((0, L"Exception: %s", AboutDLL.m_Status.c_str()));
        throw cNotAvailable(narrow(Reason.getSrcFilename()).c_str(), Reason.getSrcLineNumber(), L"%s", AboutDLL.m_Status.c_str());
      }
    }

    if (NewInstallState == 2)
    {
      AboutDLL.m_Status += String(L"Installed (") + InstallReason.str() + L"), ";
      QMSG((0, L"Success: %s", InstallReason.str().c_str())) ;
      AboutDLL.m_LMInstallAction = eInstallAction_Nothing;
      AboutDLL.m_CUInstallAction = eInstallAction_Nothing;
    }
    else
    {
      AboutDLL.m_LMInstallAction = eInstallAction_Ignore;
      AboutDLL.m_CUInstallAction = eInstallAction_Ignore;
    }

    // Record new version and location in registry
    m_Rh->setItemString(AboutDLL.m_RegPath, L"Current version", AboutDLL.m_InternalVersion.serialise());
    m_Rh->setItemString(AboutDLL.m_RegPath, L"Current location", AboutDLL.m_CodeFilename);
    if (AboutDLL.mHasCurrentUserRegistry)
    {
      m_Urh->setItemString(AboutDLL.m_RegPath, L"Current version", AboutDLL.m_InternalVersion.serialise());
      m_Urh->setItemString(AboutDLL.m_RegPath, L"Current location", AboutDLL.m_CodeFilename);
    }
    updateSharing(AboutDLL.m_DeveloperName, AboutDLL.m_ModuleName, AboutDLL.m_InternalVersion, true /* is adding? */);
    m_Rh->setItemNumber(AboutDLL.m_RegPath, L"Install state (0=uninstalled, 1=ignored, 2=installed)", NewInstallState);
    if (AboutDLL.mHasCurrentUserRegistry)
      m_Urh->setItemNumber(AboutDLL.m_RegPath, L"Install state (0=uninstalled, 1=ignored, 2=installed)", NewInstallState);
  }
  catch (cRecoverable & reason)
  {
    AboutDLL.popPaths();
    QMSG((0, L"%s", reason.getMsg().c_str()));
  }

  AboutDLL.popPaths();
}
//}}}
//{{{
void cDLLManager::_uninstallDLL(cDLL & AboutDLL, const bool IsRemovingSettingsAndData) throw(cRecoverable)
{
  cLogIndentGuard Indent(0, L"Uninstalling \"%s - %s\"", AboutDLL.m_DeveloperName.c_str(), AboutDLL.m_ModuleName.c_str());

  QVERIFY(AboutDLL.m_LMInstallAction == eInstallAction_Nothing);
  if (!cSystemInfo::singleton().isUserAnAdministrator())
  {
    QFATAL((__FILE__, __LINE__, L"Administrator privileges required for uninstall procedure"));
  }

  AboutDLL.pushPaths();

  try
  {
    cProgressMsgGuard progress(L"Examining \"%s - %s\"", AboutDLL.m_DeveloperName.c_str(), AboutDLL.m_ModuleName.c_str());
    AboutDLL.load(true /* resolve symbols? */);

    const unsigned int ShareCount = _removeSharing(true, AboutDLL.m_DeveloperName, AboutDLL.m_ModuleName, AboutDLL.m_InternalVersion)
                                  + _removeSharing(false, AboutDLL.m_DeveloperName, AboutDLL.m_ModuleName, AboutDLL.m_InternalVersion);

    m_Rh->setItemNumber(AboutDLL.m_RegPath, L"Install state (0=uninstalled, 1=ignored, 2=installed)", 0);
    m_Rh->setItemString(AboutDLL.m_RegPath, L"Current location", L"");
    m_Urh->setItemNumber(AboutDLL.m_RegPath, L"Install state (0=uninstalled, 1=ignored, 2=installed)", 0);

    if (AboutDLL.m_LMInstallAction == eInstallAction_Nothing)
    {
      if (AboutDLL.m_HasModuleUninstallFunc)
      {
        moduleUninstallFuncPtr funcptr = (moduleUninstallFuncPtr)_findFuncInDll(AboutDLL.m_hInstance, L"moduleUninstall");
        QVERIFY(funcptr);

        cLogIndentGuard IndentGuard(0, L"Uninstalling");
        cProgressMsgGuard progress(L"Uninstalling \"%s - %s\"", AboutDLL.m_DeveloperName.c_str(), AboutDLL.m_ModuleName.c_str());

        try
        {
          funcptr(AboutDLL.getInstance(), new cModuleInstallHelp, IsRemovingSettingsAndData);
          AboutDLL.m_Status += L"Uninstalled, ";
        }
        catch (cRecoverable & Reason)
        {
          AboutDLL.m_Status += L"\"" + AboutDLL.m_DeveloperName + L" " + AboutDLL.m_ModuleName + L"\" uninstall failed (" + Reason.getMsg() + String(L"), ");
          QMSG((0, L"%s", AboutDLL.m_Status.c_str()));
          throw cNotAvailable(narrow(Reason.getSrcFilename()).c_str(), Reason.getSrcLineNumber(), L"%s", AboutDLL.m_Status.c_str());
        }
      }
    }

    // Delete registry settings
    if (IsRemovingSettingsAndData && ShareCount == 0)
    {
      try { m_Rh->deleteSubKey(AboutDLL.m_RegPath, AboutDLL.m_InternalVersionFormattedAsRegistryKey); } catch (cRecoverable &) {}
      try { m_Urh->deleteSubKey(AboutDLL.m_RegPath, AboutDLL.m_InternalVersionFormattedAsRegistryKey); } catch (cRecoverable &) {}
    }

    AboutDLL.m_LMInstallAction = eInstallAction_Install;
    AboutDLL.m_CUInstallAction = eInstallAction_Install;
  }
  catch (cRecoverable &)
  {
    AboutDLL.popPaths();
    throw;
  }

  AboutDLL.popPaths();
}
//}}}
//{{{
vector<cModuleRegistrar::Ptr> & cDLLManager::_getRegistrars() throw(cNotAvailable)
{
  cLockGuard ThreadSafety(&m_RegistrarsListThreadSafety);

  if (!m_HasBuiltRegistrarsList)
  {
    _scanDLLs();

    multiset<cDLL> CopyOfDLLs;
    {
      cLockGuard ThreadSafety(&m_DLLsThreadSafety);
      CopyOfDLLs = m_DLLs;
    }
    for (multiset<cDLL>::const_iterator i = CopyOfDLLs.begin(); i != CopyOfDLLs.end(); ++i)
    {
      cModuleRegistrar::Ptr r = i->getRegistrar();
      if (r.isValid()) m_Registrars.push_back(r);
    }

    m_HasBuiltRegistrarsList = true;
  }

  return m_Registrars;
}
//}}}
//{{{
void cDLLManager::_scanDLLs()
{
  cLockGuard ThreadSafety(&m_DLLsThreadSafety);
  if (!m_HasScannedDLLs)
  {
    cProgressMsgGuard progress(L"Finding modules");

    bool allowPlugins = false;
    try { (void)iSystemInfo::singleton().getCmdLineOption(L"allowplugins"); allowPlugins = true; } catch (cNotFound &) {}
    try { (void)iSystemInfo::singleton().getCmdLineOption(L"neotestapp"); allowPlugins = false; } catch (cNotFound &) {}

    bool allowPatching = false;
    try { (void)iSystemInfo::singleton().getCmdLineOption(L"allowpatching"); allowPatching = true; } catch (cNotFound &) {}
    try { (void)iSystemInfo::singleton().getCmdLineOption(L"disablepatching"); allowPatching = false; } catch (cNotFound &) {}

    struct SearchFolder
    {
      String Path;
      bool IsPluginFolder;
      bool IsPatchFolder;
    };
    std::vector<SearchFolder> searchFolders;

    // Where are main DLLs?
    try
    {
      SearchFolder folder = {_tidyPath(iSystemInfo::singleton().getBootFolder() + iSystemInfo::singleton().getCmdLineOption(L"dlls")), false, false};
      searchFolders.push_back(folder);
    }
    catch (cNotFound &)
    {
      SearchFolder folder = {_tidyPath(iSystemInfo::singleton().getBootFolder() + L"..\\" kDllCodeFolder), false, false};
      searchFolders.push_back(folder);
    }

    // Where are more DLLs?
    try
    {
      SearchFolder folder = {_tidyPath(iSystemInfo::singleton().getBootFolder() + iSystemInfo::singleton().getCmdLineOption(L"moredlls")) + kDllCodeFolder, false, false};
      searchFolders.push_back(folder);
    }
    catch (cNotFound &)
    {
    }

    // Where are plugin DLLs?
    if (allowPlugins)
    {
      SearchFolder folder = {_tidyPath(getInstance(L"vfs", L"_KernelExec")->getPublicSettingString(L"Plugins\\Additional plugins location4")) + kDllCodeFolder, true, false};
      searchFolders.push_back(folder);
    }

    // Where are patched DLLs?
    if (allowPatching)
    {
      try
      {
        SearchFolder folder = {_tidyPath(iSystemInfo::singleton().getBootFolder() + iSystemInfo::singleton().getCmdLineOption(L"patchdlls")) + kDllCodeFolder, false, false};
        searchFolders.push_back(folder);
      }
      catch (cNotFound &)
      {
        SearchFolder folder = {_tidyPath(ivfsEnvironment::singleton().getLocalMachineRootFolder() + L"Patches\\" + iSystemInfo::singleton().getProductName() + L' ' + iSystemInfo::singleton().getProductVersion() + L"\\" kDllCodeFolder), false, true};
        searchFolders.push_back(folder);
      }
    }

    // Scan for DLLs
    StringStream pluginsSummary;
    try
    {
      std::vector<String> regFolders;
      std::vector<String> dataFolders;

      m_DLLSearchPath = cSystemInfo::singleton().getBootFolder() + L";";

      for (std::vector<SearchFolder>::const_iterator it = searchFolders.begin(); it != searchFolders.end(); ++it)
      {
        if (it->Path == L"" || !iFileManager::singleton().isFileOrFolderCreated(it->Path))
          continue;

        regFolders.push_back(iFileManager::singleton().isFileOrFolderCreated(it->Path + L"..\\DLL_Registry\\") ? _tidyPath(it->Path + L"..\\DLL_Registry\\") : it->Path);
        dataFolders.push_back(iFileManager::singleton().isFileOrFolderCreated(it->Path + L"..\\DLL_Data\\") ? _tidyPath(it->Path + L"..\\DLL_Data\\") : it->Path);

        m_DLLSearchPath = it->Path + L";" + m_DLLSearchPath;
        cDLLIterator::Ptr scanner = new cDLLIterator(m_DLLs, 0, ULONG_MAX, regFolders, dataFolders);
        iFileManager::singleton().enumerateFiles(0, it->Path, scanner);

        list<cDLL> foundDLLs = scanner->getDLLsFound();
        for (list<cDLL>::const_iterator i = foundDLLs.begin(); i != foundDLLs.end(); ++i)
        {
          if (!i->m_PatchedDLLLocation.empty())
          {
            if (!it->IsPatchFolder) // patching only allowed in specified folders
            {
              QFATAL((__FILE__, __LINE__, L"DLL \"%s\" by \"%s\" found in multiple folders and patching is not enabled", i->m_ModuleName.c_str(), i->m_DeveloperName.c_str()));
            }
            m_HasPatchedDLLs = true;
          }

          if (it->IsPluginFolder)
            pluginsSummary << i->m_DeveloperName << L" " << i->m_ModuleName << L" " << i->m_InternalVersion.serialise() << L"\n";
        }
      }
    }
    catch (cRecoverable & Reason)
    {
      QFATAL((__FILE__, __LINE__, L"_scanDLLs() failed because \"%s\"", Reason.getMsg().c_str()));
    }

    // Write plugins summary to the registry so it can be shown in the Settings Panel
    if (allowPlugins)
    {
      if (pluginsSummary.str() == L"")
        pluginsSummary << L"None";
      getInstance(L"vfs", L"_KernelExec")->setPublicSettingString(L"Plugins\\Summary", pluginsSummary.str());
    }

    m_HasScannedDLLs = true;
  }
}
//}}}
//{{{
void cDLLManager::_logDLLs(const bool IsOnlyShowingProblemDlls)
{
  cLockGuard ThreadSafety(&m_DLLsThreadSafety);

  QMSG((0, L"Current KernelSDK version is %s", kKernelSDKVersion));
  QMSG((0, L""));
  QMSG((0, L"%-5s %-10s %-32s %-10s %-17s %-20s %s", L"Load", L"Developer", L"Module", L"Module",  L"Base",    L"Modified",    L"Status"));
  QMSG((0, L"%-5s %-10s %-32s %-10s %-17s %-20s %s", L"Order", L"",          L"",       L"Version", L"Address", L"Date & Time", L""      ));
  QMSG((0, L""));

  for (multiset<cDLL>::const_iterator i = m_DLLs.begin(); i != m_DLLs.end(); ++i)
  {
    const cDLL & Entry = *i;
    if (!IsOnlyShowingProblemDlls || Entry.m_HasProblem)
    {
      Char TmpString[16];
      QMSG((0, L"%-5s %-10s %-32s %-10s %-17p %-20s %s",
        Entry.m_LoadOrderGroup   == -1 ? L"Unknown" : _itow(Entry.m_LoadOrderGroup, TmpString, 10),
        Entry.m_DeveloperName.c_str(),
        Entry.m_ModuleName.c_str(),
        Entry.m_InternalVersion == kBadVer ? L"Unknown" : Entry.m_InternalVersion.serialise().c_str(),
        Entry.m_hInstance,
        (Entry.m_ModificationDate.serialise() + L" " + Entry.m_ModificationTime.serialise().substr(0, 8)).c_str(),
        Entry.m_Status.substr(0, Entry.m_Status.size() - 2).c_str()));
    }
  }
}
//}}}
//{{{
void cDLLManager::_addSharing(const String & DeveloperName, const String & ModuleName, const cVersion & Version) throw(cRecoverable)
{
  StringStream VersionFormattedAsRegistryKey;
  VersionFormattedAsRegistryKey << Version.m_Release << L"." << Version.m_MajorVersion << L"." << Version.m_MinorVersion << L".x";
  const String VersionRegistryPath = cSystemInfo::singleton().getRegistryPathToModules() + DeveloperName + L"\\" + ModuleName + L"\\" + VersionFormattedAsRegistryKey.str() + L"\\";

  vector<String> SharedBy;
  try { SharedBy = m_Rh->getItemMultiString(VersionRegistryPath, L"Shared by product versions"); } catch (cNotFound &) {}

  const String FullProductNameAndVersion(iSystemInfo::singleton().getProductName() + L" " + iSystemInfo::singleton().getProductVersion());
  bool IsAlreadyShared = false;
  {
    for (vector<String>::const_iterator i = SharedBy.begin(); i != SharedBy.end(); ++i)
    {
      if (toLower(*i) == toLower(FullProductNameAndVersion)) IsAlreadyShared = true;
    }
  }
  if (!IsAlreadyShared) SharedBy.push_back(FullProductNameAndVersion);
  m_Rh->setItemMultiString(VersionRegistryPath, L"Shared by product versions", SharedBy);
}
//}}}
//{{{
unsigned int cDLLManager::_removeSharing(bool currentUser, const String & DeveloperName, const String & ModuleName, const cVersion & Version) throw(cRecoverable)
// returns new share count
{
  StringStream VersionFormattedAsRegistryKey;
  VersionFormattedAsRegistryKey << Version.m_Release << L"." << Version.m_MajorVersion << L"." << Version.m_MinorVersion << L".x";
  const String VersionRegistryPath = cSystemInfo::singleton().getRegistryPathToModules() + DeveloperName + L"\\" + ModuleName + L"\\" + VersionFormattedAsRegistryKey.str() + L"\\";

  unsigned int ShareCount = 1;
  try
  {
    const vector<String> OrigSharedBy = m_Rh->getItemMultiString(VersionRegistryPath, L"Shared by product versions");
    vector<String> UpdatedSharedBy;
    const String FullProductNameAndVersion(iSystemInfo::singleton().getProductName() + L" " + iSystemInfo::singleton().getProductVersion());
    for (vector<String>::const_iterator i = OrigSharedBy.begin(); i != OrigSharedBy.end(); ++i)
    {
      if (toLower(*i) != toLower(FullProductNameAndVersion)) UpdatedSharedBy.push_back(*i);
    }
    if (!currentUser)
      m_Rh->setItemMultiString(VersionRegistryPath, L"Shared by product versions", UpdatedSharedBy);
    ShareCount = (unsigned int)UpdatedSharedBy.size();
  }
  catch (cNotFound &)
  {
  }
  return ShareCount;
}
//}}}
