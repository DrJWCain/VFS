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

#include "stdafx.h"
#include "cExceptionManager.h"
#include "cLogManager.h"
#include "LogCategories.h"
#include "cSystemInfo.h"
#include <signal.h>//support for catching runtime errors
#include <sys/types.h>
#include <sys/timeb.h>
#include <typeinfo>

using namespace vfs;

namespace vfs
{
  // [GENQ-9014/AJS] Block termination until any asynchronous shutdown threads
  //                 have completed.

  cLock             gFatalCallbackLock;
  cManualResetEvent gShutdownAllowed(true);
  volatile int      gFatalCallbacksRunning;

  //{{{
  void gWaitAndTerminateProcess(HANDLE process, UINT exitCode)
  {
    int numBlockers = 0;

    do
    {
      gShutdownAllowed.waitForever();

      {
        cLockGuard  safety(&gFatalCallbackLock);
        numBlockers = gFatalCallbacksRunning;
      }
    } while (numBlockers > 0);

    TerminateProcess(process, exitCode);
  }
  //}}}
}

// =======================================================================================
// === startDrivers/stopDrivers ==========================================================
// =======================================================================================

//{{{
void QAPI cExceptionManager::startDrivers()
{
  std::list<iFactory::iConstructor::Ptr> Constructors = gFactory->getConstructors(iDebugDriver::class_info().getGuid());
  if (Constructors.size() > 1)
  {
    QFATAL((__FILE__, __LINE__, L"Only one implementation of iDebugDriver expected"));
  }
  else if (Constructors.size() == 1)
  {
    m_DebugDriver = ptr_cast<iDebugDriver::Ptr>(gFactory->createInstance(iDebugDriver::class_info().getGuid()));
  }
}

//}}}
//{{{
// ---------------------------------------------------------------------------------------

void QAPI cExceptionManager::stopDrivers()
{
  m_DebugDriver.invalidate();
}


//}}}

// =======================================================================================
// === INTERNAL_USE_ONLY_report ==========================================================
// =======================================================================================

//{{{
void QAPI cExceptionManager::INTERNAL_USE_ONLY_report(
  const char * const  Filename,
  const unsigned long Line,
  const Char * const  msgFormat,
  ...)
{
  // Validate the parameters WITHOUT throwing an exception...
  QVERIFY(Filename != 0);
  // ...so be prepared for bad values to fall through from asserts above.

  Char * pSrcFileNameW = 0;

  const unsigned int nNumFileNameChars = (unsigned int)strlen(Filename) + 1;
  pSrcFileNameW = new Char[nNumFileNameChars];
  pSrcFileNameW[0] = _L('\0');
  // Check this just in case it failed the assert above.
  if (Filename)
  {
    (void)MultiByteToWideChar(
      CP_ACP,                                                                   // Code page
      0,                                                                                // Character-type options
      Filename,                                                 // -> String to map
      nNumFileNameChars,                                                // Number of bytes in string
      pSrcFileNameW,                                                    // -> Wide-character buffer
      nNumFileNameChars);                                               // Size of buffer
  }

  va_list VarArgList;
  va_start(VarArgList, msgFormat);
  Char msgBuffer[512];
  _vsnwprintf(msgBuffer, sizeof(msgBuffer) / sizeof(Char), msgFormat, VarArgList);
  va_end(VarArgList);

  if (iLogManager::isSingletonConstructed())
  {
    cLogIndentSOSGuard IndentGuard(L"*** Non-fatal Error ***");
    QMSG((0, L"%s", msgBuffer));
    QMSG((0, L"File : %s", pSrcFileNameW));
    QMSG((0, L"Line : %d", Line));
  }
  else
  {
    Char DbgStrBuffer[1024];
    _snwprintf(
      DbgStrBuffer, sizeof(DbgStrBuffer) / sizeof(Char),
      L"*** Start Non-fatal Error ***\n%s\nFile : %s\nLine : %d\n*** End Non-fatal Error ***\n",
      msgBuffer, pSrcFileNameW, Line);

    if (IsDebuggerPresent()) OutputDebugString(DbgStrBuffer);
  }

//  if (IsDebuggerPresent()) __asm { int 3 }

  if (pSrcFileNameW) delete [] pSrcFileNameW;
}




//}}}
//{{{

// =======================================================================================
// === _onUnhandledException =============================================================
// =======================================================================================

LONG __stdcall _onUnhandledException(EXCEPTION_POINTERS * ExceptionPtrs)
{
  // IMPORTANT - This can be called from _any_ thread!

  // Present external processes with opportunity to do a mini dump
  cExceptionManager::singleton().signalMiniDumpPoint();

  DWORD exceptionCode = ExceptionPtrs->ExceptionRecord->ExceptionCode;

  cRecoverable* recoverablePtr = NULL;
  String Msg = L"Thread " + lexical_cast<String>(GetCurrentThreadId()) + L", " + cExceptionManager::singleton().convertExceptionRecord (ExceptionPtrs->ExceptionRecord, recoverablePtr);
  String File = recoverablePtr ? recoverablePtr->getSrcFilename() : L"Unknown";
  unsigned int FileLine = recoverablePtr ? recoverablePtr->getSrcLineNumber() : 0;

  if (exceptionCode == EXCEPTION_BREAKPOINT)
  {
    QSOS((L"Exception Manager : Debug break (int3) trapped - passing exception on for \"Just-In-Time Debugging\""));

    // Exception is not recognized. Continue to search up the stack for a handler, first
    // for containing try-except statements, then for handlers with the next highest
    // precedence.
    return EXCEPTION_CONTINUE_SEARCH;
  }
  else if (exceptionCode == EXCEPTION_vfs_FATAL)
  {
    if (ExceptionPtrs->ExceptionRecord->NumberParameters == 3)
    {
      cExceptionRecord * FatalInfo = reinterpret_cast<cExceptionRecord *>(ExceptionPtrs->ExceptionRecord->ExceptionInformation);
      if (FatalInfo)
      {
        Msg      = FatalInfo->Message;
        File     = FatalInfo->Filename;
        FileLine = FatalInfo->LineNumber;
      }
    }
  }

  if (cLogManager::isSingletonConstructed())
  {
    cLogIndentSOSGuard IndentGuard(L"*** Fatal ***");
    iLogManager::bufferError(L"%s", Msg.c_str());
    if (File != L"")   QMSG((0,L"File: %s", File.c_str()));
    if (FileLine != 0) QMSG((0,L"Line: %d", FileLine));
    cLogManager::singleton().flushBuffer();
  }

  OutputDebugString(L"vfs Kernel : Fatal : ");
  OutputDebugString(Msg.c_str());
  OutputDebugString(L"\n");

  Char TmpBuffer[1024];
  if (File != L"" && FileLine != 0)
  {
    (void)_snwprintf(
      TmpBuffer, sizeof(TmpBuffer) / sizeof(Char),
      L"%s\n\n%s\n\nFile: %s\nLine: %d",
      L"Fatal Error", Msg.c_str(), File.c_str(), FileLine);
   }
   else
   {
    (void)_snwprintf(
      TmpBuffer, sizeof(TmpBuffer) / sizeof(Char),
      L"%s\n\n%s",
      L"Fatal Error", Msg.c_str());
  }
  (void)ShowCursor(TRUE);
  (void)MessageBox(NULL, TmpBuffer, cExceptionManager::singleton().getDialogTitle().c_str(), MB_ICONERROR | MB_TOPMOST | MB_SETFOREGROUND | MB_TASKMODAL);

  // [GENQ-9014/AJS] We haven't started the fatal callbacks so we don't need to wait.
  //                 Should the callbacks have been started?
  (void)TerminateProcess(GetCurrentProcess(), EXIT_FAILURE);

  // Never get here!

  return EXCEPTION_EXECUTE_HANDLER;
}




