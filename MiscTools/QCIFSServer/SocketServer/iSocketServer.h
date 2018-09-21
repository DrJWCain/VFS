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

//iSocketServer.h

#pragma once

class iPacketProcessor;
class iTXPacket;

class iSocketServer
{
public:
  //QDEFINE_SINGLETON_IFACE(iSocketServer, L"{67A2394E-3349-4e3e-83CB-2F502B4E72AA} // iSocketServer", vfs::iManager);
  virtual ~iSocketServer(){}

  virtual void notifyExit() = 0;
  virtual void closeSession(UINT_PTR SessionID) = 0;
  virtual void closeClientSession(UINT_PTR SessionID) = 0;

  virtual bool addClientConnection(const USHORT clientPort, const in_addr& remoteAddr, const USHORT remotePort, const UINT_PTR associatedSessionID) = 0;
  virtual bool addListeningConnection(UINT_PTR& rListenSessionID, const USHORT listeningPort, sockaddr_in& rListeningSockAddr, const UINT_PTR associatedSessionID) = 0;
  virtual vfs::cPtr<iTXPacket> getTXPacketForSession(UINT_PTR sessionID) = 0;

  virtual int numOfClients() = 0;
  virtual std::string clientIP(int index) = 0;
};


class iSocketServerFactory : public vfs::iManager
{
public:
  QDEFINE_SINGLETON_IFACE(iSocketServerFactory, L"{41AFB8D3-0E0C-452a-A22D-A9689F3EF5B5} // iSocketServerFactory", vfs::iManager);

  virtual ~iSocketServerFactory(){}

  virtual vfs::cPtr<iSocketServer> createSocketServer(const vfs::cPtr<iPacketProcessor> pPacketProcessor
    , const std::vector<USHORT> &listeningPorts) = 0;

  virtual std::vector<vfs::String> getIpAddresses() = 0;
};
