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
#include "cvfsEnvironment.h"
#include "cSystemInfo.h"

using namespace std;
using namespace vfs;




//{{{
// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

namespace
{

String _tidyPath(const String & Path)
{
  vector<Char> Result;
  Result.resize((Path.size() + 1) * 2);
  if (_wfullpath(&Result[0], Path.c_str(), Result.size()) ==  NULL)
  {
    QFATAL((__FILE__, __LINE__, L"Tried to _wfullpath \"%s\"", Path.c_str()));
  }
  return &Result[0];
}

}

cvfsEnvironment::cvfsEnvironment()
{
  iModuleInstance::Ptr KernelExecInstance = iDLLManager::singleton().getInstance(L"vfs", L"_KernelExec");
  m_LocalMachineRootFolder = iFileManager::singleton().formatAsPath(KernelExecInstance->getPublicSettingString(L"Data\\Storage location"));
  m_BodgeDatabaseFolder    = iFileManager::singleton().formatAsPath(KernelExecInstance->getPublicSettingString(L"Database\\Storage location"));

  String RelativeRootFolder = L"\\..";
  iFileManager::singleton().setCurrentFolder(_tidyPath(iSystemInfo::singleton().getBootFolder() + RelativeRootFolder));
  const String KernelFolder = iFileManager::singleton().getCurrentFolder();
  iFileManager::singleton().setCurrentFolder(_tidyPath(iFileManager::singleton().getCurrentFolder() + RelativeRootFolder));
  m_RelativeRootFolder = iFileManager::singleton().getCurrentFolder();
  iFileManager::singleton().setCurrentFolder(KernelFolder);

  if (m_LocalMachineRootFolder != L"")
  {
    if (!iFileManager::singleton().isFileOrFolderCreated(m_LocalMachineRootFolder)) iFileManager::singleton().createFolder(m_LocalMachineRootFolder);
  }
  if (m_BodgeDatabaseFolder != L"")
  {
    if (!iFileManager::singleton().isFileOrFolderCreated(m_BodgeDatabaseFolder)) iFileManager::singleton().createFolder(m_BodgeDatabaseFolder);
  }
  if (getUserFolder() != L"")
  {
    if (!iFileManager::singleton().isFileOrFolderCreated(getUserFolder())) iFileManager::singleton().createFolder(getUserFolder());
  }
}
//}}}
//{{{
// =======================================================================================
// === getLocalMachineRootFolder/getRelativeRootFolder ===================================
// =======================================================================================

String QAPI cvfsEnvironment::getUserFolder() const
{
  return m_LocalMachineRootFolder == L"" ? L"" : m_LocalMachineRootFolder + L"User\\";
}

// ---------------------------------------------------------------------------------------

#include "cLogManager.h" // Just for GET_MODULE_NAME

#ifdef _M_IX86
#pragma optimize("y",off) // Frame pointer optimisation off so AddrOfCallingCode works
#endif

String QAPI cvfsEnvironment::getLocalMachineRootFolder() const
{
  // Remember return address (in user code) so we can identify who is calling
  void * AddrOfCallingCode;
#ifdef _M_X64
  AddrOfCallingCode = _ReturnAddress();
#else
  __asm
  { 
    __asm push  eax
    __asm mov   eax, [ebp + 4] // 4 is enough to get to return address, i.e. in user code
    __asm mov   AddrOfCallingCode, eax
    __asm pop   eax
  }
#endif
  bool IsQDatabaseCalling = false;
  {
    GET_MODULE_NAME(AddrOfCallingCode);
    IsQDatabaseCalling = (wcsicmp(ModuleName, L"vfs_QDatabase.dll") == 0);
  }
  return IsQDatabaseCalling ? m_BodgeDatabaseFolder : m_LocalMachineRootFolder;
}

#pragma optimize("",on) // Frame pointer optimisation to default

// ---------------------------------------------------------------------------------------

String QAPI cvfsEnvironment::getRelativeRootFolder() const
{
  return m_RelativeRootFolder;
}
//}}}
