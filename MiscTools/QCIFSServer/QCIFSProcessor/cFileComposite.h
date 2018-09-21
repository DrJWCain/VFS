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

//cFileComposite.h

#pragma once

#include "cWindowsComposite.h"

class cFileComposite : public cWindowsComposite
{
public:
  cFileComposite(const std::wstring &krsPath
    , DWORD dwDesiredAccess
    , DWORD dwShareMode
    , LPSECURITY_ATTRIBUTES lpAtts
    , DWORD dwCreationDisposition
    , DWORD dwFlagsAndAtts
    , HANDLE hTemplateFile
    , LPBY_HANDLE_FILE_INFORMATION pFileInfo
    , iIOCompletionPort *pIOCP
    , DWORD &err);
  ~cFileComposite(void);

  DWORD Read(tTransmitList &krTPM, DWORD& nBytes, const LARGE_INTEGER &nOffset, const int sessionID, ULONGLONG fid);
  DWORD Write(vfs::cConstPtr<vfs::cMemoryView> buffer, DWORD &nBytes, const LARGE_INTEGER &offset, const int sessionID, ULONGLONG fid);
  DWORD SetFileSize(const PLARGE_INTEGER pFileSize);
  DWORD SetFileInfo(const FILETIME& creationTime
    , const FILETIME& modifiedTime, const FILETIME& accessTime, const DWORD attributes);
  DWORD Lock(const LARGE_INTEGER &lgOffset, const LARGE_INTEGER &lgBytes, const bool bLock);

private:  //private member functions
  void setFilePointer(const LARGE_INTEGER& rFP);

private:  //private member variables
  LARGE_INTEGER m_FileSize;
  vfs::cLock m_FPAccess;
  LARGE_INTEGER m_filePointer;
};