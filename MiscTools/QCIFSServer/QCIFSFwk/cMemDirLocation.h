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

//cMemDirLocation.h

#pragma once

#include "iDirLocation.h"
#include "iChildLoaderVisitor.h"
#include "cMemFileRename.h"

class iNamed;
class iChildLoaderVisitor;

template <typename T>
class cMemDirLocation : public iDirLocation, public vfs::cRefCount
{
  const vfs::cConstPtr<iNamed> m_pNamed;
  const vfs::cPtr<iChildLoaderVisitor> m_pSearchListener;
  const vfs::cPtr<T> m_pMemComposite;

public:
  cMemDirLocation(const vfs::cConstPtr<iNamed> pNamed
    , const vfs::cPtr<iChildLoaderVisitor> pSearchListener
    , const vfs::cPtr<T> pComposite)
  : m_pNamed(pNamed)
  , m_pSearchListener(pSearchListener)
  , m_pMemComposite(pComposite)
  {}

  ~cMemDirLocation(void){}

  vfs::cPtr<iComposite> Create(const vfs::String& sName
    , DWORD dwDesiredAccess
    , DWORD dwShareMode
    , LPSECURITY_ATTRIBUTES lpAtts
    , DWORD dwCreationDisposition
    , DWORD dwFlagsAndAtts
    , HANDLE hTemplateFile
    , DWORD& dwRet
    , vfs::cPtr<iFileProcessingCallback> pCallback)
  {
    dwRet = ERROR_SUCCESS;
    return m_pMemComposite;
  }

  vfs::cPtr<iRename> Rename()
  {
    return new cMemFileRename<T>(m_pMemComposite, m_pNamed, m_pSearchListener);
  }

  DWORD Rename(const vfs::cPtr<iRename> pSourceRename
    , const vfs::String& sNewName
    , const vfs::String& sUserName)
  {
    return ERROR_ACCESS_DENIED;
  }

  void Rename(const vfs::String& sNewName)
  {}
  
  DWORD writeInfo(const PfileInfo pFileInfo, ULONGLONG fid)
  {
    return m_pMemComposite->writeInfo(pFileInfo, fid);
  }

  void refreshCache()
  {
  }
};
