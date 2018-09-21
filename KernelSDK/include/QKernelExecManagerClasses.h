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


#if !defined(__744F591309D245468FFB7F7AEB29F679)
#define __744F591309D245468FFB7F7AEB29F679
/*
**  
*/
#include "QKernelBase.h"
#include "QKernelExecEnviron.h"
#include "QKernelExecObjectClasses.h"

#if defined(_MSC_VER)
    #pragma pack(push, 8)
#endif

class cLogManager;

namespace vfs
{
//{{{
// =======================================================================================
// === iManager ==========================================================================
// =======================================================================================
/*
*/
class iManager : public iFactory::iInstance
{
  public :

    QDEFINE_CLASS_IFACE(iManager, L"{C2E85E34-341B-40EE-A8BE-3855AF8EB277} // iManager", iFactory::iInstance);

    virtual void QAPI logSummary() = 0;
    virtual void QAPI logDetailedSummary() = 0;
};
//}}}
//{{{
// =======================================================================================
// === iSystemInfo =======================================================================
// =======================================================================================
/*
*/
class iSystemInfo : public iManager
{
  public :

    QDEFINE_SINGLETON_IFACE(iSystemInfo, L"{A01CBC33-74AB-4e5f-817E-A9AD976EEAE7} // iSystemInfo", iManager);

    virtual unsigned int QAPI getProcessorHz() const = 0;
    virtual unsigned int QAPI getNumProcessors() const = 0;
    virtual cTime QAPI getCurrentUniversalTime() const = 0;
    virtual cDate QAPI getCurrentUniversalDate() const = 0;
    virtual cTime QAPI getCurrentLocalTime() const = 0;
    virtual cDate QAPI getCurrentLocalDate() const = 0;
    virtual cTime QAPI getElapsedSinceStart() const = 0;
    virtual UInt64 QAPI getMillisecsSinceStart() const = 0;

    virtual String QAPI getCmdLineOption(const String & Option) const throw(cNotFound) = 0;
    virtual UInt64 QAPI getTicksSinceStart(UInt64 * const TickFreqHz) const = 0;
    virtual String QAPI getBootFolder() const = 0;
    virtual String QAPI getProductDeveloperName() const = 0;
    virtual String QAPI getProductName() const = 0;
    virtual String QAPI getProductVersion() const = 0;
    virtual String QAPI getProductSettingsGroup() const = 0;
    virtual cPtr<iModuleInstance> QAPI getHostModuleContext() const = 0;
    virtual String QAPI getHostName() const = 0;
    virtual cVersion QAPI getHostVersion() const = 0;
    virtual void QAPI setCurrentUserName(const String& username) = 0;         //there are rules about when this can be called - see IanC
    virtual void QAPI setCurrentUserPassword(const String& userpassword) = 0; //there are rules about when this can be called - see IanC
    virtual String QAPI getCurrentUserName() const = 0;
    virtual String QAPI getCurrentUserPassword() const = 0;

    virtual double QAPI getSecsSinceStart() const = 0;
    virtual double QAPI convertToSec(UInt64 ticks) const = 0;
    virtual cTime QAPI convertToTime(UInt64 ticksSinceStart) const = 0;

    virtual UInt64 QAPI getTicksSinceStart() const = 0;
    virtual UInt64 QAPI getTicksFreq() const = 0;
    virtual UInt64 QAPI convertToMillisec (UInt64 ticks) const = 0;
    virtual UInt64 QAPI convertToMicrosec (UInt64 ticks) const = 0;
    virtual String QAPI toTimeString (UInt64 ticks) const = 0;     // including microsecs

    virtual String QAPI getPhysicalMemoryInMegaBytes() const = 0;
    virtual String QAPI getOperatingSystem() const = 0;
    virtual String QAPI getProcessorDescription() const = 0;
    virtual String QAPI getSystemArchitecture() const = 0;
    virtual String QAPI getApplicationArchitecture() const = 0;

    virtual enum eProductId QAPI getProductId() const throw(cNotFound) = 0;
    virtual enum eProductId QAPI parseProductName (const String& strProduct) const throw(cNotFound) = 0;

    virtual Char QAPI getProductVersionLetter() const = 0;
    virtual cVersion QAPI getProductVersionNumber() const = 0;
    virtual String QAPI getProductBuildId() const = 0;
};
//}}}
//{{{
// =======================================================================================
// === ivfsEnvironment ===============================================================
// =======================================================================================
/*
*/
class ivfsEnvironment : public iManager
{
  public :

    QDEFINE_SINGLETON_IFACE(ivfsEnvironment, L"{CB4709D0-F7FD-444f-8EDC-F351784B44EB} // ivfsEnvironment", iManager);

    virtual String QAPI getUserFolder() const = 0;
    virtual String QAPI getLocalMachineRootFolder() const = 0;
    virtual String QAPI getRelativeRootFolder() const = 0;
};
//}}}

//{{{  class iLogManager
// =======================================================================================
// === iLogManager =======================================================================
// =======================================================================================

/*
Manages the output of logging information to log drivers. The manager will open a DOS style
console window where all the logging output will go to. The output will additionally be
sent to all the log drivers that were found.

<b>Accessing the Single Instance</b>

This class is a singleton, all Kernel classes with a name in the form "iXManager" are
singletons. This means that there can only ever be one instance of the class per thread
at a time. Refer to the "Singletons" section in the "Kernel Architecture" chapter for a
more detailed explanation.

<b>Thread Safety</b>

All the methods within this class are thread safe.
*/
class cLogIndentGuard;
class cLogSuppressGuard;
class cLogIndentSOSGuard;

class iLogManager : public iManager
{
public:
  QDEFINE_SINGLETON_IFACE(iLogManager, L"{EA6ADBD4-4DF1-4C99-B709-32EF2A83B921} // iLogManager", iManager);

//{{{
// ---------------------------------------------------------------------------------------
/*
Outputs a formatted message with prefix.

<b>Use the QMSG macro instead of this method directly.</b>
Although this method can be called directly it is recommended that you use the QMSG
macro instead. This macro inserts low overhead code that will filter logging based on
per module catgories and whether a modules output has been supressed.

  {
    iLogManager::singleton().writeMsg(L"Passed int %d to a function", 5);
    // Or
    QMSG((0, L"Passed int %d to a function", 5));
  }

<b>Thread Safety</b>

If the main Kernel thread calls this function then the output will respect the current
indent level and suppresion. Other threads will not be indented and the output will be
highlighted and prefixed with the thread ID.
*/
  static QKERNELEXEC void QAPI_VARGS bufferMsg (const unsigned long category, const Char* const format, ...);
  static QKERNELEXEC void QAPI_VARGS bufferSOS (const Char* const format, ...);
  static QKERNELEXEC void QAPI_VARGS bufferError (const Char* const format, ...);
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
Outputs a formatted message with no prefix or indent, highlighted in red. Message is always
logged regardeless of category or suppressed modules.

<b>Use the QMSG macro instead of this method directly.</b>
Although this method can be called directly it is recommended that you use the QSOS
macro instead.

