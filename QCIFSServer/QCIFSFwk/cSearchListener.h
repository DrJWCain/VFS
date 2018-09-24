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

//cSearchListener.h

#pragma once

#include "iChildLoaderVisitor.h"
#include "iNameMangler.h"
#include "iFileNameReporter.h"

class cSearchLocation;
class cNotificationHandler;
class iChildLoader;
class cMemComposite;
class cSearchLocationCache;
class iRenameHandler;

#include "boost\multi_index_container.hpp"
#include "boost\multi_index\sequenced_index.hpp"
#include "boost\multi_index\ordered_index.hpp"

using namespace ::boost::multi_index;

class cSearchListener : public iChildLoaderVisitor, public iNameMangler, public iFileNameReporter, public vfs::cRefCount
{
public:
  static std::string m_sIconName;
  typedef std::pair<vfs::String, vfs::cPtr<cSearchLocation> > tDirEntry;
  struct _sort
  {
    bool operator () (const tDirEntry& lhs, const tDirEntry& rhs) const
    {
      return vfs::toLower(lhs.first) < vfs::toLower(rhs.first);
    }
    bool operator () (const tDirEntry& lhs, const vfs::String& rhs) const
    {
      return vfs::toLower(lhs.first) < vfs::toLower(rhs);
    }
    bool operator () (const vfs::String& lhs, const tDirEntry& rhs) const
    {
      return vfs::toLower(lhs) < vfs::toLower(rhs.first);
    }
  };

  typedef multi_index_container<
    tDirEntry,
    indexed_by<
      sequenced<>,
      ordered_unique<identity<tDirEntry>, _sort>
    >
  > tDirList;


  cSearchListener(const vfs::cPtr<cNotificationHandler> pNotificationHandler
    , const vfs::cPtr<iNameMangler> pNameMangler
    , const vfs::cConstPtr<vfs::cMemoryView> pIconMem
    , const vfs::cPtr<cSearchLocationCache>pSearchCache
    , const vfs::cPtr<iRenameHandler> pRenameHandler);
  ~cSearchListener(void);

  //iChildLoaderVisitor
  void update(const vfs::String& sPath);

  void updateFile(const vfs::String& sName
    , const DWORD dwAtts
    , const tFrameVector& frameVector
    , const vfs::cPtr<iFileLoader> pMemFileLoader
    , const vfs::cPtr<iFileEvent> pFileEvent
    , const bool bDeletable
    , bool hasBatchOpLock
    , const bool bNotify);
  void updateFile(const fileInfo& fi
    , const tFrameVector& frameVector
    , const vfs::cPtr<iFileLoader> pMemLoader
    , const vfs::cPtr<iFileEvent> pFileEvent
    , const bool bDeletable
    , bool hasBatchOpLock
    , const bool bNotify);

  void updateFile(const vfs::String& sName
    , const DWORD dwAtts
    , const vfs::cConstPtr<vfs::cMemoryView> pMem
    , const vfs::cPtr<iFileLoader> pMemFileLoader
    , const vfs::cPtr<iFileEvent> pFileEvent
    , const bool bDeletable
    , const bool bNotify);
  void updateFile(const fileInfo& fi
    , const vfs::cConstPtr<vfs::cMemoryView> pMem
    , const vfs::cPtr<iFileEvent> pFileEvent
    , const bool bDeletable
    , const bool bNotify);

  void updateFolder(const vfs::String& sName
    , const vfs::cPtr<iChildLoader> pChildLoader
    , const vfs::cPtr<iRename> pRename
    , const vfs::cPtr<iCreate> pFolderCreate
    , const vfs::cConstPtr<vfs::cMemoryView> pIconMem
    , const vfs::cPtr<iFileEvent> pFileEvent
    , const bool bDeletable
    , const bool bNotify);
  void updateFolder(const fileInfo& fi
    , const vfs::cPtr<iChildLoader> pChildLoader
    , const vfs::cPtr<iRename> pRename
    , const vfs::cPtr<iCreate> pFolderCreate
    , const vfs::cConstPtr<vfs::cMemoryView> pIconMem
    , const vfs::cPtr<iFileEvent> pFileEvent
    , const bool bDeletable
    , const bool bForceReplace
    , const bool bNotify);

  virtual void updateFile(const vfs::String& sName
    , vfs::cPtr<iReadCallback> readCallback
    , const vfs::cPtr<iFileEvent> pFileEvent
    , bool bDeletable
    , const bool bNotify);
  virtual void updateFile(
    vfs::cPtr<iWriteCallback> writeCallback
    , const vfs::String& sName
    , bool bDeletable
    , const bool bNotify);

  virtual void updateFile(const fileInfo& fi
    , vfs::cPtr<iReadCallback> readCallback
    , const vfs::cPtr<iFileEvent> pFileEvent
    , bool bDeletable
    , const bool bNotify);
  virtual void updateFile(
    vfs::cPtr<iWriteCallback> writeCallback
    , const fileInfo& fi
    , bool bDeletable
    , const bool bNotify);
  

