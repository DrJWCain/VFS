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

//cIPFC.h

#pragma once
#include "..\iTreeResource.h"

class cIPC : public iTreeResource, public vfs::cRefCount
{
public:
  cIPC(void);
  ~cIPC(void);

  DWORD Search(const vfs::String& searchPath, tFileInfoList &fileInfoList);
  vfs::cPtr<iComposite> NTCreateFolder(const vfs::String& searchPath
    , LPSECURITY_ATTRIBUTES lpAtts
    , LPBY_HANDLE_FILE_INFORMATION pFileInfo
    , DWORD &err);
  vfs::cPtr<iComposite> NTCreateFile(const vfs::String& searchPath
    , DWORD dwDesiredAccess
    , DWORD dwShareMode
    , LPSECURITY_ATTRIBUTES lpAtts
    , DWORD dwCreationDisposition
    , DWORD dwFlagsAndAtts
    , HANDLE hTemplateFile
    , LPBY_HANDLE_FILE_INFORMATION pFileInfo
    , DWORD &err
    , vfs::cPtr<iFileProcessingCallback> pCallback = vfs::cPtr<iFileProcessingCallback>()
    , ULONGLONG fid = 0);

  DWORD DeleteDirectory(const std::wstring &ksPath);
  DWORD Delete(DWORD SearchAttributes, const std::wstring &filename);
  DWORD Rename(DWORD SearchAttributes, const std::wstring &originalName, const std::wstring &newName);

  // queries
  DWORD QueryBasicInfo(const std::wstring &rsPath, LPWIN32_FILE_ATTRIBUTE_DATA pFileInfo);
  DWORD QueryStandardInfo(const std::wstring &rsPath, LPBY_HANDLE_FILE_INFORMATION pFileInf);
  DWORD QueryFSAttributeInfo(query_FS_Attribute_Info *pFSAttribInfo);
  DWORD QueryFSVolumeInfo(PQuery_FS_Volume_Info pFSVolInfo);
  DWORD QueryInfoAllocation(DISK_GEOMETRY_EX &disk_geometry, PULARGE_INTEGER pulFreeSpace
    , LPDWORD pSectorsPerCluster);

  // costants
  DWORD getBytesPerCluster();

private:
  void AssignFid(USHORT &Fid);
  void SetFileInfo(LPBY_HANDLE_FILE_INFORMATION pFileInfo);

  USHORT m_Fid;
};