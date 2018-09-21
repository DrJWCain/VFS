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

// cQCIFSProcessor.cpp

#include "stdafx.h"

#include "cQCIFSProcessor.h"

#include "cSMBRequest.h"
#include "cSMBResponse.h"
#include "cDFSReferral.h"
#include "cFileComposite.h"
#include "cFolderComposite.h"
#include "cProcessPacketMsg.h"
#include "CompositeExports.h"
#include "cQCIFSPacketProcessor.h"
#include "cShareManager.h"
#include "cOpLockManager.h"
#include "cDFSReferral.h"
#include "cQCIFSPacketProcessor.h"
#include "RebootThread.h"

#include "cFCPGConfigPage.h"

#include "LogCategories.h"
#include "..\..\..\MiscPlugins\QCoSpace\cServiceHelp.h"

#include "..\common\iCommonFactory.h"
#include "..\common\iIOCompletionPort.h"
#include "..\SocketServer\iSocketServer.h"


//class cbVersionLicense : public iLicenseUpdate, public cRefCount 
//{
//public:
//  cbVersionLicense (cQCIFSProcessor* qcifsp) : QcifsP(qcifsp) {}
//  ~cbVersionLicense() {}
//
//  void invalidate() { QcifsP->onLicenseExpired(); }
//
//private:
//  cQCIFSProcessor* QcifsP;
//};


//////////////////////////////
// cQCIFSProcessor ////////////////
//////////////////////////////

extern const String kNetManServiceName;
extern const String kNetManEvent;

static const USHORT listeningOnPorts[] = {139, 445};
static const std::vector<USHORT> listeningPorts(listeningOnPorts
  , listeningOnPorts + sizeof(listeningOnPorts)/sizeof(listeningOnPorts[0]));

tActivityMap ActivityQCIFSProcessors;
iActivityGroup::Ptr gActivityGroup;

tActivityMap ActivityQCIFSProcessors2;
iActivityGroup::Ptr gActivityGroup2;

static int numProcessors()
{
  SYSTEM_INFO sysInfo;
  GetSystemInfo(&sysInfo);
  return sysInfo.dwNumberOfProcessors;
}

//static bool checkOption(const String& optionToTest) 
//{
//  try
//  {
//    iOptionManager::singleton().checkOut (optionToTest);
//    return true;
//  }
//  catch (cRecoverable&)
//  {
//    return false;
//  }
//}

std::map<unsigned int, iActivity::Ptr> ActivityPerThread;
cLock ActivityPerThreadLock;
iActivity::Ptr getActivity()
{
  unsigned int threadId = iThreadManager::singleton().getCurrentThreadId();

  cLockGuard g(&ActivityPerThreadLock);
  if (ActivityPerThread[threadId].isNull()) 
  {
    iActivity::Ptr pActivitySignal = gActivityGroup->addSignal (L"CompletionThread");
    pActivitySignal->addActivity(kGetHeader, L"Get Header");
    pActivitySignal->addActivity(kCreateBuffer, L"Create Buffer");
    pActivitySignal->addActivity(kSMBBuffer, L"Create SMB Buffer");
    pActivitySignal->addActivity(kCreateResponse, L"Create Response");
    pActivitySignal->addActivity(kGetBlocks, L"Get Blocks");
    pActivitySignal->addActivity(kGetDispatchCommand, L"Get Dispatch Command");
    pActivitySignal->addActivity(kGetResponses, L"Get Responses");
    pActivitySignal->addActivity(kSendResponses, L"Send Responses");
    pActivitySignal->addActivity(kRXBuffer, L"RX Buffer");
    pActivitySignal->addActivity(kTXBuffer, L"TX Buffer");
    pActivitySignal->addActivity(kBufferFree, L"Buffer Free");
    ActivityPerThread[threadId] = pActivitySignal;
  }

  return ActivityPerThread[threadId];
}

namespace
{
  //bool testFCPGateway()
  //{
  //  static bool FCPGatewayTestInitialised = false;
  //  static bool FCPGatewayTestResult = false;
  //  static cLock FCPGatewayTestSafety;

  //  if(!FCPGatewayTestInitialised)//Double checked locking pattern 
  //  {
  //    cLockGuard g(&FCPGatewayTestSafety);
  //    if(!FCPGatewayTestInitialised)
  //    {
  //      try
  //      {
  //        iOptionManager::singleton().checkOut (L"FCP Gateway Folders");
  //        FCPGatewayTestResult = true;
  //      }
  //      catch (cRecoverable&)
  //      {
  //        return false;
  //      }
  //      FCPGatewayTestInitialised = true;
  //    }
  //  }
  //  return FCPGatewayTestResult;
  //}
}

