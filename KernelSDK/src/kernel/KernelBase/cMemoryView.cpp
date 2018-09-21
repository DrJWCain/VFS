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
using namespace std;




// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

cMemoryView::cMemoryView(const cMemory::Ptr & Memory) :
  m_Pimpl(new cMemoryViewImpl(Memory, 0, Memory->getSize()))
{
#if QSWITCH_CHECK_MEM_GUARD_BAND_ON_PTR_ACCESS
  setTracking(true);
#endif
}

// ---------------------------------------------------------------------------------------

cMemoryView::cMemoryView(const cMemory::Ptr & Memory, const size_t ByteOffset, const size_t NumBytes) :
  m_Pimpl(new cMemoryViewImpl(Memory, ByteOffset, NumBytes))
{
#if QSWITCH_CHECK_MEM_GUARD_BAND_ON_PTR_ACCESS
  setTracking(true);
#endif
}

// ---------------------------------------------------------------------------------------

cMemoryView::cMemoryView(const cMemory::Ptr & Memory, const size_t ByteOffset, const size_t NumBytes, cPtr<iMemoryObserver> Observer) :
  m_Pimpl(new cMemoryViewImpl(Memory, ByteOffset, NumBytes, Observer))
{
#if QSWITCH_CHECK_MEM_GUARD_BAND_ON_PTR_ACCESS
  setTracking(true);
#endif
}



// =======================================================================================
// === first/firstConst ==================================================================
// =======================================================================================

cMemoryView::Ptr QAPI cMemoryView::first(const size_t NumBytes)
{
  QKERNELBASEVERIFY(NumBytes <= getSize());
  return range(0, NumBytes);
}

// ---------------------------------------------------------------------------------------

cMemoryView::ConstPtr QAPI cMemoryView::firstConst(const size_t NumBytes) const
{
  QKERNELBASEVERIFY(NumBytes <= getSize());
  return rangeConst(0, NumBytes);
}




// =======================================================================================
// === getPrivateMemory ==================================================================
// =======================================================================================

cMemory::ConstPtr QAPI cMemoryView::getPrivateMemory() const
{
  return m_Pimpl->getPrivateMemory(0);
}

// ---------------------------------------------------------------------------------------

cMemory::ConstPtr QAPI cMemoryView::getPrivateMemory(size_t * const ByteOffset) const
{
  return m_Pimpl->getPrivateMemory(ByteOffset);
}

String QAPI cMemoryView::getName() const
{
  return m_Pimpl->getName();
}



// =======================================================================================
// === last/lastConst ====================================================================
// =======================================================================================

cMemoryView::Ptr QAPI cMemoryView::last(const size_t NumBytes)
{
  QKERNELBASEVERIFY(NumBytes <= getSize());
  return range(getSize() - NumBytes, NumBytes);
}

// ---------------------------------------------------------------------------------------

cMemoryView::ConstPtr QAPI cMemoryView::lastConst(const size_t NumBytes) const
{
  QKERNELBASEVERIFY(NumBytes <= getSize());
  return rangeConst(getSize() - NumBytes, NumBytes);
}




// =======================================================================================
// === trimmed/trimmedConst ==============================================================
// =======================================================================================

cMemoryView::Ptr QAPI cMemoryView::trimmed(const size_t ByteOffsetFromStart, const size_t ByteOffsetFromEnd)
{
  QKERNELBASEVERIFY(ByteOffsetFromStart + ByteOffsetFromEnd <= getSize());
  return range(ByteOffsetFromStart, getSize() - ByteOffsetFromStart - ByteOffsetFromEnd);
}

// ---------------------------------------------------------------------------------------

cMemoryView::ConstPtr QAPI cMemoryView::trimmedConst(const size_t ByteOffsetFromStart, const size_t ByteOffsetFromEnd) const
{
  QKERNELBASEVERIFY(ByteOffsetFromStart + ByteOffsetFromEnd <= getSize());
  return rangeConst(ByteOffsetFromStart, getSize() - ByteOffsetFromStart - ByteOffsetFromEnd);
}




// =======================================================================================
// === range/rangeConst ==================================================================
// =======================================================================================

cMemoryView::Ptr QAPI cMemoryView::range(const size_t ByteOffset, const size_t NumBytes)
{
  size_t OffsetIntoWhole;
  cMemory::Ptr WholeMemory = m_Pimpl->getPrivateMemory(&OffsetIntoWhole);
  return new cMemoryView(WholeMemory, OffsetIntoWhole + ByteOffset, NumBytes);
}

