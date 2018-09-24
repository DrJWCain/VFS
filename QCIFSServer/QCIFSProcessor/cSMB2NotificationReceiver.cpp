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

//cSMB2NotificationReceiver.cpp

#include "StdAfx.h"

#include ".\csmb2notificationreceiver.h"
#include "cSMB2Response.h"
#include "align.h"
#include "cGuardedUniqueNotify.h"

#include "..\SocketServer\iPacket.h"
#include "cSMB2Request.h"

using namespace vfs;

cSMB2NotificationReceiver::cSMB2NotificationReceiver( const cPtr<cSMB2Response> pResponse
  , const DWORD CompletionFilter
  , const cPtr<cGuardedUniqueNotify>& pGuard)
: m_pResponse(pResponse)
, m_dwSearchFilter(0)
, m_pGuard(pGuard)
{

}
cSMB2NotificationReceiver::~cSMB2NotificationReceiver(void)
{
}

DWORD cSMB2NotificationReceiver::Notify()
{
  return Notify(tNotifyList());
}

template <typename T>
static void setLengthAndOffset(const cPtr<cSMB2Response>& response
                               , const T pResp
                               , const ULONG bufferLength
                               , const PUCHAR pBuf)
{
  pResp->OutputBufferOffset = static_cast<USHORT>(pBuf - reinterpret_cast<PUCHAR>(response->header()));
  pResp->OutputBufferLength = bufferLength;
  response->addData(bufferLength);
}

DWORD cSMB2NotificationReceiver::Notify(const tNotifyList& notifyInfo)
{
  //QTRACE((L"cSMB2NotificationReceiver::Notify size:%Iu", notifyInfo.size()));

  const PRESP_SMB2_CHANGE_NOTIFY pResp = m_pResponse->getResponseType<RESP_SMB2_CHANGE_NOTIFY>();
  const PUCHAR pNotifyBuffer(reinterpret_cast<PUCHAR>(pResp+1));
  const PUCHAR pHeader(reinterpret_cast<PUCHAR>(m_pResponse->header()));

  ULONG respLength(0);
  ULONG prevRespLength(0);
  for (tNotifyList::const_iterator cit = notifyInfo.begin()
    ; notifyInfo.end()!= cit
    ; ++cit)
  {
    const tNotifyInfo& rNotifyInfo(*cit);
    const PULONG pPrevRespBuf(reinterpret_cast<PULONG>(pNotifyBuffer + prevRespLength));
    *pPrevRespBuf = respLength - prevRespLength;
    prevRespLength = respLength;
    const PSMB2_FILE_NOTIFY_INFORMATION pNI(
      reinterpret_cast<PSMB2_FILE_NOTIFY_INFORMATION>(pNotifyBuffer + respLength));
    pNI->NextEntryOffset = 0;
    pNI->Action = rNotifyInfo.first;
    pNI->FileNameLength = rNotifyInfo.second.size()*sizeof(WCHAR);
    const PUCHAR pFilename = reinterpret_cast<PUCHAR>(pNI+1);
    memcpy(pFilename, rNotifyInfo.second.c_str(), pNI->FileNameLength);

    respLength += sizeof(SMB2_FILE_NOTIFY_INFORMATION) + pNI->FileNameLength;
    respLength += getAlignment<ULONGLONG>(pNotifyBuffer + respLength, pHeader);
  }

  pResp->StructureSize = 9; //[#2] p.83
  setLengthAndOffset(m_pResponse, pResp, respLength, pNotifyBuffer);

  SMB2_SYNC_Header* Header = m_pResponse->header();
  //QSOS((L"cSMB2NotificationReceiver::Notify - Removing credit %d", Header->CreditRequest));
  Header->CreditRequest = 0;

  return SendNotification();
}

DWORD cSMB2NotificationReceiver::CancelNotification(bool synchronous)
{
  const PSMB2_ASYNC_Header pAsyncHeader = m_pResponse->async_header();

  //QTRACE((L"cSMB2NotificationReceiver::CancelNotification message %I64d async %I64d. Flags 0x%08x", pAsyncHeader->MessageId, pAsyncHeader->AsyncId, pAsyncHeader->Flags));

  m_pResponse->setNTError(STATUS_CANCELLED);

  const PRESP_SMB2_ERROR pResp = m_pResponse->getResponseType<RESP_SMB2_ERROR>();
  pResp->StructureSize = 9; //[#2] p.24
  pResp->Reserved = 0;
  pResp->ByteCount = 0;
  pResp->ErrorData = 0;

  SMB2_SYNC_Header* Header = m_pResponse->header();
  //QSOS((L"cSMB2NotificationReceiver::CancelNotification - Removing credit %d", Header->CreditRequest));
  Header->CreditRequest = 0;

  DWORD ret = SendNotification();

  if(synchronous)
  {
    ULONG flagsBefore = pAsyncHeader->Flags;
    pAsyncHeader->Flags = pAsyncHeader->Flags & (SMB2_FLAGS_ASYNC_COMMAND ^ ULONG_MAX); // Turn off SMB2_FLAGS_ASYNC_COMMAND flag
    ULONG flagsAfter = pAsyncHeader->Flags;

    //QTRACE((L"cSMB2NotificationReceiver::CancelNotification message %I64d synchronous %I64d. flagsBefore 0x%08x, flagsAfter 0x%08x", pAsyncHeader->MessageId, pAsyncHeader->AsyncId, flagsBefore, flagsAfter));
    m_pResponse->setNTError(STATUS_CANCELLED);

    const PRESP_SMB2_ERROR pResp = m_pResponse->getResponseType<RESP_SMB2_ERROR>();
    pResp->StructureSize = 9; //[#2] p.24
    pResp->Reserved = 0;
    pResp->ByteCount = 0;
    pResp->ErrorData = 0;

    SMB2_SYNC_Header* Header = m_pResponse->header();
    //QSOS((L"cSMB2NotificationReceiver::CancelNotification - Removing credit %d", Header->CreditRequest));
    Header->CreditRequest = 0;

    SendNotification();
  }

  return ret;
}

DWORD cSMB2NotificationReceiver::SendNotification()
{
  const cPtr<iRXPacket> pRXPacket = m_pResponse->getContextPacket();
  const cPtr<iTXPacket> pTXPacket = pRXPacket->getResponsePacket();
  m_pResponse->WritePacket(pTXPacket);
  pTXPacket->Send();
  return ERROR_SUCCESS;
}

