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

//cWriteInfo.h

#pragma once

#include "iWriteInfo.h"

class cWriteInfo : public iWriteInfo, public vfs::cRefCount
{
public:
  QDEFINE_SINGLETON_IMPL(cWriteInfo, L"{7612717D-46B4-49eb-8156-BEE98BCD221B} // cWriteInfo", iWriteInfo);

  void convertDateTimetoFiletime(const vfs::cDateTime &dt, FILETIME& pFT) const;

  DWORD getAtts(const bool bDirectory, const bool bReadonly) const;
  fileInfo makeFileInfo(const DWORD dwAtts
    , const vfs::String& sName) const;
  fileInfo makeFileInfo(const DWORD dwAtts
    , const vfs::cConstPtr<iNamed> pNamed) const;
  fileInfo makeFileInfo(const DWORD dwAtts
    , const vfs::String& sName
    , const FILETIME ft) const;

  void writeFileInfo(PfileInfo pFileInfo
    , const __int64& fileSize
    , const FILETIME& rCreatedTime
    , const FILETIME& rAccessTime
    , const FILETIME& rWriteTime
    , const DWORD dwAtts
    , const vfs::String& sName) const;
  void writeFileInfo(PfileInfo pFileInfo
    , const BY_HANDLE_FILE_INFORMATION& rBHI
    , const vfs::String& sName) const;
  void writeFileInfo(const LPBY_HANDLE_FILE_INFORMATION pBHI
    , const fileInfo* const pFileInfo) const;

private:
  inline fileInfo makeFileInfo(const DWORD dwAtts, const FILETIME ft) const;
};