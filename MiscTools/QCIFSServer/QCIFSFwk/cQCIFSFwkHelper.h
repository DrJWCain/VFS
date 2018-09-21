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

//cQCIFSFwkHelper.h

#pragma once

#include "iqcifsfwkhelper.h"

class cQCIFSFwkHelper :  public iQCIFSFwkHelper, public vfs::cRefCount
{
public:
  QDEFINE_SINGLETON_IMPL(cQCIFSFwkHelper, L"{D324B1B6-A16A-4cd6-8CF0-2F9C9EBEE426} // cQCIFSFwkHelper", iQCIFSFwkHelper);

  cQCIFSFwkHelper(void);
  ~cQCIFSFwkHelper(void);

  //iManager
  void QAPI logSummary(void) {QTRACE((L"cQCIFSFwkHelper"));}
  void QAPI logDetailedSummary(void) {QTRACE((L"cQCIFSFwkHelper - QCIFS framework helper classes"));}

  //iQCIFSFwkHelper
  unsigned int getFrameVectorSize(const tFrameVector& vFrame) const;

  vfs::cPtr<vfs::cMemoryView> getBufferFromFrameVector(const tFrameVector& vFrame) const;
  
  vfs::cPtr<vfs::cMemoryView> getFile(const vfs::String& sFile) const;

  vfs::String getBaseName(const vfs::String& sFullName) const;

  void makeFrameVector(const vfs::cPtr<vfs::cMemoryView> pMem, tFrameVector& frameVector) const;

  bool lowMemory(const DWORD threshold) const;

  void flattenFrameVector(const tFrameVector& vFrame
    , std::vector<vfs::cConstPtr<vfs::cMemoryView> >& retVector
    , const unsigned int start
    , const unsigned int end) const;
};
