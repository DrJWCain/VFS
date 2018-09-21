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

#include "QKernelExec.h"

// --- Use Namespace ---------------------------------------------------------------------

using namespace std;
using namespace vfs;




// =======================================================================================
// === cModuleRegistrar ==================================================================
// =======================================================================================

class cModuleRegistrar : public iModuleRegistrar, public cRefCount
{
	public :

    QDEFINE_SMARTPTR(cModuleRegistrar);

    // iModuleRegistrar
    void QAPI addClass(iFactory::iConstructor::Ptr NewConstructor)
    {
      // SJSJSJ Eventually do something more interesting in here. i.e. remember registrations in the registry
      gKernel->m_Factory->registerClass(NewConstructor);
    }
    void QAPI addATLObjects(_ATL_OBJMAP_ENTRY * const ObjectMap);
    vector<_ATL_OBJMAP_ENTRY> QAPI getATLObjects() const;
    ATL::CAtlModule * QAPI getATLModule() const;
    void QAPI addATLTypeLibrary(const unsigned int IndexInResources);
    std::vector<unsigned int> QAPI getATLTypeLibraryResourceIndices() const;
    void QAPI aliasModule(const String & Developer, const String & ModuleName);
    void QAPI deprecateModule(const String & Developer, const String & ModuleName);

    // cModuleRegistrar
		cModuleRegistrar(const HINSTANCE DllInstanceHandle);
    HINSTANCE getDllInstanceHandle() const;
    const list<pair<String, String> > & getAliases() const { return m_Aliases; }
    const list<pair<String, String> > & getDeprecates() const { return m_Deprecates; }

  public :

    list<pair<String, String> > m_Aliases;
    list<pair<String, String> > m_Deprecates;

    vector<_ATL_OBJMAP_ENTRY> m_Objects;
    vector<unsigned int>      m_TypeLibraryResourceIndices;

    class cATLModule : public ATL::CAtlDllModuleT<cATLModule>, public cRefCount
    {
      public :

        QDEFINE_SMARTPTR(cATLModule);

        cATLModule(const HINSTANCE DllInstanceHandle) :
          m_DllInstanceHandle(DllInstanceHandle)
        {
          QVERIFY(m_DllInstanceHandle != 0);
        }

	      virtual HRESULT AddCommonRGSReplacements(IRegistrarBase* pRegistrar) throw()
	      {
          HRESULT hr = S_OK;

          {
		        Char Path[_MAX_PATH];
		        (void)GetModuleFileName(m_DllInstanceHandle, Path, _MAX_PATH);

            hr = pRegistrar->AddReplacement(L"DLLMODULE", Path);
            if (FAILED(hr)) return hr;
          }

          {
		        Char Path[_MAX_PATH];
		        (void)GetModuleFileName(NULL, Path, _MAX_PATH);
		        const String QuotedPath(String(L"\"") + Path + L"\"");
            hr = pRegistrar->AddReplacement(L"EXEMODULE", QuotedPath.c_str());
            if (FAILED(hr)) return hr;
          }

 		      return CAtlDllModuleT<cATLModule>::AddCommonRGSReplacements(pRegistrar);
	      }
  	    
	    private :
	    
	      const HINSTANCE m_DllInstanceHandle; 
    };
    
    const cATLModule::Ptr m_ATLModule;
    const HINSTANCE       m_DllInstanceHandle; 
};
