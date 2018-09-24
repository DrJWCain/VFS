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

//cSpaceQuery.cpp

#include "StdAfx.h"
#include ".\cspacequery.h"

cSpaceQuery::cSpaceQuery(void)
{
}

cSpaceQuery::~cSpaceQuery(void)
{
}

unsigned __int64 cSpaceQuery::freeSpace() const
{
  const MEMORYSTATUSEX val = getCurrentMemoryStatus();
  return val.ullAvailVirtual;
}

unsigned __int64 cSpaceQuery::totalSpace() const
{
  const MEMORYSTATUSEX val = getCurrentMemoryStatus();
  return val.ullTotalVirtual;
}

inline MEMORYSTATUSEX cSpaceQuery::getCurrentMemoryStatus() const
{
  MEMORYSTATUSEX val;
  val.dwLength = sizeof(val);
  GlobalMemoryStatusEx(&val);
  return val;
}


unsigned __int64 cISASpaceQuery::freeSpace() const
{
  return 0x1000000000;//JCJCJC Fix me!
}

unsigned __int64 cISASpaceQuery::totalSpace() const
{
  return 0x2000000000;//JCJCJC Fix me!
}