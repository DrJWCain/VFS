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
/*
**  
*/

// --- Headers ---------------------------------------------------------------------------

// Standard C library headers

// Standard C++ library headers

// vfs Kernel headers
#include "QKernelExec.h"
#include "iActivityLogView.h"

// Local headers

// --- Start of Namespace ----------------------------------------------------------------

namespace vfs
{
//{{{
// =======================================================================================
// === cActivityLogEntry =================================================================
// =======================================================================================

struct cActivityLogEntry
{
  // increment this if the struct changes OR the data format stored in the struct changes
  // -- allows for viewing of activity-log files from older versions
  enum { kVersion = 1 };

  UInt64 StartTime;
  UInt64 EndTime;
  int Activity;
  int Details[iActivity::kMaxPayload];
};


//}}}
//{{{
// =======================================================================================
// === cActivityBuffer ===================================================================
// =======================================================================================

class cActivityBuffer : public cRefCount
{
public:
  QDEFINE_SMARTPTR(cActivityBuffer);
  enum { kLogSize = 1024,
         kDetailTypeMask = 0xf0000000,
         kActivityMask   = 0x00ffffff,
         kFlagsMask      = 0x0f000000, kFailed = 1 << 27L, kIsEvent = 1 << 26L, kDone = 1 << 25L };
  //cActivityBuffer();
  cActivityBuffer (int bufsize);    // in chunks of kLogSize
  cActivityBuffer (iFile::Ptr file) throw(cNotFound, cTimedOut, cAccessDenied);    // load from a log-file
  ~cActivityBuffer();

  void activityStart ();
  void activityStart (int activity);
  void activityStart (int activity, int value);
  void activityStart (int activity, int value, const cXY& size);
  void activityStart (int activity, int value, const cRect& area);
  void activityStart (int activity, int value, const cXY& pos, const cRect& area);
  void activityStart (int activity, int value, const cRect& area1, const cRect& area2);
  void activityStart (int activity, const char* description);

  // these add dest & src ID in the cXYZ(cLogDetails-dest, cLogDetails-src, flags)
  void activityStart (int activity, const cXYZ& info, const cXY& size);
  void activityStart (int activity, const cXYZ& info, const cRect& area);
  void activityStart (int activity, const cXYZ& info, const cXY& pos, const cRect& area);
  void activityStart (int activity, const cXYZ& info, const cRect& area1, const cRect& area2);
  void activityStart (int activity, const cXYZ& info, const cXY& size1, const cXY& size2);

  void activityDone();
  UInt64 activityDone2();   // returns the duration in ticks
  void activityDone(int activity, const char* description);
  void activityFailed();
  void activityFailed(int activity, const char* description);

  void activityEvent (int activity, const char* description);

  // these add ID (normally essence-offset) in details[0] & 0xffff (matches cLogDetails-dest)
  void activityStartId (int activity, int id);
  void activityStartId (int activity, int id, const char* description);
  void activityEventId (int activity, int id, const char* description);

  void activityStartId (int activity, int id, const cGuid& guid);
  void activityEventId (int activity, int id, const cGuid& guid);

  // Use these if you want to apply the id when the activity is done
  void activityDoneId (int id);
  void activityDoneId (int id, const cGuid& guid);
  void activityDoneId (int id, const char* description);
  void activityFailedId (int id);
  void activityFailedId (int id, const cGuid& guid);
  void activityFailedId (int id, const char* description);

  int entryCount() const;
  int completedEntryCount() const;
  int totalCount() const { return PtrToUlong((const void *)(LogEnd - Log)); }
  int bufSize() const { return totalCount() / kLogSize; }

  UInt64 firstLogTime() const;
  UInt64 lastLogTime() const;

  void reset();
  void dump (bool dumpData) const;
  void writeFile (iFile::Ptr file) const throw(cNotFound, cTimedOut, cAccessDenied);
  void merge (cActivityBuffer::Ptr prevBuf);

