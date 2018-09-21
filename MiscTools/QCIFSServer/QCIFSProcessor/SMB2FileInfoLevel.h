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

//SMB2FileInfoLevel.h

// MS-FSCC v20080207

#pragma once

#include "writeInfoTemplates.h"

#pragma pack(push)
#pragma pack(1)

//FileInformation Levels
static const UCHAR kFileAccessInformation = 8;
static const UCHAR kFileAlignmentInformation = 17;
static const UCHAR kFileAllInformation = 18;
static const UCHAR kFileAllocationInformation = 19;
static const UCHAR kFileAlternateNameInformation = 21;
static const UCHAR kFileAttributeTagInformation = 35;
static const UCHAR kFileBasicInformation = 4;
static const UCHAR kFileBothDirectoryInformation = 3; 
static const UCHAR kFileCompletionInformation = 30;
static const UCHAR kFileCompressionInformation = 28;
static const UCHAR kFileDirectoryInformation = 1;
static const UCHAR kFileDispositionInformation = 13;
static const UCHAR kFileEaInformation = 7;
static const UCHAR kFileEndOfFileInformation = 20;
static const UCHAR kFileFullDirectoryInformation = 2;
static const UCHAR kFileFullEaInformation = 15;
static const UCHAR kFileHardLinkInformation = 46;
static const UCHAR kFileIdBothDirectoryInformation = 37;
static const UCHAR kFileIdFullDirectoryInformation = 38;
static const UCHAR kFileInternalInformation = 6;
static const UCHAR kFileLinkInformation = 11;
static const UCHAR kFileMailslotQueryInformation = 26;
static const UCHAR kFileMailslotSetInformation = 27;
static const UCHAR kFileModeInformation = 16;
static const UCHAR kFileMoveClusterInformation = 31; 
static const UCHAR kFileNameInformation = 9;
static const UCHAR kFileNamesInformation = 12;
static const UCHAR kFileNetworkOpenInformation = 34; 
static const UCHAR kFileObjectIdInformation = 29;
static const UCHAR kFilePipeInformation = 23;
static const UCHAR kFilePipeLocalInformation = 24;
static const UCHAR kFilePipeRemoteInformation = 25; 
static const UCHAR kFilePositionInformation = 14; 
static const UCHAR kFileQuotaInformation = 32;
static const UCHAR kFileRenameInformation = 10;
static const UCHAR kFileReparsePointInformation = 33; 
static const UCHAR kFileShortNameInformation = 40; 
static const UCHAR kFileStandardInformation = 5;
static const UCHAR kFileStreamInformation = 22;
static const UCHAR kFileTrackingInformation = 36;
static const UCHAR kFileValidDataLengthInformation = 39;

struct FileAccessInformation
{
	ULONG AccessFlags;
};

#define FILE_BYTE_ALIGNMENT 0x00000000 //If this value is specified, there are no alignment requirements for the device. 
#define FILE_WORD_ALIGNMENT 0x00000001 //If this value is specified, data MUST be aligned on a 2-byte boundary. 
#define FILE_LONG_ALIGNMENT 0x00000003 //If this value is specified, data MUST be aligned on a 4-byte boundary. 
#define FILE_QUAD_ALIGNMENT 0x00000007 //If this value is specified, data MUST be aligned on an 8-byte boundary. 
#define FILE_OCTA_ALIGNMENT 0x0000000f //If this value is specified, data MUST be aligned on a 16-byte boundary. 
#define FILE_32_BYTE_ALIGNMENT 0x0000001f //If this value is specified, data MUST be aligned on a 32-byte boundary. 
#define FILE_64_BYTE_ALIGNMENT 0x0000003f //If this value is specified, data MUST be aligned on a 64-byte boundary. 
#define FILE_128_BYTE_ALIGNMENT 0x0000007f //If this value is specified, data MUST be aligned on a 128-byte boundary. 
#define FILE_256_BYTE_ALIGNMENT 0x000000ff //If this value is specified, data MUST be aligned on a 256-byte boundary. 
#define FILE_512_BYTE_ALIGNMENT 0x000001ff //If this value is specified, data MUST be aligned on a 512-byte boundary.

struct FileAlignmentInformation
{
	ULONG Requirement;
};

struct FileAlternateNameInformation
{
	ULONG Length;
};

struct FileAttributeTagInformation
{
	ULONG Attributes;
	ULONG ReparseTag;
};

struct FileBasicInformation
{
	ULONGLONG CreationTime;
	ULONGLONG LastAccessTime;
	ULONGLONG LastWriteTime;
	ULONGLONG ChangeTime;
	ULONG FileAttributes;
	ULONG Reserved;
};

