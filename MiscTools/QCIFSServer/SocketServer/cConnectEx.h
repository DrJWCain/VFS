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

//cConnectEx.h

#pragma once

#include "iconnectex.h"

class cConnectEx : public iConnectEx, public vfs::cRefCount
{
public:
  QDEFINE_SINGLETON_IMPL(cConnectEx, L"{E6885A30-35E8-45ed-9556-F0826B430A0A} // cConnectEx", iConnectEx);

  cConnectEx(void);
  ~cConnectEx(void);

  void QAPI logSummary() {QTRACE((L"cConnectEx - wraps WSA ConnectEx"));}
  void QAPI logDetailedSummary() {logSummary();}

  BOOL ConnectEx(SOCKET_DATA* pSocketData
    , cSocketServer* const pSocketServer
    , const vfs::cPtr<cClientSocket> socket
    , const UINT_PTR associatedSessionID);

  BOOL DisconnectEx(SOCKET_DATA* pSocketData, cSocketServer* const pSocketServer);

  void clientSessionDead(UINT_PTR uiSessionID);

private:  //private member functions
  template <typename func>
  bool assignFunc(SOCKET sock, func& fn, GUID fnGuid);

private:  //private member variables
  LPFN_CONNECTEX m_pfnConnectEx;
  LPFN_DISCONNECTEX m_pfnDisconnectEx;
  vfs::cLock m_connectExLock;
  std::list<vfs::cPtr<cClientSocket> > m_clientSockList;
};