// ---------------------------------------------------------------------------------------

cMemoryView::ConstPtr QAPI cMemoryView::rangeConst(const size_t ByteOffset, const size_t NumBytes) const
{
  return const_cast<cMemoryView *>(this)->range(ByteOffset, NumBytes);
}




// =======================================================================================
// === getSize/getAlignedSize ============================================================
// =======================================================================================

size_t QAPI cMemoryView::getSize() const
{
  return m_Pimpl->getSize();
}




// =======================================================================================
// === clear =============================================================================
// =======================================================================================

namespace
{

#pragma optimize("t", on) // optimise for speed

void _fastMemSet(char * dst, const char c, unsigned int i)
{
  const unsigned int c4 = ((unsigned int)c << 24) | ((unsigned int)c << 16) | ((unsigned int)c << 8) | (unsigned int)c;
  while (i > 32)
  {
    ((unsigned int *)dst)[0] = c4;
    ((unsigned int *)dst)[1] = c4;
    ((unsigned int *)dst)[2] = c4;
    ((unsigned int *)dst)[3] = c4;
    ((unsigned int *)dst)[4] = c4;
    ((unsigned int *)dst)[5] = c4;
    ((unsigned int *)dst)[6] = c4;
    ((unsigned int *)dst)[7] = c4;
    dst += 32;
    i   -= 32;
  }
  while (i > 0)
  {
    *dst++ = c;
    i -= 1;
  }
}

void _fastMemSet(char * dst, const char c, unsigned __int64 i)
{
  const unsigned int c8 = ((unsigned __int64)c << 56) | ((unsigned __int64)c << 48) | ((unsigned __int64)c << 40) | ((unsigned __int64)c << 32) |
                          ((unsigned __int64)c << 24) | ((unsigned __int64)c << 16) | ((unsigned __int64)c << 8) | (unsigned __int64)c;
  while (i > 64)
  {
    ((unsigned __int64 *)dst)[0] = c8;
    ((unsigned __int64 *)dst)[1] = c8;
    ((unsigned __int64 *)dst)[2] = c8;
    ((unsigned __int64 *)dst)[3] = c8;
    ((unsigned __int64 *)dst)[4] = c8;
    ((unsigned __int64 *)dst)[5] = c8;
    ((unsigned __int64 *)dst)[6] = c8;
    ((unsigned __int64 *)dst)[7] = c8;
    dst += 64;
    i   -= 64;
  }
  while (i > 0)
  {
    *dst++ = c;
    i -= 1;
  }
}

#pragma optimize("", on)

}

void QAPI cMemoryView::clear()
{
  _fastMemSet(getBytes(), 0, getSize());
}




// =======================================================================================
// === clone =============================================================================
// =======================================================================================

namespace
{

#pragma optimize("t", on) // optimise for speed

void _fastMemCpy(char * dst, const char * src, unsigned int i)
{
  while (i >= 32)
  {
    ((unsigned int *)dst)[0] = ((unsigned int *)src)[0];
    ((unsigned int *)dst)[1] = ((unsigned int *)src)[1];
    ((unsigned int *)dst)[2] = ((unsigned int *)src)[2];
    ((unsigned int *)dst)[3] = ((unsigned int *)src)[3];
    ((unsigned int *)dst)[4] = ((unsigned int *)src)[4];
    ((unsigned int *)dst)[5] = ((unsigned int *)src)[5];
    ((unsigned int *)dst)[6] = ((unsigned int *)src)[6];
    ((unsigned int *)dst)[7] = ((unsigned int *)src)[7];
    src += 32;
    dst += 32;
    i   -= 32;
  }
  while (i > 0)
  {
    *dst++ = *src++;
    i -= 1;
  }
}

void _fastMemCpy(char * dst, const char * src, unsigned __int64 i)
{
  while (i >= 64)
  {
    ((unsigned __int64 *)dst)[0] = ((unsigned __int64 *)src)[0];
    ((unsigned __int64 *)dst)[1] = ((unsigned __int64 *)src)[1];
    ((unsigned __int64 *)dst)[2] = ((unsigned __int64 *)src)[2];
    ((unsigned __int64 *)dst)[3] = ((unsigned __int64 *)src)[3];
    ((unsigned __int64 *)dst)[4] = ((unsigned __int64 *)src)[4];
    ((unsigned __int64 *)dst)[5] = ((unsigned __int64 *)src)[5];
    ((unsigned __int64 *)dst)[6] = ((unsigned __int64 *)src)[6];
    ((unsigned __int64 *)dst)[7] = ((unsigned __int64 *)src)[7];
    src += 64;
    dst += 64;
    i   -= 64;
  }
  while (i > 0)
  {
    *dst++ = *src++;
    i -= 1;
  }
}

#pragma optimize("", on)

}

