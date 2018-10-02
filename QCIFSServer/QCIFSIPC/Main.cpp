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

#include "cQCIFSIPC.h"
#include "cSpaceQuery.h"
#include "cIPCLoader.h"

#include "..\QCIFSFwk\iQCIFSFwk.h"
#include "..\QCIFSFwk\iNameMangler.h"
#include "..\QCIFSProcessor\iQCIFSProcessor.h"
//#include "..\..\..\MiscPlugins\QDatabase\iBlob.h"
//#include "..\..\..\MiscPlugins\QDatabase\iInfoPointer.h"
//#include "..\..\..\MiscPlugins\QDatabase\iRushFactory.h"

using namespace vfs;

iModuleContext::Ptr gPluginContext ;
static std::list<cGuard::ConstPtr> gRegdClasses;

long runSrvSvc();
class cMyRPCThread : public vfs::cThread
{
public: 
  cMyRPCThread() : vfs::cThread(L"RpcSrvSvcThread") {}
  virtual void QAPI signalLoopStop() {}
  virtual void QAPI runLoop() 
  {
    QTRACE((L"Calling runSrvSvc"));
    runSrvSvc();
  }
};

cPtr<cMyRPCThread> RPCSrvSvcThread;

// plugIn interface
extern "C" QPLUGIN void QARGS_STACK moduleInit(iModuleContext::Ptr pContext) throw( cRecoverable)
{
  // Throw a cRecoverable with an appropriate message to tell the Kernel that initialisation
  // has failed and it is safe to continue.

  gPluginContext = pContext;

  RPCSrvSvcThread = new cMyRPCThread();
  RPCSrvSvcThread->startLoop();

  //register cQCIFS classes
  gRegdClasses.push_back (gFactory->registerClass (new cQCIFSIPC::class_ctor));

#define QCIFS_DEVELOPMENT
#ifdef QCIFS_DEVELOPMENT
    try
    {
      const cPtr<iSpaceQuery> pSpaceQuery = new cSpaceQuery();

      SYSTEM_INFO sysInfo;
      ::GetSystemInfo(&sysInfo);
      WCHAR OEMIDBuffer[MAX_PATH];
      const String sOEMID(widen(_itow(sysInfo.dwOemId, OEMIDBuffer, 10)));

      const cPtr<iTreeResourceFactory> pIPCFactory
        = iQCIFSFwk::singleton().createTreeResourceFactory(pSpaceQuery
        , (cPtr<iChildLoader>)new cIPCLoader()
        , sOEMID
        , DEFAULT_SECTOR_SIZE
        , DEFAULT_CLUSTER_SIZE
        , cPtr<iNameMangler>()
        , cPtr<iNamespaceCreator>());

      if (!iQCIFSProcessor::singleton().attachResource(L"IPC$", pIPCFactory, false))
        QSOS((L"iQCIFSProc::Main - failed to attach iQCIFSProc to QCIFS Processor"));
    }
    catch (const cNotFound& nf)
    {
      QSOS((L"IPC::Main - doneInitialLoad() - %s", nf.getMsg().c_str()));
    }
#endif //QCIFS_DEVELOPMENT
}

extern "C" QPLUGIN void QARGS_STACK moduleCleanUp (void) throw() 
{
  gRegdClasses.clear();
}
