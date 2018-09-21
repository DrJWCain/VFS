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
#include "fatal.h"

using namespace vfs;




// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

cGuard::cGuard(const iGuardCreator::Ptr & Creator, const iGuarded::Ptr & Guarded) :
  m_Creator(Creator),
  m_Guarded(Guarded)
{
}

// ---------------------------------------------------------------------------------------

cGuard::~cGuard()
{
  if (m_Creator.isValid()) m_Creator->onGuardDestroyed(m_Guarded);
}




// =======================================================================================
// === Unsupported Methods ===============================================================
// =======================================================================================

// Only implemented as a private method in order to stop it being used!
cGuard::cGuard(const cGuard & Other)
{
  // Not implemented
  QKERNELBASEVERIFY(false);
}

// ---------------------------------------------------------------------------------------

// Only implemented as a private method in order to stop it being used!
void cGuard::operator=(const cGuard & Other)
{
  // Not implemented
  QKERNELBASEVERIFY(false);
}
