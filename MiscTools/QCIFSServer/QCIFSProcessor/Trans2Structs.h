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

//Trans2Structs.h
// define structures from [#1] SNIA CIFS Technical Reference Revision 1.0
// that don't come in cifs.h

#pragma once

#include "mycifs.h"

__int64 makeLg(int hiPart, int loPart);
__int64 makeLg(FILETIME ft);

typedef struct _smbQueryFileBasicInfo
{
  TIME CreationTime;    //Time when file was created
  TIME LastAccessTime;  //Time of last file access
  TIME LastWriteTime;   //Time of last write to the file
  TIME ChangeTime;      //Time when file was last changed
  ULONG Attributes;     //File Attributes
} smbQueryFileBasicInfo, *PsmbQueryFileBasicInfo;

typedef struct _smbQueryFileStandardInfo
{
  LARGE_INTEGER AllocationSize; // Allocated size of the file in number of bytes
  LARGE_INTEGER EndOfFile;      // Offset to the first free byte in the file
  ULONG NumberOfLinks;          // Number of hard links to the file
  BOOLEAN DeletePending;        // Indicates whether the file is marked for deletion
  BOOLEAN Directory;            // Indicates whether the file is a Directory
} smbQueryFileStandardInfo, *PsmbQueryFileStandardInfo;

typedef struct _smbQueryFileNameInfo  // NOTE: Do not include the path to the file [#1] p.85
{
  ULONG FileNameLength; //Length of the file name in number of bytes
  WCHAR FileName[1];    // Name of the file
} smbQueryFileNameInfo, *PsmbQueryFileNameInfo;

typedef struct _smbQueryFileStreamInfo
{
  ULONG NextEntryOffset;              // Offset to the next entry (in bytes)
  ULONG StreamNameLength;             // Length of the stream name in number of bytes
  LARGE_INTEGER StreamSize;           // Size of the stream in number of bytes
  LARGE_INTEGER StreamAllocationSize; // Allocated size of the stream in number of bytes
  WCHAR FileName[1];                  // Name of the stream
} smbQueryFileStreamInfo, *PsmbQueryFileStreamInfo;

typedef struct _smbFindFileDirectoryInfo  //[#1] p.101
{ 
  ULONG NextEntryOffset;
  ULONG FileIndex;
  TIME CreationTime;
  TIME LastAccessTime;
  TIME LastWriteTime;
  TIME ChangeTime;
  LARGE_INTEGER EndOfFile;
  LARGE_INTEGER AllocationSize;
  ULONG ExtFileAttributes;
  ULONG FileNameLength;
  WCHAR FileName[1];
} smbFindFileDirectoryInfo, *PsmbFindFileDirectoryInfo;

typedef struct _smbFindFileFullDirectoryInfo  //[#1] p.102
{ 
  ULONG NextEntryOffset;
  ULONG FileIndex;
  TIME CreationTime;
  TIME LastAccessTime;
  TIME LastWriteTime;
  TIME ChangeTime;
  LARGE_INTEGER EndOfFile;
  LARGE_INTEGER AllocationSize;
  ULONG ExtFileAttributes;
  ULONG FileNameLength;
  ULONG EaSize;
  WCHAR FileName[1];
} smbFindFileFullDirectoryInfo, *PsmbFindFileFullDirectoryInfo;

typedef struct _smbFindFileBothDirectoryInfo  //[#1] p.102
{ 
  ULONG NextEntryOffset;
  ULONG FileIndex;
  TIME CreationTime;
  TIME LastAccessTime;
  TIME LastWriteTime;
  TIME ChangeTime;
  LARGE_INTEGER EndOfFile;
  LARGE_INTEGER AllocationSize;
  ULONG ExtFileAttributes;
  ULONG FileNameLength;
  ULONG EaSize;
  UCHAR ShortNameLength;
  UCHAR Reserved;
  WCHAR ShortName[12];
  WCHAR FileName[1];
} smbFindFileBothDirectoryInfo, *PsmbFindFileBothDirectoryInfo;

//new info levels for trans 2 - [MS-SMB] v20080207 p.133
typedef struct _smbFindFileIdFullDirectoryInfo
{
  ULONG NextEntryOffset;
  ULONG FileIndex;
  TIME CreationTime;              //Time when file was created
  TIME LastAccessTime;            //Time of last file access
  TIME LastWriteTime;             //Time of last write to the file
  TIME ChangeTime;                //Time when file was last changed
  LARGE_INTEGER EndOfFile;        //Offset to the first free byte in the  file
  LARGE_INTEGER AllocationSize;   //Allocated size of the file in number of bytes
  ULONG ExtFileAttributes;            //Extended attributes for this file that MUST be marked as a DIRECTORY
  ULONG FileNameLength;           //Length of the file name in number of bytes
  ULONG EASize;                   //Size of the file's extended attributes in number of bytes
  LARGE_INTEGER FileID;           //A LARGE_INTEGER that serves as an internal file system identifier. This number MUST be unique for each file on a given volume. If a remote file system does not support unique FileId values, the FileId field MUST be set to 0
  WCHAR FileName[1];              //Name of the file
} smbFindFileIdFullDirectoryInfo, *PsmbFindFileIdFullDirectoryInfo;

