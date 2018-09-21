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

//iQCIFSIPC.h

#pragma once

#include "cLazyLoader.h"

#include "..\iComposite.h"
#include "iRootChildLoader.h"
#include "iChildLoader.h"
#include "iNameMangler.h"

class iTreeResourceFactory;
class iSpaceQuery;

class iNamespaceCreator
{
public: 
  virtual ~iNamespaceCreator(){}
  virtual bool lazyDirectoryCheck(const vfs::String& searchPath, const vfs::String& sLoginName) = 0;
};

class iQCIFSFwk : public vfs::iManager
{
public:
  QDEFINE_SINGLETON_IFACE(iQCIFSFwk, L"{6EA2FF0D-BC30-4885-BFAE-F12426642436} //iQCIFSFwk", vfs::iManager);

  virtual ~iQCIFSFwk(){}

  virtual vfs::cPtr<iTreeResourceFactory> createTreeResourceFactory(vfs::cPtr<iSpaceQuery>
    , const vfs::cPtr<iRootChildLoader> pRootChildLoader
    , const vfs::String& sName
    , const DWORD dwBytesPerSector
    , const DWORD dwSectorsPerCluster
    , const vfs::cPtr<iNameMangler> pNameMangler
    , const vfs::cPtr<iNamespaceCreator> pNamespaceCreator) = 0;
  virtual vfs::cPtr<iTreeResourceFactory> createTreeResourceFactory(vfs::cPtr<iSpaceQuery>
    , const vfs::cPtr<iChildLoader> pRootChildLoader
    , const vfs::String& sName
    , const DWORD dwBytesPerSector
    , const DWORD dwSectorsPerCluster
    , const vfs::cPtr<iNameMangler> pNameMangler
    , const vfs::cPtr<iNamespaceCreator> pNamespaceCreator) = 0;
};

const unsigned int DEFAULT_SECTOR_SIZE = 512;
const unsigned int DEFAULT_CLUSTER_SIZE = 1;