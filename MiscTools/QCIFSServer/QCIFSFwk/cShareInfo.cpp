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

//cShareInfo.cpp

#include "StdAfx.h"

#include ".\cshareinfo.h"

using namespace vfs;

cShareInfo::cShareInfo(const String& sLabel
  , cPtr<iSpaceQuery> pSpaceQuery
  , const DWORD bytesPerSector
  , const DWORD sectorsPerCluster)
: m_sLabel(sLabel)
, m_pSpaceQuery(pSpaceQuery)
, m_bytesPerSector(bytesPerSector)
, m_sectorsPerCluster(sectorsPerCluster)
{
}

cShareInfo::~cShareInfo(void)
{
}

unsigned __int64 cShareInfo::freeSpace() const
{
  return m_pSpaceQuery.isValid() ? m_pSpaceQuery->freeSpace() : 0;
}

unsigned __int64 cShareInfo::totalSpace() const
{
  return m_pSpaceQuery.isValid() ? m_pSpaceQuery->totalSpace() : 0;
}