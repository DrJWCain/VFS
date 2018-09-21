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

//cTrans2Find.cpp

#include "StdAfx.h"

#if CIFS 

#include ".\ctrans2find.h"
#include "cSMBResponse.h"
#include "cSMBSession.h"
#include "trans2Structs.h"

cTrans2Find::cTrans2Find(const cPtr<iTreeResource> pTreeResource
  , const int maxData
  , const PREQ_FIND_FIRST2 pReq
  , const cPtr<cSMBResponse> pResponse)
: cSizeOnDisk(pTreeResource)
, m_pFileIndex(new unsigned int)
, m_pFileInfoList(new tFileInfoList)
, m_pSearchPos(new tFileInfoList::const_iterator)
, m_maxSearch(pReq->SearchCount)
, m_maxData(maxData)
{
  *m_pFileIndex = 0;
#if(_MSC_VER < 1500)//prior to VS 2008
  *m_pSearchPos = 0;
#endif
  *m_pSearchPos = m_pFileInfoList->end();
}

cTrans2Find::cTrans2Find(const cPtr<iTreeResource> pTreeResource
  , cTrans2Find *pOther)
: cSizeOnDisk(pTreeResource)
, m_pFileInfoList(pOther->m_pFileInfoList)
, m_pFileIndex(pOther->m_pFileIndex)
, m_pSearchPos(pOther->m_pSearchPos)
, m_maxData(pOther->m_maxData)
, m_maxSearch(pOther->m_maxSearch)
{
}

cTrans2Find::~cTrans2Find(void)
{
}

cPtr<cSMBResponse> cTrans2Find::doFind(const USHORT level, PUCHAR &pData, const cPtr<cSMBResponse> pResponse) const
{
  switch (level)
  {
  case SMB_FIND_FILE_DIRECTORY_INFO:
    return FileDirectory(pData, pResponse);
    break;
  case SMB_FIND_FILE_FULL_DIRECTORY_INFO:
    return FileFullDirectory(pData, pResponse);
    break;
  case SMB_FIND_FILE_BOTH_DIRECTORY_INFO:
    return FileBothDirectory(pData, pResponse);
    break;
  case SMB_FIND_FILE_ID_FULL_DIRECTORY_INFO:
    return FileIDFullDirectory(pData, pResponse);
    break;
  case SMB_FIND_FILE_ID_BOTH_DIRECTORY_INFO:
    return FileIDBothDirectory(pData, pResponse);
    break;
  case SMB_FIND_FILE_UNIX:
    return FileUnix(pData, pResponse);
    break;
  default:
    return processUnhandled(pData, pResponse);
    break;
  }
}

cPtr<cSMBResponse> cTrans2Find::processUnhandled(PUCHAR &pData, const cPtr<cSMBResponse> pResponse) const
{
  pResponse->setDosError(SMB_ERR_CLASS_DOS, SMB_ERR_BAD_FUNCTION);
  return pResponse;
}

