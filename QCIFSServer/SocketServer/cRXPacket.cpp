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

//cRXPacket.cpp

#include "StdAfx.h"

#include ".\crxpacket.h"
#include "cTXPacket.h"

cRXPacket::cRXPacket(cPtr<cMemoryView> pBuffer, const SOCKET sock, cSocketServer *pSocketServer, ULONG ipAddress, USHORT port)
: m_pBuffer(pBuffer)
, m_socket(sock)
, m_pSocketServer(pSocketServer)
, IpAddress(ipAddress)
, Port(port)
{
  //QTRACE((L"cRXPacket::cRXPacket()"));
}

cRXPacket::~cRXPacket(void)
{
  //QTRACE((L"cRXPacket::~cRXPacket()"));
}

cPtr<iTXPacket> cRXPacket::getResponsePacket()
{
  return new cTXPacket(m_socket, m_pSocketServer);
}


UINT_PTR cRXPacket::getSessionID()
{
  return m_socket;
}

std::pair<ULONG, USHORT> cRXPacket::getIpAddressAndPort()
{
  return std::pair<ULONG, USHORT>(IpAddress, Port); 
}
