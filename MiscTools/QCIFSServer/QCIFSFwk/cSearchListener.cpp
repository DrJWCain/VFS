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

//cSearchListener.cpp

#include "StdAfx.h"
#include ".\csearchlistener.h"

#include "cSearchLocation.h"
#include "cQCIFSFwkHelper.h"
#include "cRealDirLocation.h"
#include "cConstNamed.h"
#include "cMemDirLocation.h"
#include "cMemComposite.h"
#include "cConstMemComposite.h"
#include "cCallbackComposite.h"
#include "cCompositeDirLocation.h"
#include "cWriteInfo.h"
#include "cVirtualDirLocation.h"
#include "cNotificationHandler.h"
#include "cNotifyTree.h"
#include "iNameMangler.h"
#include "cQCIFSFwk.h"
#include "cWriteInfo.h"
#include "cSearchLocationCache.h"
#include "iRenameHandler.h"
//#include "..\..\..\MiscPlugins\QDatabase\iClipInfo.h"
//#include "..\QCIFSBin\iISAEssenceFolderManager.h"

using namespace vfs;
using namespace std;

namespace
{
  const unsigned int kMaxListSize = 5;
}

//std::string cSearchListener::m_sIconName("icon.ico");
//cConstPtr<cMemoryView> cSearchListener::m_pDesktopINIFile;
//fileInfo cSearchListener::m_iconFI;
//fileInfo cSearchListener::m_desktopIniFI;

cSearchListener::cSearchListener(const cPtr<cNotificationHandler> pNotificationHandler
  , const cPtr<iNameMangler> pNameMangler
  , const cConstPtr<cMemoryView> pIconMem
  , const cPtr<cSearchLocationCache>pSearchCache
  , const cPtr<iRenameHandler> pRenameHandler)
: m_pNotificationHandler(pNotificationHandler)
, m_pNameMangler(pNameMangler)
, m_pSearchCache(pSearchCache)
, m_pRenameHandler(pRenameHandler)
//, m_pIconMem(pIconMem)
, m_guid(cGuid::createUnique(L"SearchListener"))
, m_bInitialised(false)
{
  //if (m_pIconMem.isValid())
  //{
  //  if (m_sIconName.empty())
  //    m_sIconName = "icon.ico";

  //  if (m_pDesktopINIFile.isNull())
  //  {
  //    m_pDesktopINIFile = cQCIFSFwk::singleton().makeDesktopIni(cSearchListener::m_sIconName);
  //    m_iconFI = cWriteInfo::singleton().makeFileInfo(
  //      FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM, widen(cSearchListener::m_sIconName));
  //    m_desktopIniFI = cWriteInfo::singleton().makeFileInfo(
  //      FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM, String(L"desktop.ini"));
  //  }
  //}

  //QTRACE((L"cSearchListener::cSearchListener() {0x%08x}", this));
}

cSearchListener::~cSearchListener(void)
{
  //QTRACE((L"cSearchListener::~cSearchListener() {0x%08x}", this));
}

void cSearchListener::update(const String& sPath)
{
  try
  {
    const cPtr<iDirLocation> pDirLocation = new cRealDirLocation(sPath);
    if (pDirLocation.isNull())
      throw exception("couldn't real dir location");
  
    secureAdd(iQCIFSFwkHelper::singleton().getBaseName(sPath), new cSearchLocation(pDirLocation
      , cPtr<cSearchListener>()
      , cPtr<iNotifyTree>()
      , cPtr<iChildLoader>()
      , m_pSearchCache));
  }
  catch (const exception& e)
  {
    QSOS((L"cSearchListener::update() - %S", e.what()));
  }
}

