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

//cShare.h

#pragma once

#include "iNamed.h"
#include "iShareInfo.h"
#include "cDirectoryFactory.h"

class cShare : public iNamed, public iShareInfo, public cDirectoryFactory
{
public:
  cShare(vfs::cPtr<iShareInfo> pShareInfo
    , vfs::cPtr<cDirectoryFactory> pDirectoryFactory
    , vfs::cPtr<iNamed> pNamed);
  ~cShare(void);
  
  //iNamed
  vfs::String Name() const;

  //iShareInfo
  unsigned __int64 freeSpace() const;
  unsigned __int64 totalSpace() const;
  vfs::String label() const;
  DWORD bytesPerSector() const;
  DWORD sectorsPerCluster() const;

private:
  vfs::cPtr<iShareInfo> m_pShareInfo;
  vfs::cPtr<iNamed> m_pNamed;
};
