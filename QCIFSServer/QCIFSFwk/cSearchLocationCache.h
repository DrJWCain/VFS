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

//cSearchLocationCache.h

#pragma once

class iChildLoader;
class cSearchLocation;

class cSearchLocationCache : public vfs::cRefCount
{
  typedef std::map<vfs::cPtr<iChildLoader>, vfs::cPtr<cSearchLocation> > tMap;

public:
  cSearchLocationCache(){}
  ~cSearchLocationCache(){}

  vfs::cPtr<cSearchLocation> get(const vfs::cPtr<iChildLoader>& index)
  {
    if (index.isNull())
      return vfs::cPtr<cSearchLocation>();

    {
      vfs::cLockGuard lg(&m_access);
      const tMap::const_iterator finder = m_map.find(index);
      if (m_map.end() != finder)
        return finder->second;
    }

    return vfs::cPtr<cSearchLocation>();
  }

  void add(const vfs::cPtr<iChildLoader>& index
    , const vfs::cPtr<cSearchLocation>& newCacheItem)
  {
    if (index.isValid() && newCacheItem.isValid())
    {
      vfs::cLockGuard lg(&m_access);
      m_map[index] = newCacheItem;
    }
  }

  void forget(const vfs::cPtr<iChildLoader>& index)
  {
    if (index.isValid())
    {
      vfs::cLockGuard lg(&m_access);
      m_map.erase(index);
    }
  }

private:
  vfs::cLock m_access;
  tMap m_map;
};