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

// cSMBSession.h

#pragma once

#if CIFS 

#include "cOpLock.h"
#include "iTreeResourceFactory.h"
#include "iTreeChangeListener.h"

class cSMBTree;
class cSMBResponse;
class cTrans2Find;
class cTrans2FindFirst2;
class iOpLockManager;

class cSMBSession : public iTreeChangeListener, public vfs::cRefCount
{
public:
  cSMBSession(const vfs::String& sLoginName
    , const std::string &rANSIpw
    , const std::string rUNICODEpw
    , const vfs::cPtr<iOpLockManager> pOpLockManager);
  ~cSMBSession(void);

  //iTreeChangeListener
  void replaceTree(const vfs::String& sName
    , const vfs::cPtr<iTreeResourceFactory>& pTreeResourceFactory);

  //public member functions
  USHORT ConnectTree(const vfs::String& sShareName
    , const vfs::cPtr<iTreeResourceFactory> pTreeResourceFactory);
  bool DisconnectTree(const SHORT Tid);

  vfs::cPtr<cSMBResponse> processOpenAndX(PREQ_OPEN_ANDX pReq, PUCHAR pHeader, vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> processNTCreateAndX(PREQ_NT_CREATE_ANDX pReq, PUCHAR pHeader, vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> processNTTransSubCommand(const USHORT functionCode, const PUCHAR pSetupData
    , const UINT setupCount, const PUCHAR transParams, const PUCHAR transData, vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> processTransSubCommand(const PUCHAR pHeader
    , const PUCHAR pSetupData
    , const UINT setupCount
    , const PUCHAR transParams
    , const PUCHAR transData
    , const PUSHORT pRespBuffer
    , vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> processTrans2SubCommand(const USHORT maxDataCount, const PUCHAR trans2Block
    , const UINT setupCount, const PUCHAR trans2Params, const PUCHAR trans2Data, vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> processReadAndX(const PREQ_READ_ANDX pReq, vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> processWrite(const USHORT Fid, vfs::cConstPtr<vfs::cMemoryView> buffer
    , const LARGE_INTEGER &offset, DWORD &nBytes, vfs::cPtr<cSMBResponse> response);

  vfs::cPtr<cSMBResponse> Close(USHORT Fid, vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> Delete(const PREQ_DELETE pReq, vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> Rename(const PREQ_RENAME pReq, vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> Lock(const PREQ_LOCKING_ANDX pReq, vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> DeleteDirectory(const std::wstring ksPath, vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> CreateDirectory(const std::wstring ksPath, vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> ProcessExit(vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> SetInformation(const PREQ_SET_INFORMATION pReq, vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> SetInformation2(const PREQ_SET_INFORMATION2 pReq, vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> QueryInformation2(const PREQ_QUERY_INFORMATION2 pReq, vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> NTCancel(vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> CheckDirectory(const PREQ_CHECK_DIRECTORY pReq, vfs::cPtr<cSMBResponse> pResponse);

  USHORT getSearchHandleID(cTrans2FindFirst2 *pTrans2FindFirst2, const bool bEndOfSearch);

  void AddOplock(vfs::cPtr<cOpLock> pOpLock);
  void RemoveOplock(vfs::cPtr<cOpLock> pOpLock);

  cTrans2Find *getSearch(USHORT searchHandle);
  void closeSearch(USHORT searchHandle);

private:
  vfs::cPtr<cSMBTree> getTree(const USHORT Tid);
  vfs::cPtr<cSMBTree> getTree(vfs::cPtr<cSMBResponse> response);

private:
  const vfs::String m_sLoginName;
  std::string m_sANSIpw;
  std::string m_sUNICODEpw;
  const vfs::cPtr<iOpLockManager> m_pOpLockManager;

  typedef std::map<int, vfs::cPtr<cSMBTree> > tTreeMap; 
  vfs::cLock m_TreeAccess;
  tTreeMap m_treeMap;
  volatile LONG m_nTreeCount;

  vfs::cLock m_SearchHandleAccess;

  typedef std::map<int, cTrans2FindFirst2 *> tSearchHandleMap; 
  tSearchHandleMap m_searchHandleMap;
  USHORT m_nSearchHandles;
};


#endif // CIFS 