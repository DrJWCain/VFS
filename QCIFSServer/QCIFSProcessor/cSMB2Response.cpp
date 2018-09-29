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

//cSMB2Response.cpp

#include "stdafx.h"
#include "main.h"
#include "cSMB2Response.h"
#include "socket_routines.h"
#include <stierr.h>
#include "cSMB2Session.h"

#include "..\Common\iCommonFactory.h"

extern tActivityMap ActivityQCIFSProcessors;

using namespace vfs;

///////////////////////////////////////////////////////////////////////////////////////
cLock CurrentCreditLevelLock;
USHORT CurrentCreditLevel = 0;

//void PrintHexDump(DWORD length, PBYTE buffer);
void makeReplyHeader(const PSMB2_HEADER pReqHeader, const PSMB2_HEADER pRespHeader)
{
  if(SMB2_NegotiateProtocol == pReqHeader->Command)
  {
    //From [MS-SMB2].pdf: <145> Section 3.2.5.5: By default Windows 8 and Windows 8.1 will try to establish alternate channels, if Connection.OutstandingRequests exceeds 8. Windows Server 2012 and Windows Server 2012 R2 will try to establish alternate channels, if Connection.OutstandingRequests exceeds 1.
    cLockGuard g(&CurrentCreditLevelLock);
    //CurrentCreditLevel = 4;//reset the credits
    CurrentCreditLevel = getSMBCredits();//reset the credits - From [MS-SMB2].pdf: <139> Section 3.2.5.5: Windows-based SMB2 clients will try to establish alternate channels, if Connection.OutstandingRequests exceeds 8.
  }
  else if(SMB2_Cancel != pReqHeader->Command)
  {
    cLockGuard g(&CurrentCreditLevelLock);
    if(pReqHeader->CreditRequest)
      ++CurrentCreditLevel;
  }

  ZeroMemory(pRespHeader, sizeof(SMB2_HEADER));

  if(0xff == *reinterpret_cast<PUCHAR>(pReqHeader))
  {
    //SMB1 negotiate header
    pRespHeader->ProtocolId = SMB2_HEADER_PROTOCOL;
    pRespHeader->Command = SMB2_NegotiateProtocol;
  }
  else
  {
    pRespHeader->ProtocolId = pReqHeader->ProtocolId;
    pRespHeader->Command = pReqHeader->Command;
    pRespHeader->MessageId = pReqHeader->MessageId;
    pRespHeader->ProcessId = pReqHeader->ProcessId;
    pRespHeader->Flags = pReqHeader->Flags;
    pRespHeader->SessionId = pReqHeader->SessionId;
    pRespHeader->TreeId = pReqHeader->TreeId;
  }

  //SNIA SDC 2013 - always grant a credit - even if(0xff == *reinterpret_cast<PUCHAR>(pReqHeader)) - SMBClient was getting 0 credits!
  pRespHeader->CreditRequest = 0;
  if(SMB2_Cancel != pRespHeader->Command)
  {
    if(pReqHeader->CreditRequest)
    {
      USHORT creditRequest = pReqHeader->CreditRequest;
      {
        cLockGuard g(&CurrentCreditLevelLock);
        USHORT allowance = min(CurrentCreditLevel,creditRequest);
        CurrentCreditLevel -= allowance;
        pRespHeader->CreditRequest = allowance;
        //if(!allowance)
        //  QTRACE((L"creditRequest:%d CurrentCreditLevel:%d allowance:%d", creditRequest, CurrentCreditLevel, allowance));
      }
    }
    //else if(CurrentCreditLevel)//boot time request supply ... don't do it mid flow it leaks credits ...
    else
    {
      pRespHeader->CreditRequest = 1;
      //QSOS((L"creditRequest:%d CurrentCreditLevel:%d allowance:%d", pReqHeader->CreditRequest, CurrentCreditLevel, pRespHeader->CreditRequest));
      //PrintHexDump(sizeof(SMB2_SYNC_Header), (PBYTE)pReqHeader);
    }
  }

  pRespHeader->StructureSize = sizeof(SMB2_HEADER);
  //pRespHeader->Status = ERROR_SUCCESS;
  pRespHeader->Flags |= SMB2_FLAGS_SERVER_TO_REDIR;
}
///////////////////////////////////////////////////////////////////////////////////////
// cSMB2Response //////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

