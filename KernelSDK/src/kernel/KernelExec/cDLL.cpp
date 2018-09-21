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
// --- Pre-Compiled Headers --------------------------------------------------------------

#include "stdafx.h"

// --- Headers ---------------------------------------------------------------------------

#include "cDLL.h"
#include "cSystemInfo.h"
#include "cLogManager.h"
#include "cDLLManager.h"

// --- Start of Namespace ----------------------------------------------------------------

using namespace vfs;
using namespace std;




// =======================================================================================
// === cDLL ==============================================================================
// =======================================================================================

namespace
{

  bool _isHostedByKernelSetup()
  {
    bool Result = false;
    if (!Result) { try { (void)iSystemInfo::singleton().getCmdLineOption(L"install"); Result = true; } catch (cRecoverable &) {} }
    if (!Result) { try { (void)iSystemInfo::singleton().getCmdLineOption(L"uninstall"); Result = true; } catch (cRecoverable &) {} }
    if (!Result) { try { (void)iSystemInfo::singleton().getCmdLineOption(L"uninstalldeep"); Result = true; } catch (cRecoverable &) {} }
    return Result;
  }

  void _getDLLModificationDateAndTime(const String & FileName, cDate * const Date, cTime * const Time) throw(cWin32Error)
  {
    WIN32_FILE_ATTRIBUTE_DATA FileAttr = { 0 };
    if (GetFileAttributesEx(
      FileName.c_str(),                   // File or directory name
      GetFileExInfoStandard,              // Attribute
      &FileAttr) == 0)                    // Attribute information
    {
      throw cWin32Error(__FILE__, __LINE__, L"Tried to GetFileAttributesEx() on \"%s\"", FileName.c_str());
    }

    SYSTEMTIME FileModifiedTime;
    if (FileTimeToSystemTime(
      &FileAttr.ftLastWriteTime,          // File time to convert
      &FileModifiedTime) == 0)            // Receives system time
    {
      throw cWin32Error(__FILE__, __LINE__, L"Tried to FileTimeToSystemTime() for \"%s\"", FileName.c_str());
    }

    *Date = cDate(
      FileModifiedTime.wYear,
      FileModifiedTime.wMonth,
      FileModifiedTime.wDay);

    *Time = cTime(
      FileModifiedTime.wHour,
      FileModifiedTime.wMinute,
      FileModifiedTime.wSecond,
      0);
  }

}

void cDLL::checkCurrentUserInstallState ()
{
  iRegistryHelp::Ptr urh = createCurrentUserRegistryHelp();
  if (urh->isItemCreated(m_RegPath, L"Install state (0=uninstalled, 1=ignored, 2=installed)"))
  {
    const unsigned int InstallStateInRegistry = urh->getItemNumber(m_RegPath, L"Install state (0=uninstalled, 1=ignored, 2=installed)");
    switch (InstallStateInRegistry)
    {
    case 0: { m_CUInstallAction = eInstallAction_Install; break; }
    case 1: { m_CUInstallAction = eInstallAction_Ignore; break; }
    case 2: { m_CUInstallAction = eInstallAction_Nothing; break; }
    default: { QFATAL((__FILE__, __LINE__, L"Unexpected InstallStateInRegistry %d", InstallStateInRegistry)); }
    }
  }

  if (!m_PreviousUserVersion.empty() || !m_PreviousUserLocation.empty())
  {
    m_Flags &= ~fModuleInstall_User_FirstTimeInstall;

    const bool relocating = (toLower(m_PreviousUserLocation) != toLower(m_CodeFilename));
    cVersion CopyOfNewVersion(m_InternalVersion);
    cVersion ExistingCUVersion(m_PreviousUserVersion);

    // Reg & data only change when top 3 components in version number change so mask out bottom 1.
    ExistingCUVersion.m_Build = 0;
    CopyOfNewVersion.m_Build = 0;

    if (relocating || CopyOfNewVersion != ExistingCUVersion)
    {
      if (relocating)
      {
        m_Flags |= fModuleInstall_Relocating; // When running from a different location than last time
        if (!urh->isKeyCreated(m_CurrentVersionRegPath) && m_PreviousVersionRegistry != iRegistryHelp::eRegistryView_64Bit && !(CopyOfNewVersion < ExistingCUVersion))
          m_Flags |= fModuleInstall_User_Migrating; // Need to migrate from 32-bit to 64-bit
        m_CUInstallAction = eInstallAction_Install;
      }

      if (CopyOfNewVersion > ExistingCUVersion)
      {
        m_Flags |= fModuleInstall_User_Upgrading;
        if (!urh->isKeyCreated(m_CurrentVersionRegPath))
          m_Flags |= fModuleInstall_User_Migrating; // Only when upgrading and settings for new version don't already exist
        m_CUInstallAction = eInstallAction_Install;
      }
      else if (CopyOfNewVersion < ExistingCUVersion)
      {
        m_Flags |= fModuleInstall_User_Downgrading;
        m_CUInstallAction = eInstallAction_Install;
      }
    }
  }
}


