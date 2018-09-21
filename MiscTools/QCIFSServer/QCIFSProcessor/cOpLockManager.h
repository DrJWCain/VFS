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

//cOpLockManager.h

#pragma once

#include "ioplockmanager.h"

#if CIFS 

class cOpLock;

class cOpLockManager : public iOpLockManager, public vfs::cRefCount
{
  typedef std::list<vfs::cPtr<cOpLock> > tOpLockList;
public:
  cOpLockManager(const bool bActive);
  ~cOpLockManager(void);

  void Release(vfs::cPtr<cOpLock> pOpLock);
#if CIFS 
  void Release(const PSMB_HEADER pSMBHeader, const USHORT Fid);
#endif // CIFS 
  void Check(const String& sPath, const cSMBRequest& rRequest);
#if CIFS 
  vfs::cPtr<cOpLock> Set(const vfs::String& sPath
    , const PSMB_HEADER pHeader
    , const USHORT Fid
    , const SOCKET socket);
#endif // CIFS 

private:
  inline void ReleaseOpLock(const tOpLockList::iterator& it);

private:
  const bool m_bActive;
  vfs::cLock m_access;
  tOpLockList m_OpLockList;
};

#endif // CIFS 