  bool first (const cActivityLogEntry*& pRead) const;
  bool next  (const cActivityLogEntry*& pRead) const;

  void activityEnter (const cActivityBuffer* buf);

  // access for new ActivityMon
  const void* constData() const { return (const void*)Log; }
  const void* firstData() const { return (const void*)(Full ? pWrite : Log); }

  // v3 extensions
  void activityStart (int activity, const int* payload, int payloadLen);     // activity = ((activitySpec << kActivitySpecShift) & kActivitySpecMask) + (activityCol & kActivityColMask)

  bool checkTrigger (const cActivityTrigger& trigger) const;

private:
  static int Count;
  bool Full;     // set when Log is full and becomes a ring buffer
  cActivityLogEntry* pCurrent;
  cActivityLogEntry* pWrite;
  cActivityLogEntry* Log;      //pointer to Log data array on heap
  cActivityLogEntry* LogEnd;   //pointer 1 beyond end of Log data array

  int num;

  static int count;
};


//}}}

//{{{
// =======================================================================================
// === cActivity =================================================================
// =======================================================================================

class cActivity : public iActivity, public cRefCount
{
friend class cActivityGroup;           // for Next & Constructor & CurrentIdent
public:
  QDEFINE_SMARTPTR(cActivity);
//---------------------------
// iActivity virtual members
//---------------------------
  // routines for trace initialisation
  void QAPI addActivity (int activity, const String name);

  // routines for log entry
  void QAPI activityStart ();
  void QAPI activityStart (int activity);
  void QAPI activityStart (int activity, int value);
  void QAPI activityStart (int activity, const cXY& size);
  void QAPI activityStart (int activity, const cRect& area);
  void QAPI activityStart (int activity, const cXY& pos, const cRect& area);
  void QAPI activityStart (int activity, const cRect& area2, const cRect& area);
  void QAPI activityStart (int activity, const char* description);
  void QAPI activityDone();
  void QARGS_STACK activityDone(int activity, const char* description, ...);
  void QAPI activityFailed();
  void QARGS_STACK activityFailed(int activity, const char* description, ...);

  void QAPI activityStart (int activity, int value, const cXY& size);
  void QAPI activityStart (int activity, int value, const cRect& area);
  void QAPI activityStart (int activity, int value, const cXY& pos, const cRect& area);
  void QAPI activityStart (int activity, int value, const cRect& area2, const cRect& area);
  void QARGS_STACK activityStart2 (int activity, const char* description, ...);
  void QARGS_STACK activityStart3 (int activity, const char* description, va_list ap);

  void QAPI activityStart (int activity, const cXYZ& info, const cXY& size);
  void QAPI activityStart (int activity, const cXYZ& info, const cRect& area);
  void QAPI activityStart (int activity, const cXYZ& info, const cXY& pos, const cRect& area);
  void QAPI activityStart (int activity, const cXYZ& info, const cRect& area2, const cRect& area);
  void QAPI activityStart (int activity, const cXYZ& info, const cXY& size2, const cXY& size);

  void QARGS_STACK activityEvent (int activity, const char* description, ...);

  //void QARGS_STACK activityStart (int activity, const char* description, va_list ap);

  virtual void QAPI activityStartId (int activity, int id);
  virtual void QAPI activityStartId (int activity, int id, const char* description);
  virtual void QARGS_STACK activityStartId2 (int activity, int id, const char* description, ...);
  virtual void QARGS_STACK activityStartId3 (int activity, int id, const char* description, va_list ap);
  virtual void QARGS_STACK activityEventId (int activity, int id, const char* description, ...);

  virtual void QAPI activityStartId (int activity, int id, const cGuid& guid);
  virtual void QAPI activityEventId (int activity, int id, const cGuid& guid);

