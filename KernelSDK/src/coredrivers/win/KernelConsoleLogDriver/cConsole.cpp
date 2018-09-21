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
**/

#include "stdafx.h"
#include "Main.h"
#include <io.h>
#include <fcntl.h>
#include "cConsole.h"
#include <wincon.h>

using namespace vfs;





// =======================================================================================
// === _ConsoleEventHandler ==============================================================
// =======================================================================================

static BOOL WINAPI _ConsoleEventHandler(DWORD CtrlType)
{
  if (iExceptionManager::isSingletonRegistered() && iExceptionManager::isSingletonConstructed())
  {
    iExceptionManager::singleton().fatal(
      0, /* flags */
      iExceptionManager::cFatalDiagnostics(EXIT_SUCCESS, "" /* src file */, 0 /* src line */, L"" /* heading */, L"Cleaning up"));
  }
  else
  {
    (void)TerminateProcess(GetCurrentProcess(), EXIT_SUCCESS);
  }
  return TRUE; // Did we handle it? Yes.
};



// =======================================================================================
// === MonitorEnumCallback ===============================================================
// =======================================================================================

static BOOL CALLBACK MonitorEnumCallback(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
  std::vector<RECT>* monitors = reinterpret_cast<std::vector<RECT>*>(dwData);

  MONITORINFO monitorInfo;
  ZeroMemory(&monitorInfo, sizeof(monitorInfo));
  monitorInfo.cbSize = sizeof(monitorInfo);
  GetMonitorInfo(hMonitor, &monitorInfo);
  if ((monitorInfo.dwFlags & MONITORINFOF_PRIMARY) != 0)
    monitors->insert(monitors->begin(), monitorInfo.rcMonitor);
  else
    monitors->push_back(monitorInfo.rcMonitor);
  return TRUE;
}


// =======================================================================================
// === Construction ======================================================================
// =======================================================================================

