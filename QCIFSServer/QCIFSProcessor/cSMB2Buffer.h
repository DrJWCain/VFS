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

//cSMB2Buffer.h

#pragma once

#include "mycifs.h"

class cSMB2Buffer
{
  unsigned int ParamsSize;
protected:
  unsigned char* Buffer;
  unsigned int BufferSize;
  PSMB2_HEADER Header;
  mutable PSMB2_HEADER pCurChainHeader;

  cSMB2Buffer(const std::pair<unsigned int, unsigned char*>& rPair)
  : ParamsSize(sizeof(SMB2_HEADER))
  , BufferSize(rPair.first)
  , Buffer(rPair.second)
  , Header(reinterpret_cast<PSMB2_HEADER>(Buffer))
  , pCurChainHeader(Header)
  {}

  void incParams(unsigned int size) {ParamsSize += size;}

  template <typename T>
  T* overrideData()
  {
    ParamsSize = reinterpret_cast<PUCHAR>(pCurChainHeader)
      - reinterpret_cast<PUCHAR>(Header)
      + sizeof(SMB2_HEADER)
      + sizeof(T);
    return reinterpret_cast<T*>(pCurChainHeader+1);
  }

public:
  template <typename T>
  T getParams() const
  {
    return reinterpret_cast<T>(reinterpret_cast<PUCHAR>(header()) + sizeof(SMB2_HEADER));
  }
  unsigned int size() const { return ParamsSize; }
  PSMB2_HEADER header() const { return pCurChainHeader;}
  PSMB2_ASYNC_Header async_header() const { return reinterpret_cast<PSMB2_ASYNC_Header>(pCurChainHeader);}
  inline ULONG TreeID() const
  {
    return (SMB2_FLAGS_RELATED_OPERATIONS == (pCurChainHeader->Flags & SMB2_FLAGS_RELATED_OPERATIONS))
      && (-1 == static_cast<LONG>(pCurChainHeader->TreeId))
        ? Header->TreeId
        : pCurChainHeader->TreeId;
  }

  inline ULONGLONG SessionID() const
  {
    return (SMB2_FLAGS_RELATED_OPERATIONS == (pCurChainHeader->Flags & SMB2_FLAGS_RELATED_OPERATIONS))
      && (-1 == static_cast<LONGLONG>(pCurChainHeader->SessionId))
        ? Header->SessionId
        : pCurChainHeader->SessionId;
  }

  bool haveChainedHeader() const
  {
    if (pCurChainHeader->NextCommand)
    {
      pCurChainHeader = reinterpret_cast<PSMB2_HEADER>(
        reinterpret_cast<PUCHAR>(pCurChainHeader) + pCurChainHeader->NextCommand);
      return true;
    }
    return false;
  }
};
