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

//cNotificationReceiver.cpp

#include "StdAfx.h"

#if CIFS 

#include ".\cnotificationreceiver.h"
#include "cSMBResponse.h"
#include "cGuardedUniqueNotify.h"

#include "..\SocketServer\iPacket.h"

const DWORD NotifyActions[9] =
{
  0,
  FILE_NOTIFY_CHANGE_ATTRIBUTES|FILE_NOTIFY_CHANGE_SIZE|FILE_NOTIFY_CHANGE_LAST_WRITE,//FILE_ACTION_ADDED 0x00000001
  FILE_NOTIFY_CHANGE_ATTRIBUTES|FILE_NOTIFY_CHANGE_SIZE|FILE_NOTIFY_CHANGE_LAST_WRITE,//FILE_ACTION_REMOVED 0x00000002
  FILE_NOTIFY_CHANGE_ATTRIBUTES|FILE_NOTIFY_CHANGE_SIZE|FILE_NOTIFY_CHANGE_LAST_WRITE,//FILE_ACTION_MODIFIED 0x00000003
  FILE_NOTIFY_CHANGE_FILE_NAME|FILE_NOTIFY_CHANGE_DIR_NAME,                           //FILE_ACTION_RENAMED_OLD_NAME 0x00000004
  FILE_NOTIFY_CHANGE_FILE_NAME|FILE_NOTIFY_CHANGE_DIR_NAME,                           //FILE_ACTION_RENAMED_NEW_NAME 0x00000005
  FILE_NOTIFY_CHANGE_ATTRIBUTES|FILE_NOTIFY_CHANGE_SIZE|FILE_NOTIFY_CHANGE_LAST_WRITE,//FILE_ACTION_ADDED_STREAM 0x00000006
  FILE_NOTIFY_CHANGE_ATTRIBUTES|FILE_NOTIFY_CHANGE_SIZE|FILE_NOTIFY_CHANGE_LAST_WRITE,//FILE_ACTION_REMOVED_STREAM 0x00000007
  FILE_NOTIFY_CHANGE_ATTRIBUTES|FILE_NOTIFY_CHANGE_SIZE|FILE_NOTIFY_CHANGE_LAST_WRITE//FILE_ACTION_MODIFIED_STREAM 0x00000008
};

cNotificationReceiver::cNotificationReceiver(const cPtr<cSMBResponse> pSMBResponse
  , const BOOL bWatchSubtree
  , const DWORD SearchFilter
  , const cPtr<cGuardedUniqueNotify>& pGuard)
: m_trans2Response(pSMBResponse)
, m_bWatchSubtree(bWatchSubtree)
, m_dwSearchFilter(GetWindowsSearchFilter(SearchFilter))
, m_pGuard(pGuard)
{
}

cNotificationReceiver::~cNotificationReceiver(void)
{
}

DWORD cNotificationReceiver::Notify()
{
  m_trans2Response.m_pResponse->clearBlock();  // return a clear block with a success
  return SendNotification();
}