void cSearchListener::updateFile(const String& sName
  , const DWORD dwAtts
  , const tFrameVector& frameVector
  , const cPtr<iFileLoader> pMemFileLoader
  , const cPtr<iFileEvent> pFileEvent
  , const bool bDeletable
  , bool hasBatchOpLock
  , const bool bNotify)
{
  try
  {
    //const fileInfo fi = iWriteInfo::singleton().makeFileInfo(dwAtts | (bDeletable ? 0 : FILE_ATTRIBUTE_READONLY)
    //  , makeMangledName(sName));
    const fileInfo fi = iWriteInfo::singleton().makeFileInfo(dwAtts, makeMangledName(sName));
    updateFile(fi, frameVector, pMemFileLoader, pFileEvent, bDeletable, hasBatchOpLock, bNotify);
    //QTRACE((L"cSearchListener::update() - %s, memValid: %d, memLoaderValid: %d", sName.c_str(), pMem.isValid(), pMemFileLoader.isValid()));
  }
  catch (const exception& e)
  {
    QSOS((L"cSearchListener::update() - %S", e.what()));
  }
}

void cSearchListener::updateFile(const String& sName
  , const DWORD dwAtts
  , const cConstPtr<cMemoryView> pMem
  , const cPtr<iFileLoader> pMemFileLoader
  , const vfs::cPtr<iFileEvent> pFileEvent
  , const bool bDeletable
  , const bool bNotify)
{
  const String sMangledName(makeMangledName(sName));
  const fileInfo fi = iWriteInfo::singleton().makeFileInfo(dwAtts | FILE_ATTRIBUTE_READONLY
    , sMangledName);

  updateFile(fi, (cConstPtr<cMemoryView>)pMem, pFileEvent, bDeletable, bNotify);
  //updateFile(fi, (cConstPtr<cMemoryView>)pMem, cPtr<iFileEvent>(), bDeletable);
}

void cSearchListener::updateFolder(const String& sName
  , const cPtr<iChildLoader> pChildLoader
  , const cPtr<iRename> pRename
  , const cPtr<iCreate> pFolderCreate
  , const cConstPtr<cMemoryView> pIconMem
  , const cPtr<iFileEvent> pFileEvent
  , const bool bDeletable
  , const bool bNotify)
{
  const fileInfo fi = cWriteInfo::singleton().makeFileInfo(FILE_ATTRIBUTE_DIRECTORY
    , makeMangledName(sName));
  updateFolder(sName, fi, pChildLoader, pRename, pFolderCreate, pIconMem, pFileEvent, bDeletable, false, bNotify);
}

void cSearchListener::updateFile(const fileInfo& fi
  , const tFrameVector& frameVector
  , const cPtr<iFileLoader> pMemLoader
  , const vfs::cPtr<iFileEvent> pFileEvent
  , const bool bDeletable
  , bool hasBatchOpLock
  , const bool bNotify)
{
  fileInfo localFI(fi);
  makeLocalFI(fi, localFI);
  try
  {
    const cPtr<cMemComposite> pComposite = makeMemComposite(localFI, frameVector, pMemLoader, pFileEvent, bDeletable, hasBatchOpLock);
    if (pComposite.isNull())
      throw exception("couldn't create cMemComposite");

    const cConstPtr<iNamed> pNamed = new cConstNamed(localFI.w32Data.cFileName);
    if (pNamed.isNull())
      throw exception("couldn't create Named");

    const cPtr<cSearchListener> pThis = cPtr<cSearchListener>(this, this);
    const cPtr<iDirLocation> pDirLocation = new cMemDirLocation<cMemComposite>(pNamed, pThis, pComposite);
    if (pDirLocation.isNull())
      throw exception("couldn't create cMemDirLocation");
  
    secureAdd(fi.w32Data.cFileName, new cSearchLocation(pDirLocation
      , cPtr<cSearchListener>()
      , cPtr<iNotifyTree>()
      , cPtr<iChildLoader>()
      , m_pSearchCache
      )
      , false
      , bNotify);
    pComposite->artificialChange();
  }
  catch (const exception& e)
  {
    QSOS((L"cSearchListener::update() - %S", e.what()));
  }
}