struct FileBothDirectoryInformation
{
	ULONG NextEntryOffset;
	ULONG FileIndex;
	ULONGLONG CreationTime;
	ULONGLONG LastAccessTime;
	ULONGLONG LastWriteTime;
	ULONGLONG ChangeTime;
	ULONGLONG EndOfFile;
	ULONGLONG AllocationSize;
	ULONG FileAttributes;
	ULONG FileNameLength;
	ULONG EaSize;
	UCHAR ShortNameLength;
	UCHAR Reserved;
	UCHAR ShortName[24];
};

struct FileCompressionInformation
{
	ULONGLONG CompressedFileSize;
	USHORT CompressionFormat;
	UCHAR CompressionUnitShift;
	UCHAR ChunkShift;
	UCHAR ClusterShift;
	UCHAR Reserved[3];
};

struct FileDirectoryInformation
{
	ULONG NextEntryOffset;
	ULONG FileIndex;
	ULONGLONG CreationTime;
	ULONGLONG LastAccessTime;
	ULONGLONG LastWriteTime;
	ULONGLONG ChangeTime;
	ULONGLONG EndOfFile;
	ULONGLONG AllocationSize;
	ULONG FileAttributes;
	ULONG FileNameLength;
};

struct FileDispositionInformation
{
	UCHAR DeletePending;
};

struct FileEaInformation
{
	ULONG Size;
};

struct FileEndOfFileInformation
{
	ULONGLONG EndOfFile;
};

struct FileFullDirectoryInformation
{
	ULONG NextEntryOffset;
	ULONG FileIndex;
	ULONGLONG CreationTime;
	ULONGLONG LastAccessTime;
	ULONGLONG LastWriteTime;
	ULONGLONG ChangeTime;
	ULONGLONG EndOfFile;
	ULONGLONG AllocationSize;
	ULONG FileAttributes;
	ULONG FileNameLength;
	ULONG EaSize;
};

struct FileFullEaInformation
{
	ULONG NextEntryOffset;
	UCHAR Flags;
	UCHAR EaNameLength;
	USHORT EaValueLength;
	ULONGLONG EaName;
};

struct FileHardLinkInformation
{
	ULONG BytesNeeded;
	ULONG EntriesReturned;
};

struct FILE_LINK_ENTRY_INFORMATION
{
  ULONG NextEntryOffset;
	ULONGLONG ParentFileId;
	ULONG FileNameLength;
};

struct FileIdBothDirectoryInformation
{
	ULONG NextEntryOffset;
	ULONG FileIndex;
	ULONGLONG CreationTime;
	ULONGLONG LastAccessTime;
	ULONGLONG LastWriteTime;
	ULONGLONG ChangeTime;
	ULONGLONG EndOfFile;
	ULONGLONG AllocationSize;
	ULONG FileAttributes;
	ULONG FileNameLength;
	ULONG EaSize;
	UCHAR ShortNameLength;
	UCHAR Reserved1;
	UCHAR ShortName[24];
	USHORT Reserved2;
	ULONGLONG FileId;
};

struct FileIdFullDirectoryInformation
{
	ULONG NextEntryOffset;
	ULONG FileIndex;
	ULONGLONG CreationTime;
	ULONGLONG LastAccessTime;
	ULONGLONG LastWriteTime;
	ULONGLONG ChangeTime;
	ULONGLONG EndOfFile;
	ULONGLONG AllocationSize;
	ULONG FileAttributes;
	ULONG FileNameLength;
	ULONG EaSize;
	ULONG Reserved;
	ULONGLONG FileId;
};

struct FileInternalInformation
{
	ULONGLONG IndexNumber;
};

struct FileLinkInformation
{
	UCHAR ReplaceIfExists;
	UCHAR Reserved[3];
	ULONGLONG RootDirectory;
	ULONG FileNameLength;
};

struct FileMailslotQueryInformation
{
	ULONG MaximumMessageSize;
	ULONG MailslotQuota;
	ULONG NextMessageSize;
	ULONG MessagesAvailable;
	ULONGLONG ReadTimeout;
};

struct FileMailslotSetInformation
{
	ULONGLONG ReadTimeout;
};

struct FileModeInformation
{
	ULONG Mode;
};

struct FileNameInformation
{
	ULONG FileNameLength;
};

struct FileNamesInformation
{
	ULONG NextEntryOffset;
	ULONG FileIndex;
	ULONG FileNameLength;
};