cSMB2Response::cSMB2Response(const cSMB2Request* pRequest, char* memory, const cPtr<cMemoryView>& pBuffer)
: cSMB2Buffer(std::pair<unsigned int, unsigned char*>(
  gkSMB2BufferSize - 4, reinterpret_cast<PUCHAR>(memory + 4)))
, m_pBuffer(pBuffer)
, m_pContextPacket(pRequest->getContextPacket())
{
  makeReplyHeader(pRequest->header(), Header);
}

cSMB2Response::~cSMB2Response()
{
}

void cSMB2Response::WritePacket(cPtr<iTXPacket> pPacket)
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

void cSMB2Response::setDosError(UCHAR errorClass, DWORD error)
{
  if (NT_ERROR(error))
  {
    //QSOS((L"cSMB2Response::setDosError() - NT ERROR {%d}", error));
    setNTError(error);
  }
  else
  {
    std::vector<int> errs = iCommonFactory::singleton().getWinNTCodes(error);
    if (errs.empty())
    {
      //QTRACE((L"cSMB2Response::setDosError() - Didn't find matching NT code for error %d", error));
      // set DOS error codes in header
/*
      Header->Flags2 &= (0xffff-SMB_FLAGS2_NT_STATUS);
      Header->ErrorClass = errorClass;
      Header->Reserved = 0;
      Header->Error = static_cast<USHORT>(error);
*/
    }
    else
    {
      // set corresponding NT error
      const DWORD ntErr(errs.front());
      //QSOS((L"cSMB2Response::setDosError() - TRANSLATE ERROR {%d -> %u}", error, ntErr));
      setNTError(ntErr);
    }
  }
}

void cSMB2Response::setNTError(ULONG error, UCHAR errorData)
{
  // set NT error codes in header
  pCurChainHeader->Status = error | 0xc0000000;
  const PRESP_SMB2_ERROR pResp = overrideData<RESP_SMB2_ERROR>();
  pResp->StructureSize = 9;//[#2] p.24
  pResp->Reserved = 0;
  pResp->ByteCount = 0;
  pResp->ErrorData = errorData;
}

PUCHAR cSMB2Response::getResponseBuffer()
{
  return reinterpret_cast<PUCHAR>(m_pBuffer->getBytes());
}

void cSMB2Response::addData(const ULONG size)
{
  cSMB2Buffer::incParams(size);
}

void cSMB2Response::addData(const SMART_TPE &pTPE)
{
  // add TransmitPackets element
  m_TPEList.push_back(pTPE);
}

tTransmitList& cSMB2Response::getTransmitList()
{
  // retun TransmitPackets element list
  return m_TPEList;
}


void PrintHexDump(DWORD length, PBYTE buffer);
void cSMB2Response::sign(cPtr<cSMB2Session> session, bool extraLogging)
{
  //QTRACE((L"cSMB2Response::sign required."));

  BYTE sig[16];
  BYTE sig2[16];
  PSMB2_HEADER head = header();
  bool isRead = (head->Command == SMB2_Read);
  memcpy(sig, head->Signature, 16);
  memcpy(sig2, head->Signature, 16);
  memset(head->Signature, 0, 16);

  if((PBYTE)head != (PBYTE)Buffer)
  {
    QSOS((L"%p %p", (PBYTE)head, (PBYTE)Buffer));
    PrintHexDump(64, (PBYTE)Buffer);
    PrintHexDump(64, (PBYTE)head);
  }
  if(isRead)
  {
    //PrintHexDump(64, (PBYTE)Buffer);
    session->SignThis((PBYTE)Buffer, size(), sig, 16);
    session->VerifyThis((PBYTE)Buffer, size(), sig, 16);
    //head->Flags &= ~SMB2_FLAGS_SIGNED;
    memcpy(head->Signature, sig, 16);
  }
  else
  {
    session->SignThis((PBYTE)Buffer, size(), sig, 16);
    memcpy(head->Signature, sig, 16);
  }
    
  

  if(isRead)
  {
    QTRACE((L"%S: old ...", __FUNCTION__));
    PrintHexDump(16, sig2);
    QTRACE((L"%S: new ... %d", __FUNCTION__, size()));
    PrintHexDump(16, sig);
  }

}