//}}}
// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

namespace
{

//{{{
void __cdecl _purecall()
{
  QFATAL((__FILE__, __LINE__, L"Call to incomplete vtable (ctor calling a pure virtual function?)"));
}

//}}}
//{{{
// [GENQ-8178/AJS] Handler function to capture errors such as invalid parameters that occur in
//                 the C Runtime library or STL implementation (for example use of an invalid
//                 iterator). We can't use the file/line information supplied as this is NULL
//                 in a release build, but calling QFATAL will provide a stack trace which
//                 should identify the location

void __cdecl _kernel_invalid_parameter(const wchar_t *expression, const wchar_t *function,
                                       const wchar_t *file, unsigned int line, uintptr_t pReserved)
{
  cLogIndentSOSGuard sos(L"");
  QFATAL((__FILE__, __LINE__, L"Invalid parameter. STL iterator issue or CRT call with invalid parameter?"));
}

//}}}

}

//{{{
cExceptionManager::cExceptionManager() :
  m_hPrevUnhandledExceptionFilter(NULL),
  m_ErrorDialogTimeoutMillisecs(10000),
  m_DialogTitle(L"KernelServer")
{
  cLogIndentGuard IndentGuard(kCtgCreateManagerDetail, L"Creating ExceptionManager");

  hEventCouldCreateMiniDump = CreateEvent(NULL, FALSE, FALSE, L"vfs::KernelExec::ExceptionMiniDump");

  m_hPrevUnhandledExceptionFilter = SetUnhandledExceptionFilter(_onUnhandledException);

  _set_purecall_handler(_purecall);

  // [GENQ-8178/AJS] Install our invalid parameter handler
  _set_invalid_parameter_handler(_kernel_invalid_parameter);
}
//}}}
//{{{
// ---------------------------------------------------------------------------------------

cExceptionManager::~cExceptionManager()
{
  cLogIndentGuard IndentGuard(0, L"Deleting ExceptionManager");

  if (m_hPrevUnhandledExceptionFilter != NULL)
  {
    (void)SetUnhandledExceptionFilter(m_hPrevUnhandledExceptionFilter) ;
    m_hPrevUnhandledExceptionFilter = NULL;
  }

  if (hEventCouldCreateMiniDump != NULL)
    CloseHandle(hEventCouldCreateMiniDump);
}

//}}}
//{{{
// ---------------------------------------------------------------------------------------

void cExceptionManager::signalMiniDumpPoint() const
{
  SetEvent(hEventCouldCreateMiniDump);
}
//}}}


// =======================================================================================
// === _isPausingOnCrash/_isPausingForeverOnCrash ========================================
// =======================================================================================

//{{{
bool cExceptionManager::_isPausingOnCrash() const
{
  bool Result = true;
  // There is a chance that getKernelExecModuleContext() could return a null pointer if
  // cSystemInfo::init() trys to use cExceptionManager
  try
  {
    Result = iDLLManager::singleton().getInstance(L"vfs", L"_KernelExec")->getPublicSettingNumber(L"Startup\\On crash - pause") != 0;
  }
  catch (cNotFound &) {}
  return Result;
}

//}}}
//{{{
// ---------------------------------------------------------------------------------------

bool cExceptionManager::_isPausingForeverOnCrash() const
{
  bool Result = false;
  if (_isPausingOnCrash())
  {
    // There is a chance that getKernelExecModuleContext() could return a null pointer if
    // cSystemInfo::init() trys to use cExceptionManager
    try
    {
      Result = iDLLManager::singleton().getInstance(L"vfs", L"_KernelExec")->getPublicSettingNumber(L"Startup\\On crash - pause forever") != 0;
    }
    catch (cNotFound &) {}
  }
  return Result;
}


//}}}
//{{{
// =======================================================================================
// === resetUnhandledExceptionFilter =====================================================
// =======================================================================================

bool cExceptionManager::resetUnhandledExceptionFilter()
{
  if (m_DebugDriver.isValid())
  {
    return m_DebugDriver->resetUnhandledExceptionFilter();
  }
  return false;
}


//}}}


//{{{
// =======================================================================================
// === addFatalCallback ==================================================================
// =======================================================================================

cGuard::ConstPtr QAPI cExceptionManager::addFatalCallback(iFatalCallback::Ptr CallbackToAdd)
{
  // Protect member variables from changes by another thread.
  cLockGuard threadSafety(&m_MemberLock);

  list<iFatalCallback::Ptr>::iterator i = std::find(m_FatalCallbacks.begin(), m_FatalCallbacks.end(), CallbackToAdd);
  if (i != m_FatalCallbacks.end())
  {
    QFATAL((__FILE__, __LINE__, L"Call back 0x%0x already registered as a fatal call back", CallbackToAdd.getRaw()));
  }

  m_FatalCallbacks.push_back(CallbackToAdd);

  return new cGuard(this, CallbackToAdd);
}




//}}}
//{{{
// =======================================================================================
// === addStatusCallback/addWarningCallback/addInformationCallback =======================
// =======================================================================================

cGuard::ConstPtr QAPI cExceptionManager::addStatusCallback(const iStatusCallback::Ptr & callbackToAdd)
{
  // Protect member variables from changes by another thread.
  cLockGuard threadSafety(&m_MemberLock);

  list<iStatusCallback::Ptr>::iterator i = std::find(m_StatusCallbacks.begin(), m_StatusCallbacks.end(), callbackToAdd);
  if (i != m_StatusCallbacks.end())
  {
    QFATAL((__FILE__, __LINE__, L"Call back 0x%0x already registered as a warning call back", callbackToAdd.getRaw()));
  }
  m_StatusCallbacks.push_back(callbackToAdd);
  return new cGuard(this, callbackToAdd);
}

//}}}
//{{{
// ---------------------------------------------------------------------------------------

cGuard::ConstPtr QAPI cExceptionManager::addWarningCallback(const iWarningCallback::Ptr & CallbackToAdd)
{
  cLockGuard threadSafety(&m_MemberLock);

  list<iWarningCallback::Ptr>::iterator i = std::find(m_WarningCallbacks.begin(), m_WarningCallbacks.end(), CallbackToAdd);
  if (i != m_WarningCallbacks.end())
  {
    QFATAL((__FILE__, __LINE__, L"Call back 0x%0x already registered as a warning call back", CallbackToAdd.getRaw()));
  }

  m_WarningCallbacks.push_back(CallbackToAdd);

  for (list<cWarning>::const_iterator j = m_WarningQueue.begin(); j != m_WarningQueue.end(); ++j)
  {
    if (j->m_FeedbackCallback.isValid())
    {
      for (list<iWarningCallback::Ptr>::iterator i = m_WarningCallbacks.begin(); i != m_WarningCallbacks.end(); ++i)
      {
        (*i)->onWarningWait(j->m_FeedbackCallback, j->m_Msg, j->m_FeedbackChoices);
      }
    }
    else
    {
      for (list<iWarningCallback::Ptr>::iterator i = m_WarningCallbacks.begin(); i != m_WarningCallbacks.end(); ++i)
      {
        (*i)->onWarning(j->m_Msg);
      }
    }
  }
  m_WarningQueue.clear();

  return new cGuard(this, CallbackToAdd);
}

