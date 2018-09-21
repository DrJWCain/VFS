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

//cConnectEx.cpp

#include "StdAfx.h"

#include ".\cConnectEx.h"
#include <mswsock.h>
#include "cConnectMsg.h"
#include "cDisconnectMsg.h"
#include "cClientSocket.h"
#include "cSocketServer.h"

using namespace vfs;

cConnectEx::cConnectEx(void)
: m_pfnConnectEx(NULL)
, m_pfnDisconnectEx(NULL)
{
}

cConnectEx::~cConnectEx(void)
{
}

BOOL cConnectEx::ConnectEx(SOCKET_DATA* pSocketData
  , cSocketServer* const pSocketServer
  , const cPtr<cClientSocket> socket
  , const UINT_PTR associatedSessionID)
{
  // make sure pointer to AcceptEx function is initialised
  if (NULL == m_pfnConnectEx)
  {
    cLockGuard lg(&m_connectExLock);
    if (NULL == m_pfnConnectEx)
    {
      GUID gid1 = WSAID_CONNECTEX;
      if (!assignFunc<LPFN_CONNECTEX>(pSocketData->sock
        , m_pfnConnectEx
        , gid1))
      {
        QSOS((L"cConnectEx::ConnectEx() - failed to get pointer to ConnectEx function"));
        return FALSE;
      }
    }
  }

  cPtr<cConnectMsg> pConnectMsg = new cConnectMsg(pSocketServer, socket, pSocketData, associatedSessionID);
  if (pConnectMsg.isNull())
  {
    QSOS((L"cConnectEx::ConnectEx() - failed to create ConnectEx msg"));
    return FALSE;
  }

  m_clientSockList.push_back(socket);
  return pConnectMsg->ConnectEx(m_pfnConnectEx);
}

BOOL cConnectEx::DisconnectEx(SOCKET_DATA* pSocketData, cSocketServer* const pSocketServer)
{
  if (NULL == m_pfnDisconnectEx)
  {
    cLockGuard lg(&m_connectExLock);
    if (NULL == m_pfnDisconnectEx)
    {
      GUID gid1 = WSAID_DISCONNECTEX;
      if (!assignFunc<LPFN_DISCONNECTEX>(pSocketData->sock
        , m_pfnDisconnectEx
        , gid1))
      {
        QSOS((L"cConnectEx::DisconnectEx() - failed to get pointer to DisconnectEx function"));
        return FALSE;
      }
    }
  }

  cPtr<cDisconnectMsg> pDisconnectMsg = new cDisconnectMsg(pSocketServer, pSocketData);
  if (pDisconnectMsg.isNull())
  {
    QSOS((L"cConnectEx::DisconnectEx() - failed to create DisconnectEx msg"));
    return FALSE;
  }

  return pDisconnectMsg->DisconnectEx(m_pfnDisconnectEx);
}


void cConnectEx::clientSessionDead(UINT_PTR uiSessionID)
{
  for (std::list<cPtr<cClientSocket> >::iterator it = m_clientSockList.begin(); it != m_clientSockList.end(); ++it)
  {
    if ((*it)->isSession(uiSessionID))
    {
      m_clientSockList.erase(it);
      break;
    }
  }
}

template <typename func>
bool cConnectEx::assignFunc(const SOCKET sock, func& fn, GUID fnGuid)
{
  DWORD dwBytes;
  if (0 != WSAIoctl(sock
    , SIO_GET_EXTENSION_FUNCTION_POINTER
    , &fnGuid
    , sizeof(fnGuid)
    , &fn
    , sizeof(fn)
    , &dwBytes
    , NULL
    , NULL))
  {
    const DWORD dwErr(WSAGetLastError());
    QSOS((L"cConnectEx::assignFunc() - failed to get pointer to WSA function with error %d - %s"
      , dwErr
      , win32ErrorToString(dwErr).c_str()));
    return false;
  }

  return true;
}