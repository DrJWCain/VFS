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

//{{{  headers
#include "stdafx.h"
#include "cCmdConsoleHandler.h"

#include "cCmdConsoleManager.h"
#include "LogCategories.h"
#include <algorithm>
//}}}

using namespace std;
using namespace vfs;

namespace
{
  const int kBufferSize = 4096;
}

//----------------------------------------------------------------------------------------------------------------------
//{{{
cCmdConsoleHandler::cCmdConsoleHandler()
  : cBasicThread(L"CmdConsole", iBasicThread::ePriority_TimeCritical, 0x400000, eThreadLogging_Collapsed),
      // request 4MB stack for cmdconsole thread to avoid crash storing recovered clips
    StopLoopFlag(false)
{
}
//}}}
//{{{
cCmdConsoleHandler::~cCmdConsoleHandler()
{
}
//}}}
//
//{{{
void QAPI cCmdConsoleHandler::cleanup()
{
  {
    cLockGuard guard(&CommandQueueLock);
    if (!CommandQueue.empty())
    {
      list<tConsoleEventEntry*>::iterator i = CommandQueue.begin();
      while(i != CommandQueue.end())
      {
        tConsoleEventEntry* entry = *i;
        i = CommandQueue.erase(i);
        delete entry;
      }
    }
  }

  signalLoopStop();

  CommandHandlerList.clear();
}
//}}}
//{{{
void QAPI cCmdConsoleHandler::logHelp(const String Params)
{
  cLockGuard guard(&CommandHandlerLock);

  if (Params == L"")
  {
    QMSG((0, L"help"));
    QMSG((0, L"[command]"));
    QMSG((0, L"Show list of available commands or detailed information on one command."));

    for (tCommandHandlersMap::iterator i = CommandHandlerList.begin(); i != CommandHandlerList.end(); ++i)
    {
      QMSG((0, L""));
      QMSG((0, L"%s", i->first.c_str()));
      iCmdConsoleManager::iCommand::ConstPtr Handler = i->second->handler;
      Handler->logOneLineParams();
      Handler->logOneLineDescription();
    }
  }
  else
  {
    tCommandHandlersMap::iterator i = CommandHandlerList.find(Params);
    if (i == CommandHandlerList.end())
    {
      QMSG((0, L"\"%s\" not found", Params.c_str()));
    }
    else
    {
      iCmdConsoleManager::iCommand::ConstPtr Handler = i->second->handler;
      Handler->logDetailedDescription();
      QMSG((0, L""));
      Handler->logOneLineParams();
      QMSG((0, L""));
      Handler->logDetailedParams();
    }
  }
}
//}}}
//{{{
void QAPI cCmdConsoleHandler::queueCommand(const String NewCmd, const String NewParams)
{
  String LowNewCmd(NewCmd);
  String LowNewParams(NewParams);

  transform(LowNewCmd.begin(),       LowNewCmd.end(),  LowNewCmd.begin(),     tolower);
  transform(LowNewParams.begin(), LowNewParams.end(),  LowNewParams.begin(),  tolower);

  if (NewCmd == L"help")
  {
    cLogIndentGuard IndentGuard(kCtgCmdOutput, L"Command console (async) \"%s %s\"", LowNewCmd.c_str(), LowNewParams.c_str());
    logHelp(NewParams);
  }
  else
  {
    if (iThreadManager::singleton().isMainThread())
    {
      //dispatch console commands here
      iCmdConsoleManager::iCommand::ConstPtr handler;
      bool MakeLowerCaseAllParams = true;
      {
        cLockGuard guard(&CommandHandlerLock);
        tCommandHandlersMap::iterator it = CommandHandlerList.find(LowNewCmd);
        if (it == CommandHandlerList.end())
        {
          QMSG((0, L"\"%s\" not found", LowNewCmd.c_str()));
        }
        else
        {
          handler = it->second->handler;
          MakeLowerCaseAllParams = it->second->makeLowerCaseAllParams;
        }
      }
      if(handler.isValid())
      {
        String params(MakeLowerCaseAllParams ? LowNewParams : NewParams);
        cLogIndentGuard IndentGuard(kCtgCmdOutput, L"Command Console \"%s %s\"", LowNewCmd.c_str(), params.c_str());
        handler->execute (params);
      }
    }
    else
    {
      iCmdConsoleManager::iCommand::ConstPtr handler;
      bool MakeLowerCaseAllParams = true;
      {
        cLockGuard guard(&CommandHandlerLock);
        tCommandHandlersMap::iterator it = CommandHandlerList.find(LowNewCmd);
        if (it == CommandHandlerList.end())
        {
          QMSG((0, L"\"%s\" not found", LowNewCmd.c_str()));
        }
        else
        {
          if(!it->second->masterThreadOnly)
            handler = it->second->handler;
          MakeLowerCaseAllParams = it->second->makeLowerCaseAllParams;
        }
      }
      if(handler.isValid())
      {
        String params(MakeLowerCaseAllParams ? LowNewParams : NewParams);
        cLogIndentGuard IndentGuard(kCtgCmdOutput, L"Command console (async) \"%s %s\"", LowNewCmd.c_str(), params.c_str());
        handler->execute (params);
        return;
      }

      tConsoleEventEntry* entry = new tConsoleEventEntry;
      entry->dest    = LowNewCmd;
      if(MakeLowerCaseAllParams)
        entry->command = LowNewParams;
      else
        entry->command = NewParams;

      {
        cLockGuard guard(&CommandQueueLock);
        CommandQueue.push_back(entry);
      }
    }
  }
}
//}}}
//{{{
cGuard::ConstPtr QAPI cCmdConsoleHandler::addCommand(
  iCmdConsoleManager::iCommand::Ptr CommandToAdd,
  const bool                   IsExecuteInMasterThreadOnly,
  const bool                   MakeLowerCaseAllParams)
{
  String LowerCaseName(CommandToAdd->getName());
  if(MakeLowerCaseAllParams)
  {
    std::transform(LowerCaseName.begin(), LowerCaseName.end(), LowerCaseName.begin(), tolower);
  }

  tCmdHandlerEntry::Ptr entry = new tCmdHandlerEntry;
  entry->handler          = CommandToAdd;
  entry->masterThreadOnly = IsExecuteInMasterThreadOnly;
  entry->makeLowerCaseAllParams = MakeLowerCaseAllParams;
  {
    cLockGuard guard(&CommandHandlerLock);
    CommandHandlerList.insert(tCommandHandlersMap::value_type(LowerCaseName, entry));
  }

  return new cGuard(this, CommandToAdd);
}
//}}}
//{{{
void QAPI cCmdConsoleHandler::onGuardDestroyed(iGuarded::Ptr Guarded)
{
  iCmdConsoleManager::iCommand::ConstPtr CommandToRemove = ptr_cast<iCmdConsoleManager::iCommand::ConstPtr>(Guarded);
  if (CommandToRemove.isValid())
  {
    String LowerCaseName(CommandToRemove->getName());
    std::transform(LowerCaseName.begin(), LowerCaseName.end(), LowerCaseName.begin(), tolower);

    cLockGuard guard(&CommandHandlerLock);
    CommandHandlerList.erase(LowerCaseName);
  }
}
//}}}
//{{{
bool QAPI cCmdConsoleHandler::dispatchWaitingCommand()
{
  bool ret = false;
  tConsoleEventEntry* entry = 0;
  {
    cLockGuard guard(&CommandQueueLock);
    if(!CommandQueue.empty())
    {
      entry = CommandQueue.front();
      CommandQueue.pop_front();
    }
  }

  if(entry)
  {
    queueCommand(entry->dest,entry->command);
    delete entry;
  }

  {
    cLockGuard guard(&CommandQueueLock);
    if(!CommandQueue.empty())ret = true;
  }

  return ret;
}
//}}}
//{{{
void QAPI cCmdConsoleHandler::signalLoopStop()
{
  StopLoopFlag = true;
  SetEvent(ReadyEvent);
}
//}}}
//{{{
void QAPI cCmdConsoleHandler::runLoop()
{
  HANDLE SharedFile;
  LPVOID SharedMem;

  SECURITY_ATTRIBUTES sa;
  SECURITY_DESCRIPTOR sd;

  sa.nLength = sizeof(SECURITY_ATTRIBUTES);
  sa.bInheritHandle = TRUE;
  sa.lpSecurityDescriptor = &sd;

  if(!InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION))
  {
    QFATAL((__FILE__,__LINE__,L"unable to InitializeSecurityDescriptor"));
  }

  if(!SetSecurityDescriptorDacl(&sd, TRUE, (PACL)NULL, FALSE))
  {
    QFATAL((__FILE__,__LINE__,L"unable to SetSecurityDescriptorDacl"));
  }

  // Fix for GENQ-9125: someone appears to be abusing our original event, so we have to create a differently named one
  ReadyEvent = CreateEventA(&sa, FALSE, FALSE, "MONTY_DATA_READY_2");

  if (!ReadyEvent)
  {
    QFATAL((__FILE__,__LINE__,L"Unable to create synchronization object"));
  }

  if (!ReadyEvent)
  {
    QFATAL((__FILE__,__LINE__,L"Unable to create synchronization object"));
  }

  SharedFile = CreateFileMappingA(
                      (HANDLE)-1,
                      &sa,
                      PAGE_READWRITE,
                      0,
                      kBufferSize,
                      "MONTY_BUFFER");

  if (!SharedFile)
  {
    QFATAL((__FILE__,__LINE__,L"Unable to create file mapping object"));
  }

  SharedMem = MapViewOfFile(
                      SharedFile,
                      FILE_MAP_READ,
                      0,
                      0,
                      0);

  if (!SharedMem)
  {
    QFATAL((__FILE__,__LINE__,L"Unable to map shared memory"));
  }

  for(;;)
  {
    Char buffer[kBufferSize/2];
    WaitForSingleObject(ReadyEvent,INFINITE);
    if(StopLoopFlag)
    {
      break;
    }
    memcpy((char*)buffer,SharedMem,kBufferSize);
    String dest;

    Char* token = wcstok(buffer,L" \t");//first word
    if(token)
    {
      dest = token;
      token = wcstok(0,L"");//rest of line
      String command;
      if(token)command = token;
      queueCommand(dest,command);
    }
    //ResetEvent(ReadyEvent); // SimonKe: not if auto-reset
  }

  UnmapViewOfFile(SharedMem);
  CloseHandle(SharedFile);
  CloseHandle(ReadyEvent);
}
//}}}
