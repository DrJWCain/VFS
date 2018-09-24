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

//cProcessPacket.cpp

#include "StdAfx.h"

#include "cProcessPacketMsg.h"
#include "iShareManager.h"
#include "iOpLockManager.h"
#include "cSMBResponse.h"
#include "cSMB2Response.h"

#include "..\SocketServer\iPacket.h"

using namespace vfs;

cProcessPacketMsg::cProcessPacketMsg(const cPtr<iRXPacket> pRXPacket
  , const cPtr<iShareManager> pShareManager
  , const cPtr<iOpLockManager> pOpLockManager)
: m_pRXPacket(pRXPacket)
, m_pShareManager(pShareManager)
, m_pOpLockManager(pOpLockManager)
{
  //QTRACE((L"cProcessPacketMsg::cProcessPacketMsg()"));
}

cProcessPacketMsg::~cProcessPacketMsg(void)
{
  //QTRACE((L"cProcessPacketMsg::~cProcessPacketMsg()"));
}

extern tActivityMap ActivityQCIFSProcessors;

void cProcessPacketMsg::deliver(ULONG_PTR pCompletionKey, const DWORD dwBytes)
{
  tResponseList responses;
  
  cSMBRequest request(m_pRXPacket, m_pShareManager, m_pOpLockManager);
  request.getResponses(responses);

  vfs::iActivity::Ptr activity = ActivityQCIFSProcessors.find(GetCurrentThreadId())->second;
  cActivityScope as1(activity,kSendResponses);
  for (tResponseList::const_iterator cit = responses.begin()
    ; cit != responses.end()
    ; ++cit)
  {
    const cPtr<cSMBResponse> &pResponse = *cit;
    if (pResponse.isValid())
    {
      cPtr<iTXPacket> pTXPacket = m_pRXPacket->getResponsePacket();
      pResponse->WritePacket(pTXPacket);
      pTXPacket->Send();
    }
  }
}

void cProcessPacketMsg::err(ULONG_PTR pCompletionKey, const DWORD err)
{
  //QTRACE((L"cProcessPacketMsg::err()"));
  //this shouldn't really happen since the message should be delivered from a 'PostCompletionStatus()'
}




cProcessPacketMsg2::cProcessPacketMsg2(const cPtr<iRXPacket> pRXPacket
  , const cPtr<iShareManager> pShareManager
  , const cPtr<iOpLockManager> pOpLockManager)
: m_pRXPacket(pRXPacket)
, m_pShareManager(pShareManager)
, m_pOpLockManager(pOpLockManager)
{
  //QTRACE((L"cProcessPacketMsg2::cProcessPacketMsg2()"));
}

cProcessPacketMsg2::~cProcessPacketMsg2(void)
{
  //QTRACE((L"cProcessPacketMsg2::~cProcessPacketMsg2()"));
}

extern tActivityMap ActivityQCIFSProcessors;

void cProcessPacketMsg2::deliver(ULONG_PTR pCompletionKey, const DWORD dwBytes)
{
  tResponseList2 responses;
  
  cSMB2Request request(m_pRXPacket, m_pShareManager, m_pOpLockManager);
  request.getResponses(responses);

  vfs::iActivity::Ptr activity = ActivityQCIFSProcessors.find(GetCurrentThreadId())->second;
  cActivityScope as1(activity,kSendResponses);
  for (tResponseList2::const_iterator cit = responses.begin()
    ; cit != responses.end()
    ; ++cit)
  {
    const cPtr<cSMB2Response> &pResponse = *cit;
    if (pResponse.isValid())
    {
      cPtr<iTXPacket> pTXPacket = m_pRXPacket->getResponsePacket();
      pResponse->WritePacket(pTXPacket);
      pTXPacket->Send();
    }
  }
}

void cProcessPacketMsg2::err(ULONG_PTR pCompletionKey, const DWORD err)
{
  //QTRACE((L"cProcessPacketMsg2::err()"));
  //this shouldn't really happen since the message should be delivered from a 'PostCompletionStatus()'
}