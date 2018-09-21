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

//cListeningSocket.cpp

#include "StdAfx.h"

#include "clisteningsocket.h"
#include "cSocketServer.h"
#include "iAcceptEx.h"

#include "..\common\iCommonFactory.h"
#include "..\common\primatives.h"

using namespace vfs;

cListeningSocket::cListeningSocket(USHORT ListeningPort, cSocketServer *pSocketServer, sockaddr_in& rListeningSockAddr, const bool bUseGivenAddress, const UINT_PTR associatedSessionID, bool bSingleUse)
: m_ListeningSocket(new SOCKET)
, m_pSocketServer(pSocketServer)
, m_associatedSessionID(associatedSessionID)
, m_bSingleUse(bSingleUse)
, m_uiUsageCount(0)
{
  if ((*m_ListeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) 
      == INVALID_SOCKET)
  {
    const DWORD dwErr(WSAGetLastError());
    QSOS((L"socket failed with error %d - %s"
      , dwErr
      , win32ErrorToString(dwErr).c_str()));
    throw -1;
  }

  if (!m_pSocketServer->AddCompletionKey(reinterpret_cast<ULONG_PTR>(m_ListeningSocket.get())))
  {
    const DWORD dwErr(WSAGetLastError());
    QSOS((L"failed to add socket to completion port with error %d - %s"
      , dwErr
      , win32ErrorToString(dwErr).c_str()));
    closesocket(*m_ListeningSocket);
    throw -1;
  }
  
  const int bufSizeRcv = 4*512*1024;
  int ret = setsockopt(*(m_ListeningSocket.get()), SOL_SOCKET, SO_RCVBUF, reinterpret_cast<const char *>(&bufSizeRcv), sizeof(int));
  if (ret == SOCKET_ERROR)
  {
    DWORD dwRet = WSAGetLastError();
    QTRACE((L"cListeningSocket::cListeningSocket() - setsockopt(SO_RCVBUF) returned WSA error [%d]", dwRet));
  }

  int buf = 0;
  int size = sizeof(int);
  getsockopt(*(m_ListeningSocket.get()), SOL_SOCKET, SO_RCVBUF, reinterpret_cast<char *>(&buf), &size);
  QTRACE((L"cListeningSocket::cListeningSocket() - getsockopt(SO_RCVBUF) %d", buf));

  const int bufSizeSnd = 4*512*1024;
  ret = setsockopt(*(m_ListeningSocket.get()), SOL_SOCKET, SO_SNDBUF, reinterpret_cast<const char *>(&bufSizeSnd), sizeof(int));
  if (ret == SOCKET_ERROR)
  {
    DWORD dwRet = WSAGetLastError();
    QTRACE((L"cListeningSocket::cListeningSocket() - setsockopt(SO_SNDBUF) returned WSA error [%d]", dwRet));
  }

  buf = 0;
  size = sizeof(int);
  getsockopt(*(m_ListeningSocket.get()), SOL_SOCKET, SO_SNDBUF, reinterpret_cast<char *>(&buf), &size);
  QTRACE((L"cListeningSocket::cListeningSocket() - getsockopt(SO_SNDBUF) %d", buf));

  //BOOL bTRUE(TRUE);
  //int ret = setsockopt(m_ListeningSocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char *>(&bTRUE), sizeof(BOOL));
  //if (ret == SOCKET_ERROR)
  //{
  //  DWORD dwRet = WSAGetLastError();
  //  QTRACE((L"cListeningSocket::cListeningSocket() - setsockopt returned WSA error [%d]", dwRet);
  //}

  // Setup a SOCKADDR_IN structure that will tell bind that we
  // want to listen for connections on all interfaces using port
  // 5150. Notice how we convert the Port variable from host byte
  // order to network byte order.

  SOCKADDR_IN ServerAddr;
  ServerAddr.sin_family = AF_INET;
  ServerAddr.sin_port = htons(ListeningPort);
  if (!bUseGivenAddress)
    ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  else
  {
    ServerAddr = rListeningSockAddr;
    ServerAddr.sin_port = htons(ListeningPort);
    QTRACE((L"cListeningSocket::cListeningSocket - bind to address %S:%d", inet_ntoa(ServerAddr.sin_addr), ListeningPort));
  }

  // Associate the address information with the socket using bind.

  if (bind(*m_ListeningSocket, (SOCKADDR *)&ServerAddr, sizeof(ServerAddr)) 
      == SOCKET_ERROR)
  {
    DWORD dw = WSAGetLastError();
	  if (dw == WSAEADDRINUSE)
		  QSOS((L"turn off port %u", ListeningPort));
	  else
		  QSOS((L"bind failed with error %d - %s", dw, win32ErrorToString(dw).c_str()));
    closesocket(*m_ListeningSocket);
    throw -1;
  }

  //read back the sock address to pass back details
  sockaddr_in sa;
  int len(sizeof(sockaddr_in));
  DWORD dwErr = getsockname(*m_ListeningSocket, (SOCKADDR*)&sa, &len);
  if (dwErr == SOCKET_ERROR)
  {
    dwErr = GetLastError();
    QSOS((L"cListeningSocket::cListeningSocket - getsockname returned err %d - %s", dwErr, win32ErrorToString(dwErr).c_str()));
  }
  else
  {
    QTRACE((L"cListeningSocket::cListeningSocket - listening on port %d", ntohs(sa.sin_port)));
    rListeningSockAddr.sin_port = sa.sin_port;
  }


  // Listen for client connections. We used a backlog of 5 which is
  // normal for many applications.
  if (listen(*m_ListeningSocket, 5) == SOCKET_ERROR)
  {
    QSOS((L"listen failed with error %d", WSAGetLastError()));
    closesocket(*m_ListeningSocket);
    throw -1;
  }
}

cListeningSocket::~cListeningSocket(void)
{
  QTRACE((L"~cListeningSocket - closing socket handle %Iu", *m_ListeningSocket));
  if (INVALID_SOCKET != *m_ListeningSocket)
    closesocket(*m_ListeningSocket);
}


UINT_PTR cListeningSocket::getListeningSocket()
{
  return *m_ListeningSocket;
}


void cListeningSocket::postAccept()
{
  if (!m_bSingleUse || m_uiUsageCount == 0)
  {
    if (FALSE == iAcceptEx::singleton().AcceptEx(*m_ListeningSocket, m_pSocketServer, this, m_associatedSessionID))
    {
      const DWORD dwErr(WSAGetLastError());
      if (ERROR_IO_PENDING != dwErr)
        QSOS((L"cListeningSocket::runLoop() - accept failed with error %d - %s"
          , dwErr
          , win32ErrorToString(dwErr).c_str()));
      else
        m_uiUsageCount++;
    }
    else
      m_uiUsageCount++;
  }
  else
    QTRACE((L"cListeningSocket::postAccept - usage count complete, no further accepts - allow listening socket to die"));
}