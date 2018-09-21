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

/*
**  
*/

// --- Pre-Compiled Headers --------------------------------------------------------------

#include "stdafx.h"

// --- Headers ---------------------------------------------------------------------------

#include "cFile.h"
#include "cActivityLog.h"
#include <algorithm>
#include <limits>
//#include "../../MiscTools/ActivityMon\iStackDump.h"

// --- Use Namespace ---------------------------------------------------------------------

using namespace vfs;

namespace
{
  const unsigned int kMaxActivity = 64;
  const unsigned int kMaxNest = 10;
  const unsigned int kMaxMap  = 16;
  const unsigned int kMaxSet  = 16;

  const unsigned int kActiveTimestamp = 0xffffffff;
};
// =======================================================================================
// === Statics ===========================================================================
// =======================================================================================

UInt64         cActivityLog::TimeZero = 0;
UInt64         cActivityLog::TickFreq = 500000000;
unsigned int   cActivity::CurrentIdent = 0;

cLock cActivity::mTriggerLock;
cActivityTrigger cActivity::mTrigger;                           // currently active trigger - copied from mTriggerStack
unsigned int cActivity::mTriggerIndex = 0;                      // index of next trigger in the stack
std::vector<cActivityTrigger> cActivity::mTriggerStack;   // consequetive triggers - trigger is actioned by the final trigger (eTriggerNext not set)


// iACtivityPool classes
//{{{
class cActivityNest    : public iActivityPool, public cRefCount
{ // for nested activity calls
public:
  QDEFINE_SMARTPTR(cActivityNest);
  //{{{
  cActivityNest (iActivity::Ptr (*funcCreateActivity)(const String& nameSuffix))
    : mIndex(0), mFuncCreateActivity(funcCreateActivity)
  {
    QVERIFY(mFuncCreateActivity);
    mNest.push_back (mFuncCreateActivity(L""));     // there is always a base activity
  }
  //}}}
  //{{{
  cActivityNest (const String nameSuffix, iActivity::Ptr (*funcCreateActivity)(const String& nameSuffix))
    : mNameSuffix(nameSuffix), mIndex(0), mFuncCreateActivity(funcCreateActivity)
  {
    QVERIFY(mFuncCreateActivity);
    mNest.push_back (mFuncCreateActivity(nameSuffix));     // there is always a base activity
  }
  //}}}

  // iActivityPool
  //{{{
  iActivity::Ptr getActivity()
  {
    QVERIFY (mFuncCreateActivity);
    unsigned int index = mIndex++;              // mIndex will continue to count above kMaxNest

    if (index < (unsigned int)mNest.size())
      return mNest[index];

    if (index < kMaxNest)   // limit to 10 nested traces
    {
      iActivity::Ptr activity = mFuncCreateActivity(mNameSuffix + L"-det" + lexical_cast<String>(mIndex-1));
      activity->setType (iActivity::kDetail);
      mNest.push_back (activity);
      return activity;
    }

    cActivity::Ptr activity = ptr_cast<cActivity::Ptr>(mNest.back());
    QSOS((L"cActivityNest -- limiting to %d traces for %s, mIndex=%d", kMaxNest, activity->title().c_str(), mIndex));
    return activity;                                 // stick on the last trace if > 10
  }
  //}}}
  iActivity::Ptr getActivity (unsigned int /*index*/)     { return getActivity(); }
  void release (unsigned int /*index*/);
  bool isBase (unsigned int /*index*/) const              { return (mIndex <= 1); }
  bool isInuse() const                                    { return (mIndex > 0); }

  // to allow inclusion in std::map
  cActivityNest () : mIndex(0), mFuncCreateActivity(NULL) { }
  //{{{
  //void setNameSuffix (const String nameSuffix, iActivity::Ptr (*funcCreateActivity)(const String nameSuffix))
  //{
    //mNameSuffix = nameSuffix;
    //mFuncCreateActivity = funcCreateActivity;
  //}
  //}}}
  //{{{
  bool contains (unsigned int activityIdent) const
  {
    for (tNest::const_iterator it = mNest.begin(); it != mNest.end(); it++)
    {
      if ((*it)->ident() == activityIdent)
        return true;
    }
    return false;
  }
  //}}}
private:
  typedef std::vector<iActivity::Ptr> tNest;
  String mNameSuffix;
  unsigned int mIndex;
  tNest mNest;
  iActivity::Ptr (*mFuncCreateActivity)(const String& nameSuffix);
};
//{{{
void cActivityNest::release (unsigned int /*index*/)
{
  if (mIndex > 0)
    mIndex--;
  else
    QSOS((L"cActivityNest::release : all levels already released"));
}
//}}}
//}}}
//{{{
class cActivityMap     : public iActivityPool, public cRefCount
{ // for an array of activities,             eg by procNum
public:
  QDEFINE_SMARTPTR(cActivityMap);
  //{{{
  cActivityMap (iActivity::Ptr (*funcCreateActivity)(const String& nameSuffix))
    : mFuncCreateActivity(funcCreateActivity)
  { QVERIFY(mFuncCreateActivity); }
  //}}}

  // iActivityPool
  iActivity::Ptr getActivity() { return getActivity (0); }
  //{{{
  iActivity::Ptr getActivity (unsigned int index)
  {
    {
      cLockGuard g (&mLock);
      if (mMap.find(index) != mMap.end())
        return mMap[index];
    }

    iActivity::Ptr activity = mFuncCreateActivity (lexical_cast<String>(index));

    cLockGuard g (&mLock);
    mMap[index] = activity;
    return activity;
  }
  //}}}
  void release (unsigned int index) {}                   // never released
  bool isBase (unsigned int index) const { return true; }

private:
  mutable cLock mLock;
  std::map<unsigned int, iActivity::Ptr> mMap;
  iActivity::Ptr (*mFuncCreateActivity)(const String& activityName);
};
//}}}
//{{{
class cActivitySet     : public iActivityPool, public cRefCount
{ // for activities callable from different threads
public:
  QDEFINE_SMARTPTR(cActivitySet);

  cActivitySet (iActivity::Ptr (*funcCreateActivity)(const String& nameSuffix))
    : mCurrentSuffix(L'A'), mCurTimestamp(0), mFuncCreateActivity(funcCreateActivity)
  { QVERIFY(mFuncCreateActivity); }

  // iActivityPool
  iActivity::Ptr getActivity();
  iActivity::Ptr getActivity (unsigned int activityIdent) { return getActivity(); }
  void release (unsigned int activityIdent);
  bool isBase (unsigned int activityIdent) const          { return true; }

private:
  mutable cLock mLock;
  Char mCurrentSuffix;
  unsigned int mCurTimestamp;
  iActivity::Ptr (*mFuncCreateActivity)(const String& nameSuffix);

  //{{{
  class cMapEntry
  {
  public:
    cMapEntry (unsigned int threadId, iActivity::Ptr activity)
     : mTimestamp(kActiveTimestamp), mThreadId(threadId), mActivity(activity) {}

    unsigned int timestamp()                            { return mTimestamp; }
    unsigned int threadId()                             { return mThreadId; }
    iActivity::Ptr activity()                           { return mActivity; }
    bool matches (unsigned int activityIdent)           { return mActivity->ident() == activityIdent; }

    iActivity::Ptr getActivity()                        { mTimestamp = kActiveTimestamp; return mActivity; }
    iActivity::Ptr getActivity (unsigned int threadId)  { mTimestamp = kActiveTimestamp; mThreadId = threadId; return mActivity; }

    void setTimestamp (unsigned int val)                { mTimestamp = val; }

  private:
    unsigned int mTimestamp;
    unsigned int mThreadId;
    iActivity::Ptr mActivity;
  };
  //}}}
  typedef std::vector<cMapEntry> tActivityList;
  typedef tActivityList::iterator tActivityListIt;

  tActivityList mActivities;
};
//{{{
iActivity::Ptr cActivitySet::getActivity()
{
  unsigned int threadId = iThreadManager::singleton().getCurrentThreadId();
  cLockGuard g (&mLock);

  unsigned int oldestTimestamp = kActiveTimestamp;
  tActivityListIt oldestIt = mActivities.end();

  for (tActivityListIt it = mActivities.begin(); it != mActivities.end(); ++it)
  {
    if (it->threadId() == threadId)
      return it->getActivity();

    if (it->timestamp() < oldestTimestamp)
    {
      oldestTimestamp = it->timestamp();
      oldestIt = it;
    }
  }

  if (oldestIt != mActivities.end())
    return oldestIt->getActivity (threadId);

  iActivity::Ptr activity = mFuncCreateActivity(lexical_cast<String>(mCurrentSuffix++));

  mActivities.push_back (cMapEntry (threadId, activity) );
  return activity;
}
//}}}
//{{{
void cActivitySet::release (unsigned int activityIdent)
{
  {
    cLockGuard g (&mLock);

    for (tActivityListIt it = mActivities.begin(); it != mActivities.end(); ++it)
    {
      if (it->matches(activityIdent))
      {
        it->setTimestamp (++mCurTimestamp);
        return;
      }
    }
  } // scope of cLockGuard
  QSOS((L"cActivitySet::release : Activity %d not found", activityIdent));
}
//}}}
//}}}

//{{{
class cActivityNestMap : public iActivityPool, public cRefCount
{ // for an array of nested activity calls,  eg by procNum
public:
  QDEFINE_SMARTPTR(cActivityNestMap);
  //{{{
  cActivityNestMap (iActivity::Ptr (*funcCreateActivity)(const String& nameSuffix))
    : mFuncCreateActivity(funcCreateActivity)
  { QVERIFY(mFuncCreateActivity); }
  //}}}

  // iActivityPool
  iActivity::Ptr getActivity()            { return getActivity (0); }
  iActivity::Ptr getActivity (unsigned int index);
  void release (unsigned int index);
  bool isBase (unsigned int index) const;

private:
  mutable cLock mLock;
  typedef std::map<unsigned int, cActivityNest> tActivityMap;
  tActivityMap mMap;
  iActivity::Ptr (*mFuncCreateActivity)(const String& activityName);
};
//{{{
iActivity::Ptr cActivityNestMap::getActivity (unsigned int index)
{
  {
    cLockGuard g (&mLock);
    tActivityMap::iterator it = mMap.find (index);
    if (it != mMap.end())
      return (*it).second.getActivity (0);
    if (mMap.size() >= 16)      // limit to 16 entries
    {
      it = (mMap.end())--;        // return the highest numbered entry
      return (*it).second.getActivity (0);
    }
  }

  cLockGuard g (&mLock);
  mMap[index] = cActivityNest (lexical_cast<String>(index), mFuncCreateActivity);
  return mMap[index].getActivity (0);
}
//}}}
//{{{
void cActivityNestMap::release (unsigned int index)
{
  {
    cLockGuard g (&mLock);
    tActivityMap::iterator it = mMap.find (index);
    if (it != mMap.end())
    {
      (*it).second.release(0);
      return;
    }
  }
  QSOS((L"cActivityNestMap::release : Index %d not found", index));
}
//}}}
//{{{
bool cActivityNestMap::isBase (unsigned int index) const
{
  cLockGuard g (&mLock);
  tActivityMap::const_iterator it = mMap.find (index);
  if (it == mMap.end())
    return true;
  return (*it).second.isBase (0);
}
//}}}
//}}}
//{{{
class cActivityNestSet : public iActivityPool, public cRefCount
{ // for nested activities callable from different threads
public:
  QDEFINE_SMARTPTR(cActivityNestSet);
  cActivityNestSet (iActivity::Ptr (*funcCreateActivity)(const String& nameSuffix))
    : mCurrentSuffix('A'), mCurTimestamp(0), mFuncCreateActivity(funcCreateActivity)
  { QVERIFY(mFuncCreateActivity); }

  // iActivityPool
  iActivity::Ptr getActivity();
  iActivity::Ptr getActivity (unsigned int activityIdent) { return getActivity(); }
  void release (unsigned int activityIdent);
  bool isBase (unsigned int activityIdent) const          { return true; }

private:
  mutable cLock mLock;
  Char mCurrentSuffix;
  unsigned int mCurTimestamp;
  iActivity::Ptr (*mFuncCreateActivity)(const String& nameSuffix);

  //{{{
  class cMapEntry: public cActivityNest
  {
  public:
    cMapEntry (unsigned int threadId, const String nameSuffix, iActivity::Ptr (*funcCreateActivity)(const String& nameSuffix))
     : cActivityNest(nameSuffix, funcCreateActivity), mTimestamp(0), mThreadId(threadId) {}

    unsigned int timestamp()                         { return mTimestamp; }
    unsigned int threadId()                          { return mThreadId; }

    iActivity::Ptr grabActivity()                        { mTimestamp = kActiveTimestamp; return getActivity(); }
    iActivity::Ptr grabActivity (unsigned int threadId)  { mTimestamp = kActiveTimestamp; mThreadId = threadId; return getActivity(); }

    void setTimestamp (unsigned int val)             { mTimestamp = val; }

  private:
    unsigned int mTimestamp;
    unsigned int mThreadId;
  };
  //}}}
  typedef std::vector<cMapEntry> tActivityList;
  typedef tActivityList::iterator tActivityListIt;

  tActivityList mActivities;
};
//{{{
iActivity::Ptr cActivityNestSet::getActivity()
{
  unsigned int threadId = iThreadManager::singleton().getCurrentThreadId();
  cLockGuard g (&mLock);

  unsigned int oldestTimestamp = kActiveTimestamp;
  tActivityListIt oldestIt = mActivities.end();

  for (tActivityListIt it = mActivities.begin(); it != mActivities.end(); ++it)
  {
    if (it->threadId() == threadId)
      return it->grabActivity();

    if (!it->isInuse() && (it->timestamp() < oldestTimestamp))
    {
      oldestTimestamp = it->timestamp();
      oldestIt = it;
    }
  }

  if (oldestIt != mActivities.end())
    return oldestIt->grabActivity (threadId);

  mActivities.push_back (cMapEntry (threadId, lexical_cast<String>(mCurrentSuffix++), mFuncCreateActivity) );
  return mActivities.back().grabActivity();
}
//}}}
//{{{
void cActivityNestSet::release (unsigned int activityIdent)
{
  cLockGuard g (&mLock);

  for (tActivityListIt it = mActivities.begin(); it != mActivities.end(); ++it)
  {
    if (it->contains(activityIdent))
    {
      it->setTimestamp (++mCurTimestamp);
      it->release(0);
      return;
    }
  }
}
//}}}
//}}}


//{{{  cActivityBuffer members
// =======================================================================================
// === cActivityBuffer ================================================================
// =======================================================================================

int cActivityBuffer::Count = 0;

//{{{
cActivityBuffer::cActivityBuffer (int bufsize)    // in chunks of kLogSize
  : Full(false), num(++Count)
{
  if (bufsize < 1) bufsize = 1;
  else if (bufsize > 5) bufsize = 5;

  Log = new cActivityLogEntry [kLogSize * bufsize];
  LogEnd = Log + kLogSize * bufsize;
  pCurrent = Log;
  pWrite = Log;
  //QSOS((L"create cActivityBuffer %d", Count));
  //QTRACE((L"cActivityBuffer Construct %d", num));
}
//}}}
//{{{
cActivityBuffer::~cActivityBuffer()
{
  delete[] Log;
  //QSOS((L"delete cActivityBuffer %d", Count));
  //QTRACE((L"cActivityBuffer Destruct  %d", num));
  if (--Count <= 0)
  {
    //QSOS((L"ActivityBuffer - all gone"));
  }
}
//}}}

