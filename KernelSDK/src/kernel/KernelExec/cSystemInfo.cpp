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
#include "cSystemInfo.h"
#include "Version.h"
#include <time.h>
#include <sys/timeb.h>
#include "..\..\tools\win\KernelSetup\cEmbed.h"
#include "cModuleInstance.h"
#include "cDLLManager.h"
#include <psapi.h>
#include <stdlib.h>
#include <algorithm>
#include "cExceptionManager.h"
#include "QKernelSDKVersion.h"
#include <Aclapi.h>
#include "..\..\..\..\Shared\Crypto\cWinCrypto.h"
#include "cDetectGFlags.h"

using namespace std;
using namespace vfs;

typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
typedef BOOL (WINAPI *PGPI)(DWORD, DWORD, DWORD, DWORD, PDWORD);

#ifndef PRODUCT_PROFESSIONAL
#define PRODUCT_PROFESSIONAL                      0x30
#endif

//{{{
void QAPI createRootKeyWithAllAccess(const String &KeyPath)
{
  // Create a registry key with all access permissions so that all users
  // of the machine can access. If the key already exists then this has
  // no effect - i.e. does not change the existing permissions

  // Create a well-known SID for the Everyone group.
  SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
  PSID pEveryoneSID(NULL);
  if(!AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &pEveryoneSID))
  {
    throw cWin32Error(__FILE__, __LINE__, L"AllocateAndInitializeSid() failed, error \"%s\"", win32ErrorToString(GetLastError()));
  }

  // Initialize an EXPLICIT_ACCESS structure for an ACE
  // The ACE will allow Everyone full access to the key
  // Set Inheritance so all sub keys inherit
  EXPLICIT_ACCESS ea[2];
  ZeroMemory(&ea, 2 * sizeof(EXPLICIT_ACCESS));
  ea[0].grfAccessPermissions = KEY_ALL_ACCESS;
  ea[0].grfAccessMode        = SET_ACCESS;
  ea[0].grfInheritance       = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
  ea[0].Trustee.TrusteeForm  = TRUSTEE_IS_SID;
  ea[0].Trustee.TrusteeType  = TRUSTEE_IS_WELL_KNOWN_GROUP;
  ea[0].Trustee.ptstrName    = (LPTSTR)pEveryoneSID;

  // Create a SID for the BUILTIN\Administrators group.
  SID_IDENTIFIER_AUTHORITY SIDAuthNT = SECURITY_NT_AUTHORITY;
  PSID pAdminSID(NULL);
  if(!AllocateAndInitializeSid(&SIDAuthNT, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &pAdminSID))
  {
    throw cWin32Error(__FILE__, __LINE__, L"AllocateAndInitializeSid() failed, error \"%s\"", win32ErrorToString(GetLastError()));
  }

  // Initialize an EXPLICIT_ACCESS structure for an ACE
  // The ACE will allow the Administrators group full access to the key
  // Set Inheritance so all sub keys inherit
  ea[1].grfAccessPermissions = KEY_ALL_ACCESS;
  ea[1].grfAccessMode        = SET_ACCESS;
  ea[1].grfInheritance       = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
  ea[1].Trustee.TrusteeForm  = TRUSTEE_IS_SID;
  ea[1].Trustee.TrusteeType  = TRUSTEE_IS_GROUP;
  ea[1].Trustee.ptstrName    = (LPTSTR)pAdminSID;

  // Create a new ACL that contains the new ACEs.
  PACL pACL(NULL);
  DWORD dwRes = SetEntriesInAcl(2, ea, NULL, &pACL);
  if(ERROR_SUCCESS != dwRes)
  {
    throw cWin32Error(__FILE__, __LINE__, L"SetEntriesInAcl() failed, error \"%s\"", win32ErrorToString(dwRes));
  }

  // Initialize a security descriptor.
  SECURITY_DESCRIPTOR sd;
  if(!InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION))
  {
    throw cWin32Error(__FILE__, __LINE__, L"InitializeSecurityDescriptor() failed, error \"%s\"", win32ErrorToString(GetLastError()));
  }

  // Add the ACL to the security descriptor.
  if(!SetSecurityDescriptorDacl(&sd, TRUE, pACL, FALSE))
  {
    throw cWin32Error(__FILE__, __LINE__, L"SetSecurityDescriptorDacl() failed, error \"%s\"", win32ErrorToString(GetLastError()));
  }

  // Initialize a security attributes structure.
  SECURITY_ATTRIBUTES sa;
  sa.nLength              = sizeof(SECURITY_ATTRIBUTES);
  sa.lpSecurityDescriptor = &sd;
  sa.bInheritHandle       = FALSE;

  // Now create the key with security attributes
  DWORD dwDisposition;
  HKEY hkSub(NULL);
  long res = RegCreateKeyEx(HKEY_LOCAL_MACHINE, KeyPath.c_str(), 0, 0, 0, KEY_READ | KEY_WRITE, &sa, &hkSub, &dwDisposition);
  if(ERROR_SUCCESS != res)
  {
    switch(res)
    {
      case ERROR_ACCESS_DENIED: { throw cAccessDenied(__FILE__, __LINE__, L"Access denied to registry when creating \"%s\"", KeyPath.c_str()); }
      default:                  { throw cWin32Error(__FILE__, __LINE__, L"RegCreateKeyEx() on \"%s\" failed, error \"%s\"", KeyPath.c_str(), win32ErrorToString(res).c_str()); }
    }
  }

  // Cleanup
  if(pEveryoneSID) FreeSid(pEveryoneSID);
  if(pAdminSID) FreeSid(pAdminSID);
  if(pACL) LocalFree(pACL);
  if(hkSub) RegCloseKey(hkSub);
}
//}}}

//{{{
// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

cSystemInfo::cSystemInfo() :
  m_Version(kVersion),
  m_TimeStartedInMillisecs(0),
  m_PerfCounterTickFreqHz(0),
  m_PerfCounterTicksAtStart(0),
  m_CpuHz(0),
  m_HasTestedIfUserIsAnAdministrator(false),
  m_IsUserAnAdministrator(false)
{
}

// ---------------------------------------------------------------------------------------

cSystemInfo::~cSystemInfo() throw()
{
}
//}}}
//{{{
// =======================================================================================
// === _getProductDetail =================================================================
// =======================================================================================

String cSystemInfo::_getProductDetail(const String & DetailId) throw(cNotFound)
{
  String Result;

  // Embedded in Product.sys ?
  cEmbed::Ptr ExamineMe = new cEmbed(m_KernelExecFolder + L"Product.sys");
  try
  {
    Result = ExamineMe->extractString(DetailId, false /* is showing debug? */);
  }
  catch (cRecoverable &)
  {
    // In cmdline?
    try
    {
      Result = getCmdLineOption(DetailId);
    }
    catch (cRecoverable &)
    {
      // In product.txt file?
      try
      {
        iFile::Ptr ProductFile = iFileManager::singleton().openFile(m_KernelExecFolder + L"Product.txt", fFileAccess_Read);
        cMemory::Ptr ProductFileInMemory = ProductFile->read();
        m_CmdLine      = m_CmdLine + L" " + widen(string(ProductFileInMemory->getBytes(), ProductFileInMemory->getSize() / sizeof(char)).c_str());
        m_LowerCmdLine = m_CmdLine;
        transform(m_LowerCmdLine.begin(), m_LowerCmdLine.end(), m_LowerCmdLine.begin(), tolower);
        Result = getCmdLineOption(DetailId);
      }
      catch (cRecoverable & Reason)
      {
        throw cNotFound(__FILE__, __LINE__, L"Required information \"%s\" not found - expected in product.sys, cmdline or product.txt\n\n(%s)", DetailId.c_str(), Reason.getMsg().c_str());
      }
    }
  }

  return Result;
}
//}}}
//{{{
// =======================================================================================
// === init ==============================================================================
// =======================================================================================

