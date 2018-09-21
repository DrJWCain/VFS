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

#pragma once
#include "..\..\..\..\shared\Utils\WinScopedHandleDef.h"
#include <vector>
#include <map>
#include <deque>
#include <set>
#include <boost/noncopyable.hpp>
//#include <boost/thread/mutex.hpp>
#include "QKernelExecObjectClasses.h"

namespace vfs
{
  struct sRegEventId;
  class iRegEventObserver;

class cNotifyRegValueChange : boost::noncopyable
{
public:
  cNotifyRegValueChange(void);
  ~cNotifyRegValueChange(void);

  bool addKeyValueChangeObserver(const sRegEventId& regId, const iRegEventObserver::Ptr& observer);
  bool removeKeyValueChangeObserver(const sRegEventId& regId, const iRegEventObserver::Ptr& observer);

private:
  struct sRegEventIdObserver
  {
    sRegEventIdObserver(const sRegEventId& id, iRegEventObserver::Ptr obs) : mRegEventId(id), mObserver(obs) {}
    sRegEventId mRegEventId;
    iRegEventObserver::Ptr mObserver;
  };

  struct sRegProperties
  {
    sRegProperties(const sRegEventId& id, iRegEventObserver::Ptr observer); 
    LONG regEvent();
    void closeHandles();

    HKEY mSubKey;
    HANDLE mEvent;
    typedef std::set<iRegEventObserver::Ptr> tNotifySet;
    tNotifySet mObservers;

  };

  static unsigned __stdcall threadProc( void * );

  unsigned run();
  void addObserver();
  void eventSignaled(int index);
  void buildHandleArray();

  tKernelHandle mStopEvent;
  tKernelHandle mAddSemaphore;
  tKernelHandle mNotifyThread;
  std::vector<HANDLE> mSyncHandles;

  //boost::mutex mMutex;
  cLock mMutex;
  typedef std::map<sRegEventId, sRegProperties> tKeyToEventMap;
  tKeyToEventMap mKeyToEventMap;

  std::deque<sRegEventIdObserver> mAddQueue;

  std::vector<unsigned char> mValue;
};

} //namespace vfs
