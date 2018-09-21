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

//iPacket.h

#pragma once

using namespace vfs;

#include "..\common\transmitlist.h"

class iTXPacket
{
public:
  virtual ~iTXPacket() {}

  virtual void setReply(const tTransmitList &transmitList) = 0;

  //virtual bool Add(const SMART_TPE tpe) = 0;
  //virtual bool Add(const cPtr<cMemoryView> pMem) = 0;
  //virtual bool Add(const HANDLE h, const ULONG len, const LARGE_INTEGER &offset) = 0;

  virtual bool Send() = 0;
};

class iRXPacket
{
public:
  virtual ~iRXPacket() {}

  virtual cPtr<cMemoryView> getRXBuffer() = 0;
  virtual cPtr<iTXPacket>   getResponsePacket() = 0;
  virtual UINT_PTR getSessionID() = 0;
  virtual std::pair<ULONG, USHORT> getIpAddressAndPort() = 0;
};