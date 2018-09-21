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
#include "cDeviceManager.h"
#include "LogCategories.h"
#include <setupapi.h>

using namespace std;
using namespace vfs;




// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

cDeviceManager::cDeviceManager() :
  m_NextAcqOrder(0)
{
  cLogIndentGuard IndentGuard(kCtgCreateManagerDetail, L"Creating DeviceManager");

  // [RFD-148/AJS] Register for dumps
  if (iKernelDumpManager::isSingletonRegistered() && iKernelDumpManager::singleton().isDumpEnabled())
    m_dumpRegistration = iKernelDumpManager::singleton().Register(this);
}




// =======================================================================================
// === logSummary/logDetailedSummary =====================================================
// =======================================================================================

void QAPI cDeviceManager::logSummary()
{
  {
    cLogIndentGuard Indent(0, L"Devices");
    logDevices();
  }
}

// ---------------------------------------------------------------------------------------

void QAPI cDeviceManager::logDetailedSummary()
{
}




// =======================================================================================
// === releaseDeviceByName ===============================================================
// =======================================================================================

bool QAPI cDeviceManager::releaseDeviceByName(const String & DriverName)
{
  bool IsReleased = true;

  iDeviceDriver::Ptr Driver;
  try
  {
    Driver = getDeviceByName(DriverName);
  }
  catch (cNotFound &)
  {
  }

  if (Driver.isValid())
  {
    list<iPlugAndPlayClient::Ptr> copyOfPlugAndPlayClients;
    {
      cLockGuard MemberGuard(&m_PlugAndPlayClientsThreadSafety);
      copyOfPlugAndPlayClients = m_PlugAndPlayClients;
    }
    // Request plug and play clients to release all their smart pointers to this device driver
    for (list<iPlugAndPlayClient::Ptr>::iterator i = copyOfPlugAndPlayClients.begin(); i != copyOfPlugAndPlayClients.end(); ++i)
    {
      (*i)->onDeviceRequestingRelease(Driver);
    }

    QMSG((0, L"Driver smart pointer reference count == %d (should be 2 for successful release at this point)", Driver.getCounter()->getNumRefs()));

    if (Driver.getCounter()->getNumRefs() == 2)
    {
      Driver.invalidate();

      {
        cLockGuard MemberGuard(&m_DevicesByNameThreadSafety);
        map<String, cAboutDevice>::iterator j = m_DevicesByName.find(toLower(DriverName));
        QVERIFY(j != m_DevicesByName.end());
        j->second.m_Comment = L"Released";
        j->second.m_Driver.invalidate();
      }

      list<iPlugAndPlayClient::Ptr> copyOfPlugAndPlayClients;
      {
        cLockGuard MemberGuard(&m_PlugAndPlayClientsThreadSafety);
        copyOfPlugAndPlayClients = m_PlugAndPlayClients;
      }
      for (list<iPlugAndPlayClient::Ptr>::iterator i = copyOfPlugAndPlayClients.begin(); i != copyOfPlugAndPlayClients.end(); ++i)
      {
        (*i)->onDeviceReleased(DriverName);
      }
    }
    else
    {
      IsReleased = false;
    }
  }

  return IsReleased;
}




// =======================================================================================
// === logDevices ========================================================================
// =======================================================================================

void QAPI cDeviceManager::logDevices() const
{
  cLockGuard MemberGuard(&m_DevicesByNameThreadSafety);
  if (m_DevicesByName.size() == 0)
  {
    QMSG((0, L"No devices"));
  }
  else
  {
    QMSG((0, L"%-40s %s", L"Driver", L"Device Result"));
    QMSG((0, L""));

    for (map<String, cAboutDevice>::const_iterator i = m_DevicesByName.begin(); i != m_DevicesByName.end(); ++i)
    {
      QMSG((0, L"%-40s %s", i->second.m_Name.c_str(), i->second.m_Comment.c_str()));
    }
  }
}




// =======================================================================================
// === acquireDevice =====================================================================
// =======================================================================================

