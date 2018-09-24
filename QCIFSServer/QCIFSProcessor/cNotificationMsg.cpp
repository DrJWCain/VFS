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

//cNotificationMsg.cpp

#include "StdAfx.h"

#include ".\cnotificationmsg.h"
#include "iNotificationReceiver.h"

#include "..\..\KernelSDK\include\QWinBase.h"

using namespace vfs;

const int nNotificationBytes(0x1000);

cNotificationMsg::cNotificationMsg(const HANDLE hFile, vfs::cPtr<iNotificationReceiver> pNotification)
: m_pNotificationReceiver(pNotification)
, m_dwRet(ERROR_SUCCESS)
, m_pNotificationMem(new cMemoryView(new cMemory((size_t)nNotificationBytes, vfs::cMemory::eVirtual), 0, (size_t)nNotificationBytes))
{
  IO_DATA* const pIOData = new IO_DATA;
  ZeroMemory(pIOData, sizeof(IO_DATA));
  pIOData->pMsg = cPtr<iCompletionMsg>(this, this);
  DWORD dw;
  if (FALSE == ReadDirectoryChangesW(hFile
    , m_pNotificationMem->getBytes()
    , nNotificationBytes
    , pNotification->WatchSubtree()
    , pNotification->SearchFilter()
    , &dw
    , reinterpret_cast<LPOVERLAPPED>(pIOData)
    , NULL))
  {
    m_dwRet = GetLastError();
    if (ERROR_IO_PENDING == m_dwRet)
      m_dwRet = ERROR_SUCCESS;
    else
      QSOS((L"cNotificationMsg::cNotificationMsg() - failed to get Directory Notification Changes with error %d - %s"
        , m_dwRet
        , win32ErrorToString(m_dwRet).c_str()));
  }
  else
  {
    QTRACE((L"cNotificationMsg::cNotificationMsg() - Got Directory Notification Changes"));
  }
}

cNotificationMsg::~cNotificationMsg(void)
{
}

void cNotificationMsg::deliver(ULONG_PTR pCompletionKey, const DWORD dwBytes)
{
  if (dwBytes > 0)
  {
    PUCHAR pBytes = reinterpret_cast<PUCHAR>(m_pNotificationMem->getBytes());
    iNotificationTarget::tNotifyList notifyList;
    PFILE_NOTIFY_INFORMATION pNotifyInfo;
    do
    {
      pNotifyInfo = reinterpret_cast<PFILE_NOTIFY_INFORMATION>(pBytes);
      notifyList.push_back(iNotificationTarget::tNotifyList::value_type(pNotifyInfo->Action, pNotifyInfo->FileName));
      pBytes += pNotifyInfo->NextEntryOffset;
    }
    while (pNotifyInfo->NextEntryOffset > 0);
    m_pNotificationReceiver->Notify(notifyList);
  }
  else
    m_pNotificationReceiver->Notify();
}

void cNotificationMsg::err(ULONG_PTR pCompletionKey, const DWORD err)
{
  QSOS((L"cNotificationMsg::err() - Failed to get change notification with error %d - %s"
      , err
      , win32ErrorToString(err).c_str()));
}