//}}}
//{{{
// ---------------------------------------------------------------------------------------

cGuard::ConstPtr QAPI cExceptionManager::addInformationCallback(const iInformationCallback::Ptr & CallbackToAdd)
{
  cLockGuard threadSafety(&m_MemberLock);

  list<iInformationCallback::Ptr>::iterator i = std::find(m_InformationCallbacks.begin(), m_InformationCallbacks.end(), CallbackToAdd);
  if (i != m_InformationCallbacks.end())
  {
    QFATAL((__FILE__, __LINE__, L"Call back 0x%0x already registered as an information call back", CallbackToAdd.getRaw()));
  }

  m_InformationCallbacks.push_back(CallbackToAdd);

  for (list<cWarning>::const_iterator j = m_InformationQueue.begin(); j != m_InformationQueue.end(); ++j)
  {
    for (list<iInformationCallback::Ptr>::iterator i = m_InformationCallbacks.begin(); i != m_InformationCallbacks.end(); ++i)
    {
      (*i)->onInformation(j->m_Msg.c_str());
    }
  }
  m_InformationQueue.clear();

  return new cGuard(this, CallbackToAdd);
}




//}}}

// =======================================================================================
// === Helpers ===========================================================================
// =======================================================================================

namespace
{

//{{{
bool _hasCmdLineOption(const String & Option)
{
  bool Result = false;
  try { (void)iSystemInfo::singleton().getCmdLineOption(Option); Result = true; } catch (cNotFound &) {}
  return Result;
}

//}}}
//{{{
BOOL CALLBACK _EnumWindowsCallback(
  HWND    ThisWindowHandle,   // handle to parent window
  LPARAM  lParam)             // application-defined value
{
  if (GetWindowLong(ThisWindowHandle, GWL_STYLE) & WS_VISIBLE)
  {
    DWORD ThisWindowProcessId;
    GetWindowThreadProcessId(ThisWindowHandle, &ThisWindowProcessId);
    if (ThisWindowProcessId == GetCurrentProcessId())
    {
      HWND * MainWindowHandle = (HWND *)lParam;
      *MainWindowHandle = ThisWindowHandle;
      return FALSE; // Stop enum
    }
  }
  return TRUE; // Continue enum
}

//}}}
//{{{
HWND getMainWindowHandle()
{
  HWND MainWindowHandle = 0;
  (void)EnumWindows(_EnumWindowsCallback, (LPARAM)&MainWindowHandle);
  return MainWindowHandle;
}

//}}}

}

// =======================================================================================
// === Vectored Exception Handling =======================================================
// =======================================================================================

// SJSJSJ This is all WIP

namespace
{

static bool _IsInVectoredExceptionHandler = false;
static bool _HasShownExceptionWithinExceptionMsgBox = false;
//{{{
LONG WINAPI _MyVectoredHandler(PEXCEPTION_POINTERS ep)
{
  cLogIndentSOSGuard Indent(L"Entering vectored exception handler");

  if (_IsInVectoredExceptionHandler)
  {
    if (!_HasShownExceptionWithinExceptionMsgBox)
    {
      _HasShownExceptionWithinExceptionMsgBox = true;
      QMSG((0, L"Exception in vectored exception handler! Caught by vectored exception handler"));
    }
  }
  else
  {
    _IsInVectoredExceptionHandler = true;
    cExceptionManager::singleton()._onVectoredException(GetCurrentThreadId(), ep);
    _IsInVectoredExceptionHandler = false;
    _HasShownExceptionWithinExceptionMsgBox = false;
  }
  QMSG((0, L"Leaving vectored exception handler"));
  return EXCEPTION_CONTINUE_SEARCH;
}
//}}}

}

//{{{
class cVectoredExceptionHandler : public iGuarded, public cRefCount
{
  public :
    cVectoredExceptionHandler(PVOID Handle) : m_Handle(Handle) {}
#if 0
  // XP only
    ~cVectoredExceptionHandler() { if (RemoveVectoredExceptionHandler(m_Handle) == 0) QSOS((L"Failed to RemoveVectoredExceptionHandler()")); }
#endif
  private :
    PVOID m_Handle;
};

//}}}
//{{{
cGuard::ConstPtr cExceptionManager::addVectoredExceptionHandler() throw(cWin32Error)
{
  return new cGuard(this, iGuarded::Ptr());
}

//}}}
//{{{
void cExceptionManager::_onVectoredException(DWORD ThreadID, const EXCEPTION_POINTERS * const ep)
{

}



//}}}

// =======================================================================================
// === logAllThreadsCallStacks/logCurrentThreadsCallStack ================================
// =======================================================================================

//{{{
void QAPI cExceptionManager::logCallStack() const
{
  logCallStack (fStack_ShowBasic);
}
//}}}
//{{{
void QAPI cExceptionManager::logCallStack( unsigned int displayFlags ) const
{
  if (m_DebugDriver.isValid())
  {
    unsigned int threadId = iThreadManager::singleton().getCurrentThreadId();
    cLogIndentGuard indentGuard (kCtgDllMain, L"*** Call Stack for thread %d (%08x) ***", threadId, threadId);
    m_DebugDriver->logCallStack (displayFlags);
  }
  else
  {
    QMSG((0, L"No Debug Driver loaded"));
  }
}
//}}}

//}}}
//{{{
String QAPI cExceptionManager::getDebugHeader( int displayFlags ) const
{
  if (m_DebugDriver.isValid())
  {
    unsigned int threadId = iThreadManager::singleton().getCurrentThreadId();
    cLogIndentGuard indentGuard (kCtgDllMain, L"*** Call Stack for thread %d (%08x) ***", threadId, threadId);
    return m_DebugDriver->getDebugHeader (displayFlags);
  }
  return String(L"No Debug Driver loaded");
}

//}}}
//{{{
String QAPI cExceptionManager::getDebugLine( const void* const addr, int displayFlags ) const
{
  if (m_DebugDriver.isValid())
  {
    unsigned int threadId = iThreadManager::singleton().getCurrentThreadId();
    cLogIndentGuard indentGuard (kCtgDllMain, L"*** Call Stack for thread %d (%08x) ***", threadId, threadId);
    return m_DebugDriver->getDebugLine (addr, displayFlags);
  }
  return String (L"No Debug Driver loaded");
}

//}}}
//{{{
void QAPI cExceptionManager::loadAllSymbols() const
{
  if (m_DebugDriver.isValid())
  {
    cLogIndentGuard indentGuard (kCtgDllMain, L"*** Loading all symbol table entries ***");
    m_DebugDriver->loadAllSymbols();
  }
  else
  {
    QMSG((0, L"No Debug Driver loaded"));
  }
}
//}}}

// =======================================================================================
// === simplifyFilename ==================================================================
// =======================================================================================

