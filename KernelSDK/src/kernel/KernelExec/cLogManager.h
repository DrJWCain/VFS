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

#include <stdarg.h>
#include <algorithm>
#include "QKernelExec.h"
#include "cThreadManager.h"
#include "cLogThreadData.h"
#include "cThreadImpl.h"
#include "cLogOutputThread.h"
#include <psapi.h>

#define MAX_NUM_LOG_MSG_CHARS 2048
#define QMODULE_PAGE_SIZE_NUM_BITS  16

//{{{
// Use the fact that HINSTANCE is the base address of where the module is loaded to find the module name.
// It eventually resolves to a (void*). HINSTANCE = HANDLE = PVOID = void*
#define GET_MODULE_NAME(___ModuleAddr) \
  MEMORY_BASIC_INFORMATION mbi; \
  Char ModuleName[MAX_PATH]; \
  if (VirtualQuery((LPCVOID)___ModuleAddr, &mbi, sizeof(MEMORY_BASIC_INFORMATION)) == sizeof(MEMORY_BASIC_INFORMATION)) \
  { \
    if (GetModuleBaseName(GetCurrentProcess(), (HINSTANCE) mbi.AllocationBase, ModuleName, MAX_PATH) !=0) \
    { \
    } \
    else \
    { \
      wcscpy(ModuleName, L"UNKNOWN"); \
    } \
  } \
  else \
  { \
    wcscpy(ModuleName, L"UNLOADED"); \
  }
//}}}

const unsigned long kLogManager_BadCtg = 0xffffffff;
const unsigned char kLogManager_ModuleFlagHighighted = 1;

// =======================================================================================
// === cLogManager =======================================================================
// =======================================================================================

class cLogManager : public iLogManager, public cRefCount
{
friend class iLogManager;
friend class cCategoryIterator;
friend class cLogIndentGuard;
friend class cLogIndentSOSGuard;
friend class cLogSuppressGuard;
friend class cLogOutputThread;
friend class cExceptionManager;

public:
  QDEFINE_SINGLETON_IMPL(cLogManager, L"{5E5FE0B8-E9B9-45E2-A8EB-5AAD64DAE8D7} // Log manager impl", iLogManager);

  static bool gIsLoggingAvailable;

  cLogManager();
  ~cLogManager();

  // iManager
  void QAPI logSummary()                { }
  void QAPI logDetailedSummary()        { }

  // iLogManager
  //{{{
  class cLogFlags
  {
  public:
    cLogFlags()
      : mDllInstance(NULL), mModuleFlags(0), mCollapseBits(0), mInjectExpandedBits(0), mInjectCollapsedBits(0), mIsModSuppressed(false)
    { }

    cLogFlags (HINSTANCE dllInstance, const String& moduleKey, const String& moduleName)
      : mDllInstance(dllInstance),
        mModuleKey(moduleKey),
        mModuleName(moduleName),
        mModuleFlags(0),
        mCollapseBits(0),
        mInjectExpandedBits(0),
        mInjectCollapsedBits(0),
        mIsModSuppressed(false)
    { }

    cLogFlags (
      HINSTANCE dllInstance,
      const String& moduleKey,
      const String& moduleName,
      unsigned int moduleFlags,
      unsigned int collapseBits,
      unsigned int injectExpandingBits,
      unsigned int injectCollapsedBits,
      bool isModSuppressed
      )
      : mDllInstance(dllInstance),
        mModuleKey(moduleKey),
        mModuleName(moduleName),
        mModuleFlags(moduleFlags),
        mCollapseBits(collapseBits),
        mInjectExpandedBits(injectExpandingBits),
        mInjectCollapsedBits(injectCollapsedBits),
        mIsModSuppressed(isModSuppressed)
    { }

    HINSTANCE getDllInstance() const                            { return mDllInstance; }
    String getModuleKey() const                                 { return mModuleKey; }
    String getModuleName() const                                { return mModuleName; }

    bool isCtgVisible (unsigned int ctg) const             { return (ctg & ~mCollapseBits) == ctg; }
    bool isCtgInjectingExpanded (unsigned int ctg) const   { return (ctg != 0) && ((ctg & mInjectExpandedBits) == ctg); }
    bool isCtgInjectingCollapsed (unsigned int ctg) const  { return (ctg != 0) && ((ctg & mInjectCollapsedBits) == ctg); }
    bool isModHighlighted() const                          { return (mModuleFlags & kLogManager_ModuleFlagHighighted) != 0; }
    bool isModVisible() const                              { return !mIsModSuppressed; }

    bool updateFlags (unsigned int moduleFlags, unsigned int collapseBits, unsigned int injectExpandedBits, unsigned int injectCollapsedBits, bool isModSuppressed)
    // returns true if different
    {
      if((mModuleFlags == moduleFlags)
      && (mCollapseBits == collapseBits)
      && (mInjectExpandedBits == injectExpandedBits)
      && (mInjectCollapsedBits == injectCollapsedBits)
      && (mIsModSuppressed == isModSuppressed) )
        return false; // no change

      mModuleFlags = moduleFlags;
      mCollapseBits = collapseBits;
      mInjectExpandedBits = injectExpandedBits;
      mInjectCollapsedBits = injectCollapsedBits;
      mIsModSuppressed = isModSuppressed;
      return true;
    }

  private:
    const HINSTANCE mDllInstance;
    const String mModuleKey;
    const String mModuleName;

    unsigned int mModuleFlags;
    unsigned int mCollapseBits;
    unsigned int mInjectExpandedBits;
    unsigned int mInjectCollapsedBits;
    bool mIsModSuppressed;
  };
  //}}}

