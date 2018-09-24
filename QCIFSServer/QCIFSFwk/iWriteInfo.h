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

//iWriteInfo.h

#pragma once

#include "..\iComposite.h"

class iNamed;

class iWriteInfo : public vfs::iManager
{
public:
  QDEFINE_SINGLETON_IFACE(iWriteInfo, L"{5E2E20D9-AA0D-4b90-A877-3A6816B7AE3B} //iWriteInfo", vfs::iManager);

  virtual ~iWriteInfo(){}

  void QAPI logSummary(void) throw() {QTRACE((L"iWriteInfo"));}
  void QAPI logDetailedSummary(void) throw() {QTRACE((L"iWriteInfo - QCIFS file info class"));}

  virtual void convertDateTimetoFiletime(const vfs::cDateTime &dt, FILETIME& pFT) const = 0;

  virtual DWORD getAtts(const bool bDirectory, const bool bReadonly) const = 0;
  virtual fileInfo makeFileInfo(const DWORD dwAtts
    , const vfs::String& sName) const = 0;
  virtual fileInfo makeFileInfo(const DWORD dwAtts
    , const vfs::String& sName
    , const FILETIME ft) const = 0;

  virtual void writeFileInfo(PfileInfo pFileInfo
    , const __int64& fileSize
    , const FILETIME& rCreatedTime
    , const FILETIME& rAccessTime
    , const FILETIME& rWriteTime
    , const DWORD dwAtts
    , const vfs::String& sName) const = 0;
  virtual void writeFileInfo(PfileInfo pFileInfo
    , const BY_HANDLE_FILE_INFORMATION& rBHI
    , const vfs::String& sName) const = 0;
  virtual void writeFileInfo(const LPBY_HANDLE_FILE_INFORMATION pBHI
    , const fileInfo* const pFileInfo) const = 0;
};