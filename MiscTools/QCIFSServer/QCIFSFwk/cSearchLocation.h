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

//cSearchLocation.h

#pragma once

#include "iChildLoaderVisitor.h"
#include "iNamed.h"
#include "cSearchListener.h"
#include "iNotifyTree.h"
#include "iDirLocation.h"

class cSearchLocation : public iNotifyTree, public vfs::cRefCount
{
public:
  cSearchLocation(const vfs::cPtr<cSearchLocation> pOther);
  cSearchLocation(const vfs::cPtr<iDirLocation> pDirLocation
    , const vfs::cPtr<cSearchListener> pSearchListener
    , const vfs::cPtr<iNotifyTree> pNotifyTree
    , const vfs::cPtr<iChildLoader> pChildLoader
    , const vfs::cPtr<cSearchLocationCache>pSearchCache);
  ~cSearchLocation(void);

  //iNotifyTree
  vfs::cPtr<iNotificationReceiver> getSubtreeWatch();
  void addSubtreeWatch(vfs::cPtr<iNotificationReceiver> pNotificationReceiver);

  // public member functions
  vfs::cPtr<iDirLocation> getDirLocation();
  void getChildren(cSearchListener::tDirList& children);
  vfs::cPtr<cSearchLocation> getChild(const vfs::String& sName);
  void clear();

private:  //private member variables
  //const vfs::cConstPtr<iNamed> m_pNamed;
  const vfs::cPtr<iDirLocation> m_pDirLocation;
  const vfs::cPtr<cSearchListener> m_pSearchListener;
  const vfs::cPtr<iNotifyTree> m_pNotifyTree;
  const vfs::cPtr<iChildLoader> m_pChildLoader;
  const vfs::cPtr<cSearchLocationCache>m_pSearchCache;
};