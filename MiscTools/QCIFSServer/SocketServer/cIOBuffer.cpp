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

//cIOBuffer.cpp

#include "StdAfx.h"

#include ".\ciobuffer.h"
#include "cRXPacket.h"
#include "iPacketProcessor.h"

iActivity::Ptr getActivity();

cIOBuffer::cIOBuffer(cSocketServer *pSocketServer)
: m_pBuffer(pSocketServer->GetRXBuffer())
, m_dwRecvBytes(0)
//, m_dwFlags(0)
, m_SequenceNumber(0)
, m_pSocketServer(pSocketServer)
{
  if (m_pBuffer.isValid())
  {
    m_WSABUF.buf = reinterpret_cast<char *>(m_pBuffer->getBytes());
    m_WSABUF.len = m_pBuffer->getSize();
  }
  else
  {
    m_WSABUF.buf = NULL;
    m_WSABUF.len = 0;
  }
}

cIOBuffer::cIOBuffer(cSocketServer *pSocketServer
  , const vfs::cPtr<vfs::cMemoryView> pMem)
: m_pBuffer(pMem)
, m_dwRecvBytes(pMem.isValid() ? pMem->getSize() : 0)
//, m_dwFlags(0)
, m_SequenceNumber(0)
, m_pSocketServer(pSocketServer)
{
}

cIOBuffer::~cIOBuffer(void)
{
  //QTRACE((L"cIOBuffer::~cIOBuffer() ref: %d", m_pBuffer.getCounter()));
}

iActivity::Ptr getActivity();

void cIOBuffer::deliver(ULONG_PTR pCompletionKey, const DWORD dwBytes)
{
  cActivityScopeId as(getActivity(),kRecvNotify, (int)this);

  if (dwBytes == 0)
  {
    QSOS((L"cIOBuffer::deliver - dwBytes = 0!!!"));
    return err(pCompletionKey, GetLastError());
  }

  m_dwRecvBytes = dwBytes;

  SOCKET_DATA *pSockData = reinterpret_cast<SOCKET_DATA *>(pCompletionKey);
  try
  {
    m_pSocketServer->NotifyRecvCompleted(cPtr<cIOBuffer>(this, this), pSockData);
  }
  catch (const iPacketProcessor::unknown_protocol&)
  {
    QSOS((L"cIOBuffer::deliver iPacketProcessor::unknown_protocol"));
    m_pSocketServer->CloseSocket(pSockData);
  }
  InterlockedDecrement(&(pSockData->outstandingRecvs));
}

void cIOBuffer::err(ULONG_PTR pCompletionKey, const DWORD errCode)
{
  SOCKET_DATA* const pSockData = reinterpret_cast<SOCKET_DATA* const>(pCompletionKey);
  InterlockedDecrement(&(pSockData->outstandingRecvs));

  if (errCode == 64)
    QTRACE((L"cIOBuffer::err - Failed to WSARecv on socket [%Iu] with error [%d] - {%s}. WSAGetLastError() = %d"
      , pSockData->sock
      , errCode
      , win32ErrorToString(errCode).c_str()
      , WSAGetLastError()));
  else
    QSOS((L"cIOBuffer::err - Failed to WSARecv on socket [%Iu] with error [%d] - {%s}. WSAGetLastError() = %d"
      , pSockData->sock
      , errCode
      , win32ErrorToString(errCode).c_str()
      , WSAGetLastError()));

  if (errCode == ERROR_SUCCESS
    || errCode == ERROR_IO_PENDING
    || !m_pSocketServer->DoRecv(pSockData))  // socket gracefully closed
    m_pSocketServer->onRecvErr(pSockData);

  //if (errCode != ERROR_IO_PENDING
  //  && (errCode == ERROR_SUCCESS || !m_pSocketServer->DoRecv(pSockData)))  // socket gracefully closed
  //  m_pSocketServer->onRecvErr(pSockData);
}

bool cIOBuffer::SendCompletePackets(SOCKET_DATA *pSockData)
{
  DWORD dwBytes = m_pSocketServer->PacketSize(m_pBuffer, pSockData, m_dwRecvBytes);
  while(dwBytes && dwBytes <= m_dwRecvBytes)
  {
    cPtr<iRXPacket> pPacket = new cRXPacket(m_pBuffer->range((size_t)0, (size_t)dwBytes), pSockData->sock, m_pSocketServer, ntohl(pSockData->addr.sin_addr.S_un.S_addr), ntohs(pSockData->addr.sin_port));
    m_pSocketServer->AddPacket(pPacket);

    if(m_dwRecvBytes == dwBytes)
      return true;

    m_dwRecvBytes -= dwBytes;
    m_pBuffer = m_pBuffer->range((size_t)dwBytes, m_pBuffer->getSize() - dwBytes);

    dwBytes = m_pSocketServer->PacketSize(m_pBuffer, pSockData, m_dwRecvBytes);
  }

  return false;
}

void cIOBuffer::Prepend(cPtr<cIOBuffer> other)
{
  cPtr<cMemoryView> view1 = other->m_pBuffer->range((size_t)0, (size_t)other->m_dwRecvBytes);
  cPtr<cMemoryView> view2 = m_pBuffer->range((size_t)0, (size_t)m_dwRecvBytes);

  m_dwRecvBytes += other->m_dwRecvBytes;

  m_pBuffer = m_pSocketServer->GetRXBuffer(view1->getSize() + view2->getSize());
  memcpy(m_pBuffer->getBytes(), view1->getBytes(), view1->getSize());
  memcpy(m_pBuffer->getBytes() + view1->getSize(), view2->getBytes(), view2->getSize());
}
