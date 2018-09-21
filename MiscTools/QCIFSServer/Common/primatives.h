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

//#include "iPointer.h"

#pragma once

//
//class cLock
//{
//  CRITICAL_SECTION cs;
//public:
//  cLock();
//  ~cLock();
//  void lock();
//  void unlock();
//};

class iEvent
{
public:
  virtual ~iEvent() {}
  virtual void set() = 0;
  virtual void reset() = 0;
  virtual void wait() = 0;
};

class cEvent : public iEvent, public vfs::cRefCount
{
  HANDLE Event;
public:
  cEvent();
  ~cEvent();
  void set();
  void reset();
  void wait();
};
//
//template <typename T>
//class monitor_guard
//{
//  T& Instance;
//  cLock& Lock;
//public:
//  monitor_guard(T& instance, cLock& lock)
//    : Instance(instance), Lock(lock)
//  {
//    Lock.lock();
//  }
//  ~monitor_guard()
//  {
//    Lock.unlock();
//  }
//  T* operator -> () { return &Instance; }
//};
//
//template <typename T>
//class monitor
//{
//  T Instance;
//  cLock Lock;
//public:
//  monitor(const T& instance) : Instance(instance)
//  {
//  }
//  monitor_guard<T> operator -> ()
//  {
//    return monitor_guard<T>(Instance, Lock);
//  }
//  monitor_guard<T> lock()
//  {
//    return monitor_guard<T>(Instance, Lock);
//  }
//  T* unsafe()
//  {
//    return &Instance;
//  }
//};
//
//template <typename T>
//class monitor_guard_ptr
//{
//  cPtr<T>& Instance;
//  cLock& Lock;
//public:
//  monitor_guard_ptr(cPtr<T>& instance, cLock& lock)
//    : Instance(instance), Lock(lock)
//  {
//    Lock.lock();
//  }
//  ~monitor_guard_ptr()
//  {
//    Lock.unlock();
//  }
//  cPtr<T> operator -> () { return Instance; }
//};
//
//template <typename T>
//class monitor_ptr
//{
//  cPtr<T> Instance;
//  cLock Lock;
//public:
//  monitor_ptr(const cPtr<T>& instance) : Instance(instance)
//  {
//  }
//  monitor_guard_ptr<T> operator -> ()
//  {
//    return monitor_guard_ptr<T>(Instance, Lock);
//  }
//};

