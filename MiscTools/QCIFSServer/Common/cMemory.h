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

#include "iPointer.h"

///////////////////////////////////////////////////////////////////////////////

class cMemory
{
public:
  virtual ~cMemory() {}
  virtual unsigned char* getBytes() = 0;
  virtual unsigned int getSize() = 0;
};

class cAllocatedMemory : public cMemory, public cRefCount
{
  unsigned char* Buffer;
  unsigned int NumBytes;
public:
  cAllocatedMemory(unsigned int numBytes);
  ~cAllocatedMemory();
  virtual unsigned char* getBytes();
  virtual unsigned int getSize();
};

class cMemoryView : public cMemory, public cRefCount
{
  cPtr<cMemory> Memory;
  unsigned int Offset;
  unsigned int NumBytes;

public:
  cMemoryView(cPtr<cMemory> memory, unsigned int offset, unsigned int numBytes);
  cMemoryView(cPtr<cMemoryView> memoryview);
  
  virtual unsigned char* getBytes();
  virtual unsigned int getSize();
  virtual cPtr<cMemoryView> range(unsigned int offset, unsigned int numBytes);
};
