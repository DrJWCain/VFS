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

//{{{  headers
#include "stdafx.h"
#include "cDebugDriver.h"

#include "..\..\..\kernel\KernelExec\cThreadImpl.h" // So we can peek Thread name
#include "cThreadManager.h"
#include "..\..\..\kernel\KernelExec\cThreadManager.h"
#include "..\..\..\kernel\KernelExec\cExceptionManager.h"
//}}}

using namespace vfs;

namespace
{
  const bool kDebug = false;
  const int kMaxFrames = 100;

  // The typedefs for the PSAPI.DLL functions used by this module.
  typedef BOOL (WINAPI *ENUMPROCESSMODULES) ( HANDLE hProcess, HMODULE* lphModule, DWORD cb, LPDWORD lpcbNeeded );
  typedef DWORD (WINAPI *GETMODULEBASENAME) ( HANDLE hProcess, HMODULE hModule, LPSTR lpBaseName, DWORD nSize );
  typedef BOOL (WINAPI *GETMODULEINFORMATION) ( HANDLE hProcess, HMODULE hModule, LPMODULEINFO lpModuleInfo, DWORD cb );

  bool g_bInitialized = false;
  ENUMPROCESSMODULES g_pEnumProcessModules = NULL;       // The pointer to EnumProcessModules.
  GETMODULEBASENAME g_pGetModuleBaseName = NULL;         // The pointer to GetModuleBaseName.
  GETMODULEINFORMATION g_pGetModuleInformation = NULL;   // The pointer to GetModuleInformation.


  // helpers
  //{{{
  const char* simplifyStackTraceString(const char* str)  // return only last parts  ie dirname\filename.ext
  {
    const char* lastSlash = strrchr(str, '\\');
    if (lastSlash)
    {
      for (const char* pch = lastSlash - 1; pch > str; --pch)
      {
        if (*(pch - 1) == '\\')
          return pch;
      }
    }

    return str;
  }
  //}}}
  //{{{
  String toStringWin32ErrCode (const long win32ErrCode)
  {
    Char * formatMessageBuffer = 0;
      (void)FormatMessage(
          FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
          NULL,
          win32ErrCode,
          MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
          (LPTSTR)&formatMessageBuffer,
          0,
          NULL);

    const String errorMessage(formatMessageBuffer);
    LocalFree(formatMessageBuffer);

    return errorMessage;
  }
  //}}}
  //{{{
  int memset_s (char* dest, int destLen, char ch, int numChars)  // returns numChars inserted
  {
    if (numChars > destLen - 1)
      numChars = destLen - 1;
    memset (dest, ch, numChars);
    dest[numChars] = 0;
    return numChars;
  }
  //}}}
  //{{{
  int errorMessage_s (char* dest, int destLen, const long win32ErrCode) // returns numChars inserted
  {
    Char * formatMessageBuffer = 0;
    FormatMessage(
          FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
          NULL,
          win32ErrCode,
          MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
          (LPTSTR)&formatMessageBuffer,
          0,
          NULL);
    int count = sprintf_s (dest, destLen, "%ls", formatMessageBuffer);
    LocalFree( formatMessageBuffer );
    return count;
  }
  //}}}
  //iActivity::Ptr gActivity;
  //{{{
  class cStackFrame
  {
  public:
    cStackFrame()
      : mAddrPC(0), mParam0(0), mParam1(0), mParam2(0), mParam3(0) {}
    cStackFrame (DWORD64 addr)
      : mAddrPC(addr), mParam0(0), mParam1(0), mParam2(0), mParam3(0) {}
    cStackFrame (DWORD64 addr, const DWORD64* params)
      : mAddrPC(addr), mParam0(params[0]), mParam1(params[1]), mParam2(params[2]), mParam3(params[3]) {}

    DWORD64 mAddrPC;
    DWORD64 mParam0;
    DWORD64 mParam1;
    DWORD64 mParam2;
    DWORD64 mParam3;
  };
  //}}}
  //typedef std::vector<cStackFrame> tStackFrameVector;
  tStackFrameVector gStackTraceVector(kMaxFrames);

  typedef std::vector<HMODULE> tModuleVector;
  tModuleVector gLoadedModules;
  //{{{
  bool modulePredicate (const HMODULE& p1, const HMODULE& p2)
  {
    // Highest address first ordering !! - makes lower_bound find matching or next lowest address
    return (p1 > p2);
  }
  //}}}

  #define SYM_BUFF_SIZE 512
  #define LINE_BUFF_SIZE 1024
  // variables allocated here to avoid them being put on the stack during a crashLog. Usage protected by mDbgHelpLock
  CONTEXT gContext;
  STACKFRAME64 gStackFrame;
  IMAGEHLP_LINE64 gImagehlpLine;
  IMAGEHLP_MODULE64 gModuleInfo;
  BYTE gSymbolBuffer [SYM_BUFF_SIZE];
  char gLineBuffer [LINE_BUFF_SIZE];
  wchar_t gLineBufferW[LINE_BUFF_SIZE];
  char gPathname[MAX_PATH];

  
  //--------------------------------------------------------------------------------------------------------------------
  // cCmdHandlers
  //{{{
  class cStackCmdHandler : public iCmdConsoleManager::iCommand, public cRefCount
  {
  public:

    cStackCmdHandler (cDebugDriver& driver) : mDriver(driver) { }

    String QAPI getName() const throw()
    {
      return L"stack";
    }

    void QAPI logOneLineDescription() const
    {
      QMSG((0, L"Dumps call stack information for all threads or a specified Thread."));
    }

    void QAPI logDetailedDescription() const
    {
      logOneLineDescription();
    }

    void QAPI logOneLineParams() const
    {
      // Do not include /? - Kernel will add this automatically. Please use DOS style for
      // consistency - see "dir /?" from you command line.
      QMSG((0, L"[thread_id] [/all]"));
    }

    void QAPI logDetailedParams() const
    {
    // Do not include /? - Kernel will add this automatically. Align descriptions in by
    // 16 chars. Please use DOS style for consistency - see "dir /?" from you command line.
      QMSG((0, L"thread_id      Dumps specified threads call stack"));
      QMSG((0, L"/all           Dumps all threads call stacks - this is the default"));
    }

    void QAPI execute (const String& params) const
    {
      if ((params == L"/all") || (params == L""))
        mDriver.logAllThreadsCallStacks();
      else
      {
        unsigned long threadID = 0;
        swscanf (params.c_str(), L"%x", &threadID);
        if (threadID == 0)
        {
          QMSG((0, L"\"%s\" is not a Thread ID", params.c_str()));
        }
        else
          mDriver.logCallStack (threadID);
      }
    }

