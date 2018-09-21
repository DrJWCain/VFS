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

#include "cProcessHelp.h"
#include <psapi.h>

// --- Use Namespace ---------------------------------------------------------------------

using namespace std;
using namespace vfs;




// =======================================================================================
// === executeProcess ====================================================================
// =======================================================================================

void cProcessHelp::executeProcess(const String & ExePath, const String & ExeFileName, const String & ExeParams, const int Flags) throw(cWin32Error)
{
  executeProcessWait(ExePath, ExeFileName, ExeParams, INFINITE /* timeout */, Flags);
}




// =======================================================================================
// === executeProcess ====================================================================
// =======================================================================================

void cProcessHelp::executeProcessWait(const String & ExePath, const String & ExeFileName, const String & ExeParams, const unsigned int TimeoutMillisecs, const int Flags) throw(cWin32Error, cTimedOut)
{
  const String Exe    = ExePath + ExeFileName;
  String       Params = ExeFileName + L" " + ExeParams;

  PROCESS_INFORMATION ProcessInfo;
  STARTUPINFO         StartupInfo;
  memset(&ProcessInfo, 0, sizeof(ProcessInfo));
  memset(&StartupInfo, 0, sizeof(StartupInfo));
  StartupInfo.cb = sizeof(StartupInfo);
  StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
  StartupInfo.wShowWindow = SW_SHOWMINNOACTIVE;

  QMSG((0, L"Executing \"%s\"", Params.c_str()));

  DWORD CreationFlags = 0;
  if ((Flags & fExeProcess_ShowWindow) == 0)
  {
    CreationFlags |= CREATE_NO_WINDOW;
  }
  else
  {
    CreationFlags |= CREATE_NEW_CONSOLE;
  }

  if (!CreateProcess(
    Exe.c_str(),                        // Name of executable module
    (Char *)Params.c_str(),             // Command line string
    NULL,                               // SD
    NULL,                               // SD
    FALSE,                              // Handle inheritance option
    CreationFlags,                      // Creation flags
    NULL,                               // New environment block
    ExePath.c_str(),                    // Current directory name
    &StartupInfo,                       // Startup information
    &ProcessInfo))                      // Process information
  {
    const DWORD LastError = GetLastError();
    throw cWin32Error(__FILE__, __LINE__, L"Tried to CreateProcess \"%s %s\" - returned %d (%s)", Exe.c_str(), Params.c_str(), LastError, win32ErrorToString(LastError).c_str());
  }
  cWin32HandleGuard ProcessHandle(ProcessInfo.hProcess);

  if (TimeoutMillisecs != INFINITE)
  {
    if (WaitForSingleObject(ProcessHandle.it, TimeoutMillisecs) == WAIT_TIMEOUT)
    {
      const DWORD LastError = GetLastError();
      throw cTimedOut(__FILE__, __LINE__, L"Timed out WaitForSingleObject on process \"%s\" - returned %d (%s)", Params.c_str(), LastError, win32ErrorToString(LastError).c_str());
    }

    DWORD ExitCode = 0;
    if (!GetExitCodeProcess(ProcessHandle.it, &ExitCode))
    {
      const DWORD LastError = GetLastError();
      throw cWin32Error(__FILE__, __LINE__, L"Tried to GetExitCodeProcess() on process \"%s\" - returned 0x%x (%s)", Params.c_str(), LastError, win32ErrorToString(LastError).c_str());
    }

    if (ExitCode != 0)
    {
      throw cWin32Error(__FILE__, __LINE__, L"Process \"%s\" returned %d (0x%x, %s)", Params.c_str(), ExitCode, ExitCode, win32ErrorToString(ExitCode).c_str());
    }
  }
}




// =======================================================================================
// === killProcess =======================================================================
// =======================================================================================

