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

//cSMB2Response.h

#pragma once

#include "cSMB2Request.h"
#include "iResponseWriter.h"
#include "..\common\TransmitList.h"

class iTXPacket;
class iRXPacket;

class cSMB2Response : public cSMB2Buffer, public iResponseWriter, public vfs::cRefCount
{
public:
  cSMB2Response(const cSMB2Request* pRequest, char* memory, const vfs::cPtr<vfs::cMemoryView>& pBuffer);
  ~cSMB2Response();

  //iResponseWriter
  void WritePacket(vfs::cPtr<iTXPacket> pPacket);

  //public member functions
  int transmitSize() {return size()+4;};

  void setDosError(UCHAR errorClass, DWORD error);//for review
  void setNTError(ULONG error, UCHAR errorData = 0);
  template <typename T>
  T* getResponseType()
  {
    addData(sizeof(T));
    return getParams<T*>();
  }

  void addData(const ULONG size);
  void addData(const SMART_TPE &pTPE);

  PUCHAR getResponseBuffer();
  tTransmitList& getTransmitList();

  vfs::cPtr<iRXPacket> getContextPacket() const {return m_pContextPacket;}

  void sign(cPtr<cSMB2Session> session);

private:
  vfs::cPtr<vfs::cMemoryView> m_pBuffer;

  tTransmitList m_TPEList;

  vfs::cPtr<iRXPacket> m_pContextPacket;
};

void makeReplyHeader(const PSMB2_HEADER pReqHeader, const PSMB2_HEADER pRespHeader);
