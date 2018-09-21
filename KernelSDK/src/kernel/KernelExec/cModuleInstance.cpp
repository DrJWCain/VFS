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
#include ".\notifyregvaluechange.h"
#include "cModuleInstance.h"
#include "cSystemInfo.h"

using namespace vfs;

static cNotifyRegValueChange s_notifyRegValueChange;

void QAPI cRegValObsGuardCreator::onGuardDestroyed(iGuarded::Ptr guarded)
{
  s_notifyRegValueChange.removeKeyValueChangeObserver(m_regId, ptr_cast<iRegEventObserver::Ptr>(guarded));
}


// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

cModuleInstance::cModuleInstance(
  const HINSTANCE   hDllInstance,
  const String &    DataPath,
  const String &    ModuleDeveloperName,
  const String &    ModuleName,
  const cVersion &  ModuleVersion,
  iRegistryHelp::eRegistryView RegistryView) :
  m_hDllInstance(hDllInstance),
  m_DataPath(DataPath),
  m_ModuleName(ModuleName),
  m_DeveloperName(ModuleDeveloperName),
  m_ModuleVersion(ModuleVersion),
  m_RegistryView(RegistryView)
{
  _init();
}

// ---------------------------------------------------------------------------------------

cModuleInstance::cModuleInstance(const String & ModuleDeveloperName, const String & ModuleName, const cVersion & ModuleVersion) :
  m_hDllInstance(NULL),
  m_ModuleName(ModuleName),
  m_DeveloperName(ModuleDeveloperName),
  m_ModuleVersion(ModuleVersion),
  m_RegistryView(iRegistryHelp::eRegistryView_Default)
{
  _init();
}

// ---------------------------------------------------------------------------------------

void cModuleInstance::_init()
{
  StringStream tmp;
  tmp <<
    cSystemInfo::singleton().getRegistryPathToModules() <<
    m_DeveloperName << L"\\" <<
    m_ModuleName << L"\\" <<
    m_ModuleVersion.m_Release << L"." << m_ModuleVersion.m_MajorVersion << L"." << m_ModuleVersion.m_MinorVersion << L".x\\";
  m_SettingsPath = tmp.str();

  m_CustomSettingsPath = tmp.str() + k_CustomSettingsStr + L"\\";
  m_PublicSettingsPath = tmp.str() + k_PublicSettingsStr + L"\\";
  m_UserSettingsPath = tmp.str() + k_UserSettingsStr + L"\\";

  m_Rh = createRegistryHelp(m_RegistryView);
  m_Urh = createCurrentUserRegistryHelp(m_RegistryView);
}


cGuard::ConstPtr QAPI cModuleInstance::addKeyValueChangeObserver(const sRegEventId& regId, const iRegEventObserver::Ptr & observer)
{
  if (s_notifyRegValueChange.addKeyValueChangeObserver(regId, observer))
  {
    const iGuardCreator::Ptr creatorPtr(new cRegValObsGuardCreator(regId));
    return cGuard::ConstPtr(new cGuard(creatorPtr, observer));
  }
  else
  {
    return cGuard::ConstPtr((cGuard*)NULL);
  }
}

// =======================================================================================
// === getUserFolder =====================================================================
// =======================================================================================

cVersion QAPI cModuleInstance::getVersion() const
{
  // Do not protect member variables from changes by another thread because we're not using
  // any members that change!
  // cLockGuard ThreadSafety(&m_ThreadSafety);

  return m_ModuleVersion;
}




// =======================================================================================
// === getUserFolder =====================================================================
// =======================================================================================

String QAPI cModuleInstance::getUserFolder() const throw(cNotAvailable)
{
  return ivfsEnvironment::singleton().getUserFolder();
};




// =======================================================================================
// === getDataFolder =====================================================================
// =======================================================================================

String QAPI cModuleInstance::getDataFolder() const throw(cNotAvailable)
{
  // Do not protect member variables from changes by another thread because we're not using
  // any members that change!
  // cLockGuard ThreadSafety(&m_ThreadSafety);

  return m_DataPath;
}




// =======================================================================================
// === getCustomSettingValueType =========================================================
// =======================================================================================

iModuleInstance::eSettingValueType QAPI cModuleInstance::getCustomSettingValueType(
  const String & Path,
  const String & Name) const throw(cNotFound)
{
  // Do not protect member variables from changes by another thread because we're not using
  // any members that change!
  // cLockGuard ThreadSafety(&m_ThreadSafety);

  return static_cast<iModuleInstance::eSettingValueType>(m_Rh->getItemType(m_CustomSettingsPath + Path, Name));
}




// =======================================================================================
// === getCustomSettingNumber/getCustomSettingString =====================================
// =======================================================================================

