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

//cBaseDirLocation.h

#pragma once

#include "iDirLocation.h"

class cBaseDirLocation : public iDirLocation
{
public:
  virtual vfs::cPtr<iComposite> Create(const vfs::String& sName
    , DWORD dwDesiredAccess
    , DWORD dwShareMode
    , LPSECURITY_ATTRIBUTES lpAtts
    , DWORD dwCreationDisposition
    , DWORD dwFlagsAndAtts
    , HANDLE hTemplateFile
    , DWORD& dwRet
    , vfs::cPtr<iFileProcessingCallback> pCallback) {return vfs::cPtr<iComposite>();}
  virtual vfs::cPtr<iRename> Rename() {return vfs::cPtr<iRename>();}
  virtual DWORD Rename(const vfs::cPtr<iRename> pSourceRename
    , const bool bToNewChild
    , const vfs::String& sNewName
    , const vfs::String& sUserName) {return ERROR_ACCESS_DENIED;}
  virtual DWORD writeInfo(const PfileInfo pFileInfo) {return ERROR_ACCESS_DENIED;}
};