cDLL::cDLL(
  const String &        CodeFilename,
  const String &        RegFilename,
  const String &        DataPath,
  const String &        DeveloperName,
  const String &        ModuleName,
  const int             LoadOrderGroup,
  const cVersion &      ModuleVersion,
  const cVersion &      BuiltWithSDKVersion,
  const bool            HasModuleRegisterFunc,
  const bool            HasModuleInstallFunc,
  const bool            HasModuleUninstallFunc) :
  m_HasModuleRegisterFunc(HasModuleRegisterFunc),
  m_HasModuleInstallFunc(HasModuleInstallFunc),
  m_HasModuleUninstallFunc(HasModuleUninstallFunc),
  m_CodeFilename(CodeFilename),
  m_RegFilename(RegFilename),
  m_DataPath(DataPath),
  m_DeveloperName(DeveloperName),
  m_ModuleName(ModuleName),
  m_LoadOrderGroup(LoadOrderGroup),
  m_IsLoaded(false),
  m_InternalVersion(ModuleVersion),
  m_BuiltWithSDKVersion(BuiltWithSDKVersion),
  m_IsInitialised(false),
  m_hInstance(0),
  m_HasSetPathEnvVar(0),
  m_HasSetFolder(0),
  m_HasProblem(false),
  m_IsRegistered(false),
  mHasCurrentUserRegistry(false),
  m_Flags(fModuleInstall_FirstTimeInstall + fModuleInstall_User_FirstTimeInstall), // Assume we need to install until we know better
  m_LMInstallAction(eInstallAction_Install), // Assume we need to install until we know better
  m_CUInstallAction(eInstallAction_Install) // Assume we need to install until we know better
{
  _getDLLModificationDateAndTime(m_CodeFilename, &m_ModificationDate, &m_ModificationTime);

  StringStream VersionFormattedAsRegistryKey;
  VersionFormattedAsRegistryKey << m_InternalVersion.m_Release << L"." << m_InternalVersion.m_MajorVersion << L"." << m_InternalVersion.m_MinorVersion << L".x";
  m_InternalVersionFormattedAsRegistryKey = VersionFormattedAsRegistryKey.str();

  m_RegPath = cSystemInfo::singleton().getRegistryPathToModules() + m_DeveloperName + L"\\" + m_ModuleName + L"\\";
  m_CurrentVersionRegPath = m_RegPath + m_InternalVersionFormattedAsRegistryKey + L"\\";

  mHasCurrentUserRegistry = hasUserSettings();


  // Look in both the 32-bit and 64-bit registries for settings to migrate
#ifdef _M_X64
  iRegistryHelp::eRegistryView kThisRegistryView = iRegistryHelp::eRegistryView_64Bit;
  iRegistryHelp::eRegistryView kOtherRegistryView = iRegistryHelp::eRegistryView_32Bit;
#else
  iRegistryHelp::eRegistryView kThisRegistryView = iRegistryHelp::eRegistryView_32Bit;
  iRegistryHelp::eRegistryView kOtherRegistryView = iRegistryHelp::eRegistryView_64Bit;
#endif
  findPreviousVersion(kThisRegistryView);
  findPreviousVersion(kOtherRegistryView);


  // Allow /reinstall on the command-line to force through a "first-time install". This helps if the registry is corrupt/confused.
  bool isForcingReinstall = false;
  try { (void)iSystemInfo::singleton().getCmdLineOption(L"reinstall"); isForcingReinstall = true; }
  catch (cRecoverable &) {}

  if (!isForcingReinstall)
  {
    {
      iRegistryHelp::Ptr rh = createRegistryHelp();
      if (rh->isItemCreated(m_RegPath, L"Install state (0=uninstalled, 1=ignored, 2=installed)"))
      {
        const unsigned int InstallStateInRegistry = rh->getItemNumber(m_RegPath, L"Install state (0=uninstalled, 1=ignored, 2=installed)");
        switch (InstallStateInRegistry)
        {
        case 0: { m_LMInstallAction = eInstallAction_Install; break; }
        case 1: { m_LMInstallAction = eInstallAction_Ignore; break; }
        case 2: { m_LMInstallAction = eInstallAction_Nothing; break; }
        default: { QFATAL((__FILE__, __LINE__, L"Unexpected InstallStateInRegistry %d", InstallStateInRegistry)); }
        }
      }
      if (!m_PreviousVersion.empty() || !m_PreviousLocation.empty())
      {
        m_Flags &= ~fModuleInstall_FirstTimeInstall;

        const bool relocating = (toLower(m_PreviousLocation) != toLower(m_CodeFilename));
        cVersion CopyOfNewVersion(m_InternalVersion);
        cVersion ExistingLMVersion(m_PreviousVersion);

        // Reg & data only change when top 3 components in version number change so mask out bottom 1.
        ExistingLMVersion.m_Build = 0;
        CopyOfNewVersion.m_Build = 0;

        if (relocating || CopyOfNewVersion != ExistingLMVersion)
        {
          if (relocating)
          {
            m_Flags |= fModuleInstall_Relocating; // When running from a different location than last time
            if (!rh->isKeyCreated(m_CurrentVersionRegPath) && m_PreviousVersionRegistry != kThisRegistryView && !(CopyOfNewVersion < ExistingLMVersion))
              m_Flags |= fModuleInstall_Migrating; // Need to migrate from 32-bit to 64-bit
		        if (!rh->isKeyCreated(m_CurrentVersionRegPath) || cSystemInfo::singleton().isUserAnAdministrator())
		          m_LMInstallAction = eInstallAction_Install;
          }

          if (CopyOfNewVersion > ExistingLMVersion)
          {
            m_Flags |= fModuleInstall_Upgrading;
            if (!rh->isKeyCreated(m_CurrentVersionRegPath))
              m_Flags |= fModuleInstall_Migrating; // Only when upgrading and settings for new version don't already exist
            if (!rh->isKeyCreated(m_CurrentVersionRegPath) || cSystemInfo::singleton().isUserAnAdministrator())
              m_LMInstallAction = eInstallAction_Install;
          }
          else if (CopyOfNewVersion < ExistingLMVersion)
          {
            m_Flags |= fModuleInstall_Downgrading;
            if (!rh->isKeyCreated(m_CurrentVersionRegPath) || cSystemInfo::singleton().isUserAnAdministrator())
              m_LMInstallAction = eInstallAction_Install;
          }
        }
      }
    }
    if (mHasCurrentUserRegistry)
      checkCurrentUserInstallState();
    else
      m_CUInstallAction = eInstallAction_Ignore;

  }

  if (m_LMInstallAction == eInstallAction_Ignore)
  {
    m_Status += L"LM Ignored, ";
  }

  if (m_CUInstallAction == eInstallAction_Ignore)
  {
    m_Status += L"CU Ignored, ";
  }

  if (_isHostedByKernelSetup())
  {
    m_Flags |= fModuleInstall_IsHostedByKernelSetup; // When being run by KernelSetup, i.e. a proper install
  }

  // Sanity: only one type of install!
  if ((m_Flags & fModuleInstall_FirstTimeInstall) != 0)
  {
    QVERIFY((m_Flags & fModuleInstall_Upgrading) == 0);
    QVERIFY((m_Flags & fModuleInstall_Downgrading) == 0);
  }
  if ((m_Flags & fModuleInstall_Upgrading) != 0)
  {
    QVERIFY((m_Flags & fModuleInstall_FirstTimeInstall) == 0);
    QVERIFY((m_Flags & fModuleInstall_Downgrading) == 0);
  }
  if ((m_Flags & fModuleInstall_Downgrading) != 0)
  {
    QVERIFY((m_Flags & fModuleInstall_FirstTimeInstall) == 0);
    QVERIFY((m_Flags & fModuleInstall_Upgrading) == 0);
  }

  // Sanity: only one type of install!
  if ((m_Flags & fModuleInstall_User_FirstTimeInstall) != 0)
  {
    QVERIFY((m_Flags & fModuleInstall_User_Upgrading) == 0);
    QVERIFY((m_Flags & fModuleInstall_User_Downgrading) == 0);
  }
  if ((m_Flags & fModuleInstall_User_Upgrading) != 0)
  {
    QVERIFY((m_Flags & fModuleInstall_User_FirstTimeInstall) == 0);
    QVERIFY((m_Flags & fModuleInstall_User_Downgrading) == 0);
  }
  if ((m_Flags & fModuleInstall_User_Downgrading) != 0)
  {
    QVERIFY((m_Flags & fModuleInstall_User_FirstTimeInstall) == 0);
    QVERIFY((m_Flags & fModuleInstall_User_Upgrading) == 0);
  }
}

