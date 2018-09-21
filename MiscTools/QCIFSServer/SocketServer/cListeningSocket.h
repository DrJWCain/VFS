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

//cListeningSocket.h

#pragma once

class cSocketServer;
class iEvent;

class cListeningSocket : public vfs::cRefCount
{
public:
  cListeningSocket(USHORT ListeningPort, cSocketServer *pSocketSever, sockaddr_in& rListeningSockAddr, const bool bUseGivenAddress = false, const UINT_PTR associatedSessionID = 0, bool bSingleUse = false);
  ~cListeningSocket(void);

  UINT_PTR getListeningSocket();
  
  void QAPI postAccept();

private:
  std::auto_ptr<SOCKET> m_ListeningSocket;
  cSocketServer* const m_pSocketServer;

  //stop auto_ptr m_ListeningSocket being copied/assigned
  cListeningSocket(const cListeningSocket&);
  cListeningSocket& operator=(const cListeningSocket&);

  UINT_PTR m_associatedSessionID; //this is for FTP passive mode - holds associated cmd session ID
  bool m_bSingleUse;
  vfs::UInt32 m_uiUsageCount;
};