void cSearchListener::updateFile(const fileInfo& fi
  , const cConstPtr<cMemoryView> pMem
  , const cPtr<iFileEvent> pFileEvent
  , const bool bDeletable
  , const bool bNotify)
{
  try
  {
    const cPtr<cSearchListener> pThis = cPtr<cSearchListener>(this, this);
    const cPtr<cConstMemComposite> pComposite = new cConstMemComposite(fi
      , pThis
      , pMem
      , pFileEvent
      , bDeletable
      , m_pRenameHandler);
    if (pComposite.isNull())
      throw exception("couldn't create cMemComposite");

    const String sName(fi.w32Data.cFileName);
    const cPtr<iDirLocation> pDirLocation = new cMemDirLocation<cConstMemComposite>(new cConstNamed(sName)
      , pThis
      , pComposite);
    if (pDirLocation.isNull())
      throw exception("couldn't create cMemDirLocation");
  
    secureAdd(sName
      , new cSearchLocation(pDirLocation
      , cPtr<cSearchListener>()
      , cPtr<iNotifyTree>()
      , cPtr<iChildLoader>()
      , m_pSearchCache)
      , false
      , bNotify);
  }
  catch (const exception& e)
  {
    QSOS((L"cSearchListener::update() - %S", e.what()));
  }
}

void cSearchListener::updateFolder(const String& filename
  , const fileInfo& fi
  , const cPtr<iChildLoader> pChildLoader
  , const cPtr<iRename> pRename
  , const cPtr<iCreate> pFolderCreate
  , const cConstPtr<cMemoryView> pIconMem
  , const cPtr<iFileEvent> pFileEvent
  , const bool bDeleteable
  , const bool bForceReplace
  , const bool bNotify)
{
  fileInfo localFI(fi);
  makeLocalFI(fi, localFI);

  if(pIconMem.isValid())
    localFI.w32Data.dwFileAttributes |= FILE_ATTRIBUTE_READONLY;

  cPtr<cSearchLocation> pSearchLocation;
  if(pChildLoader.isValid() && !pChildLoader->cacheable())
  {
    pSearchLocation = createFolderSearchLocation(localFI
      , pChildLoader
      , pRename
      , pFolderCreate
      , pIconMem
      , pFileEvent
      , bDeleteable);
  }
  else
  {
    pSearchLocation = m_pSearchCache->get(pChildLoader);
    if(pSearchLocation.isNull())
    {
      pSearchLocation = createFolderSearchLocation(localFI
        , pChildLoader
        , pRename
        , pFolderCreate
        , pIconMem
        , pFileEvent
        , bDeleteable);
      m_pSearchCache->add(pChildLoader, pSearchLocation);
    }
  }

  if(pSearchLocation.isValid())
    secureAdd(filename, pSearchLocation, bForceReplace, bNotify);
}


void cSearchListener::updateFolder(const fileInfo& fi
  , const cPtr<iChildLoader> pChildLoader
  , const cPtr<iRename> pRename
  , const cPtr<iCreate> pFolderCreate
  , const cConstPtr<cMemoryView> pIconMem
  , const cPtr<iFileEvent> pFileEvent
  , const bool bDeleteable
  , const bool bForceReplace
  , const bool bNotify)
{
  fileInfo localFI(fi);
  makeLocalFI(fi, localFI);

  if (pIconMem.isValid())
    localFI.w32Data.dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
  
  cPtr<cSearchLocation> pSearchLocation;
  if (pChildLoader.isValid() && !pChildLoader->cacheable())
  {
    pSearchLocation = createFolderSearchLocation(localFI
      , pChildLoader
      , pRename
      , pFolderCreate
      , pIconMem
      , pFileEvent
      , bDeleteable);
  }
  else
  {
    pSearchLocation = m_pSearchCache->get(pChildLoader);
    if (pSearchLocation.isNull())
    {      
      pSearchLocation = createFolderSearchLocation(localFI
        , pChildLoader
        , pRename
        , pFolderCreate
        , pIconMem
        , pFileEvent
        , bDeleteable);
      m_pSearchCache->add(pChildLoader, pSearchLocation);
    }
  }

  if (pSearchLocation.isValid())
    secureAdd(localFI.w32Data.cFileName, pSearchLocation, bForceReplace, bNotify);
}

