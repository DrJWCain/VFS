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

// cTrans2QueryPathInfo.cpp

#include "stdafx.h"

#if CIFS 

#include ".\ctrans2querypathinfo.h"

#include "cSMBResponse.h"
#include "trans2Structs.h"

#include "..\iTreeResource.h"
#include "..\QCIFSIPC\iQCIFSIPC.h"

cTrans2QueryPathInfo::cTrans2QueryPathInfo(const cPtr<iTreeResource> pTreeResource
  , const PREQ_QUERY_PATH_INFORMATION pReq
  , const cPtr<cSMBResponse> pResponse)
: cTrans2Response(pResponse)
, cSizeOnDisk(pTreeResource)
, m_pTreeResource(pTreeResource)
{
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

  // get pointers to data
  const PUSHORT pByteCount = reinterpret_cast<const PUSHORT>(pResponse->getData(0));
  PUCHAR pBuffer = reinterpret_cast<PUCHAR>(pByteCount);
  pBuffer += sizeof(USHORT);

  // set param block
  PRESP_QUERY_PATH_INFORMATION pResp = reinterpret_cast<PRESP_QUERY_PATH_INFORMATION>(pBuffer);
  pResp->EaErrorOffset = 0;
  setParams(pBuffer, sizeof(RESP_QUERY_PATH_INFORMATION));

  pBuffer += sizeof(RESP_QUERY_PATH_INFORMATION);

  DWORD retCode;
  switch (pReq->InformationLevel)
  {
  case SMB_QUERY_FILE_BASIC_INFO:
  case 1004:
    retCode = Basic(sPath, pBuffer);
    break;
  case SMB_QUERY_FILE_STANDARD_INFO:
    retCode = Standard(sPath, pBuffer);
    break;
  case SMB_QUERY_FILE_EA_INFO:
    retCode = EA(sPath, pBuffer);
    break;
  case SMB_QUERY_FILE_NAME_INFO:
    retCode = Name(sPath, pBuffer);
    break;
  case SMB_QUERY_FILE_ALL_INFO:
    retCode = All(sPath, pBuffer);
    break;
  case 1035:
    retCode = AttributeTag(sPath, pBuffer);
    break;
  case SMB_QUERY_FILE_STREAM_INFO:
    retCode = Stream(sPath, pBuffer);
    break;
  case SMB_QUERY_FILE_COMPRESSION_INFO:
    retCode = Compression(sPath, pBuffer);
    break;
  case QUERY_FILE_UNIX_BASIC:
    retCode = UnixBasic(sPath, pBuffer);
    break;
  default:
    retCode = processUnhandled(sPath, pBuffer);
    break;
  }
  if (retCode == ERROR_SUCCESS)
  {
    // set the byte count of the data section
    *pByteCount = static_cast<USHORT>(pBuffer - reinterpret_cast<PUCHAR>(pByteCount) - sizeof(USHORT));
  }
  else
  {
    // use the Dos error code since they seem to match the SMB meanings
    pResponse->setDosError(SMB_ERR_CLASS_DOS, retCode);
  }
}

cTrans2QueryPathInfo::~cTrans2QueryPathInfo(void)
{
}

// query path handlers
DWORD cTrans2QueryPathInfo::processUnhandled(const std::wstring &rsPath, PUCHAR &pBuffer) const
{
  QSOS((L"Unhandled Query Path Info request %s\n", rsPath.c_str()));
  return ERROR_INVALID_FUNCTION;
}

DWORD cTrans2QueryPathInfo::Basic(const std::wstring &rsPath, PUCHAR &pBuffer) const
// Ref: [#1] p.84
{
  WIN32_FILE_ATTRIBUTE_DATA bhfi;
  DWORD dwRet = m_pTreeResource->QueryBasicInfo(rsPath, &bhfi);
  if (dwRet != ERROR_SUCCESS)
  {
    if (String::npos != rsPath.find(L"destop.ini"))
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

    const int size(sizeof(smbQueryFileBasicInfo));
    setData(pBuffer, size);

    pBuffer += size;
  }

  return dwRet;
}

