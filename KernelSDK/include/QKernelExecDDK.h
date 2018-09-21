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

#if !defined(__0DCB93258A5E440f893CFCCA2B71D5CE)
#define __0DCB93258A5E440f893CFCCA2B71D5CE
/*
**  
*/

#include "QWinBase.h"
#include "QKernelBase.h"
#include "QKernelExec.h"

#if defined(_MSC_VER)
    #pragma pack(push, 8)
#endif

namespace vfs
{
//{{{  class iDeviceDriver
// =======================================================================================
// === iDeviceDriver =====================================================================
// =======================================================================================
/*
*/
class iDeviceDriver : public iFactory::iInstance
{
  public:

    QDEFINE_CLASS_IFACE(iDeviceDriver, L"{FD41474E-1446-4B81-942A-AA550BE7ABF9} // iDeviceDriver", iFactory::iInstance);

//{{{
// ---------------------------------------------------------------------------------------
/*
*/
    class iCaps
    {
      public :
        QDEFINE_SMARTPTR(iCaps);
        virtual ~iCaps() {}
        virtual void QAPI logSummary() const = 0;
    };
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
*/
    virtual iCaps::ConstPtr QAPI getCaps(
      void) const throw( cNotAvailable) = 0;
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
*/
    class iResourceLockGuard
    {
      public :
        QDEFINE_SMARTPTR(iResourceLockGuard);
        virtual ~iResourceLockGuard() {}
    };
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
*/
    class iDeviceLockGuard
    {
      public :
        QDEFINE_SMARTPTR(iDeviceLockGuard);
        virtual ~iDeviceLockGuard() {}
    };
    class cDeviceLockGuard : public iDeviceLockGuard, public cRefCount
    {
      public :
        QDEFINE_SMARTPTR(cDeviceLockGuard);
        cDeviceLockGuard(cConstPtr<iDeviceDriver> Driver, cLockGuard::ConstPtr LockGuard) : m_Driver(Driver), m_LockGuard(LockGuard) {}
      private :
        cConstPtr<iDeviceDriver>  m_Driver;
        cLockGuard::ConstPtr      m_LockGuard;
    };
    virtual iDeviceLockGuard::ConstPtr QAPI lockDevice() const throw(cNotSupported) = 0;
//}}}
};
//}}}
//{{{  class iLogDriver
// =======================================================================================
// === iLogDriver ========================================================================
// =======================================================================================

class iLogDriver : public iFactory::iInstance
{
  public:

    QDEFINE_CLASS_IFACE(iLogDriver, L"{dddcfdaa-af0b-11d3-9e36-009027a6c8a4} // iLogDriver", iFactory::iInstance);

    class cMsg : public cRefCount
    {
      public :
        QDEFINE_SMARTPTR(cMsg);
        enum eMsgType
        {
          ePlainMsg,
          eCollapsedIndent,
          eExpandedIndent,
          eLazyIndent,
          eOutdent,
          eTrace,
          eBreak,
          eErrorMsg
        };
        cMsg(
          const eMsgType     Type,
          const String &     ThreadName,
          const String &     ModuleName,
          const unsigned int NumIndents,
          const unsigned int NumSOSIndents,
          const unsigned int NumInjectedIndents,
          const unsigned int NumSuppressedIndents,
          const unsigned int Category,
          const String &     Text,
          const unsigned int NumSuppressedSinceLast,
          const bool         IsHighlighted,
          const cTime&       LocalTime,
          const unsigned int ThreadId) :
            m_ThreadName(ThreadName),
            m_ModuleName(ModuleName),
            m_NumIndents(NumIndents),
            m_NumSOSIndents(NumSOSIndents),
            m_NumInjectedIndents(NumInjectedIndents),
            m_NumSuppressedIndents(NumSuppressedIndents),
            m_Type(Type),
            m_Category(Category),
            m_Text(Text.c_str()),
            m_NumSuppressedSinceLast(NumSuppressedSinceLast),
            m_IsHighlighted(IsHighlighted),
            m_LocalTime(LocalTime),
            m_ThreadID(ThreadId)
        {
        }
        unsigned long m_ThreadID;
        String        m_ThreadName;
        String        m_ModuleName;
        eMsgType      m_Type;
        unsigned int  m_NumIndents;
        unsigned int  m_NumSOSIndents;
        unsigned int  m_NumInjectedIndents;
        unsigned int  m_NumSuppressedIndents;
        unsigned int  m_Category;
        String        m_Text;
        unsigned int  m_NumSuppressedSinceLast;
        bool          m_IsHighlighted;
        cTime         m_LocalTime;
    };