  {
    iLogManager::singleton().writeSosMsg(L"Passed int %d to a function", 5);
    // Or
    QSOS((L"Passed int %d to a function", 5));
  }

<b>Thread Safety</b>

If the main Kernel thread calls this function then the output will respect the current
indent level and suppresion. Other threads will not be indented and the output will be
highlighted and prefixed with the thread ID.
*/
  static QKERNELEXEC void QAPI_VARGS bufferTrace (const Char* const format, ...);
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
Inquires whether a message output at the moment by a particular category in the current
module would be visible. Will be true if the log is currently 'injecting', or the category
is set to 'expand' or 'inject' and the log is not currently 'suppressed'.

<b>Thread Safety</b>

It is safe to use this macro in another thread.
*/
  static QKERNELEXEC bool QAPI isCategoryVisible (const unsigned long category);
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
<b>Thread Safety</b>

It is safe to use this macro in another thread.
*/
  static QKERNELEXEC void QAPI_VARGS bufferBreak (const Char* const format, ...);
//}}}

  virtual void QAPI INTERNAL_USE_ONLY_createLogIndentGuard (cLogIndentGuard* const guard, const unsigned int category, const Char* const format, const va_list& args) = 0;
  virtual void QAPI INTERNAL_USE_ONLY_destroyLogIndentGuard (cLogIndentGuard* const guard) = 0;
  virtual void QAPI INTERNAL_USE_ONLY_createLogSuppressGuard (cLogSuppressGuard* const guard) = 0;
  virtual void QAPI INTERNAL_USE_ONLY_destroyLogSuppressGuard (cLogSuppressGuard* const guard) = 0;
  virtual void QAPI INTERNAL_USE_ONLY_createLogIndentSOSGuard (cLogIndentSOSGuard* const guard, const Char* const format, const va_list& args) = 0;
  virtual void QAPI INTERNAL_USE_ONLY_destroyLogIndentSOSGuard (cLogIndentSOSGuard* const guard) = 0;

  virtual void QAPI flushBuffer() = 0;

  virtual void QAPI refreshModuleFlags() = 0;

  typedef unsigned long Category;
};
//}}}
//{{{
// =======================================================================================
// === cLogIndentGuard ===================================================================
// =======================================================================================
/*
If the main Kernel thread calls this function then the output will respect the current
indent level and suppresion. Other threads will not be indented and the output will be
highlighted and prefixed with the thread ID.
*/
class cLogIndentGuard
{
  public :
    cLogIndentGuard(const unsigned int Category, const Char * const Format, ...)
    {
      if (iLogManager::isSingletonConstructed()) iLogManager::singleton().INTERNAL_USE_ONLY_createLogIndentGuard(this, Category, Format, (va_list)&Format + _INTSIZEOF(Format));
    }
    ~cLogIndentGuard()
    {
      if (iLogManager::isSingletonConstructed()) iLogManager::singleton().INTERNAL_USE_ONLY_destroyLogIndentGuard(this);
    }
    unsigned long       m_PrevCtg;
    void *              m_PrevModuleAddr;
};
//}}}
//{{{
// =======================================================================================
// === cLogIndentSOSGuard ================================================================
// =======================================================================================
/*
If the main Kernel thread calls this function then the output will respect the current
indent level and suppresion. Other threads will not be indented and the output will be
highlighted and prefixed with the thread ID.
*/
class cLogIndentSOSGuard
{
  public :
    cLogIndentSOSGuard(const Char * const Format, ...)
    {
      if (iLogManager::isSingletonConstructed()) iLogManager::singleton().INTERNAL_USE_ONLY_createLogIndentSOSGuard(this, Format, (va_list)&Format + _INTSIZEOF(Format));
    }
    ~cLogIndentSOSGuard()
    {
      if (iLogManager::isSingletonConstructed()) iLogManager::singleton().INTERNAL_USE_ONLY_destroyLogIndentSOSGuard(this);
    }
    unsigned long m_PrevCtg;
    void *        m_PrevModuleAddr;
};
//}}}
//{{{
// =======================================================================================
// === cLogSuppressGuard =================================================================
// =======================================================================================
/*
If the main Kernel thread calls this function then the output will respect the current
indent level and suppresion. Other threads will not be indented and the output will be
highlighted and prefixed with the thread ID.
*/
class cLogSuppressGuard
{
  public :
    cLogSuppressGuard()
    {
      if (iLogManager::isSingletonConstructed()) iLogManager::singleton().INTERNAL_USE_ONLY_createLogSuppressGuard(this);
    }
    ~cLogSuppressGuard()
    {
      if (iLogManager::isSingletonConstructed()) iLogManager::singleton().INTERNAL_USE_ONLY_destroyLogSuppressGuard(this);
    }
};
//}}}

//{{{  class iThreadManager
// =======================================================================================
// === iThreadManager ====================================================================
// =======================================================================================

enum eThreadLogging
{
  eThreadLogging_None,
  eThreadLogging_Expanded,
  eThreadLogging_Collapsed
};

class iThreadManager : public iManager
{
  public:

    QDEFINE_SINGLETON_IFACE(iThreadManager, L"{6812C6A4-5796-47FF-981E-CD683EC621D6} // iThreadManager", iManager);

    /*
    Returns a pointer the current threads iBasicThread class. Result is undefined if thread is not
    derived from iBasicThread.
    */
    virtual iBasicThread * QAPI getCurrentThread() = 0;

    /*
    Exception safe way of starting and stopping threads using a guard object. Hold onto the result
    for as long as you want your thread to run.
    */
    virtual cGuard::ConstPtr QAPI startThread(const iBasicThread::Ptr & thread) = 0;

    /*
    Is the current thread the one that processes messages and updates the display.
    */
    virtual bool QAPI isMainThread() = 0;

    /*
    Returns platform specific thread identifier.
    */
    virtual unsigned int QAPI getCurrentThreadId() = 0;

    //{{{
    class iBackgroundActivityCallback : public iGuarded
    {
      public:
        QDEFINE_SMARTPTR(iBackgroundActivityCallback);
        virtual void suspend() = 0; // Return when you are suspended
        virtual void unsuspend() = 0;
    };
    //}}}
    virtual cGuard::ConstPtr QAPI addBackgroundActivityCallback(iBackgroundActivityCallback::Ptr NewCallback) = 0;
    virtual cGuard::ConstPtr QAPI suspendBackgroundActivity() = 0;

    /*
    Turns the current thread into an iThread.
    */
    virtual iBasicThread::Ptr QAPI promoteCurrentThread(const String & name) = 0;