namespace
{

//{{{
const Char * _simplifyFilename(const Char * Filename)
{
  Char tmp[256];

  wcsncpy(tmp, Filename, (sizeof(tmp) / sizeof(Char)) - 1);
  Char * start = tmp;
  Char * lastSlash = wcsrchr(tmp, L'\\');
  if (lastSlash)
  {
    start = lastSlash + 1;
    *lastSlash = '\0';
    lastSlash = wcsrchr(tmp, L'\\');
    if (lastSlash) start = lastSlash + 1;
  }

  return Filename + (start - tmp);
}

//}}}
//{{{
String _simplifyAndTruncFilename(const String & SrcFilename)
{
  Char ShortFileName[71] = L"...";
  String SrcFileNameWTrunc(_simplifyFilename(SrcFilename.c_str()));
  if (SrcFileNameWTrunc.size() > 70)
  {
    wcsncpy(&ShortFileName[3], &SrcFileNameWTrunc.c_str()[SrcFileNameWTrunc.size() - 57], 57);
    ShortFileName[70] = L'\0';
    SrcFileNameWTrunc = ShortFileName;
  }
  return SrcFileNameWTrunc;
}
//}}}

}

// =======================================================================================
// === cFatalCallbacksThread =============================================================
// =======================================================================================

//{{{
cExceptionManager::cFatalCallbacksThread::cFatalCallbacksThread(const iFatalCallback::Ptr & Callback, const String & ThreadName) :
  cBasicThread(ThreadName, iBasicThread::ePriority_Normal),
  m_Callback(Callback)
{
  // [GENQ-9014/AJS] Keep process alive while the callback runs
  cLockGuard  safety(&gFatalCallbackLock);
  gShutdownAllowed.reset();
  ++gFatalCallbacksRunning;
}

//}}}
//{{{
// ---------------------------------------------------------------------------------------

void QAPI cExceptionManager::cFatalCallbacksThread::runLoop()
{
  cLogIndentGuard cLogIndentGuard(0, L"Fatal callback \"%s\"", m_Callback->getDescription().c_str());
  cEmergencyProgressMsgGuard progress(m_Callback->getDescription().c_str());
  m_Callback->onFatal();

  // [GENQ-9014/AJS] Release shutdown if we are the last callback to complete
  {
    cLockGuard safety(&gFatalCallbackLock);
    if (--gFatalCallbacksRunning == 0)
      gShutdownAllowed.set();
  }
}



//}}}

// =======================================================================================
// === cVerifyTerminateThread ============================================================
// =======================================================================================

//{{{
cExceptionManager::cFatalTerminateThread::cFatalTerminateThread (const cErrorDialogThread::Ptr & ErrorDialogThread) :
  cBasicThread(L"FatalTerminate", iBasicThread::ePriority_Normal),
  m_ErrorDialogThread(ErrorDialogThread)
{
}

//}}}
//{{{
// ---------------------------------------------------------------------------------------

void QAPI cExceptionManager::cFatalTerminateThread::runLoop()
{
  if (m_ErrorDialogThread.isValid())
  {
    m_ErrorDialogThread->stopLoop(); // Waits for Error Dialog thread to complete
  }

  // [GENQ-9014/AJS] We will wait for any callbacks that might have been started
  (void)gWaitAndTerminateProcess(GetCurrentProcess(), EXIT_FAILURE);
}



//}}}

// =======================================================================================
// === setDialogTitle/getDialogTitle =====================================================
// =======================================================================================

//{{{
void cExceptionManager::setDialogTitle(const String & NewTitle)
{
  cLockGuard ThreadSafety(&m_MemberLock);
  m_DialogTitle = NewTitle;
}

//}}}
//{{{
// ---------------------------------------------------------------------------------------

String cExceptionManager::getDialogTitle() const
{
  cLockGuard ThreadSafety(&m_MemberLock);
  return m_DialogTitle;
}



//}}}

// =======================================================================================
// === cErrorDialogThread ================================================================
// =======================================================================================

//{{{
cExceptionManager::cErrorDialogThread::cErrorDialogThread(const unsigned int Flags, const cFatalDiagnostics & diagnostics, const String & Title) :
    cBasicThread(L"ErrorDialog", iBasicThread::ePriority_Normal),
    m_Flags(Flags),
    m_Diagnostics(diagnostics),
    m_Title(Title),
    m_TimeoutMillisecs(UINT_MAX),
    m_IsAborting(false)
{
}

//}}}
//{{{
// ---------------------------------------------------------------------------------------

cExceptionManager::cErrorDialogThread::cErrorDialogThread(const unsigned int Flags, const cFatalDiagnostics & diagnostics, const String & Title, const unsigned int TimeoutMillisecs) :
    cBasicThread(L"ErrorDialog", iBasicThread::ePriority_Normal),
    m_Flags(Flags),
    m_Diagnostics(diagnostics),
    m_Title(Title),
    m_TimeoutMillisecs(TimeoutMillisecs),
    m_IsAborting(false)
{
}

//}}}
// ---------------------------------------------------------------------------------------

namespace
{
  static HHOOK gMsgBoxHook = 0;
  static HWND  gMsgBoxHwnd = 0;
}

//{{{
LRESULT CALLBACK _OnCBT(int nCode, WPARAM wParam, LPARAM lParam)
{

  if (nCode == HCBT_ACTIVATE)
  {
    gMsgBoxHwnd = (HWND)wParam;
    return 0;
  }
  else
  {
    return CallNextHookEx(gMsgBoxHook, nCode, wParam, lParam);
  }
}

//}}}
//{{{
// ---------------------------------------------------------------------------------------

void QAPI cExceptionManager::cErrorDialogThread::signalLoopStop()
{
  if (m_TimeoutMillisecs != UINT_MAX)
  {
    while (gMsgBoxHwnd == 0) Sleep(100); // Wait for _OnCBT hook to succeed

    int NumMillisecsToGo = m_TimeoutMillisecs;
    while (NumMillisecsToGo > 0 && !m_IsAborting)
    {
      StringStream tmp;
      tmp << m_Title << L" - " << NumMillisecsToGo / 1000;
      SendMessage(gMsgBoxHwnd, WM_SETTEXT, 0, (LPARAM)tmp.str().c_str());
      Sleep(1000);
      NumMillisecsToGo -= 1000;
    }
    SendMessage(gMsgBoxHwnd, WM_CLOSE, 0, 0);
  }
}

//}}}
//{{{
// ---------------------------------------------------------------------------------------

void QAPI cExceptionManager::cErrorDialogThread::runLoop()
{
  gMsgBoxHwnd = 0;
  gMsgBoxHook = SetWindowsHookEx(WH_CBT, _OnCBT, NULL, GetCurrentThreadId() /* Only install for THIS thread */);

  StringStream MsgBody;
  MsgBody << m_Diagnostics.m_MsgHeading;
  MsgBody << L"\n\n" << m_Diagnostics.m_Msg;
  MsgBody << L"\n\nFile: " << m_Diagnostics.m_SrcFilename;
  MsgBody << L"\nLine: " << m_Diagnostics.m_SrcLineNumber;
  (void)ShowCursor(TRUE);
  (void)MessageBox(NULL, MsgBody.str().c_str(), m_Title.c_str(), MB_ICONERROR | MB_TOPMOST | MB_SETFOREGROUND | MB_TASKMODAL);
//  (void)ShowCursor(FALSE);

  (void)UnhookWindowsHookEx(gMsgBoxHook);

  m_IsAborting = true;
}



//}}}

// =======================================================================================
// === reboot ============================================================================
// =======================================================================================

//{{{
void QAPI cExceptionManager::reboot(const Char * const msgFormat, ...)
{
  QVERIFY(msgFormat);

  va_list vaList;
  va_start(vaList, msgFormat);
  _vreboot(msgFormat, vaList);
  va_end(vaList);
}

//}}}
// ---------------------------------------------------------------------------------------

