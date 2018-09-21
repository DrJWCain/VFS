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
#include "cFileManager.h"
#include "shlwapi.h"
#include "cFile.h"
#include "cSystemInfo.h"
#include <algorithm>

#include <objidl.h>   /* For IPersistFile */
#include <shlobj.h>   /* For IShellLink */

using namespace std;
using namespace vfs;




// =======================================================================================
// === Construction ======================================================================
// =======================================================================================

namespace
{

void _enablePrivilege(const String & Privilege) throw(cWin32Error)
{
  HANDLE hToken;
  if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken) == 0)
  {
    const DWORD LastError = GetLastError();
    throw cWin32Error(__FILE__, __LINE__, L"Tried to OpenProcessToken() on \"TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY\" - returned 0x%x \"%s\"", LastError, win32ErrorToString(LastError).c_str());
  }
  cWin32HandleGuard TokenHandleGuard(hToken);
  TOKEN_PRIVILEGES tkp;
  if (LookupPrivilegeValue(NULL, Privilege.c_str(), &tkp.Privileges[0].Luid) == 0)
  {
    const DWORD LastError = GetLastError();
    throw cWin32Error(__FILE__, __LINE__, L"Tried to LookupPrivilegeValue() on \"SE_MANAGE_VOLUME_NAME\" - returned 0x%x \"%s\"", LastError, win32ErrorToString(LastError).c_str());
  }
  tkp.PrivilegeCount = 1; // one privilege to set 
  tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
  if (AdjustTokenPrivileges(hToken, FALSE /* DisableAllPrivileges? */, &tkp, sizeof(TOKEN_PRIVILEGES), NULL, NULL) == 0)
  {
    const DWORD LastError = GetLastError();
    throw cWin32Error(__FILE__, __LINE__, L"Tried to AdjustTokenPrivileges() - returned 0x%x \"%s\"", LastError, win32ErrorToString(LastError).c_str());
  }
}

}

cFileManager::cFileManager()
{
  try
  {
    // Ensure we have SE_MANAGE_VOLUME_NAME privilege enabled for SetFileValidData to work
    _enablePrivilege(SE_MANAGE_VOLUME_NAME);
  }
  catch (cRecoverable &)
  {
    // Perhaps we're on 2000 or not an administrator
  }
}




// =======================================================================================
// === openFile ==========================================================================
// =======================================================================================

iFile::Ptr QAPI cFileManager::openFile(const String & SrcFilename) throw(cBadFormat, cFileError, cNotFound, cAccessDenied)
{
  return openFile(SrcFilename, fFileAccess_ReadWrite /* access */, 0 /* options */);
}

// ---------------------------------------------------------------------------------------

iFile::Ptr QAPI cFileManager::openFile(const String & SrcFilename, const int WhichAccessFlags) throw(cBadFormat, cFileError, cNotFound, cAccessDenied)
{
  return openFile(SrcFilename, WhichAccessFlags, 0 /* options */);
}

// ---------------------------------------------------------------------------------------

iFile::Ptr QAPI cFileManager::openFile(const String & SrcFilename, const int WhichAccessFlags, const int WhichOptionFlags) throw(cBadFormat, cFileError, cNotFound, cAccessDenied)
{
  QMSG((0, L"Opening file \"%s\", flags 0x%x, options 0x%x", SrcFilename.c_str(), WhichAccessFlags, WhichOptionFlags));
  _validateFilenameFormatting(SrcFilename);
  return new cFile(SrcFilename, WhichAccessFlags, WhichOptionFlags);
}




// =======================================================================================
// === Utility ===========================================================================
// =======================================================================================

namespace
{

void _StringUppercase(String & it)
{
  transform(
	  it.begin(),     // Src random access iterator for first character
	  it.end(),       // Src random access iterator for last character
	  it.begin(),     // Dest random access iterator for first character
	  toupper);         // Operation
}

// ---------------------------------------------------------------------------------------

void _StringLowercase(String & it)
{
  transform(
	  it.begin(),     // Src random access iterator for first character
	  it.end(),       // Src random access iterator for last character
	  it.begin(),     // Dest random access iterator for first character
	  tolower);       // Operation
}

}



