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

//cFileComposite.cpp

#include "stdafx.h"

#include ".\cfilecomposite.h"
#include <shlwapi.h>
#include "cNOPCompletionMsg.h"

#include "..\common\iIOCompletionPort.h"
#include "..\..\..\KernelSDK\src\tools\win\KernelSetup\Win32Help.h"

using namespace vfs;

bool makeSureFolderExists(const String &krsFolderPath)
{
  if (krsFolderPath.empty())
    return false;

  HANDLE hFolder = CreateFile(krsFolderPath.c_str()
    , GENERIC_READ
    , FILE_SHARE_READ
    , NULL
    , OPEN_EXISTING
    , FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS 
    , NULL);

  if (INVALID_HANDLE_VALUE == hFolder)
  {
    const size_t slashPos(krsFolderPath.find_last_of(L"\\"));
    if (String::npos == slashPos || !makeSureFolderExists(krsFolderPath.substr(0, slashPos)))
      return false;

    if (FALSE == CreateDirectory(krsFolderPath.c_str(), NULL))
    {
      return false;
      QSOS((L"cFileComposite::makeSureFolderExists() - failed on %s with error %d"
        , krsFolderPath.c_str()
        , GetLastError()));
    }
  }
  else
    CloseHandle(hFolder);

  return true;
}

HANDLE createFileHandle(const String& krsPath
  , const DWORD dwDesiredAccess
  , const DWORD dwShareMode
  , const DWORD dwCreationDisposition
  , const DWORD dwFlagsAndAtts
  , LPBY_HANDLE_FILE_INFORMATION pFileInfo
  , DWORD& err)
{
  HANDLE hFile = INVALID_HANDLE_VALUE;
  if (makeSureFolderExists(krsPath.substr(0, krsPath.find_last_of(L"\\"))))
  {
    hFile = CreateFile(krsPath.c_str()
      , (dwDesiredAccess | GENERIC_READ) - ((dwDesiredAccess & FILE_EXECUTE) ? FILE_EXECUTE : 0)
      , dwShareMode
      , NULL
      , dwCreationDisposition
      , dwFlagsAndAtts | (FALSE == PathIsDirectory(krsPath.c_str()) ? 0 : FILE_FLAG_BACKUP_SEMANTICS) // | FILE_FLAG_OVERLAPPED
      , NULL);

    if (INVALID_HANDLE_VALUE == hFile)
    {
      err = GetLastError();
      QSOS((L"cFileComposite::cFileComposite () - can't create {%s} with error %d - [%s]"
        , krsPath.c_str()
        , err
        , win32ErrorToString(err).c_str()));
    }
    else
    {
      err = ERROR_SUCCESS;
      GetFileInformationByHandle(hFile, pFileInfo);
    }
  }
  else
    err = ERROR_CANNOT_MAKE;

  return hFile;
}

cFileComposite::cFileComposite(const String& krsPath
  , DWORD dwDesiredAccess
  , DWORD dwShareMode
  , LPSECURITY_ATTRIBUTES lpAtts
  , DWORD dwCreationDisposition
  , DWORD dwFlagsAndAtts
  , HANDLE hTemplateFile
  , LPBY_HANDLE_FILE_INFORMATION pFileInfo
  , iIOCompletionPort *pIOCP
  , DWORD &err)
: cWindowsComposite(krsPath
    , createFileHandle(krsPath, dwDesiredAccess, dwShareMode, dwCreationDisposition, dwFlagsAndAtts, pFileInfo, err)
    , pIOCP)
{
  const HANDLE hFile(cWindowsComposite::getHandle());
  if (INVALID_HANDLE_VALUE == hFile)
    m_FileSize.QuadPart = 0;
  else
    m_FileSize.LowPart = GetFileSize(hFile, reinterpret_cast<LPDWORD>(&(m_FileSize.HighPart)));
}

cFileComposite::~cFileComposite(void)
{
}

