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

#if !defined(__56524AA2B07041858BC9984F2D70B960)
#define __56524AA2B07041858BC9984F2D70B960
/*
**  
*/
#include "QKernelBase.h"
#include "QKernelExecEnviron.h"

namespace vfs
{
//{{{  QIMPORT/QEXPORT
// =======================================================================================
// === QIMPORT/QEXPORT ===================================================================
// =======================================================================================

#if defined(_WIN32)
  #define QIMPORT _declspec(dllimport)
  #define QEXPORT _declspec(dllexport)
#else
  #error Platform not supported!
#endif
//}}}
//{{{  QKERNELEXEC/QPLUGIN
// =======================================================================================
// === QKERNELEXEC/QPLUGIN ===============================================================
// =======================================================================================

#if defined(KERNELEXEC_EXPORTS)
  #define QKERNELEXEC QEXPORT
  #define QPLUGIN     QIMPORT
#else
  #define QKERNELEXEC QIMPORT
  #define QPLUGIN     QEXPORT
#endif
//}}}
//{{{  QFATAL
// =======================================================================================
// === QFATAL ============================================================================
// =======================================================================================

// Forces a crash with an message and trys to log a stack dump, never returns
#define QFATAL(___Params) vfs::iExceptionManager::singleton().fatal(vfs::fFatal_ShowAll | vfs::fFatal_IsAnError, vfs::iExceptionManager::cFatalDiagnostics##___Params);
//}}}
//{{{  QRECOVERABLE2FATAL
// =======================================================================================
// === QRECOVERABLE2FATAL ================================================================
// =======================================================================================
// Promote a cRecoverable exception to a QFATAL, never returns
#define QRECOVERABLE2FATAL(___Recoverable) { QFATAL((narrow((___Recoverable).getSrcFilename()).c_str(), (___Recoverable).getSrcLineNumber(), L"Exception caught (%s)", (___Recoverable).getMsg().c_str())); }
//}}}
//{{{  QVERIFY
// =======================================================================================
// === QVERIFY ===========================================================================
// =======================================================================================

#define QVERIFY(__EXP)                { if (!(__EXP)) QFATAL((__FILE__, __LINE__, L"QVERIFY expression \"%s\" failed!", L#__EXP)); }
#define QVERIFYMSG(__EXP, __MSG)      { if (!(__EXP)) { vfs::iLogManager::bufferError##__MSG; QFATAL((__FILE__, __LINE__, L"QVERIFYMSG expression \"%s\" failed!", L#__EXP)); } }
#define QVERIFY_AND_CONTINUE(__EXP)   ((__EXP) || (vfs::iExceptionManager::singleton().INTERNAL_USE_ONLY_report(__FILE__, __LINE__, L"QVERIFY_AND_CONTINUE expression \"%s\" failed!", L#__EXP), 0))
//}}}
//{{{  LOG messages
// =======================================================================================
// === Logging - QMSG/QSOS/QERROR ========================================================
// =======================================================================================
//{{{
/*
Outputs a formatted message with prefix. This macro inserts low overhead code
that will filter logging based on per module catgories and whether a modules output has
been supressed.

Logging macros accept a category. These are single bit masks (i.e. 1 << 6) that together
with a module name identify a switch in the registry. These switches control whether to
show or suppress a message.

With an indent, if the category is suppressed it suppresses the whole indented content.

An example of using QMSG :

  {
    QMSG((0, L"Hello %s", Name));
  }
*/
#define QMSG(___Params) vfs::iLogManager::bufferMsg##___Params
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
Outputs a formatted message with no prefix or indent, highlighted in red. Message is always
logged.

