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

//cTrans2QueryFileInfo.cpp

#include "stdafx.h"

#if CIFS 

#include ".\ctrans2queryfileinfo.h"

#include "cSMBResponse.h"
#include "trans2Structs.h"

#include "..\iComposite.h"

cTrans2QueryFileInfo::cTrans2QueryFileInfo(const cPtr<iTreeResource> pTreeResource
  , const cPtr<iComposite> pComposite
  , PREQ_QUERY_FILE_INFORMATION pReq
  , cPtr<cSMBResponse> pResponse)
: cTrans2Response(pResponse)
, cSizeOnDisk(pTreeResource)
, m_pComposite(pComposite)
{
  // get pointers to data
  const PUSHORT pByteCount = reinterpret_cast<const PUSHORT>(pResponse->getData(0));
  PUCHAR pBuffer = reinterpret_cast<PUCHAR>(pByteCount);
  pBuffer += sizeof(USHORT);

  // set param block
  PRESP_QUERY_FILE_INFORMATION pResp = reinterpret_cast<PRESP_QUERY_FILE_INFORMATION>(pBuffer);
  pResp->EaErrorOffset = 0;
  setParams(pBuffer, sizeof(RESP_QUERY_FILE_INFORMATION));

  pBuffer += sizeof(RESP_QUERY_FILE_INFORMATION);

  // do the command

  DWORD dwRet;
  
  switch (pReq->InformationLevel)
  {
    case SMB_QUERY_FILE_BASIC_INFO:
    case 1004:  // SAMBA
      dwRet = Basic(pBuffer);
      break;
    case SMB_QUERY_FILE_STANDARD_INFO:  // SAMBA
    case 1005:  // SAMBA
      dwRet = Standard(pBuffer);
      break;
    case SMB_QUERY_FILE_EA_INFO:
      dwRet = EA(pBuffer);
      break;
    case SMB_QUERY_FILE_STREAM_INFO:
      dwRet = Stream(pBuffer);
      break;
    case SMB_QUERY_FILE_ALL_INFO:
      dwRet = All(pBuffer);
      break;
    case SMB_QUERY_FILE_COMPRESSION_INFO:
      dwRet = Compression(pBuffer);
      break;
    case QUERY_FILE_UNIX_BASIC:
      dwRet = UnixBasic(pBuffer);
      break;
    default:
      dwRet = processUnhandled(pBuffer);
      break;
  }

  if (ERROR_SUCCESS == dwRet)
  {
    // set the byte count of the data section
    *pByteCount = static_cast<USHORT>(pBuffer - reinterpret_cast<PUCHAR>(pByteCount) - sizeof(USHORT));
  }
  else
  {
    pResponse = cTrans2Response::processUnhandled();
  }
}

cTrans2QueryFileInfo::~cTrans2QueryFileInfo(void)
{
}

// query path handlers
DWORD cTrans2QueryFileInfo::processUnhandled(PUCHAR &pBuffer) const
{
  //QSOS((L"Unhandled Query File Info request {%s}", m_pComposite->getPath().c_str()));
  return SMB_ERR_BAD_FUNCTION;
}

DWORD cTrans2QueryFileInfo::Basic(PUCHAR &pBuffer) const
{
  BY_HANDLE_FILE_INFORMATION bhfi;
  DWORD dwRet = m_pComposite->writeInfo(&bhfi, 0);
  if (dwRet != ERROR_SUCCESS)
  {
    cPathInfoErr::printErrMsg(dwRet);
  }
  else
  {
    // set data block
    PsmbQueryFileBasicInfo pSmbInfo = reinterpret_cast<PsmbQueryFileBasicInfo>(pBuffer);
    pSmbInfo->CreationTime.QuadPart = makeLg(bhfi.ftCreationTime);
    pSmbInfo->LastAccessTime.QuadPart = makeLg(bhfi.ftLastAccessTime);
    pSmbInfo->LastWriteTime.QuadPart = makeLg(bhfi.ftLastWriteTime);
    pSmbInfo->ChangeTime.QuadPart = makeLg(bhfi.ftLastWriteTime);   // RHRHRH - don't know how to determine this yet
    pSmbInfo->Attributes = bhfi.dwFileAttributes;
    setData(pBuffer, sizeof(smbQueryFileBasicInfo));
    pBuffer += sizeof(smbQueryFileBasicInfo);
  }

  return dwRet;
}