namespace
{

//{{{
void _enableReboot()
{
  HANDLE hProcess = GetCurrentProcess();
  HANDLE hToken;
  if( OpenProcessToken(hProcess, TOKEN_ALL_ACCESS, &hToken) )
  {
    LUID luidShutDown;
    if( LookupPrivilegeValue( NULL, SE_SHUTDOWN_NAME, &luidShutDown ) )
    {
      TOKEN_PRIVILEGES Privilege;
      Privilege.PrivilegeCount = 1;
      Privilege.Privileges[0].Luid = luidShutDown;
      Privilege.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

      AdjustTokenPrivileges(hToken, FALSE, &Privilege, 0, NULL, NULL);
    }
    CloseHandle( hToken );
  }
}
//}}}

}

//{{{
void cExceptionManager::_vreboot(const Char * const msgFormat, va_list vaList)
{
  Char msgBuffer[512];
  _vsnwprintf(msgBuffer, sizeof(msgBuffer) / sizeof(Char), msgFormat, vaList);

  cEmergencyProgressMsgGuard progress(L"%s", msgBuffer);

  {
    cLogIndentSOSGuard IndentGuard(L"*** Reboot ***");
    QMSG((0, L"%s", msgBuffer));

    StringStream MsgBoxBody;
    MsgBoxBody << L"Reboot required.\n\nReason: " << msgBuffer;
    MsgBoxBody << L"\n\nDo you want to reboot now?\nSelect \"No\" if you intend to reboot later - you will not be able to run this software successfully until you do so.";
    (void)ShowCursor(TRUE);
    const int MsgBoxResult = MessageBox(getMainWindowHandle(), MsgBoxBody.str().c_str(), m_DialogTitle.c_str(), MB_YESNO | MB_ICONQUESTION | MB_TOPMOST | MB_SETFOREGROUND | MB_TASKMODAL);
//    (void)ShowCursor(FALSE);
    QMSG((0, L"Choice = %d", MsgBoxResult));
    if (cLogManager::isSingletonConstructed()) cLogManager::singleton().flushBuffer();

    if (MsgBoxResult == IDYES)
    {
      // Restart Windows
      _enableReboot();

      InitiateSystemShutdownEx(NULL, // Machine name
                               NULL, // Msg
                               0, // Timeout
                               FALSE, // Force apps close?
                               TRUE, // Reboot after shutdown?
                               SHTDN_REASON_MAJOR_OPERATINGSYSTEM | SHTDN_REASON_MINOR_UPGRADE | SHTDN_REASON_FLAG_PLANNED); // Reason
    }

    try
    {
      iDLLManager::singleton().getInstance(L"vfs", L"_KernelExec")->setCustomSettingNumber(L"", L"Is reboot required?", 1);
    }
    catch (cNotFound &) {}

    // [GENQ-9014/AJS] We will terminate immediately as no callbacks have been started.
    //                 Should callbacks be started?
    (void)TerminateProcess(GetCurrentProcess(), EXIT_SUCCESS);
  }
}



//}}}

// =======================================================================================
// === fatal =============================================================================
// =======================================================================================

//{{{
void cExceptionManager::presentFatal(const char * const SrcFilename, const unsigned long  SrcLineNumber, const Char * const msgFormat, ...)
{
  QVERIFY(SrcFilename);
  QVERIFY(msgFormat);

  va_list MsgParams;
  va_start(MsgParams, msgFormat);
  fatal(fFatal_ShowAll | fFatal_IsAnError, cFatalDiagnostics(EXIT_FAILURE, SrcFilename, SrcLineNumber, L"Fatal Error", msgFormat, MsgParams));
  va_end(MsgParams);
}

//}}}
//{{{
// ----------------------------------------------------------------------------------------

void QAPI cExceptionManager::fatal(const unsigned int Flags, const cFatalDiagnostics & diagnostics) // throw cAttemptedFatal
{
  //QTRACE((L"cExceptionManager::fatal(const unsigned int Flags, const cFatalDiagnostics & diagnostics)"));

  cEmergencyProgressMsgGuard::Ptr progress, progress2;
  if (diagnostics.m_MsgHeading != L"") progress = new cEmergencyProgressMsgGuard(L"%s", diagnostics.m_MsgHeading.c_str());
  if (diagnostics.m_Msg != L"") progress2 = new cEmergencyProgressMsgGuard(L"%s", diagnostics.m_Msg.c_str());

  // --- Perhaps we are throwing an exception instead of crashing? -----------------------

  if (_isThrowingExceptionOnFatal())
  {
    // put up a callstack now otherwise we don't know where the attempted fatal comes from.
    logCallStack();
    //
    throw cAttemptedFatal(
      narrow(diagnostics.m_SrcFilename).c_str(),
      diagnostics.m_SrcLineNumber,
      L"%s", diagnostics.m_Msg.c_str());
  }

  // --- Present external processes with opportunity to do a mini dump ------------------

  signalMiniDumpPoint();

  // --- Describe exit code -------------------------------------------------------------

  String ExitCodeDesc;
  if (diagnostics.m_ExitCode == EXIT_SUCCESS)
  {
    iBootManager::singleton().setState(iBootManager::eBootState_CleanExiting);
    ExitCodeDesc = L"(EXIT_SUCCESS)";
  }
  else if (diagnostics.m_ExitCode == EXIT_FAILURE)
  {
    iBootManager::singleton().setState(iBootManager::eBootState_Crashing);
    ExitCodeDesc = L"(EXIT_FAILURE)";
  }

  // --- Log the error msg ---------------------------------------------------------------

  {
    if ((Flags & fFatal_ShowMsg) != 0)
    {
      cLogIndentSOSGuard IndentGuard(L"*** %s ***", diagnostics.m_MsgHeading.c_str());
      iLogManager::bufferError(L"%s", diagnostics.m_Msg.c_str());
      if ((Flags & fFatal_ShowSrcLocation) != 0)
      {
        QMSG((0, L"File: %s", diagnostics.m_SrcFilename.c_str()));
        QMSG((0, L"Line: %d", diagnostics.m_SrcLineNumber));
      }
      QMSG((0,L"Exit code: %d %s", diagnostics.m_ExitCode, ExitCodeDesc.c_str()));
    }

    if (cLogManager::isSingletonConstructed()) cLogManager::singleton().flushBuffer();

  // --- Debugger break? -----------------------------------------------------------------

    /*
    Ended up here in the debugger? Want to know what has caused this error?
    1. We are inside fatal() so examine the call stack to see who called us
    2. Examine the variable diagnostics.m_Msg
    */
    if ((Flags & fFatal_ShowMsg) != 0)
    {
      OutputDebugString(L"vfs Kernel: ");
      OutputDebugString(diagnostics.m_MsgHeading.c_str());
      OutputDebugString(L": ");
      OutputDebugString(diagnostics.m_Msg.c_str());
      OutputDebugString(L"\n");
      if ((Flags & fFatal_ShowSrcLocation) != 0)
      {
        OutputDebugString(L"File: ");
        OutputDebugString(diagnostics.m_SrcFilename.c_str());
        OutputDebugString(L"\n");
        OutputDebugString(L"Line: ");
        StringStream tmp; tmp << diagnostics.m_SrcLineNumber;
        OutputDebugString(tmp.str().c_str());
        OutputDebugString(L"\n");
      }

      if (IsDebuggerPresent() || _hasCmdLineOption(L"jit"))
#ifdef _M_X64
        __debugbreak();
#else
        __asm int 3
#endif
    }

    if ((Flags & fFatal_ShowStack) != 0)
    {
      unsigned int threadId = iThreadManager::singleton().getCurrentThreadId();
      cLogIndentSOSGuard indentGuard (L"*** Call Stack for thread %d (%08x) ***", threadId, threadId);
      if (m_DebugDriver.isValid())
        m_DebugDriver->logCallStack (fStack_ShowAll);
      else
        QMSG((0, L"No Debug Driver loaded"));
    }
  // --- Pop up MessageBox ---------------------------------------------------------------

    static bool IsAlreadyHandlingAFatal = false;
    if (IsAlreadyHandlingAFatal || !iThreadManager::isSingletonConstructed())
    {
      if ((Flags & fFatal_ShowMsg) != 0)
      {
        if (_isPausingOnCrash() && !_hasCmdLineOption(L"silentcrash"))
        {
          _doMsgBox(Flags, diagnostics);
        }
      }

      // [GENQ-9014/AJS] This will remain a hard terminate as an error has occurred
      //                 whilst we are already dealing with an error. All bets are
      //                 off now.
      (void)TerminateProcess(GetCurrentProcess(), diagnostics.m_ExitCode);
    }
    else
    {
      if ((Flags & fFatal_ShowMsg) != 0)
      {
        IsAlreadyHandlingAFatal = true;

        cEmergencyProgressMsgGuard progress(L"Cleaning up");

        cPtr<cErrorDialogThread>  ErrorDialogThread;
        vector<cGuard::ConstPtr>  FatalCallbacksThreadGuards;
        cGuard::ConstPtr          FatalTerminateThreadGuard;

        if (_isPausingForeverOnCrash())
        {
          ErrorDialogThread = new cErrorDialogThread(Flags, diagnostics, m_DialogTitle);
        }
        else
        {
          if (!_isPausingOnCrash() || _hasCmdLineOption(L"silentcrash"))
          {
            FatalCallbacksThreadGuards = startFatalCallbacks();

            // [GENQ-9014/AJS] We will wait for the callbacks to complete and then terminate
            (void)gWaitAndTerminateProcess(GetCurrentProcess(), diagnostics.m_ExitCode);
          }
          else
          {
            QMSG((0, L"Presenting error dialog for %d seconds", m_ErrorDialogTimeoutMillisecs / 1000));
            ErrorDialogThread = new cErrorDialogThread(Flags, diagnostics, m_DialogTitle, m_ErrorDialogTimeoutMillisecs);
          }
        }
        ErrorDialogThread->startLoop();
        FatalTerminateThreadGuard = iThreadManager::singleton().startThread(new cFatalTerminateThread(ErrorDialogThread));

        FatalCallbacksThreadGuards = startFatalCallbacks();

        //if ((Flags & fFatal_ShowStack) != 0)
        //{
          //cExceptionManager::singleton().logCallStack(GetCurrentThreadId());//, L"cExceptionManager::fatal()");
        //}

        Sleep (INFINITE);  // cFatalTerminateThread waits for Error Dialog thread to complete, then calls TerminateProcess
      }
      else
      {
        unsigned int n = 0;
        for (list<iFatalCallback::Ptr>::iterator i = m_FatalCallbacks.begin(); i != m_FatalCallbacks.end(); ++i)
        {
          QMSG((0, L"Starting fatal callback \"%s\"", (*i)->getDescription().c_str()));
          StringStream Name;
          Name << L"FatalCallback" << n++;
          (*i)->onFatal();
        }
        if (cLogManager::isSingletonConstructed()) cLogManager::singleton().flushBuffer();

        // [GENQ-9014/AJS] The callbacks have been run sequentially so we can terminate immediately
        (void)TerminateProcess(GetCurrentProcess(), diagnostics.m_ExitCode);
      }
    }
  }
}



