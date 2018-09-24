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

//cRealCompositeFactory.cpp

#include "StdAfx.h"

#include ".\crealcompositefactory.h"

#include "..\QCIFSProcessor\iQCIFSProcessor.h"

cRealCompositeFactory::cRealCompositeFactory(const vfs::String& sPath)
: m_sPath(sPath)
{
}

cRealCompositeFactory::~cRealCompositeFactory(void)
{
}

vfs::cPtr<iComposite> cRealCompositeFactory::create()
{
  DWORD dw;
  BY_HANDLE_FILE_INFORMATION bhi;
  return iQCIFSProcessor::singleton().getFileComposite(m_sPath
    , GENERIC_READ | GENERIC_WRITE
    , FILE_SHARE_READ
    , NULL
    , OPEN_EXISTING
    , FILE_ATTRIBUTE_NORMAL
    , NULL
    , &bhi
    , dw);
}