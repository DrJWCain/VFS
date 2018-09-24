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

//cSetFileInfo.cpp - SET_FILE_INFO command handler

#include "stdafx.h"

#if CIFS 

#include ".\csetfileinfo.h"

#include "cSMBResponse.h"
#include "trans2structs.h"

#include "..\..\KernelSDK\include\QWinBase.h"

//cSetFileInfo::tProcessSetFileInfo cSetFileInfo::SetInfoDispatch[USHRT_MAX] = {0};

cSetFileInfo::cSetFileInfo(const cPtr<iComposite> pComposite
  , PREQ_SET_FILE_INFORMATION pReq
  , const PUCHAR trans2Data
  , const cPtr<cSMBResponse> pResponse)
: cTrans2Response(pResponse)
, m_pComposite(pComposite)
{
  // create dispatch table
  //if(!SetInfoDispatch[0])
  //{
  //  for(int i = 0; i < USHRT_MAX; ++i)
  //  {
  //    SetInfoDispatch[i] = &cSetFileInfo::processUnhandled;
  //  }
  //  SetInfoDispatch[SMB_SET_FILE_DISPOSITION_INFO] = &cSetFileInfo::Disposition;
  //  SetInfoDispatch[SMB_SET_FILE_ALLOCATION_INFO] = &cSetFileInfo::Allocation;
  //  SetInfoDispatch[SMB_SET_FILE_END_OF_FILE_INFO] = &cSetFileInfo::EndOfFile;
  //  SetInfoDispatch[SMB_SET_FILE_BASIC_INFO] = &cSetFileInfo::Basic;
  //  SetInfoDispatch[1004] = &cSetFileInfo::Basic; // SAMBA invented this command value
  //}

  // get pointers to data
  const PUSHORT pByteCount = reinterpret_cast<const PUSHORT>(pResponse->getData(0));
  PUCHAR pBuffer = reinterpret_cast<PUCHAR>(pByteCount);
  pBuffer += sizeof(USHORT);

  // set param block
  PRESP_SET_FILE_INFORMATION pResp = reinterpret_cast<PRESP_SET_FILE_INFORMATION>(pBuffer);
  pResp->EaErrorOffset = 0;
  setParams(pBuffer, sizeof(RESP_QUERY_PATH_INFORMATION));

  pBuffer += sizeof(RESP_QUERY_PATH_INFORMATION);

  DWORD dwRet;
  switch (pReq->InformationLevel)
  {
  case SMB_SET_FILE_DISPOSITION_INFO:
    dwRet = Disposition(trans2Data, pBuffer);
    break;
  case SMB_SET_FILE_ALLOCATION_INFO:
    dwRet = Allocation(trans2Data, pBuffer);
    break;
  case SMB_SET_FILE_END_OF_FILE_INFO:
    dwRet = EndOfFile(trans2Data, pBuffer);
    break;
  case SMB_SET_FILE_BASIC_INFO:
  case 1004: // SAMBA seem to have invented this command value
    dwRet = Basic(trans2Data, pBuffer);
    break;
  default:
    dwRet = processUnhandled(trans2Data, pBuffer);
    break;
  }
  if (dwRet != ERROR_SUCCESS)
  {
    // use the Dos error code since they seem to match the SMB meanings
    pResponse->setDosError(SMB_ERR_CLASS_DOS, dwRet);
  }
  else
  {
    // set the byte count of the data section
    *pByteCount = static_cast<USHORT>(pBuffer - reinterpret_cast<PUCHAR>(pByteCount) - sizeof(USHORT));
  }
}

cSetFileInfo::~cSetFileInfo(void)
{
}

DWORD cSetFileInfo::processUnhandled(const PUCHAR pData, PUCHAR &pBuffer) const
{
  QSOS((L"Unhandled trans2 SET_FILE_INFO request"));
  return SMB_ERR_BAD_FUNCTION;
}

DWORD cSetFileInfo::Allocation(const PUCHAR pData, PUCHAR &pBuffer) const
{
  return m_pComposite->SetFileSize(reinterpret_cast<PLARGE_INTEGER>(pData));
}

DWORD cSetFileInfo::EndOfFile(const PUCHAR pData, PUCHAR &pBuffer) const
{
  return m_pComposite->SetFileSize(reinterpret_cast<PLARGE_INTEGER>(pData));
}

DWORD cSetFileInfo::Disposition(const PUCHAR pData, PUCHAR &pBuffer) const
{
  DWORD dwRet(ERROR_ACCESS_DENIED);

  if (static_cast<BOOL>(*pData) != FALSE)
  {
    dwRet = m_pComposite->Delete();
    if (dwRet != ERROR_SUCCESS)
      QSOS((L"cSetFileInfo::Disposition() - Failed to delete item {%s} with error %s"
        , m_pComposite->Name().c_str()
        , win32ErrorToString(dwRet).c_str()));
  }
  return dwRet;
}

DWORD cSetFileInfo::Basic(const PUCHAR pData, PUCHAR &pBuffer) const
{
  // this seems to be done to set file attributes
  PsmbQueryFileBasicInfo pBasicInfo = reinterpret_cast<PsmbQueryFileBasicInfo>(pBuffer);
  FILETIME creationTime;
  creationTime.dwLowDateTime = pBasicInfo->CreationTime.LowPart;
  creationTime.dwHighDateTime = pBasicInfo->CreationTime.HighPart;
  FILETIME modifiedTime;
  modifiedTime.dwLowDateTime = pBasicInfo->LastWriteTime.LowPart;
  modifiedTime.dwHighDateTime = pBasicInfo->LastWriteTime.HighPart;
  FILETIME accessTime;
  accessTime.dwLowDateTime = pBasicInfo->LastAccessTime.LowPart;
  accessTime.dwHighDateTime = pBasicInfo->LastAccessTime.HighPart;
  DWORD attributes = pBasicInfo->Attributes;

  return m_pComposite->SetFileInfo(creationTime, modifiedTime, accessTime, attributes);
}

#endif // CIFS 