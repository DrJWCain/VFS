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

// cTrans2FindFirst2.h
// find first 2 command dispatcher
//[#1] p.99

#include "stdafx.h"

#if CIFS 

#include ".\ctrans2findfirst2.h"
#include "cSMBResponse.h"
#include "cSMBSession.h"
#include "trans2Structs.h"
#include "cSMBRequest.h"
#include "align.h"

#include "..\QCIFSIPC\iQCIFSIPC.h"

cTrans2FindFirst2::cTrans2FindFirst2(const cPtr<iTreeResource> pTreeResource
  , const int maxData
  , cSMBSession *pParentSession
  , const PREQ_FIND_FIRST2 pReq
  , cPtr<cSMBResponse> pResponse)
: cTrans2Response(pResponse)
, cTrans2Find(pTreeResource, maxData, pReq, pResponse)
{
  // do a directory search on our tree
  String sPath(reinterpret_cast<PWCHAR>(pReq->Buffer));
  if (pResponse->Header->Flags2 & SMB_FLAGS2_DFS)
  {
    try
    {
      sPath =  iQCIFSIPC::singleton().removeDFSPart(sPath);
    }
    catch (const cNotFound&)
    {
    }
  }

  DWORD dwRet = pTreeResource->Search(sPath, *m_pFileInfoList);

  if (m_pFileInfoList->empty())
  {
    pResponse->setNTError(static_cast<ULONG>(STATUS_NO_SUCH_FILE));
  }
  else
  {
    if (dwRet == ERROR_SUCCESS)
    {
      *m_pSearchPos = m_pFileInfoList->begin();

      // get pointers to our data block
      const PUSHORT pByteCount = reinterpret_cast<PUSHORT>(pResponse->getData(0));
      PUCHAR pData = reinterpret_cast<PUCHAR>(pByteCount+1);
      align<WORD>(pData, reinterpret_cast<PUCHAR>(pResponse->Header));
      PRESP_FIND_FIRST2 pResp = reinterpret_cast<PRESP_FIND_FIRST2>(pData);
      pData += sizeof(RESP_FIND_FIRST2);
      align<DWORD64>(pData, reinterpret_cast<PUCHAR>(pResponse->Header));
      const PUCHAR pDataStart = pData;

      // run the command
      pResponse = doFind(pReq->InformationLevel,pData, pResponse);

      // set pResponse parameters
      const bool bEndOfSearch = EndOfSearch();
      pResp->Sid = pParentSession->getSearchHandleID(this, bEndOfSearch);
      pResp->SearchCount = static_cast<USHORT>(*m_pFileIndex);
      pResp->EndOfSearch = bEndOfSearch;
      pResp->EaErrorOffset = 0;
      pResp->LastNameOffset = 0;
      setParams(reinterpret_cast<PUCHAR>(pResp), sizeof(RESP_FIND_FIRST2));

      // set data info (pData has been incremented by dispatched command)
      USHORT dataSize = static_cast<USHORT>(pData - pDataStart);
      setData(pDataStart, dataSize);

      // set the byte count of the data section
      *pByteCount = static_cast<USHORT>(pData - reinterpret_cast<PUCHAR>(pResp) + 1);
    }
    else
    {
      if (ERROR_FILE_NOT_FOUND == dwRet || ERROR_PATH_NOT_FOUND == dwRet)
      {
        pResponse->setNTError(static_cast<ULONG>(STATUS_NO_SUCH_FILE));
      }
      else
        pResponse->setDosError(SMB_ERR_CLASS_DOS, dwRet);
    }
  }
}

cTrans2FindFirst2::~cTrans2FindFirst2(void)
{
  delete(m_pFileIndex);
  delete(m_pFileInfoList);
  delete(m_pSearchPos);
}

#endif // CIFS 