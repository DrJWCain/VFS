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

//cTreeResourceFactory.cpp

#include "StdAfx.h"
#include ".\ctreeresourcefactory.h"

#include "iShareInfo.h"
#include "cSearchLocation.h"
#include "cDirectoryFactory.h"
#include "cConstNamed.h"
#include "cShare.h"
#include "cGenTreeResource.h"
#include "cWriteInfo.h"
#include "cNotificationHandler.h"
#include "cNotifyTree.h"
#include "cVirtualDirLocation.h"
#include "iCreate.h"
#include "cSearchLocationCache.h"
#include "cRenameHandler.h"
//#include "..\..\MiscPlugins\QOptions\QOptions.h"


using namespace vfs;

cTreeResourceFactory::cTreeResourceFactory(cPtr<iShareInfo> pShareInfo
  , cPtr<iRootChildLoader> pRootChildLoader
  , const String& sName
  , const cPtr<iNameMangler> pNameMangler
  , const cPtr<iNamespaceCreator> pNamespaceCreator)
: m_pShareInfo(pShareInfo)
, m_pRootChildLoader(pRootChildLoader)
, m_sName(sName)
, m_pNameMangler(pNameMangler)
, m_pNotificationHandler(new cNotificationHandler())
, m_pNamespaceCreator(pNamespaceCreator)
, m_pSearchCache(new cSearchLocationCache())
{
}

cTreeResourceFactory::~cTreeResourceFactory(void)
{
}

cPtr<iTreeResource> cTreeResourceFactory::getTreeResource(const String& sLoginName)
{
  cLockGuard lg(&m_userTreeAccess);

  String loginName = sLoginName;
  //try
  //{
  //  //The ISA gatewways expose the resources in ISA to the world, so everyone should get a uniform view.
  //  //Further, during sQ Remote publish we need a single machine to offer consistent local views so that transient files and folders are findable across different logins.
  //  vfs::iOptionManager::singleton().checkOut(L"ISA Clip Mapping");
  //  loginName = L"vfs";
  //}
  //catch(cRecoverable&)
  //{
  //}


  const tUserTreeResources::const_iterator finder = m_UserTreeResources.find(loginName);
  if (m_UserTreeResources.end() != finder)
    return finder->second;

  const cPtr<iChildLoader> pChildLoader = m_pRootChildLoader->getChildLoader(loginName);//JCJCJC HERE
  const cPtr<iNamed> pNamed = new cConstNamed(m_sName);
  const fileInfo fi = cWriteInfo::singleton().makeFileInfo(FILE_ATTRIBUTE_DIRECTORY, pNamed);
  const cPtr<cRenameHandler> pRenameHandler = new cRenameHandler(cPtr<cTreeResourceFactory>(this, this), loginName);
  const cPtr<cSearchListener> pSearchListener = new cSearchListener(m_pNotificationHandler
    , m_pNameMangler
    , cPtr<cMemoryView>()
    , m_pSearchCache
    , pRenameHandler);
  const cPtr<iNotifyTree> pNotifyTree = new cNotifyTree(m_pNotificationHandler);
  const cPtr<iDirLocation> pDirLocation = new cVirtualDirLocation(fi
    , pSearchListener
    , pChildLoader
    , cPtr<cSearchListener>()
    , ptr_cast<const cPtr<iRename> >(pChildLoader)
    , pNotifyTree
    , m_pNotificationHandler
    , ptr_cast<const cPtr<iCreate> >(pChildLoader)
    , cPtr<iFileEvent>()
    , pRenameHandler);

  const cPtr<cSearchLocation> pRootSearch = new cSearchLocation(pDirLocation
    , pSearchListener
    , pNotifyTree
    , pChildLoader
    , m_pSearchCache);

  const cPtr<cDirectoryFactory> pDirectoryFactory(new cDirectoryFactory(pRootSearch, m_pNameMangler));
  cPtr<cShare> pShare = new cShare(m_pShareInfo
    , pDirectoryFactory
    , pNamed);
  if (pShare.isNull())
  {
    QSOS((L"cTreeResourceFactory::getTreeResource() - pShare.isNull()"));
    return cPtr<iTreeResource>();
  }

  const cPtr<cGenTreeResource> pGenTreeResource = new cGenTreeResource(pShare
    , pRenameHandler
    , m_pNameMangler
    , m_pNamespaceCreator);
  if (pGenTreeResource.isValid())
    m_UserTreeResources[loginName] = pGenTreeResource;

  return pGenTreeResource;
}


//-------------------------------------------------------------------------------------------
void cTreeResourceFactory::removeTreeResource(const vfs::String& sLoginName)
{
  cLockGuard lg(&m_userTreeAccess);

  tUserTreeResources::iterator it = m_UserTreeResources.find(sLoginName);
  if (m_UserTreeResources.end() != it)
    m_UserTreeResources.erase(it);

  m_pRootChildLoader->removeChildLoader(sLoginName);
}


cPtr<iDirLocation> cTreeResourceFactory::get(const String& sPath, const String& sUserName) const
{
  const cPtr<cGenTreeResource> pGenTreeResource(getExistingTreeResource(sUserName));
  if (pGenTreeResource.isNull())
    return cPtr<iDirLocation>();

  return pGenTreeResource->get(sPath);
}

cPtr<iDirLocation> cTreeResourceFactory::getParent(const String& sPath, const String& sUserName) const
{
  const cPtr<cGenTreeResource> pGenTreeResource(getExistingTreeResource(sUserName));
  if (pGenTreeResource.isNull())
    return cPtr<iDirLocation>();

  return pGenTreeResource->getParent(sPath);
}

cPtr<cGenTreeResource> cTreeResourceFactory::getExistingTreeResource(const String& sUserName) const
{
  cLockGuard lg(&m_userTreeAccess);
  const tUserTreeResources::const_iterator finder = m_UserTreeResources.find(sUserName);
  if (m_UserTreeResources.end() != finder)
    return finder->second;
  return cPtr<cGenTreeResource>();
}