cPtr<cSMBResponse> cTrans2Find::FileFullDirectory(PUCHAR &pData, const cPtr<cSMBResponse> pResponse) const
//[#1] p.102
{
  unsigned int nListedSoFar(0);
  PsmbFindFileFullDirectoryInfo ffLastInfo(NULL);

  // search from wherever the search iterator's current position
  for (
    ; (*m_pSearchPos != m_pFileInfoList->end()) && (nListedSoFar < m_maxSearch)
    ; ++*m_pSearchPos)
  {
    const fileInfo &krFI = **m_pSearchPos;

    // determine length attributes
    const int fileNameLength = static_cast<const int>(wcslen(krFI.w32Data.cFileName)*sizeof(WCHAR));
    const int entrySize = sizeof(smbFindFileBothDirectoryInfo) + fileNameLength;

    const unsigned int replySize = checked_static_cast<unsigned int>(pData - reinterpret_cast<PUCHAR>(pResponse->Header));

    if ((replySize + entrySize) < m_maxData)
    {
      // write atributes into structure template
      PsmbFindFileFullDirectoryInfo ffInfo = reinterpret_cast<PsmbFindFileFullDirectoryInfo>(pData);
      ffInfo->NextEntryOffset = entrySize;
      ffInfo->FileIndex = 0;//m_fileIndex;
      ffInfo->CreationTime.QuadPart = makeLg(krFI.w32Data.ftCreationTime);
      ffInfo->LastAccessTime.QuadPart = makeLg(krFI.w32Data.ftLastAccessTime);
      ffInfo->LastWriteTime.QuadPart = makeLg(krFI.w32Data.ftLastWriteTime);
      ffInfo->ChangeTime.QuadPart = makeLg(krFI.w32Data.ftLastWriteTime);
      ffInfo->EndOfFile.QuadPart = makeLg(krFI.w32Data.nFileSizeHigh, krFI.w32Data.nFileSizeLow);
      ffInfo->AllocationSize = getSizeOnDisk(krFI.w32Data.nFileSizeHigh, krFI.w32Data.nFileSizeLow);
      ffInfo->ExtFileAttributes = krFI.w32Data.dwFileAttributes;
      ffInfo->FileNameLength = fileNameLength;
      ffInfo->EaSize = 0;
      wcsncpy(ffInfo->FileName, krFI.w32Data.cFileName, ffInfo->FileNameLength);

      // increment iterator and data pointer
      (*m_pFileIndex)++;
      nListedSoFar++;
      pData += entrySize;

      ffLastInfo = ffInfo;
    }
    else
      break;
  }

  if (NULL != ffLastInfo)
    ffLastInfo->NextEntryOffset = 0;

  return pResponse;
}

cPtr<cSMBResponse> cTrans2Find::FileBothDirectory(PUCHAR &pData, const cPtr<cSMBResponse> pResponse) const
//[#1] p.102
{
  unsigned int nListedSoFar(0);
  PsmbFindFileBothDirectoryInfo ffLastInfo(NULL);

  // search from wherever the search iterator's current position
  for (
    ; (*m_pSearchPos != m_pFileInfoList->end()) && (nListedSoFar < m_maxSearch)
    ; ++*m_pSearchPos)
  {
    const fileInfo &krFI = **m_pSearchPos;

    // determine length attributes
    const int fileNameLength = static_cast<const int>(wcslen(krFI.w32Data.cFileName)*sizeof(WCHAR));
    const int entrySize = sizeof(smbFindFileBothDirectoryInfo) + fileNameLength;

    const unsigned int replySize = checked_static_cast<unsigned int>(pData - reinterpret_cast<PUCHAR>(pResponse->Header));

    if ((replySize + entrySize) < m_maxData)
    {
      // write atributes into structure template
      PsmbFindFileBothDirectoryInfo ffInfo = reinterpret_cast<PsmbFindFileBothDirectoryInfo>(pData);
      ffInfo->NextEntryOffset = entrySize;
      ffInfo->FileIndex = 0;//m_fileIndex;
      ffInfo->CreationTime.QuadPart = makeLg(krFI.w32Data.ftCreationTime);
      ffInfo->LastAccessTime.QuadPart = makeLg(krFI.w32Data.ftLastAccessTime);
      ffInfo->LastWriteTime.QuadPart = makeLg(krFI.w32Data.ftLastWriteTime);
      ffInfo->ChangeTime.QuadPart = makeLg(krFI.w32Data.ftLastWriteTime);
      ffInfo->EndOfFile.QuadPart = makeLg(krFI.w32Data.nFileSizeHigh, krFI.w32Data.nFileSizeLow);
      ffInfo->AllocationSize = getSizeOnDisk(krFI.w32Data.nFileSizeHigh, krFI.w32Data.nFileSizeLow);
      ffInfo->ExtFileAttributes = krFI.w32Data.dwFileAttributes;
      ffInfo->FileNameLength = fileNameLength;
      ffInfo->EaSize = 0;
      ffInfo->ShortNameLength = min(sizeof(ffInfo->ShortName), static_cast<UCHAR>(wcslen(krFI.w32Data.cAlternateFileName)*sizeof(WCHAR)));
      ffInfo->Reserved = 0;
      memset(ffInfo->ShortName, 0, sizeof(ffInfo->ShortName));
      memcpy(ffInfo->ShortName, krFI.w32Data.cAlternateFileName, ffInfo->ShortNameLength);
      wcsncpy(ffInfo->FileName, krFI.w32Data.cFileName, ffInfo->FileNameLength);

      // increment iterator and data pointer
      (*m_pFileIndex)++;
      nListedSoFar++;
      pData += entrySize;

      ffLastInfo = ffInfo;
    }
    else
      break;
  }

  if (NULL != ffLastInfo)
    ffLastInfo->NextEntryOffset = 0;

  return pResponse;
}

