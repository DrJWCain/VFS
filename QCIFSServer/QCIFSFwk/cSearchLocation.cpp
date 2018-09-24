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

//cSearchLocation.cpp

#include "StdAfx.h"

#include ".\csearchlocation.h"

#include "iDirLocation.h"
#include "iChildLoader.h"
#include "cNotificationHandler.h"
#include "cVirtualDirLocation.h"
#include "cSearchLocationCache.h"

using namespace vfs;

cSearchLocation::cSearchLocation(const cPtr<cSearchLocation> pOther)
: m_pDirLocation(pOther->m_pDirLocation)
, m_pSearchListener(pOther->m_pSearchListener)
, m_pNotifyTree(pOther->m_pNotifyTree)
, m_pChildLoader(pOther->m_pChildLoader)
, m_pSearchCache(pOther->m_pSearchCache)
{
}

cSearchLocation::cSearchLocation(const cPtr<iDirLocation> pDirLocation
  , const cPtr<cSearchListener> pSearchListener
  , const cPtr<iNotifyTree> pNotifyTree
  , const cPtr<iChildLoader> pChildLoader
  , const cPtr<cSearchLocationCache> pSearchCache)
: m_pDirLocation(pDirLocation)
, m_pSearchListener(pSearchListener)
, m_pNotifyTree(pNotifyTree)
, m_pChildLoader(pChildLoader)
, m_pSearchCache(pSearchCache)
{
}

cSearchLocation::~cSearchLocation(void)
{
  //QTRACE((L"cSearchLocation::~cSearchLocation() {0x%08x}", this));
  m_pSearchCache->forget(m_pChildLoader);
}

// public member functions
cPtr<iDirLocation> cSearchLocation::getDirLocation()
{
  return m_pDirLocation;
}

void cSearchLocation::getChildren(cSearchListener::tDirList& children)
{
  if (m_pSearchListener.isValid())
    children = m_pSearchListener->getChildren(m_pChildLoader);
}

cPtr<cSearchLocation> cSearchLocation::getChild(const String& sName)
{
  if (m_pSearchListener.isValid())
    return m_pSearchListener->getChild(sName, m_pChildLoader);

  return cPtr<cSearchLocation>();
}

void cSearchLocation::clear()
{
  if (m_pSearchListener.isValid())
    m_pSearchListener->clear();
  m_pSearchCache->forget(m_pChildLoader);
}


cPtr<iNotificationReceiver> cSearchLocation::getSubtreeWatch()
{
  if (m_pNotifyTree.isValid())
    return m_pNotifyTree->getSubtreeWatch();

  return cPtr<iNotificationReceiver>();
}

void cSearchLocation::addSubtreeWatch(const cPtr<iNotificationReceiver> pNotificationReceiver)
{
  if (m_pNotifyTree.isValid())
    m_pNotifyTree->addSubtreeWatch(pNotificationReceiver);
}