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

#if !defined(__129ECFD1289B40caBB3C1C1A828F0906)
#define __129ECFD1289B40caBB3C1C1A828F0906
/*
**  
*/
#include "QKernelBase.h"
#include "QKernelExecEnviron.h"
#include <QKernelSDKVersion.h>
#include "QKernelBaseMacros.h"

namespace vfs
{

//{{{
// =======================================================================================
// === sRegEventId: Identifying a registry value under HKEY_LOCAL_MACHINE for monitoring its change
// =======================================================================================

struct sRegEventId
{
  String mSubKeyName;
  String mValueName;

  sRegEventId(const String& subKeyName, const String& valueName) : mSubKeyName(subKeyName), mValueName(valueName) {}
  bool operator <(const sRegEventId& rhs) const
  {
    return mSubKeyName < rhs.mSubKeyName || mSubKeyName == rhs.mSubKeyName && mValueName < rhs.mValueName;
  }
};
//}}}

//{{{
// =======================================================================================
// === iRegEventObserver: Application need to implement this interface to be notified the change of registry value
// =======================================================================================
class iRegEventObserver : public iGuarded, public cRefCount
{
public :
  QDEFINE_SMARTPTR(iRegEventObserver);
  virtual ~iRegEventObserver() {}
  virtual void QAPI onRegKeyValueChanged(const sRegEventId& regId, const unsigned char* dataBuf, unsigned bufSize, long error) = 0;
};
//}}}

//{{{
// =======================================================================================
// === iModuleInstance ===================================================================
// =======================================================================================

class iModuleInstance
{
  public :

    QDEFINE_SMARTPTR(iModuleInstance);

    virtual ~iModuleInstance() {}

//{{{
// ---------------------------------------------------------------------------------------
/*
Returns the path to the private data folder allocated to this plugin. It is read only.
*/
    virtual String QAPI getDataFolder(
      void) const throw(cNotAvailable) = 0;
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
*/
    virtual unsigned long QAPI getCustomSettingNumber(
      const String & Path,
      const String & ItemName) const throw(cNotFound, cBadFormat) = 0;
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
*/
    virtual String QAPI getCustomSettingString(
      const String & Path,
      const String & ItemName) const throw(cNotFound, cBadFormat) = 0;
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
*/
    virtual void QAPI setCustomSettingNumber(
      const String &      Path,
      const String &      ItemName,
      const unsigned long NewNumber) throw(cNotFound, cBadFormat) = 0;
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
*/
    virtual void QAPI setCustomSettingString(
      const String & Path,
      const String & ItemName,
      const String & NewString) throw(cNotFound, cBadFormat) = 0;
//}}}
    //{{{
    enum eBootPhase
    {
      eBuiltIn                = 0,      // Private system drivers only
      eCore                   = 50,     // Private system drivers only
      eMonitoring             = 100,    // Private system drivers only
      eDevices                = 200,    // Device drivers only
      eSharedHighestPriority  = 1000,
      eSharedHighPriority     = 1100,
      eSharedLowPriority      = 1200,
      eSharedLowestPriority   = 1300,
      eHighestPriority        = 1400,
      eHighPriority           = 1500,
      eLowPriority            = 1600,  // Use this one if you don't care.
      eLowestPriority         = 1700
    };
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
*/
    virtual String QAPI getUserFolder(
      void) const throw(cNotAvailable) = 0;
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
*/
    virtual std::map<String, unsigned long> QAPI getCustomSettingNumbers(
      const String & Path) const throw(cNotFound, cBadFormat) = 0;
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
*/
    virtual std::map<String, String> QAPI getCustomSettingStrings(
      const String & Path) const throw(cNotFound, cBadFormat) = 0;
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
*/
    enum eSettingValueType
    {
      eSettingValueType_String,
      eSettingValueType_Number
    };
    virtual eSettingValueType QAPI getCustomSettingValueType(
      const String & Path,
      const String & Name) const throw(cNotFound) = 0;
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
*/
    virtual std::list<String> QAPI getCustomSettingSubPaths(
      const String & Path) const throw(cNotFound) = 0;
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
*/
    virtual void QAPI setPublicSettingString(
      const String & SettingId,
      const String & NewString) throw(cNotFound, cBadFormat)= 0;
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
*/
    virtual void QAPI setPublicSettingNumber(
      const String & SettingId,
      const int      NewValue) throw(cNotFound, cBadFormat) = 0;
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
*/
    virtual String QAPI getPublicSettingString(
      const String & SettingId) const throw(cNotFound, cBadFormat) = 0;
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
*/
    virtual int QAPI getPublicSettingNumber(
      const String & SettingId) const throw(cNotFound, cBadFormat) = 0;
//}}}
// ---------------------------------------------------------------------------------------
/*
*/
//// ---------------------------------------------------------------------------------------
///*
//*/
//    class iCustomSettingCallback
//    {
//      public :
//        QDEFINE_SMARTPTR(iCustomSettingCallback);
//        virtual void QAPI onItemChanged(const String & Path, const String & ItemName) = 0;
//    };
//
//    virtual cGuard::ConstPtr QAPI addCustomSettingCallback(
//      iCustomSettingCallback::Ptr NewCallback) = 0;
//}}}
//{{{
//// ---------------------------------------------------------------------------------------
///*
//*/
//    class iPublicSettingCallback
//    {
//      public :
//        QDEFINE_SMARTPTR(iCustomSettingCallback);
//        virtual void QAPI onValueChanged(const String & SettingId) = 0;
//    };
//
//    virtual cGuard::ConstPtr QAPI addPublicSettingCallback(
//      iPublicSettingCallback::Ptr NewCallback) = 0;
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
*/
    virtual eSettingValueType QAPI getPublicSettingValueType(
      const String & SettingId) const throw(cNotFound) = 0;
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
*/
    virtual cVersion QAPI getVersion() const = 0;
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
*/
    virtual String QAPI getRegistryPath() const = 0;
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
*/
    virtual void QAPI enablePublicSetting(const String & SettingId, const bool IsEnabled) throw(cNotFound, cBadFormat) = 0;
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
*/
    virtual String QAPI getRegistryCustomSettingsPath() const = 0;
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
*/
    virtual String QAPI getRegistryPublicSettingsPath() const = 0;
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
*/
    virtual String QAPI getModuleName() const = 0;
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
*/
    virtual String QAPI getDeveloperName() const = 0;
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
*/
  virtual cGuard::ConstPtr QAPI addKeyValueChangeObserver(const sRegEventId& regId, const iRegEventObserver::Ptr & observer) = 0;
//}}}

// Checks for custom registry setting to override defaultVal
//{{{
inline String checkCustomSetting (const String& path, const String& customSetting, const String& defaultVal)
{
  try
  {
    return getCustomSettingString (path, customSetting);
  }
  catch (cRecoverable&) {}
  return defaultVal;
}
//}}}
//{{{
inline String checkCustomSettingNonZero (const String& path, const String& customSetting, const String& defaultVal)
{
  try
  {
    String val (getCustomSettingString (path, customSetting));
    if (!val.empty()) return val;
  }
  catch (cRecoverable&) {}
  return defaultVal;
}
//}}}
//{{{
inline unsigned int checkCustomSetting (const String& path, const String& customSetting, unsigned int defaultVal)
{
  try
  {
    return getCustomSettingNumber (path, customSetting);
  }
  catch (cRecoverable&) {}
  return defaultVal;
}
//}}}
//{{{
inline unsigned int checkCustomSettingNonZero (const String& path, const String& customSetting, unsigned int defaultVal)
{
  try
  {
    unsigned int val = getCustomSettingNumber (path, customSetting);
    if (val != 0) return val;
  }
  catch (cRecoverable&) {}
  return defaultVal;
}
//}}}
//{{{
inline bool checkCustomSetting (const String& path, const String& customSetting, bool defaultVal)
{
  try
  {
    return (getCustomSettingNumber (path, customSetting) != 0);
  }
  catch (cRecoverable&) {}
  return defaultVal;
}
//}}}
};
typedef iModuleInstance iModuleContext; // For backwards source compatibility
//}}}
//{{{
// =======================================================================================
// === iMsgQueue =========================================================================
// =======================================================================================

const int fMsgVisitor_Abort = 1 << 0; // abort visiting msgs
const int fMsgVisitor_RemoveMsgFromQueue = 1 << 1; // remove current msg from queue

class iMsgQueue
{
  public:

