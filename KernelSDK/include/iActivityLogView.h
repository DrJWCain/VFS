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
**
**  This include file is only needed for viewer access to the ActivityLog. ie for ActivityMon
*/
#include "QKernelBase.h"
#include "QKernelExecEnviron.h"
#include <QKernelSDKVersion.h>
#include "QKernelBaseMacros.h"

namespace vfs
{
// =======================================================================================
// === iActivityLogView ==================================================================
// =======================================================================================
/*
*/
typedef std::map<int, std::string> tActivityStringMap;
typedef std::map<std::string, std::string> tActivityNamedStringMap;

//{{{
// =======================================================================================
// === iActivityIterator =================================================================
// =======================================================================================
/*
*/
class iActivityIterator
{
public:
  QDEFINE_SMARTPTR(iActivityIterator);
  virtual ~iActivityIterator() {}

  virtual bool QAPI first() = 0;
  virtual bool QAPI next() = 0;

  virtual double QAPI startTime() const = 0;
  virtual double QAPI endTime() const = 0;
  virtual UInt64 QAPI startTicks() const = 0;
  virtual UInt64 QAPI endTicks() const = 0;
  virtual int QAPI activity() const = 0;

  virtual bool QAPI event() const = 0;
  virtual bool QAPI failed() const = 0;
  virtual int QAPI detailType() const = 0;
  virtual int QAPI value() const = 0;
  virtual cXY QAPI size() const = 0;
  virtual cXY QAPI pos() const = 0;
  virtual cRect QAPI area() const = 0;
  virtual cRect QAPI area2() const = 0;
  virtual String QAPI description() const = 0;

  virtual int QAPI dest() const = 0;
  virtual int QAPI src() const = 0;

  virtual cXYZ QAPI info() const = 0;
  virtual cXY QAPI size2() const = 0;
};
//}}}
//{{{
// =======================================================================================
// === iActivityTraceIterator ============================================================
// =======================================================================================
/*
*/
class iActivityTraceIterator
{
public:
  QDEFINE_SMARTPTR(iActivityTraceIterator);
  virtual ~iActivityTraceIterator() {}

  virtual bool QAPI first() = 0;
  virtual bool QAPI next() = 0;
  virtual bool QAPI deleteTrace() = 0;

  virtual iActivityIterator::Ptr QAPI activityIterator() = 0;

  virtual String QAPI title() const = 0;
  virtual String QAPI activityName (int activity) const = 0;
  virtual int QAPI traceType() const = 0;

  virtual void QAPI dump() const = 0;
  virtual int QAPI groupIdent() const = 0;
  virtual String QAPI groupName() const = 0;

  virtual int QAPI ident() const = 0;

  virtual int QAPI activityEntryLength() const = 0;
  virtual int QAPI entryCount() const = 0;
  virtual const void* QAPI constData() const = 0;
  virtual const void* QAPI firstData() const = 0;

  // v3 Extensions
  virtual std::string QAPI getActivitySpec (int activity) const = 0;
  virtual std::string QAPI getActivitySpec (const std::string& name) const = 0;
  virtual tActivityStringMap QAPI getActivitySpecMap() const = 0;
};
//}}}
//{{{
// =======================================================================================
// === iActivityLogView ==================================================================
// =======================================================================================
/*
*/

class iActivityLogView
{
public:
  QDEFINE_SMARTPTR(iActivityLogView);
  virtual ~iActivityLogView() {}

  virtual double QAPI firstLogTime() const = 0;
  virtual double QAPI lastLogTime() const = 0;

  virtual void QAPI grabLog() = 0;
  virtual iActivityTraceIterator::Ptr QAPI traceIterator() = 0;
  virtual void QAPI updateGroups() = 0;

  virtual void loadFromFile (const String& filename) throw (cNotFound, cTimedOut, cAccessDenied, cNotAvailable) = 0;
  virtual void saveToFile (const String& filename) const throw (cNotFound, cTimedOut, cAccessDenied, cNotAvailable) = 0;

  // support for new ActivityMon
  virtual int traceCount() const = 0;
  virtual UInt64 tickFreq() const = 0;
  virtual UInt64 grabTime() const = 0;
};
//}}}

} // --- End of Namespace ------------------------------------------------------------------