//{{{
void cActivityBuffer::activityEnter (const cActivityBuffer* buf)
{
  if (buf->pCurrent != buf->pWrite)
  {
    if (pCurrent != pWrite)
    {
      pCurrent->EndTime = buf->pCurrent->StartTime;
      pCurrent = pWrite;
    }

    // copy across last entry, as it has not completed yet
    memcpy (pWrite, buf->pCurrent, sizeof (cActivityLogEntry));

    if (++pWrite == LogEnd)
    {
      pWrite = Log;
      Full = true;
    }
  }
}
//}}}
//{{{
void cActivityBuffer::activityStart (int activity, const int* payload, int payloadLen)
{
  UInt64 time = cActivityLog::getTime();

  if (pCurrent != pWrite)
  {
    pCurrent->EndTime = time;
    pCurrent = pWrite;
  }

  pWrite->StartTime = time;
  pWrite->EndTime = 0;
  pWrite->Activity = activity;   // already masked
  if (payload)
  {
    if (payloadLen > iActivity::kMaxPayload)
      payloadLen = iActivity::kMaxPayloadBytes;
    else
      payloadLen *= sizeof (int);
    memcpy (pWrite->Details, payload, payloadLen);
  }
  if (++pWrite == LogEnd)
  {
    pWrite = Log;
    Full = true;
  }
}
//}}}

//{{{
void cActivityBuffer::activityStart ()
{
  activityStart(0);
}
//}}}
//{{{
void cActivityBuffer::activityStart (int activity)
{
  UInt64 time = cActivityLog::getTime();

  if (pCurrent != pWrite)
  {
    pCurrent->EndTime = time;
    pCurrent = pWrite;
  }

  pWrite->StartTime = time;
  pWrite->EndTime = 0;
  pWrite->Activity = activity & kActivityMask;

  if (++pWrite == LogEnd)
  {
    pWrite = Log;
    Full = true;
  }
}
//}}}
//{{{
void cActivityBuffer::activityStart (int activity, int value)
{
  UInt64 time = cActivityLog::getTime();

  if (pCurrent != pWrite)
  {
    pCurrent->EndTime = time;
    pCurrent = pWrite;
  }

  pWrite->StartTime = time;
  pWrite->EndTime = 0;
  pWrite->Activity = (activity & kActivityMask) | (iActivity::kValue << 28);
  pWrite->Details[0] = value;

  if (++pWrite == LogEnd)
  {
    pWrite = Log;
    Full = true;
  }
}
//}}}
//{{{
void cActivityBuffer::activityStart (int activity, int value, const cXY& size)
{
  UInt64 time = cActivityLog::getTime();

  if (pCurrent != pWrite)
  {
    pCurrent->EndTime = time;
    pCurrent = pWrite;
  }

  pWrite->StartTime = time;
  pWrite->EndTime = 0;
  pWrite->Activity = (activity & kActivityMask) | (iActivity::kSize << 28);
  pWrite->Details[0] = value;
  pWrite->Details[1] = -1;
  pWrite->Details[2] = -1;
  pWrite->Details[3] = 0;
  pWrite->Details[4] = 0;
  pWrite->Details[5] = size.x;
  pWrite->Details[6] = size.y;

  if (++pWrite == LogEnd)
  {
    pWrite = Log;
    Full = true;
  }
}
//}}}
//{{{
void cActivityBuffer::activityStart (int activity, int value, const cRect& area)
{
  UInt64 time = cActivityLog::getTime();

  if (pCurrent != pWrite)
  {
    pCurrent->EndTime = time;
    pCurrent = pWrite;
  }

  pWrite->StartTime = time;
  pWrite->EndTime = 0;
  pWrite->Activity = (activity & kActivityMask) | (iActivity::kArea << 28);
  pWrite->Details[0] = value;
  pWrite->Details[1] = -1;
  pWrite->Details[2] = -1;
  pWrite->Details[3] = area.org.x;
  pWrite->Details[4] = area.org.y;
  pWrite->Details[5] = area.len.x;
  pWrite->Details[6] = area.len.y;

  if (++pWrite == LogEnd)
  {
    pWrite = Log;
    Full = true;
  }
}
//}}}
//{{{
void cActivityBuffer::activityStart (int activity, int value, const cXY& pos, const cRect& area)
{
  UInt64 time = cActivityLog::getTime();

  if (pCurrent != pWrite)
  {
    pCurrent->EndTime = time;
    pCurrent = pWrite;
  }

  pWrite->StartTime = time;
  pWrite->EndTime = 0;
  pWrite->Activity = (activity & kActivityMask) | (iActivity::kPosArea << 28);
  pWrite->Details[0] = value;
  pWrite->Details[1] = -1;
  pWrite->Details[2] = -1;
  pWrite->Details[3] = (area.org.y << 16) | (area.org.x & 0xffff);
  pWrite->Details[4] = (area.len.y << 16) | (area.len.x & 0xffff);
  pWrite->Details[5] = (pos.y      << 16) | (pos.x      & 0xffff);

  if (++pWrite == LogEnd)
  {
    pWrite = Log;
    Full = true;
  }
}
//}}}
//{{{
void cActivityBuffer::activityStart (int activity, int value, const cRect& area2, const cRect& area)
{
  UInt64 time = cActivityLog::getTime();

  if (pCurrent != pWrite)
  {
    pCurrent->EndTime = time;
    pCurrent = pWrite;
  }

  pWrite->StartTime = time;
  pWrite->EndTime = 0;
  pWrite->Activity = (activity & kActivityMask) | (iActivity::kAreaArea << 28);
  pWrite->Details[0] = value;
  pWrite->Details[1] = -1;
  pWrite->Details[2] = -1;
  pWrite->Details[3] = (area.org.y  << 16) | (area.org.x  & 0xffff);
  pWrite->Details[4] = (area.len.y  << 16) | (area.len.x  & 0xffff);
  pWrite->Details[5] = (area2.org.y << 16) | (area2.org.x & 0xffff);
  pWrite->Details[6] = (area2.len.y << 16) | (area2.len.x & 0xffff);

  if (++pWrite == LogEnd)
  {
    pWrite = Log;
    Full = true;
  }
}
//}}}

//{{{
void cActivityBuffer::activityStart (int activity, const char* description)
{
  UInt64 time = cActivityLog::getTime();

  if (pCurrent != pWrite)
  {
    pCurrent->EndTime = time;
    pCurrent = pWrite;
  }

  pWrite->StartTime = time;
  pWrite->EndTime = 0;
  pWrite->Activity = (activity & kActivityMask) | (iActivity::kString << 28);

  {
    char * const       DestString   = (char*) pWrite->Details;
    const unsigned int MaxDestChars = _INTSIZEOF(pWrite->Details);
    (void)strncpy (DestString, description, MaxDestChars);
  }

  if (++pWrite == LogEnd)
  {
    pWrite = Log;
    Full = true;
  }
}
//}}}
//{{{
void cActivityBuffer::activityEvent (int activity, const char* description)
{
  UInt64 time = cActivityLog::getTime();

  if (pCurrent != pWrite)
  {
    pCurrent->EndTime = time;
    pCurrent = pWrite;
  }

  pWrite->StartTime = time;
  pWrite->EndTime = time;
  pWrite->Activity = (activity & kActivityMask) | (iActivity::kString << 28) | kIsEvent;

  {
    char * const       DestString   = (char*) pWrite->Details;
    const unsigned int MaxDestChars = _INTSIZEOF(pWrite->Details);
    (void)strncpy (DestString, description, MaxDestChars);
  }

  if (++pWrite == LogEnd)
  {
    pWrite = Log;
    Full = true;
  }
}
//}}}

//{{{
void cActivityBuffer::activityStart (int activity, const cXYZ& info, const cXY& size)
{
  UInt64 time = cActivityLog::getTime();

  if (pCurrent != pWrite)
  {
    pCurrent->EndTime = time;
    pCurrent = pWrite;
  }

  pWrite->StartTime = time;
  pWrite->EndTime = 0;
  pWrite->Activity = (activity & kActivityMask) | (iActivity::kSize << 28);
  pWrite->Details[0] = info.x;
  pWrite->Details[1] = info.y;
  pWrite->Details[2] = info.z;
  pWrite->Details[3] = size.x;
  pWrite->Details[4] = size.y;

  if (++pWrite == LogEnd)
  {
    pWrite = Log;
    Full = true;
  }
}
//}}}
//{{{
void cActivityBuffer::activityStart (int activity, const cXYZ& info, const cXY& size2, const cXY& size)
{
  UInt64 time = cActivityLog::getTime();

  if (pCurrent != pWrite)
  {
    pCurrent->EndTime = time;
    pCurrent = pWrite;
  }

  pWrite->StartTime = time;
  pWrite->EndTime = 0;
  pWrite->Activity = (activity & kActivityMask) | (iActivity::kSizeSize << 28);
  pWrite->Details[0] = info.x;
  pWrite->Details[1] = info.y;
  pWrite->Details[2] = info.z;
  pWrite->Details[3] = size.x;
  pWrite->Details[4] = size.y;
  pWrite->Details[5] = size2.x;
  pWrite->Details[6] = size2.y;

  if (++pWrite == LogEnd)
  {
    pWrite = Log;
    Full = true;
  }
}
//}}}
//{{{
void cActivityBuffer::activityStart (int activity, const cXYZ& info, const cRect& area)
{
  UInt64 time = cActivityLog::getTime();

  if (pCurrent != pWrite)
  {
    pCurrent->EndTime = time;
    pCurrent = pWrite;
  }

  pWrite->StartTime = time;
  pWrite->EndTime = 0;
  pWrite->Activity = (activity & kActivityMask) | (iActivity::kArea << 28);
  pWrite->Details[0] = info.x;
  pWrite->Details[1] = info.y;
  pWrite->Details[2] = info.z;
  pWrite->Details[3] = area.org.x;
  pWrite->Details[4] = area.org.y;
  pWrite->Details[5] = area.len.x;
  pWrite->Details[6] = area.len.y;

  if (++pWrite == LogEnd)
  {
    pWrite = Log;
    Full = true;
  }
}
//}}}
//{{{
void cActivityBuffer::activityStart (int activity, const cXYZ& info, const cXY& pos, const cRect& area)
{
  UInt64 time = cActivityLog::getTime();

  if (pCurrent != pWrite)
  {
    pCurrent->EndTime = time;
    pCurrent = pWrite;
  }

  pWrite->StartTime = time;
  pWrite->EndTime = 0;
  pWrite->Activity = (activity & kActivityMask) | (iActivity::kPosArea << 28);
  pWrite->Details[0] = info.x;
  pWrite->Details[1] = info.y;
  pWrite->Details[2] = info.z;
  pWrite->Details[3] = (area.org.y << 16) | (area.org.x & 0xffff);
  pWrite->Details[4] = (area.len.y << 16) | (area.len.x & 0xffff);
  pWrite->Details[5] = (pos.y      << 16) | (pos.x      & 0xffff);

  if (++pWrite == LogEnd)
  {
    pWrite = Log;
    Full = true;
  }
}
//}}}
//{{{
void cActivityBuffer::activityStart (int activity, const cXYZ& info, const cRect& area2, const cRect& area)
{
  UInt64 time = cActivityLog::getTime();

  if (pCurrent != pWrite)
  {
    pCurrent->EndTime = time;
    pCurrent = pWrite;
  }

  pWrite->StartTime = time;
  pWrite->EndTime = 0;
  pWrite->Activity = (activity & kActivityMask) | (iActivity::kAreaArea << 28);
  pWrite->Details[0] = info.x;
  pWrite->Details[1] = info.y;
  pWrite->Details[2] = info.z;
  pWrite->Details[3] = (area.org.y  << 16) | (area.org.x  & 0xffff);
  pWrite->Details[4] = (area.len.y  << 16) | (area.len.x  & 0xffff);
  pWrite->Details[5] = (area2.org.y << 16) | (area2.org.x & 0xffff);
  pWrite->Details[6] = (area2.len.y << 16) | (area2.len.x & 0xffff);

  if (++pWrite == LogEnd)
  {
    pWrite = Log;
    Full = true;
  }
}
//}}}

  // these add ID (normally essence-offset) in details[0] & 0xffff (matches cLogDetails-dest)