cQCIFSProcessor::cQCIFSProcessor()
//: m_pCompletionPort(iCommonFactory::singleton().createIOCP(numProcessors()*2)) // (I read in Jeffery Ritcher that twice the number of actual processors is a good number) - RH
: m_pCompletionPort(iCommonFactory::singleton().createIOCP(numProcessors()))  // This seems overly generous - we only seem to use 1/2 the above - eb=ven under load - JC
, m_pShareManager(new cShareManager())
#if CIFS 
, m_pOpLockManager(new cOpLockManager(false))
#endif // CIFS 
, m_pPacketProcessor(new cQCIFSPacketProcessor(m_pCompletionPort, m_pShareManager, m_pOpLockManager))
{

  //bool canRunProductVersion = false;
  //try 
  //{
  //  // attach a callback to the VersionLicense to alert us if the license goes away
  //  mVersionLicense = iOptionManager::singleton().checkOutVersionType (new cbVersionLicense (this));
  //  canRunProductVersion = true;
  //}
  //catch (cAccessDenied& denied)
  //{
  //  iExceptionManager::singleton().presentWarning (L"%s", denied.getMsg().c_str());
  //}

  //if( canRunProductVersion )
  {
  //  try
  //  {
  //    iOptionManager& optionManager = iOptionManager::singleton();
  //    const String sProductName(optionManager.getProductName());
  //    if(!checkOption(L"Real QCifs"))
  //      iOptionManager::singleton().checkOut(L"SAM on Workstation");
  //  }
  //  catch (const cAccessDenied &)
  //  {
  //    //m_pCompletionPort.invalidate();
  //    //m_pShareManager.invalidate();
  //    //m_pOpLockManager.invalidate();
  //    //m_pPacketProcessor.invalidate();
  //    return;
  //  }

    // Delay starting NetManService until we know we're going to put our own SMB server in place
    OSVERSIONINFOW osVer;
    osVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFOW);
    if(GetVersionEx(&osVer))
    {
      if(osVer.dwMajorVersion >= 6) // Only for Vista, Win7 or Win2k8 or better
      {
        // Start our local machine NetManService
        // This disconnects the SMB transport from the protocol stack
        // It's run as a service as it needs administrator privilege
        cLogIndentGuard indent(kCtgNotify, L"Starting NetManService");
        try
        {
          // Create a global event so the NetManService can signal when it has completed
          HANDLE event = CreateEvent(NULL, TRUE, FALSE, kNetManEvent.c_str());
          if(!event) QSOS((L"NetManService CreateEvent() failed, error %d", GetLastError()));

          startService(kNetManServiceName);

          if(event)
          {
            // Wait for the NetManService complete event before we proceed
            if(WAIT_OBJECT_0 == WaitForSingleObject(event, 1000)) QTRACE((L"NetManService complete notification received"));
            else QSOS((L"NetManService timeout while waiting for complete notification"));
          }
        }
        catch(cRecoverable& err)
        {
          QSOS((L"Failed to start NetManService (service) - %s", err.getMsg().c_str()));
        }
      }
    }

    // Start up QCIFS Processor proper
    QTRACE((L"QCIFS Processor alive"));
    if (m_pCompletionPort.isValid() && m_pPacketProcessor.isValid())
    {
      std::list<int> threadIDs = m_pCompletionPort->getThreadIDs();

      gActivityGroup = iActivityLog::singleton().getGroup(L"QCIFSProcessor");

      //RHRHRH - hopefully none of the thread IDs will be 0 or 1
      iActivity::Ptr pActivitySignal = gActivityGroup->addSignal (L"Tree level");
      pActivitySignal->addActivity(kTreeRead, L"Tree Read");
      pActivitySignal->addActivity(kTreeClose, L"Tree Close GetComposite");
      pActivitySignal->addActivity(kTreeCloseRemove, L"Tree Close Remove");
      pActivitySignal->addActivity(kTreeCloseCallComposite, L"Tree Close Call Composite");
      pActivitySignal->addActivity(kCreateBuffer, L"Post Message");
      ActivityQCIFSProcessors.insert(tActivityMap::value_type(0, pActivitySignal));

      pActivitySignal = gActivityGroup->addSignal (L"Process level");
      pActivitySignal->addActivity(kProcessClose, L"Process Close");
      pActivitySignal->addActivity(kCompositeRead, L"Composite Read");
      ActivityQCIFSProcessors.insert(tActivityMap::value_type(1, pActivitySignal));

      gActivityGroup2 = iActivityLog::singleton().getGroup(L"SMB3 Processor");

      pActivitySignal = gActivityGroup2->addSignal (L"NIC 0 Read");
      pActivitySignal->addActivity(0, L"Sess:0");
      pActivitySignal->addActivity(1, L"Sess:1");
      pActivitySignal->addActivity(2, L"Sess:2");
      pActivitySignal->addActivity(3, L"Sess:3");
      ActivityQCIFSProcessors2.insert(tActivityMap::value_type(0, pActivitySignal));

      pActivitySignal = gActivityGroup2->addSignal (L"NIC 1 Read");
      pActivitySignal->addActivity(0, L"Sess:0");
      pActivitySignal->addActivity(1, L"Sess:1");
      pActivitySignal->addActivity(2, L"Sess:2");
      pActivitySignal->addActivity(3, L"Sess:3");
      ActivityQCIFSProcessors2.insert(tActivityMap::value_type(1, pActivitySignal));

      //int i=0;
      //for (std::list<int>::iterator it = threadIDs.begin()
      //  ; it != threadIDs.end()
      //  ; ++it, i++)
      //{
      //  std::wstring sThreadID(L"CompletionThread " + i);
      //  iActivity::Ptr pActivitySignal = gActivityGroup->addSignal (sThreadID);
      //  pActivitySignal->addActivity(kGetHeader, L"Get Header");
      //  pActivitySignal->addActivity(kCreateBuffer, L"Create Buffer");
      //  pActivitySignal->addActivity(kSMBBuffer, L"Create SMB Buffer");
      //  pActivitySignal->addActivity(kCreateResponse, L"Create Response");
      //  pActivitySignal->addActivity(kGetBlocks, L"Get Blocks");
      //  pActivitySignal->addActivity(kGetDispatchCommand, L"Get Dispatch Command");
      //  pActivitySignal->addActivity(kGetResponses, L"Get Responses");
      //  pActivitySignal->addActivity(kSendResponses, L"Send Responses");
      //  ActivityQCIFSProcessors.insert(tActivityMap::value_type(*it, pActivitySignal));
      //}

      try
      {
        m_pSocketServer = iSocketServerFactory::singleton().createSocketServer(m_pPacketProcessor, listeningPorts);
      }
      catch (const cNotFound& nf)
      {
        QSOS((L"cQCIFSProcessor::cQCIFSProcessor() - caught cNotFound trying to access cSocketServer singleton"));
        QSOS((L"cQCIFSProcessor::cQCIFSProcessor() - %s", nf.getMsg().c_str()));
      }
    }
    
    //if(testFCPGateway())
    //{
    //  m_pRebootThread = new cRebootThread (this);
    //  m_pRebootThread->startLoop ();
    //  new cFCPGConfigPage(m_pSocketServer, m_pRebootThread);
    //}
  }
}

