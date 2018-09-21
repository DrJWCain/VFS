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
#include "cFactory.h"
#include "cLogManager.h"
#include "cSingletonConstructorImpl.h"

using namespace vfs;
using namespace std;


const unsigned int kMaxInheritanceDepth = 15; // Any more and it's probably a circular inheritance error!

//{{{
// =======================================================================================
// === INTERNAL_USE_ONLY_destroyBaseInstance =============================================
// =======================================================================================

void QAPI cFactory::INTERNAL_USE_ONLY_destroyBaseInstance(iFactory::iInstance * const Instance)
{
  if (!_isDestructing()) _unregisterInstance(Instance);
}
//}}}
//{{{
// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

cFactory::cFactory() :
  m_NumRegisteredClasses(0),
  m_IsDestructing(false)
{
}

// ---------------------------------------------------------------------------------------

cFactory::~cFactory()
{
  m_IsDestructing = true;
}
//}}}
//{{{
// =======================================================================================
// === logSummary/logDetailedSummary =====================================================
// =======================================================================================

void QAPI cFactory::logSummary()
{
  QVERIFY(!m_IsDestructing);
  _logStats();
}

// ---------------------------------------------------------------------------------------

void QAPI cFactory::logDetailedSummary()
{
  QVERIFY(!m_IsDestructing);
  _logRegisteredClasses();
}
//}}}
//{{{
// =======================================================================================
// === registerClass =====================================================================
// =======================================================================================

#ifdef _M_IX86
#pragma optimize("y",off) // Frame pointer optimisation off so UserAddr works
#endif

// This function exists just to save modifying every registerClass to registerClass2
cGuard::ConstPtr QAPI cFactory::registerClass(iFactory::iConstructor::Ptr NewCtor)
{
  // Remember return address (in user code) so we can identify who registered classes later if necessary.
  void * AddrOfUserCode;
#ifdef _M_X64
  AddrOfUserCode = _ReturnAddress();
#else
  __asm
  {
    __asm push  eax
    __asm mov   eax, [ebp + 4] // 4 is enough to get to return address from registerClass() call, i.e. in user code
    __asm mov   AddrOfUserCode, eax
    __asm pop   eax
  }
#endif
  _registerClass(NewCtor, AddrOfUserCode);
  return new cGuard(this, NewCtor);
}

void cFactory::registerClass2(iFactory::iConstructor::Ptr NewCtor)
{
  // Remember return address (in user code) so we can identify who registered classes later if necessary.
  void * AddrOfUserCode;
#ifdef _M_X64
  AddrOfUserCode = _ReturnAddress();
#else
  __asm
  {
    __asm push  eax
    __asm mov   eax, [ebp + 4] // 4 is enough to get to return address from registerClass() call, i.e. in user code
    __asm mov   AddrOfUserCode, eax
    __asm pop   eax
  }
#endif
  _registerClass(NewCtor, AddrOfUserCode);
}

void cFactory::_registerClass(const iConstructor::Ptr & NewCtor, void * const AddrOfUserCode)
{
  QVERIFY(!m_IsDestructing);

  try
  {
    // Protect member variables from changes by another thread.
    cLockGuard threadSafety(&m_ThreadSafety);

    const ConstructorMap::const_iterator i = m_Constructors.find(NewCtor->getClassInfo()->getGuid());
    if (i != m_Constructors.end())
    {
      String ExistingRegistrantDllName, AttemptingRegistrantDllName;
      { GET_MODULE_NAME(AddrOfUserCode); AttemptingRegistrantDllName = ModuleName; }
      { GET_MODULE_NAME(i->second->getUserAddr()); ExistingRegistrantDllName = ModuleName; }
      QFATAL((
        __FILE__, __LINE__,
        L"%s (0x%p) tried to register \"%s\" but is already registered by %s (0x%p)",
        AttemptingRegistrantDllName.c_str(), AddrOfUserCode,
        NewCtor->getClassInfo()->getGuid().serialise().c_str(),
        ExistingRegistrantDllName.c_str(), i->second->getUserAddr()));
    }

    _registerClassInheritanceTree(NewCtor->getClassInfo(), new cAboutCtor(NewCtor, AddrOfUserCode), 0 /* initial recurse depth */);
    m_NumRegisteredClasses++;
  }
  catch (cRecoverable &)
  {
    /*
    Need this to make sure stack is unwound in the event of the "unhandled exception handler" being called
    which _doesn't_ unwind! Fixes m_ThreadSafety not being unlocked when cNotFound is thrown.
    */
    throw;
  }

  vector<iClassRegistrationCallback::Ptr> copyOfClassRegCallbacks;
  {
    cLockGuard threadSafety(&m_ClassRegCallbacksThreadSafety);
    copyOfClassRegCallbacks = m_ClassRegCallbacks;
  }
  for (vector<iClassRegistrationCallback::Ptr>::const_iterator i = copyOfClassRegCallbacks.begin(); i != copyOfClassRegCallbacks.end(); ++i)
  {
    (*i)->onFactoryClassRegistered(NewCtor->getClassInfo()->getGuid());
  }
}

