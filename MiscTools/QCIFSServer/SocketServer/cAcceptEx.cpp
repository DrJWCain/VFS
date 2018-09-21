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

//cAcceptEx.cpp

#include "StdAfx.h"

#include ".\cacceptex.h"
#include <mswsock.h>
#include "cAcceptMsg.h"
#include "cListeningSocket.h"

using namespace vfs;

cAcceptEx::cAcceptEx(void)
: m_pfnAcceptEx(NULL)
, m_pfnGetAcceptExSockaddrs(NULL)
{
}

cAcceptEx::~cAcceptEx(void)
{
}

BOOL cAcceptEx::AcceptEx(const SOCKET sListenSocket
  , cSocketServer* const pSocketServer
  , const vfs::cPtr<cListeningSocket> socket
  , const UINT_PTR associatedSessionID)
{
  // make sure pointer to AcceptEx function is initialised
  if (NULL == m_pfnAcceptEx)
  {
    cLockGuard lg(&m_acceptExLock);
    if (NULL == m_pfnAcceptEx)
    {
      GUID gid1 = WSAID_ACCEPTEX;
      if (!assignFunc<LPFN_ACCEPTEX>(sListenSocket
        , m_pfnAcceptEx
        , gid1))
      {
        QSOS((L"cAcceptEx::AcceptEx() - failed to get pointer to AcceptEx function"));
        return FALSE;
      }

      GUID gid2 = WSAID_GETACCEPTEXSOCKADDRS;
      if (!assignFunc<LPFN_GETACCEPTEXSOCKADDRS>(sListenSocket
        , m_pfnGetAcceptExSockaddrs
        , gid2))
      {
        QSOS((L"cAcceptEx::AcceptEx() - failed to get pointer to AcceptExSockaddrs function"));
        return FALSE;
      }
    }
  }

  cPtr<cAcceptMsg> pAcceptMsg = new cAcceptMsg(pSocketServer, socket, sListenSocket, associatedSessionID);
  if (pAcceptMsg.isNull())
  {
    QSOS((L"cAcceptEx::callAcceptEx() - failed to create AcceptEx msg"));
    return FALSE;
  }

  return pAcceptMsg->AcceptEx(m_pfnAcceptEx);
}

VOID cAcceptEx::GetAcceptExSockaddrs(PVOID lpOutputBuffer
  , DWORD dwReceiveDataLength
  , DWORD dwLocalAddressLength
  , DWORD dwRemoteAddressLength
  , sockaddr **LocalSockaddr
  , LPINT LocalSockaddrLength
  , sockaddr **RemoteSockaddr
  , LPINT RemoteSockaddrLength)
{
  if (NULL != m_pfnGetAcceptExSockaddrs)
  {
    m_pfnGetAcceptExSockaddrs(lpOutputBuffer
      , dwReceiveDataLength
      , dwLocalAddressLength
      , dwRemoteAddressLength
      , LocalSockaddr
      , LocalSockaddrLength
      , RemoteSockaddr
      , RemoteSockaddrLength);
  }
  else
    QSOS((L"cAcceptEx::GetAcceptExSockaddrs() - NULL"));
}

template <typename func>
bool cAcceptEx::assignFunc(const SOCKET sock, func& fn, GUID fnGuid)
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
    QSOS((L"cAcceptEx::call() - failed to get pointer to WSA function with error %d - %s"
      , dwErr
      , win32ErrorToString(dwErr).c_str()));
    return false;
  }

  return true;
}