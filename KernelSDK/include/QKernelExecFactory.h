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

#if !defined(__0EA6CD1F02024be79503A11BDC03D8FC)
#define __0EA6CD1F02024be79503A11BDC03D8FC
/*
**  
*/

#include "QKernelBase.h"
#include "QKernelExecEnviron.h"
#include "QKernelExecObjectClasses.h"
#include "QKernelSDKVersion.h"

namespace vfs
{
class iModuleInstance;

//{{{
// =======================================================================================
// === iFactory ==========================================================================
// =======================================================================================
/*
<b>Thread Safety</b>

All the methods within this class are thread safe.
*/

class iFactory
{
  public :
    QDEFINE_SMARTPTR(iFactory);
    virtual ~iFactory() {}

    //{{{
    class iClassInfo
    {
      public :
        QDEFINE_SMARTPTR(iClassInfo);
        virtual ~iClassInfo() {}
        virtual cGuid QAPI getGuid() const = 0;
        virtual String QAPI getName() const = 0;
        virtual std::vector<ConstPtr> QAPI getBaseInfo() const = 0;
        virtual iClassInfo::ConstPtr QAPI clone() const = 0;
    };
    //}}}
    //{{{
    class iInstance : public iGuarded
    {
      public :
        QDEFINE_SMARTPTR(iInstance);
        virtual ~iInstance();
        typedef class cClassInfo : public iClassInfo, public cRefCount
        {
          public :
            cGuid QAPI getGuid() const { static cGuid tmp(L"{8218E69-D1F2-4B4B-87C4-20F2D01F9C08} // iInstance"); return tmp; }
            String QAPI getName() const { return getGuid().getComment(); }
            std::vector<iClassInfo::ConstPtr> QAPI getBaseInfo() const { static std::vector<iClassInfo::ConstPtr> tmp; return tmp; }
            iClassInfo::ConstPtr QAPI clone() const { return new cClassInfo; }
        } class_info;
        virtual iClassInfo::ConstPtr QAPI getClassInfo() const { return new class_info; }
    };
    //}}}
    //{{{
    class iConstructor : public iGuarded
    {
      public :
        QDEFINE_SMARTPTR(iConstructor);

        /*
        Create an instance of the new class.
        */
        virtual iInstance::Ptr QAPI createInstance() throw(cRecoverable) = 0;
        class iParameters
        {
        };
        virtual iInstance::Ptr QAPI createInstanceWithParams(const iParameters & Params) throw(cRecoverable) = 0;

        virtual iClassInfo::ConstPtr QAPI getClassInfo() const = 0;
    };
    //}}}