void cSystemInfo::init(const String & CmdLine)
{
  m_CmdLine      = CmdLine;
  m_LowerCmdLine = CmdLine;
  transform(m_LowerCmdLine.begin(), m_LowerCmdLine.end(), m_LowerCmdLine.begin(), tolower);

  // --- Record system info --------------------------------------------------------------

  GetSystemInfo(&m_Win32SystemInfo);

  // Get processor speed from registry.
  m_CpuHz = createRegistryHelp()->getItemNumber(L"Hardware\\Description\\System\\CentralProcessor\\0", L"~MHz") * 1000;

  // --- Folders -------------------------------------------------------------------------

#if defined(_DEBUG)
  m_KernelExecFolder = _getDLLsFolder(GetModuleHandle(L"KernelExec_Dbg.dll"));
#else
  m_KernelExecFolder = _getDLLsFolder(GetModuleHandle(L"KernelExec.dll"));
#endif

  // --- Product details -----------------------------------------------------------------

  // Searches embedded, cmdline then product.txt for this information
  m_ProductDeveloperName   = _getProductDetail(L"developer");
  m_ProductName            = _getProductDetail(L"product");
  m_ProductVersionAsString = _getProductDetail(L"version");
  try
  {
    m_ProductVersion = cVersion(m_ProductVersionAsString); // "0.0.0.0" format?
    m_ProductVersionLetter = 'V';
  }
  catch (cBadFormat &)
  {
    String copyOfVersion(m_ProductVersionAsString.c_str()); // because scanf modifies

    int ReleaseMajor = 0, ReleaseMinor = 0, RevisionMajor = 0, RevisionMinor = 0;
    Char ReleaseType, RevisionType[800];

    // "c0 s" format instead?
    unsigned int nNumFieldsConverted = swscanf(copyOfVersion.c_str(), L"%c%d %s", &ReleaseType, &ReleaseMajor, &RevisionType);
    if (nNumFieldsConverted != 3)
    {
      // "c0.0.0.0" format instead?
      nNumFieldsConverted = swscanf(copyOfVersion.c_str(), L"%c%d.%d.%d.%d", &ReleaseType, &ReleaseMajor, &ReleaseMinor, &RevisionMajor, &RevisionMinor);
      if (nNumFieldsConverted != 5)
      {
        throw cBadFormat(__FILE__, __LINE__, L"Product version \"%s\" must be formatted at \"0.0.0.0\", \"c0.0.0.0\" or \"c0 s\"", m_ProductVersionAsString.c_str());
      }
    }
    m_ProductVersion = cVersion(ReleaseMajor, ReleaseMinor, RevisionMajor, RevisionMinor);
    m_ProductVersionLetter = ReleaseType;
  }
  if (String(L"DTSXCV").find(m_ProductVersionLetter) == String::npos)
    throw cBadFormat(__FILE__, __LINE__, L"Product version letter must be one of 'D', 'T', 'X', 'S', 'C' or 'V'");

  // Do we have a Build.sys id from the build system?
  try
  {
    cEmbed::Ptr ExamineMe = new cEmbed(m_KernelExecFolder + L"Build.sys");
    m_ProductBuildId = ExamineMe->extractString(L"build-id", false /* is showing debug? */);
  }
  catch (cRecoverable &)
  {
  }

  // Optional override for registry settings group
  try
  {
    m_ProductSettingsGroup = _getProductDetail(L"settings");
  }
  catch (cNotFound &)
  {
  }

  cExceptionManager::singleton().setDialogTitle(m_ProductName);

  // --- Try to find a product option code for this product name -------------------------

  m_ProductId = (eProductId) 0;
  
  const tProductNameToId* products = kProductNameToIdTable;
  for (; products->id != (eProductId) 0; ++products)
  {
    if (wcsicmp(m_ProductName.c_str(), products->name) == 0)
    {
      m_ProductId = products->id;
      m_ProductName = products->name; // fix any case insensitivity issues by using internally defined name
      break;
    }
  }

  // =====================================================================================
  // --- Examine performance counter -----------------------------------------------------
  LARGE_INTEGER TmpPerfCounterFreq;
  if (!QueryPerformanceFrequency(&TmpPerfCounterFreq))
  {
    QFATAL((__FILE__, __LINE__, L"Tried to QueryPerformanceFrequency()"));
  }
  m_PerfCounterTickFreqHz    = TmpPerfCounterFreq.QuadPart;
  //m_PerfCounterTickFreqPerMs = m_PerfCounterTickFreqHz / 1000;

  LARGE_INTEGER TmpCount;
  if (!QueryPerformanceCounter(&TmpCount))
  {
    QFATAL((__FILE__, __LINE__, L"Tried to QueryPerformanceCounter()"));
  }
  m_PerfCounterTicksAtStart = TmpCount.QuadPart;

  // Keep these 2 sections next to each other, so that Ticks start ~= cTime start
  {
    struct _timeb timebuffer;
    _ftime (&timebuffer);     // note: systemtime is only upated approx every 1/6 sec
    struct tm * localTime = localtime(&timebuffer.time);
    if (localTime == NULL) QFATAL((__FILE__, __LINE__, L"Tried to localtime() - returned %d", GetLastError()));
    m_TimeStarted = cTime(localTime->tm_hour, localTime->tm_min, localTime->tm_sec, timebuffer.millitm);
    m_DateStarted = cDate(localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday);
    m_TimeStartedInMillisecs = m_TimeStarted.getDurationInMillisecs();
  }
  // =====================================================================================

  // --- get username and password from command line (if present)
  // command line switch /usercredentials will be present if kernelBoot has got values from the user
  // otherwise can user /username & /userpassword on kernelboot or kernelServer
  try { m_UserName = getCmdLineOption(L"username"); } catch (cNotFound &) {}
  if (!m_UserName.empty())
  {
    try
    {
      m_UserPassword = getCmdLineOption(L"userpassword");
    }
    catch (cNotFound &)
    {
      try
      {
        String encryptedPassword = getCmdLineOption(L"encryptedpassword");
        if (!encryptedPassword.empty())
        {
          cWinCryptoHelper::decrypt(narrow(encryptedPassword), m_UserPassword);
        }
      }
      catch (cNotFound &) {}
    }
  }

  if (m_UserName.empty())
    m_UserPassword = String();

  // --- Init registry -------------------------------------------------------------------
  iRegistryHelp::Ptr rh = createRegistryHelp();
  try
  {
    if(!rh->isKeyCreated(L"Software\\vfs")) createRootKeyWithAllAccess(L"Software\\vfs");
    rh->setItemString(L"Software\\vfs", L"Do I have permission to write here?", L"Yes");
    rh->deleteItem(L"Software\\vfs", L"Do I have permission to write here?");
  }
  catch (cAccessDenied &)
  {
    throw cAccessDenied(__FILE__, __LINE__, L"Requires privileges for reading/writing to registry at HKEY_LOCAL_MACHINE\\SOFTWARE\\vfs");
  }

  if (m_ProductSettingsGroup == L"")
  {
    m_ProductSettingsGroup = m_ProductName;
    const String origSettingsGroup = m_ProductSettingsGroup;

    // If no settings, try to find historical settings to use instead
    if (!rh->isKeyCreated(getRegistryPathToModules()))
    {
      std::vector<String> historicalGroupsToTry;

      const tProductNameToOldProductName* historicalProduct = kProductNameToOldProductNameTable;
      for (; historicalProduct->currentName != NULL; ++historicalProduct)
      {
        if (wcsicmp(m_ProductName.c_str(), historicalProduct->currentName) == 0)
          historicalGroupsToTry.push_back(historicalProduct->legacyName);
      }

      if (m_ProductName != L"Kernel Packager" && m_ProductName != L"Kernel Setup" && m_ProductName != L"HyperSpace")
      {
        historicalGroupsToTry.push_back(L"Kernel Module Settings");
        historicalGroupsToTry.push_back(L"GenQ");
      }

      for (std::vector<String>::const_iterator it = historicalGroupsToTry.begin();
           it != historicalGroupsToTry.end();
           ++it)
      {
        m_ProductSettingsGroup = *it;
        if (rh->isKeyCreated(getRegistryPathToModules()))
          break; // break on first to exist
      }
    }

    // If no settings and no historical settings, use the original settings group
    if (!rh->isKeyCreated(getRegistryPathToModules()))
      m_ProductSettingsGroup = origSettingsGroup;
  }
}
//}}}
//{{{
// =======================================================================================
// === initBuiltInModules ================================================================
// =======================================================================================

