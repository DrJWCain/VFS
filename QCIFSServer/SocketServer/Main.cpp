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

//////////////////////////////////////////////////////////////////////////////
// Main.cpp

#include "stdafx.h"

#include "cSocketServer.h"
#include "cAcceptEx.h"
#include "cConnectEx.h"

#include "..\..\..\KernelSDK\include\QWinBase.h"

// =======================================================================================
// === moduleInit/moduleCleanUp ==========================================================
// =======================================================================================
using namespace vfs;

iModuleContext::Ptr gModuleContext;
iActivityGroup::Ptr gActivityGroup;

void initActivitySocketServer(iActivityGroup::Ptr activityGroup);

extern "C" QPLUGIN void QARGS_STACK moduleInstall(const int Flags, iModuleContext::Ptr Incoming, iModuleContext::Ptr OptionalOutgoing, iModuleInstallHelp::Ptr Help) throw(cRecoverable)
{
//  iRegistryHelp::Ptr rh = createRegistryHelp();
//  rh->setItemNumber (L"SYSTEM\\CurrentControlSet\\Services\\NetBT\\Parameters\\", L"SMBDeviceEnabled", 0);
//  rh->setItemNumber (L"SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\", L"TcpWindowSize", 0xfffff);
}

extern "C" QPLUGIN void QARGS_STACK moduleInit(iModuleContext::Ptr pContext) throw( cRecoverable)
{
  static std::list<cGuard::ConstPtr> gRegdClasses;
  gRegdClasses.push_back(gFactory->registerClass(new cSocketServerFactory::class_ctor));
  gRegdClasses.push_back(gFactory->registerClass(new cAcceptEx::class_ctor));
  gRegdClasses.push_back(gFactory->registerClass(new cConnectEx::class_ctor));

  gActivityGroup = iActivityLog::singleton().getGroup(L"QCIFSSocketServer");
  initActivitySocketServer(gActivityGroup);
}

extern "C" QPLUGIN void QARGS_STACK moduleUninstall(iModuleContext::Ptr Outgoing, iModuleInstallHelp::Ptr Help, const bool IsRemovingSettingsAndData) throw(cRecoverable, cIgnoreInstallFailed)
{
//  iRegistryHelp::Ptr rh = createRegistryHelp();
//  rh->deleteItem (L"SYSTEM\\CurrentControlSet\\Services\\NetBT\\Parameters\\", L"SMBDeviceEnabled");
}