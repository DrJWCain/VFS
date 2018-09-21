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

//writeInfotemplates.h

#pragma once

#include "..\QCifsFwk\iQCIFSFwk.h"

static ULONG writeFileUnhandled(const PUCHAR pRespBuf, const fileInfo& rFileInfo)
{
  return 0;
}

static void hiAndLowToLG(const ULONG hi, const ULONG low, ULONGLONG& lg)
{
  ULARGE_INTEGER lg_tmp;
  lg_tmp.HighPart = hi;
  lg_tmp.LowPart = low;
  lg = lg_tmp.QuadPart;
}

template <typename DataType, typename RespType>
static void writeAttributes(const DataType& dt, RespType pResp)
{
  hiAndLowToLG(dt.ftCreationTime.dwHighDateTime
    , dt.ftCreationTime.dwLowDateTime
    , pResp->CreationTime);
  hiAndLowToLG(dt.ftLastAccessTime.dwHighDateTime
    , dt.ftLastAccessTime.dwLowDateTime
    , pResp->LastAccessTime);
  hiAndLowToLG(dt.ftLastWriteTime.dwHighDateTime
    , dt.ftLastWriteTime.dwLowDateTime
    , pResp->LastWriteTime);
  pResp->ChangeTime = pResp->LastWriteTime;
  pResp->FileAttributes = dt.dwFileAttributes;
}

template <typename DataType, typename RespType>
static void writeAttributesAndSize(const DataType& dt, RespType pResp)
{
  writeAttributes(dt, pResp);
  hiAndLowToLG(dt.nFileSizeHigh, dt.nFileSizeLow, pResp->EndOfFile);
  pResp->AllocationSize = ((pResp->EndOfFile + DEFAULT_SECTOR_SIZE-1)/DEFAULT_SECTOR_SIZE)*DEFAULT_SECTOR_SIZE;
}

template <typename T>
static ULONG writeFileInformation(const PUCHAR pRespBuf, const fileInfo& rFileInfo)
{
  T* const pFI = reinterpret_cast<T* >(pRespBuf);
  pFI->NextEntryOffset = 0;
  pFI->FileIndex = 0;
  const String sFilename(rFileInfo.w32Data.cFileName);
  pFI->FileNameLength = (ULONG)(sFilename.size()*sizeof(WCHAR));
  memcpy(reinterpret_cast<PUCHAR>(pFI+1)
    , sFilename.c_str()
    , pFI->FileNameLength);
  return sizeof(T) + pFI->FileNameLength;
}

template <typename T>
static ULONG writeFileAttributeInformation(const PUCHAR pRespBuf, const fileInfo& rFileInfo)
{
  writeAttributesAndSize(rFileInfo.w32Data, reinterpret_cast<T*>(pRespBuf));
  return writeFileInformation<T>(pRespBuf, rFileInfo);
}

template <typename T>
static ULONG writeFileEAInformation(const PUCHAR pRespBuf, const fileInfo& rFileInfo)
{
  T* const pFI = reinterpret_cast<T* >(pRespBuf);
  pFI->EaSize = 0;
  return writeFileAttributeInformation<T>(pRespBuf, rFileInfo);
}

template <typename T>
static ULONG writeFileBothInformation(const PUCHAR pRespBuf, const fileInfo& rFileInfo)
{
  T* const pFI = reinterpret_cast<T* >(pRespBuf);
  pFI->ShortNameLength = 0;
  pFI->Reserved = 0;
  ZeroMemory(pFI->ShortName, sizeof(pFI->ShortName)/sizeof(pFI->ShortName[0]));
  return writeFileEAInformation<T>(pRespBuf, rFileInfo);
}

template <typename T>
static ULONG writeFileIdBothDirectoryInformation(const PUCHAR pRespBuf, const fileInfo& rFileInfo)
{
  T* const pFI = reinterpret_cast<T* >(pRespBuf);
  pFI->FileId = 0;
  pFI->Reserved1 = 0;
  pFI->Reserved2 = 0;
  pFI->ShortNameLength = 0;
  ZeroMemory(pFI->ShortName, sizeof(pFI->ShortName)/sizeof(pFI->ShortName[0]));
  return writeFileEAInformation<T>(pRespBuf, rFileInfo);
}

template <typename T>
static ULONG writeFileIdDirectoryInformation(const PUCHAR pRespBuf, const fileInfo& rFileInfo)
{
  T* const pFI = reinterpret_cast<T* >(pRespBuf);
  pFI->FileId = 0;
  return writeFileEAInformation<T>(pRespBuf, rFileInfo);
}