//{{{
void cSystemInfo::initBuiltInModules()
{
  iRegistryHelp::Ptr rh = createRegistryHelp();

  bool IsUninstalling = false;
  try { (void)iSystemInfo::singleton().getCmdLineOption(L"uninstall"); IsUninstalling = true; } catch (cRecoverable &) {}
  try { (void)iSystemInfo::singleton().getCmdLineOption(L"uninstalldeep"); IsUninstalling = true; } catch (cRecoverable &) {}

  cDLLManager::singleton().registerBuiltInDLL(
#if defined(_DEBUG)
    m_KernelExecFolder + L"KernelExec_Dbg.dll", // CodeFilename without extension
#else
    m_KernelExecFolder + L"KernelExec.dll", // CodeFilename without extension
#endif
    m_KernelExecFolder + L"KernelExec.reg", // RegFilename
    m_KernelExecFolder, // DataPath
    L"vfs", // DeveloperName
    L"_KernelExec", // ModuleName
    kVersion);

  // Optional app reg file
  String HostReg;
  try { HostReg  = getCmdLineOption(L"hostreg"); } catch (cNotFound &) {}
  m_HostName = m_ProductName;
  try { m_HostName = getCmdLineOption(L"hostname"); } catch (cNotFound &) {}
  m_HostVersion = m_ProductVersion;
  try { m_HostVersion = cVersion(getCmdLineOption(L"hostversion")); } catch (cNotFound &) {}
  if (HostReg != L"")
  {
    if (!rh->isKeyCreated(getHostRegistryPath()))
    {
      try
      {
        rh->setSymbolValue(iRegistryHelp::eModulePath, getHostRegistryPath());
        rh->importRegEdit4File(
          new cMemoryView(iFileManager::singleton().openFile(m_KernelExecFolder + HostReg, fFileAccess_Read)->read()));
      }
      catch (cAccessDenied &)
      {
        throw cAccessDenied(__FILE__, __LINE__, L"Must be logged in with Administrator privileges to install %s", HostReg.c_str());
      }
    }
  }

  // Update host version, but keep count of what the version was before a possible upgrade

  bool oldVersionPresent = false;
  String regPath = cSystemInfo::singleton().getRegistryPathToModules();
  String keyPath = regPath + m_ProductDeveloperName + L"\\_" + m_HostName + L"\\";
  cVersion oldVersion;
  try
  {
    oldVersion = rh->getItemString(keyPath, L"Current version");
    oldVersionPresent = true;
  }
  catch (cNotFound &) {}  

  rh->setItemString(keyPath, L"Current version", m_HostVersion.serialise());

  std::vector<Char> exePath(MAX_PATH);
  GetModuleFileName(NULL, exePath.data(), exePath.size());
  rh->setItemString(keyPath, L"Current location", exePath.data());

  // Helpers for third-parties that are trying to sniff the registry
  rh->setItemString(regPath, L"Last run product", m_ProductName);
  rh->setItemString(regPath, L"Last run version", m_ProductVersionAsString);
  rh->setItemString(regPath, L"Last run location", exePath.data());
  rh->setItemString(regPath, L"Last run time", getCurrentUniversalDate().toString() + L' ' + getCurrentUniversalTime().toString());

  try
  {
    cVersion currentVersion(rh->getItemString(keyPath, L"Current version"));

    if (oldVersionPresent && currentVersion > oldVersion)  // migrate application registry
    {
      StringStream oldPath;
      oldPath << keyPath << oldVersion.m_Release << L"." << oldVersion.m_MajorVersion << L"." << oldVersion.m_MinorVersion << L".x\\";
      StringStream newPath;
      newPath << keyPath << currentVersion.m_Release << L"." << currentVersion.m_MajorVersion << L"." << currentVersion.m_MinorVersion << L".x\\";

      iRegistryHelp::Ptr srcRH = createRegistryHelp(iRegistryHelp::eRegistryView_64Bit);
      iRegistryHelp::Ptr destRH = createRegistryHelp();
      
      if (destRH->isKeyCreated(newPath.str()) && srcRH->isKeyCreated(oldPath.str()))
          destRH->enumerateTree(newPath.str(), new cRegistryMigrator(oldPath.str(), srcRH));
    }
  }
  catch (cNotFound &) {}

  m_HostModuleContext = new cModuleInstance(m_ProductDeveloperName, String(L"_") + m_HostName, m_HostVersion);
  cDLLManager::singleton().updateSharing(m_ProductDeveloperName, String(L"_") + m_HostName, m_HostVersion, !IsUninstalling /* is adding? */);
/*
  cLogManager::singleton().addModule(m_ProductDeveloperName, String(L"_") + m_HostName, m_ProductVersion, GetModuleHandle(NULL));
*/
}
//}}}

