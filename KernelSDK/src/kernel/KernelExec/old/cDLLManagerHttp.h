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
// --- Pre-Compiled Headers --------------------------------------------------------------

#include "stdafx.h"

// --- Headers ---------------------------------------------------------------------------

#include "cDLLManager.h"

// --- Start of Namespace ----------------------------------------------------------------

using namespace vfs;


// =======================================================================================
// === Http Commands =====================================================================
// =======================================================================================

class cHttpCmd_ModulesLoaded : public iHttpManager::iCommand, public cRefCount
{
  public :
    QDEFINE_SMARTPTR(cHttpCmd_ModulesLoaded);
    cHttpCmd_ModulesLoaded(cDLLManager & Manager) throw() :
      m_DLLManager(Manager)
    {
      // Do nothing.
    }
    ~cHttpCmd_ModulesLoaded() throw()
    {
      // Do nothing.
    }
    cGuid getUniqueID(void) const throw()
    {
      return cGuid(L"{B77B2193-16AA-46C2-8159-37433C36BBEA} // Modules loaded");
    }
    String respondSoap(const ParameterList Parameters)
    {
      return L"";
    }
    String respondHtml(const String & ClientIPAddr, const String & ServerIPAddr, const ParameterList Parameters)
    {
      StringStream Response;
      Response << L"<body>\n";

        Response << L"<h1>Modules Loaded</h1>\n";

        Response << L"<p>Listing all " << (unsigned int)m_DLLManager.m_DLLs.size() << L" modules in load order :</p>\n";

        Response << L"<table style='border: 1 solid #004A94; padding: 5'  border='0' cellspacing='0' cellpadding='0'>\n";

        Response << L"<form name='Test Form' method='get' action'{FBCA4363-5469-4986-B582-B967FE569DB5}'>\n";

        Response << L"<tr>\n";

          Response << L"<td style='padding: 5' valign='top' bgcolor='#004A94' nowrap><p>";
            Response << L"<font color='white'>Developer</font>";
          Response << L"</p></td>\n";

          Response << L"<td style='padding: 5' valign='top' bgcolor='#004A94' nowrap><p>";
            Response << L"<font color='white'>Module</font>";
          Response << L"</p></td>\n";

          Response << L"<td style='padding: 5' valign='top' bgcolor='#004A94' nowrap><p>";
            Response << L"<font color='white'>Load order</font>";
          Response << L"</p></td>\n";

          Response << L"<td style='padding: 5' valign='top' bgcolor='#004A94' nowrap><p>";
            Response << L"<font color='white'>Version</font>";
          Response << L"</p></td>\n";

          Response << L"<td style='padding: 5' valign='top' bgcolor='#004A94' nowrap><p>";
            Response << L"<font color='white'>Status</font>";
          Response << L"</p></td>\n";

        Response << L"</tr>\n";

        cLockGuard MemberGuard(&m_DLLManager.m_DLLsThreadSafety);
        for (multiset<cDLL>::const_iterator j = m_DLLManager.m_DLLs.begin(); j != m_DLLManager.m_DLLs.end(); ++j)
        {
          Response << L"<tr>\n";

            Response << L"<td style='border-right: 1 solid #AAAAAA; padding: 5' valign='top'>";
              Response << j->m_DeveloperName;
            Response << L"</td>\n";

            Response << L"<td style='border-right: 1 solid #AAAAAA; padding: 5' valign='top'>";
              Response << j->m_ModuleName;
            Response << L"</td>\n";

            Response << L"<td style='text-align: Right; border-right: 1 solid #AAAAAA; padding: 5' valign='top'>";
              Response << j->m_LoadOrderGroup;
            Response << L"</td>\n";

            Response << L"<td style='border-right: 1 solid #AAAAAA; padding: 5' valign='top'>";
              Response << j->m_InternalVersion.toString();
            Response << L"</td>\n";

            Response << L"<td valign='top'><p>";
              Response << j->m_Msg;
            Response << L"</td>\n";

          Response << L"</tr>\n";
        }

        Response << L"</form>\n";

        Response << L"</table>\n";

      Response << L"</body>\n";
      return Response.str();
    }
  private :
    cDLLManager & m_DLLManager;
};




