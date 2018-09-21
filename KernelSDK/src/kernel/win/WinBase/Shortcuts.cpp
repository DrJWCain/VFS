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
#include "QWinBase.h"
#include <psapi.h>
#include <shlobj.h>
#include <shellapi.h>
#include <winioctl.h>

using namespace vfs;
using namespace std;




// =======================================================================================
// === win32ErrorToString ================================================================
// =======================================================================================

namespace
{

String _expandPathEnvVars(const String & Path) throw(cWin32Error)
{
  const DWORD numCharsReqired = ExpandEnvironmentStrings(Path.c_str(), 0, 0);
  vector<Char> buffer(numCharsReqired + 1);
  Char * expandedPath = &buffer[0];
  if (ExpandEnvironmentStrings(Path.c_str(), expandedPath, numCharsReqired) == 0)
  {
    const DWORD LastError = GetLastError();
    throw cWin32Error(__FILE__, __LINE__, L"Tried to ExpandEnvironmentStrings() on \"%s\" - returned 0x%x (%s)", Path.c_str(), LastError, win32ErrorToString(LastError).c_str());
  }
  return expandedPath;
}

void _createFolder(const String & Path) throw(cWin32Error)
{
//  QMSG((0, L"Creating folder \"%s\"", Path.c_str()));

//  _validatePathFormatting(Path);
  if (!CreateDirectory(_expandPathEnvVars(Path).c_str(), NULL))
  {
    const DWORD LastError = GetLastError();
    if (LastError != ERROR_ALREADY_EXISTS) // directory does not exist
    {
      if (Path.size() > 0)
      {
        String::size_type pos = Path.size() - 1;
        if ((L'\\' == Path[pos]) && (pos > 0))
        {
          --pos; // ignore trailing backslash
        }
        if (L'\\' != Path[pos])
        {
          pos = Path.rfind(L"\\", pos);
          if (String::npos != pos)
          {
            const String newPath = Path.substr(0, pos); // don't include backslash
            _createFolder(newPath); // recurse here
          }
        }
        if (!CreateDirectory(_expandPathEnvVars(Path).c_str(), NULL))
        {
      	  throw cWin32Error(__FILE__, __LINE__, L"Tried to CreateDirectory() \"%s\" - returned \"%s\"", Path.c_str(), win32ErrorToString(GetLastError()).c_str());
        }
      }
    }
  }
}

}

void vfs::createWin32FolderShortcut(const String & DstFolder, const String & Name, const String & TargetFolder, const String & Description) throw(cWin32Error)
{
  #if defined(QMSG)
//    QMSG((0, L"Creating folder shortcut: \"%s\\%s.lnk\" -> \"%s\"", DstFolder.c_str(), Name.c_str(), TargetFolder.c_str()));
  #endif

  (void)CoInitialize(NULL);

  // Get a pointer to the IShellLink interface.
  IShellLink* psl = NULL;

  HRESULT hr = CoCreateInstance(CLSID_FolderShortcut, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, reinterpret_cast<void**>(&psl));
  if (FAILED(hr))
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to CoCreateInstance() - 0x%x", hr);
  }

  psl->SetPath(TargetFolder.c_str());
  psl->SetDescription(Description.c_str());

  // Query IShellLink for the IPersistFile interface for
  // saving the shortcut in persistent storage.
  IPersistFile* ppf = NULL;
  hr = psl->QueryInterface(IID_IPersistFile, reinterpret_cast<void**>(&ppf));
  if (FAILED(hr)) throw cWin32Error(__FILE__, __LINE__, L"Tried to QueryInterface() - 0x%x", hr);

  _createFolder(DstFolder);

  // Save the link by calling IPersistFile::Save.
  const String lnkPath(DstFolder +  L"\\" + Name);
  hr = ppf->Save(lnkPath.c_str(), TRUE);
  if (FAILED(hr))
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to Save() \"%s\" - 0x%x", lnkPath.c_str(), hr);
  }

  ppf->Release();

  psl->Release();
}

