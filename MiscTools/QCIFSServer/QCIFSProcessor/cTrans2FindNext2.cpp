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

//cTrans2FindNext2.cpp

#include "StdAfx.h"

#if CIFS 

#include ".\ctrans2findnext2.h"

#include "cSMBResponse.h"

cTrans2FindNext2::cTrans2FindNext2(cTrans2Find *pTrans2Find
  , const cPtr<iTreeResource> pTreeResource
  , PREQ_FIND_NEXT2 pReq
  , cPtr<cSMBResponse> pResponse)
: cTrans2Response(pResponse)
, cTrans2Find(pTreeResource, pTrans2Find)
{
  // get pointers to our data block
  const PUSHORT pByteCount = reinterpret_cast<PUSHORT>(pResponse->getData(0));
  PUCHAR pData = reinterpret_cast<PUCHAR>(pByteCount+1);
  //alignWord(pData, reinterpret_cast<PUCHAR>(pResponse->Header));
  PRESP_FIND_NEXT2 pResp = reinterpret_cast<PRESP_FIND_NEXT2>(pData);
  pData += sizeof(RESP_FIND_NEXT2);
  //alignDword64(pData, reinterpret_cast<PUCHAR>(pResponse->Header));
  const PUCHAR pDataStart = pData;

  int currentIndex = *m_pFileIndex;

  // run the command
  pResponse = doFind(pReq->InformationLevel,pData, pResponse);

  // set pResponse parameters
  pResp->SearchCount = static_cast<USHORT>(*m_pFileIndex - currentIndex);
  pResp->EndOfSearch = EndOfSearch();
  pResp->EaErrorOffset = 0;
  pResp->LastNameOffset = 0;
  setParams(reinterpret_cast<PUCHAR>(pResp), sizeof(RESP_FIND_NEXT2));

  // set data info (pData has been incremented by dispatched command)
  USHORT dataSize = static_cast<USHORT>(pData - pDataStart);
  setData(pDataStart, dataSize);

  // set the byte count of the data section
  *pByteCount = static_cast<USHORT>(pData - reinterpret_cast<PUCHAR>(pResp) + 1);
}

cTrans2FindNext2::~cTrans2FindNext2(void)
{
}


#endif // CIFS 