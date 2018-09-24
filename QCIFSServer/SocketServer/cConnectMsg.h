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

//cConnectMsg.h

#pragma once

#include "..\common\icompletionmsg.h"

class cSocketServer;
class cClientSocket;
struct SOCKET_DATA;

class cConnectMsg : public iCompletionMsg, public vfs::cRefCount
{
public:
  cConnectMsg(cSocketServer* const pSockerServer
    , const vfs::cPtr<cClientSocket> socket
    , SOCKET_DATA* pSocketData
    , const UINT_PTR associatedSessionID);
  ~cConnectMsg(void);

  //iCompletionMsg
  void deliver(ULONG_PTR pCompletionKey, const DWORD dwBytes);
  void err(ULONG_PTR pCompletionKey, const DWORD err);

  //public member functions
  BOOL ConnectEx(const LPFN_CONNECTEX fn);

private:  //private member variables
  vfs::cLock m_lock;
  cSocketServer* const m_pSocketServer;
  const vfs::cPtr<cClientSocket> m_pSocket;
  SOCKET_DATA* m_pSocketData;
  BOOL m_bConnectSuccessful;
  UINT_PTR m_associatedSessionID;
};
