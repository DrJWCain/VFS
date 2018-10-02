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

//iShareManager.h

#pragma once

class iTreeResourceFactory;
class iTreeChangeListener;

class iShareManager
{
public:
  virtual ~iShareManager(){};

  virtual bool HasShare(const vfs::String& sShareName) const = 0;
  virtual vfs::cPtr<iTreeResourceFactory> Get(const vfs::String& sShareName
    , const vfs::cPtr<iTreeChangeListener>& pTreeChangeListener) const = 0;
  //virtual bool Add(const vfs::cPtr<iTreeResourceFactory> pTreeResourceFactory
  //  , bool bReplaceExisting) = 0;
  virtual bool Add(const vfs::cPtr<iTreeResourceFactory>& pTreeResourceFactory
    , const vfs::String& sShareName
    , bool bReplaceExisting) = 0;
  virtual void forgetListener(const vfs::cPtr<iTreeChangeListener>& pTreeChangeListener) const = 0;
  virtual std::map<vfs::String, vfs::cPtr<iTreeResourceFactory> > getRegisteredShares() const = 0;
};