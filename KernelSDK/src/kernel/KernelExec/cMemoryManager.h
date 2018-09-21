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

#include "QKernelExec.h"

using namespace vfs;
using namespace std;




// =======================================================================================
// === cMemoryManager ====================================================================
// =======================================================================================

class cMemoryManager : public iMemoryManager, public cRefCount
{
  public:

    QDEFINE_SINGLETON_IMPL(cMemoryManager, L"{E0C4D43F-D8E5-4118-A0C2-F930CE339203} // cMemoryManager", iMemoryManager);

    // iManager
    virtual void QAPI logSummary() {}
    virtual void QAPI logDetailedSummary() {}

    // iMemoryManager
    virtual bool QAPI setHeapTracking(const bool IsTracking) { return false; /* nop - we're deprecated */ }
    virtual void QAPI snapshotHeap(CRTMEMSTATE * const NewSnapshot) { /* nop - we're deprecated */ }
    virtual bool QAPI sosHeapDifferences(const CRTMEMSTATE * const PrevSnapshot) { return false; /* nop - we're deprecated */ }
    virtual iMemoryAllocator::Ptr QAPI createFixedSizeAllocator(
      const unsigned int NumBytesPerObject,
      const unsigned int NumObjectsPerPool,
      const unsigned int Alignment) throw(cNoRoom);
    virtual iMemoryAllocator::Ptr QAPI createFixedSizeAllocator(
      const unsigned int NumBytesPerObject,
      const unsigned int NumObjectsPerPool) throw(cNoRoom);
    virtual unsigned int QAPI getFastIOAlignment() const;
    virtual cMemoryView::ConstPtr QAPI getEmptiness(const size_t numBytes) const;

    // cMemoryManager
    cMemoryManager();

  private :
    // Not Permitted
    virtual cMemoryView::ConstPtr QAPI getEmptiness(const not_size_t numBytes) const throw(cFileError) { throw cFileError(__FILE__, __LINE__, L"Disallowed call"); }
    cMemoryView::ConstPtr _initEmptiness(const not_size_t numBytes) const throw(cFileError) { throw cFileError(__FILE__, __LINE__, L"Disallowed call"); return cMemoryView::ConstPtr(); }

    vector<cGuard::ConstPtr> m_Unwind;

    mutable cLock               m_EmptinessThreadSafety;
    mutable cMemoryView::Ptr    m_Emptiness;

    void _initHeap();
    cMemoryView::ConstPtr _initEmptiness(const size_t numBytes) const;
};
