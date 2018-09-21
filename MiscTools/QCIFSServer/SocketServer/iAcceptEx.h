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

//iAcceptEx.h

#pragma once

class cSocketServer;
class cListeningSocket;

class iAcceptEx : public vfs::iManager
{
public:
  QDEFINE_SINGLETON_IFACE(iAcceptEx, L"{77CE06EE-645F-4705-95B8-65D82B2034A0} // iAcceptEx", vfs::iManager);

  virtual ~iAcceptEx(){}

  virtual BOOL AcceptEx(const SOCKET sListenSocket
    , cSocketServer* const pSocketServer
    , const vfs::cPtr<cListeningSocket> socket
    , const UINT_PTR associatedSessionID) = 0;

  virtual VOID GetAcceptExSockaddrs(PVOID lpOutputBuffer
    , DWORD dwReceiveDataLength
    , DWORD dwLocalAddressLength
    , DWORD dwRemoteAddressLength
    , OUT struct sockaddr **LocalSockaddr
    , OUT LPINT LocalSockaddrLength
    , OUT struct sockaddr **RemoteSockaddr
    , OUT LPINT RemoteSockaddrLength) = 0;
};