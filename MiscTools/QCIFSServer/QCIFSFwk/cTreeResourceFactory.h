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

//cTreeResourceFactory.h

#pragma once

#include "iRootChildLoader.h"

#include "iNameMangler.h"

#include "..\QCIFSFwk\iQCIFSFwk.h"
#include "..\qcifsprocessor\itreeresourcefactory.h"

class iShareInfo;
class cSearchLocation;
class cGenTreeResource;
class cNotificationHandler;
class cSearchLocationCache;
class iDirLocation;

class cTreeResourceFactory : public iTreeResourceFactory
  , public vfs::cRefCount
{
  typedef std::map<vfs::String, vfs::cPtr<cGenTreeResource> > tUserTreeResources;
public:
  cTreeResourceFactory(const vfs::cPtr<iShareInfo> pShareInfo
    , const vfs::cPtr<iRootChildLoader> pRootChildLoader
    , const vfs::String& sName
    , const vfs::cPtr<iNameMangler> pNameMangler
    , const vfs::cPtr<iNamespaceCreator> pNamespaceCreator);
  ~cTreeResourceFactory(void);

  //iTreeResourceFactory
  vfs::cPtr<iTreeResource> getTreeResource(const vfs::String& sLoginName);
  void removeTreeResource(const vfs::String& sLoginName);

  //public member functions
  vfs::cPtr<iDirLocation> get(const vfs::String& sPath, const vfs::String& sUserName) const;
  vfs::cPtr<iDirLocation> getParent(const vfs::String& sPath, const vfs::String& sUserName) const;

private:  //private member functions
  vfs::cPtr<cGenTreeResource> getExistingTreeResource(const vfs::String& sUserName) const;

private:  //private member variables
  const vfs::cPtr<iShareInfo> m_pShareInfo;
  const vfs::cPtr<iRootChildLoader> m_pRootChildLoader;
  const vfs::String m_sName;
  const vfs::cPtr<iNameMangler> m_pNameMangler;
  const vfs::cPtr<cNotificationHandler> m_pNotificationHandler;
  const vfs::cPtr<cSearchLocationCache> m_pSearchCache;

  mutable vfs::cLock m_userTreeAccess;
  tUserTreeResources m_UserTreeResources;
  const vfs::cPtr<iNamespaceCreator> m_pNamespaceCreator;
};