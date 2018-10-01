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

//cAttributeComposite.h

#pragma once

#include "..\icomposite.h"

class cAttributeComposite : public iComposite
{
public:
  cAttributeComposite(const fileInfo& fi);
  ~cAttributeComposite(void);

  //iComposite
  vfs::String Name() const;
  DWORD writeInfo(LPBY_HANDLE_FILE_INFORMATION pFileInfo, ULONGLONG fid);
  DWORD writeInfo(PfileInfo pFileInfo, ULONGLONG fid);
  DWORD SetFileInfo(const FILETIME& creationTime
    , const FILETIME& modifiedTime
    , const FILETIME& accessTime
    , const DWORD attributes);
  DWORD Close(ULONGLONG fid) {return ERROR_SUCCESS;}
  DWORD Lock(const LARGE_INTEGER &lgOffset, const LARGE_INTEGER &lgBytes, const bool bLock) {return ERROR_SUCCESS;}
  void getChildren(const vfs::String& sSearchMask, tFileInfoList& searchResults){}
  bool find(const vfs::String& name, tFileInfoList& searchResults){return false;}
  DWORD Rename(const vfs::String& sNewName){return ERROR_ACCESS_DENIED;}

  //public member functions
  DWORD atts() const;

  vfs::cPtr<iTransactCallback> getTransactInterface() override;

protected:
  void access();
  void modify();

protected:
  mutable vfs::cLock m_fiAccess;
  fileInfo m_fileInfo;
};