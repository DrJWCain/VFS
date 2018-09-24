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

//cIOBuffer.h

#pragma once

#include "cSocketServer.h"

#include "..\common\iCompletionMsg.h"

class cIOBuffer : public iCompletionMsg, public vfs::cRefCount
{
public:
  cIOBuffer(cSocketServer *pSocketServer);
  cIOBuffer(cSocketServer *pSocketServer
    , const vfs::cPtr<vfs::cMemoryView> pMem);
  ~cIOBuffer(void);

  void deliver(ULONG_PTR pCompletionKey, const DWORD dwBytes);
  void err(ULONG_PTR pCompletionKey, const DWORD err);

  bool SendCompletePackets(SOCKET_DATA *pSockData);

  void Prepend(vfs::cPtr<cIOBuffer> other);

  int               sequenceNumber() const        {return m_SequenceNumber;}
  WSABUF           *getWSABuf()                   {return &m_WSABUF;}
  DWORD            getRecvBytes()                {return m_dwRecvBytes;}
  //DWORD            *getFlags()                    {return &m_dwFlags;}
  vfs::cPtr<vfs::cMemoryView> getBuffer() {return m_pBuffer;}

  void setSequenceNumber(int num) {m_SequenceNumber = num;}

private:
  int m_SequenceNumber;
  WSABUF    m_WSABUF;
  DWORD     m_dwRecvBytes;
  //DWORD     m_dwFlags;

  vfs::cPtr<vfs::cMemoryView> m_pBuffer;

  cSocketServer *m_pSocketServer;
  //LPOVERLAPPED m_pOverlapped;
};