    //{{{
    class iThreadCallback : public iGuarded
    {
      public:
        QDEFINE_SMARTPTR(iThreadCallback);
        virtual void onThreadStarted(const iBasicThread::Ptr & thread) = 0;
        virtual void onThreadStopping(const iBasicThread::Ptr & thread) = 0;
    };
    //}}}
    virtual cGuard::ConstPtr QAPI addThreadCallback(const iThreadCallback::Ptr & newCallback) = 0;
    virtual iBasicThread::Ptr QAPI INTERNAL_USE_ONLY_createBasicThreadImpl(iBasicThread * const IfaceObject, cRefCount * const IfaceObjectRefCount, const String & Name, const iBasicThread::ePriority WhichPriority) = 0;
    virtual iBasicThread::Ptr QAPI INTERNAL_USE_ONLY_createBasicThreadImpl(iBasicThread * const IfaceObject, cRefCount * const IfaceObjectRefCount, const String & Name, const iBasicThread::ePriority WhichPriority, const unsigned int InitialStackSize, const eThreadLogging Logging) = 0;
    virtual void QAPI sleep(const unsigned int delayMs) = 0;

#ifdef _WIN32
    /*
    On windows, these functions allow you to get the thread ID of the original ("WinMain") windows
    thread. This is used to then PostThreadMessages containing code to be executed in that thread's
    context. Needed for compatibility with single-threaded COM.
    */
    virtual void deprecated0() = 0;
    virtual unsigned int getOrigWinThreadId() = 0;
#endif

    virtual iMsgQueue::Ptr QAPI createAsyncMsgQueue(const String & name, const iBasicThread::ePriority priority) = 0;
    virtual iCircularMsgQueue::Ptr QAPI createCircularMsgQueueThread(const String & name, const iBasicThread::ePriority priority) = 0;
    virtual iMsgQueue::Ptr QAPI INTERNAL_USE_ONLY_createMsgQueue() = 0;

#if kKernelSDKIfaceVersion_Int < 0400000
    virtual iThread::Ptr QAPI createMsgQueueThread(const String & Name, const iThread::ePriority WhichPriority) = 0;
#endif
};
//}}}
//{{{
// =======================================================================================
// === cBasicThread ======================================================================
// =======================================================================================

class cBasicThread : public iBasicThread, public cRefCount
{
  public:

    QDEFINE_SMARTPTR(cBasicThread);

    // iBasicThread
    virtual void QAPI signalLoopStop() = 0;
    virtual iWaitable::eWakeupReason QAPI waitLoopComplete(const unsigned long timeoutMs) { return m_Pimpl->waitLoopComplete(timeoutMs); }
    virtual void QAPI waitLoopCompleteForever() { m_Pimpl->waitLoopCompleteForever(); }
    virtual void QAPI stopLoop() { m_Pimpl->stopLoop(); }
    virtual void QAPI startLoop() { m_Pimpl->startLoop(); }
    virtual String QAPI getName() const { return m_Pimpl->getName(); }
    virtual unsigned long QAPI getThreadID() const { return m_Pimpl->getThreadID(); }
    virtual void QAPI setPriority(const ePriority NewPriority) { return m_Pimpl->setPriority(NewPriority); }
    virtual ePriority QAPI getPriority() const { return m_Pimpl->getPriority(); }
    virtual eExitState QAPI getExitState(String * const desc) const  { return m_Pimpl->getExitState(desc); }

    // cBasicThread
    cBasicThread(const String & name)
    {
      const_cast<iBasicThread::Ptr &>(m_Pimpl) = iThreadManager::singleton().INTERNAL_USE_ONLY_createBasicThreadImpl(this, this, name, ePriority_Normal);
    }
    cBasicThread(const String & name, const ePriority priority)
    {
      const_cast<iBasicThread::Ptr &>(m_Pimpl) = iThreadManager::singleton().INTERNAL_USE_ONLY_createBasicThreadImpl(this, this, name, priority);
    }
    cBasicThread(
      const String & name,
      const ePriority priority,
      const unsigned int initialStackSize, // Initial stack size must be > 0. If you don't care about the size of the stack use the other
                                           //  constructor instead. The stack will grow to accomodate your threads demands.
      const eThreadLogging isLogging)
    {
      const_cast<iBasicThread::Ptr &>(m_Pimpl) = iThreadManager::singleton().INTERNAL_USE_ONLY_createBasicThreadImpl(this, this, name, priority, initialStackSize, isLogging);
    }

  protected :

    // iBasicThread
    virtual void QAPI runLoop() = 0;

  public :

    const iBasicThread::Ptr m_Pimpl;
};
//}}}
//{{{
// =======================================================================================
// === cThread ===========================================================================
// =======================================================================================

class cThread : public iThread, public cRefCount
{
  public:

    QDEFINE_SMARTPTR(cThread);

    // iBasicThread
    virtual void QAPI signalLoopStop() = 0;
    virtual iWaitable::eWakeupReason QAPI waitLoopComplete(const unsigned long timeoutMs) { return m_Pimpl->waitLoopComplete(timeoutMs); }
    virtual void QAPI waitLoopCompleteForever() { m_Pimpl->waitLoopCompleteForever(); }
    virtual void QAPI stopLoop() { m_Pimpl->stopLoop(); }
    virtual void QAPI startLoop() { m_Pimpl->startLoop(); }
    virtual String QAPI getName() const { return m_Pimpl->getName(); }
    virtual unsigned long QAPI getThreadID() const { return m_Pimpl->getThreadID(); }
    virtual void QAPI setPriority(const ePriority NewPriority) { return m_Pimpl->setPriority(NewPriority); }
    virtual ePriority QAPI getPriority() const { return m_Pimpl->getPriority(); }
    virtual eExitState QAPI getExitState(String * const desc) const  { return m_Pimpl->getExitState(desc); }

    // iThread
    virtual void QAPI postMessage(const iMsgQueue::iMsg::Ptr & NewMessage) { m_MsgQueue->postMsg(NewMessage); }
    virtual unsigned int /* num culled */ QAPI clearAllMessages() { return m_MsgQueue->clearMsgs(); }
    virtual unsigned int QAPI getNumMessages() const { return m_MsgQueue->countMsgs(); }
    virtual void QAPI postMessageFront(const iMsgQueue::iMsg::Ptr & NewMessage) { m_MsgQueue->postMsgFront(NewMessage); }
    virtual bool /* visited all? see fMsgVisitor_Abort */ visitMsgsOldestFirst(iMsgQueue::iMsgVisitor & visitor) { return m_MsgQueue->visitMsgsOldestFirst(visitor); }
    virtual bool /* visited all? see fMsgVisitor_Abort */ visitMsgsNewestFirst(iMsgQueue::iMsgVisitor & visitor) { return m_MsgQueue->visitMsgsNewestFirst(visitor); }
#if kKernelSDKIfaceVersion_Int < 0400000
    virtual unsigned int /* num culled */ QAPI clearMessages(const type_info& messageType) { return m_MsgQueue->clearMessages(messageType); }
    virtual unsigned int /* num culled */ QAPI clearMessages(const type_info& messageType, const unsigned int leavingMinNumMsgsIntact) { return m_MsgQueue->clearMessages(messageType, leavingMinNumMsgsIntact); }
    virtual unsigned int /* num culled */ clearNewestMessagesIfMoreThan(const unsigned int maxNumMsgs) { return m_MsgQueue->clearNewestMessagesIfMoreThan(maxNumMsgs); }
    virtual unsigned int /* num culled */ clearOldestMessagesIfMoreThan(const unsigned int maxNumMsgs) { return m_MsgQueue->clearOldestMessagesIfMoreThan(maxNumMsgs); }
    // iMsgQueue
    virtual void QAPI postMsg(const iMsg::Ptr & newMsg) { m_MsgQueue->postMsg(newMsg); }
    virtual unsigned int /* num culled */ QAPI clearMsgs() { return m_MsgQueue->clearMsgs(); }
    virtual unsigned int QAPI countMsgs() const { return m_MsgQueue->countMsgs(); }
    virtual bool QAPI isMsgWaiting() const { return m_MsgQueue->isMsgWaiting(); }
    virtual void QAPI waitMsgForever() const { m_MsgQueue->waitMsgForever(); }
    virtual bool QAPI waitMsg(const unsigned int timeoutMs) const { return m_MsgQueue->waitMsg(timeoutMs); }
    virtual bool QAPI getMsg(iMsg::Ptr * const resultMsg) { return m_MsgQueue->getMsg(resultMsg); }
    virtual void QAPI postMsgFront(const iMsg::Ptr & newMsg) { m_MsgQueue->postMsgFront(newMsg); }
#endif

