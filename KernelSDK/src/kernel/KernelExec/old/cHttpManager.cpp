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
#include "cHttpManager.h"
#include "cHttpDriverContext.h"
#include "QKernelExecVersion.h"
#include "LogCategories.h"
#include <algorithm>

using namespace vfs;




// =======================================================================================
// === Http Commands =====================================================================
// =======================================================================================

class cGetIndexCmd : public iHttpManager::iCommand, public cRefCount
{
  public :

    QDEFINE_SMARTPTR(cGetIndexCmd);
    cGetIndexCmd(cHttpManager & Manager) throw() :
      m_Manager(Manager)
    {
      // Do nothing.
    }
    ~cGetIndexCmd() throw()
    {
      // Do nothing.
    }
    cGuid getUniqueID(void) const throw()
    {
      return cGuid(L"{399BEA5A-FAFF-492E-8875-C30C3C6E1036} // get kernel index");
    }
    String respondSoap(const ParameterList Parameters)
    {
      return L"";
    }
    String respondHtml(const String & ClientIPAddr, const String & ServerIPAddr, const ParameterList Parameters)
    {
      StringStream Response;
      Response << L"<body background='../../images/index_bkgnd.gif'>\n";

      if (Parameters.size() > 0)
      {
        Char * End;
        const unsigned int WhichIndex = wcstol(Parameters.begin()->m_Value.c_str(), &End, 10);

        {
          // Protect member variables from changes by another thread.
          cLockGuard MemberGuard(&m_Manager.m_MemberLock);

          // For each category...
          for (cHttpManager::IndexCategoryMap::const_iterator i = m_Manager.m_IndexCategories[WhichIndex].begin();
               i != m_Manager.m_IndexCategories[WhichIndex].end();
               ++i)
          {
            const String &                           CatName = i->first;
            const cHttpManager::cIndexCategoryInfo & CatInfo = i->second;

            Response << L"<b>" << CatName << L"</b>";
//            Response << L"<br>\n";
            Response << L"<ul compact type=square style='margin-left: 20'>\n";

            // ...list each command.
            for (cHttpManager::cIndexCategoryInfo::CommandList::const_iterator j = CatInfo.m_Commands.begin();
                 j != CatInfo.m_Commands.end();
                 ++j)
            {
              const cGuid CmdGuid = (*j)->getUniqueID();
              cHttpManager::IndexCommandMap::const_iterator k = m_Manager.m_IndexCommands.find(CmdGuid);
              QVERIFY(k != m_Manager.m_IndexCommands.end());
              const cHttpManager::cIndexCommandInfo & CmdInfo = k->second;

              Response << L"<li><a target='main' href='" << CmdGuid.serialise(false) << L"?empty'>";
              Response << CmdInfo.m_HyperlinkText;
              Response << L"</a></li><br>\n";
            }

            Response << L"</ul>\n";
          }
        }
      }

      Response << L"</body>\n";

      return Response.str();
    }
  private :
    cHttpManager & m_Manager;
};




// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

cHttpManager::cHttpManager()
{
	cLogIndentGuard IndentGuard(kCtgCreateManagerDetail, L"Creating HttpManager");

  m_GetIndexFrameCmdGuard = addCommand(new cGetIndexCmd(*this));
}

// ---------------------------------------------------------------------------------------

cHttpManager::~cHttpManager() throw()
{
	cLogIndentGuard IndentGuard(0, L"Deleting HttpManager");

  m_GetIndexFrameCmdGuard.invalidate();
}



// =======================================================================================
// === logSummary/logDetailedSummary =====================================================
// =======================================================================================

void QAPI cHttpManager::logSummary(void) throw()
{
}

// ---------------------------------------------------------------------------------------

void QAPI cHttpManager::logDetailedSummary(void) throw()
{
}




// =======================================================================================
// === iniDrivers/cleanupDrivers =========================================================
// =======================================================================================

