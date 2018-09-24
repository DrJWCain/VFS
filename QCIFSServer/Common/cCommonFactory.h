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

//cCommonFactory.h
#pragma once

#include "iCommonFactory.h"

class cWinErrorCodes;

class cCommonFactory : public iCommonFactory, public vfs::cRefCount
{
public:
  QDEFINE_SINGLETON_IMPL (cCommonFactory, L"{5AA559B2-9E83-43b2-BBE9-952137A97D13} // cCommonFactory", iCommonFactory);

  cCommonFactory();
  virtual vfs::cPtr<iIOCompletionPort> createIOCP(const int threads);

  virtual std::vector<int> getWinNTCodes(const int Win32Code);
  virtual int getWin32Code(const int WinNTCode);

  virtual vfs::cPtr<iEvent> createEvent();

private:
  vfs::cPtr<cWinErrorCodes> mWinErrorCodes;
};

