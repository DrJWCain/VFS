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
#include "cLogManager.h"

#include "cThreadImpl.h"
#include "LogCategories.h"
#include "cSystemInfo.h"
#include "Version.h"
//}}}

using namespace std;
using namespace vfs;

//----------------------------------------------------------------------------------------------------------------------
// statics
bool cLogManager::gIsLoggingAvailable = false;

//----------------------------------------------------------------------------------------------------------------------
namespace
{
  cLogManager::cLogFlags gNullFlags;

  //{{{
  bool flagPairPredicate (const pair<UIntPtr,cLogManager::cLogFlags*>& p1, const pair<UIntPtr,cLogManager::cLogFlags*>& p2)
  {
    // Highest address first ordering !! - makes lower_bound find matching or next lowest address
    return (p1.first > p2.first);
  }
  //}}}
  //{{{
  cLogThreadData* getCurrThreadLogData()
  {
    // get the task pointer from the TIB
    void* fs14 = (void*) __readgsqword (0x28);
    cTib14* const tib = reinterpret_cast<cTib14*>(fs14);
    if (tib)
    {
      if(!tib->isValid())
        __debugbreak();
      return &tib->m_LogData;
    }
    return 0;
  }
  //}}}
}

//----------------------------------------------------------------------------------------------------------------------
//{{{
// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

