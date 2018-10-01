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

//cQCIFSFwkHelper.cpp

#include "StdAfx.h"

#include ".\cqcifsfwkhelper.h"

using namespace vfs;

cQCIFSFwkHelper::cQCIFSFwkHelper(void)
{
}

cQCIFSFwkHelper::~cQCIFSFwkHelper(void)
{
}

unsigned int cQCIFSFwkHelper::getFrameVectorSize(const tFrameVector& vFrame) const
{
  unsigned int size = 0;
  for(tFrameVector::const_iterator it = vFrame.begin(); it != vFrame.end(); ++it)
  {
    if (it->second.isValid())
    {
      unsigned int end = it->first + it->second->getSize();
      size = max(size,end);
    }
  }
  return size;
}

cPtr<cMemoryView> cQCIFSFwkHelper::getBufferFromFrameVector(const tFrameVector& vFrame) const
{
  if(vFrame.empty())
    return cPtr<cMemoryView>();

  if(1 == vFrame.size() && 0 == vFrame.front().first)
  {
    return ptr_const_cast<cPtr<cMemoryView> >(vFrame.front().second);
  }

  unsigned int bufferSize = iQCIFSFwkHelper::singleton().getFrameVectorSize(vFrame);
  if (bufferSize == 0)
    return cPtr<cMemoryView>();

  cPtr<cMemoryView> bufferSrc = new cMemoryView(new cMemory((size_t)bufferSize, cMemory::eVirtual));

  {
    char* buffer = bufferSrc->getBytes();
    for(tFrameVector::const_iterator it = vFrame.begin(); it != vFrame.end(); ++it)
    {
      if (it->second.isValid())
        memcpy(buffer + it->first, it->second->getConstBytes(), it->second->getSize());
    }
  }

  return bufferSrc;
}

cPtr<cMemoryView> cQCIFSFwkHelper::getFile(const String& sFile) const
{
  const HANDLE hDesktopINI = CreateFile(sFile.c_str()
    , GENERIC_READ
    , FILE_SHARE_READ
    , NULL
    , OPEN_EXISTING
    , 0
    , NULL);

  if (INVALID_HANDLE_VALUE == hDesktopINI)
    return cPtr<cMemoryView>();

  BY_HANDLE_FILE_INFORMATION bhi;
  GetFileInformationByHandle(hDesktopINI, &bhi);

  cPtr<cMemoryView> pMem;
  if (bhi.nFileSizeLow > 0)
  {
    pMem = new cMemoryView(new cMemory((size_t)bhi.nFileSizeLow, cMemory::eVirtual));
    DWORD dw;
    ReadFile(hDesktopINI, pMem->getBytes(), bhi.nFileSizeLow, &dw, NULL);
  }

  CloseHandle(hDesktopINI);

  return pMem;
}

String cQCIFSFwkHelper::getBaseName(const String& sFullName) const
{
  const size_t findPos = sFullName.find_last_of(L"\\");
  if (String::npos == findPos)
    return sFullName;

  return sFullName.substr(findPos+1);
}

void cQCIFSFwkHelper::makeFrameVector(const cPtr<cMemoryView> pMem, tFrameVector& frameVector) const
{
  if (pMem.isValid())
    frameVector.push_back(tFrameVector::value_type(0, pMem));
}

bool cQCIFSFwkHelper::lowMemory(const DWORD threshold) const
{
  MEMORYSTATUSEX val;
  val.dwLength = sizeof(val);
  GlobalMemoryStatusEx(&val);
  return val.dwMemoryLoad > threshold;
}

static void order(const tFrameVector& vFrame
  , std::vector<cConstPtr<cMemoryView> >& retVector
  , const unsigned int startPosition
  , const unsigned int endPosition)
{
  if (startPosition >= endPosition)
    return;

  //retVector.reserve(vFrame.size());
  for (tFrameVector::const_iterator cit = vFrame.begin()
    ; vFrame.end() != cit
    ; ++cit)
  {
    const tFramePart& rFrame = *cit;
    if(rFrame.second->getSize() && rFrame.first == startPosition)
    {
      retVector.push_back(rFrame.second);
      order(vFrame, retVector, startPosition + rFrame.second->getSize(), endPosition);
    }
  }
}

