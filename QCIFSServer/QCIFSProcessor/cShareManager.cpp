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

//cShareManager.h

#include "StdAfx.h"
#include ".\csharemanager.h"

#include "iTreeResourceFactory.h"
#include "iTreeChangeListener.h"
#include <algorithm>

using namespace vfs;

cShareManager::cShareManager(void)
{
}

cShareManager::~cShareManager(void)
{
}

bool cShareManager::HasShare(const String& sShareName) const
{
  cLockGuard lg(&m_access);
  return m_shareMap.end() !=  m_shareMap.find(sShareName);
}

cPtr<iTreeResourceFactory> cShareManager::Get(const String& sShareName
  , const cPtr<iTreeChangeListener>& pTreeChangeListener) const
{
  const String sShareUpper(toUpper(sShareName));

  cLockGuard lg(&m_access);
  tShareMap::const_iterator finder(m_shareMap.find(sShareUpper));
  if (m_shareMap.end() == finder)
    return cPtr<iTreeResourceFactory>();

  if (pTreeChangeListener.isValid())
  {
    const tTreeChangeMap::iterator finder = m_changeMap.find(sShareUpper);
    if (m_changeMap.end() != finder)
    {
      tTreeChangeList& rChangeList = finder->second;
      rChangeList.insert(pTreeChangeListener);
    }
  }

  return finder->second;
}
//
//bool cShareManager::Add(const cPtr<iTreeResourceFactory> pTreeResourceFactory
//  , const bool bReplaceExisting)
//{
//  if (pDefaultFactory.isNull() || bReplaceExisting)
//  {  
//    cLockGuard lg(&m_access);
//    if (pDefaultFactory.isValid())
//    {
//      for (tTreeChangeList::const_iterator cit = m_DefaultChangeListeners.begin()
//        ; m_DefaultChangeListeners.end() != cit
//        ; ++cit)
//      {
//        const cPtr<iTreeChangeListener>& rChangeListener = *cit;
//        if (rChangeListener.isValid())
//          rChangeListener->replaceTree(L"");
//      }
//      m_DefaultChangeListeners.clear();
//    }
//    
//    pDefaultFactory = pTreeResourceFactory;
//    QTRACE((L"cShareManager::Add() - default share set"));
//    return true;
//  }
//
//  QSOS((L"cShareManager::Add() - default share already set"));
//  return false;
//}

bool cShareManager::Add(const cPtr<iTreeResourceFactory>& pTreeResourceFactory
  , const String& sShareName
  , const bool bReplaceExisting)
{
  const String sShareUpper(toUpper(sShareName));

  cLockGuard lg(&m_access);
  if (bReplaceExisting)
  {
    const tTreeChangeMap::iterator finder = m_changeMap.find(sShareUpper);
    if (m_changeMap.end() != finder)
    {
      tTreeChangeList& rChangeList = finder->second;
      for (tTreeChangeList::const_iterator cit = rChangeList.begin()
        ; rChangeList.end() != cit
        ; ++cit)
      {
        const cPtr<iTreeChangeListener>& rChangeListener = *cit;
        if (rChangeListener.isValid())
          rChangeListener->replaceTree(sShareUpper, pTreeResourceFactory);
      }
    }
    else
      m_changeMap[sShareUpper] = tTreeChangeList();

    m_shareMap[sShareUpper] = pTreeResourceFactory;
    QTRACE((L"cShareManager::Add() - replaced %s", sShareUpper.c_str()));
  }
  else
  {
    tShareMap::_Pairib result = m_shareMap.insert(tShareMap::value_type(sShareUpper, pTreeResourceFactory));
    if (!result.second)
      QSOS((L"cShareManager::Add() - share %s already set", sShareUpper.c_str()));
    else
    {
      QTRACE((L"cShareManager::Add() - %s", sShareUpper.c_str()));
      m_changeMap[sShareUpper] = tTreeChangeList();
    }
    return result.second;
  }

  return true;
}

void cShareManager::forgetListener(const vfs::cPtr<iTreeChangeListener>& pTreeChangeListener) const
{
  cLockGuard lg(&m_access);
  for (tTreeChangeMap::iterator it = m_changeMap.begin()
    ; m_changeMap.end() != it
    ; ++it)
  {
    tTreeChangeList& rTreeChangeList(it->second);
    //QTRACE((L"cShareManager::forgetListener() - change map length is %d before", rTreeChangeList.size()));
    rTreeChangeList.erase(pTreeChangeListener);
    //QTRACE((L"cShareManager::forgetListener() - change map length is %d after", rTreeChangeList.size()));
  }
}

std::map<vfs::String, vfs::cPtr<iTreeResourceFactory> > cShareManager::getRegisteredShares() const
{
  return m_shareMap;
}