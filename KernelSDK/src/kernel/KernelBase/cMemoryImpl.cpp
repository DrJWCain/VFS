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
// === m_VirtualPageSize =================================================================
// =======================================================================================

size_t cMemoryImpl::m_VirtualPageSize;




// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

cMemoryImpl::cMemoryImpl(const size_t NumBytes, const cMemory::eType TypeOfMemory, const size_t Alignment) throw(cNoRoom) :
  m_Type(TypeOfMemory),
  m_VirtualMemory(0),
  m_UnalignedSize(NumBytes),
  m_AlignedSize(0),
  m_AlignedCommitedBytes(0),
  m_Alignment(Alignment),
  m_ReservedVirtualMemoryHandle(0),
  m_ClassVersion(0),
  m_PreallocatedMemory(0),
  m_BufferHandle(0),
  m_BufferBytes(0)
{
  QKERNELBASEVERIFY(m_Alignment > 0);
  _reserve();
}

// ---------------------------------------------------------------------------------------

cMemoryImpl::cMemoryImpl(char * const BytesToUse, const size_t NumBytes, const size_t Alignment, cPtr<iMemoryObserver> Observer) :
  m_Type(cMemory::ePreallocated),
  m_VirtualMemory(0),
  m_UnalignedSize(NumBytes),
  m_AlignedSize(0),
  m_AlignedCommitedBytes(0),
  m_Alignment(Alignment),
  m_ReservedVirtualMemoryHandle(0),
  m_ClassVersion(0),
  m_PreallocatedMemory(BytesToUse),
  m_Observer(Observer),
  m_BufferHandle(0),
  m_BufferBytes(0)
{
  QKERNELBASEVERIFY(m_Alignment > 0);
  QKERNELBASEVERIFY(((Int64)BytesToUse % (Int64)Alignment) == 0);
  QKERNELBASEVERIFY((NumBytes % Alignment) == 0); // size must be multiple of alignment
  _reserve();
}

// ---------------------------------------------------------------------------------------

cMemoryImpl::~cMemoryImpl()
{
  _release();
}




// =======================================================================================
// === getType/getByteAlignment ==========================================================
// =======================================================================================

cMemory::eType cMemoryImpl::getType() const
{
#if IS_USING_END_GUARD_BAND
  _verifyGuardBand();
#endif
  return m_Type;
}

// ---------------------------------------------------------------------------------------

size_t cMemoryImpl::getAlignment() const
{
#if IS_USING_END_GUARD_BAND
  _verifyGuardBand();
#endif
  return m_Alignment;
}


// =======================================================================================
// === Name ==============================================================================
// =======================================================================================

String cMemoryImpl::getName() const
{
  return m_Name;
}


// =======================================================================================
// === resize/getSize ====================================================================
// =======================================================================================

void cMemoryImpl::resize(const size_t NewSize) throw(cNoRoom, cNotSupported)
{
#if IS_USING_END_GUARD_BAND
  _verifyGuardBand();
#endif
  if (m_Type == cMemory::ePreallocated)
  {
    throw cNotSupported(__FILE__, __LINE__, L"Memory is preallocated so cannot resize");
  }

  if (m_UnalignedSize < NewSize)
  {
    _release();
    m_UnalignedSize = NewSize;
    _reserve();
  }
  else
  {
    m_UnalignedSize = NewSize;
  }
}

// ---------------------------------------------------------------------------------------

size_t cMemoryImpl::getSize() const
{
#if IS_USING_END_GUARD_BAND
  _verifyGuardBand();
#endif
  return m_UnalignedSize;
}

// ---------------------------------------------------------------------------------------

size_t cMemoryImpl::getAlignedSize() const
{
#if IS_USING_END_GUARD_BAND
  _verifyGuardBand();
#endif
  return m_AlignedSize;
}




// =======================================================================================
// === getBytes ==========================================================================
// =======================================================================================

