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

//cShareInfo.h

#pragma once

#include "ishareinfo.h"

class iSpaceQuery;

class cShareInfo : public iShareInfo, public vfs::cRefCount
{
public:
  cShareInfo(const vfs::String& sLabel
    , vfs::cPtr<iSpaceQuery> pSpaceQuery
    , const DWORD bytesPerSector
    , const DWORD sectorsPerCluster);
  ~cShareInfo(void);

  //iShareInfo
  vfs::String label() const {return m_sLabel;}
  DWORD bytesPerSector() const {return m_bytesPerSector;}
  DWORD sectorsPerCluster() const {return m_sectorsPerCluster;}

  //iSpaceQuery
  unsigned __int64 freeSpace() const;
  unsigned __int64 totalSpace() const;

private:
  const vfs::String m_sLabel;
  vfs::cPtr<iSpaceQuery> m_pSpaceQuery;
  const DWORD m_bytesPerSector;
  const DWORD m_sectorsPerCluster;
};
