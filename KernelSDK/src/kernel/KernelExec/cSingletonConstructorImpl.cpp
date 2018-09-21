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

#include "cSingletonConstructorImpl.h"

// --- Using Namespace -------------------------------------------------------------------

using namespace vfs;




// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

cSingletonConstructorImpl::cSingletonConstructorImpl(iFactory::iClassInfo::ConstPtr ClassInfo, iFactory::iSingletonCreator::Ptr Creator) :
  m_ClassInfo(ClassInfo),
  m_Creator(Creator),
  m_IsConstructingOrDestroying(false)
{
}

// ---------------------------------------------------------------------------------------

cSingletonConstructorImpl::~cSingletonConstructorImpl()
{
//  QVERIFY_AND_CONTINUE(gFactory->countInstances(m_ClassInfo->getGuid()) == 0);

  m_IsConstructingOrDestroying = true;
  m_Singleton.invalidate();
  m_IsConstructingOrDestroying = false;
}




// =======================================================================================
// === getClassInfo ======================================================================
// =======================================================================================

iFactory::iClassInfo::ConstPtr QAPI cSingletonConstructorImpl::getClassInfo() const
{
  return m_ClassInfo;
}




// =======================================================================================
// === createInstance/createInstanceWithParams ===========================================
// =======================================================================================

iFactory::iInstance::Ptr QAPI cSingletonConstructorImpl::createInstance()
{
  cLockGuard ConstructionGuard(&m_ConstructionGuard);

  if (!m_Singleton.isValid())
  {
    if (m_IsConstructingOrDestroying)
    {
      QFATAL((__FILE__, __LINE__, L"Singleton re-entrancy check failed - tried to use singleton whilst it is being constructed/destroyed"));
    }
    m_IsConstructingOrDestroying = true;
    try
    {
      m_Singleton = m_Creator->createSingleton();
    }
    catch (cRecoverable)
    {
      m_IsConstructingOrDestroying = false;
      throw;
    }
    m_IsConstructingOrDestroying = false;
  }
  return m_Singleton;
}

iFactory::iInstance::Ptr QAPI cSingletonConstructorImpl::createInstanceWithParams(const iFactory::iConstructor::iParameters & Params)
{
  throw cNotSupported(__FILE__, __LINE__, L"No parameterised \"%s\" ctor", m_ClassInfo->getName().c_str());
}

