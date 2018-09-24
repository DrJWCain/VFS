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

//cQCIFSPacketProcessor.h

#pragma once

#include "..\socketserver\ipacketprocessor.h"

class iIOCompletionPort;
class iShareManager;
class iOpLockManager;

class cQCIFSPacketProcessor : public iPacketProcessor, public vfs::cRefCount, public vfs::iMemoryObserver
{
public:
  cQCIFSPacketProcessor(const vfs::cPtr<iIOCompletionPort> pCompletionPort
    , const vfs::cPtr<iShareManager> pShareManager
    , const vfs::cPtr<iOpLockManager> pOpLockManager);
  ~cQCIFSPacketProcessor(void);

  DWORD PacketSize(UINT_PTR SessionID, const vfs::cPtr<vfs::cMemoryView> pBuffer, const DWORD dwReceivedBytes);
  void AddPacket(const vfs::cPtr<iRXPacket> pPacket);
  void onConnectionEstablished(UINT_PTR SessionID, const SOCKADDR_IN &krSockAddrIn, const UINT_PTR associatedSessionID) {}
  void onClientConnectionEstablished(UINT_PTR SessionID, const UINT_PTR associatedSessionID) {}
  void onConnectionClosed(UINT_PTR SessionID) {}

  void addCreateFileData(const unsigned __int64& sessionID, const vfs::String& filePath, const unsigned __int64& Fid);
  void dumpBuffers();

  vfs::cPtr<vfs::cMemoryView> GetRXBuffer(const unsigned int minLength);
  vfs::cPtr<vfs::cMemoryView> GetTXBufferSMB();
  vfs::cPtr<vfs::cMemoryView> GetTXBufferSMB2();

  //iMemoryObserver
  virtual void onRelease(void* ObjectPtr);


private:  //private member functions
  inline bool isSMBPacket(const unsigned char* const pBuffer);
  inline bool isSMB2Packet(const unsigned char* const pBuffer);
  inline bool isAcceptedSMBPacket(const unsigned char* const pBuffer);

private:  //private member variables
  const vfs::cPtr<iIOCompletionPort> m_pCompletionPort;
  const vfs::cPtr<iShareManager> m_pShareManager;
  const vfs::cPtr<iOpLockManager> m_pOpLockManager;

  std::list<void*> FreeAllocations;
  vfs::cLock FreeAllocationsLock;

  unsigned int NumPacketsToKeep;

  typedef std::pair<unsigned __int64, unsigned __int64> tCreateFileInfo;
  typedef std::pair<tCreateFileInfo, vfs::String> tCreateFilePath;
  std::list<tCreateFilePath> CreateFilePaths;
  vfs::cLock CreateFilePathsLock;

  unsigned int NumPathsToKeep;
};
