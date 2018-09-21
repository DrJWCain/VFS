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

#include "cModuleInstallHelp.h"

// --- Start of Namespace ----------------------------------------------------------------

using namespace vfs;




// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

cModuleInstallHelp::cModuleInstallHelp() :
  m_ProcessHelp(new cProcessHelp)
{
  Char tmp[256];
  if (GetSystemWindowsDirectory(tmp, sizeof(tmp)) == 0)
  {
    QFATAL((__FILE__, __LINE__, L"Failed to GetSystemWindowsDirectory"));
  }
  m_SystemRootFolder = String(tmp) + L"\\";
}




// =======================================================================================
// === executeProcess ====================================================================
// =======================================================================================

void QAPI cModuleInstallHelp::executeProcess(const String & ExePath, const String & ExeFileName, const String & ExeParams, const int Flags) throw(cWin32Error)
{
  m_ProcessHelp->executeProcess(ExePath, ExeFileName, ExeParams, Flags);
}

// ---------------------------------------------------------------------------------------

void QAPI cModuleInstallHelp::executeProcessWait(const String &  ExePath, const String &  ExeFileName, const String &  ExeParams, const unsigned int TimeoutMillisecs, const int Flags) throw(cWin32Error, cTimedOut)
{
  m_ProcessHelp->executeProcessWait(ExePath, ExeFileName, ExeParams, TimeoutMillisecs, Flags);
}




// =======================================================================================
// === regsvr32/unregsvr32 ===============================================================
// =======================================================================================

void QAPI cModuleInstallHelp::regsvr32(const String & DllFilePath) throw(cNotFound, cWin32Error)
{
  executeProcess(m_SystemRootFolder + L"system32\\", L"regsvr32.exe", String(L"/s \"") + DllFilePath + L"\"", 0 /* flags */);
}

// ---------------------------------------------------------------------------------------

void QAPI cModuleInstallHelp::unregsvr32(const String & DllFilePath) throw(cNotFound, cWin32Error)
{
  executeProcess(m_SystemRootFolder + L"system32\\", L"regsvr32.exe", String(L"/s /u \"") + DllFilePath + L"\"", 0 /* flags */);
}




// =======================================================================================
// === copyFileOrFolder ==================================================================
// =======================================================================================

void QAPI cModuleInstallHelp::copyFileOrFolder(const String & SrcFilePath, const String & DstFilePath) throw(cBadFormat, cFileError)
{
  iFileManager::singleton().copyFileOrFolder(SrcFilePath, DstFilePath, fFileAccess_ReadWrite);
}




// =======================================================================================
// === killProcess =======================================================================
// =======================================================================================

void QAPI cModuleInstallHelp::killProcess(const String & ProcessNameToFind, const unsigned TimeoutMillisecs) throw(cNotFound, cWin32Error)
{
  return m_ProcessHelp->killProcess(ProcessNameToFind, TimeoutMillisecs);
}




// =======================================================================================
// === isProcessRunning ==================================================================
// =======================================================================================

bool QAPI cModuleInstallHelp::isProcessRunning(const String & ProcessName) throw(cNotFound, cWin32Error)
{
  return m_ProcessHelp->isProcessRunning(ProcessName);
}




// =======================================================================================
// === installSharedDll ==================================================================
// =======================================================================================

// SJSJSJ Use correct method for installing shared DLLs
#pragma message("***** fix me ******")

void QAPI cModuleInstallHelp::installSharedDll(const String & SrcDllPath) throw(cBadFormat, cFileError, cWin32Error)
{
  String DllFilename;
  {
    iFile::Ptr Dll = iFileManager::singleton().openFile(SrcDllPath, fFileAccess_Read);
    DllFilename = Dll->getName() + Dll->getExtension();
  }
  const String DstDllPath(m_SystemRootFolder + L"system32\\" + DllFilename);
  if (!iFileManager::singleton().isFileOrFolderCreated(DstDllPath))
  {
    copyFileOrFolder(SrcDllPath, DstDllPath);
    try
    {
      regsvr32(DstDllPath);
    }
    catch (cRecoverable &)
    {
      // Maybe not be a COM server
    }
  }
}

// ---------------------------------------------------------------------------------------

// SJSJSJ Use correct method for uninstalling shared DLLs
#pragma message("***** fix me ******")

void QAPI cModuleInstallHelp::uninstallSharedDll(const String & DllFilename) throw(cBadFormat, cFileError)
{
  const String DllPath(m_SystemRootFolder + L"system32\\" + DllFilename);
  if (iFileManager::singleton().isFileOrFolderCreated(DllPath))
  {
    try
    {
//      unregsvr32(DllPath);
    }
    catch (cRecoverable &)
    {
      // Maybe not be a COM server
    }
  }
}