//}}}

//{{{
// =======================================================================================
// === _doMsgBox =========================================================================
// =======================================================================================

unsigned int cExceptionManager::_doMsgBox(const unsigned int Flags, const cFatalDiagnostics & diagnostics)
{
  StringStream DialogBody;
  if (diagnostics.m_MsgHeading != L"") DialogBody << diagnostics.m_MsgHeading << L"\n\n";
  if (diagnostics.m_Msg != L"") DialogBody << diagnostics.m_Msg << L"\n\n";
  if ((Flags & fFatal_ShowSrcLocation) != 0) DialogBody << L"File: " << _simplifyAndTruncFilename(diagnostics.m_SrcFilename) << L"\nLine: " << diagnostics.m_SrcLineNumber;

  unsigned int DialogFlags = MB_TOPMOST | MB_SETFOREGROUND | MB_TASKMODAL;
  DialogFlags |= ((Flags & fFatal_IsAnError) != 0) ? MB_ICONERROR : MB_ICONWARNING;

  (void)ShowCursor(TRUE);
  const unsigned int Result = MessageBox(getMainWindowHandle(), DialogBody.str().c_str(), m_DialogTitle.c_str(), DialogFlags);
//  (void)ShowCursor(FALSE);

  return Result;
}



//}}}
//{{{
// =======================================================================================
// === startFatalCallbacks ===============================================================
// =======================================================================================

vector<cGuard::ConstPtr> cExceptionManager::startFatalCallbacks()
{
  vector<cGuard::ConstPtr> Results;
  unsigned int n = 0;

  for (list<iFatalCallback::Ptr>::iterator i = m_FatalCallbacks.begin(); i != m_FatalCallbacks.end(); ++i)
  {
    QMSG((0, L"Starting fatal callback \"%s\"", (*i)->getDescription().c_str()));
    StringStream Name;
    Name << L"FatalCallback" << n++;
    Results.push_back(iThreadManager::singleton().startThread(new cFatalCallbacksThread(*i, Name.str())));
  }
  return Results;
}



//}}}

//{{{
// =======================================================================================
// === presentWarningWait ================================================================
// =======================================================================================

class cSimpleWarningWaitCallback : public iExceptionManager::iWarningWaitCallback, public cRefCount
{
  public :
    void QAPI onChoice(const unsigned int ChoiceNum, const String & Choice) {}
};

void QARGS_STACK cExceptionManager::presentWarningWait(const Char * const msgFormat, ...)
{
  QVERIFY(msgFormat);

  vector<String> FeedbackChoices(1);
  FeedbackChoices.at(0) = L"Continue";

  va_list vaList;
  va_start(vaList, msgFormat);
  _vpresentWarning(new cSimpleWarningWaitCallback, FeedbackChoices, msgFormat, vaList);
  va_end(vaList);
}

//}}}
//{{{
// ---------------------------------------------------------------------------------------

void QARGS_STACK cExceptionManager::presentWarningWait(
  const iWarningWaitCallback::Ptr & FeedbackCallback,
  const vector<String> & FeedbackChoices,
  const Char * const msgFormat,
  ...)
{
  QVERIFY(FeedbackCallback.isValid());
  QVERIFY(FeedbackChoices.size() > 0);
  QVERIFY(msgFormat);

  va_list vaList;
  va_start(vaList, msgFormat);
  _vpresentWarning(FeedbackCallback, FeedbackChoices, msgFormat, vaList);
  va_end(vaList);
}



//}}}

// =======================================================================================
// === presentWarning ====================================================================
// =======================================================================================

