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

#include "stdafx.h"
#include "cSMB2OpLock.h"

cSMB2OpLock::cSMB2OpLock(const ULONGLONG& fid, const ULONGLONG& durableFileID, cPtr<cSMB2Response> response) 
: m_Fid(fid), m_DurableFid(durableFileID), m_Response(response), m_Locked(true)
{
}

cSMB2OpLock::~cSMB2OpLock()
{
}

bool cSMB2OpLock::released() const
{ 
  return !m_Locked; 
}

void cSMB2OpLock::Release()
{
  m_Locked = false;
}

void cSMB2OpLock::Break()
{
  if (m_Locked)
  {
    m_Locked = false;
    const PSMB2_HEADER pHeader(reinterpret_cast<PSMB2_HEADER>(m_Response->header()));
    //ZeroMemory(pHeader, sizeof(SMB2_HEADER));
    pHeader->ProtocolId = SMB2_HEADER_PROTOCOL;
    pHeader->Command = SMB2_Break;
    pHeader->Flags = 1; // server to client
    pHeader->MessageId = 0xffffffffffffffff;
    pHeader->TreeId = 0;
    pHeader->SessionId = 0;

    const PRESP_SMB2_OPLOCK_BREAK_NOTIFY pResp = m_Response->getResponseType<RESP_SMB2_OPLOCK_BREAK_NOTIFY>();
    pResp->StructureSize = 24; //[#2] p.73
    pResp->OplockLevel = SMB2_OPLOCK_LEVEL_NONE;
    pResp->Reserved = 0;
    pResp->Reserved2 = 0;
    pResp->FileId.Persistent = m_DurableFid;//0xffffffff00000000 | m_Fid;
    pResp->FileId.Volatile = m_Fid;
    
    const cPtr<iRXPacket> pRXPacket = m_Response->getContextPacket();
    const cPtr<iTXPacket> pTXPacket = pRXPacket->getResponsePacket();
    m_Response->WritePacket(pTXPacket);
    pTXPacket->Send();
  }
}