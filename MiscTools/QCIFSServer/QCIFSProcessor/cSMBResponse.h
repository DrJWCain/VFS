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

//cSMBResponse.h

#pragma once

#if CIFS 

#include "cSMBRequest.h"
#include "logging.h"
#include "iResponseWriter.h"

#include "..\common\TransmitList.h"

class iTXPacket;
class iRXPacket;

class cSMBResponse : public cSMBBuffer, public iResponseWriter, public cRefCount
{
public:
  cSMBResponse(const cSMBRequest& request, const cPtr<cMemoryView> pBuffer);
  ~cSMBResponse();

  //iResponseWriter
  void WritePacket(cPtr<iTXPacket> pPacket);

  //public member functions
  int transmitSize() {return size()+4+ExtraPayload;};
  void log(cSMBRequest* req);

  void setDosError(UCHAR errorClass, DWORD error);
  void setNTError(ULONG error);
  unsigned char* allocParams(unsigned char size);
  void addData(const USHORT size);
  void addData(const SMART_TPE &pTPE);

  PUCHAR getResponseBuffer();
  tTransmitList &getTransmitList();

  // extensions to help with the transaction2 style messages
  void setTotalParamCount(const ULONG paramCount);
  void setTotalDataCount(const ULONG dataCount);
  void setParamCount(const ULONG paramCount);
  void setParamOffset(const PUCHAR pTrans2Params);
  void setParamDisplacement(const ULONG paramDisplacement);
  void setDataCount(const ULONG dataCount);
  void setDataOffset(const PUCHAR pTrans2Data);
  void setDataDisplacement(const ULONG dataDisplacement);
  void addSetupData(const PUCHAR pSetupData, const UCHAR setupCount);

  ULONG getTotalParamCount();
  ULONG getTotalDataCount();
  ULONG getParamCount();
  ULONG getParamOffset();
  ULONG getParamDisplacement();
  ULONG getDataCount();
  ULONG getDataOffset();
  ULONG getDataDisplacement();
  UCHAR getSetupCount();

  void pushBlock();
  void clearBlock();

  cPtr<iRXPacket> getContextPacket() const {return m_pContextPacket;}

  void addExtraData(int i);
private:
  ULONG m_nTotalParamCount;
  ULONG m_nTotalDataCount;
  ULONG m_nParamCount;
  ULONG m_nParamOffset;
  ULONG m_nParamDisplacement;
  ULONG m_nDataCount;
  ULONG m_nDataOffset;
  ULONG m_nDataDisplacement;
  UCHAR m_setupCount;

  cPtr<cMemoryView> m_pBuffer;

  tTransmitList m_TPEList;

  cPtr<iRXPacket> m_pContextPacket;

  int ExtraPayload;
};

#endif // CIFS 