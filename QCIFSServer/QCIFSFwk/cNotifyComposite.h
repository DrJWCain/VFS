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

//cNotifyComposite.h

#pragma once

#include "cDeletableComposite.h"
#include "iNotifyTree.h"
#include "cNotificationHandler.h"

class cNotifyComposite : public cDeletableComposite
{
  ULONGLONG RequestID;
public:
  cNotifyComposite(const fileInfo& fi
    , const vfs::cPtr<cSearchListener> pParentSearchListener
    , const vfs::cPtr<iNotifyTree> pNotifyTree
    , const vfs::cPtr<cNotificationHandler> pNotificationHandler
    , const vfs::cPtr<iFileEvent> fileEvent);
  ~cNotifyComposite(void);

  //iComposite
  DWORD registerNotification(const ULONGLONG requestID
    , vfs::cPtr<iNotificationReceiver> pNotification);
  DWORD cancelNotification(const ULONGLONG requestID, bool synchronous);
  void triggerNotifications();

private:
  const vfs::cPtr<iNotifyTree> m_pNotifyTree;
  const vfs::cPtr<cNotificationHandler> m_pNotificationHandler;
};
