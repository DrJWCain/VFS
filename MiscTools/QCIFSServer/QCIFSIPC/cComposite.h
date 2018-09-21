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

//cComposite.h

#pragma once

#include "..\icomposite.h"

class cComposite : public iComposite, public vfs::cRefCount
{
public:
  cComposite(void);
  ~cComposite(void);

  std::wstring getPath()  {return L"";}
  DWORD writeInfo(LPBY_HANDLE_FILE_INFORMATION pFileInfo) {return 0;}
  DWORD writeInfo(PfileInfo pFileInfo) {return 0;}
  DWORD Read(tTransmitList &krTPM, DWORD& nBytes, const LARGE_INTEGER &nOffset, const int sessionID) {return 0;}
  DWORD Write(vfs::cConstPtr<vfs::cMemoryView> buffer, DWORD &nBytes, const LARGE_INTEGER &offset, const int sessionID) {return 0;}
  DWORD SetFileSize(const PLARGE_INTEGER pFileSize) {return 0;}
  DWORD SetFileInfo(const FILETIME& creationTime
    , const FILETIME& modifiedTime, const FILETIME& accessTime, const DWORD attributes) {return 0;}
  DWORD Delete() {return 0;}
  DWORD Close() {return 0;}
  DWORD Lock(const LARGE_INTEGER &lgOffset, const LARGE_INTEGER &lgBytes, const bool bLock) {return 0;}
  DWORD registerNotification(const ULARGE_INTEGER& requestID
    , vfs::cPtr<iNotificationReceiver> pNotification) {return 0;}
};