DWORD cFileComposite::Read(tTransmitList &krTPM, DWORD& nBytes, const LARGE_INTEGER &nOffset, const int sessionID, ULONGLONG fid)
{
  DWORD dwErr(ERROR_SUCCESS); // assume success
  const HANDLE hFile(cWindowsComposite::getHandle());
  if (INVALID_HANDLE_VALUE != hFile)
  {
    if (nBytes > 0)
    {
      cPtr<cMemoryView> pMem = new cMemoryView(new cMemory((size_t)nBytes, vfs::cMemory::eVirtual));
      {
        cLockGuard lg(&m_FPAccess);
        setFilePointer(nOffset);
        if (ReadFile(hFile
          , pMem->getBytes()
          , nBytes
          , &nBytes
          , NULL) == FALSE)
        {
          dwErr = GetLastError();
          QSOS((L"cFileComposite::Read() - Failed to read file %s with error %d - %s"
            , getPath().c_str()
            , dwErr
            , win32ErrorToString(dwErr).c_str()));
        }
        else
        {
          SMART_TPE smart_tpe;
          smart_tpe.tpe.cLength = nBytes;
          smart_tpe.tpe.pBuffer = pMem->getBytes();
          smart_tpe.tpe.dwElFlags = TP_ELEMENT_MEMORY;
          smart_tpe.pMem = pMem;
          krTPM.push_back(smart_tpe);
        }
        //SMART_TPE smart_tpe;
        //int overrun = static_cast<int>(nOffset.QuadPart + nBytes - m_FileSize.QuadPart);
        //smart_tpe.tpe.cLength = (overrun > 0) ? nBytes - overrun : nBytes;
        //smart_tpe.tpe.hFile = hFile;
        //smart_tpe.tpe.nFileOffset = nOffset;
        //smart_tpe.tpe.dwElFlags = TP_ELEMENT_FILE;
        //m_filePointer.QuadPart = nOffset.QuadPart;
        //krTPM.push_back(smart_tpe);
      }
    }
  }

  return dwErr;
}

DWORD cFileComposite::Write(vfs::cConstPtr<vfs::cMemoryView> buffer, DWORD& nBytes, const LARGE_INTEGER &offset, const int sessionID, ULONGLONG fid)
{
  DWORD dwRet(ERROR_INVALID_HANDLE);
  const HANDLE hFile(cWindowsComposite::getHandle());
  if (INVALID_HANDLE_VALUE != hFile)
  {
    //HANDLE* const pCompletionHandle(cWindowsComposite::getCompletionHandle());
    //if (pCompletionHandle != NULL)
    //{
    //  m_filePointer.QuadPart = offset.QuadPart;

    //  IO_DATA *pIOData = new IO_DATA;
    //  pIOData->overlapped.Internal = 0;
    //  pIOData->overlapped.InternalHigh = 0;
    //  pIOData->overlapped.Offset = offset.LowPart;
    //  pIOData->overlapped.OffsetHigh = offset.HighPart;
    //  pIOData->overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    //  pIOData->pMsg = new cNOPCompletionMsg();
    //  const LPOVERLAPPED pOL(reinterpret_cast<LPOVERLAPPED>(pIOData));
    //  if (!WriteFile(hFile, buffer->getConstBytes(), nBytes, &nBytes, pOL))
    //  {
    //    const DWORD dwErr = GetLastError();
    //    if (dwErr != ERROR_IO_PENDING)
    //    {
    //      QSOS((L"cFileComposite::Write() - Failed to start IO Completion write to file {%s} with error %d - %s"
    //        , cWindowsComposite::Name().c_str()
    //        , dwErr
    //        , win32ErrorToString(dwErr).c_str()));

    //      CloseHandle(pIOData->overlapped.hEvent);
    //      delete(pIOData);

    //      return dwErr;
    //    }
    //    else if (FALSE == GetOverlappedResult(hFile, pOL, &nBytes, TRUE))
    //    {
    //      dwRet = GetLastError();
    //      QSOS((L"cFileComposite::Write() - Failed to start IO Completion write to file {%s} with error %d - %s"
    //        , cWindowsComposite::Name().c_str()
    //        , dwRet
    //        , win32ErrorToString(dwRet).c_str()));

    //      CloseHandle(pIOData->overlapped.hEvent);
    //      delete(pIOData);

    //      return dwRet;
    //    }
    //  }

    //  m_filePointer.QuadPart += nBytes;
    //  dwRet = ERROR_SUCCESS;
    //  CloseHandle(pIOData->overlapped.hEvent);
    //}
    //else
    {
      cLockGuard lg(&m_FPAccess);

      setFilePointer(offset);

      nBytes = min(nBytes, buffer->getSize());

      if (!WriteFile(hFile, buffer->getConstBytes(), nBytes, &nBytes, NULL))
      {
        dwRet = GetLastError();
        QSOS((L"cFileComposite::Write() - Failed to write to file {%s} with error %d - %s"
          , cWindowsComposite::Name().c_str()
          , dwRet
          , win32ErrorToString(dwRet).c_str()));
      }
      else
      {
        dwRet = ERROR_SUCCESS;
        m_filePointer.QuadPart += nBytes;
      }
    }
  }

  return dwRet;
}

