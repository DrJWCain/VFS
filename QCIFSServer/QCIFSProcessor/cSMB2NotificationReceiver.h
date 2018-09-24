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

//cSMB2NotificationReceiver.h

#pragma once

#include "inotificationreceiver.h"

class cSMB2Response;
class cGuardedUniqueNotify;

class cSMB2NotificationReceiver : public iNotificationReceiver, public vfs::cRefCount
{
public:
  cSMB2NotificationReceiver(const vfs::cPtr<cSMB2Response> pResponse
    , const DWORD CompletionFilter
    , const vfs::cPtr<cGuardedUniqueNotify>& pGuard);
  ~cSMB2NotificationReceiver(void);

  //iNotificationReceiver
  DWORD Notify();
  DWORD Notify(const tNotifyList& notifyInfo);
  BOOL WatchSubtree() { return FALSE;}
  DWORD SearchFilter() { return m_dwSearchFilter;}
  DWORD CancelNotification(bool synchronous);

private:
  DWORD SendNotification();

private:
  const vfs::cPtr<cSMB2Response> m_pResponse;
  const DWORD m_dwSearchFilter;
  const vfs::cPtr<cGuardedUniqueNotify> m_pGuard;
};