void cHttpManager::initDrivers(void)
{
  // Protect member variables from changes by another thread.
  cLockGuard MemberGuard(&m_MemberLock);

  std::list<iFactory::iConstructor::Ptr> Constructors = gFactory->getConstructors(iHttpDriver::class_info().getGuid());
  for (std::list<iFactory::iConstructor::Ptr>::iterator i = Constructors.begin(); i != Constructors.end(); ++i)
  {
    QMSG((0, L"Connecting driver \"%s\"", (*i)->getClassInfo()->getName().c_str()));

    String ConnectResultMsg(L"OK");
    iHttpDriver::Ptr        NewDriver   = ptr_cast<iHttpDriver::Ptr>(gFactory->createInstance((*i)->getClassInfo()->getGuid()));
    iHttpDriverContext::Ptr NewContext  = new cHttpDriverContext();
    (void)NewDriver->connectToServer(NewContext, &ConnectResultMsg);
    m_Drivers.push_back(cHttpManager::cDriverInfo(NewDriver, ConnectResultMsg));
  }
}

// ---------------------------------------------------------------------------------------

void cHttpManager::cleanupDrivers(void) throw()
{
  // Protect member variables from changes by another thread.
  cLockGuard MemberGuard(&m_MemberLock);

  for (DriverList::iterator i = m_Drivers.begin(); i != m_Drivers.end(); ++i)
  {
    cDriverInfo & it = *i;
    it.m_Driver->disconnectFromServer();
  }
  m_Drivers.clear();
}




// =======================================================================================
// === addCommand/addIndexCommand ========================================================
// =======================================================================================

cGuard::ConstPtr QAPI cHttpManager::addCommand(iCommand::Ptr NewCommand)
{
  // Protect member variables from changes by another thread.
  cLockGuard MemberGuard(&m_MemberLock);

  // Validate the parameters and throw an exception if one fails...
  // Error if command is already ready added to either map.
  CommandMap::const_iterator      i = m_Commands.find(NewCommand->getUniqueID());
  IndexCommandMap::const_iterator j = m_IndexCommands.find(NewCommand->getUniqueID());
  if (i != m_Commands.end() || j != m_IndexCommands.end())
  {
    QFATAL((__FILE__, __LINE__, L"Http command \"%s\" already exists", NewCommand->getUniqueID().toString().c_str()));
  }
  // ...so we will never get here if one of the above fails.

  // Add command to map.
  m_Commands.insert(std::make_pair(NewCommand->getUniqueID(), NewCommand));

  // When this cCommandGuard object is destroyed it will undo all of the above operations by
  // calling removeCommand() below.
  return new cGuard(this, NewCommand);
}

// ---------------------------------------------------------------------------------------

cGuard::ConstPtr QAPI cHttpManager::addIndexCommand(
  iCommand::Ptr   NewCommand,
  const String &  Category,
  const String &  HyperlinkText,
  const String &  Description)
{
  return addIndexCommand(NewCommand, iHttpManager::ePlugins, Category, HyperlinkText, Description);
}

// ---------------------------------------------------------------------------------------

cGuard::ConstPtr QAPI cHttpManager::addIndexCommand(
  iCommand::Ptr   NewCommand,
  const eIndex    WhichIndex,
  const String &  Category,
  const String &  HyperlinkText,
  const String &  Description)
{
  // Protect member variables from changes by another thread.
  cLockGuard MemberGuard(&m_MemberLock);
  
  // Validate the parameters and throw an exception if one fails...
  if (Category.size() == 0)
  {
    QFATAL((__FILE__, __LINE__, L"Http index command \"%s\" must have a category", NewCommand->getUniqueID().toString().c_str()));
  }
  // Error if command is already ready added to either map.
  CommandMap::const_iterator      i = m_Commands.find(NewCommand->getUniqueID());
  cGuid a(NewCommand->getUniqueID());
  IndexCommandMap::const_iterator j = m_IndexCommands.find(a);
  if (i != m_Commands.end() || j != m_IndexCommands.end())
  {
    QFATAL((__FILE__, __LINE__, L"Http index command \"%s\" already exists", NewCommand->getUniqueID().toString().c_str()));
  }
  // ...so we will never get here if one of the above fails.

  m_IndexCommands.insert(
    std::make_pair(
      NewCommand->getUniqueID(),
      cIndexCommandInfo(NewCommand, Category, HyperlinkText, Description)));

  // Add category if it doesn't already exist.
  std::pair<IndexCategoryMap::iterator, bool> AboutInserted = m_IndexCategories[WhichIndex].insert(std::make_pair(Category, cIndexCategoryInfo()));
  cIndexCategoryInfo &                        CatInfo       = AboutInserted.first->second;
  // Add new command to category if it doesn't already exist.
  CatInfo.m_Commands.push_back(NewCommand);

  // When this cCommandGuard object is destroyed it will undo all of the above operations by
  // calling removeCommand() below.
  return new cGuard(this, NewCommand);
}