namespace
{

BOOL CALLBACK enumWindowsProc(HWND hWnd, LPARAM lParam)
{
  multimap<unsigned int, HWND> * const Result = (multimap<unsigned int, HWND> *)lParam;
  DWORD ProcessId;
  (void)GetWindowThreadProcessId(hWnd, &ProcessId);
  Result->insert(make_pair(ProcessId, hWnd));
  return TRUE;
}

multimap<unsigned int, HWND> _getTopLevelWindowHandles()
{
  multimap<unsigned int, HWND> Result;
  if (EnumWindows(&enumWindowsProc, (LPARAM)&Result) == 0)
  {
    const DWORD LastError = GetLastError();
    throw cWin32Error(__FILE__, __LINE__, L"Tried to EnumWindows - returned %d (%s)", LastError, win32ErrorToString(LastError).c_str());
  }
  return Result;
}

// ---------------------------------------------------------------------------------------

unsigned int _getProcessIdFromName(const String & ProcessNameToFind) throw(cNotFound, cWin32Error)
{
  unsigned int ProcessId = 0;

  DWORD ProcessesIds[1024], cbNeeded;
  if (EnumProcesses(ProcessesIds, sizeof(ProcessesIds), &cbNeeded) == 0)
  {
    const DWORD LastError = GetLastError();
    throw cWin32Error(__FILE__, __LINE__, L"Tried to EnumProcesses - returned %d (%s)", LastError, win32ErrorToString(LastError).c_str());
  }
  const unsigned int NumProcesses = cbNeeded / sizeof(DWORD);

  for (unsigned int i = 0; i < NumProcesses; i++)
  {
    const cWin32HandleGuard ProcessHandle(OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE /* inheritable handle? */, ProcessesIds[i]));
    if (ProcessHandle.it != NULL)
    {
      HMODULE FirstModuleHandle;
      DWORD cbNeeded;
      if (EnumProcessModules(ProcessHandle.it, &FirstModuleHandle, sizeof(FirstModuleHandle), &cbNeeded) )
      {
        Char ProcessName[MAX_PATH];
        if (GetModuleBaseName(ProcessHandle.it, FirstModuleHandle, ProcessName, sizeof(ProcessName)) == 0)
        {
          const DWORD LastError = GetLastError();
          throw cWin32Error(__FILE__, __LINE__, L"Tried to GetModuleBaseName - returned %d (%s)", LastError, win32ErrorToString(LastError).c_str());
        }
        if (toUpper(ProcessNameToFind) == toUpper(ProcessName))
        {
          ProcessId = ProcessesIds[i];
          break;
        }
      }
    }
  }

  if (ProcessId == 0)
  {
    throw cNotFound(__FILE__, __LINE__, L"Process \"$s\" not found", ProcessNameToFind.c_str());
  }
  
  return ProcessId;
}

// ---------------------------------------------------------------------------------------

void _exitWindowedProcess(const unsigned int ProcessId, const unsigned TimeoutMillisecs) throw(cTimedOut, cWin32Error, cNotFound)
{
  const multimap<unsigned int, HWND> TopLevelWindowsByProcessId = _getTopLevelWindowHandles();
  multimap<unsigned int, HWND>::const_iterator i = TopLevelWindowsByProcessId.find(ProcessId);
  bool HasSentCloseMsg = false;
  if (i != TopLevelWindowsByProcessId.end())
  {
//    WaitForInputIdle();
    if (!PostMessage(i->second, WM_CLOSE, 0 /* wParam */, 0 /* lParam */))
    {
      const DWORD lastError = GetLastError();
      throw cNotFound(__FILE__, __LINE__, L"Tried to PostMessage to window with process id %d - returned 0x%x (%s)", ProcessId, lastError, win32ErrorToString(lastError).c_str());
    }
    HasSentCloseMsg = true;
  }
  if (!HasSentCloseMsg)
  {
    throw cNotFound(__FILE__, __LINE__, L"No top level window belonging to Process Id %d found", ProcessId);
  }

//    Sleep(1000);

  const cWin32HandleGuard ProcessHandle(OpenProcess(PROCESS_QUERY_INFORMATION, FALSE /* inheritable handle? */, ProcessId));
  if (ProcessHandle.it == NULL)
  {
    const DWORD LastError = GetLastError();
    if (LastError == 87 /* invalid arg */)
    {
      // Process probably exited befroe we got to do OpenProcess
    }
    else
    {
      throw cWin32Error(__FILE__, __LINE__, L"Tried to OpenProcess %d - returned %d (%s)", ProcessId, LastError, win32ErrorToString(LastError).c_str());
    }
  }
  else
  {
    if (WaitForSingleObject(ProcessHandle.it, TimeoutMillisecs) == WAIT_TIMEOUT)
    {
      const DWORD LastError = GetLastError();
      throw cTimedOut(__FILE__, __LINE__, L"Timed out WaitForSingleObject on process %d - returned %d (%s)", ProcessId, LastError, win32ErrorToString(LastError).c_str());
    }
  }
}

// ---------------------------------------------------------------------------------------