    // cThread
    cThread(const String & name)
    {
      const_cast<iBasicThread::Ptr &>(m_Pimpl) = iThreadManager::singleton().INTERNAL_USE_ONLY_createBasicThreadImpl(this, this, name, ePriority_Normal);
      const_cast<iMsgQueue::Ptr &>(m_MsgQueue) = iThreadManager::singleton().INTERNAL_USE_ONLY_createMsgQueue();
    }
    cThread(const String & name, const ePriority priority)
    {
      const_cast<iBasicThread::Ptr &>(m_Pimpl) = iThreadManager::singleton().INTERNAL_USE_ONLY_createBasicThreadImpl(this, this, name, priority);
      const_cast<iMsgQueue::Ptr &>(m_MsgQueue) = iThreadManager::singleton().INTERNAL_USE_ONLY_createMsgQueue();
    }
    cThread(
      const String & name,
      const ePriority priority,
      const unsigned int initialStackSize, // Initial stack size must be > 0. If you don't care about the size of the stack use the other
                                           //  constructor instead. The stack will grow to accomodate your threads demands.
      const eThreadLogging isLogging)
    {
      const_cast<iBasicThread::Ptr &>(m_Pimpl) = iThreadManager::singleton().INTERNAL_USE_ONLY_createBasicThreadImpl(this, this, name, priority, initialStackSize, isLogging);
      const_cast<iMsgQueue::Ptr &>(m_MsgQueue) = iThreadManager::singleton().INTERNAL_USE_ONLY_createMsgQueue();
    }

  protected :

    // iBasicThread
    virtual void QAPI runLoop() = 0;

    // iThread
    virtual bool QAPI isMessageWaiting() const { return m_MsgQueue->isMsgWaiting(); }
    virtual void QAPI waitMessageForever() const { m_MsgQueue->waitMsgForever(); }
    virtual bool QAPI waitMessage(const unsigned int timeoutMs) const { return m_MsgQueue->waitMsg(timeoutMs); }
    virtual bool QAPI getMessage(iMsgQueue::iMsg::Ptr * const optionalResult /* could be null! */) { return m_MsgQueue->getMsg(optionalResult); }
    //{{{
    virtual iMsgQueue::iMsg::Ptr QAPI getMessage() throw(cNotFound)
    {
      iMsgQueue::iMsg::Ptr result;
      getMessage(&result);
      if (!result.isValid()) throw cNotFound(__FILE__, __LINE__, L"Message queue is empty");
      return result;
    }
    //}}}

  public :

    const iBasicThread::Ptr m_Pimpl;
    const iMsgQueue::Ptr m_MsgQueue;
};
//}}}
//{{{  class iExceptionManager
// =======================================================================================
// === iExceptionManager =================================================================
// =======================================================================================
/*
*/

// Used by iExceptionManager::fatal
const int fFatal_ShowExitCode    = 1 << 0;
const int fFatal_ShowMsg         = 1 << 1;
const int fFatal_ShowSrcLocation = 1 << 2;
const int fFatal_ShowStack       = 1 << 3;
const int fFatal_ShowAll         = (fFatal_ShowExitCode | fFatal_ShowMsg | fFatal_ShowSrcLocation | fFatal_ShowStack);
const int fFatal_IsAnError       = 1 << 4;

// Used by iExceptionManager::logCallStack
const int fStack_ShowAddress     = 1 << 0;
const int fStack_ShowFile        = 1 << 1;
const int fStack_ShowFunction    = 1 << 2;
const int fStack_ShowModule      = 1 << 3;
const int fStack_ShowOffset      = 1 << 4;
const int fStack_ShowParams      = 1 << 5;
const int fStack_ShowRegisters   = 1 << 6;
const int fStack_ShowDebug       = 1 << 10;  // show additional QTRACE debug
const int fStack_ShowImmediate   = 1 << 11;  // don't post the result a separate logging thread
const int fStack_ShowAll         = (fStack_ShowAddress | fStack_ShowFile | fStack_ShowFunction | fStack_ShowModule | fStack_ShowOffset | fStack_ShowParams | fStack_ShowRegisters);
const int fStack_ShowBasic       = (fStack_ShowFile | fStack_ShowFunction);

//{{{
// A thread can be told to throw a cAttemptedFatal instead of crash when QVERIFY is called,
// see enableExceptionOnFatal() below.
class cAttemptedFatal : public cRecoverable
{
  public :
    virtual ~cAttemptedFatal() {}
    cAttemptedFatal(const char * const  SrcFilename, const unsigned long SrcLineNumber, const Char * MsgFormat, ...) :
      cRecoverable(SrcFilename, SrcLineNumber, MsgFormat, (va_list)&MsgFormat + _INTSIZEOF(MsgFormat))
    {
    }
};
//}}}

class iExceptionManager : public iManager
{
  public:

    QDEFINE_SINGLETON_IFACE(iExceptionManager, L"{63ED1E30-890B-4AB8-B713-A9B0A137CB3D} // iExceptionManager", iManager);

    //{{{
    class iFatalCallback : public iGuarded
    {
      public :
        QDEFINE_SMARTPTR(iFatalCallback);
        virtual String  QAPI getDescription() const = 0;
        virtual void    QAPI onFatal() = 0;
    };
    //}}}
    virtual cGuard::ConstPtr QAPI addFatalCallback(iFatalCallback::Ptr callbackToAdd) = 0;
    __declspec(noreturn) virtual void presentFatal(const char * const SrcFilename, const unsigned long SrcLineNumber, const Char * const MsgFormat, ...) = 0;
    //{{{
    class iWarningWaitCallback : public iGuarded
    {
      public :
        QDEFINE_SMARTPTR(iWarningWaitCallback);
        virtual void QAPI onChoice(const unsigned int ChoiceNum, const String & Choice) = 0;
    };
    //}}}
    //{{{
    class iWarningCallback : public iGuarded
    {
      public :
        QDEFINE_SMARTPTR(iWarningCallback);
        virtual void QAPI onWarning(const String & Msg) = 0;
        virtual void QAPI onWarningWait(
          iWarningWaitCallback::Ptr     FeebackCallback,
          const String &                Msg,
          const std::vector<String>     Choices) = 0;
    };
    //}}}
    virtual cGuard::ConstPtr QAPI addWarningCallback(const iWarningCallback::Ptr & CallbackToAdd) = 0;
    virtual void QARGS_STACK deprecated_presentWarning(const cRecoverable & Reason, const Char * const MsgFormat, ...) = 0;
    virtual void QAPI deprecated_logAllThreadsCallStacks() const = 0;
    virtual void QAPI logCallStack() const = 0;                                                     //!< logs the current call stack (as QMSG) - fStack_ShowBasic
    virtual void QAPI logCallStack( unsigned int displayFlags ) const = 0;                          //!< logs call stack with control of displayed info
    //virtual void QAPI logCallStack( unsigned int displayFlags, int startingAtLevelNum ) const = 0;  //!< logs call stack, skipping the initial entries
    //virtual void QAPI logCallStack( unsigned int displayFlags, const String& afterFunc ) const = 0; //!< logs call stack, skipping entries prior to a function
    virtual void QAPI deprecated_logAllModules() = 0;

