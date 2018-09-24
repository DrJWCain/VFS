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

//cSMBBuffer.h

#pragma once

#if CIFS 
#include "mycifs.h"
#include "cSMBBlock.h"

class cSMBBuffer
{
protected:
  unsigned char* Buffer;
  unsigned int BufferSize;

  typedef std::vector<cSMBBlock> tBlocks;
  mutable tBlocks Blocks;

public:
  PSMB_HEADER Header;

  unsigned char numParams() const;
  unsigned char* getParams(unsigned int size) const;
  PGENERIC_ANDX getAndX() const;
  unsigned short numData() const;
  unsigned char* getData(unsigned short offset) const;

protected:
  cSMBBuffer(unsigned char* buffer, unsigned int size);

public:
  vfs::String desc() const;

  unsigned int size() const;
  PSMB_HEADER header() const;
};

#endif // CIFS 