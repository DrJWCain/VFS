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

//cWriteInfo.cpp

#include "stdafx.h"

#include "cWriteInfo.h"
#include "iNamed.h"

using namespace vfs;


//convertDateTimetoFiletime
void cWriteInfo::convertDateTimetoFiletime(const cDateTime &dt, FILETIME& rFT) const
{
  SYSTEMTIME st;
  memset(&st, 0, sizeof(st));
  st.wYear = static_cast<WORD>(dt.year());
  st.wMonth = static_cast<WORD>(dt.month());
  st.wDay = static_cast<WORD>(dt.day());
  st.wHour = static_cast<WORD>(dt.hour());
  st.wMinute = static_cast<WORD>(dt.minute());
  st.wSecond = static_cast<WORD>(dt.second());
  st.wMilliseconds = static_cast<WORD>(dt.millisec());
  SystemTimeToFileTime(&st, &rFT);
}

//writeFileInfo
void cWriteInfo::writeFileInfo(PfileInfo pFileInfo
  , const __int64& fileSize
  , const FILETIME& rCreatedTime
  , const FILETIME& rAccessTime
  , const FILETIME& rWriteTime
  , const DWORD dwAtts
  , const String& sName) const
{
  memset(pFileInfo, 0, sizeof(fileInfo));
  pFileInfo->w32Data.dwFileAttributes = dwAtts;
  pFileInfo->w32Data.ftCreationTime = rCreatedTime;
  pFileInfo->w32Data.ftLastAccessTime = rAccessTime;
  pFileInfo->w32Data.ftLastWriteTime = rWriteTime;
  LARGE_INTEGER sz;
  sz.QuadPart = fileSize;
  pFileInfo->w32Data.nFileSizeHigh = sz.HighPart;
  pFileInfo->w32Data.nFileSizeLow = sz.LowPart;
  pFileInfo->allocSize.HighPart = pFileInfo->w32Data.nFileSizeHigh;
  pFileInfo->allocSize.LowPart = pFileInfo->w32Data.nFileSizeLow;
  wcsncpy(pFileInfo->w32Data.cFileName, sName.c_str(), sizeof(pFileInfo->w32Data.cFileName)/sizeof(WCHAR) - 1);
}

void cWriteInfo::writeFileInfo(PfileInfo pFileInfo
  , const BY_HANDLE_FILE_INFORMATION& rBHI
  , const vfs::String& sName) const
{
  memset(&pFileInfo->w32Data, 0, sizeof(WIN32_FIND_DATAW));
  pFileInfo->w32Data.dwFileAttributes = rBHI.dwFileAttributes;
  pFileInfo->w32Data.nFileSizeHigh = rBHI.nFileSizeHigh;
  pFileInfo->w32Data.nFileSizeLow = rBHI.nFileSizeLow;
  pFileInfo->w32Data.ftCreationTime = rBHI.ftCreationTime;
  pFileInfo->w32Data.ftLastAccessTime = rBHI.ftLastAccessTime;
  pFileInfo->w32Data.ftLastWriteTime = rBHI.ftLastWriteTime;
  wcsncpy(pFileInfo->w32Data.cFileName, sName.c_str(), sizeof(pFileInfo->w32Data.cFileName)/sizeof(WCHAR) - 1);
}

void cWriteInfo::writeFileInfo(const LPBY_HANDLE_FILE_INFORMATION pBHI
  , const fileInfo* const pFileInfo) const
{
  memset(pBHI, 0, sizeof(BY_HANDLE_FILE_INFORMATION));
  pBHI->dwFileAttributes = pFileInfo->w32Data.dwFileAttributes;
  pBHI->ftCreationTime = pFileInfo->w32Data.ftCreationTime;
  pBHI->ftLastAccessTime = pFileInfo->w32Data.ftLastAccessTime;
  pBHI->ftLastWriteTime = pFileInfo->w32Data.ftLastWriteTime;
  pBHI->nFileSizeHigh = pFileInfo->w32Data.nFileSizeHigh;
  pBHI->nFileSizeLow = pFileInfo->w32Data.nFileSizeLow;
}

//getAtts
DWORD cWriteInfo::getAtts(const bool bDirectory, const bool bReadonly) const
{
  return (bDirectory ? FILE_ATTRIBUTE_DIRECTORY : FILE_ATTRIBUTE_NORMAL)
    | (bReadonly ? FILE_ATTRIBUTE_READONLY : static_cast<DWORD>(0));
}

//makeFileInfo
fileInfo cWriteInfo::makeFileInfo(const DWORD dwAtts
  , const String& sName) const
{
  FILETIME ft;
  GetSystemTimeAsFileTime(&ft);
  return makeFileInfo(dwAtts, sName, ft);
}

fileInfo cWriteInfo::makeFileInfo(const DWORD dwAtts
  , const cConstPtr<iNamed> pNamed) const
{
  return makeFileInfo(dwAtts, pNamed->Name());
}

fileInfo cWriteInfo::makeFileInfo(DWORD dwAtts
  , const String& sName
  , const FILETIME ft) const
{
  fileInfo fi = makeFileInfo(dwAtts, ft);
  wcsncpy(fi.w32Data.cFileName, sName.c_str(), sizeof(fi.w32Data.cFileName)/sizeof(WCHAR) - 1);
  memset(fi.w32Data.cAlternateFileName, 0, sizeof(fi.w32Data.cAlternateFileName));
  return fi;
}

inline fileInfo cWriteInfo::makeFileInfo(const DWORD dwAtts, const FILETIME ft) const
{
  fileInfo fi;
  memset(&fi, 0, sizeof(fileInfo));
  fi.w32Data.dwFileAttributes = dwAtts;
  fi.w32Data.ftCreationTime = ft;
  fi.w32Data.ftLastAccessTime = ft;
  fi.w32Data.ftLastWriteTime = ft;
  return fi;
}