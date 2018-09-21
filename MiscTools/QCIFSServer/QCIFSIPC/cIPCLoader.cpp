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

//cIPCLoader.cpp

#include "StdAfx.h"

#include "cIPCLoader.h"

#include "..\QCIFSFwk\iChildLoaderVisitor.h"
#include "..\QCifsFwk\iQCIFSFwkHelper.h"
#include "..\QCifsFwk\iWriteInfo.h"
#include "cIPCWriteCallback.h"

using namespace vfs;

cIPCLoader::cIPCLoader(void)
{
}

cIPCLoader::~cIPCLoader(void)
{
}

#define FILE_ATTRIBUTE_NAME_PIPE 0x10000000

void cIPCLoader::registerListener(const vfs::cPtr<iChildLoaderVisitor> pChildListener)
{
  QSOS((L"cIPCLoader::registerListener"));
  if (m_pSearchListener.isNull()
    || pChildListener.isValid() && pChildListener != m_pSearchListener)
  {
    pChildListener->updateFile(new cIPCWriteCallback(L"srvsvc"), iWriteInfo::singleton().makeFileInfo(FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_NAME_PIPE, L"srvsvc"));
    pChildListener->updateFile(new cIPCWriteCallback(L"wkssvc"), iWriteInfo::singleton().makeFileInfo(FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_NAME_PIPE, L"wkssvc"));
    m_pSearchListener = pChildListener;
  }
}