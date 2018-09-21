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

//FsinfoLevel.h
// MS-FSCC v20080207

#pragma once

#pragma pack(push)
#pragma pack(1)

//FSInformation levels
static const UCHAR kFileFsVolumeInformation = 1;
static const UCHAR kFileFsLabelInformation = 2;
static const UCHAR kFileFsSizeInformation = 3;
static const UCHAR kFileFsDeviceInformation = 4; 
static const UCHAR kFileFsAttributeInformation = 5; 
static const UCHAR kFileFsControlInformation = 6;
static const UCHAR kFileFsFullSizeInformation = 7;
static const UCHAR kFileFsObjectIdInformation = 8;
static const UCHAR kFileFsDriverPathInformation = 9;

#define FILE_CASE_PRESERVED_NAMES 0x00000002 //The file system preserves the case of file names when it places a name on disk. 
#define FILE_CASE_SENSITIVE_SEARCH 0x00000001 //The file system supports case-sensitive file names when looking up (searching for) file names in a directory. 
#define FILE_FILE_COMPRESSION 0x00000010 //The file volume supports file-based compression. This flag is incompatible with the FILE_VOLUME_IS_COMPRESSED flag. 
#define FILE_NAMED_STREAMS 0x00040000 //The file system supports named streams. 
#define FILE_PERSISTENT_ACLS 0x00000008 //The file system preserves and enforces access control lists (ACLs). 
#define FILE_READ_ONLY_VOLUME 0x00080000 //The specified volume is read-only.<48> 
#define FILE_SUPPORTS_ENCRYPTION 0x00020000 //The file system supports the Encrypted File System (EFS). NTFS version 2 supports EFS. To use EFS, the operating system must support NTFS version 2, and the file system on disk must be formatted using NTFS version 2.<49> 
#define FILE_SUPPORTS_OBJECT_IDS 0x00010000 //The file system supports object identifiers. 
#define FILE_SUPPORTS_REMOTE_STORAGE 0x00000100 //The file system supports remote storage. 
#define FILE_SUPPORTS_REPARSE_POINTS 0x00000080 //The file system supports reparse points. 
#define FILE_SUPPORTS_SPARSE_FILES 0x00000040 //The file system supports sparse files. 
#define FILE_UNICODE_ON_DISK 0x00000004 //The file system supports Unicode in file and directory names. This flag applies only to file and directory names; the file system neither restricts nor interprets the bytes of data within a file. 
#define FILE_VOLUME_IS_COMPRESSED 0x00008000 //The specified volume is a compressed volume. This flag is incompatible with the FILE_FILE_COMPRESSION flag. 
#define FILE_VOLUME_QUOTAS 0x00000020 //The file system supports per-user quotas.

struct FileFsAttributeInformation
{
	ULONG FileSystemAttributes;
	ULONG MaximumComponentNameLength;
	ULONG FileSystemNameLength;
};

struct FileFsControlInformation
{
	ULONGLONG FreeSpaceStartFiltering;
	ULONGLONG FreeSpaceThreshold;
	ULONGLONG FreeSpaceStopFiltering;
	ULONGLONG DefaultQuotaThreshold;
	ULONGLONG DefaultQuotaLimit;
	ULONG FileSystemControlFlags;
};

struct FileFsDriverPathInformation
{
	UCHAR DriverInPath;
	UCHAR Reserved[3];
	ULONG DriverNameLength;
};

struct FileFsFullSizeInformation
{
	ULONGLONG TotalAllocationUnits;
	ULONGLONG CallerAvailableAllocationUnits;
	ULONGLONG ActualAvailableAllocationUnits;
	ULONG SectorsPerAllocationUnit;
	ULONG BytesPerSector;
};

struct FileFsLabelInformation
{
	ULONG VolumeLabelLength;
};

struct FileFsObjectIdInformation
{
	ULONGLONG ObjectId[2];
	ULONGLONG ExtendedInfo[6];
};

struct FileFsSizeInformation
{
	ULONGLONG TotalAllocationUnits;
	ULONGLONG ActualAvailableAllocationUnits;
	ULONG SectorsPerAllocationUnit;
	ULONG BytesPerSector;
};

struct FileFsVolumeInformation
{
	ULONGLONG VolumeCreationTime;
	ULONG VolumeSerialNumber;
	ULONG VolumeLabelLength;
	UCHAR SupportsObjects;
	UCHAR Reserved;
};

struct FileFsDeviceInformation
{
	ULONG DeviceType;
	ULONG Characteristics;
};

#pragma pack(pop)
