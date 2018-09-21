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

#include "QKernelExec.h"
#include <QWinBase.h>

using namespace std;
using namespace vfs;




// =======================================================================================
// === cFile =============================================================================
// =======================================================================================

class cFile : public iFile, public cRefCount
{
	public :

    friend class cExtendFileThread;

    QDEFINE_SMARTPTR(cFile);

    // iFile
    UInt64          QAPI getSize() const;
    cTime           QAPI getModificationTime() const;
    cDate           QAPI getModificationDate() const;
    String          QAPI getDrive() const;
    String          QAPI getPath() const;
    String          QAPI getName() const;
    String          QAPI getExtension() const;
    String          QAPI getAbsFilename() const;
    int             QAPI getAccess() const;
    void            QAPI seek(const UInt64 PositionInBytes) throw(cFileError);
    cMemory::Ptr    QAPI read() throw(cFileError);
    cMemory::Ptr    QAPI read(const UInt64 NumBytes) throw(cFileError);
    cMemoryView::Ptr QAPI read(cMemoryView::Ptr DestBytes) throw(cFileError);
    cMemory::Ptr    QAPI read(const UInt64 PositionInBytes, const UInt64 NumBytes) throw(cFileError);
    cMemoryView::Ptr QAPI read(const UInt64 PositionInBytes, cMemoryView::Ptr DestBytes) throw(cFileError);
    void            QAPI write(cMemoryView::ConstPtr SrcBytes) throw(cFileError);
    void            QAPI write(const UInt64 NumBytesToWrite, const char * const BytesToWrite) throw(cFileError);
    void            QAPI write(const UInt64 PositionInBytes, cMemoryView::ConstPtr SrcBytes) throw(cFileError);
    void            QAPI write(const UInt64 PositionInBytes, const UInt64 NumBytesToWrite, const char * const BytesToWrite) throw(cFileError);
    void            QAPI flushWrite() throw(cFileError);
    void            QAPI rename(const String & NewFilename) throw(cFileError);
    unsigned int    QAPI getFastIOAlignment() const throw(cNotFound, cAccessDenied, cFileError);
    int             QAPI getOptions() const;
    void            QAPI extendFileFast(const UInt64 NewSize) throw(cNotSupported, cFileError, cAborted);
    void            QAPI extendFile(const UInt64 NewSize, const UInt64 ValidDataLength, iFile::iExtendCallback::Ptr Callback) throw(cFileError, cAborted);
    UInt64          QAPI getPosition() const;

    // cFile
		cFile(const String & SrcFilename, const int WhichAccessFlags, const int WhichOptionFlags) throw(cFileError, cNotFound, cAccessDenied);
		~cFile();

	private :
    // Disallowed
    cMemory::Ptr    QAPI read(const unsigned int NumBytes) throw(cFileError);// { throw cFileError(__FILE__, __LINE__, L"Disallowed call"); }
    cMemory::Ptr    QAPI read(const unsigned int PositionInBytes, const unsigned int NumBytes) throw(cFileError);// { throw cFileError(__FILE__, __LINE__, L"Disallowed call"); }
    cMemoryView::Ptr QAPI read(const unsigned int PositionInBytes, cMemoryView::Ptr DestBytes) throw(cFileError);// { throw cFileError(__FILE__, __LINE__, L"Disallowed call"); }
    void            QAPI write(const unsigned int NumBytesToWrite, const char * const BytesToWrite) throw(cFileError);// { throw cFileError(__FILE__, __LINE__, L"Disallowed call"); }
    void            QAPI write(const unsigned int PositionInBytes, cMemoryView::ConstPtr SrcBytes) throw(cFileError);// { throw cFileError(__FILE__, __LINE__, L"Disallowed call"); }
    void            QAPI write(const unsigned int PositionInBytes, const unsigned int NumBytesToWrite, const char * const BytesToWrite) throw(cFileError);// { throw cFileError(__FILE__, __LINE__, L"Disallowed call"); }
    void            QAPI write(const UInt64 PositionInBytes, const unsigned int NumBytesToWrite, const char * const BytesToWrite) throw(cFileError);// { throw cFileError(__FILE__, __LINE__, L"Disallowed call"); }
    void            QAPI write(const unsigned int PositionInBytes, const UInt64 NumBytesToWrite, const char * const BytesToWrite) throw(cFileError);// { throw cFileError(__FILE__, __LINE__, L"Disallowed call"); }

    int               m_OptionFlags;
    String            m_UnsplitAbsFilename;

    mutable bool      m_HasSplitFilename;
    mutable String    m_Drive; // IMPORTANT - Use accessor!
    mutable String    m_Path; // IMPORTANT - Use accessor!
    mutable String    m_Name; // IMPORTANT - Use accessor!
    mutable String    m_Extension; // IMPORTANT - Use accessor!

    HANDLE            m_FileHandle;
    UInt64            m_Position;
    
    mutable bool         m_IsBytesPerSectorValid;
    mutable unsigned int m_BytesPerSector;
    
    //cManualResetEvent m_SignalIoComplete;
    
    void _splitFilename() const;
    void _extendFile(const UInt64 NewSize, const UInt64 ValidDataLength, iFile::iExtendCallback::Ptr OptionalCallback) throw(cFileError, cAborted);
    void _reopen(const int WhichAccessFlags, const int WhichOptionsFlags) throw(cFileError, cNotFound, cAccessDenied);
    void _open(const int WhichAccessFlags, const int WhichOptionsFlags) throw(cFileError, cNotFound, cAccessDenied);
    void _close();
};
