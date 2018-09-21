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

//cFolderComposite.cpp

#include "StdAfx.h"

#include ".\cfoldercomposite.h"

#include "..\Common\iIOCompletionPort.h"

using namespace vfs;

HANDLE createFolderHandle(const String& sPath, DWORD& err)
{
  HANDLE hFolder = INVALID_HANDLE_VALUE;
  if (0 == CreateDirectory(sPath.c_str(), NULL))
  {
    err = GetLastError();
    QTRACE((L"Failed to create directory {%s}- error code: [%d]", sPath.c_str(), err));
  }
  else
  {
    // created the directory successfully, get file attributes for SMB reply
    hFolder = CreateFile(sPath.c_str()
      , FILE_LIST_DIRECTORY
      , FILE_SHARE_READ
      , NULL
      , OPEN_EXISTING
      , FILE_FLAG_BACKUP_SEMANTICS// | FILE_FLAG_OVERLAPPED
      , NULL);
    if (INVALID_HANDLE_VALUE == hFolder)
      err = GetLastError();
    else
      err = ERROR_SUCCESS;
  }
  return hFolder;
}

cFolderComposite::cFolderComposite(const std::wstring ksFilePath
  , LPSECURITY_ATTRIBUTES lpAtts
  , LPBY_HANDLE_FILE_INFORMATION pFileInfo
  , iIOCompletionPort *pIOCP
  , DWORD &err)
: cWindowsComposite(ksFilePath, createFolderHandle(ksFilePath, err), pIOCP)
{
}

cFolderComposite::~cFolderComposite(void)
{
}

  /*** Read - return number of bytes read in ***/
DWORD cFolderComposite::Read(tTransmitList &krTPM, DWORD& nBytes, const LARGE_INTEGER &nOffset, const int sessionID, ULONGLONG fid)
{
  return ERROR_ACCESS_DENIED;
}

  /*** Write - return number of bytes written ***/
DWORD cFolderComposite::Write(vfs::cConstPtr<vfs::cMemoryView> buffer, DWORD& nBytes, const LARGE_INTEGER &offset, const int sessionID,ULONGLONG fid)
{
  return ERROR_ACCESS_DENIED;
}

DWORD cFolderComposite::SetFileSize(const PLARGE_INTEGER pFileSize)
{
  return ERROR_ACCESS_DENIED;
}

DWORD cFolderComposite::SetFileInfo(const FILETIME& creationTime
  , const FILETIME& modifiedTime, const FILETIME& accessTime, const DWORD attributes)
{
  return ERROR_ACCESS_DENIED;
}

/*** Lock - return 0 for success or error code ***/
DWORD cFolderComposite::Lock(const LARGE_INTEGER &lgOffset, const LARGE_INTEGER &lgBytes, const bool bLock)
{
  return ERROR_ACCESS_DENIED;
}