//new info levels for trans 2 - [MS-SMB] v20080207 p.135
typedef struct _smbFindFileIdBothDirectoryInfo
{
  ULONG NextEntryOffset;
  ULONG FileIndex;
  TIME CreationTime;              //Time when file was created
  TIME LastAccessTime;            //Time of last file access
  TIME LastWriteTime;             //Time of last write to the file
  TIME ChangeTime;                //Time when file was last changed
  LARGE_INTEGER EndOfFile;        //Offset to the first free byte in the  file
  LARGE_INTEGER AllocationSize;   //Allocated size of the file in number of bytes
  ULONG ExtFileAttributes;            //Extended attributes for this file that MUST be marked as a DIRECTORY
  ULONG FileNameLength;           //Length of the file name in number of bytes
  ULONG EASize;                   //Size of the file's extended attributes in number of bytes
  UCHAR ShortNameLength;
  WCHAR ShortName[16];
  LARGE_INTEGER FileID;           //A LARGE_INTEGER that serves as an internal file system identifier. This number MUST be unique for each file on a given volume. If a remote file system does not support unique FileId values, the FileId field MUST be set to 0
  WCHAR FileName[1];              //Name of the file
} smbFindFileIdBothDirectoryInfo, *PsmbFindFileIdBothDirectoryInfo;

// adjusting info All with respect to observed responses under ethereal/wireshark
// and in line with post from George Colley on cifs-protocol mailing list 22/06/07
// also ref. James Peach on cifs-protocol  mailing list June 2008

typedef struct _smbFindFileAllInfo
{
  TIME CreationTime;              //Time when file was created
  TIME LastAccessTime;            //Time of last file access
  TIME LastWriteTime;             //Time of last write to the file
  TIME ChangeTime;                //Time when file was last changed
  ULONG Attributes;              //File Attributes  - patched following discussion with George Colley himself at CIFS plugfest 2007
  LARGE_INTEGER AllocationSize;   //Allocated size of the file in number of bytes
  LARGE_INTEGER EndOfFile;        //Offset to the first free byte in the  file
  ULONG NumberOfLinks;            //Number of hard links to the file
  BOOLEAN DeletePending;          //Indicates whether the file is marked  for deletion
  BOOLEAN Directory;              //Indicates whether the file is a directory
  USHORT Unknown;
  //LARGE_INTEGER IndexNumber;      //A file system unique identifier
  ULONG EASize;                   //Size of the file's extended attributes in number of bytes
  //ULONG AccessFlags;              //Access that a caller has to the file; Possible values and meanings are specified below
  //LARGE_INTEGER IndexNumber1;     //A file system unique identifier
  //LARGE_INTEGER CurrentByteOffset;//Current byte offset within the file
  //ULONG Mode;                     //Current Open mode of the file handle to the file; possible values and meanings are detailed below
  //ULONG AlignmentRequirement;     //Buffer Alignment required by device; possible values detailed below
  ULONG FileNameLength;           //Length of the file name in number of bytes
  WCHAR FileName[1];              //Name of the file
} smbFindFileAllInfo, *PsmbFindFileAllInfo;

typedef struct _smbFindFileCompressionInfo // [#1] p.86
{
  LARGE_INTEGER  CompressedFileSize;  // Size of the compressed file in number of bytes
  USHORT CompressionFormat;           // A constant signifying the compression algorithm used. Possible  values are:
                                      //  0 - There is no compression
                                      //  2- Compression Format is LZNT
  UCHAR CompressionUnitShift;
  UCHAR ChunkShift;                   // Stored in log2 format (1 << ChunkShift = ChunkSizeInBytes)
  UCHAR ClusterShift;                 // Indicates how much space must be saved to successfully compress a compression unit
  UCHAR Reserved[3];
} smbFindFileCompressionInfo, *PsmbFindFileCompressionInfo;

#define FILE_WRITE_THROUGH 0x00000002
#define FILE_SEQUENTIAL_ONLY 0x00000004
#define FILE_SYNCHRONOUS_IO_ALERT 0x00000010
#define FILE_SYNCHRONOUS_IO_NONALERT 0x00000020

typedef struct _smbQueryFsVolumeInfo  // [#1] p.59
{
  USHORT TwoSeconds;
  USHORT Minutes;
  USHORT Hours;
  ULONG SerialNumber;
  ULONG LabelLength;
  BYTE Reserved1;
  BYTE Reserved2;
  WCHAR Label[1];       // The volume label
} smbQueryFsVolumeInfo, *PsmbQueryFsVolumeInfo;

typedef struct _smbInfoAllocation  // [#1] p.58
{
  ULONG idFileSystem;   //  File system identifier (NT server always returns 0)
  ULONG cSectorUnit;    //  Number of sectors per allocation unit
  ULONG cUnit;          //  Total number of allocation units
  ULONG cUnitAvail;     //  Total number of available allocation units
  USHORT cbSector;      //  Number of bytes per sector
} smbInfoAllocation, *PsmbInfoAllocation;

typedef struct _smbQueryFsSizeInfo  // [#1] p.59
{
  LARGE_INTEGER TotalAllocationUnits; // on the Volume
  LARGE_INTEGER FreeAllocationUnits;  // on the Volume
  ULONG sectorsPerAllocationUnit;
  ULONG bytesPerSector;
} smbQueryFsSizeInfo, *PsmbQueryFsSizeInfo;

typedef struct _smbQueryFsAttributeInfo // [#1] p.61
{
  ULONG FileSystemAttributes;
  LONG MaxFileLength;
  ULONG nameLength;
  WCHAR sName[1];
} smbQueryFsAttributeInfo, *PsmbQueryFsAttributeInfo;

typedef struct _smbQueryFsDeviceInfo // [#1] p.59
{
  ULONG DeviceType;
  ULONG Characteristics;
} smbQueryFsDeviceInfo, *PsmbQueryFsDeviceInfo;

#define FILE_REMOVABLE_MEDIA 0x00000001
#define FILE_READ_ONLY_DEVICE 0x00000002
#define FILE_FLOPPY_DISKETTE 0x00000004
#define FILE_WRITE_ONE_MEDIA 0x00000008
#define FILE_REMOTE_DEVICE 0x00000010

