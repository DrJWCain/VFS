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
#include <limits>
#include "cFile.h"
#include "cFileManager.h"
#include "shlwapi.h"

using namespace std;
using namespace vfs;

// XP x64 has a hard limit of 32mb per write call
// see: http://social.msdn.microsoft.com/Forums/en-US/vcgeneral/thread/fef1c9b5-fd92-4ada-8de5-44c2eb30b516/
const unsigned int kMaxWriteSizeLimit = 33525760 - 1024; 


// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

cFile::cFile(const String & Filename, const int WhichAccessFlags, const int WhichOptionsFlags) throw(cFileError, cNotFound, cAccessDenied) :
  m_FileHandle(INVALID_HANDLE_VALUE),
  m_HasSplitFilename(false),
//  m_SignalIoComplete(false /* initially set? */),
  m_IsBytesPerSectorValid(false)
{
  // Split filename
  Char fullPath[512];
  (void)_wfullpath(fullPath, cFileManager::singleton().expandPathEnvVars(Filename).c_str(), (sizeof(fullPath) / sizeof(Char)) - 1);
  m_UnsplitAbsFilename = fullPath;

  _open(WhichAccessFlags, WhichOptionsFlags);
}

// ---------------------------------------------------------------------------------------

cFile::~cFile()
{
  _close();
}




// =======================================================================================
// === getFastIOAlignment ================================================================
// =======================================================================================

unsigned int QAPI cFile::getFastIOAlignment() const throw(cNotFound, cAccessDenied, cFileError)
{
  if (!m_IsBytesPerSectorValid)
  {
    DWORD DontCare;
    if (!GetDiskFreeSpace(
      getDrive().c_str(),                 // -> Full filename
      &DontCare,                          // SectorsPerCluster
      (LPDWORD)&m_BytesPerSector,         // BytesPerSector
      &DontCare,                          // NumberOfFreeClusters
      &DontCare))                         // TotalNumberOfClusters
    {
      const DWORD LastErr = GetLastError();
      switch (LastErr)
      {
        case ERROR_FILE_NOT_FOUND :
        case ERROR_PATH_NOT_FOUND :
        case ERROR_INVALID_DRIVE :
        {
          throw cNotFound(__FILE__, __LINE__, L"Tried to GetDiskFreeSpace() \"%s\" - returned \"%s\"", m_UnsplitAbsFilename.c_str(), win32ErrorToString(LastErr).c_str());
        }
        case ERROR_ACCESS_DENIED :
        case ERROR_INVALID_ACCESS :
        {
          throw cAccessDenied(__FILE__, __LINE__, L"Tried to GetDiskFreeSpace() \"%s\" - returned \"%s\"", m_UnsplitAbsFilename.c_str(), win32ErrorToString(LastErr).c_str());
        }
        default:
        {
          throw cFileError(__FILE__, __LINE__, L"Tried to GetDiskFreeSpace() \"%s\" - returned \"%s\"", m_UnsplitAbsFilename.c_str(), win32ErrorToString(LastErr).c_str());
        }
      }
    }
    m_IsBytesPerSectorValid = true;
  }
  return m_BytesPerSector;
}




// =======================================================================================
// === _open/_close/_reopen ==============================================================
// =======================================================================================

void cFile::_reopen(const int WhichAccessFlags, const int WhichOptionsFlags) throw(cFileError, cNotFound, cAccessDenied)
{
  _close();
  _open(WhichAccessFlags, WhichOptionsFlags);
}

// ---------------------------------------------------------------------------------------