// =======================================================================================
// === initUsingRegistrySettings =========================================================
// =======================================================================================
/*
Used to detect if Windows has rebooted since last time we ran up
*/
//{{{
void cSystemInfo::initUsingRegistrySettings()
{
  struct __timeb64 tb;
  _ftime64(&tb);
  const Int64 SecondsSince1970 = tb.time;

  LARGE_INTEGER TicksSinceBoot;
  QueryPerformanceCounter(&TicksSinceBoot);
  const Int64 SecondsSinceBoot = TicksSinceBoot.QuadPart / m_PerfCounterTickFreqHz;
  const Int64 SecondsSince1970WhenBooted  = SecondsSince1970 - SecondsSinceBoot;

  iModuleInstance::Ptr KernelExecInstance = iDLLManager::singleton().getInstance(L"vfs", L"_KernelExec");
  const Int64 PrevCalc = _wtoi64(KernelExecInstance->getCustomSettingString(L"", L"When system booted (seconds since 1970)").c_str());

  Int64 Diff = SecondsSince1970WhenBooted - PrevCalc;
  if (Diff < 0) Diff = -Diff;
  const bool HasRebootedSinceLastRun = Diff > 5; /* within 5 seconds is considered no change */

  const bool IsRebootRequired = KernelExecInstance->getCustomSettingNumber(L"", L"Is reboot required?") != 0;
  if (IsRebootRequired && !HasRebootedSinceLastRun)
  {
    iExceptionManager::singleton().fatal(
      fFatal_ShowMsg,
      iExceptionManager::cFatalDiagnostics(EXIT_SUCCESS, "" /* src file */, 0 /* src line */, L"" /* heading */, L"Need to reboot before %s %s can start", m_ProductDeveloperName.c_str(), m_ProductName.c_str()));
  }

  {
    StringStream SecondsSince1970WhenBootedAsStr;
    SecondsSince1970WhenBootedAsStr << SecondsSince1970WhenBooted;
    KernelExecInstance->setCustomSettingString(L"", L"When system booted (seconds since 1970)", SecondsSince1970WhenBootedAsStr.str().c_str());
  }

  KernelExecInstance->setCustomSettingNumber(L"", L"Is reboot required?", 0);
}
//}}}

// =======================================================================================
// === elevatePrivileges =================================================================
// =======================================================================================

//{{{
cWin32UserImpersonationGuard::ConstPtr cSystemInfo::elevatePrivileges() throw(cWin32Error)
{
#if 0
  if (ImpersonateLoggedOnUser(m_LogonHandleGuard->m_Handle) == 0)
  {
    const DWORD ErrorCode = GetLastError();
    throw cWin32Error(__FILE__, __LINE__, L"Failed to ImpersonateLoggedOnUser - returned 0x%x (%s)", ErrorCode, win32ErrorToString(ErrorCode).c_str());
  }
#else
  return new cWin32UserImpersonationGuard;
#endif
}
//}}}
//}}}
//{{{
// =======================================================================================
// === getRegistryPathToModules ==========================================================
// =======================================================================================

String cSystemInfo::getRegistryPathToModules() const
{
  String Result = L"Software\\";
  Result += m_ProductDeveloperName + L"\\";
  Result += m_ProductSettingsGroup + L"\\";
  return Result;
}
//}}}
//{{{  getProductDeveloperName/getProductName/getProductVersion/getProductSettingsGroup
// =======================================================================================
// === getProductDeveloperName/getProductName/getProductVersion/getProductSettingsGroup ==
// =======================================================================================

String QAPI cSystemInfo::getProductDeveloperName() const
{
  return m_ProductDeveloperName;
}

// ---------------------------------------------------------------------------------------

String QAPI cSystemInfo::getProductName() const
{
  return m_ProductName;
}

// ---------------------------------------------------------------------------------------

enum eProductId QAPI cSystemInfo::getProductId() const throw(cNotFound)
{
  if (m_ProductId)
    return m_ProductId;
  else
    throw cNotFound(__FILE__, __LINE__, L"Product name \"%s\" not recognised", m_ProductName.c_str());
}

// ---------------------------------------------------------------------------------------

enum eProductId QAPI cSystemInfo::parseProductName (const String& strProductName) const throw(cNotFound)
{
  const tProductNameToId* products = kProductNameToIdTable;
  const Char* szProductName = strProductName.c_str();

  for (; products->id != (eProductId) 0; ++products)
  {
    if (wcsicmp (szProductName, products->name) == 0)
    {
      return products->id;
    }
  }
  
  throw cNotFound(__FILE__, __LINE__, L"Product name \"%s\" not recognised", szProductName);
  return eInvalidProduct;
}

// ---------------------------------------------------------------------------------------


String QAPI cSystemInfo::getProductVersion() const
{
  return m_ProductVersionAsString;
}

// ---------------------------------------------------------------------------------------

Char cSystemInfo::getProductVersionLetter() const
{
  return m_ProductVersionLetter;
}

// ---------------------------------------------------------------------------------------

cVersion QAPI cSystemInfo::getProductVersionNumber() const
{
  return m_ProductVersion;
}

// ---------------------------------------------------------------------------------------

String cSystemInfo::getProductBuildId() const
{
  return m_ProductBuildId;
}

// ---------------------------------------------------------------------------------------

String QAPI cSystemInfo::getProductSettingsGroup() const
{
  return m_ProductSettingsGroup;
}

// ---------------------------------------------------------------------------------------

String QAPI cSystemInfo::getHostName() const
{
  return m_HostName;
}

// ---------------------------------------------------------------------------------------

cVersion QAPI cSystemInfo::getHostVersion() const
{
  return m_HostVersion;
}

void QAPI cSystemInfo::setCurrentUserName(const String& username)
{
  m_UserName = username;
}

void QAPI cSystemInfo::setCurrentUserPassword(const String& userpassword)
{
  m_UserPassword = userpassword;
}

String QAPI cSystemInfo::getCurrentUserName() const
{
  return m_UserName;
}

String QAPI cSystemInfo::getCurrentUserPassword() const
{
  return m_UserPassword;
}
//}}}
//{{{
// =======================================================================================
// === logSummary/logDetailedSummary =====================================================
// =======================================================================================

void QAPI cSystemInfo::logSummary()
{
  {
    cLogIndentGuard IndentGuard(0, L"About package");
    _logPackageSummary();
  }
  {
    cLogIndentGuard IndentGuard(0, L"About build");
    _logBuildSummary();
  }
  {
    cLogIndentGuard IndentGuard(0, L"About system");
    _logSystemSummary();
  }
  {
    cLogIndentGuard IndentGuard(0, L"About user");
    bool IsAdmin;
    _examineSid(&IsAdmin);
    if (IsAdmin) QMSG((0, L"User is an administrator"));
    else         QMSG((0, L"User is not an administrator"));
  }

  detectGFlags();
}

// ---------------------------------------------------------------------------------------

void QAPI cSystemInfo::logDetailedSummary()
{
}
//}}}

//{{{
// =======================================================================================
// === getBootFolder =====================================================================
// =======================================================================================

String QAPI cSystemInfo::getBootFolder() const
{
  return m_KernelExecFolder;
}
//}}}
//{{{
// =======================================================================================
// === hasProcessorSME ===================================================================
// =======================================================================================
/*
Does the CPU have Streaming SIMD Extensions (SME) - i.e. Pentium 3 or better.
*/
/*
static bool hasProcessorSME(void) throw()
{
    bool Result = false;

    _asm
  {
    // Move the number 1 into eax - this will move the feature bits into EDX when a CPUID
    // is issued, that is, EDX will then hold the key to the cpuid
    mov eax, 1

    // Does this processor have SSE support?
    cpuid

    // Perform CPUID (puts processor feature info in EDX). Shift the bits in edx to the
    // right by 26, thus bit 25 (SSE bit) is now in CF bit in EFLAGS register.
    shr edx,0x1A

    // If CF is not set, jump over next instruction
    jnc nocarryflag

    // set the return value to 1 if the CF flag is set
    mov [Result], 1
nocarryflag:
    }

    return Result;
}
*/
//}}}
//{{{
// =======================================================================================
// === _getProcessFilename ===============================================================
// =======================================================================================