void QAPI cDeviceManager::acquireDevice(const cGuid & DriverClassGuid) throw(cNotAvailable)
{
  if (getDevicesByClass(DriverClassGuid).size() > 0)
  {
    QFATAL((__FILE__, __LINE__, L"Device \"%s\" is already acquired", DriverClassGuid.serialise().c_str()));
  }

  cAboutDevice AboutNewDevice(iDeviceDriver::Ptr(), DriverClassGuid.getComment(), L"");
  try
  {
    cProgressMsgGuard progress(L"Acquiring \"%s\"", DriverClassGuid.getComment().c_str());
    cLogIndentGuard Indent(kCtgAcquireDeviceDetail, L"Acquiring \"%s\"", DriverClassGuid.getComment().c_str());

    AboutNewDevice.m_Driver = ptr_cast<iDeviceDriver::Ptr>(gFactory->createInstance(DriverClassGuid));
    AboutNewDevice.m_Name    = DriverClassGuid.getComment();
    AboutNewDevice.m_Comment = L"OK";
    AboutNewDevice.m_AcqOrder = m_NextAcqOrder++;

    // Update plug and play
    list<iPlugAndPlayClient::Ptr> copyOfPlugAndPlayClients;
    {
      cLockGuard MemberGuard(&m_PlugAndPlayClientsThreadSafety);
      copyOfPlugAndPlayClients = m_PlugAndPlayClients;
    }
    for (list<iPlugAndPlayClient::Ptr>::iterator i = copyOfPlugAndPlayClients.begin(); i != copyOfPlugAndPlayClients.end(); ++i)
    {
      (*i)->onDeviceAcquired(AboutNewDevice.m_Driver);
    }
  }
  catch (cRecoverable & Reason)
  {
    StringStream Comment;
    Comment << L"Not available (" << Reason.getMsg() << L")";
    AboutNewDevice.m_Comment = Comment.str();
    QMSG((0, L"Failed to acquire \"%s\" (%s), ignoring", DriverClassGuid.getComment().c_str(), Reason.getMsg().c_str()));
  }

  {
    cLockGuard MemberGuard(&m_DevicesByNameThreadSafety);
    map<String, cAboutDevice>::iterator j = m_DevicesByName.find(toLower(AboutNewDevice.m_Name));
    if (j == m_DevicesByName.end())
    {
      m_DevicesByName.insert(make_pair(toLower(DriverClassGuid.getComment()), AboutNewDevice));
    }
    else
    {
      j->second.m_Driver  = AboutNewDevice.m_Driver;
      j->second.m_Comment = AboutNewDevice.m_Comment;
    }
  }

  if (AboutNewDevice.m_Comment == L"OK")
  {
    QMSG((0, L"Successfully acquired \"%s\"", DriverClassGuid.getComment().c_str()));
  }
  else
  {
    throw cNotAvailable(__FILE__, __LINE__, AboutNewDevice.m_Comment.c_str());
  }
}





// =======================================================================================
// === acquireAllDevices =================================================================
// =======================================================================================

void cDeviceManager::acquireAllDevices()
{
  list<iFactory::iConstructor::Ptr> Constructors = gFactory->getConstructors(iDeviceDriver::class_info().getGuid());
  for (list<iFactory::iConstructor::Ptr>::iterator i = Constructors.begin();
       i != Constructors.end() && iBootManager::singleton().getState() < iBootManager::eBootState_CleanExiting;
       ++i)
  {
    bool IsAlreadyAcquired = false;
    {
      cLockGuard MemberGuard(&m_DevicesByNameThreadSafety);
      IsAlreadyAcquired = m_DevicesByName.find(toLower((*i)->getClassInfo()->getName())) != m_DevicesByName.end();
    }
    if (!IsAlreadyAcquired)
    {
      try
      {
        acquireDevice((*i)->getClassInfo()->getGuid());
      }
      catch (cRecoverable &)
      {
      }
    }
  }
}




// =======================================================================================
// === getDeviceByName ===================================================================
// =======================================================================================

iDeviceDriver::Ptr /* invalid if not found! */ QAPI cDeviceManager::findDeviceByName(const String & DriverName) const
{
  iDeviceDriver::Ptr result;
  cLockGuard MemberGuard(&m_DevicesByNameThreadSafety);
  map<String, cAboutDevice>::const_iterator i = m_DevicesByName.find(toLower(DriverName));
  if (i != m_DevicesByName.end())
  {
    if (i->second.m_Driver.isValid())
    {
      result = i->second.m_Driver;
    }
  }
  return result;
}

iDeviceDriver::Ptr QAPI cDeviceManager::getDeviceByName(const String & DriverName) const throw(cNotFound)
{
  cLockGuard MemberGuard(&m_DevicesByNameThreadSafety);
  map<String, cAboutDevice>::const_iterator i = m_DevicesByName.find(toLower(DriverName));
  if (i == m_DevicesByName.end())
  {
    throw cNotFound(__FILE__, __LINE__, L"Device \"%s\" not found", DriverName.c_str());
  }
  if (!i->second.m_Driver.isValid())
  {
    throw cNotFound(__FILE__, __LINE__, L"Device \"%s\" not acquired", DriverName.c_str());
  }
  return i->second.m_Driver;
}




// =======================================================================================
// === getDevicesByClass =================================================================
// =======================================================================================

vector<iDeviceDriver::Ptr> QAPI cDeviceManager::getDevicesByClass(const cGuid & DriverClassGuid) const
{
  cLockGuard MemberGuard(&m_DevicesByNameThreadSafety);
  vector<iDeviceDriver::Ptr> Result;
  for (map<String, cAboutDevice>::const_iterator i = m_DevicesByName.begin(); i != m_DevicesByName.end(); ++i)
  {
    if (i->second.m_Driver.isValid())
    {
      if (gFactory->isInstanceDerivedFrom(i->second.m_Driver, DriverClassGuid))
      {
        Result.push_back(i->second.m_Driver);
      }
    }
  }
  return Result;
}




