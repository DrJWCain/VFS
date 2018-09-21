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

//cRXPacket.h

#pragma once

#include "ipacket.h"

class cSocketServer;

class cRXPacket : public iRXPacket, public cRefCount
{
public:
  cRXPacket(cPtr<cMemoryView> pBuffer, const SOCKET sock, cSocketServer *pSocketServer, ULONG ipAddress, USHORT port);
  ~cRXPacket(void);

  cPtr<cMemoryView> getRXBuffer() {return m_pBuffer;}
  cPtr<iTXPacket> getResponsePacket();
  UINT_PTR getSessionID();
  std::pair<ULONG, USHORT> getIpAddressAndPort();

private:
  const SOCKET m_socket;
  cPtr<cMemoryView> m_pBuffer;
  cSocketServer *m_pSocketServer;
  ULONG IpAddress;
  USHORT Port;
};