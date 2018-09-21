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

//cSMB2Buffer.cpp

#include "stdafx.h"
#include "cSMB2Buffer.h"

extern tActivityMap ActivityQCIFSProcessors;

///////////////////////////////////////////////////////////////////////////////////////
// cSMB2Buffer ////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

cSMB2Buffer::cSMB2Buffer(unsigned char* buffer, unsigned int size)
: Buffer(buffer), BufferSize(size), ParamsSize(0)
{
  vfs::iActivity::Ptr activity = ActivityQCIFSProcessors.find(GetCurrentThreadId())->second;
  cActivityScope as1(activity,kSMBBuffer);
}

PSMB2_HEADER cSMB2Buffer::header() const
{
  return Header;
}

///////////////////////////////////////////////////////////////////////////////////////
// blocks /////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

unsigned int cSMB2Buffer::size() const
{
  return ParamsSize + sizeof(SMB2_HEADER);
}
