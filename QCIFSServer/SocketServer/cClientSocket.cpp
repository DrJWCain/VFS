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

//cClientSocket.cpp

#include "StdAfx.h"

#include <Mswsock.h>
#include "cClientSocket.h"
#include "cSocketServer.h"
#include "iConnectEx.h"

#include "..\common\iCommonFactory.h"
#include "..\common\primatives.h"

using namespace vfs;

//-------------------------------------------------------------------------------------------
cClientSocket::cClientSocket(const USHORT clientPort, cSocketServer *pSocketServer, const in_addr& remoteAddr, const USHORT remotePort, const UINT_PTR associatedSessionID)
: m_ClientSocket(new SOCKET)
, m_pSocketServer(pSocketServer)
, m_remoteAddr(remoteAddr)
, m_remotePort(remotePort)
, m_associatedSessionID(associatedSessionID)
, m_pSocketData(0)
{
  if ((*m_ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
  {
    const DWORD dwErr(WSAGetLastError());
    QSOS((L"cClientSocket::cClientSocket - socket failed with error %d - %s"
      , dwErr
      , win32ErrorToString(dwErr).c_str()));
    throw -1;
  }

  SOCKADDR_IN RemoteAddr;
  RemoteAddr.sin_family = AF_INET;
  RemoteAddr.sin_port = htons(m_remotePort);
  RemoteAddr.sin_addr = m_remoteAddr;

  SOCKADDR_IN LocalAddr;
  LocalAddr.sin_family = AF_INET;
  LocalAddr.sin_port = htons(clientPort);
  LocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  m_pSocketData = m_pSocketServer->AddClientCompletionKey(*m_ClientSocket, LocalAddr, RemoteAddr);
  if (!m_pSocketData)
  {
    const DWORD dwErr(WSAGetLastError());
    QSOS((L"cClientSocket::cClientSocket - failed to add socket to completion port with error %d - %s"
      , dwErr
      , win32ErrorToString(dwErr).c_str()));
    closesocket(*m_ClientSocket);
    throw -1;
  }

  BOOL bTRUE(TRUE);
  int ret = setsockopt(*m_ClientSocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char *>(&bTRUE), sizeof(BOOL));
  if (ret == SOCKET_ERROR)
  {
    DWORD dwRet = WSAGetLastError();
    QSOS((L"cClientSocket::cClientSocket() - setsockopt SO_REUSEADDR returned WSA error [%d]", dwRet));
  }

  // Associate the local address information with the socket using bind.
  if (bind(*m_ClientSocket, (SOCKADDR *)&LocalAddr, sizeof(LocalAddr)) == SOCKET_ERROR)
  {
    DWORD dw = WSAGetLastError();
	  if (dw == WSAEADDRINUSE)
		  QSOS((L"cClientSocket::cClientSocket - address in use, turn off port %u", clientPort));
	  else
		  QSOS((L"cClientSocket::cClientSocket - bind failed with error %d", dw));
    closesocket(*m_ClientSocket);
    throw -1;
  }

  //connect to server address and port - done using connectEx via postConnest call
  doConnect(m_pSocketData);
}


//-------------------------------------------------------------------------------------------
cClientSocket::~cClientSocket(void)
{
  if (INVALID_SOCKET != *m_ClientSocket)
    closesocket(*m_ClientSocket);
}


//-------------------------------------------------------------------------------------------
in_addr cClientSocket::getRemoteAddress(void)
{
  return m_remoteAddr;
}


//-------------------------------------------------------------------------------------------
USHORT cClientSocket::getRemotePort(void)
{
  return m_remotePort;
}


//-------------------------------------------------------------------------------------------
bool cClientSocket::doConnect(SOCKET_DATA* pSocketData)
{
  bool bSuccess = iConnectEx::singleton().ConnectEx(pSocketData, m_pSocketServer, this, m_associatedSessionID) == TRUE;
  if (!bSuccess)
  {
    const DWORD dwErr(WSAGetLastError());
    if (ERROR_IO_PENDING != dwErr)
      QSOS((L"cClientSocket::doConnect - connect failed with error %d - %s"
        , dwErr
        , win32ErrorToString(dwErr).c_str()));
  }

  return bSuccess;
}


//-------------------------------------------------------------------------------------------
bool cClientSocket::isSession(UINT_PTR uiSessionID)
{
  bool bRet(false);
  if (m_pSocketData != 0)
    bRet = m_pSocketData->sock == uiSessionID;

  return bRet;
}