cPtr<cSMBResponse> cTrans2Find::FileDirectory(PUCHAR &pData, const cPtr<cSMBResponse> pResponse) const
{
  unsigned int nListedSoFar(0);
  PsmbFindFileBothDirectoryInfo ffLastInfo(NULL);

  // search from wherever the search iterator's current position
  for (
    ; (*m_pSearchPos != m_pFileInfoList->end()) && (nListedSoFar < m_maxSearch)
    ; ++*m_pSearchPos)
  {
    const fileInfo &krFI = **m_pSearchPos;

    // determine length attributes
    const int fileNameLength = static_cast<const int>(wcslen(krFI.w32Data.cFileName)*sizeof(WCHAR));
    const int entrySize = sizeof(smbFindFileDirectoryInfo) + fileNameLength;

    const unsigned int replySize = checked_static_cast<unsigned int>(pData - reinterpret_cast<PUCHAR>(pResponse->Header));

    if ((replySize + entrySize) < m_maxData)
    {
      // write atributes into structure template
      PsmbFindFileDirectoryInfo ffInfo = reinterpret_cast<PsmbFindFileDirectoryInfo>(pData);
      ffInfo->NextEntryOffset = entrySize;
      ffInfo->FileIndex = 0;//m_fileIndex;
      ffInfo->CreationTime.QuadPart = makeLg(krFI.w32Data.ftCreationTime);
      ffInfo->LastAccessTime.QuadPart = makeLg(krFI.w32Data.ftLastAccessTime);
      ffInfo->LastWriteTime.QuadPart = makeLg(krFI.w32Data.ftLastWriteTime);
      ffInfo->ChangeTime.QuadPart = makeLg(krFI.w32Data.ftLastWriteTime);
      ffInfo->EndOfFile.QuadPart = makeLg(krFI.w32Data.nFileSizeHigh, krFI.w32Data.nFileSizeLow);
      ffInfo->AllocationSize = getSizeOnDisk(krFI.w32Data.nFileSizeHigh, krFI.w32Data.nFileSizeLow);
      ffInfo->ExtFileAttributes = krFI.w32Data.dwFileAttributes;
      ffInfo->FileNameLength = fileNameLength;
      wcsncpy(ffInfo->FileName, krFI.w32Data.cFileName, ffInfo->FileNameLength);

      // increment iterator and data pointer
      (*m_pFileIndex)++;
      nListedSoFar++;
      pData += entrySize;
    }
    else
      break;
  }

  if (NULL != ffLastInfo)
    ffLastInfo->NextEntryOffset = 0;

  return pResponse;
}

