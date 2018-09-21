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

//CompositeExports.h

#pragma once

#ifdef QCIFS_PROCESSOR
#define QCIFS_EXPORT __declspec(dllexport)
#else
#define QCIFS_EXPORT __declspec(dllimport)
#endif

extern "C" QCIFS_EXPORT iComposite *getFileComposite(const std::wstring &sPath
  , DWORD dwDesiredAccess
  , DWORD dwShareMode
  , LPSECURITY_ATTRIBUTES lpAtts
  , DWORD dwCreationDisposition
  , DWORD dwFlagsAndAtts
  , HANDLE hTemplateFile
  , LPBY_HANDLE_FILE_INFORMATION pFileInfo
  , DWORD &err);

extern "C" QCIFS_EXPORT iComposite *getFolderComposite(const std::wstring ksFilePath
  , LPSECURITY_ATTRIBUTES lpAtts
  , LPBY_HANDLE_FILE_INFORMATION pFileInfo
  , DWORD &err);