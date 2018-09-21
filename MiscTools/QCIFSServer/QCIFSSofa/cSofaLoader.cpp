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

//cSofaLoader.cpp

#include "StdAfx.h"

#include "cSofaLoader.h"
#include "cBasicFile.h"

#include "..\QCIFSFwk\iChildLoaderVisitor.h"
#include "..\QCIFSFwk\iQCIFSFwkHelper.h"
#include "..\QCIFSFwk\iWriteInfo.h"

#include <boost/regex.hpp>
using namespace vfs;

#include "..\..\Shared\cURL\include\curl.h"

 
#ifdef NDEBUG
#pragma comment(lib, "libcurl.lib")
#else 
#pragma comment(lib, "libcurl_debug.lib")
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////

void cSofaLoader::addVirtualFile(const String& name, const String& path)
{
  const iWriteInfo& writeInfo = iWriteInfo::singleton();

  cPtr<cBasicFile> p = new cBasicFile(path);

  WIN32_FILE_ATTRIBUTE_DATA attrs = p->getFileAttributes();

  fileInfo fi;
  LARGE_INTEGER sz;
  sz.HighPart = attrs.nFileSizeHigh;
  sz.LowPart = attrs.nFileSizeLow;
  writeInfo.writeFileInfo(&fi
    , sz.QuadPart
    , attrs.ftCreationTime
    , attrs.ftLastAccessTime
    , attrs.ftLastWriteTime
    , attrs.dwFileAttributes
    , name);

  ReadableFileMap.insert(tReadableFileMap::value_type(name, tReadInfo(p, fi)));
}

cSofaLoader::cSofaLoader(const vfs::String& name, cSofaLoader* parent) : Name(name), Parent(parent)
{
  QTRACE((L"cSofaLoader::cSofaLoader"));

  addVirtualFile(L"David Bowie - Station To Station.mp3", L"D:\\Music\\B\\Bowie 1966 - 1976\\David Bowie - Station To Station [1976]\\David Bowie - Station To Station.mp3");
  addVirtualFile(L"Motorhead - Ace of Spades.mp3", L"D:\\Music\\M\\Motorhead - The Best Of Greatest Hits [Bubanee]\\01 - Ace of Spades.mp3");
}

void cSofaLoader::registerListener(const vfs::cPtr<iChildLoaderVisitor> pChildListener)
{
  QTRACE((L"cSofaLoader::registerListener"));
  if (pChildListener.isValid())
  {
    cChildLoader::registerListener(pChildListener);

    for(tReadableFileMap::const_iterator it = ReadableFileMap.begin(); it != ReadableFileMap.end(); ++it)
    {
      pChildListener->updateFile(
        it->second.second,
        it->second.first,
        cPtr<iFileEvent>());
    }

    for(tFolderMap::const_iterator it = FolderMap.begin(); it != FolderMap.end(); ++it)
    {
      //QTRACE((L"cSofaLoader::registerListener %s %s %s", Name.c_str(), it->first.c_str(), it->second->name().c_str()));
      pChildListener->updateFolder(
        it->first,
        it->second, 
        cPtr<iRename>(),
        cPtr<iCreate>(),
        cConstPtr<cMemoryView>(),
        cPtr<iFileEvent>(),
        true);
    }
  }
}