// =======================================================================================
// === Http Commands =====================================================================
// =======================================================================================

class cHttpCmd_ClassesToBlock : public iHttpManager::iCommand, public cRefCount
{
  public :
    QDEFINE_SMARTPTR(cHttpCmd_ClassesToBlock);
    cHttpCmd_ClassesToBlock(cDLLManager & Manager) throw() :
      m_DLLManager(Manager)
    {
      // Do nothing.
    }
    ~cHttpCmd_ClassesToBlock() throw()
    {
      // Do nothing.
    }
    cGuid getUniqueID(void) const throw()
    {
      return cGuid(L"{C64C08FF-F4B5-47C7-95BB-4FD91B8DCE81} // Classes to block");
    }
    String respondSoap(const ParameterList Parameters)
    {
      return L"";
    }
    String respondHtml(const String & ClientIPAddr, const String & ServerIPAddr, const ParameterList Parameters)
    {
      StringStream Response;
      Response << L"<body>\n";

        Response << L"<h1>Classes to Block Registering</h1>\n";

        Response << L"<table style='border: 1 solid #004A94; padding: 5'  border='0' cellspacing='0' cellpadding='0'>\n";

        Response << L"<tr>\n";

          Response << L"<td style='padding: 5' valign='top' bgcolor='#004A94' nowrap><p>";
            Response << L"<font color='white'>Name</font>";
          Response << L"</p></td>\n";

          Response << L"<td style='padding: 5' valign='top' bgcolor='#004A94' nowrap><p>";
            Response << L"<font color='white'>Guid</font>";
          Response << L"</p></td>\n";

          Response << L"<td style='padding: 5' valign='top' bgcolor='#004A94' nowrap><p>";
            Response << L"<font color='white'>Derived from</font>";
          Response << L"</p></td>\n";

          Response << L"<td style='padding: 5' valign='top' bgcolor='#004A94' nowrap><p>";
            Response << L"<font color='white'>Total instances</font>";
          Response << L"</p></td>\n";

          Response << L"<td style='padding: 5' valign='top' bgcolor='#004A94' nowrap><p>";
            Response << L"<font color='white'>Is blocked from registering?</font>";
          Response << L"</p></td>\n";

        Response << L"</tr>\n";

        cLockGuard MemberGuard(&m_DLLManager.m_DLLsThreadSafety);
        for (multiset<cDLL>::const_iterator j = m_DLLManager.m_DLLs.begin(); j != m_DLLManager.m_DLLs.end(); ++j)
        {
          Response << L"<tr>\n";

            Response << L"<td style='border-right: 1 solid #AAAAAA; padding: 5' valign='top'>";
            Response << L"tmp";
            Response << L"</td>\n";

            Response << L"<td style='border-right: 1 solid #AAAAAA; padding: 5' valign='top'>";
            Response << L"tmp";
            Response << L"</td>\n";

            Response << L"<td style='border-right: 1 solid #AAAAAA; padding: 5' valign='top'>";
            Response << L"tmp";
            Response << L"</td>\n";

            Response << L"<td style='text-align: Right; border-right: 1 solid #AAAAAA; padding: 5' valign='top'>";
            Response << L"0";
            Response << L"</td>\n";

            Response << L"<td style='text-align: Center' valign='top'>";
              Response << L"<input type='checkbox' name='" << L"classguid" << L"' value='ON' disabled>\n";
            Response << L"</td>\n";

          Response << L"</tr>\n";
        }

        Response << L"</table>\n";

      Response << L"</body>\n";
      return Response.str();
    }
  private :
    cDLLManager & m_DLLManager;
};
