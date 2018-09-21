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

//cTXPacket.h

#pragma once

#include "ipacket.h"
#include "..\common\transmitlist.h"

class cSocketServer;

class cTXPacket : public iTXPacket, public cRefCount
{
public:
  cTXPacket(const SOCKET sock, cSocketServer *pSocketServer);
  cTXPacket(const cTXPacket *pOther);
  ~cTXPacket(void);

  void setReply(const tTransmitList &transmitList);
  //bool Add(const SMART_TPE tpe);
  //bool Add(const cPtr<cMemoryView> pMem);
  //bool Add(const HANDLE h, const ULONG len, const LARGE_INTEGER &offset);
  bool Send();

protected:
  const SOCKET m_socket;
  tTransmitList m_TransmitList;
  //std::set<cPtr<cMemoryView> > m_MemReference;
  cSocketServer *m_pSocketServer;
};