DWORD cTrans2QueryFileInfo::Standard(PUCHAR &pBuffer) const
{
  BY_HANDLE_FILE_INFORMATION bhfi;
  DWORD dwRet = m_pComposite->writeInfo(&bhfi, 0);
  if (dwRet != ERROR_SUCCESS)
  {
    cPathInfoErr::printErrMsg(dwRet);
  }
  else
  {
    // set data block
    PsmbQueryFileStandardInfo pSmbInfo = reinterpret_cast<PsmbQueryFileStandardInfo>(pBuffer);
    pSmbInfo->AllocationSize = getSizeOnDisk(bhfi.nFileSizeHigh, bhfi.nFileSizeLow);
    pSmbInfo->EndOfFile.QuadPart = makeLg(bhfi.nFileSizeHigh, bhfi.nFileSizeLow);
    pSmbInfo->NumberOfLinks = bhfi.nNumberOfLinks < 1 ? 1 : bhfi.nNumberOfLinks;
    pSmbInfo->DeletePending = 0;
    pSmbInfo->Directory = ((bhfi.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
    setData(pBuffer, sizeof(smbQueryFileStandardInfo));

    pBuffer += sizeof(smbQueryFileStandardInfo);
  }

  return dwRet;
}

DWORD cTrans2QueryFileInfo::Stream(PUCHAR &pBuffer) const
{
  BY_HANDLE_FILE_INFORMATION bhfi;
  DWORD dwRet = m_pComposite->writeInfo(&bhfi, 0);
  if (dwRet != ERROR_SUCCESS)
  {
    cPathInfoErr::printErrMsg(dwRet);
  }
  else
  {
    // set data block
    PsmbQueryFileStreamInfo pSmbInfo = reinterpret_cast<PsmbQueryFileStreamInfo>(pBuffer);
    pSmbInfo->StreamSize.QuadPart = makeLg(bhfi.nFileSizeHigh, bhfi.nFileSizeLow);
    pSmbInfo->StreamAllocationSize = pSmbInfo->StreamSize;//getSizeOnDisk(bhfi.nFileSizeHigh, bhfi.nFileSizeLow);
    pSmbInfo->NextEntryOffset = 0;
    const std::wstring ksStreamName(L"::$DATA");
    pSmbInfo->StreamNameLength = checked_static_cast<ULONG>(ksStreamName.size()*sizeof(WCHAR));
    memcpy(pSmbInfo->FileName, ksStreamName.c_str(), pSmbInfo->StreamNameLength);
    const USHORT dataLength = static_cast<USHORT>(sizeof(smbQueryFileStreamInfo) + pSmbInfo->StreamNameLength);
    setData(pBuffer, dataLength);

    pBuffer += dataLength;
  }

  return dwRet;
}

DWORD cTrans2QueryFileInfo::All(PUCHAR &pBuffer) const
{
  BY_HANDLE_FILE_INFORMATION bhfi;
  DWORD dwRet = m_pComposite->writeInfo(&bhfi, 0);
  if (dwRet != ERROR_SUCCESS)
  {
  }
  else
  {
    // set data block
    PsmbFindFileAllInfo pSmbInfo = reinterpret_cast<PsmbFindFileAllInfo>(pBuffer);
    pSmbInfo->CreationTime.QuadPart = makeLg(bhfi.ftCreationTime);
    pSmbInfo->LastAccessTime.QuadPart = makeLg(bhfi.ftLastAccessTime);
    pSmbInfo->LastWriteTime.QuadPart = makeLg(bhfi.ftLastWriteTime);
    pSmbInfo->ChangeTime.QuadPart = makeLg(bhfi.ftLastWriteTime);   // RHRHRH - don't know how to determine this yet
    pSmbInfo->Attributes = static_cast<USHORT>(bhfi.dwFileAttributes);
    pSmbInfo->AllocationSize.QuadPart = makeLg(bhfi.nFileSizeHigh, bhfi.nFileSizeLow);
    pSmbInfo->EndOfFile = pSmbInfo->AllocationSize;
    pSmbInfo->NumberOfLinks = 1;
    pSmbInfo->DeletePending = 0;
    pSmbInfo->Directory = (bhfi.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? TRUE : FALSE;
    //pSmbInfo->IndexNumber = pSmbInfo->ChangeTime;
    pSmbInfo->EASize = 0;
    //pSmbInfo->AccessFlags = FILE_GENERIC_READ | FILE_GENERIC_WRITE;
    //pSmbInfo->IndexNumber1 = pSmbInfo->ChangeTime;
    //pSmbInfo->CurrentByteOffset.QuadPart = 0;
    //pSmbInfo->Mode = FILE_WRITE_THROUGH;
    //pSmbInfo->AlignmentRequirement = 0;

    const String sFilename = m_pComposite->Name();
    pSmbInfo->FileNameLength = sFilename.size() * sizeof(WCHAR);
    memcpy(pSmbInfo->FileName, sFilename.c_str(), pSmbInfo->FileNameLength);
    const USHORT dataLength = static_cast<USHORT>(sizeof(smbFindFileAllInfo) + pSmbInfo->FileNameLength);
    setData(pBuffer, dataLength);

    pBuffer += dataLength;
  }

  return dwRet;
}

DWORD cTrans2QueryFileInfo::Compression(PUCHAR &pBuffer) const
{
  BY_HANDLE_FILE_INFORMATION bhfi;
  DWORD dwRet = m_pComposite->writeInfo(&bhfi, 0);
  if (dwRet == ERROR_SUCCESS)
  {
    // set data block
    PsmbFindFileCompressionInfo pSmbInfo = reinterpret_cast<PsmbFindFileCompressionInfo>(pBuffer);
    pSmbInfo->ChunkShift = 0;
    pSmbInfo->ClusterShift = 0;
    LARGE_INTEGER lg;
    lg.HighPart = bhfi.nFileSizeHigh;
    lg.LowPart = bhfi.nFileSizeLow;
    pSmbInfo->CompressedFileSize = lg;
    pSmbInfo->CompressionFormat = 0;
    pSmbInfo->CompressionUnitShift = 0;

    const USHORT dataLength = static_cast<USHORT>(sizeof(smbFindFileCompressionInfo));
    setData(pBuffer, dataLength);

    pBuffer += dataLength;
  }

  return dwRet;
}

DWORD cTrans2QueryFileInfo::EA(PUCHAR &pBuffer) const
{
  DWORD dwRet(ERROR_SUCCESS);
  PULONG pEASize = reinterpret_cast<PULONG>(pBuffer);
  *pEASize = 0;
  setData(pBuffer, sizeof(PULONG));
  pBuffer += sizeof(PULONG);
  return dwRet;
}

DWORD cTrans2QueryFileInfo::UnixBasic(PUCHAR &pBuffer) const
{
  BY_HANDLE_FILE_INFORMATION bhfi;
  DWORD dwRet = m_pComposite->writeInfo(&bhfi, 0);
  if (dwRet != ERROR_SUCCESS)
  {
    cPathInfoErr::printErrMsg(dwRet);
  }
  else
  {
    PSMB_QUERY_FILE_UNIX_BASIC pUnixInfo = reinterpret_cast<PSMB_QUERY_FILE_UNIX_BASIC>(pBuffer);

    pUnixInfo->ENDOFFILE = makeLg(bhfi.nFileSizeHigh, bhfi.nFileSizeLow);//FILE SIZE
    pUnixInfo->NumOfBlocks = 1;//Number of file system block used to store file
    pUnixInfo->LastStatusChange = makeLg(bhfi.ftLastWriteTime);//Last time the status of the file was changed. This is in DCE time
    pUnixInfo->LastAccessTime = makeLg(bhfi.ftLastAccessTime);//Last access time. This is in DCE time.
    pUnixInfo->LastModificationTime = makeLg(bhfi.ftLastWriteTime);// Last modification time. This is in DCE time.
    pUnixInfo->Uid = 0;//Numeric user id for the owner
    pUnixInfo->Gid = 0;//Numeric group id of owner
    pUnixInfo->Type = (bhfi.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? eDirectory : eFile;
    pUnixInfo->devmajor = 0;//Major device number if type is device
    pUnixInfo->devminor = 0;//Minor device number if type is device
    pUnixInfo->uniqueid = 1;//This is a server-assigned unique id for the file.
    //  The client will typically map this onto an inode
    //  number. The scope of uniqueness is the share.
    pUnixInfo->permissions = 777;//Standard UNIX file permissions
    pUnixInfo->nlinks = 1;//The number of directory entries that map to this entry (number of hard links)

    setData(pBuffer, sizeof(SMB_QUERY_FILE_UNIX_BASIC));
    pBuffer += sizeof(SMB_QUERY_FILE_UNIX_BASIC);
  }

  return dwRet;
}

#endif // CIFS 