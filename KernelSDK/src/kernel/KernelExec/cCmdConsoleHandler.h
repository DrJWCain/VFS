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

#include "QKernelExec.h"

using namespace vfs;

// =======================================================================================
// === cCmdConsoleHandler ================================================================
// =======================================================================================

class cCmdConsoleHandler : public cBasicThread, public iGuardCreator
{
  public:

    QDEFINE_SMARTPTR(cCmdConsoleHandler);

    cCmdConsoleHandler();
    ~cCmdConsoleHandler();
    void QAPI cleanup() throw();

    //{{{
    struct tConsoleEventEntry // Command queue thread safe types and members.
    {
      String dest;
      String command;
    };
    //}}}

    void QAPI queueCommand(const String Dest, const String NewCommand);
    cGuard::ConstPtr QAPI addCommand(iCmdConsoleManager::iCommand::Ptr CommandToAdd, const bool IsExecuteInMasterThreadOnly,const bool MakeLowerCaseAllParams);
    bool QAPI dispatchWaitingCommand();

  private:
    cGuard::ConstPtr m_HttpCmdGuard;

    //{{{
    struct tCmdHandlerEntry : public cRefCount
    {
      QDEFINE_SMARTPTR(tCmdHandlerEntry);

      iCmdConsoleManager::iCommand::ConstPtr handler;
      bool masterThreadOnly;
      bool makeLowerCaseAllParams;
    };
    //}}}

    typedef std::map<String,tCmdHandlerEntry::Ptr> tCommandHandlersMap;
    tCommandHandlersMap CommandHandlerList;
    cLock CommandHandlerLock;

    bool StopLoopFlag;
    HANDLE ReadyEvent;

    //ALL access to EventQueue must be guarged by EventQueueLock;
    std::list<tConsoleEventEntry*> CommandQueue;
    cLock CommandQueueLock;

    void QAPI signalLoopStop() throw();
    void QAPI runLoop();
    void QAPI logHelp (const String Params);

    void QAPI onGuardDestroyed (iGuarded::Ptr Guarded) throw();
};
