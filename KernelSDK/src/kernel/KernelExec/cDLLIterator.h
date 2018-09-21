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
/*
**  
*/

// --- Headers ---------------------------------------------------------------------------

#include <set>
#include "QKernelExec.h"
#include "cDLLManager.h"
#include "cDLL.h"

// --- Start of Namespace ----------------------------------------------------------------

using namespace vfs;




// =======================================================================================
// === Helpers ===========================================================================
// =======================================================================================

namespace
{

void * _findFuncInDll(const HINSTANCE DllInstance, const String &  FuncName)
{
  void * const Func = GetProcAddress(DllInstance, narrow(FuncName).c_str());
  return Func;
}

bool _isAModule(const HINSTANCE DllHandle)
{
  QVERIFY(DllHandle);
  return _findFuncInDll(DllHandle, L"moduleInit") != 0 || _findFuncInDll(DllHandle, L"moduleRegister") != 0;
}

}




// =======================================================================================
// === cDLLIterator ======================================================================
// =======================================================================================

class cDLLIterator : public iFileManager::iFileCallback, public cRefCount
{
  public :

    QDEFINE_SMARTPTR(cDLLIterator);

    cDLLIterator(
      multiset<cDLL> & DLLs,
      const unsigned long MinGroup,
      const unsigned long MaxGroup,
      const vector<String> & RegFolders,
      const vector<String> & DataFolders) :
        m_DLLs(DLLs),
        m_MinGroup(MinGroup),
        m_MaxGroup(MaxGroup),
        m_RegFolders(RegFolders),
        m_DataFolders(DataFolders)
    {
    }

    NextNode QAPI onFolder(const String & AbsCodePath, const String & RelCodePath, const String & Name, const unsigned int Depth)
    {
      return eNextNode_NextSibling;
    }

