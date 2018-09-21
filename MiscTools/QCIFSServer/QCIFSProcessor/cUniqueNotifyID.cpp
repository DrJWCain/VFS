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

//cUniqueNotifyID.cpp

#include "stdafx.h"

#include "cUniqueNotifyID.h"
#include "cGuardedUniqueNotify.h"

using namespace vfs;

cUniqueNotifyID::cUniqueNotifyID()
: m_notifyID(1)
{
}

cUniqueNotifyID::~cUniqueNotifyID(void)
{
}

cPtr<cGuardedUniqueNotify> cUniqueNotifyID::getUniqueNotifyID(const cNotificationID& rNID)
{
  //not using InterlockedCompareExchange64 since this may be deployed on WinXP
  InterlockedCompareExchange(&m_notifyID, 1, LONG_MAX);
  cLockGuard lg(&m_access);

  tIdMap::const_iterator it = m_IDMap.begin();
  while (it != m_IDMap.end())
  {
    if (it->second == m_notifyID)
    {
      InterlockedIncrement(&m_notifyID);
      it = m_IDMap.begin();
    }
    ++it;
  }

  m_IDMap[rNID] = m_notifyID;

  //QTRACE((L"cUniqueNotifyID::getUniqueNotifyID() - notify map size is %d", m_IDMap.size()));

  return new cGuardedUniqueNotify(m_notifyID, rNID, cPtr<iGuardCreator>(this, this));
}

ULONGLONG cUniqueNotifyID::cancelNotifyID(const cNotificationID& rNID)
{
  cLockGuard lg(&m_access);

  tIdMap::iterator finder = m_IDMap.find(rNID);
  if (m_IDMap.end() == finder)
    return 0;

  const ULONGLONG ret = finder->second;
  m_IDMap.erase(finder);
  return ret;
}

void QAPI cUniqueNotifyID::onGuardDestroyed(iGuarded::Ptr Guarded)
{
  const cPtr<cGuardNotificationID> pNotifyID = ptr_cast<cPtr<cGuardNotificationID> >(Guarded);
  if (pNotifyID.isValid())
  {
    cLockGuard lg(&m_access);
    m_IDMap.erase(pNotifyID->getNid());
  }
}