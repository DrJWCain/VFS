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

//cModuleFactory.h


#pragma once

class iTreeResource;
class cSMBServer;
enum eTreeType;

using namespace vfs;

typedef iTreeResource *(*GETTREEPROC)(const std::wstring &sPath); 
typedef void (__cdecl*LOGPROC)(const wchar_t **format); 

class cModuleFactory : public cRefCount
{
public:
  cModuleFactory(cSMBServer *pServer, const std::wstring &sFolderPath);
  ~cModuleFactory();

  iTreeResource *getTreeResource(eTreeType treeType, const String& sLoginName);

private:
  std::list<std::wstring> getModulePaths(eTreeType treeType);
  HMODULE getModule(const std::wstring &krsPath);

private:
  const std::wstring m_sFolderPath;

  std::map<std::wstring, HMODULE> m_libraryMap;
  cSMBServer *m_pServer;
};