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

//cNTTransIOCTL.cpp

#include "stdafx.h"

#if CIFS 

#include ".\cnttransioctl.h"

#include "cSMBResponse.h"


cNTTransIOCTL::cNTTransIOCTL(const cPtr<iTreeResource> pTreeResource
  , const PREQ_NT_IO_CONTROL pSetupData
  , cPtr<cSMBResponse> pResponse)
: cTrans2Response(pResponse)
, m_pTreeResource(pTreeResource)
{
  pResponse->addSetupData(reinterpret_cast<const PUCHAR>(&(pSetupData->FunctionCode)), 1);

  // Not using a pointer to member function table because it would be as
  // big as ULONG_MAX ~4Gb * sizeof (pmfn)
  //switch (pSetupData->FunctionCode)
  //{
  //case FSCTL_CREATE_OR_GET_OBJECT_ID:
  //  pResponse = processCreateOrGetObjectID(pSetupData->Fid, pTreeResource, pResponse);
  //  break;
  //case FSCTL_GET_REPARSE_POINT:
  //  pResponse = processGetReparsePoint(pSetupData->Fid, pTreeResource, pResponse);
  //  break;
  //default:
    pResponse = processUnhandled(pResponse, pSetupData->FunctionCode);
  //  break;
  //}
}

cNTTransIOCTL::~cNTTransIOCTL(void)
{
}

cPtr<cSMBResponse> cNTTransIOCTL::processUnhandled(cPtr<cSMBResponse> pResponse, const ULONG functionCode)
{
  QTRACE((L"Unhandled NT_TRANS_IOCTL function: %d", functionCode));
  pResponse->setNTError(static_cast<ULONG>(STATUS_NOT_SUPPORTED));
  return pResponse;
}

cPtr<cSMBResponse> cNTTransIOCTL::processCreateOrGetObjectID(const USHORT Fid
  , cPtr<cSMBResponse> pResponse)
{
  //FILE_OBJECTID_BUFFER _foidb;
  
  //= pTreeResource->CreateOrGetObjectID(Fid, _foidb);
  PUCHAR pBuffer = pResponse->getData(0);
  const PUSHORT pByteCount = reinterpret_cast<PUSHORT>(pBuffer);
  pBuffer += sizeof(USHORT);
  setParams(pBuffer, 0);

  DWORD nBytes(0);
  setData(pBuffer, static_cast<USHORT>(nBytes));
  pBuffer += nBytes;

  *pByteCount = static_cast<USHORT>(pBuffer - reinterpret_cast<PUCHAR>(pByteCount) - sizeof(USHORT));

  return pResponse;
}

cPtr<cSMBResponse> cNTTransIOCTL::processGetReparsePoint(const USHORT Fid
  , cPtr<cSMBResponse> pResponse)
{
  //FILE_OBJECTID_BUFFER _foidb;
  
  //= pTreeResource->GetReparsePoint(Fid, _foidb);
  PUCHAR pBuffer = pResponse->getData(0);
  const PUSHORT pByteCount = reinterpret_cast<PUSHORT>(pBuffer);
  pBuffer += sizeof(USHORT);
  setParams(pBuffer, 0);

  DWORD nBytes(0);
  setData(pBuffer, static_cast<USHORT>(nBytes));
  pBuffer += nBytes;

  *pByteCount = static_cast<USHORT>(pBuffer - reinterpret_cast<PUCHAR>(pByteCount) - sizeof(USHORT));

  return pResponse;
}

#endif // CIFS 