DWORD cTrans2QueryPathInfo::Standard(const std::wstring &rsPath, PUCHAR &pBuffer) const
{
  BY_HANDLE_FILE_INFORMATION bhfi;
  DWORD dwRet = m_pTreeResource->QueryStandardInfo(rsPath, &bhfi);
  if (dwRet != ERROR_SUCCESS)
  {
    if (String::npos != rsPath.find(L"destop.ini"))
      cPathInfoErr::printErrMsg(dwRet);
  }
  else
  {
    // set data block
    PsmbQueryFileStandardInfo pSmbInfo = reinterpret_cast<PsmbQueryFileStandardInfo>(pBuffer);
    pSmbInfo->AllocationSize = getSizeOnDisk(bhfi.nFileSizeHigh, bhfi.nFileSizeLow);
    pSmbInfo->EndOfFile.QuadPart = makeLg(bhfi.nFileSizeHigh, bhfi.nFileSizeLow);
    pSmbInfo->NumberOfLinks = bhfi.nNumberOfLinks;
    pSmbInfo->DeletePending = 0;
    pSmbInfo->Directory = ((bhfi.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);

    const int size(sizeof(smbQueryFileStandardInfo));
    setData(pBuffer, size);

    pBuffer += size;
  }

  return dwRet;
}

DWORD cTrans2QueryPathInfo::EA(const std::wstring &rsPath, PUCHAR &pBuffer) const
{
  // don't add any extended attributes yet
  DWORD dwRet(ERROR_SUCCESS);
  PULONG pEASize = reinterpret_cast<PULONG>(pBuffer);
  *pEASize = 0;
  const int size(sizeof(PULONG));
  setData(pBuffer, size);
  pBuffer += size;
  return dwRet;
}

DWORD cTrans2QueryPathInfo::Name(const std::wstring &rsPath, PUCHAR &pBuffer) const
{
  WIN32_FILE_ATTRIBUTE_DATA bhfi;
  DWORD dwRet = m_pTreeResource->QueryBasicInfo(rsPath, &bhfi);
  if (dwRet != ERROR_SUCCESS)
  {
    if (String::npos != rsPath.find(L"destop.ini"))
      cPathInfoErr::printErrMsg(dwRet);
  }
  else
  {
    String stripPath(rsPath);
    size_t pos = stripPath.find(L"\\");
    while (pos != String::npos)
    {
      stripPath = stripPath.substr(pos+1);
      pos = stripPath.find(L"\\");
    }

    PsmbQueryFileNameInfo pSmbInfo = reinterpret_cast<PsmbQueryFileNameInfo>(pBuffer);
    pSmbInfo->FileNameLength = stripPath.size() * sizeof(WCHAR);
    wcscpy(pSmbInfo->FileName, stripPath.c_str());

    const unsigned int size(sizeof(smbQueryFileBasicInfo) + (stripPath.size() * sizeof(WCHAR)));
    setData(pBuffer, static_cast<USHORT>(size));
    pBuffer += size;
  }
  return dwRet;
}

DWORD cTrans2QueryPathInfo::AttributeTag(const std::wstring &rsPath, PUCHAR &pBuffer) const
{
  BY_HANDLE_FILE_INFORMATION bhfi;
  DWORD dwRet = m_pTreeResource->QueryStandardInfo(rsPath, &bhfi);

  if (dwRet != ERROR_SUCCESS)
  {
    if (String::npos != rsPath.find(L"destop.ini"))
      cPathInfoErr::printErrMsg(dwRet);
  }
  else
  {
    PFILE_ATTRIBUTE_TAG_INFO pInfo = reinterpret_cast<PFILE_ATTRIBUTE_TAG_INFO>(pBuffer);
    pInfo->FileAttributes = bhfi.dwFileAttributes;
    pInfo->ReparseTag = 0;

    const int size(sizeof(FILE_ATTRIBUTE_TAG_INFO));
    setData(pBuffer, size);
    pBuffer += size;
  }
  return dwRet;
}

DWORD cTrans2QueryPathInfo::Stream(const std::wstring &rsPath, PUCHAR &pBuffer) const
{
  BY_HANDLE_FILE_INFORMATION bhfi;
  DWORD dwRet = m_pTreeResource->QueryStandardInfo(rsPath, &bhfi);
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

DWORD cTrans2QueryPathInfo::All(const std::wstring &rsPath, PUCHAR &pBuffer) const
{
  BY_HANDLE_FILE_INFORMATION bhfi;
  DWORD dwRet = m_pTreeResource->QueryStandardInfo(rsPath, &bhfi);
  if (dwRet != ERROR_SUCCESS)
  {
    if (String::npos != rsPath.find(L"destop.ini"))
      cPathInfoErr::printErrMsg(dwRet);
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

    pSmbInfo->FileNameLength = rsPath.size() * sizeof(WCHAR);
    memcpy(pSmbInfo->FileName, rsPath.c_str(), pSmbInfo->FileNameLength);
    const USHORT dataLength = static_cast<USHORT>(sizeof(smbFindFileAllInfo) + pSmbInfo->FileNameLength - sizeof(WCHAR));
    setData(pBuffer, dataLength);

    pBuffer += dataLength;
  }

  return dwRet;
}

DWORD cTrans2QueryPathInfo::Compression(const std::wstring &rsPath, PUCHAR &pBuffer) const
{
  BY_HANDLE_FILE_INFORMATION bhfi;
  DWORD dwRet = m_pTreeResource->QueryStandardInfo(rsPath, &bhfi);
  if (dwRet != ERROR_SUCCESS)
  {
    if (String::npos != rsPath.find(L"destop.ini"))
      cPathInfoErr::printErrMsg(dwRet);
  }
  else
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

DWORD cTrans2QueryPathInfo::UnixBasic(const std::wstring &rsPath, PUCHAR &pBuffer) const
{
  BY_HANDLE_FILE_INFORMATION bhfi;
  DWORD dwRet = m_pTreeResource->QueryStandardInfo(rsPath, &bhfi);
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