struct FileNetworkOpenInformation
{
	ULONGLONG CreationTime;
	ULONGLONG LastAccessTime;
	ULONGLONG LastWriteTime;
	ULONGLONG ChangeTime;
	ULONGLONG AllocationSize;
	ULONGLONG EndOfFile;
	ULONG FileAttributes;
	ULONG Reserved;
};

struct FileObjectIdInformation
{
	ULONGLONG FileReferenceNumber;
	ULONGLONG ObjectId[2];
	ULONGLONG BirthVolumeId[2];
	ULONGLONG BirthObjectId[2];
	ULONGLONG DomainId[2];
};

struct FileObjectIdInformationType2
{
	ULONGLONG FileReferenceNumber;
  ULONGLONG ObjectId[2];
	ULONGLONG ExtendedInfo[2];
};

struct FilePipeInformation
{
	ULONG ReadMode;
	ULONG CompletionMode;
};

struct FilePipeLocalInformation
{
	ULONG NamedPipeType;
	ULONG NamedPipeConfiguration;
	ULONG MaximumInstances;
	ULONG CurrentInstances;
	ULONG InboundQuota;
	ULONG ReadDataAvailable;
	ULONG OutboundQuota;
	ULONG WriteQuotaAvailable;
	ULONG NamedPipeState;
	ULONG NamedPipeEnd;
};

struct FilePipeRemoteInformation
{
	ULONGLONG CollectDataTime;
	ULONG MaximumCollectionCount;
};

struct FilePositionInformation
{
	ULONGLONG CurrentByteOffset;
};

struct FileQuotaInformation
{
	ULONG NextEntryOffset;
	ULONG SidLength;
	ULONGLONG ChangeTime;
	ULONGLONG QuotaUsed;
	ULONGLONG QuotaThreshold;
	ULONGLONG QuotaLimit;
};

struct FileGetQuotaInformation
{
	ULONG NextEntryOffset;
	ULONG SidLength;
};

struct FileRenameInformation
{
	UCHAR ReplaceIfExists;
	UCHAR Reserved[7];
	ULONGLONG RootDirectory;
	ULONG FileNameLength;
};

struct FileReparsePointInformation
{
	ULONGLONG FileReferenceNumber;
	ULONG Tag;
};

struct FileShortNameInformation
{
	ULONG FileNameLength;
};

struct FileStandardInformation
{
	ULONGLONG AllocationSize;
	ULONGLONG EndOfFile;
	ULONG NumberOfLinks;
	UCHAR DeletePending;
	UCHAR Directory;
	USHORT Reserved;
};

struct FileStreamInformation
{
	ULONG NextEntryOffset;
	ULONG StreamNameLength;
	ULONGLONG StreamSize;
	ULONGLONG StreamAllocationSize;
};

struct FileValidDataLengthInformation
{
	ULONGLONG FileNameLength;
};

struct FileAllInformation
{
	FileBasicInformation Basic;
	FileStandardInformation Standard;
	FileInternalInformation Internal;
	FileEaInformation Ea;
	FileAccessInformation Access;
	FilePositionInformation Position;
	FileModeInformation Mode;
	FileAlignmentInformation Alignment;
	FileNameInformation Name;
};

typedef ULONG (*tSMB2FileInfoFuncPtr)(const PUCHAR pRespBuf, const fileInfo& rFileInfo);
static tSMB2FileInfoFuncPtr SMB2FileInfoDispatch[UCHAR_MAX] = {0};

static void initialiseFileInfoHandlers()
{
  if (!SMB2FileInfoDispatch[0])
  {
    for(int i = 0; i < UCHAR_MAX; ++i)
    {
      SMB2FileInfoDispatch[i] = &writeFileUnhandled;
    }

    SMB2FileInfoDispatch[kFileDirectoryInformation] = &writeFileAttributeInformation<FileDirectoryInformation>;
    SMB2FileInfoDispatch[kFileFullDirectoryInformation] = &writeFileEAInformation<FileFullDirectoryInformation>;
    SMB2FileInfoDispatch[kFileIdFullDirectoryInformation] = &writeFileIdDirectoryInformation<FileIdFullDirectoryInformation>;
    SMB2FileInfoDispatch[kFileBothDirectoryInformation] = &writeFileBothInformation<FileBothDirectoryInformation>;
    SMB2FileInfoDispatch[kFileIdBothDirectoryInformation] = &writeFileIdBothDirectoryInformation<FileIdBothDirectoryInformation>;
    SMB2FileInfoDispatch[kFileNamesInformation] = &writeFileInformation<FileNamesInformation>;
  }
}

#pragma pack(pop)