// ---------------------------------------------------------------------------------------

bool QAPI cDLL::operator<(const cDLL & Other) const // Sort asc group then dsc date
{
  if (m_LoadOrderGroup == Other.m_LoadOrderGroup)
  {
    return m_ModuleName < Other.m_ModuleName;
  }
  else
  {
    return m_LoadOrderGroup < Other.m_LoadOrderGroup;
  }
}




// =======================================================================================
// === load/unload =======================================================================
// =======================================================================================

void cDLL::load(const bool isResolvingSymbols) /* throw cWin32Error */
{
  if (!m_IsLoaded)
  {
    cProgressMsgGuard progress(L"Loading version %s", m_InternalVersion.serialise().c_str());

    pushPaths();
    try
    {
      QVERIFY(m_hInstance == NULL);
      m_hInstance = cDLLManager::singleton().loadRawDLL(m_CodeFilename, isResolvingSymbols);
      m_Instance = new cModuleInstance(m_hInstance, m_DataPath, m_DeveloperName, m_ModuleName, m_InternalVersion, iRegistryHelp::eRegistryView_Default);
      m_IsLoaded = true;
    }
    catch (cRecoverable &)
    {
      popPaths();
      throw;
    }
    popPaths();

    {
      cLogIndentGuard IndentGuard(0, L"Add module to log manager");
      cLogManager::singleton().addModule(m_DeveloperName, m_ModuleName, m_InternalVersion, m_hInstance);
    }

  }
}

// ---------------------------------------------------------------------------------------

void cDLL::unload()
{
  if (m_IsLoaded)
  {
    FreeLibrary(m_hInstance);
    m_hInstance = 0;
    m_IsLoaded = false;
  }
}




// =======================================================================================
// === register_ =========================================================================
// =======================================================================================

namespace
{

  void * _findFuncInDll(const HINSTANCE DllInstance, const String &  FuncName)
  {
    void * const Func = GetProcAddress(DllInstance, narrow(FuncName).c_str());
    return Func;
  }

}

void cDLL::_register() throw(cRecoverable)
{
  if (!m_IsRegistered)
  {
    cProgressMsgGuard progress(L"Registering module \"%s\"", m_ModuleName.c_str());

    if (m_HasModuleRegisterFunc)
    {
      pushPaths();
      try
      {
        load(true /* resolve symbols? */);
        m_Registrar = new cModuleRegistrar(m_hInstance);
        moduleRegisterFuncPtr funcptr = (moduleRegisterFuncPtr)_findFuncInDll(m_hInstance, L"moduleRegister");
        funcptr(m_Registrar);
      }
      catch (cRecoverable & Reason)
      {
        popPaths();
        throw cNotAvailable(__FILE__, __LINE__, L"Loading DLL \"%s\" to enumerate ATL objects failed because \"%s\"", m_ModuleName.c_str(), Reason.getMsg().c_str());
      }
      popPaths();
    }
    m_IsRegistered = true;
  }
}




// =======================================================================================
// === init ==============================================================================
// =======================================================================================

void cDLL::init() throw(cRecoverable)
{
  if (!m_IsInitialised)
  {
    static unsigned int count = 0;
    count++;
    cProgressMsgGuard progress(L"Initialising module %d \"%s - %s\"", count, m_DeveloperName.c_str(), m_ModuleName.c_str());
    if (mHasCurrentUserRegistry)
      copyUserSettings();

    pushPaths();
    try
    {
      load(true /* resolve symbols? */);
      _register();

      {
        cLogIndentGuard IndentGuard(0, L"Initialisation");

        moduleInitFuncPtr Init = (moduleInitFuncPtr)_findFuncInDll(m_hInstance, L"moduleInit");
        if (Init)
        {
          Init(m_Instance);
        }

        // Success!
        m_Status += L"Init, ";
        m_IsInitialised = true;

        QMSG((0, L"Module init result = \"%s\"", m_Status.c_str()));
      }
    }
    catch (cRecoverable & reason)
    {
      m_Status += L"Init failed (" + reason.getMsg() + L"), ";
      QMSG((0, L"%s", m_Status.c_str()));
      popPaths();
      throw;
    }

    popPaths();
  }
}




// =======================================================================================
// === pushPaths/popPaths ================================================================
// =======================================================================================

namespace
{

  typedef BOOL(__stdcall *setDllDirectoryFuncPtr) (IN LPCWSTR lpPathName);

  void _setDllDirectory(const String & Path) throw(cNotSupported)
  {
    // SetDllDirectory is in xp sp1 or later
    const setDllDirectoryFuncPtr FuncPtr = (setDllDirectoryFuncPtr)GetProcAddress(GetModuleHandle(L"kernel32"), "SetDllDirectoryW");
    if (FuncPtr == NULL) throw cNotSupported(__FILE__, __LINE__, L"SetDllDirectory() not found in kernel32"); // Must be running on 2000
    if (FuncPtr(Path.c_str()) == 0)
    {
      const int LastError = GetLastError();
      QFATAL((__FILE__, __LINE__, L"Tried to SetDllDirectory(\"%s\") - %d (%s)", Path.c_str(), LastError, win32ErrorToString(LastError).c_str()));
    }
  }

