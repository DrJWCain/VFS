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

//cClientSocket.h

#pragma once

class cSocketServer;
class iEvent;
struct SOCKET_DATA;

class cClientSocket : public vfs::cRefCount
{
public:
  cClientSocket(const USHORT clientPort, cSocketServer *pSocketSever, const in_addr& remoteAddr, const USHORT remotePort, const UINT_PTR associatedSessionID);
  ~cClientSocket(void);
  
  in_addr getRemoteAddress(void);
  USHORT getRemotePort(void);

  bool isSession(UINT_PTR uiSessionID);

private:
  bool doConnect(SOCKET_DATA* pSocketData);

private:
  std::auto_ptr<SOCKET> m_ClientSocket;
  cSocketServer* const m_pSocketServer;

  //stop auto_ptr m_ListeningSocket being copied/assigned
  cClientSocket(const cClientSocket&);
  cClientSocket& operator=(const cClientSocket&);

  in_addr m_remoteAddr;
  SHORT m_remotePort;
  UINT_PTR m_associatedSessionID;
  SOCKET_DATA* m_pSocketData;
};
