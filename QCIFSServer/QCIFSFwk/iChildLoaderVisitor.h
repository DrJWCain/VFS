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

//iChildLoaderVisitor.h

#pragma once

#include "iChildLoader.h"
#include "iRename.h"
#include "iFileEvent.h"
#include "iFileLoader.h"
#include "iCreate.h"

#include "..\iComposite.h"

class iCallbackChangeListener
{
public:
  virtual ~iCallbackChangeListener() {}

  virtual bool stillListening() = 0;
  virtual void callbackChange() = 0;
};

//Support for lazy file contents delivery.
class iReadCallback
{
public:
  virtual ~iReadCallback() {}
  virtual unsigned __int64 getSize(ULONGLONG fid) = 0;
  virtual DWORD readBytes(tTransmitList &krTPM, DWORD& nBytes, const LARGE_INTEGER &nOffset, const int sessionID, ULONGLONG fid) = 0;
  virtual bool canExecute() { return false; }//defaults to no execution rights
  virtual bool canCache() { return true; }//defaults to SMB2 BATCH OPLOCK
  virtual DWORD registerChangeListener(vfs::cPtr<iCallbackChangeListener> listener) = 0;
  virtual DWORD close(ULONGLONG fid) {return 0;}
};

class iWriteCallback : public iReadCallback
{
public:
  virtual DWORD setSize(unsigned __int64 newSize) = 0;
  virtual DWORD writeBytes(vfs::cConstPtr<vfs::cMemoryView> buffer, const LARGE_INTEGER &offset, const int sessionID, ULONGLONG fid) = 0;
  virtual DWORD close(ULONGLONG fid) = 0;
};

class iChildLoaderVisitor
{
public:
  virtual ~iChildLoaderVisitor(){}

  virtual void update(const vfs::String& sPath) = 0;

  virtual void updateFile(const vfs::String& sName
    , const DWORD dwAtts
    , const tFrameVector& frameVector
    , const vfs::cPtr<iFileLoader> pMemFileLoader
    , const vfs::cPtr<iFileEvent> pFileEvent = vfs::cPtr<iFileEvent>()
    , const bool bDeletable=false
    , bool hasBatchOpLock=true
    , const bool bNotify = true) = 0;
  virtual void updateFile(const fileInfo& fi
    , const tFrameVector& frameVector
    , const vfs::cPtr<iFileLoader> pMemLoader
    , const vfs::cPtr<iFileEvent> pFileEvent
    , const bool bDeletable=false
    , bool hasBatchOpLock=true
    , const bool bNotify = true) = 0;

  virtual void updateFile(const vfs::String& sName
    , const DWORD dwAtts
    , const vfs::cConstPtr<vfs::cMemoryView> pMem
    , const vfs::cPtr<iFileLoader> pMemFileLoader
    , const vfs::cPtr<iFileEvent> pFileEvent = vfs::cPtr<iFileEvent>()
    , const bool bDeletable=false
    , const bool bNotify = true) = 0;
  virtual void updateFile(const fileInfo& fi
    , const vfs::cConstPtr<vfs::cMemoryView> pMem
    , const vfs::cPtr<iFileEvent> pFileEvent
    , const bool bDeletable=false
    , const bool bNotify = true) = 0;

  virtual void updateFolder(const vfs::String& sName
    , const vfs::cPtr<iChildLoader> pChildLoader
    , const vfs::cPtr<iRename> pRename
    , const vfs::cPtr<iCreate> pFolderCreate
    , const vfs::cConstPtr<vfs::cMemoryView> pIconMem
    , const vfs::cPtr<iFileEvent> pFileEvent = vfs::cPtr<iFileEvent>()
    , const bool bDeletable=false
    , const bool bNotify = true) = 0;
  virtual void updateFolder(const fileInfo& fi
    , const vfs::cPtr<iChildLoader> pChildLoader
    , const vfs::cPtr<iRename> pRename
    , const vfs::cPtr<iCreate> pFolderCreate
    , const vfs::cConstPtr<vfs::cMemoryView> pIconMem
    , const vfs::cPtr<iFileEvent> pFileEvent = vfs::cPtr<iFileEvent>()
    , const bool bDeletable=false
    , const bool bForceReplace=false
    , const bool bNotify = true) = 0;

  virtual void updateFile(const vfs::String& sName
    , vfs::cPtr<iReadCallback> readCallback
    , const vfs::cPtr<iFileEvent> pFileEvent = vfs::cPtr<iFileEvent>()
    , bool bDeletable = true
    , const bool bNotify = true) = 0;
  virtual void updateFile(
    vfs::cPtr<iWriteCallback> writeCallback
    , const vfs::String& sName
    , bool bDeletable = true
    , const bool bNotify = true) = 0;

  virtual void updateFile(const fileInfo& fi
    , vfs::cPtr<iReadCallback> readCallback
    , const vfs::cPtr<iFileEvent> pFileEvent
    , bool bDeletable = true
    , const bool bNotify = true) = 0;
  virtual void updateFile(
    vfs::cPtr<iWriteCallback> writeCallback
    , const fileInfo& fi
    , bool bDeletable = true
    , const bool bNotify = true) = 0;

  virtual void addFile(const vfs::String& sName
    , vfs::cPtr<iReadCallback> readCallback
    , const vfs::cPtr<iFileEvent> pFileEvent = vfs::cPtr<iFileEvent>()
    , unsigned int limit = 0
    , bool bDeletable = true
    , const bool bNotify = true) = 0;
  virtual void addFile(const vfs::String& sName
    , const DWORD dwAtts
    , const vfs::cConstPtr<vfs::cMemoryView> pMem
    , const vfs::cPtr<iFileLoader> pMemFileLoader
    , const vfs::cPtr<iFileEvent> pFileEvent = vfs::cPtr<iFileEvent>()
    , unsigned int limit = 0
    , bool bDeletable = true
    , const bool bNotify = true) = 0;
  virtual void addFolder(const vfs::String& sName
    , const vfs::cPtr<iChildLoader> pChildLoader
    , const vfs::cPtr<iRename> pRename
    , const vfs::cPtr<iCreate> pFolderCreate
    , const vfs::cConstPtr<vfs::cMemoryView> pIconMem
    , unsigned int limit = 0
    , const vfs::cPtr<iFileEvent> pFileEvent = vfs::cPtr<iFileEvent>()
    , const bool bDeletable=false
    , const bool bNotify = true) = 0;

  virtual void rename(const vfs::String& sOriginalName
    , const vfs::String& sNewName
    , const vfs::cPtr<iChildLoader> pChildLoader) = 0;
  virtual void remove(const vfs::String& sName, bool notify = true) = 0;
  virtual void clear(bool notify = true) = 0;
};