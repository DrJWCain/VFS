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

//cMemory.cpp

#include "stdafx.h"
#include "cMemory.h"

cAllocatedMemory::cAllocatedMemory(unsigned int numBytes) : NumBytes(numBytes)
{
  //PRINT(L"cAllocatedMemory::cAllocatedMemory()\n");
  Buffer = (unsigned char*)VirtualAlloc(0, NumBytes, MEM_COMMIT, PAGE_READWRITE);
}

cAllocatedMemory::~cAllocatedMemory()
{
  //PRINT(L"cAllocatedMemory::~cAllocatedMemory()\n");
  VirtualFree(Buffer,0,MEM_RELEASE);
}

unsigned char* cAllocatedMemory::getBytes()
{
  return Buffer;
}

unsigned int cAllocatedMemory::getSize()
{
  return NumBytes;
}


cMemoryView::cMemoryView(cPtr<cMemory> memory, unsigned int offset, unsigned int numBytes)
: Memory(memory), Offset(offset), NumBytes(numBytes)
{
}

cMemoryView::cMemoryView(cPtr<cMemoryView> memoryview)
: Memory(memoryview->Memory), Offset(memoryview->Offset), NumBytes(memoryview->NumBytes)
{
}

unsigned char* cMemoryView::getBytes()
{
  return Memory->getBytes() + Offset;
}

unsigned int cMemoryView::getSize()
{
  return NumBytes;
}

cPtr<cMemoryView> cMemoryView::range(unsigned int offset, unsigned int numBytes)
{
  return new cMemoryView(Memory, offset, numBytes);
}