  private:
    cDebugDriver& mDriver;
  };
  //}}}
  //{{{
  class cModulesCmdHandler : public iCmdConsoleManager::iCommand, public cRefCount
  {
  public:

    cModulesCmdHandler (cDebugDriver& driver) : mDriver(driver) { }

    String QAPI getName() const throw()
    {
      return L"modules";
    }

    void QAPI logOneLineDescription() const
    {
      QMSG((0, L"Lists all loaded modules."));
    }

    void QAPI logDetailedDescription() const
    {
      logOneLineDescription();
    }

    void QAPI logOneLineParams() const
    {
    // Do not include /? - Kernel will add this automatically. Please use DOS style for
    // consistency - see "dir /?" from you command line.
    }

    void QAPI logDetailedParams() const
    {
    // Do not include /? - Kernel will add this automatically. Align descriptions in by
    // 16 chars. Please use DOS style for consistency - see "dir /?" from you command line.
    }

    void QAPI execute (const String& params) const
    {
      mDriver.logAllModules();

      cLogSuppressGuard g; // stop the full debug being printed out while we refresh module flags
      iLogManager::singleton().refreshModuleFlags();
    }

  private:
    cDebugDriver& mDriver;
  };
  //}}}
  //{{{
  class cThreadsCmdHandler : public iCmdConsoleManager::iCommand, public cRefCount
  {
  public:

    cThreadsCmdHandler (cDebugDriver& driver) : mDriver(driver) { }

    String QAPI getName() const throw()
    {
      return L"cputime";
    }

    void QAPI logOneLineDescription() const
    {
      QMSG((0, L"Lists all threads CPU usage in Kernel process."));
    }

    void QAPI logDetailedDescription() const
    {
      logOneLineDescription();
    }

    void QAPI logOneLineParams() const
    {
      // Do not include /? - Kernel will add this automatically. Please use DOS style for
      // consistency - see "dir /?" from you command line.
      QMSG((0, L"[/delay ms] [/ms]"));
    }

    void QAPI logDetailedParams() const
    {
      // Do not include /? - Kernel will add this automatically. Align descriptions in by
      // 16 chars. Please use DOS style for consistency - see "dir /?" from you command line.
      QMSG((0, L"/delay ms       Will show the CPU usage across specified time"));
      QMSG((0, L"/ms             Results in millisecs as opposed to percentages"));
    }

    void QAPI execute (const String& params) const
    {
      bool isPercentages = true;
      if (params.find (L"/ms") != String::npos)
        isPercentages = false;

      unsigned long delay;
      const unsigned int fieldsFound = swscanf (params.c_str(), L"/delay %d", &delay);
      if (fieldsFound == 1)
      {
        {
          cLogIndentGuard g(0, L"First run");
          mDriver.logAllThreadsInfo (isPercentages);
        }
        QMSG((0, L"Waiting %d millisecs", delay));
        Sleep (delay);
        {
          cLogIndentGuard g(0, L"Second run");
          mDriver.logAllThreadsInfo (isPercentages);
        }
      }
      else
        mDriver.logAllThreadsInfo (isPercentages);
    }

  private:
    cDebugDriver& mDriver;
  };
  //}}}
}

//----------------------------------------------------------------------------------------------------------------------
// Construction & Destruction
//{{{
cDebugDriver::cDebugDriver() throw()
  : mIsInitialised(false),
    mFilenameWidth(35),
    mRoutineWidth(50)
{
  cLogIndentGuard IndentGuard(0, L"Creating DebugDriver");

  resetUnhandledExceptionFilter();

  // The false parameter means we can get called on any Thread.
  mStackConsoleCmdGuard   = iCmdConsoleManager::singleton().addCommand(new cStackCmdHandler(*this),   false);
  mModulesConsoleCmdGuard = iCmdConsoleManager::singleton().addCommand(new cModulesCmdHandler(*this), false);
  mThreadsConsoleCmdGuard = iCmdConsoleManager::singleton().addCommand(new cThreadsCmdHandler(*this), false);
}
//}}}
//{{{
cDebugDriver::~cDebugDriver()
{
  cLogIndentGuard IndentGuard(0, L"Deleting cDebugDriver at 0x%p", this);

  if (mIsInitialised)
  {
    cLockGuard g (&mDbgHelpLock);
    SymCleanup (GetCurrentProcess());
  }
  // Non-throwing functions should :
  // * Never throw anything!
  // * Gracefully handle bogus parameters and half initialised data structures.

  mStackConsoleCmdGuard.invalidate();
  mModulesConsoleCmdGuard.invalidate();
  mThreadsConsoleCmdGuard.invalidate();
}


//}}}
//
//{{{
void cDebugDriver::getFaultReason( DWORD threadID, const EXCEPTION_POINTERS* const ep, iExceptionManager::cFatalDiagnostics& pDiagToLoad)
{
  if (TRUE == IsBadReadPtr( ep , sizeof(EXCEPTION_POINTERS) ))
  {
    QSOS ((L"Bad parameter to GetFaultReason()")) ;
    return;
  }

  try
  {
    cLockGuard g (&mDbgHelpLock);

    initialise();

    StringStream ss;
    ss << L"In thread " << threadID << L", ";

    HMODULE hmod = getModule (ep->ExceptionRecord->ExceptionAddress);
    if (hmod)
    {
      GetModuleBaseName(GetCurrentProcess(), hmod, gLineBufferW, LINE_BUFF_SIZE);
      ss << gLineBufferW;
    }
    else
      ss << L"<UNKNOWN>";

    cRecoverable* recoverablePtr = NULL;
    ss << L" caused a " << cExceptionManager::singleton().convertExceptionRecord(ep->ExceptionRecord, recoverablePtr);
    pDiagToLoad.m_Msg = ss.str();

    // Print out full info to log if we got a cRecoverable
    if (recoverablePtr)
    {
      pDiagToLoad.m_SrcFilename = recoverablePtr->getSrcFilename();
      pDiagToLoad.m_SrcLineNumber = recoverablePtr->getSrcLineNumber();
      // make sure get full info in the log if we have a cRecoverable
      QSOS((L"%s", pDiagToLoad.m_Msg.c_str()));
      QSOS((L"File: %s", pDiagToLoad.m_SrcFilename.c_str()));
      QSOS((L"Line: %d", pDiagToLoad.m_SrcLineNumber));
      QSOS((L""));
    }

    loadSymbols (ep->ExceptionRecord->ExceptionAddress);

    DWORD dwOffset;
    ZeroMemory( &gImagehlpLine, sizeof( IMAGEHLP_LINE64 ) );
    gImagehlpLine.SizeOfStruct = sizeof( IMAGEHLP_LINE64 );

    if ( SymGetLineFromAddr64( GetCurrentProcess(), (DWORD64)ep->ExceptionRecord->ExceptionAddress, &dwOffset, &gImagehlpLine ))
    {
      const char* filename = simplifyStackTraceString (gImagehlpLine.FileName);
      const int kFilenameWidth = 50;

      int len = (int)strlen (filename);
      if (len > kFilenameWidth)
      {
        filename += (len - kFilenameWidth) + 2;
        pDiagToLoad.m_SrcFilename = L".." + widen (filename);
      }
      else
        pDiagToLoad.m_SrcFilename = widen (filename);

      pDiagToLoad.m_SrcLineNumber = gImagehlpLine.LineNumber;
    }
    else
    {
      QSOS((L"SymGetLineFromAddr64 failed for address : %0Ix", (size_t)ep->ExceptionRecord->ExceptionAddress));
      DWORD err = GetLastError();
      QSOS((L"%d [0x%08x] \"%s\"", err, err, toStringWin32ErrCode(err).c_str()));
    }
  }
  catch (cRecoverable & Reason)
  {
    QSOS((L"GetFaultReason failed (%s)", Reason.getMsg().c_str()));
  }
}

