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

// --- Main.cpp ---
// --- ---
#include "stdafx.h"

#include "cQCIFSFwk.h"
#include "cWriteInfo.h"
#include "cQCIFSFwkHelper.h"

using namespace vfs;

void initactivities(iActivityGroup::Ptr activityGroup);

iModuleContext::Ptr gPluginContext ;
static std::list<cGuard::ConstPtr> gRegdClasses;
iActivityGroup::Ptr gActivityGroup;

// plugIn interface
extern "C" QPLUGIN void QARGS_STACK moduleInit(iModuleContext::Ptr pContext) throw( cRecoverable)
{
  // Throw a cRecoverable with an appropriate message to tell the Kernel that initialisation
  // has failed and it is safe to continue.

  gPluginContext = pContext;

  gRegdClasses.push_back (gFactory->registerClass (new cQCIFSFwk::class_ctor));
  gRegdClasses.push_back (gFactory->registerClass (new cWriteInfo::class_ctor));
  gRegdClasses.push_back (gFactory->registerClass (new cQCIFSFwkHelper::class_ctor));

  
  gActivityGroup = iActivityLog::singleton().getGroup(L"QCIFS Fwk");
  initactivities(gActivityGroup);
}

extern "C" QPLUGIN void QARGS_STACK moduleCleanUp (void) throw() 
{
  gRegdClasses.clear();
}