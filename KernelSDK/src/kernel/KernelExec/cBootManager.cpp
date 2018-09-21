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
#include "cBootManager.h"
#include "LogCategories.h"

using namespace vfs;
using namespace std;




// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

cBootManager::cBootManager() :
  m_BootMsgServer(iSystemInfo::singleton().getProductName() + L" {76037833-A354-4325-A0D3-64D0EE570BF1} // KernelBoot status"),
  m_IsBootMsgEnabled(true),
  m_BootState(eBootState_Booting),
  m_NextProgressMsgId(0),
  m_SizeOfProgressMsgStackLastTransmitted(0),
  m_ProgressMsgQueue(iThreadManager::singleton().createAsyncMsgQueue(L"boot progress", iBasicThread::ePriority_Normal)),
  m_IsProgressMsgEnabled(false)
{
  cLogIndentGuard IndentGuard(kCtgCreateManagerDetail, L"Creating BootManager");
}




// =======================================================================================
// === ProgressMsgGuard ==================================================================
// =======================================================================================

class cTransmitProgressMsgStack : public iMsgQueue::iMsg, public cRefCount
{
  public :
    cTransmitProgressMsgStack(
      cBootManager * const bootManager,
      const map<unsigned int, String> & msgStack) :
        m_BootManager(bootManager),
        m_MsgStack(msgStack)
    {
    }
    virtual void QAPI deliver2() { m_BootManager->_doTransmitProgressMsgStack(m_MsgStack); }
  private :
    cBootManager * const m_BootManager;
    const map<unsigned int, String> m_MsgStack;
};

void QAPI cBootManager::INTERNAL_USE_ONLY_createProgressMsgGuard(cProgressMsgGuard * const guard, const Char * const format, const va_list & VArgs)
{
  cLockGuard threadSafety(&m_BootMsgThreadSafety);
  Char formatted[2048] = L"\0";
  _vsnwprintf(formatted, sizeof(formatted) / sizeof(Char), format, VArgs);
  StringStream msg;
  msg << formatted;
  guard->m_Id = m_NextProgressMsgId++;
  m_ProgressMsgStack.insert(make_pair(guard->m_Id, msg.str()));

//  if (m_BootState != eBootState_CleanExiting && m_BootState != eBootState_Crashing) 
    m_ProgressMsgQueue->clearMsgs();
  m_ProgressMsgQueue->postMsg(new cTransmitProgressMsgStack(
    this,
    m_EmergencyProgressMsgStack.empty() ? m_ProgressMsgStack : m_EmergencyProgressMsgStack));
}

// ---------------------------------------------------------------------------------------

void QAPI cBootManager::INTERNAL_USE_ONLY_destroyProgressMsgGuard(cProgressMsgGuard * const guard)
{
  cLockGuard threadSafety(&m_BootMsgThreadSafety);
  map<unsigned int, String>::iterator i = m_ProgressMsgStack.find(guard->m_Id);
  if (i != m_ProgressMsgStack.end())
  {
    m_ProgressMsgStack.erase(i);

//    if (m_BootState != eBootState_CleanExiting && m_BootState != eBootState_Crashing)
      m_ProgressMsgQueue->clearMsgs();
    m_ProgressMsgQueue->postMsg(new cTransmitProgressMsgStack(
      this,
      m_EmergencyProgressMsgStack.empty() ? m_ProgressMsgStack : m_EmergencyProgressMsgStack));
  }
}




// =======================================================================================
// === EmergencyProgressMsgGuard =========================================================
// =======================================================================================

void QAPI cBootManager::INTERNAL_USE_ONLY_createEmergencyProgressMsgGuard(cEmergencyProgressMsgGuard * const guard, const Char * const format, const va_list & VArgs)
{
  cLockGuard threadSafety(&m_BootMsgThreadSafety);
  Char formatted[2048] = L"\0";
  _vsnwprintf(formatted, sizeof(formatted) / sizeof(Char), format, VArgs);
  StringStream msg;
  msg << formatted;
  guard->m_Id = m_NextProgressMsgId++;
  m_EmergencyProgressMsgStack.insert(make_pair(guard->m_Id, msg.str()));

//  if (m_BootState != eBootState_CleanExiting && m_BootState != eBootState_Crashing)
    m_ProgressMsgQueue->clearMsgs();
  m_ProgressMsgQueue->postMsg(new cTransmitProgressMsgStack(
    this,
    m_EmergencyProgressMsgStack.empty() ? m_ProgressMsgStack : m_EmergencyProgressMsgStack));
}

// ---------------------------------------------------------------------------------------

