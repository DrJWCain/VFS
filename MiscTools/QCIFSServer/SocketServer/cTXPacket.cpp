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

//cTXPacket.cpp

#include "StdAfx.h"

#include ".\ctxpacket.h"
#include "cSocketServer.h"

cTXPacket::cTXPacket(const SOCKET sock, cSocketServer *pSocketServer)
: m_socket(sock)
, m_pSocketServer(pSocketServer)
{
}

cTXPacket::cTXPacket(const cTXPacket *pOther)
: m_socket(pOther->m_socket)
, m_pSocketServer(pOther->m_pSocketServer)
, m_TransmitList(pOther->m_TransmitList)
//, m_MemReference(pOther->m_MemReference)
{
}

cTXPacket::~cTXPacket(void)
{
}

//bool cTXPacket::Add(cPtr<cMemoryView> pMem)
//{
//  SMART_TPE smart_tpe;
//  smart_tpe.tpe.cLength = pMem->getSize();
//  smart_tpe.tpe.pBuffer = pMem->getBytes();
//  smart_tpe.tpe.dwElFlags = TP_ELEMENT_MEMORY;
//  smart_tpe.pMem = pMem;
//
//  m_TransmitList.push_back(smart_tpe);
//  //m_MemReference.insert(pMem);
//
//  return true;
//}
//
//bool cTXPacket::Add(const HANDLE h, const ULONG len, const LARGE_INTEGER &offset)
//{
//  SMART_TPE smart_tpe;
//  smart_tpe.tpe.hFile = h;
//  smart_tpe.tpe.cLength = len;
//  smart_tpe.tpe.nFileOffset = offset;
//  smart_tpe.tpe.dwElFlags = TP_ELEMENT_FILE;
//
//  m_TransmitList.push_back(smart_tpe);
//
//  return true;
//}

//bool Add(const SMART_TPE tpe)
//{
//  m_TransmitList.push_back(TPE);
//  return true;
//}

void cTXPacket::setReply(const tTransmitList &transmitList)
{
  m_TransmitList = transmitList;
}

bool cTXPacket::Send()
{
  if (!m_TransmitList.empty())
    return m_pSocketServer->AddPacket(this);
  else
    QSOS((L"cTXPacket::Send() - empty packet list"));

  return false;
}