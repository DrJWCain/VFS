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

//cMemFileRename.h

#pragma once

#include "iRename.h"
#include "cQCIFSFwkHelper.h"
//#include "iFileLoader.h"

class iChildLoaderVisitor;
class iNamed;

template <typename T>
class cMemFileRename : public iRename, public vfs::cRefCount
{
  const vfs::cPtr<T> m_pComposite;
  const vfs::cConstPtr<iNamed> m_pNamed;
  const vfs::cPtr<iChildLoaderVisitor> m_pSearchListener;

public:
  cMemFileRename(const vfs::cPtr<T> pComposite
    , const vfs::cConstPtr<iNamed> pNamed
    , const vfs::cPtr<iChildLoaderVisitor> pSearchListener)
  : m_pComposite(pComposite)
  , m_pNamed(pNamed)
  , m_pSearchListener(pSearchListener)
  {}

  ~cMemFileRename(void)
  {}

  DWORD From(vfs::cPtr<iRename> pRenameSource
    , const vfs::String& sNewName
    , const vfs::String& sUserName)
  {
    const vfs::cPtr<cMemFileRename<T> > pSourceMemFile = ptr_cast<const vfs::cPtr<cMemFileRename<T> > >(pRenameSource);
    if (pSourceMemFile.isNull())
      return ERROR_ACCESS_DENIED;

    const String sFilename = iQCIFSFwkHelper::singleton().getBaseName(sNewName);

    const vfs::cPtr<iChildLoaderVisitor> pSourceSearchListener = pSourceMemFile->m_pSearchListener;
    if (pSourceSearchListener.isNull())
      return ERROR_CANNOT_MAKE;

    pSourceSearchListener->remove(pSourceMemFile->m_pNamed->Name());
    try
    {
    m_pSearchListener->updateFile(sFilename
      , pSourceMemFile->m_pComposite->atts()
      , pSourceMemFile->m_pComposite->getMem()
      , pSourceMemFile->m_pComposite->getMemLoader()
      , cPtr<iFileEvent>()
      , pSourceMemFile->m_pComposite->canDelete());
    }
    catch (const cBadFormat& e)
    {
      QSOS((L"cVirtualDirLocation::tryFileRename - %s", e.getMsg().c_str()));
      return ERROR_ACCESS_DENIED;
    }

    return ERROR_SUCCESS;
  }
};