void cFile::_open(const int WhichAccessFlags, const int WhichOptionsFlags) throw(cFileError, cNotFound, cAccessDenied)
{
  QVERIFY(m_FileHandle == INVALID_HANDLE_VALUE);

  m_OptionFlags = WhichOptionsFlags;
  m_Position = 0;

  DWORD AccessMode = 0;
  DWORD ShareMode = FILE_SHARE_READ;
  DWORD CreateDistribFlags = OPEN_EXISTING;
  DWORD MiscFlagsAndAttr = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED;

  if (WhichOptionsFlags & fFileOption_Fast)
  {
    MiscFlagsAndAttr |= FILE_FLAG_NO_BUFFERING;
  }

  if (WhichAccessFlags & fFileAccess_Read)
  {
    AccessMode |= GENERIC_READ;
  }
  if (WhichAccessFlags & fFileAccess_Write)
  {
    AccessMode |= GENERIC_WRITE;
    CreateDistribFlags = OPEN_ALWAYS;
  }

  if (WhichOptionsFlags & fFileOption_Temporary)
  {
    MiscFlagsAndAttr |= FILE_ATTRIBUTE_TEMPORARY;
  }
  if (WhichOptionsFlags & fFileOption_Hidden)
  {
    MiscFlagsAndAttr |= FILE_ATTRIBUTE_HIDDEN;
  }

  if (WhichOptionsFlags & fFileOption_ShareReadWrite)
  {
    ShareMode |= FILE_SHARE_READ | FILE_SHARE_WRITE;
  }

  m_FileHandle = CreateFile(
    m_UnsplitAbsFilename.c_str(),       // -> Full filename
    AccessMode,                         // Access (read-write) mode
    ShareMode,                          // Share mode
    NULL,                               // No security
    CreateDistribFlags,                 // Creation distribution flags
    MiscFlagsAndAttr,                   // File attributes
    NULL);                              // Handle to file with attributes to copy
  if (m_FileHandle == INVALID_HANDLE_VALUE)
  {
    DWORD LastErr = GetLastError();
    switch (LastErr)
    {
      case ERROR_FILE_NOT_FOUND :
      case ERROR_PATH_NOT_FOUND :
      case ERROR_INVALID_DRIVE :
      {
        throw cNotFound(__FILE__, __LINE__, L"Tried to CreateFile() \"%s\" - returned 0x%x \"%s\"", m_UnsplitAbsFilename.c_str(), LastErr, win32ErrorToString(LastErr).c_str());
      }
      case ERROR_ACCESS_DENIED :
      case ERROR_INVALID_ACCESS :
      case ERROR_SHARING_VIOLATION :
      {
        throw cAccessDenied(__FILE__, __LINE__, L"Tried to CreateFile() \"%s\" - returned 0x%x \"%s\"", m_UnsplitAbsFilename.c_str(), LastErr, win32ErrorToString(LastErr).c_str());
      }
      default:
      {
        throw cFileError(__FILE__, __LINE__, L"Tried to CreateFile() \"%s\" - returned 0x%x \"%s\"", m_UnsplitAbsFilename.c_str(), LastErr, win32ErrorToString(LastErr).c_str());
      }
    }
  }
}

// ---------------------------------------------------------------------------------------

void cFile::_close()
{
  if (m_FileHandle != INVALID_HANDLE_VALUE)
  {
    QMSG((0, L"Closing file \"%s\"", m_UnsplitAbsFilename.c_str()));
    (void)CloseHandle(m_FileHandle);
    m_FileHandle = INVALID_HANDLE_VALUE;
  }
}




// =======================================================================================
// === getModificationTime/getModificationDate ===========================================
// =======================================================================================

cTime QAPI cFile::getModificationTime() const
{
  WIN32_FILE_ATTRIBUTE_DATA FileAttr = { 0 };
  if (GetFileAttributesEx(
    m_UnsplitAbsFilename.c_str(),       // File or directory name
    GetFileExInfoStandard,              // Attribute
    &FileAttr) == 0)                    // Attribute information
  {
    QFATAL((__FILE__, __LINE__, L"Tried to GetFileAttributesEx() on \"%s\"", m_UnsplitAbsFilename.c_str()));
  }

  SYSTEMTIME UniversalTime;
  if (FileTimeToSystemTime(
    &FileAttr.ftLastWriteTime,          // File time to convert
    &UniversalTime) == 0)                // Receives system time
  {
    QFATAL((__FILE__, __LINE__, L"Tried to FileTimeToSystemTime() for \"%s\"", m_UnsplitAbsFilename.c_str()));
  }

  TIME_ZONE_INFORMATION tz;
  if (GetTimeZoneInformation(&tz) == TIME_ZONE_ID_INVALID)
  {
    QFATAL((__FILE__, __LINE__, L"Tried to GetTimeZoneInformation()"));
  }

  SYSTEMTIME LocalTime;
  if (SystemTimeToTzSpecificLocalTime(
    &tz,                    // time zone
    &UniversalTime,         // universal time
    &LocalTime) == 0)       // local time
  {
    QFATAL((__FILE__, __LINE__, L"Tried to SystemTimeToTzSpecificLocalTime()"));
  }

  return cTime(LocalTime.wHour, LocalTime.wMinute, LocalTime.wSecond, 0);
}

