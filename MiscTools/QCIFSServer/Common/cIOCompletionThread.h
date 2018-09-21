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

//cIOCompletionThread.h

#pragma once

#include "primatives.h"

class cIOCompletionThread : public vfs::cThread
{
public:
  cIOCompletionThread(HANDLE hIOCompletionPort, int threadID);
  ~cIOCompletionThread(void);

  void QAPI runLoop();
  void QAPI signalLoopStop() {m_bRun = false;}

  DWORD getThreadID();

private:
  HANDLE m_hIOCompletionPort;
  const int m_threadID;
  DWORD m_kernelThreadID;
  bool m_bRun;
  cEvent threadIDSet;
};