//}}}
//{{{
map<unsigned int, iThread::Ptr> QAPI cDebugDriver::getAllThreads()
{
  QSOS((L"Stubbed %S", __FUNCTION__));
  return map<unsigned int, iThread::Ptr>();
}

//}}}
//{{{
void QAPI cDebugDriver::logCallStack( unsigned int displayFlags )
{
  if (kDebug)
    QTRACE((L"cDebugDriver::logCallStack(unsigned int displayFlags)"));

  if (!mIsInitialised)
    initialise();

  cLockGuard g (&mDbgHelpLock);

  ZeroMemory( &gContext, sizeof( CONTEXT ) );
  gContext.ContextFlags = CONTEXT_FULL;
  RtlCaptureContext( &gContext );

  if (displayFlags & fStack_ShowRegisters)
  {
    logRegisters (&gContext);
    QMSG((0, L""));
  }

  logStackWalk (displayFlags);    // input = gContext
}
//}}}
//{{{
void QAPI cDebugDriver::logCallStack( DWORD threadID, const EXCEPTION_POINTERS* const ep )
{
  if (kDebug)
    QTRACE((L"cDebugDriver::logCallStack(DWORD ThreadID, const EXCEPTION_POINTERS* const ep)"));

  //cThreadManagerEx::singleton().unhandledException(ep);
  if (!mIsInitialised)
    initialise();

  cLockGuard g (&mDbgHelpLock);

  ZeroMemory( &gContext, sizeof( CONTEXT ) );
  CopyMemory( &gContext, ep->ContextRecord, sizeof( CONTEXT ) );

  int displayFlags = fStack_ShowAll;

  if (displayFlags & fStack_ShowRegisters)
  {
    logRegisters (&gContext);
    QMSG((0, L""));
  }
  logStackWalk (displayFlags);    // input = gContext
}

//}}}
//{{{
void QAPI cDebugDriver::loadAllSymbols()
{
  if (kDebug)
    QTRACE((L"cDebugDriver::loadSymbols()"));

  if (!mIsInitialised)
    initialise();

  cLockGuard g (&mDbgHelpLock);

  DWORD dwOpts = SymGetOptions();               // Set up the symbol engine.
  SymSetOptions (dwOpts | SYMOPT_LOAD_LINES);   // Turn on load lines.

  // Initialize the symbol engine.
  QVERIFYMSG( SymInitialize( GetCurrentProcess(), NULL, FALSE ), (L"cDebugManager::initialiseSymbols: SymInitialize failed") );

  // Find out how many modules there are.
  UINT uiCount;
  QVERIFYMSG( getLoadedModules( GetCurrentProcessId(), 0, NULL, &uiCount ), (L"cDebugManager::initialiseSymbols: GetLoadedModules failed") );

  if (kDebug)
    QMSG((0, L"cDebugManager::initialiseSymbols : %d modules", uiCount));

  // Allocate something big enough to hold the list.
  vector<HMODULE> moduleHandles(uiCount);

  // Get the list for real.
  QVERIFYMSG( getLoadedModules( GetCurrentProcessId(), uiCount, &moduleHandles.at(0), &uiCount ), (L"cDebugManager::initialiseSymbols: GetLoadedModules failed") );

  char szModName[MAX_PATH]; // The module filename.
  for ( UINT uiCurr = 0; uiCurr < uiCount; uiCurr++ )
  {
    HMODULE hmod = moduleHandles.at(uiCurr);
    if ( GetModuleFileNameA( hmod, szModName, sizeof( szModName )  ) ) // Get the module's filename.
    {
      if ( SymLoadModuleEx( GetCurrentProcess(), NULL, szModName, NULL, (DWORD64)hmod, 0, NULL, 0 ) )  // let dbgHelp use its search path
      {
        QMSG((0, L"Loaded symbols for \"%hs\"", szModName));
      }
      else
      {
        DWORD err = GetLastError();
        if (err == ERROR_SUCCESS)
          QMSG((0, L"Already loaded for \"%hs\"", szModName));
        else
          QSOS(( L"cDebugManager::initialiseSymbols: SymLoadModule failed for \"%hs\" : %s", szModName, toStringWin32ErrCode(err).c_str() ));
      }
    }
    else
    {
      QSOS(( L"cDebugManager::initialiseSymbols: GetModuleFileNameA failed for \"%hs\" : %s", szModName, toStringWin32ErrCode(GetLastError()).c_str() ));
    }
  }
}
//}}}
//{{{
String QAPI cDebugDriver::getDebugHeader( int displayFlags )
{
  cLockGuard g (&mDbgHelpLock);

  loadDebugHeader (displayFlags);
  return widen (gLineBuffer);
}
//}}}
//{{{
String QAPI cDebugDriver::getDebugLine( const void* const addr, int displayFlags )
{
  cLockGuard g (&mDbgHelpLock);

  loadSymbols (addr);

  ZeroMemory (&gStackFrame, sizeof (STACKFRAME64));
  gStackFrame.AddrPC.Offset = (DWORD64)addr;

  loadDebugLine (displayFlags);
  return widen (gLineBuffer);
}
//}}}
//{{{
void QAPI cDebugDriver::logAllThreadsCallStacks() throw()
{
  QSOS((L"cDebugDriver::logAllThreadsCallStacks() - not available"));
  //cThreadManagerEx::singleton().logAllThreadsStacks();
}

//}}}
//{{{
void QAPI cDebugDriver::logCallStack( unsigned long ThreadID, int displayFlags ) throw()
{
  QSOS((L"cDebugDriver::logCallStack(unsigned long ThreadID, int displayFlags) - not available"));
}

