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

#include "cNetManServiceStart.h"
#include <shlobj.h>
#include <Lm.h>

// --- Use Namespace ---------------------------------------------------------------------

using namespace std;
using namespace vfs;

static const String kNetBiosSmb (L"\\Device\\NetbiosSmb");
static const String kNetManEvent (L"Global\\NetManCompleteEvent");

// Function to disconnect a specified transport protocol from the host
// - see MSDN NetServerTransportEnum function
//
bool disconnectTransport(const String &transport)
{
  LPSERVER_TRANSPORT_INFO_0 pBuf = NULL;
  LPSERVER_TRANSPORT_INFO_0 pTmpBuf;
  DWORD dwLevel = 0;
  DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
  DWORD dwEntriesRead = 0;
  DWORD dwTotalEntries = 0;
  DWORD dwResumeHandle = 0;
  DWORD dwTotalCount = 0;
  NET_API_STATUS nStatus;
  LPTSTR pszServerName = NULL;
  DWORD i;
  bool found = false;

  do
  {
    // Call the NetServerTransportEnum function; specify level 0.
    nStatus = NetServerTransportEnum(pszServerName,
                                     dwLevel,
                                     (LPBYTE *) &pBuf,
                                     dwPrefMaxLen,
                                     &dwEntriesRead,
                                     &dwTotalEntries,
                                     &dwResumeHandle);

    if((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA))
    {
      if((pTmpBuf = pBuf) != NULL)
      {
        // Loop through the entries; process access errors
        for(i = 0; i < dwEntriesRead; i++)
        {
          if(pTmpBuf == NULL)
          {
            QSOS((L"An access violation has occurred"));
            break;
          }

          // Print the transport protocol name 
          QTRACE((L"\tFound Transport: %s", pTmpBuf->svti0_transportname));

          // Delete it if it's the one we want
          if(0 == wcscmp(pTmpBuf->svti0_transportname, transport.c_str()))
          {
            NET_API_STATUS status = NetServerTransportDel(pszServerName, dwLevel, (LPBYTE)pTmpBuf);
            if(NERR_Success == status)
            {
              QTRACE((L"\tDeleted %s", transport.c_str()));
              found = true;
            }
            else QSOS((L"\tError %d while deleting %s",status, transport.c_str()));
          }

          pTmpBuf++;
          dwTotalCount++;
        }
      }
    }
    else QSOS((L"A system error has occurred: %d", nStatus));

    // Free the allocated buffer
    if(pBuf != NULL)
    {
       NetApiBufferFree(pBuf);
       pBuf = NULL;
    }
  }
  while(nStatus == ERROR_MORE_DATA); // Continue to call NetServerTransportEnum while there are more entries

  // Check again for an allocated buffer.
  if(pBuf != NULL) NetApiBufferFree(pBuf);

  QTRACE((L"Total of %d entries enumerated", dwTotalCount));

  return found;
}

// =======================================================================================
// === cMyWorkerThread ===================================================================
// =======================================================================================

class cMyWorkerThread : public cThread
{
public :
  QDEFINE_SMARTPTR(cMyWorkerThread);

  cMyWorkerThread()
    : cThread(L"NetManService", iThread::eNormal, 0x100000, vfs::eThreadLogging_Expanded),
      m_HasThrownAnException(false) {}

  void QAPI signalLoopStop() {}

  void rethrowUnhandledException() throw(cRecoverable)
  {
    if(m_HasThrownAnException) throw m_Exception;
  }

private :
  void QAPI runLoop()
  {
    HANDLE event = CreateEvent(NULL, TRUE, FALSE, kNetManEvent.c_str());
    if(!event) QSOS((L"CreateEvent() failed, error %d", GetLastError()));

    disconnectTransport(kNetBiosSmb);
    if(event) 
    {
      QTRACE((L"Sending complete notification"));
      SetEvent(event);
    }
    else QSOS((L"Could not send complete notification"));
  }

  bool m_HasThrownAnException;
  cRecoverable m_Exception;
};

cGuard::ConstPtr cNetManServiceStart::start (const String& cmdLine) throw (cRecoverable)
{
  String LogFolder;

  Char OrigCurrentFolder[MAX_PATH];
  if(GetCurrentDirectory(sizeof(OrigCurrentFolder) / sizeof(Char), OrigCurrentFolder) == 0)
  {
    const DWORD LastError = GetLastError();
    throw cWin32Error(__FILE__, __LINE__, L"Tried to GetCurrentDirectory - returned %d", LastError);
  }

  LogFolder = L"C:\\Data\\Logs\\NetManService\\";

  String CmdLine(cmdLine + L" /hostreg=\"NetManService.reg\" /logfolder=\"" + LogFolder + L"\" /developer=\"vfs\" /product=\"NetManService\" /version=\"1.1.0.0\" /settings=\"\"");
  #if defined(_DEBUG)
    CmdLine += L" /dlls=\"KernelModulesDbg\\\"";
  #else
    CmdLine += L" /dlls=\"KernelModules\\\"";
  #endif
  kernelCreate(CmdLine);

  // So user can find logs easier we'll make a shortcut on the Start menu
  const String MyCommonProgramsFolder = getWin32SpecialFolder(CSIDL_COMMON_PROGRAMS) + L"vfs\\Logs\\";
  iFileManager::singleton().createFolder(MyCommonProgramsFolder);
  try { iFileManager::singleton().deleteFileOrFolder(MyCommonProgramsFolder + L"NetManService" + L".lnk", 0 /* flags */); } catch (cRecoverable &) {}
  createWin32FileShortcut(MyCommonProgramsFolder, L"NetManService", LogFolder, L"" /* params */, L"" /* desc */);

  // Boot kernel - modules loaded and initialised
  cGuard::ConstPtr guard = gKernelMainThread->start();
  gKernelMainThread->waitBootedForever();

  // Bodge to make the console log visible!
  iDLLManager::singleton().getInstance(L"vfs", L"KernelConsoleLogDriver")->setPublicSettingNumber(
    L"User Interface: Tweaks\\Log console visible 2\\",
    1);

  iFileManager::singleton().setCurrentFolder(OrigCurrentFolder);
  mQThread = iThreadManager::singleton().promoteCurrentThread (L"Service");

  cMyWorkerThread::Ptr myWorkerThread = new cMyWorkerThread();
  mMyWorkerThreadGuard = iThreadManager::singleton().startThread(myWorkerThread);

  return new cGuard (this, myWorkerThread);
}

// iGuardCreator
void QAPI cNetManServiceStart::onGuardDestroyed (iGuarded::Ptr guarded)
{
  cMyWorkerThread::Ptr myWorkerThread = ptr_cast<cMyWorkerThread::Ptr>(guarded);

  if (myWorkerThread.isValid())
  {
    try
    {
      myWorkerThread->stopLoop(); // Can throw if thread runLoop had an exception
      myWorkerThread->rethrowUnhandledException();
    }
    catch (cAborted &)
    {
      // ignore exception, because we told the thread to stop
    }

    mMyWorkerThreadGuard.setNull();
  }
}
