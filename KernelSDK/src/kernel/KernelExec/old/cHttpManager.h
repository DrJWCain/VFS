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

#include <map>
#include <list>
#include "QKernelExec.h"

// --- Use Namespace ---------------------------------------------------------------------

using namespace std;
using namespace vfs;




// =======================================================================================
// === cHttpManager ======================================================================
// =======================================================================================

const unsigned int NumIndexes = iHttpManager::ePlugins + 1;

class cHttpManager : public iHttpManager, public iGuardCreator, public cRefCount
{
  friend class cCommandGuard;
  friend class cHttpDriverEnumerator;

	public:

    QDEFINE_SINGLETON_IMPL(cHttpManager, L"{106043C3-8053-4F34-8BB7-93CB7330DC3D} // Http manager impl", iHttpManager);

		cHttpManager();
		~cHttpManager() throw();

    // iManager
    void QAPI logSummary(void) throw();
    void QAPI logDetailedSummary(void) throw();

    cGuard::ConstPtr QAPI addCommand(iCommand::Ptr CommandToAdd);
    String           QAPI getHtmlResponse(const String & Command, const char * const Parameters);
    String           QAPI getSoapResponse(const String & Command, const Char * const Parameters);

    cGuard::ConstPtr QAPI addIndexCommand(
      iCommand::Ptr   CommandToAdd,
      const String &  Category,
      const String &  HyperlinkText,
      const String &  Description);

    cGuard::ConstPtr QAPI addIndexCommand(
      iCommand::Ptr   CommandToAdd,
      const eIndex    WhichIndex,
      const String &  Category,
      const String &  HyperlinkText,
      const String &  Description);

		void QAPI logDrivers(void) throw();

    struct cDriverInfo
    {
      cDriverInfo(iHttpDriver::Ptr Driver, const String & ConnectResultMsg) :
        m_Driver(Driver),
        m_ConnectResultMsg(ConnectResultMsg)
      {
        // Do nothing.
      }

      iHttpDriver::Ptr m_Driver;
      String           m_ConnectResultMsg;
    };

    // To protect member variables from changes by another thread.
    mutable cLock                         m_MemberLock;
 
    typedef std::list<cDriverInfo> DriverList;
    DriverList                            m_Drivers;

    cGuard::ConstPtr m_GetIndexFrameCmdGuard;

    class cIndexCommandInfo
    {
      public :
        cIndexCommandInfo(
          iCommand::Ptr   Command,
          const String &  Category,
          const String &  HyperlinkText,
          const String &  Description) :
            m_Command(Command),
            m_Category(Category),
            m_HyperlinkText(HyperlinkText),
            m_Description(Description)
        {
          // Do nothing.
        }
        iCommand::Ptr   m_Command;
        String          m_Category;
        String          m_HyperlinkText;
        String          m_Description;
    };

    class cIndexCategoryInfo
    {
      public :
        typedef std::list<iCommand::Ptr> CommandList;
        CommandList     m_Commands;
    };

    typedef std::map<cGuid, iCommand::Ptr> CommandMap;
    CommandMap                            m_Commands;

    typedef std::map<cGuid, cIndexCommandInfo> IndexCommandMap;
    IndexCommandMap                       m_IndexCommands;

    typedef std::map<String, cIndexCategoryInfo> IndexCategoryMap;
    IndexCategoryMap                      m_IndexCategories[NumIndexes];

		void initDrivers(void);
		void cleanupDrivers(void) throw();

    void parseURLParameters(
      const Char * const                            ParameterStr,
      iHttpManager::iCommand::ParameterList * const List) throw();

    void parseMIMEParameters(
      const char * const                            ParameterBuffer,
      iHttpManager::iCommand::ParameterList * const List) throw();

    String encodeStringForURL(const String & Src) const throw();
    String decodeStringFromURL(const String & Src) const throw();

    // iGuardCreator
    void QAPI onGuardDestroyed(iGuarded::Ptr Guarded) throw();
};
