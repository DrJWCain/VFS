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

#if 0
#pragma once
/*
**  
*/

// --- Headers ---------------------------------------------------------------------------

#include "QKernelBase.h"

// --- Use Namespace ---------------------------------------------------------------------

using namespace vfs;




// =======================================================================================
// === cFixedSizeAllocator ===============================================================
// =======================================================================================

class cFixedSizeAllocator : public cRefCount
{
  private :

    class cPool : public cRefCount
    {
      public :

        QDEFINE_SMARTPTR(cPool);

        cPool(const unsigned int NumBytesPerObject, const unsigned int NumObjectsPerPool);
        ~cPool();
        void * allocate(void);
        void release(void * const ObjectToDelete);
        bool isObjectInPool(void * const Object) const;
        unsigned int getNumObjectsAllocated(void) const;

      private :

        const unsigned int  m_NumBytesPerObject;
        char *              m_Memory;
        const unsigned int  m_SizeOfMemory;
        std::vector<char>   m_IsAllocated;
        int                 m_LastBlockAllocd;
        int                 m_LastBlockFreed;
        int                 m_NumObjectsAllocated;
        bool                m_IsFull;
    };

    cLock                   m_Lock;
    int                     m_NumBytesPerObject;
    unsigned int            m_NumObjectsPerPool;
    std::vector<cPool::Ptr> m_Pools;

  public :

    QDEFINE_SMARTPTR(cFixedSizeAllocator);

    cFixedSizeAllocator(const unsigned int NumBytesPerObject, const unsigned int NumObjectsPerPool) throw(cNoRoom);
    void * QAPI allocate(void) throw(cNoRoom);
    void QAPI release(void * const ObjectToDelete);
};
#endif