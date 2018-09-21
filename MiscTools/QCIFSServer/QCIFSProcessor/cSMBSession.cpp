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

// cSMBSession.cpp
// class encapsulates the state of a client session
//
// created 14/5/05

#include "stdafx.h"

#if CIFS 

#include "mycifs.h"

#include ".\csmbsession.h"
#include "cSMBTree.h"
#include "cSMBResponse.h"
#include "cOpLock.h"
#include "cTrans2FindFirst2.h"
#include "iOpLockManager.h"

#include "..\iTreeResource.h"

extern tActivityMap ActivityQCIFSProcessors;

cSMBSession::cSMBSession(const String& sLoginName
  , const std::string &rANSIpw
  , const std::string rUNICODEpw
  , const cPtr<iOpLockManager> pOpLockManager)
: m_sLoginName(sLoginName)
, m_sANSIpw(rANSIpw)
, m_sUNICODEpw(rUNICODEpw)
, m_pOpLockManager(pOpLockManager)
, m_nTreeCount(2)
, m_nSearchHandles(1)
{
}

cSMBSession::~cSMBSession(void)
{
}

void cSMBSession::replaceTree(const String& sShareName
  , const cPtr<iTreeResourceFactory>& pTreeResourceFactory)
{
  cLockGuard lg(&m_TreeAccess);

  for (tTreeMap::iterator cit = m_treeMap.begin()
    ; m_treeMap.end() != cit
    ; ++cit)
  {
    const cPtr<cSMBTree>& pTree = cit->second;
    if (pTree->NameIs(sShareName))
    {
      const cPtr<cSMBTree> pOldTree = cit->second;
      const cPtr<iTreeResource> pNewTreeResource = pTreeResourceFactory->getTreeResource(m_sLoginName);
      const cPtr<cSMBTree> pNewSMBTree = new cSMBTree(this, pNewTreeResource, m_pOpLockManager, sShareName);
      cit->second = pNewSMBTree;
      if (pOldTree.isValid())
        pOldTree->notifyRemoved(pNewSMBTree);
    }
  }
}

// connects a tree and returns the ID of its connection
USHORT cSMBSession::ConnectTree(const String& sShareName
  , const cPtr<iTreeResourceFactory> pTreeResourceFactory)
{
  const cPtr<iTreeResource> pTreeResource = pTreeResourceFactory->getTreeResource(m_sLoginName);
  if (pTreeResource.isNull())
    return 0;

  // when count wraps back round, don't return 0
  InterlockedCompareExchange(&m_nTreeCount, 2, SHRT_MAX);

  const USHORT Tid = static_cast<USHORT>(InterlockedIncrement(&m_nTreeCount));

  //QTRACE((L"cSMBSession::ConnectTree() - %d", Tid));

  try
  {
    const cPtr<cSMBTree> pTree = new cSMBTree(this, pTreeResource, m_pOpLockManager, sShareName);
    cLockGuard lg(&m_TreeAccess);
    m_treeMap.insert(tTreeMap::value_type(Tid, pTree));
  }
  catch (const bool &)
  {
    QSOS((L"failed to connect tree"));
  }

  return Tid;
}

bool cSMBSession::DisconnectTree(const SHORT Tid)
{
  // find the tree
  cPtr<cSMBTree> pTree = getTree(Tid);
  if (!pTree.isValid())
  {
    QSOS((L"Disconnect Tree [%d] not found", Tid));
    return false;
  }

  //QTRACE((L"DisconnectTree [%d]", Tid));
  cLockGuard lg(&m_TreeAccess);
  // delete the tree
  m_treeMap.erase(Tid);

  return true;
}

// returns the tree associated with the tree ID
cPtr<cSMBTree> cSMBSession::getTree(const USHORT Tid)
{
  cPtr<cSMBTree> pTree;
  cLockGuard lg(&m_TreeAccess);
  if (!m_treeMap.empty())
  {
    tTreeMap::iterator it = m_treeMap.find(Tid);
    if (it != m_treeMap.end())
    {
      pTree = it->second;
    }
    else
    {
      QSOS((L"getTree() failure - tree [%d] not found", Tid));
    }
  }
  else
  {
    QSOS((L"getTree() failure - tree map is empty"));
  }

  return pTree;
}

