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

//cNotificationReceiver.h
#pragma once

#include "inotificationreceiver.h"
#include "cTrans2Response.h"

class cSMBResponse;
class cGuardedUniqueNotify;

class cNotificationReceiver : public iNotificationReceiver, public vfs::cRefCount
{
public:
  cNotificationReceiver(const vfs::cPtr<cSMBResponse> pSMBResponse
    , const BOOL bWatchSubtree
    , const DWORD SearchFilter
    , const vfs::cPtr<cGuardedUniqueNotify>& pGuard);
  ~cNotificationReceiver(void);

  //iNotificationReceiver
  DWORD Notify();
  DWORD Notify(const tNotifyList& notifyInfo);
  BOOL WatchSubtree() { return m_bWatchSubtree;}
  DWORD SearchFilter() { return m_dwSearchFilter;}
  DWORD CancelNotification(bool synchronous);

private:
  DWORD GetWindowsSearchFilter(const DWORD SMBSearchFilter);
  DWORD SendNotification();

private:
  const cTrans2Response m_trans2Response;
  const BOOL m_bWatchSubtree;
  const DWORD m_dwSearchFilter;
  const vfs::cPtr<cGuardedUniqueNotify> m_pGuard;
};