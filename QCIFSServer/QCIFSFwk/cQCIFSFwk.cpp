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

#include "stdafx.h"

#include "cQCIFSFwk.h"

#include "cTreeResourceFactory.h"
#include "cConstRootChildLoader.h"
#include "cShareInfo.h"
#include "iWriteInfo.h"
#include "cSearchLocation.h"

using namespace vfs;

cPtr<iTreeResourceFactory> cQCIFSFwk::createTreeResourceFactory(cPtr<iSpaceQuery> pSpaceQuery
  , const cPtr<iRootChildLoader> pRootChildLoader
  , const String& sName
  , const DWORD dwBytesPerSector
  , const DWORD dwSectorsPerCluster
  , const cPtr<iNameMangler> pNameMangler
  , const vfs::cPtr<iNamespaceCreator> pNamespaceCreator)
{
  const cPtr<cShareInfo> pShareInfo = new cShareInfo(sName, pSpaceQuery, dwBytesPerSector, dwSectorsPerCluster);
  return new cTreeResourceFactory(pShareInfo, pRootChildLoader, sName, pNameMangler, pNamespaceCreator);
}

cPtr<iTreeResourceFactory> cQCIFSFwk::createTreeResourceFactory(cPtr<iSpaceQuery> pSpaceQuery
  , const cPtr<iChildLoader> pChildLoader
  , const String& sName
  , const DWORD dwBytesPerSector
  , const DWORD dwSectorsPerCluster
  , const cPtr<iNameMangler> pNameMangler
  , const vfs::cPtr<iNamespaceCreator> pNamespaceCreator)
{
  const cPtr<iRootChildLoader> pRootChildLoader = new cConstRootChildLoader(pChildLoader);
  return createTreeResourceFactory(pSpaceQuery
    , (cPtr<iRootChildLoader>)pRootChildLoader
    , sName
    , dwBytesPerSector
    , dwSectorsPerCluster
    , pNameMangler
    , pNamespaceCreator);
}

cConstPtr<cMemoryView> cQCIFSFwk::makeDesktopIni(const std::string& sIconName)
{
  std::stringstream sIniText;
  sIniText << "[.ShellClassInfo]\r\nIconFile="
    << sIconName
    << "\r\nIconIndex=0";
  const std::string& sText(sIniText.str());
  const cPtr<cMemoryView> pINIFile = new cMemoryView(new cMemory(sText.length(), cMemory::eVirtual));
  memcpy(pINIFile->getBytes(), sText.c_str(), sText.length());
  return pINIFile;
}