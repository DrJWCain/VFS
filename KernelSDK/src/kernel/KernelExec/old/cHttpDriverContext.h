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
// === cHttpDriverContext ================================================================
// =======================================================================================

class cHttpDriverContext : public iHttpDriverContext, public cRefCount
{
	public :

    static unsigned int m_NumInstances;

		cHttpDriverContext();
		~cHttpDriverContext() throw();

    String QAPI getHtmlResponse(const String & Command, const char * const Parameters);
    String QAPI getSoapResponse(const String & Command, const Char * const Parameters);

	private :

    mutable cLock m_MemberLock; // To protect member variables from changes by another thread.
};




// --- End of Namespace ------------------------------------------------------------------

}
