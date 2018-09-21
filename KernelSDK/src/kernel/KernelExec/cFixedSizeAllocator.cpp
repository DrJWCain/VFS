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

// --- Pre-Compiled Headers --------------------------------------------------------------

#include "stdafx.h"

// --- Headers ---------------------------------------------------------------------------

#include "cFixedSizeAllocator.h"

// --- Using Namespace -------------------------------------------------------------------

using namespace vfs;




// =======================================================================================
// === cFixedSizeAllocator2::cPool ========================================================
// =======================================================================================

cFixedSizeAllocator2::cPool::cPool(const unsigned int NumBytesPerObject, const unsigned int NumObjectsPerPool, const unsigned int Alignment) throw() :
  m_NumBytesPerObject(NumBytesPerObject),
  m_LastBlockAllocd(-1),
  m_LastBlockFreed(-1),
  m_NumObjectsAllocated(0),
  m_IsFull(false),
  m_MaxNumNumObjects(NumObjectsPerPool)
{
  m_Memory = new cMemory((size_t)NumBytesPerObject * (size_t)NumObjectsPerPool, cMemory::eHeap, (size_t)Alignment);

  m_IsAllocated.resize(NumObjectsPerPool);
  (void)memset(&m_IsAllocated[0], 0, NumObjectsPerPool * sizeof(char));
}

void * cFixedSizeAllocator2::cPool::allocate() throw()
{
  if (m_IsFull) return 0;

  int BlockAllocd;
  if (m_LastBlockFreed == -1 || m_IsAllocated[m_LastBlockFreed])
  {
    bool HasSearchedFromBeginning = false;

    do
    {
      m_LastBlockAllocd++;

      if (m_LastBlockAllocd == (signed)m_MaxNumNumObjects) // Is at end of pool?
      {
        m_LastBlockAllocd = 0;
        if (HasSearchedFromBeginning) // Already searched from beginning?
        {
          m_IsFull = true;
          return 0;
        }
        HasSearchedFromBeginning = true;
      }
    } while (m_IsAllocated[m_LastBlockAllocd]);

    BlockAllocd = m_LastBlockAllocd;
  }
  else
  {
    BlockAllocd = m_LastBlockFreed;
    m_LastBlockFreed = -1;
  }

  QVERIFY(BlockAllocd < (signed)m_MaxNumNumObjects);

  m_NumObjectsAllocated++;
  m_IsAllocated[BlockAllocd] = true;
  return m_Memory->getBytes() + BlockAllocd * m_NumBytesPerObject;
}

void cFixedSizeAllocator2::cPool::release(void * const ObjectToDelete) throw()
{
  const int BlockToFree = (int)(((char *)ObjectToDelete - m_Memory->getBytes()) / m_NumBytesPerObject);
  QVERIFY(BlockToFree >= 0 && BlockToFree < (signed)m_IsAllocated.size());

  m_IsAllocated[BlockToFree] = false;
  m_LastBlockFreed           = BlockToFree;
  m_NumObjectsAllocated--;
  m_IsFull = false;
}

bool cFixedSizeAllocator2::cPool::isObjectInPool(void * const Object) const throw()
{
  return (Object >= m_Memory->getBytes()) &&
         (Object < (m_Memory->getBytes() + m_Memory->getSize()));
}

unsigned int cFixedSizeAllocator2::cPool::getNumObjectsAllocated(void) const throw()
{
  return m_NumObjectsAllocated;
}




// =======================================================================================
// === cFixedSizeAllocator2 ==============================================================
// =======================================================================================

cFixedSizeAllocator2::cFixedSizeAllocator2(const unsigned int NumBytesPerObject, const unsigned int NumObjectsPerPool, const unsigned int Alignment) throw() :
  m_NumBytesPerObject(NumBytesPerObject),
  m_NumObjectsPerPool(NumObjectsPerPool),
  m_Alignment(Alignment)
{
}

void * QAPI cFixedSizeAllocator2::allocate() throw()
{
  cLockGuard LockGuard(&m_Lock); // Protect from changes by another thread.

  for (unsigned int i = 0; i < m_Pools.size(); ++i)
  {
    void * NewObject = m_Pools[i]->allocate();
    if (NewObject != 0)
    {
      return NewObject;
    }
  }
  cPool::Ptr NewPool = new cPool(m_NumBytesPerObject, m_NumObjectsPerPool, m_Alignment);
  m_Pools.push_back(NewPool);
  return NewPool->allocate();
}

void QAPI cFixedSizeAllocator2::release(void * const ObjectToDelete) throw()
{
  cLockGuard LockGuard(&m_Lock); // Protect from changes by another thread.

  for (unsigned int i = 0; i < m_Pools.size(); ++i)
  {
    if (m_Pools[i]->isObjectInPool(ObjectToDelete))
    {                
      m_Pools[i]->release(ObjectToDelete);
      return;
    }
  }
  QVERIFYMSG(false, (L"Tried to free object not in any pool"));
}