    QDEFINE_SMARTPTR(iMsgQueue);

    virtual ~iMsgQueue() {}

    class iMsg
    {
      public :
        QDEFINE_SMARTPTR(iMsg);
        virtual ~iMsg() {}
#if kKernelSDKIfaceVersion_Int < 0400000
        // for backwards compatiblity with old code that uses "deliver() const"
        virtual void QAPI deliver2() { deliver(); }
        virtual void QAPI deliver() const {}
        virtual bool QAPI isQuitMessage() const { return false; }
        virtual bool QAPI isQuitMsg() const { return isQuitMessage(); }
#else
        // more flexible non-const deliver
        virtual void QAPI deliver2() = 0;
        virtual bool QAPI isQuitMsg() const { return false; }
#endif

#if defined(_DEBUG)
        iMsg()
        {
          // copied from _getCurrentThreadId() in cThreadManager.cpp
          unsigned long result;
#ifdef _M_X64
          result = (unsigned long)GetCurrentThreadId();
#else
          __asm
          {
            mov eax, fs:[0x24] // as documented by Matt Pietrik in MSJ - assumes win NT or W2K
            mov result, eax
          }
#endif
          mCreatorThreadId = result;
        }
      private:
        unsigned int mCreatorThreadId;
#endif
    };

#if kKernelSDKIfaceVersion_Int < 0400000
    class cQuitMessage : public iMsg, public cRefCount
    {
      public :
        QDEFINE_SMARTPTR(cQuitMessage);
        cQuitMessage() : m_SignalDelivered(false /* initially set? */) {}
        void QAPI deliver2() { m_SignalDelivered.set(); }
        bool QAPI isQuitMessage() const { return true; }
        iWaitable::eWakeupReason QAPI waitDelivered (const unsigned int timeoutMs) const { return m_SignalDelivered.wait(timeoutMs); }
        void QAPI waitDeliveredForever() const { m_SignalDelivered.waitForever(); }
      private :
        cManualResetEvent m_SignalDelivered;
    };
#endif
    class cQuitMsg : public iMsg, public cRefCount
    {
      public :
        QDEFINE_SMARTPTR(cQuitMsg);
        cQuitMsg() : m_SignalDelivered(false /* initially set? */) {}
        void QAPI deliver2() { m_SignalDelivered.set(); }
        bool QAPI isQuitMsg() const { return true; }
        iWaitable::eWakeupReason QAPI waitDelivered (const unsigned int timeoutMs) const { return m_SignalDelivered.wait(timeoutMs); }
        void QAPI waitDeliveredForever() const { m_SignalDelivered.waitForever(); }
      private :
        cManualResetEvent m_SignalDelivered;
    };
    /*
    Post a iMsg for delivery in the postBox's thread. Messages will accumulate
    in a queue if the thread is not running.
    */
    virtual void QAPI postMsg(const iMsg::Ptr & newMsg) = 0;
    /*
    Clear the message queue of all accumulated messages
    */
    virtual unsigned int /* num culled */ QAPI clearMsgs() = 0;
    virtual unsigned int QAPI countMsgs() const = 0;
    /*
    Returns true if a message is waiting on the internal message queue. Avoid busy waiting
    for a new message with this method - use waitMessageForever() instead.
    */
    virtual bool QAPI isMsgWaiting() const = 0;
    /*
    Waits forever for a message to appear on the internal message queue. Use getMessage() to
    retrieve it. Returns immediately if there are message waiting.
    */
    virtual void QAPI waitMsgForever() const = 0;
    /*
    Waits for a message to appear on the internal message queue. Use getMessage() to
    retrieve it. Returns immediately if there are message waiting.
    */
    virtual bool QAPI waitMsg(const unsigned int timeoutMs) const = 0;
    /*
    Pops the oldest message off the internal message queue into a specified message ptr.
    Returns true if a message was waiting in the queue, false if not.
    If false is returned then the specified message ptr is unmodified.
    */
    virtual bool QAPI getMsg(iMsg::Ptr * const resultMsg) = 0;
    /*
    Post a iMsg for delivery in the postBox's thread. Messages will accumulate
    in a queue if the thread is not running.
    */
    virtual void QAPI postMsgFront(const iMsg::Ptr & newMsg) = 0;

