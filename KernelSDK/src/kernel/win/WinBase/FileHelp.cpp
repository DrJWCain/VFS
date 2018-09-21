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

/*
**  
*/

#include "stdafx.h"
#include <QWinBase.h>
#include <shlobj.h>

using namespace vfs;




// =======================================================================================
// === win32ErrorToString ================================================================
// =======================================================================================

String vfs::getWin32SpecialFolder(const int Where /* e.g. CSIDL_COMMON_PROGRAMS */) throw(cWin32Error)
{
  // Get a pointer to an item ID list that represents the path
  // of a special folder
  LPITEMIDLIST pidl;
  HRESULT hr = SHGetSpecialFolderLocation(NULL, Where, &pidl);
  if (FAILED(hr)) throw cWin32Error(__FILE__, __LINE__, L"Tried to SHGetSpecialFolderLocation() - 0x%x", hr);

  // Convert the item ID list's binary representation into a file
  // system path
  Char szPath[_MAX_PATH];
  (void)SHGetPathFromIDList(pidl, szPath);

  // Get the address of our task allocator's IMalloc interface
  LPMALLOC pMalloc;
  hr = SHGetMalloc(&pMalloc);
  if (FAILED(hr)) throw cWin32Error(__FILE__, __LINE__, L"Tried to SHGetMalloc() - 0x%x", hr);

  // Free the item ID list allocated by SHGetSpecialFolderLocation
  pMalloc->Free(pidl);

  // Free our task allocator
  pMalloc->Release();

  return String(szPath) + L"\\";
}