//}}}
//{{{
//void QAPI cDebugDriver::logCallStack( unsigned long ThreadID, const String & AfterFunc) throw()
//{
  //cLockGuard g (&mDbgHelpLock);

  //if (kDebug)
    //QTRACE((L"cDebugDriver::logCallStack(DWORD ThreadID, const String & AfterFunc)"));
  ////cThreadManagerEx::singleton().logCallStack(ThreadID, AfterFunc);
  //if (!mIsInitialised)
    //initialise();


  //ZeroMemory( &gContext, sizeof( CONTEXT ) );
  //gContext.ContextFlags = CONTEXT_FULL;
  //RtlCaptureContext( &gContext );

  //int displayFlags = eShowAddress | eShowFile | eShowOffset | eShowRoutine | eShowParams
                   //| eShowModule | eShowRegisters | eShowErrors | eShowNofile;

  //if (displayFlags & eShowRegisters)
  //{
    //logRegisters (&gContext);
    //QMSG((0, L""));
  //}
  //logStackWalk (displayFlags);    // input = gContext
//}

//}}}
//{{{
void QAPI cDebugDriver::logAllThreadsInfo(const bool IsPercentages) throw()
{
  cThreadManagerEx::singleton().logAllThreadsTiming(IsPercentages);
}

//}}}
//{{{
bool QAPI cDebugDriver::resetUnhandledExceptionFilter() throw()
{
  return cThreadManagerEx::singleton().resetUnhandledExceptionFilter();
}