cPtr<cSearchLocation> cSearchListener::createFolderSearchLocation(const fileInfo& localFI
  , const cPtr<iChildLoader> pChildLoader
  , const cPtr<iRename> pRename
  , const cPtr<iCreate> pFolderCreate
  , const cConstPtr<cMemoryView> pIconMem
  , const cPtr<iFileEvent> pFileEvent
  , const bool bDeleteable)
{
  const cPtr<cNotificationHandler> pNotificationHandler = new cNotificationHandler;
  const cPtr<cSearchListener> pSearchListener = new cSearchListener(pNotificationHandler
        , m_pNameMangler
        , pIconMem
        , m_pSearchCache
        , m_pRenameHandler);
  const cPtr<cNotifyTree> pNotifyTree = new cNotifyTree(pNotificationHandler);
  const cPtr<iDirLocation> pDirLocation = new cVirtualDirLocation(localFI
    , pSearchListener
    , pChildLoader
    , bDeleteable ? cPtr<cSearchListener>(this, this) : cPtr<cSearchListener>()
    , pRename
    , pNotifyTree
    , pNotificationHandler
    , pFolderCreate
    , pFileEvent
    , m_pRenameHandler);
  return new cSearchLocation(pDirLocation
    , pSearchListener
    , pNotifyTree
    , pChildLoader
    , m_pSearchCache);
}


void cSearchListener::updateFile(const fileInfo& fi
                                 , vfs::cPtr<iReadCallback> readCallback
                                 , const vfs::cPtr<iFileEvent> pFileEvent
                                 , bool bDeletable
                                 , const bool bNotify)
{
  cPtr<iComposite> composite = new cCallbackComposite(fi,cPtr<cSearchListener>(this, this),readCallback, pFileEvent, bDeletable);

  const cPtr<iDirLocation> pDirLocation = new cCompositeDirLocation(composite, ptr_cast<cPtr<iRename> >(readCallback));
  secureAdd(fi.w32Data.cFileName
    , new cSearchLocation(pDirLocation
      , cPtr<cSearchListener>()
      , cPtr<iNotifyTree>()
      , cPtr<iChildLoader>()
      , m_pSearchCache)
      , false
      , bNotify);
}

void cSearchListener::updateFile(const String& sName
                                 , cPtr<iReadCallback> readCallback
                                 , const vfs::cPtr<iFileEvent> pFileEvent
                                 , bool bDeletable
                                 , const bool bNotify)
{
  const fileInfo fi = iWriteInfo::singleton().makeFileInfo(FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_READONLY, sName);
  updateFile(fi, readCallback, pFileEvent, bDeletable, bNotify);
}

void cSearchListener::updateFile(vfs::cPtr<iWriteCallback> writeCallback
  , const fileInfo& fi
  , bool bDeletable
  , const bool bNotify)
{
  cPtr<iComposite> composite = new cWriteableCallbackComposite(fi,cPtr<cSearchListener>(this, this),writeCallback,bDeletable);

  const cPtr<iDirLocation> pDirLocation = new cCompositeDirLocation(composite, ptr_cast<cPtr<iRename> >(writeCallback));
  secureAdd(fi.w32Data.cFileName
    , new cSearchLocation(pDirLocation
      , cPtr<cSearchListener>()
      , cPtr<iNotifyTree>()
      , cPtr<iChildLoader>()
      , m_pSearchCache)
      , false
      , bNotify);
}

void cSearchListener::updateFile(
  vfs::cPtr<iWriteCallback> writeCallback
  , const vfs::String& sName
  , bool bDeletable
  , const bool bNotify)
{
  const fileInfo fi = iWriteInfo::singleton().makeFileInfo(FILE_ATTRIBUTE_NORMAL, sName);
  updateFile(writeCallback, fi, bDeletable, bNotify);
}

