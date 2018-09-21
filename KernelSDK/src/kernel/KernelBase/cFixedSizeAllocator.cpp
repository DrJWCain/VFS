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
// === cFixedSizeAllocator::cPool ========================================================
// =======================================================================================

cFixedSizeAllocator::cPool::cPool(const unsigned int NumBytesPerObject, const unsigned int NumObjectsPerPool) :
  m_NumBytesPerObject(NumBytesPerObject),
  m_LastBlockAllocd(-1),
  m_LastBlockFreed(-1),
  m_NumObjectsAllocated(0),
  m_SizeOfMemory(NumBytesPerObject * NumObjectsPerPool),
  m_IsFull(false)
{
  m_Memory = (char *)malloc(m_SizeOfMemory);
  m_IsAllocated.resize(NumObjectsPerPool);
  (void)memset(&m_IsAllocated[0], 0, NumObjectsPerPool * sizeof(char));
}

cFixedSizeAllocator::cPool::~cPool()
{
  delete m_Memory;
}

void * cFixedSizeAllocator::cPool::allocate(void)
{
  if (m_IsFull) return 0;

  int BlockAllocd;

  if (m_LastBlockFreed == -1 || m_IsAllocated.at(m_LastBlockFreed))
  {
    bool HasSearchedFromBeginning = false;

    do
    {
      m_LastBlockAllocd++;

      if (m_LastBlockAllocd == (signed)m_IsAllocated.size()) // Is at end of pool?
      {
        m_LastBlockAllocd = 0;
        if (HasSearchedFromBeginning) // Already searched from beginning?
        {
          m_IsFull = true;
          return 0;
        }
        HasSearchedFromBeginning = true;
      }
    } while (m_IsAllocated.at(m_LastBlockAllocd));

    BlockAllocd = m_LastBlockAllocd;
  }
  else
  {
    BlockAllocd = m_LastBlockFreed;

    m_LastBlockFreed = -1;
  }

  assert(BlockAllocd < (signed)m_IsAllocated.size());

  m_NumObjectsAllocated++;
  m_IsAllocated.at(BlockAllocd) = true;
  return m_Memory + BlockAllocd * m_NumBytesPerObject;
}

void cFixedSizeAllocator::cPool::release(void * const ObjectToDelete)
{
  const int BlockToFree = (int)(((char *)ObjectToDelete - m_Memory) / m_NumBytesPerObject);
  assert(BlockToFree >= 0 && BlockToFree < (signed)m_IsAllocated.size());

  m_IsAllocated.at(BlockToFree) = false;
  m_LastBlockFreed              = BlockToFree;
  m_NumObjectsAllocated--;
  m_IsFull = false;
}

bool cFixedSizeAllocator::cPool::isObjectInPool(void * const Object) const
{
  return (Object >= m_Memory) &&
         (Object < (m_Memory + m_SizeOfMemory));
}

unsigned int cFixedSizeAllocator::cPool::getNumObjectsAllocated(void) const
{
  return m_NumObjectsAllocated;
}




// =======================================================================================
// === cFixedSizeAllocator ===============================================================
// =======================================================================================

cFixedSizeAllocator::cFixedSizeAllocator(const unsigned int NumBytesPerObject, const unsigned int NumObjectsPerPool) throw(cNoRoom) :
  m_NumBytesPerObject(NumBytesPerObject),
  m_NumObjectsPerPool(NumObjectsPerPool),
  m_Lock(L"FixedSizeMemAllocator")
{
}

void * QAPI cFixedSizeAllocator::allocate(void) throw(cNoRoom)
{
  cLockGuard LockGuard(&m_Lock); // Protect from changes by another thread.

  for (unsigned int i = 0; i < m_Pools.size(); ++i)
  {
    void * NewObject = m_Pools.at(i)->allocate();
    if (NewObject != 0)
    {
      return NewObject;
    }
  }
  cPool::Ptr NewPool = new cPool(m_NumBytesPerObject, m_NumObjectsPerPool);
  m_Pools.push_back(NewPool);
  return NewPool->allocate();
}

void QAPI cFixedSizeAllocator::release(void * const ObjectToDelete)
{
  cLockGuard LockGuard(&m_Lock); // Protect from changes by another thread.

  for (unsigned int i = 0; i < m_Pools.size(); ++i)
  {
    if (m_Pools.at(i)->isObjectInPool(ObjectToDelete))
    {                
      m_Pools.at(i)->release(ObjectToDelete);
      return;
    }
  }
  assert(false); // Tried to free object not in any pool
}
