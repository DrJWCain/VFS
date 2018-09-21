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

// --- Start of Namespace ----------------------------------------------------------------

using namespace vfs;




// =======================================================================================
// === cSingletonConstructorImpl =========================================================
// =======================================================================================

class cSingletonConstructorImpl : public iFactory::iConstructor, public cRefCount
{
  public :

    // iFactory::iConstructor
    iFactory::iClassInfo::ConstPtr QAPI getClassInfo() const;
    iFactory::iInstance::Ptr QAPI createInstance();
    iFactory::iInstance::Ptr QAPI createInstanceWithParams(const iFactory::iConstructor::iParameters & Params);

    // cSingletonConstructorImpl
    cSingletonConstructorImpl(iFactory::iClassInfo::ConstPtr ClassInfo, iFactory::iSingletonCreator::Ptr Creator);
    ~cSingletonConstructorImpl();

  private :
    iFactory::iClassInfo::ConstPtr    m_ClassInfo;
    iFactory::iSingletonCreator::Ptr  m_Creator;
    iFactory::iInstance::Ptr         m_Singleton;
    bool                              m_IsConstructingOrDestroying;
    cLock                             m_ConstructionGuard;
};