//}}}
//{{{
void QAPI cDebugDriver::logAllModules() throw()
{
  using namespace std;

  HMODULE modules[0x400];
  unsigned long bytesUsed = 0;
  EnumProcessModules(GetCurrentProcess(),modules,0x400,&bytesUsed);
  unsigned long numModules = bytesUsed / sizeof(HMODULE);

  sort(&modules[0],&modules[numModules]);

  cLogIndentGuard guard(0, L"  MemRange-------------  Size----  ------------BaseName  ------------------------------------------FileName  FileDate----------------");

  QMSG ((0, L""));

  Char name[400];
  for(unsigned long count = 0; count < numModules; ++count)
  {
    StringStream buf;
    size_t module = (size_t)modules[count];
    buf << L"0x" << hex << setfill(L'0')  << setw(8) << module << L"-";

    MODULEINFO modInfo;
    GetModuleInformation(GetCurrentProcess(),modules[count],&modInfo,sizeof(MODULEINFO));
    buf << L"0x" << hex << setfill(L'0') << setw(8) << module+modInfo.SizeOfImage << L"  ";

    buf << dec << setfill(L' ') << setw(7) << modInfo.SizeOfImage / 1024 << L"K  ";

    GetModuleBaseName(GetCurrentProcess(),modules[count],name,400);
    buf << setw(20) << setfill(L' ') << name << L"  ";

    GetModuleFileNameEx(GetCurrentProcess(),modules[count],name,400);
    const int namelen = (int)wcslen(name);
    if (namelen > 50)
    {
      name[namelen - 50] = L'.';
      name[namelen - 49] = L'.';
      name[namelen - 48] = L'.';
      buf << setw(50) << setfill(L' ') << &name[namelen - 50] << L"  ";
    }
    else
    {
      buf << setw(50) << setfill(L' ') << name << L"  ";
    }

    time_t fileTime = GetTimestampForLoadedLibrary(modules[count]);
    buf << widen(ctime(&fileTime)) << endl;

    QMSG((0, L"%s", buf.str().c_str()));
 }
}
//}}}
//{{{
void cDebugDriver::initialise()
{
  if (mIsInitialised)
    return;

  cLockGuard g (&mDbgHelpLock);

  DWORD dwOpts = SymGetOptions();              // Set up the symbol engine.
  SymSetOptions (dwOpts | SYMOPT_LOAD_LINES);  // Turn on load lines.

  // Initialize the symbol engine.
  QVERIFYMSG( SymInitialize( GetCurrentProcess(), NULL, FALSE ), (L"cDebugDriver::initialise: SymInitialize failed") );

  // Find out how many modules there are.
  UINT uiCount;
  QVERIFYMSG( getLoadedModules( GetCurrentProcessId(), 0, NULL, &uiCount ), (L"cDebugDriver::initialise: GetLoadedModules failed") );

  if (kDebug)
    QTRACE((L"cDebugDriver::initialise : %d modules", uiCount));

  // Allocate something big enough to hold the list.
  vector<HMODULE> moduleHandles(uiCount);

  // Get the list for real.
  QVERIFYMSG( getLoadedModules( GetCurrentProcessId(), uiCount, &moduleHandles.at(0), &uiCount ), (L"cDebugDriver::initialise: GetLoadedModules failed") );

  //{{{  build sorted module list - high to low
  for ( UINT uiCurr = 0; uiCurr < uiCount; uiCurr++ )
  {
    HMODULE hmod = moduleHandles.at(uiCurr);
    tModuleVector::iterator foundIt = lower_bound (gLoadedModules.begin(), gLoadedModules.end(), hmod, modulePredicate);

    if ((foundIt == gLoadedModules.end()) || (*foundIt != hmod))
      gLoadedModules.insert (foundIt, hmod);
  }
  //}}}

  mIsInitialised = true;

  //if (SymGetSearchPath (GetCurrentProcess(), gLineBuffer, LINE_BUFF_SIZE / 2))
    //QTRACE((L"Symbol Search Path : %hs",  gLineBuffer));

  //Char* buf = (Char*)gLineBuffer;
  //if (GetCurrentDirectory (LINE_BUFF_SIZE / 2, buf))
    //QTRACE((L"Current Directory  : %s",  buf));
}
//}}}
//----------------------------------------------------------------------------------------------------------------------
// privates
//{{{
HMODULE cDebugDriver::getModule (const void* const addr)  // which contans this address - search moduleVector
{
  if (!mIsInitialised)
    initialise();

  cLockGuard g (&mDbgHelpLock);

  tModuleVector::iterator it = lower_bound (gLoadedModules.begin(),  gLoadedModules.end(), (HMODULE)addr, modulePredicate);

  if (it != gLoadedModules.end())
    return *it;

  return 0;
}
//}}}
//{{{
void cDebugDriver::loadSymbols (const void* const addr)   // for the module which contains this address
{
  cLockGuard g (&mDbgHelpLock);

  HMODULE hmod = getModule (addr);

  if (hmod)
  {
    char filename[MAX_PATH];

    if ( GetModuleFileNameA( hmod, filename, MAX_PATH  ) )
    {
      char pdbname[MAX_PATH];
      strcpy (pdbname, filename);
      char* dot = strrchr (pdbname, '.');
      if (dot && (strcmp (dot, ".dll") == 0))
        strcpy (dot, ".pdb");

      if ( SymLoadModuleEx( GetCurrentProcess(), NULL, pdbname, NULL, (DWORD64)hmod, 0, NULL, 0 ) )        // try for the pdb first! (it should have DllSize set)
      {
        if (kDebug)
          QTRACE((L"Loaded Symbols : %hs", pdbname ));
      }
      else if ( SymLoadModuleEx( GetCurrentProcess(), NULL, filename, NULL, (DWORD64)hmod, 0, NULL, 0 ) )  // let dbgHelp use its search path
      {
        if (kDebug)
          QTRACE((L"Loaded Symbols : %hs", filename ));
      }
      else if (GetLastError() != ERROR_SUCCESS)
      {
        QSOS(( L"cDebugManager::loadSymbols SymLoadModule failed for \"%hs\" : %s", filename, toStringWin32ErrCode(GetLastError()).c_str() ));
      }
      else if (kDebug)
      {
        QTRACE((L"Symbols already loaded for : %hs", filename ));
      }
    }
    else
    {
      QSOS((L"cDebugManager::loadSymbols GetModuleFileNameA failed %p : %s", addr, toStringWin32ErrCode(GetLastError()).c_str() ));
    }
  }
}
//}}}
//{{{
bool cDebugDriver::initPSAPI()
{
  if (!g_bInitialized)
  {
    // Load up PSAPI.DLL.
    HINSTANCE hInst = LoadLibraryA ( "PSAPI.DLL" );
    QVERIFY ( NULL != hInst );
    if ( NULL == hInst )
    {
      QSOS(( L"Unable to load PSAPI.DLL!\n" ));
      return false;
    }

    // Now do the GetProcAddress stuff.
    g_pEnumProcessModules = (ENUMPROCESSMODULES)GetProcAddress ( hInst, "EnumProcessModules" );
    QVERIFY( NULL != g_pEnumProcessModules );
    if ( NULL == g_pEnumProcessModules )
    {
      QSOS(( L"GetProcAddress failed on EnumProcessModules!" ));
      return false;
    }

    g_pGetModuleBaseName = (GETMODULEBASENAME)GetProcAddress ( hInst, "GetModuleBaseNameA" );
    QVERIFY( NULL != g_pGetModuleBaseName );
    if ( NULL == g_pGetModuleBaseName )
    {
      QSOS(( L"GetProcAddress failed on GetModuleBaseNameA!" ));
      return false;
    }

    g_pGetModuleInformation = (GETMODULEINFORMATION)GetProcAddress ( hInst, "GetModuleInformation" );
    QVERIFY( NULL != g_pGetModuleInformation );
    if ( NULL == g_pGetModuleInformation )
    {
      QSOS(( L"GetProcAddress failed on GetModuleInformation!" ));
      return false;
    }

    // All OK, Jumpmaster!
    g_bInitialized = TRUE;
  }
  return true;
}
//}}}
//{{{
bool cDebugDriver::getLoadedModules ( DWORD dwPID, UINT uiCount, HMODULE* paModArray, LPUINT puiRealCount )
{
  // Initialize PSAPI.DLL, if needed.
  if ( !initPSAPI() )
  {
    QVERIFY( FALSE );
    SetLastErrorEx( ERROR_DLL_INIT_FAILED, SLE_ERROR );
    return false;
  }

  // Convert the process ID into a process handle.
  HANDLE hProc = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPID );
  QVERIFY( NULL != hProc );
  if ( NULL == hProc )
  {
    QSOS(( L"Unable to OpenProcess on %08X\n" , dwPID ));
    return false;
  }

  // Now get the modules for the specified process.
  QVERIFY( NULL != g_pEnumProcessModules );
  // Because of possible DLL unload order differences, make sure that
  //  PSAPI.DLL is still loaded in case this function is called during
  //  shutdown.
  QVERIFY( FALSE == IsBadCodePtr( (FARPROC)g_pEnumProcessModules ) );
  if ( TRUE == IsBadCodePtr( (FARPROC)g_pEnumProcessModules ) )
  {
    QSOS(( L"PSAPI.DLL has been unloaded on us!\n" ));

    // Close the process handle used.
    QVERIFY( CloseHandle (hProc) );

    SetLastErrorEx( ERROR_INVALID_DLL, SLE_ERROR );

    return false;
  }

  DWORD dwTotal = 0;
  BOOL bRet = g_pEnumProcessModules( hProc, paModArray, uiCount* sizeof( HMODULE ), &dwTotal );

  // Close the process handle used.
  QVERIFY( CloseHandle ( hProc ) );

  // Convert the count from bytes to HMODULE values.
  *puiRealCount = dwTotal / sizeof( HMODULE );

  // If bRet was FALSE, and the user was not just asking for the
  //  total, there was a problem.
  if ( (( FALSE == bRet ) && ( uiCount > 0 )) || ( 0 == dwTotal ) )
  {
    QVERIFY( FALSE );
    QSOS(( L"EnumProcessModules failed!\n" ));
    return false;
  }

  // If the total returned in puiRealCount is larger than the value in
  //  uiCount, then return an error.  If uiCount is zero, then it is
  //  not an error.
  if ( (*puiRealCount > uiCount) && (uiCount > 0) )
  {
    QVERIFY( FALSE );
    QSOS(( L"Buffer is too small in GetLoadedModules!" ));
    SetLastErrorEx( ERROR_INSUFFICIENT_BUFFER, SLE_ERROR );
    return false;
  }

  // All OK, Jumpmaster!
  SetLastError( ERROR_SUCCESS );
  return true;
}
//}}}
// these load the gLineBuffer - hence avoid heap or stack allocation
//{{{
void cDebugDriver::loadDebugHeader (int displayFlags)
{
  // must be called with mDbgHelpLock taken

  int lineIndex = 0;
  gLineBuffer[0] = 0;

  if (displayFlags & fStack_ShowAddress)
  {
    int extraDigits = (int)sizeof(size_t) * 2 - 8;
    if (extraDigits > 0)
      lineIndex += memset_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, '-', extraDigits);

    lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "-Address ");
  }

  if (displayFlags & fStack_ShowFile)
  {
    if (mFilenameWidth > 10)
      lineIndex += memset_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, '-', mFilenameWidth - 10);

    lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "SourceFile (line) ");

    if (displayFlags & fStack_ShowOffset)
      lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "offset ");
  }

  if (displayFlags & fStack_ShowFunction)
  {
    if (mRoutineWidth > 8)
      lineIndex += memset_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, '-', mRoutineWidth - 8);

    lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "FuncName   ");   // 2 extra trailing spaces in place of "()"

    if (displayFlags & fStack_ShowOffset)
      lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "offset ");
  }

  if (displayFlags & fStack_ShowParams)
  {
    lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "Parameters---");  // 3 '-' for the 3 gaps between params

    int extraDigits = (int)sizeof(size_t) * 8 - 10;
    lineIndex += memset_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, '-', extraDigits);

    lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, " ");
  }

  if (displayFlags & fStack_ShowModule)
  {
    lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "Module----------");
  }
}
//}}}
//{{{
void cDebugDriver::loadDebugLine (int displayFlags)   // uses gStackFrame
{
  // must be called with mDbgHelpLock taken & the symbols loaded

  DWORD64 addr64 = gStackFrame.AddrPC.Offset;

  int lineIndex = 0;
  gLineBuffer[0] = 0;
  bool hasError = false;

  if (displayFlags & fStack_ShowAddress)
    lineIndex = sprintf_s (gLineBuffer, SYM_BUFF_SIZE, "%p ", (size_t)addr64);

  //{{{
  if (displayFlags & fStack_ShowFile)
  {
    DWORD dwOffset;
    ZeroMemory( &gImagehlpLine, sizeof( IMAGEHLP_LINE64 ) );
    gImagehlpLine.SizeOfStruct = sizeof( IMAGEHLP_LINE64 );
    //{{{
    if ( SymGetLineFromAddr64( GetCurrentProcess(), addr64, &dwOffset, &gImagehlpLine ))
    {
      const char* filename = simplifyStackTraceString (gImagehlpLine.FileName);

      int len = (int)strlen (filename);
      if (len > mFilenameWidth)
      {
        filename += (len - mFilenameWidth) + 2;
        lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "..%*s (%-4d) ", mFilenameWidth - 2, filename, gImagehlpLine.LineNumber);
      }
      else
        lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "%*s (%-4d) ", mFilenameWidth, filename, gImagehlpLine.LineNumber);

      if (displayFlags & fStack_ShowOffset)
      {
        if (dwOffset > 0)
          lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "+ %-4d ", dwOffset);
        else
          lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "       ");
      }
    }
    //}}}
    //{{{
    else
    {
      //DWORD errCode = GetLastError();

      int curIndex = lineIndex;

      if ((displayFlags & fStack_ShowAddress) == 0)  // force showing of the address
        lineIndex = sprintf_s (gLineBuffer, LINE_BUFF_SIZE, "%p ", addr64);

      int used = lineIndex - curIndex;

      gPathname[0] = 0;
      const char* filename = gPathname;

      if ((displayFlags & fStack_ShowModule) == 0)  // show module here, if not showing it later
      {
        HMODULE hmod = getModule ((const void* const)addr64);
        if (hmod)
        {
          if ( GetModuleFileNameA( hmod, gPathname, MAX_PATH  ) )
          {
            const char* slash = strrchr (gPathname, '\\');
            if (slash)
              filename = slash + 1;
          }
        }
      }

      lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "%*s        ", mFilenameWidth - used, filename);

      if (displayFlags & fStack_ShowOffset)
          lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "       ");

      hasError = true;
    }
    //}}}
  }
  //}}}
  //{{{
  if (displayFlags & fStack_ShowFunction)
  {
    DWORD64 dwDisp64;
    ZeroMemory ( gSymbolBuffer, SYM_BUFF_SIZE ); // This avoids buffer overrun static checking

    PIMAGEHLP_SYMBOL64 pSym = (PIMAGEHLP_SYMBOL64)&gSymbolBuffer;
    pSym->SizeOfStruct = sizeof( IMAGEHLP_SYMBOL64 );
    pSym->MaxNameLength = SYM_BUFF_SIZE - sizeof( IMAGEHLP_SYMBOL64 );

    //{{{
    if ( SymGetSymFromAddr64( GetCurrentProcess(), addr64, &dwDisp64, pSym ) )
    {
      const char* routineName = pSym->Name;
      int len = (int)strlen (routineName);
      if (len > mRoutineWidth)
      {
        routineName += (len - mRoutineWidth) + 2;
        lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "..%*s() ", mRoutineWidth - 2, routineName);
      }
      else
        lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "%*s() ", mRoutineWidth, routineName);

      if (displayFlags & fStack_ShowOffset)
      {
        if (dwDisp64 > 0)
          lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "+ %-4d ", dwDisp64);
        else
          lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "       ");
      }
    }
    //}}}
    //{{{
    else
    {
      int count = errorMessage_s( gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, GetLastError() );
      lineIndex += count;

      int padding = mRoutineWidth + 3 - count;
      if (displayFlags & fStack_ShowOffset)
        padding += 7;

      if ( padding > 0)
        lineIndex += memset_s( gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, ' ', padding );

      hasError = true;
    }
    //}}}
  }
  //}}}

  //{{{
  if (displayFlags & fStack_ShowParams)
  {
    lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "%p %p %p %p ",
      (void*)gStackFrame.Params[0],
      (void*)gStackFrame.Params[1],
      (void*)gStackFrame.Params[2],
      (void*)gStackFrame.Params[3] );
  }
  //}}}
  //{{{
  if (displayFlags & fStack_ShowModule)
  {
    const void* const addr = (const void* const)gStackFrame.AddrPC.Offset;
    HMODULE hmod = getModule (addr);
    if (hmod)
    {
      if ( GetModuleFileNameA( hmod, gPathname, MAX_PATH  ) )
      {
        const char* filename = gPathname;
        const char* lastSlash = strrchr (gPathname, '\\');
        if (lastSlash)
          filename = lastSlash + 1;

        lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "%s", filename);
      }
    }
  }
  //}}}
}
//}}}
//{{{
void cDebugDriver::loadDebugLine (const cStackFrame& stackframe, int displayFlags)
{
  // must be called with mDbgHelpLock taken & the symbols loaded

  DWORD64 addr64 = stackframe.mAddrPC;
  const void* const addr = (const void* const)stackframe.mAddrPC;

  int lineIndex = 0;
  gLineBuffer[0] = 0;
  bool hasError = false;

  if (displayFlags & fStack_ShowAddress)
    lineIndex = sprintf_s (gLineBuffer, SYM_BUFF_SIZE, "%p ", (size_t)addr64);

  //{{{
  if (displayFlags & fStack_ShowFile)
  {
    DWORD dwDisp;
    ZeroMemory( &gImagehlpLine, sizeof( IMAGEHLP_LINE64 ) );
    gImagehlpLine.SizeOfStruct = sizeof( IMAGEHLP_LINE64 );
    //{{{
    if ( SymGetLineFromAddr64( GetCurrentProcess(), addr64, &dwDisp, &gImagehlpLine ))
    {
      const char* filename = simplifyStackTraceString (gImagehlpLine.FileName);

      int len = (int)strlen (filename);
      if (len > mFilenameWidth)
      {
        filename += (len - mFilenameWidth) + 2;
        lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "..%*s (%-4d) ", mFilenameWidth - 2, filename, gImagehlpLine.LineNumber);
      }
      else
        lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "%*s (%-4d) ", mFilenameWidth, filename, gImagehlpLine.LineNumber);

      if (displayFlags & fStack_ShowOffset)
      {
        if (dwDisp > 0)
          lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "+ %-4d ", dwDisp);
        else
          lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "       ");
      }
    }
    //}}}
    //{{{
    else
    {
      int curIndex = lineIndex;

      if ((displayFlags & fStack_ShowAddress) == 0)  // force showing of the address
        lineIndex = sprintf_s (gLineBuffer, LINE_BUFF_SIZE, "%p ", addr64);

      int used = lineIndex - curIndex;

      gPathname[0] = 0;
      const char* filename = gPathname;

      if ((displayFlags & fStack_ShowModule) == 0)  // show module here, if not showing it later
      {
        HMODULE hmod = getModule ((const void* const)addr64);
        if (hmod)
        {
          if ( GetModuleFileNameA( hmod, gPathname, MAX_PATH  ) )
          {
            const char* slash = strrchr (gPathname, '\\');
            if (slash)
              filename = slash + 1;
          }
        }
      }

      lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "%*s        ", mFilenameWidth - used, filename);

      if (displayFlags & fStack_ShowOffset)
          lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "       ");

      hasError = true;
    }
    //}}}
  }
  //}}}
  //{{{
  if (displayFlags & fStack_ShowFunction)
  {
    DWORD64 dwDisp64;
    ZeroMemory ( gSymbolBuffer, SYM_BUFF_SIZE ); // This avoids buffer overrun static checking

    PIMAGEHLP_SYMBOL64 pSym = (PIMAGEHLP_SYMBOL64)&gSymbolBuffer;
    pSym->SizeOfStruct = sizeof( IMAGEHLP_SYMBOL64 );
    pSym->MaxNameLength = SYM_BUFF_SIZE - sizeof( IMAGEHLP_SYMBOL64 );

    //{{{
    if ( SymGetSymFromAddr64( GetCurrentProcess(), addr64, &dwDisp64, pSym ) )
    {
      const char* routineName = pSym->Name;
      int len = (int)strlen (routineName);
      if (len > mRoutineWidth)
      {
        routineName += (len - mRoutineWidth) + 2;
        lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "..%*s() ", mRoutineWidth - 2, routineName);
      }
      else
        lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "%*s() ", mRoutineWidth, routineName);

      if (displayFlags & fStack_ShowOffset)
      {
        if (dwDisp64 > 0)
          lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "+ %-4d ", dwDisp64);
        else
          lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "       ");
      }
    }
    //}}}
    //{{{
    else
    {
      int count = errorMessage_s( gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, GetLastError() );
      lineIndex += count;

      int padding = mRoutineWidth + 3 - count;
      if (displayFlags & fStack_ShowOffset)
        padding += 7;

      if ( padding > 0)
        lineIndex += memset_s( gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, ' ', padding );

      hasError = true;
    }
    //}}}
  }
  //}}}

  //{{{
  if (displayFlags & fStack_ShowParams)
  {
    lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "%p %p %p %p ",
      (void*)stackframe.mParam0,
      (void*)stackframe.mParam1,
      (void*)stackframe.mParam2,
      (void*)stackframe.mParam3 );
  }
  //}}}
  //{{{
  if (displayFlags & fStack_ShowModule)
  {
    HMODULE hmod = getModule (addr);
    if (hmod)
    {
      if ( GetModuleFileNameA( hmod, gPathname, MAX_PATH  ) )
      {
        const char* filename = gPathname;
        const char* lastSlash = strrchr (gPathname, '\\');
        if (lastSlash)
          filename = lastSlash + 1;

        lineIndex += sprintf_s (gLineBuffer + lineIndex, LINE_BUFF_SIZE - lineIndex, "%s", filename);
      }
    }
  }
  //}}}
}
//}}}
//{{{  bool cDebugDriver::captureStackTrace( tStackTraceVector& stackTraceVector )
/*++
  Routine Description:
    Capture the stack trace based on the given CONTEXT.

  Parameters:
    Context    Thread context. If NULL, the current context is
               used.
    Event      Event structure to populate.
    MaxFrames  Max number of frames the structure can hold.
--*/
#ifdef _M_IX86
  #pragma optimize( "g", off )      // Disable global optimization
  #pragma warning( push )
  #pragma warning( disable : 4748 ) // ignore /GS warning caused by inline assembly.
