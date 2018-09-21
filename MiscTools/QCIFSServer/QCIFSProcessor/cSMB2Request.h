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

//cSMB2Request.h

#pragma once

#include "cSMB2Buffer.h"
#include "iResponseWriter.h"
#include "..\SocketServer\iPacket.h"

class cSMB2Response;
class iShareManager;
class iOpLockManager;
class cSMB2Session;
class cSMB2Tree;
class iComposite;
class cQCIFSPacketProcessor;

class cSMB2Request : public cSMB2Buffer, public cRefCount
{
public:
  cSMB2Request(const vfs::cPtr<iRXPacket> pRXPacket
    , const vfs::cPtr<iShareManager> pShareManager
    , const vfs::cPtr<iOpLockManager> pOpLockManager
    , cQCIFSPacketProcessor* pQCIFSPacketProcessor);
  ~cSMB2Request();

  vfs::cPtr<iRXPacket> getContextPacket() const {return m_pContextPacket;}

  void getResponses(vfs::cPtr<vfs::cMemoryView> pMem, tResponseList& responses, vfs::iActivity::Ptr activity);

private:
  typedef tResponseList (cSMB2Request::* tProcessFuncPtr)(cPtr<cSMB2Response>) const;
  static tProcessFuncPtr Dispatch[UCHAR_MAX];

private:
  inline tResponseList singleResponse(const vfs::cPtr<cSMB2Response>& pResponse) const;
  inline tResponseList errorResponse(const vfs::cPtr<cSMB2Response>& pResponse) const;
  inline tResponseList errorResponse(const vfs::cPtr<cSMB2Response>& pResponse
    , const DWORD ntError
    , UCHAR errorData = 0) const;
  inline tResponseList errorResponse(const vfs::cPtr<cSMB2Response>& pResponse
    , const UCHAR errorClass
    , const DWORD error) const;

  // command parsers
  tResponseList dispatchCommand(USHORT command, cPtr<cSMB2Response> pResponse) const;
  tResponseList processChain(cPtr<cSMB2Response> pResponse, const ULONG curResponseLength) const;

  // command handlers
  tResponseList processUnhandled(cPtr<cSMB2Response> response) const;
  tResponseList processNegotiate(cPtr<cSMB2Response> response) const;
  tResponseList processNegotiate2(cPtr<cSMB2Response> response) const;
  tResponseList processNegotiate2Any(cPtr<cSMB2Response> response) const;
  tResponseList processSessionSetup(cPtr<cSMB2Response> response) const;
  tResponseList processSessionLogoff(cPtr<cSMB2Response> response) const;
  tResponseList processTreeConnect(cPtr<cSMB2Response> response) const;
  tResponseList processTreeDisconnect(cPtr<cSMB2Response> response) const;
  tResponseList processCreate(cPtr<cSMB2Response> response) const;
  tResponseList processClose(cPtr<cSMB2Response> response) const;
  tResponseList processFlush(cPtr<cSMB2Response> response) const;
  tResponseList processRead(cPtr<cSMB2Response> response) const;
  tResponseList processWrite(cPtr<cSMB2Response> response) const;
  tResponseList processLock(cPtr<cSMB2Response> response) const;
  tResponseList processIoctl(cPtr<cSMB2Response> response) const;
  tResponseList processCancel(cPtr<cSMB2Response> response) const;
  tResponseList processKeepAlive(cPtr<cSMB2Response> response) const;
  tResponseList processFind(cPtr<cSMB2Response> response) const;
  tResponseList processNotify(cPtr<cSMB2Response> response) const;
  tResponseList processGetInfo(cPtr<cSMB2Response> response) const;
  tResponseList processSetInfo(cPtr<cSMB2Response> response) const;
  tResponseList processBreak(cPtr<cSMB2Response> response) const;

  // private member functions
  vfs::cPtr<cSMB2Session> getSession(const vfs::cPtr<cSMB2Response>& response) const;
  vfs::cPtr<cSMB2Tree> getTree(const vfs::cPtr<cSMB2Response>& response) const;
  template <typename ReqType>
  vfs::cPtr<iComposite> getComposite(ReqType*& rpReq, const vfs::cPtr<cSMB2Response>& response) const;
  void check(cPtr<cSMB2Session> session) const; 

private:
  const vfs::cPtr<iRXPacket> m_pContextPacket;
  const vfs::cPtr<iShareManager> m_pShareManager;
  const vfs::cPtr<iOpLockManager> m_pOpLockManager;
  cQCIFSPacketProcessor* m_pQCIFSPacketProcessor;

  void saveCompoundFid(const ULONGLONG compoundFid) const
  {
    m_compoundFid = compoundFid;
  }

  template <typename T>
  inline ULONGLONG Fid(T t) const
  {
    return ((header()->Flags & SMB2_FLAGS_RELATED_OPERATIONS)
      && -1 == static_cast<LONGLONG>(t->FileId.Volatile))
      ? m_compoundFid
      : t->FileId.Volatile;
  }

  mutable ULONGLONG m_compoundFid;  

public:
};