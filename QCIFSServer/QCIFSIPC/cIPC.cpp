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

//cIPFC.cpp

#include "StdAfx.h"
#include ".\cipc.h"
#include "..\QCIFSFwk\iQCIFSFwk.h"
#include "..\QCIFSFwk\iChildLoader.h"
#include "..\QCIFSFwk\iDirLocation.h"

using namespace vfs;

cIPC::cIPC(void)
: m_Fid(2)
{
}

cIPC::~cIPC(void)
{
}

DWORD cIPC::Search(const String& searchPath, tFileInfoList &fileInfoList)
{
  QSOS((L"cIPC::Search"));
  return ERROR_PATH_NOT_FOUND;
}

cPtr<iComposite> cIPC::NTCreateFolder(const String& ksFilePath
  , LPSECURITY_ATTRIBUTES lpAtts
  , LPBY_HANDLE_FILE_INFORMATION pFileInfo
  , DWORD &err)
{
  QSOS((L"cIPC::NTCreateFolder"));
  err = ERROR_ACCESS_DENIED;
  return cPtr<iComposite>();
}

cPtr<iComposite> cIPC::NTCreateFile(const String& ksFilePath
  , DWORD dwDesiredAccess
  , DWORD dwShareMode
  , LPSECURITY_ATTRIBUTES lpAtts
  , DWORD dwCreationDisposition
  , DWORD dwFlagsAndAtts
  , HANDLE hTemplateFile
  , LPBY_HANDLE_FILE_INFORMATION pFileInfo
  , DWORD &err
  , cPtr<iFileProcessingCallback> pCallback
  , ULONGLONG fid)
{
  QSOS((L"cIPC::NTCreateFile"));
  try
  {
    if (ksFilePath.empty())
    {
      //const cPtr<iSearchLocation> pSearchLocation
      //  = iQCIFSFwk::singleton().createVirtualFolder(ksFilePath, cPtr<iChildLoader>());
      //if (pSearchLocation.isValid())
      //{
      //  const cPtr<iDirLocation> pDirLocation = pSearchLocation->getDirLocation();
      //  if (pDirLocation.isValid())
      //    return pDirLocation->Create(ksFilePath
      //      , dwDesiredAccess
      //      , dwShareMode
      //      , lpAtts
      //      , dwCreationDisposition
      //      , dwFlagsAndAtts
      //      , hTemplateFile
      //      , err);
      //}
    }
  }
  catch (const cNotFound&)
  {
  }

  err = ERROR_FILE_NOT_FOUND;
  return cPtr<iComposite>();
}

void cIPC::AssignFid(USHORT &Fid)
{
  Fid = m_Fid;
  m_Fid++;
}

void cIPC::SetFileInfo(LPBY_HANDLE_FILE_INFORMATION pFileInfo)
{
  ZeroMemory(pFileInfo, sizeof(BY_HANDLE_FILE_INFORMATION));
  //FILETIME fileTime;
  //fileTime.dwLowDateTime = 0;
  //fileTime.dwHighDateTime = 0;
  //pFileInfo->dwFileAttributes = 0;
  //pFileInfo->dwVolumeSerialNumber = 0;
  //pFileInfo->ftCreationTime = fileTime;
  //pFileInfo->ftLastAccessTime = fileTime;
  //pFileInfo->ftLastWriteTime = fileTime;
  //pFileInfo->ftLastAccessTime = fileTime;
  //pFileInfo->nFileIndexHigh = 0;
  //pFileInfo->nFileIndexLow = 0;
  //pFileInfo->nFileSizeHigh = 0;
  //pFileInfo->nFileSizeLow = 0;
  //pFileInfo->nNumberOfLinks = 0;
}

DWORD cIPC::DeleteDirectory(const std::wstring &ksPath)
{
  return ERROR_FILE_NOT_FOUND;
}

DWORD cIPC::Delete(DWORD SearchAttributes, const std::wstring &filename)
{
  return FALSE;
}

DWORD cIPC::Rename(DWORD SearchAttributes, const std::wstring &originalName, const std::wstring &newName)
{
  return FALSE;
}


// queries
DWORD cIPC::QueryBasicInfo(const std::wstring &rsPath, LPWIN32_FILE_ATTRIBUTE_DATA pFileInfo)
{
  return 1;
}

DWORD cIPC::QueryStandardInfo(const std::wstring &rsPath, LPBY_HANDLE_FILE_INFORMATION pFileInf)
{
  return 1;
}

DWORD cIPC::QueryFSAttributeInfo(query_FS_Attribute_Info *pFSAttribInfo)
{
  return FALSE;
}

DWORD cIPC::QueryFSVolumeInfo(PQuery_FS_Volume_Info pFSVolInfo)
{
  return 1;
}

DWORD cIPC::QueryInfoAllocation(DISK_GEOMETRY_EX &disk_geometry, PULARGE_INTEGER pulFreeSpace
  , LPDWORD pSectorsPerCluster)
{
  return FALSE;
}

// constants
DWORD cIPC::getBytesPerCluster()
{
  return 1;
}
