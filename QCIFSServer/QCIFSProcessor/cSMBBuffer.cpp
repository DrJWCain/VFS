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

//cSMBBuffer.cpp

#include "stdafx.h"

#if CIFS 

#include "cSMBBuffer.h"
#include "logging.h"

///////////////////////////////////////////////////////////////////////////////////////
// cSMBBuffer /////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
extern tActivityMap ActivityQCIFSProcessors;

cSMBBuffer::cSMBBuffer(unsigned char* buffer, unsigned int size)
: Buffer(buffer), BufferSize(size)
{
  vfs::iActivity::Ptr activity = getActivity();
  cActivityScope as1(activity,kSMBBuffer);
}

PSMB_HEADER cSMBBuffer::header() const
{
  return Header;
}

///////////////////////////////////////////////////////////////////////////////////////
// logging ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

vfs::String cSMBBuffer::desc() const
{
  wchar_t buffer[400];
  wsprintf(buffer,
    L"%s%02x %08x F%02x F%04x K%x S%x "
    L"SN%x G%x T%x P%04x P%x U%x M%04x",
    Header->Flags&0x80?L"R":L"C",
    Header->Command,
    *((PDWORD)&(Header->ErrorClass)),
    Header->Flags,
    Header->Flags2,
    Header->Key,
    Header->Sid,
    Header->SequenceNumber,
    Header->Gid,
    Header->Tid,
    Header->Pid,
    Header->PidHigh,
    Header->Uid,
    Header->Mid);
  return vfs::String(buffer, wcslen(buffer));
}


///////////////////////////////////////////////////////////////////////////////////////
// blocks /////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

unsigned int cSMBBuffer::size() const
{
  unsigned int diff = (unsigned int)(Blocks.back().getBaseAddress() - (unsigned char*)Header);
  return diff + Blocks.back().getTotalSize();
}

unsigned char cSMBBuffer::numParams() const
{
  return Blocks.back().numParams();
}

unsigned char* cSMBBuffer::getParams(unsigned int size) const
{
  return Blocks.back().getParams(size);
}

PGENERIC_ANDX cSMBBuffer::getAndX() const
{
  return Blocks.back().getAndX();
}

unsigned short cSMBBuffer::numData() const
{
  return Blocks.back().numData();
}

unsigned char* cSMBBuffer::getData(unsigned short offset) const
{
  return Blocks.back().getData(offset);
}

#endif // CIFS 