  void QAPI INTERNAL_USE_ONLY_createLogIndentGuard (cLogIndentGuard* const guard, const unsigned int category, const Char* const format, const va_list& args);
  void QAPI INTERNAL_USE_ONLY_destroyLogIndentGuard (cLogIndentGuard* const guard);
  void QAPI INTERNAL_USE_ONLY_createLogSuppressGuard (cLogSuppressGuard* const guard);
  void QAPI INTERNAL_USE_ONLY_destroyLogSuppressGuard (cLogSuppressGuard* const guard);
  void QAPI INTERNAL_USE_ONLY_createLogIndentSOSGuard (cLogIndentSOSGuard* const guard, const Char* const format, const va_list& args);
  void QAPI INTERNAL_USE_ONLY_destroyLogIndentSOSGuard (cLogIndentSOSGuard* const guard);
  void QAPI flushBuffer();
  void QAPI refreshModuleFlags();

  void lockBuffer();
  void releaseBuffer();
  void startDrivers();
  void stopDrivers();
  void addModule (const String& developerName, const String& moduleName, const cVersion& moduleVersion, const HINSTANCE dllInstance);

private:
  iRegistryHelp::Ptr mRegHelp;
  bool mAreDriversStarted;
  bool mIsShowingNonIThreadMsgs;
  cLock mFlagMapLock;
  std::vector<std::pair<UIntPtr,cLogFlags*>> mFlagMap;
  UIntPtr mLastModulePage;
  const cLogFlags* mLastFlags;
  unsigned int mNumSuppressedMsgs;
  cLogOutputThread::Ptr mOutputThread;

  bool updateLogFlags (cLogFlags* flags);
  bool visCategoryVisible (const unsigned long category, void* moduleAddr);
  bool isHighlighted (void* moduleAddr);
  bool isAKernelThread();
  cLogThreadData& getCurrentThreadDataAndName (String& name);
  const cLogFlags* getLogFlags (void* const moduleAddr);

  void bufferLazyIndent (void* moduleAddr);
  void bufferMsg (iLogDriver::cMsg::eMsgType msgType, const unsigned long category, void* moduleAddr, const Char* const format, va_list ap);
  void bufferIndent (const unsigned long newCategory, void* moduleAddr, unsigned long* prevCategory, void** prevModuleAddr, const Char* const format, va_list ap) throw();
  void bufferOutdent (const unsigned long categoryToRestore, void* moduleAddr);
};

//{{{
class cCategoryIterator : public iRegistryHelp::iItemIterator, public cRefCount
{
public:
  QDEFINE_SMARTPTR(cCategoryIterator);

  cCategoryIterator(const String& moduleName)
    : mModuleName(moduleName),
      mCollapseBits(0),
      mHideBits(0),
      mModuleFlags(0),
      mInjectExpandedBits(0),
      mInjectCollapsedBits(0)
  {
  }

  bool onSubKey (const String& keyPath, const list<String>& subKeyStack, const iRegistryHelp::Ptr& regHelp)
  {
    return false; // go into?
  }

  void onItem (const String& keyPath, const String& itemName, const list<String>& subKeyStack, const iRegistryHelp::Ptr& regHelp)
  {
    unsigned long bitNum = 0;
    Char bitName[128] = L"";

    const unsigned int numFieldsConverted = swscanf (itemName.c_str(), L"%d %s", &bitNum, bitName);
    if (numFieldsConverted == 2)
    {
      if (bitNum > 31)
      {
        QSOS((L"Log Manager : Badly formed log category \"%s\" for module \"%s\"", itemName.c_str(), mModuleName.c_str()));
        return;
      }

      String itemString = regHelp->getItemString (keyPath, itemName);
      std::transform (itemString.begin(), itemString.end(), itemString.begin(), toupper);

      if (itemString == L"COLLAPSE")
      {
        mCollapseBits |= (1 << bitNum);
      }
      else if (itemString == L"EXPAND")
      {
        // Do nothing , default is fine.
      }
      else if (itemString == L"HIDE") // Yuck
      {
        mHideBits |= (1 << bitNum);
      }
      else if (itemString == L"INJECT") // For compatibility only
      {
        mInjectExpandedBits |= (1 << bitNum);
      }
      else if (itemString == L"INJECTEXPANDED")
      {
        mInjectExpandedBits |= (1 << bitNum);
      }
      else if (itemString == L"INJECTCOLLAPSED")
      {
        mInjectCollapsedBits |= (1 << bitNum);
      }
      else
      {
        QSOS((L"Log Manager : Unsupported log category bit value \"%s\" for category \"%s\" in module \"%s\"", itemString.c_str(), itemName.c_str(), mModuleName.c_str()));
        return;
      }

      QMSG((0, L"Bit %2d \"%-20s\" = \"%s\"", bitNum, bitName, itemString.c_str()));
    }
    else
    {
      // Ignore item that was not formatted properly.
    }
  }

  unsigned char         GetModuleFlags()         { return mModuleFlags; }
  iLogManager::Category GetCollapseBits()        { return mCollapseBits; }
  unsigned long         GetInjectExpandedBits()  { return mInjectExpandedBits; }
  unsigned long         GetInjectCollapsedBits() { return mInjectCollapsedBits; }
  unsigned long         GetHideBits()            { return mHideBits; }

private:
  String mModuleName;
  iLogManager::Category mCollapseBits;
  unsigned char mModuleFlags;
  unsigned long mInjectExpandedBits;
  unsigned long mInjectCollapsedBits;
  unsigned long mHideBits;
};
//}}}