  // Use these if you want to apply the id when the activity is done
  virtual void QAPI activityDoneId (int id);
  virtual void QAPI activityDoneId (int id, const cGuid& guid);
  virtual void QAPI activityDoneId (int id, const char* description);
  virtual void QARGS_STACK activityDoneId2 (int id, const char* description, ...);
  virtual void QARGS_STACK activityDoneId3 (int id, const char* description, va_list ap);

  virtual void QAPI activityFailedId (int id);
  virtual void QAPI activityFailedId (int id, const cGuid& guid);
  virtual void QAPI activityFailedId (int id, const char* description);
  virtual void QARGS_STACK activityFailedId2 (int id, const char* description, ...);
  virtual void QARGS_STACK activityFailedId3 (int id, const char* description, va_list ap);

  virtual double QAPI activityDone2();  // return the duration in seconds

  unsigned int QAPI ident() const { return Ident; }
  void QAPI setType (eTraceType type) { Type = type; }

  // v3 extensions
  void QAPI defineActivitySpec (int activitySpec, const std::string& displaySpec);
  void QAPI defineActivitySpec (const std::string& name, const std::string& displaySpec);
  void QAPI defineActivityCol  (int activityCol, const std::string& displayName);
  const std::string QAPI getActivitySpec (int activitySpec) const;
  tActivityStringMap QAPI getActivitySpecMap (int signalIdent) const;
  void QAPI activityStart (int activity, const int* payload, int payloadLen);     // activity = ((activitySpec << kActivitySpecShift) & kActivitySpecMask) + (activityCol & kActivityColMask)
  void QAPI activityEvent (int activity, const int* payload, int payloadLen);
  void QAPI activityDone  (int activity, const int* payload, int payloadLen);

//---------------------------
// cActivity members
//---------------------------
  // routines for log display
  void setEnable (bool on);
  bool enabled() const { return Enabled; }
  bool signalEnabled() const { return SignalEnabled; }

  const String title() const { return Title; }
  const std::vector<String> getActivityNames() const { return ActivityNameVec; }
  eTraceType type() const { return Type; }

  cActivity* enable();
  cActivity* disable();
  cActivity* startLogging();
  cActivity* stopLogging();
  cActivity* resetLog();
  // for debug
  void QAPI dump() const;

  ~cActivity();

  static void actionTrigger();
  static void addTrigger (const cActivityTrigger& trigger);
  static void resetTrigger();

private:
  static int Count;
  cActivity (String title, eTraceType type, int bufsize);

  cActivityBuffer::Ptr grabBuffer();

  const cActivityLogEntry* first() const;
  const cActivityLogEntry* last() const;
  bool next (const cActivityLogEntry*& pRead) const;

  // To protect member variables from changes by another thread.
  mutable cLock LoggingLock;
  mutable cLock GrabLock;
  mutable cLock mLock;     // all vector & map access

  static unsigned int CurrentIdent;
  unsigned int Ident;

  cActivity* Next;   // this cannot be asmart pointers, as we want the signal
                     // to auto-delete when the Source Nulls its SmartPointer
  String Title;
  std::vector<String> ActivityNameVec;
  eTraceType Type;
  bool SignalEnabled;  // This signal is available for logging (controlled by enable/disable)
  bool Enabled;        // Enable logging (controlled by start/stopLogging if SignalEnabled)

  int Bufsize;  // 1..5 * kLogSize
  cActivityBuffer::Ptr LogBuffer;
  cActivityBuffer::Ptr PrevLogBuffer;

  tActivityStringMap mActivitySpecMap;
  tActivityNamedStringMap mActivityNamedSpecMap;

  static cLock mTriggerLock;
  static cActivityTrigger mTrigger;                         // the currently active trigger (copied from mTriggerStack)
  static unsigned int mTriggerIndex;                        // index of next trigger to use
  static std::vector<cActivityTrigger> mTriggerStack; // the only or list of consecutive triggers. Note: for repeat mCount is refetched from here
};


//}}}

//{{{
// =======================================================================================
// === cActivityTrace =================================================================
// =======================================================================================

class cActivityTrace : public cRefCount
{
friend class cActivityLogView;       // for Next & Constructor
public:
  QDEFINE_SMARTPTR(cActivityTrace);
  ~cActivityTrace();