// ---------------------------------------------------------------------------------------

cDate QAPI cFile::getModificationDate() const
{
  WIN32_FILE_ATTRIBUTE_DATA FileAttr = { 0 };
  if (GetFileAttributesEx(
    m_UnsplitAbsFilename.c_str(),                 // File or directory name
    GetFileExInfoStandard,              // Attribute
    &FileAttr) == 0)                    // Attribute information
  {
    QFATAL((__FILE__, __LINE__, L"Tried to GetFileAttributesEx() on \"%s\"", m_UnsplitAbsFilename.c_str()));
  }

  SYSTEMTIME UniversalTime;
  if (FileTimeToSystemTime(
    &FileAttr.ftLastWriteTime,          // File time to convert
    &UniversalTime) == 0)            // Receives system time
  {
    QFATAL((__FILE__, __LINE__, L"Tried to FileTimeToSystemTime() for \"%s\"", m_UnsplitAbsFilename.c_str()));
  }

  return cDate(UniversalTime.wYear, UniversalTime.wMonth, UniversalTime.wDay);
}




// =======================================================================================
// === rename ============================================================================
// =======================================================================================

void QAPI cFile::rename(const String & NewFilename) throw(cFileError)
{
  const String expandedNewFilename(cFileManager::singleton().expandPathEnvVars(NewFilename));
  iFileManager::singleton().renameFile(m_UnsplitAbsFilename, expandedNewFilename);

  Char fullPath[512];
  (void)_wfullpath(fullPath, expandedNewFilename.c_str(), (sizeof(fullPath) / sizeof(Char)) - 1);
  m_UnsplitAbsFilename = fullPath;
  m_HasSplitFilename = false;
}




// =======================================================================================
// === seek/getPosition ==================================================================
// =======================================================================================

void QAPI cFile::seek(const UInt64 PositionInBytes) throw(cFileError)
{
  m_Position = PositionInBytes;
}

// ---------------------------------------------------------------------------------------

UInt64 QAPI cFile::getPosition() const
{
  return m_Position;
}




// =======================================================================================
// === _splitFilename ====================================================================
// =======================================================================================

void cFile::_splitFilename() const
{
  if (!m_HasSplitFilename)
  {
    iFileManager::singleton().splitPath(m_UnsplitAbsFilename, &m_Drive, &m_Path, &m_Name, &m_Extension);
    m_HasSplitFilename = true;
  }
}




// =======================================================================================
// === getName/getPath/getAccess =========================================================
// =======================================================================================

String QAPI cFile::getDrive() const
{
  _splitFilename();
  return m_Drive;
}

// ---------------------------------------------------------------------------------------

String QAPI cFile::getName() const
{
  _splitFilename();
  return m_Name;
}

// ---------------------------------------------------------------------------------------

String QAPI cFile::getPath() const
{
  _splitFilename();
  return m_Path;
}

// ---------------------------------------------------------------------------------------

String QAPI cFile::getExtension() const
{
  _splitFilename();
  return m_Extension;
}

// ---------------------------------------------------------------------------------------

String QAPI cFile::getAbsFilename() const
{
  return m_UnsplitAbsFilename;
}

// ---------------------------------------------------------------------------------------

