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

//cSMBTree.cpp
// class holds state of an SMB tree connection
//
// created 14-5-05
//
// References:
// [#1] SNIA CIFS Technical Reference Revision 1.0


#include "stdafx.h"

#if CIFS 

#include ".\csmbtree.h"

#include "mycifs.h"

#include "..\iComposite.h"
#include "cSMBResponse.h"
#include "cSMBSession.h"
#include "Trans2Structs.h"
#include "cTrans2QueryFSInfo.h"
#include "cTrans2FindFirst2.h"
#include "cTrans2FindNext2.h"
#include "cTrans2QueryPathInfo.h"
#include "cTrans2QueryFileInfo.h"
#include "cTrans2GetDFSReferral.h"
#include "cSetFileInfo.h"
#include "cNTTransIOCTL.h"
#include "smb_routines.h"
#include "cNotificationReceiver.h"
#include "iOpLockManager.h"
#include "align.h"
#include "srvsvc.h"
#include "ms_rpc.h"
#include "iUniqueNotifyID.h"
#include "cGuardedUniqueNotify.h"

#include "..\iTreeResource.h"
#include "..\QCIFSIPC\iQCIFSIPC.h"
#include "..\SocketServer\iPacket.h"

#include "..\..\KernelSDK\include\QWinBase.h"

cSMBTree::tProcessTrans2FuncPtr cSMBTree::Trans2Dispatch[UCHAR_MAX] = {0};

const DWORD cSMBTree::m_kCreationAttsMap[6] = { CREATE_ALWAYS
                                        , OPEN_EXISTING
                                        , CREATE_NEW
                                        , OPEN_ALWAYS
                                        , OPEN_EXISTING
                                        , OPEN_ALWAYS}; // [#1] p.65

extern tActivityMap ActivityQCIFSProcessors;

cSMBTree::cSMBTree(cSMBSession* const pSession
  , const cPtr<iTreeResource> pTreeResource
  , const cPtr<iOpLockManager> pOpLockManager
  , const String& sShareName)
: m_pSession(pSession)
, m_pTreeResource(pTreeResource)
, m_pOpLockManager(pOpLockManager)
, m_sShareName(sShareName)
, m_maxDataCount(0)
{
}

cSMBTree::~cSMBTree(void)
{
  for(std::list<cPtr<cOpLock> >::iterator it = m_OpLocks.begin()
    ; it != m_OpLocks.end()
    ; ++it)
  {
    ReleaseOpLock(*it);
  }
}

static DWORD getAccessMode(USHORT access)
{
  UINT32 flags = 0;
  switch (access & SMB_DA_ACCESS_MASK) 
  {
  case SMB_DA_ACCESS_READ:
      flags |= GENERIC_READ;
      break;
  case SMB_DA_ACCESS_EXECUTE:
      flags |= GENERIC_EXECUTE;
      break;
  case SMB_DA_ACCESS_WRITE:
      flags |= GENERIC_WRITE;
      break;
  case SMB_DA_ACCESS_READ_WRITE:
      flags |= GENERIC_READ | GENERIC_WRITE;
      break;
  }
  return flags;
}

static DWORD getShareMode(USHORT access)
{
  UINT32 flags = 0;
  switch (access & SMB_DA_SHARE_MASK) 
  {
  case SMB_DA_SHARE_COMPATIBILITY:
  case SMB_DA_SHARE_DENY_NONE:
      flags |= FILE_SHARE_READ | FILE_SHARE_WRITE;
      break;
  case SMB_DA_SHARE_DENY_WRITE:
      flags |= FILE_SHARE_READ;
      break;
  case SMB_DA_SHARE_DENY_READ:
      flags |= FILE_SHARE_WRITE;
      break;
  case SMB_DA_SHARE_EXCLUSIVE:
  default:
      break;
  }
  return flags;
}
static DWORD getCreationDisposition(USHORT openfunc)
{
    switch (openfunc & (SMB_OFUN_OPEN_MASK | SMB_OFUN_CREATE_MASK)) {
    case (SMB_OFUN_CREATE_FAIL   | SMB_OFUN_OPEN_FAIL):
        return 0;
    case (SMB_OFUN_CREATE_FAIL   | SMB_OFUN_OPEN_OPEN):
        return OPEN_EXISTING;
    case (SMB_OFUN_CREATE_FAIL   | SMB_OFUN_OPEN_TRUNCATE):
        return TRUNCATE_EXISTING;
    case (SMB_OFUN_CREATE_CREATE | SMB_OFUN_OPEN_FAIL):
        return CREATE_NEW;
    case (SMB_OFUN_CREATE_CREATE | SMB_OFUN_OPEN_OPEN):
        return OPEN_ALWAYS;
    case (SMB_OFUN_CREATE_CREATE | SMB_OFUN_OPEN_TRUNCATE):
        return CREATE_ALWAYS;
    default:
        return 0;
    }
}

static ULONG toSeconds(const FILETIME& ft)
{
  ULARGE_INTEGER ulg;
  ulg.HighPart = ft.dwHighDateTime;
  ulg.LowPart = ft.dwLowDateTime;
  return static_cast<ULONG>(ulg.QuadPart / 10000000);  //'FILETIME' in 100 nano-second intervals
}