// =======================================================================================
// === addPlugAndPlayClient ==============================================================
// =======================================================================================

cGuard::ConstPtr QAPI cDeviceManager::addPlugAndPlayClient(iPlugAndPlayClient::Ptr ClientToAdd)
{
  {
    cLockGuard MemberGuard(&m_PlugAndPlayClientsThreadSafety);
    m_PlugAndPlayClients.push_back(ClientToAdd);
  }
  map<String, cAboutDevice> copyOfDevicesByName;
  {
    cLockGuard MemberGuard(&m_DevicesByNameThreadSafety);
    copyOfDevicesByName = m_DevicesByName;
  }
  map<unsigned int, iDeviceDriver::Ptr> deviceDriversInAcqOrder;
  for (map<String, cAboutDevice>::iterator i = copyOfDevicesByName.begin(); i != copyOfDevicesByName.end(); ++i)
  {
    if (i->second.m_Comment == L"OK")
    {
      deviceDriversInAcqOrder.insert(make_pair(i->second.m_AcqOrder, i->second.m_Driver));
    }
  }
  for (map<unsigned int, iDeviceDriver::Ptr>::iterator i = deviceDriversInAcqOrder.begin(); i != deviceDriversInAcqOrder.end(); ++i)
  {
    ClientToAdd->onDeviceAcquired(i->second);
  }
  return new cGuard(this, ClientToAdd);
}




// =======================================================================================
// === onGuardDestroyed ==================================================================
// =======================================================================================

void QAPI cDeviceManager::onGuardDestroyed(iGuarded::Ptr Guarded)
{
  iPlugAndPlayClient::Ptr ClientToRemove = ptr_cast<iPlugAndPlayClient::Ptr>(Guarded);
  if (ClientToRemove.isValid())
  {
    cLockGuard MemberGuard(&m_PlugAndPlayClientsThreadSafety);
    for (list<iPlugAndPlayClient::Ptr>::iterator i = m_PlugAndPlayClients.begin(); i != m_PlugAndPlayClients.end(); ++i)
    {
      if (*i == ClientToRemove) { m_PlugAndPlayClients.erase(i); break; }
    }
  }
}




// =======================================================================================
// === getWin32DevicesByType =============================================================
// =======================================================================================

multimap<cGuid, String> QAPI cDeviceManager::getWin32DevicesByType() throw(cWin32Error)
{
  multimap<cGuid, String> Result;

  const HDEVINFO SetHandle = SetupDiGetClassDevs(NULL, 0, 0, DIGCF_PRESENT | DIGCF_ALLCLASSES);
  if (SetHandle == INVALID_HANDLE_VALUE) throw cWin32Error(__FILE__, __LINE__, L"Tried to SetupDiGetClassDevs - %s", win32ErrorToString(GetLastError()).c_str());

  SP_DEVINFO_DATA DeviceInfoData;
  DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

  int i = 0;
  do
  {
    if (SetupDiEnumDeviceInfo(SetHandle, i++, &DeviceInfoData) != 0)
    {
      DWORD DataT;
      LPTSTR buffer = NULL;
      DWORD buffersize = 0;

      // Call function with null to begin with, then use the returned buffer size to Alloc the buffer. Keep calling until success or an unknown failure.
      while (!SetupDiGetDeviceRegistryProperty(
          SetHandle,
          &DeviceInfoData,
          SPDRP_DEVICEDESC,
          &DataT,
          (PBYTE)buffer,
          buffersize,
          &buffersize))
      {
        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
          // Change the buffer size.
          if (buffer) LocalFree(buffer);
          buffer = (LPTSTR)LocalAlloc(LPTR, buffersize);
        }
        else
        {
          throw cWin32Error(__FILE__, __LINE__, L"Tried to SetupDiGetDeviceRegistryProperty - %s", win32ErrorToString(GetLastError()).c_str());
        }
      }
      Result.insert(make_pair(cGuid(DeviceInfoData.ClassGuid), buffer));
      if (buffer) LocalFree(buffer);
    }
  } while(GetLastError() != ERROR_NO_MORE_ITEMS);

  (void)SetupDiDestroyDeviceInfoList(SetHandle);
  
  return Result;
}

// [RFD-148/AJS] Generate dump information when required
void QAPI cDeviceManager::onDump(iKernelDumpProcessor * const proc)
{
  proc->GenFile(L"devices.qdi");

  for (map<String, cAboutDevice>::const_iterator i = m_DevicesByName.begin(); i != m_DevicesByName.end(); ++i)
    proc->AppendF(L"DEV~%s~%s\r\n", i->second.m_Name.c_str(), i->second.m_Comment.c_str());

  proc->Close();
}