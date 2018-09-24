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

// socket_routines.h
// stateless socket routines

#pragma once

static void setNetBIOSResponse(unsigned char* buffer, const unsigned int length)
{
  *(int*)buffer = htonl(length);
  *(unsigned char*)buffer = 0x00;
}

#if CIFS 
// strip NetBIOS header from packet if it is present
static unsigned char* getSmbBufferFromNetBIOS(unsigned char* buffer, unsigned int& length)
{
  if(0 == buffer[0])
  {
    length = ntohl( *(unsigned int *)buffer);
    if(length >= sizeof(SMB_HEADER))
      return buffer + 4;
  }
  return 0;
}
#endif // CIFS 

// strip NetBIOS header from packet if it is present
static std::pair<unsigned int, unsigned char*> getSmbBufferFromNetBIOS(unsigned char* buffer)
{
  if(0 == buffer[0])
      return std::pair<unsigned int, unsigned char*>(ntohl( *(unsigned int *)buffer), buffer + 4);
  return std::pair<unsigned int, unsigned char*>(0, 0);
}

static PSMB2_HEADER getSmb2(unsigned char* buffer, const int length)
{
  if(length >= sizeof(SMB2_HEADER))
  {
    PSMB2_HEADER pSmb2 = (PSMB2_HEADER)buffer;
    if(pSmb2 && SMB2_HEADER_PROTOCOL == pSmb2->ProtocolId)
      return pSmb2;
  }
  return 0;
}

static PSMB_HEADER getSmb(unsigned char* buffer, const int length)
{
  if(length >= sizeof(SMB_HEADER))
  {
    PSMB_HEADER pSmb = (PSMB_HEADER)buffer;
    if(pSmb && SMB_HEADER_PROTOCOL == *(PULONG)pSmb->Protocol)
      return pSmb;
  }
  return 0;
}