// =======================================================================================
// === renameFile ========================================================================
// =======================================================================================

void QAPI cFileManager::renameFile(const String & CurrentFilename, const String & NewFilename) throw(cBadFormat, cFileError)
{
  QMSG((0, L"Renaming file \"%s\" to \"%s\"", CurrentFilename.c_str(), NewFilename.c_str()));

  _validateFilenameFormatting(CurrentFilename);
  _validateFilenameFormatting(NewFilename);
  if (!MoveFile(expandPathEnvVars(CurrentFilename).c_str(), expandPathEnvVars(NewFilename).c_str()))
  {
    const DWORD LastError = GetLastError();
    throw cFileError(__FILE__, __LINE__, L"Tried to MoveFile() from \"%s\" to \"%s\" - returned 0x%x (%s)", CurrentFilename.c_str(), NewFilename.c_str(), LastError, win32ErrorToString(LastError).c_str());
  }
}




// =======================================================================================
// === formatAsPath ======================================================================
// =======================================================================================

// Add trailing '\' if it needs it
String QAPI cFileManager::formatAsPath(const String & ToFormat)
{
  String Result(ToFormat);
  if (Result.size() > 0)
  {
    const Char LastChar = *Result.rbegin();
    if (LastChar != L'\\' && LastChar != L'/') Result += L"\\";
  }
  return Result;
}

// =======================================================================================
// === getShortcutTargetDir ==============================================================
// =======================================================================================

// Turn shortcut LNK file into the actual path to which it refers (Windows specific).
String QAPI cFileManager::getShortcutTargetDir(const String& linkFilename) // modified from MSDN 'Shell Links' page
{ 
  String targetPath; // assume failure 

  CoInitialize(NULL);

  // Get a pointer to the IShellLink interface. 
  IShellLink* psl; 
  HRESULT hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl); 

  if (SUCCEEDED(hres)) 
  { 
    // Get a pointer to the IPersistFile interface. 
    IPersistFile* ppf; 
    hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf); 

    if (SUCCEEDED(hres)) 
    { 
      // Load the shortcut. 
      hres = ppf->Load(linkFilename.c_str(), STGM_READ); 

      if (SUCCEEDED(hres)) 
      { 
        // Get the path to the link target. 
        Char gotPath[MAX_PATH]; 
        WIN32_FIND_DATA findData; 
        hres = psl->GetPath(gotPath, MAX_PATH, (WIN32_FIND_DATA*)&findData, SLGP_UNCPRIORITY); 

        if (SUCCEEDED(hres) && (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
          targetPath = gotPath;
      } 

      // Release the pointer to the IPersistFile interface. 
      ppf->Release(); 
    } 

    // Release the pointer to the IShellLink interface. 
    psl->Release(); 
  } 

  return targetPath; 
}


// =======================================================================================
// === scanFiles =========================================================================
// =======================================================================================

namespace
{

class cScanFiles : public iFileManager::iFileCallback, public cRefCount
{
  public :
	QDEFINE_SMARTPTR(cScanFiles);

  cScanFiles(const bool IsCreatingAbsPaths, const bool IsRecursive, const bool IsListingFolders) :
    m_IsCreatingAbsPaths(IsCreatingAbsPaths), m_IsRecursive(IsRecursive), m_IsListingFolders(IsListingFolders)
  {
  }
	NextNode QAPI onFolder(const String & AbsPath, const String & RelPath, const String & Name, const unsigned int Depth)
	{
	  if (m_IsListingFolders) m_Files.push_back(m_IsCreatingAbsPaths ? AbsPath : RelPath);
	  return m_IsRecursive ? eNextNode_NextChild : eNextNode_NextSibling;
	}
	NextNode QAPI onFile(const String & AbsPath, const String & RelPath, const String & Name, const String & Ext, const unsigned int Depth)
	{
  	m_Files.push_back((m_IsCreatingAbsPaths ? AbsPath : RelPath) + Name + Ext);
	  return eNextNode_NextChild;
	}

	list<String>  m_Files;
	const bool    m_IsCreatingAbsPaths;
	const bool    m_IsRecursive;
	const bool    m_IsListingFolders;
};

}

