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

//cDirectoryFactory.h

#pragma once

#include "idirectoryfactory.h"
#include "cSearchLocation.h"

class iNameMangler;
class iNotificationReceiver;

class cDirectoryFactory : public vfs::cRefCount
{
public:
  typedef std::vector<vfs::cPtr<cSearchLocation> > tSrchList;

  cDirectoryFactory(vfs::cPtr<cDirectoryFactory> pDirectoryFactory);
  cDirectoryFactory(const vfs::cPtr<cSearchLocation> pRoot);
  cDirectoryFactory(const vfs::cPtr<cSearchLocation> pRoot, const vfs::cPtr<iNameMangler> pNameMangler);

  virtual ~cDirectoryFactory(void);

  //public member functions
  vfs::cPtr<iDirLocation> get(const vfs::String& sPath) const;
  vfs::cPtr<iDirLocation> getParent(const vfs::String& sPath) const;
  DWORD getSearchResults(const vfs::String& sSearchPath, tFileInfoList& fileInfoList) const;

private:  //private member functions
  void parsePath(const vfs::String& sPath, tSrchList& dirList) const;
  vfs::String removeEndName(const vfs::String& sPath) const;
  inline vfs::cPtr<iDirLocation> getOnlyDirLocation(const tSrchList& dirList) const;
  inline void makeRootIntoSearchList(tSrchList& dirList) const;
  void getMatchingChildren(const tSrchList& dirlist
    , const vfs::String& sMatchPattern
    , tSrchList& childList) const;
  inline bool isWildcardSearch(const vfs::String& sMatchPattern) const;
  void getWildcardSearchResults(const vfs::String& sMatchPattern
    , const vfs::cPtr<iNotificationReceiver> pWatchSubtree
    , const vfs::cPtr<cSearchLocation> pParentSearchLocation
    , tSrchList& matchingChildren) const;

private:  //private member variables
  vfs::cPtr<cSearchLocation> m_pRoot;
  vfs::cPtr<iNameMangler> m_pNameMangler;
};