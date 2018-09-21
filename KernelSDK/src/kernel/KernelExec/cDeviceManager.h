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

#include <QWinBase.h>
#include <QKernelExecDDK.h>

// [RFD-148/AJS] Add the kernel dump interface for support info gathering
#include "KernelDumpInterface.h"

using namespace vfs;
using namespace std;




// =======================================================================================
// === cDeviceManager ====================================================================
// =======================================================================================

// [RFD-148/AJS] Add the kernel dump interface for support info gathering
class cDeviceManager : public iDeviceManager, public iGuardCreator, public cRefCount, 
                       public iKernelDumpManager::iKernelDumpParticipant
{
  friend class cHttpCmd_Devices;

  public :

    QDEFINE_SINGLETON_IMPL(cDeviceManager, L"{CA2EBA02-CB9C-41B7-9456-4F78ADB4962C} // cDeviceManager", iDeviceManager);

    // iManager
    virtual void QAPI logSummary();
    virtual void QAPI logDetailedSummary();

    // iDeviceManager
    virtual cGuard::ConstPtr QAPI addPlugAndPlayClient(iPlugAndPlayClient::Ptr ClientToAdd);
    virtual void QAPI logDevices() const;
    virtual iDeviceDriver::Ptr QAPI getDeviceByName(const String & DriverName) const throw(cNotFound);
    virtual vector<iDeviceDriver::Ptr> QAPI getDevicesByClass(const cGuid & DriverClassGuid) const;
    virtual void acquireAllDevices();
    virtual bool QAPI releaseDeviceByName(const String & DriverName);
    virtual void QAPI acquireDevice(const cGuid & DriverClassGuid) throw(cNotAvailable);
    virtual multimap<cGuid, String> QAPI getWin32DevicesByType() throw(cWin32Error);
    virtual iDeviceDriver::Ptr /* invalid if not found! */ QAPI findDeviceByName(const String & DriverName) const;

    // cDeviceManager
    cDeviceManager();

    // [RFD-148/AJS] Implement kernel dump participant interface
    virtual void QAPI      onDump(iKernelDumpProcessor * const proc);
    virtual String QAPI    getParticipantID() const { return L"Device Manager"; }

  private :

    class cAboutDevice
    {
      public :
        iDeviceDriver::Ptr m_Driver;
        String             m_Name;
        String             m_Comment;
        unsigned int       m_AcqOrder;
        cAboutDevice(iDeviceDriver::Ptr Driver, const String & Name, const String & Comment) :
          m_Driver(Driver), m_Name(Name), m_Comment(Comment), m_AcqOrder(0) {}
    };
    mutable cLock m_DevicesByNameThreadSafety;
    map<String, cAboutDevice> m_DevicesByName;

    mutable cLock m_PlugAndPlayClientsThreadSafety;
    list<iPlugAndPlayClient::Ptr> m_PlugAndPlayClients;

    unsigned int m_NextAcqOrder;

    // [RFD-148/AJS] Retain dump registration
    cGuard::ConstPtr      m_dumpRegistration;

    // iGuardCreator
    void QAPI onGuardDestroyed(iGuarded::Ptr Guarded);
};
