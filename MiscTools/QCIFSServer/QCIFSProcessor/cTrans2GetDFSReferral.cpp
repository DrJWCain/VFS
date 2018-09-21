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

//cTrans2GetDFSReferral.cpp

#include "stdafx.h"

#if CIFS 

#include "cTrans2GetDFSReferral.h"
#include "cSMBResponse.h"

#include "..\iTreeResource.h"
#include "..\QCIFSIPC\iQCIFSIPC.h"

cTrans2GetDFSReferral::cTrans2GetDFSReferral(const cPtr<iTreeResource> pTreeResource
  , const PREQ_GET_DFS_REFERRAL pReq
  , const cPtr<cSMBResponse> pResponse)
: cTrans2Response(pResponse)
{
  const String fileName(reinterpret_cast<PWCHAR>(&(pReq->RequestFileName[0])));

  // get pointers to our data block
  const PUSHORT pByteCount = reinterpret_cast<PUSHORT>(pResponse->getData(0));
  PRESP_GET_DFS_REFERRAL pResp = reinterpret_cast<PRESP_GET_DFS_REFERRAL>(pByteCount + 1);
  ZeroMemory(pResp, sizeof(RESP_GET_DFS_REFERRAL));

  setParams(reinterpret_cast<PUCHAR>(pResp), 0);
  pResp->PathConsumed = static_cast<USHORT>(fileName.size()*sizeof(WCHAR));
  pResp->NumberOfReferrals = 1;//static_cast<USHORT>(filesList.size());
  pResp->ReferralServers = 1;
  pResp->StorageServers = 1;

  PUCHAR pReferrals = reinterpret_cast<PUCHAR>(&(pResp->Referrals[0]));

  //for (std::list<std::wstring>::iterator it = filesList.begin()
  //  ; it != filesList.end()
  //  ; ++it)
  //{
  //  const std::wstring ksPath = *it;
  //  switch(pReq->MaxReferralLevel)
  //  {
  //  //case 0:
  //  //  break;
  //  //case 1:
  //  //  pReferrals = v1Referral(pReferrals, ksPath);
  //  //  break;
  //  //case 2:
  //  //  pReferrals = v2Referral(pReferrals, ksPath);
  //  //  break;
  //  //case 3:
  //  //  pReferrals = v3Referral(pReferrals, ksPath);
  //  //  break;
  //  default:
  //    pReferrals = v1Referral(pReferrals, ksPath);
  //    break;
  //  }
  //}

  pReferrals = v1Referral(pReferrals, fileName);
  setData(reinterpret_cast<PUCHAR>(pResp), static_cast<USHORT>(pReferrals - reinterpret_cast<PUCHAR>(pResp)));

  // set the byte count of the data section
  *pByteCount = static_cast<USHORT>(pReferrals - reinterpret_cast<PUCHAR>(pResp));
}

cTrans2GetDFSReferral::~cTrans2GetDFSReferral()
{
}

PUCHAR cTrans2GetDFSReferral::v1Referral(const PUCHAR pReferalSpace, const String& ksFileName)
{
  const PDFS_REFERRAL_V1 pReferral = reinterpret_cast<PDFS_REFERRAL_V1>(pReferalSpace);

  try
  {
    if (iQCIFSIPC::singleton().hasShare(ksFileName))
    {
      wcscpy(pReferral->ShareName, ksFileName.c_str());
      pReferral->Size = static_cast<USHORT>(sizeof(DFS_REFERRAL_V1) + (ksFileName.length()*sizeof(WCHAR)));
      pReferral->ServerType = 1;
    }
    else
    {
      const String sReferral(iQCIFSIPC::singleton().getDFSPath(ksFileName));
      wcscpy(pReferral->ShareName, sReferral.c_str());
      pReferral->Size = static_cast<USHORT>(sizeof(DFS_REFERRAL_V1) + (sReferral.length()*sizeof(WCHAR)));
      pReferral->ServerType = 0;
    }

    pReferral->VersionNumber = 1;
    pReferral->StripPath = 1;
    return pReferalSpace + pReferral->Size;
  }
  catch (const cNotFound&) { }
  
  QSOS((L"! cTrans2GetDFSReferral::v1Referral() - QCIFSIPC DLL MISSING! !"));
  return pReferalSpace;
}

PUCHAR cTrans2GetDFSReferral::v2Referral(const PUCHAR pReferalSpace, const String& ksFileName)
{
  PDFS_REFERRAL_V2 pReferral = reinterpret_cast<PDFS_REFERRAL_V2>(pReferalSpace);
  pReferral->VersionNumber = 2;
  pReferral->ServerType = 1;

  return pReferalSpace + sizeof(DFS_REFERRAL_V1);
}

PUCHAR cTrans2GetDFSReferral::v3Referral(const PUCHAR pReferalSpace, const String& ksFileName)
{
  PDFS_REFERRAL_V3 pReferral = reinterpret_cast<PDFS_REFERRAL_V3>(pReferalSpace);
  pReferral->VersionNumber = 3;
  pReferral->ServerType = 1;

  return pReferalSpace + sizeof(DFS_REFERRAL_V1);
}

#endif // CIFS 