unsigned long QAPI cModuleInstance::getCustomSettingNumber(const String & Path, const String & ItemName) const throw(cNotFound, cBadFormat)
{
  // Do not protect member variables from changes by another thread because we're not using
  // any members that change!
  // cLockGuard ThreadSafety(&m_ThreadSafety);

  return m_Rh->getItemNumber(m_CustomSettingsPath + Path, ItemName);
}

// ---------------------------------------------------------------------------------------

String QAPI cModuleInstance::getCustomSettingString(const String & Path, const String & ItemName) const throw(cNotFound, cBadFormat)
{
  // Do not protect member variables from changes by another thread because we're not using
  // any members that change!
  // cLockGuard ThreadSafety(&m_ThreadSafety);

  return m_Rh->getItemString(m_CustomSettingsPath + Path, ItemName);
}

// ---------------------------------------------------------------------------------------

void QAPI cModuleInstance::setCustomSettingNumber(const String & Path, const String & ItemName, const unsigned long NewNumber) throw(cNotFound, cBadFormat)
{
  // Do not protect member variables from changes by another thread because we're not using
  // any members that change!
  // cLockGuard ThreadSafety(&m_ThreadSafety);

  m_Rh->setItemNumber(m_CustomSettingsPath + Path, ItemName, NewNumber);
  if (isUserCustomSetting(Path, ItemName))
    m_Urh->setItemNumber(m_CustomSettingsPath + Path, ItemName, NewNumber);
}

// ---------------------------------------------------------------------------------------

void QAPI cModuleInstance::setCustomSettingString(const String & Path, const String & ItemName, const String & NewString) throw(cNotFound, cBadFormat)
{
  // Do not protect member variables from changes by another thread because we're not using
  // any members that change!
  // cLockGuard ThreadSafety(&m_ThreadSafety);

  m_Rh->setItemString(m_CustomSettingsPath + Path, ItemName, NewString);
  if (isUserCustomSetting(Path, ItemName))
    m_Urh->setItemString(m_CustomSettingsPath + Path, ItemName, NewString);

}




// =======================================================================================
// === getCustomSettingNumbers/getCustomSettingStrings ===================================
// =======================================================================================

std::map<String, unsigned long> QAPI cModuleInstance::getCustomSettingNumbers(const String & Path) const throw(cNotFound, cBadFormat)
{
  // Do not protect member variables from changes by another thread because we're not using
  // any members that change!
  // cLockGuard ThreadSafety(&m_ThreadSafety);

  return m_Rh->getKeyNumbers(m_CustomSettingsPath + Path);
}

// ---------------------------------------------------------------------------------------

std::map<String, String> QAPI cModuleInstance::getCustomSettingStrings(const String & Path) const throw(cNotFound, cBadFormat)
{
  // Do not protect member variables from changes by another thread because we're not using
  // any members that change!
  // cLockGuard ThreadSafety(&m_ThreadSafety);

  return m_Rh->getKeyStrings(m_CustomSettingsPath + Path);
}




// =======================================================================================
// === getCustomSettingSubPaths ==========================================================
// =======================================================================================

std::list<String> QAPI cModuleInstance::getCustomSettingSubPaths(const String & Path) const throw(cNotFound)
{
  // Do not protect member variables from changes by another thread because we're not using
  // any members that change!
  // cLockGuard ThreadSafety(&m_ThreadSafety);

  return m_Rh->getSubKeys(m_CustomSettingsPath + Path);
}



bool cModuleInstance::isUserPublicSetting(const String & SettingId) const
{
  bool userSetting(false);
  try {
    userSetting = m_Rh->getItemNumber(m_PublicSettingsPath + SettingId, L"User Setting") != 0;
  }
  catch (cNotFound &) {}
  return userSetting;
}

bool cModuleInstance::isUserCustomSetting(const String & Path, const String & Name) const
{
  bool userSetting(false);
  try {
    userSetting = m_Rh->getItemNumber(m_UserSettingsPath + Path, Name) != 0;
  }
  catch (cNotFound &) {}
  return userSetting;
}


// =======================================================================================
// === setPublicSettingString/getPublicSettingString =====================================
// =======================================================================================

void QAPI cModuleInstance::setPublicSettingString(const String & SettingId, const String & NewString) throw(cNotFound, cBadFormat)
{
  // Do not protect member variables from changes by another thread because we're not using
  // any members that change!
  // cLockGuard ThreadSafety(&m_ThreadSafety);

  const String Type = m_Rh->getItemString(m_PublicSettingsPath + SettingId, L"Type (checkbox, radio, text, number)");
  String name(L"Text Value");
  if (Type == L"select")
    name = L"Select value";

  m_Rh->setItemString(m_PublicSettingsPath + SettingId, name, NewString);
  if (isUserPublicSetting(SettingId))
    m_Urh->setItemString(m_PublicSettingsPath + SettingId, name, NewString);
}

