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

// cFCPGConfigPage.h
// 
//

#pragma once

//#include "..\..\..\miscPlugins\HttpServer\CGI.h"
//#include "..\..\..\miscPlugins\HttpServer\iHttpServer.h"
//#include "..\SocketServer\iSocketServer.h"
//#include "RebootThread.h"
//
//class cAdminPage : public iHttpPage
//{
//public:
//  cAdminPage();
//  
//protected:
//  // iHttpPage
//  bool generateContent(CGICALL* pContext);
//  eAuthenticationLevel getAuthLevel() { return eHttpAuthHigh; } 
//
//private:
//  bool addRebootButton(cHtmlGenerator& html);
//  DWORD softwareShutdownKey;
//
//  class cShutdownThread : public vfs::cThread
//  {
//  public:
//    cShutdownThread(bool success) : mSuccess(success), cThread(L"Shutdown Thread", cThread::eNormal) {}
//    void QAPI signalLoopStop() {}
//    void QAPI runLoop() 
//    {
//      QTRACE((L"-= Web user requests %s - Terminating Kernel =-", mSuccess?L"shutdown":L"restart"));
//
//      Sleep(5000);
//      
//      vfs::iExceptionManager::singleton().fatal(0, /* flags */
//        vfs::iExceptionManager::cFatalDiagnostics(mSuccess?EXIT_SUCCESS:EXIT_FAILURE,
//          "" /* src file */, 0 /* src line */, L"" /* heading */, L"Closing"));
//    }
//  private:
//    bool mSuccess;
//  };
//};
//
//
//class cFCPGConfigPage : public iHttpPage
//{
//public:
//  cFCPGConfigPage( vfs::cPtr<iSocketServer> pSocketServer, vfs::cPtr<cRebootThread> pRebootThread);
//  
//protected:
//  // iHttpPage
//  bool generateContent(CGICALL* pContext);
//  eAuthenticationLevel getAuthLevel() { return eHttpAuthNone; } 
//
//private:
//   void createWebUI();
//   vfs::cPtr<iSocketServer> mSocketServer;
//   vfs::cPtr<cRebootThread> mRebootThread;
//};