#endif
//{{{
bool cDebugDriver::captureStackTrace( tStackFrameVector& stackTraceVector )
{
  stackTraceVector.clear();

  // Set up stack frame.
  DWORD machineType;

  ZeroMemory( &gStackFrame, sizeof( STACKFRAME64 ) );
#ifdef _M_IX86
  machineType                 = IMAGE_FILE_MACHINE_I386;
  gStackFrame.AddrPC.Offset    = gContext.Eip;
  gStackFrame.AddrPC.Mode      = AddrModeFlat;
  gStackFrame.AddrFrame.Offset = gContext.Ebp;
  gStackFrame.AddrFrame.Mode   = AddrModeFlat;
  gStackFrame.AddrStack.Offset = gContext.Esp;
  gStackFrame.AddrStack.Mode   = AddrModeFlat;
#elif _M_X64
  machineType                 = IMAGE_FILE_MACHINE_AMD64;
  gStackFrame.AddrPC.Offset    = gContext.Rip;
  gStackFrame.AddrPC.Mode      = AddrModeFlat;
  gStackFrame.AddrFrame.Offset = gContext.Rbp;
  gStackFrame.AddrFrame.Mode   = AddrModeFlat;
  gStackFrame.AddrStack.Offset = gContext.Rsp;
  gStackFrame.AddrStack.Mode   = AddrModeFlat;
#elif _M_IA64
  machineType                 = IMAGE_FILE_MACHINE_IA64;
  gStackFrame.AddrPC.Offset    = gContext.StIIP;
  gStackFrame.AddrPC.Mode      = AddrModeFlat;
  gStackFrame.AddrFrame.Offset = gContext.IntSp;
  gStackFrame.AddrFrame.Mode   = AddrModeFlat;
  gStackFrame.AddrBStore.Offset= gContext.RsBSP;
  gStackFrame.AddrBStore.Mode  = AddrModeFlat;
  gStackFrame.AddrStack.Offset = gContext.IntSp;
  gStackFrame.AddrStack.Mode   = AddrModeFlat;
#else
  #error "Unsupported platform"
#endif

  //
  // Dbghelp is is singlethreaded, so acquire a lock.
  //
  // Note that the code assumes that
  // SymInitialize( GetCurrentProcess(), NULL, TRUE ) has
  // already been called.
  //
  //cLockGuard g (&mDbgHelpLock);     // we are called with lock set  !!

  for (int n = 0; n < kMaxFrames; ++n)
  {
    if (FALSE == StackWalk64(
      machineType,
      GetCurrentProcess(),
      GetCurrentThread(),
      &gStackFrame,
      &gContext,
      NULL,
      SymFunctionTableAccess64,
      SymGetModuleBase64,
      NULL ) )
    {
      // does not set GetLastError !
      if (kDebug)
      {
        if (gStackFrame.AddrPC.Offset == 0)
          QTRACE(( L"StackWalk64 finished with %d entries", n ));
        else
          QTRACE(( L"StackWalk64 stopped with %d entries", n)); //, toStringWin32ErrCode(GetLastError()).c_str() ));
      }
      break; // Maybe it failed, maybe we have finished walking the stack.
    }

    if (gStackFrame.AddrPC.Offset == 0)
    {
      if (kDebug)
        QTRACE(( L"StackWalk64 finished with %d entries", n ));
      break; // Base reached.
    }
    stackTraceVector.push_back (cStackFrame(gStackFrame.AddrPC.Offset, gStackFrame.Params));

    // load the symbols for the current module otherwise subsequent StackWalk call can fail
    loadSymbols ((const void* const)gStackFrame.AddrPC.Offset);
  }

  return true;
}
//}}}
#ifdef _M_IX86
  #pragma warning( pop )
  #pragma optimize( "g", on )
