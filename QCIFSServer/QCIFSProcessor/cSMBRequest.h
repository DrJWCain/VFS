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

//cSMBRequest.h

#pragma once

#if CIFS 

#include "cSMBBuffer.h"
#include "iResponseWriter.h"
#include "..\SocketServer\iPacket.h"

class cSMBResponse;
class cSMBSession;
class iShareManager;
class iOpLockManager;
class cQCIFSPacketProcessor;

class cSMBRequest : public cSMBBuffer, public cRefCount
{
public:
  cSMBRequest(const vfs::cPtr<iRXPacket> pRXPacket
    , const vfs::cPtr<iShareManager> pShareManager
    , const vfs::cPtr<iOpLockManager> pOpLockManager
    , cQCIFSPacketProcessor* pQCIFSPacketProcessor);
  ~cSMBRequest();

  //cSMBServer *getServer() const {return m_pServer;}
  vfs::cPtr<iRXPacket> getContextPacket() const {return m_pContextPacket;}

  void getResponses(vfs::cPtr<vfs::cMemoryView> pMem, tResponseList& responses, vfs::iActivity::Ptr activity);

  void log() const;

private:
  typedef tResponseList (cSMBRequest::* tProcessFuncPtr)(cPtr<cSMBResponse>) const;
  static tProcessFuncPtr Dispatch[UCHAR_MAX];

private:
  void nextBlock() const;
  inline tResponseList singleResponse(const vfs::cPtr<cSMBResponse> pResponse) const;
  inline tResponseList errorResponse(const vfs::cPtr<cSMBResponse> pResponse
    , const DWORD ntError) const;
  inline tResponseList errorResponse(const vfs::cPtr<cSMBResponse> pResponse
    , const UCHAR errorClass
    , const DWORD error) const;

  // command parsers
  tResponseList dispatchCommand(unsigned char command, cPtr<cSMBResponse> pResponse) const;
  tResponseList processAndX(cPtr<cSMBResponse> response) const;

  // command handlers
  tResponseList processUnhandled(cPtr<cSMBResponse> response) const;
  tResponseList processNegotiate(cPtr<cSMBResponse> response) const;
  tResponseList processSessionSetupAndX(cPtr<cSMBResponse> response) const;
  tResponseList processTreeConnectAndX(cPtr<cSMBResponse> response) const;
  tResponseList processTreeDisconnect(cPtr<cSMBResponse> response) const;
  tResponseList processOpenAndX(cPtr<cSMBResponse> response) const;
  tResponseList processEcho(cPtr<cSMBResponse> response) const;
  tResponseList processNTCreateAndX(cPtr<cSMBResponse> response) const;
  tResponseList processTransaction2(cPtr<cSMBResponse> response) const;
  tResponseList processLogoffAndX(cPtr<cSMBResponse> response) const;
  tResponseList processTransaction2Secondary(cPtr<cSMBResponse> response) const;
  tResponseList processClose(cPtr<cSMBResponse> response) const;
  tResponseList processNtTrans(cPtr<cSMBResponse> response) const;
  tResponseList processReadAndX(cPtr<cSMBResponse> response) const;
  tResponseList processWriteAndX(cPtr<cSMBResponse> response) const;
  tResponseList processRename(cPtr<cSMBResponse> response) const;
  tResponseList processDelete(cPtr<cSMBResponse> response) const;
  tResponseList processLockingAndX(cPtr<cSMBResponse> response) const;
  tResponseList processWrite(cPtr<cSMBResponse> response) const;
  tResponseList processDeleteDirectory(cPtr<cSMBResponse> response) const;
  tResponseList processFindClose2(cPtr<cSMBResponse> response) const;
  tResponseList processCreateDirectory(cPtr<cSMBResponse> response) const;
  tResponseList processProcessExit(cPtr<cSMBResponse> pResponse) const;
  tResponseList processFlush(cPtr<cSMBResponse> pResponse) const;
  tResponseList processSetInformation(cPtr<cSMBResponse> pResponse) const;
  tResponseList processSetInformation2(cPtr<cSMBResponse> pResponse) const;
  tResponseList processQueryInformation2(cPtr<cSMBResponse> pResponse) const;
  tResponseList processNTCancel(cPtr<cSMBResponse> pResponse) const;
  tResponseList processCheckDirectory(cPtr<cSMBResponse> pResponse) const;
  tResponseList processTransaction(cPtr<cSMBResponse> pResponse) const;

  // private member functions
  vfs::cPtr<cSMBSession> getSession(const SHORT Uid, vfs::cPtr<cSMBResponse> response) const;
  vfs::cPtr<cSMBResponse> handleNTSession(cPtr<cSMBResponse> response) const;
  vfs::cPtr<cSMBResponse> handleSession(cPtr<cSMBResponse> response) const;

private:
  const vfs::cPtr<iRXPacket> m_pContextPacket;
  const vfs::cPtr<iShareManager> m_pShareManager;
  const vfs::cPtr<iOpLockManager> m_pOpLockManager;
  cQCIFSPacketProcessor* m_pQCIFSPacketProcessor;
  unsigned int m_nDataLength;
};

#endif // CIFS 