//{{{
void QAPI cExceptionManager::presentWarning(const Char * const msgFormat, ...)
{
  QVERIFY(msgFormat);

  va_list vaList;
  va_start(vaList, msgFormat);
  _vpresentWarning(iWarningWaitCallback::Ptr(), vector<String>(), msgFormat, vaList);
  va_end(vaList);
}

//}}}
//{{{
// ---------------------------------------------------------------------------------------

void cExceptionManager::_vpresentWarning(
  iWarningWaitCallback::Ptr FeedbackCallback,
  const vector<String> &    FeedbackChoices,
  const Char * const        msgFormat,
  va_list                   vaList)
{
  // Protect member variables from changes by another thread.
  cLockGuard threadSafety(&m_MemberLock);

  Char msgBuffer[512];
  _vsnwprintf(msgBuffer, sizeof(msgBuffer) / sizeof(Char), msgFormat, vaList);

  cLogIndentGuard IndentGuard(kCtgPresentWarning, L"*** Warning Presented ***");

  QMSG((0, L"%s", msgBuffer));

  if (m_WarningCallbacks.size() == 0)
  {
    m_WarningQueue.push_back(cWarning(FeedbackCallback, FeedbackChoices, msgBuffer));
  }
  else
  {
    if (FeedbackCallback.isValid())
    {
      for (list<iWarningCallback::Ptr>::iterator i = m_WarningCallbacks.begin(); i != m_WarningCallbacks.end(); ++i)
      {
        (*i)->onWarningWait(FeedbackCallback, msgBuffer, FeedbackChoices);
      }
    }
    else
    {
      for (list<iWarningCallback::Ptr>::iterator i = m_WarningCallbacks.begin(); i != m_WarningCallbacks.end(); ++i)
      {
        (*i)->onWarning(msgBuffer);
      }
    }
  }
}



//}}}

// =======================================================================================
// === presentStatus =====================================================================
// =======================================================================================

//{{{
void QARGS_STACK cExceptionManager::presentStatus(const Char * const msgFormat, ...)
{
  QVERIFY(msgFormat);

  va_list vaList;
  va_start(vaList, msgFormat);
  Char msgBuffer[512];
  _vsnwprintf(msgBuffer, sizeof(msgBuffer) / sizeof(Char), msgFormat, vaList);

  QMSG((kCtgPresentStatus, L"presentStatus: %s", msgBuffer));

  list<iStatusCallback::Ptr> copyOfCallbacks;
  {
    cLockGuard threadSafety(&m_MemberLock);
    copyOfCallbacks = m_StatusCallbacks;
  }
  for (list<iStatusCallback::Ptr>::iterator i = m_StatusCallbacks.begin(); i != m_StatusCallbacks.end(); ++i)
  {
    (*i)->onStatus(msgBuffer);
  }

  va_end(vaList);
}



//}}}

// =======================================================================================
// === presentInformation/addInformationCallback =========================================
// =======================================================================================

//{{{
void QARGS_STACK cExceptionManager::presentInformation(const Char * const msgFormat, ...)
{
  QVERIFY(msgFormat);

  va_list vaList;
  va_start(vaList, msgFormat);
  _vpresentInformation(msgFormat, vaList);
  va_end(vaList);
}

//}}}
//{{{
// ---------------------------------------------------------------------------------------

void cExceptionManager::_vpresentInformation(const Char * const msgFormat, va_list vaList)
{
  // Protect member variables from changes by another thread.
  cLockGuard threadSafety(&m_MemberLock);

  Char msgBuffer[512];
  _vsnwprintf(msgBuffer, sizeof(msgBuffer) / sizeof(Char), msgFormat, vaList);

  cLogIndentGuard IndentGuard(0, L"*** Information Presented ***");

  QMSG((0, L"%s", msgBuffer));

  if (m_InformationCallbacks.size() == 0)
  {
    m_InformationQueue.push_back(cWarning(iWarningWaitCallback::Ptr(), vector<String>(), msgBuffer));
  }
  else
  {
    for (list<iInformationCallback::Ptr>::iterator i = m_InformationCallbacks.begin(); i != m_InformationCallbacks.end(); ++i)
    {
      (*i)->onInformation(msgBuffer);
    }
  }
}



//}}}

// =======================================================================================
// === onGuardDestroyed ==================================================================
// =======================================================================================

//{{{
void QAPI cExceptionManager::onGuardDestroyed(iGuarded::Ptr guarded)
{
  // Protect member variables from changes by another thread.
  cLockGuard threadSafety(&m_MemberLock);

  // Either a warning or fatal callback.
  if (ptr_cast<iFatalCallback::Ptr>(guarded).isValid())
  {
    m_FatalCallbacks.remove(ptr_cast<iFatalCallback::Ptr>(guarded));
  }
  else if (ptr_cast<iWarningCallback::Ptr>(guarded).isValid())
  {
    m_WarningCallbacks.remove(ptr_cast<iWarningCallback::Ptr>(guarded));
  }
  else if (ptr_cast<iStatusCallback::Ptr>(guarded).isValid())
  {
    m_StatusCallbacks.remove(ptr_cast<iStatusCallback::Ptr>(guarded));
  }
  else if (ptr_cast<iInformationCallback::Ptr>(guarded).isValid())
  {
    m_InformationCallbacks.remove(ptr_cast<iInformationCallback::Ptr>(guarded));
  }
  else if (ptr_cast<cExceptionOnFatalGuard::Ptr>(guarded).isValid())
  {
    cLockGuard threadSafety(&m_ExceptionOnFatalGuardsThreadSafety);
    m_ExceptionOnFatalGuards.remove(ptr_cast<cExceptionOnFatalGuard::Ptr>(guarded));
  }
  else
  {
    QFATAL((__FILE__, __LINE__, L"Unrecognised guard object passed to onGuardDestroyed()"));
  }
}



//}}}

// =======================================================================================
// === enableExceptionOnFatal ============================================================
// =======================================================================================

//{{{
cGuard::ConstPtr QAPI cExceptionManager::enableExceptionOnFatal()
{
  cLockGuard threadSafety(&m_ExceptionOnFatalGuardsThreadSafety);
  cExceptionOnFatalGuard::Ptr guarded = new cExceptionOnFatalGuard(iThreadManager::singleton().getCurrentThreadId());
  m_ExceptionOnFatalGuards.push_back(guarded);
  return new cGuard(this, guarded);
}

//}}}
//{{{
bool cExceptionManager::_isThrowingExceptionOnFatal() const
{
  cLockGuard threadSafety(&m_ExceptionOnFatalGuardsThreadSafety);
  const unsigned int currThreadId = iThreadManager::singleton().getCurrentThreadId();
  for (list<cExceptionOnFatalGuard::Ptr>::const_iterator i = m_ExceptionOnFatalGuards.begin(); i != m_ExceptionOnFatalGuards.end(); ++i)
  {
    if ((*i)->m_ThreadId == currThreadId) return true;
  }
  return false;
}



//}}}

// =======================================================================================
// === winExceptionCodeToString ==========================================================
// =======================================================================================

#define EXCEPTION_MSVC ((DWORD)0xE06D7363L)