int QAPI cFile::getAccess() const
{
  int Result = fFileAccess_ReadWrite;

  WIN32_FILE_ATTRIBUTE_DATA FileAttr = { 0 };
  if (GetFileAttributesEx(
    m_UnsplitAbsFilename.c_str(),       // File or directory name
    GetFileExInfoStandard,              // Attribute
    &FileAttr) == 0)                    // Attribute information
  {
    QFATAL((__FILE__, __LINE__, L"Tried to GetFileAttributesEx() on \"%s\"", m_UnsplitAbsFilename.c_str()));
  }

  if ((FileAttr.dwFileAttributes & FILE_ATTRIBUTE_READONLY)  == FILE_ATTRIBUTE_READONLY) Result &= ~fFileAccess_Write;

  return Result;
}




// =======================================================================================
// === write =============================================================================
// =======================================================================================

void QAPI cFile::write(const UInt64 NumBytesToWrite, const char * const BytesToWrite) throw(cFileError)
{
  write(m_Position, NumBytesToWrite, BytesToWrite);
  m_Position += NumBytesToWrite;
}

// ---------------------------------------------------------------------------------------

void QAPI cFile::write(cMemoryView::ConstPtr SrcBytes) throw(cFileError)
{
  if (SrcBytes->getSize() > 0)
  {
    write((UInt64)SrcBytes->getSize(), SrcBytes->getConstBytes());
  }
}

// ---------------------------------------------------------------------------------------

// m_Position is not modified
void QAPI cFile::write(const UInt64 PositionInBytes, const UInt64 NumBytesToWrite, const char * const BytesToWrite) throw(cFileError)
{
  if (m_OptionFlags & fFileOption_Fast)
  {
    const unsigned int bytesPerSector = getFastIOAlignment();
    if ( (NumBytesToWrite % bytesPerSector) != 0 )              throw cFileError(__FILE__, __LINE__, L"cFile::write: on \"%s\" - data size needs to aligned to %d for fast io", m_UnsplitAbsFilename.c_str(), bytesPerSector);
    if ( ((UInt64)BytesToWrite % (UInt64)bytesPerSector) != 0 ) throw cFileError(__FILE__, __LINE__, L"cFile::write: on \"%s\" - data addr needs to aligned to %d for fast io", m_UnsplitAbsFilename.c_str(), bytesPerSector);
    if ( (PositionInBytes % (UInt64)bytesPerSector) != 0 )      throw cFileError(__FILE__, __LINE__, L"cFile::write: on \"%s\" - file pos needs to aligned to %d for fast io", m_UnsplitAbsFilename.c_str(), bytesPerSector);
  }

  QVERIFY(BytesToWrite);
  if (NumBytesToWrite > 0)
  {
    UInt64 bytePosition = PositionInBytes;
    UInt64 byteCountDown = NumBytesToWrite;
    const char* sourceBytes = BytesToWrite;

    do
    {
      // Calculate bytes to write out based on safe OS maximum
      DWORD byteWriteSize;
      
      if ( byteCountDown >= kMaxWriteSizeLimit )
      {
        byteWriteSize = kMaxWriteSizeLimit;
        byteCountDown -= kMaxWriteSizeLimit;
      }
      else
      {
        byteWriteSize = (DWORD)byteCountDown;
        byteCountDown = 0; // Indicate this is the last block out
      }

      LARGE_INTEGER posAsLargeInt;
      posAsLargeInt.QuadPart = bytePosition;
    
      // Overlapped write info set up
      OVERLAPPED overlapped;
      overlapped.Internal     = 0;
      overlapped.InternalHigh = 0;
      overlapped.Offset       = posAsLargeInt.LowPart;
      overlapped.OffsetHigh   = posAsLargeInt.HighPart;
      cManualResetEvent signalIoComplete(false /* initially set? */);
      overlapped.hEvent       = signalIoComplete.getWin32EventHandle();

      // Write out data
      DWORD numBytesWritten = 0;
      if ( WriteFile( m_FileHandle,
                      sourceBytes,
                      byteWriteSize,
                      &numBytesWritten,
                      &overlapped ) == 0 )
      {
        // Try waiting for write
        if ( GetLastError() == ERROR_IO_PENDING )
        {
          while ( signalIoComplete.wait(20000) == cManualResetEvent::eTimedOut )
          {
            QSOS((L"cFile::write: overlapped io timed out after 20 seconds, retrying"));
          }
          QVERIFY(HasOverlappedIoCompleted(&overlapped));
        }
        else
        {
          const DWORD lastError = GetLastError();
          throw cFileError( __FILE__, __LINE__,
                            L"Tried to WriteFile() on \"%s\" - returned 0x%x \"%s\"",
                            m_UnsplitAbsFilename.c_str(),
                            lastError,
                            win32ErrorToString(lastError).c_str() );
        }
      }

      if ( GetOverlappedResult( m_FileHandle,
                                &overlapped,
                                &numBytesWritten, 
                                FALSE /* wait io complete? */) == 0 )
      {
        const DWORD lastError = GetLastError();      
        throw cFileError( __FILE__, __LINE__, 
                          L"Tried to GetOverlappedResult() on \"%s\" - returned 0x%x \"%s\"",
                          m_UnsplitAbsFilename.c_str(),
                          lastError,
                          win32ErrorToString(lastError).c_str() );
      }

      // Update byte positions
      sourceBytes += kMaxWriteSizeLimit;
      bytePosition += kMaxWriteSizeLimit;      
    }
    while ( byteCountDown != 0 );
  }
}

