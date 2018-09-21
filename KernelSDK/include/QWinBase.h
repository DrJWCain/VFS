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


// QWinBase.h
//{{{  includes
#if !defined(__505910BB34724ea5843FCF6EC0B91C33)
#define __505910BB34724ea5843FCF6EC0B91C33
/*
**  
*/

#if defined(QWINBASE_ATL)
  #include <crtdbg.h> // required before malloc
  #include <malloc.h> // required for some inline atl code
#endif

#if defined(_MSC_VER)
  #pragma warning(push, 3)  // The headers in here won't compile on a higher warning level
#endif

  #undef _WIN32_WINNT
  #define _WIN32_WINNT 0x0600 // vista/server 2008
  #undef WINVER
  #define WINVER       0x0600 // vista/server 2008

  #if !defined(STRICT)
    #define STRICT            // Extra type safety because I'm using C++ please.
  #endif

  #if !defined(WIN32_EXTRA_LEAN)
    #define WIN32_EXTRA_LEAN  // We only need the bare bones of Windows.
  #endif

  #if !defined(WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
  #endif

  #include <windows.h>
  #if defined(QWINBASE_SHELLAPI)
    #include <shellapi.h>     // normally, excluded by WIN32_LEAN_AND_MEAN
  #endif

#if defined(_MSC_VER)
  #pragma warning(pop)
#endif

#include <QKernelBase.h>

#if defined(QWINBASE_ATL)
  #include <atlbase.h>
  #include <atlwin.h>
#endif
//}}}

