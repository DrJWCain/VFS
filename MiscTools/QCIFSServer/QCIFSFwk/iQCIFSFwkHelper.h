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

//iQCIFSFwkHelper.h

#pragma once

typedef std::pair<unsigned int,vfs::cConstPtr<vfs::cMemoryView> > tFramePart;
typedef std::vector<tFramePart> tFrameVector;

class iQCIFSFwkHelper : public vfs::iManager
{
public:
  QDEFINE_SINGLETON_IFACE(iQCIFSFwkHelper, L"{166410D5-25B1-4bec-B4E0-25F542CC7715} //iQCIFSFwkHelper", vfs::iManager);

  virtual ~iQCIFSFwkHelper(){}

  virtual unsigned int getFrameVectorSize(const tFrameVector& vFrame) const = 0;

  virtual vfs::cPtr<vfs::cMemoryView> getBufferFromFrameVector(const tFrameVector& vFrame) const = 0;
  
  virtual vfs::cPtr<vfs::cMemoryView> getFile(const vfs::String& sFile) const = 0;

  virtual vfs::String getBaseName(const vfs::String& sFullName) const = 0;

  virtual void makeFrameVector(const vfs::cPtr<vfs::cMemoryView> pMem
    , tFrameVector& frameVector) const = 0;

  virtual bool lowMemory(const DWORD threshold) const = 0;

  virtual void flattenFrameVector(const tFrameVector& vFrame
    , std::vector<vfs::cConstPtr<vfs::cMemoryView> >& retVector
    , const unsigned int start
    , const unsigned int end) const = 0;
};