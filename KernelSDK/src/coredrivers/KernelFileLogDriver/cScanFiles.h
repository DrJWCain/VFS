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

#pragma once

namespace vfs
{
  //{{{
  class cScanFiles : public iFileManager::iFileCallback, public cRefCount
  {
  public:
    cScanFiles(std::list<String>& files) : Files(files) { }

    NextNode QAPI onFolder(const String& AbsPath, const String& RelPath, const String& Name, const unsigned int Depth) { return eNextNode_NextSibling; }

    NextNode QAPI onFile(const String& AbsPath, const String& RelPath, const String& Name, const String& Ext, const unsigned int Depth)
    {
	    Files.push_back(RelPath + Name + Ext);
      return eNextNode_NextChild;
    }

  private:
    std::list<String>& Files;
  };
  //}}}
}