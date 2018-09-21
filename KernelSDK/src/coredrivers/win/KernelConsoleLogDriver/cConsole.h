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

#include <QKernelBase.h>
#include <QWinBase.h>
#include "QKernelExecDDK.h"

using namespace vfs;




// =======================================================================================
// === cConsole ==========================================================================
// =======================================================================================

class cConsole : public cRefCount
{
	public:

    QDEFINE_SMARTPTR(cConsole); // Define smart pointer types.

		cConsole(const bool IsShowingStdout, const unsigned int NumColumns, const unsigned int NumRows) throw(cWin32Error);
		~cConsole();

    typedef unsigned int WriteFlag;
    #define LWF_UPPERCASE          (1 << 0)
    #define LWF_APPENDNEWLINE      (1 << 1)
    #define LWF_STRIPCR            (1 << 3)
    #define LWF_RED                (1 << 4)
    #define LWF_GREEN              (1 << 5)
    #define LWF_BLUE               (1 << 6)
    #define LWF_REDBKGND           (1 << 7)
    #define LWF_GREENBKGND         (1 << 8)
    #define LWF_BLUEBKGND          (1 << 9)
    #define LWF_INTENSITY          (1 << 10)
    #define LWF_INTENSITYBKGND     (1 << 11)
    #define LWF_WHITE              (LWF_RED | LWF_GREEN | LWF_BLUE)
    #define LWF_YELLOW             (LWF_RED | LWF_GREEN)
    #define LWF_CYAN               (LWF_GREEN | LWF_BLUE)
    #define LWF_MAGENTA            (LWF_RED | LWF_BLUE)

		void write(const WriteFlag	Flags, const String & Msg) throw(cWin32Error);

    void show(const bool IsTopmost);
    void hide();

	private:

    bool   m_IsShowingStdout;
    HANDLE m_ConsoleOutHandle;
    HANDLE m_ConsoleInHandle;
    int    m_StdErrFileHandle;
    int    m_StdInFileHandle;
    int    m_StdOutFileHandle;
    BOOL   m_IsConsoleEventHandlerAdded;
    HWND   m_ConsoleWndHandle;
    bool   m_IsShown;

    DWORD  m_StartOnMonitor;
    bool   m_StartMaximised;
};
