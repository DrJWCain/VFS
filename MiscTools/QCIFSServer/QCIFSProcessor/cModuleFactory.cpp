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

//cModuleFactory.cpp

#include "stdafx.h"

#include "cSMBServer.h"

#include "..\iTreeResource.h"
#include "..\QCIFSBin\iQCIFSBin.h"

cModuleFactory::cModuleFactory(cSMBServer *pServer, const std::wstring &sFolderPath)
: m_pServer(pServer)
, m_sFolderPath(sFolderPath)
{
}

cModuleFactory::~cModuleFactory()
{
  // free any libraries we may have loaded
  for (std::map<std::wstring, HMODULE>::iterator it = m_libraryMap.begin()
    ; it != m_libraryMap.end()
    ; ++it)
  {
    if (it->second != NULL)
    {
      FreeLibrary(it->second);
    }
  }
}

iTreeResource *cModuleFactory::getTreeResource(eTreeType treeType, const String& sLoginName)
{
  // determine path of the module we want to load based on tree type and build settings
  iTreeResource *pTreeResource(NULL);
  try
  {
    pTreeResource = iQCIFSBin::singleton().getTreeResource(sLoginName);
  }
  catch (const cNotFound &)
  {
    HMODULE hModule(NULL);
    std::list<std::wstring> sPaths(getModulePaths(treeType));

    for (std::list<std::wstring>::const_iterator cit = sPaths.begin()
      ; (cit != sPaths.end()) && (hModule == NULL)
      ; ++cit)
    {
      const std::wstring sPath(*cit);

      // load module if it isn't already loaded
      hModule = getModule(sPath);

      // get address of 'getTreeResource' procedure
      if (hModule != NULL) 
      { 
        GETTREEPROC ProcAdd = NULL;
        ProcAdd = reinterpret_cast<GETTREEPROC>(GetProcAddress(hModule, "getTreeResource")); 

        if (ProcAdd == NULL) 
        {
          QTRACE((L"Failed to get getTreeResource proc address with error %d", GetLastError()));
        }
        else
        {
          pTreeResource = (ProcAdd)(m_sFolderPath);
        }
      }
    }
  }

  return pTreeResource;
}

std::list<std::wstring> cModuleFactory::getModulePaths(eTreeType treeType)
{
  std::list<std::wstring> sPaths;
  switch(treeType)
  {
    case folderMap:
      #ifdef _DEBUG
        sPaths.push_back(L".\\QCIFSFolderMap_Dbg.dll");
      #else
        sPaths.push_back(L".\\QCIFSFolderMap.dll");
      #endif
      break;
    case IPC$:
      #ifdef _DEBUG
        sPaths.push_back(L".\\QCIFSIPC_Dbg.dll");
      #else
        sPaths.push_back(L".\\QCIFSIPC.dll");
      #endif
      break;
    default:
      break;
  }

  return sPaths;
}

HMODULE cModuleFactory::getModule(const std::wstring &krsPath)
{
  HMODULE hModule(NULL);
  std::map<std::wstring, HMODULE>::iterator it = m_libraryMap.find(krsPath);
  if (it == m_libraryMap.end())
  {
    hModule = LoadLibrary(krsPath.c_str());
    if (hModule != NULL)
    {
      m_libraryMap.insert(std::map<std::wstring, HMODULE>::value_type(krsPath, hModule));
    }
    else
    {
      DWORD err = GetLastError();
      QSOS((L"Failed to load %s with error - %d", krsPath.c_str(), err));
    }
  }
  else
    hModule = it->second;

  return hModule;
}