cPtr<cSMBResponse> cTrans2Find::FileIDFullDirectory(PUCHAR &pData, const cPtr<cSMBResponse> pResponse) const
{
  unsigned int nListedSoFar(0);
  PsmbFindFileIdFullDirectoryInfo ffLastInfo(NULL);

  // search from wherever the search iterator's current position
  for (
    ; (*m_pSearchPos != m_pFileInfoList->end()) && (nListedSoFar < m_maxSearch)
    ; ++*m_pSearchPos)
  {
    const fileInfo &krFI = **m_pSearchPos;

    // determine length attributes
    const int fileNameLength = static_cast<const int>(wcslen(krFI.w32Data.cFileName)*sizeof(WCHAR));
    const int entrySize = sizeof(smbFindFileIdFullDirectoryInfo) + fileNameLength;

    const unsigned int replySize = checked_static_cast<unsigned int>(pData - reinterpret_cast<PUCHAR>(pResponse->Header));

    if ((replySize + entrySize) < m_maxData)
    {
      // write atributes into structure template
      PsmbFindFileIdFullDirectoryInfo ffInfo = reinterpret_cast<PsmbFindFileIdFullDirectoryInfo>(pData);
      ffInfo->NextEntryOffset = entrySize;
      ffInfo->FileIndex = 0;//m_fileIndex;
      ffInfo->CreationTime.QuadPart = makeLg(krFI.w32Data.ftCreationTime);
      ffInfo->LastAccessTime.QuadPart = makeLg(krFI.w32Data.ftLastAccessTime);
      ffInfo->LastWriteTime.QuadPart = makeLg(krFI.w32Data.ftLastWriteTime);
      ffInfo->ChangeTime.QuadPart = makeLg(krFI.w32Data.ftLastWriteTime);
      ffInfo->EndOfFile.QuadPart = makeLg(krFI.w32Data.nFileSizeHigh, krFI.w32Data.nFileSizeLow);
      ffInfo->AllocationSize = getSizeOnDisk(krFI.w32Data.nFileSizeHigh, krFI.w32Data.nFileSizeLow);
      ffInfo->ExtFileAttributes = krFI.w32Data.dwFileAttributes;
      ffInfo->FileNameLength = fileNameLength;
      ffInfo->EASize = 0;
      ffInfo->FileID.QuadPart = 0;
      wcsncpy(ffInfo->FileName, krFI.w32Data.cFileName, ffInfo->FileNameLength);

      // increment iterator and data pointer
      (*m_pFileIndex)++;
      nListedSoFar++;
      pData += entrySize;
    }
    else
      break;
  }

  if (NULL != ffLastInfo)
    ffLastInfo->NextEntryOffset = 0;

  return pResponse;
}

cPtr<cSMBResponse> cTrans2Find::FileIDBothDirectory(PUCHAR &pData, const cPtr<cSMBResponse> pResponse) const
{
  unsigned int nListedSoFar(0);
  PsmbFindFileIdBothDirectoryInfo ffLastInfo(NULL);

  // search from wherever the search iterator's current position
  for (
    ; (*m_pSearchPos != m_pFileInfoList->end()) && (nListedSoFar < m_maxSearch)
    ; ++*m_pSearchPos)
  {
    const fileInfo &krFI = **m_pSearchPos;

    // determine length attributes
    const int fileNameLength = static_cast<const int>(wcslen(krFI.w32Data.cFileName)*sizeof(WCHAR));
    const int entrySize = sizeof(smbFindFileIdBothDirectoryInfo) + fileNameLength;

    const unsigned int replySize = checked_static_cast<unsigned int>(pData - reinterpret_cast<PUCHAR>(pResponse->Header));

    if ((replySize + entrySize) < m_maxData)
    {
      // write atributes into structure template
      PsmbFindFileIdBothDirectoryInfo ffInfo = reinterpret_cast<PsmbFindFileIdBothDirectoryInfo>(pData);
      ffInfo->NextEntryOffset = entrySize;
      ffInfo->FileIndex = 0;//m_fileIndex;
      ffInfo->CreationTime.QuadPart = makeLg(krFI.w32Data.ftCreationTime);
      ffInfo->LastAccessTime.QuadPart = makeLg(krFI.w32Data.ftLastAccessTime);
      ffInfo->LastWriteTime.QuadPart = makeLg(krFI.w32Data.ftLastWriteTime);
      ffInfo->ChangeTime.QuadPart = makeLg(krFI.w32Data.ftLastWriteTime);
      ffInfo->EndOfFile.QuadPart = makeLg(krFI.w32Data.nFileSizeHigh, krFI.w32Data.nFileSizeLow);
      ffInfo->AllocationSize = getSizeOnDisk(krFI.w32Data.nFileSizeHigh, krFI.w32Data.nFileSizeLow);
      ffInfo->ExtFileAttributes = krFI.w32Data.dwFileAttributes;
      ffInfo->FileNameLength = fileNameLength;
      ffInfo->EASize = 0;
      ffInfo->ShortNameLength = 0;
      ZeroMemory(&(ffInfo->ShortNameLength), sizeof(WCHAR)*16);
      ffInfo->FileID.QuadPart = 0;
      wcsncpy(ffInfo->FileName, krFI.w32Data.cFileName, ffInfo->FileNameLength);

      // increment iterator and data pointer
      (*m_pFileIndex)++;
      nListedSoFar++;
      pData += entrySize;
    }
    else
      break;
  }

  if (NULL != ffLastInfo)
    ffLastInfo->NextEntryOffset = 0;

  return pResponse;
}