    /*
    Helper class if you want to roll your own constructor.
    */
    //{{{
    class cCustomConstructor : public iConstructor, public cRefCount
    {
      private :
        iClassInfo::ConstPtr m_ClassInfo;
        class cClassInfo : public iFactory::iClassInfo, public cRefCount
        {
          public :
            cClassInfo(const cGuid & Guid, iClassInfo::ConstPtr BaseClass) : m_Guid(Guid) { m_Bases.resize(1); m_Bases[0] = BaseClass; }
            cClassInfo(const cGuid & Guid, iClassInfo::ConstPtr BaseClass1, iClassInfo::ConstPtr BaseClass2) : m_Guid(Guid) { m_Bases.resize(2); m_Bases[0] = BaseClass1; m_Bases[1] = BaseClass2; }
            cClassInfo(const cGuid & Guid, iClassInfo::ConstPtr BaseClass1, iClassInfo::ConstPtr BaseClass2, iClassInfo::ConstPtr BaseClass3) : m_Guid(Guid) { m_Bases.resize(3); m_Bases[0] = BaseClass1; m_Bases[1] = BaseClass2; m_Bases[2] = BaseClass2; }
            cClassInfo(const cGuid & Guid, iClassInfo::ConstPtr BaseClass1, iClassInfo::ConstPtr BaseClass2, iClassInfo::ConstPtr BaseClass3, iClassInfo::ConstPtr BaseClass4) : m_Guid(Guid) { m_Bases.resize(4); m_Bases[0] = BaseClass1; m_Bases[1] = BaseClass2; m_Bases[2] = BaseClass2; m_Bases[3] = BaseClass2; }
            cGuid QAPI getGuid() const                                  { return m_Guid; }
            String QAPI getName() const                                 { return m_Guid.getComment(); }
            std::vector<iClassInfo::ConstPtr> QAPI getBaseInfo() const  { return m_Bases; }
            iClassInfo::ConstPtr QAPI clone() const                     { return new cClassInfo(*this); }
          private :
            const cGuid                       m_Guid;
            std::vector<iClassInfo::ConstPtr> m_Bases;
            cClassInfo(const cClassInfo & Other) : m_Guid(Other.m_Guid), m_Bases(Other.m_Bases) {}
        };
      public :
        QDEFINE_SMARTPTR(cCustomConstructor);
        iClassInfo::ConstPtr QAPI getClassInfo() const { return m_ClassInfo; }
        iFactory::iInstance::Ptr QAPI createInstance() { throw cNotSupported(__FILE__, __LINE__, L"No default \"%s\" ctor", getClassInfo()->getName().c_str()); }
        iFactory::iInstance::Ptr QAPI createInstanceWithParams(const iFactory::iConstructor::iParameters & Params) { throw cNotSupported(__FILE__, __LINE__, L"No parameterised \"%s\" ctor", getClassInfo()->getName().c_str()); }
      protected :
        cCustomConstructor(const cGuid & ClassGuid, iClassInfo::ConstPtr BaseClass) : m_ClassInfo(new cClassInfo(ClassGuid, BaseClass)) {}
        cCustomConstructor(const cGuid & ClassGuid, iClassInfo::ConstPtr BaseClass1, iClassInfo::ConstPtr BaseClass2) : m_ClassInfo(new cClassInfo(ClassGuid, BaseClass1, BaseClass2)) {}
        cCustomConstructor(const cGuid & ClassGuid, iClassInfo::ConstPtr BaseClass1, iClassInfo::ConstPtr BaseClass2, iClassInfo::ConstPtr BaseClass3) : m_ClassInfo(new cClassInfo(ClassGuid, BaseClass1, BaseClass2, BaseClass3)) {}
        cCustomConstructor(const cGuid & ClassGuid, iClassInfo::ConstPtr BaseClass1, iClassInfo::ConstPtr BaseClass2, iClassInfo::ConstPtr BaseClass3, iClassInfo::ConstPtr BaseClass4) : m_ClassInfo(new cClassInfo(ClassGuid, BaseClass1, BaseClass2, BaseClass3, BaseClass4)) {}
    };
    //}}}
    //{{{
    class iSingletonCreator
    {
      public :
        QDEFINE_SMARTPTR(iSingletonCreator);
        virtual ~iSingletonCreator() {}
        virtual iInstance::Ptr QAPI createSingleton() throw(cRecoverable) = 0;
    };
    //}}}

    /*
    Returns a pointer to a guard object - hang onto this for as long as you want the class to
    remain registered.
    */
    virtual cGuard::ConstPtr QAPI registerClass(iConstructor::Ptr NewConstructor) = 0;

    /*
    Returns a list of all class constructors derived from the specified registered class.
    */
    virtual std::list<iConstructor::Ptr> QAPI getConstructors(const cGuid & ConcreteClassGuid) const = 0;

    /*
    Creates an instance using the first constructor derived from the specified registered class.
    */
    virtual iInstance::Ptr QAPI createInstance(const cGuid & ConcreteClassGuid) throw(cNotFound, cNotAvailable) = 0;

    /*
    Counts number of instances of a class created using createInstance(). Returns zero if class
    is not registered.
    */
    virtual unsigned int QAPI countInstances(const cGuid & ConcreteClassGuid) const = 0;

