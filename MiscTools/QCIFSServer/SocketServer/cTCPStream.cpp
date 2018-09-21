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

//cTCPStream.cpp

#include "StdAfx.h"

#include ".\ctcpstream.h"
#include "cTCPStream.h"
#include "cIOBuffer.h"

using namespace vfs;

cTCPStream::cTCPStream()
: m_LastSuccessfulPacketNumber(0)
{
}


cTCPStream::~cTCPStream()
{
}

iActivity::Ptr getActivity();
void cTCPStream::NotifyRecvCompleted(cPtr<cIOBuffer> pIOBuffer, SOCKET_DATA *pSocketData)
{
  vfs::iActivity::Ptr activity = getActivity();
  {
    activity->activityStart(kRecvInterlocked);
    cLockGuard g(&m_IncomingAccess);
    m_IncomingBuffers.insert(tBufferStream::value_type(pIOBuffer->sequenceNumber(), pIOBuffer));
  }

  NewBuffersAvailable(pSocketData, activity);
}

void cTCPStream::NewBuffersAvailable(SOCKET_DATA *pSocketData, vfs::iActivity::Ptr activity)
{
  activity->activityStart(kRecvBufferAssign);
  while(1)
  {
    cPtr<cIOBuffer> pIOBuffer;
    {
      cLockGuard g(&m_IncomingAccess);
      if(m_IncomingBuffers.empty())
        return;

      tBufferStream::iterator it = m_IncomingBuffers.begin();
      if(m_LastSuccessfulPacketNumber != it->first - 1)
      {
        //QSOS((L"NewBuffersAvailable - did not find a packet in sequence %d != %d", m_LastSuccessfulPacketNumber, (it->first - 1)));
        return;
      }

      pIOBuffer = it->second;
      m_IncomingBuffers.erase(it);
    }

    activity->activityStart(kRecvIOBufferBytes);
    bool complete = pIOBuffer->SendCompletePackets(pSocketData);

    if(complete)
    {
      cLockGuard g(&m_IncomingAccess);
      m_LastSuccessfulPacketNumber = pIOBuffer->sequenceNumber();
    }
    else
    {
      cLockGuard g(&m_IncomingAccess);
      if(m_IncomingBuffers.empty())
      {
        m_IncomingBuffers.insert(tBufferStream::value_type(pIOBuffer->sequenceNumber(), pIOBuffer));
        return;
      }
      else
      {
        tBufferStream::iterator it = m_IncomingBuffers.begin();
        if(pIOBuffer->sequenceNumber() == it->first - 1)
        {
          if (pIOBuffer->getBuffer().isValid() && pIOBuffer->getBuffer()->getSize() > 0)
          {
            it->second->Prepend(pIOBuffer);
          }
          m_LastSuccessfulPacketNumber = pIOBuffer->sequenceNumber();
        }
        else
        {
          m_IncomingBuffers.insert(tBufferStream::value_type(pIOBuffer->sequenceNumber(), pIOBuffer));
          return;
        }
      }
    }
  }
}
