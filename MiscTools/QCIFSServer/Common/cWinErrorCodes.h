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

//cWinErrorCodes.h

#pragma once

#include "QKernelBase.h"

class cWinErrorCodes : public vfs::cRefCount
{
public:
  QDEFINE_SMARTPTR(cWinErrorCodes);

  cWinErrorCodes(void);
  ~cWinErrorCodes(void);

  std::vector<int> getWinNTCodes(const int Win32Code) const;
  int getWin32Code(const int WinNTCode) const;

private:
  void addCodes(const int WinNTErrCode, const int Win32ErrCode);

private:
  typedef std::multimap<int, int> tErrCodeMultiMap;
  static tErrCodeMultiMap m_32toNTMap;

  typedef std::multimap<int, int> tErrCodeMap;
  static tErrCodeMap m_NTto32Map;

  vfs::cLock init;
};

