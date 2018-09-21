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

//{{{  headers
#include "stdafx.h"
#include "fatal.h"

#include <strsafe.h>
//}}}

using namespace vfs;

//{{{
cLock::cLock()
{
  InitializeCriticalSection (&mSection);
}
//}}}
//{{{
cLock::cLock (const unsigned int spinCount)
{
#if defined(_DEBUG)
  QKERNELBASEVERIFY(spinCount > 0);
  QKERNELBASEVERIFY(spinCount <= kLockMaxSpinCount);
#endif
  InitializeCriticalSectionAndSpinCount (&mSection, spinCount);
}
//}}}
//{{{
cLock::~cLock()
{
#if defined(_DEBUG)
  if (mSection.OwningThread != NULL)
  {
    const int kBufSize = 512;
    Char tmp[kBufSize];
    StringCbPrintf (tmp, kBufSize, L"cLock::~cLock() : Critical Section still locked on destruction. Owning Thread is %p.\n", mSection.OwningThread);
    QKERNELBASEVERIFYMSG((mSection.OwningThread == NULL), tmp);
  }
#endif
  DeleteCriticalSection (&mSection);
}
//}}}
//{{{
void cLock::lock()
{
  EnterCriticalSection (&mSection);
}
//}}}
//{{{
bool cLock::tryLock()
{
  return (TryEnterCriticalSection(&mSection) == TRUE);
}
//}}}
//{{{
void cLock::unlock()
{
#if defined(_DEBUG)
  if (GetCurrentThreadId() != (DWORD)mSection.OwningThread)
  {
    const int kBufSize = 512;
    Char tmp[kBufSize];
    StringCbPrintf (tmp, kBufSize, L"cLock::unlock() : Critical Section unlock in wrong thread. Owning Thread is %p.\n", mSection.OwningThread);
    QKERNELBASEVERIFYMSG((GetCurrentThreadId() == (DWORD)mSection.OwningThread), tmp);
  }
#endif
  LeaveCriticalSection (&mSection);
}
//}}}