cPtr<cSMBResponse> cSMBTree::processOpenAndX(const PREQ_OPEN_ANDX pReq
  , const PUCHAR pHeader
  , const cPtr<cSMBResponse> response) const
{
  //const bool additional_info = (pReq->Flags & SMB_OPEN_QUERY_INFORMATION);

  const UCHAR wordAlign = static_cast<UCHAR>((reinterpret_cast<PUCHAR>(&(pReq->Buffer)) - pHeader) % sizeof(USHORT));
  const String sPath(getName(reinterpret_cast<PWCHAR>(pReq->Buffer + wordAlign), response->Header->Flags2));

  BY_HANDLE_FILE_INFORMATION _bHFI;
  PBY_HANDLE_FILE_INFORMATION const pBHFI = &_bHFI;
  SECURITY_ATTRIBUTES _sa;
  DWORD retCode;

  const cPtr<iComposite> pComposite(m_pTreeResource->NTCreateFile(sPath
    , getAccessMode(pReq->DesiredAccess)
    , getShareMode(pReq->DesiredAccess)
    , &_sa
    , getCreationDisposition(pReq->OpenFunction)
    , pReq->FileAttributes// | ((pReq->CreateOptions & 1) ? FILE_FLAG_BACKUP_SEMANTICS : 0)
    , NULL
    , pBHFI
    , retCode));

  if (retCode == ERROR_SUCCESS && pComposite.isValid())
  {
    PRESP_OPEN_ANDX const pResp = (PRESP_OPEN_ANDX const)response->allocParams(sizeof(RESP_OPEN_ANDX));
    if ((pResp->Fid = m_compositeMap.add(pComposite, sPath)) != 0)
    {
      pResp->FileAttributes = static_cast<USHORT>(_bHFI.dwFileAttributes);
      pResp->LastWriteTimeInSeconds = toSeconds(_bHFI.ftLastWriteTime);
      pResp->DataSize = _bHFI.nFileSizeLow;
      pResp->GrantedAccess = pReq->DesiredAccess;
      pResp->FileType = 0;
      pResp->DeviceState = 0;
      pResp->Action = 0;
      pResp->ServerFid = pResp->Fid;
      pResp->Reserved = 0;
      pResp->Buffer[0] = 0;
    }
    else
      response->setDosError(SMB_ERR_CLASS_SERVER, SMB_ERR_SERVER_ERROR);
  }
  else
    response->setDosError(SMB_ERR_CLASS_DOS, retCode);

  return response;
}

