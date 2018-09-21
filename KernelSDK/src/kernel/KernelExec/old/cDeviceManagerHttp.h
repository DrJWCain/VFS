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

#include "QKernelExec.h"
#include "cDeviceManager.h"

// --- Using Namespace -------------------------------------------------------------------

using namespace vfs;




// =======================================================================================
// === Http Commands =====================================================================
// =======================================================================================

class cHttpCmd_Devices : public iHttpManager::iCommand, public cRefCount
{
  public :

    QDEFINE_SMARTPTR(cHttpCmd_Devices);

    cHttpCmd_Devices(cDeviceManager & Manager) throw() :
      m_DeviceManager(Manager)
    {
      // Do nothing.
    }
    ~cHttpCmd_Devices() throw()
    {
      // Do nothing.
    }
    cGuid getUniqueID(void) const throw()
    {
      return cGuid(L"{CB7F5EA6-3D80-464C-8653-E1D421E08ABA} // Devices");
    }
    String respondSoap(const ParameterList Parameters)
    {
      return L"";
    }
    String respondHtml(const String & ClientIPAddr, const String & ServerIPAddr, const ParameterList Parameters)
    {
      cDeviceManager & dm = m_DeviceManager;

      // Protect member variables from changes by another thread.
      cLockGuard MemberGuard(&dm.m_ThreadSafety);

      StringStream Response;
      Response << L"<body>\n";

        Response << L"<h1>View Devices</h1>\n";

        Response << L"<p>Listing device drivers - " << (unsigned int)dm.m_DevicesByName.size() << L" drivers(s) found :</p>\n";

        if (dm.m_DevicesByName.size() > 0)
        {
          Response << L"<table style='border: 1 solid #004A94; padding: 5'  border='0' cellspacing='0' cellpadding='0'>\n";

            Response << L"<td style='padding: 5' valign='top' bgcolor='#004A94' nowrap><p>";
              Response << L"<font color='white'>Driver</font>";
            Response << L"</p></td>\n";

            Response << L"<td style='padding: 5' valign='top' bgcolor='#004A94' nowrap><p>";
              Response << L"<font color='white'>Device Status</font>";
            Response << L"</p></td>\n";

            for (std::map<String, cDeviceManager::cAboutDevice>::const_iterator j = dm.m_DevicesByName.begin(); j != dm.m_DevicesByName.end(); ++j)
            {
              Response << L"<tr>\n";

                Response << L"<td style='border-right: 1 solid #AAAAAA; padding: 5' valign='top'>";
                Response << j->second.m_Name;
                Response << L"</td>\n";

                Response << L"<td style='border-right: 1 solid #AAAAAA; padding: 5' valign='top'>";
                Response << j->second.m_Comment;
                Response << L"</td>\n";

              Response << L"</tr>\n";
            }

          Response << L"</table>\n";
        }
/*
      Response << L"<p id=iq_comment>";
        Response << L"<u>Information</u><br>\n";
        Response << L"Options in passwords that do no decrypt on this machine are not visible in the above list.\n";
      Response << L"</p>";
*/
      Response << L"</body>\n";
      return Response.str();
    }
  private :
    cDeviceManager & m_DeviceManager;
};
