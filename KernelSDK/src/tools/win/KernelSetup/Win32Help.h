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

#include <psapi.h>
#include <shlobj.h>
#include <QKernelBase.h>
#include <QWinBase.h>

using namespace std;
using namespace vfs;




// =======================================================================================
// === Win32 Helpers =====================================================================
// =======================================================================================

namespace
{

bool _createAppMutex(const String & AppName, HANDLE * const MutexHandle)
{
  *MutexHandle = 0;
  *MutexHandle = CreateMutex(NULL, FALSE, String(AppName + L" Running").c_str());
  return !((*MutexHandle == NULL) || (GetLastError() == ERROR_ALREADY_EXISTS));
}

String tostringWin32ErrCode(const long Win32ErrCode)
{
  String ErrorMessage;
  Char * FormatMessageBuffer = 0;
	if (FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
		NULL,
		Win32ErrCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&FormatMessageBuffer,
		0,
		NULL) != 0)
  {
    ErrorMessage = FormatMessageBuffer;
    LocalFree(FormatMessageBuffer);
  }
  else
  {
    ErrorMessage = L"Unknown Win32 error code";
  }
  return ErrorMessage;
}

// ---------------------------------------------------------------------------------------

String _getModulePath(const HINSTANCE InstanceHandle) throw(cWin32Error)
{
  Char Buffer[512];
  if (GetModuleFileNameEx(
    GetCurrentProcess(),                        // Handle to the process
    InstanceHandle,                             // Handle to the module
    Buffer,                                     // -> Buffer that receives the path
    (sizeof(Buffer) - 1) / sizeof(Char)) == 0)  // Maximum number of characters to retrieve
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to GetModuleFileNameEx (%s)", tostringWin32ErrCode(GetLastError()).c_str());
  }
  return Buffer;
}

// ---------------------------------------------------------------------------------------

String _getModuleFilename(const HINSTANCE InstanceHandle) throw(cWin32Error)
{
  Char Buffer[512];
  if (GetModuleFileNameEx(
    GetCurrentProcess(),                        // Handle to the process
    InstanceHandle,                             // Handle to the module
    Buffer,                                     // -> Buffer that receives the path
    (sizeof(Buffer) - 1) / sizeof(Char)) == 0)  // Maximum number of characters to retrieve
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to GetModuleFileNameEx (%s)", tostringWin32ErrCode(GetLastError()).c_str());
  }
  Char * const LastSlash = wcsrchr(Buffer, L'\\');
  assert(LastSlash);
  return LastSlash + 1;
}

// ---------------------------------------------------------------------------------------

String _getModuleFolder(const HINSTANCE InstanceHandle) throw(cWin32Error)
{
  Char Buffer[512];
  if (GetModuleFileNameEx(
    GetCurrentProcess(),                        // Handle to the process
    InstanceHandle,                             // Handle to the module
    Buffer,                                     // -> Buffer that receives the path
    (sizeof(Buffer) - 1) / sizeof(Char)) == 0)  // Maximum number of characters to retrieve
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to GetModuleFileNameEx (%s)", tostringWin32ErrCode(GetLastError()).c_str());
  }
  // Chop name after last slash - we only want to return the folder path.
  Char * const LastSlash = wcsrchr(Buffer, L'\\');
  assert(LastSlash);
  LastSlash[1] = L'\0';
  return Buffer;
}

// ---------------------------------------------------------------------------------------

HANDLE _launchProcess(
  const HINSTANCE     InstanceHandle,
  const String        ExecutableFilename,
  const String        CommandLineParams,
  const String        CurrentFolder,
  const bool          IsWaitingToComplete) throw(cWin32Error)
{
  PROCESS_INFORMATION ProcessInfo;
  memset(&ProcessInfo, 0, sizeof(ProcessInfo));

  STARTUPINFO StartupInfo;
  memset(&StartupInfo, 0, sizeof(StartupInfo));
  StartupInfo.cb          = sizeof(StartupInfo);
  StartupInfo.dwFlags     = STARTF_USESHOWWINDOW;
  StartupInfo.wShowWindow = SW_NORMAL;

  String CopyOfCommandLineParams(ExecutableFilename + L" " + CommandLineParams);
  if (!CreateProcess(
    ExecutableFilename.c_str(),                // Name of executable module
    (TCHAR *)CopyOfCommandLineParams.c_str(),  // Command line string
    NULL,                                      // SD
    NULL,                                      // SD
    FALSE,                                     // Handle inheritance option
    NORMAL_PRIORITY_CLASS,                     // Creation flags
    NULL,                                      // New environment block
    CurrentFolder.c_str(),                     // Current directory name
    &StartupInfo,                              // Startup information
    &ProcessInfo))                             // Process information
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to CreateProcess (%s)", tostringWin32ErrCode(GetLastError()).c_str());
  }

  if (IsWaitingToComplete)
  {
    (void)WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
  }
  
  return ProcessInfo.hProcess;
}

// ---------------------------------------------------------------------------------------