  iActivityIterator::Ptr activityIterator (UInt64 tickFreq);
  double firstLogTime() const;
  double lastLogTime() const;

  void grabLog();
  void dump() const;
  void writeFile (iFile::Ptr file) const throw(cNotFound, cTimedOut, cAccessDenied);
  cActivityTrace::Ptr deleteNext();

  cActivityTrace::Ptr next() const         { return Next; }
  int signalIdent() const                  { return SignalIdent; }
  int groupIdent() const                   { return GroupIdent; }

  iActivity::eTraceType signalType() const { return SignalType; };
  String signalName() const                { return SignalName; }
  String groupName() const                 { return GroupName; }
  String activityName (int activity) const;

  // access for new ActivityMon
  int activityEntryLength() const;
  int entryCount() const;
  const void* constData() const;
  const void* firstData() const;

  // v3 Extensions
  std::string QAPI getActivitySpec (int activity) const;
  std::string QAPI getActivitySpec (const std::string& name) const;
  tActivityStringMap getActivitySpecMap() const { return mActivitySpecMap; }

private:
  static int Count;
  cActivityTrace (cActivityLogView* parent, int signalIdent, int groupIdent, cActivityBuffer::Ptr buffer);
  cActivityTrace (cActivityLogView* parent, iFile::Ptr file) throw(cNotFound, cTimedOut, cAccessDenied);

  int SignalIdent;
  int GroupIdent;
  cActivityTrace::Ptr Next;
  cActivityBuffer::Ptr LogBuffer;
  cActivityLogView* Parent;

  // local copies, to allow viewing of activity-log files
  iActivity::eTraceType SignalType;
  String SignalName;
  String GroupName;
  std::vector<String> ActivityNameVec;

  typedef std::vector<String>::const_iterator tActivityNameConstIt;

  tActivityStringMap mActivitySpecMap;
  tActivityNamedStringMap mActivityNamedSpecMap;
};
//}}}
//{{{
// =======================================================================================
// === cActivityLogView ======================================================================
// =======================================================================================

class cActivityLogView : public iActivityLogView, public cRefCount
{
public:
  QDEFINE_SMARTPTR(cActivityLogView);
  // increment this if activity-log file format changes
  // -- allows for loading of activity-log files from older versions
  enum { kLogVersion = 2 };

//---------------------------
// iActivityLogView virtual members
//---------------------------
  double QAPI firstLogTime() const;
  double QAPI lastLogTime() const;

  void QAPI grabLog();
  iActivityTraceIterator::Ptr QAPI traceIterator();
  void QAPI updateGroups();

  void loadFromFile (const String& filename) throw (cNotFound, cTimedOut, cAccessDenied, cNotAvailable);
  void saveToFile (const String& filename) const throw (cNotFound, cTimedOut, cAccessDenied, cNotAvailable);

//---------------------------
// cActivityLogView members
//---------------------------
  bool addTrace (int signalIdent, int groupIdent, cActivityBuffer::Ptr buffer);
  bool deleteTrace (int signalIdent);

  cActivityTrace::Ptr firstTrace() const { return Traces; }
  UInt64 tickFreq() const { return TickFreq; }
  UInt64 grabTime() const { return GrabTime; }
  int traceCount() const;
  double toSeconds (UInt64 time) const;

  void readFile (iFile::Ptr file) throw(cNotFound, cTimedOut, cAccessDenied);
  void writeFile (iFile::Ptr file) const throw(cNotFound, cTimedOut, cAccessDenied);

