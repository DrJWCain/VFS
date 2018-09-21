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

/*
**  
*/

// --- Pre-Compiled Headers --------------------------------------------------------------

#include "stdafx.h"

// --- Headers ---------------------------------------------------------------------------

#include "cModuleRegistrar.h"

// --- Start of Namespace ----------------------------------------------------------------

using namespace vfs;




// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

cModuleRegistrar::cModuleRegistrar(const HINSTANCE DllInstanceHandle) :
  m_DllInstanceHandle(DllInstanceHandle),
  m_ATLModule(new cATLModule(DllInstanceHandle))
{
  // Need to do this because ATL will complain if multiple CAtlModuleT<> derivec class in
  // a compilation module.
  ATL::_pAtlModule = 0;
}




// =======================================================================================
// === aliasModule =======================================================================
// =======================================================================================

void QAPI cModuleRegistrar::aliasModule(const String & Developer, const String & ModuleName)
{
  m_Aliases.push_back(make_pair(Developer, ModuleName));
}




// =======================================================================================
// === deprecateModule ===================================================================
// =======================================================================================

void QAPI cModuleRegistrar::deprecateModule(const String & Developer, const String & ModuleName)
{
  m_Deprecates.push_back(make_pair(Developer, ModuleName));
}




// =======================================================================================
// === getATLModule ======================================================================
// =======================================================================================

ATL::CAtlModule * QAPI cModuleRegistrar::getATLModule() const
{
  return m_ATLModule.getRaw();
}




// =======================================================================================
// === getDllInstanceHandle ==============================================================
// =======================================================================================

HINSTANCE cModuleRegistrar::getDllInstanceHandle() const
{
  return m_DllInstanceHandle;
}




// =======================================================================================
// === addATLTypeLibrary/getATLTypeLibraryResourceIndices ================================
// =======================================================================================

void QAPI cModuleRegistrar::addATLTypeLibrary(const unsigned int IndexInResources)
{
  m_TypeLibraryResourceIndices.push_back(IndexInResources);
}

// ---------------------------------------------------------------------------------------

vector<unsigned int> QAPI cModuleRegistrar::getATLTypeLibraryResourceIndices() const
{
  return m_TypeLibraryResourceIndices;
}




// =======================================================================================
// === addATLObjects =====================================================================
// =======================================================================================

void QAPI cModuleRegistrar::addATLObjects(_ATL_OBJMAP_ENTRY * const ObjectMap)
{
	const _ATL_OBJMAP_ENTRY * i = ObjectMap;
	while (i->pclsid != NULL)
	{
    m_Objects.push_back(*i);
		i++;
	}
}




// =======================================================================================
// === getATLObjects =====================================================================
// =======================================================================================

vector<_ATL_OBJMAP_ENTRY> QAPI cModuleRegistrar::getATLObjects() const
{
  return m_Objects;
}
