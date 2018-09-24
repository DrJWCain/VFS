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
#include "couchAccess.h"

#include "..\QCIFSFwk\iChildLoaderVisitor.h"
#include "..\QCIFSFwk\iQCIFSFwkHelper.h"
#include "..\QCIFSFwk\iWriteInfo.h"


using namespace vfs;

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
  QTRACE((L"cSofaLoader::cSofaLoader %s", Name.c_str()));

  if(Name.empty())//root
  {
    createAlienSiteTransferDB(false);

    FolderMap.insert(tFolderMap::value_type(L"Durations", new cSofaLoader(L"Durations", this)));
    FolderMap.insert(tFolderMap::value_type(L"Tracknums", new cSofaLoader(L"Tracknums", this)));
  }
}

void cSofaLoader::registerListener(const vfs::cPtr<iChildLoaderVisitor> pChildListener)
{
  QTRACE((L"cSofaLoader::registerListener"));
  if (pChildListener.isValid())
  {
    pChildListener->clear();

    cChildLoader::registerListener(pChildListener);

    if(Name.empty())//root
    {
      auto artists = allArtists(false);
      for(auto artist : artists)
      {
        FolderMap.insert(tFolderMap::value_type(widen(artist), new cSofaLoader(widen(artist), this)));
      }
    }
    else if(Name.find(L"Durations") == 0)
    {
      auto records = recordsByDuration(narrow(std::wstring(Name, 9)), false);
      for(auto record : records)
      {
        //QTRACE((L"%S", record.first.c_str()));
        addVirtualFile(widen(record.first), widen(record.second));
      }

    }
    else if(Name.find(L"Tracknums") == 0)
    {
      auto records = recordsByTracknum(narrow(std::wstring(Name, 9)), false);
      for(auto record : records)
      {
        //QTRACE((L"%S", record.first.c_str()));
        addVirtualFile(widen(record.first), widen(record.second));
      }

    }
    else
    {
      auto records = recordsByArtist(narrow(Name), false);
      for(auto record : records)
      {
        //QTRACE((L"%S", record.first.c_str()));
        addVirtualFile(widen(record.first), widen(record.second));
      }
    }


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
        it->second,
        it->second,
        cConstPtr<cMemoryView>(),
        it->second,
        true);
    }
  }
}

DWORD cSofaLoader::Directory(const vfs::String& sName, LPSECURITY_ATTRIBUTES lpAtts)
{
  QTRACE((L"%S - %s", __FUNCTION__, sName.c_str()));

  const cPtr<iChildLoaderVisitor> pChildListener = getChildListener();
  if(pChildListener.isNull())
    return ERROR_ACCESS_DENIED;

  tFolderMap::iterator it = FolderMap.find(sName);
  if(it != FolderMap.end())
  {
    return ERROR_CANNOT_MAKE;
  }

  tFolderMap::value_type incoming(sName, new cSofaLoader(sName, this));
  FolderMap.insert(incoming);

  pChildListener->updateFolder(
    incoming.first,
    incoming.second,
    incoming.second,
    incoming.second,
    cConstPtr<cMemoryView>(),
    incoming.second,
    true);

  return ERROR_SUCCESS;
}

DWORD cSofaLoader::File(const vfs::String& sName, LPSECURITY_ATTRIBUTES lpAtts, const DWORD dwAtts, vfs::cPtr<iFileProcessingCallback> pCallback)
{
  return ERROR_ACCESS_DENIED;
}

DWORD cSofaLoader::From(vfs::cPtr<iRename> pRenameSource
  , const vfs::String& sNewName
  , const vfs::String& sUserName)
{
  const cPtr<iChildLoaderVisitor> pChildListener = getChildListener();
  if(pChildListener.isNull())
    return ERROR_ACCESS_DENIED;

  cPtr<cSofaLoader> pSofaLoader = ptr_cast<cPtr<cSofaLoader> >(pRenameSource);
  if(pSofaLoader.isValid())
  {
    const String newName = iQCIFSFwkHelper::singleton().getBaseName(sNewName);
    //QTRACE((L"cSofaLoader::rename %s to %s", pTestLoader->name().c_str(), newName.c_str()));

    pSofaLoader->rename(newName, this);//remove pTestLoader from its parent, and adopt it

    tFolderMap::value_type incoming(pSofaLoader->name(), pSofaLoader);
    FolderMap.insert(incoming);

    pChildListener->updateFolder(
      incoming.first,
      incoming.second,
      incoming.second,
      incoming.second,
      cConstPtr<cMemoryView>(),
      incoming.second,
      true);
  }

  return ERROR_SUCCESS;
}

void cSofaLoader::removeFolder(const String& name)
{
  QTRACE((L"cSofaLoader::removeFolder %s", name.c_str()));

  tFolderMap::iterator it = FolderMap.find(name);
  if(it != FolderMap.end())
  {
    FolderMap.erase(it);
  }
  const cPtr<iChildLoaderVisitor> pChildListener = getChildListener();
  if(pChildListener.isValid())
  {
    pChildListener->remove(name);
  }
}


void cSofaLoader::rename(const vfs::String& newName, cSofaLoader* newParent)
{
  const cPtr<iChildLoaderVisitor> pChildListener = getChildListener();
  if(pChildListener.isValid())
  {
    //rename means we need to clear search results ...
    ReadableFileMap.clear();
    pChildListener->clear();
  }
  Parent->removeFolder(Name);
  Name = newName;
  Parent = newParent;
}

DWORD cSofaLoader::notifyDelete()
{
  QSOS((L"%S %s", __FUNCTION__, Name.c_str()));

  if(Parent)
  {
    Parent->removeFolder(Name);
  }
  return ERROR_SUCCESS;
}