String cSystemInfo::_getProcessFilename(const HINSTANCE hInstance) const throw(cWin32Error)
{
  Char Buffer[512];
  if (GetModuleFileNameEx(
    GetCurrentProcess(),                        // Handle to the process
    hInstance,                                  // Handle to the module
    Buffer,                                     // -> Buffer that receives the path
    (sizeof(Buffer) - 1) / sizeof(Char)) == 0)  // Maximum number of characters to retrieve
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to GetModuleFileNameEx");
  }
  return Buffer;
}
//}}}
//{{{
// =======================================================================================
// === _getDLLsFolder =================================================================
// =======================================================================================

String cSystemInfo::_getDLLsFolder(const HINSTANCE hInstance) const throw(cWin32Error)
{
  Char Buffer[512];
  if (GetModuleFileNameEx(
    GetCurrentProcess(),                        // Handle to the process
    hInstance,                                  // Handle to the module
    Buffer,                                     // -> Buffer that receives the path
    (sizeof(Buffer) - 1) / sizeof(Char)) == 0)  // Maximum number of characters to retrieve
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to GetModuleFileNameEx");
  }
  // Chop name after last slash - we only want to return the folder path.
  Char * const LastSlash = wcsrchr(Buffer, L'\\');
  QVERIFY(LastSlash);
  LastSlash[1] = L'\0';
  return Buffer;
}
//}}}
//{{{
// =======================================================================================
// === _getVersion/_getOldestSupportedVersion ============================================
// =======================================================================================

cVersion cSystemInfo::_getVersion() const
{
  return m_Version;
}
//}}}

// =======================================================================================
// ---- ticks and time
//{{{
// =======================================================================================
// === getTicksSinceStart ================================================================
// =======================================================================================

UInt64 QAPI cSystemInfo::getTicksSinceStart() const
{
  LARGE_INTEGER TmpCount;
  if (!QueryPerformanceCounter(&TmpCount))
    return 0;
  return TmpCount.QuadPart - m_PerfCounterTicksAtStart;
}
//}}}
//{{{
// =======================================================================================
// === getTicksSinceStart ================================================================
// =======================================================================================

UInt64 QAPI cSystemInfo::getTicksFreq() const
{
  return m_PerfCounterTickFreqHz;
}
//}}}
//{{{
UInt64 QAPI cSystemInfo::getTicksSinceStart(UInt64 * const TickFreqHz) const
{
  if (TickFreqHz)
    *TickFreqHz = m_PerfCounterTickFreqHz;

  return getTicksSinceStart();
}
//}}}

//{{{
UInt64 QAPI cSystemInfo::convertToMillisec (UInt64 ticks) const
{
  // (ticks * 1000) / Freq : but without overflowing ticks
  UInt64 secs = ticks / m_PerfCounterTickFreqHz;
  UInt64 remainder = ticks - secs * m_PerfCounterTickFreqHz;
  UInt64 mSec = (remainder * 1000) / m_PerfCounterTickFreqHz;

  return secs * 1000 + mSec;
}
//}}}
//{{{
UInt64 QAPI cSystemInfo::convertToMicrosec (UInt64 ticks) const
{
  // (ticks * 1000000) / Freq : but without overflowing ticks
  UInt64 secs = ticks / m_PerfCounterTickFreqHz;
  UInt64 remainder = ticks - secs * m_PerfCounterTickFreqHz;
  UInt64 uSec = (remainder * 1000000) / m_PerfCounterTickFreqHz;

  return secs * 1000000 + uSec;
}
//}}}

//{{{
// =======================================================================================
// === getMillisecsSinceStart ============================================================
// =======================================================================================

UInt64 QAPI cSystemInfo::getMillisecsSinceStart() const
{
  return convertToMillisec (getTicksSinceStart());
}
//}}}
//{{{
// =======================================================================================
// === getElapsedSinceStart ==============================================================
// =======================================================================================

cTime QAPI cSystemInfo::getElapsedSinceStart() const
{
  cTime Result;
  Result.setDurationFromMillisecs(getMillisecsSinceStart());
  return Result;
}
//}}}

// double-secs is tick-accurate
//{{{
// =======================================================================================
// === getTicksSinceStart ================================================================
// =======================================================================================

double QAPI cSystemInfo::getSecsSinceStart() const
{
  return double(getTicksSinceStart()) / double(m_PerfCounterTickFreqHz);
}
//}}}
//{{{
// =======================================================================================
// === convertToTime ==============================================================
// =======================================================================================

double QAPI cSystemInfo::convertToSec(UInt64 ticks) const
{
  return double(ticks) / double(m_PerfCounterTickFreqHz);
}
//}}}

//{{{
// =======================================================================================
// === convertToTime ==============================================================
// =======================================================================================

cTime QAPI cSystemInfo::convertToTime(UInt64 ticksSinceStart) const
{
  UInt64 mSecs = convertToMillisec (ticksSinceStart) + m_TimeStartedInMillisecs;

  cTime result;
  result.setDurationFromMillisecs (mSecs % (24 * 60 * 60 * 1000));
  return result;
}
//}}}
//{{{
String QAPI cSystemInfo::toTimeString (UInt64 ticks) const     // including microsecs
{
  UInt64 secs = ticks / m_PerfCounterTickFreqHz;
  UInt64 remainder = ticks - secs * m_PerfCounterTickFreqHz;
  UInt64 microsecs = (remainder * 1000000) / m_PerfCounterTickFreqHz;

  UInt64 millisecs = microsecs / 1000;
  int uSec = (int)(microsecs - millisecs * 1000);

  millisecs += secs * 1000 + m_TimeStartedInMillisecs;
  int mSec = millisecs % (24 * 60 * 60 * 1000);

  int hour = mSec / (1000 * 60 * 60);
  mSec -= hour * (1000 * 60 * 60);

  int minute = mSec / (1000 * 60);
  mSec -= minute * (1000 * 60);

  int second = mSec / 1000;
  mSec -= second * 1000;

  Char buffer[32];
  swprintf_s  (buffer, 32, L"%02d:%02d:%02d.%03d %03d", hour, minute, second, mSec, uSec);
  return buffer;
}
//}}}

// =======================================================================================

//{{{
// =======================================================================================
// === _logSystemSummary/_logPackageSummary ==============================================
// =======================================================================================

void cSystemInfo::_logSystemSummary() const
{
  QMSG((0, L"# CPUs           : %d", getNumProcessors()));
  QMSG((0, L"CPU speed        : %s", getProcessorDescription().c_str()));
  QMSG((0, L"Physical memory  : %s", getPhysicalMemoryInMegaBytes().c_str()));
  QMSG((0, L"Operating System : %s", getOperatingSystem().c_str()));
  QMSG((0, L"System Type      : %s", getSystemArchitecture().c_str()));
  QMSG((0, L"Application      : %s", getApplicationArchitecture().c_str()));
}

// ---------------------------------------------------------------------------------------

