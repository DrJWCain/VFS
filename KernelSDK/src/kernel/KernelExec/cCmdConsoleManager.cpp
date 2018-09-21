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
#include "cCmdConsoleManager.h"
#include "LogCategories.h"
#include "cSystemInfo.h"

using namespace vfs;



// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

cCmdConsoleManager::cCmdConsoleManager()
{
  cLogIndentGuard IndentGuard(kCtgCreateManagerDetail, L"Creating cCmdConsoleManager");

  m_CmdService = new cCmdConsoleHandler();
  if (cSystemInfo::singleton().getProductName() != L"HyperSpace") m_CmdService->startLoop();
}

// ---------------------------------------------------------------------------------------

cCmdConsoleManager::~cCmdConsoleManager()
{
  cLogIndentGuard IndentGuard(0, L"Deleting EventManager");

  m_CmdService->cleanup();
}




// =======================================================================================
// === addCommand ========================================================================
// =======================================================================================

cGuard::ConstPtr QAPI cCmdConsoleManager::addCommand(const iCommand::Ptr & CommandToAdd, const bool IsExecuteInMasterThreadOnly,const bool MakeLowerCaseAllParams)
{
  return m_CmdService->addCommand(CommandToAdd, IsExecuteInMasterThreadOnly,MakeLowerCaseAllParams);
}




// =======================================================================================
// === dispatchMainThreadCommands ========================================================
// =======================================================================================

void QAPI cCmdConsoleManager::dispatchMainThreadCommands()
{
  while (m_CmdService->dispatchWaitingCommand());
}




// =======================================================================================
// === queueCommand ======================================================================
// =======================================================================================

void QAPI cCmdConsoleManager::queueCommand(const String & command, const String & params)
{
  return m_CmdService->queueCommand(command, params);
}
