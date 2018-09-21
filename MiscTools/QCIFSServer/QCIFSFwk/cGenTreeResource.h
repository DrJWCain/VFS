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

//cGenTreeResource.h

#pragma once

#include "..\itreeresource.h"

class cShare;
class iComposite;
class iDirLocation;
class iNameMangler;
class iNamespaceCreator;
class cRenameHandler;

class cGenTreeResource : public iTreeResource, public vfs::cRefCount
{
public:
  cGenTreeResource(vfs::cPtr<cShare> pShare
    , const vfs::cPtr<cRenameHandler> pRenameHandler
    , const vfs::cPtr<iNameMangler> pNameMangler
    , const vfs::cPtr<iNamespaceCreator> pNamespaceCreator);
  ~cGenTreeResource(void);

  //iTreeResource
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

  DWORD DeleteDirectory(const vfs::String &sPath);
  DWORD Delete(DWORD SearchAttributes, const vfs::String &filename);
  DWORD Rename(DWORD SearchAttributes, const vfs::String &originalName, const vfs::String &newName);

  // queries
  DWORD QueryBasicInfo(const vfs::String &rsPath, LPWIN32_FILE_ATTRIBUTE_DATA pFileInfo);
  DWORD QueryStandardInfo(const vfs::String &rsPath, LPBY_HANDLE_FILE_INFORMATION pFileInf);
  DWORD QueryFSAttributeInfo(query_FS_Attribute_Info *pFSAttribInfo);
  DWORD QueryFSVolumeInfo(PQuery_FS_Volume_Info pFSVolInfo);
  DWORD QueryInfoAllocation(DISK_GEOMETRY_EX &disk_geometry, PULARGE_INTEGER pulFreeSpace
    , LPDWORD pSectorsPerCluster);

  // constants
  DWORD getBytesPerCluster();

  //public member functions
  vfs::cPtr<iDirLocation> get(const vfs::String& sPath) const;
  vfs::cPtr<iDirLocation> getParent(const vfs::String& sPath) const;

private:  //private member functions
  inline vfs::cPtr<iDirLocation> getItem(const vfs::String& sPath) const;  
  inline DWORD getData(const vfs::String& sPath, LPBY_HANDLE_FILE_INFORMATION pFileInfo) const;
  inline vfs::String makeMangledName(const vfs::String& sPath) const;

  vfs::cPtr<iComposite> openExisting(const vfs::String& searchPath
    , DWORD dwDesiredAccess
    , DWORD dwShareMode
    , LPSECURITY_ATTRIBUTES lpAtts
    , DWORD dwCreationDisposition
    , DWORD dwFlagsAndAtts
    , HANDLE hTemplateFile
    , LPBY_HANDLE_FILE_INFORMATION pFileInfo
    , DWORD &err
    , vfs::cPtr<iFileProcessingCallback> pCallback
    , bool isFolder
    , ULONGLONG fid = 0) const;
  vfs::cPtr<iComposite> createNew(const vfs::String& searchPath
    , DWORD dwDesiredAccess
    , DWORD dwShareMode
    , LPSECURITY_ATTRIBUTES lpAtts
    , DWORD dwCreationDisposition
    , DWORD dwFlagsAndAtts
    , HANDLE hTemplateFile
    , LPBY_HANDLE_FILE_INFORMATION pFileInfo
    , DWORD &err
    , vfs::cPtr<iFileProcessingCallback> pCallback);

private:  //private member variables
  const vfs::cPtr<cShare> m_pShare;
  const vfs::cPtr<cRenameHandler> m_pRenameHandler;
  const vfs::cPtr<iNameMangler> m_pNameMangler;
  const vfs::cPtr<iNamespaceCreator> m_pNamespaceCreator;
  const SYSTEMTIME m_creationTime;

  // QTube Options
  static bool optionCheck(const vfs::String& path);
};