void vfs::createWin32FileShortcut(const String & DstFolder, const String & Name, const String & TargetFilename, const String & Parameters, const String & Description) throw(cWin32Error)
{
  #if defined(QMSG)
    QMSG((0, L"Creating file shortcut: \"%s\\%s.lnk\" -> \"%s\" (params \"%s\")", DstFolder.c_str(), Name.c_str(), TargetFilename.c_str(), Parameters.c_str()));
  #endif

  (void)CoInitialize(NULL);

  // Get a pointer to the IShellLink interface.
  IShellLink* psl = NULL;
  HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, reinterpret_cast<void**>(&psl));
  if (FAILED(hr))
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to CoCreateInstance() - 0x%x", hr);
  }

  psl->SetPath(TargetFilename.c_str());
  psl->SetArguments(Parameters.c_str());
  psl->SetDescription(Description.c_str());

  // Query IShellLink for the IPersistFile interface for
  // saving the shortcut in persistent storage.
  IPersistFile* ppf = NULL;
  hr = psl->QueryInterface(IID_IPersistFile, reinterpret_cast<void**>(&ppf));
  if (FAILED(hr)) throw cWin32Error(__FILE__, __LINE__, L"Tried to QueryInterface() - 0x%x", hr);

  _createFolder(DstFolder);

  // Save the link by calling IPersistFile::Save.
  const String lnkPath(DstFolder +  L"\\" + Name + L".lnk");
  hr = ppf->Save(lnkPath.c_str(), TRUE);
  if (FAILED(hr))
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to Save() \"%s\" - 0x%x", lnkPath.c_str(), hr);
  }

  ppf->Release();

  psl->Release();
}

void vfs::deleteWin32FileShortcut(const String & path) throw(cNotFound, cAccessDenied, cWin32Error)
{
  if (DeleteFile(_expandPathEnvVars(path).c_str()) == 0)
  {
    const DWORD LastError = GetLastError();
    const String LastErrorAsString = win32ErrorToString(LastError);
    switch (LastError)
    {
      case ERROR_FILE_NOT_FOUND :
      case ERROR_PATH_NOT_FOUND :
      {
    	  throw cNotFound(__FILE__, __LINE__, L"An error occured whilst trying to delete \"%s\" (%s)", path.c_str(), LastErrorAsString.c_str());
      }
      case ERROR_ACCESS_DENIED :
      {
    	  throw cAccessDenied(__FILE__, __LINE__, L"An error occured whilst trying to delete \"%s\" (%s)", path.c_str(), LastErrorAsString.c_str());
      }
      default :
      {
    	  throw cWin32Error(__FILE__, __LINE__, L"An error occured whilst trying to delete \"%s\" (%s)", path.c_str(), LastErrorAsString.c_str());
    	}
    }
  }
}

#define REPARSE_MOUNTPOINT_HEADER_SIZE   8

typedef struct {
  DWORD ReparseTag;
  DWORD ReparseDataLength;
  WORD Reserved;
  WORD ReparseTargetLength;
  WORD ReparseTargetMaximumLength;
  WORD Reserved1;
  WCHAR ReparseTarget[1];
} REPARSE_MOUNTPOINT_DATA_BUFFER, *PREPARSE_MOUNTPOINT_DATA_BUFFER;

#define DIR_ATTR  (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_REPARSE_POINT)

// Returns directory handle or INVALID_HANDLE_VALUE if failed to open.
// To get extended error information, call GetLastError.

HANDLE _openDirectory(const String & path, BOOL bReadWrite)
{
// Obtain backup/restore privilege in case we don't have it
  HANDLE hToken;
  TOKEN_PRIVILEGES tp;
  ::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken);
  ::LookupPrivilegeValue(NULL,
                         (bReadWrite ? SE_RESTORE_NAME : SE_BACKUP_NAME),
                         &tp.Privileges[0].Luid);
  tp.PrivilegeCount = 1;
  tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
  ::AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
  ::CloseHandle(hToken);

// Open the directory
  DWORD dwAccess = bReadWrite ? (GENERIC_READ | GENERIC_WRITE) : GENERIC_READ;
  HANDLE hDir = ::CreateFile(path.c_str(), dwAccess, 0, NULL, OPEN_EXISTING,
                     FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS, NULL);

  return hDir;
}