// ---------------------------------------------------------------------------------------

// m_Position is not modified
void QAPI cFile::write(const UInt64 PositionInBytes, cMemoryView::ConstPtr SrcBytes) throw(cFileError)
{
  if (SrcBytes->getSize() > 0)
  {
    write(PositionInBytes, (UInt64)SrcBytes->getSize(), SrcBytes->getConstBytes());
  }
}




// =======================================================================================
// === flushWrite ========================================================================
// =======================================================================================

void QAPI cFile::flushWrite() throw(cFileError)
{
  if (!FlushFileBuffers(m_FileHandle))
  {
    const DWORD LastError = GetLastError();
    throw cFileError(__FILE__, __LINE__, L"Tried to FlushFileBuffers() on \"%s\" - returned 0x%x \"%s\"", m_UnsplitAbsFilename.c_str(), LastError, win32ErrorToString(LastError).c_str());
  }
}




// =======================================================================================
// === getSize ===========================================================================
// =======================================================================================

UInt64 QAPI cFile::getSize() const
{
  DWORD lpFileSizeHigh(0);
  DWORD lowSize(0);
  lowSize = GetFileSize(m_FileHandle, &lpFileSizeHigh);
  const UInt64 Result = ((UInt64)lpFileSizeHigh * 0x100000000) + lowSize;
  if (Result == INVALID_FILE_SIZE )
  {
    const DWORD LastError = GetLastError();
    throw cFileError(__FILE__, __LINE__, L"Tried to GetFileSize() \"%s\" - returned 0x%x \"%s\"", m_UnsplitAbsFilename.c_str(), LastError, win32ErrorToString(LastError).c_str());
  }
  return Result;
}




// =======================================================================================
// === read ==============================================================================
// =======================================================================================

cMemory::Ptr cFile::read() throw(cFileError)
{
  seek(0);
  return read(getSize());
}

// ---------------------------------------------------------------------------------------

cMemory::Ptr cFile::read(const UInt64 NumBytes) throw(cFileError)
{
  cMemory::Ptr Result = read(m_Position, NumBytes);
  m_Position += (UInt64)Result->getSize();
  return Result;
}

// ---------------------------------------------------------------------------------------

cMemoryView::Ptr QAPI cFile::read(cMemoryView::Ptr DestBytes) throw(cFileError)
{
  cMemoryView::Ptr Result = read(m_Position, DestBytes);
  m_Position += (UInt64)Result->getSize();
  return Result;
}

// ---------------------------------------------------------------------------------------