  void _unsetDllDirectory() throw(cNotSupported)
  {
    // SetDllDirectory is in xp sp1 or later
    const setDllDirectoryFuncPtr FuncPtr = (setDllDirectoryFuncPtr)GetProcAddress(GetModuleHandle(L"kernel32"), "SetDllDirectory");
    if (FuncPtr == NULL) throw cNotSupported(__FILE__, __LINE__, L"SetDllDirectory() not found in kernel32"); // Must be running on 2000
    if (FuncPtr(NULL) == 0)
    {
      const int LastError = GetLastError();
      QFATAL((__FILE__, __LINE__, L"Tried to SetDllDirectory(NULL) - %d (%s)", LastError, win32ErrorToString(LastError).c_str()));
    }
  }

  String _tidyPath(const String & Path)
  {
    vector<Char> Result;
    Result.resize((Path.size() + 1) * 2);
    if (_wfullpath(&Result[0], Path.c_str(), Result.size()) == NULL)
    {
      QFATAL((__FILE__, __LINE__, L"Tried to _wfullpath \"%s\"", Path.c_str()));
    }
    return &Result[0];
  }

}

void cDLL::pushPaths()
{
  // --- Temporarily change directory to the data folder -------------------------------

  if (m_HasSetFolder == 0)
  {
    m_OrigFolder = iFileManager::singleton().getCurrentFolder();
    (void)iFileManager::singleton().setCurrentFolder(m_DataPath);
    QMSG((0, L"Set current folder to \"%s\"", m_DataPath.c_str()));

    try
    {
      _setDllDirectory(m_DataPath);
      QMSG((0, L"Set dll folder to \"%s\"", m_DataPath.c_str()));
    }
    catch (cNotSupported &)
    {
      // Probably Windows 2000 so not a problem. Windows 2000 search use current directory and find our DLL_Data files,
      // later Windows versions will not so they need SetDllDirectory to find them.
    }
  }
  m_HasSetFolder++;

  // --- Temporarily modify the dll search path ------------------------------------------

  if (m_HasSetPathEnvVar == 0)
  {
    // Attempt to change path. Note that this only affects the environment variable of the
    // current process. The command processor's environment is not changed.
    m_OrigPathEnvVar = _wgetenv(L"PATH");
    const String sharedDataPath(_tidyPath(m_DataPath + L"..\\Shared\\"));
    const String newEnvPath(
      String(L"PATH=") +
      m_DataPath + L";" +
      sharedDataPath + L";" +
      cDLLManager::singleton().getDLLSearchPath() +
      m_OrigPathEnvVar + L";");

    if (_wputenv(newEnvPath.c_str()) == -1)
    {
      QFATAL((__FILE__, __LINE__, L"Tried to put environment variable PATH"));
    }
    QMSG((0, L"Set environment variable PATH to \"%s\"", _wgetenv(L"PATH")));
  }
  m_HasSetPathEnvVar++;
}

// ---------------------------------------------------------------------------------------

void cDLL::popPaths()
{
  if (m_HasSetPathEnvVar > 0)
  {
    m_HasSetPathEnvVar--;
    if (m_HasSetPathEnvVar == 0)
    {
      if (_wputenv(String(String(L"PATH=") + String(m_OrigPathEnvVar)).c_str()) == -1)
      {
        QFATAL((__FILE__, __LINE__, L"Tried to put environment variable PATH"));
      }
    }
  }

  if (m_HasSetFolder > 0)
  {
    m_HasSetFolder--;
    if (m_HasSetFolder == 0)
    {
      try { _unsetDllDirectory(); }
      catch (cNotSupported &) {}
      (void)iFileManager::singleton().setCurrentFolder(m_OrigFolder);
    }
  }
}




// =======================================================================================
// === migrateSettings ===================================================================
// =======================================================================================

cRegistryMigrator::cRegistryMigrator(const String & OldVersionRegistryPath, iRegistryHelp::Ptr srcRH) :
  m_OldVersionRegistryPath(OldVersionRegistryPath),
  mIsInPublicSettings(false),
  mIsInLogCategories(false),
  m_SrcRH(srcRH)
{
}

bool /* go into */ QAPI cRegistryMigrator::onSubKey(const String & WholeKeyPath, const list<String> & SubKeyStack, const iRegistryHelp::Ptr & destRH)
{
  // Descend into this sub key if it exists in old version too
  StringStream SubKeyPath;
  { for (list<String>::const_reverse_iterator i = SubKeyStack.rbegin(); i != SubKeyStack.rend(); ++i) { SubKeyPath << *i << L"\\"; } }
  m_SubKeyPath = SubKeyPath.str();
  mIsInPublicSettings = (m_SubKeyPath.find(k_PublicSettingsStr) != String::npos);
  mIsInUserSettings = (m_SubKeyPath.find(k_UserSettingsStr) != String::npos);
  mIsInLogCategories = (m_SubKeyPath.find(L"Log Categories") != String::npos);
  return m_SrcRH->isKeyCreated(m_OldVersionRegistryPath + m_SubKeyPath) || ((m_SubKeyPath == L"User Setting Flags\\") && m_SrcRH->isKeyCreated(m_OldVersionRegistryPath + L"Custom Settings\\"));
}

