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

//cCompositeMap.h

#pragma once

class iComposite;
class cSMBResponse;
class cSMB2Response;
class cNotificationReceiver;
class cSMB2NotificationReceiver;

template <typename FidType>
class cCompositeMapTraits;

template <>
class cCompositeMapTraits<USHORT>
{
public:
  typedef vfs::cPtr<cSMBResponse> respType;
  typedef cNotificationReceiver notifyType;
};

template <>
class cCompositeMapTraits<ULONGLONG>
{
public:
  typedef vfs::cPtr<cSMB2Response> respType;
  typedef cSMB2NotificationReceiver notifyType;
};


template <typename FidType
  , typename NotifyIndex
  , typename RespType = cCompositeMapTraits<FidType>::respType
  , typename NotifyType = cCompositeMapTraits<FidType>::notifyType>
class cCompositeMap
{
  typedef std::pair<vfs::cPtr<iComposite>, vfs::String> tCompositePathPair;
  typedef std::map<FidType, tCompositePathPair > tFidMap;
  vfs::cLock m_access;
  FidType m_Fid;
  tFidMap m_FidMap;

public:
  cCompositeMap(void) : m_Fid(2){}
  ~cCompositeMap(void) {emptyMap();}

  typedef std::pair<FidType, vfs::String> tFidPathPair;

  FidType allocateFid()
  {
    vfs::cLockGuard lg(&m_access);
    // create a unique FID
    m_Fid++;
    if (m_Fid <= 0) //in case using signed type or wrap round
      m_Fid = 2;
    while (m_FidMap.find(m_Fid) != m_FidMap.end())
      m_Fid++;
    return m_Fid;
  }

  FidType add(const vfs::cPtr<iComposite>& pComposite, const vfs::String& sPath)
  {
    if (pComposite.isNull())
      return 0;

    vfs::cLockGuard lg(&m_access);

    FidType fid = allocateFid();

    // insert the FID
    m_FidMap.insert(tFidMap::value_type(fid, tCompositePathPair(pComposite, sPath)));
    return fid;
  }

  void addFid(const FidType Fid, const vfs::cPtr<iComposite>& pComposite, const vfs::String& sPath)
  {
    vfs::cLockGuard lg(&m_access);
    m_FidMap[Fid] = tCompositePathPair(pComposite, sPath);
  }

  vfs::cPtr<iComposite> getComposite(const FidType Fid, const RespType& response)
  {
    {
      vfs::cLockGuard lg(&m_access);
      const tFidMap::const_iterator finder = m_FidMap.find(Fid);
      if (m_FidMap.end() != finder)
        return finder->second.first;
    }

    if(response.isValid())
      response->setDosError(SMB_ERR_CLASS_DOS, SMB_ERR_BAD_FID);
    return cPtr<iComposite>();
  }

  void remove(const FidType Fid)
  {
    vfs::cLockGuard lg(&m_access);
    m_FidMap.erase(Fid);
  }

  void clear()
  {
    emptyMap();
  }

  void cancelNotifications(const NotifyIndex& requestID, bool synchronous)
  {
    vfs::cLockGuard lg(&m_access);
    for (tFidMap::iterator it = m_FidMap.begin()
      ; m_FidMap.end() != it
      ; ++it)
    {
      const cPtr<iComposite>& pComposite = it->second.first;
      if (pComposite.isValid())
      {
        pComposite->cancelNotification(requestID, synchronous);
      }
    }
  }

  void triggerAllNotificationsAndClear(std::vector<tFidPathPair>& rFidPathPair)
  {
    vfs::cLockGuard lg(&m_access);
    rFidPathPair.reserve(m_FidMap.size());
    for (tFidMap::iterator it = m_FidMap.begin()
      ; m_FidMap.end() != it
      ; ++it)
    {
      const cPtr<iComposite>& pComposite = it->second.first;
      if (pComposite.isValid())
      {
        pComposite->triggerNotifications();
        rFidPathPair.push_back(tFidPathPair(it->first, it->second.second));
      }
    }
    m_FidMap.clear();
  }

  vfs::String getPath(const FidType& fid)
  {
    vfs::cLockGuard lg(&m_access);
    const tFidMap::const_iterator finder = m_FidMap.find(fid);
    if (m_FidMap.end() != finder)
      return finder->second.second;
    return vfs::String();
  }

private:
  void emptyMap()
  {
    cLockGuard lg(&m_access);
    m_FidMap.clear();
  }
};