    /*
    Visit all the msgs in the queue. Note that queue is locked so do not take too long in onVisitMsg()!
    */
    class iMsgVisitor
    {
      public :
        QDEFINE_SMARTPTR(iMsgVisitor);
        virtual ~iMsgVisitor() {}
        virtual int /* fMsgVisitor */ QAPI onMsgVisit(const unsigned int position /* 0 = head, oldest */, const unsigned int totalNumMsgs, const iMsg::ConstPtr & msg)  = 0;
    };
    virtual bool /* visited all? see fMsgVisitor_Abort */ visitMsgsOldestFirst(iMsgVisitor & visitor) = 0;
    virtual bool /* visited all? see fMsgVisitor_Abort */ visitMsgsNewestFirst(iMsgVisitor & visitor) = 0;

#if kKernelSDKIfaceVersion_Int < 0400000
    virtual unsigned int /* num culled */ QAPI clearMessages(const type_info& messageType) = 0;
    virtual unsigned int /* num culled */ QAPI clearMessages(const type_info& messageType, const unsigned int leavingMinNumMsgsIntact) = 0;
    virtual unsigned int /* num culled */ clearNewestMessagesIfMoreThan(const unsigned int maxNumMsgs) = 0;
    virtual unsigned int /* num culled */ clearOldestMessagesIfMoreThan(const unsigned int maxNumMsgs) = 0;
#endif
};
//}}}
//{{{
// =======================================================================================
// === iCircularMsgQueue =================================================================
// =======================================================================================

class iCircularMsgQueue
{
  public:

    QDEFINE_SMARTPTR(iCircularMsgQueue);

    virtual ~iCircularMsgQueue() {}

    class iMsg
    {
      public :
        QDEFINE_SMARTPTR(iMsg);
        virtual ~iMsg() {}
        virtual void QAPI deliver() = 0;
        virtual bool QAPI isQuitMsg() const { return false; }
    };

    class cQuitMsg : public iMsg, public cRefCount
    {
      public :
        QDEFINE_SMARTPTR(cQuitMsg);
        cQuitMsg() : m_SignalDelivered(false /* initially set? */) {}
        void QAPI deliver() { m_SignalDelivered.set(); }
        bool QAPI isQuitMsg() const { return true; }
        iWaitable::eWakeupReason QAPI waitDelivered (const unsigned int timeoutMs) const { return m_SignalDelivered.wait(timeoutMs); }
        void QAPI waitDeliveredForever() const { m_SignalDelivered.waitForever(); }
      private :
        cManualResetEvent m_SignalDelivered;
    };
    /*
    Post a iMsg for delivery in the postBox's thread. Messages will accumulate
    in a queue if the thread is not running.
    */
    virtual void QAPI postMsg(const iMsg::Ptr & newMsg) = 0;
    /*
    Clear the message queue of all accumulated messages
    */
    virtual unsigned int /* num culled */ QAPI clearMsgs() = 0;
    virtual unsigned int QAPI countMsgs() const = 0;
    /*
    Returns true if a message is waiting on the internal message queue. Avoid busy waiting
    for a new message with this method - use waitMessageForever() instead.
    */
    virtual bool QAPI isMsgWaiting() const = 0;
    /*
    Waits forever for a message to appear on the internal message queue. Use getMessage() to
    retrieve it. Returns immediately if there are message waiting.
    */
    virtual void QAPI waitMsgForever() const = 0;
    /*
    Waits for a message to appear on the internal message queue. Use getMessage() to
    retrieve it. Returns immediately if there are message waiting.
    */
    virtual bool QAPI waitMsg(const unsigned int timeoutMs) const = 0;
    /*
    Pops the oldest message off the internal message queue into a specified message ptr.
    Returns true if a message was waiting in the queue, false if not.
    If false is returned then the specified message ptr is unmodified.
    */
    virtual bool QAPI getMsg(iMsg::Ptr * const resultMsg) = 0;
    /*
    Post a iMsg for delivery in the postBox's thread. Messages will accumulate
    in a queue if the thread is not running.
    */
    virtual void QAPI postMsgFront(const iMsg::Ptr & newMsg) = 0;
};
//}}}
//{{{
// =======================================================================================
// === iBasicThread ======================================================================
// =======================================================================================
/*
Provides a shell for spawning threads. You should implement runLoop() being the function
doing the aynshronous task which either returns after it is done or loops until it is
told to quit when signalLoopStop() is called.

The actual system thread spawned is not actually created until you call startLoop() either
in your derived class' constructor or elsewhere. At some point you will want to make
sure that the thread is stopped so you should can stopThread(). This will 'spike' your
loop causing it to terminate. stopThread() will do this for you but will not return until
your runLoop() method has finished.

There are two ways to use cThread: when you don't care about when it finishes, and when
you want to make sure it is finished and cleaned-up at some point. In your code only
difference is whether you wait for the thread to finish at some point.
*/
class iBasicThread : public iGuarded
{
  public:

