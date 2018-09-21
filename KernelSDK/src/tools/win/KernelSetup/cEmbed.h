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

#include <QKernelBase.h>
#if defined(NO_KERNELEXEC)
#include "..\KernelSetupLoader\cFile.h"
#endif

// --- Use Namespace ---------------------------------------------------------------------

using namespace std;
using namespace vfs;




// =======================================================================================
// === cEmbed ============================================================================
// =======================================================================================

class cEmbed :  public cRefCount
{
  friend class cSetupLoaderReader;

  public :

    QDEFINE_SMARTPTR(cEmbed);
    
    // cEmbed
    cEmbed(const String & Filename) throw(cFileError);
    virtual ~cEmbed();
    void embedData(const String & ChunkId, cMemoryView::Ptr ChunkData, const bool IsShowingDebug) throw(cFileError);
    void embedString(const String & ChunkId, const String & ChunkData, const bool IsShowingDebug) throw(cFileError);
    void embedFile(const String & ChunkFilename, const bool IsShowingDebug) throw(cFileError);
    cMemoryView::Ptr extractData(const String & ChunkId, const bool IsShowingDebug) throw(cBadFormat, cFileError);
    String extractString(const String & ChunkId, const bool IsShowingDebug) throw(cBadFormat, cFileError);
#if defined(EMBED_COMPRESSION)
    void embedFileWithCompression(const String & ChunkSrcFilename, const String & ChunkDstFilename, const bool IsShowingDebug) throw(cFileError);
#endif

  private :
  
    const String          m_Filename;
#if defined(NO_KERNELEXEC)
    cFile::Ptr            m_File;
#else
    iFile::Ptr            m_File;
#endif
    cMemoryView::ConstPtr m_Marker;
    cMemoryView::Ptr      m_Buffer;

    void _initEmbed();
    void _initExtract(const bool IsShowingDebug);
    void _writeHeader(const String & ChunkId, const unsigned int CompressedChunkSize, const unsigned int UncompressedChunkSize, const bool IsShowingDebug);
    void _embedData(const String & ChunkId, cMemoryView::Ptr ChunkData, const unsigned int UncompressedSize, const bool IsShowingDebug) throw(cFileError);
    void _encrypt(cMemoryView::Ptr Data);
    void _decrypt(cMemoryView::Ptr Data);
};