//{{{
void cActivityBuffer::activityStartId (int activity, int id)
{
  UInt64 time = cActivityLog::getTime();

  if (pCurrent != pWrite)
  {
    pCurrent->EndTime = time;
    pCurrent = pWrite;
  }

  pWrite->StartTime = time;
  pWrite->EndTime = 0;
  pWrite->Activity = (activity & kActivityMask) | (iActivity::kIdString << 28);
  pWrite->Details[0] = id;
  pWrite->Details[1] = 0;    // null string

  if (++pWrite == LogEnd)
  {
    pWrite = Log;
    Full = true;
  }
}
//}}}
//{{{
void cActivityBuffer::activityStartId (int activity, int id, const cGuid& guid)
{
  UInt64 time = cActivityLog::getTime();

  if (pCurrent != pWrite)
  {
    pCurrent->EndTime = time;
    pCurrent = pWrite;
  }

  pWrite->StartTime = time;
  pWrite->EndTime = 0;
  pWrite->Activity = (activity & kActivityMask) | (iActivity::kIdGuid << 28);
  pWrite->Details[0] = id;

  UInt64* pDetail64 = (UInt64*) (&(pWrite->Details[1]));
  pDetail64[0] = guid.getData64_0();
  pDetail64[1] = guid.getData64_1();

  if (++pWrite == LogEnd)
  {
    pWrite = Log;
    Full = true;
  }
}
//}}}
//{{{
void cActivityBuffer::activityStartId (int activity, int id, const char* description)
{
  UInt64 time = cActivityLog::getTime();

  if (pCurrent != pWrite)
  {
    pCurrent->EndTime = time;
    pCurrent = pWrite;
  }

  pWrite->StartTime = time;
  pWrite->EndTime = 0;
  pWrite->Activity = (activity & kActivityMask) | (iActivity::kIdString << 28);
  pWrite->Details[0] = id;

  {
    char * const       DestString   = ((char*) pWrite->Details) + sizeof(int);    // start after ID
    const unsigned int MaxDestChars = sizeof(pWrite->Details) - sizeof(int);
    (void)strncpy (DestString, description, MaxDestChars);
  }

  if (++pWrite == LogEnd)
  {
    pWrite = Log;
    Full = true;
  }
}
//}}}
//{{{
void cActivityBuffer::activityEventId (int activity, int id, const char* description)
{
  UInt64 time = cActivityLog::getTime();

  if (pCurrent != pWrite)
  {
    pCurrent->EndTime = time;
    pCurrent = pWrite;
  }

  pWrite->StartTime = time;
  pWrite->EndTime = time;
  pWrite->Activity = (activity & kActivityMask) | (iActivity::kIdString << 28) | kIsEvent;
  pWrite->Details[0] = id;

  {
    char * const       DestString   = ((char*) pWrite->Details) + sizeof(int);    // start after ID
    const unsigned int MaxDestChars = sizeof(pWrite->Details) - sizeof(int);
    (void)strncpy (DestString, description, MaxDestChars);
  }

  if (++pWrite == LogEnd)
  {
    pWrite = Log;
    Full = true;
  }
}
//}}}
//{{{
void cActivityBuffer::activityEventId (int activity, int id, const cGuid& guid)
{
  UInt64 time = cActivityLog::getTime();

  if (pCurrent != pWrite)
  {
    pCurrent->EndTime = time;
    pCurrent = pWrite;
  }

  pWrite->StartTime = time;
  pWrite->EndTime = time;
  pWrite->Activity = (activity & kActivityMask) | (iActivity::kIdGuid << 28) | kIsEvent;
  pWrite->Details[0] = id;

  UInt64* pDetail64 = (UInt64*) (&(pWrite->Details[1]));
  pDetail64[0] = guid.getData64_0();
  pDetail64[1] = guid.getData64_1();

  if (++pWrite == LogEnd)
  {
    pWrite = Log;
    Full = true;
  }
}
//}}}


  // Use these if you want to apply the id when the activity is done
  //{{{
  void cActivityBuffer::activityDoneId (int id)
  {
    UInt64 time = cActivityLog::getTime();

    if (pCurrent != pWrite)
    {
      int detailType = (pCurrent->Activity >> 28) & 0x0f;

      switch (detailType)
      {
        case iActivity::kSize:
        case iActivity::kArea:
        case iActivity::kPosArea:
        case iActivity::kAreaArea:
        case iActivity::kSizeSize:
          pCurrent->Details[0] = (pCurrent->Details[0] & ~0xffff) | (id & 0xffff);
          break;
        case iActivity::kString:
        {
          pCurrent->Activity = (pCurrent->Activity & kActivityMask) | (iActivity::kIdString << 28);
          // make room for ID by shifting the string
          char* const src = (char*) pCurrent->Details;
          char* const dst = src + sizeof(int);
          const unsigned int maxChars = sizeof(pCurrent->Details) - sizeof(int);
          (void)strncpy (dst, src, maxChars);
          pCurrent->Details[0] = id;
        }
          break;
        case iActivity::kIdString:
          pCurrent->Details[0] = id;
          break;
        default:
          pCurrent->Activity = (pCurrent->Activity & kActivityMask) | (iActivity::kIdString << 28);
          pCurrent->Details[0] = id;
          pCurrent->Details[1] = 0;  // null string
      }
      pCurrent->EndTime = time;
      pCurrent = pWrite;
    }
  }
  //}}}
  //{{{
  void cActivityBuffer::activityDoneId (int id, const cGuid& guid)
  {
    UInt64 time = cActivityLog::getTime();

    if (pCurrent != pWrite)
    {
      pWrite->Activity = (pWrite->Activity & ~kDetailTypeMask) | (iActivity::kIdGuid << 28);
      pWrite->Details[0] = id;

      UInt64* pDetail64 = (UInt64*) (&(pWrite->Details[1]));
      pDetail64[0] = guid.getData64_0();
      pDetail64[1] = guid.getData64_1();

      pCurrent->EndTime = time;
      pCurrent = pWrite;
    }
  }
  //}}}
  //{{{
  void cActivityBuffer::activityDoneId (int id, const char* description)
  {
    UInt64 time = cActivityLog::getTime();

    if (pCurrent != pWrite)
    {
      pWrite->Activity = (pCurrent->Activity & kActivityMask) | (iActivity::kIdString << 28);
      pWrite->Details[0] = id;

      {
        char * const       DestString   = ((char*) pWrite->Details) + sizeof(int);    // start after ID
        const unsigned int MaxDestChars = sizeof(pWrite->Details) - sizeof(int);
        (void)strncpy (DestString, description, MaxDestChars);
      }

      pCurrent->EndTime = time;
      pCurrent = pWrite;
    }
  }
  //}}}
  //{{{
  void cActivityBuffer::activityFailedId (int id)
  {
    UInt64 time = cActivityLog::getTime();

    if (pCurrent != pWrite)
    {
      int detailType = (pCurrent->Activity >> 28) & 0x0f;

      switch (detailType)
      {
        case iActivity::kSize:
        case iActivity::kArea:
        case iActivity::kPosArea:
        case iActivity::kAreaArea:
        case iActivity::kSizeSize:
          pCurrent->Details[0] = (pCurrent->Details[0] & ~0xffff) | (id & 0xffff);
          break;
        case iActivity::kString:
        {
          pCurrent->Activity = (pCurrent->Activity & kActivityMask) | (iActivity::kIdString << 28);
          // make room for ID by shifting the string
          char* const src = (char*) pCurrent->Details;
          char* const dst = src + sizeof(int);
          const unsigned int maxChars = sizeof(pCurrent->Details) - sizeof(int);
          (void)strncpy (dst, src, maxChars);
          pCurrent->Details[0] = id;
        }
          break;
        case iActivity::kIdString:
          pCurrent->Details[0] = id;
          break;
        default:
          pCurrent->Activity = (pCurrent->Activity & kActivityMask) | (iActivity::kIdString << 28);
          pCurrent->Details[0] = id;
          pCurrent->Details[1] = 0;  // null string
      }
      pCurrent->EndTime = time;
      pCurrent->Activity |= kFailed;
      pCurrent = pWrite;
    }
  }
  //}}}
  //{{{
  void cActivityBuffer::activityFailedId (int id, const cGuid& guid)
  {
    UInt64 time = cActivityLog::getTime();

    if (pCurrent != pWrite)
    {
      pWrite->Activity = (pWrite->Activity & ~kDetailTypeMask) | (iActivity::kIdGuid << 28) | kFailed;
      pWrite->Details[0] = id;

      UInt64* pDetail64 = (UInt64*) (&(pWrite->Details[1]));
      pDetail64[0] = guid.getData64_0();
      pDetail64[1] = guid.getData64_1();


      pCurrent->EndTime = time;
      pCurrent = pWrite;
    }
  }
  //}}}
  //{{{
  void cActivityBuffer::activityFailedId (int id, const char* description)
  {
    UInt64 time = cActivityLog::getTime();

    if (pCurrent != pWrite)
    {
      pWrite->Activity = (pCurrent->Activity & kActivityMask) | (iActivity::kIdString << 28);
      pWrite->Details[0] = id;

      {
        char * const       DestString   = ((char*) pWrite->Details) + sizeof(int);    // start after ID
        const unsigned int MaxDestChars = sizeof(pWrite->Details) - sizeof(int);
        (void)strncpy (DestString, description, MaxDestChars);
      }

      pCurrent->EndTime = time;
      pCurrent->Activity |= kFailed;
      pCurrent = pWrite;
    }
  }
  //}}}

//{{{
void cActivityBuffer::activityDone()
{
  UInt64 time = cActivityLog::getTime();

  if (pCurrent != pWrite)
  {
    pCurrent->EndTime = time;
    pCurrent = pWrite;
  }
}
//}}}
//{{{
UInt64 cActivityBuffer::activityDone2()
{
  UInt64 time = cActivityLog::getTime();

  if (pCurrent != pWrite)
  {
    UInt64 startTime = pCurrent->StartTime;
    pCurrent->EndTime = time;
    pCurrent = pWrite;
    if (time > startTime)
      return (time - startTime);
  }
  return 0;
}
//}}}
//{{{
void cActivityBuffer::activityDone(int activity, const char* description)
{
  UInt64 time = cActivityLog::getTime();

  if (pCurrent != pWrite)
  {
    pCurrent->Activity = (activity & kActivityMask) | (iActivity::kString << 28);
    {
      char * const       DestString   = (char*) pCurrent->Details;
      const unsigned int MaxDestChars = _INTSIZEOF(pCurrent->Details);
      (void)strncpy (DestString, description, MaxDestChars);
    }

    pCurrent->EndTime = time;
    pCurrent = pWrite;
  }
}
//}}}
//{{{
void cActivityBuffer::activityFailed()
{
  UInt64 time = cActivityLog::getTime();

  if (pCurrent != pWrite)
  {
    pCurrent->EndTime = time;
    pCurrent->Activity |= kFailed;
    pCurrent = pWrite;
  }
}
//}}}
//{{{
void cActivityBuffer::activityFailed(int activity, const char* description)
{
  UInt64 time = cActivityLog::getTime();

  if (pCurrent != pWrite)
  {
    pCurrent->Activity = (activity & kActivityMask) | (iActivity::kString << 28);
    {
      char * const       DestString   = (char*) pCurrent->Details;
      const unsigned int MaxDestChars = _INTSIZEOF(pCurrent->Details);
      (void)strncpy (DestString, description, MaxDestChars);
    }

    pCurrent->EndTime = time;
    pCurrent->Activity |= kFailed;
    pCurrent = pWrite;
  }
}
//}}}

//{{{
int  cActivityBuffer::entryCount() const
{
  if (Full)
    return PtrToUlong((const void *)(LogEnd - Log));
  return PtrToUlong((const void *)(pWrite - Log));
}
//}}}
//{{{
int  cActivityBuffer::completedEntryCount() const
{
  if (!Full)
    return PtrToUlong((const void *)(pCurrent - Log));
  if (pCurrent != pWrite)
    return PtrToUlong((const void *)(LogEnd - Log)) - 1;
  return PtrToUlong((const void *)(LogEnd - Log));
}
//}}}

//{{{
void cActivityBuffer::dump (bool dumpData) const
{
  QTRACE((L"cActivityBuffer::dump %d Full:%d Log:%p pWrite:%p loCount:%Iu Count:%d",
          num, Full?1:0, Log, pWrite, pWrite-Log, entryCount()));

  if (dumpData)
  {
    int count = 0;
    const cActivityLogEntry* entry;
    bool ok = first (entry);
    while (ok)
    {
      QTRACE((L"%4d  %0.5f to %0.5f %d", count++,
            cActivityLog::toSeconds(entry->StartTime),
            cActivityLog::toSeconds(entry->EndTime),
            entry->Activity));
      ok = next (entry);
    }
  }
}
//}}}

//{{{
void cActivityBuffer::reset()
{
  Full = false;
  pCurrent = Log;
  pWrite = Log;
}
//}}}
//{{{
void cActivityBuffer::merge (cActivityBuffer::Ptr prevBuf)
{ // Note: assumes that both this & prevBuf are already shuffled

  //QTRACE((L"cActivityBuffer::merge %d from %d", num, prevBuf.isValid() ? prevBuf->num : 0));
  if (Full || !prevBuf.isValid()) return;

  int prevCount = prevBuf->completedEntryCount();
  if (prevCount <= 0) return;

  //prevBuf->dump(false);
  //dump(false);
  int myCount = entryCount();
  int space = PtrToUlong((const void *)(LogEnd - Log)) - myCount;

  //QTRACE((L"  prevCount %d myCount %d space %d", prevCount, myCount, space));

  if (prevCount < space)
    //{{{  copy all of prev-data
    {
    if (myCount > 0)
      //{{{  shuffle up data to make room
      {
      char* dst = (char*) &Log[prevCount];
      memmove (dst, Log, myCount * sizeof (cActivityLogEntry));
      }
      //}}}
    // copy-in prev-data
    memcpy (Log, prevBuf->Log, prevCount * sizeof (cActivityLogEntry));
    pCurrent += prevCount;
    pWrite += prevCount;
    }
    //}}}
  else
  {
    if (!prevBuf->Full || (prevBuf->pWrite == prevBuf->Log))
      //{{{  copy latest of prev-data
      {
      char* src = (char*) &prevBuf->Log[prevCount - space];
      memcpy (pWrite, src, space * sizeof (cActivityLogEntry));
      }
      //}}}
    else
    {
      int prevLoCount = PtrToUlong((const void *)(prevBuf->pCurrent - prevBuf->Log));
      if (prevLoCount >= space)
        //{{{  copy latest from lo-section of prev-data
        {
        char* src = (char*) &prevBuf->Log[prevLoCount - space];
        memcpy (pWrite, src, space * sizeof (cActivityLogEntry));
        }
        //}}}
      else
      {
        //{{{  copy all of lo-section of prev-data to my-highest
        char* dst = (char*) &Log[kLogSize - prevLoCount];
        memmove (dst, prevBuf->Log, prevLoCount * sizeof (cActivityLogEntry));
        space -= prevLoCount;
        //}}}
        //{{{  copy latest from hi-section of prev-data
        char* src = (char*) &prevBuf->Log[kLogSize - space];
        memcpy (pWrite, src, space * sizeof (cActivityLogEntry));
        //}}}
      }
    }
    Full = true;
  }

  //QTRACE((L"cActivityBuffer::merge - done"));
  //dump(false);
}
//}}}

//{{{
UInt64 cActivityBuffer::firstLogTime() const
{
  if (Full)
    return pWrite->StartTime;

  if (pWrite != Log)
    return Log[0].StartTime;

  return _UI64_MAX; //(UInt64)((__int64)(-1)); // flag no time logged
}
//}}}
//{{{
UInt64 cActivityBuffer::lastLogTime() const
{
  const cActivityLogEntry* entry = 0;
  if (pWrite != Log)
    entry = pWrite - 1;
  else if (Full)
    entry = LogEnd - 1;

  if (!entry)
    return 0;

  if (entry->EndTime < entry->StartTime)  // still active
    return _UI64_MAX; //(UInt64)((__int64)(-1)); // flag no time logged

  return entry->EndTime;
}
//}}}

//{{{
bool cActivityBuffer::first (const cActivityLogEntry*& pRead) const
{
  if (Full)
    pRead = pWrite;
  else
    pRead = Log;

  return (Full || (pWrite != Log));
}
//}}}
//{{{
bool cActivityBuffer::next  (const cActivityLogEntry*& pRead) const
{
  if (pWrite == Log)
  {
    if (!Full)   // buffer empty
      return false;
    if (pRead == (LogEnd - 1))  // Full & pRead at last entry
      return false;
  }
  else if (pRead == (pWrite - 1))       // pRead at last entry
    return false;

  if (++pRead == LogEnd)
    pRead = Log;

  return true;
}
//}}}

//{{{
cActivityBuffer::cActivityBuffer (iFile::Ptr file) throw(cNotFound, cTimedOut, cAccessDenied)    // load from a log-file
  : Full(false), num(++Count), Log(NULL)
{
  //QSOS((L"create cActivityBuffer %d", Count));

  // read header = block len (bytes)
  cMemory::Ptr memDataLen = file->read ((UInt64)sizeof(int));
  UInt64 dataLen = *((int*) memDataLen->getBytes());
  size_t logSize = (size_t)(dataLen / sizeof (cActivityLogEntry));

  //QTRACE((L"cActivityBuffer %d Construct from File, dataLen=%d, logSize=%d", num, dataLen, logSize));

  if (dataLen > 0)  // allow for no-data
  {
    Log = new cActivityLogEntry [logSize];
    cMemory::Ptr memData = file->read (dataLen);
    memcpy (Log, memData->getBytes(), logSize * sizeof (cActivityLogEntry));
    Full = true;
  }
  else
    Log = new cActivityLogEntry [1];  // create a minimum 'empty' Log

  LogEnd = Log + logSize;
  pCurrent = Log;
  pWrite = Log;

}
//}}}
//{{{
void cActivityBuffer::writeFile (iFile::Ptr file) const throw(cNotFound, cTimedOut, cAccessDenied)
{
  QVERIFY(entryCount() * sizeof (cActivityLogEntry) <= (size_t)((std::numeric_limits<int>::max)()));
  int dataLen = entryCount() * sizeof (cActivityLogEntry);

  // header = block len (bytes)
  file->write ((UInt64)sizeof(int), (const char* const) &dataLen);

  // data
  if (Full && (pWrite != Log))
  {
    ptrdiff_t len = (LogEnd - pWrite);
    UInt64 lenBytes = len * sizeof (cActivityLogEntry);
    QVERIFY(lenBytes <= (UInt64)((std::numeric_limits<int>::max)()));

    if (lenBytes > 0UI64)
      file->write (lenBytes, (const char* const) pWrite);
    dataLen -= (int)lenBytes;
  }

  if (dataLen > 0)
    file->write ((UInt64)dataLen, (const char* const) Log);
}
//}}}