list<String> QAPI cFileManager::scanFiles(const String & Path, const bool IsCreatingAbsPaths) throw(cBadFormat, cFileError)
{
  QMSG((0, L"Scanning files in \"%s\" (IsCreatingAbsPaths = %d)", Path.c_str(), IsCreatingAbsPaths));
  _validatePathFormatting(Path);
  return _scan(Path, IsCreatingAbsPaths, true /* recursive? */, false /* folders too? */);
}

list<String> cFileManager::_scan(const String & Path, const bool IsCreatingAbsPaths, const bool IsRecursive, const bool IsListingFolders) throw(cFileError)
{
  cScanFiles::Ptr MyEnum = new cScanFiles(IsCreatingAbsPaths, IsRecursive, IsListingFolders);
  enumerateFiles(0, expandPathEnvVars(formatAsPath(Path)), MyEnum);
  return MyEnum->m_Files;
}




// =======================================================================================
// === expandPathEnvVars =================================================================
// =======================================================================================

String cFileManager::expandPathEnvVars(const String & Path) const throw(cFileError)
{
  const DWORD numCharsReqired = ExpandEnvironmentStrings(Path.c_str(), 0, 0);
  Char * const expandedPath = (Char *)_alloca((numCharsReqired + 1) * sizeof(Char));
  if (ExpandEnvironmentStrings(Path.c_str(), expandedPath, numCharsReqired) == 0)
  {
    const DWORD LastError = GetLastError();
    throw cFileError(__FILE__, __LINE__, L"Tried to ExpandEnvironmentStrings() on \"%s\" - returned 0x%x (%s)", Path.c_str(), LastError, win32ErrorToString(LastError).c_str());
  }
  return expandedPath;
}




// =======================================================================================
// === isFileOrFolderCreated =============================================================
// =======================================================================================

bool QAPI cFileManager::isFileOrFolderCreated(const String & Name) const throw(cBadFormat)
{
  _validatePathOrFilenameFormatting(Name);
  return PathFileExists(expandPathEnvVars(Name).c_str()) == TRUE;
}




// =======================================================================================
// === setFileOrFolderAccess/getFileOrFolderAccess =======================================
// =======================================================================================

void QAPI cFileManager::setFileOrFolderAccess(const String & Filename, const int NewAccess) throw(cBadFormat, cFileError)
{
  QMSG((0, L"Setting file or folder access \"%s\" to 0x%x", Filename.c_str(), NewAccess));
  _validatePathOrFilenameFormatting(Filename);
  DWORD NewAttr = 0;
  if ((NewAccess & fFileAccess_Write) == 0) NewAttr |= FILE_ATTRIBUTE_READONLY;
  if (!SetFileAttributes(expandPathEnvVars(Filename).c_str(), NewAttr))
  {
	  throw cFileError(__FILE__, __LINE__, L"Tried to SetFileAttributes() 0x%x on \"%s\" - returned \"%s\"", NewAttr, Filename.c_str(), win32ErrorToString(GetLastError()).c_str());
  }
}

// ---------------------------------------------------------------------------------------

int QAPI cFileManager::getFileOrFolderAccess(const String & Filename) throw(cBadFormat, cFileError)
{
  _validatePathOrFilenameFormatting(Filename);
  DWORD Attr = GetFileAttributes(expandPathEnvVars(Filename).c_str());
  if (Attr == INVALID_FILE_ATTRIBUTES)
  {
	  throw cFileError(__FILE__, __LINE__, L"Tried to GetFileAttributes() \"%s\" - returned \"%s\"", Filename.c_str(), win32ErrorToString(GetLastError()).c_str());
  }

  int Result = fFileAccess_ReadWrite;
  if ((Attr & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY) Result &= ~fFileAccess_Write;

  return Result;
}




// =======================================================================================
// === createFolder ======================================================================
// =======================================================================================

void QAPI cFileManager::createFolder(const String & Path) throw(cFileError, cBadFormat)
{
  QMSG((0, L"Creating folder \"%s\"", Path.c_str()));

  _validatePathFormatting(Path);
  if (!CreateDirectory(expandPathEnvVars(Path).c_str(), NULL))
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
            createFolder(newPath); // recurse here
          }
        }
        if (!CreateDirectory(expandPathEnvVars(Path).c_str(), NULL))
        {
      	  throw cFileError(__FILE__, __LINE__, L"Tried to CreateDirectory() \"%s\" - returned \"%s\"", Path.c_str(), win32ErrorToString(GetLastError()).c_str());
        }
      }
    }
  }
}