DWORD cFileComposite::SetFileSize(const PLARGE_INTEGER pFileSize)
{
  DWORD dwRet(ERROR_SUCCESS);
  const HANDLE hFile(cWindowsComposite::getHandle());
  if (INVALID_HANDLE_VALUE != hFile)
  {
    setFilePointer(*pFileSize);
    if (SetEndOfFile(hFile) != FALSE)
    {
      m_FileSize = *pFileSize;
    }
    else
    {
      dwRet = GetLastError();
    }
  }

  return dwRet;
}

DWORD cFileComposite::SetFileInfo(const FILETIME& creationTime
  , const FILETIME& modifiedTime, const FILETIME& accessTime, const DWORD attributes)
{
  if (FALSE != SetFileAttributes(cWindowsComposite::Name().c_str(), attributes))
  {
    const HANDLE hFile(cWindowsComposite::getHandle());
    if (INVALID_HANDLE_VALUE != hFile)
    {
      if (FALSE != SetFileTime(hFile
        , (creationTime.dwLowDateTime == 0 && creationTime.dwHighDateTime == 0) ? NULL : &creationTime
        , (accessTime.dwLowDateTime == 0 && accessTime.dwHighDateTime == 0) ? NULL : &accessTime
        , (modifiedTime.dwLowDateTime == 0 && modifiedTime.dwHighDateTime == 0) ? NULL : &modifiedTime))
        return ERROR_SUCCESS;
    }
  }

  return GetLastError();
}

DWORD cFileComposite::Lock(const LARGE_INTEGER &lgOffset, const LARGE_INTEGER &lgBytes, const bool bLock)
{
  DWORD dwRet(0);
  const HANDLE hFile(cWindowsComposite::getHandle());
  if (INVALID_HANDLE_VALUE != hFile)
  {
    BOOL bSuccess(FALSE);

    if (bLock)
    {
      bSuccess = LockFile(hFile
        , lgOffset.LowPart
        , lgOffset.HighPart
        , lgBytes.LowPart
        , lgBytes.HighPart);
    }
    else
    {
      bSuccess = UnlockFile(hFile
        , lgOffset.LowPart
        , lgOffset.HighPart
        , lgBytes.LowPart
        , lgBytes.HighPart);
    }
 
    if (bSuccess == FALSE)
    {
      dwRet = GetLastError();
      QSOS((L"cFileComposite::Lock() - failed with error %d - %s"
        , dwRet
        , win32ErrorToString(dwRet).c_str()));
    }
    //IO_DATA *pIOData = new IO_DATA;
    //pIOData->overlapped.Internal = 0;
    //pIOData->overlapped.InternalHigh = 0;
    //pIOData->overlapped.Offset = lgOffset.LowPart;
    //pIOData->overlapped.OffsetHigh = lgOffset.HighPart;
    //pIOData->overlapped.hEvent = 0;

    //pIOData->pMsg = new cNOPCompletionMsg();

    //if (bLock)
    //{
    //  DWORD dwFlags(0);
    //  bSuccess = LockFileEx(hFile
    //    , dwFlags
    //    , 0
    //    , lgBytes.LowPart
    //    , lgBytes.HighPart
    //    , reinterpret_cast<LPOVERLAPPED>(pIOData));
    //}
    //else
    //  bSuccess = UnlockFileEx(hFile
    //    , 0
    //    , lgBytes.LowPart
    //    , lgBytes.HighPart
    //    , reinterpret_cast<LPOVERLAPPED>(pIOData));

    //if (bSuccess == FALSE)
    //{
    //  dwRet = GetLastError();
    //  if (dwRet = ERROR_IO_PENDING)
    //  {
    //    QSOS((L"cFileComposite::Lock() - Failed on file {%s} with error [%s]"
    //      , m_sPath.c_str()
    //      , win32ErrorToString(dwRet).c_str()));
    //    delete(pIOData);
    //  }
    //  else
    //    dwRet = pStatus->getStatus();
    //}
    //else
    //  dwRet = pStatus->getStatus();
  }

  return dwRet;
}

void cFileComposite::setFilePointer(const LARGE_INTEGER& rFP)
{
  const HANDLE hFile(getHandle());
  if (INVALID_HANDLE_VALUE != hFile)
  {
    cLockGuard lg(&m_FPAccess);
    if (m_filePointer.QuadPart != rFP.QuadPart)
      SetFilePointerEx(hFile, rFP, &m_filePointer, FILE_BEGIN);
  }
}