// m_Position is not modified
cMemoryView::Ptr QAPI cFile::read(const UInt64 PositionInBytes, cMemoryView::Ptr DestBytes) throw(cFileError)
{
  unsigned int NumBytesToRead = DestBytes->getSize();
  if (m_OptionFlags & fFileOption_Fast)
  {
    const unsigned int BytesPerSector = getFastIOAlignment();
    size_t Offset;
    const size_t AlignedSize = DestBytes->getPrivateMemory(&Offset)->getAlignedSize();
    if (((AlignedSize - Offset) % BytesPerSector) != 0)                throw cFileError(__FILE__, __LINE__, L"cFile::read: on \"%s\" - data size needs to aligned to %d for fast io", m_UnsplitAbsFilename.c_str(), BytesPerSector);
    if (((UInt64)DestBytes->getBytes() % (UInt64)BytesPerSector) != 0) throw cFileError(__FILE__, __LINE__, L"cFile::read: on \"%s\" - data addr needs to aligned to %d for fast io", m_UnsplitAbsFilename.c_str(), BytesPerSector);
    if ((PositionInBytes % (UInt64)BytesPerSector) != 0)               throw cFileError(__FILE__, __LINE__, L"cFile::read: on \"%s\" - file pos needs to aligned to %d for fast io", m_UnsplitAbsFilename.c_str(), BytesPerSector);
    NumBytesToRead = ((NumBytesToRead + BytesPerSector - 1) / BytesPerSector) * BytesPerSector; // Round up
  }

  LARGE_INTEGER PosAsLargeInt;
  PosAsLargeInt.QuadPart = PositionInBytes;
  OVERLAPPED Overlapped;
  Overlapped.Internal     = 0;
  Overlapped.InternalHigh = 0;
  Overlapped.Offset       = PosAsLargeInt.LowPart;
  Overlapped.OffsetHigh   = PosAsLargeInt.HighPart;
  cManualResetEvent SignalIoComplete(false /* initially set? */);
  Overlapped.hEvent       = SignalIoComplete.getWin32EventHandle();
  //m_SignalIoComplete.reset();
  DWORD NumBytesRead = 0;
  if (ReadFile(m_FileHandle, DestBytes->getBytes(), NumBytesToRead, &NumBytesRead, &Overlapped) == 0)
  {
    if (ERROR_IO_PENDING == GetLastError())
    {
      while (SignalIoComplete.wait(20000) == cManualResetEvent::eTimedOut)
      {
        QSOS((L"cFile::read: overlapped io timed out after 20 seconds, retrying"));
      }
      QVERIFY(HasOverlappedIoCompleted(&Overlapped));
    }
    else
    {
      const DWORD LastError = GetLastError();
      throw cFileError(__FILE__, __LINE__, L"Tried to ReadFile() on \"%s\" - returned 0x%x \"%s\"", m_UnsplitAbsFilename.c_str(), LastError, win32ErrorToString(LastError).c_str());
    }
  }
  if (GetOverlappedResult(m_FileHandle, &Overlapped, &NumBytesRead, FALSE /* wait io complete? */) == 0)
  {
    const DWORD LastError = GetLastError();
    throw cFileError(__FILE__, __LINE__, L"Tried to GetOverlappedResult() on \"%s\" - returned 0x%x \"%s\"", m_UnsplitAbsFilename.c_str(), LastError, win32ErrorToString(LastError).c_str());
  }

  return DestBytes->first(Min<size_t>(DestBytes->getSize(), (size_t)NumBytesRead));
}

// ---------------------------------------------------------------------------------------

// m_Position is not modified
cMemory::Ptr QAPI cFile::read(const UInt64 PositionInBytes, const UInt64 NumBytes) throw(cFileError)
{
  QVERIFY(NumBytes <= (UInt64)((std::numeric_limits<size_t>::max)()));
  cMemory::Ptr Result;
  if (m_OptionFlags & fFileOption_Fast)
  {
    Result = new cMemory((size_t)NumBytes, cMemory::eHeap, (size_t)getFastIOAlignment());
  }
  else
  {
    Result = new cMemory((size_t)NumBytes, cMemory::eHeap);
  }
  if (NumBytes > 0) (void)read(PositionInBytes, new cMemoryView(Result));
  return Result;
}




// =======================================================================================
// === getOptions ========================================================================
// =======================================================================================

int QAPI cFile::getOptions() const
{
  return m_OptionFlags;
}