void QAPI cBootManager::INTERNAL_USE_ONLY_destroyEmergencyProgressMsgGuard(cEmergencyProgressMsgGuard * const guard)
{
  cLockGuard threadSafety(&m_BootMsgThreadSafety);
  map<unsigned int, String>::iterator i = m_EmergencyProgressMsgStack.find(guard->m_Id);
  if (i != m_EmergencyProgressMsgStack.end())
  {
    m_EmergencyProgressMsgStack.erase(i);

//    if (m_BootState != eBootState_CleanExiting && m_BootState != eBootState_Crashing)
      m_ProgressMsgQueue->clearMsgs();
    m_ProgressMsgQueue->postMsg(new cTransmitProgressMsgStack(
      this,
      m_EmergencyProgressMsgStack.empty() ? m_ProgressMsgStack : m_EmergencyProgressMsgStack));
  }
}




// =======================================================================================
// === getState/setState =================================================================
// =======================================================================================

iBootManager::eBootState QAPI cBootManager::getState() const
{
  return m_BootState;
}

// ---------------------------------------------------------------------------------------

void cBootManager::INTERNAL_USE_ONLY_notifyAppWindowCreated(const HWND hAppWindow)
{
  cLockGuard threadSafety(&m_BootMsgThreadSafety);

  StringStream tmp; tmp << L"__KernelAppWindowCreated " << hex << hAppWindow;
  if (!m_BootMsgServer.sendStringWait(tmp.str().c_str(), 2000))
  {
    QSOS((L"Timed out waiting for KernelBoot process - perhaps KernelBoot was not running?"));
    m_IsBootMsgEnabled = false;
  }
}

// ---------------------------------------------------------------------------------------

void QAPI cBootManager::setState(const eBootState newState)
{
  if (m_BootState != newState)
  {
    m_BootState = newState;
    switch (m_BootState)
    {
      case eBootState_Booting :
      {
        break;
      }
      case eBootState_Booted :
      {
        cLockGuard threadSafety(&m_BootMsgThreadSafety);
        if (!m_BootMsgServer.sendStringWait(L"__KernelBooted", 2000))
        {
          QSOS((L"Timed out waiting for KernelBoot process - perhaps KernelBoot was not running?"));
          m_IsBootMsgEnabled = false;
        }
        break;
      }
      case eBootState_CleanExiting :
      {
        cLockGuard threadSafety(&m_BootMsgThreadSafety);
        if (!m_BootMsgServer.sendStringWait(L"__KernelCleanExiting", 2000))
        {
          QSOS((L"Timed out waiting for KernelBoot process - perhaps KernelBoot was not running?"));
          m_IsBootMsgEnabled = false;
        }
        break;
      }
      case eBootState_Crashing :
      {
        break;
      }
      case eBootState_InstallDllsNecessary:
      {
        cLockGuard threadSafety(&m_BootMsgThreadSafety);
        if (!m_BootMsgServer.sendStringWait(L"__KernelInstallDllsNescessary", 2000))
        {
          QSOS((L"Timed out waiting for KernelBoot process - perhaps KernelBoot was not running?"));
          m_IsBootMsgEnabled = false;
        }
        break;
      }
      case eBootState_RestartNecessary:
      {
        cLockGuard threadSafety(&m_BootMsgThreadSafety);
        if (!m_BootMsgServer.sendStringWait(L"__KernelRestartNescessary", 2000))
        {
          QSOS((L"Timed out waiting for KernelBoot process - perhaps KernelBoot was not running?"));
          m_IsBootMsgEnabled = false;
        }
        break;
      }
      default :
      {
        QFATAL((__FILE__, __LINE__, L"Unexpected m_BootState %d", m_BootState));
        break;
      }
    }

    list<iProgressCallback::Ptr> Callbacks;
    {
      // Protect member variables from another thread
      cLockGuard threadSafety(&m_CallbacksThreadSafety);
      Callbacks = m_Callbacks;
    }

    for (list<iProgressCallback::Ptr>::const_iterator i = Callbacks.begin(); i != Callbacks.end(); ++i)
    {
      (*i)->onBootStateChanged(m_BootState);
    }
  }
}




// =======================================================================================
// === _doTransmitProgressMsgStack =======================================================
// =======================================================================================

void cBootManager::_doTransmitProgressMsgStack(const map<unsigned int, String> & newMsgStack)
{
  if (m_IsBootMsgEnabled && m_IsProgressMsgEnabled)
  {
    if (newMsgStack.size() >= m_SizeOfProgressMsgStackLastTransmitted) // avoid bottom line flashing
    {
      String formatted;
      for (map<unsigned int, String>::const_iterator i = newMsgStack.begin(); i != newMsgStack.end(); ++i)
      {
        formatted += String(L"$") + i->second + L"$ ";
      }
      if (!m_BootMsgServer.sendStringWait(formatted, 500))
      {
        if (!m_BootMsgServer.sendStringWait(formatted, 500))
        {
          QMSG((kCtgKernelBootComms, L"Timed out waiting for KernelBoot process - perhaps KernelBoot was not running?"));
          m_IsBootMsgEnabled = false;
        }
      }
    }
    m_SizeOfProgressMsgStackLastTransmitted = (unsigned int)newMsgStack.size();
  }
}