  {
    QSOS((L"Unexpected error %d", ErrNum));
  }
*/
#define QSOS(___Params) vfs::iLogManager::bufferSOS##___Params
//}}}
//{{{
// ---------------------------------------------------------------------------------------
/*
Outputs a formatted message with no prefix or indent, highlighted in cyan. Message ignores
indentation and categories.
*/
#define QTRACE(___Params) vfs::iLogManager::bufferTrace##___Params
//}}}
//}}}
//{{{  QDEFINE_FIXEDSIZEALLOCATOR_NEW_AND_DELETE
// =======================================================================================
// === QDEFINE_FIXEDSIZEALLOCATOR_NEW_AND_DELETE =========================================
// =======================================================================================
#if defined(_DEBUG)

#define QDEFINE_FIXEDSIZEALLOCATOR_NEW_AND_DELETE(__CLASS, __OBJECTSPERPAGE) \
  namespace { static cLock m_##__CLASS##AllocatorPtrThreadSafety; static iMemoryAllocator::Ptr m_##__CLASS##Allocator; } \
  void * __CLASS##::operator new (size_t NumBytes) \
  { \
    QVERIFY(NumBytes == sizeof(__CLASS)); \
    { \
      cLockGuard ThreadSafety(&m_##__CLASS##AllocatorPtrThreadSafety); \
      if (!m_##__CLASS##Allocator.isValid()) m_##__CLASS##Allocator = iMemoryManager::singleton().createFixedSizeAllocator(sizeof(__CLASS), __OBJECTSPERPAGE); \
    } \
    return m_##__CLASS##Allocator->allocate(); \
  } \
  void __CLASS##::operator delete(void * const Bytes) { if (Bytes) m_##__CLASS##Allocator->release(Bytes); }

#else

#define QDEFINE_FIXEDSIZEALLOCATOR_NEW_AND_DELETE(__CLASS, __OBJECTSPERPAGE) \
  namespace { static cLock m_##__CLASS##AllocatorPtrThreadSafety; static iMemoryAllocator::Ptr m_##__CLASS##Allocator; } \
  void * __CLASS##::operator new (size_t NumBytes) \
  { \
    { \
      cLockGuard ThreadSafety(&m_##__CLASS##AllocatorPtrThreadSafety); \
      if (!m_##__CLASS##Allocator.isValid()) m_##__CLASS##Allocator = iMemoryManager::singleton().createFixedSizeAllocator(sizeof(__CLASS), __OBJECTSPERPAGE); \
    } \
    return m_##__CLASS##Allocator->allocate(); \
  } \
  void __CLASS##::operator delete(void * const Bytes) { if (Bytes) m_##__CLASS##Allocator->release(Bytes); }

#endif

#define QDECLARE_FIXEDSIZEALLOCATOR_NEW_AND_DELETE() \
  static void * operator new(size_t NumBytes); \
  static void operator delete(void * const Bytes);
//}}}
//{{{  QDEFINE_SINGLETONCTOR
// =======================================================================================
// === QDEFINE_SINGLETONCTOR =============================================================
// =======================================================================================

#define QDEFINE_SINGLETONCTOR(__CLASS)\
  typedef class cClassCtor : public vfs::iFactory::iConstructor, public vfs::cRefCount \
  { \
    private : \
      class cCreator : public vfs::iFactory::iSingletonCreator, public vfs::cRefCount \
      { \
        public : \
          vfs::iFactory::iInstance::Ptr QAPI createSingleton() { return new __CLASS; } \
      }; \
      vfs::iFactory::iConstructor::Ptr m_Pimpl; \
    public : \
      cClassCtor() : m_Pimpl(vfs::gFactory->INTERNAL_USE_ONLY_createSingletonConstructor(new __CLASS::class_info, new cCreator)) {} \
      vfs::iFactory::iClassInfo::ConstPtr QAPI getClassInfo() const { return m_Pimpl->getClassInfo(); } \
      vfs::iFactory::iInstance::Ptr QAPI createInstance() { return m_Pimpl->createInstance(); } \
      vfs::iFactory::iInstance::Ptr QAPI createInstanceWithParams(const iParameters & Params) { throw vfs::cNotSupported(__FILE__, __LINE__, L"No parameterised \"%s\" ctor", #__CLASS); } \
  } class_ctor
//}}}
//{{{  QDEFINE_CLASSCTOR
// =======================================================================================
// === QDEFINE_CLASSCTOR =================================================================
// =======================================================================================

#define QDEFINE_CLASSCTOR(__CLASS)\
  typedef class cClassCtor : public vfs::iFactory::iConstructor, public vfs::cRefCount \
  { \
    public : \
      vfs::iFactory::iClassInfo::ConstPtr QAPI getClassInfo() const { return new __CLASS::class_info; } \
      vfs::iFactory::iInstance::Ptr QAPI createInstance() { return new __CLASS; } \
      vfs::iFactory::iInstance::Ptr QAPI createInstanceWithParams(const vfs::iFactory::iConstructor::iParameters & Params) { throw vfs::cNotSupported(__FILE__, __LINE__, L"No parameterised \"%s\" ctor", #__CLASS); } \
  } class_ctor

#define QDEFINE_PARAMETERISED_CLASSCTOR(__CLASS)\
  typedef class cClassCtor : public vfs::iFactory::iConstructor, public vfs::cRefCount \
  { \
    public : \
      vfs::iFactory::iClassInfo::ConstPtr QAPI getClassInfo() const { return new __CLASS##::class_info; } \
      vfs::iFactory::iInstance::Ptr QAPI createInstance() { throw vfs::cNotSupported(__FILE__, __LINE__, L"No default \"%s\" ctor", #__CLASS); } \
      vfs::iFactory::iInstance::Ptr QAPI createInstanceWithParams(const vfs::iFactory::iConstructor::iParameters & Params) { return new __CLASS(Params); } \
  } class_ctor
//}}}
//{{{  QDEFINE_CLASSINFO
// =======================================================================================
// === QDEFINE_CLASSINFO =================================================================
// =======================================================================================

// Single inheritance - 1 base
#define QDEFINE_CLASSINFO(__CLASSGUID, __BASECLASS) \
  typedef class cClassInfo : public vfs::iFactory::iClassInfo, public vfs::cRefCount \
  { \
    public : \
      QDEFINE_SMARTPTR(cClassInfo); \
      cClassInfo() {} \
      vfs::cGuid QAPI getGuid() const \
      { \
        static long constructed = 0; \
        static const long constructedFinalValue = 0x7FBDE80F; \
        static vfs::cGuid guid; \
        if (::InterlockedCompareExchange(&constructed, 0, 0) != constructedFinalValue) \
        { \
          static vfs::Char mutexName[] = L"7FBDE80F-C75F-47AB-BB86-ECD60440DC74"; \
          HANDLE hMutex = CreateMutexW(0, 0, mutexName); \
          ::WaitForSingleObject(hMutex, INFINITE); \
          try \
          { \
            if (constructed != constructedFinalValue) \
            { \
              static vfs::cGuid kGuid(__CLASSGUID); \
              guid = kGuid; \
            } \
          } \
          catch (...) \
          { \
            ::ReleaseMutex(hMutex); \
            ::CloseHandle(hMutex); \
            throw; \
          } \
          ::InterlockedExchange(&constructed, constructedFinalValue); \
          ::ReleaseMutex(hMutex); \
          ::CloseHandle(hMutex); \
        } \
        return guid; \
      } \
      vfs::String QAPI getName() const { return getGuid().getComment(); } \
      std::vector<vfs::iFactory::iClassInfo::ConstPtr> QAPI getBaseInfo() const \
      { \
        std::vector<vfs::iFactory::iClassInfo::ConstPtr> Bases(1 /* initial size */); \
        Bases[0] = vfs::iFactory::iClassInfo::ConstPtr(new __BASECLASS::class_info); \
        return Bases; \
      } \
      iClassInfo::ConstPtr QAPI clone() const { return new cClassInfo; } \
  } class_info

// Multiple inheritance - 2 bases
#define QDEFINE_CLASSINFO2(__CLASSGUID, __BASECLASS1, __BASECLASS2) \
  typedef class cClassInfo : public vfs::iFactory::iClassInfo, public vfs::cRefCount \
  { \
    public : \
      QDEFINE_SMARTPTR(cClassInfo); \
      cClassInfo() {} \
      vfs::cGuid QAPI getGuid() const \
      { \
        static long constructed = 0; \
        static const long constructedFinalValue = 0x7FBDE80F; \
        static vfs::cGuid guid; \
        if (::InterlockedCompareExchange(&constructed, 0, 0) != constructedFinalValue) \
        { \
          static vfs::Char mutexName[] = L"7FBDE80F-C75F-47AB-BB86-ECD60440DC74"; \
          HANDLE hMutex = CreateMutexW(0, 0, mutexName); \
          ::WaitForSingleObject(hMutex, INFINITE); \
          try \
          { \
            if (constructed != constructedFinalValue) \
            { \
              static vfs::cGuid kGuid(__CLASSGUID); \
              guid = kGuid; \
            } \
          } \
          catch (...) \
          { \
            ::ReleaseMutex(hMutex); \
            ::CloseHandle(hMutex); \
            throw; \
          } \
          ::InterlockedExchange(&constructed, constructedFinalValue); \
          ::ReleaseMutex(hMutex); \
          ::CloseHandle(hMutex); \
        } \
        return guid; \
      } \
      vfs::String QAPI getName() const { return getGuid().getComment(); } \
      std::vector<vfs::iFactory::iClassInfo::ConstPtr> QAPI getBaseInfo() const \
      { \
        std::vector<vfs::iFactory::iClassInfo::ConstPtr> Bases(2 /* initial size */); \
        Bases[0] = vfs::iFactory::iClassInfo::ConstPtr(new __BASECLASS1::class_info); \
        Bases[1] = vfs::iFactory::iClassInfo::ConstPtr(new __BASECLASS2::class_info); \
        return Bases; \
      } \
      iClassInfo::ConstPtr QAPI clone() const { return new cClassInfo; } \
  } class_info

// Multiple inheritance - 3 bases
#define QDEFINE_CLASSINFO3(__CLASSGUID, __BASECLASS1, __BASECLASS2, __BASECLASS3) \
  typedef class cClassInfo : public vfs::iFactory::iClassInfo, public vfs::cRefCount \
  { \
    public : \
      QDEFINE_SMARTPTR(cClassInfo); \
      cClassInfo() {} \
      vfs::cGuid QAPI getGuid() const \
      { \
        static long constructed = 0; \
        static const long constructedFinalValue = 0x7FBDE80F; \
        static vfs::cGuid guid; \
        if (::InterlockedCompareExchange(&constructed, 0, 0) != constructedFinalValue) \
        { \
          static vfs::Char mutexName[] = L"7FBDE80F-C75F-47AB-BB86-ECD60440DC74"; \
          HANDLE hMutex = CreateMutexW(0, 0, mutexName); \
          ::WaitForSingleObject(hMutex, INFINITE); \
          try \
          { \
            if (constructed != constructedFinalValue) \
            { \
              static vfs::cGuid kGuid(__CLASSGUID); \
              guid = kGuid; \
            } \
          } \
          catch (...) \
          { \
            ::ReleaseMutex(hMutex); \
            ::CloseHandle(hMutex); \
            throw; \
          } \
          ::InterlockedExchange(&constructed, constructedFinalValue); \
          ::ReleaseMutex(hMutex); \
          ::CloseHandle(hMutex); \
        } \
        return guid; \
      } \
      vfs::String QAPI getName() const { return getGuid().getComment(); } \
      std::vector<vfs::iFactory::iClassInfo::ConstPtr> QAPI getBaseInfo() const \
      { \
        std::vector<vfs::iFactory::iClassInfo::ConstPtr> Bases(3 /* initial size */); \
        Bases[0] = vfs::iFactory::iClassInfo::ConstPtr(new __BASECLASS1::class_info); \
        Bases[1] = vfs::iFactory::iClassInfo::ConstPtr(new __BASECLASS2::class_info); \
        Bases[2] = vfs::iFactory::iClassInfo::ConstPtr(new __BASECLASS3::class_info); \
        return Bases; \
      } \
      iClassInfo::ConstPtr QAPI clone() const { return new cClassInfo; } \
  } class_info

// Multiple inheritance - 4 bases
#define QDEFINE_CLASSINFO4(__CLASSGUID, __BASECLASS1, __BASECLASS2, __BASECLASS3, __BASECLASS4) \
  typedef class cClassInfo : public vfs::iFactory::iClassInfo, public vfs::cRefCount \
  { \
    public : \
      QDEFINE_SMARTPTR(cClassInfo); \
      cClassInfo() {} \
      vfs::cGuid QAPI getGuid() const \
      { \
        static long constructed = 0; \
        static const long constructedFinalValue = 0x7FBDE80F; \
        static vfs::cGuid guid; \
        if (::InterlockedCompareExchange(&constructed, 0, 0) != constructedFinalValue) \
        { \
          static vfs::Char mutexName[] = L"7FBDE80F-C75F-47AB-BB86-ECD60440DC74"; \
          HANDLE hMutex = CreateMutexW(0, 0, mutexName); \
          ::WaitForSingleObject(hMutex, INFINITE); \
          try \
          { \
            if (constructed != constructedFinalValue) \
            { \
              static vfs::cGuid kGuid(__CLASSGUID); \
              guid = kGuid; \
            } \
          } \
          catch (...) \
          { \
            ::ReleaseMutex(hMutex); \
            ::CloseHandle(hMutex); \
            throw; \
          } \
          ::InterlockedExchange(&constructed, constructedFinalValue); \
          ::ReleaseMutex(hMutex); \
          ::CloseHandle(hMutex); \
        } \
        return guid; \
      } \
      vfs::String QAPI getName() const { return getGuid().getComment(); } \
      std::vector<vfs::iFactory::iClassInfo::ConstPtr> QAPI getBaseInfo() const \
      { \
        std::vector<vfs::iFactory::iClassInfo::ConstPtr> Bases(4 /* initial size */); \
        Bases[0] = vfs::iFactory::iClassInfo::ConstPtr(new __BASECLASS1::class_info); \
        Bases[1] = vfs::iFactory::iClassInfo::ConstPtr(new __BASECLASS2::class_info); \
        Bases[2] = vfs::iFactory::iClassInfo::ConstPtr(new __BASECLASS3::class_info); \
        Bases[3] = vfs::iFactory::iClassInfo::ConstPtr(new __BASECLASS4::class_info); \
        return Bases; \
      } \
      iClassInfo::ConstPtr QAPI clone() const { return new cClassInfo; } \
  } class_info

//}}}
//{{{  QDEFINE_SINGLETONMETHODS
// =======================================================================================
// === QDEFINE_SINGLETONMETHODS ==========================================================
// =======================================================================================

#define QDEFINE_SINGLETONMETHODS(__CLASS) \
  static __CLASS & QAPI singleton() throw(vfs::cNotFound) \
  { \
    static iInstance::Ptr Singleton; \
    if (!Singleton.isValid()) Singleton = vfs::gFactory->createInstance(__CLASS::class_info().getGuid()); \
    return static_cast<__CLASS &>(*Singleton.getRaw()); \
  } \
  static bool QAPI isSingletonRegistered() \
  { \
    return vfs::gFactory.isValid() ? vfs::gFactory->getConstructors(__CLASS::class_info().getGuid()).size() > 0 : false; \
  } \
  static bool QAPI isSingletonConstructed() \
  { \
    return vfs::gFactory.isValid() ? vfs::gFactory->countInstances(__CLASS::class_info().getGuid()) > 0 : false; \
  }
//}}}
//{{{  QDEFINE_SINGLETON_IMPL
// =======================================================================================
// === QDEFINE_SINGLETON_IMPL ============================================================
// =======================================================================================

// Single inheritance - 1 base
#define QDEFINE_SINGLETON_IMPL(__IMPLCLASS, __IMPLCLASSGUID, __BASEIFACECLASS) \
  QDEFINE_CLASSINFO(__IMPLCLASSGUID, __BASEIFACECLASS); \
  virtual vfs::iFactory::iClassInfo::ConstPtr QAPI getClassInfo() const { return new __IMPLCLASS::class_info; } \
  QDEFINE_SINGLETONCTOR(__IMPLCLASS); \
  QDEFINE_SMARTPTR(__IMPLCLASS); \
  QDEFINE_SINGLETONMETHODS(__IMPLCLASS)

// Multiple inheritance - 2 bases
#define QDEFINE_SINGLETON_IMPL2(__IMPLCLASS, __IMPLCLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2) \
  QDEFINE_CLASSINFO2(__IMPLCLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2); \
  virtual vfs::iFactory::iClassInfo::ConstPtr QAPI getClassInfo() const { return new __IMPLCLASS::class_info; } \
  QDEFINE_SINGLETONCTOR(__IMPLCLASS); \
  QDEFINE_SMARTPTR(__IMPLCLASS); \
  QDEFINE_SINGLETONMETHODS(__IMPLCLASS)

// Multiple inheritance - 3 bases
#define QDEFINE_SINGLETON_IMPL3(__IMPLCLASS, __IMPLCLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2, __BASEIFACECLASS3) \
  QDEFINE_CLASSINFO3(__IMPLCLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2, __BASEIFACECLASS3); \
  virtual vfs::iFactory::iClassInfo::ConstPtr QAPI getClassInfo() const { return new __IMPLCLASS::class_info; } \
  QDEFINE_SINGLETONCTOR(__IMPLCLASS); \
  QDEFINE_SMARTPTR(__IMPLCLASS); \
  QDEFINE_SINGLETONMETHODS(__IMPLCLASS)

// Multiple inheritance - 4 bases
#define QDEFINE_SINGLETON_IMPL4(__IMPLCLASS, __IMPLCLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2, __BASEIFACECLASS3, __BASEIFACECLASS4) \
  QDEFINE_CLASSINFO3(__IMPLCLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2, __BASEIFACECLASS3, __BASEIFACECLASS4); \
  virtual vfs::iFactory::iClassInfo::ConstPtr QAPI getClassInfo() const { return new __IMPLCLASS::class_info; } \
  QDEFINE_SINGLETONCTOR(__IMPLCLASS); \
  QDEFINE_SMARTPTR(__IMPLCLASS); \
  QDEFINE_SINGLETONMETHODS(__IMPLCLASS)
//}}}
//{{{  QDEFINE_CLASS_IMPL
// =======================================================================================
// === QDEFINE_CLASS_IMPL ================================================================
// =======================================================================================

#define QDEFINE_CLASS_IMPL(__IMPLCLASS, __IMPLCLASSGUID, __BASEIFACECLASS) \
  QDEFINE_CLASSINFO(__IMPLCLASSGUID, __BASEIFACECLASS); \
  virtual vfs::iFactory::iClassInfo::ConstPtr QAPI getClassInfo() const { return new __IMPLCLASS::class_info; } \
  QDEFINE_CLASSCTOR(__IMPLCLASS); \
  QDEFINE_SMARTPTR(__IMPLCLASS)
#define QDEFINE_PARAMETERISED_CLASS_IMPL(__IMPLCLASS, __IMPLCLASSGUID, __BASEIFACECLASS) \
  QDEFINE_CLASSINFO(__IMPLCLASSGUID, __BASEIFACECLASS); \
  virtual vfs::iFactory::iClassInfo::ConstPtr QAPI getClassInfo() const { return new __IMPLCLASS::class_info; } \
  QDEFINE_PARAMETERISED_CLASSCTOR(__IMPLCLASS); \
  QDEFINE_SMARTPTR(__IMPLCLASS)

// Multiple inheritance - 2 bases
#define QDEFINE_CLASS_IMPL2(__IMPLCLASS, __IMPLCLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2) \
  QDEFINE_CLASSINFO2(__IMPLCLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2); \
  virtual vfs::iFactory::iClassInfo::ConstPtr QAPI getClassInfo() const { return new __IMPLCLASS::class_info; } \
  QDEFINE_CLASSCTOR(__IMPLCLASS); \
  QDEFINE_SMARTPTR(__IMPLCLASS)
#define QDEFINE_PARAMETERISED_CLASS_IMPL2(__IMPLCLASS, __IMPLCLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2) \
  QDEFINE_CLASSINFO2(__IMPLCLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2); \
  virtual vfs::iFactory::iClassInfo::ConstPtr QAPI getClassInfo() const { return new __IMPLCLASS::class_info; } \
  QDEFINE_PARAMETERISED_CLASSCTOR(__IMPLCLASS); \
  QDEFINE_SMARTPTR(__IMPLCLASS)

// Multiple inheritance - 3 bases
#define QDEFINE_CLASS_IMPL3(__IMPLCLASS, __IMPLCLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2, __BASEIFACECLASS3) \
  QDEFINE_CLASSINFO3(__IMPLCLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2, __BASEIFACECLASS3); \
  virtual vfs::iFactory::iClassInfo::ConstPtr QAPI getClassInfo() const { return new __IMPLCLASS::class_info; } \
  QDEFINE_CLASSCTOR(__IMPLCLASS); \
  QDEFINE_SMARTPTR(__IMPLCLASS)
#define QDEFINE_PARAMETERISED_CLASS_IMPL3(__IMPLCLASS, __IMPLCLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2, __BASEIFACECLASS3) \
  QDEFINE_CLASSINFO3(__IMPLCLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2, __BASEIFACECLASS3); \
  virtual vfs::iFactory::iClassInfo::ConstPtr QAPI getClassInfo() const { return new __IMPLCLASS::class_info; } \
  QDEFINE_PARAMETERISED_CLASSCTOR(__IMPLCLASS); \
  QDEFINE_SMARTPTR(__IMPLCLASS)

// Multiple inheritance - 4 bases
#define QDEFINE_CLASS_IMPL4(__IMPLCLASS, __IMPLCLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2, __BASEIFACECLASS3, __BASEIFACECLASS4) \
  QDEFINE_CLASSINFO4(__IMPLCLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2, __BASEIFACECLASS3, __BASEIFACECLASS4); \
  virtual vfs::iFactory::iClassInfo::ConstPtr QAPI getClassInfo() const { return new __IMPLCLASS::class_info; } \
  QDEFINE_CLASSCTOR(__IMPLCLASS); \
  QDEFINE_SMARTPTR(__IMPLCLASS)
#define QDEFINE_PARAMETERISED_CLASS_IMPL4(__IMPLCLASS, __IMPLCLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2, __BASEIFACECLASS3, __BASEIFACECLASS4) \
  QDEFINE_CLASSINFO4(__IMPLCLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2, __BASEIFACECLASS3, __BASEIFACECLASS4); \
  virtual vfs::iFactory::iClassInfo::ConstPtr QAPI getClassInfo() const { return new __IMPLCLASS::class_info; } \
  QDEFINE_PARAMETERISED_CLASSCTOR(__IMPLCLASS); \
  QDEFINE_SMARTPTR(__IMPLCLASS)
//}}}
//{{{  QDEFINE_SINGLETON_IFACE
// =======================================================================================
// === QDEFINE_SINGLETON_IFACE ===========================================================
// =======================================================================================

#define QDEFINE_SINGLETON_IFACE(__IFACECLASS, __IFACECLASSGUID, __BASEIFACECLASS) \
  QDEFINE_CLASSINFO(__IFACECLASSGUID, __BASEIFACECLASS); \
  QDEFINE_SMARTPTR(__IFACECLASS); \
  QDEFINE_SINGLETONMETHODS(__IFACECLASS)

// Multiple inheritance - 2 bases
#define QDEFINE_SINGLETON_IFACE2(__IFACECLASS, __IFACECLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2) \
  QDEFINE_CLASSINFO2(__IFACECLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2); \
  QDEFINE_SMARTPTR(__IFACECLASS); \
  QDEFINE_SINGLETONMETHODS(__IFACECLASS)

// Multiple inheritance - 3 bases
#define QDEFINE_SINGLETON_IFACE3(__IFACECLASS, __IFACECLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2, __BASEIFACECLASS3) \
  QDEFINE_CLASSINFO3(__IFACECLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2, __BASEIFACECLASS3); \
  QDEFINE_SMARTPTR(__IFACECLASS); \
  QDEFINE_SINGLETONMETHODS(__IFACECLASS)

// Multiple inheritance - 4 bases
#define QDEFINE_SINGLETON_IFACE4(__IFACECLASS, __IFACECLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2, __BASEIFACECLASS3, __BASEIFACECLASS4) \
  QDEFINE_CLASSINFO3(__IFACECLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2, __BASEIFACECLASS3, __BASEIFACECLASS4); \
  QDEFINE_SMARTPTR(__IFACECLASS); \
  QDEFINE_SINGLETONMETHODS(__IFACECLASS)
//}}}
//{{{  QDEFINE_CLASS_IFACE
// =======================================================================================
// === QDEFINE_CLASS_IFACE ===============================================================
// =======================================================================================

// Single inheritance - 1 base
#define QDEFINE_CLASS_IFACE(__IFACECLASS, __IFACECLASSGUID, __BASEIFACECLASS) \
  QDEFINE_CLASSINFO(__IFACECLASSGUID, __BASEIFACECLASS); \
  QDEFINE_SMARTPTR(__IFACECLASS)

// Multiple inheritance - 2 bases
#define QDEFINE_CLASS_IFACE2(__IFACECLASS, __IFACECLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2) \
  QDEFINE_CLASSINFO2(__IFACECLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2); \
  QDEFINE_SMARTPTR(__IFACECLASS)

// Multiple inheritance - 3 bases
#define QDEFINE_CLASS_IFACE3(__IFACECLASS, __IFACECLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2, __BASEIFACECLASS3) \
  QDEFINE_CLASSINFO3(__IFACECLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2, __BASEIFACECLASS3); \
  QDEFINE_SMARTPTR(__IFACECLASS)

// Multiple inheritance - 4 bases
#define QDEFINE_CLASS_IFACE4(__IFACECLASS, __IFACECLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2, __BASEIFACECLASS3, __BASEIFACECLASS4) \
  QDEFINE_CLASSINFO3(__IFACECLASSGUID, __BASEIFACECLASS1, __BASEIFACECLASS2, __BASEIFACECLASS3, , __BASEIFACECLASS4); \
  QDEFINE_SMARTPTR(__IFACECLASS)
//}}}
}
#endif
