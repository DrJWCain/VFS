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

//cWindowsComposite.h

#pragma once

#include "..\icomposite.h"

class iIOCompletionPort;
class cNotificationThread;

class cWindowsComposite : public iComposite, public vfs::cRefCount
{
public:
  cWindowsComposite(const vfs::String& sName, const HANDLE h, iIOCompletionPort* const pIOCP);
  ~cWindowsComposite(void);

  //iComposite
  vfs::String Name() const { return m_sPath;}
  DWORD writeInfo(LPBY_HANDLE_FILE_INFORMATION pFileInfo, ULONGLONG fid);
  DWORD writeInfo(PfileInfo pFileInfo, ULONGLONG fid);
  virtual DWORD Read(tTransmitList &krTPM, DWORD& nBytes, const LARGE_INTEGER &nOffset, const int sessionID, ULONGLONG fid) = 0;
  virtual DWORD Write(vfs::cConstPtr<vfs::cMemoryView> buffer, DWORD &nBytes, const LARGE_INTEGER &offset, const int sessionID, ULONGLONG fid) = 0;
  virtual DWORD SetFileSize(const PLARGE_INTEGER pFileSize) = 0;
  virtual DWORD SetFileInfo(const FILETIME& creationTime
    , const FILETIME& modifiedTime, const FILETIME& accessTime, const DWORD attributes) = 0;
  DWORD Delete();
  DWORD Close(ULONGLONG fid);
  virtual DWORD Lock(const LARGE_INTEGER &lgOffset, const LARGE_INTEGER &lgBytes, const bool bLock) = 0;
  DWORD registerNotification(const ULONGLONG requestID
    , vfs::cPtr<iNotificationReceiver> pNotification);
  DWORD cancelNotification(const ULONGLONG requestID, bool synchronous);
  
  DWORD registerChangeListener(vfs::cPtr<iFileChangeListener> listener) {return ERROR_NOT_SUPPORTED;}

  void getChildren(const vfs::String& sSearchMask, tFileInfoList& searchResults){}
  bool find(const vfs::String& name, tFileInfoList& searchResults){return false;}
  DWORD Rename(const vfs::String& sNewName);
  void triggerNotifications(){}
  virtual bool canBeDeleted() { return true; }
  bool hasSecurityDescriptor() { return false; }

protected:  //protected member functions
  HANDLE getHandle() const {return m_hFile;}
  HANDLE* getCompletionHandle() const {return m_pCompletionHandle;}
  vfs::String getPath() const {return m_sPath;}
private:
  HANDLE m_hFile;
  HANDLE* m_pCompletionHandle;
  const vfs::String m_sPath;
};
