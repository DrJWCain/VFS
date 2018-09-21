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

#include <QKernelExecDDK.h>

using namespace vfs;
using namespace std;




// =======================================================================================
// === cExceptionManager =================================================================
// =======================================================================================

class cExceptionManager : public iExceptionManager, public iGuardCreator, public cRefCount
{
  friend iExceptionManager;

  public :

    QDEFINE_SINGLETON_IMPL(cExceptionManager, L"{91A64A11-CB18-440C-88BA-4964036AC18B} // Exception manager impl", iExceptionManager);

    // iManager
    virtual void QAPI logSummary() {}
    virtual void QAPI logDetailedSummary() {}

    // iExceptionManager
    virtual void QAPI startDrivers();
    virtual void QAPI stopDrivers();
    virtual cGuard::ConstPtr QAPI addFatalCallback(iFatalCallback::Ptr callbackToAdd);
    virtual void QAPI fatal(const unsigned int Flags, const cFatalDiagnostics & Diagnostics); // throw cAttemptedFatal

    virtual void QAPI logCallStack() const;                                                     //!< logs the current call stack (as QMSG) - fStack_ShowBasic
    virtual void QAPI logCallStack( unsigned int displayFlags ) const;                          //!< logs call stack with control of displayed info
    //virtual void QAPI logCallStack( unsigned int displayFlags, int startingAtLevelNum ) const;  //!< logs call stack, skipping the initial entries
    //virtual void QAPI logCallStack( unsigned int displayFlags, const String& afterFunc ) const; //!< logs call stack, skipping entries prior to a function
    virtual void QAPI loadAllSymbols() const;  // warning: may take a while

    virtual String QAPI getDebugHeader( int displayFlags ) const;                         //!< returns a column header string to match getDebugLine calls
    virtual String QAPI getDebugLine( const void* const addr, int displayFlags ) const;   //!< returns the src file & routine at this address (same as logCallStack)

    virtual cGuard::ConstPtr QAPI addWarningCallback(const iWarningCallback::Ptr & callbackToAdd);
    virtual void QARGS_STACK presentWarning(const Char * const FormatMsg, ...);
    virtual void QARGS_STACK presentWarningWait(const Char * const MsgFormat, ...);
    virtual void QARGS_STACK presentWarningWait(const iWarningWaitCallback::Ptr & feedbackCallback, const vector<String> & FeedbackChoices, const Char * const MsgFormat, ...);
    virtual void QARGS_STACK presentInformation(const Char * const FormatMsg, ...);
    virtual cGuard::ConstPtr QAPI addInformationCallback(const iInformationCallback::Ptr & callbackToAdd);
    virtual void QARGS_STACK reboot(const Char * const  MsgFormat, ...);
    virtual void QARGS_STACK INTERNAL_USE_ONLY_report(const char * const Filename, const unsigned long Line, const Char * const  MsgFormat, ...);
    virtual void QARGS_STACK presentFatal(const char * const SrcFilename,const unsigned long SrcLineNumber,const Char * const  MsgFormat, ...);
    virtual cGuard::ConstPtr QAPI addStatusCallback(const iStatusCallback::Ptr & callbackToAdd);
    virtual void QARGS_STACK presentStatus(const Char * const MsgFormat, ...);
    virtual cGuard::ConstPtr QAPI enableExceptionOnFatal();

    virtual void QARGS_STACK deprecated_presentWarning(const cRecoverable & Reason, const Char * const MsgFormat, ...) {}
    virtual void QARGS_STACK deprectated_presentInformation(const cRecoverable & Reason, const Char * const MsgFormat, ...) {}
    virtual void QAPI deprecated_logAllThreadsCallStacks() const {}
    virtual void QAPI deprecated_logAllModules() {}

    // cExceptionManager : BEWARE: KernelDebugDriver uses this interface so build that too if this changes!
    cExceptionManager();
    ~cExceptionManager();
    virtual cGuard::ConstPtr addVectoredExceptionHandler() throw(cWin32Error);
    virtual void _onVectoredException(DWORD ThreadID, const EXCEPTION_POINTERS * const ep);
    virtual std::vector<cGuard::ConstPtr> startFatalCallbacks();
    virtual void setDialogTitle(const String & NewTitle);
    virtual String getDialogTitle() const;
    virtual bool resetUnhandledExceptionFilter();
    virtual void signalMiniDumpPoint() const;
    virtual iDebugDriver::Ptr debugDriver() const { return m_DebugDriver; }
    virtual String QAPI convertExceptionRecord (EXCEPTION_RECORD* ex, cRecoverable*& recoverablePtr) const;
      // recoverablePtr will be set to NULL if not a cRecoverable exception