void cSearchListener::addFolder(const vfs::String& sName
  , const vfs::cPtr<iChildLoader> pChildLoader
  , const vfs::cPtr<iRename> pRename
  , const vfs::cPtr<iCreate> pFolderCreate
  , const vfs::cConstPtr<vfs::cMemoryView> pIconMem
  , unsigned int limit
  , const vfs::cPtr<iFileEvent> pFileEvent
  , const bool bDeletable
  , const bool bNotify)
{
  bool add = false;
  {
    cLockGuard lg(&m_access);
    typedef tDirList::nth_index<1>::type tLookup;
    tLookup& idIndex = m_dirList.get<1>();

    const tLookup::iterator finder = idIndex.find(sName);
    if (finder == idIndex.end())
    {
      add = true;
    }
  }

  if (add)
  {
    updateFolder(sName, pChildLoader, pRename, pFolderCreate, pIconMem, pFileEvent, bDeletable, bNotify);
    if(limit)
    {
      cLockGuard lg(&m_access);
      typedef tDirList::nth_index<0>::type tOrder;
      tOrder& idOrder = m_dirList.get<0>();
      if(limit < idOrder.size())
        idOrder.pop_front();
    }
  }
}

void cSearchListener::addFile(const vfs::String& sName
  , vfs::cPtr<iReadCallback> readCallback
  , const vfs::cPtr<iFileEvent> pFileEvent
  , unsigned int limit
  , bool bDeletable
  , const bool bNotify)
{
  bool add = false;
  {
    cLockGuard lg(&m_access);
    typedef tDirList::nth_index<1>::type tLookup;
    tLookup& idIndex = m_dirList.get<1>();

    const tLookup::iterator finder = idIndex.find(sName);
    if (finder == idIndex.end())
    {
      add = true;
    }
  }

  if (add)
  {
    updateFile(sName, readCallback, pFileEvent, bDeletable, bNotify);
    if(limit)
    {
      cLockGuard lg(&m_access);
      typedef tDirList::nth_index<0>::type tOrder;
      tOrder& idOrder = m_dirList.get<0>();
      if(limit < idOrder.size())
        idOrder.pop_front();
    }
  }
}
void cSearchListener::addFile(const vfs::String& sName
  , const DWORD dwAtts
  , const vfs::cConstPtr<vfs::cMemoryView> pMem
  , const vfs::cPtr<iFileLoader> pMemFileLoader
  , const vfs::cPtr<iFileEvent> pFileEvent
  , unsigned int limit
  , bool bDeletable
  , const bool bNotify)
{
  bool add = false;
  {
    cLockGuard lg(&m_access);
    typedef tDirList::nth_index<1>::type tLookup;
    tLookup& idIndex = m_dirList.get<1>();

    const tLookup::iterator finder = idIndex.find(sName);
    if (finder == idIndex.end())
    {
      add = true;
    }
  }

  if (add)
  {
    updateFile(sName, dwAtts, pMem, pMemFileLoader, pFileEvent, bDeletable, bNotify);
    if(limit)
    {
      cLockGuard lg(&m_access);
      typedef tDirList::nth_index<0>::type tOrder;
      tOrder& idOrder = m_dirList.get<0>();
      if(limit < idOrder.size())
        idOrder.pop_front();
    }
  }
}

void cSearchListener::rename(const String& sOriginalName
  , const String& sNewName
  , const cPtr<iChildLoader> pChildLoader)
{
  const String sMangledOriginalName(makeMangledName(sOriginalName));
  const cPtr<cSearchLocation> pSearchLocation = getChild(sMangledOriginalName, pChildLoader);
  if (pSearchLocation.isValid())
  {
    bool bRemoved(false);
    {
      cLockGuard lg(&m_access);
      {
        typedef tDirList::nth_index<1>::type tLookup;
        tLookup& idIndex = m_dirList.get<1>();

        const tLookup::iterator finder = idIndex.find(sMangledOriginalName);
        if (finder != idIndex.end())
        {
          idIndex.erase(finder);
          bRemoved = true;
        }
      }
    }
    if (bRemoved)
    {
      const cPtr<iDirLocation> pDirLocation = pSearchLocation->getDirLocation();
      const String sMangledNewName(makeMangledName(sNewName));
      pDirLocation->Rename(sMangledNewName);
      secureAdd(sMangledNewName, new cSearchLocation(pSearchLocation));
    }
  }
}

