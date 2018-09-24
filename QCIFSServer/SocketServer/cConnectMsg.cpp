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

//cConnectMsg .cpp

#include "StdAfx.h"

#include ".\cConnectmsg.h"
#include "iAcceptEx.h"
#include "iConnectEx.h"
#include "cSocketServer.h"
#include "cIOBuffer.h"
#include "cClientSocket.h"

using namespace vfs;

cConnectMsg::cConnectMsg(cSocketServer* const pSocketServer
  , const cPtr<cClientSocket> _socket
  , SOCKET_DATA* pSocketData
  , const UINT_PTR associatedSessionID)
: m_pSocketServer(pSocketServer)
, m_pSocket(_socket)
, m_pSocketData(pSocketData)
, m_bConnectSuccessful(TRUE)
, m_associatedSessionID(associatedSessionID)
{
  QTRACE((L"cConnectMsg::cConnectMsg"));
}

cConnectMsg::~cConnectMsg(void)
{
}

void cConnectMsg::deliver(ULONG_PTR pCompletionKey, const DWORD dwBytes)
{
  cLockGuard lg(&m_lock); //use lock to make sure this doesn't happen until ConnectEx has set m_bConnectSuccessful
  if (!m_bConnectSuccessful)
  {
    QSOS((L"cConnectMsg::deliver - connect failed."));
    return;
  }

  m_pSocketServer->AddClientSocket(m_pSocketData, m_associatedSessionID);
}

void cConnectMsg::err(ULONG_PTR pCompletionKey, const DWORD err)
{
  QSOS((L"cConnectMsg::err() - failed %p with error %d - %s"
    , pCompletionKey
    , err
    , win32ErrorToString(err).c_str()));
}

BOOL cConnectMsg::ConnectEx(const LPFN_CONNECTEX fn)
{
  IO_DATA *pIOData = new IO_DATA;
  ZeroMemory(&(pIOData->overlapped), sizeof(OVERLAPPED));

  {
    cLockGuard lg(&m_lock);
    pIOData->pMsg = cPtr<iCompletionMsg>(this, this);

    SOCKADDR_IN RemoteAddr;
    RemoteAddr.sin_family = AF_INET;
    RemoteAddr.sin_port = htons(m_pSocket->getRemotePort());
    RemoteAddr.sin_addr = m_pSocket->getRemoteAddress();

    m_bConnectSuccessful =  fn(m_pSocketData->sock
       , (SOCKADDR *)&RemoteAddr
       , sizeof(RemoteAddr)
       , NULL
       , 0
       , NULL
       , &(pIOData->overlapped));

    if (!m_bConnectSuccessful)
    {
      DWORD dwErr = WSAGetLastError();

      if (dwErr == ERROR_IO_PENDING || dwErr == WSA_IO_PENDING || dwErr == 0)
        m_bConnectSuccessful = TRUE;
      else
        QSOS((L"cConnectMsg::ConnectEx - failed with error %d, \"%s\"", dwErr, win32ErrorToString(dwErr).c_str()));
    }
  }

  if (m_bConnectSuccessful)
  {
    DWORD dwBt(0);
    DWORD dwFlags(0);
    BOOL bRet = WSAGetOverlappedResult(m_pSocketData->sock, 
      &(pIOData->overlapped),
      &dwBt,
      TRUE,
      &dwFlags);

    if (!bRet)
    {
      DWORD dwErr = WSAGetLastError();
      QSOS((L"cConnectMsg::ConnectEx - WSAGetOverlappedResult failed with error %d, \"%s\"", dwErr, win32ErrorToString(dwErr).c_str()));
    }
  }

  return m_bConnectSuccessful;
}