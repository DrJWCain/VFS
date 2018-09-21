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

// Standard C library headers

// Standard C++ library headers

// vfs Kernel headers
#include "QKernelExec.h"

// Local headers

// --- Start of Namespace ----------------------------------------------------------------

namespace vfs
{



// =======================================================================================
// === cLogThreadData ====================================================================
// =======================================================================================

class cLogThreadData
{
  public :

    cLogThreadData() throw() :
      m_IsLazyIndenting(false),
      m_CurrCtg(0),
      m_CurrCtgModuleAddr(0),
      m_NumSOSIndents(0),
      m_NumInjectedIndents(0),
      m_NumSuppressedIndents(0),
      m_Indent(0),
      m_SuppressionCount(0)
    {
      // Do nothing.
    }

    ~cLogThreadData() throw()
    {
      // Do nothing.
    }

    bool                  m_IsLazyIndenting;
    iLogManager::Category m_CurrCtg;
    void *                m_CurrCtgModuleAddr;
    unsigned int          m_NumSOSIndents;
    unsigned int          m_NumInjectedIndents;
    unsigned int          m_NumSuppressedIndents;
    unsigned int          m_Indent;
    int                   m_SuppressionCount;
};




// --- End of Namespace ------------------------------------------------------------------

}