cConsole::cConsole(
  const bool          IsShowingStdout,
  const unsigned int  NumColumns,
  const unsigned int  NumRows) :
    m_IsShowingStdout(IsShowingStdout),
    m_ConsoleOutHandle(INVALID_HANDLE_VALUE),
    m_ConsoleInHandle(INVALID_HANDLE_VALUE),
    m_StdErrFileHandle(-1),
    m_StdInFileHandle(-1),
    m_StdOutFileHandle(-1),
    m_IsConsoleEventHandlerAdded(FALSE),
    m_ConsoleWndHandle(0),
    m_StartOnMonitor(0),
    m_StartMaximised(false),
    m_IsShown(false)
{
  // --- Start up a console window -------------------------------------------------------

  if (!AllocConsole())
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to AllocConsole()");
  }

  // --- Set the icon of the console window. ---------------------------------------------

  const HINSTANCE Kernel32DllHandle = LoadLibrary(L"kernel32.dll");
  if (!Kernel32DllHandle)
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to load Kernel32.dll");
  }
  // See MS notes on GetConsoleWindow -
  // "Although this function was implemented in Windows 2000, it was not included in the
  //  version of Wincon.h that was included with the Platform SDK for Windows 2000. To use
  //  this function on Windows 2000, use the prototype included in this documentation, and
  //  dynamically link to the function in Kernel32.dll."
  HWND (WINAPI *GetConsoleWindow)() = reinterpret_cast<decltype(GetConsoleWindow)>(GetProcAddress(Kernel32DllHandle, "GetConsoleWindow"));
  if (!GetConsoleWindow)
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to find GetConsoleWindow() in Kernel32.dll");
  }

  m_ConsoleWndHandle = GetConsoleWindow();
  if (m_ConsoleWndHandle == NULL)
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to GetConsoleWindow()");
  }

  (void)ShowWindow(m_ConsoleWndHandle, SW_HIDE); // Twice because first nCmdShow is ignored in favour of CreateProcess' STARTUPINFO preference.
  (void)ShowWindow(m_ConsoleWndHandle, SW_HIDE);

  // --- Try to position the console window on the secondary monitor ---------------------

  try
  {
    if (gModuleContext->getCustomSettingNumber(L"", L"StartMaximised") != 0)
      m_StartMaximised = true;
  }
  catch (cNotFound)
  {

  }

  try
  {
    m_StartOnMonitor = gModuleContext->getCustomSettingNumber(L"", L"StartOnMonitor");
  }
  catch (cNotFound)
  {

  }

  // --- Open the console file -----------------------------------------------------------

  m_ConsoleOutHandle = CreateFile(
    L"CONOUT$",                             // -> Full filename
    GENERIC_WRITE | GENERIC_READ,           // Access (read-write) mode
    FILE_SHARE_WRITE | FILE_SHARE_READ,     // Share mode
    0,                                      // No security
    OPEN_EXISTING,                          // Creation distribution flags
    FILE_ATTRIBUTE_NORMAL,                  // File attributes
    0);                                     // Handle to file with attributes to copy
  if (m_ConsoleOutHandle == INVALID_HANDLE_VALUE)
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to CreateFile()");
  }
  m_ConsoleInHandle = CreateFile(
    L"CONIN$",                              // -> Full filename
    GENERIC_WRITE | GENERIC_READ,           // Access (read-write) mode
    FILE_SHARE_WRITE | FILE_SHARE_READ,     // Share mode
    0,                                      // No security
    OPEN_EXISTING,                          // Creation distribution flags
    FILE_ATTRIBUTE_NORMAL,                  // File attributes
    0);                                     // Handle to file with attributes to copy
  if (m_ConsoleInHandle == INVALID_HANDLE_VALUE)
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to CreateFile()");
  }

  // --- I/O Mode ------------------------------------------------------------------------

  // No input.
  SetConsoleMode(m_ConsoleInHandle, 0);

  // --- Set the title of the console window. --------------------------------------------

  #ifdef _DEBUG
  const String Title = iSystemInfo::singleton().getProductName() + L" Log (Debug)";
  #else
  const String Title = iSystemInfo::singleton().getProductName() + L" Log";
  #endif

  if (!SetConsoleTitle(Title.c_str()))
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to SetConsoleTitle()");
  }

  // --- Set the number of columns and rows to buffer in the console ---------------------

  const COORD ConsoleCharSize = { (short)NumColumns, (short)NumRows };
  if (ConsoleCharSize.Y < 80)
  {
    throw cWin32Error(__FILE__, __LINE__, L"Num Console Rows must be at least 80");
  }
  if (SetConsoleScreenBufferSize(m_ConsoleOutHandle, ConsoleCharSize) == 0)
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to SetConsoleScreenBufferSize()");
  }

  // --- Apply our console event handler -------------------------------------------------

  m_IsConsoleEventHandlerAdded =  SetConsoleCtrlHandler(_ConsoleEventHandler, TRUE);
  if (!m_IsConsoleEventHandlerAdded)
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to SetConsoleCtrlHandler()");
  }

  // --- Associate a C run-time file handle with a existing operating-system file handle -

  m_StdOutFileHandle = _open_osfhandle((intptr_t)m_ConsoleOutHandle, _O_TEXT);
  if (m_StdOutFileHandle == -1)
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to _open_osfhandle()");
  }
  m_StdInFileHandle = _open_osfhandle((intptr_t)m_ConsoleInHandle, _O_TEXT);
  if (m_StdInFileHandle == -1)
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to _open_osfhandle()");
  }

  // --- Duplicate NewStdOut handle as NewStdErr -----------------------------------------

  m_StdErrFileHandle= dup(m_StdOutFileHandle);
  if (m_StdErrFileHandle == -1)
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to dup2()");
  }

  // --- Connect stdout and stderr -------------------------------------------------------

  // Very Visual C specific, just dive in and change the file descriptors directly.
  #if defined(_MSC_VER)
    if (m_IsShowingStdout)
    {
      _dup2(m_StdErrFileHandle, _fileno(stderr));
      _dup2(m_StdOutFileHandle, _fileno(stdout));
    }
    _dup2(m_StdInFileHandle, _fileno(stdin));
  #else
    #error Compiler not supported!
  #endif

  // --- Switch off buffering ------------------------------------------------------------

  if (setvbuf(stdout, NULL, _IONBF, 0) != 0)
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to setvbuf()");
  }
  if (setvbuf(stderr, NULL, _IONBF, 0) != 0)
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to setvbuf()");
  }

  if (fflush(stdout) == EOF)
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to fflush()");
  }
}