#define FILE_ATTRIBUTE_NAME_PIPE 0x10000000
cPtr<cSMBResponse> cSMBTree::processNTCreateAndX(PREQ_NT_CREATE_ANDX pReq
  , PUCHAR pHeader
  , cPtr<cSMBResponse> response) const
{
  const UCHAR wordAlign = static_cast<UCHAR>((reinterpret_cast<PUCHAR>(&(pReq->Buffer)) - pHeader) % sizeof(USHORT));
  const String sPath(getName(reinterpret_cast<PWCHAR>(pReq->Buffer + wordAlign), response->Header->Flags2));

  BY_HANDLE_FILE_INFORMATION _bHFI;
  PBY_HANDLE_FILE_INFORMATION pBHFI = &_bHFI;
  SECURITY_ATTRIBUTES _sa;
  DWORD retCode;

  //QTRACE((L"processNTCreateAndX() - %s", sPath.c_str()));
  
  DWORD createDisposition =
    (pReq->CreateDisposition < sizeof(m_kCreationAttsMap)/sizeof(m_kCreationAttsMap[0]))
    ? m_kCreationAttsMap[pReq->CreateDisposition] // [#1] p.65
    : 0;

  cPtr<iComposite> pComposite;
  if ((pReq->CreateOptions & 1) && (createDisposition == CREATE_NEW))
  {
    pComposite = m_pTreeResource->NTCreateFolder(sPath
      , &_sa
      , pBHFI
      , retCode);
  }
  else
  {
    pComposite = m_pTreeResource->NTCreateFile(sPath
      , pReq->DesiredAccess 
      , pReq->ShareAccess
      , &_sa
      , createDisposition
      , pReq->FileAttributes | ((pReq->CreateOptions & 1) ? FILE_FLAG_BACKUP_SEMANTICS : 0)
      , NULL
      , pBHFI
      , retCode);
  }

  if (retCode == ERROR_SUCCESS && pComposite.isValid())
  {
    if(FILE_ATTRIBUTE_NAME_PIPE & pBHFI->dwFileAttributes)//JCJCJC SNIA CIFS PLugfest 2009 - test code.
    {
      pBHFI->dwFileAttributes &= ~FILE_ATTRIBUTE_NAME_PIPE;

      PRESP_NT_CREATE_ANDX pResp = (PRESP_NT_CREATE_ANDX)response->allocParams(sizeof(RESP_NT_CREATE_ANDX) + 16);
      if ((pResp->Fid = m_compositeMap.add(pComposite, sPath)) != 0)
      {
        //QTRACE((L"processNTCreateAndX() - %s - Fid[%d]", sPath.c_str(), pResp->Fid));
        //pResp->OplockLevel = 0;//SMB_OPLOCK_LEVEL_BATCH;
        //pResp->CreateAction = pReq->CreateDisposition;
        //pResp->CreationTime.QuadPart = 0;//makeLg(_bHFI.ftCreationTime);
        //pResp->LastAccessTime.QuadPart = 0;//makeLg(_bHFI.ftLastAccessTime);
        //pResp->LastWriteTime.QuadPart = 0;//makeLg(_bHFI.ftLastWriteTime);
        //pResp->ChangeTime.QuadPart = 0;//makeLg(_bHFI.ftLastWriteTime);
        //pResp->FileAttributes = _bHFI.dwFileAttributes;
        //pResp->AllocationSize.QuadPart = 4096;//sizeOnDisk.getSizeOnDisk(_bHFI.nFileSizeHigh, _bHFI.nFileSizeLow);
        //pResp->EndOfFile.QuadPart = makeLg(_bHFI.nFileSizeHigh, _bHFI.nFileSizeLow);
        //pResp->FileType =  0x0002;// for message oriented pipes
        //pResp->DeviceState = 0x5ff; // for pipes
        //pResp->Directory = 0;
        //pResp->ByteCount = 0;
        //
        //static UCHAR payload[] = {
        //  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        //  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        //  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9b, 0x01, 
        //  0x12, 0x00, 0x9b, 0x01, 0x12, 0x00, 0x00, 0x00};
        //memcpy(pResp->Buffer, payload, 32);
        //response->setDataOffset(pResp->Buffer+16);
        //response->setDataCount(16);
        //response->addExtraData(16);
      }
      else
        response->setDosError(SMB_ERR_CLASS_SERVER, SMB_ERR_SERVER_ERROR);
    }
    else
    {
      PRESP_NT_CREATE_ANDX pResp = (PRESP_NT_CREATE_ANDX)response->allocParams(sizeof(RESP_NT_CREATE_ANDX));
      if ((pResp->Fid = m_compositeMap.add(pComposite, sPath)) != 0)
      {
        //QTRACE((L"processNTCreateAndX() - %s - Fid[%d]", sPath.c_str(), pResp->Fid));
        pResp->OplockLevel = SMB_OPLOCK_LEVEL_BATCH;
        pResp->CreateAction = pReq->CreateDisposition;
        pResp->CreationTime.QuadPart = makeLg(_bHFI.ftCreationTime);
        pResp->LastAccessTime.QuadPart = makeLg(_bHFI.ftLastAccessTime);
        pResp->LastWriteTime.QuadPart = makeLg(_bHFI.ftLastWriteTime);
        pResp->ChangeTime.QuadPart = makeLg(_bHFI.ftLastWriteTime);
        pResp->FileAttributes = _bHFI.dwFileAttributes;
        cSizeOnDisk sizeOnDisk(m_pTreeResource);
        pResp->AllocationSize = sizeOnDisk.getSizeOnDisk(_bHFI.nFileSizeHigh, _bHFI.nFileSizeLow);
        pResp->EndOfFile.QuadPart = makeLg(_bHFI.nFileSizeHigh, _bHFI.nFileSizeLow);
        pResp->FileType = FileTypeDisk;
        pResp->FileStatusFlags = SMB_FSF_NO_EAS | SMB_FSF_NO_SUBSTREAMS | SMB_FSF_NO_REPARSETAG; //JCJCJC This was "pResp->DeviceState = 7; // from Ethereal". Rob had this right, but the semantics are clearer with the bitamsks
        pResp->Directory = ((_bHFI.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
        pResp->ByteCount = 0;
      }
      else
        response->setDosError(SMB_ERR_CLASS_SERVER, SMB_ERR_SERVER_ERROR);
    }
  }
  else
  {
    response->setDosError(SMB_ERR_CLASS_DOS, retCode);
  }
  
  return response;
}

cPtr<cSMBResponse> cSMBTree::processNTTransSubCommand(const USHORT functionCode, const PUCHAR pSetupData
  , const UINT setupCount, const PUCHAR transParams, const PUCHAR transData, cPtr<cSMBResponse> response) const
{
  switch (functionCode)
  {
  case NT_TRANSACT_IOCTL:
    return NTTransIOCTL(pSetupData, setupCount, transParams, transData, response);
    break;
  case NT_TRANSACT_NOTIFY_CHANGE:
    return NTNotify(pSetupData, setupCount, transParams, transData, response);
    break;
  case NT_TRANSACT_QUERY_SECURITY_DESC://JCJCJC SNIA2009 Queried when listing tiff files
  default:
    break;
  }
  return processUnhandled(pSetupData, setupCount, transParams, transData, response);
}

cPtr<cSMBResponse> cSMBTree::processTransSubCommand(const PUCHAR pHeader
  , const PUCHAR pSetupData
  , const UINT setupCount
  , const PUCHAR transParams
  , const PUCHAR transData
  , const PUSHORT pRespBuffer
  , cPtr<cSMBResponse> response) const
{
  const PUSHORT pSubCommand = reinterpret_cast<PUSHORT>(pSetupData);
  switch (pSubCommand[0])
  {
  case TRANS_TRANSACT_NMPIPE:
    return processTransactNamedPipe(pHeader
      , pSubCommand[1]
      , reinterpret_cast<PUCHAR>(pSubCommand+2)
        , transData
        , pRespBuffer
        , response);
    break;
  default:
    break;
  }
  return processUnhandled(pSetupData, setupCount, transParams, transData, response);
}

// trans2 block dispatcher
cPtr<cSMBResponse> cSMBTree::processTrans2SubCommand(const USHORT maxDataCount, const PUCHAR trans2Block
  , const UINT setupCount, const PUCHAR trans2Params, const PUCHAR trans2Data, cPtr<cSMBResponse> response) const
{
  m_maxDataCount = maxDataCount;
  if(!Trans2Dispatch[0])
  {
    for(int i = 0; i < UCHAR_MAX; ++i)
    {
      Trans2Dispatch[i] = &cSMBTree::processUnhandled;
    }
    Trans2Dispatch[TRANS2_FIND_FIRST2] = &cSMBTree::trans2FindFirst2;
    Trans2Dispatch[TRANS2_FIND_NEXT2] = &cSMBTree::trans2FindNext2;
    Trans2Dispatch[TRANS2_QUERY_FS_INFORMATION] = &cSMBTree::trans2QueryFSInfo;
    Trans2Dispatch[TRANS2_QUERY_PATH_INFORMATION] = &cSMBTree::trans2QueryPathInfo;
    Trans2Dispatch[TRANS2_QUERY_FILE_INFORMATION] = &cSMBTree::trans2QueryFileInfo;
    Trans2Dispatch[TRANS2_SET_FILE_INFORMATION] = &cSMBTree::trans2SetFileInfo;
    Trans2Dispatch[TRANS2_GET_DFS_REFERRAL] = &cSMBTree::trans2GetDFSReferral;
  }
  return (this->*Trans2Dispatch[(USHORT)(*trans2Block)])(trans2Params, trans2Data, response);
}

// generic unhandled response
cPtr<cSMBResponse> cSMBTree::processUnhandled(const PUCHAR trans2Params
  , const PUCHAR trans2Data, cPtr<cSMBResponse> response) const
{
  QSOS((L"Unhandled cSMBTree request %d", response->Header->Command));

  response->setDosError(SMB_ERR_CLASS_DOS, SMB_ERR_BAD_FUNCTION);
  return response;
}

cPtr<cSMBResponse> cSMBTree::processUnhandled(const PUCHAR pSetupData
  , const UINT setupCount, const PUCHAR transParams
  , const PUCHAR transData, cPtr<cSMBResponse> response) const
{
  return processUnhandled(transParams, transData, response);
}

cPtr<cSMBResponse> cSMBTree::NTTransIOCTL(const PUCHAR pSetupData
  , const UINT setupCount, const PUCHAR transParams
  , const PUCHAR transData, cPtr<cSMBResponse> response) const
{
  cNTTransIOCTL ioCtl(m_pTreeResource, reinterpret_cast<PREQ_NT_IO_CONTROL>(pSetupData), response);
  return response;
}

cPtr<cSMBResponse> cSMBTree::NTNotify(const PUCHAR pSetupData
  , const UINT setupCount, const PUCHAR transParams
  , const PUCHAR transData, cPtr<cSMBResponse> response) const
{
  PREQ_NOTIFY_CHANGE pReq = reinterpret_cast<PREQ_NOTIFY_CHANGE>(pSetupData);
  cPtr<iComposite> pComposite = m_compositeMap.getComposite(pReq->Fid, response);
  if (pComposite.isNull())
    return response;

  const cPtr<cGuardedUniqueNotify> pGuard
    = iUniqueNotifyID::singleton().getUniqueNotifyID(getRequestID(response->Header));

  DWORD dwRet = pComposite->registerNotification(pGuard->getUniqueId()
    , new cNotificationReceiver(response, pReq->WatchTree, pReq->CompletionFilter, pGuard));
  if (dwRet != ERROR_SUCCESS)
  {
    response->setNTError(dwRet);
    return response;
  }

  return cPtr<cSMBResponse>();  //null return response will delay response until notification occurs
}

// find first 2 command dispatcher
//[#1] p.99
cPtr<cSMBResponse> cSMBTree::trans2FindFirst2(const PUCHAR trans2Params
  , const PUCHAR trans2Data, cPtr<cSMBResponse> response) const
{
  cTrans2FindFirst2 *pFindFirstFirst2 = new cTrans2FindFirst2(m_pTreeResource, m_maxDataCount
    , m_pSession, reinterpret_cast<PREQ_FIND_FIRST2>(trans2Params), response);

  if (pFindFirstFirst2->EndOfSearch())
    delete(pFindFirstFirst2);

  return response;
}

// find next 2 command dispatcher
//[#1] p.105
cPtr<cSMBResponse> cSMBTree::trans2FindNext2(const PUCHAR trans2Params
  , const PUCHAR trans2Data, cPtr<cSMBResponse> response) const
{
  PREQ_FIND_NEXT2 pReq = reinterpret_cast<PREQ_FIND_NEXT2>(trans2Params);
  cTrans2Find *pFind = m_pSession->getSearch(pReq->Sid);

  if (pFind != NULL)
  {
    cTrans2FindNext2 findNext2(pFind, m_pTreeResource, pReq, response);

    if (pFind->EndOfSearch())
    {
      m_pSession->closeSearch(pReq->Sid);
    }
  }
  else
    response->setDosError(SMB_ERR_CLASS_DOS, SMB_ERR_BAD_MEMORY);

  return response;
}

// query fs command dispatcher
cPtr<cSMBResponse> cSMBTree::trans2QueryFSInfo(const PUCHAR trans2Params
  , const PUCHAR trans2Data, cPtr<cSMBResponse> response) const
{
  cTrans2QueryFsInfo queryFSInfo(m_pTreeResource,
    reinterpret_cast<PREQ_QUERY_FS_INFORMATION>(trans2Params), response);
  return response;
}

// query path command dispatcher
cPtr<cSMBResponse> cSMBTree::trans2QueryPathInfo(const PUCHAR trans2Params
  , const PUCHAR trans2Data, cPtr<cSMBResponse> response) const
{
  cTrans2QueryPathInfo queryPathInfo(m_pTreeResource,
    reinterpret_cast<PREQ_QUERY_PATH_INFORMATION>(trans2Params), response);
  return response;
}

cPtr<cSMBResponse> cSMBTree::trans2QueryFileInfo(const PUCHAR trans2Params
  , const PUCHAR trans2Data, cPtr<cSMBResponse> response) const
{
  PREQ_QUERY_FILE_INFORMATION pReq = reinterpret_cast<PREQ_QUERY_FILE_INFORMATION>(trans2Params);
  cPtr<iComposite> pComposite = m_compositeMap.getComposite(pReq->Fid, response);

  if (pComposite.isValid())
    cTrans2QueryFileInfo queryFileInfo(m_pTreeResource, pComposite, pReq, response);

  return response;
}

cPtr<cSMBResponse> cSMBTree::trans2SetFileInfo(const PUCHAR trans2Params
  , const PUCHAR trans2Data, cPtr<cSMBResponse> response) const
{
  PREQ_SET_FILE_INFORMATION pReq = reinterpret_cast<PREQ_SET_FILE_INFORMATION>(trans2Params);
  cPtr<iComposite> pComposite = m_compositeMap.getComposite(pReq->Fid, response);

  if (pComposite.isValid())
    cSetFileInfo setFileInfo(pComposite, pReq, trans2Data, response);

  return response;
}

cPtr<cSMBResponse> cSMBTree::trans2GetDFSReferral(const PUCHAR trans2Params
  , const PUCHAR trans2Data, cPtr<cSMBResponse> response) const
{
  const PREQ_GET_DFS_REFERRAL pReq = reinterpret_cast<PREQ_GET_DFS_REFERRAL>(trans2Params);
  cTrans2GetDFSReferral(m_pTreeResource, pReq, response);

  return response;
}

cPtr<cSMBResponse> cSMBTree::Close(USHORT Fid, cPtr<cSMBResponse> response)
{
  cPtr<iComposite> pComposite;
  {
    cActivityScope as1(ActivityQCIFSProcessors.find(0)->second,kTreeClose);
    pComposite = m_compositeMap.getComposite(Fid, response);
  }
  if (pComposite.isValid())
  {
    DWORD dwRet;
    {
      cActivityScope as1(ActivityQCIFSProcessors.find(0)->second,kTreeCloseCallComposite);
      dwRet = pComposite->Close(Fid);
    }
    if (dwRet != ERROR_SUCCESS)
    {
      QSOS((L"Failed to close Fid [%d] - error code: %s."
        , Fid
        , win32ErrorToString(dwRet).c_str()));

      response->setDosError(SMB_ERR_CLASS_DOS, dwRet);
    }
    else
    {
      cActivityScope as1(ActivityQCIFSProcessors.find(0)->second,kTreeCloseRemove);
      m_compositeMap.remove(Fid);
    }
  }
  return response;
}

cPtr<cSMBResponse> cSMBTree::Read(const PREQ_READ_ANDX pReq, cPtr<cSMBResponse> response)
{
  cActivityScope as1(ActivityQCIFSProcessors.find(0)->second,kTreeRead);
  const USHORT Fid = (pReq->Fid == 0) ? getChainedCreateFid(response) : pReq->Fid;
  cPtr<iComposite> pComposite = m_compositeMap.getComposite(Fid, response);
  if (pComposite.isValid())
  {
    LARGE_INTEGER nOffset;
    nOffset.LowPart = pReq->Offset;
    if (12 != pReq->WordCount)
      nOffset.HighPart = 0;
    else
    {
      // [MS-SMB v20080207] p.182
      // Section 4.2.4 http://www.microsoft.com/about/legal/intellectualproperty/protocols/BSTD/CIFS/draft-leach-cifs-v1-spec-02.txt
      const PREQ_NT_READ_ANDX pNTReq = reinterpret_cast<PREQ_NT_READ_ANDX>(pReq);
      nOffset.HighPart = pNTReq->OffsetHigh;
    }
    PUCHAR pBuffer = response->getData(sizeof(USHORT));

    cActivityScope as2(ActivityQCIFSProcessors.find(1)->second,kCompositeRead);
    DWORD nBytes(max(pReq->MinCount, min(pReq->MaxCount, gkBufferSize)));
    DWORD nRet = pComposite->Read(response->getTransmitList(), nBytes, nOffset, getSessionID(response->Header), Fid);

    if (nRet == ERROR_SUCCESS)
    {
      response->setDataOffset(pBuffer);
      response->setDataCount(nBytes);
      response->addData(static_cast<USHORT>(nBytes));
    }
    else
    {
      response->setDosError(SMB_ERR_CLASS_DOS, nRet);
    }
  }

  return response;
}

cPtr<cSMBResponse> cSMBTree::Write(const USHORT Fid, cConstPtr<cMemoryView> buffer
  , const LARGE_INTEGER &offset, DWORD &nBytes, cPtr<cSMBResponse> response)
{
  cPtr<iComposite> pComposite = m_compositeMap.getComposite(Fid, response);
  if (pComposite.isValid())
  {
    DWORD dwRet = pComposite->Write(buffer, nBytes, offset, getSessionID(response->Header), Fid);
    if (dwRet != ERROR_SUCCESS)
      response->setDosError(SMB_ERR_CLASS_DOS, dwRet);
  }

  return response;
}

cPtr<cSMBResponse> cSMBTree::Delete(const PREQ_DELETE pReq, cPtr<cSMBResponse> response) //[#1] p.77
{
  PUCHAR pData = pReq->Buffer;
  pData++;
  std::wstring filename(reinterpret_cast<PWCHAR>(pData));
  DWORD dwRet = m_pTreeResource->Delete(pReq->SearchAttributes, filename);
  if (dwRet != ERROR_SUCCESS)
  {
    response->setDosError(SMB_ERR_CLASS_DOS, dwRet);
  }

  return response;
}

cPtr<cSMBResponse> cSMBTree::Rename(const PREQ_RENAME pReq, cPtr<cSMBResponse> response) //[#1] p.78
{
  PUCHAR pData = pReq->Buffer;
  pData++;

  const String sFullName(reinterpret_cast<PWCHAR>(pData));
  const String sOriginalName(getName(reinterpret_cast<PWCHAR>(pData), response->Header->Flags2));

  pData += sFullName.length()*sizeof(WCHAR) + sizeof(WCHAR) + sizeof(UCHAR); // add terminator and buffer format
  pData += static_cast<UCHAR>((reinterpret_cast<PUCHAR>(pReq) - pData) % sizeof(USHORT)); // word align
  const String sNewName(getName(reinterpret_cast<PWCHAR>(pData), response->Header->Flags2));

  //QTRACE((L"cSMBTree::Rename() - original name (%s), new name (%s)", sOriginalName.c_str(), sNewName.c_str()));

  const DWORD dwRet = m_pTreeResource->Rename(pReq->SearchAttributes, sOriginalName, sNewName);
  if (ERROR_SUCCESS != dwRet)
  {
    response->setDosError(SMB_ERR_CLASS_DOS, dwRet);
  }

  return response;
}

cPtr<cSMBResponse> cSMBTree::Lock(const PREQ_LOCKING_ANDX pReq, cPtr<cSMBResponse> response)
{
  LARGE_INTEGER bytes;
  LARGE_INTEGER offset;
  DWORD dwRet;

  for (int i=0; i<pReq->NumberOfUnlocks; i++)
  {
    if (pReq->LockType & LOCKING_ANDX_LARGE_FILES)
    {
      PNTLOCKING_ANDX_RANGE pLockingRange = reinterpret_cast<PNTLOCKING_ANDX_RANGE>(pReq->Buffer
        + i*sizeof(NTLOCKING_ANDX_RANGE));
      bytes.HighPart = pLockingRange->LengthHigh;
      bytes.LowPart = pLockingRange->LengthLow;
      offset.HighPart = pLockingRange->OffsetHigh;
      offset.LowPart = pLockingRange->OffsetLow;
    }
    else
    {
      PLOCKING_ANDX_RANGE pLockingRange = reinterpret_cast<PLOCKING_ANDX_RANGE>(pReq->Buffer
        + i*sizeof(LOCKING_ANDX_RANGE));
      bytes.HighPart = 0;
      bytes.LowPart = pLockingRange->Length;
      offset.HighPart = 0;
      offset.LowPart = pLockingRange->Offset;
    }

    cPtr<iComposite> pComposite = m_compositeMap.getComposite(pReq->Fid, response);
    if (pComposite.isValid())
      dwRet = pComposite->Lock(offset, bytes, false);
    else
      return response;

    if (dwRet != ERROR_SUCCESS)
    {
      QSOS((L"Failed to unlock file with error [%d]", dwRet));
      response->setDosError(SMB_ERR_CLASS_SERVER, dwRet);
      return response;
    }
  }

  for (int i=0; i<pReq->NumberOfLocks; i++)
  {
    if (pReq->LockType & LOCKING_ANDX_LARGE_FILES)
    {
      PNTLOCKING_ANDX_RANGE pLockingRange = reinterpret_cast<PNTLOCKING_ANDX_RANGE>(pReq->Buffer
        + i*sizeof(NTLOCKING_ANDX_RANGE)
        + pReq->NumberOfUnlocks*sizeof(NTLOCKING_ANDX_RANGE));
      bytes.HighPart = pLockingRange->LengthHigh;
      bytes.LowPart = pLockingRange->LengthLow;
      offset.HighPart = pLockingRange->OffsetHigh;
      offset.LowPart = pLockingRange->OffsetLow;
    }
    else
    {
      PLOCKING_ANDX_RANGE pLockingRange = reinterpret_cast<PLOCKING_ANDX_RANGE>(pReq->Buffer
        + i*sizeof(LOCKING_ANDX_RANGE)
        + pReq->NumberOfUnlocks*sizeof(LOCKING_ANDX_RANGE));
      bytes.HighPart = 0;
      bytes.LowPart = pLockingRange->Length;
      offset.HighPart = 0;
      offset.LowPart = pLockingRange->Offset;
    }

    cPtr<iComposite> pComposite = m_compositeMap.getComposite(pReq->Fid, response);
    if (pComposite.isValid())
      dwRet = pComposite->Lock(offset, bytes, true);
    else
      return response;

    if (dwRet != ERROR_SUCCESS)
    {
      QSOS((L"Failed to lock file with error [%d]", dwRet));
      response->setDosError(SMB_ERR_CLASS_SERVER, dwRet);
      return response;
    }
  }

  return response;
}

cPtr<cSMBResponse> cSMBTree::DeleteDirectory(const std::wstring ksPath, cPtr<cSMBResponse> response)
{
  DWORD dwRet = m_pTreeResource->DeleteDirectory(ksPath);
  if (dwRet != ERROR_SUCCESS)
      response->setDosError(SMB_ERR_CLASS_DOS, dwRet);

  return response;
}

cPtr<cSMBResponse> cSMBTree::CreateDirectory(const std::wstring ksPath, cPtr<cSMBResponse> response)
{
  SECURITY_ATTRIBUTES _sa;
  BY_HANDLE_FILE_INFORMATION bhfi;
  DWORD retCode(0);
  cPtr<iComposite> pComposite = m_pTreeResource->NTCreateFolder(ksPath
      , &_sa
      , &bhfi
      , retCode);

  if (retCode != ERROR_SUCCESS)
  {
    response->setDosError(SMB_ERR_CLASS_DOS, retCode);
  }

  return response;
}

cPtr<cSMBResponse> cSMBTree::ProcessExit(const cPtr<cSMBResponse> response)
{
  m_compositeMap.clear();
  return response;
}

cPtr<cSMBResponse> cSMBTree::SetInformation(const PREQ_SET_INFORMATION pReq, cPtr<cSMBResponse> response)
{
  const String sFilePath = (WCHAR*)(pReq->Buffer + sizeof(UCHAR));

  BY_HANDLE_FILE_INFORMATION bhi;
  DWORD dwRet;
  cPtr<iComposite> pComposite = m_pTreeResource->NTCreateFile(sFilePath
    , GENERIC_WRITE
    , FILE_SHARE_READ
    , NULL
    , OPEN_EXISTING
    , FILE_ATTRIBUTE_NORMAL
    , NULL
    , &bhi
    , dwRet);

  if (pComposite.isNull())
    response->setDosError(SMB_ERR_CLASS_DOS, dwRet);
  else
  {
    // convert seconds time into FILETIME (base: 100 nano-seconds)
    ULARGE_INTEGER lg;
    lg.QuadPart = pReq->LastWriteTimeInSeconds * 100000000; 
    FILETIME modifiedTime;
    modifiedTime.dwHighDateTime = lg.HighPart;
    modifiedTime.dwLowDateTime = lg.LowPart;
    dwRet = pComposite->SetFileInfo(FILETIME(), modifiedTime, FILETIME(), pReq->FileAttributes);

    if (dwRet != ERROR_SUCCESS)
      response->setDosError(SMB_ERR_CLASS_DOS, dwRet);
  }

  return response;
}

cPtr<cSMBResponse> cSMBTree::SetInformation2(const PREQ_SET_INFORMATION2 pReq, cPtr<cSMBResponse> response)
{
  cPtr<iComposite> pComposite = m_compositeMap.getComposite(pReq->Fid, response);

  if (pComposite.isValid())
  {
    FILETIME creationTime;
    SMBTimeToFiletime(pReq->CreationDate, pReq->CreationTime, &creationTime);
    FILETIME modifiedTime;
    SMBTimeToFiletime(pReq->LastWriteDate, pReq->LastWriteTime, &modifiedTime);
    FILETIME accessTime;
    SMBTimeToFiletime(pReq->LastAccessDate, pReq->LastAccessTime, &accessTime);
    const DWORD dwRet = pComposite->SetFileInfo(creationTime, modifiedTime, accessTime, 0);

    if (dwRet != ERROR_SUCCESS)
    {
      response->setDosError(SMB_ERR_CLASS_DOS, dwRet);
    }
  }

  return response;
}

cPtr<cSMBResponse> cSMBTree::QueryInformation2(const PREQ_QUERY_INFORMATION2 pReq, cPtr<cSMBResponse> response)
{
  cPtr<iComposite> pComposite = m_compositeMap.getComposite(pReq->Fid, response);

  if (pComposite.isValid())
  {
    fileInfo fi;
    pComposite->writeInfo(&fi, 0);
    PRESP_QUERY_INFORMATION2 pResp = (PRESP_QUERY_INFORMATION2)response->allocParams(sizeof(RESP_QUERY_INFORMATION2));
    FiletimeToSMBTime(&(fi.w32Data.ftCreationTime), &(pResp->CreationDate), &(pResp->CreationTime));
    FiletimeToSMBTime(&(fi.w32Data.ftLastAccessTime), &(pResp->LastAccessDate), &(pResp->LastAccessTime));
    FiletimeToSMBTime(&(fi.w32Data.ftLastWriteTime), &(pResp->LastWriteDate), &(pResp->LastWriteTime));
    pResp->FileDataSize = fi.allocSize.LowPart;
    pResp->FileAllocationSize = fi.allocSize.LowPart;
    pResp->FileAttributes = static_cast<USHORT>(fi.w32Data.dwFileAttributes);
    pResp->ByteCount = 0;
  }

  return response;
}

cPtr<cSMBResponse> cSMBTree::NTCancel(const cPtr<cSMBResponse> response)
{
  const ULONGLONG uid = iUniqueNotifyID::singleton().cancelNotifyID(getRequestID(response->Header));
  if (0 != uid)
    m_compositeMap.cancelNotifications(uid, false);
  return response;
}

cPtr<cSMBResponse> cSMBTree::CheckDirectory(const PREQ_CHECK_DIRECTORY pReq, cPtr<cSMBResponse> pResponse)
{
  const String sPath(getName(reinterpret_cast<PWCHAR>(pReq->Buffer + sizeof(UCHAR)), pResponse->Header->Flags2));
  //const String sPath = reinterpret_cast<PWCHAR>(pReq->Buffer + );
  BY_HANDLE_FILE_INFORMATION bhi;
  DWORD retCode;
  const cPtr<iComposite> pComposite = m_pTreeResource->NTCreateFile(sPath
      , GENERIC_READ
      , FILE_SHARE_READ
      , NULL
      , OPEN_EXISTING
      , FILE_ATTRIBUTE_DIRECTORY
      , NULL
      , &bhi
      , retCode);
  if (!pComposite.isValid() || ERROR_SUCCESS != retCode)
    pResponse->setDosError(SMB_ERR_CLASS_DOS, retCode);

  return pResponse;
}

void cSMBTree::AddOplock(const cPtr<cOpLock> pOpLock)
{
  // control the lifetime of the oplock with respect to the tree connection
  cLockGuard lg(&m_opLockListAccess);
  m_OpLocks.push_back(pOpLock);
}

void cSMBTree::RemoveOplock(const cPtr<cOpLock> pOpLock)
{
  // control the lifetime of the oplock with respect to the tree connection
  cLockGuard lg(&m_opLockListAccess);

  for (std::list<cPtr<cOpLock> >::iterator it = m_OpLocks.begin()
    ; it != m_OpLocks.end()
    ; ++it)
  {
    cPtr<cOpLock> findLock = *it;
    if (findLock.getRaw() == pOpLock.getRaw())
    {
      m_OpLocks.erase(it);
      break;
    }
  }
}

void cSMBTree::notifyRemoved(const vfs::cPtr<cSMBTree>& pNewTree)
{
  std::vector<tCompositeMap::tFidPathPair> fidPathPair;
  m_compositeMap.triggerAllNotificationsAndClear(fidPathPair);
  SECURITY_ATTRIBUTES _sa;
  BY_HANDLE_FILE_INFORMATION bhfi;
  DWORD ret;
  for (tFidPathPairList::const_iterator cit = fidPathPair.begin()
    ; fidPathPair.end() != cit
    ; ++cit)
  {
    const String& sPath = cit->second;
    const cPtr<iComposite> pNewComposite = pNewTree->m_pTreeResource->NTCreateFile(sPath
      , 0
      , 0
      , &_sa
      , OPEN_EXISTING
      , FILE_ATTRIBUTE_NORMAL
      , NULL
      , &bhfi
      , ret);
    if (pNewComposite.isValid() && ERROR_SUCCESS == ret)
      pNewTree->m_compositeMap.addFid(cit->first, pNewComposite, sPath);
  }
}

void cSMBTree::ReleaseOpLock(const cPtr<cOpLock> pOpLock)
{
  // control the lifetime of the oplock with respect to the tree connection
  m_pOpLockManager->Release(pOpLock);
}

inline int cSMBTree::getSessionID(PSMB_HEADER const pSMBHeader) const
{
  union {
    struct {
      USHORT Uid;
      USHORT Tid;
    };
    ULONG Sid;
  } mapper;

  mapper.Uid = pSMBHeader->Uid;
  mapper.Tid = pSMBHeader->Tid;
  return mapper.Sid;
}

inline cNotificationID cSMBTree::getRequestID(PSMB_HEADER const pSMBHeader) const
{
  return cNotificationID(pSMBHeader->Sid, pSMBHeader->Tid, pSMBHeader->Mid);
}

USHORT cSMBTree::getChainedCreateFid(const cConstPtr<cSMBResponse> pSMBResponse) const
{
  //RHRHRH this could be a more comprehensive solution, just trying to get Mac OS X to work
  const PSMB_HEADER pHeader = pSMBResponse->Header;
  if (SMB_COM_NT_CREATE_ANDX != pHeader->Command)
    return 0;

  PRESP_NT_CREATE_ANDX const pResp = reinterpret_cast<PRESP_NT_CREATE_ANDX const>(pHeader+1);
  return pResp->Fid;
}

inline String cSMBTree::getName(const PWCHAR pName, const USHORT flags2) const
{
  if (flags2 & SMB_FLAGS2_DFS)
  {
    try
    {
      return iQCIFSIPC::singleton().removeDFSPart(String(pName));
    }
    catch (const cNotFound&)
    {
    }
  }

  // RFHRFH - doing a simple 'return pName' causes crash in default constructor of
  // basic_string in Release build. If you know why please tell me - RFHRFH
  return String(pName, wcslen(pName));
}

PUCHAR writeShi1Buffer(const PSHARE_INFO_1_BUFFER pShareInfoBuffer, const String& sName)
{
  if (sName.empty())
  {
    pShareInfoBuffer->MaxCount = 0;
    pShareInfoBuffer->Offset = 0;
    pShareInfoBuffer->ActualCount = 0;
    return reinterpret_cast<PUCHAR>(pShareInfoBuffer+1);
  }

  pShareInfoBuffer->MaxCount = sName.size()+1;
  pShareInfoBuffer->Offset = 0;
  pShareInfoBuffer->ActualCount = pShareInfoBuffer->MaxCount;
  const PWCHAR pShareData = reinterpret_cast<PWCHAR>(pShareInfoBuffer+1);
  wcscpy(pShareData, sName.c_str());

  return reinterpret_cast<PUCHAR>(pShareData + pShareInfoBuffer->MaxCount);
}

static volatile LONG assocGid = 2;

cPtr<cSMBResponse> cSMBTree::processTransactNamedPipe(const PUCHAR pHeader
  , const USHORT Fid
  , const PUCHAR pData
  , const PUCHAR pTransData
  , const PUSHORT pRespBuffer
  , cPtr<cSMBResponse> response ) const
{
  cPtr<iComposite> pComposite = m_compositeMap.getComposite(Fid, response);
  if (pComposite.isNull())
    return response;

  const PUSHORT pReqByteCount = reinterpret_cast<PUSHORT>(pData);
  PUCHAR pName(reinterpret_cast<PUCHAR>(pReqByteCount+1));
  const std::string sPipeName(reinterpret_cast<PCHAR>(pName));

  QTRACE((L"cSMBTree::processTransactNamedPipe() - %S", sPipeName.c_str()));

  const PUCHAR pRespStart(reinterpret_cast<PUCHAR>(pRespBuffer+1));
  PUCHAR pBuffer = pRespStart;
  align<DWORD>(pBuffer, reinterpret_cast<PUCHAR>(response->header()));
  cTrans2Response transResponse(response);
  transResponse.setParams(pBuffer, 0);

  typedef std::pair<String, String> tShareDesc;
  typedef std::vector<tShareDesc> tShareList;
  static tShareList shares;
  if (shares.empty())
  {
    //shares.push_back(tShareList::value_type(L"IPC$", L"Remote IPC"));
    //shares.push_back(tShareList::value_type(L"ADMIN$", L"Remote Admin"));
    //shares.push_back(tShareList::value_type(L"C$", L"Default Share"));
    shares.push_back(tShareList::value_type(L"vfs", L"vfs SAM"));
  }

  const PRPC_HEADER pReqRPCHeader = reinterpret_cast<PRPC_HEADER>(pTransData);
  switch (pReqRPCHeader->PTYPE)  //RPC PType RFHRFH - got values from MS network monitor 3
  {
  case 0xb: //bind
    {
      const PRPC_BIND_ACK pRespRPCHeader = reinterpret_cast<PRPC_BIND_ACK>(pBuffer);
      memcpy(reinterpret_cast<PUCHAR>(pRespRPCHeader)
        , reinterpret_cast<PUCHAR>(pReqRPCHeader)
        , sizeof(RPC_HEADER));
      static UCHAR bindAck[] = {0x0D, 0x00, 0x5C, 0x50, 0x49, 0x50, 0x45, 0x5C, 0x73, 0x72, 0x76, 0x73, 0x76, 0x63, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x5D, 0x88, 0x8A, 0xEB, 0x1C, 0xC9, 0x11, 0x9F, 0xE8, 0x08, 0x00, 0x2B, 0x10, 0x48, 0x60, 0x02, 0x00, 0x00, 0x00};
      static const USHORT kAckSize(sizeof(bindAck)/sizeof(bindAck[0]));
      pRespRPCHeader->Common.PTYPE = 0xc;
      pRespRPCHeader->Common.frag_length = kAckSize + sizeof(RPC_BIND_ACK);
      pRespRPCHeader->max_recv_frag = 4280;
      pRespRPCHeader->max_xmit_frag = 4280;
      pRespRPCHeader->assoc_group_id = InterlockedIncrement(&assocGid);
      const PUCHAR pRespData = reinterpret_cast<PUCHAR>(pRespRPCHeader+1);
      memcpy(pRespData, bindAck, kAckSize);
      transResponse.setData(pBuffer, pRespRPCHeader->Common.frag_length);
      *pRespBuffer = pBuffer+pRespRPCHeader->Common.frag_length - pRespStart;
    }
    break;
  case 0: //Request
    {
      const PRPC_REQ_RESPONSE pRPCResponse = reinterpret_cast<PRPC_REQ_RESPONSE>(pBuffer);
      const PSHARE_ENUM_STRUCT pShareEnum = reinterpret_cast<PSHARE_ENUM_STRUCT>(pRPCResponse+1);
      pShareEnum->Level = 1;
      pShareEnum->SwitchValue = 1;

      const PSHARE_INFO_1_CONTAINER pShareInfoContainer = reinterpret_cast<PSHARE_INFO_1_CONTAINER>(pShareEnum+1);
      pShareInfoContainer->Pointer = rand();
      pShareInfoContainer->EntriesRead = shares.size();

      const PSHARE_INFO_1_CONTAINER pShareInfo = reinterpret_cast<PSHARE_INFO_1_CONTAINER>(pShareInfoContainer+1);
      pShareInfo->Pointer = rand();
      pShareInfo->EntriesRead = shares.size();

      PSHARE_INFO_1 pShareInfoData = reinterpret_cast<PSHARE_INFO_1>(pShareInfo+1);
      for (tShareList::const_iterator cit = shares.begin()
        ; shares.end() != cit
        ; ++cit)
      {
        const String& sShareName(cit->first);
        pShareInfoData->shi1_netname = 0;
        pShareInfoData->shi1_type = sShareName == L"IPC$" ? (STYPE_SPECIAL | STYPE_IPC): STYPE_SPECIAL;
        pShareInfoData->shi1_remark = 0;
        ++pShareInfoData;
      }

      PUCHAR pNext = reinterpret_cast<PUCHAR>(pShareInfoData);
      align<DWORD>(pNext, reinterpret_cast<PUCHAR>(pShareEnum));
      for (tShareList::const_iterator cit = shares.begin()
        ; shares.end() != cit
        ; ++cit)
      {
        pNext = writeShi1Buffer(reinterpret_cast<PSHARE_INFO_1_BUFFER>(pNext), cit->first); //Name
        align<DWORD>(pNext, reinterpret_cast<PUCHAR>(pShareEnum));
        pNext = writeShi1Buffer(reinterpret_cast<PSHARE_INFO_1_BUFFER>(pNext), cit->second);  //Remark
        align<DWORD>(pNext, reinterpret_cast<PUCHAR>(pShareEnum));
      }

      const PSRV_SVC_RESPONSE pSrvSvcResponse
        = reinterpret_cast<PSRV_SVC_RESPONSE >(pNext);
      pSrvSvcResponse->TotalEntries = shares.size();
      pSrvSvcResponse->ResumeHandle = 0;
      pSrvSvcResponse->ReturnValue = 0;

      *pRespBuffer = reinterpret_cast<PUCHAR>(pSrvSvcResponse+1) - pRespStart;

      pRPCResponse->Common.rpc_vers = 5;
      pRPCResponse->Common.rpc_vers_minor = 0;
      pRPCResponse->Common.PTYPE = 2;
      pRPCResponse->Common.pfc_flags = 3;
      pRPCResponse->Common.packed_drep = 0x10;
      pRPCResponse->Common.frag_length = *pRespBuffer - 1;
      pRPCResponse->Common.auth_length = 0;
      pRPCResponse->Common.call_id = pReqRPCHeader->call_id;
      pRPCResponse->AllocHint = pRPCResponse->Common.frag_length - sizeof(RPC_REQ_RESPONSE);
      pRPCResponse->PContId = 1;
      pRPCResponse->CancelCount = 0;
      pRPCResponse->Reserved = 0;

      transResponse.setData(pBuffer, pRPCResponse->Common.frag_length);
    }
    break;
  default:
    processUnhandled(NULL, 0, NULL, NULL, response);
    break;
  }

  return response;
}

#endif // CIFS 