    QDEFINE_SMARTPTR(iBasicThread);

    enum ePriority
    {
// DEPRECATED
      eIdle                  = 0, // Old style - use ePriority_ prefixed version instead
      eLowest                = 1, // Old style - use ePriority_ prefixed version instead
      eBelowNormal           = 2, // Old style - use ePriority_ prefixed version instead
      eNormal                = 3, // Old style - use ePriority_ prefixed version instead
      eAboveNormal           = 4, // Old style - use ePriority_ prefixed version instead
      eHighest               = 5, // Old style - use ePriority_ prefixed version instead
      eTimeCritical          = 6, // Old style - use ePriority_ prefixed version instead
// DEPRECATED
      ePriority_Idle         = 0,
      ePriority_Lowest       = 1,
      ePriority_BelowNormal  = 2,
      ePriority_Normal       = 3,
      ePriority_AboveNormal  = 4,
      ePriority_Highest      = 5,
      ePriority_TimeCritical = 6
    };
    /*
    You must implement this method which should 'spike' your loop inside runLoop() which should
    cause it to return. The Kernel will call this method during stopThread() before waiting
    indefinitely for runLoop() to return.
    */
    virtual void QAPI signalLoopStop() = 0;
    /*
    This function returns true if the thread completed in time, false otherwise. You should
    still call stopLoop(). You may have to use signalLoopStop() to make the runLoop() method exit.
    */
    virtual iWaitable::eWakeupReason QAPI waitLoopComplete(const unsigned long timeoutMs) = 0;
    /*
    This function returns only when the loop has completed. You may have to use signalLoopStop()
    to make the runLoop() method exit.
    */
    virtual void QAPI waitLoopCompleteForever() = 0;
    /*
    Signals the loop to stop then waits for it's runLoop() method to exit. Will wait forever
    if necessary.
    */
    virtual void QAPI stopLoop() = 0;
    virtual void QAPI startLoop() = 0;
    /*
    Returns the name of this thread.
    */
    virtual String QAPI getName() const = 0;
    /*
    Returns the operting system specific thread ID. Returns zero if thread is not running.
    */
    virtual unsigned long QAPI getThreadID() const = 0;
    virtual void QAPI setPriority(const ePriority NewPriority) = 0;
    virtual ePriority QAPI getPriority() const = 0;
    /*
    You must implement this function. It is called from within startLoop() and on its
    return will attempt to destroy the cThread derived object by way of a smart pointer held
    internally for the duration of runLoop().
    */
    virtual void QAPI runLoop() = 0;
#if kKernelSDKIfaceVersion_Int < 0400000
    virtual void QAPI onBeforeLoopStarted() {}
    virtual void QAPI onAfterLoopStopped() {}
#endif
    enum eExitState { eExitState_StillRunning, eExitState_Ok, eExitState_UnhandledException };
    virtual eExitState QAPI getExitState(String * const desc) const = 0;
};
//}}}
//{{{
// =======================================================================================
// === iThread ===========================================================================
// =======================================================================================

#if kKernelSDKIfaceVersion_Int < 0400000
class iThread : public iBasicThread, public iMsgQueue
#else
class iThread : public iBasicThread
#endif
{
  public:

    QDEFINE_SMARTPTR(iThread);

    virtual void QAPI postMessage(const iMsgQueue::iMsg::Ptr & newMsg) = 0;
    virtual void QAPI postMessageFront(const iMsgQueue::iMsg::Ptr & newMsg) = 0;
    virtual unsigned int /* num culled */ QAPI clearAllMessages() = 0;
    virtual unsigned int QAPI getNumMessages() const = 0;
    virtual bool QAPI isMessageWaiting() const = 0;
    virtual void QAPI waitMessageForever() const = 0;
    virtual bool QAPI waitMessage(const unsigned int timeoutMs) const = 0;
    virtual iMsgQueue::iMsg::Ptr QAPI getMessage() throw(cNotFound) = 0;
    virtual bool /* visited all? see fMsgVisitor_Abort */ visitMsgsOldestFirst(iMsgQueue::iMsgVisitor & visitor) = 0;
    virtual bool /* visited all? see fMsgVisitor_Abort */ visitMsgsNewestFirst(iMsgQueue::iMsgVisitor & visitor) = 0;
};
//}}}
//{{{
// =======================================================================================
// === iMemoryAllocator ==================================================================
// =======================================================================================

class iMemoryAllocator
{
  public :

