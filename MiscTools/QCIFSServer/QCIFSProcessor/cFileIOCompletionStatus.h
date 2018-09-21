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

//cFileIOCompletionStatus.h

#pragma once

#include "..\common\icompletionmsg.h"

class iEvent;

class cFileIOCompletionStatus : public iCompletionMsg, public vfs::cRefCount
{
public:
  cFileIOCompletionStatus(void);
  ~cFileIOCompletionStatus(void);

  void deliver(ULONG_PTR pCompletionKey, const DWORD dwBytes);
  void err(ULONG_PTR pCompletionKey, const DWORD err);

  DWORD getStatus();
  DWORD getBytes();

private:
  DWORD m_dwStatus;
  DWORD m_dwBytes;
  vfs::cPtr<iEvent> m_statusSet;
};