  private :

    //{{{
    class cFatalCallbacksThread : public cBasicThread
    {
      public :
        QDEFINE_SMARTPTR(cFatalCallbacksThread);
        cFatalCallbacksThread(const iFatalCallback::Ptr & Callback, const String & ThreadName);
        void QAPI signalLoopStop() {} // Can't stop!
      private :
        iFatalCallback::Ptr m_Callback;
        void QAPI runLoop();
    };
    //}}}

    class cErrorDialogThread;
    //{{{
    class cFatalTerminateThread : public cBasicThread
    {
      public :
        QDEFINE_SMARTPTR(cFatalTerminateThread);
        cFatalTerminateThread (const cPtr<cErrorDialogThread> & ErrorDialogThread);
        void QAPI signalLoopStop() {} // Can't stop!
      private :
        void QAPI runLoop();
        cPtr<cErrorDialogThread> m_ErrorDialogThread;
    };
    //}}}

    //{{{
    class cErrorDialogThread : public cBasicThread
    {
      public :
        QDEFINE_SMARTPTR(cErrorDialogThread);
        cErrorDialogThread(const unsigned int Flags, const cFatalDiagnostics & Diagnostics, const String & Title);
        cErrorDialogThread(const unsigned int Flags, const cFatalDiagnostics & Diagnostics, const String & Title, const unsigned int TimeoutMillisecs);
        void QAPI signalLoopStop();
      private :
        unsigned int      m_Flags;
        cFatalDiagnostics m_Diagnostics;
        String            m_Title;
        unsigned int      m_TimeoutMillisecs;
        bool              m_IsAborting;
        void QAPI runLoop();
    };
    //}}}

  private :

    mutable cLock                      m_MemberLock; // To protect member variables from changes by
                                                      //  another thread.

    list<iFatalCallback::Ptr>          m_FatalCallbacks;

    //{{{
    class cWarning
    {
      public :
        iWarningWaitCallback::Ptr m_FeedbackCallback;
        vector<String>            m_FeedbackChoices;
        String                    m_Msg;
        cWarning(
          iWarningWaitCallback::Ptr FeedbackCallback,
          const vector<String> &    FeedbackChoices,
          const String &            Msg) :
            m_FeedbackCallback(FeedbackCallback),
            m_FeedbackChoices(FeedbackChoices),
            m_Msg(Msg)
        {
        }
    };
    //}}}

    list<cWarning>                      m_WarningQueue;
    list<iWarningCallback::Ptr>         m_WarningCallbacks;

    list<iStatusCallback::Ptr>          m_StatusCallbacks;

    list<cWarning>                      m_InformationQueue;
    list<iInformationCallback::Ptr>     m_InformationCallbacks;

    iDebugDriver::Ptr                   m_DebugDriver;

    unsigned int                        m_ErrorDialogTimeoutMillisecs;

    LPTOP_LEVEL_EXCEPTION_FILTER        m_hPrevUnhandledExceptionFilter;

    String                              m_DialogTitle;

    mutable cLock m_ExceptionOnFatalGuardsThreadSafety;
    //{{{
    class cExceptionOnFatalGuard : public iGuarded, public cRefCount
    {
      public :
        QDEFINE_SMARTPTR(cExceptionOnFatalGuard);
        cExceptionOnFatalGuard(const unsigned int threadId) : m_ThreadId(threadId) {}
        const unsigned int m_ThreadId;
    };
    //}}}
    list<cExceptionOnFatalGuard::Ptr> m_ExceptionOnFatalGuards;
    bool _isThrowingExceptionOnFatal() const;

    bool _isPausingOnCrash() const;
    bool _isPausingForeverOnCrash() const;

    void _vpresentWarning(
      iWarningWaitCallback::Ptr FeedbackCallback,
      const vector<String> &    FeedbackChoices,
      const Char * const        MsgFormat,
      va_list                   VAList);
    void _vpresentInformation(const Char * const MsgFormat, va_list VAList);
    void _vreboot(const Char * const Msg, va_list VAList);
    unsigned int _doMsgBox(const unsigned int Flags, const cFatalDiagnostics & Diagnostics);

    // iGuardCreator
    void QAPI onGuardDestroyed(iGuarded::Ptr Guarded);

    // Global event used in the event of an exception so that
    // KernelBoot can create a MiniDump if required
    HANDLE hEventCouldCreateMiniDump;
};
