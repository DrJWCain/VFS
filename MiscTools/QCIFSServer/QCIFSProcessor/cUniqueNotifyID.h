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

// cQCIFSProcessor.h

#pragma once

#include "iUniqueNotifyID.h"

class cUniqueNotifyID : public iUniqueNotifyID, public vfs::iGuardCreator, public vfs::cRefCount
{
  typedef std::map<cNotificationID, ULONGLONG> tIdMap;

public:
  QDEFINE_SINGLETON_IMPL(cUniqueNotifyID, L"{C2DB6A58-2C0B-4a85-9271-2FBE60A79A3A} // cUniqueNotifyID", iUniqueNotifyID);

  cUniqueNotifyID();
  ~cUniqueNotifyID(void);

  void QAPI logSummary(){QTRACE((L"cUniqueNotifyID"));}
  void QAPI logDetailedSummary(){QTRACE((L"cUniqueNotifyID - generate unique notification IDs"));}

  //iUniqueNotifyID
  vfs::cPtr<cGuardedUniqueNotify> getUniqueNotifyID(const cNotificationID& rNID);
  ULONGLONG cancelNotifyID(const cNotificationID& rNID);

  //iGuardCreator
  void QAPI onGuardDestroyed(iGuarded::Ptr Guarded);

private:
  volatile LONG m_notifyID;
  vfs::cLock m_access;
  tIdMap m_IDMap;
};