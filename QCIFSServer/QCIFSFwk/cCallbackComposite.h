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

//cCallbackComposite.h

#pragma once

#include "cDeletableComposite.h"

class cCallbackComposite : public cDeletableComposite, public vfs::cRefCount
{
public:
  cCallbackComposite(const fileInfo& fi
    , const vfs::cPtr<cSearchListener> pSearchListener
    , vfs::cPtr<iReadCallback> readCallback
    , const vfs::cPtr<iFileEvent> pFileEvent
    , bool bDeletable);
  ~cCallbackComposite(void);

  //iComposite
  DWORD writeInfo(LPBY_HANDLE_FILE_INFORMATION pFileInfo, ULONGLONG fid);
  DWORD writeInfo(PfileInfo pFileInfo, ULONGLONG fid);
  DWORD Read(tTransmitList &krTPM, DWORD& nBytes, const LARGE_INTEGER &nOffset, const int sessionID, ULONGLONG fid);
  DWORD Write(vfs::cConstPtr<vfs::cMemoryView> buffer
    , DWORD &nBytes
    , const LARGE_INTEGER &offset
    , const int sessionID
    , ULONGLONG fid) {return ERROR_ACCESS_DENIED;}
  DWORD SetFileSize(const PLARGE_INTEGER pFileSize) {return ERROR_ACCESS_DENIED;}
  DWORD Close(ULONGLONG fid);
  bool hasSecurityDescriptor();
  bool hasBatchOpLock();
  DWORD registerChangeListener(vfs::cPtr<iFileChangeListener> listener);

  //public member functions
  bool canDelete() const {return cDeletableComposite::canDelete();}

private:
  vfs::cPtr<iReadCallback> ReadCallback;
  unsigned __int64 CurrentSize;

  void checkAttibutesValid(ULONGLONG fid);
  const vfs::cPtr<cNotificationHandler> m_pNotificationHandler;
};

class cWriteableCallbackComposite : public cDeletableComposite, public vfs::cRefCount
{
public:
  cWriteableCallbackComposite(const fileInfo& fi
    , const vfs::cPtr<cSearchListener> pSearchListener
    , vfs::cPtr<iWriteCallback> writeCallback
    , bool bDeletable);
  ~cWriteableCallbackComposite(void);

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
  bool hasSecurityDescriptor() { return false; }
  DWORD registerChangeListener(vfs::cPtr<iFileChangeListener> listener);

  //public member functions
  bool canDelete() const {return cDeletableComposite::canDelete();}
  vfs::cPtr<iTransactCallback> getTransactInterface() override;

private:
  vfs::cPtr<iWriteCallback> WriteCallback;
};
