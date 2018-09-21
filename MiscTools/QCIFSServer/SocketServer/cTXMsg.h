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

//cTXMsg.h

#pragma once

#include "cTXPacket.h"

#include "..\common\iCompletionMsg.h"

class cTXMsg : public iCompletionMsg, public cTXPacket
{
public:
  cTXMsg(cTXPacket *pPacket);
  ~cTXMsg(void);

  void deliver(ULONG_PTR pCompletionKey, const DWORD dwBytes);
  void err(ULONG_PTR pCompletionKey, const DWORD err);

  int                       retries()         {return m_nRetries++;}
  int                       numPackets()      {return checked_static_cast<int>(m_TransmitList.size());}
  WSABUF                   *getWSABuf();
  SOCKET                    getSocket()       {return m_socket;}
  tTransmitList             getTransmitList() {return m_TransmitList;}
  LPTRANSMIT_FILE_BUFFERS   getTFB();
  TRANSMIT_PACKETS_ELEMENT *getTPE();

private:
  int                           m_nRetries;
  WSABUF                       *m_pWSABuf;
  TRANSMIT_FILE_BUFFERS         m_TFB;
  TRANSMIT_PACKETS_ELEMENT     *m_pTPE;
};
