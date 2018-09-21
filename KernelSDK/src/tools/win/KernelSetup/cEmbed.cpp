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

#include "cEmbed.h"
#include "EmbedHelp.h"
#if defined(EMBED_COMPRESSION)
#include <ZLibHelp.h>
#endif

// --- Use Namespace ---------------------------------------------------------------------

using namespace std;
using namespace vfs;




// =======================================================================================
// === Construction/Destruction ==========================================================
// =======================================================================================

cEmbed::cEmbed(const String & Filename) :
  m_Filename(Filename)
{
  cMemoryView::Ptr tmp = new cMemoryView(new cMemory((size_t)kMarkerNumBytes, cMemory::eHeap));
  {
    unsigned int * p = (unsigned int *)tmp->getBytes();
    unsigned int i = tmp->getSize() / sizeof(kMarkerLong);
    while (i > 0) { *p++ = kMarkerLong; i--; }
  }
  m_Marker = tmp;
}

// ---------------------------------------------------------------------------------------

cEmbed::~cEmbed()
{
}




// =======================================================================================
// === _initEmbed/_writeHeader ===========================================================
// =======================================================================================

void cEmbed::_initEmbed()
{
  if (!m_File.isValid())
  {
#if !defined(NO_KERNELEXEC)
    m_File = iFileManager::singleton().openFile(m_Filename, fFileAccess_Write);
#else
    m_File = new cFile(m_Filename, fFileAccess_Write, false /* temporary? */);
#endif
    m_File->seek(m_File->getSize()); // Seek to end of file
  }
}

// ---------------------------------------------------------------------------------------

void cEmbed::_initExtract(const bool IsShowingDebug)
{
  if (!m_File.isValid())
  {
#if !defined(NO_KERNELEXEC)
    m_File = iFileManager::singleton().openFile(m_Filename, fFileAccess_Read);
    if (IsShowingDebug) QMSG((0, L"Reading %dK file \"%s\"", m_File->getSize() / 1024, m_Filename.c_str()));
    m_Buffer = new cMemoryView(m_File->read());
#else
    m_File = new cFile(m_Filename, fFileAccess_Read, false /* temporary? */);
    m_Buffer = new cMemoryView(m_File->read(cMemory::eHeap));
#endif
  }
}

// ---------------------------------------------------------------------------------------

void cEmbed::_writeHeader(const String & ChunkId, const unsigned int CompressedChunkSize, const unsigned int UncompressedChunkSize, const bool IsShowingDebug)
{
  m_File->write(m_Marker);

  StringStream Header;
  Header << L"(\"" << ChunkId << L"\" " << CompressedChunkSize << L" " << UncompressedChunkSize << L")";
#if !defined(NO_KERNELEXEC)
  if (IsShowingDebug) QMSG((0, L"Writing file header:  %s", Header.str().c_str()));
#endif
  m_File->write((UInt64)Header.str().size() * (UInt64)sizeof(Char), (char *)Header.str().c_str());
}




// =======================================================================================
// === embedData/embedFile ===============================================================
// =======================================================================================

void cEmbed::_embedData(const String & ChunkId, cMemoryView::Ptr ChunkData, const unsigned int UncompressedSize, const bool IsShowingDebug) throw(cFileError)
{
  _initEmbed();
  _writeHeader(ChunkId, ChunkData->getSize(), UncompressedSize, IsShowingDebug);
  m_File->write(ChunkData);
}

// ---------------------------------------------------------------------------------------

void cEmbed::embedData(const String & ChunkId, cMemoryView::Ptr ChunkData, const bool IsShowingDebug) throw(cFileError)
{
  _embedData(ChunkId, ChunkData, ChunkData->getSize(), IsShowingDebug);
}

// ---------------------------------------------------------------------------------------

void cEmbed::embedString(const String & ChunkId, const String & ChunkData, const bool IsShowingDebug) throw(cFileError)
{
#if !defined(NO_KERNELEXEC)
  if (ChunkData.size() == 0) QFATAL((__FILE__, __LINE__, L"Can't embed data into zero sized file \"%s\"", m_Filename.c_str()));
#endif
  cMemoryView::Ptr TmpChunkBuffer = new cMemoryView(new cMemory(ChunkData.size() * sizeof(Char), cMemory::eHeap));
  memcpy(TmpChunkBuffer->getBytes(), ChunkData.c_str(), TmpChunkBuffer->getSize());
  _encrypt(TmpChunkBuffer);
  embedData(ChunkId, TmpChunkBuffer, IsShowingDebug);
}

// ---------------------------------------------------------------------------------------

#if defined(EMBED_COMPRESSION)

void cEmbed::embedFileWithCompression(const String & ChunkSrcFilename, const String & ChunkDstFilename, const bool IsShowingDebug) throw(cFileError)
{
  #if !defined(NO_KERNELEXEC)
    iFile::Ptr SrcFile = iFileManager::singleton().openFile(ChunkSrcFilename, fFileAccess_Read);
  #else
    cFile::Ptr SrcFile = new cFile(ChunkSrcFilename, fFileAccess_Read);
  #endif
  cMemoryView::Ptr Compressed = _compress(new cMemoryView(SrcFile->read()), 9 /* best compression level */);
  if (IsShowingDebug) QMSG((0, L"Compressed \"%s\" from %Iu bytes to %Iu", ChunkSrcFilename.c_str(),  SrcFile->getSize(), Compressed->getSize()));
  _embedData(ChunkDstFilename, Compressed, (unsigned int)SrcFile->getSize(), IsShowingDebug);
}

