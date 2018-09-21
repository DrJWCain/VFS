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

//cCommonFactory.cpp

#include "StdAfx.h"

#include "cCommonFactory.h"
#include "cIOCompletionPort.h"
#include "cWinErrorCodes.h"
#include "primatives.h"

cCommonFactory::cCommonFactory()
  : mWinErrorCodes(new cWinErrorCodes)
{
}

vfs::cPtr<iIOCompletionPort> cCommonFactory::createIOCP(const int threads)
{
  return new cIOCompletionPort(threads);
}

std::vector<int> cCommonFactory::getWinNTCodes(const int Win32Code)
{
  return mWinErrorCodes->getWinNTCodes(Win32Code);
}

int cCommonFactory::getWin32Code(const int WinNTCode)
{
  return mWinErrorCodes->getWin32Code(WinNTCode);
}

vfs::cPtr<iEvent> cCommonFactory::createEvent()
{
  return new cEvent();
}