static void splitChunks(std::list<tFramePart>& chunkList, const int start, const int end)
{
  // check through chunks to see if the next one intersects any previous chunks
  for (std::list<tFramePart>::iterator it = chunkList.begin()
    ; chunkList.end() != it
    ; )
  {
    bool moved = false;
    const tFramePart& rFrame = *it;
    const int chunkEnd = rFrame.first + rFrame.second->getSize();
    const int diffIn = start - rFrame.first;
    const int diffOut = chunkEnd - end;
    if (diffIn <= 0)  // front of chunk needs to be chopped off
    { 
      if (diffOut <= 0)
      {// chunk lies within existing chunk => erase from list
        chunkList.erase(it);
        it = chunkList.begin();
        moved = true;
      }
      else if (static_cast<unsigned int>(diffOut) < rFrame.second->getSize())
      {
        const cConstPtr<cMemoryView> pMem = rFrame.second->lastConst((size_t)diffOut);
        chunkList.erase(it);
        chunkList.push_back(tFrameVector::value_type(end, pMem));
        it = chunkList.begin();
        moved = true;
      }
    }
    else
    {
      const cConstPtr<cMemoryView> pMem = rFrame.second;
      if (static_cast<unsigned int>(diffIn) < pMem->getSize())
      {
        const cConstPtr<cMemoryView> pTrimMem = rFrame.second->firstConst((size_t)diffIn);
        chunkList.erase(it);
        chunkList.push_back(tFrameVector::value_type(rFrame.first, pTrimMem));
        it = chunkList.begin();
        moved = true;
      }
      if (diffOut > 0 && static_cast<unsigned int>(diffOut) < pMem->getSize())
      {
        const cConstPtr<cMemoryView> pTrimMem = rFrame.second->lastConst((size_t)diffOut);
        chunkList.erase(it);
        chunkList.push_back(tFrameVector::value_type(end, pTrimMem));
        it = chunkList.begin();
        moved = true;
      }
    }
    if(!moved)
      ++it;
  }
}

//deduplicate chunks in return vector
static void deduplicate(const tFrameVector& vFrame, tFrameVector& retVector)
{
  // go backwards through the vFrame, trimming each mem block
  for (tFrameVector::const_reverse_iterator cit = vFrame.rbegin()
    ; vFrame.rend() != cit
    ; ++cit)
  {
    const tFramePart& rFrame = *cit;
    std::list<tFramePart> chunkList;
    chunkList.push_back(rFrame);
    for (tFrameVector::const_iterator cit2 = retVector.begin()
      ; retVector.end() != cit2
      ; ++cit2)
    {
      const tFramePart& rFrame2 = *cit2;
      splitChunks(chunkList, rFrame2.first, rFrame2.first + rFrame2.second->getSize());
    }

    // put non-intersecting chunks into return vector
    for (std::list<tFramePart>::const_iterator cit2 = chunkList.begin()
      ; chunkList.end() != cit2
      ; ++cit2)
      retVector.push_back(*cit2);
  }
}

//split vector at boundary points and store in return vector
static void trimBoundaries(const tFrameVector& vFrame
  , tFrameVector& boundedFrame
  , const unsigned int start
  , const unsigned int end)
{
  for (tFrameVector::const_iterator cit = vFrame.begin()
    ; vFrame.end() != cit
    ; ++cit)
  {
    const tFramePart& rFrame = *cit;
    const cConstPtr<cMemoryView> pMem = rFrame.second;
    if (pMem.isValid())
    {
      const int frameEnd = (pMem->getSize() + rFrame.first);
      const int diffIn = start - rFrame.first;
      const int diffOut = frameEnd - end;
      if (diffIn <= 0)
      {
        if (diffOut <= 0)  // chunk lies completely within boundaries
          boundedFrame.push_back(tFrameVector::value_type(rFrame.first, pMem));
        else if (static_cast<unsigned int>(diffOut) < pMem->getSize())
          boundedFrame.push_back(tFrameVector::value_type(rFrame.first
            , pMem->firstConst((size_t)end - rFrame.first)));
      }
      else 
      {
        const unsigned int memSize = pMem->getSize();
        if (static_cast<unsigned int>(diffIn) < memSize)
        {
          const cConstPtr<cMemoryView> pRangeMem = pMem->rangeConst((size_t)diffIn
            , (size_t)memSize - diffIn - (diffOut > 0 ? diffOut : 0));
          boundedFrame.push_back(tFrameVector::value_type(start, pRangeMem));
        }
      }
    }
  }
}

void cQCIFSFwkHelper::flattenFrameVector(const tFrameVector& vFrame
  , std::vector<cConstPtr<cMemoryView> >& retVector
  , const unsigned int start
  , const unsigned int end) const
{
  //QSOS((L"%S, %d-%d", __FUNCTION__, start, end));
  if (start >= end)
    return;

  tFrameVector boundedFrame;
  trimBoundaries(vFrame, boundedFrame, start, end);

  tFrameVector deduplicated;
  deduplicate(boundedFrame, deduplicated);

  //put into sequence
  order(deduplicated, retVector, start, end);
}