    QDEFINE_SMARTPTR(iMemoryAllocator);

    virtual ~iMemoryAllocator() {}

    virtual void * QAPI allocate() = 0;
    virtual void QAPI release(void * const ObjectToDelete) = 0;
};
//}}}
//{{{  iActivityLog
// =======================================================================================
// === iActivity =========================================================================
// =======================================================================================
/*
*/
// logging
// (at least 5) SJSJSJ Increased from 3 to 7 to allow of longer activity description

//{{{
class iActivity
{
public:
  QDEFINE_SMARTPTR(iActivity);
  virtual ~iActivity() {}
  enum
  {
    kMaxPayload = 7,
    kMaxPayloadBytes = kMaxPayload  * sizeof(int),
    kActivitySpecShift = 8,
    kUserDefinedMask   = 0xf0000000,
    kActivitySpecMask  = 0x00ffff00,
    kActivityColMask   = 0x000000ff,
    kActivityMask      = 0x00ffffff,
    kFlagsMask         = 0x0f000000, kFailed = 1 << 27L, kEvent = 0x2 << 25L, kDone = 0x1 << 25L, kSpare = 1 << 24L
  };
  enum eTraceType { kSimple=0, kMultiColour, kNested,     kDetail=4 };
  enum eDetailType { kNone=0, kValue, kSize, kArea, kPosArea, kAreaArea, kString, kSizeSize, kIdString, kIdGuid };
  enum eActivityColour
  {
    kRed        = 0,          // only need simple names as it is already iActivity::kRed
    kGreen      = 1,
    kBlue       = 2,
    kCyan       = 3,
    kMagenta    = 4,
    kYellow     = 5,
    kDkRed      = 6,
    kDkGreen    = 7,
    kDkBlue     = 8,
    kDkCyan     = 9,
    kDkMagenta  = 10,
    kDkYellow   = 11,
    kOrange     = 12,
    kPink       = 13,
    kLtBlue     = 14,
    kPurple     = 15,
    kLimeGreen  = 16,
    kTurquoise  = 17,
    kMidOrange  = 18,
    kMidYellow  = 19,
    kMidRed     = 20,
    kMidPink    = 21,
    kMidBlue    = 22,
    kMidCyan    = 23,
    kMidPurple  = 24,
    kMidMagenta = 25,
    kSoftLime   = 26,
    kSoftYellow = 27,
    kSoftGreen  = 28,
    kSoftCyan   = 29,
    kBlack30    = 30,
    kBlack31    = 31,

    kActivityColour_Red       = 0,     // iActivity::kActivityColour_Red      !!!
    kActivityColour_Green     = 1,
    kActivityColour_Blue      = 2,
    kActivityColour_Cyan      = 3,
    kActivityColour_Magenta   = 4,
    kActivityColour_Yellow    = 5,
    kActivityColour_DkRed     = 6,
    kActivityColour_DkGreen   = 7,
    kActivityColour_DkBlue    = 8,
    kActivityColour_DkCyan    = 9,
    kActivityColour_DkMagenta = 10,
    kActivityColour_DkYellow  = 11,
    kActivityColour_Orange    = 12,
    kActivityColour_Pink      = 13,
    kActivityColour_LtBlue    = 14,
    kActivityColour_Purple    = 15
  };

  // routines for trace initialisation
  virtual void QAPI addActivity (int activity, const String name) = 0;

  // routines for log entry
  virtual void QAPI activityStart (int activity) = 0;
  virtual void QAPI activityStart (int activity, int value) = 0;
  virtual void QAPI activityStart (int activity, const cXY& size) = 0;
  virtual void QAPI activityStart (int activity, const cRect& area) = 0;
  virtual void QAPI activityStart (int activity, const cXY& pos, const cRect& area) = 0;
  virtual void QAPI activityStart (int activity, const cRect& area2, const cRect& area) = 0;
  virtual void QAPI activityStart (int activity, const char* description) = 0;
  virtual void QAPI activityDone() = 0;
  virtual void QAPI activityFailed() = 0;

  virtual void QAPI activityStart (int activity, int value, const cXY& size) = 0;
  virtual void QAPI activityStart (int activity, int value, const cRect& area) = 0;
  virtual void QAPI activityStart (int activity, int value, const cXY& pos, const cRect& area) = 0;
  virtual void QAPI activityStart (int activity, int value, const cRect& area2, const cRect& area) = 0;
  virtual void QARGS_STACK activityStart2 (int activity, const char* description, ...) = 0;

  virtual void QAPI activityStart (int activity, const cXYZ& info, const cXY& size) = 0;
  virtual void QAPI activityStart (int activity, const cXYZ& info, const cRect& area) = 0;
  virtual void QAPI activityStart (int activity, const cXYZ& info, const cXY& pos, const cRect& area) = 0;
  virtual void QAPI activityStart (int activity, const cXYZ& info, const cRect& area2, const cRect& area) = 0;
  virtual void QAPI activityStart (int activity, const cXYZ& info, const cXY& size2, const cXY& size) = 0;

  // Use these if you want to apply the label when the activity is done
  virtual void QAPI activityStart() = 0;
  virtual void QARGS_STACK activityDone(int activity, const char* description, ...) = 0;
  virtual void QARGS_STACK activityFailed(int activity, const char* description, ...) = 0;