namespace vfs
{
//{{{
// =======================================================================================
// === Error Help ========================================================================
// =======================================================================================
/*
*/
extern String win32ErrorToString(const long ErrorCode);

//}}}
//{{{
// =======================================================================================
// === cWin32Error =======================================================================
// =======================================================================================
/*
*/
typedef cOSError cWin32Error;
//}}}

//{{{
// =======================================================================================
// === cGdiObjSelectGuard ================================================================
// =======================================================================================

class cGdiObjSelectGuard
{
  public :
    cGdiObjSelectGuard(const HDC DC, const HGDIOBJ NewObjectHandle) throw(cWin32Error) : m_DC(DC)
    {
      m_OrigSelected = SelectObject(DC, NewObjectHandle);
      if (m_OrigSelected == HGDI_ERROR || m_OrigSelected == 0)
      {
        throw cWin32Error(__FILE__, __LINE__, L"Tried to SelectObject() %d into DC %d", NewObjectHandle, DC);
      }
    }
    ~cGdiObjSelectGuard() throw(cWin32Error)
    {
      (void)SelectObject(m_DC, m_OrigSelected);
    }
  private :
    HDC     m_DC;
    HGDIOBJ m_OrigSelected;
};
//}}}
//{{{
// =======================================================================================
// === cKeyGuard =========================================================================
// =======================================================================================

class cKeyGuard
{
  public :
    HKEY m_KeyHandle;
    cKeyGuard(HKEY KeyHandle) : m_KeyHandle(KeyHandle)
    {
    }
    ~cKeyGuard()
    {
      if (m_KeyHandle)
        (void)RegCloseKey(m_KeyHandle);
    }
};
//}}}

//{{{  cComPtr
// =======================================================================================
// === cComPtr ===========================================================================
// =======================================================================================

template<typename T> class cComPtr
{
  public :

    T * it;

    cComPtr() throw() : it(0)                                       {}
    cComPtr(T * Interface) throw() : it(Interface)                  { if (it) it->AddRef();  }
    ~cComPtr() throw()                                              { (void)invalidate(); }
    cComPtr(const cComPtr & Other) throw()                          { it = Other.it;
                                                                      if (it) it->AddRef(); }

    bool operator == (const cComPtr<T> & Other) const throw()  { return it == Other.it; }
    bool operator != (const cComPtr<T> & Other) const throw()  { return !(*this == Other); }

    T* operator=(T * const Other) throw()                      { if (it) it->Release();
                                                                      it = Other;
                                                                      if (it) it->AddRef();
                                                                      return it; }
    T* operator=(const cComPtr<T> & Other) throw()             { if (it) it->Release();
                                                                      it = Other.it;
                                                                      if (it) it->AddRef();
                                                                      return it; }
    #if defined(_DEBUG)
      operator T*() const throw()                              { assert(it); return (T*)it; }
      T *  operator->() const throw()                          { assert(it); return it; }
      T &  operator*() const throw()                           { assert(it); return *it; }
      T ** operator&() throw()                                 { assert(it == 0); return &it; }
    #else
      operator T*() const throw()                              { return (T*)it; }
      T *  operator->() const throw()                          { return it; }
      T &  operator*() const throw()                           { return *it; }
      T ** operator&() throw()                                 { return &it; }
    #endif

    bool isValid() const throw()                               { return it != 0; }
    unsigned int invalidate() throw()                          { unsigned int RefCount = 0;
                                                                      if (it) RefCount = it->Release();
                                                                      it = 0;
                                                                      return RefCount; }
};

#define COMFATAL(___MSG) \
  QFATAL(( \
    __FILE__, __LINE__, \
    ___MSG L" - returned 0x%08x (\"%s\")", \
    hr, vfs::win32ErrorToString(hr).c_str()))

#define COMSAFECALL(__EXP) \
  { hr = __EXP; if (FAILED(hr)) COMFATAL(L#__EXP); }

//}}}
//{{{  Guards
// =======================================================================================
// === Guards ============================================================================
// =======================================================================================
//{{{
template<typename T> class cWin32Guard
{
  public :
    const T it;
    cWin32Guard() : it(0) {}
    cWin32Guard(T initial) : it(initial) {}
    virtual ~cWin32Guard() {} // Override me to clean up the handle
    void* const * operator&() { assert(it == 0); return &it; }
    operator void*() const { assert(it != 0); return it; }
  private :
    cWin32Guard(const cWin32Guard & Other) : it(Other.it) {} // Not permitted
    cWin32Guard & operator=(const cWin32Guard & Other) {} // Not permitted
};
//}}}
//{{{
class cGdiObjGuard : public cWin32Guard<HGDIOBJ>, public cRefCount
{
  public :
    QDEFINE_SMARTPTR(cGdiObjGuard);
    cGdiObjGuard(HANDLE initial) : cWin32Guard<HGDIOBJ>(initial) {}
    ~cGdiObjGuard() { if (it) (void)DeleteObject(it); }
};
//}}}
//{{{
class cWin32HandleGuard : public cWin32Guard<HANDLE>, public cRefCount
{
  public :
    QDEFINE_SMARTPTR(cWin32HandleGuard);
    cWin32HandleGuard(HANDLE initial) : cWin32Guard<HANDLE>(initial) {}
    ~cWin32HandleGuard() { if (it) (void)CloseHandle(it); }
};
//}}}
//{{{
class cDCDeleteGuard : public cWin32Guard<HDC>, public cRefCount
{
  public :
    QDEFINE_SMARTPTR(cDCDeleteGuard);
    cDCDeleteGuard(HDC initial) : cWin32Guard<HDC>(initial) {}
    ~cDCDeleteGuard() { if (it) (void)DeleteDC(it); }
};
//}}}
//{{{
class cDCReleaseGuard : public cWin32Guard<HDC>, public cRefCount
{
  public :
    QDEFINE_SMARTPTR(cDCReleaseGuard);
    HWND m_Wnd;
    cDCReleaseGuard(HWND wnd, HDC initial) : m_Wnd(wnd), cWin32Guard<HDC>(initial) {}
    ~cDCReleaseGuard() { if (it) (void)ReleaseDC(m_Wnd, it); }
};
//}}}
//{{{
class cWin32SysAllocGuard : public cWin32Guard<void *>, public cRefCount
{
  public :
    QDEFINE_SMARTPTR(cWin32SysAllocGuard);
    cWin32SysAllocGuard() : cWin32Guard<void *>() {}
    cWin32SysAllocGuard(void * initial) : cWin32Guard<void *>(initial) {}
    ~cWin32SysAllocGuard() { if (it) (void)LocalFree(it); }
};
//}}}
//{{{
class cWin32SidGuard : public cWin32Guard<PSID>, public cRefCount
{
  public :
    QDEFINE_SMARTPTR(cWin32SidGuard);
    cWin32SidGuard() : cWin32Guard<PSID>() {}
    cWin32SidGuard(PSID initial) : cWin32Guard<PSID>(initial) {}
    ~cWin32SidGuard() { if (it) (void)FreeSid(it); }
};
//}}}
//{{{
class cWin32UserImpersonationGuard : public cRefCount
{
  public :
    QDEFINE_SMARTPTR(cWin32UserImpersonationGuard);
    cWin32UserImpersonationGuard() {}
    ~cWin32UserImpersonationGuard() { (void)RevertToSelf(); }
  private :
    cWin32UserImpersonationGuard(const cWin32UserImpersonationGuard & Other) {} // Not permitted
    cWin32UserImpersonationGuard & operator=(const cWin32UserImpersonationGuard & Other) {} // Not permitted
};
//}}}
//{{{
class cWin32MappedFileGuard : public cWin32Guard<char *>, public cRefCount
{
  public :
    QDEFINE_SMARTPTR(cWin32MappedFileGuard);
    cWin32MappedFileGuard() : cWin32Guard<char *>() {}
    cWin32MappedFileGuard(char * initial) : cWin32Guard<char *>(initial) {}
    ~cWin32MappedFileGuard() { if (it) (void)UnmapViewOfFile(it); }
};
//}}}
//{{{
class cWinAccelGuard : public cWin32Guard<HACCEL>, public cRefCount
{
  public :
    QDEFINE_SMARTPTR(cWinAccelGuard);
    cWinAccelGuard(HACCEL initial) : cWin32Guard<HACCEL>(initial) {}
    ~cWinAccelGuard() { (void)DestroyAcceleratorTable(it); }
};
//}}}
//{{{
class cWinHKeyGuard : public cWin32Guard<HKEY>, public cRefCount
{
  public :
    QDEFINE_SMARTPTR(cWinHKeyGuard);
    cWinHKeyGuard(HKEY initial) : cWin32Guard<HKEY>(initial) {}
    ~cWinHKeyGuard() { (void)RegCloseKey(it); }
};
//}}}
//}}}

//{{{
// =======================================================================================
// === getWin32SpecialFolder =============================================================
// =======================================================================================
/*
Inlcude shlobj.h for definitions of 'Where', e.g. CSIDL_COMMON_PROGRAM
*/
extern String getWin32SpecialFolder(const int where) throw(cWin32Error);
//}}}
//{{{  iRegistryHelp
class iRegistryHelp
{
  public :
    QDEFINE_SMARTPTR(iRegistryHelp);

    class iItemIterator
    {
      public :
        QDEFINE_SMARTPTR(iItemIterator);
        virtual ~iItemIterator() {};
        virtual bool /* go into */ QAPI onSubKey(const String & WholeKeyPath, const std::list<String> & SubKeyStack, const iRegistryHelp::Ptr & Help) = 0;
        virtual void QAPI onItem(const String & KeyPath, const String & ItemName, const std::list<String> & SubKeyStack, const iRegistryHelp::Ptr & Help) = 0;
    };

    enum eSymbol
    {
      eModulePath,
      eNumSymbols
    };

    enum eItemType
    {
      eItemType_String,
      eItemType_Number,
      eItemType_MultiString
    };

    enum eRegistryView
    {
      eRegistryView_Default = 0,
      eRegistryView_64Bit = 0x100,
      eRegistryView_32Bit = 0x200
    };

    virtual ~iRegistryHelp() {}

    virtual bool QAPI isKeyCreated(const String & KeyPath) = 0;
    virtual bool QAPI isItemCreated(const String & KeyPath, const String & ItemName) = 0;
    virtual void QAPI enumerateTree(const String & RootKeyPath, iItemIterator::Ptr Iterator) throw(cNotFound, cOSError) = 0;
    virtual String QAPI getItemString(const String & KeyPath, const String & ItemName) throw(cNotFound, cAccessDenied, cOSError) = 0;
    virtual unsigned long QAPI getItemNumber(const String & KeyPath, const String & ItemName) throw(cNotFound, cAccessDenied, cOSError) = 0;
    virtual void QAPI setItemString(const String & KeyPath, const String & ItemName, const String & NewValue) throw(cNotFound, cAccessDenied, cOSError) = 0;
    virtual void QAPI setItemNumber(const String & KeyPath, const String & ItemName, const unsigned long NewValue) throw(cNotFound, cAccessDenied, cOSError) = 0;
    virtual void QAPI setSymbolValue(const eSymbol WhichSymbol, const String & NewValue) = 0;
    virtual void QAPI importRegEdit4File(const cMemoryView::ConstPtr & srcData) throw(cAccessDenied, cOSError) = 0;
    virtual String QAPI substituteSymbol(const String & TextToSearch, const String & Symbol, const String & Replacement) = 0;
    virtual String QAPI substituteSymbols(const String & TextToSearch) = 0;
    virtual std::map<String, unsigned long> QAPI getKeyNumbers(const String & KeyPath) throw(cNotFound) = 0;
    virtual std::map<String, String> QAPI getKeyStrings(const String & KeyPath) throw(cNotFound) = 0;
    virtual std::list<String> QAPI getSubKeys(const String & KeyPath) throw(cNotFound) = 0;
    virtual void QAPI deleteSubKey(const String & KeyPath, const String & SubKeyName) throw(cNotFound, cAccessDenied, cOSError) = 0;
    virtual void QAPI deleteItem(const String & KeyPath, const String & ItemName) throw(cNotFound, cAccessDenied, cOSError) = 0;
    virtual eItemType QAPI getItemType(const String & KeyPath, const String & ItemName) throw(cNotFound, cNotSupported) = 0;
    virtual cGuard::ConstPtr QAPI watchKey(const String & KeyPath, cAutoResetEvent * const EventToSet) throw(cNotFound) = 0;
    virtual std::vector<String> QAPI getItemMultiString(const String & KeyPath, const String & ItemName) throw(cNotFound, cAccessDenied, cOSError) = 0;
    virtual void QAPI setItemMultiString(const String & KeyPath, const String & ItemName, const std::vector<String> & NewValues) throw(cNotFound, cAccessDenied, cOSError) = 0;
    virtual void QAPI copyTree(const String & SrcRootKeyPath, const String & DstRootKeyPath) throw(cNotFound, cOSError) = 0;

    virtual cWinHKeyGuard::Ptr QAPI connectRegistryByName (const String & FQName) throw(cAccessDenied, cOSError) = 0;

    virtual unsigned long QAPI getItemNumber(const HKEY hKey, const String & KeyPath, const String & ItemName) throw(cNotFound, cAccessDenied, cOSError) = 0;
    virtual std::vector<String> QAPI getItemMultiString(const HKEY hKey, const String & KeyPath, const String & ItemName) throw(cNotFound, cAccessDenied, cOSError) = 0;

    virtual void QAPI setItemNumber(const HKEY hKey, const String & KeyPath, const String & ItemName, const unsigned long NewValue) throw(cNotFound, cAccessDenied, cOSError) = 0;
    virtual void QAPI setItemMultiString(const HKEY hKey, const String & KeyPath, const String & ItemName, const std::vector<String> & NewValues) throw(cNotFound, cAccessDenied, cOSError) = 0;

    typedef std::map<std::pair<String, String>, std::pair<eItemType, String> > tRegEdit4Values; // (key, value) -> (type, contents)
    virtual tRegEdit4Values QAPI getRegEdit4Values(const cMemoryView::ConstPtr & srcData) throw(cAccessDenied, cOSError) = 0;
};
extern iRegistryHelp::Ptr createRegistryHelp(iRegistryHelp::eRegistryView registryView = iRegistryHelp::eRegistryView_Default);
extern iRegistryHelp::Ptr createCurrentUserRegistryHelp(iRegistryHelp::eRegistryView registryView = iRegistryHelp::eRegistryView_Default);
//}}}

//{{{
extern void createWin32FileShortcut(
  const String & dstFolder,
  const String & dstName,
  const String & targetPath,
  const String & targetParameters,
  const String & targetDescription) throw(cWin32Error);
extern void deleteWin32FileShortcut(const String & path) throw(cNotFound, cAccessDenied, cWin32Error);

extern void createWin32FolderShortcut(
  const String & dstFolder,
  const String & dstName,
  const String & targetPath,
  const String & targetDescription) throw(cWin32Error);
extern void deleteWin32FolderShortcut(const String & path) throw(cNotFound, cAccessDenied, cWin32Error);
//}}}

//{{{
// =======================================================================================
// === pumpCurrThreadWinMsgsUntilQuit ====================================================
// =======================================================================================
/*
Typical Windows message pump. Returns when quit message received.
*/
class cWinKeybdAccelerator : public cRefCount
{
  public :
    QDEFINE_SMARTPTR(cWinKeybdAccelerator);
    virtual ~cWinKeybdAccelerator() {}
    virtual void setAccelerators(const std::vector<ACCEL> & newAccels);
    virtual cWinAccelGuard::Ptr getAccelHandle() const;
  private :
    mutable cLock m_ThreadSafety;
    std::vector<ACCEL> m_Accels;
    cWinAccelGuard::Ptr m_AccelHandle;
};
//}}}
extern cWinKeybdAccelerator::Ptr createKeybdAccelerator() /* throw cWin32Error */;
extern bool /* is quit? */ pumpCurrThreadWinMsgsUntilEventOrQuit(const HANDLE hEvent, const HWND hOptionalKeybdAccelWnd, const cWinKeybdAccelerator::Ptr & optionalAccel) /* throw cWin32Error */;

#define INCLUDED_QWINBASE
}

#endif
