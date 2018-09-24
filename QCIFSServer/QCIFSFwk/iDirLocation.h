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

//iDirLocation.h

#pragma once

#include "..\iTreeResource.h"

#include "iRename.h"

class iDirLocation
{
public:
  virtual ~iDirLocation(){}

  virtual vfs::cPtr<iComposite> Create(const vfs::String& sName
    , DWORD dwDesiredAccess
    , DWORD dwShareMode
    , LPSECURITY_ATTRIBUTES lpAtts
    , DWORD dwCreationDisposition
    , DWORD dwFlagsAndAtts
    , HANDLE hTemplateFile
    , DWORD& dwRet
    , vfs::cPtr<iFileProcessingCallback> pCallback) = 0;
  virtual vfs::cPtr<iRename> Rename() = 0;
  virtual DWORD Rename(const vfs::cPtr<iRename> pSourceRename
    , const vfs::String& sNewName
    , const vfs::String& sUserName) = 0;
  virtual void Rename(const vfs::String& sNewName) = 0;
  virtual DWORD writeInfo(const PfileInfo pFileInfo, ULONGLONG fid) = 0;
  virtual void refreshCache() = 0;
};