cPtr<cSMBTree> cSMBSession::getTree(cPtr<cSMBResponse> response)
{
  const USHORT Tid = response->Header->Tid;
  cLockGuard lg(&m_TreeAccess);
  try
  {
    if (m_treeMap.empty())
    {
      QSOS((L"getTree() failure - tree map is empty"));
      throw SMB_ERR_BAD_TID;
    }
    
    tTreeMap::iterator it = m_treeMap.find(Tid);
    if (it == m_treeMap.end())
    {
      QSOS((L"getTree() failure - tree [%d] not found", Tid));
      throw SMB_ERR_BAD_TID;
    }

    return it->second;
  }
  catch (const int &errCode)
  {
    response->Header->Flags2 &= (0xffff-SMB_FLAGS2_NT_STATUS);
    response->Header->ErrorClass = SMB_ERR_CLASS_SERVER;
    response->Header->Reserved = 0;
    response->Header->Error = static_cast<USHORT>(errCode);
    response->Header->Tid = 0;
    response->clearBlock();
  }

  return cPtr<cSMBTree>();
}

// trans2 block handlers
cPtr<cSMBResponse> cSMBSession::processTrans2SubCommand(const USHORT maxDataCount, const PUCHAR trans2Block
  , const UINT setupCount, const PUCHAR trans2Params, const PUCHAR trans2Data, cPtr<cSMBResponse> response)
{
  cPtr<cSMBTree> pTree = getTree(response);
  if (pTree.isValid())
  {
    response = pTree->processTrans2SubCommand(maxDataCount, trans2Block, setupCount, trans2Params, trans2Data, response);
  }
  return response;
}

USHORT cSMBSession::getSearchHandleID(cTrans2FindFirst2 *pTrans2FindFirst2, const bool bEndOfSearch)
{
  cLockGuard lg(&m_SearchHandleAccess);
  if (m_nSearchHandles == 0xffff)
    m_nSearchHandles = 1;

  //// get a new search ID that isn't stored
  USHORT newVal = m_nSearchHandles++;
  while (m_searchHandleMap.find(newVal) != m_searchHandleMap.end())
    newVal++;
  m_nSearchHandles = newVal+1;

  if (!bEndOfSearch)
  {
    m_searchHandleMap.insert(tSearchHandleMap::value_type(newVal, pTrans2FindFirst2));
  }

  return newVal;
}

cTrans2Find *cSMBSession::getSearch(USHORT searchHandle)
{
  //QTRACE((L"cSMBSession::getSearch () - %d", searchHandle));
  cLockGuard lg(&m_SearchHandleAccess);

  cTrans2Find *pFind(NULL);
  tSearchHandleMap::iterator find = m_searchHandleMap.find(searchHandle);
  if (find != m_searchHandleMap.end())
    pFind = find->second;

  return pFind;
}

void cSMBSession::closeSearch(USHORT searchHandle)
{
  //QTRACE((L"cSMBSession::closeSearch () - %d", searchHandle));
  cLockGuard lg(&m_SearchHandleAccess);

  tSearchHandleMap::iterator find = m_searchHandleMap.find(searchHandle);
  if (find != m_searchHandleMap.end())
    delete(find->second);

  m_searchHandleMap.erase(searchHandle);
}

cPtr<cSMBResponse> cSMBSession::processNTTransSubCommand(const USHORT functionCode, const PUCHAR pSetupData
                                                         , const UINT setupCount, const PUCHAR transParams, const PUCHAR transData, cPtr<cSMBResponse> response)
{
  cPtr<cSMBTree> pTree = getTree(response);
  if (pTree.isValid())
  {
    response = pTree->processNTTransSubCommand(functionCode
      , pSetupData
      , setupCount
      , transParams
      , transData
      , response);
  }
  return response;
}

cPtr<cSMBResponse> cSMBSession::processTransSubCommand(const PUCHAR pHeader
  , const PUCHAR pSetupData
  , const UINT setupCount
  , const PUCHAR transParams
  , const PUCHAR transData
  , const PUSHORT pRespBuffer
  , cPtr<cSMBResponse> response)
{
  cPtr<cSMBTree> pTree = getTree(response);
  if (pTree.isValid())
  {
    response = pTree->processTransSubCommand(pHeader
      , pSetupData
      , setupCount
      , transParams
      , transData
      , pRespBuffer
      , response);
  }
  return response;
}

cPtr<cSMBResponse> cSMBSession::processOpenAndX(PREQ_OPEN_ANDX pReq
  , PUCHAR pHeader
  , const cPtr<cSMBResponse> response)
{
  const cPtr<cSMBTree> pTree(getTree(response));

  if (pTree.isValid())
    pTree->processOpenAndX(pReq, pHeader, response);

  return response;
}

cPtr<cSMBResponse> cSMBSession::processNTCreateAndX(PREQ_NT_CREATE_ANDX pReq
  , PUCHAR pHeader
  , cPtr<cSMBResponse> response)
{
  cPtr<cSMBTree> pTree = getTree(response);
  if (pTree.isValid())
  {
    response = pTree->processNTCreateAndX(pReq, pHeader, response);
  }

  return response;
}

