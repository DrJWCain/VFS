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

//cAcceptMsg .cpp

#include "StdAfx.h"

#include ".\cacceptmsg.h"
#include "iAcceptEx.h"
#include "cSocketServer.h"
#include "cIOBuffer.h"
#include "cListeningSocket.h"

using namespace vfs;

cAcceptMsg::cAcceptMsg(cSocketServer* const pSocketServer
  , const cPtr<cListeningSocket> _socket
  , const SOCKET listenSocket
  , const UINT_PTR associatedSessionID)
: m_pSocketServer(pSocketServer)
, m_pSocket(_socket)
, m_listenSocket(listenSocket)
, m_sock(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
, m_pBuffer(new cMemoryView(new cMemory((size_t)1024, cMemory::eVirtual)))
, m_associatedSessionID(associatedSessionID)
{
  if (INVALID_SOCKET == m_sock)
  {
    const DWORD dwErr = WSAGetLastError();
    QSOS((L"cAcceptEx::callAcceptEx() - faild to create new connection socket with error %d - %s"
      , dwErr
      , win32ErrorToString(dwErr).c_str()));
  }
}

cAcceptMsg::~cAcceptMsg(void)
{
}

void cAcceptMsg::deliver(ULONG_PTR pCompletionKey, const DWORD dwBytes)
{
  if (m_pBuffer.isValid())
  {
    SOCKADDR* lpLocalSockaddr(NULL);
    SOCKADDR* lpRemoteSockaddr(NULL);
    int LocalSockaddrLen(0);
    int RemoteSockaddrLen(0);
  
    iAcceptEx::singleton().GetAcceptExSockaddrs(m_pBuffer->getBytes()
      //, m_pBuffer->getSize() - ((sizeof(SOCKADDR_STORAGE) + 16) * 2)
      , 0
      , sizeof(SOCKADDR_IN) + 16
      , sizeof(SOCKADDR_IN) + 16
      , &lpLocalSockaddr
      , &LocalSockaddrLen
      , &lpRemoteSockaddr
      , &RemoteSockaddrLen);

    const SOCKADDR_IN* const pLSAI = reinterpret_cast<SOCKADDR_IN*>(lpLocalSockaddr);
    std::string localIP = inet_ntoa(pLSAI->sin_addr);
    const SOCKADDR_IN * const pRSAI = reinterpret_cast<SOCKADDR_IN*>(lpRemoteSockaddr);
    std::string remoteIP = inet_ntoa(pRSAI->sin_addr);
    QTRACE((L"Accepted a client connection from %S:%d to %S:%d. (Socket [%p])"
      , remoteIP.c_str()
      , ntohs(pRSAI->sin_port)
      , localIP.c_str()
      , ntohs(pLSAI->sin_port)
      , m_sock));

    m_pSocketServer->AddSocket(m_sock
      , *pLSAI
      , *pRSAI
      , (dwBytes > 0) ? m_pBuffer->first((size_t)dwBytes) : cPtr<cMemoryView>()
      , m_associatedSessionID);
  }


  m_pSocket->postAccept();
}

void cAcceptMsg::err(ULONG_PTR pCompletionKey, const DWORD err)
{
  QSOS((L"cAcceptMsg::err() - failed %p with error %d - %s"
    , pCompletionKey
    , err
    , win32ErrorToString(err).c_str()));
  m_pSocket->postAccept();
}

BOOL cAcceptMsg::AcceptEx(const LPFN_ACCEPTEX fn)
{
  IO_DATA *pIOData = new IO_DATA;
  ZeroMemory(pIOData, sizeof(OVERLAPPED));
  pIOData->pMsg = cPtr<iCompletionMsg>(this, this);

  DWORD dwBytes;
  return fn(m_listenSocket
    , m_sock
    , m_pBuffer->getBytes()
    , 0
    //, m_pBuffer->getSize() - ((sizeof(SOCKADDR_IN) + 16) * 2)
    , sizeof(SOCKADDR_IN) + 16
    , sizeof(SOCKADDR_IN) + 16
    , &dwBytes
    , reinterpret_cast<LPOVERLAPPED>(pIOData));
}