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

//cCompositeMap.cpp

#include "StdAfx.h"

#include ".\ccompositemap.h"
#include "cNotificationReceiver.h"
#include "cSMBResponse.h"

#include "..\iComposite.h"

cCompositeMap::cCompositeMap(void)
: m_Fid(2)
{
}

cCompositeMap::~cCompositeMap(void)
{
  emptyMap();
}

USHORT cCompositeMap::add(const cPtr<iComposite> pComposite)
{
  USHORT Ret(0);
  if (pComposite.isValid())
  {
    cLockGuard lg(&m_access);

    // create a unique FID
    m_Fid++;

    if (m_Fid == 0)
      m_Fid = 2;

    while (m_FidMap.find(m_Fid) != m_FidMap.end())
      m_Fid++;

    // insert the FID
    Ret = m_Fid;
    m_FidMap.insert(tFidMap::value_type(Ret, pComposite));
  }

  return Ret;
}

cPtr<iComposite> cCompositeMap::getComposite(const USHORT Fid, const cPtr<cSMBResponse> response)
{
  {
    cLockGuard lg(&m_access);
    const tFidMap::const_iterator finder = m_FidMap.find(Fid);
    if (m_FidMap.end() != finder)
      return finder->second;
  }

  response->setDosError(SMB_ERR_CLASS_DOS, SMB_ERR_BAD_FID);
  return cPtr<iComposite>();
}

bool cCompositeMap::remove(const USHORT Fid)
{
  cLockGuard lg(&m_access);
  m_FidMap.erase(Fid);

  return true;
}

void cCompositeMap::clear()
{
  emptyMap();
}

void cCompositeMap::emptyMap()
{
  cLockGuard lg(&m_access);
  m_FidMap.clear();
}

void cCompositeMap::cancelNotifications(const ULARGE_INTEGER& requestID, const cPtr<cSMBResponse> pResponse)
{
  cLockGuard lg(&m_access);

  for (tFidMap::iterator it = m_FidMap.begin()
    ; m_FidMap.end() != it
    ; ++it)
  {
    const cPtr<iComposite> pComposite = it->second;
    if (pComposite.isValid())
      pComposite->registerNotification(requestID, new cNotificationReceiver(pResponse));
  }
}