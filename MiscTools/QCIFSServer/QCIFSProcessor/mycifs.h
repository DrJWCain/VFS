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

#ifndef __MY_CIFS_H__
#define __MY_CIFS_H__

#ifndef TIME
#define TIME LARGE_INTEGER
#endif

#ifndef CLONG
#define CLONG ULONG
#endif

#include <windows.h>

#if CIFS 
#include "cifs.h" //this file is available in thw Windows 10 WDK includes folder.
#endif // CIFS 
#include "smb2.h"
#include "ntlm.h"
#include "gss.h"

#if !CIFS 

#pragma pack push
#pragma pack 1
typedef struct _SMB_HEADER {
  UCHAR Protocol[4];
  UCHAR Command;
  //rest of struct removed - as not used in our SMB2 impl ...
} SMB_HEADER;
typedef SMB_HEADER *PSMB_HEADER;
#pragma pack pop

#define SMB_COM_NEGOTIATE       (UCHAR)0x72
#define SMB_HEADER_PROTOCOL     (ULONG)(0xFF + ('S' << 8) + ('M' << 16) + ('B' << 24))//JCJCJC added cast

#define SMB_ERR_CLASS_DOS       (UCHAR)0x01
#define SMB_ERR_CLASS_SERVER    (UCHAR)0x02
#define SMB_ERR_CLASS_HARDWARE  (UCHAR)0x03

#define SMB_ERR_BAD_FUNCTION    1 
#define SMB_ERR_BAD_TID         5
#define SMB_ERR_BAD_FID         6 
#define SMB_ERR_BAD_NET_NAME    6 
#define SMB_ERR_GENERAL         31  
#define SMB_ERR_BAD_UID         91  

#define STATUS_ENDPOINT_CLOSED              (0xC0000000L | 0x00980000L | 0x01)
#define STATUS_DISCONNECTED                 (0xC0000000L | 0x00980000L | 0x02)
#define STATUS_SERVER_ALREADY_STARTED       (0xC0000000L | 0x00980000L | 0x04)
#define STATUS_SERVER_NOT_STARTED           (0xC0000000L | 0x00980000L | 0x05)
#define STATUS_OPLOCK_BREAK_UNDERWAY        (0xC0000000L | 0x00980000L | 0x06)
#define STATUS_NONEXISTENT_NET_NAME         (0xC0000000L | 0x00980000L | 0x08)


#endif // !CIFS 


// define more error codes here
#define IPC_NOT_SUPPORTED 66

//  UNIX Capabilities
// http://wiki.samba.org/index.php/UNIX_Extensions
#define CAP_UNIX 0x00800000

#define CIFS_UNIX_FCNTL_LOCKS_CAP 	 0x001
#define CIFS_UNIX_POSIX_ACLS_CAP 	0x002
#define CIFS_UNIX_XATTR_CAP 	0x004
#define CIFS_UNIX_EXATTR_CAP 	0x008
#define CIFS_UNIX_POSIX_PATHNAMES_CAP 	0x010 	//All characters except '/' should be supported in pathnames
#define CIFS_UNIX_POSIX_PATH_OPERATIONS_CAP 	0x020
#define CIFS_UNIX_LARGE_READ_CAP 	0x040 	//We can cope with 24 bit reads in readX.
#define CIFS_UNIX_LARGE_WRITE_CAP 	0x080 	//We can cope with 24 bit writes in writeX.
#define CIFS_UNIX_TRANSPORT_ENCRYPTION_CAP 	0x100 	//We can do SPNEGO negotiations for encryption.
#define CIFS_UNIX_TRANSPORT_ENCRYPTION_MANDATORY_CAP 	0x200 	//We *must* SPNEGO negotiations for encryption.
#define CIFS_UNIX_PROXY_CAP 	0x400 	//Proxy capability, supports 0xACE ntioctl and QFS PROXY call

//info levels
#define SMB_QUERY_CIFS_UNIX_INFO 	 0x200
#define SMB_QUERY_POSIX_FS_INFO 	0x201 	
#define SMB_QUERY_POSIX_WHO_AM_I 	0x202
#define SMB_REQUEST_TRANSPORT_ENCRYPTION 	0x203 	//Call to set up an encryption context.
#define SMB_QUERY_FS_PROXY 	0x204 	//New NTIOCTL available (0xACE) for WAN friendly SMB (see below)

