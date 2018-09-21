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

#include "cQCIFSTest.h"
#include "cSpaceQuery.h"

#include "..\QCIFSFwk\iQCIFSFwk.h"
#include "..\QCIFSProcessor\iQCIFSProcessor.h"
#include "..\QCIFSProcessor\iTreeResourceFactory.h"
#include "..\qcifsfwk\iChildLoaderVisitor.h"

using namespace vfs;

static cPtr<iTreeResourceFactory> gpProcFactory;

iModuleContext::Ptr gPluginContext ;
static std::list<cGuard::ConstPtr> gRegdClasses;

// plugIn interface
extern "C" QPLUGIN void QARGS_STACK moduleInit(iModuleContext::Ptr pContext) throw( cRecoverable)
{
  // Throw a cRecoverable with an appropriate message to tell the Kernel that initialisation
  // has failed and it is safe to continue.

  gPluginContext = pContext;

  gRegdClasses.push_back (gFactory->registerClass (new cQCIFSTest::class_ctor));

  try
  {
    const cPtr<iSpaceQuery> pSpaceQuery = new cISASpaceQuery();

    SYSTEM_INFO sysInfo;
    ::GetSystemInfo(&sysInfo);
    WCHAR OEMIDBuffer[MAX_PATH];
    const String sOEMID(widen(_itow(sysInfo.dwOemId, OEMIDBuffer, 10)));

    const cPtr<iTreeResourceFactory> pProcFactory
      = iQCIFSFwk::singleton().createTreeResourceFactory(
          pSpaceQuery
        , iQCIFSTest::singleton().createRootChildLoader()
        , sOEMID
        , DEFAULT_SECTOR_SIZE
        , DEFAULT_CLUSTER_SIZE
        , cPtr<iNameMangler>()
        , cPtr<iNamespaceCreator>());

    if (!iQCIFSProcessor::singleton().attachResource(iQCIFSTest::singleton().shareName(), pProcFactory, false))
      QSOS((L"iQCIFSTest::Main - failed to attach iQCIFSTest to QCIFS Processor"));
    iQCIFSProcessor::singleton().attachResource(L"ShareFolder1", pProcFactory, false);
    iQCIFSProcessor::singleton().attachResource(L"ShareFolder2", pProcFactory, false);
    iQCIFSProcessor::singleton().attachResource(L"ShareFolder3", pProcFactory, false);
    iQCIFSProcessor::singleton().attachResource(L"ShareFolder4", pProcFactory, false);
  }
  catch (const cNotFound& nf)
  {
    QSOS((L"iQCIFSTest::Main - doneInitialLoad() - %s", nf.getMsg().c_str()));
  }
}

extern "C" QPLUGIN void QARGS_STACK moduleCleanUp (void) throw() 
{
  gRegdClasses.clear();
}