bool _isDirectoryJunction(const String & path)
{
  DWORD dwAttr = ::GetFileAttributes(path.c_str());
  if (dwAttr == -1) return false;  // Not exists
  if ((dwAttr & DIR_ATTR) != DIR_ATTR) return false;  // Not dir or no reparse point

  HANDLE hDir = _openDirectory(path.c_str(), FALSE);
  if (hDir == INVALID_HANDLE_VALUE) return false;  // Failed to open directory

  BYTE buf[MAXIMUM_REPARSE_DATA_BUFFER_SIZE];
  REPARSE_MOUNTPOINT_DATA_BUFFER& ReparseBuffer = (REPARSE_MOUNTPOINT_DATA_BUFFER&)buf;
  DWORD dwRet;
  BOOL br = ::DeviceIoControl(hDir, FSCTL_GET_REPARSE_POINT, NULL, 0, &ReparseBuffer,
                                      MAXIMUM_REPARSE_DATA_BUFFER_SIZE, &dwRet, NULL);
  ::CloseHandle(hDir);
  return br ? (ReparseBuffer.ReparseTag == IO_REPARSE_TAG_MOUNT_POINT) : false;
}

void _deleteDirectoryJunction(const String & path)
{
  // Open for reading and writing (see OpenDirectory definition above)
  HANDLE hDir = _openDirectory(path, TRUE);

  BYTE buf[REPARSE_MOUNTPOINT_HEADER_SIZE];
  REPARSE_MOUNTPOINT_DATA_BUFFER& ReparseBuffer = (REPARSE_MOUNTPOINT_DATA_BUFFER&)buf;
  DWORD dwRet;

  memset(buf, 0, sizeof(buf));
  ReparseBuffer.ReparseTag = IO_REPARSE_TAG_MOUNT_POINT;

  if (::DeviceIoControl(hDir, FSCTL_DELETE_REPARSE_POINT, &ReparseBuffer,
                            REPARSE_MOUNTPOINT_HEADER_SIZE, NULL, 0, &dwRet, NULL)) {
    // Success
    ::CloseHandle(hDir);
  }
  else {  // Error
//    DWORD dr = ::GetLastError();
    ::CloseHandle(hDir);
    // Some error action (throw or MessageBox)
  }
}

void vfs::deleteWin32FolderShortcut(const String & path) throw(cNotFound, cAccessDenied, cWin32Error)
{
#if 0
  if (_isDirectoryJunction(path))
  {
    _deleteDirectoryJunction(path);
    return;
  }

  vector<Char> terminatedPath(path.size() + 2);
  memset(&terminatedPath[0], 0, terminatedPath.size() * sizeof(Char));
  wcscpy(&terminatedPath[0], path.c_str());

  SHFILEOPSTRUCT fos;
  memset((void *)&fos, 0, sizeof(fos));
  fos.wFunc = FO_DELETE;
  fos.pFrom = &terminatedPath[0];
  fos.fFlags = FOF_NORECURSEREPARSE;
  const int result = SHFileOperation(&fos);
  if (result)
  {
    throw cWin32Error(__FILE__, __LINE__, L"SHFileOperation failed \"%s\"", path.c_str());
  }
#else

  if (RemoveDirectory(_expandPathEnvVars(path).c_str()) == 0)
  {
    const DWORD LastError = GetLastError();
    const String LastErrorAsString = win32ErrorToString(LastError);
    switch (LastError)
    {
      case ERROR_FILE_NOT_FOUND :
      case ERROR_PATH_NOT_FOUND :
      {
    	  throw cNotFound(__FILE__, __LINE__, L"An error occured whilst trying to delete \"%s\" (%s)", path.c_str(), LastErrorAsString.c_str());
      }
      case ERROR_ACCESS_DENIED :
      {
    	  throw cAccessDenied(__FILE__, __LINE__, L"An error occured whilst trying to delete \"%s\" (%s)", path.c_str(), LastErrorAsString.c_str());
      }
      default :
      {
    	  throw cWin32Error(__FILE__, __LINE__, L"An error occured whilst trying to delete \"%s\" (%s)", path.c_str(), LastErrorAsString.c_str());
    	}
    }
  }

#endif
}