// ---------------------------------------------------------------------------------------

String QAPI cModuleInstance::getPublicSettingString(const String & SettingId) const throw(cNotFound, cBadFormat)
{
  // Do not protect member variables from changes by another thread because we're not using
  // any members that change!
  // cLockGuard ThreadSafety(&m_ThreadSafety);

  const String Type = m_Rh->getItemString(m_PublicSettingsPath + SettingId, L"Type (checkbox, radio, text, number)");
  String name(L"Text Value");
  if (Type == L"select")
    name = L"Select value";

  return m_Rh->getItemString(m_PublicSettingsPath + SettingId, name);

  //iRegistryHelp::Ptr rh(isUserPublicSetting(SettingId) ? m_Urh : m_Rh);
  //return rh->getItemString(m_PublicSettingsPath + SettingId, name);
}

// =======================================================================================
// === getPublicSettingValueType =========================================================
// =======================================================================================

iModuleInstance::eSettingValueType QAPI cModuleInstance::getPublicSettingValueType(const String & SettingId) const throw(cNotFound)
{
  const String Type = m_Rh->getItemString(m_PublicSettingsPath + SettingId, L"Type (checkbox, radio, text, number)");
  if (Type == L"number")
  {
    return eSettingValueType_Number;
  }
  else if (Type == L"checkbox")
  {
    return eSettingValueType_Number;
  }
  else if (Type == L"radio")
  {
    return eSettingValueType_Number;
  }
  else if (Type == L"text")
  {
    return eSettingValueType_String;
  }
  else if (Type == L"select")
  {
    return eSettingValueType_String;
  }
  else
  {
    throw cBadFormat(__FILE__, __LINE__, L"Public setting \"%s\" unexpected type \"%s\"", SettingId.c_str(), Type.c_str());
  }
}




// =======================================================================================
// === enablePublicSettingNumber =========================================================
// =======================================================================================

void QAPI cModuleInstance::enablePublicSetting(const String & SettingId, const bool IsEnabled) throw(cNotFound, cBadFormat)
{
  m_Rh->setItemNumber(m_PublicSettingsPath + SettingId, L"Is enabled", IsEnabled);
}




// =======================================================================================
// === setPublicSettingNumber/getPublicSettingNumber =====================================
// =======================================================================================

void QAPI cModuleInstance::setPublicSettingNumber(const String & SettingId, const int NewValue) throw(cNotFound, cBadFormat)
{
  // Do not protect member variables from changes by another thread because we're not using
  // any members that change!
  // cLockGuard ThreadSafety(&m_ThreadSafety);

  const String Type = m_Rh->getItemString(m_PublicSettingsPath + SettingId, L"Type (checkbox, radio, text, number)");
  String name;
  if (Type == L"number")
    name = L"Number value";
  else if (Type == L"checkbox")
    name = L"Checkbox value (0 = unchecked, 1 = checked)";
  else if (Type == L"radio")
    name = L"Radio value (0 = unselected, 1 = selected)";
  else
  {
    throw cBadFormat(__FILE__, __LINE__, L"Public setting \"%s\" cannot be set as a number", SettingId.c_str());
  }
  m_Rh->setItemNumber(m_PublicSettingsPath + SettingId, name, NewValue);
  if (isUserPublicSetting(SettingId))
    m_Urh->setItemNumber(m_PublicSettingsPath + SettingId, name, NewValue);
}

// ---------------------------------------------------------------------------------------

int QAPI cModuleInstance::getPublicSettingNumber(const String & SettingId) const throw(cNotFound, cBadFormat)
{
  // Do not protect member variables from changes by another thread because we're not using
  // any members that change!
  // cLockGuard ThreadSafety(&m_ThreadSafety);

  const String Type = m_Rh->getItemString(m_PublicSettingsPath + SettingId, L"Type (checkbox, radio, text, number)");
  String name;
  if (Type == L"number")
    name = L"Number value";
  else if (Type == L"checkbox")
    name = L"Checkbox value (0 = unchecked, 1 = checked)";
  else if (Type == L"radio")
    name = L"Radio value (0 = unselected, 1 = selected)";
  else
  {
    throw cBadFormat(__FILE__, __LINE__, L"Public setting \"%s\" cannot be read as a number", SettingId.c_str());
  }
  iRegistryHelp::Ptr rh(isUserPublicSetting(SettingId) ? m_Urh : m_Rh);
  return rh->getItemNumber(m_PublicSettingsPath + SettingId, name);
}