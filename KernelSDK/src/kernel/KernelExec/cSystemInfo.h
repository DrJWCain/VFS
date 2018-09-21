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

#pragma once
/*
**  
*/

#include "QKernelExec.h"
#include "QKernelProducts.h"
#include <QWinBase.h>

namespace vfs
{

  const String  k_CustomSettingsStr = L"Custom Settings";
  const String  k_PublicSettingsStr = L"Public Settings";
  const String  k_UserSettingsStr = L"User Setting Flags";

  // Used for mapping products to their option numbers
  struct tProductNameToId
  {
    Char*      name;
    eProductId id;
  };
  extern const tProductNameToId kProductNameToIdTable[];

  // Used for allowing a new product to access the registry under its old name
  struct tProductNameToOldProductName
  {
    Char* currentName;
    Char* legacyName;
  };
  extern const tProductNameToOldProductName kProductNameToOldProductNameTable[];


// =======================================================================================
// === cSystemInfo =======================================================================
// =======================================================================================

class cSystemInfo : public iSystemInfo, public cRefCount
{
  public :

    QDEFINE_SINGLETON_IMPL(cSystemInfo, L"{68C56A9A-E53C-4308-85CF-3CDC55FBA9A7} // cSystemInfo", iSystemInfo);

    cSystemInfo();
    ~cSystemInfo();

    // iManager
    virtual void QAPI logSummary();
    virtual void QAPI logDetailedSummary();

    // iSystemInfo
    virtual cTime QAPI getCurrentUniversalTime() const;
    virtual cDate QAPI getCurrentUniversalDate() const;
    virtual cTime QAPI getCurrentLocalTime() const;
    virtual cDate QAPI getCurrentLocalDate() const;

    virtual String QAPI getCmdLineOption(const String & Option) const throw(cNotFound);
    virtual unsigned int QAPI getNumProcessors() const;

    virtual UInt64 QAPI getMillisecsSinceStart() const;
    virtual UInt64 QAPI getTicksSinceStart(UInt64 * const TickFreqHz) const;
    virtual cTime QAPI getElapsedSinceStart() const;

    virtual unsigned int QAPI getProcessorHz() const;
    virtual String QAPI getBootFolder() const;
    virtual String QAPI getProductDeveloperName() const;
    virtual String QAPI getProductName() const;
    virtual String QAPI getProductVersion() const;
    virtual String QAPI getProductSettingsGroup() const;
    virtual String QAPI getHostName() const;
    virtual cPtr<iModuleInstance> QAPI getHostModuleContext() const;
    virtual cVersion QAPI getHostVersion() const;
    virtual void QAPI setCurrentUserName(const String& username);
    virtual void QAPI setCurrentUserPassword(const String& userpassword);
    virtual String QAPI getCurrentUserName() const;
    virtual String QAPI getCurrentUserPassword() const;

    virtual double QAPI getSecsSinceStart() const;
    virtual double QAPI convertToSec(UInt64 ticksSinceStart) const;
    virtual cTime QAPI convertToTime(UInt64 ticksSinceStart) const;

    virtual UInt64 QAPI getTicksSinceStart() const;
    virtual UInt64 QAPI getTicksFreq() const;
    virtual UInt64 QAPI convertToMillisec (UInt64 ticks) const;
    virtual UInt64 QAPI convertToMicrosec (UInt64 ticks) const;
    virtual String QAPI toTimeString (UInt64 ticks) const;     // including microsecs

    virtual String QAPI getPhysicalMemoryInMegaBytes() const;
    virtual String QAPI getOperatingSystem() const;
    virtual String QAPI getProcessorDescription() const;
    virtual String QAPI getSystemArchitecture() const;
    virtual String QAPI getApplicationArchitecture() const;

    virtual enum eProductId QAPI getProductId() const throw(cNotFound);
    virtual enum eProductId QAPI parseProductName (const String& strProduct) const throw(cNotFound);

    virtual Char QAPI getProductVersionLetter() const;
    virtual cVersion QAPI getProductVersionNumber() const;
    virtual String QAPI getProductBuildId() const;

    // cSystemInfo
    virtual void init(const String & CmdLine);
    virtual String getKernelExecRegistryPath() const;
    virtual bool isUserAnAdministrator() const;
    virtual cWin32UserImpersonationGuard::ConstPtr elevatePrivileges() throw(cWin32Error);

    virtual void initUsingRegistrySettings();
    virtual void initBuiltInModules();
    virtual String getRegistryPathToModules() const;
    virtual String getHostRegistryPath() const;

  private :

    String                  m_CmdLine;
    String                  m_LowerCmdLine;

    cDate                   m_DateStarted;
    cTime                   m_TimeStarted;               // time of day
    UInt64                  m_TimeStartedInMillisecs;    // time of day in millisecs since midnight

    cVersion                m_Version;

    String                  m_ProductDeveloperName;
    String                  m_ProductName;
    eProductId              m_ProductId;
    cVersion                m_ProductVersion;
    String                  m_ProductVersionAsString;
    Char                    m_ProductVersionLetter; // D, T, S, C, X, V
    String                  m_ProductBuildId;
    String                  m_ProductSettingsGroup;

    String                  m_HostName;
    cVersion                m_HostVersion;
    String                  m_UserName;
    String                  m_UserPassword;

    String                  m_KernelExecFolder;

    UInt64                  m_PerfCounterTickFreqHz;
    UInt64                  m_PerfCounterTicksAtStart;

    SYSTEM_INFO             m_Win32SystemInfo;
    unsigned int            m_CpuHz;

    iModuleInstance::Ptr     m_HostModuleContext;

    mutable bool            m_HasTestedIfUserIsAnAdministrator;
    mutable bool            m_IsUserAnAdministrator;

    cWin32HandleGuard::Ptr  m_LogonHandleGuard;

    void _logSystemSummary() const;
    void _logPackageSummary() const;
    void _logBuildSummary() const;
    cVersion _getVersion() const;
    String _getProcessFilename(const HINSTANCE hInstance) const throw(cWin32Error);
    String _getDLLsFolder(const HINSTANCE hInstance) const throw(cWin32Error);
    String _getProductDetail(const String & DetailId) throw(cNotFound);
    void _examineSid(bool * const IsAdmin) const;
};

};