    /*
    */
    virtual bool QAPI isInstanceDerivedFrom(iInstance::ConstPtr Instance, const cGuid & BaseClassGuid) const = 0;

    /*
    */
    virtual iConstructor::Ptr QAPI deprecated0(iInstance::ConstPtr Instance) const throw(cNotFound) = 0;

    /*
    */
    virtual bool QAPI isClassDerivedFrom(const cGuid & ConcreteClassGuid, const cGuid & BaseClassGuid) const = 0;

    /*
    */
    virtual iClassInfo::ConstPtr QAPI findClassInfoByGuid(const cGuid & ConcreteClassGuid) const throw(cNotFound) = 0;

    /*
    */
    virtual void QAPI logSummary() = 0;

    /*
    */
    virtual void QAPI logDetailedSummary() = 0;

    /*
    */
    virtual std::vector<iClassInfo::ConstPtr> QAPI getBases(iFactory::iClassInfo::ConstPtr ClassInfo) const = 0;

    /*
    Creates an instance using the first constructor derived from the specified registered and pass in a parameter object
    */
    virtual iInstance::Ptr QAPI createInstanceWithParams(const cGuid & ConcreteClassGuid, const iConstructor::iParameters & Params) throw(cNotFound, cNotAvailable) = 0;

    // Internal use only!
    virtual iConstructor::Ptr QAPI INTERNAL_USE_ONLY_createSingletonConstructor(iClassInfo::Ptr ClassInfo, iSingletonCreator::Ptr Creator) = 0;
    virtual void QAPI INTERNAL_USE_ONLY_destroyBaseInstance(iInstance * const Instance) = 0;

    //{{{
    class iClassRegistrationCallback : public iGuarded
    {
      public :
        QDEFINE_SMARTPTR(iClassRegistrationCallback);
        virtual ~iClassRegistrationCallback() {}
        virtual void QAPI onFactoryClassRegistered(const cGuid & newClassGuid) = 0;
    };
    //}}}
    virtual cGuard::ConstPtr QAPI addClassRegistrationCallback(const iClassRegistrationCallback::Ptr & newCallback) = 0;

    // [RFD-148/AJS] registration interface for support information dumps
    virtual void QAPI checkAndRegisterDump() = 0;
};
//}}}
//{{{
// =======================================================================================
// === iModuleRegistrar ==================================================================
// =======================================================================================
class iModuleRegistrar
{
  public :
    QDEFINE_SMARTPTR(iModuleRegistrar);
    virtual ~iModuleRegistrar() {}
    virtual void QAPI addClass(iFactory::iConstructor::Ptr NewConstructor) = 0;
    #if defined(_ATL_VER) && (_ATL_VER >= 0x0710)
      virtual void QAPI addATLObjects(_ATL_OBJMAP_ENTRY * const ObjectMap) = 0;
      virtual void QAPI addATLTypeLibrary(const unsigned int IndexInResources) = 0;
      virtual ATL::CAtlModule * QAPI getATLModule() const = 0;
      virtual std::vector<_ATL_OBJMAP_ENTRY> QAPI getATLObjects() const = 0;
      virtual std::vector<unsigned int> QAPI getATLTypeLibraryResourceIndices() const = 0;
    #else
      virtual void QAPI addATLObjects() = 0;
      virtual void QAPI addATLTypeLibrary() = 0;
      virtual void QAPI getATLModule() const = 0;
      virtual void QAPI getATLObjects() const = 0;
      virtual void QAPI getATLTypeLibraryResourceIndices() const = 0;
    #endif

    // Named settings migrate to me if an earlier version of my settings don't already exist
    virtual void QAPI aliasModule(const String & Developer, const String & ModuleName) = 0;