//{{{
bool cActivityBuffer::checkTrigger (const cActivityTrigger& trigger) const
{
  if ((pCurrent == pWrite) || (trigger.mActivity != pCurrent->Activity))
    return false;
  if ((trigger.mEnable & cActivityTrigger::eTriggerFlagsMask) == cActivityTrigger::eTriggerActivity)  // no other enables
    return true;
  int first = 3;
  int last = iActivity::kMaxPayload;
  switch (trigger.mActivity >> 28)
  {
    default:
    case iActivity::kNone:     return false;
    case iActivity::kValue:    return (trigger.mPayload[0] == pCurrent->Details[0]);
    case iActivity::kString:
    case iActivity::kIdString:
    case iActivity::kIdGuid:
      first = 0;
      break;
    case iActivity::kSize:
      last  = (trigger.mEnable & cActivityTrigger::eTriggerDstArea) ? 5 : 0; // 3-4
      break;
    case iActivity::kArea:
      last  = (trigger.mEnable & cActivityTrigger::eTriggerDstArea) ? 7 : 0; // 3-6
      break;
    case iActivity::kPosArea:
      first = (trigger.mEnable & cActivityTrigger::eTriggerSrcArea) ? 3 : 4; // 3
      last  = (trigger.mEnable & cActivityTrigger::eTriggerDstArea) ? 6 : 4; // 4-5
      break;
    case iActivity::kAreaArea:
    case iActivity::kSizeSize:
      first = (trigger.mEnable & cActivityTrigger::eTriggerSrcArea) ? 3 : 5; // 3-4
      last  = (trigger.mEnable & cActivityTrigger::eTriggerDstArea) ? 7 : 5; // 5-6
      break;
  }
  if (first > 0)
  {
    if ((trigger.mEnable & cActivityTrigger::eTriggerDstType) && ((trigger.mPayload[0] & 0xffff0000) != (pCurrent->Details[0] & 0xffff0000)))
      return false;
    if ((trigger.mEnable & cActivityTrigger::eTriggerDstId)   && ((trigger.mPayload[0] & 0x0000ffff) != (pCurrent->Details[0] & 0x0000ffff)))
      return false;
    if ((trigger.mEnable & cActivityTrigger::eTriggerSrcType) && ((trigger.mPayload[1] & 0xffff0000) != (pCurrent->Details[1] & 0xffff0000)))
      return false;
    if ((trigger.mEnable & cActivityTrigger::eTriggerSrcId)   && ((trigger.mPayload[1] & 0x0000ffff) != (pCurrent->Details[1] & 0x0000ffff)))
      return false;
    if ((trigger.mEnable & cActivityTrigger::eTriggerFlags)   && (trigger.mPayload[2] != pCurrent->Details[2]))
      return false;
  }
  for (int n = first; n < last; ++n)
  {
    if (trigger.mPayload[n] != pCurrent->Details[n])
      return false;
  }
  return true;
}
//}}}

//}}}
//{{{  cActivity members
// =======================================================================================
// === cActivity =========================================================================
// =======================================================================================

//{{{
cActivity::cActivity (String title, eTraceType type, int bufsize)
  : Ident(++CurrentIdent), Next(NULL), Enabled(false), SignalEnabled(false),
    Title(title), Type(type), Bufsize(bufsize)
{
  if (Bufsize < 1) Bufsize = 1;
  if (Bufsize > 5) Bufsize = 5;
  //QTRACE((L"--Creating Activity id:%d %s", Ident, Title.c_str()));
}
//}}}
//{{{
cActivity::~cActivity()
{
  cActivityLog::singleton().forgetSignal (Ident);

  // SmartPointers ensure nobody will be logging if we are being deleted
  LogBuffer.setNull();
  PrevLogBuffer.setNull();
  //QTRACE((L"--Destroying Activity id:%d %s", Ident, Title.c_str()));
}
//}}}

//{{{
cActivity* cActivity::enable()
{
  cLockGuard MemberGuard (&GrabLock);

  if (!Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer = new cActivityBuffer (Bufsize);
    Enabled = LogBuffer.isValid();
  }

  return Next;
}
//}}}
//{{{
cActivity* cActivity::disable()
{
  cLockGuard MemberGuard (&GrabLock);

  Enabled = false;           // stop any other thread logging
  PrevLogBuffer.setNull();   // free up the heap space, as we are not logging

  if (LogBuffer.isValid())
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer.setNull();    // free up the heap space, as we are not logging
  }
  return Next;
}
//}}}
//{{{
cActivity* cActivity::startLogging()
{
  cLockGuard MemberGuard (&GrabLock);

  if (!LogBuffer.isValid())
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer = new cActivityBuffer (Bufsize);
  }
  Enabled = LogBuffer.isValid();

  return Next;
}
//}}}
//{{{
cActivity* cActivity::stopLogging()
{
  cLockGuard MemberGuard (&GrabLock);
  Enabled = false;

  return Next;
}

//}}}

// v3 extensions
//{{{
void QAPI cActivity::defineActivitySpec (int activitySpec, const std::string& displaySpec)
{
  cLockGuard g (&mLock);
  mActivitySpecMap[activitySpec] = displaySpec;
}
//}}}
//{{{
void QAPI cActivity::defineActivitySpec (const std::string& name, const std::string& displaySpec)
{
  cLockGuard g (&mLock);
  mActivityNamedSpecMap[name] = displaySpec;
}
//}}}
//{{{
void QAPI cActivity::defineActivityCol  (int activityCol, const std::string& displayName)
{
  addActivity (activityCol & kActivityColMask, widen (displayName));
}
//}}}
//{{{
const std::string QAPI cActivity::getActivitySpec (int activitySpec) const
{
  cLockGuard g (&mLock);
  tActivityStringMap::const_iterator it = mActivitySpecMap.find (activitySpec);
  if (it != mActivitySpecMap.end())
    return it->second;
  return std::string();
}
//}}}
//{{{
tActivityStringMap QAPI cActivity::getActivitySpecMap (int signalIdent) const
{
  if ((unsigned int)signalIdent == Ident)
  {
    cLockGuard g (&mLock);
    return mActivitySpecMap;
  }
  return tActivityStringMap();
}
//}}}

//{{{
void QAPI cActivity::activityStart (int activity, const int* payload, int payloadLen)     // activity = ((activitySpec << kActivitySpecShift) & kActivitySpecMask) + (activityCol & kActivityColMask)
{
  if (Enabled)
  {
    cLockGuard g (&LoggingLock);
    LogBuffer->activityStart ((activity & kActivityMask) | kUserDefinedMask, payload, payloadLen);
  }
}
//}}}
//{{{
void QAPI cActivity::activityEvent (int activity, const int* payload, int payloadLen)
{
  if (Enabled)
  {
    cLockGuard g (&LoggingLock);
    LogBuffer->activityStart ((activity & kActivityMask) | kUserDefinedMask | kEvent, payload, payloadLen);
  }
}
//}}}
//{{{
void QAPI cActivity::activityDone (int activity, const int* payload, int payloadLen)
{
  if (Enabled)
  {
    cLockGuard g (&LoggingLock);
    LogBuffer->activityStart ((activity & kActivityMask) | kUserDefinedMask | kDone, payload, payloadLen);
  }
}
//}}}

// iActivity virtual members
//{{{
void QAPI cActivity::addActivity (int activity, const String name)
{
  QVERIFY (activity >= 0);
  //QVERIFY (activity < kMaxActivity);

  cLockGuard g (&mLock);
  if ((int)ActivityNameVec.size() < (activity + 1))
    ActivityNameVec.resize (activity + 1);

  ActivityNameVec.at(activity) = name;
}
//}}}

//{{{
void QAPI cActivity::activityStart ()
{
  if (Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityStart ();
  }
}
//}}}
//{{{
void QAPI cActivity::activityStart (int activity)
{
  if (Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityStart (activity);
  }
}
//}}}
//{{{
void QAPI cActivity::activityStart (int activity, int value)
{
  if (Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityStart (activity, value);
  }
}
//}}}
//{{{
void QAPI cActivity::activityStart (int activity, const cXY& size)
{
  if (Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityStart (activity, cXYZ(-1,-1,0), size);
  }
}
//}}}
//{{{
void QAPI cActivity::activityStart (int activity, const cRect& area)
{
  if (Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityStart (activity, cXYZ(-1,-1,0), area);
  }
}
//}}}
//{{{
void QAPI cActivity::activityStart (int activity, const cXY& pos, const cRect& area)
{
  if (Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityStart (activity, cXYZ(-1,-1,0), pos, area);
  }
}
//}}}
//{{{
void QAPI cActivity::activityStart (int activity, const cRect& area2, const cRect& area)
{
  if (Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityStart (activity, cXYZ(-1,-1,0), area2, area);
  }
}
//}}}
//{{{
void QAPI cActivity::activityStart (int activity, int value, const cXY& size)
{
  if (Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityStart (activity, cXYZ(value,-1,0), size);
  }
}
//}}}
//{{{
void QAPI cActivity::activityStart (int activity, int value, const cRect& area)
{
  if (Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityStart (activity, cXYZ(value,-1,0), area);
  }
}
//}}}
//{{{
void QAPI cActivity::activityStart (int activity, int value, const cXY& pos, const cRect& area)
{
  if (Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityStart (activity, cXYZ(value,-1,0), pos, area);
  }
}
//}}}
//{{{
void QAPI cActivity::activityStart (int activity, int value, const cRect& area2, const cRect& area)
{
  if (Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityStart (activity, cXYZ(value,-1,0), area2, area);
  }
}
//}}}
//{{{
void QAPI cActivity::activityStart (int activity, const char* description)
{
  if (Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityStart (activity, description);
  }
}
//}}}
//{{{
void QARGS_STACK cActivity::activityStart3 (int activity, const char* format, va_list ap)
{
  if (Enabled)
  {
    char buffer[256] = "\0";
    _vsnprintf(buffer, sizeof(buffer) / sizeof(char), format, ap);

    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityStart (activity, buffer);
  }
}
//}}}
//{{{
void QARGS_STACK cActivity::activityStart2 (int activity, const char* format, ...)
{
  if (Enabled)
  {
    if (strchr(format, '%') != 0)
    {
      va_list ap;
      va_start(ap, format);
      activityStart3(activity, format, ap);
      va_end(ap);
    }
    else
    {
      cLockGuard MemberGuard (&LoggingLock);
      LogBuffer->activityStart (activity, format);
    }
  }
}
//}}}
//{{{
void QARGS_STACK cActivity::activityEvent (int activity, const char* format, ...)
{
  if (Enabled)
  {
    if (strchr(format, '%') != 0)
    {
      va_list ap;
      va_start(ap, format);
      char buffer[256] = "\0";
      _vsnprintf(buffer, sizeof(buffer) / sizeof(char), format, ap);
      va_end(ap);

      cLockGuard MemberGuard (&LoggingLock);
      LogBuffer->activityEvent (activity, buffer);
    }
    else
    {
      cLockGuard MemberGuard (&LoggingLock);
      LogBuffer->activityEvent (activity, format);
    }
  }
}
//}}}
//{{{
void QAPI cActivity::activityStart (int activity, const cXYZ& info, const cXY& size)
{
  if (Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityStart (activity, info, size);
  }
}
//}}}
//{{{
void QAPI cActivity::activityStart (int activity, const cXYZ& info, const cXY& size2, const cXY& size)
{
  if (Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityStart (activity, info, size2, size);
  }
}
//}}}
//{{{
void QAPI cActivity::activityStart (int activity, const cXYZ& info, const cRect& area)
{
  if (Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityStart (activity, info, area);
  }
}
//}}}
//{{{
void QAPI cActivity::activityStart (int activity, const cXYZ& info, const cXY& pos, const cRect& area)
{
  if (Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityStart (activity, info, pos, area);
  }
}
//}}}
//{{{
void QAPI cActivity::activityStart (int activity, const cXYZ& info, const cRect& area2, const cRect& area)
{
  if (Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityStart (activity, info, area2, area);
  }
}
//}}}

//{{{
void QARGS_STACK cActivity::activityDone (int activity, const char* format, ...)
{
  if (Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);

    if (strchr(format, '%') != 0)
    {
      va_list ap;
      va_start(ap, format);
      char buffer[256] = "\0";
      _vsnprintf(buffer, sizeof(buffer) / sizeof(char), format, ap);
      va_end(ap);

      cLockGuard MemberGuard (&LoggingLock);
      LogBuffer->activityDone(activity, buffer);
    }
    else
    {
      cLockGuard MemberGuard (&LoggingLock);
      LogBuffer->activityDone(activity, format);
    }
  }
}
//}}}
//{{{
void QAPI cActivity::activityDone()
{
  if (Enabled)
  {
    UInt64 ticks;
    bool triggered = false;
    {
      cLockGuard MemberGuard (&LoggingLock);
      if (mTrigger.mCount && (mTrigger.mSignalIdent == Ident)) // mTriggerCount : a read is atomic & hence threadsafe
        triggered = LogBuffer->checkTrigger (mTrigger);
      ticks = LogBuffer->activityDone2();
    }
    if (triggered)
    {
      if (cActivityLog::toSeconds (ticks) >= mTrigger.mDuration)
        actionTrigger();   // mCount will be retested in actionTrigger
    }
  }
}
//}}}
//{{{
double QAPI cActivity::activityDone2()
{
  if (Enabled)
  {
    UInt64 ticks;
    bool triggered = false;
    {
      cLockGuard MemberGuard (&LoggingLock);
      if (mTrigger.mCount && (mTrigger.mSignalIdent == Ident)) // mTriggerCount : a read is atomic & hence threadsafe
        triggered = LogBuffer->checkTrigger (mTrigger);
      ticks = LogBuffer->activityDone2();
    }
    double duration = cActivityLog::toSeconds (ticks);
    if (triggered)
    {
      if (duration >= mTrigger.mDuration)
        actionTrigger();   // mCount will be retested in actionTrigger
    }
    return duration;
  }
  return 0.0;
}
//}}}
//{{{
void QAPI cActivity::activityFailed()
{
  if (Enabled)
  {
    bool triggered = false;
    {
      cLockGuard MemberGuard (&LoggingLock);
      if (mTrigger.mCount && (mTrigger.mSignalIdent == Ident)) // mTriggerCount : a read is atomic & hence threadsafe
        triggered = LogBuffer->checkTrigger (mTrigger);
      LogBuffer->activityFailed();
    }
    if (triggered)
      actionTrigger();
  }
}
//}}}
//{{{
void QARGS_STACK cActivity::activityFailed(int activity, const char* format, ...)
{
  if (Enabled)
  {
    if (strchr(format, '%') != 0)
    {
      va_list ap;
      va_start(ap, format);
      char buffer[256] = "\0";
      _vsnprintf(buffer, sizeof(buffer) / sizeof(char), format, ap);
      va_end(ap);

      cLockGuard MemberGuard (&LoggingLock);
      LogBuffer->activityFailed(activity, buffer);
    }
    else
    {
      cLockGuard MemberGuard (&LoggingLock);
      LogBuffer->activityFailed(activity, format);
    }
  }
}
//}}}

//{{{
void QAPI cActivity::activityStartId (int activity, int id)
{
  if (Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityStartId (activity, id);
  }
}
//}}}
//{{{
void QAPI cActivity::activityStartId (int activity, int id, const char* description)
{
  if (Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityStartId (activity, id, description);
  }
}
//}}}
//{{{
void QARGS_STACK cActivity::activityStartId3 (int activity, int id, const char* format, va_list ap)
{
  if (Enabled)
  {
    char buffer[256] = "\0";
    _vsnprintf(buffer, sizeof(buffer) / sizeof(char), format, ap);

    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityStartId (activity, id, buffer);
  }
}
//}}}
//{{{
void QARGS_STACK cActivity::activityStartId2 (int activity, int id, const char* format, ...)
{
  if (Enabled)
  {
    if (strchr(format, '%') != 0)
    {
      va_list ap;
      va_start(ap, format);
      activityStartId3(activity, id, format, ap);
      va_end(ap);
    }
    else
    {
      cLockGuard MemberGuard (&LoggingLock);
      LogBuffer->activityStartId (activity, id, format);
    }
  }
}
//}}}
//{{{
void QARGS_STACK cActivity::activityEventId (int activity, int id, const char* format, ...)
{
  if (Enabled)
  {
    if (strchr(format, '%') != 0)
    {
      va_list ap;
      va_start(ap, format);
      char buffer[256] = "\0";
      _vsnprintf(buffer, sizeof(buffer) / sizeof(char), format, ap);
      va_end(ap);

      cLockGuard MemberGuard (&LoggingLock);
      LogBuffer->activityEventId (activity, id, buffer);
    }
    else
    {
      cLockGuard MemberGuard (&LoggingLock);
      LogBuffer->activityEventId (activity, id, format);
    }
  }
}
//}}}

//{{{
void QAPI cActivity::activityStartId (int activity, int id, const cGuid& guid)
{
  if (Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityStartId (activity, id, guid);
  }
}
//}}}
//{{{
void QAPI cActivity::activityEventId (int activity, int id, const cGuid& guid)
{
  if (Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityEventId (activity, id, guid);
  }
}
//}}}