void cSystemInfo::_logPackageSummary() const
{
  QMSG((0, L"Product  : %s", iSystemInfo::singleton().getProductName().c_str()));
  QMSG((0, L"Version  : %s", iSystemInfo::singleton().getProductVersion().c_str()));
  const String buildId = iSystemInfo::singleton().getProductBuildId();
  if (!buildId.empty())
    QMSG((0, L"Build ID : %s", buildId.c_str()));
  QMSG((0, L"Settings : %s", getRegistryPathToModules().c_str()));
}
//}}}
//{{{
// =======================================================================================
// === _logBuildSummary ==================================================================
// =======================================================================================

void cSystemInfo::_logBuildSummary() const
{
  QMSG((0, L"vfs Kernel"));

  StringStream Line1;
  #if defined(_DEBUG)
    Line1 << L"Debug build";
  #else
    Line1 << L"Release build";
  #endif
  Line1 << L" of KernelSDK " << kKernelSDKVersion;
  QMSG((0, L"%s", Line1.str().c_str()));

  QMSG((0, L""));

  StringStream Line2;
  Line2 << L"Compiled on " << widen(__DATE__) << L" at " << widen(__TIME__)<< L" (BST)";
  QMSG((0, L"%s", Line2.str().c_str()));

  StringStream Line3;
  Line3 << L"Execution started on " << m_DateStarted.serialise() << L" at " << m_TimeStarted.serialise() << L" (local time)";
  QMSG((0, L"%s", Line3.str().c_str()));

  if (m_CmdLine != L"")
  {
    QMSG((0, L""));
    QMSG((0, L"Command line : \"%s\"", m_CmdLine.c_str()));
  }
}
//}}}
//{{{
// =======================================================================================
// === getCmdLineOption ==================================================================
// =======================================================================================

String QAPI cSystemInfo::getCmdLineOption(const String & Option) const throw(cNotFound)
{
  String            Result;
  bool              IsFound = false;
  const string NarrowOption(narrow(Option));

  const unsigned int NumCmdLineChars = (unsigned int)m_LowerCmdLine.size();
  if (NumCmdLineChars > 0)
  {
    string NarrowCmdLine(narrow(m_LowerCmdLine) + " "); // Cmd line is already lower case.

    // Establish string and get the first token.
    char * Token = strtok((char *)NarrowCmdLine.c_str(), "/");
    while(Token != NULL)
    {
      if (strstr(Token, NarrowOption.c_str()) == Token)
      {
        char * Args = Token + NarrowOption.size();
        if (*Args == '\n' || *Args == ' ' || *Args == '=')
        {
          if (*Args == ' ' || *Args == '=') Args++;
          char * FirstQuote = strchr(Args, '\"');
          Result = trim(widen(Args));
          if (FirstQuote)
          {
            char * SecondQuote = strchr(FirstQuote + 1, '\"');
            if (SecondQuote)
            {
              *SecondQuote = '\0';
              Result = trim(widen(FirstQuote + 1));
              Result = m_CmdLine.substr((FirstQuote + 1) - NarrowCmdLine.c_str(), Result.size());
            }
          }
          IsFound = true;
          break;
        }
      }

      // Get next token.
      Token = strtok(NULL, "/");
    }
  }

  if (!IsFound) throw cNotFound(__FILE__, __LINE__, L"Cmd line option \"%s\" not found in \"%s\"", Option.c_str(), m_CmdLine.c_str());

  return Result;
}
//}}}
//{{{
// =======================================================================================
// === getNumProcessors/getProcessorHz ===================================================
// =======================================================================================

unsigned int QAPI cSystemInfo::getNumProcessors() const
{
  return m_Win32SystemInfo.dwNumberOfProcessors;
}

// ---------------------------------------------------------------------------------------

unsigned int QAPI cSystemInfo::getProcessorHz() const
{
  return m_CpuHz;
}
//}}}

//{{{
// =======================================================================================
// === getCurrentUniversalTime ===========================================================
// =======================================================================================
/*
since midnight (00:00:00), January 1, 1970
*/
cTime QAPI cSystemInfo::getCurrentUniversalTime() const
{
  // get time with milliseconds
  struct _timeb timebuffer;
  _ftime (&timebuffer);

  struct tm * universalTime = gmtime(&timebuffer.time);
  if (universalTime == NULL)
  {
    QFATAL((__FILE__, __LINE__, L"Tried to gmtime() - returned %d", GetLastError()));
  }

  return cTime(
    universalTime->tm_hour,
    universalTime->tm_min,
    universalTime->tm_sec,
    timebuffer.millitm);
}
//}}}
//{{{
// =======================================================================================
// === getCurrentUniversalDate ===========================================================
// =======================================================================================
/*
since midnight (00:00:00), January 1, 1970
*/
cDate QAPI cSystemInfo::getCurrentUniversalDate() const
{
  time_t Now;
  time(&Now);

  struct tm * UniversalTime = gmtime(&Now);
  if (UniversalTime == NULL)
  {
    QFATAL((__FILE__, __LINE__, L"Tried to gmtime() - returned %d", GetLastError()));
  }

  return cDate(
    UniversalTime->tm_year + 1900,
    UniversalTime->tm_mon + 1,
    UniversalTime->tm_mday);
}
//}}}
//{{{
// =======================================================================================
// === getCurrentLocalTime ===============================================================
// =======================================================================================

cTime QAPI cSystemInfo::getCurrentLocalTime() const
{
  __int64 timeStarted = m_TimeStarted.getDurationInMillisecs();
  __int64 milliSecsSinceStart = getMillisecsSinceStart();
  __int64 timeInMillisecs (timeStarted+milliSecsSinceStart);
  timeInMillisecs %= 24 * 1000 * 60 * 60; //in case we stay up past midnight!
  cTime Result;
  Result.setDurationFromMillisecs(timeInMillisecs);
  return Result;
  /*
    I have refactored this from below as the localtime call, while puporting to return
    milliseconds, actually only has a resolution of 1/6 or 1/7 of a second and so the
    millisecs value changes in large jumps.
  */


  /*
  // get time with milliseconds
  struct _timeb timebuffer;
  _ftime (&timebuffer);

  struct tm * localTime = localtime(&timebuffer.time);
  if (localTime == NULL)
  {
    QFATAL((__FILE__, __LINE__, L"Tried to localtime() - returned %d", GetLastError()));
  }

  return cTime(localTime->tm_hour, localTime->tm_min, localTime->tm_sec, timebuffer.millitm);
  */

}
//}}}
//{{{
// =======================================================================================
// === getCurrentLocalDate ===============================================================
// =======================================================================================

cDate QAPI cSystemInfo::getCurrentLocalDate() const
{
  time_t Now;
  time(&Now);

  struct tm * LocalTime = localtime(&Now);
  if (LocalTime == NULL)
  {
    QFATAL((__FILE__, __LINE__, L"Tried to localtime() - returned %d", GetLastError()));
  }

  return cDate(LocalTime->tm_year + 1900, LocalTime->tm_mon + 1, LocalTime->tm_mday);
}
//}}}

//{{{
// =======================================================================================
// === _examineSid =======================================================================
// =======================================================================================