#pragma optimize("",on) // Frame pointer optimisation to default
//}}}
//{{{
// =======================================================================================
// === _registerClassInheritanceTree =====================================================
// =======================================================================================

void cFactory::_registerClassInheritanceTree(
  const iClassInfo::ConstPtr & ClassInfo,
  const cAboutCtor::ConstPtr & AboutCtor,
  const unsigned int           RecurseDepth)
{
  if (RecurseDepth > kMaxInheritanceDepth)
  {
    cLogIndentSOSGuard SOSIndent(L"Circular inheritance?");
    _logBases(ClassInfo, 0 /* initial indent level */);
    QFATAL((__FILE__, __LINE__, L"Possible circular inheritance - recursive depth > %d classes!", kMaxInheritanceDepth));
  }

  const cGuid & ClassGuid = ClassInfo->getGuid();
  m_InstanceCounts.insert(make_pair(ClassGuid, 0));
  if (ClassGuid != iFactory::iInstance::class_info().getGuid()) // Avoid adding iInstance otherwise we end up with thousands of entries in m_Constructors!
  {
    m_Constructors.insert(make_pair(ClassGuid, AboutCtor));

    const vector<iFactory::iClassInfo::ConstPtr> BaseInfo(ClassInfo->getBaseInfo());
    for (vector<iFactory::iClassInfo::ConstPtr>::const_iterator i = BaseInfo.begin(); i != BaseInfo.end(); ++i)
    {
      _registerClassInheritanceTree(*i, AboutCtor, RecurseDepth + 1);
    }
  }
}
//}}}
//{{{
// =======================================================================================
// === getConstructors ===================================================================
// =======================================================================================

list<iFactory::iConstructor::Ptr> QAPI cFactory::getConstructors(const cGuid & ClassGuid) const
{
  QVERIFY(!m_IsDestructing);

  list<iFactory::iConstructor::Ptr> ResultList;
  try
  {
    // Protect member variables from changes by another.
    cLockGuard threadSafety(&m_ThreadSafety);
    for (ConstructorMap::const_iterator i = m_Constructors.find(ClassGuid); i != m_Constructors.end() && i->first == ClassGuid; ++i)
    {
      // Add to list if it isn't there already.
      if (find(ResultList.begin(), ResultList.end(), i->second->getCtor()) == ResultList.end())
      {
        ResultList.push_back(i->second->getCtor());
      }
    }
    return ResultList;
  }
  catch (cRecoverable &)
  {
    /*
    Need this to make sure stack is unwound in the event of the "unhandled exception handler" being called
    which _doesn't_ unwind! Fixes m_ThreadSafety not being unlocked when cNotFound is thrown.
    */
    throw;
  }
}
//}}}
//{{{
// =======================================================================================
// === findClassInfoByGuid ===============================================================
// =======================================================================================