#define QUERY_FILE_UNIX_BASIC 	 0x200 	 //Part of the initial Unix Extensions
#define QUERY_FILE_UNIX_LINK 	0x201 	//Part of the initial Unix Extensions
#define QUERY_POSIX_ACL 	0x204 	//Requires CIFS_UNIX_POSIX_ACL_CAP, MUST be supported if set
#define QUERY_XATTR 	0x205 	//Requires CIFS_UNIX_XATTR_CAP, MUST be supported if set
#define QUERY_ATTR_FLAGS 	0x206 	//Requires CIFS_UNIX_EXTATTR_CAP, MUST be supported if set
#define QUERY_POSIX_PERMISSION 	0x207
#define QUERY_POSIX_LOCK 	0x208 	//Requires CIFS_UNIX_FCNTL_CAP, MUST be supported if set
#define SMB_POSIX_PATH_OPEN 	0x209 	//Requires CIFS_UNIX_POSIX_PATH_OPERATIONS_CAP, MUST be supported if set
#define SMB_POSIX_PATH_UNLINK 	0x20a 	//Requires CIFS_UNIX_POSIX_PATH_OPERATIONS_CAP, SHOULD be supported if set
#define SMB_QUERY_FILE_UNIX_INFO2 	0x20b 	//Requires CIFS_UNIX_EXTATTR_CAP, SHOULD be supported if set

enum eUnixFileType {eFile
  , eDirectory
  , eSymbolic
  , eCharacter
  , eBlock
  , eFIFO};

#pragma pack(push)
#pragma pack(1)

typedef struct _RESP_SMB_QUERY_CIFS_UNIX_INFO
{
  USHORT MajorVersion;
  USHORT MinorVersion;
  ULONGLONG Flags;
} RESP_SMB_QUERY_CIFS_UNIX_INFO, *PRESP_SMB_QUERY_CIFS_UNIX_INFO;

typedef struct _SMB_QUERY_FILE_UNIX_BASIC
{
  ULONGLONG ENDOFFILE;//FILE SIZE
  ULONGLONG NumOfBlocks;//Number of file system block used to store file
  ULONGLONG LastStatusChange;//Last time the status of the file was changed. This is in DCE time
  ULONGLONG LastAccessTime;//Last access time. This is in DCE time.
  ULONGLONG LastModificationTime;// Last modification time. This is in DCE time.
  ULONGLONG Uid;//Numeric user id for the owner
  ULONGLONG Gid;//Numeric group id of owner
  ULONG Type;
  /*Enumeration specifying the pathname type:
  0 -- File
  1 -- Directory
  2 -- Symbolic link
  3 -- Character device
  4 -- Block device
  5 -- FIFO (named pipe)*/
  ULONGLONG devmajor;//Major device number if type is device
  ULONGLONG devminor;//Minor device number if type is device
  ULONGLONG uniqueid;//This is a server-assigned unique id for the file.
  //  The client will typically map this onto an inode
  //  number. The scope of uniqueness is the share.
  ULONGLONG permissions;//Standard UNIX file permissions
  ULONGLONG nlinks;//The number of directory entries that map to this entry (number of hard links)
} SMB_QUERY_FILE_UNIX_BASIC, *PSMB_QUERY_FILE_UNIX_BASIC;

typedef struct _SMB_QUERY_FILE_UNIX_BASIC2
{
  SMB_QUERY_FILE_UNIX_BASIC Basic;
  ULONGLONG CreationTime;
  ULONG FileFlags;
  ULONG FileFlagsMask;
} SMB_QUERY_FILE_UNIX_BASIC2, *PSMB_QUERY_FILE_UNIX_BASIC2;

#define SMB_FIND_FILE_UNIX 	 0x202 	 //Part of the initial Unix Extensions
#define SMB_FIND_FILE_UNIX2 	 0x20B 	 //Part of the initial Unix Extensions

typedef struct _SMB_FIND_FILE_UNIX_INFO
{
  ULONG 	NextEntryOffset;
  ULONG 	FileIndex;
  SMB_QUERY_FILE_UNIX_BASIC UnixInfo;
  USHORT FileName[1];
} SMB_FIND_FILE_UNIX_INFO, *PSMB_FIND_FILE_UNIX_INFO;

typedef struct _SMB_FIND_FILE_UNIX_INFO2
{
  ULONG 	NextEntryOffset;
  ULONG 	FileIndex;
  SMB_QUERY_FILE_UNIX_BASIC2 UnixInfo;
  ULONG 	FileNameLength;
  USHORT FileName[1];
} SMB_FIND_FILE_UNIX_INFO2, *PSMB_FIND_FILE_UNIX_INFO2;

#pragma pack(pop)

///////////////////////////////////////////////////////////////////////////////////////
// SMB2 support ///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//

typedef SMB2_SYNC_Header SMB2_HEADER;
typedef SMB2_SYNC_Header *PSMB2_HEADER;

//new info levels for trans 2 - [MS-SMB] v20080207 p.131
#define SMB_FIND_FILE_ID_FULL_DIRECTORY_INFO  0x105
#define SMB_FIND_FILE_ID_BOTH_DIRECTORY_INFO  0x106

#endif
