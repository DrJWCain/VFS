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

//cTreeResourceFactory.h

#pragma once

#include "..\threads\iPointer.h"

class iTreeResource;
enum eTreeType;

typedef iTreeResource *(*GETTREEPROC)(); 
typedef void *(*LOGPROC)(const wchar_t *format, ...); 

class cTreeResourceFactory : public cRefCount
{
public:
  cTreeResourceFactory();
  ~cTreeResourceFactory();

  iTreeResource *getTreeResource(eTreeType treeType);

private:
  std::string getModulePath(eTreeType treeType);
  HMODULE getModule(const std::string &krsPath);

private:
  std::map<std::string, HMODULE> m_libraryMap;
};