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

namespace vfs
{
  //class cModuleInstance;
  class cRegValObsGuardCreator;
}

using namespace std;
using namespace vfs;


// =======================================================================================
// === cModuleInstance ====================================================================
// =======================================================================================

class cModuleInstance : public iModuleInstance, public cRefCount
{
	public :

    QDEFINE_SMARTPTR(cModuleInstance);

    // iModuleInstance
 	  String QAPI getDataFolder() const throw(cNotAvailable);
 	  String QAPI getUserFolder() const throw(cNotAvailable);
    unsigned long QAPI getCustomSettingNumber(const String & Path, const String & Name) const throw(cNotFound, cBadFormat);
    map<String, unsigned long> QAPI getCustomSettingNumbers(const String & Path) const throw(cNotFound, cBadFormat);
    String QAPI getCustomSettingString(const String & Path, const String & Name) const throw(cNotFound, cBadFormat);
    map<String, String> QAPI getCustomSettingStrings(const String & Path) const throw(cNotFound, cBadFormat);
    void QAPI setCustomSettingNumber(const String & Path, const String & ItemName, const unsigned long NewNumber) throw(cNotFound, cBadFormat);
    void QAPI setCustomSettingString(const String & Path, const String & ItemName, const String & NewString) throw(cNotFound, cBadFormat);
    eSettingValueType QAPI getCustomSettingValueType(const String & Path, const String & Name) const throw(cNotFound);
    list<String> QAPI getCustomSettingSubPaths(const String & Path) const throw( cNotFound);
    eSettingValueType QAPI getPublicSettingValueType(const String & SettingId) const throw(cNotFound);
    void QAPI setPublicSettingString(const String & SettingId, const String & NewString) throw(cNotFound, cBadFormat);
    void QAPI setPublicSettingNumber(const String & SettingId, const int NewValue) throw(cNotFound, cBadFormat);
    String QAPI getPublicSettingString(const String & SettingId) const throw(cNotFound, cBadFormat);
    int QAPI getPublicSettingNumber(const String & SettingId) const throw(cNotFound, cBadFormat);
    //cGuard::ConstPtr QAPI addCustomSettingCallback(iCustomSettingCallback::Ptr NewCallback) { throw cNotSupported(__FILE__, __LINE__, L"Not implemented yet!"); }
    //cGuard::ConstPtr QAPI addPublicSettingCallback(iPublicSettingCallback::Ptr NewCallback) { throw cNotSupported(__FILE__, __LINE__, L"Not implemented yet!"); }
    void QAPI enablePublicSetting(const String & SettingId, const bool IsEnabled) throw(cNotFound, cBadFormat);
    cVersion QAPI getVersion() const;
    String QAPI getRegistryPath() const               { return m_SettingsPath; }
    String QAPI getRegistryCustomSettingsPath() const { return m_CustomSettingsPath; }
    String QAPI getRegistryUserSettingsPath() const { return m_CustomSettingsPath; }
    String QAPI getRegistryPublicSettingsPath() const { return m_PublicSettingsPath; }
    String QAPI getModuleName() const                 { return m_ModuleName; }
    String QAPI getDeveloperName() const              { return m_DeveloperName; }
    virtual cGuard::ConstPtr QAPI addKeyValueChangeObserver(const sRegEventId& regId, const iRegEventObserver::Ptr & observer);

    // cModuleInstance
		cModuleInstance(const HINSTANCE hDllInstance, const String & RootPath, const String & ModuleDeveloperName, const String & ModuleName, const cVersion & ModuleVersion, iRegistryHelp::eRegistryView RegistryView);
		// For cSystemInfo use only: used to create 'virtual' modules for KernelExec and the App itself
		cModuleInstance(const String & ModuleDeveloperName, const String & ModuleName, const cVersion & ModuleVersion);

    mutable cLock   m_ThreadSafety;

		const HINSTANCE m_hDllInstance;
		const String    m_DataPath;
		const String	  m_DeveloperName;
		const String    m_ModuleName;
    const cVersion  m_ModuleVersion;

    iRegistryHelp::Ptr m_Rh;
    iRegistryHelp::Ptr m_Urh;

    String          m_SettingsPath;
    String          m_CustomSettingsPath;
    String          m_UserSettingsPath;
    String          m_PublicSettingsPath;

  private :
    bool isUserPublicSetting(const String & SettingId) const;
    bool isUserCustomSetting(const String & Path, const String & Name) const;
    iRegistryHelp::eRegistryView m_RegistryView;
 
    void _init();
};

namespace vfs
{
  class cRegValObsGuardCreator : public iGuardCreator, public cRefCount
  {
  public:
    QDEFINE_SMARTPTR(cRegValObsGuardCreator);
    cRegValObsGuardCreator(const sRegEventId& regId) : m_regId(regId) {}
    virtual ~cRegValObsGuardCreator() {}

    virtual void QAPI onGuardDestroyed(iGuarded::Ptr guarded);// { m_moduleCtx->removeKeyValueChangeObserver(m_regId, guarded); }
  private:
    sRegEventId m_regId;
  };
}

