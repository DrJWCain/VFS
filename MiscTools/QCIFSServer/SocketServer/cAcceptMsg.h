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

//cAcceptMsg.h

#pragma once

#include "..\common\icompletionmsg.h"

class cSocketServer;
class cListeningSocket;

class cAcceptMsg : public iCompletionMsg, public vfs::cRefCount
{
public:
  cAcceptMsg(cSocketServer* const pSockerServer
    , const vfs::cPtr<cListeningSocket> socket
    , const SOCKET listenSocket
    , const UINT_PTR associatedSessionID);
  ~cAcceptMsg(void);

  //iCompletionMsg
  void deliver(ULONG_PTR pCompletionKey, const DWORD dwBytes);
  void err(ULONG_PTR pCompletionKey, const DWORD err);

  //public member functions
  BOOL AcceptEx(const LPFN_ACCEPTEX fn);

private:  //private member variables
  cSocketServer* const m_pSocketServer;
  const vfs::cPtr<cListeningSocket> m_pSocket;
  const SOCKET m_listenSocket;
  const SOCKET m_sock;
  const vfs::cPtr<vfs::cMemoryView> m_pBuffer;
  UINT_PTR m_associatedSessionID;
};
