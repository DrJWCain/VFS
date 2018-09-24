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

//iThread.cpp

#include "stdafx.h"
#include "iThreadHeader.h"

///////////////////////////////////////////////////////////////////////////////

class cThreadImpl : public cRefCount
{
  cThreadImpl(iThread* thread);
  ~cThreadImpl();

  friend iThread;
  cPtr<iThread> Container;
  HANDLE ThreadHandle;
  unsigned int ThreadID;

  static unsigned int _stdcall StaticBaseThreadStart(void*);
};

unsigned int _stdcall cThreadImpl::StaticBaseThreadStart(void* ptr)
{
  iThread* thread = reinterpret_cast<iThread*>(ptr);
  cPtr<iThread> Container = thread->__ThreadPimpl->Container;
  thread->__ThreadPimpl->Container.invalidate();

  thread->runLoop();

  Container.invalidate();
  _endthreadex(0);
  return 0;
}

cThreadImpl::cThreadImpl(iThread* thread) : Container(thread), ThreadID(0)
{
  ThreadHandle = (HANDLE)_beginthreadex(0, 0, StaticBaseThreadStart, thread, CREATE_SUSPENDED, &ThreadID);
  ResumeThread(ThreadHandle);
}

cThreadImpl::~cThreadImpl()
{
  CloseHandle(ThreadHandle);
}

///////////////////////////////////////////////////////////////////////////////

iThread::iThread()
{
  __ThreadPimpl = new cThreadImpl(this);
}

iThread::~iThread()
{
  delete __ThreadPimpl;
}

unsigned int iThread::threadID()
{
  return __ThreadPimpl->ThreadID;
}

void iThread::waitLoopComplete()
{
  WaitForSingleObject(__ThreadPimpl->ThreadHandle, INFINITE);
}

