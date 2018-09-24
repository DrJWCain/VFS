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

//cWindowsComposite.cpp

#include "StdAfx.h"

#include ".\cwindowscomposite.h"
#include "cNotificationMsg.h"

#include "..\common\iIOCompletionPort.h"
#include "..\..\KernelSDK\include\QWinBase.h"

using namespace vfs;

cWindowsComposite::cWindowsComposite(const vfs::String& sName
  , const HANDLE h
  , iIOCompletionPort* const pIOCP)
: m_sPath(sName)
, m_hFile(h)
, m_pCompletionHandle(INVALID_HANDLE_VALUE == h || NULL == pIOCP ? NULL : pIOCP->AddFileHandle(h))
{
}

cWindowsComposite::~cWindowsComposite(void)
{
  Close(0);
}

DWORD cWindowsComposite::writeInfo(LPBY_HANDLE_FILE_INFORMATION pFileInfo, ULONGLONG fid)
{
  DWORD dwRet(ERROR_SUCCESS);
  if (m_hFile != INVALID_HANDLE_VALUE)
  {
    if (GetFileInformationByHandle(m_hFile, pFileInfo) == FALSE)
      dwRet = GetLastError();
  }
  return dwRet;
}

DWORD cWindowsComposite::writeInfo(PfileInfo pFileInfo, ULONGLONG fid)
{
  DWORD dwRet(ERROR_ACCESS_DENIED);
  if (m_hFile != INVALID_HANDLE_VALUE)
  {
    HANDLE hFind = FindFirstFile(m_sPath.c_str(), &(pFileInfo->w32Data));
    if (hFind != INVALID_HANDLE_VALUE)
    {
      pFileInfo->allocSize.HighPart = pFileInfo->w32Data.nFileSizeHigh;
      pFileInfo->allocSize.LowPart = pFileInfo->w32Data.nFileSizeLow;
      FindClose(hFind);
      dwRet = ERROR_SUCCESS;
    }
  }
  return dwRet;
}

DWORD cWindowsComposite::Close(ULONGLONG fid)
{
  DWORD dwRet = ERROR_SUCCESS;
  if (m_hFile != INVALID_HANDLE_VALUE)
  {
    if (CloseHandle(m_hFile) == FALSE)
      dwRet = GetLastError();
    else
      m_hFile = INVALID_HANDLE_VALUE;
  }

  if (NULL != m_pCompletionHandle)
  {
    delete m_pCompletionHandle;
    m_pCompletionHandle = NULL;
  }

  return dwRet;
}

DWORD cWindowsComposite::Delete()
{
  DWORD dwRet(Close(0));
  if ((dwRet == ERROR_SUCCESS) && (DeleteFile(m_sPath.c_str()) == FALSE))
  {
    dwRet = GetLastError();
    QSOS((L"cFileComposite::Delete() - Failed to delete file {%s} with error [%s]"
      , m_sPath.c_str()
      , win32ErrorToString(dwRet).c_str()));
  }

  return dwRet;
}

DWORD cWindowsComposite::cancelNotification(const ULONGLONG requestID, bool synchronous)
{
  return (DWORD)STATUS_NOT_IMPLEMENTED;
}

DWORD cWindowsComposite::registerNotification(const ULONGLONG requestID
  , vfs::cPtr<iNotificationReceiver> pNotification)
{
  return (DWORD)STATUS_NOT_IMPLEMENTED;
}

DWORD cWindowsComposite::Rename( const vfs::String& sNewName )
{
  return MoveFile(m_sPath.c_str(), sNewName.c_str());
}