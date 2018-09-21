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

//cNotificationHandler.cpp

#include "StdAfx.h"
#include ".\cnotificationhandler.h"

using namespace vfs;

cNotificationHandler::cNotificationHandler()
{
}

cNotificationHandler::~cNotificationHandler(void)
{
}

DWORD cNotificationHandler::registerNotification(const ULONGLONG requestID
  , vfs::cPtr<iNotificationReceiver> pNotification)
{
  {
    cLockGuard lg(&m_access);
    m_Notifications[requestID] = pNotification;
    if (pNotification->WatchSubtree())
      m_bWatchSubtree = true;
  }

  return ERROR_SUCCESS;
}

//iNotificationTarget
DWORD cNotificationHandler::Notify()
{
  const tNotifications notifications(copyNotifications());
  for (tNotifications::const_iterator cit = notifications.begin()
    ; notifications.end() != cit
    ; ++cit)
  {
    //QTRACE((L"cNotificationHandler::Notify %I64d", cit->first));
    const cPtr<iNotificationTarget> pNT = cit->second;
    pNT->Notify();
  }
  return ERROR_SUCCESS;
}

DWORD cNotificationHandler::Notify(const tNotifyList& notifyInfo)
{
  const tNotifications notifications(copyNotifications());

  for (tNotifications::const_iterator cit = notifications.begin()
    ; notifications.end() != cit
    ; ++cit)
  {
    //QTRACE((L"cNotificationHandler::Notify %I64d %Id", cit->first, notifyInfo.size()));
    const cPtr<iNotificationTarget> pNT = cit->second;
    pNT->Notify(notifyInfo);
  }
  return ERROR_SUCCESS;
}

BOOL cNotificationHandler::WatchSubtree()
{
  return m_bWatchSubtree ? TRUE : FALSE;
}

DWORD cNotificationHandler::SearchFilter()
{
  return 0;
}

inline cNotificationHandler::tNotifications cNotificationHandler::copyNotifications()
{
  tNotifications ret;
  {
    cLockGuard lg(&m_access);
    ret.swap(m_Notifications);
    m_bWatchSubtree = false;
  }
  return ret;
}

DWORD cNotificationHandler::cancelNotification(const ULONGLONG requestID, bool synchronous)
{
  cPtr<iNotificationTarget> pNT;
  {
    cLockGuard lg(&m_access);
    for (tNotifications::iterator cit = m_Notifications.begin()
      ; m_Notifications.end() != cit
      ; ++cit)
    {
      if(requestID == cit->first)
      {
        pNT = cit->second;
        m_Notifications.erase(cit);
        break;
      }
    }
  }
  if(pNT.isValid())
    return pNT->CancelNotification(synchronous);
  return ERROR_NOT_FOUND;
}

DWORD cNotificationHandler::CancelNotification(bool synchronous)
{
  return ERROR_SUCCESS;
}