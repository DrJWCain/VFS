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

//cRealDirLocation.h

#pragma once

#include "iDirLocation.h"

class cRealDirLocation : public iDirLocation, public vfs::cRefCount
{
public:
  cRealDirLocation(const vfs::String& sPath);
  ~cRealDirLocation(void);

  vfs::cPtr<iComposite> Create(const vfs::String& sName
    , DWORD dwDesiredAccess
    , DWORD dwShareMode
    , LPSECURITY_ATTRIBUTES lpAtts
    , DWORD dwCreationDisposition
    , DWORD dwFlagsAndAtts
    , HANDLE hTemplateFile
    , DWORD& dwRet
    , vfs::cPtr<iFileProcessingCallback> pCallback);
  vfs::cPtr<iRename> Rename();
  DWORD Rename(const vfs::cPtr<iRename> pSourceRename
    , const vfs::String& sNewName
    , const vfs::String& sUserName);
  void Rename(const vfs::String& sNewName){}
  DWORD writeInfo(const PfileInfo pFileInfo, ULONGLONG fid);
  void refreshCache();

private:
  const vfs::String m_sPath;
};
