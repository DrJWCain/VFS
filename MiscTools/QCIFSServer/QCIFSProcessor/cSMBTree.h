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

//cSMTree.cpp

#pragma once

#if CIFS 

#include "mycifs.h"
#include "cCompositeMap.h"
#include "cOpLock.h"
#include "cNotificationID.h"

#include "..\iTreeResource.h"
#include "..\iComposite.h"

class cSMBResponse;
class cSMBSession;
class iOpLockManager;

class cSMBTree : public vfs::cRefCount
{
private:
  typedef vfs::cPtr<cSMBResponse> (cSMBTree::* tProcessTrans2FuncPtr)(const PUCHAR trans2Params
    , const PUCHAR trans2Data, vfs::cPtr<cSMBResponse> response) const;
  static tProcessTrans2FuncPtr Trans2Dispatch[UCHAR_MAX];

  typedef vfs::cPtr<cSMBResponse> (cSMBTree::* tProcessNTTransFuncPtr)(const PUCHAR pSetupData
    , const UINT setupCount, const PUCHAR transParams
    , const PUCHAR transData, vfs::cPtr<cSMBResponse> response) const;

public:
  cSMBTree(cSMBSession* const pSession
    , const vfs::cPtr<iTreeResource> pTreeResource
    , const vfs::cPtr<iOpLockManager> pOpLockManager
    , const vfs::String& sShareName);
  ~cSMBTree(void);