//{{{
void QAPI cActivity::activityDoneId (int id)
{
  if (Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityDoneId (id);
  }
}
//}}}
//{{{
void QAPI cActivity::activityDoneId (int id, const cGuid& guid)
{
  if (Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityDoneId (id, guid);
  }
}
//}}}
//{{{
void QAPI cActivity::activityDoneId (int id, const char* description)
{
  if (Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityDoneId (id, description);
  }
}
//}}}
//{{{
void QARGS_STACK cActivity::activityDoneId2 (int id, const char* format, ...)
{
  if (Enabled)
  {
    if (strchr(format, '%') != 0)
    {
      va_list ap;
      va_start(ap, format);
      activityDoneId3 (id, format, ap);
      va_end(ap);
    }
    else
    {
      cLockGuard MemberGuard (&LoggingLock);
      LogBuffer->activityDoneId (id, format);
    }
  }
}
//}}}
//{{{
void QARGS_STACK cActivity::activityDoneId3 (int id, const char* format, va_list ap)
{
  if (Enabled)
  {
    char buffer[256] = "\0";
    _vsnprintf(buffer, sizeof(buffer) / sizeof(char), format, ap);

    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityDoneId (id, buffer);
  }
}
//}}}

//{{{
void QAPI cActivity::activityFailedId (int id)
{
  if (Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityFailedId (id);
  }
}
//}}}
//{{{
void QAPI cActivity::activityFailedId (int id, const cGuid& guid)
{
  if (Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityFailedId (id, guid);
  }
}
//}}}
//{{{
void QAPI cActivity::activityFailedId (int id, const char* description)
{
  if (Enabled)
  {
    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityFailedId (id, description);
  }
}
//}}}
//{{{
void QARGS_STACK cActivity::activityFailedId2 (int id, const char* format, ...)
{
  if (Enabled)
  {
    if (strchr(format, '%') != 0)
    {
      va_list ap;
      va_start(ap, format);
      activityDoneId3 (id, format, ap);
      va_end(ap);
    }
    else
    {
      cLockGuard MemberGuard (&LoggingLock);
      LogBuffer->activityFailedId (id, format);
    }
  }
}
//}}}
//{{{
void QARGS_STACK cActivity::activityFailedId3 (int id, const char* format, va_list ap)
{
  if (Enabled)
  {
    char buffer[256] = "\0";
    _vsnprintf(buffer, sizeof(buffer) / sizeof(char), format, ap);

    cLockGuard MemberGuard (&LoggingLock);
    LogBuffer->activityFailedId (id, buffer);
  }
}
//}}}

// cActivity members
//{{{
cActivityBuffer::Ptr cActivity::grabBuffer()
{
  //QTRACE((L"cActivity::grabBuffer %s", Title.data()));
  cLockGuard MemberGuard (&GrabLock);

  if (Enabled)
  {
    cActivityBuffer::Ptr newLogBuffer = new cActivityBuffer (Bufsize);
    cActivityBuffer::Ptr prevBuf = PrevLogBuffer;

    PrevLogBuffer = LogBuffer;

    { // switch Logging Buffer
      cLockGuard MemberGuard (&LoggingLock);
      newLogBuffer->activityEnter (LogBuffer.getRaw());
      LogBuffer = newLogBuffer;
    }

    if (PrevLogBuffer.isValid())
      PrevLogBuffer->merge (prevBuf);

    return PrevLogBuffer;
  }

  return cActivityBuffer::Ptr();
}
//}}}
//{{{
cActivity* cActivity::resetLog()
{
  cLockGuard MemberGuard (&GrabLock);
  {
    cLockGuard MemberGuard (&LoggingLock);
    PrevLogBuffer.setNull();
    if (LogBuffer.isValid())
      LogBuffer->reset();
  }
  return Next;
}
//}}}

// static routines
//{{{
void cActivity::actionTrigger()
{
  if (mTrigger.mCount)         // still ok ?  -- simple reads are threadsafe
  {
    cLockGuard g (&mTriggerLock);
    if (mTrigger.mCount)         // still ok ?  -- it is now threadsafe to continue
    {
      if (--mTrigger.mCount == 0)    // triggered?
      {
        // if TriggerStack has changed mTrigger-eTriggerRepeat && mTrigger-eTriggerNext will have been removed
        if ((mTrigger.mEnable & cActivityTrigger::eTriggerNext) && (mTriggerIndex < (unsigned int)mTriggerStack.size()))
        {
          QTRACE((L"-- cActivity::Trigger %d --", mTriggerIndex));
          // not done yet - move onto next trigger
          mTrigger = mTriggerStack.at(mTriggerIndex++);  // change the reference
          return;
        }

        if (mTrigger.mEnable & cActivityTrigger::eTriggerRepeat)
        {
          // repeating - setup next trigger
          mTriggerIndex = 1;
          mTrigger = mTriggerStack.front(); // change the reference - start again
        }

        QTRACE((L"-- cActivity::Triggered --"));
        //switch (mTrigger.mAction)
        //{
          //case cActivityTrigger::eStackDump:
          //default:
          //{
                //iStackDump::dumpStack(0);
                // equivalent to p_walkback
                //cLogIndentSOSGuard Indent(L"");
                //iExceptionManager::singleton().logCallStack();
            //StackWalk64
            //StackDumpThread.postMessage (stackWalkData);
          //}
        //}
        iActivityLog::singleton().triggered();   // do callbacks
      }
    }
  }
}
//}}}
//{{{
void cActivity::addTrigger (const cActivityTrigger& trigger)
{
  mTrigger.mCount = 0; // stop current trigger  - a simple write is threadsafe

  cLockGuard g (&mTriggerLock);
  mTriggerStack.push_back (trigger);

  if ((trigger.mEnable & cActivityTrigger::eTriggerNext) == 0)
  {
    mTriggerIndex = 1;
    mTrigger = mTriggerStack.front();    // change the reference
  }
}
//}}}
//{{{
void cActivity::resetTrigger()
{
  mTrigger.mCount = 0; // stop current trigger  - a simple write is threadsafe

  cLockGuard g (&mTriggerLock);

  mTriggerIndex = 0;
  mTriggerStack.clear();

  mTrigger.mCount = 0;
  mTrigger.mEnable = 0;
  mTrigger.mSignalIdent = 0;
}
//}}}


//}}}
//{{{  cActivityGroup members
// =======================================================================================
// === cActivityGroup ======================================================================
// =======================================================================================

//{{{
cActivityGroup::cActivityGroup (String title)
  : mIdent(++cActivity::CurrentIdent),    // use cActivity::CurrentIdent to keep all indents unique
    pSignals(0),
    //Next(0),
    mActivated(false),
    mEnabled(false),
    mTitle(title)
{
  //QTRACE((L"--Creating ActivityGroup id:%d %s", mIdent, mTitle.c_str()));
}
//}}}
//{{{
cActivityGroup::~cActivityGroup()
{
  // don't delete signals - let the Source SmartPointers do that
  // just remove their Parent pointers
  //QTRACE((L"--Destroying ActivityGroup id:%d, %s", mIdent, mTitle.c_str()));
  Next.setNull();
}
//}}}

//{{{
void cActivityGroup::enable()
{
  cLockGuard MemberGuard (&SignalLock);
  mEnabled = true;

  cActivity* cur = pSignals;
  while (cur)
    cur = cur->enable();

}
//}}}
//{{{
void cActivityGroup::disable()
{
  cLockGuard MemberGuard (&SignalLock);
  mEnabled = false;

  cActivity* cur = pSignals;
  while (cur)
    cur = cur->disable();
}
//}}}

//{{{
void cActivityGroup::resetLog()
{
  cLockGuard MemberGuard (&SignalLock);

  cActivity* cur = pSignals;
  while (cur)
    cur = cur->resetLog();
}
//}}}
//{{{
void cActivityGroup::startLogging()
{
  cLockGuard MemberGuard (&SignalLock);

  if (mEnabled)
  {
    cActivity* cur = pSignals;
    while (cur)
      cur = cur->startLogging();
  }
}
//}}}
//{{{
void cActivityGroup::stopLogging()
{
  cLockGuard MemberGuard (&SignalLock);

  cActivity* cur = pSignals;
  while (cur)
    cur = cur->stopLogging();
}

//}}}

//{{{
int cActivityGroup::signalCount()
{
  cLockGuard MemberGuard (&SignalLock);

  int count = 0;
  cActivity* signal = pSignals;
  while (signal)
  {
    count += 1;
    signal = signal->Next;
  }
  return count;
}
//}}}
//{{{
iActivity::Ptr cActivityGroup::addSignal (const String name, iActivity::eTraceType type)
{
  return addSignal(name, type, 1);
}
//}}}
//{{{
iActivity::Ptr cActivityGroup::addSignal (const String name, iActivity::eTraceType type, const int bufSize)
{
  cLockGuard MemberGuard (&SignalLock);

  cActivity* newSignal = new cActivity (name, type, bufSize);

  cActivity* last = pSignals;
  while (last && last->Next)
    last = last->Next;

  if (last)
    last->Next = newSignal;
  else
    pSignals = newSignal;

  if (mEnabled)
    newSignal->enable();

  return newSignal;
}
//}}}
//{{{
iActivity::Ptr cActivityGroup::addSignal (const String name)
{
  return addSignal (name, iActivity::kSimple);
}
//}}}
//{{{
iActivity::Ptr cActivityGroup::addSignal2 (const int bufSize, const Char* format, ...)
{
  va_list ap;
  va_start(ap, format);
  Char buffer[256] = L"\0";
  _vsnwprintf(buffer, sizeof(buffer) / sizeof(Char), format, ap);
  va_end(ap);

  return addSignal (buffer, iActivity::kSimple, bufSize);
}
//}}}
//{{{
iActivity::Ptr cActivityGroup::addSignal2 (iActivity::eTraceType type, const int bufSize, const Char* format, ...)
{
  va_list ap;
  va_start(ap, format);
  Char buffer[256] = L"\0";
  _vsnwprintf(buffer, sizeof(buffer) / sizeof(Char), format, ap);
  va_end(ap);

  return addSignal (buffer, type, bufSize);
}
//}}}
//{{{
bool cActivityGroup::forgetSignal (int signalIdent)
{
  cLockGuard MemberGuard (&SignalLock);

  // search signal list for matching signal & get its previous signal
  cActivity* prev = 0;
  cActivity* cur = pSignals;
  while (cur)
  {
    if (cur->ident() == (unsigned int)signalIdent)
    {
      // unlink signal
      if (prev)
        prev->Next = cur->Next;
      else
        pSignals = cur->Next;
      return true;
    }
    prev = cur;
    cur = cur->Next;
  }
  return false;
}
//}}}

//{{{
bool cActivityGroup::addSignalsToView (cActivityLogView::Ptr view)
{
  if (mEnabled)
  {
    cLockGuard MemberGuard (&SignalLock);

    cActivity* signal = pSignals;
    while (signal)
    {
      view->addTrace (signal->Ident, mIdent, signal->grabBuffer());
      signal = signal->Next;
    }
    return true;
  }
  return false;
}
//}}}
//{{{
bool cActivityGroup::updateView (cActivityLogView::Ptr view)
{
  cLockGuard MemberGuard (&SignalLock);
  bool changed = false;

  cActivity* signal = pSignals;
  while (signal)
  {
    if (mEnabled)
      changed |= view->addTrace (signal->Ident, mIdent, cActivityBuffer::Ptr());
    else
      changed |= view->deleteTrace (signal->Ident);
    signal = signal->Next;
  }
  return changed;
}
//}}}

//{{{
cActivityBuffer::Ptr cActivityGroup::grabBuffer (int signalIdent)
{
  //QTRACE((L"cActivityLog::grabBuffer ident %d", signalIdent));

  cLockGuard MemberGuard (&SignalLock);

  // search group list for matching signal
  cActivity* cur = pSignals;
  while (cur)
  {
    if (cur->Ident == (unsigned int)signalIdent)
      return cur->grabBuffer();
    cur = cur->Next;
  }

  return cActivityBuffer::Ptr();    // null pointer - signal no longer exists
}
//}}}
//{{{
const String cActivityGroup::signalTitle (int signalIdent) const
{
  cLockGuard MemberGuard (&SignalLock);

  // search signal list for matching signal
  cActivity* cur = pSignals;
  while (cur)
  {
    if (cur->Ident == (unsigned int)signalIdent)
      return cur->title();
    cur = cur->Next;
  }

  return String();    // null string - signal no longer exists
}
//}}}
//{{{
int cActivityGroup::signalType (int signalIdent) const
{
  cLockGuard MemberGuard (&SignalLock);

  // search signal list for matching signal
  cActivity* cur = pSignals;
  while (cur)
  {
    if (cur->Ident == (unsigned int)signalIdent)
      return cur->type();
    cur = cur->Next;
  }

  return -1;    // - signal no longer exists
}
//}}}

//{{{
std::vector<String> cActivityGroup::getActivityNames (int signalIdent) const
{
  cLockGuard MemberGuard (&SignalLock);

  // search signal list for matching signal
  cActivity* cur = pSignals;
  while (cur)
  {
    if (cur->Ident == (unsigned int)signalIdent)
      return cur->getActivityNames();
    cur = cur->Next;
  }

  return std::vector<String>();    // null stringVec - signal no longer exists
}
//}}}
//{{{
std::vector<String> cActivityGroup::getSignalNames() const
{
  cLockGuard MemberGuard (&SignalLock);

  std::vector<String> names;

  cActivity* cur = pSignals;
  while (cur)
  {
    names.push_back (cur->title());
    cur = cur->Next;
  }

  return names;
}
//}}}

//{{{
void QAPI cActivityGroup::defineActivityCol  (int activityCol, const std::string& displayName)
{
  cLockGuard g (&SignalLock);
  mActivityColMap[activityCol] = displayName;
}
//}}}
//{{{
void QAPI cActivityGroup::defineActivitySpec (int activitySpec, const std::string& displaySpec)
{
  cLockGuard g (&SignalLock);
  mActivitySpecMap[activitySpec] = displaySpec;
}
//}}}
//{{{
void QAPI cActivityGroup::defineActivitySpec (const std::string& name, const std::string& displaySpec)
{
  cLockGuard g (&SignalLock);
  mActivityNamedSpecMap[name] = displaySpec;
}
//}}}
//{{{
tActivityStringMap QAPI cActivityGroup::getActivitySpecMap (int signalIdent) const
{
  cLockGuard g (&SignalLock);
  tActivityStringMap combinedMap = mActivitySpecMap;

  // search signal list for matching signal
  cActivity* cur = pSignals;
  while (cur)
  {
    if (cur->ident() == (unsigned int)signalIdent)
    {
      tActivityStringMap map = cur->getActivitySpecMap (signalIdent);
      for (tActivityStringMap::const_iterator it = map.begin(); it != map.end(); ++it)
        combinedMap[it->first] = it->second;
      return combinedMap;
    }
    cur = cur->Next;
  }

  cActivityGroup::Ptr curg = mSubGroups;
  while (curg.isValid())
  {
    tActivityStringMap map = curg->getActivitySpecMap (signalIdent);
    if (!map.empty())
    {
      for (tActivityStringMap::const_iterator it = map.begin(); it != map.end(); ++it)
        combinedMap[it->first] = it->second;
      return combinedMap;
    }
    curg = curg->Next;
  }

  return combinedMap;
}
//}}}
//{{{
iActivityGroup::Ptr QAPI cActivityGroup::getGroup (const String name)
{ // call cLockGuard BEFORE using this routine

  cActivityGroup::Ptr last;
  cActivityGroup::Ptr cur = mSubGroups;
  while (cur.isValid())
  {
    if (cur->mTitle.compare(name) == 0)
      return cur;          // group already exists, return it
    last = cur;
    cur = cur->Next;
  }

  cActivityGroup::Ptr newGroup = new cActivityGroup (name);

  if (last.isValid())
    last->Next = newGroup;
  else
    mSubGroups = newGroup;

  return newGroup;
}
//}}}
//}}}

//{{{  cActivityTrace members
// =======================================================================================
// === cActivityTrace =================================================================
// =======================================================================================
int cActivityTrace::Count = 0;

