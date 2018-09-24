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

//cFolderComposite.h

#pragma once

#include "cWindowsComposite.h"

class cFolderComposite : public cWindowsComposite
{
public:
  cFolderComposite(const std::wstring ksFilePath
    , LPSECURITY_ATTRIBUTES lpAtts
    , LPBY_HANDLE_FILE_INFORMATION pFileInfo
    , iIOCompletionPort *pIOCP
    , DWORD &err);
  ~cFolderComposite(void);

  /*** Read - return number of bytes read in ***/
  DWORD Read(tTransmitList &krTPM, DWORD& nBytes, const LARGE_INTEGER &nOffset, const int sessionID, ULONGLONG fid);
  /*** Write - return number of bytes written ***/
  DWORD Write(vfs::cConstPtr<vfs::cMemoryView> buffer, DWORD& nBytes, const LARGE_INTEGER &offset, const int sessionID, ULONGLONG fid);
  DWORD SetFileSize(const PLARGE_INTEGER pFileSize);
  DWORD SetFileInfo(const FILETIME& creationTime
    , const FILETIME& modifiedTime, const FILETIME& accessTime, const DWORD attributes);
  /*** Lock - return 0 for success or error code ***/
  DWORD Lock(const LARGE_INTEGER &lgOffset, const LARGE_INTEGER &lgBytes, const bool bLock);
};
