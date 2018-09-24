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

//cSMBBlock.h

#pragma once

#include "mycifs.h"

#if CIFS 

class cSMBBlock
{
  unsigned char* Parameters;
  unsigned char* Data;
  int ExtraData;
public:
  cSMBBlock();
  char* dump(char* buffer) const;

  unsigned char numParams() const;
  unsigned char* getParams(unsigned int size) const;
  void allocParams(unsigned char size);

  unsigned short numData() const;
  unsigned char* getData(unsigned short offset) const;
  void addData(const USHORT size);

  unsigned char* getBaseAddress() const;
  unsigned int getTotalSize() const;

  PGENERIC_ANDX getAndX() const;

  unsigned int read(unsigned char* buffer, unsigned int size);
  void create(unsigned char* buffer);

  void clearBlock();

  void nudgeData(const PUCHAR pSetupData, const UCHAR nudgeAmount);
  void addExtraData(int i) {ExtraData +=i;}
};

#endif // CIFS 