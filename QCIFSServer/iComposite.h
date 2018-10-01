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

//iComposite.h

#pragma once

#ifdef CLIENT_MODE
#define CLIENT_DLL_MODE __declspec(dllexport)
#else
#define CLIENT_DLL_MODE __declspec(dllimport)
#endif

#include "Common\TransmitList.h"
#include "QCIFSProcessor\iNotificationReceiver.h"
#include "QCIFSFwk\iNamed.h"

typedef struct _fileInfo
{
  WIN32_FIND_DATAW w32Data;
  LARGE_INTEGER allocSize;
  ULONG extFileAtts;
  ULONG extAttsSize;
} fileInfo, *PfileInfo;

class iFileChangeListener
{
public:
  virtual ~iFileChangeListener() {}

  virtual bool stillListening() = 0;
  virtual void internalFileChange() = 0;
};

typedef std::vector<fileInfo> tFileInfoList;

class iTransactCallback
{
public:
  virtual DWORD transact(vfs::cConstPtr<vfs::cMemoryView> buffer, tTransmitList &krTPM, DWORD& nBytesRead, const int sessionID, ULONGLONG fid) = 0;
};

class iComposite : public iNamed
{
public:
  virtual ~iComposite() {}
    
  virtual DWORD writeInfo(LPBY_HANDLE_FILE_INFORMATION pFileInfo, ULONGLONG fid) = 0;
  virtual DWORD writeInfo(PfileInfo pFileInfo, ULONGLONG fid) = 0;
  /*** Read - return number of bytes read in ***/
  virtual DWORD Read(tTransmitList &krTPM, DWORD& nBytes, const LARGE_INTEGER &nOffset, const int sessionID, ULONGLONG fid) = 0;
  /*** Write - set number of bytes written in nYbtes, return error code (0 = success)***/
  virtual DWORD Write(vfs::cConstPtr<vfs::cMemoryView> buffer, DWORD &nBytes, const LARGE_INTEGER &offset, const int sessionID, ULONGLONG fid) = 0;
  virtual DWORD SetFileSize(const PLARGE_INTEGER pFileSize) = 0;
  virtual DWORD SetFileInfo(const FILETIME& creationTime
    , const FILETIME& modifiedTime, const FILETIME& accessTime, const DWORD attributes) = 0;
  /*** MarkForDeletion - return 0 for success or error code ***/
  virtual DWORD Delete() = 0;
  /*** Close - return 0 for success or error code ***/
  virtual DWORD Close(ULONGLONG fid) = 0;
  /*** Lock - return 0 for success or error code ***/
  virtual DWORD Lock(const LARGE_INTEGER &lgOffset, const LARGE_INTEGER &lgBytes, const bool bLock) = 0;
  virtual DWORD registerNotification(const ULONGLONG requestID
    , vfs::cPtr<iNotificationReceiver> pNotification) = 0;
  virtual DWORD cancelNotification(const ULONGLONG requestID, bool synchronous) = 0;

  virtual DWORD registerChangeListener(vfs::cPtr<iFileChangeListener> listener) = 0;

  //SMB2 search done by handle rather than path
  virtual void getChildren(const vfs::String& sSearchMask, tFileInfoList& searchResults) = 0;
  virtual bool find(const vfs::String& name, tFileInfoList& searchResults) = 0;
  virtual DWORD Rename(const vfs::String& sNewName) = 0;
  virtual bool hasSecurityDescriptor() = 0;
  virtual bool hasBatchOpLock() {return true;}

  virtual void triggerNotifications() = 0;
  virtual bool canBeDeleted() = 0;
  virtual vfs::cPtr<iTransactCallback> getTransactInterface() = 0;
};