void QAPI cRegistryMigrator::onItem(const String & KeyPath, const String & ItemName, const list<String> & SubKeyStack, const iRegistryHelp::Ptr & destRH)
{
  if (!mIsInUserSettings && _isMigratable(ItemName))
  {
    String subKeyPath;
    if (m_SrcRH->isItemCreated(m_OldVersionRegistryPath + m_SubKeyPath, ItemName))
      subKeyPath = m_SubKeyPath;
    else if ((m_SubKeyPath == L"User Setting Flags\\") && (m_SrcRH->isItemCreated(m_OldVersionRegistryPath + L"Custom Settings\\", ItemName)))
      subKeyPath = L"Custom Settings\\";


    if (subKeyPath.length())
    {
      const iRegistryHelp::eItemType ItemType = m_SrcRH->getItemType(m_OldVersionRegistryPath + subKeyPath, ItemName);
        switch (ItemType)
        {
        case iRegistryHelp::eItemType_MultiString: { destRH->setItemMultiString(KeyPath, ItemName, m_SrcRH->getItemMultiString(m_OldVersionRegistryPath + subKeyPath, ItemName)); break; }
        case iRegistryHelp::eItemType_String: { destRH->setItemString(KeyPath, ItemName, m_SrcRH->getItemString(m_OldVersionRegistryPath + subKeyPath, ItemName)); break; }
        case iRegistryHelp::eItemType_Number: { destRH->setItemNumber(KeyPath, ItemName, m_SrcRH->getItemNumber(m_OldVersionRegistryPath + subKeyPath, ItemName)); break; }
        default: { QFATAL((__FILE__, __LINE__, L"Unexpected item type %d", ItemType)); }
        }
    }
  }
}

bool cRegistryMigrator::_isMigratable(const String & ItemName)
{
  bool IsMigratable = true;
  if (mIsInPublicSettings)
  {
    IsMigratable = false;

    if (ItemName.find(L"value") != String::npos || ItemName == L"Is enabled") IsMigratable = true;
  }
  else if (mIsInLogCategories)
  {
    IsMigratable = false;
  }
  else /* must be in custom settings */
  {
    if (ItemName == L"Shared by product versions") IsMigratable = false;
  }
  //      QTRACE((L"_isMigratable: %s, m_IsInPublicSettings %d, m_IsInLogCategories %d, IsMigratable %d", ItemName.c_str(), m_IsInPublicSettings, m_IsInLogCategories, IsMigratable));
  return IsMigratable;
}


// =======================================================================================
// === CurrentUserBuilder ===================================================================
// =======================================================================================

cCurrentUserBuilder::cCurrentUserBuilder(const String & OldVersionRegistryPath, iRegistryHelp::Ptr destRH, bool testOnly) :
  m_OldVersionRegistryPath(OldVersionRegistryPath),
  m_IsInPublicSettings(false),
  m_IsInUserSettings(false),
  m_HasCurrentUserSettings(false),
  m_DestRH(destRH),
  m_TestOnly(testOnly)
{
}

bool /* go into */ QAPI cCurrentUserBuilder::onSubKey(const String & WholeKeyPath, const list<String> & SubKeyStack, const iRegistryHelp::Ptr & srcRH)
{
  // Descend into this sub key if it exists in old version too
  StringStream SubKeyPath;
  { for (list<String>::const_reverse_iterator i = SubKeyStack.rbegin(); i != SubKeyStack.rend(); ++i) { SubKeyPath << *i << L"\\"; } }
  m_SubKeyPath = SubKeyPath.str();
  m_IsInPublicSettings = (m_SubKeyPath.find(k_PublicSettingsStr) != String::npos);
  m_IsInCustomSettings = (m_SubKeyPath.find(k_CustomSettingsStr) != String::npos);
  m_IsInUserSettings = false;// (m_SubKeyPath.find(k_UserSettingsStr) != String::npos);
  m_IsPublicUserSettings = false;

  return srcRH->isKeyCreated(m_OldVersionRegistryPath + m_SubKeyPath);
}

void QAPI cCurrentUserBuilder::onItem(const String & KeyPath, const String & ItemName, const list<String> & SubKeyStack, const iRegistryHelp::Ptr & srcRH)
{
  bool isUserCustomSetting = false;
  if (srcRH->isItemCreated(m_OldVersionRegistryPath + m_SubKeyPath, ItemName))
  {
    if (m_IsInPublicSettings)
    {
      if (srcRH->isItemCreated(m_OldVersionRegistryPath + m_SubKeyPath, L"User Setting"))
        m_IsPublicUserSettings = (srcRH->getItemNumber(m_OldVersionRegistryPath + m_SubKeyPath, L"User Setting") != 0);
      else if (m_DestRH->isItemCreated(m_OldVersionRegistryPath + m_SubKeyPath, L"User Setting"))
        m_IsPublicUserSettings = (m_DestRH->getItemNumber(m_OldVersionRegistryPath + m_SubKeyPath, L"User Setting") != 0);
    }

    if (m_IsInCustomSettings && m_SubKeyPath.length())
    {
      String subKeyPath = k_UserSettingsStr + m_SubKeyPath.substr(k_CustomSettingsStr.length());
      if (srcRH->isItemCreated(m_OldVersionRegistryPath + subKeyPath, ItemName))
        isUserCustomSetting = (srcRH->getItemNumber(m_OldVersionRegistryPath + subKeyPath, ItemName) != 0);
      else if (m_DestRH->isItemCreated(m_OldVersionRegistryPath + subKeyPath, ItemName))
        isUserCustomSetting = (m_DestRH->getItemNumber(m_OldVersionRegistryPath + subKeyPath, ItemName) != 0);
    }

    const iRegistryHelp::eItemType ItemType = srcRH->getItemType(m_OldVersionRegistryPath + m_SubKeyPath, ItemName);
    if (isUserCustomSetting || _isUserSetting(ItemName))
    {
      if (m_TestOnly)
      {
        m_HasCurrentUserSettings = true;
        return;
      }

      switch (ItemType)
      {
      case iRegistryHelp::eItemType_MultiString: { m_DestRH->setItemMultiString(KeyPath, ItemName, srcRH->getItemMultiString(m_OldVersionRegistryPath + m_SubKeyPath, ItemName)); break; }
      case iRegistryHelp::eItemType_String: { m_DestRH->setItemString(KeyPath, ItemName, srcRH->getItemString(m_OldVersionRegistryPath + m_SubKeyPath, ItemName)); break; }
      case iRegistryHelp::eItemType_Number: { m_DestRH->setItemNumber(KeyPath, ItemName, srcRH->getItemNumber(m_OldVersionRegistryPath + m_SubKeyPath, ItemName)); break; }
      default: { QFATAL((__FILE__, __LINE__, L"Unexpected item type %d", ItemType)); }
      }
    }
  }
}