  virtual void QARGS_STACK activityEvent (int activity, const char* description, ...) = 0;
  virtual void QARGS_STACK activityStart3 (int activity, const char* description, va_list ap) = 0;

  virtual void QAPI activityStartId (int activity, int id) = 0;
  virtual void QAPI activityStartId (int activity, int id, const char* description) = 0;
  virtual void QARGS_STACK activityStartId2 (int activity, int id, const char* description, ...) = 0;
  virtual void QARGS_STACK activityStartId3 (int activity, int id, const char* description, va_list ap) = 0;
  virtual void QARGS_STACK activityEventId (int activity, int id, const char* description, ...) = 0;

  virtual void QAPI activityStartId (int activity, int id, const cGuid& guid) = 0;
  virtual void QAPI activityEventId (int activity, int id, const cGuid& guid) = 0;

  // Use these if you want to apply the id when the activity is done
  virtual void QAPI activityDoneId (int id) = 0;
  virtual void QAPI activityDoneId (int id, const cGuid& guid) = 0;
  virtual void QAPI activityDoneId (int id, const char* description) = 0;
  virtual void QARGS_STACK activityDoneId2 (int id, const char* description, ...) = 0;
  virtual void QARGS_STACK activityDoneId3 (int id, const char* description, va_list ap) = 0;

  virtual void QAPI activityFailedId (int id) = 0;
  virtual void QAPI activityFailedId (int id, const cGuid& guid) = 0;
  virtual void QAPI activityFailedId (int id, const char* description) = 0;
  virtual void QARGS_STACK activityFailedId2 (int id, const char* description, ...) = 0;
  virtual void QARGS_STACK activityFailedId3 (int id, const char* description, va_list ap) = 0;

  virtual double QAPI activityDone2() = 0;  // return the duration in seconds
  virtual unsigned int QAPI ident() const = 0;
  virtual void QAPI setType (eTraceType type) = 0;

  // v3 extensions
  virtual void QAPI activityStart (int activity, const int* payload, int payloadLen) = 0;
  virtual void QAPI defineActivitySpec (int activitySpec, const std::string& displaySpec) = 0;
  virtual void QAPI defineActivityCol  (int activityCol, const std::string& displayName) = 0;
  virtual void QAPI defineActivitySpec (const std::string& name, const std::string& displaySpec) = 0;   // named sub-spec
};
//}}}
//{{{
class iActivityGroup
{
public:
  QDEFINE_SMARTPTR(iActivityGroup);
  virtual ~iActivityGroup() {}

  virtual iActivity::Ptr QAPI addSignal (const String name, iActivity::eTraceType type) = 0;
  virtual iActivity::Ptr QAPI addSignal (const String name) = 0;
  virtual iActivity::Ptr QARGS_STACK addSignal2 (iActivity::eTraceType type, const int bufSize, const Char * const format, ...) = 0;
  virtual iActivity::Ptr QARGS_STACK addSignal2 (const int bufSize, const Char * const format, ...) = 0;
  virtual String QAPI getName() const = 0;

  // v3 extensions
  virtual iActivityGroup::Ptr QAPI getGroup (const String name) = 0;        // nested groups
  virtual void QAPI defineActivitySpec (int activitySpec, const std::string& displaySpec) = 0;
  virtual void QAPI defineActivityCol  (int activityCol, const std::string& displayName) = 0;
  virtual void QAPI defineActivitySpec (const std::string& name, const std::string& displaySpec) = 0;
};
//}}}

//{{{
// =======================================================================================
// === iActivityPool ============= Base class for all iActivity providers ================
// =======================================================================================
class iActivityPool
{
public:
  QDEFINE_SMARTPTR(iActivityPool);
  virtual ~iActivityPool() {}

  virtual iActivity::Ptr getActivity() = 0;
  virtual iActivity::Ptr getActivity (unsigned int index) = 0;