    NextNode QAPI onFile(const String & AbsCodePath, const String & RelCodePath, const String & CodeFilenameNoPath, const String & Ext, const unsigned int Depth)
    {
      QVERIFY(Depth == 0);

      // Ignore folders and non-dll files.
      if (Ext == String(L".dll"))
      {
        const String::size_type LastSlashPos = AbsCodePath.find_last_of (L'\\');
        QVERIFY (LastSlashPos != String::npos);
        const String PathNoLastSlash (AbsCodePath.substr (0, LastSlashPos));
        const String::size_type SecondLastSlashPos = PathNoLastSlash.find_last_of (L'\\');
        QVERIFY (SecondLastSlashPos != String::npos);
        const String RootPath = AbsCodePath.substr (0, SecondLastSlashPos + 1);

        // Load the DLL without resolving references
        String LoadDLLMsg;
        try
        {
          const String::size_type LastUnderscorePos = CodeFilenameNoPath.find_last_of (L'_');
          if (LastUnderscorePos == String::npos)
          {
            throw cBadFormat(__FILE__, __LINE__, L"Expected underscore in \"%s\" seperating developer and module name", CodeFilenameNoPath.c_str());
          }
          const String DeveloperName = CodeFilenameNoPath.substr (0, LastUnderscorePos);
          const String ModuleName = CodeFilenameNoPath.substr (LastUnderscorePos + 1, String::npos);

          cProgressMsgGuard progress(L"Found %d \"%s - %s\"", m_DLLs.size() + 1, DeveloperName.c_str(), ModuleName.c_str());

          const String CodeFilename(AbsCodePath + CodeFilenameNoPath + Ext);
          HINSTANCE hDllInstance = cDLLManager::singleton().loadRawDLL(
            CodeFilename,
            false /* resolve symbols? */);
          if (_isAModule(hDllInstance))
          {
            cVersion DllVer(kBadVer);
            cVersion BuiltWithSDKVer(kBadVer);
            unsigned long LoadOrderGroup;

            {
              Char Tmp[128];

              if (LoadString(hDllInstance, 1, Tmp, sizeof(Tmp)) == 0)
              {
                // no dll version, that's ok
              }
              else
              {
                DllVer = cVersion(Tmp);
              }

              if (LoadString(hDllInstance, 3, Tmp, sizeof(Tmp)) == 0) QFATAL((__FILE__, __LINE__, L"IDS_LOAD_ORDER_GROUP (Resource ID #3) not found in string table resource in DLL \"%s\"", CodeFilename.c_str()));
              LoadOrderGroup = wcstoul(Tmp, NULL, 10);

              if (LoadString(hDllInstance, 5, Tmp, sizeof(Tmp)) == 0)
              {
                // no SDK version, that's ok
              }
              else
              {
                BuiltWithSDKVer = cVersion(Tmp);
              }
            }

            if (LoadOrderGroup >= m_MinGroup && LoadOrderGroup <= m_MaxGroup)
            {
              const bool HasModuleRegisterFunc  = _findFuncInDll(hDllInstance, L"moduleRegister") != 0;
              const bool HasModuleInstallFunc   = _findFuncInDll(hDllInstance, L"moduleInstall") != 0;
              const bool HasModuleUninstallFunc = _findFuncInDll(hDllInstance, L"moduleUninstall") != 0;

              String DataPath;
              for (std::vector<String>::const_iterator it = m_DataFolders.begin();;)
              {
                String path = *it + DeveloperName + L"\\" + ModuleName + L"\\";
                if (iFileManager::singleton().isFileOrFolderCreated(path) || (++it) == m_DataFolders.end())
                {
                  DataPath = path;
                  break;
                }
              }

              String RegFilename;
              for (std::vector<String>::const_iterator it = m_RegFolders.begin(); it != m_RegFolders.end(); ++it)
              {
                // SJSJSJ Bodge - put .reg into dll as resource in future
                String path = *it + DeveloperName + L"_" + ModuleName + L".reg";
                if (iFileManager::singleton().isFileOrFolderCreated(path))
                {
                  RegFilename = path;
                  break;
                }
                path = *it + DeveloperName + L"\\" + ModuleName + L".reg";
                if (iFileManager::singleton().isFileOrFolderCreated(path))
                {
                  RegFilename = path;
                  break;
                }
                // SJSJSJ Bodge - put .reg into dll as resource in future
              }

              cDLL NewCandidate(CodeFilename, RegFilename, DataPath, DeveloperName, ModuleName, LoadOrderGroup, DllVer, BuiltWithSDKVer, HasModuleRegisterFunc, HasModuleInstallFunc, HasModuleUninstallFunc);

              QMSG((0, L"Found %s", CodeFilename.c_str()));

              // Another DLL of same developer and name already scanned?
              for (multiset<cDLL>::iterator i = m_DLLs.begin(); i != m_DLLs.end();)
              {
                if (i->m_DeveloperName == DeveloperName && i->m_ModuleName == ModuleName)
                {
                  if (i->m_PatchedDLLLocation.empty())
                    NewCandidate.m_PatchedDLLLocation = i->m_CodeFilename;
                  else
                    NewCandidate.m_PatchedDLLLocation = i->m_PatchedDLLLocation;
                  i = m_DLLs.erase(i);
                }
                else
                  ++i;
              }

              // Insert DLL into the multiset
              m_DLLs.insert(NewCandidate);
              m_NewDLLs.push_back(NewCandidate);
            }
          }
          FreeLibrary(hDllInstance);
        }
        catch (cRecoverable & Reason)
        {
          QSOS((L"Examining \"%s\" failed: %s, dll ignored....", CodeFilenameNoPath.c_str(), Reason.getMsg().c_str()));
        }
      }

      const bool isAborting = (iBootManager::singleton().getState() >= iBootManager::eBootState_CleanExiting);
      return isAborting ? eNextNode_Parent : eNextNode_NextChild;
    }

    const list<cDLL> & getDLLsFound() const { return m_NewDLLs; }

  private :

    list<cDLL> m_NewDLLs;
    multiset<cDLL> & m_DLLs;
    const unsigned long m_MinGroup;
    const unsigned long m_MaxGroup;
    const vector<String> m_RegFolders;
    const vector<String> m_DataFolders;
};
