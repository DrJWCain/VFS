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

//cSizeOnDisk.cpp - handle common disk allocation request for different commands

#include "stdafx.h"
#include ".\csizeondisk.h"

#include "..\iTreeResource.h"

using namespace vfs;

cSizeOnDisk::cSizeOnDisk(const cPtr<iTreeResource> pTreeResource)
: m_dwBytesPerCluster(pTreeResource->getBytesPerCluster())
{
	if (m_dwBytesPerCluster == 0)
		m_dwBytesPerCluster = 1;
}

cSizeOnDisk::~cSizeOnDisk(void)
{
}

LARGE_INTEGER cSizeOnDisk::getSizeOnDisk(DWORD hiSize, DWORD loSize) const
{
  __int64 size64 = (static_cast<__int64>(hiSize) << 32) + loSize;

  DWORD remainder = static_cast<DWORD>(size64 % m_dwBytesPerCluster);

  if (remainder != 0)
  {
    // expand file size to cluster boundary
    size64 += m_dwBytesPerCluster - remainder;
  }

  LARGE_INTEGER ret;
  ret.QuadPart = size64;
  return ret;
}