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

//cIOCompletionPort.h

#pragma once

#include "iIOCompletionPort.h"

class cIOCompletionThread;

using namespace vfs;

class cIOCompletionPort : public iIOCompletionPort, public cRefCount
{
public:
  cIOCompletionPort(const unsigned int threads);
  ~cIOCompletionPort(void);

  bool PostMessage(cPtr<iCompletionMsg> pMsg);
  bool AddCompletionKey(ULONG_PTR pCompletionKey);
  HANDLE *AddFileHandle(const HANDLE hFile);
  //unsigned int ThreadCount() {return m_IOCompletionThreads.size();}
  std::list<int> getThreadIDs() {return m_threadIDs;}

private:
  HANDLE m_hCompletionPort;
  std::list<cPtr<cIOCompletionThread> > m_IOCompletionThreads;
  std::list<int> m_threadIDs;
};