// =======================================================================================
// === addProgressCallback ===============================================================
// =======================================================================================

cGuard::ConstPtr QAPI cBootManager::addProgressCallback(iProgressCallback::Ptr Callback)
{
  {
    cLockGuard threadSafety(&m_CallbacksThreadSafety);
    m_Callbacks.push_back(Callback);
  }
  Callback->onBootStateChanged(m_BootState);
  return new cGuard(this, Callback);
}




// =======================================================================================
// === onGuardDestroyed ==================================================================
// =======================================================================================

void QAPI cBootManager::onGuardDestroyed(iGuarded::Ptr Guarded) throw()
{
  iProgressCallback::Ptr ToRemove = ptr_cast<iProgressCallback::Ptr>(Guarded);

  if (ToRemove.isValid())
  {
    cLockGuard threadSafety(&m_CallbacksThreadSafety);
    m_Callbacks.remove(ToRemove);
  }
}




// =======================================================================================
// === setProgressMsg/hideSplash =========================================================
// =======================================================================================

bool QAPI cBootManager::isKernelBootVisible(HWND * const kernelBootWindowHandle) const
{
  const String wndTitleToFind(iSystemInfo::singleton().getProductName() + L" Boot");
  *kernelBootWindowHandle = FindWindow(NULL /* class name, NULL = any class */, wndTitleToFind.c_str());
  return *kernelBootWindowHandle && IsWindowVisible(*kernelBootWindowHandle);
}




// =======================================================================================
// === _updateBootDrivers ================================================================
// =======================================================================================

void cBootManager::_updateBootDrivers()
{
  cLockGuard threadSafety(&m_DriversByImplGuidThreadSafety);

  const list<iFactory::iConstructor::Ptr> ctors = gFactory->getConstructors(iBootPhaseDriver::class_info().getGuid());
  for (list<iFactory::iConstructor::Ptr>::const_iterator i = ctors.begin(); i != ctors.end(); ++i)
  {
    const cGuid implGuid((*i)->getClassInfo()->getGuid());
    if (m_DriversByImplGuid.find(implGuid) == m_DriversByImplGuid.end())
    {
      m_DriversByImplGuid.insert(make_pair(implGuid, ptr_cast<iBootPhaseDriver::Ptr>(gFactory->createInstance(implGuid))));
    }
  }
}




// =======================================================================================
// === hasPhase ==========================================================================
// =======================================================================================

bool cBootManager::hasPhase(const iBootPhaseDriver::eBootPhase WhichPhase)
{
  _updateBootDrivers();
  {
    cLockGuard threadSafety(&m_DriversByImplGuidThreadSafety);
    for (map<cGuid, iBootPhaseDriver::Ptr>::const_iterator i = m_DriversByImplGuid.begin(); i != m_DriversByImplGuid.end(); ++i)
    {
      if (i->second->getPhase() == WhichPhase)
      {
        return true;
      }
    }
  }
  return false;
}




// =======================================================================================
// === doPhase ===========================================================================
// =======================================================================================

void cBootManager::doPhase(const iBootPhaseDriver::eBootPhase WhichPhase) throw(cRecoverable)
{
  String PhaseName;
  switch (WhichPhase)
  {
    case iBootPhaseDriver::eMonitoring : { PhaseName = L"Monitoring"; break; }
    case iBootPhaseDriver::eDevices    : { PhaseName = L"Devices"; break; }
    case iBootPhaseDriver::eDisplay    : { PhaseName = L"Display"; break; }
    case iBootPhaseDriver::eSecurity   : { PhaseName = L"Security"; break; }
    case iBootPhaseDriver::ePlugins    : { PhaseName = L"Plugins"; break; }
    case iBootPhaseDriver::eBooted     : { PhaseName = L"Booted"; break; }
  }

  {
    map<cGuid, iBootPhaseDriver::Ptr> copyOfDrivers;
    {
      cLockGuard threadSafety(&m_DriversByImplGuidThreadSafety);
      copyOfDrivers = m_DriversByImplGuid;
    }
    for (map<cGuid, iBootPhaseDriver::Ptr>::const_iterator i = copyOfDrivers.begin(); i != copyOfDrivers.end(); ++i)
    {
      if (i->second->getPhase() == WhichPhase)
      {
        i->second->doPhase();
      }
    }
  }
}