  virtual void addFile(const vfs::String& sName
    , vfs::cPtr<iReadCallback> readCallback
    , const vfs::cPtr<iFileEvent> pFileEvent
    , unsigned int limit
    , bool bDeletable
    , const bool bNotify);
  virtual void addFile(const vfs::String& sName
    , const DWORD dwAtts
    , const vfs::cConstPtr<vfs::cMemoryView> pMem
    , const vfs::cPtr<iFileLoader> pMemFileLoader
    , const vfs::cPtr<iFileEvent> pFileEvent = vfs::cPtr<iFileEvent>()
    , unsigned int limit = 0
    , bool bDeletable = true
    , const bool bNotify = true);
  virtual void addFolder(const vfs::String& sName
    , const vfs::cPtr<iChildLoader> pChildLoader
    , const vfs::cPtr<iRename> pRename
    , const vfs::cPtr<iCreate> pFolderCreate
    , const vfs::cConstPtr<vfs::cMemoryView> pIconMem
    , unsigned int limit
    , const vfs::cPtr<iFileEvent> pFileEvent
    , const bool bDeletable
    , const bool bNotify);

  void rename(const vfs::String& sOriginalName
    , const vfs::String& sNewName
    , const vfs::cPtr<iChildLoader> pChildLoader);
  void remove(const vfs::String& sName, bool notify = true);
  void clear(bool notify = true);

  vfs::String makeMangledName(const vfs::String& sFullName) const;
  vfs::String makeMangledName(const vfs::String& sFullName
    , const std::vector<vfs::String>& replacePatterns) const;

  //public member functions
  tDirList getChildren(const vfs::cPtr<iChildLoader> pChildLoader);
  vfs::cPtr<cSearchLocation> getChild(const vfs::String& sName, const vfs::cPtr<iChildLoader> pChildLoader);
  vfs::cPtr<iFileLoader> getFileLoader(const vfs::String& sFileName);
  void addSubtreeWatch(const vfs::cPtr<iNotificationReceiver> pNotificationHandler);

  virtual std::list<vfs::String> getNameList() const { vfs::cLockGuard guard(&m_fileNameListLock); return m_fileNameList;}

  vfs::cGuid getGuid() const { return m_guid; }

private:  //private member functions
  void secureAdd(const vfs::String& sName, const vfs::cPtr<cSearchLocation> pSL, const bool bForceReplace=false, const bool bNotify = true);
  void setFileLoader(const vfs::String& sFileName, const vfs::cPtr<iFileLoader> pFileLoader);
  vfs::cPtr<cMemComposite> makeMemComposite(const fileInfo& fi
    , const tFrameVector& frameVector
    , const vfs::cPtr<iFileLoader> pFileLoader
    , vfs::cPtr<iFileEvent> pFileEvent
    , const bool bDeletable
    , bool hasBatchOpLock);
  void makeLocalFI(const fileInfo& fi, fileInfo& localFI) const;
  vfs::cPtr<cSearchLocation> createFolderSearchLocation(const fileInfo& fi
    , const vfs::cPtr<iChildLoader> pChildLoader
    , const vfs::cPtr<iRename> pRename
    , const vfs::cPtr<iCreate> pFolderCreate
    , const vfs::cConstPtr<vfs::cMemoryView> pIconMem
    , const vfs::cPtr<iFileEvent> pFileEvent
    , const bool bDeleteable);

  void addToNameList(const vfs::String& name);

  void updateFolder(const vfs::String& filename
    , const fileInfo& fi
    , const vfs::cPtr<iChildLoader> pChildLoader
    , const vfs::cPtr<iRename> pRename
    , const vfs::cPtr<iCreate> pFolderCreate
    , const vfs::cConstPtr<vfs::cMemoryView> pIconMem
    , const vfs::cPtr<iFileEvent> pFileEvent
    , const bool bDeleteable
    , const bool bForceReplace
    , const bool bNotify);

private:  //private member variables
  const vfs::cPtr<cNotificationHandler> m_pNotificationHandler;
  const vfs::cPtr<iNameMangler> m_pNameMangler;
  const vfs::cPtr<cSearchLocationCache>m_pSearchCache;
  const vfs::cPtr<iRenameHandler> m_pRenameHandler;
  //const vfs::cConstPtr<vfs::cMemoryView> m_pIconMem;
  //static vfs::cConstPtr<vfs::cMemoryView> m_pDesktopINIFile;
  //static fileInfo m_iconFI;
  //static fileInfo m_desktopIniFI;
  vfs::cGuid m_guid;
  bool m_bInitialised;

  mutable vfs::cLock m_access;
  tDirList m_dirList;

  typedef std::map<vfs::String, vfs::cPtr<iFileLoader> > tFileLoaderMap;
  vfs::cLock m_fileLoaderAccess;
  tFileLoaderMap m_FileLoaderMap;

  std::list<vfs::String> m_fileNameList;
  mutable vfs::cLock m_fileNameListLock;
};
