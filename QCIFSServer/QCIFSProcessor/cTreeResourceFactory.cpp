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

//cTreeResourceFactory.cpp

#include "stdafx.h"

#include "..\iTreeResource.h"
#include "cTreeResourceFactory.h"

cTreeResourceFactory::cTreeResourceFactory()
{
}

cTreeResourceFactory::~cTreeResourceFactory()
{
  // free any libraries we may have loaded
  for (std::map<std::string, HMODULE>::iterator it = m_libraryMap.begin()
    ; it != m_libraryMap.end()
    ; ++it)
  {
    if (it->second != NULL)
    {
      FreeLibrary(it->second);
    }
  }
}

iTreeResource *cTreeResourceFactory::getTreeResource(eTreeType treeType)
{
  // determine path of the module we want to load based on tree type and build settings
  std::string sPath(getModulePath(treeType));

  // load module if it isn't already loaded
  HMODULE hModule(getModule(sPath));

  // get address of 'getTreeResource' procedure
  iTreeResource *pTreeResource(NULL);
  if (hModule != NULL) 
  { 
    GETTREEPROC ProcAdd = NULL;
    ProcAdd = reinterpret_cast<GETTREEPROC>(GetProcAddress(hModule, "getTreeResource")); 

    if (ProcAdd == NULL) 
    {
      PRINT(L"Failed to get getTreeResource proc address with error %d\n", GetLastError());
    }
    else
    {
      pTreeResource = (ProcAdd)();
    }

    if (pPrintFn == NULL)
      pPrintFn = reinterpret_cast<LOGPROC>(GetProcAddress(hModule, "PrintFN")); 
    if (pSOSFn == NULL)
      pSOSFn = reinterpret_cast<LOGPROC>(GetProcAddress(hModule, "SOSFN"));
  }

  return pTreeResource;
}

std::string cTreeResourceFactory::getModulePath(eTreeType treeType)
{
  char pBuff[MAX_PATH];
  GetCurrentDirectory(MAX_PATH, pBuff);
  std::string sPath(pBuff);
  switch(treeType)
  {
    case folderMap:
      #ifdef QCIFS_EXEC_VERSION
        sPath = "QCIFSFolderMap.dll";
      #else
        #ifdef _DEBUG
          sPath += "\\DLL_Code_Debug\\vfs_QCIFS.dll";
        #else
          sPath += "\\DLL_Code_Release\\vfs_QCIFS.dll";
        #endif
      #endif
      break;
    case IPC$:
      #ifdef QCIFS_EXEC_VERSION
        #ifdef _DEBUG
          sPath = "QCIFSIPC_Dbg.dll";
        #else
          sPath = "QCIFSIPC.dll";
        #endif
      #else
        #ifdef _DEBUG
          sPath += "\\DLL_Data\\vfs\\QCIFS\\QCIFSIPC_Dbg.dll";
        #else
          sPath += "\\DLL_Data\\vfs\\QCIFS\\QCIFSIPC.dll";
        #endif
      #endif
      break;
    default:
      break;
  }

  return sPath;
}

HMODULE cTreeResourceFactory::getModule(const std::string &krsPath)
{
  HMODULE hModule(NULL);
  std::map<std::string, HMODULE>::iterator it = m_libraryMap.find(krsPath);
  if (it == m_libraryMap.end())
  {
    hModule = LoadLibrary(krsPath.c_str());
    if (hModule != NULL)
    {
      m_libraryMap.insert(std::map<std::string, HMODULE>::value_type(krsPath, hModule));
    }
    else
    {
      DWORD err = GetLastError();
      SOS(L"Failed to load %s with error - %d\n", krsPath.c_str(), err);
    }
  }
  else
    hModule = it->second;

  return hModule;
}