bool cCurrentUserBuilder::_isUserSetting(const String & ItemName)
{
  bool UserSetting = false;
  if (m_IsInUserSettings) return true;

  if (m_IsInPublicSettings)
  {
    UserSetting = false;

    if (ItemName.find(L"value") != String::npos || ItemName == L"Is enabled")
    {
      // test if user setting
      if (m_IsPublicUserSettings)
        UserSetting = true;
    }
  }
  //      QTRACE((L"_isUserSetting: %s, m_IsInPublicSettings %d, m_IsInLogCategories %d, IsMigratable %d", ItemName.c_str(), m_IsInPublicSettings, m_IsInLogCategories, IsMigratable));
  return UserSetting;
}

namespace
{

  map<cVersion, String> _getSettingsPathsByVersion(const String & DeveloperName, const String & ModuleName, const bool isFullPath, iRegistryHelp::Ptr rh)
  {
    map<cVersion, String> Result;

    const String Path = cSystemInfo::singleton().getRegistryPathToModules() + DeveloperName + L"\\" + ModuleName + L"\\";

    list<String> SubKeys;
    try
    {
      SubKeys = rh->getSubKeys(Path);
    }
    catch (cNotFound &)
    {
    }

    for (list<String>::iterator i = SubKeys.begin(); i != SubKeys.end(); ++i)
    {
      const Char * const tmp1 = wcsstr(i->c_str(), L"x.x");
      if (tmp1 != 0)
      {
        // Old style - ignored
      }
      else
      {
        // New style
        const Char * const tmp1 = wcsstr(i->c_str(), L"x");
        if (tmp1 != 0)
        {
          const String tmp2 = i->substr(0, tmp1 - i->c_str()) + L"0";
          Result.insert(make_pair(cVersion(tmp2), isFullPath ? Path + *i : *i));
        }
      }
    }

    return Result;
  }

  String _buildRegPath(const String & DeveloperName, const String & ModuleName, const cVersion & Version)
  {
    StringStream tmp;
    tmp <<
      cSystemInfo::singleton().getRegistryPathToModules() <<
      DeveloperName << L"\\" <<
      ModuleName << L"\\" <<
      Version.m_Release << L"." << Version.m_MajorVersion << L"." << Version.m_MinorVersion << L".x\\";
    return tmp.str();
  }

  String _buildRegPathToLatest(const String & DeveloperName, const String & ModuleName, iRegistryHelp::Ptr rh) throw(cNotFound)
  {
    const map<cVersion, String> PathsByVersion(_getSettingsPathsByVersion(DeveloperName, ModuleName, true /* gen full path? */, rh));
    if (PathsByVersion.size() == 0) throw cNotFound(__FILE__, __LINE__, L"Settings for module \"%s - %s\" do not exist", DeveloperName.c_str(), ModuleName.c_str());
    return cSystemInfo::singleton().getRegistryPathToModules() + DeveloperName + L"\\" + ModuleName + L"\\" + PathsByVersion.rbegin()->second;
  }

}

void cDLL::migrateSettings(iRegistryHelp::Ptr srcRH, iRegistryHelp::Ptr destRH, const String & previousVersion, iRegistryHelp::eRegistryView  previousVersionRegistry)
{
  if (destRH->isKeyCreated(m_CurrentVersionRegPath))
  {
    list<String> MigrationSources;
    MigrationSources.push_back(_buildRegPath(m_DeveloperName, m_ModuleName, previousVersion));

    {
      cModuleRegistrar::Ptr Registrar = getRegistrar();
      if (Registrar.isValid())
      {
        const list<pair<String, String> > & Aliases = Registrar->getAliases();
        for (list<pair<String, String> >::const_iterator i = Aliases.begin(); i != Aliases.end(); ++i)
        {
          try
          {
            MigrationSources.push_back(_buildRegPathToLatest(i->first /* developer name */, i->second /* module name */, srcRH));
          }
          catch (cNotFound &)
          {
          }
        }
      }
    }

    for (list<String>::const_iterator i = MigrationSources.begin(); i != MigrationSources.end(); ++i)
    {
      if (srcRH->isKeyCreated(*i))
      {
        QMSG((0, L"Migrating settings from \"%s\" (%d-bit)", i->c_str(), (previousVersionRegistry == iRegistryHelp::eRegistryView_64Bit ? 64 : 32)));
        destRH->enumerateTree(m_CurrentVersionRegPath, new cRegistryMigrator(*i, srcRH));
        break;
      }
    }
  }
}


void cDLL::migrateLMSettings()
{
  iRegistryHelp::Ptr srcRH = createRegistryHelp(m_PreviousVersionRegistry);
  iRegistryHelp::Ptr destRH = createRegistryHelp();

  migrateSettings(srcRH, destRH, m_PreviousVersion, m_PreviousVersionRegistry);

}


void cDLL::migrateCUSettings()
{
  iRegistryHelp::Ptr srcURH = createCurrentUserRegistryHelp(m_PreviousVersionRegistry);
  iRegistryHelp::Ptr destURH = createCurrentUserRegistryHelp();

  migrateSettings(srcURH, destURH, m_PreviousUserVersion, iRegistryHelp::eRegistryView_64Bit);

}

void cDLL::copyUserSettings()
{
  iRegistryHelp::Ptr srcURH = createCurrentUserRegistryHelp();
  iRegistryHelp::Ptr destRH = createRegistryHelp();

  copyUserSettings(srcURH, destRH);

}



// =======================================================================================
// === cullSettingsVersions ==============================================================
// =======================================================================================

const unsigned int kMaxNumVersionsOfRegistrySettingsPerModule = 20;