    virtual void QAPI writeOutdent(iLogDriver::cMsg::ConstPtr Record) = 0;
    virtual void QAPI writeMsg(iLogDriver::cMsg::ConstPtr Record) = 0;
    virtual void QAPI writeCollapsedIndent(iLogDriver::cMsg::ConstPtr Record) = 0;
    virtual void QAPI writeExpandedIndent(iLogDriver::cMsg::ConstPtr Record) = 0;
    virtual void QAPI writeLazyIndent(iLogDriver::cMsg::ConstPtr Record) = 0;
    virtual void QAPI writeTrace(iLogDriver::cMsg::ConstPtr Record) = 0;
    virtual void QAPI writeBreak(iLogDriver::cMsg::ConstPtr Record) = 0;
};
//}}}
//{{{  class iBootPhaseDriver
// =======================================================================================
// === iBootPhaseDriver ==================================================================
// =======================================================================================

class iBootPhaseDriver : public iFactory::iInstance
{
  public:

    QDEFINE_CLASS_IFACE(iBootPhaseDriver, L"{ED7E3DFB-8861-4E38-A30C-55182B01E73F} // iBootPhaseDriver", iFactory::iInstance);

    enum eBootPhase
    {
      eMonitoring = 0,
      eDevices    = 1,
      eDisplay    = 2,
      eSecurity   = 3,
      ePlugins    = 4,
      eBooted     = 6
    };

    virtual eBootPhase QAPI getPhase() = 0;
    virtual void QAPI doPhase() throw(cRecoverable) = 0;
};
//}}}
//{{{  class iDebugDriver
// =======================================================================================
// === iDebugDriver ======================================================================
// =======================================================================================
/*
*/
//Parameters passed to RaiseException()
#define EXCEPTION_vfs_FATAL ((DWORD)0xC0695156L)

typedef struct _cExceptionRecord
{
  const Char* Filename;
  unsigned long LineNumber;
  const Char* Message;
} cExceptionRecord;

class iDebugDriver : public iFactory::iInstance
{
  public:

    QDEFINE_CLASS_IFACE(iDebugDriver, L"{7ACA74F0-DFC1-441E-93C2-4E6342440DA2} // iDebugDriver", iFactory::iInstance);

    virtual void QAPI logAllModules() = 0;
    virtual void QAPI logCallStack( unsigned int displayFlags ) = 0;  // Logs the call stack of the current thread.
    //virtual void QAPI logCallStack(unsigned long ThreadID, const String & AfterFunc) = 0;  //Logs the call stack of the specified thread.
    //virtual void QAPI logAllThreadsCallStacks() = 0;  //Logs the call stacks of all threads including current thread.

    virtual bool QAPI resetUnhandledExceptionFilter() = 0; // Ensures that the Debug driver has the unhandled Exception filter set.
    virtual std::map<unsigned int, iThread::Ptr> QAPI getAllThreads() = 0;

    #if defined(_WINDOWS_) // is windows header included?
      virtual void QAPI logCallStack( DWORD ThreadID, const EXCEPTION_POINTERS * const ep ) = 0;
    #endif

    virtual String QAPI getDebugHeader( int displayFlags ) = 0;                         //!< returns a column header string to match getDebugLine calls
    virtual String QAPI getDebugLine( const void* const addr, int displayFlags ) = 0;   //!< returns the src file & routine at this address (same as logCallStack)

    virtual void QAPI getFaultReason( DWORD threadID, const EXCEPTION_POINTERS* const ep, iExceptionManager::cFatalDiagnostics& pDiagToLoad ) = 0;
    virtual void QAPI loadAllSymbols() = 0;  // warning: may take a while
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
