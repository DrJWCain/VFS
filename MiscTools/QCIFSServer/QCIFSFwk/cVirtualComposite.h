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

//cVirtualComposite.h

#pragma once

#include "cNotifyComposite.h"

class cSearchListener;
class iNotifyTree;
class cNotificationHandler;
class iChildLoader;
class iRenameHandler;

class cVirtualComposite : public cNotifyComposite, public vfs::cRefCount
{
  const vfs::cPtr<cSearchListener> m_pSearchListener;
  const vfs::cPtr<iChildLoader> m_pChildLoader;
  const vfs::cPtr<iRename> m_pRename;
  const vfs::cPtr<iRenameHandler> m_pRenameHandler;
public:
  cVirtualComposite(const fileInfo& fi
    , const vfs::cPtr<cSearchListener> pSearchListener
    , const vfs::cPtr<cSearchListener> pParentSearchListener
    , const vfs::cPtr<iChildLoader> pChildLoader
    , const vfs::cPtr<iNotifyTree> pNotifyTree
    , const vfs::cPtr<cNotificationHandler> pNotificationReceiver
    , const vfs::cPtr<iFileEvent> fileEvent
    , const vfs::cPtr<iRename> pRename
    , const vfs::cPtr<iRenameHandler> pRenameHandler);
  ~cVirtualComposite(void);

  DWORD Read(tTransmitList &krTPM, DWORD& nBytes, const LARGE_INTEGER &nOffset, const int sessionID, ULONGLONG fid);
  DWORD Write(vfs::cConstPtr<vfs::cMemoryView> buffer, DWORD& nBytes, const LARGE_INTEGER &offset, const int sessionID, ULONGLONG fid);
  DWORD SetFileSize(const PLARGE_INTEGER pFileSize);
  DWORD Delete();
  DWORD Close(ULONGLONG fid) {return ERROR_SUCCESS;}
  DWORD Lock(const LARGE_INTEGER &lgOffset, const LARGE_INTEGER &lgBytes, const bool bLock) {return ERROR_ACCESS_DENIED;}
  DWORD registerNotification(const ULONGLONG requestID
    , vfs::cPtr<iNotificationReceiver> pNotification);
  DWORD registerChangeListener(vfs::cPtr<iFileChangeListener> listener) {return ERROR_NOT_SUPPORTED;}

  //SMB2 search done by handle rather than path
  void getChildren(const vfs::String& sSearchMask, tFileInfoList& searchResults);
  bool find(const vfs::String& name, tFileInfoList& searchResults);
  DWORD Rename(const vfs::String& sNewName);
  bool hasSecurityDescriptor() { return false; }
};
