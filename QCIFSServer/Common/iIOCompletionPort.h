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

//iIOCompletionPort.h

#pragma once

#include "iCompletionMsg.h"

class iIOCompletionPort
{
public:
  virtual ~iIOCompletionPort() {}

  virtual bool PostMessage(vfs::cPtr<iCompletionMsg> pMsg) = 0;
  virtual bool AddCompletionKey(ULONG_PTR pCompletionKey) = 0;
  virtual HANDLE *AddFileHandle(const HANDLE hFile) = 0;
  virtual std::list<int> getThreadIDs() = 0;
};