cPtr<cSMBResponse> cSMBSession::Close(USHORT Fid, cPtr<cSMBResponse> response)
{
  cPtr<cSMBTree> pTree = getTree(response);
  if (pTree.isValid())
  {
    response = pTree->Close(Fid, response);
  }
  return response;
}

cPtr<cSMBResponse> cSMBSession::Rename(const PREQ_RENAME pReq, cPtr<cSMBResponse> response)
{
  cPtr<cSMBTree> pTree = getTree(response);
  if (pTree.isValid())
  {
    response = pTree->Rename(pReq, response);
  }
  return response;
}

cPtr<cSMBResponse> cSMBSession::Lock(const PREQ_LOCKING_ANDX pReq, cPtr<cSMBResponse> response)
{
  cPtr<cSMBTree> pTree = getTree(response);
  if (pTree.isValid())
  {
    response = pTree->Lock(pReq, response);
  }
  return response;
}

cPtr<cSMBResponse> cSMBSession::Delete(const PREQ_DELETE pReq, cPtr<cSMBResponse> response)
{
  cPtr<cSMBTree> pTree = getTree(response);
  if (pTree.isValid())
  {
    response = pTree->Delete(pReq, response);
  }
  return response;
}

cPtr<cSMBResponse> cSMBSession::processReadAndX(const PREQ_READ_ANDX pReq, cPtr<cSMBResponse> response)
{
  cPtr<cSMBTree> pTree = getTree(response);
  if (pTree.isValid())
  {
    response = pTree->Read(pReq, response);
  }
  return response;
}

cPtr<cSMBResponse> cSMBSession::processWrite(const USHORT Fid, cConstPtr<cMemoryView> buffer
  , const LARGE_INTEGER &offset, DWORD &nBytes, cPtr<cSMBResponse> response)
{
  cPtr<cSMBTree> pTree = getTree(response);
  if (pTree.isValid())
  {
    response = pTree->Write(Fid, buffer, offset, nBytes, response);
  }
  return response;
}

cPtr<cSMBResponse> cSMBSession::DeleteDirectory(const std::wstring ksPath, cPtr<cSMBResponse> response)
{
  cPtr<cSMBTree> pTree = getTree(response);
  if (pTree.isValid())
  {
    response = pTree->DeleteDirectory(ksPath, response);
  }
  return response;
}

cPtr<cSMBResponse> cSMBSession::CreateDirectory(const std::wstring ksPath, cPtr<cSMBResponse> response)
{
  cPtr<cSMBTree> pTree = getTree(response);
  if (pTree.isValid())
  {
    response = pTree->CreateDirectory(ksPath, response);
  }
  return response;
}

cPtr<cSMBResponse> cSMBSession::ProcessExit(cPtr<cSMBResponse> response)
{
  cPtr<cSMBTree> pTree = getTree(response);
  if (pTree.isValid())
  {
    response = pTree->ProcessExit(response);
  }
  return response;
}

cPtr<cSMBResponse> cSMBSession::SetInformation(PREQ_SET_INFORMATION pReq, cPtr<cSMBResponse> response)
{
  cPtr<cSMBTree> pTree = getTree(response);
  if (pTree.isValid())
  {
    response = pTree->SetInformation(pReq, response);
  }
  return response;
}

cPtr<cSMBResponse> cSMBSession::SetInformation2(PREQ_SET_INFORMATION2 pReq, cPtr<cSMBResponse> response)
{
  cPtr<cSMBTree> pTree = getTree(response);
  if (pTree.isValid())
  {
    response = pTree->SetInformation2(pReq, response);
  }
  return response;
}

cPtr<cSMBResponse> cSMBSession::QueryInformation2(const PREQ_QUERY_INFORMATION2 pReq, cPtr<cSMBResponse> response)
{
  cPtr<cSMBTree> pTree = getTree(response);
  if (pTree.isValid())
  {
    response = pTree->QueryInformation2(pReq, response);
  }
  return response;
}

cPtr<cSMBResponse> cSMBSession::NTCancel(cPtr<cSMBResponse> response)
{
  cPtr<cSMBTree> pTree = getTree(response);
  if (pTree.isValid())
  {
    response = pTree->NTCancel(response);
  }
  return response;
}

cPtr<cSMBResponse> cSMBSession::CheckDirectory(const PREQ_CHECK_DIRECTORY pReq, cPtr<cSMBResponse> pResponse)
{
  cPtr<cSMBTree> pTree = getTree(pResponse);
  if (pTree.isValid())
  {
    pResponse = pTree->CheckDirectory(pReq, pResponse);
  }
  return pResponse;
}

void cSMBSession::AddOplock(cPtr<cOpLock> pOpLock)
{
  cPtr<cSMBTree> pTree = getTree(pOpLock->getTid());
  if (pTree.isValid())
  {
    pTree->AddOplock(pOpLock);
  }
}

#endif // CIFS 