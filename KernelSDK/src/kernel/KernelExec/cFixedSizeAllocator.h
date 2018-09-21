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

// --- Headers ---------------------------------------------------------------------------

#include "QKernelBase.h"

// --- Use Namespace ---------------------------------------------------------------------

using namespace vfs;




// =======================================================================================
// === cFixedSizeAllocator2 ==============================================================
// =======================================================================================

class cFixedSizeAllocator2 : public iMemoryAllocator, public cRefCount
{
  private :

    class cPool : public cRefCount
    {
      public :

        QDEFINE_SMARTPTR(cPool);

        cPool(const unsigned int NumBytesPerObject, const unsigned int NumObjectsPerPool, const unsigned int Alignment) throw();
        void * allocate() throw();
        void release(void * const ObjectToDelete) throw();
        bool isObjectInPool(void * const Object) const throw();
        unsigned int getNumObjectsAllocated(void) const throw();

      private :

        const unsigned int  m_NumBytesPerObject;
        cMemory::Ptr        m_Memory;
        std::vector<char>   m_IsAllocated;
        int                 m_LastBlockAllocd;
        int                 m_LastBlockFreed;
        int                 m_NumObjectsAllocated;
        bool                m_IsFull;
        const unsigned int  m_MaxNumNumObjects;
    };

    cLock                   m_Lock;
    int                     m_NumBytesPerObject;
    unsigned int            m_NumObjectsPerPool;
    const unsigned int      m_Alignment;
    std::vector<cPool::Ptr> m_Pools;

  public :

    QDEFINE_SMARTPTR(cFixedSizeAllocator2);

    cFixedSizeAllocator2(const unsigned int NumBytesPerObject, const unsigned int NumObjectsPerPool, const unsigned int Alignment) throw();
    void * QAPI allocate(void) throw();
    void QAPI release(void * const ObjectToDelete) throw();
};
