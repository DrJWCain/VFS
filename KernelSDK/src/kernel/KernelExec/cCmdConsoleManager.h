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

// --- Headers ---------------------------------------------------------------------------

#include "cCmdConsoleHandler.h"
#pragma warning(push)
#pragma warning(disable: 4201)  // Ignore "nonstandard extension used : nameless struct/union"
#include <mmsystem.h>
#pragma warning(pop)

// --- Namespace -------------------------------------------------------------------------

using namespace std;
using namespace vfs;



// =======================================================================================
// === cCmdConsoleManager ================================================================
// =======================================================================================

class cCmdConsoleManager : public iCmdConsoleManager, public cRefCount
{
  public :

    QDEFINE_SINGLETON_IMPL(cCmdConsoleManager, L"{8CDA4BCD-9204-4e0a-8D94-6EAC08D7C635} // cCmdConsoleManager", iCmdConsoleManager);

    // iCmdConsoleManager
    virtual void QAPI queueCommand(const String & command, const String & params);
    virtual cGuard::ConstPtr QAPI addCommand(const iCommand::Ptr & CommandToAdd, const bool IsExecuteInMasterThreadOnly,const bool MakeLowerCaseAllParams);
    virtual void QAPI dispatchMainThreadCommands();

    // cCmdConsoleManager
    cCmdConsoleManager();
    ~cCmdConsoleManager();

  private :

    cCmdConsoleHandler::Ptr          m_CmdService;
};
