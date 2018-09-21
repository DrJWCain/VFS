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

//cConstMemComposite.h

#pragma once

#include "cDeletableComposite.h"

class cConstMemComposite : public cDeletableComposite, public vfs::cRefCount
{
public:
  cConstMemComposite(const fileInfo& fi
    , const vfs::cPtr<cSearchListener> pSearchListener
    , const vfs::cConstPtr<vfs::cMemoryView> pMem
    , vfs::cPtr<iFileEvent> pFileEvent
    , const bool bDeletable
    , const vfs::cPtr<iRenameHandler> pRenameHandler);
  ~cConstMemComposite(void);

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
  DWORD Rename(const vfs::String& sNewName);
  bool hasSecurityDescriptor() { return false; }
  DWORD registerChangeListener(vfs::cPtr<iFileChangeListener> listener) {return ERROR_NOT_SUPPORTED;}

  //public member functions
  vfs::cConstPtr<vfs::cMemoryView> getMem() {return m_pMem;}
  vfs::cPtr<iFileLoader> getMemLoader() {return vfs::cPtr<iFileLoader>();}
  bool canDelete() const {return cDeletableComposite::canDelete();}

private:
  const vfs::cConstPtr<vfs::cMemoryView> m_pMem;
  const vfs::cPtr<iRenameHandler> m_pRenameHandler;
};

