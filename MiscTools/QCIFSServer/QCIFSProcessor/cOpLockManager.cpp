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

//cOpLockManager.cpp

#include "StdAfx.h"

#if CIFS 

#include ".\coplockmanager.h"
#include "cOpLock.h"

#include "..\common\iCompletionMsg.h"

cOpLockManager::cOpLockManager(const bool bActive)
: m_bActive(bActive)
{
}

cOpLockManager::~cOpLockManager(void)
{
}

void cOpLockManager::Release(const cPtr<cOpLock> pOpLock)
{
  if (!m_bActive)
    return;

  cLockGuard lg(&m_access);
  for (std::list<cPtr<cOpLock> >::iterator it = m_OpLockList.begin()
    ; it != m_OpLockList.end()
    ; ++it)
  {
    const cPtr<cOpLock> pFindOpLock = *it;
    if (pFindOpLock == pOpLock)
    {
      ReleaseOpLock(it);
      break;
    }
  }
}

void cOpLockManager::Release(const PSMB_HEADER pSMBHeader, const USHORT Fid)
{
  if (!m_bActive)
    return;

  cLockGuard lg(&m_access);
  for (std::list<cPtr<cOpLock> >::iterator it = m_OpLockList.begin()
    ; it != m_OpLockList.end()
    ; ++it)
  {
    const cPtr<cOpLock> pOpLock = *it;
    const PSMB_HEADER pSearchHeader = pOpLock->Header();
    if (pSearchHeader->Uid == pSMBHeader->Uid
      && pSearchHeader->Sid == pSMBHeader->Sid
      && pSearchHeader->Tid == pSMBHeader->Tid
      && pOpLock->Fid() == Fid)
    {
      ReleaseOpLock(it);
      break;
    }
  }
}

void cOpLockManager::Check(const String& sPath, const cSMBRequest& rRequest)
{
  if (!m_bActive)
    return;

  //const PSMB_HEADER pReqHd(rRequest.Header);

  QTRACE((L"cOpLockManager::CheckOpLock () - {%s}", sPath.c_str()));

  cLockGuard lg(&m_access);
  for (std::list<cPtr<cOpLock> >::iterator it = m_OpLockList.begin()
    ; it != m_OpLockList.end()
    ; ++it)
  {
    cPtr<cOpLock> pOpLock = *it;
    if (pOpLock->compare(sPath) == 0)
    {
      cPtr<cSMBResponse> pResponse = new cSMBResponse(rRequest
        , new cMemoryView(new cMemory((size_t)gkBufferSize, cMemory::eVirtual), 0, (size_t)gkBufferSize));
      memcpy(pResponse->Header, pOpLock->Header(), sizeof(SMB_HEADER));

      pResponse->Header->Command = SMB_COM_LOCKING_ANDX;
      pResponse->Header->Mid = 0;
      pResponse->Header->Pid = 0;
      pResponse->Header->Flags = 0;
      pResponse->Header->Flags2 = 0;
      memset(pResponse->Header->SecuritySignature, 0, 8);
      pResponse->Header->Reserved = 0;

      PREQ_LOCKING_ANDX pReq = (PREQ_LOCKING_ANDX)pResponse->allocParams(sizeof(REQ_LOCKING_ANDX));
      pReq->AndXCommand = 0xff;
      pReq->AndXReserved = 0;
      pReq->AndXOffset = 0;
      pReq->Fid = pOpLock->Fid();
      pReq->LockType = LOCKING_ANDX_OPLOCK_RELEASE;
      pReq->OplockLevel = OPLOCK_BROKEN_TO_NONE;  // OPLOCK_BROKEN_TO_II
      pReq->Timeout = 0;
      pReq->NumberOfUnlocks = 0;
      pReq->NumberOfLocks = 0;
      pReq->Buffer[0] = 0;

      IO_DATA *pIOData = new IO_DATA;
      pIOData->overlapped.Internal = 0;
      pIOData->overlapped.InternalHigh = 0;
      pIOData->overlapped.Offset = 0;
      pIOData->overlapped.OffsetHigh = 0;
      pIOData->overlapped.hEvent = 0;
      //pIOData->pMsg = pResponse;

      // TODO: Sort OpLocks out proper with new architecture
      //if (!pResponse->sendData(reinterpret_cast<LPOVERLAPPED>(pIOData)))
      //  delete(pIOData);

      // block until the previous oplock client writes back to the server
      QTRACE((L"Waiting for oplock for %s ...", sPath.c_str()));
      pOpLock->P();
      QTRACE((L"Got oplock for %s", sPath.c_str()));
    }
  }
}


cPtr<cOpLock> cOpLockManager::Set(const String& sPath
  , const PSMB_HEADER pHeader
  , const USHORT Fid
  , const SOCKET socket)
{
  if (!m_bActive)
    return cPtr<cOpLock>();

  const cPtr<cOpLock> pOpLock(new cOpLock(sPath, pHeader, Fid, socket));

  cLockGuard lg(&m_access);
  m_OpLockList.push_back(pOpLock);

  return pOpLock;
}


inline void cOpLockManager::ReleaseOpLock(const tOpLockList::iterator& it)
{
  // DON'T lock 'm_access' here
  const cPtr<cOpLock> pOpLock(*it);
  pOpLock->V();
  m_OpLockList.erase(it);
}

#endif // CIFS 