//{{{
cActivityTrace::cActivityTrace (cActivityLogView* parent, int signalIdent, int groupIdent, cActivityBuffer::Ptr buffer)
  : Parent (parent),
    SignalIdent(signalIdent),
    GroupIdent(groupIdent)
{
  Count++;
  //QSOS((L"create cActivityTrace %d signal %d", Count, SignalIdent));
  //QTRACE((L"cActivityTrace::cActivityTrace signal %d", SignalIdent));
  LogBuffer = buffer;

  // take local copies of the names
  SignalType      = (iActivity::eTraceType) cActivityLog::singleton().signalType (SignalIdent);
  SignalName      = cActivityLog::singleton().signalTitle (SignalIdent);
  GroupName       = cActivityLog::singleton().title (GroupIdent);
  ActivityNameVec = cActivityLog::singleton().getActivityNames (SignalIdent);
  mActivitySpecMap = cActivityLog::singleton().getActivitySpecMap (SignalIdent);
}
//}}}
//{{{
cActivityTrace::~cActivityTrace()
{
  //QTRACE((L"cActivityTrace::~cActivityTrace"));
  if (--Count <= 0)
  {
    //QSOS((L"ActivityTrace - all gone"));
  }
}
//}}}
//{{{
void cActivityTrace::dump() const
{
  QMSG((0, L"Trace: %d ", SignalIdent));
}
//}}}

//{{{
//void cActivityTrace::appendTrace(int signalIdent, cActivityBuffer::Ptr buffer)
//{
  //cActivityTrace::Ptr last = this;
  //while (last->Next.isValid())
    //last = last->Next;

  //last->Next = new cActivityTrace (signalIdent, buffer);
//}
//}}}

//{{{
void cActivityTrace::grabLog()
{
  LogBuffer  = cActivityLog::singleton().grabBuffer (SignalIdent);

  // re-grab local copies of the names
  SignalType      = (iActivity::eTraceType) cActivityLog::singleton().signalType (SignalIdent);
  SignalName      = cActivityLog::singleton().signalTitle (SignalIdent);
  GroupName       = cActivityLog::singleton().title (GroupIdent);
  ActivityNameVec = cActivityLog::singleton().getActivityNames (SignalIdent);

  mActivitySpecMap = cActivityLog::singleton().getActivitySpecMap (SignalIdent);
}
//}}}
//{{{
double cActivityTrace::firstLogTime() const
{
  if (!LogBuffer.isValid())
    return -1.0;

  UInt64 time = LogBuffer->firstLogTime();

  if (Parent)
  {
    if (time < 0) time = Parent->grabTime();
    return Parent->toSeconds (time);
  }

  if (time < 0) time = cActivityLog::getTime();
  return cActivityLog::toSeconds (time);
}
//}}}
//{{{
double cActivityTrace::lastLogTime() const
{
  if (!LogBuffer.isValid())
    return -1.0;

  UInt64 time = LogBuffer->lastLogTime();

  if (Parent)
  {
    if (time < 0) time = Parent->grabTime();
    return Parent->toSeconds (time);
  }

  if (time < 0) time = cActivityLog::getTime();
  return cActivityLog::toSeconds (time);
}
//}}}
//{{{
cActivityTrace::Ptr cActivityTrace::deleteNext()
{
  if (Next.isValid())
  {
    cActivityTrace::Ptr temp (Next);
    Next = temp->Next;
    temp.setNull();
  }
  return Next;
}
//}}}

//{{{
String cActivityTrace::activityName (int activity) const
{
  if ((activity >= 0) && (activity < (int)ActivityNameVec.size()))
    return ActivityNameVec.at(activity);

  return String();
}
//}}}

//{{{
iActivityIterator::Ptr cActivityTrace::activityIterator (UInt64 tickFreq)
{
  return new cActivityIterator (LogBuffer, tickFreq);
}
//}}}


//{{{
cActivityTrace::cActivityTrace (cActivityLogView* parent, iFile::Ptr file) throw(cNotFound, cTimedOut, cAccessDenied)
  : Parent (parent),
    SignalIdent(0),
    GroupIdent(0)
{
  Count++;

  cMemory::Ptr memHeaderLen = file->read ((UInt64)sizeof(int));
  UInt64 headerLen = *((int*) memHeaderLen->getBytes());

  cMemory::Ptr memHeader = file->read (headerLen);
  int* pHeader = (int*) memHeader->getBytes();

  SignalType = (iActivity::eTraceType) (*pHeader++);

  char* pHeaderStr = (char*) pHeader;
  char* pHeaderEnd = memHeader->getBytes() + memHeader->getSize();


  if (pHeaderStr < pHeaderEnd)
  {
    SignalName = (Char*) pHeaderStr;
    pHeaderStr += (SignalName.length() + 1) * sizeof(Char);  // skip string
  }

  if (pHeaderStr < pHeaderEnd)
  {
    GroupName = (Char*) pHeaderStr;
    pHeaderStr += (GroupName.length() + 1) * sizeof(Char);   // skip string
  }

  while (pHeaderStr < pHeaderEnd)
  {
    String activityName ((Char*) pHeaderStr);
    ActivityNameVec.push_back (activityName);
    pHeaderStr += (activityName.length() + 1) * sizeof(Char);   // skip string
  }

  LogBuffer = new cActivityBuffer (file);
}
//}}}
//{{{
void cActivityTrace::writeFile (iFile::Ptr file) const throw(cNotFound, cTimedOut, cAccessDenied)
{
  const int kLen = sizeof(Char);
  int i;
  int totalLen = sizeof(int);
  int signalLen = (int)SignalName.length() * kLen;
  int groupLen = (int)GroupName.length() * kLen;
  totalLen += signalLen + groupLen + 2 * kLen;

  std::vector<int> activityLen;
  activityLen.resize(ActivityNameVec.size(), 0);
  for (i=0; i<(int)activityLen.size(); i++)
  {
    activityLen.at(i) = (int)ActivityNameVec.at(i).length() * kLen;
    totalLen += activityLen.at(i) + kLen;
  }

  int zero = 0;
  const char* const pZero = (const char* const) &zero;

  file->write ((const UInt64)sizeof(int), (const char* const) &totalLen);   // length of strings block  -- to help when reading
  file->write ((const UInt64)sizeof(int), (const char* const) &SignalType);
  file->write ((const UInt64)(signalLen + kLen), (signalLen > 0) ? ((const char* const) SignalName.c_str()) : pZero);
  file->write ((const UInt64)(groupLen + kLen), (groupLen > 0) ? ((const char* const) GroupName.c_str()) : pZero);
  for (i=0; i<(int)activityLen.size(); i++)
    file->write ((const UInt64)(activityLen.at(i) + kLen), (activityLen.at(i) > 0) ? ((const char* const) ActivityNameVec.at(i).c_str()) : pZero);

  if (LogBuffer.isValid())
    LogBuffer->writeFile (file);
  else
    file->write ((const UInt64)sizeof(int), pZero);
}
//}}}

//{{{
int cActivityTrace::entryCount() const
{
  if (LogBuffer.isValid())
    return LogBuffer->entryCount();
  return 0;
}
//}}}
//{{{
const void* cActivityTrace::constData() const
{
  if (LogBuffer.isValid())
    return LogBuffer->constData();
  return 0;
}
//}}}
//{{{
const void* cActivityTrace::firstData() const
{
  if (LogBuffer.isValid())
    return LogBuffer->firstData();
  return 0;
}
//}}}

// v3 Extensions
//{{{
std::string QAPI cActivityTrace::getActivitySpec (int activity) const
{
  int activitySpec = (activity & iActivity::kActivitySpecMask) >> iActivity::kActivitySpecShift;
  //cLockGuard g (&mLock);
  tActivityStringMap::const_iterator it = mActivitySpecMap.find (activitySpec);
  if (it != mActivitySpecMap.end())
    return it->second;
  return std::string();
}
//}}}
//{{{
std::string QAPI cActivityTrace::getActivitySpec (const std::string& name) const
{
  //int activitySpec = (activity & iActivity::kActivitySpecMask) >> iActivity::kActivitySpecShift;
  ////cLockGuard g (&mLock);
  //tActivityStringMap::const_iterator it = mActivitySpecMap.find (activitySpec);
  //if (it != mActivitySpecMap.end())
    //return it->second;
  return std::string();
}
//}}}
//}}}
//{{{  cActivityIterator members
// =======================================================================================
// === cActivityIterator ==============================================================
// =======================================================================================
int cActivityIterator::Count = 0;

//{{{
cActivityIterator::cActivityIterator (cActivityBuffer::Ptr buffer, UInt64 tickFreq)
  : pRead(NULL),
    dTickFreq ((double) tickFreq)
{
  Count++;
  //QSOS((L"create cActivityIterator %d", Count));
  LogBuffer = buffer;
  first();
}

//}}}
//{{{
cActivityIterator::~cActivityIterator()
{
  LogBuffer.setNull();
  if (--Count <= 0)
  {
    //QSOS((L"ActivityIterator - all gone"));
  }
}

//}}}
//{{{
bool QAPI cActivityIterator::first()
{
  if (LogBuffer.isValid())
    return LogBuffer->first (pRead);
  return false;
}
//}}}
//{{{
bool QAPI cActivityIterator::next()
{
  if (LogBuffer.isValid())
    return LogBuffer->next (pRead);
  return false;
}
//}}}

// access routines - using pRead
//{{{
double QAPI cActivityIterator::startTime() const
{
  if (pRead)
    //return cActivityLog::toSeconds (pRead->StartTime);
    return ((double) (pRead->StartTime)) / dTickFreq;
  return 0.0;
}
//}}}
//{{{
double QAPI cActivityIterator::endTime() const
{
  if (pRead)
    //return cActivityLog::toSeconds (pRead->EndTime);
    return ((double) (pRead->EndTime)) / dTickFreq;
  return 0.0;
}
//}}}
//{{{
UInt64 QAPI cActivityIterator::startTicks() const
{
  if (pRead)
    return pRead->StartTime;
  return 0;
}
//}}}
//{{{
UInt64 QAPI cActivityIterator::endTicks() const
{
  if (pRead)
    return pRead->EndTime;
  return 0;
}
//}}}

//{{{
int QAPI cActivityIterator::activity() const
{
  if (pRead)
    return (pRead->Activity) & cActivityBuffer::kActivityMask;
  return 0;
}
//}}}
//{{{
bool QAPI cActivityIterator::event() const
{
  if (pRead)
    if (pRead->Activity & cActivityBuffer::kIsEvent)
      return true;

  return false;
}
//}}}
//{{{
bool QAPI cActivityIterator::failed() const
{
  if (pRead)
    if (pRead->Activity & cActivityBuffer::kFailed)
      return true;

  return false;
}
//}}}

//{{{
int QAPI cActivityIterator::detailType() const
{
  if (pRead)
    return (pRead->Activity >> 28) & 0x0f;

  return 0;
}
//}}}
//{{{
int QAPI cActivityIterator::value() const
{
  if (detailType() == iActivity::kValue)  // will be kNone if pRead=NULL
    return pRead->Details[0];

  return 0;
}
//}}}
//{{{
int QAPI cActivityIterator::dest() const
{
  switch (detailType())     // will be kNone if pRead=NULL
  {
    case iActivity::kValue:
    case iActivity::kSize:
    case iActivity::kArea:
    case iActivity::kPosArea:
    case iActivity::kAreaArea:
    case iActivity::kSizeSize:
      return pRead->Details[0] & 0xffff;
    case iActivity::kIdString:
    case iActivity::kIdGuid:
      return pRead->Details[0];
  }

  return -1;
}
//}}}
//{{{
int QAPI cActivityIterator::src() const
{
  switch (detailType())     // will be kNone if pRead=NULL
  {
    case iActivity::kValue:
    case iActivity::kSize:
    case iActivity::kArea:
    case iActivity::kPosArea:
    case iActivity::kAreaArea:
    case iActivity::kSizeSize:
      if (pRead->Details[1] == -1)   // old style
        return (pRead->Details[0] >> 16) & 0xffff;
      else
        return pRead->Details[1] & 0xffff;
  }

  return -1;
}
//}}}

//{{{
cXY QAPI cActivityIterator::size() const
{
  switch (detailType())     // will be kNone if pRead=NULL
  {
    case iActivity::kSize:
    case iActivity::kSizeSize:
      return cXY (pRead->Details[3], pRead->Details[4]);

    case iActivity::kArea:
      return cXY (pRead->Details[5], pRead->Details[6]);

    case iActivity::kPosArea:
    case iActivity::kAreaArea:
      return cXY (pRead->Details[4] & 0xffff, (pRead->Details[4] >> 16) & 0xffff);
  }

  return cXY(0,0);
}
//}}}
//{{{
cXY QAPI cActivityIterator::size2() const
{
  switch (detailType())     // will be kNone if pRead=NULL
  {
    case iActivity::kSize:     // repeat size1
      return cXY (pRead->Details[3], pRead->Details[4]);

    case iActivity::kArea:     // repeat size1
    case iActivity::kSizeSize:
      return cXY (pRead->Details[5], pRead->Details[6]);

    case iActivity::kAreaArea:
      return cXY (pRead->Details[6] & 0xffff, (pRead->Details[6] >> 16) & 0xffff);

    case iActivity::kPosArea:  // repeat size1
      return cXY (pRead->Details[4] & 0xffff, (pRead->Details[4] >> 16) & 0xffff);
  }

  return cXY(0,0);
}
//}}}
//{{{
cXY QAPI cActivityIterator::pos() const
{
  switch (detailType())     // will be kNone if pRead=NULL
  {
    case iActivity::kPosArea:
    case iActivity::kAreaArea:
      return cXY (pRead->Details[5] & 0xffff, (pRead->Details[5] >> 16) & 0xffff);
  }

  return cXY(0,0);
}
//}}}
//{{{
cRect QAPI cActivityIterator::area() const
{
  switch (detailType())     // will be kNone if pRead=NULL
  {
    case iActivity::kSize:
    case iActivity::kSizeSize:
      return cRect (0,0, pRead->Details[3], pRead->Details[4]);

    case iActivity::kArea:
      return cRect (pRead->Details[3], pRead->Details[4], pRead->Details[5], pRead->Details[6]);

    case iActivity::kPosArea:
    case iActivity::kAreaArea:
      return cRect (pRead->Details[3] & 0xffff, (pRead->Details[3] >> 16) & 0xffff,
                    pRead->Details[4] & 0xffff, (pRead->Details[4] >> 16) & 0xffff);
  }

  return cRect(0,0,0,0);
}
//}}}
//{{{
cRect QAPI cActivityIterator::area2() const
{
  switch (detailType())     // will be kNone if pRead=NULL
  {
    case iActivity::kSize:  // repeat size1
      return cRect (0,0, pRead->Details[3], pRead->Details[4]);

    case iActivity::kArea:  // repeat area1
      return cRect (pRead->Details[3], pRead->Details[4], pRead->Details[5], pRead->Details[6]);

    case iActivity::kSizeSize:
      return cRect (0,0, pRead->Details[5], pRead->Details[6]);

    case iActivity::kPosArea:
      return cRect (pRead->Details[5] & 0xffff, (pRead->Details[5] >> 16) & 0xffff,
                    pRead->Details[4] & 0xffff, (pRead->Details[4] >> 16) & 0xffff);  // repeat size1

    case iActivity::kAreaArea:
      return cRect (pRead->Details[5] & 0xffff, (pRead->Details[5] >> 16) & 0xffff,
                    pRead->Details[6] & 0xffff, (pRead->Details[6] >> 16) & 0xffff);
  }

  return cRect(0,0,0,0);
}
//}}}
//{{{
String QAPI cActivityIterator::description() const
{
  if (detailType() == iActivity::kString)    // will be kNone if pRead=NULL
  {
    // copy description from log to ensure a terminating zero
    char str [_INTSIZEOF(pRead->Details) + 1];
    strncpy (str, (char*) pRead->Details, sizeof(pRead->Details));
    str[_INTSIZEOF(pRead->Details)] = 0;
    return widen (str);
  }
  else if (detailType() == iActivity::kIdString)    // will be kNone if pRead=NULL
  {
    // copy description from log to ensure a terminating zero
    const int len = sizeof(pRead->Details) - sizeof(int);
    char* pSrc = ((char*) pRead->Details) + sizeof(int);
    char str [len + 1];
    strncpy (str, pSrc, len);
    str[len] = 0;
    return widen (str);
  }
  else if (detailType() == iActivity::kIdGuid)    // will be kNone if pRead=NULL
  {
    UInt64* pDetail64 = (UInt64*) (&(pRead->Details[1]));
    cGuid guid (pDetail64[0], pDetail64[1]);
    return guid.toString();
  }
  return L"";
}
//}}}