DWORD cNotificationReceiver::Notify(const tNotifyList& notifyInfo)
{
  //typedef struct {
  //ULONG NextEntryOffset;
  //ULONG Action;
  //ULONG FileNameLength;
  //WCHAR FileName[1];
  //} FILE_NOTIFY_INFORMATION;  //[#1]p.107

  DWORD dataSize(0);
  for (tNotifyList::const_iterator cit = notifyInfo.begin()
    ; notifyInfo.end() != cit
    ; ++cit)
  {
    const tNotifyInfo& notifyInfo(*cit);
    dataSize += sizeof(FILE_NOTIFY_INFORMATION) + (static_cast<DWORD>(notifyInfo.second.size()) * sizeof(WCHAR));
  }

  const PRESP_NT_TRANSACTION pResp
    = (PRESP_NT_TRANSACTION)m_trans2Response.m_pResponse->getParams(sizeof(RESP_NT_TRANSACTION) + sizeof(USHORT));

  PUCHAR pBuffer = pResp->Buffer;
  const PUSHORT pByteCount = reinterpret_cast<PUSHORT>(pBuffer);
  pBuffer += sizeof(USHORT);
  m_trans2Response.setParams(pBuffer, static_cast<USHORT>(dataSize));

  const __int64 dataSpace(gkBufferSize
    - (pBuffer - reinterpret_cast<PUCHAR>(m_trans2Response.m_pResponse->Header) + 4));  // +4 to be safe
  FILE_NOTIFY_INFORMATION* pPrevNotify(NULL);
  for (tNotifyList::const_iterator cit = notifyInfo.begin()
    ; notifyInfo.end() != cit && ((pBuffer - reinterpret_cast<PUCHAR>(m_trans2Response.m_pResponse->Header)) < dataSpace)
    ; ++cit)
  {
    const tNotifyInfo& notifyInfo(*cit);
    const String& sFilename(notifyInfo.second);

    FILE_NOTIFY_INFORMATION* const pNotifyInfo(reinterpret_cast<FILE_NOTIFY_INFORMATION* const>(pBuffer));
    pNotifyInfo->Action = notifyInfo.first;
    wcscpy(pNotifyInfo->FileName, sFilename.c_str());
    pNotifyInfo->FileNameLength = checked_static_cast<DWORD>(sFilename.size()*sizeof(WCHAR));
    pNotifyInfo->NextEntryOffset = 0;

    if (NULL != pPrevNotify)
      pPrevNotify->NextEntryOffset = checked_static_cast<DWORD>(pBuffer - reinterpret_cast<PUCHAR>(pPrevNotify));

    pPrevNotify = reinterpret_cast<FILE_NOTIFY_INFORMATION* const>(pBuffer);
    pBuffer += sizeof(FILE_NOTIFY_INFORMATION) + pNotifyInfo->FileNameLength;
  }

  m_trans2Response.setData(pBuffer, 0);
  *pByteCount = static_cast<USHORT>(pBuffer - reinterpret_cast<PUCHAR>(pByteCount) - sizeof(USHORT));

  pResp->WordCount = 18 + m_trans2Response.m_pResponse->getSetupCount();
  pResp->TotalParameterCount = m_trans2Response.m_pResponse->getTotalParamCount();
  pResp->TotalDataCount = m_trans2Response.m_pResponse->getTotalDataCount();
  pResp->ParameterCount = m_trans2Response.m_pResponse->getParamCount();
  pResp->ParameterOffset = m_trans2Response.m_pResponse->getParamOffset();
  pResp->ParameterDisplacement = m_trans2Response.m_pResponse->getParamDisplacement();
  pResp->DataCount = m_trans2Response.m_pResponse->getDataCount();
  pResp->DataOffset = m_trans2Response.m_pResponse->getDataOffset();
  pResp->DataDisplacement = m_trans2Response.m_pResponse->getDataDisplacement();
  pResp->SetupCount = m_trans2Response.m_pResponse->getSetupCount();
  pResp->Reserved1 = 0;
  pResp->Reserved2 = 0;

  return SendNotification();
}

DWORD cNotificationReceiver::GetWindowsSearchFilter(const DWORD SMBSearchFilter)
{
  DWORD dwRet(0);
  DWORD ShiftFilter(SMBSearchFilter);
  for (int i=0; ShiftFilter!=0; ShiftFilter = ShiftFilter >> 1, i++)
  {
    if (ShiftFilter & 1)
      dwRet |= NotifyActions[i];
  }

  return dwRet;
}

DWORD cNotificationReceiver::SendNotification()
{
  const cPtr<iRXPacket> pRXPacket = m_trans2Response.m_pResponse->getContextPacket();
  const cPtr<iTXPacket> pTXPacket = pRXPacket->getResponsePacket();
  m_trans2Response.m_pResponse->WritePacket(pTXPacket);
  pTXPacket->Send();
  return ERROR_SUCCESS;
}

DWORD cNotificationReceiver::CancelNotification(bool synchronous) 
{ 
  return STATUS_SUCCESS;
}


#endif // CIFS 