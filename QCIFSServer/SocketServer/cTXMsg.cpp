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

//cTXMsg.cpp

#include "StdAfx.h"

#include ".\ctxmsg.h"
#include "cSocketServer.h"


cTXMsg::cTXMsg(cTXPacket *pPacket)
: cTXPacket(pPacket)
, m_pTPE(NULL)
, m_pWSABuf(NULL)
, m_nRetries(0)
{
  ZeroMemory(&m_TFB, sizeof(TRANSMIT_FILE_BUFFERS));
}

cTXMsg::~cTXMsg(void)
{
  m_pSocketServer->txMsgDone();
  if (m_pTPE != NULL)
    delete[] m_pTPE;

  if (m_pWSABuf != NULL)
    delete [] m_pWSABuf;
}

iActivity::Ptr getActivity();

void cTXMsg::deliver(ULONG_PTR pCompletionKey, const DWORD dwBytes)
{
  cActivityScopeId as(getActivity(), kSent, (int)this);
  //QSOS((L"cTXMsg::deliver()"));
}

void cTXMsg::err(ULONG_PTR pCompletionKey, const DWORD errCode)
{
  //QSOS((L"cTXMsg::err()"));
  if (errCode != ERROR_SUCCESS || errCode != ERROR_IO_PENDING)
  {
    // error with data
    SOCKET_DATA *pSockData = reinterpret_cast<SOCKET_DATA*>(pCompletionKey);
    QSOS((L"cTXMsg::err() - (Socket [%Iu]) %S:%d - %d - %s"
      , pSockData->sock
      , inet_ntoa(pSockData->addr.sin_addr)
      , ntohs(pSockData->addr.sin_port)
      , errCode
      , win32ErrorToString(errCode).c_str()));

    if (pSockData->outstandingRecvs <= 0)
      m_pSocketServer->CloseSocket(pSockData);
  }
}

TRANSMIT_PACKETS_ELEMENT *cTXMsg::getTPE()
{
  if (m_pTPE == NULL && !m_TransmitList.empty())
  {
    m_pTPE = new TRANSMIT_PACKETS_ELEMENT[m_TransmitList.size()];
    for (unsigned int i=0; i<m_TransmitList.size(); i++)
      m_pTPE[i] = m_TransmitList[i].tpe;
  }
  return m_pTPE;
}

WSABUF *cTXMsg::getWSABuf()
{
  if (m_pWSABuf == NULL)
  {
    m_pWSABuf = new WSABUF[m_TransmitList.size()];

    unsigned int i=0;
    for (tTransmitList::iterator it = m_TransmitList.begin()
      ; (it != m_TransmitList.end())
      ; ++it, i++)
    {
      SMART_TPE &smart_tpe = *it;
      if (smart_tpe.tpe.dwElFlags == TP_ELEMENT_FILE)
      {
        cPtr<cMemoryView> pBuffer = new cMemoryView(new cMemory((size_t)smart_tpe.tpe.cLength, cMemory::eVirtual), 0, (size_t)smart_tpe.tpe.cLength);
        DWORD dwBytes;
        if (ReadFile(smart_tpe.tpe.hFile
          , pBuffer->getBytes()
          , smart_tpe.tpe.cLength
          , &dwBytes
          , NULL) == FALSE)
        {
          QSOS((L"cTXMsg::getWSABuf() - couldn't read contents of file."));
          delete[] m_pWSABuf;
          m_pWSABuf = NULL;
          break;
        }
        smart_tpe.tpe.dwElFlags = TP_ELEMENT_MEMORY;
        smart_tpe.tpe.pBuffer = pBuffer->getBytes();
        smart_tpe.pMem = pBuffer;
        m_pWSABuf[i].buf = reinterpret_cast<char*>(pBuffer->getBytes());
        m_pWSABuf[i].len = dwBytes;
        //m_MemReference.insert(pBuffer);
      }
      else
      {
        m_pWSABuf[i].buf = reinterpret_cast<char*>(smart_tpe.tpe.pBuffer);
        m_pWSABuf[i].len = smart_tpe.tpe.cLength;
      }
    }
  }
  return m_pWSABuf;
}

LPTRANSMIT_FILE_BUFFERS cTXMsg::getTFB()
{
  if (m_TransmitList.empty() || m_TransmitList.size() > 3)
    return NULL;

  bool bBeforeFile(true);
  for (tTransmitList::iterator it = m_TransmitList.begin()
    ; it != m_TransmitList.end()
    ; ++it)
  {
    const SMART_TPE &smart_tpe = *it;
    if (smart_tpe.tpe.dwElFlags == TP_ELEMENT_FILE)
    {
      if (m_pTPE != NULL)
        delete[] m_pTPE;
      m_pTPE = new TRANSMIT_PACKETS_ELEMENT[1]; //create an array of size 1 so that delete[] won't break
      m_pTPE[0] = smart_tpe.tpe;
      bBeforeFile = false;
    }
    else if (bBeforeFile)
    {
      m_TFB.Head = smart_tpe.tpe.pBuffer;
      m_TFB.HeadLength = smart_tpe.tpe.cLength;
    }
    else
    {
      m_TFB.Tail = smart_tpe.tpe.pBuffer;
      m_TFB.TailLength = smart_tpe.tpe.cLength;
    }
  }

  return &m_TFB;
}