void cDLL::cullSettingsVersions()
{
  cLogIndentGuard(0, L"Culling versions of settings");
  {
    iRegistryHelp::Ptr rh = createRegistryHelp();
    map<cVersion, String> SubKeysByVersion(_getSettingsPathsByVersion(m_DeveloperName, m_ModuleName, false /* gen full path? */, rh));
    while (SubKeysByVersion.size() > kMaxNumVersionsOfRegistrySettingsPerModule)
    {
      QMSG((0, L"Deleting settings for version %s", SubKeysByVersion.begin()->first.serialise().c_str()));
      rh->deleteSubKey(m_RegPath, SubKeysByVersion.begin()->second);
      SubKeysByVersion.erase(SubKeysByVersion.begin());
    }
  }
  {
    iRegistryHelp::Ptr urh = createCurrentUserRegistryHelp();
    map<cVersion, String> SubKeysByVersion(_getSettingsPathsByVersion(m_DeveloperName, m_ModuleName, false /* gen full path? */, urh));
    while (SubKeysByVersion.size() > kMaxNumVersionsOfRegistrySettingsPerModule)
    {
      QMSG((0, L"Deleting (Current User) settings for version %s", SubKeysByVersion.begin()->first.serialise().c_str()));
      urh->deleteSubKey(m_RegPath, SubKeysByVersion.begin()->second);
      SubKeysByVersion.erase(SubKeysByVersion.begin());
    }
  }
}




// =======================================================================================
// === restoreSettings ===================================================================
// =======================================================================================

void cDLL::restoreSettings(bool userSettings)
{
  if (m_RegFilename != L"" && iFileManager::singleton().isFileOrFolderCreated(m_RegFilename))
  {
    QMSG((0, L"Restoring registry settings from file \"%s\"", m_RegFilename.c_str()));
    if (userSettings)
    {
      // Local Machine copies should already be present
      cLogSuppressGuard Suppress; // stop iFileManager from logging "Opening file.." "Closing file.."
      iRegistryHelp::Ptr uRh = createCurrentUserRegistryHelp();
      iRegistryHelp::Ptr srcRH = createRegistryHelp();
      copyUserSettings(srcRH, uRh);
    }
    else
    {
      cLogSuppressGuard Suppress; // stop iFileManager from logging "Opening file.." "Closing file.."
      iRegistryHelp::Ptr rh = createRegistryHelp();
      rh->setSymbolValue(iRegistryHelp::eModulePath, m_CurrentVersionRegPath);
      rh->importRegEdit4File(new cMemoryView(iFileManager::singleton().openFile(m_RegFilename, fFileAccess_Read)->read()));
      if (!mHasCurrentUserRegistry && hasUserSettings()) // discovered user setting flags
      {
        mHasCurrentUserRegistry = true;
        m_CUInstallAction = eInstallAction_Install;// default back to install mode
        checkCurrentUserInstallState();
      }
    }


    cullSettingsVersions();
    m_Status += L"Reg written, ";
  }
}




// =======================================================================================
// === logChangedSettings ================================================================
// =======================================================================================

namespace
{
  typedef std::pair<iRegistryHelp::eItemType, String> tRegistryValue;
  typedef std::map<String, tRegistryValue> tRegistryMap;

  class cLogChangedSettingsIterator : public iRegistryHelp::iItemIterator, public cRefCount
  {
  public:
    cLogChangedSettingsIterator(String rootKeyPath, tRegistryMap& settings) : RootKeyPath(rootKeyPath), Settings(settings)
    {
      if (RootKeyPath.empty() || *RootKeyPath.rbegin() != L'\\')
        RootKeyPath += L'\\';
    }

    bool QAPI onSubKey(const String& WholeKeyPath, const std::list<String>& SubKeyStack, const iRegistryHelp::Ptr& Help) { return true; }

    void QAPI onItem(const String& KeyPath, const String& ItemName, const std::list<String>& SubKeyStack, const iRegistryHelp::Ptr& Help)
    {
      // Path must match RootKeyPath, and we also don't care about the top-level directory ("Current version", "Current location" etc.)
      if (KeyPath.size() <= RootKeyPath.size() || KeyPath.substr(0, RootKeyPath.size()) != RootKeyPath)
        return;

      const iRegistryHelp::eItemType type = Help->getItemType(KeyPath, ItemName);
      String value;

      switch (type)
      {
      case iRegistryHelp::eItemType_MultiString:
      {
        std::vector<String> multiString = Help->getItemMultiString(KeyPath, ItemName);
        for (std::vector<String>::const_iterator it = multiString.begin(); it != multiString.end(); ++it)
        {
          if (!value.empty())
            value += L'|';
          value += *it;
        }
        break;
      }
      case iRegistryHelp::eItemType_String:
      {
        value = Help->getItemString(KeyPath, ItemName);
        break;
      }
      case iRegistryHelp::eItemType_Number:
      {
        value = lexical_cast<String>(Help->getItemNumber(KeyPath, ItemName));
        break;
      }
      default:
      {
        return; // unknown type
      }
      }

      Settings.insert(tRegistryMap::value_type(
        toLower(KeyPath.substr(RootKeyPath.size())) + toLower(ItemName),
        tRegistryValue(type, value)));
    }

  private:
    String RootKeyPath;
    tRegistryMap& Settings;
  };
}

