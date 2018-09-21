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

#if !defined(__6327390207BC43998BB046D3BDAC8F09)
#define __6327390207BC43998BB046D3BDAC8F09
/*
**  
*/

#if defined(_DEBUG) && defined(_MSC_VER)
  #define _CRTDBG_MAP_ALLOC
  #include <stdlib.h>
  #include <crtdbg.h>
#endif


// =======================================================================================
// === Streaming Media Extensions (SME) ==================================================
// =======================================================================================
// If compiler is better than MSVC 6 then assume support for SME

#define QSWITCH_SME 1


// =======================================================================================
// === Smart Pointer Tracking ============================================================
// =======================================================================================
// Expensive way of tracking who is keeping alive a smart pointable object. Call
// cRefCount::setTracking(true) in your objects constructor  then call verifyRefCount(1) on
// your object when you expect the ref count to be 1 - the outstanding refs will be listed.

#if defined(_DEBUG)
  #define QSWITCH_TRACKPTR 1
#else
  #define QSWITCH_TRACKPTR 0
#endif

//{{{
#define QDEFINE_TRACKPTR_IMPL() \
  typedef std::multimap<const void* const, const void * const> PtrMap; \
  mutable PtrMap m_PtrMap; \
  mutable cLock  m_PtrMapProtect; \
  void QAPI onAfterAddRef( \
    const void * const PtrID, \
    const void * const ModuleAddr, \
    const unsigned int  RefCount) const \
  { \
    cLockGuard ThreadSafety(&m_PtrMapProtect); \
    m_PtrMap.insert(std::make_pair(PtrID, ModuleAddr)); \
  } \
  void QAPI onBeforeRemoveRef( \
    const void * const PtrID, \
    const void * const ModuleAddr, \
    const unsigned int  RefCount) const \
  { \
    cLockGuard ThreadSafety(&m_PtrMapProtect); \
    PtrMap::iterator i = m_PtrMap.find(PtrID); \
    QVERIFY(i != m_PtrMap.end()); \
    m_PtrMap.erase(i); \
  } \
  void QAPI verifyRefCount(const unsigned int ExpectedRefCount) const \
  { \
    cLockGuard ThreadSafety(&m_PtrMapProtect); \
    if (ExpectedRefCount != m_PtrMap.size()) \
    { \
      Char Tmp[256]; \
      wsprintf(Tmp, L"Object expected to have %d references but actually has %d", ExpectedRefCount, m_PtrMap.size()); \
      OutputDebugString(String(String(Tmp) + L"\n").c_str()); \
      cLogIndentSOSGuard IndentGuard(Tmp); \
      for (PtrMap::iterator i = m_PtrMap.begin(); i != m_PtrMap.end(); i++) \
      { \
        wsprintf(Tmp, L"Ptr 0x%p by module 0x%p", *i, i->second); \
        OutputDebugString(String(String(Tmp) + L"\n").c_str()); \
        QMSG((0, Tmp)); \
      } \
    } \
  }
//}}}
//{{{
#define QDEFINE_TRACKPTR_IFACE() \
  virtual void QAPI onAfterAddRef( \
    const void * const PtrID, \
    const void * const ModuleAddr, \
    const unsigned int  RefCount) const = 0; \
  virtual void QAPI onBeforeRemoveRef( \
    const void * const PtrID, \
    const void * const ModuleAddr, \
    const unsigned int  RefCount) const = 0; \
  virtual void QAPI verifyRefCount(const unsigned int ExpectedRefCount) const = 0;\
//}}}

//{{{  QAPI/QAPI_VARGS
// =======================================================================================
// === QAPI/QAPI_VARGS ===================================================================
// =======================================================================================

// QAPI (and QAPI_VARG) are used to declare the calling convention of all vfs
// Kernel functions. It is important that all Kernel functions are declared using this
// so that the correct linkage does not depend on the callers compiler settings.
// e.g. A client may choose to use stack based parameter passing and set their
// compiler as such, without our explicit QAPI declarations the clients program would
// expect the Kernels function to also used stacked based parameters - which is incorrect.
// However in this example the program would not actually link because the decorated
// names used at the linkage stage would be different due to the differing calling
// conventions.

#if defined(_MSC_VER)
  #define QARGS_STACK __cdecl
  #define QARGS_REG   __fastcall
#else
  #error Compiler not supported!
#endif

#if defined(_DEBUG)
  // To help the debugger show the state of variables and function arguments correctly
  // we will use stacked based parameter passing for debug builds of the Kernel.
  #define QAPI QARGS_STACK
#else
  // For efficiency we will use registerised parameter passing for our function
  // arguments in release builds of the Kernel.
  #define QAPI QARGS_REG
#endif

// If a function is using C style variable arguments it must always use stack based
// parameters.
#define QAPI_VARGS QARGS_STACK

// New vector calling convention uses SSE registers for floating-point parameters
#if _MSC_VER >= 1800
  #define QARGS_VEC   __vectorcall
#else
  #define QARGS_VEC   QAPI
#endif

//}}}

//{{{  QDEFINE_SMARTPTR
// =======================================================================================
// === QDEFINE_SMARTPTR ==================================================================
// =======================================================================================

// Use this inside a class declaration to define smart pointer types Ptr and ConstPtr for
// your class.

#define QDEFINE_SMARTPTR(T) \
  typedef vfs::cPtr< T >        Ptr; \
  typedef vfs::cConstPtr< T >   ConstPtr
//}}}
//{{{  QDEFINE_SMARTCOWPTR
// =======================================================================================
// === QDEFINE_SMARTCOWPTR ===============================================================
// =======================================================================================

#define QDEFINE_SMARTCOWPTR(T) \
  typedef vfs::cCowPtr< T > Ptr; \
  typedef vfs::cCowPtr< T > CowPtr
//}}}

//{{{  QDEPRECATED
// =======================================================================================
// === QDEPRECATED =======================================================================
// =======================================================================================

#if defined(_WIN32)
  #define QDEPRECATED __declspec(deprecated)
#else
  #define QDEPRECATED
#endif
//}}}

#endif