char * cMemoryImpl::getBytes() throw(cNotAvailable)
{
  if (getSize() == 0) throw cNotAvailable(__FILE__, __LINE__, L"Attempt to get bytes from a zero sized cMemory");
  if (!_isCommited()) _commit();
  return (char *)m_AlignedCommitedBytes;
}




// =======================================================================================
// === _release ===========================================================================
// =======================================================================================

void cMemoryImpl::_release()
{
#if IS_USING_END_GUARD_BAND
  _verifyGuardBand();
#endif
  if (m_AlignedSize > 0)
  {
    if (m_Type == cMemory::eVirtual)
    {
      if (m_VirtualMemory)
      {
        if (VirtualFree(m_VirtualMemory, 0, MEM_DECOMMIT) == 0)
        {
          Char Tmp[256];
          wsprintf(Tmp, L"Thread %x: During _release - tried to VirtualFree(MEM_DECOMMIT) on %Iu bytes - last error %d\n", GetCurrentThreadId(), m_AlignedSize, GetLastError());
          QKERNELBASEVERIFYMSG(false, Tmp);
        }
        m_VirtualMemory = 0;
      }

      if (VirtualFree(m_ReservedVirtualMemoryHandle, 0, MEM_RELEASE) == 0)
      {
        Char Tmp[256];
        wsprintf(Tmp, L"Thread %x: During _release - tried to VirtualFree(MEM_RELEASE) on %Iu bytes - last error %d\n", GetCurrentThreadId(), m_AlignedSize, GetLastError());
        QKERNELBASEVERIFYMSG(false, Tmp);
      }
      m_ReservedVirtualMemoryHandle = 0;
    }
    else if (m_Type == cMemory::eHeap)
    {
      m_HeapMemory.clear();
    }
    else if (m_Type == cMemory::ePreallocated)
    {
      // Notify observer if we have one
      if (m_Observer.isValid())
        m_Observer->onRelease(m_PreallocatedMemory);
    }
    else if (m_Type == cMemory::eNamed)
    {
      UnmapViewOfFile(m_BufferBytes);
      CloseHandle(m_BufferHandle);
    }

    m_AlignedSize = 0;
  }
}




// =======================================================================================
// === _reserve ===========================================================================
// =======================================================================================

void cMemoryImpl::_reserve()
{
#if defined(_DEBUG)
  QKERNELBASEVERIFY(!_isCommited());
#endif

  if (!_isReserved())
  {
    if (m_Type == cMemory::eVirtual)
    {
      if (m_VirtualPageSize == 0)
      {
        SYSTEM_INFO SysInfo;
        GetSystemInfo(&SysInfo);
        m_VirtualPageSize = SysInfo.dwPageSize;
      }
      
      // VirtualAlloc will align allocation start and end to page boundaries regardless of requested alignment
      m_Alignment = m_VirtualPageSize;

      m_AlignedSize = m_UnalignedSize;
      m_AlignedSize += m_Alignment - (m_AlignedSize % m_Alignment); // round up size to alignment boundary
#if IS_USING_END_GUARD_BAND
      m_AlignedSize += kEndGuardBandNumBytes;
      m_AlignedSize += m_Alignment - (m_AlignedSize % m_Alignment); // round up size to alignment boundary
#endif

      m_ReservedVirtualMemoryHandle = VirtualAlloc(0, m_AlignedSize, MEM_RESERVE, PAGE_READWRITE);
      if (m_ReservedVirtualMemoryHandle == 0)
      {
        Char Tmp[256];
        wsprintf(Tmp, L"Thread %x: During reserve - tried to VirtualAlloc(MEM_RESERVE) on %Iu bytes - last error %d\n", GetCurrentThreadId(), m_AlignedSize, GetLastError());
        QKERNELBASEVERIFYMSG(m_ReservedVirtualMemoryHandle, Tmp);
      }
    }
    else if (m_Type == cMemory::eHeap)
    {
      // heap allocation will not necessarily start on allocation boundary (unlike virtual allocation)
      m_AlignedSize = m_UnalignedSize + m_Alignment - 1; // allow for worst case alignment of allocation start

      m_AlignedSize += m_Alignment - (m_AlignedSize % m_Alignment); // round up size to alignment boundary (or beyond it)
#if IS_USING_END_GUARD_BAND
      m_AlignedSize += kEndGuardBandNumBytes;
      m_AlignedSize += m_Alignment - (m_AlignedSize % m_Alignment); // round up size to alignment boundary (or beyond it)
#endif
    }
    else if (m_Type == cMemory::ePreallocated)
    {
      m_AlignedSize = m_UnalignedSize;
#if IS_USING_END_GUARD_BAND
      // No guard band when preallocated
#endif
    }
    else if (m_Type == cMemory::eNamed)
    {
      m_AlignedSize = m_UnalignedSize;
#if IS_USING_END_GUARD_BAND
      // No guard band when named
#endif
    }
  }
}




