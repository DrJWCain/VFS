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

//iCommonFactory.h

#pragma once

class iIOCompletionPort;
class iEvent;

class iCommonFactory : public vfs::iFactory::iInstance
{
public:
  QDEFINE_SINGLETON_IFACE (iCommonFactory, L"{F76F207C-3A8A-4509-B6A4-9CC66D7028EB} // iCommonFactory", vfs::iFactory::iInstance);
  virtual ~iCommonFactory() {}

  virtual vfs::cPtr<iIOCompletionPort> createIOCP(const int threads) = 0;

  virtual std::vector<int> getWinNTCodes(const int Win32Code) = 0;
  virtual int getWin32Code(const int WinNTCode) = 0;

  virtual vfs::cPtr<iEvent> createEvent() = 0;
};