cPtr<cSMBResponse> cTrans2Find::FileUnix(PUCHAR &pData, const cPtr<cSMBResponse> pResponse) const
{
  unsigned int nListedSoFar(0);
  PSMB_FIND_FILE_UNIX_INFO ffLastInfo(NULL);

  // search from wherever the search iterator's current position
  for (
    ; (*m_pSearchPos != m_pFileInfoList->end()) && (nListedSoFar < m_maxSearch)
    ; ++*m_pSearchPos)
  {
    const fileInfo &krFI = **m_pSearchPos;

    // determine length attributes
    const int fileNameLength = static_cast<const int>(wcslen(krFI.w32Data.cFileName)*sizeof(WCHAR));
    const int entrySize = sizeof(SMB_FIND_FILE_UNIX_INFO) + fileNameLength;

    const unsigned int replySize = checked_static_cast<unsigned int>(pData - reinterpret_cast<PUCHAR>(pResponse->Header));

    if ((replySize + entrySize) < m_maxData)
    {
      // write atributes into structure template
      PSMB_FIND_FILE_UNIX_INFO ffInfo = reinterpret_cast<PSMB_FIND_FILE_UNIX_INFO>(pData);
      ffInfo->NextEntryOffset = entrySize;
      ffInfo->FileIndex = 0;//m_fileIndex;

      ffInfo->UnixInfo.ENDOFFILE = makeLg(krFI.w32Data.nFileSizeHigh, krFI.w32Data.nFileSizeLow);//FILE SIZE
      ffInfo->UnixInfo.NumOfBlocks = 1;//Number of file system block used to store file
      ffInfo->UnixInfo.LastStatusChange = makeLg(krFI.w32Data.ftLastWriteTime);//Last time the status of the file was changed. This is in DCE time
      ffInfo->UnixInfo.LastAccessTime = makeLg(krFI.w32Data.ftLastAccessTime);//Last access time. This is in DCE time.
      ffInfo->UnixInfo.LastModificationTime = makeLg(krFI.w32Data.ftLastWriteTime);// Last modification time. This is in DCE time.
      ffInfo->UnixInfo.Uid = 0;//Numeric user id for the owner
      ffInfo->UnixInfo.Gid = 0;//Numeric group id of owner
      ffInfo->UnixInfo.Type = (krFI.w32Data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? eDirectory : eFile;
      ffInfo->UnixInfo.devmajor = 0;//Major device number if type is device
      ffInfo->UnixInfo.devminor = 0;//Minor device number if type is device
      ffInfo->UnixInfo.uniqueid = 1;
      ffInfo->UnixInfo.permissions = 777;//Standard UNIX file permissions
      ffInfo->UnixInfo.nlinks = 1;//The number of directory entries that map to this entry (number of hard links)

//      ffInfo->FileNameLength = fileNameLength;
      wcsncpy((wchar_t*)ffInfo->FileName, krFI.w32Data.cFileName, fileNameLength);

      // increment iterator and data pointer
      (*m_pFileIndex)++;
      nListedSoFar++;
      pData += entrySize;
    }
    else
      break;
  }

  if (NULL != ffLastInfo)
    ffLastInfo->NextEntryOffset = 0;

  return pResponse;
}


bool cTrans2Find::EndOfSearch() const 
{
#if(_MSC_VER >= 1500 && _MSC_VER < 1900)//VS 2008 - VS 2013
  if(!m_pSearchPos->_Same_container(m_pFileInfoList->end()))
    return false;
#endif
  return *m_pSearchPos == m_pFileInfoList->end();
}

#endif // CIFS  