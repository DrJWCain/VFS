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

#pragma once
/*
**  
*/

// --- Headers ---------------------------------------------------------------------------

#include "QKernelExec.h"
#include "cFileManager.h"
#include "cFile.h"

// --- Use Namespace ---------------------------------------------------------------------

using namespace std;
using namespace vfs;




// =======================================================================================
// === cFileManager ======================================================================
// =======================================================================================

class cFileManager : public iFileManager, public cRefCount
{
	public :

    QDEFINE_SINGLETON_IMPL(cFileManager, L"{27184ADE-DE99-4902-A865-49AD312ED80C} // cFileManager", iFileManager);

    // iManager
    void QAPI logSummary() {}
    void QAPI logDetailedSummary() {}

    // iFileManager
    iFile::Ptr QAPI openFile(const String & SrcFilename) throw(cBadFormat, cFileError, cNotFound, cAccessDenied);
    iFile::Ptr QAPI openFile(const String & SrcFilename, const int WhichAccessFlags) throw(cBadFormat, cFileError, cNotFound, cAccessDenied);
    iFile::Ptr QAPI openFile(const String & SrcFilename, const int WhichAccessFlags, const int WhichOptionFlags) throw(cBadFormat, cFileError, cNotFound, cAccessDenied);
    void QAPI enumerateFiles(const int Flags, const String & Path, iFileCallback::Ptr Enumerator) throw(cBadFormat, cFileError);
    std::list<String> QAPI scanFiles(const String & Path, const bool IsCreatingAbsPaths) throw(cBadFormat, cFileError);
    bool QAPI isFileOrFolderCreated(const String & Filename) const throw(cBadFormat, cFileError);
    void QAPI deleteFileOrFolder(const String & Filename, const unsigned int Flags) throw(cBadFormat, cFileError, cNotFound, cAccessDenied);
    void QAPI createFolder(const String & Path) throw(cBadFormat, cFileError);
    bool QAPI setCurrentFolder(const String & NewFolderPath) throw(cBadFormat, cFileError);
    String QAPI getCurrentFolder();
    void QAPI renameFile(const String & CurrentFilename, const String & NewFilename) throw(cBadFormat, cFileError);
    bool QAPI isFolder(const String & Name) throw(cBadFormat, cFileError);
    void QAPI copyFileOrFolder(const String & SrcFilename, const String & DstFilename, const int DstAccess) throw(cBadFormat, cFileError);
    void QAPI setFileOrFolderAccess(const String & Filename, const int NewAttrib) throw(cBadFormat, cFileError);
    int QAPI getFileOrFolderAccess(const String & Filename) throw(cBadFormat, cFileError);
    String QAPI createTemporaryFolder(const String & Prefix) throw(cFileError);
    void QAPI deleteTemporaryFolders(const String & Prefix, const unsigned int Flags) throw(cFileError);
    eFileLocation QAPI whereIsFileOrFolder(const String & Filename) throw(cFileError);
    Int64 QAPI getDiskSize(const String & Path) throw(cBadFormat, cFileError);
    Int64 QAPI getDiskAvailableSize(const String & Path) throw(cBadFormat, cFileError);
    void QAPI splitPath(const String & Path, String * const ResultDrive, String * const ResultPath, String * const ResultName, String * const ResultExtension) throw(cBadFormat);
    String QAPI formatAsPath(const String & ToFormat);
    String QAPI getShortcutTargetDir(const String & linkFilename);

    // cFileManager
    cFileManager();
    virtual String expandPathEnvVars(const String & Path) const throw(cFileError);
    
  private :

    list<String> _scan(const String & Path, const bool IsCreatingAbsPaths, const bool IsRecursive, const bool IsListingFolders) throw(cNotFound);
    void _deleteFileOrFolder(const String & Name, const unsigned int Flags) throw(cFileError);
    void _validatePathFormatting(const String & Candidate) const throw(cBadFormat);
    void _validateFilenameFormatting(const String & Candidate) const throw(cBadFormat);
    void _validatePathOrFilenameFormatting(const String & Candidate) const throw(cBadFormat);
};