// =======================================================================================
// === onGuardDestroyed ==================================================================
// =======================================================================================

void cHttpManager::onGuardDestroyed(iGuarded::Ptr Guarded) throw()
{
  iCommand::ConstPtr CommandToRemove = ptr_cast<iCommand::ConstPtr>(Guarded);

  if (CommandToRemove.isValid())
  {
    // Protect member variables from changes by another thread.
    cLockGuard MemberGuard(&m_MemberLock);

    // Remove command from category.
    IndexCommandMap::iterator CmdInfoIter = m_IndexCommands.find(CommandToRemove->getUniqueID());
    cIndexCommandInfo &       CmdInfo     = CmdInfoIter->second;
    if (CmdInfoIter != m_IndexCommands.end())
    {
      for (unsigned int i = 0; i < NumIndexes; ++i)
      {
        IndexCategoryMap::iterator CatInfoIter = m_IndexCategories[i].find(CmdInfo.m_Category);
        if (CatInfoIter != m_IndexCategories[i].end())
        {
          cIndexCategoryInfo & CatInfo = CatInfoIter->second;
          CatInfo.m_Commands.remove(CmdInfo.m_Command);
        }
      }
    }

    // Remove command from command map (if it is there).
    m_Commands.erase(CommandToRemove->getUniqueID());

    // Remove command from index command map (if it is there).
    m_IndexCommands.erase(CommandToRemove->getUniqueID());
  }
}




// =======================================================================================
// === parseURLParameters ================================================================
// =======================================================================================

void cHttpManager::parseURLParameters(
  const Char * const                            ParameterStr,
  iHttpManager::iCommand::ParameterList * const List) throw()
{
  String CopyOfParameterStr = String(L"&") + String(ParameterStr);

  // Establish string and get the first token.
  Char * Name = wcstok((Char *)CopyOfParameterStr.c_str(), L"&");
  while(Name != NULL)
  {
    Char * const Equals = wcschr(Name, L'=');
    if (Equals)
    {
      *Equals = L'\0';
      const Char * const Value  = Equals + 1;

      String LowerName(Name);
      transform(LowerName.begin(), LowerName.end(), LowerName.begin(), tolower);
      List->push_back(
        iHttpManager::iCommand::cParameter(
          decodeStringFromURL(LowerName),
          decodeStringFromURL(widen(Value))));
    }

    // Get next token.
    Name = wcstok(NULL, L"&");
  }
}




// =======================================================================================
// === getHtmlResponse ===================================================================
// =======================================================================================

String QAPI cHttpManager::getHtmlResponse(
  const String &      CmdStr,
  const char * const  ParameterBuffer)
{
  StringStream                Response;
  iHttpManager::iCommand::Ptr Cmd;

  Response << L"<html>\n";

  Response << L"<head>\n";
  Response << L"<meta http-equiv='pragma' content='no-cache'>\n";
  Response << L"<meta http-equiv='expires' content='-1'>\n";
  Response << L"<title>" << CmdStr << L"</title>\n";
  Response << L"<link rel='stylesheet' type='text/css' href='../../style.css'>\n";
  Response << L"</head>\n";

  {
    // Protect member variables from changes by another thread.
    cLockGuard MemberGuard(&m_MemberLock);

    try
    {
      const cGuid CmdUniqueID(decodeStringFromURL(CmdStr));

      CommandMap::const_iterator      i = m_Commands.find(CmdUniqueID);
      IndexCommandMap::const_iterator j = m_IndexCommands.find(CmdUniqueID);
      if (i == m_Commands.end() && j == m_IndexCommands.end())
      {
        Response << L"<body>Command not found</body>";
      }
      else
      {
        Cmd = i != m_Commands.end() ? i->second : j->second.m_Command;
      }
    }
    catch (cBadFormat &)
    {
      Response << L"<body>Command must be a guid</body>";
    }
  }

  if (Cmd.isValid())
  {
    iHttpManager::iCommand::ParameterList Parameters;
    parseURLParameters(widen(ParameterBuffer).c_str(), &Parameters);

    String ClientIPAddr, ServerIPAddr;
    {
      iHttpManager::iCommand::ParameterList::iterator i = Parameters.begin();
      QVERIFY(i->m_Name == L"cltip");
      ClientIPAddr = i->m_Value;
      Parameters.erase(i);
    }
    {
      iHttpManager::iCommand::ParameterList::iterator i = Parameters.begin();
      QVERIFY(i->m_Name == L"svrip");
      ServerIPAddr = i->m_Value;
      Parameters.erase(i);
    }

    Response << Cmd->respondHtml(ClientIPAddr, ServerIPAddr, Parameters);
  }

  // Work around IE cache bug
  Response << L"<head>\n";
  Response << L"<meta http-equiv='pragma' content='no-cache'>\n";
  Response << L"<meta http-equiv='expires' content='-1'>\n";
  Response << L"</head>\n";

  Response << L"</html>\n";

  return Response.str();
}