void cSearchListener::remove(const String& sName, bool notify)
{
  const String sMangledName(makeMangledName(sName));
  cPtr<cSearchLocation> pSearchLocation;

  {
    cLockGuard lg(&m_access);
    typedef tDirList::nth_index<1>::type tLookup;
    tLookup& idIndex = m_dirList.get<1>();

    const tLookup::iterator finder = idIndex.find(sMangledName);
    if (finder != idIndex.end())
    {
      pSearchLocation = finder->second;
      idIndex.erase(finder);
    }
  }

  if (notify && pSearchLocation.isValid() && m_pNotificationHandler.isValid())
  {
    pSearchLocation->clear();
    iNotificationTarget::tNotifyList notifyList;
    notifyList.push_back(iNotificationTarget::tNotifyList::value_type(FILE_ACTION_REMOVED, sMangledName));
    m_pNotificationHandler->Notify(notifyList);
  }
}

void cSearchListener::clear(bool notify)
{
  m_bInitialised = false;
  tDirList copy;
  {
    cLockGuard lg(&m_access);
    m_dirList.swap(copy);
  }

  for (tDirList::const_iterator cit = copy.begin()
    ; copy.end() != cit
    ; ++cit)
    if (cit->second.isValid())
      cit->second->clear();

  if (notify && m_pNotificationHandler.isValid())
    m_pNotificationHandler->Notify();
}

cSearchListener::tDirList cSearchListener::getChildren(const cPtr<iChildLoader> pChildLoader)
{
  cLockGuard lg(&m_access);
  if (pChildLoader.isValid())
  {
    if (m_dirList.empty() || !pChildLoader->cacheable())
    {
      m_bInitialised = false;
      pChildLoader->registerListener(cPtr<iChildLoaderVisitor>(this, this));
      m_bInitialised = true;
    }
  }

  return m_dirList;
}

cPtr<cSearchLocation> cSearchListener::getChild(const String& sName, const cPtr<iChildLoader> pChildLoader)
{
  const String sMangledName(makeMangledName(sName));
  {
    cLockGuard lg(&m_access);
    typedef tDirList::nth_index<1>::type tLookup;
    tLookup& idIndex = m_dirList.get<1>();

    const tLookup::const_iterator finder = idIndex.find(sMangledName);
    if (idIndex.end() != finder)
      return finder->second;
  }
  addToNameList(sName);
  if (pChildLoader.isValid())
  {
    //if(pChildLoader->lazyLoad(sName, this))
    //{
    //  iISAEssenceFolderManager::singleton().newLazilyLoadedFile(sName, this);
    //}
    //else
    {
      if (!m_dirList.empty() && pChildLoader->cacheable())
        return cPtr<cSearchLocation>();//early return - we're not going to find any new entries under these conditions anyway - and OS-X keeps looking for ._ files!
      getChildren(pChildLoader);
    }
    return getChild(sName, cPtr<iChildLoader>());
  }

  return cPtr<cSearchLocation>();
}

cPtr<iFileLoader> cSearchListener::getFileLoader(const String& sFileName)
{
  cLockGuard lg(&m_fileLoaderAccess);
  const tFileLoaderMap::const_iterator cit = m_FileLoaderMap.find(makeMangledName(sFileName));
  if (m_FileLoaderMap.end() != cit)
  {
    //QTRACE((L"cSearchListener::getFileLoader() - got file event for %s", sFileName.c_str()));
    return cit->second;
  }

  //QTRACE((L"cSearchListener::getFileLoader() - no file event for %s", sFileName.c_str()));
  return cPtr<iFileLoader>();
}

void cSearchListener::addSubtreeWatch(const cPtr<iNotificationReceiver> pNotificationHandler)
{
  cLockGuard lg(&m_access);
  for (cSearchListener::tDirList::const_iterator cit = m_dirList.begin()
    ; cit != m_dirList.end()
    ; ++cit)
  {
    const cPtr<cSearchLocation>& pSL = cit->second;
    pSL->addSubtreeWatch(pNotificationHandler);
  }
}

