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

//iConnectEx.h

#pragma once

class cSocketServer;
class cClientSocket;
struct SOCKET_DATA;

class iConnectEx : public vfs::iManager
{
public:
  QDEFINE_SINGLETON_IFACE(iConnectEx, L"{C0494587-ABDC-49d6-8EE9-EDAF16FAB87C} // iConnectEx", vfs::iManager);

  virtual ~iConnectEx(){}

  virtual BOOL ConnectEx(SOCKET_DATA* pSocketData
    , cSocketServer* const pSocketServer
    , const vfs::cPtr<cClientSocket> socket
    , const UINT_PTR associatedSessionID) = 0;

  virtual BOOL DisconnectEx(SOCKET_DATA* pSocketData
    , cSocketServer* const pSocketServer) = 0;

  virtual void clientSessionDead(UINT_PTR uiSessionID) = 0;
};