    //{{{
    class cFatalDiagnostics
    {
      public :
        //{{{
        cFatalDiagnostics(const unsigned int ExitCode, const char * const SrcFilename, const unsigned long SrcLineNumber, const Char * const MsgHeading, const Char * const MsgFormat, va_list MsgParams) :
         m_ExitCode(ExitCode), m_SrcFilename(widen(SrcFilename)), m_SrcLineNumber(SrcLineNumber), m_MsgHeading(MsgHeading)
        {
          Char tmp[512];
          _vsnwprintf(tmp, sizeof(tmp) / sizeof(Char), MsgFormat, MsgParams);
          m_Msg = tmp;
        }
        //}}}
        //{{{
        cFatalDiagnostics(const unsigned int ExitCode, const char * const SrcFilename, const unsigned long SrcLineNumber, const Char * const MsgHeading, const Char * const MsgFormat, ...) :
          m_ExitCode(ExitCode), m_SrcFilename(widen(SrcFilename)), m_SrcLineNumber(SrcLineNumber), m_MsgHeading(MsgHeading)
        {
          va_list MsgParams;
          va_start(MsgParams, MsgFormat);
          Char tmp[512];
          _vsnwprintf(tmp, sizeof(tmp) / sizeof(Char), MsgFormat, MsgParams);
          va_end(MsgParams);
          m_Msg = tmp;
        }
        //}}}
        // Special ctor to support QFATAL macro
        //{{{
        cFatalDiagnostics(const char * const SrcFilename, const unsigned long SrcLineNumber, const Char * const MsgFormat, ...) :
          m_ExitCode(EXIT_FAILURE), m_SrcFilename(widen(SrcFilename)), m_SrcLineNumber(SrcLineNumber), m_MsgHeading(L"Fatal Error")
        {
          va_list MsgParams;
          va_start(MsgParams, MsgFormat);
          Char tmp[512];
          _vsnwprintf(tmp, sizeof(tmp) / sizeof(Char), MsgFormat, MsgParams);
          va_end(MsgParams);
          m_Msg = tmp;
        }
        //}}}
        unsigned int  m_ExitCode;
        String        m_SrcFilename;
        unsigned long m_SrcLineNumber;
        String        m_MsgHeading;
        String        m_Msg;
    };
    //}}}
    __declspec(noreturn) virtual void QAPI fatal(const unsigned int Flags, const cFatalDiagnostics & Diagnostics) = 0;
    virtual void QARGS_STACK presentWarning(const Char * const MsgFormat, ...) = 0;
    /*
    Display msg in warning style and wait for user to aknowledge it
    */
    virtual void QARGS_STACK presentWarningWait(const Char * const MsgFormat, ...) = 0;
    /*
    Display msg in warning style  and wait for user to aknowledge it using choices provided
    */
    virtual void QARGS_STACK presentWarningWait(
      const iWarningWaitCallback::Ptr & FeedbackCallback,
      const std::vector<String> &   FeedbackChoices,
      const Char * const            MsgFormat,
      ...) = 0;
    /*
    Display msg in information style
    */
    virtual void QARGS_STACK presentInformation(const Char * const MsgFormat, ...) = 0;
    virtual void QARGS_STACK deprectated_presentInformation(const cRecoverable & Reason, const Char * const MsgFormat, ...) = 0;

    //{{{
    class iInformationCallback : public iGuarded
    {
      public :
        QDEFINE_SMARTPTR(iInformationCallback);
        virtual void QAPI onInformation(const String & Msg) = 0;
    };
    //}}}
    virtual cGuard::ConstPtr QAPI addInformationCallback(const iInformationCallback::Ptr & callbackToAdd) = 0;
    __declspec(noreturn) virtual void QARGS_STACK reboot(const Char * const MsgFormat, ...) = 0;

    // Internal use only!
    virtual void QAPI INTERNAL_USE_ONLY_report(const char * const  Filename, const unsigned long Line, const Char * const  MsgFormat, ...) = 0;

    //{{{
    class iStatusCallback : public iGuarded
    {
      public :
        QDEFINE_SMARTPTR(iStatusCallback);
        virtual void QAPI onStatus(const String & msg) = 0;
    };
    //}}}
    virtual cGuard::ConstPtr QAPI addStatusCallback(const iStatusCallback::Ptr & callbackToAdd) = 0;
    virtual void QARGS_STACK presentStatus(const Char * const MsgFormat, ...) = 0;
    // Current thread will throw a cFatal instead of crash
    virtual cGuard::ConstPtr QAPI enableExceptionOnFatal() = 0;

    virtual String QAPI getDebugHeader( int displayFlags ) const = 0;                        //!< returns a column header string to match getDebugLine calls
    virtual String QAPI getDebugLine( const void* const addr, int displayFlags ) const = 0;  //!< returns the src file & routine at this address (same as logCallStack)
    virtual void QAPI loadAllSymbols() const = 0;  // warning: may take a while
};
//}}}

//{{{
// =======================================================================================
// === iMemoryManager ====================================================================
// =======================================================================================

class iMemoryManager : public iManager
{
  public:

    QDEFINE_SINGLETON_IFACE(iMemoryManager, L"{FF590787-EC7E-4BE6-BFD3-48F37E8E8848} // iMemoryManager", iManager);

    __declspec(deprecated) virtual bool QAPI setHeapTracking(const bool IsTracking) = 0;
    #if defined(_DEBUG)
      #define CRTMEMSTATE _CrtMemState
    #else
      #define CRTMEMSTATE int
    #endif
    #if defined(_WINDOWS_) // is windows header included?
      __declspec(deprecated) virtual void QAPI snapshotHeap(CRTMEMSTATE * const NewSnapshot) = 0;
    #else
      __declspec(deprecated) virtual void QAPI snapshotHeap(void * const NewSnapshot) = 0;
    #endif
    #if defined(_WINDOWS_) // is windows header included?
      __declspec(deprecated) virtual bool QAPI sosHeapDifferences(const CRTMEMSTATE * const PrevSnapshot) = 0;
    #else
      __declspec(deprecated) virtual bool QAPI sosHeapDifferences(const void * const PrevSnapshot) = 0;
    #endif