#endif

// ---------------------------------------------------------------------------------------

void cEmbed::embedFile(const String & ChunkFilename, const bool IsShowingDebug) throw(cFileError)
{
  _initEmbed();

  #if !defined(NO_KERNELEXEC)
    iFile::Ptr SrcFile = iFileManager::singleton().openFile(ChunkFilename, fFileAccess_Read);
  #else
    cFile::Ptr SrcFile = new cFile(ChunkFilename, fFileAccess_Read, false /* temporary? */);
  #endif

  // Grow copy buffer if necesseary
  const size_t MinBufferSize = (size_t)Min<UInt64>(512UI64 * 1024UI64, SrcFile->getSize());
  if (!m_Buffer.isValid() || MinBufferSize > m_Buffer->getSize())
  {
    m_Buffer.invalidate(); // Dealloc first to avoid old and new in existence at same time
    m_Buffer = new cMemoryView(new cMemory(MinBufferSize, cMemory::eFastIO));
  }

  _writeHeader(SrcFile->getName() + SrcFile->getExtension(), (unsigned int)SrcFile->getSize(), (unsigned int)SrcFile->getSize(), IsShowingDebug);

  // Copy src to dst file in chunks of 512K
  UInt64 NumBytesToCopy = SrcFile->getSize();
  while (NumBytesToCopy > (UInt64)m_Buffer->getSize())
  {
    SrcFile->read(m_Buffer);
    m_File->write(m_Buffer);
    NumBytesToCopy -= (UInt64)m_Buffer->getSize();
  }
  if (NumBytesToCopy > 0UI64)
  {
    cMemoryView::Ptr SmallerCopyBuffer = m_Buffer->first((size_t)NumBytesToCopy);
    SrcFile->read(SmallerCopyBuffer);
    m_File->write(SmallerCopyBuffer);
  }
}




// =======================================================================================
// === extractData =======================================================================
// =======================================================================================

cMemoryView::Ptr cEmbed::extractData(const String & ChunkIdToFind, const bool IsShowingDebug) throw(cBadFormat, cFileError)
{
  _initExtract(IsShowingDebug);

  const char * p = m_Buffer->getBytes();

  {
    const unsigned int Offset = _findMarker(m_Buffer);
#if !defined(NO_KERNELEXEC)
    if (IsShowingDebug) QMSG((0, L"Marker found at %d", Offset));
#endif
    p += Offset + kMarkerNumBytes;
  }

  while (p < m_Buffer->getBytes() + m_Buffer->getSize())
  {
    Char          ChunkId[256] = L"";
    unsigned int  CompressedChunkSize = 0, UncompressedChunkSize = 0;
    {
      const unsigned int NumFieldsConverted = swscanf((Char *)p, L"(\"%255[^\"]\" %d %d)", &ChunkId, &CompressedChunkSize, &UncompressedChunkSize);
      if (NumFieldsConverted < 2) throw cBadFormat(__FILE__, __LINE__, L"Malformed file header (only %d fields found)", NumFieldsConverted);
    }
#if !defined(NO_KERNELEXEC)
    QMSG((0, L"Extracting: \"%s\" (%dK)", ChunkId, CompressedChunkSize / 1024));
#endif

    while (*(Char *)p != L')')
    {
      p += sizeof(Char);
    }
    p += sizeof(Char);

    if (ChunkIdToFind == ChunkId)
    {
      return m_Buffer->range((size_t)((Int64)p - (Int64)m_Buffer->getBytes()), (size_t)CompressedChunkSize);
    }

    p += CompressedChunkSize + kMarkerNumBytes;
  }

  throw cNotFound(__FILE__, __LINE__, L"Chunk \"%s\" not found", ChunkIdToFind.c_str());
}




// =======================================================================================
// === _encrypt/_decrypt =================================================================
// =======================================================================================

void cEmbed::_encrypt(cMemoryView::Ptr Data)
{
  unsigned char * p = (unsigned char *)Data->getBytes();
  const unsigned int c = Data->getSize();
  for (unsigned int i = 0; i < c; ++i)
  {
    p[i] = ~p[i];
  }
}

// ---------------------------------------------------------------------------------------

void cEmbed::_decrypt(cMemoryView::Ptr Data)
{
  unsigned char * p = (unsigned char *)Data->getBytes();
  const unsigned int c = Data->getSize();
  for (unsigned int i = 0; i < c; ++i)
  {
    p[i] = ~p[i];
  }
}




// =======================================================================================
// === extractString =====================================================================
// =======================================================================================

String cEmbed::extractString(const String & ChunkId, const bool IsShowingDebug) throw(cBadFormat, cFileError)
{
  cMemoryView::Ptr ChunkFound = extractData(ChunkId, IsShowingDebug);
  _decrypt(ChunkFound);
  return String((Char *)ChunkFound->getBytes(), ChunkFound->getSize() / sizeof(Char));
}
