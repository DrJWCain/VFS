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

#include "cThreadManager.h"
#include "cDebugDriver.h"

// --- Use Namespace ---------------------------------------------------------------------

using namespace vfs;
using namespace std;




// =======================================================================================
// === gRegdClasses ======================================================================
// =======================================================================================

static list<cGuard::ConstPtr> gRegdClasses;




// =======================================================================================
// === DllMain ===========================================================================
// =======================================================================================

BOOL WINAPI DllMain(
  HINSTANCE,            // handle to the DLL module
  DWORD fdwReason,      // reason for calling function
  LPVOID)               // reserved
{
  switch(fdwReason)
  {
    case DLL_PROCESS_ATTACH :
    {
      gRegdClasses.push_back(gFactory->registerClass(new cThreadManagerEx::class_ctor));
      cThreadManagerEx::singleton().addNewThread();
      break;
    }
    case DLL_THREAD_ATTACH :
    {
      cThreadManagerEx::singleton().addNewThread();
      break;
    }
    case DLL_THREAD_DETACH :
    {
      cThreadManagerEx::singleton().removeOldThread();
      break;
    }
    case DLL_PROCESS_DETACH :
    {
      cThreadManagerEx::singleton().removeOldThread();
      break;
    }
    default :
    {
      // Do nothing.
    }
  }

  return TRUE;
}




// =======================================================================================
// === moduleInit ========================================================================
// =======================================================================================

extern "C" QPLUGIN void QARGS_STACK moduleInit(iModuleInstance::ConstPtr Context) throw( cRecoverable)
{
  // Throw a cRecoverable with an appropriate message to tell the Kernel that initialisation
  // has failed and it is safe to continue.
 
  gRegdClasses.push_back(gFactory->registerClass(new cDebugDriver::class_ctor));

// Already registered in DllMain above
//  gRegdClasses.push_back(gFactory->registerClass(new cThreadManagerEx::class_ctor));
}

// ---------------------------------------------------------------------------------------

extern "C" QPLUGIN void QARGS_STACK moduleCleanUp(void) throw()
{
}