//{{{
cXYZ QAPI cActivityIterator::info() const
{
  switch (detailType())     // will be kNone if pRead=NULL
  {
    case iActivity::kSize:
    case iActivity::kArea:
    case iActivity::kPosArea:
    case iActivity::kAreaArea:
      if (pRead->Details[1] == -1)   // old style
        return cXYZ (pRead->Details[0] & 0xffff, pRead->Details[0] >> 16, 0);
      else
        return cXYZ (pRead->Details[0], pRead->Details[1], pRead->Details[2]);
  }

  return cXYZ(-1,-1,0);
}
//}}}
//}}}
//{{{  cActivityTraceIterator members
// =======================================================================================
// === cActivityTraceIterator ============================================================
// =======================================================================================
int cActivityTraceIterator::Count = 0;

//{{{
cActivityTraceIterator::cActivityTraceIterator (cActivityLogView* parent)
  : Parent(parent)
{
  Count++;
  //QSOS((L"create cActivityTraceIterator %d", Count));
  if (Parent)
    Trace = Parent->firstTrace();
}
//}}}
//{{{
cActivityTraceIterator::~cActivityTraceIterator()
{
  if (--Count <= 0) {
    //QSOS((L"ActivityTraceIterator - all gone"));
    }
}
//}}}
//{{{
bool QAPI cActivityTraceIterator::first()
{
  if (Parent)
    Trace = Parent->firstTrace();

  return Trace.isValid();
}
//}}}
//{{{
bool QAPI cActivityTraceIterator::next()
{
  if (Trace.isValid())
    Trace = Trace->next();

  return Trace.isValid();
}
//}}}
//{{{
bool QAPI cActivityTraceIterator::deleteTrace()
{
  if (Parent && Trace.isValid())
  {
    int ident = Trace->signalIdent();
    Trace = Trace->next();
    Parent->deleteTrace (ident);
  }
  return Trace.isValid();
}
//}}}
//{{{
void QAPI cActivityTraceIterator::dump() const
{
  if (Trace.isValid())
    Trace->dump();
}
//}}}

//{{{
iActivityIterator::Ptr QAPI cActivityTraceIterator::activityIterator()
{
  if (!Trace.isValid())
    return iActivityIterator::Ptr();  // null smart pointer

  return Trace->activityIterator (Parent ? Parent->tickFreq() : cActivityLog::singleton().tickFreq());
}
//}}}

//{{{
int QAPI cActivityTraceIterator::ident() const
{
  return Trace.isValid() ? Trace->signalIdent() : 0;
}
//}}}
//{{{
String QAPI cActivityTraceIterator::title() const
{
  return Trace.isValid() ? Trace->signalName() : String();
}
//}}}
//{{{
int QAPI cActivityTraceIterator::traceType() const
{
  return Trace.isValid() ? Trace->signalType() : 0;
}
//}}}
//{{{
String QAPI cActivityTraceIterator::activityName (int activity) const
{
  return Trace.isValid() ? Trace->activityName (activity) : String();
}
//}}}
//{{{
int QAPI cActivityTraceIterator::groupIdent() const
{
  return Trace.isValid() ? Trace->groupIdent() : 0;
}
//}}}
//{{{
String QAPI cActivityTraceIterator::groupName() const
{
  return Trace.isValid() ? Trace->groupName() : String();
}
//}}}

//{{{
int QAPI cActivityTraceIterator::activityEntryLength() const
{
  return sizeof (cActivityLogEntry);
}
//}}}
//{{{
int QAPI cActivityTraceIterator::entryCount() const
{
  if (Trace.isValid())
    return Trace->entryCount();
  return 0;
}
//}}}
//{{{
const void* QAPI cActivityTraceIterator::constData() const
{
  if (Trace.isValid())
    return Trace->constData();
  return 0;
}
//}}}
//{{{
const void* QAPI cActivityTraceIterator::firstData() const
{
  if (Trace.isValid())
    return Trace->firstData();
  return 0;
}
//}}}

//{{{
std::string QAPI cActivityTraceIterator::getActivitySpec (int activity) const
{
  if (Trace.isValid())
    return Trace->getActivitySpec (activity);
  return std::string();
}
//}}}
//{{{
std::string QAPI cActivityTraceIterator::getActivitySpec (const std::string& name) const
{
  if (Trace.isValid())
    return Trace->getActivitySpec (name);
  return std::string();
}
//}}}
//{{{
tActivityStringMap QAPI cActivityTraceIterator::getActivitySpecMap() const
{
  if (Trace.isValid())
    return Trace->getActivitySpecMap();
  return tActivityStringMap();
}
//}}}

//}}}
//{{{  cActivityLogView members
// =======================================================================================
// === cActivityLogView ==============================================================
// =======================================================================================
int cActivityLogView::Count = 0;

//{{{
cActivityLogView::cActivityLogView()
  : TickFreq (cActivityLog::singleton().tickFreq()),
    GrabTime (cActivityLog::singleton().getTime())
{
  Count++;
  //QSOS((L"create cActivityLogView %d", Count));
  //QTRACE((L"cActivityLogView::cActivityLogView"));
}
//}}}
//{{{
cActivityLogView::~cActivityLogView()
{
  Traces.setNull();  // smart pointers will cause whole list to be deleted
  if (--Count <= 0)
  {
    //QSOS((L"ActivityLogView - all gone"));
  }
}
//}}}

//{{{
bool cActivityLogView::addTrace (int signalIdent, int groupIdent, cActivityBuffer::Ptr buffer)
{
  cActivityTrace::Ptr last;
  cActivityTrace::Ptr trace = Traces;

  while (trace.isValid())
  {
    if (trace->signalIdent() == signalIdent)
      return false;
    last = trace;
    trace = trace->Next;
  }

  if (last.isValid())
    last->Next = new cActivityTrace (this, signalIdent, groupIdent, buffer);
  else
    Traces = new cActivityTrace (this, signalIdent, groupIdent, buffer);
  return true;
}
//}}}
//{{{
bool cActivityLogView::deleteTrace (int signalIdent)
{
  if (Traces.isValid())
  {
    cActivityTrace::Ptr trace = Traces;
    if (Traces->signalIdent() == signalIdent)
    {
      Traces = Traces->next();
      trace.setNull();
      return true;
    }

    while (trace->next().isValid())
    {
      if (trace->next()->signalIdent() == signalIdent)
      {
        trace->deleteNext();
        return true;
      }
      trace = trace->Next;
    }
  }
  return false;
}
//}}}

//{{{
double cActivityLogView::toSeconds (UInt64 time) const
{
  return ((double) time) / ((double) TickFreq);
}
//}}}
//{{{
double QAPI cActivityLogView::firstLogTime() const
{
  double minTime = toSeconds (GrabTime);

  cActivityTrace::Ptr cur = Traces;
  while (cur.isValid())
  {
    double time = cur->firstLogTime();
    if (time < minTime)
      minTime = time;
    cur = cur->Next;
  }
  return minTime;
}
//}}}
//{{{
double QAPI cActivityLogView::lastLogTime() const
{
  double maxTime = 0;

  cActivityTrace::Ptr cur = Traces;
  while (cur.isValid())
  {
    double time = cur->lastLogTime();
    if (time > maxTime)
      maxTime = time;
    cur = cur->Next;
  }
  return maxTime;
}
//}}}
//{{{
int cActivityLogView::traceCount() const
{
  int count = 0;
  cActivityTrace::Ptr cur = Traces;
  while (cur.isValid())
  {
    count++;
    cur = cur->next();
  }
  return count;
}
//}}}

//{{{
void QAPI cActivityLogView::grabLog()
{
  //QTRACE((L"cActivityLogView::grabLog"));
  cActivityTrace::Ptr trace = Traces;
  while (trace.isValid())
  {
    trace->grabLog();
    trace = trace->Next;
  }
  GrabTime = cActivityLog::singleton().getTime();
}
//}}}
//{{{
void QAPI cActivityLogView::updateGroups()
{
  cActivityLog::singleton().updateView (this);
}
//}}}
//{{{
iActivityTraceIterator::Ptr QAPI cActivityLogView::traceIterator()
{
  return new cActivityTraceIterator (this);
}
//}}}

//{{{
void cActivityLogView::readFile (iFile::Ptr file) throw(cNotFound, cTimedOut, cAccessDenied)
{
  UInt64 headerLen = 3 * sizeof(int) + 2 * sizeof(UInt64);
  Traces.setNull();  // smart pointers will cause whole list to be deleted

  cMemory::Ptr memHeader = file->read (headerLen);
  int* pHeader = (int*) memHeader->getBytes();

  int logVersion = *pHeader++;
  int entryVersion = *pHeader++;
  int numTraces = *pHeader++;

  UInt64* pHeader64 = (UInt64*) pHeader;

  TickFreq = *pHeader64++;
  GrabTime = *pHeader64++;

  //QTRACE((L"cActivityLogView::readFile -------------------------------------------"));
  //QTRACE((L"  logVersion=%d, entryVersion=%d, numTraces=%d", logVersion, entryVersion, numTraces));

  if ((logVersion > kLogVersion) || (entryVersion != 1) || (numTraces < 0) || (numTraces > 50))
  {
    QSOS((L"cActivityLogView::readFile - BAD FILE - aborting"));
    return;
  }

  cActivityTrace::Ptr curTrace;
  for (int i=0; i<numTraces; i++)
  {
    if (curTrace.isValid())
    {
      curTrace->Next = new cActivityTrace (this, file);
      curTrace = curTrace->Next;
    }
    else
    {
      Traces = new cActivityTrace (this, file);
      curTrace = Traces;
    }
  }
}
//}}}
//{{{
void cActivityLogView::writeFile (iFile::Ptr file) const throw(cNotFound, cTimedOut, cAccessDenied)
{
  const size_t headerLen = 3 * sizeof(int) + 2 * sizeof(UInt64);

  cMemory::Ptr memHeader = new cMemory (headerLen, cMemory::eHeap);

  int* pHeader = (int*) memHeader->getBytes();

  *pHeader++ = kLogVersion;
  *pHeader++ = cActivityLogEntry::kVersion;
  *pHeader++ = traceCount();

  UInt64* pHeader64 = (UInt64*) pHeader;

  *pHeader64++ = TickFreq;
  *pHeader64++ = GrabTime;

  file->write ((UInt64)headerLen, memHeader->getBytes());

  cActivityTrace::Ptr curTrace = Traces;
  while (curTrace.isValid())
  {
    curTrace->writeFile (file);
    curTrace = curTrace->next();
  }

  file->flushWrite();
}
//}}}

//{{{
void cActivityLogView::loadFromFile (const String& filename) throw (cNotFound, cTimedOut, cAccessDenied, cNotAvailable)
{
  readFile (iFileManager::singleton().openFile(filename, fFileAccess_Read));
}
//}}}
//{{{
void cActivityLogView::saveToFile (const String& filename) const throw (cNotFound, cTimedOut, cAccessDenied, cNotAvailable)
{
  writeFile (iFileManager::singleton().openFile(filename, fFileAccess_Write));
}
//}}}
//}}}

//{{{  cActivityLog members

// =======================================================================================
// === cActivityLog ======================================================================
// =======================================================================================

//{{{
cActivityLog::cActivityLog()
{
  TimeZero = iSystemInfo::singleton().getTicksSinceStart(&TickFreq);

  //QTRACE((L"--create cActivityLog %d", Count));
  pGroups = new cActivityGroup (L"Main"); // ensure this is first group (for Picbus & cPicTile)
  QTRACE((L"Tick freq %d", (unsigned int)TickFreq));
}
//}}}
//{{{
cActivityLog::~cActivityLog()
{
  pGroups.setNull();
  //QTRACE((L"--destroy ActivityLog"));
}
//}}}

//{{{
double QAPI cActivityLog::currentTime()
{
  return toSeconds (getTime());
}
//}}}
//{{{
UInt64 QAPI cActivityLog::currentTicks()
{
  return getTime();
}
//}}}
//{{{
UInt64 QAPI cActivityLog::tickFreq()
{
  return TickFreq;
}
//}}}

//{{{
int QAPI cActivityLog::groupCount() const
{
  cLockGuard MemberGuard (&SignalLock);

  int count = 0;
  cActivityGroup::Ptr group = pGroups;
  while (group.isValid())
  {
    count += 1;
    group = group->Next;
  }
  return count;
}
//}}}
//{{{
int QAPI cActivityLog::signalCount() const
{
  cLockGuard MemberGuard (&SignalLock);

  int count = 0;
  cActivityGroup::Ptr group = pGroups;
  while (group.isValid())
  {
    count += group->signalCount();
    group = group->Next;
  }
  return count;
}
//}}}

//{{{
void QAPI cActivityLog::resetLog()
{
  cLockGuard MemberGuard (&SignalLock);

  cActivityGroup::Ptr cur = pGroups;
  while (cur.isValid())
  {
    cur->resetLog();
    cur = cur->Next;
  }
  LARGE_INTEGER num;
  QueryPerformanceCounter (&num);
  TimeZero = num.QuadPart;
}
//}}}
//{{{
void QAPI cActivityLog::startLogging()
{
  cLockGuard MemberGuard (&SignalLock);

  cActivityGroup::Ptr cur = pGroups;
  while (cur.isValid())
  {
    cur->startLogging();
    cur = cur->Next;
  }
}
//}}}
//{{{
void QAPI cActivityLog::stopLogging()
{
  cLockGuard MemberGuard (&SignalLock);

  cActivityGroup::Ptr cur = pGroups;
  while (cur.isValid())
  {
    cur->stopLogging();
    cur = cur->Next;
  }
}

//}}}

//{{{
UInt64 cActivityLog::getTime()
{
  UInt64 NotUsed;
  return iSystemInfo::singleton().getTicksSinceStart(&NotUsed /* TickFreqHz */);
}
//}}}
//{{{
double cActivityLog::toSeconds (UInt64 time)
{
  return ((double) time) / ((double) TickFreq);
}
//}}}

//{{{
void cActivityLog::registerGroup (const String name, bool enabled)
{ // call cLockGuard BEFORE using this routine
  cLockGuard MemberGuard (&SignalLock);

  cActivityGroup::Ptr last;
  cActivityGroup::Ptr cur = pGroups;
  while (cur.isValid())
  {
    if (cur->title().compare(name) == 0)
    {
      if (enabled)
        cur->enable();
      return;          // group already exists
    }
    last = cur;
    cur = cur->Next;
  }

  cActivityGroup::Ptr newGroup = new cActivityGroup (name);
  if (newGroup.isValid())
  {
    if (enabled)
      newGroup->enable();

    if (last.isValid())
      last->Next = newGroup;
    else
      pGroups = newGroup;
  }
}
//}}}
//{{{
cActivityGroup::Ptr cActivityLog::findGroup (const String name)
{ // call cLockGuard BEFORE using this routine

  cActivityGroup::Ptr last;
  cActivityGroup::Ptr cur = pGroups;
  while (cur.isValid())
  {
    if (cur->title().compare(name) == 0)
    {
      cur->activate();
      return cur;          // group already exists, return it
    }
    last = cur;
    cur = cur->Next;
  }

  cActivityGroup::Ptr newGroup = new cActivityGroup (name);
  if (newGroup.isValid())
  {
    newGroup->activate();
    if (last.isValid())
      last->Next = newGroup;
    else
      pGroups = newGroup;
 }
  return newGroup;
}
//}}}
//{{{
iActivityGroup::Ptr QAPI cActivityLog::getGroup (const String name)
{
  cLockGuard MemberGuard (&SignalLock);

  return findGroup(name);
}
//}}}
//{{{
iActivity::Ptr QAPI cActivityLog::addSignal (const String name, iActivity::eTraceType type)
{
  String groupName (L"Others");
  String signalName (L"Unknown");

  String::size_type nameLen  = name.size();
  String::size_type slashpos = name.find('/');
  if (slashpos > nameLen)  // not found
    signalName = name;
  else
  {
    if (slashpos > 0)          // slash is not first character
      groupName = name.substr (0, slashpos);
    if (slashpos < nameLen-1)  // slash is not last character
      signalName = name.substr (slashpos+1);
  }

  cLockGuard MemberGuard (&SignalLock);

  cActivityGroup::Ptr group = findGroup (groupName);

  return group->addSignal (signalName, type);
}
//}}}
//{{{
iActivity::Ptr QAPI cActivityLog::addSignal (const String name)
{
  return addSignal (name, iActivity::kSimple);
}
//}}}

