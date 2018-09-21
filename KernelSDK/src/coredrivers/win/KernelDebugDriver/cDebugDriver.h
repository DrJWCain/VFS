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

#include <QKernelExec.h>

namespace
{
  class cStackFrame;
  typedef std::vector<cStackFrame> tStackFrameVector;
}

namespace vfs
{

  class cDebugDriver : public iDebugDriver, public cRefCount
  {
  public :
    QDEFINE_CLASS_IMPL(cDebugDriver, L"{8B3F6B2E-B6B6-4D1F-A309-AF8468E66926} // cDebugDriver", iDebugDriver);

    cDebugDriver();
    ~cDebugDriver();

    // iDebugDriver
    void QAPI logCallStack( unsigned int displayFlags );                          //!< logs call stack with control of displayed info
    void QAPI logCallStack( DWORD ThreadID, const EXCEPTION_POINTERS * const ep );

    String QAPI getDebugHeader( int displayFlags );                         //!< returns a column header string to match getDebugLine calls
    String QAPI getDebugLine( const void* const addr, int displayFlags );   //!< returns the src file & routine at this address (same as logCallStack)

    void QAPI logCallStack( unsigned long ThreadID, int displayFlags );
    void QAPI logAllThreadsCallStacks();
    //void QAPI logCallStack( unsigned long ThreadID, const String & AfterFunc );

    void QAPI logAllThreadsInfo( const bool IsPercentages );
    void QAPI logAllModules();
    bool QAPI resetUnhandledExceptionFilter();

    void QAPI getFaultReason( DWORD threadID, const EXCEPTION_POINTERS* const ep, iExceptionManager::cFatalDiagnostics& pDiagToLoad );

    std::map<unsigned int, iThread::Ptr> QAPI getAllThreads();

    // cDebugDriver
    void initialise();

    void QAPI logStackWalk( int displayFlags );
    void QAPI loadAllSymbols();

  private :
    bool mIsInitialised;
    int mFilenameWidth;  // display width of filename
    int mRoutineWidth;   // display width of routine name

    cLock mDbgHelpLock;
    cGuard::ConstPtr mStackConsoleCmdGuard;
    cGuard::ConstPtr mModulesConsoleCmdGuard;
    cGuard::ConstPtr mThreadsConsoleCmdGuard;

    void loadDebugHeader (int displayFlags);
    void loadDebugLine (int displayFlags);
    void loadDebugLine (const cStackFrame& stackframe, int displayFlags);

    void loadSymbols (const void* const addr);
    HMODULE getModule (const void* const addr);  // which contans this address - search moduleVector
    void loadSymbolsUpto (const char* filename);

    bool captureStackTrace (tStackFrameVector& stackTraceVector);
    bool getLoadedModules( DWORD dwPID, UINT uiCount, HMODULE* paModArray, LPUINT puiRealCount );
    bool initPSAPI();

    void logRegisters( const CONTEXT* const pContext );
  };

}
