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

//cSMB2Tree.h

#pragma once

#include "cCompositeMap.h"
#include "..\iComposite.h"

class iTreeResource;
class iNotificationReceiver;
class cSMB2Response;
class cSMB2SearchResults;
class cSMB2OpLock;

class cSMB2Tree : public vfs::cRefCount
{
public:
  cSMB2Tree(const vfs::cPtr<iTreeResource>& pTreeResource
    , const vfs::String& sLoginName
    , const vfs::String& sShareName);
  ~cSMB2Tree(void);

  //public member functions
  vfs::cPtr<iComposite> getComposite(const ULONGLONG Fid, const vfs::cPtr<cSMB2Response> response);
  void closeComposite(const ULONGLONG Fid, const ULONGLONG durableFid);

  ULONGLONG Create(const vfs::String& sFilename
    , const PREQ_SMB2_CREATE pReq
    , const LPBY_HANDLE_FILE_INFORMATION pFI
    , const vfs::cPtr<cSMB2Response> pOplockResponse
    , ULONGLONG& durableFileID
    , DWORD& err);
  DWORD QueryInfoAllocation(DISK_GEOMETRY_EX &disk_geometry
    , PULARGE_INTEGER pulFreeSpace
    , LPDWORD pSectorsPerCluster);
  DWORD Find(const ULONGLONG Fid
    , const PREQ_SMB2_QUERY_DIRECTORY pReq
    , const vfs::String& sSearchPath
    , const PUCHAR pHeader
    , const PUCHAR pRespStart
    , ULONG& respLength
    , const vfs::cPtr<cSMB2Response>& pResponse);
  void CancelNotifications(const ULONGLONG& messageId, bool synchronous);
  bool NameIs(const vfs::String& sName) const {return m_sShareName == sName;}
  void notifyRemoved(const vfs::cPtr<cSMB2Tree>& pNewTree);
  vfs::String name() {return m_sShareName;}

  DWORD Rename(const ULONGLONG& fid, const std::wstring& newName);

private:  //private member variables
  const vfs::cPtr<iTreeResource> m_pTreeResource;
  const vfs::String m_sLoginName;
  const vfs::String m_sShareName;

  typedef cCompositeMap<ULONGLONG, ULONGLONG> tCompositeMap;
  typedef std::vector<tCompositeMap::tFidPathPair> tFidPathPairList;
  mutable tCompositeMap m_compositeMap;

  typedef std::map<ULONGLONG, vfs::cPtr<cSMB2SearchResults> > tSearchResultsMap;
  vfs::cLock m_searchMapAccess;
  tSearchResultsMap m_searchResultsMap;
  
  typedef std::map<ULONGLONG, vfs::cPtr<cSMB2OpLock> > tOpLocksMap;
  vfs::cLock m_opLocksMapAccess;
  tOpLocksMap m_OpLocksMap;
};
