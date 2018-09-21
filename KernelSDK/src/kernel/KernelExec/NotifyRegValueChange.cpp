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

#include "StdAfx.h"
#include ".\notifyregvaluechange.h"
//#include <boost/thread/locks.hpp>
#include <boost/foreach.hpp>
#include <process.h>

using namespace vfs;

cNotifyRegValueChange::sRegProperties::sRegProperties(const sRegEventId& id, iRegEventObserver::Ptr observer) : mSubKey(NULL), mEvent(CreateEvent(NULL, FALSE, FALSE, NULL)) 
{
  LONG ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, id.mSubKeyName.c_str(), NULL, KEY_READ, &mSubKey);
  if(0 == ret)
  {
    ret = RegNotifyChangeKeyValue(mSubKey, FALSE, REG_NOTIFY_CHANGE_LAST_SET, mEvent, TRUE);
    if(0 == ret)
    {
      mObservers.insert(observer);
    }
  }
}

LONG cNotifyRegValueChange::sRegProperties::regEvent()
{
  return RegNotifyChangeKeyValue(mSubKey, FALSE, REG_NOTIFY_CHANGE_LAST_SET, mEvent, TRUE);
}

void cNotifyRegValueChange::sRegProperties::closeHandles() 
{
  if(mSubKey)
  {
    RegCloseKey(mSubKey);
    mSubKey = NULL;
  }
  if(mEvent)
  {
    CloseHandle(mEvent);
    mEvent = NULL;
  }
}

cNotifyRegValueChange::cNotifyRegValueChange(void)
: mStopEvent(CreateEvent(NULL, TRUE, FALSE, NULL))
, mAddSemaphore(CreateSemaphore(NULL, 0, 1024, NULL))
, mSyncHandles(2)
, mValue(4)
{
  mNotifyThread.reset((HANDLE) _beginthreadex(NULL, 0, threadProc, this, 0, NULL));  
}

cNotifyRegValueChange::~cNotifyRegValueChange(void)
{
  SetEvent(mStopEvent);
  WaitForSingleObject(mNotifyThread, INFINITE);

  BOOST_FOREACH(tKeyToEventMap::value_type& item, mKeyToEventMap)
  {
    item.second.closeHandles();
  }
}

unsigned __stdcall cNotifyRegValueChange::threadProc( void * param)
{
  return static_cast<cNotifyRegValueChange*>(param)->run();
}

bool cNotifyRegValueChange::addKeyValueChangeObserver(const sRegEventId& regId, const iRegEventObserver::Ptr& observer)
{
  //boost::lock_guard<boost::mutex> lock(mMutex);
  cLockGuard threadSafety(&mMutex);
  // We need to add the notifier immediately if the id already exists 
  // to prevent the client from missing change happening during thread context switching.
  tKeyToEventMap::iterator i = mKeyToEventMap.find(regId);
  if(i != mKeyToEventMap.end())
  {
    i->second.mObservers.insert(observer);
  }
  else
  {
    mAddQueue.push_back(sRegEventIdObserver(regId, observer));
    ReleaseSemaphore(mAddSemaphore, 1, NULL);
  }
  return true;
}

bool cNotifyRegValueChange::removeKeyValueChangeObserver(const sRegEventId& regId, const iRegEventObserver::Ptr& observer)
{
  //boost::lock_guard<boost::mutex> lock(mMutex);
  cLockGuard threadSafety(&mMutex);
  // We have to remove the notifier immediately since its life time may end after this call is returned.
  tKeyToEventMap::iterator i = mKeyToEventMap.find(regId);
  if(i != mKeyToEventMap.end())
  {
    std::set<iRegEventObserver::Ptr>::iterator itObserver = i->second.mObservers.find(observer);
    if(itObserver != i->second.mObservers.end())
    {
      i->second.mObservers.erase(itObserver);
      if(i->second.mObservers.empty())
      {
        SetEvent(i->second.mEvent); // This causes the entry to be removed.
      }
      return true;
    }
  }
  return false;
}

void cNotifyRegValueChange::addObserver()
{
  //boost::lock_guard<boost::mutex> lock(mMutex);
  cLockGuard threadSafety(&mMutex);
  while(!mAddQueue.empty())
  {
    sRegEventIdObserver& p = mAddQueue.front();
    tKeyToEventMap::iterator i = mKeyToEventMap.find(p.mRegEventId);
    if(mKeyToEventMap.end() == i)
    {
      sRegProperties regProp(p.mRegEventId, p.mObserver);
      if(regProp.mSubKey && regProp.mEvent)
      {
        mKeyToEventMap.insert(std::make_pair(p.mRegEventId, regProp));
        buildHandleArray();
      }
      else
      {
        regProp.closeHandles();
      }
    }
    else
    {
      i->second.mObservers.insert(p.mObserver);
    }
    mAddQueue.pop_front();
  }
}


void cNotifyRegValueChange::eventSignaled(int index)
{
  if((size_t) index >= mKeyToEventMap.size())
  {
    QTRACE((L"*** cNotifyRegValueChange::eventSignaled() - invalid index(%d), mKeyToEventMap.size(%Iu)\n", index, mKeyToEventMap.size()));
    return;
  }

  tKeyToEventMap::iterator i = mKeyToEventMap.begin();
  std::advance(i, index);

  DWORD bufSize = mValue.size();
  LONG ret = RegQueryValueEx(i->second.mSubKey, i->first.mValueName.c_str(), NULL, NULL, &mValue[0], &bufSize);
  if(ERROR_MORE_DATA == ret || bufSize > mValue.size())
  {
    mValue.resize(bufSize);
    ret = RegQueryValueEx(i->second.mSubKey, i->first.mValueName.c_str(), NULL, NULL, &mValue[0], &bufSize);
  }

  if(ERROR_SUCCESS == ret)
  {
    //boost::lock_guard<boost::mutex> lock(mMutex);
    cLockGuard threadSafety(&mMutex);
    if(!i->second.mObservers.empty())
    {
      i->second.regEvent();
      BOOST_FOREACH(iRegEventObserver::Ptr const& p, i->second.mObservers)
      {
        p->onRegKeyValueChanged(i->first, &mValue[0], bufSize, ret);
      }
    }
    else
    {
      i->second.closeHandles();
      mKeyToEventMap.erase(i);
      buildHandleArray();
    }
  }
}

void cNotifyRegValueChange::buildHandleArray()
{
  mSyncHandles.resize(mKeyToEventMap.size() + 2);
  int i = 2;
  BOOST_FOREACH(tKeyToEventMap::value_type& item, mKeyToEventMap)
  {
    mSyncHandles[i++] = item.second.mEvent;
  }
}


unsigned cNotifyRegValueChange::run()
{
  bool stop = false;
  DWORD ret = 0;
  mSyncHandles[0] = (HANDLE) mStopEvent; 
  mSyncHandles[1] = (HANDLE) mAddSemaphore; 

  // The registry event handles in m_syncHandle is ordered in the same way as mKeyToEventMap
  do 
  {
    ret = WaitForMultipleObjects(mSyncHandles.size(), &mSyncHandles[0], FALSE, INFINITE);
    switch(ret)
    {
    case WAIT_OBJECT_0:
      stop = true;
      break;
    case WAIT_OBJECT_0 + 1:
      addObserver();
      break;
    default:
      eventSignaled(ret - WAIT_OBJECT_0 - 2);
    }
  } while (!stop);

  return 0;
}
