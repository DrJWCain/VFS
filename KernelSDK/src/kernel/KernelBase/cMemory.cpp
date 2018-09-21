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
#include "cMemoryImpl.h"
#include "fatal.h"

using namespace vfs;




// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

cMemory::cMemory(const size_t NumBytes, const eType TypeOfMemory) throw(cNoRoom) :
  m_Pimpl(new cMemoryImpl(NumBytes, TypeOfMemory, 4))
{
#if QSWITCH_CHECK_MEM_GUARD_BAND_ON_PTR_ACCESS
  setTracking(true);
#endif
}

// ---------------------------------------------------------------------------------------

cMemory::cMemory(const size_t NumBytes,const eType TypeOfMemory, const size_t ByteAlignment) throw(cNoRoom) :
  m_Pimpl(new cMemoryImpl(NumBytes, TypeOfMemory, ByteAlignment))
{
#if QSWITCH_CHECK_MEM_GUARD_BAND_ON_PTR_ACCESS
  setTracking(true);
#endif
}

// ---------------------------------------------------------------------------------------

cMemory::cMemory(const String & StringToCopy, const eType TypeOfMemory) throw(cNoRoom) :
  m_Pimpl(new cMemoryImpl((1u + StringToCopy.size()) * sizeof(Char), TypeOfMemory, 4))
{
  (void)wcscpy((Char *)getBytes(), &StringToCopy[0]);
#if QSWITCH_CHECK_MEM_GUARD_BAND_ON_PTR_ACCESS
  setTracking(true);
#endif
}

// ---------------------------------------------------------------------------------------

cMemory::cMemory(char * const BytesToUse, const size_t NumBytes, const size_t ByteAlignment, cPtr<iMemoryObserver> Observer) :
  m_Pimpl(new cMemoryImpl(BytesToUse, NumBytes, ByteAlignment, Observer))
{
#if QSWITCH_CHECK_MEM_GUARD_BAND_ON_PTR_ACCESS
  setTracking(true);
#endif
}

// ---------------------------------------------------------------------------------------

cMemory::~cMemory()
{
}




// =======================================================================================
// === getType/getAlignment ==============================================================
// =======================================================================================

cMemory::eType QAPI cMemory::getType() const
{
  return m_Pimpl->getType();
}

// ---------------------------------------------------------------------------------------

size_t QAPI cMemory::getAlignment() const
{
  return m_Pimpl->getAlignment();
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
  
//    const unsigned int tmp1 = ((unsigned int *)src)[0];
//    const unsigned int tmp2 = ((unsigned int *)src)[1];
//    ((unsigned int *)dst)[0] = tmp1;
//    ((unsigned int *)dst)[1] = tmp2;
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

cMemory::Ptr QAPI cMemory::clone() const
{
  cMemory::Ptr Clone = new cMemory(getSize(), getType(), getAlignment());
  _fastMemCpy(Clone->getBytes(), getConstBytes(), getSize());
  return Clone;
}

// ---------------------------------------------------------------------------------------

cMemory::Ptr QAPI cMemory::cloneAppending(cMemory::ConstPtr ToAppend) const
{
  cMemory::Ptr Clone = new cMemory(getSize() + ToAppend->getSize(), getType(), getAlignment());
  _fastMemCpy(Clone->getBytes(), getConstBytes(), getSize());
  _fastMemCpy(Clone->getBytes() + getSize(), ToAppend->getConstBytes(), ToAppend->getSize());
  return Clone;
}

String QAPI cMemory::getName() const
{
  return m_Pimpl->getName();
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
  while (i >= 32)
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
  const unsigned __int64 c8 = ((unsigned __int64)c << 56) | ((unsigned __int64)c << 48) | ((unsigned __int64)c << 40) | ((unsigned __int64)c << 32) |
                              ((unsigned __int64)c << 24) | ((unsigned __int64)c << 16) | ((unsigned __int64)c << 8) | (unsigned __int64)c;
  while (i >= 64)
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

void QAPI cMemory::clear()
{
  _fastMemSet(getBytes(), 0, getSize());
}




// =======================================================================================
// === resize/getSize/getAlignedSize =====================================================
// =======================================================================================

void QAPI cMemory::resize(const size_t NewSize) throw(cNoRoom)
{
  m_Pimpl->resize(NewSize);
}

// ---------------------------------------------------------------------------------------

size_t QAPI cMemory::getSize() const
{
  return m_Pimpl->getSize();
}

// ---------------------------------------------------------------------------------------

size_t QAPI cMemory::getAlignedSize() const
{
  return m_Pimpl->getAlignedSize();
}




// =======================================================================================
// === getConstBytes/getBytes ============================================================
// =======================================================================================

const char * QAPI cMemory::getConstBytes() const throw(cNotAvailable)
{
  return m_Pimpl->getBytes();
}

// ---------------------------------------------------------------------------------------

char * QAPI cMemory::getBytes() throw(cNotAvailable)
{
  return m_Pimpl->getBytes();
}




// =======================================================================================
// === cMemory not permitted ===========================================================
// =======================================================================================

cMemory::cMemory(cMemory & Other)
{
  // Not implemented yet!
  QKERNELBASEVERIFY(false);
}

// ---------------------------------------------------------------------------------------

void cMemory::operator=(cMemory & Other)
{
  // Not implemented yet!
  QKERNELBASEVERIFY(false);
}

// ---------------------------------------------------------------------------------------

void cMemory::operator&()
{
  // Not implemented yet!
  QKERNELBASEVERIFY(false);
}

// ---------------------------------------------------------------------------------------

void cMemory::operator,(cMemory & Other)
{
  // Not implemented yet!
  QKERNELBASEVERIFY(false);
}




// =======================================================================================
// === onAfterAddRef/onBeforeRemoveRef ===================================================
// =======================================================================================

#if QSWITCH_CHECK_MEM_GUARD_BAND_ON_PTR_ACCESS

void QAPI cMemory::onAfterAddRef(const void * const PtrID, const void * const ModuleAddr, const unsigned int  RefCount) const
{
  (void)getConstBytes(); // Just to prompt impl to check it's guard band
}

// ---------------------------------------------------------------------------------------

void QAPI cMemory::onBeforeRemoveRef(const void * const PtrID, const void * const ModuleAddr, const unsigned int  RefCount) const
{
  (void)getConstBytes(); // Just to prompt impl to check it's guard band
}

#endif