  cActivityLogView();
  ~cActivityLogView();

private:
  static int Count;
  cActivityTrace::Ptr Traces;
  UInt64 TickFreq;  // local copy, to allow for viewing of data from a different computer
  UInt64 GrabTime;
};


//}}}

//{{{
// =======================================================================================
// === cActivityIterator ==============================================================
// =======================================================================================

class cActivityIterator : public iActivityIterator, public cRefCount
{
public:
//---------------------------
// iActivityIterator virtual members
//---------------------------
  bool QAPI first();
  bool QAPI next();

  double QAPI startTime() const;
  double QAPI endTime() const;
  UInt64 QAPI startTicks() const;
  UInt64 QAPI endTicks() const;
  int QAPI activity() const;

  bool QAPI event() const;
  bool QAPI failed() const;
  int QAPI detailType() const;
  int QAPI value() const;
  cXY QAPI size() const;
  cXY QAPI pos() const;
  cRect QAPI area() const;
  cRect QAPI area2() const;
  String QAPI description() const;

  int QAPI dest() const;
  int QAPI src() const;

  cXYZ QAPI info() const;
  cXY QAPI size2() const;

//---------------------------
// cActivityIterator members
//---------------------------
  cActivityIterator (cActivityBuffer::Ptr buffer, UInt64 tickFreq);
  ~cActivityIterator();

private:
  static int Count;
  cActivityBuffer::Ptr LogBuffer;
  const cActivityLogEntry* pRead;
  double dTickFreq;  // local copy, to allow for viewing of data from a different computer
};


//}}}
//{{{
// =======================================================================================
// === cActivityTraceIterator ============================================================
// =======================================================================================

class cActivityTraceIterator : public iActivityTraceIterator, public cRefCount
{
public:
//---------------------------
// iActivityTraceIterator virtual members
//---------------------------
  bool QAPI first();
  bool QAPI next();
  bool QAPI deleteTrace();

  iActivityIterator::Ptr QAPI activityIterator();

  int QAPI ident() const;
  String QAPI title() const;
  String QAPI activityName (int activity) const;
  int QAPI traceType() const;

  void QAPI dump() const;
  int QAPI groupIdent() const;
  String QAPI groupName() const;

  // access for new ActivityMon
  int QAPI activityEntryLength() const;
  int QAPI entryCount() const;
  const void* QAPI constData() const;
  const void* QAPI firstData() const;

  // v3 Extensions
  std::string QAPI getActivitySpec (int activity) const;
  std::string QAPI getActivitySpec (const std::string& name) const;
  tActivityStringMap QAPI getActivitySpecMap() const;

//---------------------------
// cActivityTraceIterator members
//---------------------------
  cActivityTraceIterator (cActivityLogView* parent);

private:
  ~cActivityTraceIterator();

  static int Count;
  cActivityLogView* Parent;
  cActivityTrace::Ptr Trace;
};
//}}}

//{{{
// =======================================================================================
// === cActivityGroup =================================================================
// =======================================================================================

class cActivityGroup : public iActivityGroup, public cRefCount
{
friend class cActivityLog;          // for Next & Constructor
public:
  QDEFINE_SMARTPTR(cActivityGroup);
//---------------------------
// iActivityGroup virtual members
//---------------------------
  iActivity::Ptr QAPI addSignal (const String name, iActivity::eTraceType type, const int bufSize);
  iActivity::Ptr QAPI addSignal (const String name, iActivity::eTraceType type);
  iActivity::Ptr QAPI addSignal (const String name);
  iActivity::Ptr QARGS_STACK addSignal2(iActivity::eTraceType type, const int bufSize, const Char* const name, ...);
  iActivity::Ptr QARGS_STACK addSignal2(const int bufSize, const Char* const name, ...);
  String QAPI getName() const { return mTitle; }

