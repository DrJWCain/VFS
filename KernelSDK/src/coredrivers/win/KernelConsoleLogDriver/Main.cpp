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

#include "Main.h"
#include "cConsoleLogDriver.h"
/*
#include "KernelConsoleLogDriver.h"
#include "KernelConsoleLogDriver_i.c"
#include "cTrayMenuItemCallback.h"
*/
// --- Use Namespace ---------------------------------------------------------------------

using namespace vfs;
using namespace std;




// =======================================================================================
// === _ATLObjects =======================================================================
// =======================================================================================
/*
namespace
{

BEGIN_OBJECT_MAP(_ATLObjects)
OBJECT_ENTRY(CLSID_TrayMenuItemCallback, cTrayMenuItemCallback)
END_OBJECT_MAP()

}
*/



// =======================================================================================
// === gModuleContext ====================================================================
// =======================================================================================

iModuleInstance::Ptr gModuleContext;




// =======================================================================================
// === Entry Points ======================================================================
// =======================================================================================

extern "C" QPLUGIN void QARGS_STACK moduleRegister(iModuleRegistrar::Ptr Context)
{
//  ATL::_pAtlModule = Context->getATLModule(); // Assign preprepared ATL module object to local ATL instance
//  Context->addATLTypeLibrary(0 /* index of type library in resources */);
//  Context->addATLObjects(_ATLObjects);
}

// ---------------------------------------------------------------------------------------

extern "C" QPLUGIN void QARGS_STACK moduleInit(iModuleInstance::Ptr Context) throw( cRecoverable)
{
  gModuleContext = Context;

  static list<cGuard::ConstPtr> RegdClasses;

  bool IsRegistering = true;
  if (Context->getCustomSettingNumber(L"", L"Don't load if debugger present") != 0)
  {
    // Don't want to use console output with debugger because of dead lock between them when
    // and exception is raised.
    if (IsDebuggerPresent())
    {
      IsRegistering = false;
    }
  }

  if (IsRegistering)
  {
    RegdClasses.push_back(gFactory->registerClass(new cConsoleLogDriver::class_ctor));
  }
}

// ---------------------------------------------------------------------------------------

extern "C" QPLUGIN void QARGS_STACK moduleCleanUp(void) throw()
{
}
