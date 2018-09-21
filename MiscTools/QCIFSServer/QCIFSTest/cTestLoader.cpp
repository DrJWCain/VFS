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

//cTestLoader.cpp

#include "StdAfx.h"

#include "cTestLoader.h"
#include "cTestWriteCallback.h"
#include "cBasicFile.h"

#include "..\QCIFSFwk\iChildLoaderVisitor.h"
#include "..\QCIFSFwk\iQCIFSFwkHelper.h"
#include "..\QCIFSFwk\iWriteInfo.h"

#include <boost/regex.hpp>
using namespace vfs;

//////////////////////////////////////////////////////////////////////////////////////////////////////

class cTestLoaderDeleteEvent : public iFileEvent, public cRefCount
{
  cTestLoader* Parent;
  String Name;
public:
  cTestLoaderDeleteEvent(cTestLoader* parent, const String& name);
  virtual DWORD notifyDelete();
};

cTestLoaderDeleteEvent::cTestLoaderDeleteEvent(cTestLoader* parent, const String& name)
: Parent(parent), Name(name)
{
}

DWORD cTestLoaderDeleteEvent::notifyDelete()
{
  QTRACE((L"cTestLoaderDeleteEvent::notifyDelete %s", Name.c_str()));
  Parent->removeFolder(Name);
  return ERROR_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void cTestLoader::addVirtualFile(const String& name, const String& path)
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

cTestLoader::cTestLoader(const vfs::String& name, cTestLoader* parent) : Name(name), Parent(parent)
{
  QTRACE((L"cTestLoader::cTestLoader"));

  //addVirtualFile(L"test.iso", L"C:\\Store\\Images\\Visual Studio\\en_visual_studio_professional_2015_x86_x64_dvd_6846629.iso");
  addVirtualFile(L"reel.mxf", L"E:\\import\\0-1500.mxf");
  addVirtualFile(L"james.mxf", L"E:\\import\\MVR_RUSHES_for_RC.mxf");

  //ReadableFileMap.insert(tReadableFileMap::value_type(L"james.mxf 48000.pek", new cBasicFile(L"C:\\Data\\Import\\MVR_RUSHES_for_RC.mxf 48000.pek")));
  //ReadableFileMap.insert(tReadableFileMap::value_type(L"james.mxf.xmp", new cBasicFile(L"C:\\Data\\Import\\MVR_RUSHES_for_RC.mxf.xmp")));
}

void cTestLoader::registerListener(const vfs::cPtr<iChildLoaderVisitor> pChildListener)
{
  QTRACE((L"cTestLoader::registerListener"));
  if (pChildListener.isValid())
  {
    cChildLoader::registerListener(pChildListener);
    //Test FILE_ATTRIBUTE_OFFLINE - Thanks to Tridge - as ever!
    //if(FolderMap.empty() && !Parent)
    //{
    //  File(L"james.txt", 0, FILE_ATTRIBUTE_OFFLINE);
    //}

    for(tReadableFileMap::const_iterator it = ReadableFileMap.begin(); it != ReadableFileMap.end(); ++it)
    {
      pChildListener->updateFile(
        it->second.second,
        it->second.first,
        cPtr<iFileEvent>());
    }

    for(tWriteableFileMap::const_iterator it = WriteableFileMap.begin(); it != WriteableFileMap.end(); ++it)
    {
      pChildListener->updateFile(
        it->second.first,
        it->second.second);
    }

    for(tFolderMap::const_iterator it = FolderMap.begin(); it != FolderMap.end(); ++it)
    {
      //QTRACE((L"cTestLoader::registerListener %s %s %s", Name.c_str(), it->first.c_str(), it->second->name().c_str()));
      pChildListener->updateFolder(
        it->first,
        it->second, 
        it->second, 
        it->second, 
        cConstPtr<cMemoryView>(),
        new cTestLoaderDeleteEvent(this, it->first),
        true);
    }
  }
}

DWORD cTestLoader::Directory(const vfs::String& sName, LPSECURITY_ATTRIBUTES lpAtts)
{
  //QTRACE((L"cTestLoader::Directory %s", sName.c_str()));

  const cPtr<iChildLoaderVisitor> pChildListener = getChildListener();
  if (pChildListener.isNull())
    return ERROR_ACCESS_DENIED;

  tFolderMap::iterator it = FolderMap.find(sName);
  if(it != FolderMap.end())
  {
    return ERROR_CANNOT_MAKE;
  }

  tFolderMap::value_type incoming(sName, new cTestLoader(sName, this));
  FolderMap.insert(incoming);

  pChildListener->updateFolder(
    incoming.first,
    incoming.second, 
    incoming.second, 
    incoming.second, 
    cConstPtr<cMemoryView>(),
    new cTestLoaderDeleteEvent(this, incoming.first),
    true);

  return ERROR_SUCCESS;
}

DWORD cTestLoader::File(const vfs::String& sName, LPSECURITY_ATTRIBUTES lpAtts, const DWORD dwAtts, cPtr<iFileProcessingCallback> pCallback)
{
  QTRACE((L"cTestLoader::File %s", sName.c_str()));

  const cPtr<iChildLoaderVisitor> pChildListener = getChildListener();
  if (pChildListener.isNull())
    return ERROR_ACCESS_DENIED;

  tWriteableFileMap::iterator it = WriteableFileMap.find(sName);
  if(it != WriteableFileMap.end())
  {
    return ERROR_FILE_EXISTS;
  }

  fileInfo fi; 
  FILETIME ft;
  GetSystemTimeAsFileTime(&ft);

  vfs::cPtr<iWriteCallback> pWriter = new cTestWriteCallback(sName, this);
  iWriteInfo::singleton().writeFileInfo(&fi, pWriter->getSize(0), ft, ft, ft, dwAtts, sName);
  tWriteableFileMap::value_type incoming(sName, tWriteInfo(pWriter,fi));
  WriteableFileMap.insert(incoming);

  pChildListener->updateFile(
    incoming.second.first, 
    fi);

  return ERROR_SUCCESS;
}

DWORD cTestLoader::From(vfs::cPtr<iRename> pRenameSource
    , const vfs::String& sNewName
    , const vfs::String& sUserName)
{
  const cPtr<iChildLoaderVisitor> pChildListener = getChildListener();
  if (pChildListener.isNull())
    return ERROR_ACCESS_DENIED;

  cPtr<cTestWriteCallback> pTestWriteCallback = ptr_cast<cPtr<cTestWriteCallback> >(pRenameSource);
  if(pTestWriteCallback.isValid())
  {
    const String newName = iQCIFSFwkHelper::singleton().getBaseName(sNewName);
    //QTRACE((L"cTestLoader::rename %s to %s", pTestWriteCallback->name().c_str(), newName.c_str()));

    pTestWriteCallback->rename(newName, this);//remove pTestWriteCallback from its parent, and adopt it

    fileInfo fi; 
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);

    iWriteInfo::singleton().writeFileInfo(&fi, pTestWriteCallback->getSize(0), ft, ft, ft, FILE_ATTRIBUTE_NORMAL, pTestWriteCallback->name());
    tWriteableFileMap::value_type incoming(pTestWriteCallback->name(), tWriteInfo(pTestWriteCallback,fi));
    WriteableFileMap.insert(incoming);

    pChildListener->updateFile(
      incoming.second.first, 
      fi);
  }

  cPtr<cTestLoader> pTestLoader = ptr_cast<cPtr<cTestLoader> >(pRenameSource);
  if(pTestLoader.isValid())
  {
    const String newName = iQCIFSFwkHelper::singleton().getBaseName(sNewName);
    //QTRACE((L"cTestLoader::rename %s to %s", pTestLoader->name().c_str(), newName.c_str()));

    pTestLoader->rename(newName, this);//remove pTestLoader from its parent, and adopt it

    tFolderMap::value_type incoming(pTestLoader->name(), pTestLoader);
    FolderMap.insert(incoming);

    pChildListener->updateFolder(
      incoming.first,
      incoming.second, 
      incoming.second, 
      incoming.second, 
      cConstPtr<cMemoryView>(),
      new cTestLoaderDeleteEvent(this, incoming.first),
      true);
  }

  return ERROR_SUCCESS;
}

void cTestLoader::removeFile(const String& name)
{
  QTRACE((L"cTestLoader::removeFile %s", name.c_str()));

  tWriteableFileMap::iterator it = WriteableFileMap.find(name);
  if(it != WriteableFileMap.end())
  {
    WriteableFileMap.erase(it);
  }
  const cPtr<iChildLoaderVisitor> pChildListener = getChildListener();
  if (pChildListener.isValid())
  {
    pChildListener->remove(name);
  }
}

void cTestLoader::removeFolder(const String& name)
{
  QTRACE((L"cTestLoader::removeFolder %s", name.c_str()));

  tFolderMap::iterator it = FolderMap.find(name);
  if(it != FolderMap.end())
  {
    FolderMap.erase(it);
  }
  const cPtr<iChildLoaderVisitor> pChildListener = getChildListener();
  if (pChildListener.isValid())
  {
    pChildListener->remove(name);
  }
}

void cTestLoader::rename(const vfs::String& newName, cTestLoader* newParent) 
{
  Parent->removeFolder(Name);
  Name=newName;
  Parent = newParent;
}