cLogManager::cLogManager()
  : mNumSuppressedMsgs(0),
    mAreDriversStarted(false),
    mIsShowingNonIThreadMsgs(false),
    mLastModulePage(0),
    mLastFlags(&gNullFlags),
    mRegHelp(createRegistryHelp())
{
  cLogIndentGuard g(kCtgCreateManagerDetail, L"Creating LogManager");

  // Initialise the sub-systems we depend on. If every sub-system initialises the others
  // they depend on then we can guarantee that they are all initialised in the correct order.
//  m_SystemManager     = iSystemManager::initSingleton();
//  m_InterfaceManager  = iInstanceManager::initSingleton();

  mIsShowingNonIThreadMsgs = mRegHelp->getItemNumber (
    cSystemInfo::singleton().getKernelExecRegistryPath() + L"Custom Settings\\Log\\", L"Is showing msgs from a non-iThread"
    ) != 0;

  // Add virtual modules
  #if defined(_DEBUG)
  addModule (L"vfs", L"_KernelExec", kVersion, GetModuleHandle(L"KernelExec_Dbg.dll"));
  #else
  addModule (L"vfs", L"_KernelExec", kVersion, GetModuleHandle(L"KernelExec.dll"));
  #endif

  addModule (
    cSystemInfo::singleton().getProductDeveloperName(),
    String(L"_") + cSystemInfo::singleton().getHostName(),
    cSystemInfo::singleton().getHostVersion(),
    GetModuleHandle(NULL)
    );

  mOutputThread = new cLogOutputThread();
  gIsLoggingAvailable = true;
}
//}}}
//{{{
cLogManager::~cLogManager()
{
  gIsLoggingAvailable = false;
  stopDrivers();
}
//}}}
//----------------------------------------------------------------------------------------------------------------------
// iLogManager interface static functions
//{{{
void QAPI_VARGS iLogManager::bufferMsg (const unsigned long category, const Char* const format, ...)
{
  QVERIFY(format != 0);

  if (cLogManager::gIsLoggingAvailable)
  {
    cLogManager & lm = cLogManager::singleton();

    // The following macro reads the return address off the stack, looks for the start of
    // the allocation block on the heap and then finds the module name from the start
    // address.
    void* moduleAddr = _ReturnAddress();

    va_list ap;
    va_start(ap, format);

    if (lm.isAKernelThread())
      lm.bufferMsg (iLogDriver::cMsg::ePlainMsg, category, moduleAddr, format, ap);
    else
    {
      if (lm.mIsShowingNonIThreadMsgs)
      {
        const cLogManager::cLogFlags* const flags = lm.getLogFlags (moduleAddr);
        if (flags->isModVisible())
        {
          Char buffer[MAX_NUM_LOG_MSG_CHARS];
          if (_vsnwprintf (buffer, MAX_NUM_LOG_MSG_CHARS - 1, format, ap) == MAX_NUM_LOG_MSG_CHARS - 1)
            buffer[MAX_NUM_LOG_MSG_CHARS - 1] = L'\0';

          QTRACE((L"Non-kernel thread 0x%x tried to QMSG \"%s\"", GetCurrentThreadId(), buffer));
        }
      }
    }
    va_end(ap);
  }
}
//}}}
//{{{
void QAPI_VARGS iLogManager::bufferSOS (const Char* const format, ...) throw()
{
  QVERIFY(format != 0);

  if (cLogManager::gIsLoggingAvailable)
  {
    cLogManager& lm = cLogManager::singleton();
    cLogThreadData* const td = getCurrThreadLogData();
    if (td)
    {
      // The following macro reads the return address off the stack, looks for the start of
      // the allocation block on the heap and then finds the module name from the start
      // address.
      void* moduleAddr = _ReturnAddress();

      va_list ap;
      va_start (ap, format);

      td->m_NumSOSIndents++;
      lm.bufferMsg (iLogDriver::cMsg::ePlainMsg, 0, moduleAddr, format, ap);
      td->m_NumSOSIndents--;

      va_end(ap);
    }
    else
    {
      if (lm.mIsShowingNonIThreadMsgs)
      {
        // The following macro reads the return address off the stack, looks for the start of
        // the allocation block on the heap and then finds the module name from the start
        // address.
        void* moduleAddr = _ReturnAddress();
        const cLogManager::cLogFlags* const flags = lm.getLogFlags (moduleAddr);
        if (flags->isModVisible())
        {
          va_list ap;
          va_start (ap, format);

          Char buffer[MAX_NUM_LOG_MSG_CHARS];
          if (_vsnwprintf (buffer, MAX_NUM_LOG_MSG_CHARS - 1, format, ap) == MAX_NUM_LOG_MSG_CHARS - 1)
            buffer[MAX_NUM_LOG_MSG_CHARS - 1] = L'\0';

          QTRACE((L"Log Manager : Non-kernel thread 0x%x tried to QSOS \"%s\"", GetCurrentThreadId(), buffer));

          va_end (ap);
        }
      }
    }
  }
}
//}}}
//{{{
void QAPI_VARGS iLogManager::bufferError (const Char* const format, ...) throw()
{
  QVERIFY(format != 0);

  if (cLogManager::gIsLoggingAvailable)
  {
    cLogManager& lm = cLogManager::singleton();
    cLogThreadData* const td = getCurrThreadLogData();
    if (td)
    {
      // The following macro reads the return address off the stack, looks for the start of
      // the allocation block on the heap and then finds the module name from the start
      // address.
      void* moduleAddr = _ReturnAddress();

      va_list ap;
      va_start (ap, format);

      td->m_NumSOSIndents++;
      lm.bufferMsg (iLogDriver::cMsg::eErrorMsg, 0, moduleAddr, format, ap);
      td->m_NumSOSIndents--;

      va_end (ap);
    }
    else
    {
      if (lm.mIsShowingNonIThreadMsgs)
      {
        // The following macro reads the return address off the stack, looks for the start of
        // the allocation block on the heap and then finds the module name from the start
        // address.
        void* moduleAddr = _ReturnAddress();
        const cLogManager::cLogFlags* const flags = lm.getLogFlags (moduleAddr);

        if (flags->isModVisible())
        {
          va_list ap;
          va_start (ap, format);

          Char buffer[MAX_NUM_LOG_MSG_CHARS];
          if (_vsnwprintf (buffer, MAX_NUM_LOG_MSG_CHARS - 1, format, ap) == MAX_NUM_LOG_MSG_CHARS - 1)
            buffer[MAX_NUM_LOG_MSG_CHARS - 1] = L'\0';

          QTRACE((L"Log Manager : Non-kernel thread 0x%x tried to bufferError \"%s\"", GetCurrentThreadId(), buffer));

          va_end (ap);
        }
      }
    }
  }
}
//}}}
//{{{
void QAPI_VARGS iLogManager::bufferTrace (const Char* const format, ...) throw()
{
  QVERIFY(format != 0);

  if (cLogManager::gIsLoggingAvailable)
  {
    cLogManager& lm = cLogManager::singleton();
    cTime localTime = iSystemInfo::singleton().getCurrentLocalTime();

    // The following macro reads the return address off the stack, looks for the start of
    // the allocation block on the heap and then finds the module name from the start address.
    void* moduleAddr = _ReturnAddress();
    GET_MODULE_NAME(moduleAddr);

    va_list ap;
    va_start (ap, format);

    Char buffer[MAX_NUM_LOG_MSG_CHARS];
    if (_vsnwprintf (buffer, MAX_NUM_LOG_MSG_CHARS - 1, format, ap) == MAX_NUM_LOG_MSG_CHARS - 1)
      buffer[MAX_NUM_LOG_MSG_CHARS - 1] = L'\0';

    iBasicThread* thread = iThreadManager::singleton().getCurrentThread();
    if (thread)
    {
      String threadName;
      cLogThreadData& td = lm.getCurrentThreadDataAndName (threadName);
      if (td.m_SuppressionCount <= 1)
      {
        lm.mOutputThread->pushRecord(
          new iLogDriver::cMsg(
            iLogDriver::cMsg::eTrace,           // Type,
            threadName,                         // ThreadName
            ModuleName,                         // ModuleName
            0,                                  // NumIndents
            0,                                  // NumSOSIndents
            0,                                  // NumInjectedIndents
            0,                                  // NumSuppressedIndents
            0,                                  // Category
            buffer,                             // Text
            lm.mNumSuppressedMsgs,             // NumSuppressedSinceLast
            false,
            localTime,                          // MillisecsSinceStart
            iThreadManager::singleton().getCurrentThreadId() // Thread id
          )
        );
      }
    }
    else if (lm.mIsShowingNonIThreadMsgs)
    {
      lm.mOutputThread->pushRecord(
        new iLogDriver::cMsg(
          iLogDriver::cMsg::eTrace,           // Type,
          L"Non-Kernel Thread",               // ThreadName
          ModuleName,                         // ModuleName
          0,                                  // NumIndents
          0,                                  // NumSOSIndents
          0,                                  // NumInjectedIndents
          0,                                  // NumSuppressedIndents
          0,                                  // Category
          buffer,                             // Text
          lm.mNumSuppressedMsgs,             // NumSuppressedSinceLast
          false,
          localTime,                            // MillisecsSinceStart
          iThreadManager::singleton().getCurrentThreadId() // Thread id
        )
      );
    }

    va_end (ap);
  }
}
//}}}
//{{{
bool QAPI iLogManager::isCategoryVisible (const unsigned long category)
{
  bool isVisible = false;
  if (cLogManager::gIsLoggingAvailable)
  {
    void* moduleAddr = _ReturnAddress();
    isVisible = cLogManager::singleton().visCategoryVisible (category, moduleAddr);
  }
  return isVisible;
}
//}}}
//{{{
void QAPI_VARGS iLogManager::bufferBreak (const Char* const format, ...) throw()
{
  if (cLogManager::gIsLoggingAvailable)
  {
    cLogManager& lm = cLogManager::singleton();
    cTime localTime = iSystemInfo::singleton().getCurrentLocalTime();

    // The following macro reads the return address off the stack, looks for the start of
    // the allocation block on the heap and then finds the module name from the start
    // address.
    void* moduleAddr = _ReturnAddress();
    GET_MODULE_NAME(moduleAddr);

    va_list ap;
    va_start (ap, format);

    Char buffer[MAX_NUM_LOG_MSG_CHARS] = L"\0";
    _vsnwprintf (buffer, sizeof(buffer) / sizeof(Char), format, ap);

    iBasicThread* thread = iThreadManager::singleton().getCurrentThread();
    if (thread)
    {
      String threadName;
      cLogThreadData& td = lm.getCurrentThreadDataAndName (threadName);

      if (td.m_SuppressionCount <= 0)
      {
        lm.mOutputThread->pushRecord(
          new iLogDriver::cMsg(
            iLogDriver::cMsg::eBreak,           // Type,
            threadName,                         // ThreadName
            ModuleName,                         // ModuleName
            0,                                  // NumIndents
            0,                                  // NumSOSIndents
            0,                                  // NumInjectedIndents
            0,                                  // NumSuppressedIndents
            0,                                  // Category
            buffer,                             // Text
            lm.mNumSuppressedMsgs,           // NumSuppressedSinceLast
            false,
            localTime,                            // MillisecsSinceStart
            iThreadManager::singleton().getCurrentThreadId() // Thread id
          )
        );
      }
    }
    else if (lm.mIsShowingNonIThreadMsgs)
    {
      lm.mOutputThread->pushRecord(
        new iLogDriver::cMsg(
          iLogDriver::cMsg::eTrace,           // Type,
          L"Non-iThread",                     // ThreadName
          ModuleName,                         // ModuleName
          0,                                  // NumIndents
          0,                                  // NumSOSIndents
          0,                                  // NumInjectedIndents
          0,                                  // NumSuppressedIndents
          0,                                  // Category
          buffer,                             // Text
          lm.mNumSuppressedMsgs,           // NumSuppressedSinceLast
          false,
          localTime,                            // MillisecsSinceStart
          iThreadManager::singleton().getCurrentThreadId() // Thread id
        )
      );
    }
    va_end (ap);
  }
}
//}}}
//----------------------------------------------------------------------------------------------------------------------
// iLogManager interface
//{{{
void QAPI cLogManager::INTERNAL_USE_ONLY_createLogIndentGuard (cLogIndentGuard* const guard, const unsigned int newCategory, const Char* const format, const va_list & args)
{
  if (cLogManager::gIsLoggingAvailable)
  {
    void* moduleAddr = _ReturnAddress();

    cLogManager& lm = cLogManager::singleton();
    if (lm.isAKernelThread())
      lm.bufferIndent (newCategory, moduleAddr, &guard->m_PrevCtg, &guard->m_PrevModuleAddr, format, args);
    else
    {
      if (lm.mIsShowingNonIThreadMsgs)
      {
        const cLogManager::cLogFlags* const flags = lm.getLogFlags (moduleAddr);
        if (flags->isModVisible()) //(QPRVLOG_IS_MOD_VISIBLE2(impl, moduleAddr))
        {
          Char buffer[MAX_NUM_LOG_MSG_CHARS] = L"\0";
          _vsnwprintf (buffer, sizeof(buffer) / sizeof(Char), format, args);
          QTRACE((L"Non-kernel thread 0x%x tried to cLogIndentGuard \"%s\"", GetCurrentThreadId(), buffer));
        }
      }
    }
  }
  else
    guard->m_PrevCtg = kLogManager_BadCtg;
}
//}}}
//{{{
void QAPI cLogManager::INTERNAL_USE_ONLY_destroyLogIndentGuard (cLogIndentGuard* const guard)
{
  if (cLogManager::gIsLoggingAvailable && (guard->m_PrevCtg != kLogManager_BadCtg))
  {
    cLogManager& lm = cLogManager::singleton();
    if (lm.isAKernelThread())
      lm.bufferOutdent (guard->m_PrevCtg, guard->m_PrevModuleAddr);
  }
}
//}}}
//{{{
void QAPI cLogManager::INTERNAL_USE_ONLY_createLogSuppressGuard (cLogSuppressGuard* const guard)
{
  if (cLogManager::gIsLoggingAvailable)
  {
    cLogManager& lm = cLogManager::singleton();
    if (lm.isAKernelThread())
    {
      String threadName;
      cLogThreadData& threadData = lm.getCurrentThreadDataAndName (threadName);
      threadData.m_SuppressionCount++;
    }
  }
}
//}}}
//{{{
void QAPI cLogManager::INTERNAL_USE_ONLY_destroyLogSuppressGuard (cLogSuppressGuard* const guard)
{
  if (cLogManager::gIsLoggingAvailable)
  {
    cLogManager& lm = cLogManager::singleton();
    if (lm.isAKernelThread())
    {
      String threadName;
      cLogThreadData& threadData = lm.getCurrentThreadDataAndName (threadName);
      threadData.m_SuppressionCount--;
    }
  }
}
//}}}
//{{{
void QAPI cLogManager::INTERNAL_USE_ONLY_createLogIndentSOSGuard (cLogIndentSOSGuard* const guard, const Char* const format, const va_list& args)
{
  if (cLogManager::gIsLoggingAvailable)
  {
    // The following macro reads the return address off the stack, looks for the start of
    // the allocation block on the heap. Result is in ModuleAddr.
    void* moduleAddr = _ReturnAddress();

    cLogManager& lm = cLogManager::singleton();
    cLogThreadData* td = getCurrThreadLogData();
    if (td)
    {
      td->m_NumSOSIndents++;
      lm.bufferIndent (0, moduleAddr, &guard->m_PrevCtg, &guard->m_PrevModuleAddr, format, args);
      td->m_NumSOSIndents--;
    }
    else
    {
      if (lm.mIsShowingNonIThreadMsgs)
      {
        const cLogManager::cLogFlags* const flags = lm.getLogFlags (moduleAddr);
        if (flags->isModVisible()) //(QPRVLOG_IS_MOD_VISIBLE2(lm, moduleAddr))
        {
          Char buffer[MAX_NUM_LOG_MSG_CHARS] = L"\0";
          _vsnwprintf (buffer, sizeof(buffer) / sizeof(Char), format, args);
          QTRACE((L"Non-kernel thread 0x%x tried to cLogIndentSOSGuard \"%s\"", GetCurrentThreadId(), buffer));
        }
      }
    }
  }
  else
    guard->m_PrevCtg = kLogManager_BadCtg;
}
//}}}
//{{{
void QAPI cLogManager::INTERNAL_USE_ONLY_destroyLogIndentSOSGuard (cLogIndentSOSGuard* const guard)
{
  if (cLogManager::gIsLoggingAvailable && (guard->m_PrevCtg != kLogManager_BadCtg))
  {
    cLogManager& lm = cLogManager::singleton();
    if (lm.isAKernelThread())
      lm.bufferOutdent (guard->m_PrevCtg, guard->m_PrevModuleAddr);
  }
}
//}}}
//{{{
void cLogManager::flushBuffer()
{
  if (mOutputThread.isValid())
    mOutputThread->flushBuffer();
}
//}}}
//----------------------------------------------------------------------------------------------------------------------
// cLogManager interface
//{{{
void cLogManager::lockBuffer()
{
  mOutputThread->lockBuffer();
}
//}}}
//{{{
void cLogManager::releaseBuffer()
{
  mOutputThread->releaseBuffer();
}
//}}}
//{{{
void cLogManager::startDrivers()
{
  gIsLoggingAvailable = mOutputThread->startDrivers();
  mOutputThread->startLoop();
  mAreDriversStarted = true;
}
//}}}
//{{{
void cLogManager::stopDrivers()
{
  mAreDriversStarted = false;
  mOutputThread->stopDrivers();
}
//}}}
//{{{
void cLogManager::addModule (const String& developerName, const String& moduleName, const cVersion& moduleVersion, const HINSTANCE dllInstance)
{
  QVERIFY(dllInstance != 0);

  MODULEINFO moduleInfo;
  if (!GetModuleInformation(GetCurrentProcess(), dllInstance, &moduleInfo, sizeof(MODULEINFO)))
  {
    QFATAL((__FILE__, __LINE__, L"Tried to GetModuleInformation() on module \"%s\"", moduleName.c_str()));
  }
  const UIntPtr start = ((UIntPtr)moduleInfo.lpBaseOfDll) >> QMODULE_PAGE_SIZE_NUM_BITS;
  {
    // Check if already registered
    cLockGuard g(&mFlagMapLock);
    pair<UIntPtr,cLogFlags*> findFlags = make_pair (start, (cLogFlags*)0);
    vector<pair<UIntPtr,cLogFlags*>>::iterator foundIt = lower_bound (mFlagMap.begin(), mFlagMap.end(), findFlags, flagPairPredicate);
    if (foundIt != mFlagMap.end() && (*foundIt).first == start)
      return;
  }

  String moduleKey;
  cLogIndentGuard indent(kCtgLoadLoggingCategories, L"cLogManager::addModule: %s\\%s", developerName.c_str(), moduleName.c_str());
  {
    StringStream tmp;
    tmp <<
      cSystemInfo::singleton().getRegistryPathToModules() <<
      developerName << L"\\" <<
      moduleName << L"\\" <<
      moduleVersion.m_Release << L"." << moduleVersion.m_MajorVersion << L"." << moduleVersion.m_MinorVersion << L".x\\" <<
      L"Log Categories";
    moduleKey = tmp.str();
  }

  cLogFlags* flags = new cLogFlags (dllInstance, moduleKey, moduleName);
  updateLogFlags (flags);

  QMSG((0, L"Is Module Logging Enabled   = %s", flags->isModVisible() ? L"Yes" : L"No"));
  QMSG((0, L"Is Highlighted              = %s", flags->isModHighlighted() ? L"Yes" : L"No"));
  QMSG((0, L"%s DLL in memory : start 0x%p", moduleName.c_str(), moduleInfo.lpBaseOfDll));

  {
    cLockGuard g(&mFlagMapLock);
    pair<UIntPtr,cLogFlags*> modFlagsPair = make_pair (start, flags);
    vector<pair<UIntPtr,cLogFlags*>>::iterator addIt = lower_bound (mFlagMap.begin(), mFlagMap.end(), modFlagsPair, flagPairPredicate);
    mFlagMap.insert (addIt, modFlagsPair);
  }
}
//}}}
//{{{
void QAPI cLogManager::refreshModuleFlags()
{
  QTRACE((L">>"));
  {
    cLockGuard g(&mFlagMapLock);
    for (vector<pair<UIntPtr,cLogFlags*>>::iterator it = mFlagMap.begin(); it != mFlagMap.end(); ++it)
    {
      if (updateLogFlags (it->second))
        QTRACE((L"cLogManager::refreshModuleFlags():        Updated log categories for \"%s\"", it->second->getModuleKey().c_str()));
    }
  }
  QTRACE((L"<<"));
}
//}}}
//----------------------------------------------------------------------------------------------------------------------
// privates
//{{{
bool cLogManager::updateLogFlags (cLogFlags* flags)
// Modifies the <flags> record if it can.
// Takes the moduleKey and moduleName from the <flags> record.
// Returns true if flags have changed.
{
  if (flags == NULL)
    return false;

  unsigned char moduleFlags = 0;
  iLogManager::Category collapseBits = 0;
  unsigned long injectExpandedBits = 0;
  unsigned long injectCollapsedBits = 0;
  unsigned long hideBits = 0;
  bool isModuleSuppressed = false;

  String moduleKey = flags->getModuleKey();
  String moduleName = flags->getModuleName();

  if (mRegHelp->isKeyCreated (moduleKey))
  {
    isModuleSuppressed = (mRegHelp->getItemNumber (moduleKey, L"Is Module Logging Enabled") == 0);
    bool isModuleMsgsHighlighted =  (mRegHelp->getItemNumber (moduleKey, L"Is Highlighted") != 0);

    cCategoryIterator::Ptr it = new cCategoryIterator (moduleName);
    {
      cLogIndentGuard indent(0, L"Categories...");
      mRegHelp->enumerateTree (moduleKey, ptr_cast<iRegistryHelp::iItemIterator::Ptr>(it));
    }

    collapseBits        = it->GetCollapseBits();
    moduleFlags         = it->GetModuleFlags();
    injectExpandedBits  = it->GetInjectExpandedBits();
    injectCollapsedBits = it->GetInjectCollapsedBits();
    hideBits            = it->GetHideBits();            // not used CT?

    if (isModuleSuppressed)
      collapseBits = 0xffffffff;

    if (isModuleMsgsHighlighted)
      moduleFlags |= kLogManager_ModuleFlagHighighted;
  }
  else
  {
    QMSG((0, L"No categories found - all categories logged by default"));
  }
  // return change status
  return flags->updateFlags (moduleFlags, collapseBits, injectExpandedBits, injectCollapsedBits, isModuleSuppressed);
}
//}}}
//{{{
bool cLogManager::visCategoryVisible (const unsigned long category, void* moduleAddr)
{
  bool isVisible = false;
  const cLogFlags* const flags = getLogFlags (moduleAddr);

  if (flags->isModVisible())
  {
    if (isAKernelThread())
    {
      String threadName;
      cLogThreadData& td = getCurrentThreadDataAndName (threadName);

      if (td.m_SuppressionCount <= 0)
      {
        if (td.m_NumSOSIndents > 0)
          isVisible = true;
        else if (!flags->isModVisible())
          ; // Do nothing.
        else if ((td.m_NumInjectedIndents > 0) || flags->isCtgInjectingExpanded (category))
          isVisible = true;
        else if (flags->isCtgInjectingCollapsed (category))
          isVisible = true;
        else if ((td.m_NumSuppressedIndents == 0) && flags->isCtgVisible (category))
          isVisible = true;
      }
    }
    else
    {
      if (mIsShowingNonIThreadMsgs)
      {
        if (flags->isModVisible())
          isVisible = true;
      }
    }
  }
  return isVisible;
}
//}}}
//{{{
bool cLogManager::isHighlighted (void* moduleAddr)
{
  const cLogFlags* const flags = getLogFlags (moduleAddr);
  return flags->isModHighlighted();
}
//}}}
//{{{
bool cLogManager::isAKernelThread()
{
  return iThreadManager::singleton().getCurrentThread() != 0;
}
//}}}
//{{{
cLogThreadData& cLogManager::getCurrentThreadDataAndName (String& name)
{
  // get the task pointer from the TIB
  void* fs14 = (void*) __readgsqword(0x28);
  cTib14* const tib = reinterpret_cast<cTib14*>(fs14);
  if (!tib)
  {
    QFATAL((__FILE__, __LINE__, L"Non-Kernel thread tried to write to log"));
  }
  name = tib->m_ThreadImpl->m_Name;
  return tib->m_LogData;
}
//}}}
//{{{
const cLogManager::cLogFlags* cLogManager::getLogFlags (void* const moduleAddr)
{
  const UIntPtr modulePage = (UIntPtr)moduleAddr >> QMODULE_PAGE_SIZE_NUM_BITS;

  cLockGuard g(&mFlagMapLock);
  if (mLastModulePage == modulePage)
    // Quick return if same as last time
    return mLastFlags;

  if (mFlagMap.empty() || (moduleAddr == 0))
    // No module registered yet for this addess
    return &gNullFlags;

  // find the flags for this module by getting an iterator to the base page for the calling module
  pair<UIntPtr,cLogFlags*> findFlags = make_pair (modulePage, (cLogFlags*)0);
  vector<pair<UIntPtr,cLogFlags*>>::iterator foundIt = lower_bound (mFlagMap.begin(), mFlagMap.end(), findFlags, flagPairPredicate);

  if (foundIt == mFlagMap.end()) // can't log !!!
    __debugbreak();

  const cLogFlags* const flags = foundIt->second;
  mLastModulePage = modulePage;
  mLastFlags = flags;

  return flags;
}
//}}}
//{{{
void cLogManager::bufferLazyIndent (void* moduleAddr)
{
  String threadName;
  cLogThreadData& td = getCurrentThreadDataAndName (threadName);

  if (td.m_IsLazyIndenting)
  {
    td.m_IsLazyIndenting = false;

    cTime localTime = iSystemInfo::singleton().getCurrentLocalTime();

    GET_MODULE_NAME(moduleAddr);        // sets up local var ModuleName
    iBasicThread* thread = iThreadManager::singleton().getCurrentThread();

    if (td.m_SuppressionCount <= 0)
    if (td.m_SuppressionCount <= 0 || td.m_NumSOSIndents > 0)
    {
      mOutputThread->pushRecord(
        new iLogDriver::cMsg(
          iLogDriver::cMsg::eLazyIndent,      // Type,
          thread ? thread->getName() : L"Non-Kernel Thread", // ThreadName
          ModuleName,                         // ModuleName
          td.m_Indent,                // NumIndents
          td.m_NumSOSIndents,         // NumSOSIndents
          td.m_NumInjectedIndents,    // NumInjectedIndents
          td.m_NumSuppressedIndents,  // NumSuppressedIndents
          0,                                  // Category
          L"",                                // Text
          mNumSuppressedMsgs,                // NumSuppressedSinceLast
          false,                              // IsHighlighted
          localTime,                            // MillisecsSinceStart
          iThreadManager::singleton().getCurrentThreadId() // Thread id
        )
      );
    }
    mNumSuppressedMsgs = 0;
    td.m_Indent++;
  }
}
//}}}
//{{{
void cLogManager::bufferMsg (iLogDriver::cMsg::eMsgType msgType, const unsigned long category, void* moduleAddr, const Char* const format, va_list ap)
{
  QVERIFY(format != 0);
  QVERIFY(isAKernelThread());

  cTime localTime = iSystemInfo::singleton().getCurrentLocalTime();
  String threadName;
  cLogThreadData& td = getCurrentThreadDataAndName (threadName);
  const cLogFlags* const flags = getLogFlags (moduleAddr);

  int tmpInjecting = 0;
  bool isLogging = false;

  if (td.m_NumSOSIndents > 0)
    isLogging = true;
  else if (!flags->isModVisible())
    ;
  else if ((td.m_NumInjectedIndents > 0) || flags->isCtgInjectingExpanded (category))
  {
    tmpInjecting = 1;
    isLogging = true;
  }
  else if (flags->isCtgInjectingCollapsed (category))
  {
    tmpInjecting = 1;
    isLogging = true;
  }
  else if ((td.m_NumSuppressedIndents == 0) && flags->isCtgVisible (category))
    isLogging = true;

  if (isLogging)
  {
    bufferLazyIndent (moduleAddr);

    td.m_NumInjectedIndents += tmpInjecting;

    if (td.m_SuppressionCount <= 0 || td.m_NumSOSIndents > 0)
    {
      GET_MODULE_NAME(moduleAddr);
      Char buffer[MAX_NUM_LOG_MSG_CHARS];
      if (_vsnwprintf (buffer, MAX_NUM_LOG_MSG_CHARS - 1, format, ap) == MAX_NUM_LOG_MSG_CHARS - 1)
        buffer[MAX_NUM_LOG_MSG_CHARS - 1] = L'\0';

      mOutputThread->pushRecord(
        new iLogDriver::cMsg(
          msgType,                            // Type,
          threadName,                         // ThreadName
          ModuleName,                         // ModuleName
          td.m_Indent,                // NumIndents
          td.m_NumSOSIndents,         // NumSOSIndents
          td.m_NumInjectedIndents,    // NumInjectedIndents
          td.m_NumSuppressedIndents,  // NumSuppressedIndents
          category,                           // Category
          buffer,                             // Text
          mNumSuppressedMsgs,                // NumSuppressedSinceLast
          isHighlighted (moduleAddr),         // IsHighlighted
          localTime,                            // MillisecsSinceStart
          iThreadManager::singleton().getCurrentThreadId() // Thread id
          )
        );
    }
    mNumSuppressedMsgs = 0;
    td.m_NumInjectedIndents -= tmpInjecting;
  }
  else
    mNumSuppressedMsgs++;
}
//}}}
//{{{
void cLogManager::bufferIndent (const unsigned long newCategory, void* moduleAddr, unsigned long* prevCategory, void** prevModuleAddr, const Char* const format, va_list ap) throw()
{
  QVERIFY(format != 0);

  String threadName;
  cLogThreadData& td = getCurrentThreadDataAndName (threadName);
  const cLogFlags* const flags = getLogFlags (moduleAddr);

  bool isLogging  = false;
  bool isExpanded = false;

  if (td.m_NumSOSIndents > 0)
  {
    td.m_NumSOSIndents++;
    isExpanded = true;
    isLogging = true;
  }
  else if (!flags->isModVisible())
    td.m_NumSuppressedIndents++;

  else if ((td.m_NumInjectedIndents > 0) || flags->isCtgInjectingExpanded (newCategory))
  {
    td.m_NumInjectedIndents++;
    isExpanded = true;
    isLogging = true;
  }
  else if (flags->isCtgInjectingCollapsed (newCategory))
  {
    td.m_NumSuppressedIndents++;
    isExpanded = false;
    isLogging = true;
  }
  else if (td.m_NumSuppressedIndents == 0)
  {
    const cLogFlags* const threadFlags = getLogFlags (td.m_CurrCtgModuleAddr);
    if (threadFlags->isCtgVisible (td.m_CurrCtg))
    {
      isLogging = true;
      if (flags->isCtgVisible (newCategory))
        isExpanded = true;
      else
        td.m_NumSuppressedIndents++;
    }
  }
  else
    td.m_NumSuppressedIndents++;

  if (isLogging)
  {
    bufferLazyIndent (moduleAddr);

    if ((td.m_SuppressionCount <= 0) || (td.m_NumSOSIndents > 0))
    {
      cTime localTime = iSystemInfo::singleton().getCurrentLocalTime();
      Char buffer[MAX_NUM_LOG_MSG_CHARS];
      if (_vsnwprintf (buffer, MAX_NUM_LOG_MSG_CHARS - 1, format, ap) == MAX_NUM_LOG_MSG_CHARS - 1)
        buffer[MAX_NUM_LOG_MSG_CHARS - 1] = L'\0';

      iBasicThread* thread = iThreadManager::singleton().getCurrentThread();

      GET_MODULE_NAME(moduleAddr);
      mOutputThread->pushRecord(
        new iLogDriver::cMsg(
          isExpanded ? iLogDriver::cMsg::eExpandedIndent : iLogDriver::cMsg::eCollapsedIndent, // Type,
          thread ? thread->getName() : L"Non-Kernel Thread", // ThreadName
          ModuleName,                         // ModuleName
          td.m_Indent,                // NumIndents
          td.m_NumSOSIndents,         // NumSOSIndents
          td.m_NumInjectedIndents,    // NumInjectedIndents
          td.m_NumSuppressedIndents,  // NumSuppressedIndents
          newCategory,                        // Category
          buffer,                             // Text
          mNumSuppressedMsgs,                  // NumSuppressedSinceLast
          isHighlighted (moduleAddr),           // IsHighlighted
          localTime,                            // MillisecsSinceStart
          iThreadManager::singleton().getCurrentThreadId())); // Thread id
    }
    mNumSuppressedMsgs = 0;

    if (isExpanded)
      td.m_IsLazyIndenting = true;
  }
  else
    mNumSuppressedMsgs++;

  *prevCategory   = td.m_CurrCtg;
  *prevModuleAddr = td.m_CurrCtgModuleAddr;
  td.m_CurrCtg = newCategory;
  td.m_CurrCtgModuleAddr = moduleAddr;
}
//}}}
//{{{
void cLogManager::bufferOutdent (const unsigned long categoryToRestore, void* moduleAddr)
{
  String threadName;
  cLogThreadData& td = getCurrentThreadDataAndName (threadName);

  if (td.m_IsLazyIndenting)
    td.m_IsLazyIndenting = false;
  else
  {
    bool isLogging = false;

    if (td.m_NumSOSIndents > 0)
      isLogging = true;
    else if (td.m_NumInjectedIndents > 0)
      isLogging = true;
    else if (td.m_NumSuppressedIndents == 0)
      isLogging   = true;

    if (isLogging)
    {
      td.m_Indent--;
      if (td.m_Indent == 0xffffffff)
      {
        QTRACE((L"Indent level < 0!")); // Can't buffer anything in here - must use QTRACE!!
        td.m_Indent = 0;
      }

      if (td.m_SuppressionCount <= 0 || td.m_NumSOSIndents > 0)
      {
        cTime localTime = iSystemInfo::singleton().getCurrentLocalTime();
        GET_MODULE_NAME(moduleAddr);
        mOutputThread->pushRecord(
          new iLogDriver::cMsg(
            iLogDriver::cMsg::eOutdent,         // Type,
            threadName,                         // ThreadName
            ModuleName,                         // ModuleName
            td.m_Indent,                // NumIndents
            td.m_NumSOSIndents,         // NumSOSIndents
            td.m_NumInjectedIndents,    // NumInjectedIndents
            td.m_NumSuppressedIndents,  // NumSuppressedIndents
            0,                                  // Category
            L"",                                // Text
            mNumSuppressedMsgs,                // NumSuppressedSinceLast
            false,                              // IsHighlighted
            localTime,                            // MillisecsSinceStart
            iThreadManager::singleton().getCurrentThreadId() // Thread id
          )
        );
      }
      mNumSuppressedMsgs = 0;
    }

    td.m_CurrCtg = categoryToRestore;
    td.m_CurrCtgModuleAddr = moduleAddr;
  }

  if (td.m_NumSOSIndents > 0)
    td.m_NumSOSIndents--;
  else if (td.m_NumInjectedIndents > 0)
    td.m_NumInjectedIndents--;
  else if (td.m_NumSuppressedIndents > 0)
    td.m_NumSuppressedIndents--;
}
//}}}
