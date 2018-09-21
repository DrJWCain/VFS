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
/*
**  
*/

#include <stdlib.h>
#include <float.h>
#include <string.h>
#include <map>
#include "QKernelExec.h"
#include "cClientServer.h"

// [RFD-148/AJS] Add support information dump
#include "KernelDumpInterface.h"

namespace vfs
{



// =======================================================================================
// === cFactory ==========================================================================
// =======================================================================================

// [RFD-148/AJS] Add support information dump interface
  class cFactory : public iFactory, public cRefCount, public iGuardCreator, 
                   public iKernelDumpManager::iKernelDumpParticipant
{
  friend class cMySubkeyEnum;

  public :

    QDEFINE_SMARTPTR(cFactory);

    // iManager
    void QAPI logSummary();
    void QAPI logDetailedSummary();

    // iFactory
#if 1
    virtual unsigned int QAPI countInstances(const cGuid & ClassGuid) const;
    virtual iFactory::iInstance::Ptr QAPI createInstance(const cGuid & ClassGuid) throw( cNotFound, cNotAvailable);
    virtual bool QAPI isInstanceDerivedFrom(iInstance::ConstPtr Instance, const cGuid & BaseClassGuid) const;
    virtual bool QAPI isClassDerivedFrom(const cGuid & ConcreteClassGuid, const cGuid & BaseClassGuid) const;
    virtual iClassInfo::ConstPtr QAPI findClassInfoByGuid(const cGuid & ClassGuid) const throw(cNotFound);
    virtual vector<iFactory::iClassInfo::ConstPtr> QAPI getBases(iFactory::iClassInfo::ConstPtr ClassInfo) const;
    virtual iFactory::iInstance::Ptr QAPI createInstanceWithParams(const cGuid & ClassGuid, const iConstructor::iParameters & Params) throw(cNotFound, cNotAvailable);
    virtual iConstructor::Ptr QAPI INTERNAL_USE_ONLY_createSingletonConstructor(iClassInfo::Ptr ClassInfo, iFactory::iSingletonCreator::Ptr Creator);
    virtual void QAPI INTERNAL_USE_ONLY_destroyBaseInstance(iFactory::iInstance * const Instance);
    virtual cGuard::ConstPtr QAPI addClassRegistrationCallback(const iClassRegistrationCallback::Ptr & newCallback);
    virtual cGuard::ConstPtr QAPI registerClass(iFactory::iConstructor::Ptr NewConstructor); // For backwards compatibility - returns nop guard
    virtual std::list<iConstructor::Ptr> QAPI getConstructors(const cGuid & ClassGuid) const;
    virtual iFactory::iConstructor::Ptr QAPI deprecated0(iFactory::iInstance::ConstPtr Instance) const throw(cNotFound) { return iFactory::iConstructor::Ptr(); }
#else
    virtual cGuard::ConstPtr QAPI registerImplementation(const iConstructor::Ptr & newCtor);
    virtual iClassInfo::ConstPtr QAPI findClassInfo(const cGuid & classGuid) const throw(cNotFound);
    virtual std::vector<cGuid> QAPI findImplementations(const cGuid & baseClassGuid) const;
    virtual unsigned int QAPI countImplementations(const cGuid & baseClassGuid) const;
    virtual iInstance::Ptr QAPI createInstance(const cGuid & classGuid) throw(cNotFound, cNotAvailable);
    virtual iInstance::Ptr QAPI createInstanceWithParams(const cGuid & classGuid, const iConstructor::iParameters & params) throw(cNotFound, cNotAvailable);
    virtual unsigned int QAPI countInstances(const cGuid & classGuid) const;
    virtual bool QAPI isInstanceDerivedFrom(const iInstance::ConstPtr & instance, const cGuid & baseClassGuid) const;
    virtual bool QAPI isClassDerivedFrom(const cGuid & classGuid, const cGuid & baseClassGuid) const;
    virtual vector<iClassInfo::ConstPtr> QAPI getBases(const iClassInfo::ConstPtr & classInfo) const;
    virtual cGuard::ConstPtr QAPI addClassRegistrationCallback(const iClassRegistrationCallback::Ptr & newCallback);
    virtual iConstructor::Ptr QAPI INTERNAL_USE_ONLY_createSingletonConstructor(const iClassInfo::Ptr & classInfo, const iFactory::iSingletonCreator::Ptr & ctor);
    virtual void QAPI INTERNAL_USE_ONLY_destroyBaseInstance(iInstance * const instance);
#endif

    // [RFD-148/AJS] registration and dump interface for support information dumps
    virtual void QAPI checkAndRegisterDump();
    virtual String QAPI getParticipantID() const { return L"Object Factory"; }
    virtual void QAPI onDump(iKernelDumpProcessor * const proc);

    // cFactory
    cFactory();
    ~cFactory();
    void registerClass2(iFactory::iConstructor::Ptr NewCtor);
    
  private :

    mutable cLock           m_ThreadSafety;

    bool                    m_IsDestructing;

    class cAboutCtor : public cRefCount
    {
      public :
        QDEFINE_SMARTPTR(cAboutCtor);
        cAboutCtor(iFactory::iConstructor::Ptr Ctor, void * const UserAddr) :
          m_Ctor(Ctor),
          m_UserAddr(UserAddr)
        {
          QVERIFY(m_Ctor.isValid());
          QVERIFY(m_UserAddr != 0);
        }
        iFactory::iConstructor::Ptr getCtor() const { return m_Ctor; }
        iFactory::iClassInfo::ConstPtr getClassInfo() const { return m_Ctor->getClassInfo(); }
        void * getUserAddr() const { return m_UserAddr; }
      private :
        iFactory::iConstructor::Ptr m_Ctor;
        void *                      m_UserAddr;
    };

    typedef std::multimap<cGuid, cAboutCtor::ConstPtr> ConstructorMap;
    ConstructorMap          m_Constructors;

    typedef std::map<iFactory::iInstance *, cAboutCtor::ConstPtr> InstancesMap;
    InstancesMap            m_Instances;

    typedef std::map<cGuid, unsigned int> CountMap;
    CountMap                m_InstanceCounts;

    unsigned int            m_NumRegisteredClasses;

    cLock m_ClassRegCallbacksThreadSafety;
    std::vector<iClassRegistrationCallback::Ptr> m_ClassRegCallbacks;

    // iGuardCreator
    void QAPI onGuardDestroyed(iGuarded::Ptr Guarded);

    // cFactory
    void _getBases(const iClassInfo::ConstPtr & ClassInfo, vector<iClassInfo::ConstPtr> * const Result) const;
    void _registerClassInheritanceTree(const iClassInfo::ConstPtr & ClassInfo, const cAboutCtor::ConstPtr & AboutCtor, const unsigned int TreeDepth);
    String _getKernelExecRegistryPath() const;
    void _logBases(const iClassInfo::ConstPtr & ClassInfo, const unsigned int Indent) const;
    void _logRegisteredClasses() const;
    unsigned int _modifyClassInheritanceTreeInstanceCounts(const iClassInfo::ConstPtr & ClassInfo, const int InstanceCountChange);
    void _logStats() const;
    iFactory::iInstance::Ptr _createInstance(const cGuid & ClassGuid, const iFactory::iConstructor::iParameters * const OptionalParams) throw(cNotFound, cNotAvailable);
    bool _isClassDerivedFrom(const iClassInfo::ConstPtr & ConcreteCandidate, const cGuid & From, const unsigned int RecurseDepth) const;
    bool _isConcreteClass(const cGuid & ClassGuid) const { return getConstructors(ClassGuid).size() > 0; }
    void _registerInstance(iInstance * const NewInstance, cAboutCtor::ConstPtr AboutCtor);
    void _unregisterInstance(iInstance * const Instance);
    bool _isDestructing() const { return m_IsDestructing; }
    void _registerClass(const iConstructor::Ptr & NewCtor, void * const AddrOfUserCode);
    bool _isKnownClass(const cGuid & ClassGuid) const;

    // [RFD-148/AJS] Factory dump registration for support info gathering
    cGuard::ConstPtr m_dumpRegistration;
};




// --- End of Namespace ------------------------------------------------------------------

}
