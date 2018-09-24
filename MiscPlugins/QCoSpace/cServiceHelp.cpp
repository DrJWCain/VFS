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

#include "stdafx.h"
#include "cServiceHelp.h"
#include "LogCategories.h"
#include <QWinBase.h>
#include <Aclapi.h>

using namespace vfs;
//{{{
struct cServiceHandle {
public:
  SC_HANDLE mHandle;
  cServiceHandle(SC_HANDLE handle) : mHandle(handle) {}
  ~cServiceHandle() { ::CloseServiceHandle(mHandle); }
  operator SC_HANDLE () { return mHandle; }
};
//}}}

//{{{
bool isServiceRegistered (const String& serviceName)
{
  bool isRegistered = false;

  cServiceHandle hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
  if (hSCM == NULL)
    throw cNotAvailable(__FILE__, __LINE__, L"Failed to open service manager for query");

  cServiceHandle hService = ::OpenService(hSCM, serviceName.c_str(), SERVICE_QUERY_CONFIG);
  if (hService != NULL)
    isRegistered = true;

  QMSG((0, L"%s service %s registered", serviceName.c_str(), isRegistered?L"is":L"is not"));

  return isRegistered;
}
//}}}

//{{{
String getServicePath (const String& serviceName)
{
  String servicePath;

  cServiceHandle hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
  if (hSCM == NULL)
    throw cNotAvailable(__FILE__, __LINE__, L"Failed to open service manager for query");

  cServiceHandle hService = ::OpenService(hSCM, serviceName.c_str(), SERVICE_QUERY_CONFIG);
  if (hService == NULL)
  {
    throw cNotAvailable(__FILE__, __LINE__, L"%s is not registered with Service Manager", serviceName.c_str());
  }

  // find the path
  LPQUERY_SERVICE_CONFIG ssConfig = 0;
  DWORD bytesRequired;
  QueryServiceConfig(hService, ssConfig, 0, &bytesRequired);
  if (ERROR_INSUFFICIENT_BUFFER == GetLastError())
  {
    ssConfig = (LPQUERY_SERVICE_CONFIG)new unsigned char[bytesRequired];
    BOOL configOK = QueryServiceConfig(hService, ssConfig, bytesRequired, &bytesRequired);
    if (configOK)
    {
      const String fullServicePath = ssConfig->lpBinaryPathName;
      const String::size_type firstQuotePos = fullServicePath.find_first_not_of (L"\"");
      const String::size_type lastSlashPos = fullServicePath.find_last_of (L'\\');
      //{{{
      if ((firstQuotePos == String::npos) || (lastSlashPos == String::npos))
      {
        throw cBadFormat(__FILE__, __LINE__, L"Bad path for %s service: \"%s\"", serviceName.c_str(), fullServicePath.c_str());
      }
      //}}}
      servicePath = (fullServicePath.substr (firstQuotePos, lastSlashPos-firstQuotePos+1));  // skip leading quote, include trailing slash
    }
    delete[] ssConfig;
  }

  if (servicePath.empty())
    throw cNotAvailable(__FILE__, __LINE__, L"Failed to read %s service path from Service Manager", serviceName.c_str());

  return servicePath;
}
//}}}

//{{{
bool isServiceRunning (const String& serviceName)
{
  bool isRunning = false;

  cServiceHandle hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
  if (hSCM == NULL)
    throw cNotAvailable(__FILE__, __LINE__, L"Failed to open service manager for query");

  cServiceHandle hService = ::OpenService(hSCM, serviceName.c_str(), SERVICE_QUERY_STATUS);
  if (hService == NULL)
  {
    throw cNotAvailable(__FILE__, __LINE__, L"%s is not registered with Service Manager", serviceName.c_str());
  }

  // check whether it is running
  SERVICE_STATUS ssStatus;
  BOOL statusOK = QueryServiceStatus(hService, &ssStatus);
  if (statusOK)
  {
    if (ssStatus.dwCurrentState == SERVICE_RUNNING)
    {
      isRunning = true;
    }
  }

  if (!statusOK)
    throw cNotAvailable(__FILE__, __LINE__, L"Failed to query status of %s service", serviceName.c_str());

  QMSG((0, L"%s service %s running", serviceName.c_str(), isRunning?L"is":L"is not"));

  return isRunning;
}
//}}}

