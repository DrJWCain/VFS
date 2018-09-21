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

#include "cHttpDriverContext.h"
#include "cHttpManager.h"

// --- Use Namespace ---------------------------------------------------------------------

using namespace vfs;




// =======================================================================================
// === m_NumInstances ====================================================================
// =======================================================================================

unsigned int cHttpDriverContext::m_NumInstances = 0;




// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

cHttpDriverContext::cHttpDriverContext() :
  m_MemberLock(L"HttpDriverContext members")
{
  m_NumInstances++;
}

// ---------------------------------------------------------------------------------------

cHttpDriverContext::~cHttpDriverContext()
{
  m_NumInstances--;
}




// =======================================================================================
// === getHtmlResponse ===================================================================
// =======================================================================================

String QAPI cHttpDriverContext::getHtmlResponse(
  const String &     Command,
  const char * const Parameters)
{
  // Do not protect member variables from changes by another thread because we're not using
  // any!
  // cLockGuard MemberGuard(&m_MemberLock);

  cHttpManager & hm = static_cast<cHttpManager &>(iHttpManager::singleton());
  return hm.getHtmlResponse(Command, Parameters);
}




// =======================================================================================
// === getSoapResponse ===================================================================
// =======================================================================================

String QAPI cHttpDriverContext::getSoapResponse(
  const String &     Command,
  const Char * const Parameters)
{
  // Do not protect member variables from changes by another thread because we're not using
  // any!
  // cLockGuard MemberGuard(&m_MemberLock);

  cHttpManager & hm = static_cast<cHttpManager &>(iHttpManager::singleton());
  return hm.getSoapResponse(Command, Parameters);
}