String _createTemporaryFolder(const String & Prefix) throw(cWin32Error)
{
  Char Win32TempPath[MAX_PATH];
  if (GetTempPath(MAX_PATH - 1, Win32TempPath) == 0)
  {
	  throw cWin32Error(__FILE__, __LINE__, L"Tried to GetTempPath (%s)", tostringWin32ErrCode(GetLastError()).c_str());
  }
  const String NewFolder(String(Win32TempPath) + Prefix + L"_" + cGuid::createUnique(L"").toString(false /* no comment */));
  if (!CreateDirectory(
	  NewFolder.c_str(),  // Folder name
	  NULL))              // -> Securit attributes
  {
	  throw cWin32Error(__FILE__, __LINE__, L"Tried to CreateDirectory() \"%s\" - returned \"%s\"", NewFolder.c_str(), tostringWin32ErrCode(GetLastError()).c_str());
  }
  return NewFolder +  L"\\";
}

}




// =======================================================================================
// === _createShortcut ===================================================================
// =======================================================================================

namespace
{

void _createShortcut(const String & DstFolder, const String & Name, const String & Filename, const String & Parameters, const String & Description, const String & Branding, const int IconId = 1) throw(cWin32Error)
{
  #if defined(QMSG)
    QMSG((0, L"Creating shortcut: \"%s\\%s.lnk\" -> \"%s\" (params \"%s\")", DstFolder.c_str(), Name.c_str(), Filename.c_str(), Parameters.c_str()));
  #endif

  (void)CoInitialize(NULL);

  // Get a pointer to the IShellLink interface.
  IShellLink* psl = NULL;
  HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, reinterpret_cast<void**>(&psl));
  if (FAILED(hr)) throw cWin32Error(__FILE__, __LINE__, L"Tried to CoCreateInstance() - 0x%x", hr);

  psl->SetPath(Filename.c_str());
  psl->SetArguments(Parameters.c_str());
  psl->SetDescription(Description.c_str());
  if (Branding != L"")
  {
    psl->SetIconLocation(Branding.c_str(), -IconId);
  }
  // Query IShellLink for the IPersistFile interface for
  // saving the shortcut in persistent storage.
  IPersistFile* ppf = NULL;
  hr = psl->QueryInterface(IID_IPersistFile, reinterpret_cast<void**>(&ppf));
  if (FAILED(hr)) throw cWin32Error(__FILE__, __LINE__, L"Tried to QueryInterface() - 0x%x", hr);

  // Save the link by calling IPersistFile::Save.
  hr = ppf->Save(String(DstFolder +  L"\\" + Name + L".lnk").c_str(), TRUE);
  if (FAILED(hr)) throw cWin32Error(__FILE__, __LINE__, L"Tried to Save() - 0x%x", hr);

  ppf->Release();

  psl->Release();
}

}




// =======================================================================================
// === _getThisModuleFolder ==============================================================
// =======================================================================================

namespace
{

String _getThisModuleFolder()
{
  String Result;
  HMODULE hMods[1024];
  DWORD cbNeeded;
  HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId());
  if (NULL == hProcess) return L"";
  if( EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
  {
    for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
    {
      Char szModName[MAX_PATH];
      if ( GetModuleFileNameEx(hProcess, hMods[i], szModName, sizeof(szModName)))
      {
        if (toLower(szModName).find(toLower(L"KernelControlPanelLauncher")) != String::npos)
        {
          Result = _getModuleFolder(hMods[i]);
          break;
        }
      }
    }
  }
  CloseHandle( hProcess );
  return Result;
}

}




// =======================================================================================
// === _getThisModulePath ================================================================
// =======================================================================================

namespace
{

String _getThisModulePath()
{
  String Result;
  HMODULE hMods[1024];
  DWORD cbNeeded;
  HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId());
  if (NULL == hProcess) return L"";
  if( EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
  {
    for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
    {
      Char szModName[MAX_PATH];
      if ( GetModuleFileNameEx(hProcess, hMods[i], szModName, sizeof(szModName)))
      {
        if (toLower(szModName).find(toLower(L"KernelControlPanelLauncher")) != String::npos)
        {
          Result = _getModulePath(hMods[i]);
          break;
        }
      }
    }
  }
  CloseHandle( hProcess );
  return Result;
}

}




// =======================================================================================
// === _getDiskAvailableSize =============================================================
// =======================================================================================

namespace
{

Int64 _getDiskAvailableSize(const String & Path) throw(cWin32Error)
{
  ULARGE_INTEGER freeBytesAvailable;      // bytes available to caller
  ULARGE_INTEGER totalNumberOfBytes;      // bytes on disk
  ULARGE_INTEGER totalNumberOfFreeBytes;  // free bytes on disk
  if (!GetDiskFreeSpaceEx(Path.c_str(), &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes))
  {
    const DWORD LastError = GetLastError();
    throw cWin32Error(__FILE__, __LINE__, L"Tried to GetDiskFreeSpaceEx() on \"%s\" - returned %d (%s)", Path.c_str(), LastError, tostringWin32ErrCode(LastError));
  }
  return freeBytesAvailable.QuadPart;
}

}
