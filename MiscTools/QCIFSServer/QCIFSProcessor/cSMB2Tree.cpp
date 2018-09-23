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

//cSMB2Tree.cpp

#include "StdAfx.h"
#include ".\csmb2tree.h"
#include "cSMB2Response.h"
#include "cCalcDiskGeometry.h"
#include "cSMB2SearchResults.h"
#include "cSMB2NotificationReceiver.h"
#include "iUniqueNotifyID.h"
#include "cSMB2OplockBreakListener.h"

#include "..\iTreeResource.h"

using namespace vfs;

typedef std::set<ULONGLONG> tFileIdMap;
static tFileIdMap gGlobalFileIdMap;
static cLock gGlobalFileIdMapLock;

cSMB2Tree::cSMB2Tree(const cPtr<iTreeResource>& pTreeResource
  , const String& sLoginName
  , const String& sShareName)
: m_pTreeResource(pTreeResource)
, m_sLoginName(sLoginName)
, m_sShareName(sShareName)
{
}

cSMB2Tree::~cSMB2Tree(void)
{
}

cPtr<iComposite> cSMB2Tree::getComposite(const ULONGLONG Fid, const cPtr<cSMB2Response> response)
{
  return m_compositeMap.getComposite(Fid, response);
}

void cSMB2Tree::closeComposite(const ULONGLONG Fid, const ULONGLONG durableFid)
{
  cPtr<iComposite> composite = getComposite(Fid, cPtr<cSMB2Response>());
  const bool deleteOnClose = m_compositeMap.testAndRemoveDeleteOnClose(Fid);
  if(composite.isValid())
  {
    if(deleteOnClose)
    {
      QTRACE((L"%S found Delete on CLose", __FUNCTION__));
      const DWORD dwRet = composite->Delete();
    }
    composite->Close(Fid);
  }
  
  {
    cLockGuard lg2(&m_opLocksMapAccess);
    tOpLocksMap::iterator oplock = m_OpLocksMap.find(Fid);
    if(oplock != m_OpLocksMap.end())
      oplock->second->Release();
  }

  m_compositeMap.remove(Fid);
  {
    cLockGuard guard(&gGlobalFileIdMapLock);
    tFileIdMap::iterator gfid = gGlobalFileIdMap.find(durableFid);
    if (gfid != gGlobalFileIdMap.end())
      gGlobalFileIdMap.erase(gfid);
  }
  cLockGuard lg(&m_searchMapAccess);
  m_searchResultsMap.erase(Fid);
  cLockGuard lg2(&m_opLocksMapAccess);
  m_OpLocksMap.erase(Fid);
}