  virtual void release (unsigned int index) = 0;
  virtual bool isBase (unsigned int index) const = 0;
};
//}}}
//{{{
class cActivityScoper
// =======================================================================================
// === iActivityPool ============= Base class for all iActivity providers ================
// =======================================================================================
{
public:
  //{{{
  cActivityScoper (iActivity::Ptr activity)
    : mActivity(activity),
      mParent(iActivityPool::Ptr()),
      mIndex(0)
  {}
  //}}}
  //{{{
  cActivityScoper (iActivityPool::Ptr parent)
    : mActivity(parent->getActivity()),
      mParent(parent),
      mIndex(mActivity->ident())
  {}
  //}}}
  //{{{
  cActivityScoper (unsigned int index, iActivityPool::Ptr parent)
    : mActivity(parent->getActivity(index)),
      mParent(parent),
      mIndex(index)
  {}
  //}}}
  //{{{
  ~cActivityScoper()
  {
    mActivity->activityDone();
    if (mParent.isValid())
      mParent->release (mIndex);
  }
  //}}}

  // extras to match old usage
  //{{{
  cActivityScoper (iActivity::Ptr activity, int action)
    : mActivity(activity),
      mParent(iActivityPool::Ptr()),
      mIndex(0)
  {
    mActivity->activityStart (action);
  }
  //}}}
  //{{{
  cActivityScoper (iActivity::Ptr activity, int action, int value)
    : mActivity(activity),
      mParent(iActivityPool::Ptr()),
      mIndex(0)
  {
    mActivity->activityStart (action, value);
  }
  //}}}
  //{{{
  cActivityScoper (iActivity::Ptr activity, int action, const char* comment)
    : mActivity(activity),
      mParent(iActivityPool::Ptr()),
      mIndex(0)
  {
    mActivity->activityStart (action, comment);
  }
  //}}}

  //{{{
  void release()        // release scope, so that this level can be re-used
  {
    mActivity->activityDone();
    if (mParent.isValid())
      mParent->release (mIndex);
    mParent.setNull();
  }
  //}}}
  //{{{
  bool isBase() const   // is the first level of issued iActivity
  {
    if (mParent.isValid())
      return mParent->isBase (mIndex);
    return true;
  }
  //}}}

  inline iActivity* operator -> () { return mActivity.getRaw(); }
  inline operator iActivity*()     { return mActivity.getRaw(); }
private:
  iActivity::Ptr mActivity;
  iActivityPool::Ptr mParent;
  unsigned int mIndex;        // either a map-index or an activity-ident
};
//}}}

// OLD STUFF
//{{{
class cActivityGuard
{
  public :
    cActivityGuard(const iActivity::Ptr & optionalSignal, const int activity, const char* const desc, ...) :
      m_OptionalSignal(optionalSignal.getRaw())
    {
      if (m_OptionalSignal) m_OptionalSignal->activityStart3(activity, desc, (va_list)&desc + _INTSIZEOF(desc));
    }
    ~cActivityGuard() { if (m_OptionalSignal) m_OptionalSignal->activityDone(); }
    void failed() { if (m_OptionalSignal) { m_OptionalSignal->activityFailed(); m_OptionalSignal = 0; } }
  private :
    iActivity * m_OptionalSignal;
};
//}}}
const unsigned int kActivityDetails_MaxNumDescChars = iActivity::kMaxPayloadBytes;  // 7 * sizeof(int);

//{{{
class cActivityTrigger
{
public:
  cActivityTrigger() : mCount(0), mEnable(0), mSignalIdent(0), mActivity(0) {}
  enum {
    eTriggerActivity=1,
    eTriggerDstId=2,    eTriggerDstType=4,    eTriggerDstArea=8,
    eTriggerSrcId=0x10, eTriggerSrcType=0x20, eTriggerSrcArea=0x40,
    eTriggerFlags=0x80,
    eTriggerFlagsMask = 0xffff,
    eTriggerRepeat=0x10000,      // restart the trigger again after it has triggered
    eTriggerNext=0x20000,        // another trigger follows
    eTriggerCallStack=0x100000   // trigger action - do a StackWalk dump
  };
  unsigned int mSignalIdent; // signal the triggers applies to
  double mDuration;  // activity duration longer than this (in Secs)
  int mActivity;    // includes the high-bits flags
  int mPayload[iActivity::kMaxPayload];
  unsigned int mEnable;      // contains the bit-flags of what-to-match (hence 0 = disabled) and what-to-action
  // keep this last so that it is the last item to be copied when enabling the trigger
  unsigned int mCount;       // number of matches before it triggers
};
//}}}
//{{{
class iDebugControlCallback
{
 public:
   QDEFINE_SMARTPTR(iDebugControlCallback);
   virtual ~iDebugControlCallback() {}

   virtual bool getState() const = 0;
   virtual void setState(bool on) const = 0;
};
//}}}
//}}}
//{{{  iFile
// =======================================================================================
// === iFile =============================================================================
// =======================================================================================

/*
This class is _not_ thread safe.
*/

class cFileError : public cRecoverable
{
  public :
    virtual ~cFileError() {}
    cFileError(const char * const  SrcFilename, const unsigned long SrcLineNumber, const Char * MsgFormat, ...) :
      cRecoverable(SrcFilename, SrcLineNumber, MsgFormat, (va_list)&MsgFormat + _INTSIZEOF(MsgFormat))
    {
    }
};

// Used by iFileManager::openFile/getAccess
const int fFileAccess_Read      = 1 << 0;
const int fFileAccess_Write     = 1 << 1;
const int fFileAccess_ReadWrite = fFileAccess_Read | fFileAccess_Write;

// Used by iFileManager::openFile/getOptions
const int fFileOption_Fast           = 1 << 0;   // On Win32: opens file for overlapped and unbuffered io
const int fFileOption_Temporary      = 1 << 1;   // On Win32: avoids writing to disk, keep in ram if possible
const int fFileOption_Hidden         = 1 << 2;
const int fFileOption_ShareReadWrite = 1 << 3;

class iFile
{
  public :

    QDEFINE_SMARTPTR(iFile);

    virtual ~iFile() {}

    virtual UInt64         QAPI getSize() const = 0;
    virtual cTime          QAPI getModificationTime() const = 0;
    virtual cDate          QAPI getModificationDate() const = 0;
    virtual String         QAPI getName() const = 0;
    virtual String         QAPI getPath() const = 0;
    virtual int            QAPI getAccess() const = 0;

    // Set current poisition to 0
    virtual void           QAPI seek(const UInt64 PositionInBytes) throw(cFileError) = 0;

    // Move current poisition to 0, read whole of file into a new cMemory, inc current position
    virtual cMemory::Ptr   QAPI read() throw(cFileError) = 0;

    // Read NumBytes of file at current position into a new cMemory, inc current position
    virtual cMemory::Ptr   QAPI read(const UInt64 NumBytes) throw(cFileError) = 0;

    // Read size of DestBytes at current position into DestBytes, inc current position
    virtual cMemoryView::Ptr QAPI read(cMemoryView::Ptr DestBytes) throw(cFileError) = 0;