    // This module replaces named one, delete named binaries if they exist
    virtual void QAPI deprecateModule(const String & Developer, const String & ModuleName) = 0;
};
//}}}
//{{{
// SJSJSJ To be replaced with moduleRegister
//#pragma message("******** remove me *********")
extern "C" QPLUGIN void QARGS_STACK moduleInit(cPtr<iModuleInstance> Instance) throw(cRecoverable);
typedef void (QARGS_STACK * moduleInitFuncPtr)(cPtr<iModuleInstance>);

extern "C" QPLUGIN void QARGS_STACK moduleRegister(iModuleRegistrar::Ptr Context) throw(cRecoverable);
typedef void (QARGS_STACK * moduleRegisterFuncPtr)(iModuleRegistrar::Ptr);
//}}}
//{{{  iModuleInstallHelp
// =======================================================================================
// === iModuleInstallHelp ================================================================
// =======================================================================================
/*
<b>Thread Safety</b>
None of the methods within this class are thread safe. Do not attempt to call them
from another thread that you have created.
*/
const int fExeProcess_ShowWindow     = 1 << 1;

class iRegistryHelp;
class iModuleInstallHelp
{
  public :

    QDEFINE_SMARTPTR(iModuleInstallHelp);

    virtual ~iModuleInstallHelp() {}

    virtual void QAPI regsvr32(const String & DllFilePath) throw(cNotFound, cNotSupported) = 0;
    virtual void QAPI unregsvr32(const String & DllFilePath) throw(cNotFound, cNotSupported) = 0;
    virtual String QAPI getWin32RootFolder() const = 0;
    virtual void QAPI executeProcess(const String &  ExePath, const String &  ExeFileName, const String &  ExeParams, const int Flags) throw(cFileError) = 0;
    virtual void QAPI copyFileOrFolder(const String & SrcFilePath, const String & DstFilePath) throw(cBadFormat, cFileError) = 0;
    virtual void QAPI killProcess(const String & ProcessName, const unsigned TimeoutMillisecs) throw(cNotFound, cTimedOut) = 0;
    virtual bool QAPI isProcessRunning(const String & ProcessName) throw(cNotFound) = 0;
    virtual void QAPI installSharedDll(const String & SrcDllPath) throw(cBadFormat, cFileError) = 0;

// DEPRECATED!
#if kKernelSDKIfaceVersion_Int >= 0400000
    QDEPRECATED
#endif
    virtual cPtr<iRegistryHelp> QAPI createRegistryHelp() = 0;
    virtual cPtr<iRegistryHelp> QAPI createCurrentUserRegistryHelp() = 0;
    // DEPRECATED!

