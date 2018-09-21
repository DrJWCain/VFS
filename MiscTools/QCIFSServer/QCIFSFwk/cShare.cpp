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

//cShare.cpp

#include "StdAfx.h"
#include ".\cshare.h"

#include "cShare.h"
#include "cDirectoryFactory.h"
#include "iNamed.h"

using namespace vfs;

cShare::cShare(cPtr<iShareInfo> pShareInfo
  , cPtr<cDirectoryFactory> pDirectoryFactory
  , cPtr<iNamed> pNamed)
: cDirectoryFactory(pDirectoryFactory)
, m_pShareInfo(pShareInfo)
, m_pNamed(pNamed)
{
}

cShare::~cShare(void)
{
}

unsigned __int64 cShare::freeSpace() const
{
  return m_pShareInfo->freeSpace();
}

unsigned __int64 cShare::totalSpace() const
{
  return m_pShareInfo->totalSpace();
}

vfs::String cShare::label() const
{
  return m_pShareInfo->label();
}

DWORD cShare::bytesPerSector() const
{
  return m_pShareInfo->bytesPerSector();
}

DWORD cShare::sectorsPerCluster() const
{
  return m_pShareInfo->sectorsPerCluster();
}

String cShare::Name() const
{
  return m_pNamed->Name();
}