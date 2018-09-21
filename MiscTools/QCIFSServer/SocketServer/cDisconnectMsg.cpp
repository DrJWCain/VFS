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

//cDisconnectMsg .cpp

#include "StdAfx.h"

#include ".\cDisconnectMsg.h"
#include "iConnectEx.h"
#include "cSocketServer.h"

using namespace vfs;

cDisconnectMsg::cDisconnectMsg(cSocketServer* const pSocketServer, SOCKET_DATA* pSocketData)
: m_pSocketServer(pSocketServer)
, m_pSocketData(pSocketData)
, m_bDisconnectSuccessful(TRUE)
{
}

cDisconnectMsg::~cDisconnectMsg(void)
{
}

void cDisconnectMsg::deliver(ULONG_PTR pCompletionKey, const DWORD dwBytes)
{
}

void cDisconnectMsg::err(ULONG_PTR pCompletionKey, const DWORD err)
{
  QSOS((L"cDisconnectMsg::err() - failed %p with error %d - %s"
    , pCompletionKey
    , err
    , win32ErrorToString(err).c_str()));
}

BOOL cDisconnectMsg::DisconnectEx(const LPFN_DISCONNECTEX fn)
{
  IO_DATA *pIOData = new IO_DATA;
  ZeroMemory(&(pIOData->overlapped), sizeof(OVERLAPPED));
  pIOData->pMsg = cPtr<iCompletionMsg>(this, this);

  m_bDisconnectSuccessful = fn(m_pSocketData->sock, &(pIOData->overlapped), 0, 0);

  if (!m_bDisconnectSuccessful)
  {
    DWORD dwErr = WSAGetLastError();

    if (dwErr == ERROR_IO_PENDING || dwErr == WSA_IO_PENDING)
      m_bDisconnectSuccessful = TRUE;
    else
      QSOS((L"cDisconnectMsg::DisconnectEx - failed with error %d, \"%s\"", dwErr, win32ErrorToString(dwErr).c_str()));
  }

  return m_bDisconnectSuccessful;
}