//{{{
void installService (iModuleInstallHelp::Ptr Help, const String& serviceName,
                     const String& servicePath, const String& serviceFileName,
                     const String& serviceDescription)
{
  cLogIndentGuard indent(0, L"Installing %s service at path %s", serviceName.c_str(), servicePath.c_str());
  if (!isServiceRegistered (serviceName))
  {
    QMSG((0, L"Running %s service install", serviceName.c_str()));
    try
    {
      Help->executeProcessWait (servicePath, serviceFileName, L"/install", 4000 /* timeout */, 0 /* flags */);
    }
    catch (cTimedOut &)
    {
      // Must have started ok
    }
    catch (cWin32Error &)
    {
      throw cNotAvailable(__FILE__, __LINE__, L"Failed to register %s with service manager", serviceName.c_str());
    }

    cServiceHandle hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (hSCM == NULL)
      throw cNotAvailable(__FILE__, __LINE__, L"Failed to open service manager for config check");

    cServiceHandle hService = ::OpenService(hSCM, serviceName.c_str(), SERVICE_CHANGE_CONFIG | READ_CONTROL | WRITE_DAC);
    if (hService == NULL)
    {
      throw cNotAvailable(__FILE__, __LINE__, L"Failed to open %s for start, error %d", serviceName.c_str(), GetLastError());
    }

    /*
    if (::ChangeServiceConfig (hService,
                               SERVICE_NO_CHANGE,       // Service Type
                               SERVICE_NO_CHANGE,       // Start Type
                               SERVICE_NO_CHANGE,       // Error Control
                               NULL,                    // Binary Path Name
                               NULL,                    // Load Order Group
                               NULL,                    // TagID
                               L"",                     // Dependencies
                               NULL,                    // Service Start Name
                               L"",                     // Password
                               NULL                     // Display Name
                               ) == 0)
    {
      throw cNotAvailable(__FILE__, __LINE__, L"Failed to set %s to automatic start", serviceName.c_str());
    }
    */

    QMSG((0, L"Setting %s description", serviceName.c_str()));
    SERVICE_DESCRIPTION desc;
    desc.lpDescription = const_cast<LPWSTR>(serviceDescription.c_str());
    if (::ChangeServiceConfig2 (hService, SERVICE_CONFIG_DESCRIPTION, &desc) == 0)
    {
      throw cNotAvailable(__FILE__, __LINE__, L"Failed to set description for %s", serviceName.c_str());
    }

    // Need to set the object security as the default will only allow access by administrator
    //
    QMSG((0, L"Setting %s security", serviceName.c_str()));

    // Get the current security descriptor.
    PSECURITY_DESCRIPTOR psd(NULL);
    DWORD dwBytesNeeded(0);
    cPtr<cMemory> pSecurityDesc;
    if(!QueryServiceObjectSecurity(hService, DACL_SECURITY_INFORMATION, &psd, 0, &dwBytesNeeded))
    {
      if(GetLastError() == ERROR_INSUFFICIENT_BUFFER)
      {
        pSecurityDesc = new cMemory((size_t)dwBytesNeeded, cMemory::eVirtual); 
        psd = (PSECURITY_DESCRIPTOR)pSecurityDesc->getBytes();
 
        if(!QueryServiceObjectSecurity(hService, DACL_SECURITY_INFORMATION, psd, pSecurityDesc->getSize(), &dwBytesNeeded))
        {
          throw cNotAvailable(__FILE__, __LINE__, L"QueryServiceObjectSecurity() failed for %s, error %d", serviceName.c_str(), GetLastError());
        }
      }
      else 
      {
          throw cNotAvailable(__FILE__, __LINE__, L"QueryServiceObjectSecurity() failed for %s, error %d", serviceName.c_str(), GetLastError());
      }
    }

    // Get the current DACL
    BOOL bDaclPresent(FALSE);
    PACL pacl(NULL);
    BOOL bDaclDefaulted(FALSE);
    if(!GetSecurityDescriptorDacl(psd, &bDaclPresent, &pacl, &bDaclDefaulted))
    {
      throw cNotAvailable(__FILE__, __LINE__, L"GetSecurityDescriptorDacl() failed for %s, error %d", serviceName.c_str(), GetLastError());
    }

    // Build the ACE for "EVERYONE"
    EXPLICIT_ACCESS ea;
    PACL pNewAcl(NULL);
    BuildExplicitAccessWithName(&ea, TEXT("EVERYONE"), SERVICE_ALL_ACCESS, SET_ACCESS, NO_INHERITANCE);
    DWORD dwError = SetEntriesInAcl(1, &ea, pacl, &pNewAcl);
    if(dwError != ERROR_SUCCESS)
    {
      throw cNotAvailable(__FILE__, __LINE__, L"SetEntriesInAcl() failed for %s, error %d", serviceName.c_str(), GetLastError());
    }

    // Initialize a new security descriptor
    SECURITY_DESCRIPTOR  sd;
    if(!InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION))
    {
      throw cNotAvailable(__FILE__, __LINE__, L"InitializeSecurityDescriptor() failed for %s, error %d", serviceName.c_str(), GetLastError());
    }

    // Set the new DACL in the security descriptor
    if(!SetSecurityDescriptorDacl(&sd, TRUE, pNewAcl, FALSE))
    {
      throw cNotAvailable(__FILE__, __LINE__, L"SetSecurityDescriptorDacl() failed for %s, error %d", serviceName.c_str(), GetLastError());
    }

    // Set the new DACL for the service object
    if(!SetServiceObjectSecurity(hService, DACL_SECURITY_INFORMATION, &sd))
    {
      throw cNotAvailable(__FILE__, __LINE__, L"SetServiceObjectSecurity() failed for %s, error %d", serviceName.c_str(), GetLastError());
    }

    CloseServiceHandle(hSCM);
    CloseServiceHandle(hService);
    if(pNewAcl) LocalFree((HLOCAL)pNewAcl);

    QMSG((0, L"%s service installed OK", serviceName.c_str()));
  }
}
//}}}