iFactory::iClassInfo::ConstPtr QAPI cFactory::findClassInfoByGuid(const cGuid & ConcreteClassGuid) const throw(cNotFound)
{
  QVERIFY(!m_IsDestructing);
  if (!_isKnownClass(ConcreteClassGuid)) throw cNotFound(__FILE__, __LINE__, L"\"%s\" is not a known concrete class", ConcreteClassGuid.serialise().c_str());
  QVERIFY(_isConcreteClass(ConcreteClassGuid));

  try
  {
    ConstructorMap::const_iterator i;
    {
      // Protect member variables from changes by another thread.
      cLockGuard threadSafety(&m_ThreadSafety);

      i = m_Constructors.find(ConcreteClassGuid);
      if (i == m_Constructors.end())
      {
        throw cNotFound(__FILE__, __LINE__, L"ClassInfo for %s not found", ConcreteClassGuid.toString().c_str());
      }
    }
    return i->second->getClassInfo();
  }
  catch (cRecoverable &)
  {
    /*
    Need this to make sure stack is unwound in the event of the "unhandled exception handler" being called
    which _doesn't_ unwind! Fixes m_ThreadSafety not being unlocked when cNotFound is thrown.
    */
    throw;
  }
}
//}}}
//{{{
// =======================================================================================
// === createInstance ====================================================================
// =======================================================================================

iFactory::iInstance::Ptr cFactory::_createInstance(const cGuid & ClassGuid, const iFactory::iConstructor::iParameters * const OptionalParams) throw(cNotFound, cNotAvailable)
{
  cAboutCtor::ConstPtr AboutCtor;
  {
    // Protect member variables from changes by another thread.
    cLockGuard threadSafety(&m_ThreadSafety);

    const ConstructorMap::iterator i = m_Constructors.find(ClassGuid);
    if (i == m_Constructors.end())
    {
      throw cNotFound(__FILE__, __LINE__, L"Constructor for %s not found", ClassGuid.toString().c_str());
    }

    AboutCtor = i->second;
  }

  iInstance::Ptr NewInstance;
  try
  {
    if (OptionalParams)
    {
      NewInstance = ptr_cast<iInstance::Ptr>(AboutCtor->getCtor()->createInstanceWithParams(*OptionalParams));
    }
    else
    {
      NewInstance = ptr_cast<iInstance::Ptr>(AboutCtor->getCtor()->createInstance());
    }
  }
  catch (cRecoverable & Reason)
  {
    QMSG((0, L"Re-throwing exception (file \"%s\", line %d) \"%s\" caught during construction of \"%s\"", Reason.getSrcFilename().c_str(), Reason.getSrcLineNumber(), Reason.getMsg().c_str(), ClassGuid.serialise().c_str()));
    throw;
  }
  catch (...)
  {
    QSOS((L"Unexpected exception during construction of \"%s\"", ClassGuid.serialise().c_str()));
    throw;
  }

  _registerInstance(NewInstance.getRaw(), AboutCtor);

  return NewInstance;
}

iFactory::iInstance::Ptr QAPI cFactory::createInstance(const cGuid & ClassGuid) throw(cNotFound, cNotAvailable)
{
  QVERIFY(!m_IsDestructing);
  return ptr_cast<iFactory::iInstance::Ptr>(_createInstance(ClassGuid, (iFactory::iConstructor::iParameters*)0));
}

iFactory::iInstance::Ptr QAPI cFactory::createInstanceWithParams(const cGuid & ClassGuid, const iFactory::iConstructor::iParameters & Params) throw(cNotFound, cNotAvailable)
{
  QVERIFY(!m_IsDestructing);
  return ptr_cast<iFactory::iInstance::Ptr>(_createInstance(ClassGuid, &Params));
}
//}}}
//{{{
// =======================================================================================
// === INTERNAL_USE_ONLY_createSingletonConstructor ======================================
// =======================================================================================

iFactory::iConstructor::Ptr QAPI cFactory::INTERNAL_USE_ONLY_createSingletonConstructor(iFactory::iClassInfo::Ptr ClassInfo, iFactory::iSingletonCreator::Ptr Creator)
{
  return new cSingletonConstructorImpl(ClassInfo, Creator);
}
//}}}
//{{{
// =======================================================================================
// === _logStats =========================================================================
// =======================================================================================