// =======================================================================================
// === getSoapResponse ===================================================================
// =======================================================================================

String QAPI cHttpManager::getSoapResponse(
  const String &      CmdStr,
  const Char * const  ParameterBuffer)
{
  StringStream                Response;
  iHttpManager::iCommand::Ptr Cmd;

  {
    // Protect member variables from changes by another thread.
    cLockGuard MemberGuard(&m_MemberLock);

    try
    {
      const cGuid CmdUniqueID(decodeStringFromURL(CmdStr));

      CommandMap::const_iterator      i = m_Commands.find(CmdUniqueID);
      IndexCommandMap::const_iterator j = m_IndexCommands.find(CmdUniqueID);
      if (i == m_Commands.end() && j == m_IndexCommands.end())
      {
        Response << L"Command not found";
      }
      else
      {
        Cmd = i != m_Commands.end() ? i->second : j->second.m_Command;
      }
    }
    catch (cBadFormat &)
    {
      Response << L"Command must be a guid";
    }
  }

  if (Cmd.isValid())
  {
    iHttpManager::iCommand::ParameterList Parameters;
    parseURLParameters(ParameterBuffer, &Parameters);

    Response << Cmd->respondSoap(Parameters);
  }

  return Response.str();
}




// =======================================================================================
// === encodeStringForURL/decodeStringFromURL ============================================
// =======================================================================================

String cHttpManager::encodeStringForURL(const String & Src) const throw()
{
  String Result;
  Result.reserve(Src.size());
  for (String::const_iterator i = Src.begin(); i != Src.end(); i++)
  {
    if (*i == L' ')
    {
      Result += L"%20";
    }
    else
    {
      Result += *i;
    }
  }

  return Result;
}

// ---------------------------------------------------------------------------------------

String cHttpManager::decodeStringFromURL(const String & Src) const throw()
{
  String Result;
  Result.reserve(Src.size());
  for (String::const_iterator i = Src.begin(); i != Src.end(); i++)
  {
    if (*i == L'%')
    {
      Char * Stop;
      String Number;
      Number += *(i+1);
      Number += *(i+2);
      std::string LowerNumber;
      LowerNumber += (char)wcstoul(Number.c_str(), &Stop, 16);
      Result += widen(LowerNumber);
      i += 2;
    }
    else if (*i == L'+')
    {
      Result += L' ';
    }
    else
    {
      Result += *i;
    }
  }

  return Result;
}




// =======================================================================================
// === logDrivers ========================================================================
// =======================================================================================

void QAPI cHttpManager::logDrivers(void) throw()
{
  // Protect member variables from changes by another thread.
  cLockGuard MemberGuard(&m_MemberLock);

  if (m_Drivers.size() > 0)
  {
    QMSG((0, L"%-30s %s", L"Driver Name", L"Connect Result"));
    QMSG((0, L""));

    for (DriverList::iterator i = m_Drivers.begin(); i != m_Drivers.end(); ++i)
    {
      cDriverInfo & it = *i;

      QMSG((0, L"%-30s %s",
        it.m_Driver->getClassInfo()->getName().c_str(),
        it.m_ConnectResultMsg.c_str()));
    }
  }
  else
  {
      QMSG((0, L"No drivers"));
  }
}