cMemoryView::Ptr QAPI cMemoryView::clone() const
{
  const cMemory::ConstPtr UnderlyingMem = getPrivateMemory();
  const cMemory::eType Type = UnderlyingMem->getType();
  cMemoryView::Ptr Clone = new cMemoryView(new cMemory(getSize(), Type == cMemory::ePreallocated ? cMemory::eHeap : Type, UnderlyingMem->getAlignment()));
  _fastMemCpy(Clone->getBytes(), getConstBytes(), getSize());
  return Clone;
}

// ---------------------------------------------------------------------------------------

cMemoryView::Ptr QAPI cMemoryView::cloneAppending(const cMemoryView::ConstPtr & ToAppend) const
{
  list<cMemoryView::Ptr> ToAppendList;
  ToAppendList.push_back(ptr_const_cast<cMemoryView::Ptr>(ToAppend));
  return cloneAppending(ToAppendList);
}

// ---------------------------------------------------------------------------------------

cMemoryView::Ptr QAPI cMemoryView::cloneAppending(const list<cMemoryView::Ptr> & ToAppend) const
{
  list<cMemoryView::ConstPtr> ToAppendConst;
  for (list<cMemoryView::Ptr>::const_iterator i = ToAppend.begin(); i != ToAppend.end(); ++i) ToAppendConst.push_back(*i);
  return cloneAppendingConst(ToAppendConst);
}

// ---------------------------------------------------------------------------------------

cMemoryView::Ptr QAPI cMemoryView::cloneAppendingConst(const list<cMemoryView::ConstPtr> & ToAppend) const
{
  size_t ResultSize = getSize();
  for (list<cMemoryView::ConstPtr>::const_iterator i = ToAppend.begin(); i != ToAppend.end(); ++i) ResultSize += (*i)->getSize();
  const cMemory::ConstPtr UnderlyingMem = getPrivateMemory();
  const cMemory::eType Type = UnderlyingMem->getType();
  cMemoryView::Ptr Result = new cMemoryView(new cMemory(ResultSize, Type == cMemory::ePreallocated ? cMemory::eHeap : Type, UnderlyingMem->getAlignment()));
  char * WritePtr = Result->getBytes();
  _fastMemCpy(WritePtr, getConstBytes(), getSize());
  WritePtr += getSize();
  for (list<cMemoryView::ConstPtr>::const_iterator j = ToAppend.begin(); j != ToAppend.end(); ++j)
  {
    _fastMemCpy(WritePtr, (*j)->getConstBytes(), (*j)->getSize());
    WritePtr += (*j)->getSize();
  }
  return Result;
}




// =======================================================================================
// === getOffset =========================================================================
// =======================================================================================

size_t QAPI cMemoryView::getOffset() const
{
  return m_Pimpl->getOffset();
}




// =======================================================================================
// === getConstBytes/getBytes ============================================================
// =======================================================================================

const char * QAPI cMemoryView::getConstBytes() const throw(cNotAvailable)
{
  return m_Pimpl->getBytes();
}

// ---------------------------------------------------------------------------------------

char * QAPI cMemoryView::getBytes() throw(cNotAvailable)
{
  return m_Pimpl->getBytes();
}




// =======================================================================================
// === onAfterAddRef/onBeforeRemoveRef ===================================================
// =======================================================================================

#if QSWITCH_CHECK_MEM_GUARD_BAND_ON_PTR_ACCESS

void QAPI cMemoryView::onAfterAddRef(const void * const PtrID, const void * const ModuleAddr, const unsigned int  RefCount) const
{
  (void)getConstBytes(); // Just to prompt impl to check it's guard band
}

// ---------------------------------------------------------------------------------------

void QAPI cMemoryView::onBeforeRemoveRef(const void * const PtrID, const void * const ModuleAddr, const unsigned int  RefCount) const
{
  (void)getConstBytes(); // Just to prompt impl to check it's guard band
}

#endif
