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
#include "QKernelBase.h"
//}}}

using namespace vfs;

//{{{
cLockGuard::cLockGuard(cLock * const lock) 
  : m_Lock(lock)
{
  m_Lock->lock();
}
//}}}
//{{{
cLockGuard::cLockGuard(const cLockGuard & other)
{
  other.m_Lock->lock();
  m_Lock = other.m_Lock;
}
//}}}
//{{{
cLockGuard::~cLockGuard()
{
  m_Lock->unlock();
}
//}}}
//
//{{{
void cLockGuard::operator= (const cLockGuard& other)
{
  other.m_Lock->lock();
  m_Lock->unlock();
  m_Lock = other.m_Lock;
}
//}}}
