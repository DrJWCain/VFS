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

//iUniqueNotifyID.h

#pragma once

#include "cNotificationID.h"

class cGuardedUniqueNotify;

class iUniqueNotifyID : public vfs::iManager
{
public:
  QDEFINE_SINGLETON_IFACE(iUniqueNotifyID, L"{369BA3EC-52FA-4e38-9210-7467DF63EAB7} // iUniqueNotifyID", vfs::iManager);

  virtual ~iUniqueNotifyID() {}

  virtual vfs::cPtr<cGuardedUniqueNotify> getUniqueNotifyID(const cNotificationID& rNID) = 0;
  virtual ULONGLONG cancelNotifyID(const cNotificationID& rNID) = 0;
};