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

// cQCIFSProcessor.h

#pragma once

#include "iQCIFSProcessor.h"

//#include "..\..\..\MiscPlugins\QOptions\QOptions.h"

#include "..\common\primatives.h"


class cSMBResponse;
class cSMBRequest;
class iTreeResource;
class iIOCompletionPort;
class iCompletionMsg;
class iShareManager;
class iOpLockManager;
class iPacketProcessor;
class iSocketServer;
//class cRebootThread;

class cQCIFSProcessor : public iQCIFSProcessor, public vfs::cRefCount
{
public:
  QDEFINE_SINGLETON_IMPL(cQCIFSProcessor, L"{812CED76-0F0D-45d9-8703-0F84076BBD32} // cQCIFSProcessor", iQCIFSProcessor);

  cQCIFSProcessor();
  ~cQCIFSProcessor(void);

  void QAPI logSummary() {}
  void QAPI logDetailedSummary() {}

  //iQCIFSProcessor
  //bool attachResource(const vfs::cPtr<iTreeResourceFactory> pTreeResourceFactory);
  bool attachResource(const vfs::String& sShareName
    , const vfs::cPtr<iTreeResourceFactory> pTreeResourceFactory
    , bool bReplaceExisting);

  vfs::cPtr<iComposite> getFileComposite(const vfs::String &sPath
    , DWORD dwDesiredAccess
    , DWORD dwShareMode
    , LPSECURITY_ATTRIBUTES lpAtts
    , DWORD dwCreationDisposition
    , DWORD dwFlagsAndAtts
    , HANDLE hTemplateFile
    , LPBY_HANDLE_FILE_INFORMATION pFileInfo
    , DWORD &err);

  vfs::cPtr<iComposite> getFolderComposite(const vfs::String ksFilePath
    , LPSECURITY_ATTRIBUTES lpAtts
    , LPBY_HANDLE_FILE_INFORMATION pFileInfo
    , DWORD &err);

  void dumpBuffers();

  //public
  HANDLE *AddFileHandle(HANDLE hFile);

  void onLicenseExpired();

private:
  const vfs::cPtr<iIOCompletionPort> m_pCompletionPort;
  const vfs::cPtr<iShareManager> m_pShareManager;
  const vfs::cPtr<iOpLockManager> m_pOpLockManager;
  const vfs::cPtr<iPacketProcessor> m_pPacketProcessor;
  vfs::cPtr<iSocketServer> m_pSocketServer;
  //vfs::cPtr<cRebootThread> m_pRebootThread;

  //vfs::iLicenseGuard::ConstPtr mVersionLicense;

};