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

// NetManService.cpp : Implementation of WinMain

#include "stdafx.h"
#include "resource.h"
#include "NetManService_h.h"
#include "cNetManServiceStart.h"

using namespace vfs;

class CNetManServiceModule : public CAtlServiceModuleT< CNetManServiceModule, IDS_SERVICENAME >
{
public :
  DECLARE_LIBID(LIBID_NetManServiceLib)
  DECLARE_REGISTRY_APPID_RESOURCEID(IDR_NetManService, "{6C008433-7DDA-444f-9E02-F22868DA5BD0}")

  typedef CAtlServiceModuleT< CNetManServiceModule, IDS_SERVICENAME > base;

  HRESULT InitializeSecurity() throw()
  {
    // TODO : Call CoInitializeSecurity and provide the appropriate security settings for
    // your service
    // Suggested - PKT Level Authentication,
    // Impersonation Level of RPC_C_IMP_LEVEL_IDENTIFY
    // and an appropiate Non NULL Security Descriptor.

    return CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_PKT, RPC_C_IMP_LEVEL_IDENTIFY, NULL, EOAC_NONE, NULL);
  }

  HRESULT PreMessageLoop(int nShowCmd) throw()
  {
    HRESULT hr = base::PreMessageLoop(nShowCmd);
    if (FAILED(hr))
      return hr;

    hr = ERROR_ACCESS_DENIED; //assume bad
    try
    {
      if (m_NetManServiceGuard.isNull())
        m_NetManServiceGuard = (new cNetManServiceStart)->start (GetCommandLine());

      hr = S_OK;
    }
    catch (cRecoverable& reason)
    {
      QSOS((L"Failed to start NetMan service: %s", reason.getMsg().c_str()));
      if (m_bService)
      {
        LogEvent (reason.getMsg().c_str());
      }
      else
      {
        StringStream msg;
        msg << L"(" << reason.getSrcFilename() << L", line " << reason.getSrcLineNumber() << L") " << reason.getMsg();;

        (void)MessageBox (
          NULL,
          msg.str().c_str(),
          L"vfs NetManService",
          MB_ICONEXCLAMATION | MB_TOPMOST | MB_APPLMODAL);
      }
      throw;
    }

    return hr;
  }

  HRESULT PostMessageLoop() throw()
  {
    try
    {
      m_NetManServiceGuard.setNull();
    }
    catch (cRecoverable& reason)
    {
      LogEvent (reason.getMsg().c_str());
    }

    return base::PostMessageLoop();
  }

private:
  cGuard::ConstPtr m_NetManServiceGuard;
};

CNetManServiceModule _AtlModule;


//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/,
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    return _AtlModule.WinMain(nShowCmd);
}