void cFactory::_logStats() const
{
  try
  {
    // Protect member variables from changes by another thread.
    cLockGuard threadSafety(&m_ThreadSafety);

    QMSG((0, L"%d classes registered", m_NumRegisteredClasses));
    QMSG((0, L"%Iu instances created", m_Instances.size()));
  }
  catch (cRecoverable &)
  {
    /*
    Need this to make sure stack is unwound in the event of the "unhandled exception handler" being called
    which _doesn't_ unwind! Fixes m_ThreadSafety not being unlocked when cNotFound is thrown.
    */
    throw;
  }
}
//}}}
//{{{
// =======================================================================================
// === _logRegisteredClasses =============================================================
// =======================================================================================

void cFactory::_logRegisteredClasses() const
{
  try
  {
    // Protect member variables from changes by another thread.
    cLockGuard threadSafety(&m_ThreadSafety);

    QMSG((0, L"%-5s   %-47s %-75s", L"Count", L"Class Name", L"Class Guid"));

    for (ConstructorMap::const_iterator  i = m_Constructors.begin(); i != m_Constructors.end(); ++i)
    {
      if (i->first == i->second->getClassInfo()->getGuid())
      {
        QMSG((
          0, L"%5d   %-47s %-75s",
          countInstances(i->second->getClassInfo()->getGuid()),
          i->second->getClassInfo()->getName().c_str(),
          i->first.serialise(false).c_str()));

        _logBases(i->second->getClassInfo(), 1);

        QMSG((0, L""));
      }
    }
  }
  catch (cRecoverable &)
  {
    /*
    Need this to make sure stack is unwound in the event of the "unhandled exception handler" being called
    which _doesn't_ unwind! Fixes m_ThreadSafety not being unlocked when cNotFound is thrown.
    */
    throw;
  }
}

// ---------------------------------------------------------------------------------------

void cFactory::_logBases(const iClassInfo::ConstPtr & ClassInfo, const unsigned int Indent) const
{
  if (Indent > kMaxInheritanceDepth)
  {
    QMSG((0, L"Possible inheritance error - recursive depth > %d classes! Stopping here.", kMaxInheritanceDepth));
  }
  else
  {
    const vector<iFactory::iClassInfo::ConstPtr> Bases(ClassInfo->getBaseInfo());
    for (vector<iFactory::iClassInfo::ConstPtr>::const_reverse_iterator i = Bases.rbegin(); i != Bases.rend(); ++i)
    {
      QMSG((0, L"        %s%s", String(Indent * 2, L' ').c_str(), (*i)->getName().c_str()));
      _logBases(*i, Indent + 1);
    }
  }
}
//}}}
//{{{
// =======================================================================================
// === _getBases =========================================================================
// =======================================================================================

vector<iFactory::iClassInfo::ConstPtr> cFactory::getBases(iFactory::iClassInfo::ConstPtr ClassInfo) const
{
  QVERIFY(!m_IsDestructing);
  vector<iClassInfo::ConstPtr> Result;
  _getBases(ClassInfo, &Result);
  return Result;
}

void cFactory::_getBases(const iClassInfo::ConstPtr & ClassInfo, vector<iClassInfo::ConstPtr> * const Result) const
{
  QVERIFY(Result);

  if (Result->size() > kMaxInheritanceDepth)
  {
    cLogIndentSOSGuard SOSIndent(L"Circular inheritance?");
    _logBases(ClassInfo, 0 /* initial indent level */);
    QFATAL((__FILE__, __LINE__, L"Possible inheritance error - recursive depth > %d classes!", kMaxInheritanceDepth));
  }

  const vector<iFactory::iClassInfo::ConstPtr> BaseInfo(ClassInfo->getBaseInfo());
  for (vector<iFactory::iClassInfo::ConstPtr>::const_reverse_iterator i = BaseInfo.rbegin(); i != BaseInfo.rend(); ++i)
  {
    Result->push_back(*i);
    _getBases(*i, Result);
  }
}
//}}}
//{{{
// =======================================================================================
// === countInstances ====================================================================
// =======================================================================================