  vfs::cPtr<cSMBResponse> Close(USHORT Fid, vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> Read(const PREQ_READ_ANDX pReq, vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> Write(const USHORT Fid, vfs::cConstPtr<vfs::cMemoryView> buffer
    , const LARGE_INTEGER &offset, DWORD &nBytes, vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> Delete(const PREQ_DELETE pReq, vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> Rename(const PREQ_RENAME pReq, vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> Lock(const PREQ_LOCKING_ANDX pReq, vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> DeleteDirectory(const std::wstring ksPath, vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> CreateDirectory(const std::wstring ksPath, vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> ProcessExit(const vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> SetInformation(const PREQ_SET_INFORMATION pReq, vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> SetInformation2(const PREQ_SET_INFORMATION2 pReq, vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> QueryInformation2(const PREQ_QUERY_INFORMATION2 pReq, vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> NTCancel(const vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> CheckDirectory(const PREQ_CHECK_DIRECTORY pReq, vfs::cPtr<cSMBResponse> pResponse);

  // processCreateAndX dispatcher
  vfs::cPtr<cSMBResponse> processOpenAndX(const PREQ_OPEN_ANDX pReq
    , const PUCHAR pHeader
    , const vfs::cPtr<cSMBResponse> response) const;
  vfs::cPtr<cSMBResponse> processNTCreateAndX(PREQ_NT_CREATE_ANDX pReq, PUCHAR pHeader, vfs::cPtr<cSMBResponse> response) const;
  // transaction2 dispatcher
  vfs::cPtr<cSMBResponse> processTrans2SubCommand(const USHORT maxDataCount
    , const PUCHAR trans2Block
    , const UINT setupCount
    , const PUCHAR trans2Params
    , const PUCHAR trans2Data
    , vfs::cPtr<cSMBResponse> response) const;
  vfs::cPtr<cSMBResponse> processNTTransSubCommand(const USHORT functionCode
    , const PUCHAR trans2Block
    , const UINT setupCount
    , const PUCHAR trans2Params
    , const PUCHAR trans2Data
    , vfs::cPtr<cSMBResponse> response) const;
  vfs::cPtr<cSMBResponse> processTransSubCommand(const PUCHAR pHeader
    , const PUCHAR trans2Block
    , const UINT setupCount
    , const PUCHAR trans2Params
    , const PUCHAR trans2Data
    , const PUSHORT pRespBuffer
    , vfs::cPtr<cSMBResponse> response) const;

  void AddOplock(const vfs::cPtr<cOpLock> pOpLock);
  void RemoveOplock(const vfs::cPtr<cOpLock> pOpLock);

  bool NameIs(const vfs::String& sName) const {return m_sShareName == sName;}
  void notifyRemoved(const vfs::cPtr<cSMBTree>& pNewTree);

private:  // private functions
  void ReleaseOpLock(const vfs::cPtr<cOpLock> pOpLock);
  inline int getSessionID(PSMB_HEADER const pSMBHeader) const;
  inline cNotificationID getRequestID(PSMB_HEADER const pSMBHeader) const;
  inline vfs::String getName(const PWCHAR pName, const USHORT flags2) const;
  USHORT getChainedCreateFid(const vfs::cConstPtr<cSMBResponse> pSMBResponse) const;

  // generic unhandled response
  vfs::cPtr<cSMBResponse> processUnhandled(const PUCHAR trans2Params
    , const PUCHAR trans2Data, vfs::cPtr<cSMBResponse> response) const;

  vfs::cPtr<cSMBResponse> processUnhandled(const PUCHAR pSetupData
    , const UINT setupCount, const PUCHAR transParams
    , const PUCHAR transData, vfs::cPtr<cSMBResponse> response) const;

  // NTTransaction functions
  vfs::cPtr<cSMBResponse> NTTransIOCTL(const PUCHAR pSetupData
    , const UINT setupCount, const PUCHAR transParams
    , const PUCHAR transData, vfs::cPtr<cSMBResponse> response) const;
  vfs::cPtr<cSMBResponse> NTNotify(const PUCHAR pSetupData
    , const UINT setupCount, const PUCHAR transParams
    , const PUCHAR transData, vfs::cPtr<cSMBResponse> response) const;

  // Trans2 functions
  vfs::cPtr<cSMBResponse> trans2FindFirst2(const PUCHAR trans2Params
    , const PUCHAR trans2Data, vfs::cPtr<cSMBResponse> response) const;
  vfs::cPtr<cSMBResponse> trans2FindNext2(const PUCHAR trans2Params
    , const PUCHAR trans2Data, vfs::cPtr<cSMBResponse> response) const;
  vfs::cPtr<cSMBResponse> trans2QueryPathInfo(const PUCHAR trans2Params
    , const PUCHAR trans2Data, vfs::cPtr<cSMBResponse> response) const;
  vfs::cPtr<cSMBResponse> trans2QueryFSInfo(const PUCHAR trans2Params
    , const PUCHAR trans2Data, vfs::cPtr<cSMBResponse> response) const;
  vfs::cPtr<cSMBResponse> trans2QueryFileInfo(const PUCHAR trans2Params
    , const PUCHAR trans2Data, vfs::cPtr<cSMBResponse> response) const;
  vfs::cPtr<cSMBResponse> trans2SetFileInfo(const PUCHAR trans2Params
    , const PUCHAR trans2Data, vfs::cPtr<cSMBResponse> response) const;
  vfs::cPtr<cSMBResponse> trans2GetDFSReferral(const PUCHAR trans2Params
    , const PUCHAR trans2Data, vfs::cPtr<cSMBResponse> response) const;

  //Trans functions
  vfs::cPtr<cSMBResponse> processTransactNamedPipe(const PUCHAR pHeader
    , const USHORT Fid
    , const PUCHAR pData
    , const PUCHAR pTransData
    , const PUSHORT pRespBuffer
    , vfs::cPtr<cSMBResponse> response) const;

private:  // private member variables
  cSMBSession* const m_pSession;
  const vfs::cPtr<iTreeResource> m_pTreeResource;
  const vfs::cPtr<iOpLockManager> m_pOpLockManager;
  const vfs::String m_sShareName;
  mutable USHORT m_maxDataCount;

  typedef cCompositeMap<USHORT, ULONGLONG> tCompositeMap;
  typedef std::vector<tCompositeMap::tFidPathPair> tFidPathPairList;
  mutable tCompositeMap m_compositeMap;

  vfs::cLock m_opLockListAccess;
  std::list<vfs::cPtr<cOpLock> > m_OpLocks;

  static const DWORD m_kCreationAttsMap[6];
};

#endif // CIFS 