  // v3 extensions
  void QAPI defineActivitySpec (int activitySpec, const std::string& displaySpec);
  void QAPI defineActivitySpec (const std::string& name, const std::string& displaySpec);
  void QAPI defineActivityCol  (int activityCol, const std::string& displayName);
  const std::string QAPI getActivitySpec (int activitySpec) const;
  tActivityStringMap QAPI getActivitySpecMap (int signalIdent) const;
  iActivityGroup::Ptr QAPI getGroup (const String name);        // nested groups

//---------------------------
// cActivityGroup members
//---------------------------
  void startLogging();
  void stopLogging();

  void activate() { mActivated = true; }   // declare DLL exists
  void enable();
  void disable();
  void resetLog();

  int QAPI signalCount();

  bool forgetSignal (int signalIdent);

  //void setEnable (bool on);
  //bool enabled() const { return Enabled; }
  //bool signalEnabled() const { return SignalEnabled; }

  int ident() { return mIdent; }
  const String title() const { return mTitle; }
  bool isEnabled() const { return mEnabled; }
  bool isActivated() const { return mActivated; }

  bool addSignalsToView (cActivityLogView::Ptr view);
  bool updateView (cActivityLogView::Ptr view);

  cActivityBuffer::Ptr grabBuffer (int signalIdent);
  const String signalTitle (int signalIdent) const;
  int signalType (int signalIdent) const;

  std::vector<String> getActivityNames (int signalIdent) const;
  std::vector<String> getSignalNames() const;

  // for debug
  void QAPI dump() const;

  ~cActivityGroup();

//-------------------------------------
private:
  static int Count;
  cActivityGroup (String title);

  const cActivity* first() const;
  const cActivity* last() const;

  // To protect member variables from changes by another thread.
  mutable cLock SignalLock;

  //static int CurrentIdent;
  int mIdent;

  cActivity* pSignals;  // this cannot be a smart pointer, as we want the signal
                        // to auto-delete when the Source Nulls its SmartPointer
  cActivityGroup::Ptr Next;

  String mTitle;
  bool mActivated;
  bool mEnabled;

  // v3 extensions
  tActivityStringMap mActivityColMap;
  tActivityStringMap mActivitySpecMap;
  tActivityNamedStringMap mActivityNamedSpecMap;
  cActivityGroup::Ptr mSubGroups;  // a linked list !!!
};


//}}}
//{{{
// =======================================================================================
// === cActivityLog ======================================================================
// =======================================================================================

class cActivityLog : public iActivityLog, public cRefCount, public iGuardCreator
{
public:

  QDEFINE_SINGLETON_IMPL(cActivityLog, L"{B7210747-F168-470d-B0C4-8A07E50C83E3} // cActivityLog", iActivityLog);

  // iManager
  void QAPI logSummary() throw() {}
  void QAPI logDetailedSummary() throw() {}

  static UInt64 getTime();
  static double toSeconds (UInt64 time);

// =======================================================================================
// iActivityLog virtual members
// =======================================================================================
  double QAPI currentTime();
  UInt64 QAPI currentTicks();
  UInt64 QAPI tickFreq();

  void QAPI resetLog();
  void QAPI startLogging();
  void QAPI stopLogging();

  int QAPI signalCount() const;

  iActivity::Ptr QAPI addSignal (const String name, iActivity::eTraceType type);
  iActivity::Ptr QAPI addSignal (const String name);

  int QAPI groupCount() const;
  void QAPI registerGroup (const String name, bool enabled);
  iActivityGroup::Ptr QAPI getGroup (const String name);

  int QAPI firstGroup() const;
  int QAPI nextGroup (int groupIdent) const;
  const String QAPI title (int groupIdent) const;
  bool QAPI isEnabled (int groupIdent) const;
  bool QAPI enableGroup (int groupIdent) const;
  bool QAPI disableGroup (int groupIdent) const;
  int QAPI signalCount (int groupIdent) const;

  cPtr<iActivityLogView> QAPI createView();

  void QAPI requestGrab();                                // callback with onGrab
  void QAPI triggered();                                  // callback with onTrigger
  cGuard::ConstPtr QAPI addCallback(iCallback::Ptr NewCallback);