// =======================================================================================
// === extendFileFast ====================================================================
// =======================================================================================

typedef BOOL (__stdcall *setFileValidDataFuncPtr) (HANDLE, LONGLONG); 

void QAPI cFile::extendFileFast(const UInt64 NewLength) throw(cNotSupported, cFileError, cAborted, cNoRoom)
{
  const UInt64 ActualNewLength = Max<UInt64>(NewLength, getSize());

  const setFileValidDataFuncPtr FuncPtr = (setFileValidDataFuncPtr)GetProcAddress(GetModuleHandle(L"kernel32"), "SetFileValidData");
  if (FuncPtr == NULL)
  {
    throw cNotSupported(__FILE__, __LINE__, L"SetFileValidData() not found in kernel32");
  }

  // Extend allocation length
  LARGE_INTEGER NewSizeAsLargeInt;
  NewSizeAsLargeInt.QuadPart = ActualNewLength;
  if (!SetFilePointerEx(m_FileHandle, NewSizeAsLargeInt, NULL, FILE_BEGIN))
  {
    const DWORD LastError = GetLastError();
    throw cFileError(__FILE__, __LINE__, L"Tried to SetFilePointerEx() on \"%s\" to %I64d - returned 0x%x \"%s\"", m_UnsplitAbsFilename.c_str(), ActualNewLength, LastError, win32ErrorToString(LastError).c_str());
  }
  if (SetEndOfFile(m_FileHandle) == 0)
  {
    const DWORD LastError = GetLastError();
    if (LastError == 112)
    {
      throw cFileError(__FILE__, __LINE__, L"No room for file \"%s\" %I64d bytes - returned 0x%x \"%s\"", m_UnsplitAbsFilename.c_str(), ActualNewLength, LastError, win32ErrorToString(LastError).c_str());
    }
    else
    {
      throw cFileError(__FILE__, __LINE__, L"Tried to SetEndOfFile() on \"%s\" - returned 0x%x \"%s\"", m_UnsplitAbsFilename.c_str(), LastError, win32ErrorToString(LastError).c_str());
    }
  }

  // Extend VDL (valid data length)
  if (FuncPtr(m_FileHandle, ActualNewLength) == 0)
  {
    const DWORD LastError = GetLastError();
    if (LastError == ERROR_PRIVILEGE_NOT_HELD)
    {
      throw cNotSupported(__FILE__, __LINE__, L"SE_MANAGE_VOLUME_NAME privilege missing?, tried to SetFileValidData() on \"%s\" for %I64d bytes - returned 0x%x \"%s\"", m_UnsplitAbsFilename.c_str(), NewLength, LastError, win32ErrorToString(LastError).c_str());
    }
    else
    {
      // Perhaps not an NTFS drive?
      throw cFileError(__FILE__, __LINE__, L"Tried to SetFileValidData() on \"%s\" for %I64d bytes - returned 0x%x \"%s\"", m_UnsplitAbsFilename.c_str(), NewLength, LastError, win32ErrorToString(LastError).c_str());
    }
  }
}

