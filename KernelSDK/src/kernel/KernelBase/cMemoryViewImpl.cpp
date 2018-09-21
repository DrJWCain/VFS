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
#include "QKernelBase.h"
#include "cMemoryViewImpl.h"
#include "fatal.h"

using namespace vfs;




// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

cMemoryViewImpl::cMemoryViewImpl(cMemory::Ptr Memory, const size_t ByteOffset, const size_t NumBytes, cPtr<iMemoryObserver> Observer) :
  m_Memory(Memory),
  m_ByteOffset(ByteOffset),
  m_NumBytes(NumBytes),
  m_Observer(Observer),
  m_ClassVersion(0)
{
  // allow for r/w access into memory beyond declared size but available because of alignment
  QKERNELBASEVERIFY(m_ByteOffset <= m_Memory->getAlignedSize());
  QKERNELBASEVERIFY(m_ByteOffset + m_NumBytes <= m_Memory->getAlignedSize());
}

cMemoryViewImpl::~cMemoryViewImpl()
{
  if (m_Observer.isValid()) 
    m_Observer->onRelease (m_Memory->getBytes() + m_ByteOffset);
}



// =======================================================================================
// === getPrivateMemory ==================================================================
// =======================================================================================

cMemory::Ptr cMemoryViewImpl::getPrivateMemory(size_t * const OptionalByteOffset) const
{
  if (OptionalByteOffset) *OptionalByteOffset = m_ByteOffset;
  return m_Memory;
}

String cMemoryViewImpl::getName() const
{
  return m_Memory->getName();
}


// =======================================================================================
// === setOffset/getOffset ===============================================================
// =======================================================================================

void cMemoryViewImpl::setOffset(const size_t NewByteOffset)
{
  QKERNELBASEVERIFY(NewByteOffset <= m_Memory->getSize());
  QKERNELBASEVERIFY(NewByteOffset + m_NumBytes <= m_Memory->getSize());
  m_ByteOffset = NewByteOffset;
}

// ---------------------------------------------------------------------------------------

size_t cMemoryViewImpl::getOffset() const
{
  return m_ByteOffset;
}




// =======================================================================================
// === setSize/getSize ===================================================================
// =======================================================================================

void cMemoryViewImpl::setSize(const size_t NewNumBytes)
{
  QKERNELBASEVERIFY(m_ByteOffset + NewNumBytes <= m_Memory->getSize());
  m_NumBytes = NewNumBytes;
}

// ---------------------------------------------------------------------------------------

size_t cMemoryViewImpl::getSize() const
{
  return m_NumBytes;
}

// ---------------------------------------------------------------------------------------

size_t cMemoryViewImpl::getAlignedSize() const
{
  return m_NumBytes;
}




// =======================================================================================
// === getBytes ==========================================================================
// =======================================================================================

char * cMemoryViewImpl::getBytes() throw(cNotAvailable)
{
  return m_Memory->getBytes() + m_ByteOffset;
}
