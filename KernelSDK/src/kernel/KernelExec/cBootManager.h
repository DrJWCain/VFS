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

#include <QKernelExecDDK.h>
#include "cClientServer.h"

using namespace vfs;
using namespace std;




// =======================================================================================
// === cBootManager ======================================================================
// =======================================================================================

class cBootManager : public iBootManager, public iGuardCreator, public cRefCount
{
	public:

    QDEFINE_SINGLETON_IMPL(cBootManager, L"{67D9855F-BD32-4F2A-93A5-5E269C681CC7} // Boot manager impl", iBootManager);

		cBootManager();

    // iManager
    void QAPI logSummary() {}
    void QAPI logDetailedSummary() {}

    // iBootManager
    virtual cGuard::ConstPtr QAPI addProgressCallback(iProgressCallback::Ptr Callback);
    virtual bool QAPI isKernelBootVisible(HWND * const kernelBootWindowHandle) const;
    virtual eBootState QAPI getState() const;
    virtual void QAPI setState(const eBootState newState);
    virtual void QAPI INTERNAL_USE_ONLY_createProgressMsgGuard(cProgressMsgGuard * const guard, const Char * const format, const va_list & vargs);
    virtual void QAPI INTERNAL_USE_ONLY_destroyProgressMsgGuard(cProgressMsgGuard * const guard);
    virtual void QAPI INTERNAL_USE_ONLY_createEmergencyProgressMsgGuard(cEmergencyProgressMsgGuard * const guard, const Char * const format, const va_list & vargs);
    virtual void QAPI INTERNAL_USE_ONLY_destroyEmergencyProgressMsgGuard(cEmergencyProgressMsgGuard * const guard);
    virtual void QAPI INTERNAL_USE_ONLY_notifyAppWindowCreated(const HWND hAppWindow);

    // cBootManager
    bool hasPhase(const iBootPhaseDriver::eBootPhase WhichPhase);
    void doPhase(const iBootPhaseDriver::eBootPhase WhichPhase) throw(cRecoverable);
    void _doTransmitProgressMsgStack(const map<unsigned int, String> & newMsgStack);
    virtual void enableProgressMsgs(const bool isEnabled) { m_IsProgressMsgEnabled = isEnabled; }

  private :

    eBootState                m_BootState;

    mutable cLock             m_BootMsgThreadSafety;
    bool                      m_IsBootMsgEnabled;
    cServer                   m_BootMsgServer;

    mutable cLock             m_CallbacksThreadSafety;
    list<iProgressCallback::Ptr> m_Callbacks;
    
    bool                      m_IsProgressMsgEnabled;
    iMsgQueue::Ptr            m_ProgressMsgQueue;
    map<unsigned int, String> m_ProgressMsgStack;
    map<unsigned int, String> m_EmergencyProgressMsgStack;
    unsigned int              m_SizeOfProgressMsgStackLastTransmitted;
    String                    m_ProgressErrorMsg;
    unsigned int              m_NextProgressMsgId;
    
    mutable cLock                     m_DriversByImplGuidThreadSafety;
    map<cGuid, iBootPhaseDriver::Ptr> m_DriversByImplGuid;

    void _updateBootDrivers();

    // iGuardCreator
    void QAPI onGuardDestroyed(iGuarded::Ptr Guarded);
};