// =======================================================================================
// === _commit ============================================================================
// =======================================================================================

void cMemoryImpl::_commit()
{
#if defined(_DEBUG)
  QKERNELBASEVERIFY(_isReserved());
#endif
  if (!_isCommited())
  {
    void * CommitedBytes = 0;
    if (m_Type == cMemory::eVirtual)
    {
      m_VirtualMemory = VirtualAlloc(m_ReservedVirtualMemoryHandle, m_AlignedSize, MEM_COMMIT, PAGE_READWRITE);
      if (m_VirtualMemory == 0)
      {
        Char Tmp[256];
        wsprintf(Tmp, L"Thread %x: During _commit - tried to VirtualAlloc(MEM_COMMIT) on %Iu bytes - last error %d\n", GetCurrentThreadId(), m_AlignedSize, GetLastError());
        QKERNELBASEVERIFYMSG(m_VirtualMemory, Tmp);
      }
      CommitedBytes = m_VirtualMemory;
    }
    else if (m_Type == cMemory::eHeap)
    {
      m_HeapMemory.resize(m_AlignedSize);
      CommitedBytes = &m_HeapMemory[0];
    }
    else if (m_Type == cMemory::ePreallocated)
    {
      CommitedBytes = m_PreallocatedMemory;
    }
    else if (m_Type == cMemory::eNamed)
    {
      m_Name = cGuid::createUnique(L"").toString();
      m_BufferHandle = CreateFileMapping(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, m_AlignedSize, m_Name.c_str());
      m_BufferBytes = MapViewOfFile(m_BufferHandle, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
      CommitedBytes = m_BufferBytes;
    }

    // Align pointer
    unsigned char * const Unaligned = (unsigned char *)CommitedBytes;
    unsigned char * const RoundedUp = Unaligned + m_Alignment - 1;
    unsigned char * const Aligned   = RoundedUp - (((size_t)RoundedUp) % m_Alignment);
    m_AlignedCommitedBytes = Aligned;

    // Set guard band
#if IS_USING_END_GUARD_BAND
    if (m_Type != cMemory::ePreallocated)
    {
      (void)memset((unsigned char *)m_AlignedCommitedBytes + m_UnalignedSize, 0xf0, kEndGuardBandNumBytes);
    }
#endif
  }
}




// =======================================================================================
// === _verifyGuardBand ==================================================================
// =======================================================================================

#if IS_USING_END_GUARD_BAND
void cMemoryImpl::_verifyGuardBand() const
{
  if (_isCommited())
  {
    if (m_Type != cMemory::ePreallocated)
    {
      bool IsGuardBandValid = true;

      for (size_t i = 0; i < kEndGuardBandNumBytes; ++i)
      {
        if (((unsigned char *)m_AlignedCommitedBytes)[m_UnalignedSize + i] != 0xf0)
        {
          IsGuardBandValid = false;
          break;
        }
      }
      QKERNELBASEVERIFY(IsGuardBandValid);
    }
  }
}
#endif