void cDLL::logChangedSettings() const
{
  tRegistryMap defaultSettings;
  tRegistryMap currentSettings;

  iRegistryHelp::Ptr rh = createRegistryHelp();

  // Load Default Settings from the registry file
  if (m_RegFilename != L"" && iFileManager::singleton().isFileOrFolderCreated(m_RegFilename))
  {
    iRegistryHelp::tRegEdit4Values defaultSettingsWithCase;

    try
    {
      cLogSuppressGuard Suppress; // stop iFileManager from logging "Opening file.." "Closing file.."
      rh->setSymbolValue(iRegistryHelp::eModulePath, L""); // no module path, so keys are just relative
      defaultSettingsWithCase = rh->getRegEdit4Values(new cMemoryView(iFileManager::singleton().openFile(m_RegFilename, fFileAccess_Read)->read()));
    }
    catch (cRecoverable&)
    {
      // treat as no default settings
    }

    // Convert keys and values to lowercase
    for (iRegistryHelp::tRegEdit4Values::const_iterator it = defaultSettingsWithCase.begin(); it != defaultSettingsWithCase.end(); ++it)
    {
      if (!it->first.first.empty())
        defaultSettings.insert(tRegistryMap::value_type(toLower(it->first.first) + L"\\" + toLower(it->first.second), it->second));
    }
  }

  // Load Current Settings from the registry itself
  rh->enumerateTree(m_CurrentVersionRegPath, new cLogChangedSettingsIterator(m_CurrentVersionRegPath, currentSettings));

  // Find all the keys in both maps
  std::set<String> unionKeys;
  for (tRegistryMap::const_iterator it = defaultSettings.begin(); it != defaultSettings.end(); ++it)
    unionKeys.insert(it->first);
  for (tRegistryMap::const_iterator it = currentSettings.begin(); it != currentSettings.end(); ++it)
    unionKeys.insert(it->first);

  // Collect differences
  std::vector<std::pair<String, std::pair<String, String> > > differences;
  size_t maxKeyLen = 14;
  size_t maxValueLen = 13;

  for (std::set<String>::const_iterator it = unionKeys.begin(); it != unionKeys.end(); ++it)
  {
    tRegistryMap::const_iterator defaultSetting = defaultSettings.find(*it);
    tRegistryMap::const_iterator currentSetting = currentSettings.find(*it);

    if (defaultSetting != defaultSettings.end() && currentSetting != currentSettings.end())
    {
      // Skip only if identical type AND value
      if (*defaultSetting == *currentSetting)
        continue;
    }

    String defaultValue;
    if (defaultSetting == defaultSettings.end())
      defaultValue = L"(none)";
    else if (defaultSetting->second.first == iRegistryHelp::eItemType_String)
      defaultValue = L"\"" + defaultSetting->second.second + L"\"";
    else
      defaultValue = defaultSetting->second.second;

    String currentValue;
    if (currentSetting == currentSettings.end())
      currentValue = L"(none)";
    else if (currentSetting->second.first == iRegistryHelp::eItemType_String)
      currentValue = L"\"" + currentSetting->second.second + L"\"";
    else
      currentValue = currentSetting->second.second;

    maxKeyLen = Max(maxKeyLen, it->size());
    maxValueLen = Max(Max(maxValueLen, defaultValue.size()), currentValue.size());
    differences.push_back(std::pair<String, std::pair<String, String> >(*it, std::pair<String, String>(defaultValue, currentValue)));
  }

  // If there are differences, print them to the log
  if (differences.empty())
    return;

  StringStream formatStr;
  formatStr << L"%-" << Min<size_t>(maxKeyLen, 200) << L"s | %-" << Min<size_t>(maxValueLen, 100) << L"s | %s";
  String format = formatStr.str();

  cLogIndentGuard Indent(0, L"%s", m_ModuleName.c_str());
  QMSG((0, format.c_str(), L"-- KEY NAME --", L"-- DEFAULT --", L"-- CURRENT --"));
  for (std::vector<std::pair<String, std::pair<String, String> > >::const_iterator it = differences.begin(); it != differences.end(); ++it)
    QMSG((0, format.c_str(), it->first.c_str(), it->second.first.c_str(), it->second.second.c_str()));
}




// =======================================================================================
// === findPreviousVersion ===============================================================
// =======================================================================================


bool cDLL::hasUserSettings()
{
  iRegistryHelp::Ptr rh = createRegistryHelp();
//  rh->setSymbolValue(iRegistryHelp::eModulePath, m_CurrentVersionRegPath);
  if (rh->isItemCreated(m_RegPath, L"Current version") && rh->isItemCreated(m_RegPath, L"Current location"))
    if (rh->isKeyCreated(m_CurrentVersionRegPath))
    {
    cCurrentUserBuilder::Ptr CUBuilder = new cCurrentUserBuilder(m_CurrentVersionRegPath, rh, true);
    rh->enumerateTree(m_CurrentVersionRegPath, CUBuilder);
    return CUBuilder->hasUserSettings();
  }
  return false;
}

void cDLL::copyUserSettings(iRegistryHelp::Ptr srcRH, iRegistryHelp::Ptr destRH)
{
  cCurrentUserBuilder::Ptr CUBuilder = new cCurrentUserBuilder(m_CurrentVersionRegPath, destRH, false);
  srcRH->enumerateTree(m_CurrentVersionRegPath, CUBuilder);
}

void cDLL::findPreviousVersion(vfs::iRegistryHelp::eRegistryView registryView)
{
  // If we have already found ourself then we don't need to continue searching.
  iRegistryHelp::Ptr rh = createRegistryHelp(registryView);
  if (toLower(m_PreviousLocation) != toLower(m_CodeFilename) || (cVersion(m_PreviousVersion) != m_InternalVersion))
  {
    if (rh->isItemCreated(m_RegPath, L"Current version") && rh->isItemCreated(m_RegPath, L"Current location"))
    {
      const String newVersion = rh->getItemString(m_RegPath, L"Current version");
      const String newLocation = rh->getItemString(m_RegPath, L"Current location");

      if (m_PreviousVersion.empty() ||
        (!newVersion.empty() && cVersion(m_PreviousVersion) < cVersion(newVersion)) ||
        (newLocation == m_CodeFilename))
      {
        m_PreviousVersionRegistry = registryView;
        m_PreviousVersion = newVersion;
        m_PreviousLocation = newLocation;
      }
    }
  }


  if (mHasCurrentUserRegistry)
  {
    iRegistryHelp::Ptr uRh = createCurrentUserRegistryHelp(registryView);
  }

  if (toLower(m_PreviousUserLocation) != toLower(m_CodeFilename) || (cVersion(m_PreviousUserVersion) != m_InternalVersion))
  {
    iRegistryHelp::Ptr uRh = createCurrentUserRegistryHelp(registryView);
    if (uRh->isItemCreated(m_RegPath, L"Current version") && uRh->isItemCreated(m_RegPath, L"Current location"))
    {
      const String newVersion = uRh->getItemString(m_RegPath, L"Current version");
      const String newLocation = uRh->getItemString(m_RegPath, L"Current location");

      if (m_PreviousUserLocation.empty() ||
        (!newVersion.empty() && cVersion(m_PreviousUserVersion) < cVersion(newVersion)) ||
        (newLocation == m_CodeFilename))
      {
        m_PreviousUserVersion = newVersion;
        m_PreviousUserLocation = newLocation;
      }
    }
  }
}

