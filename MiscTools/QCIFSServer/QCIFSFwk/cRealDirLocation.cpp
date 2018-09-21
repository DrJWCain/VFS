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

//cRealDirLocation.cpp

#include "StdAfx.h"
#include ".\crealdirlocation.h"

#include "..\QCIFSProcessor\iQCIFSProcessor.h"

using namespace vfs;

cRealDirLocation::cRealDirLocation(const String& sPath)
: m_sPath(sPath)
{
}

cRealDirLocation::~cRealDirLocation(void)
{
}

cPtr<iComposite> cRealDirLocation::Create(const String& sName
  , DWORD dwDesiredAccess
  , DWORD dwShareMode
  , LPSECURITY_ATTRIBUTES lpAtts
  , DWORD dwCreationDisposition
  , DWORD dwFlagsAndAtts
  , HANDLE hTemplateFile
  , DWORD& dwRet
  , cPtr<iFileProcessingCallback> pCallback)
{
  BY_HANDLE_FILE_INFORMATION bhi;
  return iQCIFSProcessor::singleton().getFileComposite(m_sPath
    , dwDesiredAccess
    , dwShareMode
    , lpAtts
    , dwCreationDisposition
    , dwFlagsAndAtts
    , hTemplateFile
    , &bhi
    , dwRet);
}

cPtr<iRename> cRealDirLocation::Rename()
{
  return cPtr<iRename>();
}

DWORD cRealDirLocation::Rename(const vfs::cPtr<iRename> pSourceRename
  , const vfs::String& sNewName
  , const vfs::String& sUserName)
{
  return ERROR_ACCESS_DENIED;
}

DWORD cRealDirLocation::writeInfo(const PfileInfo pFileInfo, ULONGLONG fid)
{
  HANDLE hFind = FindFirstFile(m_sPath.c_str(), &(pFileInfo->w32Data));
  if (INVALID_HANDLE_VALUE != hFind)
  {
    FindClose(hFind);
    return 0;
  }

  return GetLastError();
}

void cRealDirLocation::refreshCache()
{
}
