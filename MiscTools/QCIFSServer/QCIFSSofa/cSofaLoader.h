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

//cSofaLoader.h

#pragma once

#include "..\qcifsfwk\cChildLoader.h"
#include "..\qcifsfwk\iCreate.h"
#include "..\qcifsfwk\iRename.h"


class cSofaLoader : public cChildLoader, public vfs::cRefCount
{
  //typedef std::pair<vfs::cPtr<iWriteCallback>, fileInfo> tWriteInfo;
  //typedef std::map<vfs::String, tWriteInfo> tWriteableFileMap;
  //tWriteableFileMap WriteableFileMap;

  typedef std::pair<vfs::cPtr<iReadCallback>, fileInfo> tReadInfo;
  typedef std::map<vfs::String, tReadInfo> tReadableFileMap;
  tReadableFileMap ReadableFileMap;

  typedef std::map<vfs::String, vfs::cPtr<cSofaLoader> > tFolderMap;
  tFolderMap FolderMap;

  vfs::String Name;
  cSofaLoader* Parent;

  void cSofaLoader::addVirtualFile(const vfs::String& name, const vfs::String& path);

public:
  cSofaLoader(const vfs::String& name, cSofaLoader* parent);

  //iChildLoader
  void registerListener(const vfs::cPtr<iChildLoaderVisitor> pChildUpdate);
  bool cacheable() {return false;}

  void removeFile(const vfs::String& name);
  void removeFolder(const vfs::String& name);

  vfs::String name() {return Name;}
  void rename(const vfs::String& newName, cSofaLoader* newParent);
};