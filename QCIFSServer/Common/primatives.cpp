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

//primatives.h

#include "stdafx.h"
#include "primatives.h"
//
//cLock::cLock()
//{
//  InitializeCriticalSection(&cs);
//}
//
//cLock::~cLock()
//{
//  DeleteCriticalSection(&cs);
//}
//
//void cLock::lock()
//{
//  EnterCriticalSection(&cs);
//}
//
//void cLock::unlock()
//{
//  LeaveCriticalSection(&cs);
//}

cEvent::cEvent()
{
  Event = CreateEvent(0, TRUE, FALSE, 0);
  reset();
}

cEvent::~cEvent()
{
  CloseHandle(Event);
}

void cEvent::set()
{
  SetEvent(Event);
}

void cEvent::reset()
{
  ResetEvent(Event);
}

void cEvent::wait()
{
  WaitForSingleObject(Event,INFINITE);
  reset();
}