    virtual void QAPI uninstallSharedDll(const String & DllFilename) throw(cBadFormat, cFileError) = 0;
    virtual void QAPI executeProcessWait(const String &  ExePath, const String &  ExeFileName, const String &  ExeParams, const unsigned int TimeoutMillisecs, const int Flags) throw(cFileError, cTimedOut) = 0;
};
//}}}

//{{{
class cIgnoreInstallFailed : public cRecoverable
{
  public :
    virtual ~cIgnoreInstallFailed() {}
    cIgnoreInstallFailed(const char * const  SrcFilename, const unsigned long SrcLineNumber, const Char * MsgFormat, ...) :
      cRecoverable(SrcFilename, SrcLineNumber, MsgFormat, (va_list)&MsgFormat + _INTSIZEOF(MsgFormat))
    {
    }
};
//}}}
//{{{
class cInstallRequiresReboot : public cRecoverable
{
  public :
    virtual ~cInstallRequiresReboot() {}
    cInstallRequiresReboot(const char * const  SrcFilename, const unsigned long SrcLineNumber, const Char * MsgFormat, ...) :
      cRecoverable(SrcFilename, SrcLineNumber, MsgFormat, (va_list)&MsgFormat + _INTSIZEOF(MsgFormat))
    {
    }
};
//}}}

const int fModuleInstall_FirstTimeInstall       = 1 << 0; // First time install because module does not exist
const int fModuleInstall_Upgrading              = 1 << 1; // Upgrading from older version
const int fModuleInstall_Downgrading            = 1 << 2; // Downgrading from newer version
const int fModuleInstall_Migrating              = 1 << 3; // Upgrading from older version when new version settings do not already exist - migration required
const int fModuleInstall_Relocating             = 1 << 4; // Is running from a different location than last time?
const int fModuleInstall_IsHostedByKernelSetup  = 1 << 5; // Am I installing properly using the Installer?

const int fModuleInstall_User_FirstTimeInstall = 1 << 7; // First time install because module does not exist
const int fModuleInstall_User_Upgrading = 1 << 8; // Upgrading from older version
const int fModuleInstall_User_Downgrading = 1 << 9; // Downgrading from newer version
const int fModuleInstall_User_Migrating = 1 << 10; // Upgrading from older version when new version settings do not already exist - migration required

// On first_install/upgrade/downgrade/relocate
// OptionalOutgoing is only valid when upgrading or downgrading
extern "C" QPLUGIN void QARGS_STACK moduleInstall(const int Flags, cPtr<iModuleInstance> Incoming, cPtr<iModuleInstance> OptionalOutgoing, iModuleInstallHelp::Ptr Help) throw(cRecoverable, cIgnoreInstallFailed, cInstallRequiresReboot);
typedef void (QARGS_STACK * moduleInstallFuncPtr)(const int Flags, cPtr<iModuleInstance> Incoming, cPtr<iModuleInstance> OptionalOutgoing, iModuleInstallHelp::Ptr Help);

// On complete uninstall
extern "C" QPLUGIN void QARGS_STACK moduleUninstall(cPtr<iModuleInstance> Outgoing, iModuleInstallHelp::Ptr Help, const bool IsRemovingSettingsAndData) throw(cRecoverable);
typedef void (QARGS_STACK * moduleUninstallFuncPtr)(cPtr<iModuleInstance> Outgoing, iModuleInstallHelp::Ptr Help, const bool IsRemovingSettingsAndData);

// =======================================================================================
// === Statically Linked =================================================================
// =======================================================================================
class cKernelImpl;
//{{{
class cKernel : public cRefCount
{
  public :
    QDEFINE_SMARTPTR(cKernel);
    QKERNELEXEC cKernel(const String & cmdLine) /* throw cRecoverable */;
    virtual ~cKernel();
    virtual void installApp() /* throw cRecoverable */;
    virtual void uninstallApp() /* throw cRecoverable */;
    virtual void startApp() /* throw cRecoverable */;
    virtual void waitAppStartedForever();
    virtual void waitAppStarted(const unsigned int timeoutMs);
    virtual void exitApp() /* throw cRecoverable */;
    virtual void waitAppExitForever();
    virtual void waitAppExit(const unsigned int timeoutMs);
    QKERNELEXEC static iFactory::Ptr m_Factory;
  private :
    cPtr<cKernelImpl> m_Pimpl;
};
//}}}
extern QKERNELEXEC cKernel * gKernel;

/* deprecated! use cKernel above instead */ 
extern QKERNELEXEC iFactory::Ptr gFactory;

/* deprecated! use cKernel above instead */ 
//{{{
class iKernelMainThread
{
  public :
    QDEFINE_SMARTPTR(iKernelMainThread);
    virtual ~iKernelMainThread() {}
    virtual cGuard::ConstPtr start() = 0;
    virtual void waitDevicesReady() = 0;
    virtual void waitBootedForever() = 0;
    virtual void waitBooted(const unsigned int timeoutMs) = 0;
};
//}}}

/* deprecated! use cKernel above instead */ 
extern QKERNELEXEC iKernelMainThread::Ptr gKernelMainThread;

/* deprecated! use cKernel above instead */ 
QKERNELEXEC void QAPI kernelCreate(const String & CmdLine);

/* deprecated! use cKernel above instead */ 
QKERNELEXEC bool QAPI kernelIsCreated();

/* deprecated! use cKernel above instead */ 
QKERNELEXEC void QAPI kernelDestroy();

//{{{
inline iFactory::iInstance::~iInstance()
{
  gKernel->m_Factory->INTERNAL_USE_ONLY_destroyBaseInstance(this);
}
//}}}
}
#endif
