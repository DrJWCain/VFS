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

//cProcessPacket.h

#pragma once

#include "..\common\icompletionmsg.h"

class iRXPacket;
class iShareManager;
class iOpLockManager;
class cQCIFSPacketProcessor;

template <typename RequestType>
class cProcessPacketMsg : public iCompletionMsg, public vfs::cRefCount
{
public:
  cProcessPacketMsg(vfs::cPtr<vfs::cMemoryView> pMem
    , const vfs::cPtr<iRXPacket> pRXPacket
    , const vfs::cPtr<iShareManager> pShareManager
    , const vfs::cPtr<iOpLockManager> pOpLockManager
    , cQCIFSPacketProcessor* pQCIFSPacketProcessor)
  : m_pMem(pMem)
  , m_pRXPacket(pRXPacket)
  , m_pShareManager(pShareManager)
  , m_pOpLockManager(pOpLockManager)
  , m_pQCIFSPacketProcessor(pQCIFSPacketProcessor)
  {}

  void deliver(ULONG_PTR pCompletionKey, const DWORD dwBytes)
  {
    vfs::iActivity::Ptr activity = getActivity();
    tResponseList responses;
    {
      cActivityScope as1(activity,kGetResponses);
      RequestType request(m_pRXPacket, m_pShareManager, m_pOpLockManager, m_pQCIFSPacketProcessor);
      request.getResponses(m_pMem, responses, activity);
    }

    cActivityScope as1(activity,kSendResponses);
    for (tResponseList::const_iterator cit = responses.begin()
      ; cit != responses.end()
      ; ++cit)
    {
      const cPtr<iResponseWriter>& pResponse = *cit;
      if (pResponse.isValid())
      {
        cPtr<iTXPacket> pTXPacket = m_pRXPacket->getResponsePacket();
        activity->activityStart(kSendResponses+1);
        pResponse->WritePacket(pTXPacket);
        activity->activityStart(kSendResponses+2);
        pTXPacket->Send();
      }
    }
  }

  //this shouldn't really happen since the message should be delivered from a 'PostCompletionStatus()'
  void err(ULONG_PTR pCompletionKey, const DWORD err) {}

private:
  const vfs::cPtr<vfs::cMemoryView> m_pMem;
  const vfs::cPtr<iRXPacket> m_pRXPacket;
  const vfs::cPtr<iShareManager> m_pShareManager;
  const vfs::cPtr<iOpLockManager> m_pOpLockManager;
  cQCIFSPacketProcessor* m_pQCIFSPacketProcessor;
};