void cSearchListener::secureAdd(const String& sName, const cPtr<cSearchLocation> pSL, const bool bForceReplace, const bool bNotify)
{
  if (pSL.isValid())
  {
    iNotificationTarget::tNotifyList notifyList;
    {
      cLockGuard lg(&m_access);
      typedef tDirList::nth_index<1>::type tLookup;
      tLookup& idIndex = m_dirList.get<1>();

      const tLookup::iterator finder = idIndex.find(sName);
      if (finder != idIndex.end())
      {
        const cPtr<cSearchLocation>& pCurrSL = finder->second;
        const cPtr<iDirLocation> pCurrDL = pCurrSL->getDirLocation();
        fileInfo fi;
        pCurrDL->writeInfo(&fi, 0);
        const FILETIME currFt = fi.w32Data.ftCreationTime;

        const cPtr<iDirLocation> pNewDL = pSL->getDirLocation();
        pNewDL->writeInfo(&fi, 0);

        const LONG cmp = CompareFileTime(&currFt, &(fi.w32Data.ftCreationTime));
        if (-1 == cmp || (bForceReplace && 0 == cmp))
        {
          idIndex.erase(finder);
          idIndex.insert(tDirEntry(sName,pSL));
          notifyList.push_back(iNotificationTarget::tNotifyList::value_type(FILE_ACTION_MODIFIED, sName));
        }
      }
      else
      {
        idIndex.insert(tDirEntry(sName,pSL));
        notifyList.push_back(iNotificationTarget::tNotifyList::value_type(FILE_ACTION_ADDED, sName));
      }
    }

    if (m_bInitialised && !notifyList.empty() && bNotify)
      m_pNotificationHandler->Notify(notifyList);
  }
}

inline void cSearchListener::setFileLoader(const String& sFileName
  , const cPtr<iFileLoader> pFileLoader)
{
  //QTRACE((L"cSearchListener::setFileLoader() - %s", sFileName.c_str()));
  cLockGuard lg(&m_fileLoaderAccess);
  m_FileLoaderMap[makeMangledName(sFileName)] = pFileLoader;
}

inline cPtr<cMemComposite> cSearchListener::makeMemComposite(const fileInfo& fi
  , const tFrameVector& frameVector
  , const cPtr<iFileLoader> pFileLoader
  , vfs::cPtr<iFileEvent> pFileEvent
  , const bool bDeletable
  , bool hasBatchOpLock)
{
  if (pFileLoader.isValid())
  {
    setFileLoader(fi.w32Data.cFileName, pFileLoader);
    return new cMemComposite(fi
      , cPtr<cSearchListener>(this, this)
      , m_pNotificationHandler
      , bDeletable
      , frameVector
      , pFileLoader
      , pFileEvent
      , m_pRenameHandler
      , hasBatchOpLock);
  }
  
  return new cMemComposite(fi
    , cPtr<cSearchListener>(this, this)
    , m_pNotificationHandler
    , bDeletable
    , frameVector
    , getFileLoader(fi.w32Data.cFileName)
    , pFileEvent
    , m_pRenameHandler
    , hasBatchOpLock);
}

String cSearchListener::makeMangledName(const String& sName) const
{
  if (m_pNameMangler.isValid())
    return m_pNameMangler->makeMangledName(sName);
  return sName;
}

String cSearchListener::makeMangledName(const String& sName
  , const std::vector<String>& replacePatterns) const
{
  if (m_pNameMangler.isValid())
    return m_pNameMangler->makeMangledName(sName, replacePatterns);
  return sName;
}

inline void cSearchListener::makeLocalFI(const fileInfo& fi, fileInfo& localFI) const
{
  wcsncpy(localFI.w32Data.cFileName
    , makeMangledName(fi.w32Data.cFileName).c_str()
    , (sizeof(fi.w32Data.cFileName)/sizeof(fi.w32Data.cFileName[0])-1));
}

void cSearchListener::addToNameList(const vfs::String& name )
{
  cLockGuard guard(&m_fileNameListLock);
  while(m_fileNameList.size() >= kMaxListSize)
  {
    m_fileNameList.pop_front();
  }
  m_fileNameList.push_back(name);
}
