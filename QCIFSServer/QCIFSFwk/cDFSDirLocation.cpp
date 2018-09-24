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

//cDFSDirLocation.cpp

#include "StdAfx.h"
#include ".\cdfsdirlocation.h"
#include "cWriteInfo.h"

#include "..\QCIFSIPC\iQCIFSIPC.h"

using namespace vfs;

FILETIME getCurrentTime()
{
  FILETIME ft;
  GetSystemTimeAsFileTime(&ft);
  return ft;
}

cDFSDirLocation::cDFSDirLocation(const cPtr<iNamed> pNamed
  , const String& sSourceShare
  , const std::vector<String>& sDFSPaths)
: m_pNamed(pNamed)
, m_sSourceShare(sSourceShare)
, m_sDFSPaths(sDFSPaths)
, m_createdTime(getCurrentTime())
{
}

cDFSDirLocation::~cDFSDirLocation(void)
{
}

cPtr<iComposite> cDFSDirLocation::Create(const String& sName
  , DWORD dwDesiredAccess
  , DWORD dwShareMode
  , LPSECURITY_ATTRIBUTES lpAtts
  , DWORD dwCreationDisposition
  , DWORD dwFlagsAndAtts
  , HANDLE hTemplateFile
  , DWORD& dwRet)
{
  try
  {
    for (std::vector<String>::const_iterator cit = m_sDFSPaths.begin()
      ; m_sDFSPaths.end() != cit
      ;  ++cit)
    {
      const String& sDFSPath(*cit);
      iQCIFSIPC::singleton().addDFSPath(L"\\" + m_sSourceShare + sName, sDFSPath);
    }
    dwRet = static_cast<DWORD>(STATUS_PATH_NOT_COVERED);
  }
  catch (const cNotFound&)
  {
    dwRet = ERROR_PATH_NOT_FOUND;
  }
  return cPtr<iComposite>();
}

DWORD cDFSDirLocation::writeInfo(const PfileInfo pFileInfo, ULONGLONG fid)
{
  cWriteInfo::singleton().writeFileInfo(pFileInfo
    , 0
    , m_createdTime
    , m_createdTime
    , m_createdTime
    , FILE_ATTRIBUTE_NORMAL
    , m_pNamed->Name());
  return ERROR_SUCCESS;
}