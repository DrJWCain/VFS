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

#pragma once

#include "isharemanager.h"

class cShareManager : public iShareManager, public vfs::cRefCount
{
public:
  cShareManager(void);
  ~cShareManager(void);

  bool HasShare(const vfs::String& sShareName) const;
  vfs::cPtr<iTreeResourceFactory> Get(const vfs::String& sShareName
    , const vfs::cPtr<iTreeChangeListener>& pTreeChangeListener) const;
  //bool Add(const vfs::cPtr<iTreeResourceFactory> pTreeResourceFactory
  //  , bool bReplaceExisting);
  bool Add(const vfs::cPtr<iTreeResourceFactory>& pTreeResourceFactory
    , const vfs::String& sShareName
    , bool bReplaceExisting);
  void forgetListener(const vfs::cPtr<iTreeChangeListener>& pTreeChangeListener) const;

private:
  typedef std::map<vfs::String, vfs::cPtr<iTreeResourceFactory> > tShareMap;
  typedef std::set<vfs::cPtr<iTreeChangeListener> > tTreeChangeList;
  typedef std::map<vfs::String, tTreeChangeList> tTreeChangeMap;

  mutable vfs::cLock m_access;
  tShareMap m_shareMap;
  mutable tTreeChangeMap m_changeMap;
  //vfs::cPtr<iTreeResourceFactory> pDefaultFactory;
  //mutable tTreeChangeList m_DefaultChangeListeners;
};