cQCIFSProcessor::~cQCIFSProcessor(void)
{
  QSOS((L"cQCIFSProcessor dead"));
}

//bool cQCIFSProcessor::attachResource(const cPtr<iTreeResourceFactory> pTreeResourceFactory)
//{
//  return m_pShareManager->Add(pTreeResourceFactory, true);
//}

bool cQCIFSProcessor::attachResource(const String& sShareName
  , const cPtr<iTreeResourceFactory> pTreeResourceFactory
  , bool bReplaceExisting)
{
  return m_pShareManager->Add(pTreeResourceFactory, sShareName, bReplaceExisting);
}

cPtr<iComposite> cQCIFSProcessor::getFileComposite(const std::wstring &sPath
  , DWORD dwDesiredAccess
  , DWORD dwShareMode
  , LPSECURITY_ATTRIBUTES lpAtts
  , DWORD dwCreationDisposition
  , DWORD dwFlagsAndAtts
  , HANDLE hTemplateFile
  , LPBY_HANDLE_FILE_INFORMATION pFileInfo
  , DWORD &err)
{
  return new cFileComposite(sPath
    , dwDesiredAccess
    , dwShareMode
    , lpAtts
    , dwCreationDisposition
    , dwFlagsAndAtts
    , hTemplateFile
    , pFileInfo
    , NULL//m_pCompletionPort.getRaw()
    , err);
}

cPtr<iComposite> cQCIFSProcessor::getFolderComposite(const std::wstring ksFilePath
  , LPSECURITY_ATTRIBUTES lpAtts
  , LPBY_HANDLE_FILE_INFORMATION pFileInfo
  , DWORD &err)
{
  return new cFolderComposite(ksFilePath
    , lpAtts
    , pFileInfo
    , NULL//m_pCompletionPort.getRaw()
    , err);
}

HANDLE *cQCIFSProcessor::AddFileHandle(HANDLE hFile)
{
  return m_pCompletionPort->AddFileHandle(hFile);
}

void cQCIFSProcessor::dumpBuffers()
{
  m_pPacketProcessor->dumpBuffers();
}

void cQCIFSProcessor::onLicenseExpired()
{
  QSOS((L"The license period for this software has expired.")); //There is no gui so just log the expiration
}