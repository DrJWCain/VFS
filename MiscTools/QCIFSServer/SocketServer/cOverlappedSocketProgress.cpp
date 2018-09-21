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

//cOverlappedSocketProgress.h

#include "StdAfx.h"

#include ".\coverlappedsocketprogress.h"
#include "cIOBuffer.h"

cOverlappedSocketProgress::cOverlappedSocketProgress(SOCKET_DATA *pSocketData)
: m_pSocketData(pSocketData)
{
}

cOverlappedSocketProgress::~cOverlappedSocketProgress(void)
{
}

bool cOverlappedSocketProgress::AddIncompletePacket(cIOBuffer *pIOBuffer)
{
  QTRACE((L"cOverlappedSocketProgress::AddIncompletePacket()"));
  bool bRet(false);
  m_access.lock();
  if (m_SequencePacketMap.empty())
  {
    m_SequencePacketMap.insert(tSequencePacketMap::value_type(pIOBuffer->sequenceNumber(), pIOBuffer));
    m_access.unlock();
    bRet = true;
  }
  else
  {
    tSequencePacketMap::iterator finder = m_SequencePacketMap.begin();
    if (finder->first == pIOBuffer->sequenceNumber() - 1)
    {
      // the packet at the front of m_SequencePacketMap precedes pIOBuffer
      m_SequencePacketMap.erase(finder);
      m_access.unlock();

      bRet = finder->second.Concatenate(pIOBuffer, m_pSocketData);
    }
    else if (finder->first -1 == pIOBuffer->sequenceNumber())
    {
      // pIOBuffer precedes the packet at the front of m_SequencePacketMap
      m_SequencePacketMap.erase(finder);
      m_access.unlock();

      bRet = pIOBuffer->Concatenate(finder->second, m_pSocketData);
    }
    else
    {
      m_SequencePacketMap.insert(tSequencePacketMap::value_type(pIOBuffer->sequenceNumber(), pIOBuffer));
      m_access.unlock();
      bRet = true;
    }
  }

  return bRet;
}