bool cSystemInfo::isUserAnAdministrator() const
{
  if (!m_HasTestedIfUserIsAnAdministrator)
  {
    bool IsSpoofingAdmin = false;
    try { (void)getCmdLineOption(L"spoofadmin"); IsSpoofingAdmin = true; } catch (cRecoverable &) {}
    if (IsSpoofingAdmin)
    {
      m_IsUserAnAdministrator = true;
    }
    else
    {
      _examineSid(&m_IsUserAnAdministrator);
    }
    m_HasTestedIfUserIsAnAdministrator = true;
  }
  return m_IsUserAnAdministrator;
}
//}}}
//{{{
// =======================================================================================
// === _examineSid =======================================================================
// =======================================================================================

#define BUFF_SIZE   1024
#define MAX_NAME    256
void cSystemInfo::_examineSid(bool * const IsAdmin) const
{
  *IsAdmin = false;

  try
  {
    HANDLE          hToken = NULL;
    PSID            pAdminSid = NULL;
    SID_IDENTIFIER_AUTHORITY siaNtAuth = SECURITY_NT_AUTHORITY;

    // get token handle
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken) == 0)
    {
      const DWORD LastError = GetLastError();
      throw cWin32Error(__FILE__, __LINE__, L"Tried to OpenProcessToken() - returned %d (%s)", LastError, win32ErrorToString(LastError).c_str());
    }

    cMemory::Ptr buffer = new cMemory((size_t)2048, cMemory::eHeap);
    DWORD bufferSizeRequired; // extact buffer size required
    if (GetTokenInformation(hToken, TokenGroups, (LPVOID)buffer->getBytes(), (DWORD)buffer->getSize(), &bufferSizeRequired) == 0)
    {
      // Perhaps buffer was too small - try again
      buffer = new cMemory((size_t)bufferSizeRequired, cMemory::eHeap);
      if (GetTokenInformation(hToken, TokenGroups, (LPVOID)buffer->getBytes(), (DWORD)buffer->getSize(), &bufferSizeRequired) == 0)
      {
        const DWORD LastError = GetLastError();
        throw cWin32Error(__FILE__, __LINE__, L"Tried to GetTokenInformation() - returned %d (%s)", LastError, win32ErrorToString(LastError).c_str());
      }
    }
    CloseHandle(hToken);
    if (AllocateAndInitializeSid(&siaNtAuth, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0,0,0,0,0,0, &pAdminSid) == 0)
    {
      const DWORD LastError = GetLastError();
      throw cWin32Error(__FILE__, __LINE__, L"Tried to AllocateAndInitializeSid() - returned %d (%s)", LastError, win32ErrorToString(LastError).c_str());
    }

    {
      cLogIndentGuard Indent(0, L"Current user is a member of these groups");
      PTOKEN_GROUPS pGroups = (PTOKEN_GROUPS)buffer->getBytes();
      for (unsigned int i = 0; i < pGroups->GroupCount; ++i)
      {
        if (EqualSid(pAdminSid, pGroups->Groups[i].Sid) && (pGroups->Groups[i].Attributes & SE_GROUP_ENABLED))
        {
          *IsAdmin = true;
        }

        // Lookup the account name and print it.
        DWORD NameSize = MAX_NAME;
        Char DomainName[MAX_NAME] = L"";
        Char GroupName[MAX_NAME] = L"";
        SID_NAME_USE SidType;
        if (LookupAccountSid( NULL, pGroups->Groups[i].Sid, GroupName, &NameSize, DomainName, &NameSize, &SidType ) == 0)
        {
          const DWORD LastError = GetLastError();
          wcscpy(GroupName, win32ErrorToString(LastError).c_str());
/*
          if (LastError == ERROR_NONE_MAPPED)
          {
            wcscpy(GroupName, L"NONE_MAPPED");
          }
          else
          {
            throw cWin32Error(__FILE__, __LINE__, L"Tried to LookupAccountSid() - returned %d (%s)", LastError, win32ErrorToString(LastError).c_str());
          }
*/
        }

        String GroupAttr = L"SID is not enabled";
        if (pGroups->Groups[i].Attributes & SE_GROUP_ENABLED)                 GroupAttr = L"SID enabled";
        else if (pGroups->Groups[i].Attributes & SE_GROUP_USE_FOR_DENY_ONLY)  GroupAttr = L"SID is a deny-only";

        QMSG((0, L"\"%s\\%s\" (%s)", DomainName, GroupName, GroupAttr.c_str()));
      }
    }
    (void)FreeSid(pAdminSid);
  }
  catch (cWin32Error & Reason)
  {
    QRECOVERABLE2FATAL(Reason);
  }
}
/*
bool cSystemInfo::isUserAnAdministrator() const
{
  // No need because we're not using anything that changes
  // cLockGuard MemberGuard(&m_ThreadSafety);

  SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
  PSID AdministratorsGroup;

  int IsAdministrator = 0;
  if (AllocateAndInitializeSid(
    &NtAuthority,
    2,
    SECURITY_BUILTIN_DOMAIN_RID,
    DOMAIN_ALIAS_RID_ADMINS,
    0, 0, 0, 0, 0, 0,
    &AdministratorsGroup))
  {
    if (CheckTokenMembership(NULL, AdministratorsGroup, &IsAdministrator))
    {
      IsAdministrator = 1;
    }
    FreeSid(AdministratorsGroup);
  }
  return IsAdministrator != 0;
}
*/
//}}}
//{{{
// =======================================================================================
// === getHostRegistryPath ================================================================
// =======================================================================================

String cSystemInfo::getHostRegistryPath() const
{
  StringStream Path;
  Path <<
    getRegistryPathToModules() <<
    m_ProductDeveloperName << L"\\" <<
    L"_" << m_HostName << L"\\" <<
    m_HostVersion.m_Release << L"." << m_HostVersion.m_MajorVersion << L"." << m_HostVersion.m_MinorVersion << L".x\\";
  return Path.str();
}
//}}}
//{{{
// =======================================================================================
// === getKernelExecRegistryPath =========================================================
// =======================================================================================

String cSystemInfo::getKernelExecRegistryPath() const
{
  StringStream Path;
  Path <<
    getRegistryPathToModules() <<
    L"vfs" << L"\\" <<
    L"_KernelExec" << L"\\" <<
    cVersion(kVersion).m_Release << L"." << cVersion(kVersion).m_MajorVersion << L"." << cVersion(kVersion).m_MinorVersion << L".x\\";
  return Path.str();
}
//}}}
//{{{
// =======================================================================================
// === getKernelExecModuleContext/getAppModuleContext ====================================
// =======================================================================================

iModuleInstance::Ptr cSystemInfo::getHostModuleContext() const
{
  return m_HostModuleContext;
}
//}}}
//{{{
// =======================================================================================
// === getPhysicalMemoryInMegaBytes ===================================================
// =======================================================================================

String QAPI cSystemInfo::getPhysicalMemoryInMegaBytes() const
{
  MEMORYSTATUSEX stat;
  stat.dwLength = sizeof(MEMORYSTATUSEX);
  GlobalMemoryStatusEx(&stat);

  StringStream Memory;
  Memory << (size_t)(stat.ullTotalPhys / (1024 * 1024)) << " MB";
  return Memory.str();
}
//}}}
//{{{
// =======================================================================================
// === getOperatingSystem ===================================================
// =======================================================================================

