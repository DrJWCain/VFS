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

//iDirectoryFactory.h

#pragma once

#include "..\iTreeResource.h"

class iDirLocation;
class iComposite;

class iDirectoryFactory
{
public:
  virtual ~iDirectoryFactory(){}

  virtual vfs::cPtr<iDirLocation> get(const vfs::String& sPath) const = 0;
  virtual vfs::cPtr<iDirLocation> getParent(const vfs::String& sPath) const = 0;
  virtual DWORD getSearchResults(const vfs::String& sSearchPath, tFileInfoList& fileInfoList) const = 0;

  virtual vfs::cPtr<iComposite> CreateFolder(const vfs::String& sPath
    , LPSECURITY_ATTRIBUTES lpAtts
    , LPBY_HANDLE_FILE_INFORMATION pFileInfo
    , DWORD &err) = 0;
  virtual DWORD DeleteFolder(const vfs::String& sPath) = 0;
};