//{{{
void startService (const String& serviceName)
{
  if (!isServiceRunning (serviceName))
  {
    QMSG((0, L"Starting %s service", serviceName.c_str()));

    cServiceHandle hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (hSCM == NULL)
      throw cNotAvailable(__FILE__, __LINE__, L"Failed to open service manager");

    cServiceHandle hService = ::OpenService(hSCM, serviceName.c_str(), SERVICE_START);
    if (hService == NULL)
    {
      throw cNotAvailable(__FILE__, __LINE__, L"Failed to open %s for start, error %d", serviceName.c_str(), GetLastError());
    }

    BOOL startOK = ::StartService(hService, 0, NULL);

    if (!startOK)
      throw cNotAvailable(__FILE__, __LINE__, L"Failed to start %s", serviceName.c_str());

    QMSG((0, L"%s service started OK", serviceName.c_str()));
  }
}
//}}}

//{{{
void removeService (iModuleInstallHelp::Ptr Help, const String& serviceName,
                    const String& servicePath, const String& serviceFileName)
{
  cLogIndentGuard indent(0, L"Removing %s service at path %s", serviceName.c_str(), servicePath.c_str());

  if (isServiceRunning (serviceName))
  {
    // send a stop control to service
    cServiceHandle hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (hSCM == NULL)
      throw cNotAvailable(__FILE__, __LINE__, L"Failed to open service manager for remove");

    cServiceHandle hService = ::OpenService(hSCM, serviceName.c_str(), SERVICE_STOP | SERVICE_QUERY_STATUS);
    if (hService != NULL)
    {
      QMSG((0, L"Stopping %s service", serviceName.c_str()));
      SERVICE_STATUS ssStatus;
      BOOL stopOK = ::ControlService(hService, SERVICE_CONTROL_STOP, &ssStatus);

      if (stopOK)
      {
        // wait for stop...
        while (ssStatus.dwCurrentState == SERVICE_STOP_PENDING)
        {
          Sleep (200);
          if (!QueryServiceStatus(hService, &ssStatus))
            throw cNotAvailable(__FILE__, __LINE__, L"Failed to query status of %s for stop", serviceName.c_str());
        }
      }

      if (!stopOK)
        throw cNotAvailable(__FILE__, __LINE__, L"Failed to stop %s", serviceName.c_str());
    }
  }

  QMSG((0, L"Unregistering %s service", serviceName.c_str()));

  try
  {
    Help->executeProcessWait (servicePath, serviceFileName, L"/remove", 4000 /* timeout */, 0 /* flags */);
  }
  catch (cRecoverable & Reason)
  {
    throw cNotAvailable(__FILE__, __LINE__, L"Failed to remove %s from service manager - %s", serviceName.c_str(), Reason.getMsg().c_str());
  }
}
//}}}

