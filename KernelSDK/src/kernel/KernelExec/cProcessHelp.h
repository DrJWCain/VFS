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

#include <QWinBase.h>
#include "QKernelExec.h"

using namespace std;
using namespace vfs;




// =======================================================================================
// === cProcessHelp ======================================================================
// =======================================================================================

class cProcessHelp : public cRefCount
{
	public :
    QDEFINE_SMARTPTR(cProcessHelp);
  
    virtual void executeProcess(const String &  ExePath, const String &  ExeFileName, const String &  ExeParams, const int Flags) throw(cWin32Error);
    virtual void killProcess(const String & ProcessName, const unsigned TimeoutMillisecs) throw(cWin32Error, cNotFound, cTimedOut);
    virtual bool isProcessRunning(const String & ProcessName) throw(cWin32Error, cNotFound);
    virtual vector<String> getAllProcessesNames() throw(cWin32Error);
    virtual void executeProcessWait(const String &  ExePath, const String &  ExeFileName, const String &  ExeParams, const unsigned int TimeoutMillisec, const int Flags) throw(cWin32Error, cTimedOut);
};
