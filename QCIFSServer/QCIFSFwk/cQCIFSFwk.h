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

//cQCIFSIPC.h

#pragma once

#include "iqcifsfwk.h"

#include "cMemComposite.h"

class cQCIFSFwk : public iQCIFSFwk, public vfs::cRefCount
{
public:
  QDEFINE_SINGLETON_IMPL(cQCIFSFwk, L"{D207431F-BD6D-4da4-81EF-3D915E704BEF} // cQCIFSFwk", iQCIFSFwk);

  cQCIFSFwk(){}
  ~cQCIFSFwk(void){}

  void QAPI logSummary(void) {QTRACE((L"cQCIFSFwk"));}
  void QAPI logDetailedSummary(void) {QTRACE((L"cQCIFSFwk - QCIFS framework classes"));}

  // iQCIFSFwk
  vfs::cPtr<iTreeResourceFactory> createTreeResourceFactory(vfs::cPtr<iSpaceQuery> pSpaceQuery
    , const vfs::cPtr<iRootChildLoader> pRootChildLoader
    , const vfs::String& sName
    , const DWORD dwBytesPerSector
    , const DWORD dwSectorsPerCluster
    , const vfs::cPtr<iNameMangler> pNameMangler
    , const vfs::cPtr<iNamespaceCreator> pNamespaceCreator);
  vfs::cPtr<iTreeResourceFactory> createTreeResourceFactory(vfs::cPtr<iSpaceQuery>
    , const vfs::cPtr<iChildLoader> pRootChildLoader
    , const vfs::String& sName
    , const DWORD dwBytesPerSector
    , const DWORD dwSectorsPerCluster
    , const vfs::cPtr<iNameMangler> pNameMangler
    , const vfs::cPtr<iNamespaceCreator> pNamespaceCreator);

  //public member functions
  vfs::cConstPtr<vfs::cMemoryView> makeDesktopIni(const std::string& sIconName);
};