unsigned int QAPI cFactory::countInstances(const cGuid & ClassGuid) const
{
  //QVERIFY(!m_IsDestructing);  // - replaced with below to help clean exit !!! SRSR !!!
  if (m_IsDestructing) return 0;
  
  try
  {
    // Protect member variables from changes by another thread.
    cLockGuard threadSafety(&m_ThreadSafety);

    CountMap::const_iterator j = m_InstanceCounts.find(ClassGuid);
    if (j == m_InstanceCounts.end())
    {
      return 0;
    }
    else
    {
      return j->second;
    }
  }
  catch (cRecoverable &)
  {
    /*
    Need this to make sure stack is unwound in the event of the "unhandled exception handler" being called
    which _doesn't_ unwind! Fixes m_ThreadSafety not being unlocked when cNotFound is thrown.
    */
    throw;
  }
}
//}}}
//{{{
// =======================================================================================
// === _registerInstance/unregisterInstance ===============================================
// =======================================================================================

unsigned int cFactory::_modifyClassInheritanceTreeInstanceCounts(const iClassInfo::ConstPtr & ClassInfo, const int InstanceCountChange)
{
  CountMap::iterator c = m_InstanceCounts.find(ClassInfo->getGuid());
  #if defined(_DEBUG)
    QVERIFY(c != m_InstanceCounts.end());
//    if (InstanceCountChange < 0) QVERIFY((signed)c->second >= -InstanceCountChange);
  #endif

  c->second = (unsigned)((signed)c->second + InstanceCountChange);

  const vector<iFactory::iClassInfo::ConstPtr> Bases(ClassInfo->getBaseInfo());
  for (vector<iFactory::iClassInfo::ConstPtr>::const_iterator i = Bases.begin(); i != Bases.end(); ++i)
  {
    _modifyClassInheritanceTreeInstanceCounts(*i, InstanceCountChange);
  }
  return (unsigned)c->second;
}

void cFactory::_registerInstance(iFactory::iInstance * const NewInstance, cAboutCtor::ConstPtr AboutCtor)
{
  QVERIFY(NewInstance);
  QVERIFY(AboutCtor.isValid());

  try
  {
    // Protect member variables from changes by another thread.
    cLockGuard threadSafety(&m_ThreadSafety);

    InstancesMap::iterator it = m_Instances.find(NewInstance);
    if (it == m_Instances.end())
    {
      m_Instances.insert(make_pair(NewInstance, AboutCtor));
      _modifyClassInheritanceTreeInstanceCounts(AboutCtor->getClassInfo(), 1 /* increment */);
    }
    /*else
    {
      // This instance may already have been registered if singleton - in which case registered class info should match
      // However QLibrary cQBasicFieldType uses custom constructor in which createInstance calls factory createInstance
      // This causes the same instance to be registered with two different class infos
      // Therefore cannot assert that class info guids match
      if (it->second->getClassInfo()->getGuid() != AboutCtor->getClassInfo()->getGuid())
        QSOS((L"instance already registered with different class info - %s %s", it->second->getClassInfo()->getGuid().toString().c_str(), AboutCtor->getClassInfo()->getGuid().toString().c_str()));
    }*/
  }
  catch (cRecoverable &)
  {
    /*
    Need this to make sure stack is unwound in the event of the "unhandled exception handler" being called
    which _doesn't_ unwind! Fixes m_ThreadSafety not being unlocked when cNotFound is thrown.
    */
    throw;
  }
}

// ---------------------------------------------------------------------------------------

