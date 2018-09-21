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

//cSMBResponse

#include "stdafx.h"

#if CIFS 

#include "mycifs.h"
#include "cSMBResponse.h"
#include "socket_routines.h"
#include <stierr.h>
#include "align.h"

#include "..\SocketServer\iPacket.h"
#include "..\Common\iCommonFactory.h"
#include "Trans2Structs.h"
#include "LogCategories.h"

extern tActivityMap ActivityQCIFSProcessors;

///////////////////////////////////////////////////////////////////////////////////////
// cSMBResponse ///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

cSMBResponse::cSMBResponse(const cSMBRequest& request, const cPtr<cMemoryView> pBuffer)
: cSMBBuffer(reinterpret_cast<unsigned char *>(pBuffer->getBytes() + 4), gkBufferSize)
, m_pBuffer(pBuffer)
, m_nTotalParamCount(0)
, m_nTotalDataCount(0)
, m_nParamCount(0)
, m_nParamOffset(0)
, m_nParamDisplacement(0)
, m_nDataCount(0)
, m_nDataOffset(0)
, m_nDataDisplacement(0)
, m_setupCount(0)
, m_pContextPacket(request.getContextPacket())
, ExtraPayload(0)
{
  vfs::iActivity::Ptr activity = getActivity();

  {
    cActivityScope as1(activity,kGetHeader);
    PSMB_HEADER reqHeader = request.header();

    Header = (PSMB_HEADER)Buffer;
    ZeroMemory(Header, sizeof(NT_SMB_HEADER));
    CopyMemory(Header->Protocol, reqHeader->Protocol, sizeof(Header->Protocol));
  
    Header->Command = reqHeader->Command;
    Header->Flags = SMB_FLAGS_SERVER_TO_REDIR | reqHeader->Flags;//SMB_FLAGS_CASE_INSENSITIVE;
    Header->Flags2 = reqHeader->Flags2;// | SMB_FLAGS2_DFS | SMB_FLAGS2_UNICODE | SMB_FLAGS2_NT_STATUS | SMB_FLAGS2_KNOWS_LONG_NAMES;
    Header->Pid = reqHeader->Pid;
    Header->Tid = reqHeader->Tid;
    Header->Mid = reqHeader->Mid;
    Header->Uid = reqHeader->Uid;
  }

  cActivityScope as1(activity,kGetBlocks);
  unsigned int offset = sizeof(NT_SMB_HEADER);
  cSMBBlock block;
  block.create(Buffer+offset);

  Blocks.push_back(block);
}

cSMBResponse::~cSMBResponse()
{
}

void cSMBResponse::WritePacket(cPtr<iTXPacket> pPacket)
{   
  // set Header as first element
  SMART_TPE smart_tpe;
  smart_tpe.tpe.dwElFlags = TP_ELEMENT_MEMORY;
  smart_tpe.tpe.cLength = transmitSize();
  smart_tpe.tpe.pBuffer = getResponseBuffer();
  smart_tpe.pMem = m_pBuffer;

  setNetBIOSResponse(reinterpret_cast<unsigned char *>(smart_tpe.tpe.pBuffer), size());
 
  // add any extra data buffers as new elements to the transmit packet array
  for (tTransmitList::const_iterator cit = m_TPEList.begin()
    ; cit != m_TPEList.end()
    ; ++cit)
  {
    const SMART_TPE &r_smart_tpe = *cit;
    if (r_smart_tpe.tpe.cLength > 0)
    {
      smart_tpe.tpe.cLength -= r_smart_tpe.tpe.cLength;
    }
  }

  tTransmitList transmitList;
  transmitList.push_back(smart_tpe);
  transmitList.insert(transmitList.end(), m_TPEList.begin(), m_TPEList.end());

  pPacket->setReply(transmitList);
}

///////////////////////////////////////////////////////////////////////////////////////
// errors /////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

void cSMBResponse::setDosError(UCHAR errorClass, DWORD error)
{
  if (NT_ERROR(error))
  {
    setNTError(error);
  }
  else
  {
    std::vector<int> errs = iCommonFactory::singleton().getWinNTCodes(error);
    if (errs.empty())
    {
      //QTRACE((L"cSMBResponse::setDosError() - Didn't find matching NT code for error %d", error));
      // set DOS error codes in header
      Header->Flags2 &= (0xffff-SMB_FLAGS2_NT_STATUS);
      Header->ErrorClass = errorClass;
      Header->Reserved = 0;
      Header->Error = static_cast<USHORT>(error);
    }
    else
    {
      // set corresponding NT error
      const DWORD ntErr(errs.front());
      setNTError(ntErr);
    }
  }

  clearBlock();
}

void cSMBResponse::setNTError(ULONG error)
{
  // set NT error codes in header
  Header->Flags2 |= SMB_FLAGS2_NT_STATUS;
  memcpy(&Header->ErrorClass, &error, sizeof(ULONG));
  clearBlock();
}

///////////////////////////////////////////////////////////////////////////////////////
// logging ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

