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

//cSMBBlock.cpp

#include "stdafx.h"

#if CIFS 

#include "mycifs.h"
#include "cSMBBlock.h"
#include "logging.h"

using namespace std;


// size that the WordCount, ByteCount and Buffer fields take in header structs
const static UINT SIZE_WORDCOUNT = sizeof(UCHAR);
const static UINT SIZE_BYTECOUNT_BUFFER = sizeof(USHORT) + sizeof(UCHAR);
const static UINT SIZE_WORDCOUNT_BYTECOUNT_BUFFER = SIZE_WORDCOUNT + SIZE_BYTECOUNT_BUFFER;

///////////////////////////////////////////////////////////////////////////////////////
// cSMBBlock //////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

cSMBBlock::cSMBBlock() : Parameters(0), Data(0), ExtraData(0)
{
}

char* cSMBBlock::dump(char* buffer) const
{
  buffer = dumpData(buffer, Parameters, numParams() + sizeof(unsigned char));
  buffer = dumpData(buffer, Data, numData() + sizeof(unsigned short));
  return buffer;
}

unsigned char cSMBBlock::numParams() const
{
  return (*Parameters)*sizeof(unsigned short);
}

unsigned char* cSMBBlock::getParams(unsigned int size) const
{
  size -= SIZE_WORDCOUNT_BYTECOUNT_BUFFER;
  if(size > numParams())
    throw exception("cSMBBlock::getParams()");
  return Parameters;
}

void cSMBBlock::allocParams(unsigned char size)
{
  size -= SIZE_WORDCOUNT_BYTECOUNT_BUFFER;
  unsigned char* newData = Data+size;
  memmove(newData,Data,numData()+sizeof(unsigned short));
  Data = newData;
  *Parameters += size/2;
}

unsigned short cSMBBlock::numData() const
{
  return *(unsigned short*)Data + ExtraData;
}

unsigned char* cSMBBlock::getData(unsigned short offset) const
{
  return Data + offset;
}

void cSMBBlock::addData(const USHORT size)
{
  *(PUSHORT)Data += size;
}

PGENERIC_ANDX cSMBBlock::getAndX() const
{
  return (PGENERIC_ANDX)(getParams(4));
}

unsigned char* cSMBBlock::getBaseAddress() const
{
  return Parameters;
}

unsigned int cSMBBlock::getTotalSize() const
{
  return numParams() + numData() + sizeof(unsigned short) + sizeof(unsigned char);
}

unsigned int cSMBBlock::read(unsigned char* buffer, unsigned int size)
{
  Parameters = buffer;
  unsigned int len = (*Parameters)*sizeof(unsigned short) + sizeof(unsigned char);
  if(len > size)
  {
    QSOS((L"cSMBBlock::read () - Parameter block longer than expected packet size."));
  }

  Data = Parameters + len;
  len += *(unsigned short*)Data + sizeof(unsigned short);
  if(len > size)
  {
    QSOS((L"cSMBBlock::read () - Data block longer than expected packet size."));
  }

  return len;
}

void cSMBBlock::create(unsigned char* buffer)
{
  Parameters = buffer;
  clearBlock();
}

void cSMBBlock::clearBlock()
{
  *Parameters = 0;

  Data = Parameters + sizeof(unsigned char);
  *(unsigned short*)Data = 0;
}

//allows us to add variable length SetupData fields in Transaction style commands
void cSMBBlock::nudgeData(const PUCHAR pSetupData, const UCHAR nudgeAmount)
{
  USHORT dataTotal = *Data;
  memcpy(Data, pSetupData, nudgeAmount*sizeof(USHORT));
  Data += (nudgeAmount*sizeof(USHORT));
  *Data = static_cast<unsigned char>(dataTotal);
}

#endif // CIFS 