void cFactory::_unregisterInstance(iFactory::iInstance * const Instance)
{
  QVERIFY(Instance);

  try
  {
    // Protect member variables from changes by another thread.
    cLockGuard threadSafety(&m_ThreadSafety);

    InstancesMap::iterator i = m_Instances.find(Instance);
    if (i != m_Instances.end()) // instance will not be found if exception thrown from constructor before instance was registered
    {
      if (_modifyClassInheritanceTreeInstanceCounts(i->second->getClassInfo(), -1 /* decrement */) == 0)
      {
        //m_InstanceCounts.erase(); // leave instance count there for now to avoid changing existing behaviour unnecessarily
      }

      m_Instances.erase(i);
    }
  }
  catch (cRecoverable &)
  {
    /*
    Need this to make sure stack is unwound in the event of the "unhandled exception handler" being called
    which _doesn't_ unwind! Fixes m_ThreadSafety not being unlocked when cNotFound is thrown.
    */
    throw;
  }
}
//}}}
//{{{
// =======================================================================================
// === isClassDerivedFrom ================================================================
// =======================================================================================

bool cFactory::_isClassDerivedFrom(const iFactory::iClassInfo::ConstPtr & ConcreteCandidate, const cGuid & From, const unsigned int RecurseDepth) const
{
  QVERIFY(ConcreteCandidate.isValid());
  if (RecurseDepth > kMaxInheritanceDepth)
  {
    cLogIndentSOSGuard SOSIndent(L"Circular inheritance?");
    _logBases(ConcreteCandidate, 0 /* initial indent level */);
    QFATAL((__FILE__, __LINE__, L"Possible circular inheritance - recursive depth > %d classes!", kMaxInheritanceDepth));
  }
  if (From == ConcreteCandidate->getGuid())
  {
    // ClassInfo _is_ a BaseClassInfo!
    return true;
  }
  else
  {
    const vector<iClassInfo::ConstPtr> CandidateBases(ConcreteCandidate->getBaseInfo());
    for (vector<iClassInfo::ConstPtr>::const_iterator i = CandidateBases.begin(); i != CandidateBases.end(); ++i)
    {
      if (_isClassDerivedFrom(*i, From, RecurseDepth + 1))
      {
        return true;
      }
    }
  }
  return false;
}

bool cFactory::_isKnownClass(const cGuid & ClassGuid) const
{
  return m_Constructors.find(ClassGuid) != m_Constructors.end();
}

bool QAPI cFactory::isClassDerivedFrom(const cGuid & ConcreteClassGuid, const cGuid & BaseClassGuid) const throw(cNotFound)
{
  QVERIFY(!m_IsDestructing);
  if (!_isKnownClass(ConcreteClassGuid)) throw cNotFound(__FILE__, __LINE__, L"\"%s\" is not a known concrete or interface class", ConcreteClassGuid.serialise().c_str());
  QVERIFY(_isConcreteClass(ConcreteClassGuid));
  try
  {
    return _isClassDerivedFrom(findClassInfoByGuid(ConcreteClassGuid), BaseClassGuid, 0 /* initial recurse depth */);
  }
  catch (cRecoverable &)
  {
    /*
    Need this to make sure stack is unwound in the event of the "unhandled exception handler" being called
    which _doesn't_ unwind! Fixes m_ThreadSafety not being unlocked when cNotFound is thrown.
    */
    throw;
  }
}
//}}}
//{{{
// =======================================================================================
// === isInstanceDerivedFrom =============================================================
// =======================================================================================

bool QAPI cFactory::isInstanceDerivedFrom(iInstance::ConstPtr Instance, const cGuid & BaseClassGuid) const
{
  QVERIFY(!m_IsDestructing);
  QVERIFY(Instance.isValid());
  try
  {
    return _isClassDerivedFrom(Instance->getClassInfo(), BaseClassGuid, 0 /* initial recurse depth */);
  }
  catch (cRecoverable &)
  {
    /*
    Need this to make sure stack is unwound in the event of the "unhandled exception handler" being called
    which _doesn't_ unwind! Fixes m_ThreadSafety not being unlocked when cNotFound is thrown.
    */
    throw;
  }
}
//}}}
//{{{
// =======================================================================================
// === addClassRegistrationCallback ======================================================
// =======================================================================================

