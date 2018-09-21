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
#include "cModuleInstance.h"
#include "cModuleRegistrar.h"

using namespace vfs;




// =======================================================================================
// === eInstallAction =====================================================================
// =======================================================================================

enum eInstallAction
{
  eInstallAction_Ignore,
  eInstallAction_Nothing,
  eInstallAction_Install
};




// =======================================================================================
// === kBadVer ===========================================================================
// =======================================================================================

const cVersion kBadVer(99, 99 ,99 ,99);




// =======================================================================================
// === cDLL ==============================================================================
// =======================================================================================

class cDLL
{
  public :

    enum eDLLType
    {
      eUnknown,
      eDriver,
      ePlugin,
      eModule // New style
    };

    enum eInitDLLResult
    {
      eFailedRecoverable,
      eOK
    };

    cDLL(
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
      const bool            HasModuleUninstallFunc);
    bool QAPI operator<(const cDLL & Other) const;
    
    void load(const bool isResolvingSymbols) /* throw cWin32Error */;
    void unload();
    void init() throw(cRecoverable);
    bool isLoaded() const { return m_IsLoaded; }
    bool isInitialised() const { return m_IsInitialised; }
    cModuleInstance::Ptr getInstance() const { return m_Instance; }
    cModuleRegistrar::Ptr getRegistrar() const { const_cast<cDLL *>(this)->_register(); return m_Registrar; }
    void pushPaths();
    void popPaths();
    void checkCurrentUserInstallState();
    void migrateLMSettings();
    void migrateCUSettings();
    void copyUserSettings();
    void cullSettingsVersions();
    bool isHostedByKernelSetup() const { return (m_Flags & fModuleInstall_IsHostedByKernelSetup) != 0; }
    bool isLMUpgrading() const { return (m_Flags & fModuleInstall_Upgrading) != 0; }
    bool isLMDowngrading() const { return (m_Flags & fModuleInstall_Downgrading) != 0; }
    bool isRelocating() const { return (m_Flags & fModuleInstall_Relocating) != 0; }
    bool isLMMigrating() const { return (m_Flags & fModuleInstall_Migrating) != 0; }
    bool isCUMigrating() const { return (m_Flags & fModuleInstall_User_Migrating) != 0; }
    bool isLMFirstTimeInstall() const { return (m_Flags & fModuleInstall_FirstTimeInstall) != 0; }
    int getFlags() const { return m_Flags; }
    void restoreSettings(bool userSettings);
    void logChangedSettings() const;

    bool              m_HasProblem;
    const bool        m_HasModuleRegisterFunc;
    const bool        m_HasModuleInstallFunc;
    const bool        m_HasModuleUninstallFunc;
    const String      m_CodeFilename;
    String            m_RegFilename;
    const String      m_DataPath;
    const String      m_DeveloperName;
    const String      m_ModuleName;
    const int         m_LoadOrderGroup;
    String            m_Status;
    cVersion          m_InternalVersion;
    String            m_InternalVersionFormattedAsRegistryKey;
    const cVersion    m_BuiltWithSDKVersion;
    cDate             m_ModificationDate;
    cTime             m_ModificationTime;
    eInstallAction    m_LMInstallAction;
    eInstallAction    m_CUInstallAction;
    String            m_RegPath;
    String            m_CurrentVersionRegPath;
    HINSTANCE         m_hInstance;
    bool              mHasCurrentUserRegistry;
    iRegistryHelp::eRegistryView  m_PreviousVersionRegistry;
    String            m_PreviousVersion;
    String            m_PreviousUserVersion;
    String            m_PreviousLocation;
    String            m_PreviousUserLocation;

    String            m_PatchedDLLLocation; // empty unless DLL is patched, then it contains the original DLL location

  private :

    int               m_Flags;

    bool              m_IsLoaded;
    bool              m_IsInitialised;
    bool              m_IsRegistered;

    String            m_OrigPathEnvVar;
    String            m_OrigFolder;
    int               m_HasSetPathEnvVar;
    int               m_HasSetFolder;

    cModuleInstance::Ptr m_Instance;
    cModuleRegistrar::Ptr m_Registrar;

    void _register() throw(cRecoverable);

    void findPreviousVersion(iRegistryHelp::eRegistryView registryView);
    void migrateSettings(iRegistryHelp::Ptr srcRH, iRegistryHelp::Ptr destRH, const String & previousVersion, iRegistryHelp::eRegistryView  m_PreviousVersionRegistry);
    bool hasUserSettings();
    void copyUserSettings(iRegistryHelp::Ptr srcRH, iRegistryHelp::Ptr destRH);
};

class cRegistryMigrator : public iRegistryHelp::iItemIterator, public cRefCount
{
  public :
    QDEFINE_SMARTPTR(cRegistryMigrator);
    cRegistryMigrator(const String & OldVersionRegistryPath, iRegistryHelp::Ptr srcRH);

    bool /* go into */ QAPI onSubKey(const String & WholeKeyPath, const list<String> & SubKeyStack, const iRegistryHelp::Ptr & destRH);
    void QAPI onItem(const String & KeyPath, const String & ItemName, const list<String> & SubKeyStack, const iRegistryHelp::Ptr & destRH);

private :
    const String        m_OldVersionRegistryPath;
    String              m_SubKeyPath;
    bool                mIsInPublicSettings;
    bool                mIsInUserSettings;
    bool                mIsInLogCategories;
    iRegistryHelp::Ptr  m_SrcRH;

    bool _isMigratable(const String & ItemName);
};

class cCurrentUserBuilder : public iRegistryHelp::iItemIterator, public cRefCount
{
public:
  QDEFINE_SMARTPTR(cCurrentUserBuilder);
  cCurrentUserBuilder(const String & OldVersionRegistryPath, iRegistryHelp::Ptr srcRH, bool testOnly);

  bool /* go into */ QAPI onSubKey(const String & WholeKeyPath, const list<String> & SubKeyStack, const iRegistryHelp::Ptr & destRH);
  void QAPI onItem(const String & KeyPath, const String & ItemName, const list<String> & SubKeyStack, const iRegistryHelp::Ptr & destRH);
  bool hasUserSettings() { return m_HasCurrentUserSettings; }


private:
  const String        m_OldVersionRegistryPath;
  String              m_SubKeyPath;
  bool                m_IsInPublicSettings;
  bool                m_IsPublicUserSettings;
  bool                m_IsInCustomSettings;
  bool                m_IsInUserSettings;
  bool                m_HasCurrentUserSettings;
  bool                m_TestOnly;
  iRegistryHelp::Ptr  m_DestRH;

  bool _isUserSetting(const String & ItemName);
};
