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

// cTrans2Response.cpp
// wraps generic functionality when forming a transaction response

#include "stdafx.h"

#if CIFS 

#include ".\ctrans2response.h"

#include "mycifs.h"
#include "cSMBResponse.h"

cTrans2Response::cTrans2Response(const cPtr<cSMBResponse> pResponse)
: m_pResponse(pResponse)
{
}

cTrans2Response::~cTrans2Response(void)
{
}

cPtr<cSMBResponse> cTrans2Response::processUnhandled() const
{
  m_pResponse->setDosError(SMB_ERR_CLASS_DOS, SMB_ERR_BAD_FUNCTION);
  return m_pResponse;
}

void cTrans2Response::setParams(PUCHAR pParams, USHORT byteCount) const
{
  m_pResponse->setTotalParamCount(byteCount);
  m_pResponse->setParamCount(byteCount);
  m_pResponse->setParamDisplacement(0);
  m_pResponse->setParamOffset(pParams);
}

void cTrans2Response::setData(PUCHAR pData, USHORT byteCount) const
{
  m_pResponse->setTotalDataCount(byteCount);
  m_pResponse->setDataCount(byteCount);
  m_pResponse->setDataDisplacement(0);
  m_pResponse->setDataOffset(pData);
}

#endif // CIFS 