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

//cVirtualDirLocation.cpp

#include "StdAfx.h"
#include ".\cvirtualdirlocation.h"

#include "iQCIFSFwk.h"
#include "iNamed.h"
#include "cSearchListener.h"
#include "cVirtualComposite.h"
#include "cConstNamed.h"
#include "cMemComposite.h"
#include "cConstMemComposite.h"
#include "cMemFileRename.h"
#include "cSearchlocation.h"
#include "iCreate.h"
#include "iChildLoader.h"
#include "iRenameHandler.h"
#include "cWriteInfo.h"

using namespace vfs;

cVirtualDirLocation::cVirtualDirLocation(const fileInfo& rFileInfo
  , const cPtr<cSearchListener> pSearchListener
  , const cPtr<iChildLoader> pChildLoader
  , const cPtr<cSearchListener> pParentSearchListener
  , const cPtr<iRename> pRename
  , const cPtr<iNotifyTree> pNotifyTree
  , const cPtr<cNotificationHandler> m_pNotificationReceiver
  , const cPtr<iCreate> pCreate
  , const cPtr<iFileEvent> pFileEvent
  , const cPtr<iRenameHandler> pRenameHandler)
: m_fileInfo(rFileInfo)
, m_pSearchListener(pSearchListener)
, m_pChildLoader(pChildLoader)
, m_pParentSearchListener(pParentSearchListener)
, m_pRename(pRename)
, m_pNotifyTree(pNotifyTree)
, m_pNotificationReceiver(m_pNotificationReceiver)
, m_pCreate(pCreate)
, m_pFileEvent(pFileEvent)
, m_pRenameHandler(pRenameHandler)
{
}

cVirtualDirLocation::~cVirtualDirLocation(void)
{
}

cPtr<iComposite> cVirtualDirLocation::Create(const String& sName
  , DWORD dwDesiredAccess
  , DWORD dwShareMode
  , LPSECURITY_ATTRIBUTES lpAtts
  , DWORD dwCreationDisposition
  , DWORD dwFlagsAndAtts
  , HANDLE hTemplateFile
  , DWORD& dwRet
  , cPtr<iFileProcessingCallback> pCallback)
{
  fileInfo fi = m_fileInfo;

  if (CREATE_NEW == dwCreationDisposition
    || CREATE_ALWAYS == dwCreationDisposition
    || OPEN_ALWAYS == dwCreationDisposition)
  {
    if (m_pSearchListener.isNull())
    {
      dwRet = ERROR_CANNOT_MAKE;
      return cPtr<iComposite>();
    }

    const String sBaseName = iQCIFSFwkHelper::singleton().getBaseName(sName);

    fi = cWriteInfo::singleton().makeFileInfo(dwFlagsAndAtts, sBaseName);

    const bool bDirectory = (0 != (dwFlagsAndAtts & FILE_ATTRIBUTE_DIRECTORY));
    if (m_pCreate.isValid())
    {
      if (bDirectory)
      {
        if (0 == (dwFlagsAndAtts & FILE_ATTRIBUTE_HIDDEN))
        {
          dwRet = m_pCreate->Directory(sBaseName, lpAtts);
          ////SNIA SDC 2011 Start .... 
          //if(ERROR_SUCCESS == dwRet)
          //{
          //  if(m_pSearchListener.isValid())
          //  {
          //    const cPtr<cSearchLocation> pThisUpdated
          //      = m_pSearchListener->getChild(sBaseName, cPtr<iChildLoader>());
          //    if (pThisUpdated.isNull())
          //    {
          //      QSOS((L"cVirtualDirLocation::Create() - couldn't create new sub directory"));
          //      dwRet = ERROR_CANNOT_MAKE;
          //      return cPtr<iComposite>();
          //    }

          //    pThisUpdated->getChild(L"");//call register listener & thus allow children to be created before a DIR /find is issued.
          //  }
          //}
          ////SNIA SDC 2011 .... stop
        }
        else
        {
          dwRet = ERROR_SUCCESS;
          m_pSearchListener->updateFolder(sBaseName
            , cPtr<iChildLoader>()
            , cPtr<iRename>()
            , cPtr<iCreate>()
            , cPtr<cMemoryView>()
            , cPtr<iFileEvent>()
            , true
            , true);
        }
      }
      else
      {
        dwRet = m_pCreate->File(sBaseName, lpAtts, dwFlagsAndAtts, pCallback);
        if (ERROR_SUCCESS == dwRet && m_pSearchListener.isValid())
        {
          const cPtr<cSearchLocation> pSearchLocation = m_pSearchListener->getChild(sBaseName, cPtr<iChildLoader>());
          if (pSearchLocation.isValid())
          {
            const cPtr<iDirLocation> pDirLocation = pSearchLocation->getDirLocation();
            if (pDirLocation.isValid())
              return pDirLocation->Create(sName
                , dwDesiredAccess
                , dwShareMode
                , lpAtts
                , OPEN_EXISTING
                , dwFlagsAndAtts
                , hTemplateFile
                , dwRet
                , pCallback);
          }
        }
      }

      if (ERROR_SUCCESS != dwRet)
        return cPtr<iComposite>();
    }
    else
    {
      //don't allow creation without inheriting from iCreate
      return  cPtr<iComposite>();
    }

    if (m_pParentSearchListener.isValid())
    {
      // need to reload ourselves since the model might have changed
      const cPtr<cSearchLocation> pThisUpdated
        = m_pParentSearchListener->getChild(m_fileInfo.w32Data.cFileName, cPtr<iChildLoader>());
      if (pThisUpdated.isNull())
      {
        QSOS((L"cVirtualDirLocation::Create() - couldn't create new version of self."));
        dwRet = ERROR_CANNOT_MAKE;
        return cPtr<iComposite>();
      }

      const cPtr<cSearchLocation> pSearchLocation = pThisUpdated->getChild(sBaseName);
      if (pSearchLocation.isValid())
      {
        const cPtr<iDirLocation> pDirLocation = pSearchLocation->getDirLocation();
        if (pDirLocation.isValid())
          return pDirLocation->Create(sName
            , dwDesiredAccess
            , dwShareMode
            , lpAtts
            , OPEN_EXISTING
            , dwFlagsAndAtts
            , hTemplateFile
            , dwRet
            , pCallback);
      }
      dwRet = ERROR_FILE_NOT_FOUND;
      return cPtr<iComposite>();
    }
  }

  dwRet = ERROR_SUCCESS;
  return new cVirtualComposite(fi
    , m_pSearchListener
    , m_pParentSearchListener
    , m_pChildLoader
    , m_pNotifyTree
    , m_pNotificationReceiver
    , m_pFileEvent
    , m_pRename
    , m_pRenameHandler);
}