// =======================================================================================
// === splitPath =========================================================================
// =======================================================================================

void QAPI cFileManager::splitPath(const String & Path, String * const ResultDrive, String * const ResultPath, String * const ResultName, String * const ResultExtension) throw(cBadFormat)
{
  Char TmpDrive[32];
  Char TmpPath[512];
  Char TmpName[128];
  Char TmpExtension[32];
  (void)_wsplitpath(expandPathEnvVars(Path).c_str(), TmpDrive, TmpPath, TmpName, TmpExtension);
  *ResultDrive     = String(TmpDrive) + L"\\";
  *ResultPath      = TmpPath;
  *ResultName      = TmpName;
  *ResultExtension = TmpExtension;
}




// =======================================================================================
// === isFolder ==========================================================================
// =======================================================================================

bool QAPI cFileManager::isFolder(const String & Name) throw(cBadFormat, cFileError)
{
  _validatePathOrFilenameFormatting(Name);
  DWORD Attr = GetFileAttributes(expandPathEnvVars(Name).c_str());
  if (Attr == INVALID_FILE_ATTRIBUTES)
  {
    throw cNotFound(__FILE__, __LINE__, L"Could not find \"%s\" in order to determine what it is (%s)", Name.c_str(), win32ErrorToString(GetLastError()).c_str());
  }
  return (Attr & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
}




// =======================================================================================
// === copyFileOrFolder ==================================================================
// =======================================================================================

void QAPI cFileManager::copyFileOrFolder(const String & SrcFilename, const String & DstFilename, const int DstAccess) throw(cBadFormat, cNotFound, cFileError)
{
  QMSG((0, L"Copying file or folder \"%s\" to \"%s\", dstaccess flags 0x%x", SrcFilename.c_str(), DstFilename.c_str(), DstAccess));

  _validatePathOrFilenameFormatting(SrcFilename);
  _validatePathOrFilenameFormatting(DstFilename);

  if (!isFileOrFolderCreated(SrcFilename))
  {
    throw cNotFound(__FILE__, __LINE__, L"Source file \"%s\" does not exist", SrcFilename.c_str());
  }

  if (isFolder(SrcFilename))
  {
    createFolder(DstFilename); 
    const String  SrcAsPath(formatAsPath(SrcFilename));
    const String  DstAsPath(formatAsPath(DstFilename));
    const list<String> FolderContents = _scan(SrcAsPath, false /* abs paths? */, false /* recursive? */, true /* folders too? */);
    for (list<String>::const_iterator i = FolderContents.begin(); i != FolderContents.end(); ++i)
    {
      copyFileOrFolder(SrcAsPath + *i, DstAsPath + *i, DstAccess);
    }
  }
  else
  {
    if (CopyFile(expandPathEnvVars(SrcFilename).c_str(), expandPathEnvVars(DstFilename).c_str(), FALSE /* don't fail if exists */) == 0)
    {
	    throw cFileError(__FILE__, __LINE__, L"Tried to CopyFile() \"%s\" to \"%s\"- returned \"%s\"", SrcFilename.c_str(), DstFilename.c_str(), win32ErrorToString(GetLastError()).c_str());
    }
  }
  setFileOrFolderAccess(DstFilename, DstAccess);
}




// =======================================================================================
// === deleteFileOrFolder ================================================================
// =======================================================================================

void QAPI cFileManager::deleteFileOrFolder(const String & Name, const unsigned int Flags) throw(cBadFormat, cFileError, cNotFound, cAccessDenied)
{
  QMSG((0, L"Deleting file or folder \"%s\", flags 0x%x", Name.c_str(), Flags));
  _validatePathOrFilenameFormatting(Name);
  _deleteFileOrFolder(Name, Flags);
}

void cFileManager::_deleteFileOrFolder(const String & Name, const unsigned int Flags) throw(cFileError, cNotFound, cAccessDenied)
{
  if (isFolder(Name))
  {
    const String NameAsPath(formatAsPath(Name));
    const list<String> FilesToDelete = _scan(NameAsPath, true /* abs paths please */, false /* recursive? */, true /* folders too? */);
    for (list<String>::const_iterator i = FilesToDelete.begin(); i != FilesToDelete.end(); ++i)
    {
      if (Flags & fDeleteFileOrFolder_IgnoreErrorsWhenRecursing)
      {
        try { _deleteFileOrFolder(*i, Flags); } catch (cRecoverable &) {}
      }
      else
      {
        _deleteFileOrFolder(*i, Flags);
      }
    }
    QMSG((0, L"Deleting folder \"%s\"", Name.c_str()));
    if (RemoveDirectory(expandPathEnvVars(NameAsPath).c_str()) == 0)
    {
      const DWORD LastError = GetLastError();
      const String LastErrorAsString = win32ErrorToString(LastError);
      switch (LastError)
      {
        case ERROR_FILE_NOT_FOUND :
        case ERROR_PATH_NOT_FOUND :
        {
    	    throw cNotFound(__FILE__, __LINE__, L"An error occured whilst trying to delete \"%s\" (%s)", NameAsPath.c_str(), LastErrorAsString.c_str());
        }
        case ERROR_ACCESS_DENIED :
        {
    	    throw cAccessDenied(__FILE__, __LINE__, L"An error occured whilst trying to delete \"%s\" (%s)", NameAsPath.c_str(), LastErrorAsString.c_str());
        }
        default :
        {
    	    throw cFileError(__FILE__, __LINE__, L"An error occured whilst trying to delete \"%s\" (%s)", NameAsPath.c_str(), LastErrorAsString.c_str());
    	  }
      }
    }
  }
  else
  {
    QMSG((0, L"Deleting file \"%s\"", Name.c_str()));
    if (Flags & fDeleteFileOrFolder_DeleteReadOnly)
    {
      DWORD Attr = GetFileAttributes(expandPathEnvVars(Name).c_str());
      if (Attr == INVALID_FILE_ATTRIBUTES)
      {
	      throw cFileError(__FILE__, __LINE__, L"Tried to GetFileAttributes() \"%s\" - returned \"%s\"", Name.c_str(), win32ErrorToString(GetLastError()).c_str());
      }
      if (SetFileAttributes(Name.c_str(), Attr & ~FILE_ATTRIBUTE_READONLY) == 0)
      {
	      throw cFileError(__FILE__, __LINE__, L"Tried to SetFileAttributes() \"%s\" - returned \"%s\"", Name.c_str(), win32ErrorToString(GetLastError()).c_str());
      }
    }
    if (DeleteFile(expandPathEnvVars(Name).c_str()) == 0)
    {
      const DWORD LastError = GetLastError();
      const String LastErrorAsString = win32ErrorToString(LastError);
      switch (LastError)
      {
        case ERROR_FILE_NOT_FOUND :
        case ERROR_PATH_NOT_FOUND :
        {
    	    throw cNotFound(__FILE__, __LINE__, L"An error occured whilst trying to delete \"%s\" (%s)", Name.c_str(), LastErrorAsString.c_str());
        }
        case ERROR_ACCESS_DENIED :
        {
    	    throw cAccessDenied(__FILE__, __LINE__, L"An error occured whilst trying to delete \"%s\" (%s)", Name.c_str(), LastErrorAsString.c_str());
        }
        default :
        {
    	    throw cFileError(__FILE__, __LINE__, L"An error occured whilst trying to delete \"%s\" (%s)", Name.c_str(), LastErrorAsString.c_str());
    	  }
      }
    }
  }
}




// =======================================================================================
// === setCurrentFolder/getCurrentFolder =================================================
// =======================================================================================

bool QAPI cFileManager::setCurrentFolder(const String & NewFolderPath) throw(cBadFormat, cFileError)
{
  QMSG((0, L"Setting current folder \"%s\"", NewFolderPath.c_str()));
  _validatePathFormatting(NewFolderPath);
  if (_wchdir(expandPathEnvVars(NewFolderPath).c_str()) != -1 && SetCurrentDirectory(NewFolderPath.c_str()))
  {
	  return true;
  }
  else
  {
	  return false;
  }
}

// ---------------------------------------------------------------------------------------

String QAPI cFileManager::getCurrentFolder()
{
  Char Buffer[MAX_PATH];
  if (GetCurrentDirectory(sizeof(Buffer) / sizeof(Char), Buffer) == 0)
  {
    const DWORD LastError = GetLastError();
    throw cFileError(__FILE__, __LINE__, L"Tried to GetCurrentDirectory - returned 0x%x (%s)", LastError, win32ErrorToString(LastError).c_str());
  }

  return String(Buffer) + L"\\";
}




// =======================================================================================
// === enumerateFiles ====================================================================
// =======================================================================================

namespace
{

void _enumerateChildFiles(
  const int                         Flags,
  const Char * const                AbsPath,
  const Char * const                RelPath,
  iFileManager::iFileCallback::Ptr  Enumerator,
  const unsigned int                nDepth) throw(cNotFound)
{
  
  QVERIFY(AbsPath != 0);
  

  HANDLE hFind = INVALID_HANDLE_VALUE;
  iFileManager::iFileCallback::NextNode WhatIsNextNode = iFileManager::iFileCallback::eNextNode_NextChild;

  try
  {
	  WIN32_FIND_DATA FindFileData;
	  const String FilePattern = String(AbsPath) + L"\\*.*";

	  hFind = FindFirstFile(FilePattern.c_str(), &FindFileData);
	  if (hFind == INVALID_HANDLE_VALUE)
	  {
	    throw cFileError(__FILE__, __LINE__, L"Could not find files at path \"%s\" in order to enumerate them", FilePattern.c_str());
	  }

	  do
	  {
	    String ThisFilename(FindFileData.cFileName);
	    String ThisAbsPath(AbsPath);
	    String ThisRelPath(RelPath);

	    // Callee may want results in upper or lower case only.
	    if (Flags & fEnumerateFiles_Uppercase)
	    {
		    _StringUppercase(ThisFilename);
		    _StringUppercase(ThisAbsPath);
	    }
	    else if (Flags & fEnumerateFiles_Lowercase)
	    {
		    _StringLowercase(ThisFilename);
		    _StringLowercase(ThisAbsPath);
	    }

	    if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	    {
		  // Ignore bogus pseudo-directories that are returned by some file systems
		    // (e.g. FAT).
		    if (ThisFilename != L"." && ThisFilename != L"..")
		    {
		      // --- This node is a folder ---------------------------------------------

		      WhatIsNextNode = Enumerator->onFolder(ThisAbsPath + ThisFilename + L"\\", ThisRelPath + ThisFilename + L"\\", ThisFilename, nDepth);

		      // Recursive into the folder if the enumerator wants to explore this
		      // folder nodes children.
		      if (WhatIsNextNode == iFileManager::iFileCallback::eNextNode_NextChild)
		      {
			      _enumerateChildFiles(
			        Flags,
			        String(ThisAbsPath + ThisFilename + L"\\").c_str(),
			        String(ThisRelPath + ThisFilename + L"\\").c_str(),
			        Enumerator,
			        nDepth + 1);
		      }
		    }
	    }
	    else
	    {
		    // --- This node is a file ---------------------------------------------------

		    // Seperate out extension from name in temp string.
		    const String::size_type nExt = ThisFilename.find_first_of(L".");
		    String ThisExtension(L"");

		    if (nExt != String::npos)
		    {
		      ThisExtension = ThisFilename.substr(nExt, ThisFilename.length() - nExt);
		      ThisFilename.resize(nExt);
		    }

		    WhatIsNextNode = Enumerator->onFile(ThisAbsPath.c_str(), ThisRelPath.c_str(), ThisFilename.c_str(), ThisExtension.c_str(), nDepth);
	    }

	  } while (WhatIsNextNode != iFileManager::iFileCallback::eNextNode_Parent && FindNextFile(hFind, &FindFileData));

	  // Clean-up
	  if (FindClose(hFind) == 0)
	  {
      const DWORD LastError = GetLastError();
      throw cFileError(__FILE__, __LINE__, L"Tried to FindClose() handle for \"%s\" - returned 0x%x (%s)", FilePattern.c_str(), LastError, win32ErrorToString(LastError).c_str());
	  }
  }
  catch (cRecoverable &)
  {
	  // Clean-up
	  if (hFind != INVALID_HANDLE_VALUE) (void)FindClose(hFind);
	  throw;
  }
}

}

// ---------------------------------------------------------------------------------------

void QAPI cFileManager::enumerateFiles(const int Flags, const String & Path, iFileCallback::Ptr Enumerator) throw(cBadFormat, cFileError)
{
  QMSG((0, L"Enumerating files in \"%s\", flags 0x%x", Path.c_str(), Flags));
  _validatePathFormatting(Path);
  _enumerateChildFiles(Flags, expandPathEnvVars(Path).c_str(), L"", Enumerator, 0);
}




// =======================================================================================
// === deleteTemporaryFolders ============================================================
// =======================================================================================

String QAPI cFileManager::createTemporaryFolder(const String & Prefix) throw(cFileError)
{
  QMSG((0, L"Creating temporary folder (prefix \"%s\")", Prefix.c_str()));
  Char Win32TempPath[MAX_PATH];
  if (GetTempPath(MAX_PATH - 1, Win32TempPath) == 0)
  {
    const DWORD LastError = GetLastError();
    throw cFileError(__FILE__, __LINE__, L"Tried to GetTempPath() - returned 0x%x (%s)", LastError, win32ErrorToString(LastError).c_str());
  }
  const String NewFolder(String(Win32TempPath) + Prefix + L"_" + cGuid::createUnique(L"").toString(false /* no comment */));
  createFolder(NewFolder);
  return NewFolder +  L"\\";
}

// ---------------------------------------------------------------------------------------

namespace
{

class cDeleteTemporaryFolders : public iFileManager::iFileCallback, public cRefCount
{
  public :
	QDEFINE_SMARTPTR(cDeleteTemporaryFolders);

  cDeleteTemporaryFolders(const String & Prefix) : m_Prefix(Prefix) {}

	NextNode QAPI onFolder(const String & AbsPath, const String & RelPath, const String & Name, const unsigned int Depth)
	{
	  if (Name.substr(0, m_Prefix.size()) == m_Prefix) m_Files.push_back(AbsPath);
	  return eNextNode_NextSibling; // Don't descend into sub-folders
	}

	NextNode QAPI onFile(const String & AbsPath, const String & RelPath, const String & Name, const String & Ext, const unsigned int Depth)
	{
	  if (Name.substr(0, m_Prefix.size()) == m_Prefix) m_Files.push_back(AbsPath + Name + Ext);
	  return eNextNode_NextChild;
	}

	list<String> m_Files;
	const String m_Prefix;
};

}

void QAPI cFileManager::deleteTemporaryFolders(const String & Prefix, const unsigned int Flags) throw(cFileError)
{
  QMSG((0, L"Deleting temporary folders (prefix \"%s\")", Prefix.c_str()));

  Char Win32TempPath[MAX_PATH];
  if (GetTempPath(MAX_PATH - 1, Win32TempPath) == 0)
  {
    const DWORD LastError = GetLastError();
    throw cFileError(__FILE__, __LINE__, L"Tried to GetTempPath() - returned 0x%x (%s)", LastError, win32ErrorToString(LastError).c_str());
  }

  cDeleteTemporaryFolders::Ptr MyEnum = new cDeleteTemporaryFolders(toLower(Prefix));
  enumerateFiles(fEnumerateFiles_Lowercase, formatAsPath(Win32TempPath), MyEnum);
  for (list<String>::const_iterator i = MyEnum->m_Files.begin(); i != MyEnum->m_Files.end(); ++i)
  {
    if (Flags & fDeleteFileOrFolder_IgnoreErrorsWhenRecursing)
    {
      try { deleteFileOrFolder(*i, Flags); } catch (cRecoverable &) {}
    }
    else
    {
      deleteFileOrFolder(*i, Flags);
    }
  }
}




// =======================================================================================
// === _validatePathFormatting/FilenameFormatting/PathOrFilenameFormatting ===============
// =======================================================================================

void cFileManager::_validatePathFormatting(const String & Candidate) const throw(cBadFormat)
{
  _validatePathOrFilenameFormatting(Candidate);
}

// ---------------------------------------------------------------------------------------

void cFileManager::_validateFilenameFormatting(const String & Candidate) const throw(cBadFormat)
{
  _validatePathOrFilenameFormatting(Candidate);
}

// ---------------------------------------------------------------------------------------

void cFileManager::_validatePathOrFilenameFormatting(const String & Candidate) const throw(cBadFormat)
{
  if (Candidate.find(L"\"") != String::npos ||
      Candidate.find(L"*") != String::npos ||
      Candidate.find(L"<") != String::npos ||
      Candidate.find(L">") != String::npos ||
      Candidate.find(L"/") != String::npos ||
      Candidate.find(L"?") != String::npos ||
//      Candidate.find(L":") != String::npos ||
      Candidate.find(L"|") != String::npos)
  {
    throw cBadFormat(__FILE__, __LINE__,  L"\"%s\" contains an invalid character", Candidate.c_str());
  }
  if (Candidate.size() > 0 && *Candidate.rbegin() == L' ')
  {
    throw cBadFormat(__FILE__, __LINE__,  L"\"%s\" cannot end with space", Candidate.c_str());
  }
}




// =======================================================================================
// === getDiskSize =======================================================================
// =======================================================================================

Int64 QAPI cFileManager::getDiskSize(const String & Path) throw(cBadFormat, cFileError)
{
  QMSG((0, L"Getting disk size at \"%s\"", Path.c_str()));

  _validatePathFormatting(Path);

  ULARGE_INTEGER freeBytesAvailable;      // bytes available to caller
  ULARGE_INTEGER totalNumberOfBytes;      // bytes on disk
  ULARGE_INTEGER totalNumberOfFreeBytes;  // free bytes on disk
  if (!GetDiskFreeSpaceEx(expandPathEnvVars(Path).c_str(), &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes))
  {
    const DWORD LastError = GetLastError();
    throw cFileError(__FILE__, __LINE__, L"Tried to GetDiskFreeSpaceEx() on \"%s\" - returned %d (%s)", Path.c_str(), LastError, win32ErrorToString(LastError).c_str());
  }
  
  return totalNumberOfBytes.QuadPart;
}




// =======================================================================================
// === getDiskAvailableSize ==============================================================
// =======================================================================================

Int64 QAPI cFileManager::getDiskAvailableSize(const String & Path) throw(cBadFormat, cFileError)
{
  QMSG((0, L"Getting available disk size at \"%s\"", Path.c_str()));

  _validatePathFormatting(Path);

  ULARGE_INTEGER freeBytesAvailable;      // bytes available to caller
  ULARGE_INTEGER totalNumberOfBytes;      // bytes on disk
  ULARGE_INTEGER totalNumberOfFreeBytes;  // free bytes on disk
  if (!GetDiskFreeSpaceEx(expandPathEnvVars(Path).c_str(), &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes))
  {
    const DWORD LastError = GetLastError();
    throw cFileError(__FILE__, __LINE__, L"Tried to GetDiskFreeSpaceEx() on \"%s\" - returned %d (%s)", Path.c_str(), LastError, win32ErrorToString(LastError).c_str());
  }
  
  return freeBytesAvailable.QuadPart;
}




// =======================================================================================
// === whereIsFileOrFolder ===============================================================
// =======================================================================================

eFileLocation QAPI cFileManager::whereIsFileOrFolder(const String & Filename) throw(cFileError)
{
  _validatePathOrFilenameFormatting(Filename);
  eFileLocation Result = eFileLocation_Undetermined;
  switch (GetDriveType(expandPathEnvVars(Filename).c_str()))
  {
    case DRIVE_REMOTE :
    {
      Result = eFileLocation_Remote;
      break;
    } 
    case DRIVE_REMOVABLE :
    case DRIVE_FIXED :
    case DRIVE_CDROM :
    case DRIVE_RAMDISK :
    {
      Result = eFileLocation_Local;
      break;
    } 
    case DRIVE_UNKNOWN :
    case DRIVE_NO_ROOT_DIR :
    {
      break;
    }
  }
  return Result;
}