#endif
//}}}
//{{{
/*++
  Routine Description:
    Capture the stack trace based on the given CONTEXT.

  Parameters:
    Context    Thread context. If NULL, the current context is
               used.
    Event      Event structure to populate.
    MaxFrames  Max number of frames the structure can hold.
--*/
//{{{
void cDebugDriver::logStackWalk (int displayFlags)    // input = gContext
{
  loadDebugHeader (displayFlags);
  QMSG((0, L"%hs", gLineBuffer));

  // Set up stack frame.
  DWORD machineType;

  ZeroMemory( &gStackFrame, sizeof( STACKFRAME64 ) );
#ifdef _M_IX86
  machineType                 = IMAGE_FILE_MACHINE_I386;
  gStackFrame.AddrPC.Offset    = gContext.Eip;
  gStackFrame.AddrPC.Mode      = AddrModeFlat;
  gStackFrame.AddrFrame.Offset = gContext.Ebp;
  gStackFrame.AddrFrame.Mode   = AddrModeFlat;
  gStackFrame.AddrStack.Offset = gContext.Esp;
  gStackFrame.AddrStack.Mode   = AddrModeFlat;
#elif _M_X64
  machineType                 = IMAGE_FILE_MACHINE_AMD64;
  gStackFrame.AddrPC.Offset    = gContext.Rip;
  gStackFrame.AddrPC.Mode      = AddrModeFlat;
  gStackFrame.AddrFrame.Offset = gContext.Rbp;
  gStackFrame.AddrFrame.Mode   = AddrModeFlat;
  gStackFrame.AddrStack.Offset = gContext.Rsp;
  gStackFrame.AddrStack.Mode   = AddrModeFlat;
#elif _M_IA64
  machineType                 = IMAGE_FILE_MACHINE_IA64;
  gStackFrame.AddrPC.Offset    = gContext.StIIP;
  gStackFrame.AddrPC.Mode      = AddrModeFlat;
  gStackFrame.AddrFrame.Offset = gContext.IntSp;
  gStackFrame.AddrFrame.Mode   = AddrModeFlat;
  gStackFrame.AddrBStore.Offset= gContext.RsBSP;
  gStackFrame.AddrBStore.Mode  = AddrModeFlat;
  gStackFrame.AddrStack.Offset = gContext.IntSp;
  gStackFrame.AddrStack.Mode   = AddrModeFlat;
#else
  #error "Unsupported platform"
#endif

  //
  // Dbghelp is is singlethreaded, so acquire a lock.
  //
  // Note that the code assumes that
  // SymInitialize( GetCurrentProcess(), NULL, TRUE ) has
  // already been called.
  //
  //cLockGuard g (&mDbgHelpLock);     // we are called with lock set  !!

  for (int n = 0; n < kMaxFrames; ++n)
  {
    if (FALSE == StackWalk64(
      machineType,
      GetCurrentProcess(),
      GetCurrentThread(),
      &gStackFrame,
      &gContext,
      NULL,
      SymFunctionTableAccess64,
      SymGetModuleBase64,
      NULL ) )
    {
      // does not set GetLastError !
      if (kDebug)
      {
        if (gStackFrame.AddrPC.Offset == 0)
          QTRACE(( L"StackWalk64 finished with %d entries", n ));
        else
          QTRACE(( L"StackWalk64 stopped with %d entries", n)); //, toStringWin32ErrCode(GetLastError()).c_str() ));
      }
      break; // Maybe it failed, maybe we have finished walking the stack.
    }

    if (gStackFrame.AddrPC.Offset == 0)
    {
      if (kDebug)
        QTRACE(( L"StackWalk64 finished with %d entries", n ));
      break; // Base reached.
    }
    // load the symbols for the current module otherwise subsequent StackWalk call can fail
    loadSymbols ((const void* const)gStackFrame.AddrPC.Offset);

    loadDebugLine (displayFlags);
    QMSG((0, L"%hs", gLineBuffer));
  }
}
//}}}
//}}}
//{{{
void cDebugDriver::logRegisters( const CONTEXT* const pContext )
{
  // Check the parameter.
  QVERIFY(!IsBadReadPtr(pContext, sizeof(CONTEXT)));

  QMSG((0, L""));
  QMSG((0, L"Registers:   CS=%04X  DS=%04X  SS=%04X  ES=%04X  FS=%04X  GS=%04X", pContext->SegCs, pContext->SegDs, pContext->SegSs, pContext->SegEs, pContext->SegFs, pContext->SegGs));
  QMSG((0, L"RAX=%016IX RBX=%016IX RCX=%016IX RDX=%016IX RSI=%016IX RDI=%016IX", pContext->Rax, pContext->Rbx, pContext->Rcx, pContext->Rdx, pContext->Rsi, pContext->Rdi));
  QMSG((0, L"RBP=%016IX RSP=%016IX  R8=%016IX  R9=%016IX R10=%016IX R11=%016IX", pContext->Rbp, pContext->Rsp, pContext->R8, pContext->R9, pContext->R10, pContext->R11));
  QMSG((0, L"R12=%016IX R13=%016IX R14=%016IX R15=%016IX FLG=%08x", pContext->R12, pContext->R13, pContext->R14, pContext->R15, pContext->EFlags));
}
//}}}