    virtual iMemoryAllocator::Ptr QAPI createFixedSizeAllocator(
      const unsigned int NumBytesPerObject,
      const unsigned int NumObjectsPerPool,
      const unsigned int Alignment) throw(cNoRoom) = 0;

    // Defaults to best alignment for processor
    virtual iMemoryAllocator::Ptr QAPI createFixedSizeAllocator(
      const unsigned int NumBytesPerObject,
      const unsigned int NumObjectsPerPool) throw(cNoRoom) = 0;

    // Typically 4K on Win32
    virtual unsigned int QAPI getFastIOAlignment() const = 0;

    virtual cMemoryView::ConstPtr QAPI getEmptiness(const size_t numBytes) const = 0;
  private:
    //Not Permitted
    virtual cMemoryView::ConstPtr QAPI getEmptiness(const not_size_t numBytes) const throw(cFileError) = 0;
};
//}}}
//{{{  class iFileManager
// =======================================================================================
// === iFileManager ======================================================================
// =======================================================================================

// Used by iFileManager::enumerateFiles
const int fEnumerateFiles_Uppercase = 1 << 0;
const int fEnumerateFiles_Lowercase = 1 << 1;

// Used by iFileManager::deleteFileOrFolder and deleteTemporaryFolders
const int fDeleteFileOrFolder_DeleteReadOnly = 1 << 0;
const int fDeleteFileOrFolder_IgnoreErrorsWhenRecursing = 1 << 1;

enum eFileLocation
{
  eFileLocation_Undetermined,
  eFileLocation_Local,
  eFileLocation_Remote
};

class iFileManager : public iManager
{
  public:

    QDEFINE_SINGLETON_IFACE(iFileManager, L"{AC26AA18-7339-4586-8B5F-C36ADDDD8124} // iFileManager", iManager);

    virtual iFile::Ptr QAPI openFile(const String & SrcFilename, const int WhichAccessFlags) throw(cBadFormat, cFileError, cNotFound, cAccessDenied) = 0;
    class iFileCallback
    {
      public :
        QDEFINE_SMARTPTR(iFileCallback);
        virtual ~iFileCallback() {}
        enum NextNode
        {
          eNextNode_Parent,
          eNextNode_NextChild,
          eNextNode_NextSibling
        };
        virtual NextNode QAPI onFile(
            const String &     AbsPath,
            const String &     RelPath,
          const String &     Name,
          const String &     Ext,
          const unsigned int Depth) = 0;
        virtual NextNode QAPI onFolder(
            const String &     AbsPath,
            const String &     RelPath,
          const String &     Name,
          const unsigned int Depth) = 0;
    };
    virtual void QAPI enumerateFiles(const int Flags, const String & Path, iFileCallback::Ptr Enumerator) throw(cBadFormat, cFileError) = 0;
    virtual std::list<String> QAPI scanFiles(const String & Path, const bool IsCreatingAbsPaths) throw(cBadFormat, cFileError) = 0;
    virtual bool QAPI isFileOrFolderCreated(const String & Filename) const throw(cBadFormat, cFileError) = 0;
    virtual void QAPI deleteFileOrFolder(const String & Name, const unsigned int Flags) throw(cBadFormat, cFileError, cNotFound, cAccessDenied) = 0;
    virtual void QAPI createFolder(const String & Path) throw(cBadFormat, cFileError) = 0;
    virtual bool QAPI setCurrentFolder(const String & NewFolderPath) throw(cBadFormat, cFileError) = 0;
    virtual String QAPI getCurrentFolder() = 0;
    virtual void QAPI renameFile(const String & CurrentFilename, const String & NewFilename) throw(cBadFormat, cFileError) = 0;
    virtual bool QAPI isFolder(const String & Name) throw(cBadFormat) = 0;
    virtual void QAPI copyFileOrFolder(const String & SrcFilename, const String & DstFilename, const int DstAccess) throw(cBadFormat, cFileError) = 0;
    virtual void QAPI setFileOrFolderAccess(const String & Filename, const int NewAccess) throw(cBadFormat, cFileError) = 0;
    virtual int QAPI getFileOrFolderAccess(const String & Filename) throw(cBadFormat, cFileError) = 0;
    virtual String QAPI createTemporaryFolder(const String & Prefix) throw(cFileError) = 0;
    virtual void QAPI deleteTemporaryFolders(const String & Prefix, const unsigned int Flags) throw(cFileError) = 0;
    virtual iFile::Ptr QAPI openFile(const String & SrcFilename, const int WhichAccessFlags, const int WhichOptionFlags) throw(cBadFormat, cFileError, cNotFound, cAccessDenied) = 0;
    virtual iFile::Ptr QAPI openFile(const String & SrcFilename) throw(cBadFormat, cFileError, cNotFound, cAccessDenied) = 0;
    virtual eFileLocation QAPI whereIsFileOrFolder(const String & Filename) throw(cFileError) = 0;
    virtual Int64 QAPI getDiskSize(const String & Path) throw(cBadFormat, cFileError) = 0;
    virtual Int64 QAPI getDiskAvailableSize(const String & Path) throw(cBadFormat, cFileError) = 0;
    virtual void QAPI splitPath(const String & Path, String * const ResultDrive, String * const ResultPath, String * const ResultName, String * const ResultExtension) throw(cBadFormat) = 0;

    // Tidy formatting of string (e.g. add trailing '\'). Does not rely on path actually existing.
    virtual String QAPI formatAsPath(const String & ToFormat) = 0;

    // Turn shortcut LNK file into the actual path to which it refers (Windows specific).
    virtual String QAPI getShortcutTargetDir(const String & linkFilename) = 0;

};
//}}}
//{{{
// =======================================================================================
// === iCmdConsoleManager ================================================================
// =======================================================================================

class iCmdConsoleManager : public iFactory::iInstance
{
  public:

    QDEFINE_SINGLETON_IFACE(iCmdConsoleManager, L"{4FAF66D7-3032-4372-B2D9-B702502E4CB3} // iCmdConsoleManager", iFactory::iInstance);

    //{{{
    class iCommand : public iGuarded
    {
      public :
        QDEFINE_SMARTPTR(iCommand);
        virtual String QAPI getName() const = 0;
        virtual void QAPI logOneLineDescription() const = 0;
        virtual void QAPI logDetailedDescription() const = 0;
        // Do not include /? - Kernel will add this automatically. Please use DOS style for
        // consistency - see "dir /?" from you command line.
        virtual void QAPI logOneLineParams() const = 0;
        // Do not include /? - Kernel will add this automatically. Align descriptions in by
        // 16 chars. Please use DOS style for consistency - see "dir /?" from you command line.
        virtual void QAPI logDetailedParams() const = 0;
        virtual void QAPI execute(const String & Parameters) const = 0;
    };
    //}}}