    // Write contents of SrcBytes at current position, inc current position
    virtual void           QAPI write(cMemoryView::ConstPtr SrcBytes) throw(cFileError) = 0;

    // Write NumBytesToWrite from BytesToWrite at current position, inc current position
    virtual void           QAPI write(const UInt64 NumBytesToWrite, const char * const BytesToWrite) throw(cFileError) = 0;

    virtual void           QAPI rename(const String & NewFilename) throw(cFileError) = 0;

    // Block until any write operations have been put to disk
    virtual void           QAPI flushWrite() throw(cFileError) = 0;

    virtual String         QAPI getDrive() const = 0;
    virtual String         QAPI getExtension() const = 0;
    virtual String         QAPI getAbsFilename() const = 0;

    // Return fFileOption_XXX flags as specified when file was opened
    virtual int            QAPI getOptions() const = 0;

    class iExtendCallback
    {
      public :
        QDEFINE_SMARTPTR(iExtendCallback);
        virtual ~iExtendCallback() {}
        virtual void onExtendProgress(const unsigned int PercentageDone) throw(cAborted) = 0;
    };
    virtual void           QAPI extendFileFast(const UInt64 NewSize) throw(cNotSupported, cFileError, cAborted) = 0; // Only on XP as an admin
    virtual void           QAPI extendFile(const UInt64 NewSize, const UInt64 ValidDataLength, iExtendCallback::Ptr Callback) throw(cFileError, cAborted) = 0;

    virtual UInt64         QAPI getPosition() const = 0;

    // Read NumBytes at PositionInBytes into a new cMemory, "current position" is _not_ modified
    // This method is atomic
    virtual cMemory::Ptr   QAPI read(const UInt64 PositionInBytes, const UInt64 NumBytes) throw(cFileError) = 0;

    // Read size of DestBytes at PositionInBytes into DestBytes, "current position" is _not_ modified
    // This method is atomic
    virtual cMemoryView::Ptr QAPI read(const UInt64 PositionInBytes, cMemoryView::Ptr DestBytes) throw(cFileError) = 0;

    // Write contents of SrcBytes at PositionInBytes, "current position" is _not_ modified
    // This method is atomic
    virtual void           QAPI write(const UInt64 PositionInBytes, cMemoryView::ConstPtr SrcBytes) throw(cFileError) = 0;

    // Write NumBytesToWrite from BytesToWrite at PositionInBytes, "current position" is _not_ modified
    // This method is atomic
    virtual void           QAPI write(const UInt64 PositionInBytes, const UInt64 NumBytesToWrite, const char * const BytesToWrite) throw(cFileError) = 0;

    virtual unsigned int   QAPI getFastIOAlignment() const throw(cNotFound, cAccessDenied, cFileError) = 0;
private:
    // Disallowed
    cMemory::Ptr   QAPI read(const unsigned int NumBytes) throw(cFileError);
    cMemory::Ptr   QAPI read(const unsigned int PositionInBytes, const unsigned int NumBytes) throw(cFileError) ;
    cMemoryView::Ptr QAPI read(const unsigned int PositionInBytes, cMemoryView::Ptr DestBytes) throw(cFileError) ;
    void           QAPI write(const unsigned int NumBytesToWrite, const char * const BytesToWrite) throw(cFileError) ;
    void           QAPI write(const unsigned int PositionInBytes, cMemoryView::ConstPtr SrcBytes) throw(cFileError) ;
    void           QAPI write(const unsigned int PositionInBytes, const unsigned int NumBytesToWrite, const char * const BytesToWrite) throw(cFileError);
};
//}}}

//{{{  class cDevTestToggle
//! \brief Implements feature toggling behaviour for development testing
class cDevTestToggle
{
public:
  QKERNELEXEC cDevTestToggle(const char* const variableName, const Char* const description);
  ~cDevTestToggle() {}

  //! \brief This whole class is just a fancy boolean, so provide the operator for ease of use
  inline operator bool () const
  {
    return (*mToggleState);
  }

  //! \brief Prints all the DevTest toggles and their state to the log
  //! \return The number of DevTest toggles currently enabled
  static QKERNELEXEC size_t logAll();
  //! \brief Updates all the DevTest toggles with latest values from the registry
  static QKERNELEXEC void updateAll();

private:
  //! \brief Points to the current state of the toggle.
  mutable bool* mToggleState;

  //! \brief May contain the current state of the toggle.
  //!
  //! This boolean is only used for the first feature toggle under each name.
  //! For subsequent feature toggles mToggleState points to the mBoolStorage of the original.
  mutable bool mBoolStorage;
};
//}}}
//{{{  QDEFINE_DEVTEST_TOGGLE(ToggleName, ToggleDescription)
//! \brief Macro to define DevTest feature toggles.
#define QDEFINE_DEVTEST_TOGGLE(ToggleName, ToggleDescription) \
  namespace devtest { \
    __declspec(selectany) ::vfs::cDevTestToggle ToggleName(#ToggleName, ToggleDescription); \
  }
//}}}
//{{{  QDEFINE_DEVTEST_FORCE_ON(ToggleName, ToggleDescription)
//! \brief Macro to force DevTest on
#define QDEFINE_DEVTEST_FORCE_ON(ToggleName, ToggleDescription) \
  namespace devtest { \
    __declspec(selectany) extern const bool ToggleName = true; \
}
//}}}
// --- End of Namespace ------------------------------------------------------------------
}
#endif
