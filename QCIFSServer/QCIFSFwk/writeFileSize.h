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

//writeFileSize.h

#pragma once

template <typename T>
DWORD writeFileSize(T pFileInfo, const LARGE_INTEGER& fileSize)
{
  pFileInfo->nFileSizeHigh = fileSize.HighPart;
  pFileInfo->nFileSizeLow = fileSize.LowPart;
  return ERROR_SUCCESS;
}

template <typename T>
DWORD writeFileSize(T pFileInfo, const unsigned __int64& fileSize)
{
  LARGE_INTEGER lg;
  lg.QuadPart = fileSize;
  pFileInfo->nFileSizeHigh = lg.HighPart;
  pFileInfo->nFileSizeLow = lg.LowPart;
  return ERROR_SUCCESS;
}

template <typename T>
DWORD writeFileSize(T pFileInfo, const unsigned int nSize)
{
  LARGE_INTEGER lg;
  lg.HighPart = 0;
  lg.LowPart = nSize;
  return writeFileSize(pFileInfo, lg);
}

template <typename T, typename U>
DWORD writeFileSize(T pFileInfo, U pMem)
{
  LARGE_INTEGER lg;
  lg.HighPart = 0;
  lg.LowPart = pMem.isValid() ? pMem->getSize() : 0;
  return writeFileSize(pFileInfo, lg);
}