    /*
    To add a console command handler for a 'name'. If masterThreadOnly is true it will only be
    called in the master thread. If masterThreadOnly is false the command handler could be
    called by any thread.
    */
    virtual cGuard::ConstPtr QAPI addCommand(
      const iCommand::Ptr & newCommand,
      const bool IsExecuteInMasterThreadOnly = true,
      const bool MakeLowerCaseAllParams = true) = 0;

    virtual void QAPI queueCommand(const String & command, const String & params) = 0;

    virtual void QAPI dispatchMainThreadCommands() = 0;
};
//}}}
//{{{
// =======================================================================================
// === iTimerManager =====================================================================
// =======================================================================================

class iTimerManager : public iFactory::iInstance
{
  public:

    QDEFINE_SINGLETON_IFACE(iTimerManager, L"{049D5D3A-4BD9-4c68-847C-513C94B717B9} // iTimerManager", iFactory::iInstance);

    //{{{
    class iOneSecondTimerCallback : public iGuarded
    {
      public :
        QDEFINE_SMARTPTR(iOneSecondTimerCallback);
        virtual void onOneSecondElapsed(const Int64 MillisecsElapsedSinceStart) = 0;
    };
    //}}}
    virtual cGuard::ConstPtr QAPI addOneSecondTimerCallback(const iOneSecondTimerCallback::Ptr & CallbackToAdd) = 0;
    virtual void QAPI waitNextSecondTick() = 0;
};
//}}}

//{{{  class iDeviceManager
// =======================================================================================
// === iDeviceManager ====================================================================
// =======================================================================================

class iDeviceDriver;
class iDeviceManager : public iManager
{
public:

    QDEFINE_SINGLETON_IFACE(iDeviceManager, L"{DAA29080-662F-4D9F-8E6C-E6EFFBF3A861} // iDeviceManager", iManager);

    virtual cPtr<iDeviceDriver> QAPI getDeviceByName(const String & DriverName) const throw(cNotFound) = 0;
    virtual std::vector<cPtr<iDeviceDriver> > QAPI getDevicesByClass(const cGuid & DriverClassGuid) const = 0;
    //{{{
    class iPlugAndPlayClient : public iGuarded
    {
      public :
        QDEFINE_SMARTPTR(iPlugAndPlayClient);
        virtual void QAPI onDeviceAcquired(cPtr<iDeviceDriver> Device) = 0;
        virtual void QAPI onDeviceRequestingRelease(cPtr<iDeviceDriver> Device) = 0;
        virtual void QAPI onDeviceReleased(const String & DeviceName) = 0;
    };
    //}}}
    virtual cGuard::ConstPtr QAPI addPlugAndPlayClient(iPlugAndPlayClient::Ptr ClientToAdd) = 0;
    virtual bool QAPI releaseDeviceByName(const String & DriverName) = 0;
    virtual void QAPI acquireDevice(const cGuid & ClassGuid) throw(cNotAvailable) = 0;
    virtual std::multimap<cGuid, String> QAPI getWin32DevicesByType() = 0;
    //{{{
    class iPlugAndPlayOverride : public iGuarded
    {
      public :
        QDEFINE_SMARTPTR(iPlugAndPlayOverride);
        virtual cPtr<iDeviceDriver> QAPI deviceAcquire(const cGuid & DriverClassGuid) = 0;
        virtual void QAPI deviceRelease(cPtr<iDeviceDriver> Device) = 0;
    };
    //}}}
    virtual cPtr<iDeviceDriver> /* invalid if not found! */ QAPI findDeviceByName(const String & DriverName) const = 0;
};
//}}}
//{{{  class iBootManager
// =======================================================================================
// === iBootManager ======================================================================
// =======================================================================================

class cProgressMsgGuard;
class cEmergencyProgressMsgGuard;

class iBootManager : public iManager
{
  public :

    QDEFINE_SINGLETON_IFACE(iBootManager, L"{728D7933-6AB5-4A9C-9078-9B8E5D1EAD9E} // iBootManager", iManager);

    enum eBootState
    {
      eBootState_Booting = 100,
      eBootState_Booted = 200,
      eBootState_CleanExiting = 300,
      eBootState_Crashing = 400,
      eBootState_InstallDllsNecessary = 500,
      eBootState_RestartNecessary = 600,
    };
    //{{{
    class iProgressCallback : public iGuarded
    {
      public :
        QDEFINE_SMARTPTR(iProgressCallback);
        virtual void QAPI onBootStateChanged(const eBootState newState) = 0;
    };
    //}}}
    virtual cGuard::ConstPtr QAPI addProgressCallback(iProgressCallback::Ptr Callback) = 0;
#if defined(_WIN32)
  #if defined(_WINDOWS_) // is windows header included?
    virtual bool QAPI isKernelBootVisible(HWND * const kernelBootWindowHandle) const = 0;
  #else
    virtual void QAPI isKernelBootVisible() const = 0;
  #endif
#endif
    virtual eBootState QAPI getState() const = 0;
    virtual void QAPI setState(const eBootState newState) = 0;

    virtual void QAPI INTERNAL_USE_ONLY_createProgressMsgGuard(cProgressMsgGuard * const guard, const Char * const format, const va_list & vargs) = 0;
    virtual void QAPI INTERNAL_USE_ONLY_destroyProgressMsgGuard(cProgressMsgGuard * const guard) = 0;
    virtual void QAPI INTERNAL_USE_ONLY_createEmergencyProgressMsgGuard(cEmergencyProgressMsgGuard * const guard, const Char * const format, const va_list & vargs) = 0;
    virtual void QAPI INTERNAL_USE_ONLY_destroyEmergencyProgressMsgGuard(cEmergencyProgressMsgGuard * const guard) = 0;
#if defined(_WIN32)
  #if defined(_WINDOWS_) // is windows header included?
    virtual void QAPI INTERNAL_USE_ONLY_notifyAppWindowCreated(const HWND hAppWindow) = 0;
  #else
    virtual void QAPI INTERNAL_USE_ONLY_notifyAppWindowCreated() = 0;
  #endif
#endif
};
//}}}
//{{{
// =======================================================================================
// === cProgressMsgGuard =================================================================
// =======================================================================================
/*
*/
class cProgressMsgGuard : public cRefCount
{
  public :
    QDEFINE_SMARTPTR(cProgressMsgGuard);
    cProgressMsgGuard(const Char * const Format, ...) : m_Id(0)
    {
      if (iBootManager::isSingletonConstructed()) iBootManager::singleton().INTERNAL_USE_ONLY_createProgressMsgGuard(this, Format, (va_list)&Format + _INTSIZEOF(Format));
    }
    virtual ~cProgressMsgGuard()
    {
      if (iBootManager::isSingletonConstructed()) iBootManager::singleton().INTERNAL_USE_ONLY_destroyProgressMsgGuard(this);
    }
    unsigned int m_Id;
};
//}}}
//{{{
// =======================================================================================
// === cEmergencyProgressMsgGuard ========================================================
// =======================================================================================
/*
*/
class cEmergencyProgressMsgGuard : public cRefCount
{
  public :
    QDEFINE_SMARTPTR(cEmergencyProgressMsgGuard);
    cEmergencyProgressMsgGuard(const Char * const Format, ...) : m_Id(0)
    {
      if (iBootManager::isSingletonConstructed()) iBootManager::singleton().INTERNAL_USE_ONLY_createEmergencyProgressMsgGuard(this, Format, (va_list)&Format + _INTSIZEOF(Format));
    }
    virtual ~cEmergencyProgressMsgGuard()
    {
      if (iBootManager::isSingletonConstructed()) iBootManager::singleton().INTERNAL_USE_ONLY_destroyEmergencyProgressMsgGuard(this);
    }
    unsigned int m_Id;
};
//}}}

//{{{  class iActivityLog
// =======================================================================================
// === iActivityLog ======================================================================
// =======================================================================================
/*
*/

class iActivityLogView;
class iActivityPool;
class cActivityTrigger;
class iDebugControlCallback;

class iActivityLog : public iManager
{
public:

  QDEFINE_SINGLETON_IFACE(iActivityLog, L"{7F1AFF41-13B1-48dc-9769-1CD595F1EC9A} // iActivityLog", iManager);

  virtual double QAPI currentTime() = 0;
  virtual UInt64 QAPI currentTicks() = 0;
  virtual UInt64 QAPI tickFreq() = 0;

  virtual void QAPI resetLog() = 0;
  virtual void QAPI startLogging() = 0;
  virtual void QAPI stopLogging() = 0;

  virtual int QAPI signalCount() const = 0;

  virtual iActivity::Ptr QAPI addSignal (const String name, iActivity::eTraceType type) = 0;
  virtual iActivity::Ptr QAPI addSignal (const String name) = 0;

  virtual int QAPI groupCount() const = 0;
  virtual iActivityGroup::Ptr QAPI getGroup (const String name) = 0;

  virtual int QAPI firstGroup() const = 0;
  virtual int QAPI nextGroup (int groupIdent) const = 0;
  virtual const String QAPI title (int groupIdent) const =0;
  virtual bool QAPI isEnabled (int groupIdent) const = 0;
  virtual bool QAPI enableGroup (int groupIdent) const = 0;
  virtual bool QAPI disableGroup (int groupIdent) const = 0;
  virtual int QAPI signalCount (int groupIdent) const = 0;

  virtual cPtr<iActivityLogView> QAPI createView() = 0;

  virtual void QAPI requestGrab() = 0;

  class iCallback : public iGuarded
  {
    public:
      QDEFINE_SMARTPTR(iCallback);
      virtual void onGrabRequest() = 0;
      virtual void onTrigger() = 0;
  };
  virtual cGuard::ConstPtr QAPI addCallback(iCallback::Ptr NewCallback) = 0;

  virtual std::vector<String> QAPI getSignalNames (int groupIdent) const = 0;

  // iActivityPool creators  - for creating multiple lines of activities in ActivityMon
  virtual cPtr<iActivityPool> QAPI createActivityNest (iActivity::Ptr (*funcCreateActivity)(const String& nameSuffix)) = 0;    // nestable activities  (all except Base are marked as Detail)
  virtual cPtr<iActivityPool> QAPI createActivityMap (iActivity::Ptr (*funcCreateActivity)(const String& nameSuffix)) = 0;     // indexed activities eg ProcNum                         suffix=number
  virtual cPtr<iActivityPool> QAPI createActivitySet (iActivity::Ptr (*funcCreateActivity)(const String& nameSuffix)) = 0;     // multi-thread - gives new activity when calls overlap, suffix=letter
  virtual cPtr<iActivityPool> QAPI createActivityNestMap (iActivity::Ptr (*funcCreateActivity)(const String& nameSuffix)) = 0; // a Map of Nested activities
  virtual cPtr<iActivityPool> QAPI createActivityNestSet (iActivity::Ptr (*funcCreateActivity)(const String& nameSuffix)) = 0; // a Set of Nested activities

  virtual cPtr<iActivityPool> QAPI createActivityNest (const String& nameSuffix, iActivity::Ptr (*funcCreateActivity)(const String& nameSuffix)) = 0;   // internal use

  // v3 extensions
  virtual void QAPI defineActivitySpec (int activitySpec, const std::string& displaySpec) = 0;
  virtual std::string QAPI getActivityColourName (int activity) = 0;

  // ActivityLog triggers - return true if ok = mSignaIdent was found  - trigger will be activated when entry has (eTriggerNext not set)
  virtual bool QAPI setTrigger (const cActivityTrigger& trigger) = 0; // use for only or first entry (eTriggerNext set)
  virtual bool QAPI addTrigger (const cActivityTrigger& trigger) = 0; // use for subsequent or last entry (eTriggerNext not set)
  virtual void QAPI resetTrigger() = 0;
  virtual void QAPI triggered() = 0;    // callback with onTrigger()

  virtual void QAPI defineActivitySpec (const std::string& name, const std::string& displaySpec) = 0;   // named sub-spec

  // DebugControl
  virtual void registerDebugControl (cPtr<iDebugControlCallback> callback, const String& name) = 0;
  // for use by ActivityMonitor
  virtual bool getDebugControl (size_t index) const = 0;
  virtual void setDebugControl (size_t index, bool newState) = 0;
  virtual size_t getDebugControlCount() const = 0;
  virtual String getDebugControlName(size_t index) const = 0;

  virtual void QAPI registerGroup (const String name, bool enabled) = 0;

};
//}}}

//{{{
class iDLLManager : public iManager
{
  public :
QDEFINE_SINGLETON_IFACE(iDLLManager, L"{C074E79B-9708-4563-82AA-E2488966C52C} // iDLLManager", iManager);

    virtual void QAPI installDLLs(const unsigned long MinGroup, const unsigned long MaxGroup) = 0;
    virtual void QAPI uninstallDLLs(const bool IsRemovingSettingsAndData) = 0;
    virtual unsigned int QAPI countDLLs(const unsigned long MinGroup, const unsigned long MaxGroup) = 0;
    virtual void QAPI initDLLs(const unsigned long MinGroup, const unsigned long MaxGroup) = 0;

#if defined(__ATLBASE_H__)
    virtual std::vector<_ATL_OBJMAP_ENTRY> QAPI getATLObjects() throw(cNotAvailable) = 0;
#else
    virtual std::vector<int> QAPI getATLObjects() throw(cNotAvailable) = 0;
#endif
#if defined(_WINDOWS_) // is windows header included?
    virtual std::multimap<HINSTANCE, unsigned int> getATLTypeLibraries() = 0;
#else
    virtual std::multimap<int, unsigned int> getATLTypeLibraries() = 0;
#endif
    virtual iModuleInstance::Ptr QAPI getInstance(const String & DeveloperName, const String & ModuleName) throw(cNotFound) = 0;
    virtual void QAPI registerDLLs() = 0;
    virtual bool QAPI isInstallDllsNecessary() = 0;

    virtual void QAPI logChangedSettings() = 0;

    virtual bool QAPI hasPatchedDLLs() = 0;
};
//}}}
// --- End of Namespace ------------------------------------------------------------------
}

// --- End of Structure/Class Packing Size -----------------------------------------------
//{{{
#if defined(_MSC_VER)
    #pragma pack(pop)
#endif
//}}}
#endif
