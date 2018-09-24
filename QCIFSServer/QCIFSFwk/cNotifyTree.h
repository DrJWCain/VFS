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

//cNotifyTree .h

#pragma once

#include "inotifytree.h"
#include "cSearchListener.h"

class cNotificationHandler;

class cNotifyTree : public iNotifyTree, public vfs::cRefCount
{
public:
  cNotifyTree(const vfs::cPtr<cNotificationHandler> pNotificationHandler);
  ~cNotifyTree(void);

  vfs::cPtr<iNotificationReceiver> getSubtreeWatch();
  void addSubtreeWatch(vfs::cPtr<iNotificationReceiver> pNotificationReceiver);

private:
  const vfs::cPtr<cNotificationHandler> m_pNotificationHandler;
};