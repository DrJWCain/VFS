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

//cNotificationHandler.h

#pragma once

#include "..\iComposite.h"
#include "..\qcifsprocessor\inotificationreceiver.h"

class cNotificationHandler : public iNotificationReceiver, public vfs::cRefCount
{
  typedef std::map<ULONGLONG, vfs::cPtr<iNotificationTarget> > tNotifications;

public:
  cNotificationHandler(void);
  ~cNotificationHandler(void);

  //iNotificationTarget
  DWORD Notify();
  DWORD Notify(const tNotifyList& notifyInfo);
  BOOL WatchSubtree();
  DWORD SearchFilter();
  DWORD CancelNotification(bool synchronous);

  // public member functions
  DWORD registerNotification(const ULONGLONG requestID
    , vfs::cPtr<iNotificationReceiver> pNotification);
  DWORD cancelNotification(const ULONGLONG requestID, bool synchronous);

private:  //private member functions
  inline tNotifications copyNotifications();

private:  //private member variables
  bool m_bWatchSubtree;

  mutable vfs::cLock m_access;
  tNotifications m_Notifications;
};