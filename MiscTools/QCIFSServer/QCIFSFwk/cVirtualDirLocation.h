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

//cVirtualDirLocation.h

#pragma once

#include "iDirLocation.h"

#include "..\iComposite.h"

class iRename;
class iNamed;
class cSearchListener;
class iNotifyTree;
class cNotificationHandler;
class iCreate;
class iChildLoader;
class iFileEvent;
class iRenameHandler;

class cVirtualDirLocation : public iDirLocation, public vfs::cRefCount
{
public:
  cVirtualDirLocation(const fileInfo& rFileInfo
    , const vfs::cPtr<cSearchListener> pSearchListener
    , const vfs::cPtr<iChildLoader> pChildLoader
    , const vfs::cPtr<cSearchListener> pParentSearchListener
    , const vfs::cPtr<iRename> pRename
    , const vfs::cPtr<iNotifyTree> pNotifyTree
    , const vfs::cPtr<cNotificationHandler> pNotificationReceiver
    , const vfs::cPtr<iCreate> pFolderCreate
    , const vfs::cPtr<iFileEvent> pFileEvent
    , const vfs::cPtr<iRenameHandler> pRenameHandler);

  ~cVirtualDirLocation(void);

  vfs::cPtr<iComposite> Create(const vfs::String& sName
    , DWORD dwDesiredAccess
    , DWORD dwShareMode
    , LPSECURITY_ATTRIBUTES lpAtts
    , DWORD dwCreationDisposition
    , DWORD dwFlagsAndAtts
    , HANDLE hTemplateFile
    , DWORD& dwRet
    , vfs::cPtr<iFileProcessingCallback> pCallback);
  vfs::cPtr<iRename> Rename();
  DWORD Rename(const vfs::cPtr<iRename> pRenameSource
    , const vfs::String& sNewName
    , const vfs::String& sUserName);
  void Rename(const vfs::String& sNewName);
  DWORD writeInfo(const PfileInfo pFileInfo, ULONGLONG fid);
  void refreshCache();

private:  //private member functions
  template <typename T>
  DWORD tryFileRename(const vfs::cPtr<iRename> pRenameSource
    , const vfs::String& sNewName
    , const vfs::String& sUserName);

private:  //private member variables
  vfs::cLock m_rename;
  fileInfo m_fileInfo;
  const vfs::cPtr<cSearchListener> m_pSearchListener;
  const vfs::cPtr<iChildLoader> m_pChildLoader;
  const vfs::cPtr<cSearchListener> m_pParentSearchListener;
  const vfs::cPtr<iRename> m_pRename;
  const vfs::cPtr<iNotifyTree> m_pNotifyTree;
  const vfs::cPtr<cNotificationHandler> m_pNotificationReceiver;
  const vfs::cPtr<iCreate> m_pCreate;
  const vfs::cPtr<iFileEvent> m_pFileEvent;
  const vfs::cPtr<iRenameHandler> m_pRenameHandler;
};