void cSMBResponse::log(cSMBRequest* req)
{
  if(SMB_COM_NT_CREATE_ANDX == Header->Command)
  {
    if(STATUS_SUCCESS == Header->ErrorClass)
    {
      PRESP_NT_CREATE_ANDX pResp = reinterpret_cast<PRESP_NT_CREATE_ANDX>(getParams(sizeof(RESP_NT_CREATE_ANDX)));
      QMSG((kCtgLogFID, L"%s F%x", desc().c_str(), pResp->Fid));
      return;
    }
  }
  if(SMB_COM_OPEN_ANDX == Header->Command)
  {
    if(STATUS_SUCCESS == Header->ErrorClass)
    {
      PRESP_OPEN_ANDX pResp = reinterpret_cast<PRESP_OPEN_ANDX>(getParams(sizeof(RESP_OPEN_ANDX)));
      QMSG((kCtgLogFID, L"%s F%x", desc().c_str(), pResp->Fid));
      return;
    }
  }

  if(SMB_COM_WRITE_ANDX == Header->Command)
  {
    QMSG((kCtgLogIO, L"%s", desc().c_str()));
    return;
  }
  if(SMB_COM_WRITE == Header->Command)
  {
    QMSG((kCtgLogIO, L"%s", desc().c_str()));
    return;
  }
  if(SMB_COM_READ_ANDX == Header->Command)
  {
    PRESP_READ_ANDX pResp = reinterpret_cast<PRESP_READ_ANDX>(getParams(sizeof(RESP_READ_ANDX)));
    QMSG((kCtgLogIO, L"%s R%x%x", desc().c_str(), pResp->DataLengthHigh, pResp->DataLength));
    return;
  }

  if(SMB_COM_TRANSACTION2 == Header->Command)
  {
    PREQ_TRANSACTION pReq = (PREQ_TRANSACTION)req->getParams(sizeof(REQ_TRANSACTION));
    if(TRANS2_QUERY_FILE_INFORMATION == pReq->Buffer[0])
    {
      PREQ_QUERY_FILE_INFORMATION pReqFile = reinterpret_cast<PREQ_QUERY_FILE_INFORMATION>(reinterpret_cast<const PUCHAR>(req->Header) + pReq->ParameterOffset);
      if(SMB_QUERY_FILE_STANDARD_INFO == pReqFile->InformationLevel)
      {
        PsmbQueryFileStandardInfo pSmbInfo = reinterpret_cast<PsmbQueryFileStandardInfo>(
          reinterpret_cast<const PUCHAR>(Header) + getDataOffset());

        QMSG((kCtgLogSMB, L"%s %I64x", desc().c_str(), pSmbInfo->AllocationSize.QuadPart));
        return;
      }
    }
  }

  QMSG((kCtgLogSMB, L"%s", desc().c_str()));
}


///////////////////////////////////////////////////////////////////////////////////////
// blocks /////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

unsigned char* cSMBResponse::allocParams(unsigned char size)
{
  Blocks.back().allocParams(size);
  return getParams(size);
}

void cSMBResponse::addData(const USHORT size)
{
  Blocks.back().addData(size);
}

void cSMBResponse::pushBlock()
{
  PGENERIC_ANDX pAndX = getAndX();

  cSMBBlock block;
  block.create(Buffer+pAndX->AndXOffset);
  Blocks.push_back(block);
}

void cSMBResponse::clearBlock()
{
  Blocks.back().clearBlock();
}

///////////////////////////////////////////////////////////////////////////////////////
// Trans2 params //////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

// total param count
void cSMBResponse::setTotalParamCount(const ULONG paramCount)
{
  m_nTotalParamCount = paramCount;
}

ULONG cSMBResponse::getTotalParamCount()
{
  return m_nTotalParamCount;
}

// total data count
void cSMBResponse::setTotalDataCount(const ULONG dataCount)
{
  m_nTotalDataCount = dataCount;
}

ULONG cSMBResponse::getTotalDataCount()
{
  return m_nTotalDataCount;
}

// param count
void cSMBResponse::setParamCount(const ULONG paramCount)
{
  m_nParamCount = paramCount;
}

ULONG cSMBResponse::getParamCount()
{
  return m_nParamCount;
}

// param offset
void cSMBResponse::setParamOffset(const PUCHAR pTrans2Params)
{
  m_nParamOffset = checked_static_cast<ULONG>(pTrans2Params - reinterpret_cast<PUCHAR>(Header));
}

ULONG cSMBResponse::getParamOffset()
{
  return m_nParamOffset;
}

// param displacement
void cSMBResponse::setParamDisplacement(const ULONG paramDisplacement)
{
  m_nParamDisplacement = paramDisplacement;
}

ULONG cSMBResponse::getParamDisplacement()
{
  return m_nParamDisplacement;
}

// data count
void cSMBResponse::setDataCount(const ULONG dataCount)
{
  m_nDataCount = dataCount;
}

ULONG cSMBResponse::getDataCount()
{
  return m_nDataCount;
}

// data offset
void cSMBResponse::setDataOffset(const PUCHAR pTrans2Data)
{
  m_nDataOffset = checked_static_cast<ULONG>(pTrans2Data - reinterpret_cast<PUCHAR>(Header));
}

ULONG cSMBResponse::getDataOffset()
{
  return m_nDataOffset;
}

// data displacement
void cSMBResponse::setDataDisplacement(const ULONG dataDisplacement)
{
  m_nDataDisplacement = dataDisplacement;
}

ULONG cSMBResponse::getDataDisplacement()
{
  return m_nDataDisplacement;
}

// setup count
void cSMBResponse::addSetupData(const PUCHAR pSetupData, const UCHAR setupCount)
{
  // move data when using variable sized SetupCount field
  Blocks.back().nudgeData(pSetupData, setupCount);
  m_setupCount = setupCount;
}

UCHAR cSMBResponse::getSetupCount()
{
  return m_setupCount;
}

PUCHAR cSMBResponse::getResponseBuffer()
{
  return reinterpret_cast<PUCHAR>(m_pBuffer->getBytes());
}

void cSMBResponse::addData(const SMART_TPE &pTPE)
{
  // add TransmitPackets element
  m_TPEList.push_back(pTPE);
}

tTransmitList &cSMBResponse::getTransmitList()
{
  // retun TransmitPackets element list
  return m_TPEList;
}

void cSMBResponse::addExtraData(int i)
{
  Blocks.back().addExtraData(i);
}

#endif // CIFS 