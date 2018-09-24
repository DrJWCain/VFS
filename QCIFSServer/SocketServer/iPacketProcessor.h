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

//iPacketProcessor.h

#pragma once

class iRXPacket;
class iTXPacket;

class iPacketProcessor
{
public:
  virtual ~iPacketProcessor() {}

  virtual DWORD PacketSize(UINT_PTR SessionID, const vfs::cPtr<vfs::cMemoryView> pBuffer, const DWORD dwReceivedBytes) = 0;
  virtual void AddPacket(const vfs::cPtr<iRXPacket> pPacket) = 0;
  virtual vfs::cPtr<vfs::cMemoryView> GetRXBuffer(const unsigned int minLength) = 0;
  virtual void onConnectionEstablished(UINT_PTR SessionID, const SOCKADDR_IN &krSockAddrIn, const UINT_PTR associatedSessionID) = 0;
  virtual void onClientConnectionEstablished(UINT_PTR SessionID, const UINT_PTR associatedSessionID) = 0;
  virtual void onConnectionClosed(UINT_PTR SessionID) = 0;
  virtual void dumpBuffers() = 0;

  class unknown_protocol : public std::exception{};
};