class cExtendFileThread : public cBasicThread
{
  public :
    QDEFINE_SMARTPTR(cExtendFileThread);
    cExtendFileThread(cFile::Ptr File, const UInt64 NewSize, const UInt64 ValidDataLength, iFile::iExtendCallback::Ptr OptionalCallback) :
      cBasicThread(L"ExtendFileThread", iBasicThread::ePriority_Normal),
      m_HasThrownAnException(false),
      m_File(File),
      m_NewSize(NewSize),
      m_ValidDataLength(ValidDataLength),
      m_OptionalCallback(OptionalCallback)
    {
      QVERIFY(NewSize >= ValidDataLength);
    }
    void rethrowUnhandledException() throw(cRecoverable) { if (m_HasThrownAnException) throw m_Exception; }
  private :
    // iThread
    void QAPI signalLoopStop() {}
    void QAPI runLoop()
    {
      m_OrigOptionFlags = m_File->getOptions();
      m_OrigAccessFlags = m_File->getAccess();

      try
      {
        const unsigned int BytesPerSector = m_File->getFastIOAlignment();
        const UInt64 StartPos = ((m_ValidDataLength + BytesPerSector - 1) / BytesPerSector) * BytesPerSector; // Round up
        const UInt64 NumBytesToWrite = m_NewSize - StartPos;

        const UInt64 ChunkSize = Min<UInt64>(UInt64(BytesPerSector) * 512UI64, NumBytesToWrite);
        cMemoryView::Ptr Chunk = new cMemoryView(new cMemory((size_t)ChunkSize, cMemory::eFastIO));
        memset(Chunk->getBytes(), 0xff, Chunk->getSize()); // Write non-zeros to avoid Windows optimising the write away

        m_File->_reopen(fFileAccess_ReadWrite, (ChunkSize % BytesPerSector) == 0 ? fFileOption_Fast : 0);
        m_File->seek(StartPos);

        UInt64 NumChunksRemaining = NumBytesToWrite / Chunk->getSize();
        UInt64 NumBytesWritten = 0;
        unsigned int LastPercentageDone = UINT_MAX; // Some bogus number just to force initial callback
        while (NumChunksRemaining > 0)
        {
//          QTRACE((L"Formatting %I64d, %d bytes", StartPos + NumBytesWritten, Chunk->getSize()));
          m_File->write(Chunk);
          NumBytesWritten += Chunk->getSize();
          NumChunksRemaining--;

          const unsigned int PercentageDone = (unsigned int)(100.0 * ((double)NumBytesWritten / (double)NumBytesToWrite));
          if (PercentageDone != LastPercentageDone)
          {
            if (m_OptionalCallback.isValid()) m_OptionalCallback->onExtendProgress(PercentageDone);
          }
          LastPercentageDone = PercentageDone;
        }
        
        // Write remainder if any
        if (NumBytesWritten < NumBytesToWrite)
        {
          Chunk = new cMemoryView(new cMemory((size_t)(NumBytesToWrite - NumBytesWritten), cMemory::eHeap));
          memset(Chunk->getBytes(), 1, Chunk->getSize()); // Write non-zeros to avoid Windows optimising the write away
          const UInt64 Pos = m_File->getPosition();
          m_File->_reopen(fFileAccess_ReadWrite, 0 /* options */);
          m_File->seek(Pos);
          m_File->write(Chunk);
        }
        
        QVERIFY(m_File->getSize() >= m_NewSize);
      }
      catch (cRecoverable & Exception)
      {
        m_HasThrownAnException = true;
        m_Exception            = Exception;
        m_Exception.setMsg(String(L"In thread \"") + getName() + L"\": " + m_Exception.getMsg());
      }

      try
      {
        m_File->_reopen(m_OrigAccessFlags, m_OrigOptionFlags);
      }
      catch (cRecoverable & Exception)
      {
        m_HasThrownAnException = true;
        m_Exception            = Exception;
        m_Exception.setMsg(String(L"In thread \"") + getName() + L"\": " + m_Exception.getMsg());
      }
    }
    bool              m_HasThrownAnException;
    cRecoverable      m_Exception;
    const cFile::Ptr  m_File;
    const UInt64      m_NewSize;
    const UInt64      m_ValidDataLength;
    const iFile::iExtendCallback::Ptr m_OptionalCallback;
    int               m_OrigAccessFlags;
    int               m_OrigOptionFlags;
};

void cFile::extendFile(const UInt64 NewLength, const UInt64 ValidDataLength, iFile::iExtendCallback::Ptr OptionalCallback) throw(cFileError, cAborted, cNoRoom)
{
  QVERIFY(OptionalCallback.isValid());
  QVERIFY(NewLength >= getSize());
  QVERIFY(ValidDataLength <= NewLength);

  cExtendFileThread::Ptr Thread = new cExtendFileThread(this, NewLength, ValidDataLength, OptionalCallback);
  (void)iThreadManager::singleton().startThread(Thread);
  Thread->stopLoop(); // Waits until thread finished
  Thread->rethrowUnhandledException();
  if (OptionalCallback.isValid()) OptionalCallback->onExtendProgress(100 /* 100% */); // Can throw cAborted
}