cGuard::ConstPtr QAPI cFactory::addClassRegistrationCallback(const iClassRegistrationCallback::Ptr & newCallback)
{
  cLockGuard threadSafety(&m_ClassRegCallbacksThreadSafety);
  ConstructorMap copyOfCtors = m_Constructors;
  {
    cLockGuard threadSafety2(&m_ThreadSafety);
    for (ConstructorMap::const_iterator i = copyOfCtors.begin(); i != copyOfCtors.end(); ++i)
    {
      newCallback->onFactoryClassRegistered(i->second->getClassInfo()->getGuid());
    }
  }
  m_ClassRegCallbacks.push_back(newCallback);
  return new cGuard(this, newCallback);
}
//}}}
//{{{
// =======================================================================================
// === onGuardDestroyed ==================================================================
// =======================================================================================

void QAPI cFactory::onGuardDestroyed(iGuarded::Ptr guarded)
{
  iConstructor::Ptr candidate = ptr_cast<iConstructor::Ptr>(guarded);
  if (candidate.isValid())
  {
    try
    {
      cLockGuard threadSafety(&m_ThreadSafety);

      m_NumRegisteredClasses--;

      const cGuid GuidOfConcreteClassToRemove(candidate->getClassInfo()->getGuid());

      m_Constructors.erase(GuidOfConcreteClassToRemove);

      {
        vector<iClassInfo::ConstPtr> Bases;
        _getBases(candidate->getClassInfo(), &Bases);

        for (vector<iClassInfo::ConstPtr>::const_iterator i = Bases.begin(); i != Bases.end(); ++i)
        {
          ConstructorMap::iterator j = m_Constructors.find((*i)->getGuid());
            
          while ((j != m_Constructors.end()) && (j->first == (*i)->getGuid()))
          {
            if (j->second->getClassInfo()->getGuid() == GuidOfConcreteClassToRemove)
            {
              j = m_Constructors.erase(j);
            }
            else
            {
              j++;
            }
          }
        }
      }
    }
    catch (cRecoverable &)
    {
      /*
      Need this to make sure stack is unwound in the event of the "unhandled exception handler" being called
      which _doesn't_ unwind! Fixes m_ThreadSafety not being unlocked when cNotFound is thrown.
      */
      throw;
    }
  }
  else
  {
    iClassRegistrationCallback::Ptr candidate = ptr_cast<iClassRegistrationCallback::Ptr>(guarded);
    if (candidate.isValid())
    {
       cLockGuard threadSafety(&m_ClassRegCallbacksThreadSafety);
       vector<iClassRegistrationCallback::Ptr>::iterator i = find(m_ClassRegCallbacks.begin(), m_ClassRegCallbacks.end(), candidate);
       QVERIFY(i != m_ClassRegCallbacks.end());
       m_ClassRegCallbacks.erase(i);
    }
  }
}
//}}}

//{{{
/* Method "checkAndRegisterDump" registers the factory as a sump participant if the dump manager
                                 has been loaded
*/

void QAPI cFactory::checkAndRegisterDump()
{
  if (iKernelDumpManager::isSingletonRegistered() && iKernelDumpManager::singleton().isDumpEnabled())
    m_dumpRegistration = iKernelDumpManager::singleton().Register(this);
}
//}}}

//{{{
/* Method "onDump" generates the dump information when required
*/

void QAPI cFactory::onDump(vfs::iKernelDumpProcessor *const proc)
{
  proc->GenFile(L"factory.qdi");
    // Protect member variables from changes by another thread.
  cLockGuard threadSafety(&m_ThreadSafety);

  proc->AppendF(L"FAC~%d~%Iu\r\n", m_NumRegisteredClasses, m_Instances.size());

  for (ConstructorMap::const_iterator i = m_Constructors.begin(); i != m_Constructors.end(); ++i)
  {
    if (i->first == i->second->getClassInfo()->getGuid())
    {
      proc->AppendF(L"OBJ~%d~%s~%s\r\n",
                        countInstances(i->second->getClassInfo()->getGuid()),
                        i->second->getClassInfo()->getName().c_str(),
                        i->first.serialise(false).c_str());
    }
  }

  proc->Close();
}

//}}}
//}}}
