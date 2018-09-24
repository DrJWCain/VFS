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

//iTreeResource.h

// defines interface between the SMB server and the target resource

#pragma once

#include "iComposite.h"
#include "iFileProcessingCallback.h"

typedef struct _query_FS_Attribute_Info
{
  ULONG attribs;
  LONG maxFileNameLength;
  std::wstring sName;
} query_FS_Attribute_Info, *PQuery_FS_Attribute_Info;

typedef struct _query_FS_Size_Info
{
  LARGE_INTEGER TotalNumberAllocationUnits;
  LARGE_INTEGER NumberFreeAllocationUnits;
  ULONG sectorsPerAllocationUnit;
  ULONG bytesPerSector;
} query_FS_Size_Info, *Pquery_FS_Size_Info;

typedef struct _query_FS_Volume_Info
{
  SYSTEMTIME volCreationTime;
  ULONG serialNumber;
  std::wstring sLabel;
} query_FS_Volume_Info, *PQuery_FS_Volume_Info;


class iTreeResource
{
public:
  virtual ~iTreeResource() {}

  // operations
  virtual DWORD Search(const std::wstring& searchPath, tFileInfoList &fileInfoList) = 0;
  virtual vfs::cPtr<iComposite> NTCreateFolder(const std::wstring& searchPath
    , LPSECURITY_ATTRIBUTES lpAtts
    , LPBY_HANDLE_FILE_INFORMATION pFileInfo
    , DWORD &err) = 0;
  virtual vfs::cPtr<iComposite> NTCreateFile(const std::wstring& searchPath
    , DWORD dwDesiredAccess
    , DWORD dwShareMode
    , LPSECURITY_ATTRIBUTES lpAtts
    , DWORD dwCreationDisposition
    , DWORD dwFlagsAndAtts
    , HANDLE hTemplateFile
    , LPBY_HANDLE_FILE_INFORMATION pFileInfo
    , DWORD &err
    , vfs::cPtr<iFileProcessingCallback> pCallback = vfs::cPtr<iFileProcessingCallback>()
    , ULONGLONG fid = 0) = 0;
  /*** DeleteDirectory - return 0 for success or error code ***/
  virtual DWORD DeleteDirectory(const std::wstring &ksPath) = 0;
  virtual DWORD Delete(DWORD SearchAttributes, const std::wstring &filename) = 0;
  virtual DWORD Rename(DWORD SearchAttributes, const std::wstring &originalName, const std::wstring &newName) = 0;

  // queries
  /*** QueryBasicInfo - return 0 for success or error code ***/
  virtual DWORD QueryBasicInfo(const std::wstring &rsPath, LPWIN32_FILE_ATTRIBUTE_DATA pFileInfo) = 0;
  /*** QueryStandardInfo - return 0 for success or error code ***/
  virtual DWORD QueryStandardInfo(const std::wstring &rsPath, LPBY_HANDLE_FILE_INFORMATION pFileInf) = 0;
  virtual DWORD QueryFSAttributeInfo(query_FS_Attribute_Info *pFSAttribInfo) = 0;
  /*** QueryFSVolumeInfo - return 0 for success or error code ***/
  virtual DWORD QueryFSVolumeInfo(PQuery_FS_Volume_Info pFSVolInfo) = 0;
  virtual DWORD QueryInfoAllocation(DISK_GEOMETRY_EX &disk_geometry, PULARGE_INTEGER pulFreeSpace
    , LPDWORD pSectorsPerCluster) = 0;

  // constants
  virtual DWORD getBytesPerCluster() = 0;
};