void _terminateProcess(const unsigned int ProcessId, const unsigned TimeoutMillisecs) throw(cWin32Error, cNotFound)
{
  const cWin32HandleGuard ProcessHandle(OpenProcess(PROCESS_TERMINATE, FALSE /* inheritable handle? */, ProcessId));
  if (ProcessHandle.it == NULL)
  {
    const DWORD LastError = GetLastError();
    if (LastError == 87)
    {
      throw cNotFound(__FILE__, __LINE__, L"Tried to OpenProcess %d - returned %d (%s), process exited already?", ProcessId, LastError, win32ErrorToString(LastError).c_str());
    }
    else
    {
      throw cWin32Error(__FILE__, __LINE__, L"Tried to OpenProcess %d - returned %d (%s)", ProcessId, LastError, win32ErrorToString(LastError).c_str());
    }
  }
  if (TerminateProcess(ProcessHandle, EXIT_FAILURE) == 0)
  {
    const DWORD LastError = GetLastError();
    throw cWin32Error(__FILE__, __LINE__, L"Tried to TerminateProcess %d - returned %d (%s)", ProcessId, LastError, win32ErrorToString(LastError).c_str());
  }
  if (WaitForSingleObject(ProcessHandle.it, TimeoutMillisecs) == WAIT_TIMEOUT)
  {
    const DWORD LastError = GetLastError();
    throw cTimedOut(__FILE__, __LINE__, L"Timed out WaitForSingleObject on process %d - returned %d (%s)", ProcessId, LastError, win32ErrorToString(LastError).c_str());
  }
}

}

// ---------------------------------------------------------------------------------------

void cProcessHelp::killProcess(const String & ProcessNameToFind, const unsigned TimeoutMillisecs) throw(cNotFound, cWin32Error, cTimedOut)
{
  const unsigned int ProcessId = _getProcessIdFromName(ProcessNameToFind);
  try
  {
    _exitWindowedProcess(ProcessId, TimeoutMillisecs);
    const Int64 start = iSystemInfo::singleton().getMillisecsSinceStart();
    while (isProcessRunning(ProcessNameToFind))
    {
      const Int64 now = iSystemInfo::singleton().getMillisecsSinceStart();
      if (now - start > (Int64)TimeoutMillisecs) throw cTimedOut(__FILE__, __LINE__, L"Timed out waiting for isProcessRunning() to return false");
      Sleep(100);
    }
  }
  catch (cNotFound &)
  {
    // Process did not have a top level window
    try
    {
      _terminateProcess(ProcessId, TimeoutMillisecs);
    }
    catch (cNotFound &)
    {
      // Process exited already?
    }
  }
  catch (cTimedOut &)
  {
    try
    {
      _terminateProcess(ProcessId, TimeoutMillisecs);
    }
    catch (cNotFound &)
    {
      // Process exited already?
    }
  }
  const Int64 start = iSystemInfo::singleton().getMillisecsSinceStart();
  while (isProcessRunning(ProcessNameToFind))
  {
    const Int64 now = iSystemInfo::singleton().getMillisecsSinceStart();
    if (now - start > (Int64)TimeoutMillisecs) throw cTimedOut(__FILE__, __LINE__, L"Timed out waiting for isProcessRunning() to return false");
    Sleep(100);
  }
}




// =======================================================================================
// === isProcessRunning ==================================================================
// =======================================================================================

bool cProcessHelp::isProcessRunning(const String & ProcessName) throw(cWin32Error)
{
  try
  {
    (void)_getProcessIdFromName(ProcessName);
    return true;
  }
  catch (cNotFound &)
  {
    return false;
  }
}




// =======================================================================================
// === getAllProcessesNames ==============================================================
// =======================================================================================

vector<String> cProcessHelp::getAllProcessesNames() throw(cWin32Error)
{
  vector<String> Result;

  DWORD ProcessesIds[1024], cbNeeded;
  if (EnumProcesses(ProcessesIds, sizeof(ProcessesIds), &cbNeeded) == 0)
  {
    const DWORD LastError = GetLastError();
    throw cWin32Error(__FILE__, __LINE__, L"Tried to EnumProcesses - returned %d (%s)", LastError, win32ErrorToString(LastError).c_str());
  }
  const unsigned int NumProcesses = cbNeeded / sizeof(DWORD);

  for (unsigned int i = 0; i < NumProcesses; i++)
  {
    const cWin32HandleGuard ProcessHandle(OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE /* inheritable handle? */, ProcessesIds[i]));
    if (ProcessHandle.it != NULL)
    {
      HMODULE FirstModuleHandle;
      DWORD cbNeeded;
      if (EnumProcessModules(ProcessHandle.it, &FirstModuleHandle, sizeof(FirstModuleHandle), &cbNeeded) )
      {
        Char ProcessName[MAX_PATH];
        if (GetModuleBaseName(ProcessHandle.it, FirstModuleHandle, ProcessName, sizeof(ProcessName)) == 0)
        {
          const DWORD LastError = GetLastError();
          throw cWin32Error(__FILE__, __LINE__, L"Tried to GetModuleBaseName - returned %d (%s)", LastError, win32ErrorToString(LastError).c_str());
        }
        Result.push_back(ProcessName);
      }
    }
  }

  return Result;
}