// =======================================================================================
// === Desctruction ======================================================================
// =======================================================================================

cConsole::~cConsole() throw()
{
  // Wait for user to press enter before hiding the console window

  if (m_StdErrFileHandle != -1)
  {
    (void)close(m_StdErrFileHandle);
    m_StdErrFileHandle = -1;
  }

  if (m_StdOutFileHandle != -1)
  {
    (void)close(m_StdOutFileHandle);
    m_StdOutFileHandle = -1;
  }

  if (m_IsConsoleEventHandlerAdded)
  {
    (void)SetConsoleCtrlHandler(_ConsoleEventHandler, FALSE);
  }

  if (m_ConsoleInHandle != INVALID_HANDLE_VALUE)
  {
    CloseHandle(m_ConsoleInHandle);
    m_ConsoleInHandle = INVALID_HANDLE_VALUE;
  }

  if (m_ConsoleOutHandle != INVALID_HANDLE_VALUE)
  {
    CloseHandle(m_ConsoleOutHandle);
    m_ConsoleOutHandle = INVALID_HANDLE_VALUE;
  }

  (void)FreeConsole();
}




// =======================================================================================
// === show/hide =========================================================================
// =======================================================================================

void cConsole::show(const bool IsTopmost)
{
  if (!m_IsShown)
  {
    ShowWindow(m_ConsoleWndHandle, SW_RESTORE);
    m_IsShown = true;
  }

  if (m_StartOnMonitor > 0)
  {
    std::vector<RECT> monitors;
    EnumDisplayMonitors(NULL, NULL, &MonitorEnumCallback, reinterpret_cast<LPARAM>(&monitors));

    m_StartOnMonitor--;
    const RECT& target = monitors.at(m_StartOnMonitor < monitors.size() ? m_StartOnMonitor : 0);

    SetWindowPos(m_ConsoleWndHandle, NULL, target.left + 50, target.top + 50, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
    m_StartOnMonitor = 0; // only move screen on first call to show()
  }

  if (m_StartMaximised)
  {
    ShowWindow(m_ConsoleWndHandle, SW_SHOWMAXIMIZED);
    m_StartMaximised = false; // only maximise on first call to show()
  }

  SetWindowPos(m_ConsoleWndHandle, IsTopmost ? HWND_TOPMOST : HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

// ---------------------------------------------------------------------------------------

void cConsole::hide(void)
{
  ShowWindow(m_ConsoleWndHandle, SW_HIDE);
  m_IsShown = false;
}




// =======================================================================================
// === write =============================================================================
// =======================================================================================

void cConsole::write(const WriteFlag Flags, const String & Msg) throw(cWin32Error)
{

  WORD ConsoleAttr = 0;
  if (Flags & LWF_RED)              ConsoleAttr |= FOREGROUND_RED;
  if (Flags & LWF_GREEN)            ConsoleAttr |= FOREGROUND_GREEN;
  if (Flags & LWF_BLUE)             ConsoleAttr |= FOREGROUND_BLUE;
  if (Flags & LWF_REDBKGND)         ConsoleAttr |= BACKGROUND_RED;
  if (Flags & LWF_GREENBKGND)       ConsoleAttr |= BACKGROUND_GREEN;
  if (Flags & LWF_BLUEBKGND)        ConsoleAttr |= BACKGROUND_BLUE;
  if (Flags & LWF_INTENSITY)        ConsoleAttr |= FOREGROUND_INTENSITY;
  if (Flags & LWF_INTENSITYBKGND)   ConsoleAttr |= BACKGROUND_INTENSITY;
  if (ConsoleAttr)
  {
    if (!SetConsoleTextAttribute(m_ConsoleOutHandle, ConsoleAttr))
    {
      throw cWin32Error(__FILE__, __LINE__, L"Tried to SetConsoleTextAttribute()");
    }
  }

  DWORD NumCharsWritten;
  if (!WriteConsole(
    m_ConsoleOutHandle, // Handle to screen buffer
    Msg.c_str(),        // Write buffer
    (DWORD) Msg.size(), // Number of characters to write
    &NumCharsWritten,   // Number of characters written
    NULL))              // Reserved
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to WriteFile()");
  }
}
