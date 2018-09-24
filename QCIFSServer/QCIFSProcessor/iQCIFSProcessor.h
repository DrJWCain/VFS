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

//iQCIFSProcessor.h

#pragma once

#include "iTreeResourceFactory.h"
#include "..\SocketServer\iPacketProcessor.h"

class iComposite;

class iQCIFSProcessor : public vfs::iManager
{
public:
  QDEFINE_SINGLETON_IFACE(iQCIFSProcessor, L"{983F8AE5-3C8B-4181-B9BB-AE0C924072F7} // iQCIFSProcessor", vfs::iManager);

  virtual ~iQCIFSProcessor() {}

  //virtual bool attachResource(const vfs::cPtr<iTreeResourceFactory> pTreeResourceFactory) = 0;
  virtual bool attachResource(const vfs::String& sShareName
    , const vfs::cPtr<iTreeResourceFactory> pTreeResourceFactory
    , bool bReplaceExisting) = 0;

  virtual vfs::cPtr<iComposite> getFileComposite(const std::wstring &sPath
    , DWORD dwDesiredAccess
    , DWORD dwShareMode
    , LPSECURITY_ATTRIBUTES lpAtts
    , DWORD dwCreationDisposition
    , DWORD dwFlagsAndAtts
    , HANDLE hTemplateFile
    , LPBY_HANDLE_FILE_INFORMATION pFileInfo
    , DWORD &err) = 0;

  virtual vfs::cPtr<iComposite> getFolderComposite(const std::wstring ksFilePath
    , LPSECURITY_ATTRIBUTES lpAtts
    , LPBY_HANDLE_FILE_INFORMATION pFileInfo
    , DWORD &err) = 0;

  virtual void dumpBuffers() = 0;
};