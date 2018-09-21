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
#include "QKernelBase.h"

using namespace vfs;




// =======================================================================================
// === onAfterAddRef/onBeforeRemoveRef/verifyRefCount ====================================
// =======================================================================================

#if QSWITCH_TRACKPTR

void QAPI cRefCount::onAfterAddRef(
  const void * const PtrID,
  const void * const ModuleAddr,
  const unsigned int RefCount) const
{
  // Do nothing.
}

#endif

// ---------------------------------------------------------------------------------------

#if QSWITCH_TRACKPTR

void QAPI cRefCount::onBeforeRemoveRef(
  const void * const PtrID,
  const void * const ModuleAddr,
  const unsigned int RefCount) const
{
  // Do nothing.
}

#endif

// ---------------------------------------------------------------------------------------

#if QSWITCH_TRACKPTR

void QAPI cRefCount::verifyRefCount(
  const unsigned int ExpectedRefCount) const
{
  // Do nothing.
}

#endif

// ---------------------------------------------------------------------------------------

#if QSWITCH_TRACKPTR

void QAPI cRefCount::setTracking(
  const bool IsTracking)
{
  m_IsTracking = IsTracking;
}

#endif




// =======================================================================================
// === cAddRefGuard Construction & Destruction ===========================================
// =======================================================================================

cRefCount::cAddRefGuard::cAddRefGuard(cRefCount * const RefCount) :
  m_RefCount(RefCount)
{
  assert(RefCount);
#if QSWITCH_TRACKPTR
  m_RefCount->addRef(this, getFuncCallerAddr());
#else
  m_RefCount->addRef();
#endif
}

// ---------------------------------------------------------------------------------------

cRefCount::cAddRefGuard::~cAddRefGuard()
{
#if QSWITCH_TRACKPTR
  m_RefCount->removeRef(this, getFuncCallerAddr());
#else
  m_RefCount->removeRef();
#endif
}
