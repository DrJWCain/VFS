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

#include "cDLL.h"

namespace vfs
{

  class cDLLManager : public iDLLManager, public cRefCount
  {
  friend class cHttpCmd_ModulesToBlock;
  friend class cHttpCmd_ModulesLoaded;
  friend class cHttpCmd_ClassesToBlock;

  public:
    QDEFINE_SINGLETON_IMPL(cDLLManager, L"{45239BF5-459D-40EA-BE9E-4518F785FE9C} // cDLLManager", iDLLManager);

    cDLLManager();
    ~cDLLManager();

    // iManager
    void QAPI logSummary();
    void QAPI logDetailedSummary();

    // iDLLManager
    void QAPI installDLLs(const unsigned long MinGroup, const unsigned long MaxGroup);
    void QAPI uninstallDLLs(const bool IsRemovingSettingsAndData);
    unsigned int QAPI countDLLs(const unsigned long MinGroup, const unsigned long MaxGroup);
    void QAPI initDLLs(const unsigned long MinGroup, const unsigned long MaxGroup);
    std::vector<_ATL_OBJMAP_ENTRY> QAPI getATLObjects() throw(cNotAvailable);
    std::multimap<HINSTANCE, unsigned int> getATLTypeLibraries() throw(cNotAvailable);
    iModuleInstance::Ptr QAPI getInstance(const String & DeveloperName, const String & ModuleName) throw(cNotFound);
    void QAPI registerDLLs();
    bool QAPI isInstallDllsNecessary();
    void QAPI logChangedSettings();
    bool QAPI hasPatchedDLLs();

    // cDLLManager
    void updateSharing(const String & DeveloperName, const String & ModuleName, const cVersion & ModuleVersion, const bool IsAdding);
    HINSTANCE loadRawDLL(
      const String & filename,
      const bool isResolvingSymbols) /* throw cWin32Error */;
    void registerBuiltInDLL(
      const String & CodeFilename, // without extension
      const String & RegFilename,
      const String & DataPath,
      const String & DeveloperName,
      const String & ModuleName,
      const cVersion & ModuleVersion);
    void installDLLsNoFileScan(const unsigned long MinGroup, const unsigned long MaxGroup) throw(cRecoverable);
    void initDLLsNoFileScan(const unsigned long MinGroup, const unsigned long MaxGroup);
    String getDLLSearchPath() const;

  private:
    String m_DLLSearchPath;

    iRegistryHelp::Ptr m_Rh;
    iRegistryHelp::Ptr m_Urh;

    cLock m_DLLsThreadSafety;
    bool m_HasScannedDLLs;
    std::multiset<cDLL> m_DLLs;
    bool m_HasPatchedDLLs;

    std::vector<cGuard::ConstPtr> m_HttpCmdGuards;

    cLock m_RegistrarsListThreadSafety;
    bool m_HasBuiltRegistrarsList;
    std::vector<cModuleRegistrar::Ptr> m_Registrars;

    void _installDLL (cDLL& aboutDLL) throw(cRecoverable);
    void _uninstallDLL (cDLL& aboutDLL, const bool isRemovingSettingsAndData) throw(cRecoverable);
    std::vector<cModuleRegistrar::Ptr>& _getRegistrars() throw(cNotAvailable);
    void _scanDLLs();
    void _logDLLs (const bool isOnlyShowingProblemDlls);
    void _addSharing (const String& developerName, const String& moduleName, const cVersion& version) throw(cRecoverable);
    unsigned int _removeSharing (bool currentUser, const String& developerName, const String& moduleName, const cVersion& version) throw(cRecoverable);
      //!< Returns new share count.
  };

}