  std::vector<String> QAPI getSignalNames (int groupIdent) const;

  // iActivityPool creators  - for creating multiple lines of activities in ActivityMon
  cPtr<iActivityPool> QAPI createActivityNest (iActivity::Ptr (*funcCreateActivity)(const String& nameSuffix));    // nestable activities  (all except Base are marked as Detail)
  cPtr<iActivityPool> QAPI createActivityMap (iActivity::Ptr (*funcCreateActivity)(const String& nameSuffix));     // indexed activities eg ProcNum                         suffix=number
  cPtr<iActivityPool> QAPI createActivitySet (iActivity::Ptr (*funcCreateActivity)(const String& nameSuffix));     // multi-thread - gives new activity when calls overlap, suffix=letter
  cPtr<iActivityPool> QAPI createActivityNestMap (iActivity::Ptr (*funcCreateActivity)(const String& nameSuffix)); // a Map of Nested activities
  cPtr<iActivityPool> QAPI createActivityNestSet (iActivity::Ptr (*funcCreateActivity)(const String& nameSuffix)); // a Set of Nested activities

  cPtr<iActivityPool> QAPI createActivityNest (const String& nameSuffix, iActivity::Ptr (*funcCreateActivity)(const String& nameSuffix));   // internal use

  // v3 extensions
  void QAPI defineActivitySpec (int activitySpec, const std::string& displaySpec);
  void QAPI defineActivitySpec (const std::string& name, const std::string& displaySpec);
  const std::string QAPI getActivitySpec (int activitySpec) const;
  //const tActivityStringMap QAPI getActivitySpecMap () const;
  tActivityStringMap QAPI getActivitySpecMap (int signalIdent) const;
  std::string QAPI getActivityColourName (int activity);

  // ActivityLog triggers - return true if ok = mSignaIdent was found
  bool QAPI setTrigger (const cActivityTrigger& trigger); // use for only or first entry (eTriggerNext set)
  bool QAPI addTrigger (const cActivityTrigger& trigger); // use for subsequent or last entry (eTriggerNext not set)
  void QAPI resetTrigger();

  // DebugControls
  void registerDebugControl (cPtr<iDebugControlCallback> callback, const String& name);
  // for use by ActivityMonitor
  size_t getDebugControlCount() const;
  String getDebugControlName(size_t index) const;
  bool getDebugControl (size_t index) const;
  void setDebugControl (size_t index, bool newState);

// =======================================================================================
// cActivityLog members
// =======================================================================================
  bool forgetGroup (int groupIdent);
  bool forgetSignal (int signalIdent);

  void updateView (cActivityLogView::Ptr view);

  cActivityBuffer::Ptr grabBuffer (int signalIdent);
  const String signalTitle (int signalIdent) const;
  int signalType (int signalIdent) const;

  std::vector<String> getActivityNames (int signalIdent) const;

  cActivityLog();
  ~cActivityLog();

private:
  static int Count;
  static cActivityLog* mActivityLogSingleton;
  static UInt64 TimeZero;
  static UInt64 TickFreq;
  static cTime mStartTime;  // time/date of TimeZero

  // To protect Signal list from changes by another thread.
  mutable cLock SignalLock;
  mutable cLock mSpecLock;

  mutable cLock CallbacksThreadSafety;
  std::vector<iCallback::Ptr> Callbacks;

  cActivityGroup::Ptr pGroups;

  cActivityGroup::Ptr findGroup (const String name);

  // iGuardCreator
  void QAPI onGuardDestroyed(iGuarded::Ptr Guarded) throw();

  // v3 extensions
  tActivityStringMap mActivitySpecMap;
  tActivityNamedStringMap mActivityNamedSpecMap;

  mutable cLock mDebugControlLock;
  std::vector< std::pair<cPtr<iDebugControlCallback>, String> > mDebugControls;
};
//}}}


// --- End of Namespace ------------------------------------------------------------------

}