String QAPI cSystemInfo::getOperatingSystem() const
{
  OSVERSIONINFOEX osvi;
  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
  if (GetVersionEx((LPOSVERSIONINFO)&osvi) == 0)
  {
    const DWORD LastError = GetLastError();
    throw cWin32Error(__FILE__, __LINE__, L"Tried to GetVersionEx() - returned %d (%s)", LastError, win32ErrorToString(LastError).c_str());
  }
  
  StringStream OSName;
  switch (osvi.dwMajorVersion)
  {
    case 5:
      {
        switch (osvi.dwMinorVersion)
        {
        case 0 :  OSName << L"Windows 2000 " << osvi.szCSDVersion; break;
        case 1 :  OSName << L"Windows XP " << osvi.szCSDVersion; break;
        case 2 :  OSName << L"Windows Server 2003 family " << osvi.szCSDVersion; break;
        default : OSName << L"Unrecognised Windows Version (" << osvi.dwMajorVersion << L"." << osvi.dwMinorVersion << L") " << osvi.szCSDVersion; break;
        }
        break;
      }
    case 6:
      {
        switch (osvi.dwMinorVersion)
        {
        case 0:
          if(VER_NT_WORKSTATION == osvi.wProductType) OSName << L"Windows Vista " << osvi.szCSDVersion;
          else                                        OSName << L"Windows Server 2008 " << osvi.szCSDVersion;
          break;

        case 1 :
          if(VER_NT_WORKSTATION == osvi.wProductType) OSName << L"Windows 7 " << osvi.szCSDVersion;
          else                                        OSName << L"Windows Server 2008 R2 " << osvi.szCSDVersion;
          break;

        case 2 :
          if(VER_NT_WORKSTATION == osvi.wProductType) OSName << L"Windows 8 " << osvi.szCSDVersion;
          else                                        OSName << L"Windows Server 2012 " << osvi.szCSDVersion;
          break;

        case 3 :
          if(VER_NT_WORKSTATION == osvi.wProductType) OSName << L"Windows 8.1 " << osvi.szCSDVersion;
          else                                        OSName << L"Windows Server 2012 R2 " << osvi.szCSDVersion;
          break;

        default : OSName << L"Unrecognised Windows Version (" << osvi.dwMajorVersion << L"." << osvi.dwMinorVersion << L") " << osvi.szCSDVersion; break;
        }

        // See if we can get the product info - GetProductInfo() only works Vista or later so check its available
        DWORD dwType;
        PGPI pGPI = (PGPI)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetProductInfo");

        if(pGPI)
        {
          pGPI(osvi.dwMajorVersion, osvi.dwMinorVersion, 0, 0, &dwType);

          if(wcslen(osvi.szCSDVersion)) OSName << L" ";

          switch(dwType)
          {
          case PRODUCT_ULTIMATE:                     OSName << L"Ultimate Edition"; break;
          case PRODUCT_PROFESSIONAL:                 OSName << L"Professional Edition"; break;
          case PRODUCT_HOME_PREMIUM:                 OSName << L"Home Premium Edition"; break;
          case PRODUCT_HOME_BASIC:                   OSName << L"Home Basic Edition"; break;
          case PRODUCT_ENTERPRISE:                   OSName << L"Enterprise Edition"; break;
          case PRODUCT_BUSINESS:                     OSName << L"Business Edition"; break;
          case PRODUCT_STARTER:                      OSName << L"Starter Edition"; break;
          case PRODUCT_CLUSTER_SERVER:               OSName << L"Cluster Server Edition"; break;
          case PRODUCT_DATACENTER_SERVER:            OSName << L"Datacenter Edition"; break;
          case PRODUCT_DATACENTER_SERVER_CORE:       OSName << L"Datacenter Edition (core installation)"; break;
          case PRODUCT_ENTERPRISE_SERVER:            OSName << L"Enterprise Edition"; break;
          case PRODUCT_ENTERPRISE_SERVER_CORE:       OSName << L"Enterprise Edition (core installation)"; break;
          case PRODUCT_ENTERPRISE_SERVER_IA64:       OSName << L"Enterprise Edition (for Itanium-based Systems)"; break;
          case PRODUCT_SMALLBUSINESS_SERVER:         OSName << L"Small Business Server Edition"; break;
          case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM: OSName << L"Small Business Server Premium Edition"; break;
          case PRODUCT_STANDARD_SERVER:              OSName << L"Standard Edition"; break;
          case PRODUCT_STANDARD_SERVER_CORE:         OSName << L"Standard Edition (core installation)"; break;
          case PRODUCT_WEB_SERVER:                   OSName << L"Web Server Edition"; break;
          case PRODUCT_UNLICENSED:                   OSName << L"Unlicensed"; break;
          default:                                   OSName << L"Unrecognised Edition (" << dwType << L")"; break;
          }
        }
        break;
      }
    case 10:
      {
        switch (osvi.dwMinorVersion)
        {
        case 0:
          if(VER_NT_WORKSTATION == osvi.wProductType) OSName << L"Windows 10 " << osvi.szCSDVersion;
          else                                        OSName << L"Windows Server 2016 " << osvi.szCSDVersion;
          break;
        default : OSName << L"Unrecognised Windows Version (" << osvi.dwMajorVersion << L"." << osvi.dwMinorVersion << L") " << osvi.szCSDVersion; break;
        }
        break;
      }
    default : OSName << L"Unrecognised Windows Version (" << osvi.dwMajorVersion << L"." << osvi.dwMinorVersion << L") " << osvi.szCSDVersion; break;
  }

  return OSName.str();
}
//}}}
//{{{
// =======================================================================================
// === getProcessorDescription ===================================================
// =======================================================================================

String QAPI cSystemInfo::getProcessorDescription() const
{
  StringStream CpuDescription;
  try
  {
    const String strCPUDescFromRegistry = createRegistryHelp()->getItemString(L"Hardware\\Description\\System\\CentralProcessor\\0", L"ProcessorNameString");
    CpuDescription << (size_t)(getProcessorHz() / 1000) << L" MHz (" << trim(strCPUDescFromRegistry) << L")";
  }
  catch (cNotFound &)
  {
    CpuDescription << (size_t)(getProcessorHz() / 1000) << L" MHz";
  }  

  return CpuDescription.str();
}
//}}}
//{{{
// =======================================================================================
// === getSystemArchitecture ===================================================
// =======================================================================================

String QAPI cSystemInfo::getSystemArchitecture() const
{
  // Determine OS Type - use GetNativeSystemInfo() as this works if our application is running under WOW64 mode, if
  // called from a 64-bit application, it is equivalent to GetSystemInfo()
  
  StringStream OSType;
  SYSTEM_INFO info;
  PGNSI pGNSI = (PGNSI)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetNativeSystemInfo");
  if(pGNSI) pGNSI(&info);
  else GetSystemInfo(&info);

  switch(info.wProcessorArchitecture)
  {
  case PROCESSOR_ARCHITECTURE_INTEL:
    OSType << "32-bit";
    break;
  case PROCESSOR_ARCHITECTURE_AMD64:
  case PROCESSOR_ARCHITECTURE_IA64:
    OSType << "64-bit";
    break;
  default:
    OSType << "Unknown";
    break;
  }

  return OSType.str();
}
//}}}
//{{{
// =======================================================================================
// === getApplicationArchitecture ===================================================
// =======================================================================================

String QAPI cSystemInfo::getApplicationArchitecture() const
{
#ifdef _WIN64
  return L"64-bit";
#elif _WIN32
  return L"32-bit";
#endif 
}
//}}}