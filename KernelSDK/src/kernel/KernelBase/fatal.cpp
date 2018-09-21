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
#include "fatal.h"
#include "psapi.h"

using namespace vfs;




//extern "C" _declspec(dllexport) bool /* was handled? */ QARGS_STACK kernelBaseFatalHandler(const char * const srcFilename, const unsigned long srcLineNumber, const Char * const msg);
typedef bool /* was handled? */ (QARGS_STACK * kernelBaseFatalHandlerFuncPtr)(const char * const srcFilename, const unsigned long srcLineNumber, const Char * const msg);

__declspec(noreturn) void fatal(const char * const srcFilename, const unsigned long srcLineNumber, const Char * const msgFormat, ...)
{
  bool wasHandled = false;

  va_list msgParams;
  va_start(msgParams, msgFormat);
  Char msg[512];
  _vsnwprintf(msg, sizeof(msg) / sizeof(Char), msgFormat, msgParams);
  va_end(msgParams);

  kernelBaseFatalHandlerFuncPtr handlerFuncPtr = 0;
  {
    Char exeName[MAX_PATH];
    if (GetModuleBaseName(GetCurrentProcess(), NULL /* module handle */, exeName, sizeof(exeName)) != 0)
    {
      const HMODULE exeModuleHandle = LoadLibrary(exeName);
      if (exeModuleHandle != NULL)
      {
        handlerFuncPtr = (kernelBaseFatalHandlerFuncPtr)GetProcAddress(exeModuleHandle, "kernelBaseFatalHandler");
        if (handlerFuncPtr)
        {
          wasHandled = handlerFuncPtr(srcFilename, srcLineNumber, msg);
        }
      }
    }
  }

  if (!wasHandled)
  {
    OutputDebugString(L"vfs KernelBase : Fatal : ");
    OutputDebugString(msg);
    OutputDebugString(L"\n");

    // Breakpoint
    if (IsDebuggerPresent())
      __debugbreak();

    Char tmp[1024];
    (void)_snwprintf(
      tmp, sizeof(tmp) / sizeof(Char),
      L"%s\n\n%s\n\nFile: %s\nLine: %d",
      L"Fatal Error", msg, widen(srcFilename).c_str(), srcLineNumber);
    (void)ShowCursor(TRUE);
    (void)MessageBox(NULL, tmp, L"KernelBase", MB_ICONERROR | MB_TOPMOST | MB_SETFOREGROUND | MB_TASKMODAL);

    (void)TerminateProcess(GetCurrentProcess(), EXIT_FAILURE);
}
}
