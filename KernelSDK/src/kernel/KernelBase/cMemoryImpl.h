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
#include "cFixedSizeAllocator.h"

// --- Start of Namespace ----------------------------------------------------------------

namespace vfs
{




// =======================================================================================
// === kGuardBandNumBytes ================================================================
// =======================================================================================

#if defined(_DEBUG)
  #define IS_USING_END_GUARD_BAND 0
  const size_t kEndGuardBandNumBytes = 16;
#endif




// =======================================================================================
// === cMemoryImpl =======================================================================
// =======================================================================================

/*
      .            .
      .            .
      .            .
      |            |
      |            |
      |            |
      +------------+ start of allocation        --+
      |  Alignment |                              |
      |   Buffer   |                              |
      +------------+ m_AlignedCommitedBytes       |                    --+
      |            |                              |                      |
      |            |                              |                      |
      |   Data     |                              +-- m_AlignedSize      +-- m_UnalignedSize
      |            |                              |                      |
      |            |                              |                      |
      +------------+                              |                    --+
      | Guard Band |                              |
      +------------+                              |
      |  Alignment |                              |
      |   Buffer   |                              |
      +------------+ end of allocation          --+
      |            |
      |            |
      |            |
      .            .
      .            .
      .            .
*/

class cMemoryImpl : public cRefCount
{
  public :

    cMemoryImpl(const size_t NumBytes, const enum cMemory::eType TypeOfMemory, const size_t ByteAlignment) throw(cNoRoom);
    cMemoryImpl(char * const BytesToUse, const size_t NumBytes, const size_t Alignment, cPtr<iMemoryObserver> Observer);
    ~cMemoryImpl();
    void         resize(const size_t NewSize) throw(cNoRoom, cNotSupported);
    size_t       getSize() const;
    size_t       getAlignedSize() const;
    char *       getBytes() throw(cNotAvailable);
    cMemory::eType getType() const;
    size_t       getAlignment() const;
    String       getName() const;

  private :
    // Disallowed
    cMemoryImpl(const not_size_t NumBytes, const enum cMemory::eType TypeOfMemory, const not_size_t ByteAlignment) throw(cNoRoom);
    cMemoryImpl(char * const BytesToUse, const not_size_t NumBytes, const not_size_t Alignment, cPtr<iMemoryObserver> Observer);
    void resize(const not_size_t NewSize) throw(cNoRoom, cNotSupported);

    unsigned int          m_ClassVersion;    

    const cMemory::eType  m_Type;
    size_t                m_Alignment;

    size_t                m_UnalignedSize;
    size_t                m_AlignedSize;

    char * const          m_PreallocatedMemory;
    cPtr<iMemoryObserver> m_Observer;

    std::vector<char>     m_HeapMemory;

    mutable void *        m_VirtualMemory;
    mutable void *        m_ReservedVirtualMemoryHandle;
    static size_t         m_VirtualPageSize;

    void *                m_AlignedCommitedBytes;

    String                m_Name;
    HANDLE                m_BufferHandle;
    LPVOID                m_BufferBytes;

    void _reserve();
    void _commit();
    void _release();
    inline bool _isCommited() const { return m_AlignedCommitedBytes != 0; }
    inline bool _isReserved() const { return m_AlignedSize != 0; }
#if IS_USING_END_GUARD_BAND
    void _verifyGuardBand() const;
#endif
};




// --- End of Namespace ------------------------------------------------------------------

}
