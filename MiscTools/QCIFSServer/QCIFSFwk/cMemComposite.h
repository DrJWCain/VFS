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

//cMemComposite.h

#pragma once

#include "cDeletableComposite.h"
#include "cQCIFSFwkHelper.h"
#include "iFileLoader.h"

class cNotificationHandler;

class cMemComposite : public cDeletableComposite, public iFileEventCallback, public vfs::cRefCount
{
public:
  cMemComposite(const fileInfo& fi
    , const vfs::cPtr<cSearchListener> pSearchListener
    , const vfs::cPtr<cNotificationHandler> pNotificationHandler
    , const bool bDeletable
    , const tFrameVector& frameVector
    , const vfs::cPtr<iFileLoader> pFileLoader
    , vfs::cPtr<iFileEvent> pFileEvent  
    , const vfs::cPtr<iRenameHandler> pRenameHandler
    , bool hasBatchOpLock);
  ~cMemComposite(void);

  //iComposite
  DWORD writeInfo(LPBY_HANDLE_FILE_INFORMATION pFileInfo, ULONGLONG fid);
  DWORD writeInfo(PfileInfo pFileInfo, ULONGLONG fid);
  DWORD Read(tTransmitList &krTPM, DWORD& nBytes, const LARGE_INTEGER &nOffset, const int sessionID, ULONGLONG fid);
  DWORD Write(vfs::cConstPtr<vfs::cMemoryView> buffer
    , DWORD &nBytes
    , const LARGE_INTEGER &offset
    , const int sessionID
    , ULONGLONG fid);
  DWORD SetFileSize(const PLARGE_INTEGER pFileSize);
  DWORD Close(ULONGLONG fid);
  DWORD Rename(const vfs::String& sNewName);
  bool hasSecurityDescriptor() { return false; }
  virtual bool hasBatchOpLock() { return m_bBatchOpLock; }
  DWORD registerChangeListener(vfs::cPtr<iFileChangeListener> listener) {return ERROR_NOT_SUPPORTED;}

  //iFileEventCallback
  void clearMemCache();
  void errorRemoveFromDirectory();
  
  //public member functions
  tFrameVector getMem() const {return m_frameVector;}
  vfs::cPtr<iFileLoader> getMemLoader() {return m_pMemLoader;}
  void artificialChange();
  bool canDelete() const {return cDeletableComposite::canDelete();}

private:  //private member functions
  template <typename T>
  DWORD writeInfoTemplate(T pFileInfo);
  void doNotify(ULONGLONG fid);
  void sendValidNotifications();
  tFrameVector copyFrameVector() const;

private:  //private member variables
  mutable vfs::cLock m_access;
  mutable tFrameVector m_frameVector;
  const vfs::cPtr<cNotificationHandler> m_pNotificationHandler;
  const vfs::cPtr<iFileLoader> m_pMemLoader;
  const vfs::cPtr<iRenameHandler> m_pRenameHandler;
  volatile LONG m_sessionID;
  mutable bool m_bWritten;
  unsigned int m_uiHeaderBytes;
  unsigned int m_uiBytesRead;
  bool m_bBatchOpLock;
};