//{{{
String QAPI cExceptionManager::convertExceptionRecord (EXCEPTION_RECORD* ex, cRecoverable*& recoverablePtr) const
// NOTE that this gets used from cDebugDriver in KernelDebugDriver project.
{
  // assume not a cRecoverable first.
  recoverablePtr = NULL;

  const Char* exceptionString;

  switch (ex->ExceptionCode)
  {
  case EXCEPTION_ACCESS_VIOLATION:          exceptionString = L"EXCEPTION_ACCESS_VIOLATION"; break;
  case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:     exceptionString = L"EXCEPTION_ARRAY_BOUNDS_EXCEEDED"; break;
  case EXCEPTION_BREAKPOINT:                exceptionString = L"EXCEPTION_BREAKPOINT"; break;
  case EXCEPTION_DATATYPE_MISALIGNMENT:     exceptionString = L"EXCEPTION_DATATYPE_MISALIGNMENT"; break;
  case EXCEPTION_FLT_DENORMAL_OPERAND:      exceptionString = L"EXCEPTION_FLT_DENORMAL_OPERAND"; break;
  case EXCEPTION_FLT_DIVIDE_BY_ZERO:        exceptionString = L"EXCEPTION_FLT_DIVIDE_BY_ZERO"; break;
  case EXCEPTION_FLT_INEXACT_RESULT:        exceptionString = L"EXCEPTION_FLT_INEXACT_RESULT"; break;
  case EXCEPTION_FLT_INVALID_OPERATION:     exceptionString = L"EXCEPTION_FLT_INVALID_OPERATION"; break;
  case EXCEPTION_FLT_OVERFLOW:              exceptionString = L"EXCEPTION_FLT_OVERFLOW"; break;
  case EXCEPTION_FLT_STACK_CHECK:           exceptionString = L"EXCEPTION_FLT_STACK_CHECK"; break;
  case EXCEPTION_FLT_UNDERFLOW:             exceptionString = L"EXCEPTION_FLT_UNDERFLOW"; break;
  case EXCEPTION_GUARD_PAGE:                exceptionString = L"EXCEPTION_GUARD_PAGE"; break;
  case EXCEPTION_ILLEGAL_INSTRUCTION:       exceptionString = L"EXCEPTION_ILLEGAL_INSTRUCTION"; break;
  case EXCEPTION_IN_PAGE_ERROR:             exceptionString = L"EXCEPTION_IN_PAGE_ERROR"; break;
  case EXCEPTION_INT_DIVIDE_BY_ZERO:        exceptionString = L"EXCEPTION_INT_DIVIDE_BY_ZERO"; break;
  case EXCEPTION_INT_OVERFLOW:              exceptionString = L"EXCEPTION_INT_OVERFLOW"; break;
  case EXCEPTION_INVALID_DISPOSITION:       exceptionString = L"EXCEPTION_INVALID_DISPOSITION"; break;
  case EXCEPTION_INVALID_HANDLE:            exceptionString = L"EXCEPTION_INVALID_HANDLE"; break;
  case EXCEPTION_NONCONTINUABLE_EXCEPTION:  exceptionString = L"EXCEPTION_NONCONTINUABLE_EXCEPTION"; break;
  case EXCEPTION_PRIV_INSTRUCTION:          exceptionString = L"EXCEPTION_PRIV_INSTRUCTION"; break;
  case EXCEPTION_SINGLE_STEP:               exceptionString = L"EXCEPTION_SINGLE_STEP"; break;
  case EXCEPTION_STACK_OVERFLOW:            exceptionString = L"EXCEPTION_STACK_OVERFLOW"; break;

  case EXCEPTION_MSVC:                      exceptionString = L"unknown C++ exception"; break;
  case EXCEPTION_vfs_FATAL:             exceptionString = L"QVERIFY FAILED"; break;

  default:
    {
      StringStream ss;
      ss << L"unhandled exception 0x" << std::hex << std::setw(8) << std::setfill(L'0') << ex->ExceptionCode;
      return ss.str();
    }
  }

#ifdef _M_X64
  if (ex->ExceptionCode == EXCEPTION_MSVC && ex->NumberParameters == 4)
  {
    // Sources: http://blogs.msdn.com/b/oldnewthing/archive/2010/07/30/10044061.aspx, http://www.openrce.org/articles/full_view/21
    // and http://www.hexblog.com/wp-content/uploads/2012/06/Recon-2012-Skochinsky-Compiler-Internals.pdf

    struct TypeInfo
    {
      void* pvfnTable;
      void* reserved;
      char name[1];
    };

    struct CatchableType
    {
      DWORD properties;
      DWORD /* TypeInfo* */ pType;
      LONG thisDisplacement;
      //int sizeOrOffset;
      //void* copyFunction;
    };

    struct CatchableTypeArray
    {
      int nCatchableTypes;
      DWORD /* CatchableType* */ arrayOfCatchableTypes[1];
    };

    struct ThrowInfo
    {
      DWORD attributes;
      DWORD /* void* */ pmfnUnwind;
      DWORD /* int* */ pForwardCompat;
      DWORD /* CatchableTypeArray* */ pCatchableTypeArray;
    };

    ULONG_PTR module = ex->ExceptionInformation[3];
    ThrowInfo* throwInfo = (ThrowInfo*) ex->ExceptionInformation[2];

    CatchableTypeArray* catchableTypeArray = (CatchableTypeArray*) (module + throwInfo->pCatchableTypeArray);

    if (catchableTypeArray->nCatchableTypes > 0)
    {
      // scan decorated names of all classes in hierarchy to see if the exception type is a cRecoverable
      for (int i = 0; i < catchableTypeArray->nCatchableTypes; i++)
      {
        CatchableType* catchableType = (CatchableType*) (module + catchableTypeArray->arrayOfCatchableTypes[i]);
        TypeInfo* typeInfo = (TypeInfo*) (module + catchableType->pType);
        // do an exact check for a cRecoverable decorated name
        if (std::string(typeInfo->name) == ".?AVcRecoverable@vfs@@")
        {
          // Good this is a recoverable exception or inherits therefrom
          recoverablePtr = reinterpret_cast<cRecoverable*>(ex->ExceptionInformation[1] + catchableType->thisDisplacement);
          // return most derived class name - hence typeid(*recoverablePtr) and message
          return widen(typeid(*recoverablePtr).name()) + L" exception. \"" + recoverablePtr->getMsg() + L"\"";
        }
      }

      // not a cRecoverable exception
      CatchableType* catchableType = (CatchableType*) (module + catchableTypeArray->arrayOfCatchableTypes[0]);
      TypeInfo* typeInfo = (TypeInfo*) (module + catchableType->pType);
      return L"C++ exception \"" + widen(typeInfo->name) + L"\"";
    }
  }
#endif
  if (ex->ExceptionCode == EXCEPTION_ACCESS_VIOLATION || ex->ExceptionCode == EXCEPTION_IN_PAGE_ERROR)
  {
    // Source: http://msdn.microsoft.com/en-us/library/windows/desktop/aa363082(v=vs.85).aspx

    StringStream s;
    s << exceptionString;

    switch (ex->ExceptionInformation[0])
    {
    case 0: s << L" reading "; break;
    case 1: s << L" writing "; break;
    case 8: s << L" executing "; break;
    default: s << L" at ";
    }

    s << L"0x" << std::hex << std::setw(8) << std::setfill(L'0') << ex->ExceptionInformation[1];

    if (ex->ExceptionCode == EXCEPTION_IN_PAGE_ERROR)
    {
      StringStream ss;
      ss << L"NTSTATUS code: " 
        << std::dec << ex->ExceptionInformation[2] 
        << L"[0x" << std::hex << std::setw(8) << std::setfill(L'0') << ex->ExceptionInformation[2] << L"]";

      s << L" " << ss.str();
    }

    return s.str();
  }
  return exceptionString;
}
//}}}