ULONGLONG cSMB2Tree::Create(const String& sFilename
  , const PREQ_SMB2_CREATE pReq
  , const LPBY_HANDLE_FILE_INFORMATION pFI
  , const vfs::cPtr<cSMB2Response> pOplockResponse
  , ULONGLONG& durableFileID
  , DWORD& err)
{
  //QTRACE((L"cSMB2Tree::Create() - %s", sFilename.c_str()));
#define FILE_SUPERSEDE 0x00000000 //If the file already exists, supersede it. Otherwise, create the file. This value SHOULD NOT be used for a printer object. <16> 
#define FILE_OPEN 0x00000001 //If the file already exists, return success; otherwise, fail the operation. MUST NOT be used for a printer object. 
#define FILE_CREATE 0x00000002 //If the file already exists, fail the operation; otherwise, create the file. 
#define FILE_OPEN_IF 0x00000003 //Open the file if it already exists; otherwise, create the file. This value SHOULD NOT be used for a printer object. <17> 
#define FILE_OVERWRITE 0x00000004 //Overwrite the file if it already exists; otherwise, fail the operation. MUST NOT be used for a printer object. 
#define FILE_OVERWRITE_IF 0x00000005 //Overwrite the file if it already exists; otherwise, create the file. This value SHOULD NOT be used for a printer object. <18>

  static DWORD DispositionTrans[6] = {CREATE_ALWAYS//FILE_SUPERSEDE
    , OPEN_EXISTING//FILE_OPEN
    , CREATE_NEW//FILE_CREATE
    , OPEN_ALWAYS//FILE_OPEN_IF
    , OPEN_EXISTING//FILE_OVERWRITE
    , CREATE_ALWAYS//FILE_OVERWRITE_IF
  };

  ULONGLONG fid = m_compositeMap.allocateFid();

  const cPtr<iComposite> pComposite =
    //(FILE_ATTRIBUTE_DIRECTORY == (pReq->FileAttributes & FILE_ATTRIBUTE_DIRECTORY)) ||
    (FILE_DIRECTORY_FILE == (pReq->CreateOptions & FILE_DIRECTORY_FILE))
        && (CREATE_NEW == DispositionTrans[pReq->CreateDisposition])
    ? m_pTreeResource->NTCreateFolder(sFilename, NULL, pFI, err)
    : m_pTreeResource->NTCreateFile(sFilename
      , pReq->DesiredAccess
      , pReq->ShareAccess
      , NULL
      , DispositionTrans[pReq->CreateDisposition]
      , pReq->FileAttributes
      , NULL
      , pFI
      , err
      , vfs::cPtr<iFileProcessingCallback>()
      , fid);

  if ((FILE_ATTRIBUTE_DIRECTORY != (pFI->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    && (pReq->CreateOptions & FILE_DIRECTORY_FILE))
  {
    err = static_cast<DWORD>(STATUS_FILE_IS_A_DIRECTORY);
    return 0;
  }

  if (pComposite.isValid())
  {
    const ULONG kFileDeleteOnClose = 0x00001000;
    bool deleteOnClose = ((pReq->CreateOptions & kFileDeleteOnClose) == kFileDeleteOnClose);
    if(deleteOnClose)
    {
      //QSOS((L"deleteOnClose!"));
      m_compositeMap.addDeleteOnClose(fid);
    }
      
    m_compositeMap.addFid(fid, pComposite, sFilename);
    durableFileID = fid;
    {
      cLockGuard guard(&gGlobalFileIdMapLock);
      while (gGlobalFileIdMap.find(durableFileID) != gGlobalFileIdMap.end())
        ++durableFileID;
      gGlobalFileIdMap.insert(durableFileID);
    }
    
    cLockGuard lg(&m_opLocksMapAccess);
    cPtr<cSMB2OpLock> oplock = new cSMB2OpLock(fid, durableFileID, pOplockResponse);
    m_OpLocksMap[fid] = oplock;    
    pComposite->registerChangeListener(new cSMB2OplockBreakListener(oplock));
    //QTRACE((L"fid = %I64d", fid));
    return fid;
  }

  return 0;
}

DWORD cSMB2Tree::QueryInfoAllocation(DISK_GEOMETRY_EX &disk_geometry
  , PULARGE_INTEGER pulFreeSpace
  , LPDWORD pSectorsPerCluster )
{
  return m_pTreeResource->QueryInfoAllocation(disk_geometry, pulFreeSpace, pSectorsPerCluster);
}

extern tActivityMap ActivityQCIFSProcessors;

DWORD cSMB2Tree::Find(const ULONGLONG Fid
  , const PREQ_SMB2_QUERY_DIRECTORY pReq
  , const String& sSearchPath
  , const PUCHAR pRespHeader
  , const PUCHAR pRespStart
  , ULONG& respLength
  , const cPtr<cSMB2Response>& pResponse)
{
  vfs::iActivity::Ptr activity = getActivity();
  activity->activityStart(kCreateBuffer);
  cPtr<cSMB2SearchResults> pSearchResults;
  {
    cLockGuard lg(&m_searchMapAccess);
    const tSearchResultsMap::const_iterator finder = m_searchResultsMap.find(Fid);
    if (m_searchResultsMap.end() == finder || (SMB2_REOPEN == (pReq->Flags & SMB2_REOPEN)))
    {
      const cPtr<iComposite> pComposite = m_compositeMap.getComposite(Fid, pResponse);
      if (pComposite.isNull())
        return static_cast<DWORD>(STATUS_FILE_INVALID);

      activity->activityStart(kCreateResponse, pReq->Flags);
      String path = m_compositeMap.getPath(Fid);
      pSearchResults = new cSMB2SearchResults(pComposite, sSearchPath, SMB2_RETURN_SINGLE_ENTRY == (pReq->Flags & SMB2_RETURN_SINGLE_ENTRY), path, m_pTreeResource);
      activity->activityStart(kGetBlocks);
      m_searchResultsMap[Fid] = pSearchResults;
    }
    else
      pSearchResults = finder->second;
  }

  if (pSearchResults.isNull())
    return static_cast<DWORD>(STATUS_FILE_INVALID);

  return pSearchResults->getSearchResults(pReq, pRespHeader, pRespStart, respLength);
}

void cSMB2Tree::CancelNotifications(const ULONGLONG& messageId, bool synchronous)
{
  m_compositeMap.cancelNotifications(messageId, synchronous);
}

DWORD cSMB2Tree::Rename(const ULONGLONG& fid, const std::wstring& newName)
{
  String originalName = m_compositeMap.getPath(fid);
  return m_pTreeResource->Rename(0, originalName, newName);
}

void cSMB2Tree::notifyRemoved(const cPtr<cSMB2Tree>& pNewTree)
{
  tFidPathPairList fidPathPair;
  m_compositeMap.triggerAllNotificationsAndClear(fidPathPair);
  SECURITY_ATTRIBUTES _sa;
  BY_HANDLE_FILE_INFORMATION bhfi;
  DWORD ret;
  for (tFidPathPairList::const_iterator cit = fidPathPair.begin()
    ; fidPathPair.end() != cit
    ; ++cit)
  {
    const String& sPath = cit->second;
    const cPtr<iComposite> pNewComposite = pNewTree->m_pTreeResource->NTCreateFile(sPath
      , 0
      , 0
      , &_sa
      , OPEN_EXISTING
      , FILE_ATTRIBUTE_NORMAL
      , NULL
      , &bhfi
      , ret);
    if (pNewComposite.isValid() && ERROR_SUCCESS == ret)
      pNewTree->m_compositeMap.addFid(cit->first, pNewComposite, sPath);
  }
}