cPtr<iRename> cVirtualDirLocation::Rename()
{
  return m_pRename;
}

DWORD cVirtualDirLocation::Rename(const cPtr<iRename> pRenameSource
  , const String& sNewName
  , const String& sUserName)
{
  DWORD dwRet(ERROR_ACCESS_DENIED);
  if (m_pRename.isValid())
    dwRet = m_pRename->From(pRenameSource, sNewName, sUserName);

  if (ERROR_SUCCESS != dwRet)
  {
    dwRet = tryFileRename<cMemComposite>(pRenameSource, sNewName, sUserName);
    if (ERROR_SUCCESS != dwRet)
      dwRet = tryFileRename<cConstMemComposite>(pRenameSource, sNewName, sUserName);
  }
  
  return dwRet;
}

void cVirtualDirLocation::Rename(const String& sNewName)
{
  cLockGuard lg(&m_rename);
  wcsncpy(m_fileInfo.w32Data.cFileName, sNewName.c_str(), sizeof(m_fileInfo.w32Data.cFileName)/sizeof(WCHAR) - 1);
}

DWORD cVirtualDirLocation::writeInfo(const PfileInfo pFileInfo, ULONGLONG fid)
{
  memcpy(pFileInfo, &m_fileInfo, sizeof(fileInfo));
  return ERROR_SUCCESS;
}

void cVirtualDirLocation::refreshCache()
{
  QTRACE((L"cVirtualDirLocation::refreshCache()"));
  if(m_pSearchListener.isValid())
    m_pSearchListener->clear();
}

template <typename T>
DWORD cVirtualDirLocation::tryFileRename(const cPtr<iRename> pRenameSource
  , const String& sNewName
  , const String& sUserName)
{
  const cPtr<cMemFileRename<T> > pMemFileType = ptr_cast<cPtr<cMemFileRename<T> > >(pRenameSource);
  if (pMemFileType.isValid())
  {
    const cPtr<iRename> pRename = new cMemFileRename<T>(cPtr<T>(), cPtr<iNamed>(), m_pSearchListener);
    if (pRename.isValid())
      return pRename->From(pRenameSource, sNewName, sUserName);
  }

  return ERROR_ACCESS_DENIED;
}