//{{{
bool cActivityLog::forgetGroup (int groupIdent)
{
  cLockGuard MemberGuard (&SignalLock);

  // search signal list for matching group & get its previous group
  cActivityGroup::Ptr prev;
  cActivityGroup::Ptr cur = pGroups;
  while (cur.isValid())
  {
    if (cur->ident() == groupIdent)
    {
      // unlink group
      if (prev.isValid())
        prev->Next = cur->Next;
      else
        pGroups = cur->Next;
      return true;
    }
    prev = cur;
    cur = cur->Next;
  }
  return false;
}
//}}}
//{{{
bool cActivityLog::forgetSignal (int signalIdent)
{
  cLockGuard MemberGuard (&SignalLock);

  cActivityGroup::Ptr cur = pGroups;
  while (cur.isValid())
  {
    if (cur->forgetSignal (signalIdent))
      return true;
    cur = cur->Next;
  }
  return false;
}
//}}}

//{{{
iActivityLogView::Ptr QAPI cActivityLog::createView()
{
  //QTRACE((L"cActivityLog::createView"));
  cActivityLogView::Ptr view = new cActivityLogView;

  cLockGuard MemberGuard (&SignalLock);

  cActivityGroup::Ptr group = pGroups;
  while (group.isValid())
  {
    group->addSignalsToView (view);
    group = group->Next;
  }
  return view;
}
//}}}
//{{{
void cActivityLog::updateView (cActivityLogView::Ptr view)
{
  //QTRACE((L"cActivityLog::createView"));
  if (view.isValid())
  {
    cLockGuard MemberGuard (&SignalLock);

    cActivityGroup::Ptr group = pGroups;
    while (group.isValid())
    {
      group->updateView (view);
      group = group->Next;
    }
  }
}
//}}}

//{{{
cActivityBuffer::Ptr cActivityLog::grabBuffer (int signalIdent)
{
  //QTRACE((L"cActivityLog::grabBuffer ident %d", signalIdent));

  cLockGuard MemberGuard (&SignalLock);
  cActivityBuffer::Ptr buf;

  // search signal list for matching signal
  cActivityGroup::Ptr cur = pGroups;
  while (cur.isValid() && !buf.isValid())
  {
    buf = cur->grabBuffer (signalIdent);
    cur = cur->Next;
  }

  return buf;
}
//}}}
//{{{
const String cActivityLog::signalTitle (int signalIdent) const
{
  cLockGuard MemberGuard (&SignalLock);
  String str;

  // search signal list for matching signal
  cActivityGroup::Ptr cur = pGroups;
  while (cur.isValid() && !str.length())
  {
    str = cur->signalTitle (signalIdent);
    cur = cur->Next;
  }

  return str;
}
//}}}
//{{{
int cActivityLog::signalType (int signalIdent) const
{
  cLockGuard MemberGuard (&SignalLock);

  // search signal list for matching signal
  cActivityGroup::Ptr cur = pGroups;
  while (cur.isValid())
  {
    int type = cur->signalType (signalIdent);
    if (type >= 0)
      return type;
    cur = cur->Next;
  }

  return -1;    // - signal no longer exists
}
//}}}
//{{{
std::vector<String> cActivityLog::getActivityNames (int signalIdent) const
{
  cLockGuard MemberGuard (&SignalLock);
  std::vector<String> strVec;

  // search signal list for matching signal
  cActivityGroup::Ptr cur = pGroups;
  while (cur.isValid() && !strVec.size())
  {
    strVec = cur->getActivityNames (signalIdent);
    cur = cur->Next;
  }

  return strVec;
}
//}}}


//{{{
int QAPI cActivityLog::firstGroup() const
{
  cLockGuard MemberGuard (&SignalLock);

  cActivityGroup::Ptr cur = pGroups;
  while (cur.isValid())
  {
    if (cur->isActivated())
      return cur->ident();
    cur = cur->Next;
  }
  return 0;
}
//}}}
//{{{
int QAPI cActivityLog::nextGroup (int groupIdent) const
{
  cLockGuard MemberGuard (&SignalLock);

  cActivityGroup::Ptr cur = pGroups;
  while (cur.isValid())
  {
    if (cur->ident() == groupIdent)   // found current group, now search for next activated group
    {
      cur = cur->Next;
      while (cur.isValid())
      {
        if (cur->isActivated())
          return cur->ident();
        cur = cur->Next;
      }
      return 0;
    }
    cur = cur->Next;
  }

  return 0;
}
//}}}
//{{{
const String QAPI cActivityLog::title (int groupIdent) const
{
  cLockGuard MemberGuard (&SignalLock);

  cActivityGroup::Ptr cur = pGroups;
  while (cur.isValid())
  {
    if (cur->ident() == groupIdent)
      return cur->title();
    cur = cur->Next;
  }

  return String();
}
//}}}
//{{{
bool QAPI cActivityLog::isEnabled (int groupIdent) const
{
  cLockGuard MemberGuard (&SignalLock);

  cActivityGroup::Ptr cur = pGroups;
  while (cur.isValid())
  {
    if (cur->ident() == groupIdent)
      return cur->isEnabled();
    cur = cur->Next;
  }

  return false;
}
//}}}
//{{{
int QAPI cActivityLog::signalCount (int groupIdent) const
{
  cLockGuard MemberGuard (&SignalLock);

  cActivityGroup::Ptr cur = pGroups;
  while (cur.isValid())
  {
    if (cur->ident() == groupIdent)
      return cur->signalCount();
    cur = cur->Next;
  }
  return 0;
}
//}}}
//{{{
bool QAPI cActivityLog::enableGroup (int groupIdent) const
{
  cLockGuard MemberGuard (&SignalLock);

  cActivityGroup::Ptr cur = pGroups;
  while (cur.isValid())
  {
    if (cur->ident() == groupIdent)
    {
      cur->enable();
      return true;
    }
    cur = cur->Next;
  }

  return false;
}
//}}}
//{{{
bool QAPI cActivityLog::disableGroup (int groupIdent) const
{
  cLockGuard MemberGuard (&SignalLock);

  cActivityGroup::Ptr cur = pGroups;
  while (cur.isValid())
  {
    if (cur->ident() == groupIdent)
    {
      cur->disable();
      return true;
    }
    cur = cur->Next;
  }

  return false;
}
//}}}

//{{{
void QAPI cActivityLog::requestGrab()
{
  cLockGuard g (&CallbacksThreadSafety);

  for (vector<iCallback::Ptr>::const_iterator it = Callbacks.begin(); it != Callbacks.end(); ++it)
  {
    (*it)->onGrabRequest();
  }
}
//}}}
//{{{
void QAPI cActivityLog::triggered()
{
  cLockGuard g (&CallbacksThreadSafety);

  for (vector<iCallback::Ptr>::const_iterator it = Callbacks.begin(); it != Callbacks.end(); ++it)
  {
    (*it)->onTrigger();
  }
}
//}}}
//{{{
cGuard::ConstPtr QAPI cActivityLog::addCallback(iCallback::Ptr NewCallback)
{
  cLockGuard g (&CallbacksThreadSafety);
  Callbacks.push_back(NewCallback);
  return new cGuard(this, NewCallback);
}
//}}}
//{{{
void QAPI cActivityLog::onGuardDestroyed(iGuarded::Ptr Guarded) throw()
{
  iCallback::Ptr Callback = ptr_cast<iCallback::Ptr>(Guarded);
  if (Callback.isValid())
  {
    cLockGuard g (&CallbacksThreadSafety);
    vector<iCallback::Ptr>::iterator it = find(Callbacks.begin(), Callbacks.end(), Guarded);
    if (it != Callbacks.end())
      Callbacks.erase(it);
  }
}
//}}}

//{{{
std::vector<String> cActivityLog::getSignalNames (int groupIdent) const
{
  cLockGuard MemberGuard (&SignalLock);

  cActivityGroup::Ptr cur = pGroups;
  while (cur.isValid())
  {
    if (groupIdent == cur->ident())
      return cur->getSignalNames();
    cur = cur->Next;
  }

  return std::vector<String>();
}
//}}}

//{{{
void QAPI cActivityLog::defineActivitySpec (int activitySpec, const std::string& displaySpec)
{
  cLockGuard g (&SignalLock);
  mActivitySpecMap[activitySpec] = displaySpec;
}
//}}}
//{{{
void QAPI cActivityLog::defineActivitySpec (const std::string& name, const std::string& displaySpec)
{
  cLockGuard g (&SignalLock);
  mActivityNamedSpecMap[name] = displaySpec;
}
//}}}
//{{{
//std::string cActivityLog::getActivitySpec (int activity) const
//{
  //cLockGuard g (&SignalLock);

  //// search signal list for matching signal
  //cActivityGroup::Ptr cur = pGroups;
  //while (cur.isValid() && map.empty())
  //{
    //tActivityStringMap map = cur->getActivitySpecMap (signalIdent);
    //if (!map.empty())
      //return map;
    //cur = cur->Next;
  //}

  //return tActivityStringMap();
//}
//}}}
//{{{
tActivityStringMap QAPI cActivityLog::getActivitySpecMap (int signalIdent) const
{
  cLockGuard g (&SignalLock);
  tActivityStringMap combinedMap = mActivitySpecMap;

  // search signal list for matching signal
  cActivityGroup::Ptr cur = pGroups;
  while (cur.isValid())
  {
    tActivityStringMap map = cur->getActivitySpecMap (signalIdent);
    if (!map.empty())
    {
      for (tActivityStringMap::const_iterator it = map.begin(); it != map.end(); ++it)
        combinedMap[it->first] = it->second;              // note: this overwrites, whereas .insert() does not
      return combinedMap;
    }
    cur = cur->Next;
  }

  return combinedMap;
}
//}}}
  //{{{
  std::string QAPI cActivityLog::getActivityColourName (int activity)
  {
    switch (activity & 31)
    {
      case  0: return "Red";
      case  1: return "Green";
      case  2: return "Blue";
      case  3: return "Cyan";
      case  4: return "Magenta";
      case  5: return "Yellow";
      case  6: return "DkRed";
      case  7: return "DkGreen";
      case  8: return "DkBlue";
      case  9: return "DkCyan";
      case 10: return "DkMagenta";
      case 11: return "DkYellow";
      case 12: return "Orange";
      case 13: return "Pink";
      case 14: return "LtBlue";
      case 15: return "Purple";
      case 16: return "LimeGreen";
      case 17: return "Turquoise";
      case 18: return "MidOrange";
      case 19: return "MidYellow";
      case 20: return "MidRed";
      case 21: return "MidPink";
      case 22: return "MidBlue";
      case 23: return "MidCyan";
      case 24: return "MidPurple";
      case 25: return "MidMagenta";
      case 26: return "SoftLime";
      case 27: return "SoftYellow";
      case 28: return "SoftGreen";
      case 29: return "SoftCyan";
      case 30: return "Black30";
      case 31: return "Black31";
    }
    return "Black";
  }
  //}}}

// ActivityLog triggers - use setTrigger for only or first entry (eTriggerNext set)
//                        use addTrigger for subsequent or last entry (eTriggerNext not set)
//{{{
bool QAPI cActivityLog::setTrigger (const cActivityTrigger& trigger)
{
  cActivity::resetTrigger();

  if (signalType (trigger.mSignalIdent) >= 0)  // signal was found
  {
    cActivity::addTrigger (trigger);
    return true;
  }
  return false;
}
//}}}
//{{{
bool QAPI cActivityLog::addTrigger (const cActivityTrigger& trigger)
{
  if (signalType (trigger.mSignalIdent) >= 0)  // signal was found
  {
    cActivity::addTrigger (trigger);
    return true;
  }
  return false;
}
//}}}
//{{{
void QAPI cActivityLog::resetTrigger()
{
  cActivity::resetTrigger();
}
//}}}

// iActivityPool creators  - for creating multiple lines of activities in ActivityMon
//{{{
iActivityPool::Ptr QAPI cActivityLog::createActivityNest (iActivity::Ptr (*funcCreateActivity)(const String& nameSuffix))    // nestable activities  (all except Base are marked as Detail)
{
  return new cActivityNest (funcCreateActivity);
}
//}}}
//{{{
iActivityPool::Ptr QAPI cActivityLog::createActivityMap (iActivity::Ptr (*funcCreateActivity)(const String& nameSuffix))     // indexed activities eg ProcNum                         suffix=number
{
  return new cActivityMap (funcCreateActivity);
}
//}}}
//{{{
iActivityPool::Ptr QAPI cActivityLog::createActivitySet (iActivity::Ptr (*funcCreateActivity)(const String& nameSuffix))     // multi-thread - gives new activity when calls overlap, suffix=letter
{
  return new cActivitySet (funcCreateActivity);
}
//}}}
//{{{
iActivityPool::Ptr QAPI cActivityLog::createActivityNestMap (iActivity::Ptr (*funcCreateActivity)(const String& nameSuffix)) // a Map of Nested activities
{
  return new cActivityNestMap (funcCreateActivity);
}
//}}}
//{{{
iActivityPool::Ptr QAPI cActivityLog::createActivityNestSet (iActivity::Ptr (*funcCreateActivity)(const String& nameSuffix)) // a Set of Nested activities
{
  return new cActivityNestSet (funcCreateActivity);
}
//}}}
//{{{
iActivityPool::Ptr QAPI cActivityLog::createActivityNest (const String& nameSuffix, iActivity::Ptr (*funcCreateActivity)(const String& nameSuffix))   // internal use
{
  return new cActivityNest (nameSuffix, funcCreateActivity);
}
//}}}

// DebugControlBits
//{{{
void cActivityLog::registerDebugControl (iDebugControlCallback::Ptr callback, const String& name)
{
  QVERIFY(name.size() > 0);
  QVERIFY(callback.isValid());
  cLockGuard g(&mDebugControlLock);
  mDebugControls.push_back (std::pair<iDebugControlCallback::Ptr, String>(callback, name));
}
//}}}
//{{{
bool cActivityLog::getDebugControl (size_t index) const
{
  cLockGuard g(&mDebugControlLock);
  if (index < mDebugControls.size())
    return mDebugControls[index].first->getState();
  return false;
}
//}}}
//{{{
void cActivityLog::setDebugControl (size_t index, bool newState)
{
  cLockGuard g(&mDebugControlLock);
  if (index < mDebugControls.size())
    mDebugControls[index].first->setState(newState);
}
//}}}
//{{{
size_t cActivityLog::getDebugControlCount() const
{
  cLockGuard g(&mDebugControlLock);
  return mDebugControls.size();
}
//}}}
//{{{
String cActivityLog::getDebugControlName(size_t index) const
{
  cLockGuard g(&mDebugControlLock);
  if (index < mDebugControls.size())
    return mDebugControls[index].second;
  return String(L"?");
}
//}}}

//}}}

