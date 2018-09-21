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

// --- Namespace -------------------------------------------------------------------------

using namespace vfs;




// =======================================================================================
// === cvfsEnvironment ===============================================================
// =======================================================================================

class cvfsEnvironment : public ivfsEnvironment, public cRefCount
{
  public :

    QDEFINE_SINGLETON_IMPL(cvfsEnvironment, L"{F27A61CA-8C30-48aa-9092-7029199B8332} // cvfsEnvironment", ivfsEnvironment);

    cvfsEnvironment();

    // iManager
    void QAPI logSummary() {}
    void QAPI logDetailedSummary() {}

    // ivfsEnvironment
    String QAPI getUserFolder() const;
    String QAPI getLocalMachineRootFolder() const;
    String QAPI getRelativeRootFolder() const;

  private :

    mutable cLock           m_ThreadSafety;

    String                  m_LocalMachineRootFolder;
    String                  m_BodgeDatabaseFolder;
    String                  m_RelativeRootFolder;
};
