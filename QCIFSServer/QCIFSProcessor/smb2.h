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

//[#2] [MS-SMB2] - v20080207


#define SMB2_HEADER_PROTOCOL   (ULONG)(0xFE + ('S' << 8) + ('M' << 16) + ('B' << 24))

#define SMB2_PROTOCOL_02_02 "SMB 2.002"
#define SMB2_PROTOCOL_02_FF "SMB 2.???"

const UCHAR SMB2_NegotiateProtocol = 0x00;
const UCHAR SMB2_SessionSetup = 0x01;
const UCHAR SMB2_SessionLogoff = 0x02;
const UCHAR SMB2_TreeConnect = 0x03;
const UCHAR SMB2_TreeDisconnect = 0x04;
const UCHAR SMB2_Create = 0x05;
const UCHAR SMB2_Close = 0x06;
const UCHAR SMB2_Flush = 0x07;
const UCHAR SMB2_Read = 0x08;
const UCHAR SMB2_Write = 0x09;
const UCHAR SMB2_Lock = 0x0a;
const UCHAR SMB2_Ioctl = 0x0b;
const UCHAR SMB2_Cancel = 0x0c;
const UCHAR SMB2_KeepAlive = 0x0d;
const UCHAR SMB2_Find = 0x0e;
const UCHAR SMB2_Notify = 0x0f;
const UCHAR SMB2_GetInfo = 0x10;
const UCHAR SMB2_SetInfo = 0x11;
const UCHAR SMB2_Break = 0x12;

#pragma pack(push)
#pragma pack(1)

//structs pseudo-auto-generated from [#2]

// Header flags [#2]p.22
#define SMB2_FLAGS_SERVER_TO_REDIR 0x00000001 //When set, indicates the message is a response, rather than a request. This MUST be set on responses sent from the server to the client and MUST NOT be set on requests sent from the client to the server.
#define SMB2_FLAGS_ASYNC_COMMAND 0x00000002 //When set, indicates that this is an asynchronously processed command.
#define SMB2_FLAGS_RELATED_OPERATIONS 0x00000004 //When set, indicates that this command is a related operation in a compounded request chain. The use of this flag is as specified in 3.2.4.1.4.
#define SMB2_FLAGS_SIGNED 0x00000008 //When set, indicates that this packet has been signed. The use of this flag is as specified in 3.1.5.1.
#define SMB2_FLAGS_DFS_OPERATIONS 0x10000000 //When set, indicates that this command is a Distributed File System (DFS) operation. The use of this flag is as specified in 3.3.5.9.
#define SMB2_FLAGS_REPLAY_OPERATION 0x20000000 //This flag is only valid for the SMB 3.0 dialect. When set, it indicates that this command is a replay operation.

typedef struct _SMB2_ASYNC_Header
{
  ULONG      ProtocolId;
  USHORT     StructureSize;
  USHORT     CreditCharge;
  ULONG      Status;
  USHORT     Command;
  USHORT     CreditRequest;
  ULONG      Flags;
  ULONG      NextCommand;
  ULONGLONG  MessageId;
  ULONGLONG  AsyncId;
  ULONGLONG  SessionId;
  ULONGLONG  Signature[2];
} SMB2_ASYNC_Header, *PSMB2_ASYNC_Header;

typedef struct _SMB2_SYNC_Header
{
  ULONG      ProtocolId;
  USHORT     StructureSize;
  USHORT     CreditCharge;
  ULONG      Status;
  USHORT     Command;
  USHORT     CreditRequest;
  ULONG      Flags;
  ULONG      NextCommand;
  ULONGLONG  MessageId;
  ULONG      ProcessId;
  ULONG      TreeId;
  ULONGLONG  SessionId;
  ULONGLONG  Signature[2];
} SMB2_SYNC_Header, *PSMB2_SYNC_Header;

typedef struct _RESP_SMB2_ERROR
{
  USHORT  StructureSize;
  USHORT  Reserved;
  ULONG  ByteCount;
  UCHAR ErrorData;
} RESP_SMB2_ERROR;
typedef RESP_SMB2_ERROR *PRESP_SMB2_ERROR;

typedef struct _RESP_SMB2_ERROR_SYM_LINK
{
  ULONG  SymLinkLength;
  ULONG  SymLinkErrorTag;
  ULONG  ReparseTag;
  USHORT  ReparseDataLength;
  USHORT  Reserved2;
  USHORT  SubstituteNameOffset;
  USHORT  SubstituteNameLength;
  USHORT  PrintNameOffset;
  USHORT  PrintNameLength;
  ULONG  Flags;
} RESP_SMB2_ERROR_SYM_LINK;
typedef RESP_SMB2_ERROR_SYM_LINK *PRESP_SMB2_ERROR_SYM_LINK;

typedef struct _REQ_SMB2_NEGOTIATE
{
  USHORT  StructureSize;
  USHORT  DialectCount;
  USHORT  SecurityMode;
  USHORT  Reserved;
  ULONG  Capabilities;
  ULONGLONG  ClientGuid[2];
  ULONGLONG  ClientStartTime;
} REQ_SMB2_NEGOTIATE;
typedef REQ_SMB2_NEGOTIATE *PREQ_SMB2_NEGOTIATE;

//[#2] p.29 - SecurityMode
#define SMB2_NEGOTIATE_SIGNING_ENABLED 0x0001 //When set, indicates that on the server.
#define SMB2_NEGOTIATE_SIGNING_REQUIRED 0x0002 //When set, indicates that by the server.

#define SMB2_GLOBAL_CAP_DFS 0x00000001 //When set, indicates that the server supports the Distributed File System (DFS).
#define SMB2_GLOBAL_CAP_LEASING 0x00000002 //When set, indicates that the server supports leasing. This flag is not valid for the SMB 2.002 dialect.
#define SMB2_GLOBAL_CAP_LARGE_MTU 0x00000004 //When set, indicates that the server supports multi-credit operations. This flag is not valid for the SMB 2.002 dialect.
#define SMB2_GLOBAL_CAP_MULTI_CHANNEL 0x00000008 //When set, indicates that the server supports establishing multiple channels for a single session. This flag is only valid for the SMB 3.0 dialect.
#define SMB2_GLOBAL_CAP_PERSISTENT_HANDLES 0x00000010 //When set, indicates that the server supports persistent handles. This flag is only valid for the SMB 3.0 dialect.
#define SMB2_GLOBAL_CAP_DIRECTORY_LEASING 0x00000020 //When set, indicates that the server supports directory leasing. This flag is only valid for the SMB 3.0 dialect.
#define SMB2_GLOBAL_CAP_ENCRYPTION 0x00000040 //When set, indicates that the server supports encryption. This flag is only valid for the SMB 3.0 dialect.


typedef struct _RESP_SMB2_NEGOTIATE
{
  USHORT  StructureSize;
  USHORT  SecurityMode; //SMB2_NEGOTIATE_SIGNING_ENABLED | SMB2_NEGOTIATE_SIGNING_REQUIRED
  USHORT  DialectRevision;
  USHORT  Reserved;
  ULONGLONG  ServerGuid[2];
  ULONG  Capabilities; //SMB2_GLOBAL_CAP_DFS
  ULONG  MaxTransactSize;
  ULONG  MaxReadSize;
  ULONG  MaxWriteSize;
  ULONG  SystemTime[2];
  ULONG ServerStartTime[2];
  USHORT  SecurityBufferOffset;
  USHORT  SecurityBufferLength;
  ULONG  Reserved2;
} RESP_SMB2_NEGOTIATE;
typedef RESP_SMB2_NEGOTIATE *PRESP_SMB2_NEGOTIATE;

typedef struct _REQ_SMB2_SESSION_SETUP
{
  USHORT StructureSize;
  UCHAR Flags;
  UCHAR SecurityMode;
  ULONG Capabilities;
  ULONG Channel;
  USHORT SecurityBufferOffset;
  USHORT SecurityBufferLength;
  ULONGLONG PreviousSessionId;
} REQ_SMB2_SESSION_SETUP;
typedef REQ_SMB2_SESSION_SETUP *PREQ_SMB2_SESSION_SETUP;

#define SMB2_SESSION_FLAG_IS_GUEST 0x0001 //If set, the client has been authenticated as a guest user. 
#define SMB2_SESSION_FLAG_IS_NULL 0x0002 //If set, the client has been authenticated as an anonymous user.
#define SMB2_SESSION_FLAG_BINDING 0x01 // When set, indicates that the request is to bind an existing session to a new connection.

typedef struct _RESP_SMB2_SESSION_SETUP
{
  USHORT  StructureSize;
  USHORT  SessionFlags;
  USHORT  SecurityBufferOffset;
  USHORT  SecurityBufferLength;
//  USHORT  Buffer[1];
} RESP_SMB2_SESSION_SETUP;
typedef RESP_SMB2_SESSION_SETUP *PRESP_SMB2_SESSION_SETUP;

typedef struct _REQ_SMB2_LOGOFF
{
  USHORT  StructureSize;
  USHORT  Reserved;
} REQ_SMB2_LOGOFF;
typedef REQ_SMB2_LOGOFF *PREQ_SMB2_LOGOFF;

typedef struct _RESP_SMB2_LOGOFF
{
  USHORT  StructureSize;
  USHORT  Reserved;
} RESP_SMB2_LOGOFF;
typedef RESP_SMB2_LOGOFF *PRESP_SMB2_LOGOFF;

typedef struct _REQ_SMB2_TREE_CONNECT
{
  USHORT  StructureSize;
  USHORT  Reserved;
  USHORT  PathOffset;
  USHORT  PathLength;
} REQ_SMB2_TREE_CONNECT;
typedef REQ_SMB2_TREE_CONNECT *PREQ_SMB2_TREE_CONNECT;

#define SMB2_SHARE_TYPE_DISK 0x01
#define SMB2_SHARE_TYPE_PIPE 0x02
#define SMB2_SHARE_TYPE_PRINT 0x03

#define SMB2_SHAREFLAG_MANUAL_CACHING 0x00000000 //The client MAY cache files that are explicitly selected by the user for offline use. 
#define SMB2_SHAREFLAG_AUTO_CACHING 0x00000010 //The client MAY automatically cache files that are used by the user for offline access. 
#define SMB2_SHAREFLAG_VDO_CACHING 0x00000020 //The client MAY automatically cache files that are used by the user for offline access, and MAY use those files in an offline mode even if the share is available. 
#define SMB2_SHAREFLAG_NO_CACHING 0x00000030 //Offline caching MUST NOT occur. 

#define SMB2_SHAREFLAG_DFS 0x00000001 //The specified share is present in a Distributed File System (DFS) tree structure. The server SHOULD set the SMB2_SHAREFLAG_DFS bit in the ShareFlags field if the per-share property Share.IsDfs is TRUE.
#define SMB2_SHAREFLAG_DFS_ROOT 0x00000002 //The specified share is present in a DFS tree structure. The server SHOULD set the SMB2_SHAREFLAG_DFS_ROOT bit in the ShareFlags field if the per-share property Share.IsDfs is TRUE.
#define SMB2_SHAREFLAG_RESTRICT_EXCLUSIVE_OPENS 0x00000100 //The specified share disallows exclusive file opens that deny reads to an open file.
#define SMB2_SHAREFLAG_FORCE_SHARED_DELETE 0x00000200 //Shared files in the specified share can be forcibly deleted.
#define SMB2_SHAREFLAG_ALLOW_NAMESPACE_CACHING 0x00000400 //The client MUST ignore this flag.
#define SMB2_SHAREFLAG_ACCESS_BASED_DIRECTORY_ENUM 0x00000800 //The server will filter directory entries based on the access permissions of the client.
#define SMB2_SHAREFLAG_FORCE_LEVELII_OPLOCK 0x00001000 //The server will not issue exclusive caching rights on this share.<24>
#define SMB2_SHAREFLAG_ENABLE_HASH_V1 0x00002000 //The share supports hash generation for branch cache retrieval of data. For more information, see section 2.2.31.2. This value is only supported for the SMB 2.1 and 3.0 dialects.
#define SMB2_SHAREFLAG_ENABLE_HASH_V2 0x00004000 //The share supports v2 hash generation for branch cache retrieval of data. For more information, see section 2.2.31.2. This value is only supported for the SMB 3.0 dialect.
#define SMB2_SHAREFLAG_ENCRYPT_DATA 0x00008000 //The server requires encryption of remote file access messages on this share, per the conditions specified in section 3.3.5.2.11. This flag is only valid for the SMB 3.0 dialect.

#define SMB2_SHARE_CAP_DFS 0x00000008 //The specified share is present in a DFS tree structure. The server MUST set the SMB2_SHARE_CAP_DFS bit in the Capabilities field if the per-share property Share.IsDfs is TRUE.
#define SMB2_SHARE_CAP_CONTINUOUS_AVAILABILITY 0x00000010 //The specified share is continuously available. This flag is only valid for the SMB 3.0 dialect.
#define SMB2_SHARE_CAP_SCALEOUT 0x00000020 //The specified share is present on a server configuration which facilitates faster recovery of durable handles. This flag is only valid for the SMB 3.0 dialect.
#define SMB2_SHARE_CAP_CLUSTER 0x00000040 //The specified share is present on a server configuration which provides monitoring of the availability of share through the Witness service specified in [MS-SWN]. This flag is only valid for the SMB 3.0 dialect.

#define SHI1005_FLAGS_DFS 0x00000001 //The specified share is present in a DFS tree structure. 
#define SHI1005_FLAGS_DFS_ROOT 0x00000002 //The specified share is the root volume in a DFS tree structure. 
#define SHI1005_FLAGS_RESTRICT_EXCLUSIVE_OPENS 0x00000100 //The specified share disallows exclusive file opens that deny reads to an open file. 
#define SHI1005_FLAGS_FORCE_SHARED_DELETE 0x00000200 //Shared files in the specified share can be forcibly deleted. 
#define SHI1005_FLAGS_ALLOW_NAMESPACE_CACHING 0x00000400 //Clients are allowed to cache the namespace of the specified share. 
#define SHI1005_FLAGS_ACCESS_BASED_DIRECTORY_ENUM 0x00000800 //The server will filter directory entries based on the access permissions of the client.

//IOCTL Codes
#define FSCTL_DFS_GET_REFERRALS 0x00060194
#define FSCTL_PIPE_PEEK 0x0011400C
#define FSCTL_PIPE_WAIT 0x00110018
#define FSCTL_PIPE_TRANSCEIVE 0x0011C017
#define FSCTL_SRV_COPYCHUNK 0x001440F2
#define FSCTL_SRV_ENUMERATE_SNAPSHOTS 0x00144064
#define FSCTL_SRV_REQUEST_RESUME_KEY 0x00140078
#define FSCTL_SRV_READ_HASH 0x001441bb
#define FSCTL_SRV_COPYCHUNK_WRITE 0x001480F2
#define FSCTL_LMR_REQUEST_RESILIENCY 0x001401D4
#define FSCTL_QUERY_NETWORK_INTERFACE_INFO 0x001401FC
#define FSCTL_DFS_GET_REFERRALS_EX 0x000601B0
#define FSCTL_VALIDATE_NEGOTIATE_INFO 0x00140204 

#define FSCTL_LMR_SET_LINK_TRACKING_INFORMATION 0x1400ec

//file access masks [#2]p.42
#define MAXIMAL_ACCESS 0x02000000 //This value indicates that the client is requesting an open to the file  with the highest level of access the client has on this file. If no access is granted for the client on this file, the server MUST fail the open with STATUS_ACCESS_DENIED. 

typedef struct _RESP_SMB2_TREE_CONNECT
{
  USHORT  StructureSize;
  UCHAR  ShareType;
  UCHAR  Reserved;
  ULONG  ShareFlags;
  ULONG  Capabilities;
  ULONG  MaximalAccess;
} RESP_SMB2_TREE_CONNECT;
typedef RESP_SMB2_TREE_CONNECT *PRESP_SMB2_TREE_CONNECT;

typedef struct _REQ_SMB2_TREE_DISCONNECT
{
  USHORT  StructureSize;
  USHORT  Reserved;
} REQ_SMB2_TREE_DISCONNECT;
typedef REQ_SMB2_TREE_DISCONNECT *PREQ_SMB2_TREE_DISCONNECT;

typedef struct _RESP_SMB2_TREE_DISCONNECT
{
  USHORT  StructureSize;
  USHORT  Reserved;
} RESP_SMB2_TREE_DISCONNECT;
typedef RESP_SMB2_TREE_DISCONNECT *PRESP_SMB2_TREE_DISCONNECT;

//create disposition
#define FILE_SUPERSEDE 0x00000000 //If the file already exists, supersede it. Otherwise, create the file. This value SHOULD NOT be used for a printer object. <16> 
#define FILE_OPEN 0x00000001 //If the file already exists, return success; otherwise, fail the operation. MUST NOT be used for a printer object. 
#define FILE_CREATE 0x00000002 //If the file already exists, fail the operation; otherwise, create the file. 
#define FILE_OPEN_IF 0x00000003 //Open the file if it already exists; otherwise, create the file. This value SHOULD NOT be used for a printer object. <17> 
#define FILE_OVERWRITE 0x00000004 //Overwrite the file if it already exists; otherwise, fail the operation. MUST NOT be used for a printer object. 
#define FILE_OVERWRITE_IF 0x00000005 //Overwrite the file if it already exists; otherwise, create the file. This value SHOULD NOT be used for a printer object. <18>

//create options
#define FILE_DIRECTORY_FILE 0x00000001 //The file being created or opened is a directory file. With this flag, the CreateDisposition field MUST be set to FILE_CREATE or FILE_OPEN_IF. With this flag, only the following CreateOptions values are valid: FILE_WRITE_THROUGH, and FILE_OPEN_FOR_BACKUP_INTENT. 
#define FILE_WRITE_THROUGH 0x00000002 //The server MUST propagate writes to this open to persistent storage before returning success to the client on write operations. 
#define FILE_SEQUENTIAL_ONLY 0x00000004 //A hint indicating that accesses to the file will be sequential. This flag value is incompatible with the FILE_RANDOM_ACCESS value, which indicates that the accesses to the file can be random. 
#define FILE_NO_INTERMEDIATE_BUFFERING 0x00000008 //The server or underlying object store SHOULD NOT cache data at intermediate layers and SHOULD allow it to flow through to persistent storage. 
#define FILE_NON_DIRECTORY_FILE 0x00000040 //The file being opened MUST NOT be a directory file or this call MUST be failed. This flag MUST NOT be used with FILE_DIRECTORY_FILE. 
#define FILE_NO_EA_KNOWLEDGE 0x00000200 //The caller does not understand how to handle extended attributes. If extended attributes are associated with the file being opened, the server MUST fail this request. 
#define FILE_OPEN_FOR_BACKUP_INTENT 0x00004000 //The file is being opened for backup intent. That is, it is being opened or created for the purposes of either a backup or a restore operation. Thus, the server MAY make appropriate checks to ensure that the caller is capable of overriding whatever security checks have been placed on the file to allow a backup or restore operation to occur. The server MAY choose to check for certain access rights to the file before checking the DesiredAccess field. 
#define FILE_RANDOM_ACCESS 0x00000800 //A hint that indicates that accesses to the file can be random; so sequential read-ahead operations SHOULD NOT be performed on the file. This flag value is incompatible with the FILE_SEQUENTIAL_ONLY value, which indicates that the accesses to the file will be sequential. 
#define FILE_NO_COMPRESSION 0x00008000 //The file cannot be compressed. 
#define FILE_OPEN_REPARSE_POINT 0x00200000 //If the file or directory being opened is a reparse point, open the reparse point itself rather than the target that the reparse point references. 
#define FILE_OPEN_NO_RECALL 0x00400000 //In an HSM (Hierarchical Storage Management) environment, this flag means the file should not be recalled from tertiary storage such as tape. The recall can take several minutes. The caller can specify this flag to avoid those delays.

typedef struct _REQ_SMB2_CREATE
{
  USHORT  StructureSize;
  UCHAR  SecurityFlags;
  UCHAR  RequestedOplockLevel;
  ULONG  ImpersonationLevel;
  ULONGLONG  SmbCreateFlags;
  ULONGLONG  Reserved;
  ULONG  DesiredAccess;
  ULONG  FileAttributes;
  ULONG  ShareAccess;
  ULONG  CreateDisposition;
  ULONG  CreateOptions;
  USHORT  NameOffset;
  USHORT  NameLength;
  ULONG  CreateContextsOffset;
  ULONG  CreateContextsLength;
} REQ_SMB2_CREATE;
typedef REQ_SMB2_CREATE *PREQ_SMB2_CREATE;

typedef struct _REQ_SMB2_ACCESS_MASK_ENCODING
{
  ULONG  File_Pipe_Printer_Access_Mask;
} REQ_SMB2_ACCESS_MASK_ENCODING;
typedef REQ_SMB2_ACCESS_MASK_ENCODING *PREQ_SMB2_ACCESS_MASK_ENCODING;

typedef struct _REQ_SMB2_DIRECTORY_ACCESS_MASK
{
  ULONG  Directory_Access_Mask;
} REQ_SMB2_DIRECTORY_ACCESS_MASK;
typedef REQ_SMB2_DIRECTORY_ACCESS_MASK *PREQ_SMB2_DIRECTORY_ACCESS_MASK;

typedef struct _REQ_SMB2_CREATE_CONTEXT
{
  ULONG  Next;
  USHORT  NameOffset;
  USHORT  NameLength;
  USHORT  Reserved;
  USHORT  DataOffset;
  ULONG  DataLength;
} REQ_SMB2_CREATE_CONTEXT;
typedef REQ_SMB2_CREATE_CONTEXT *PREQ_SMB2_CREATE_CONTEXT;

typedef struct _REQ_SMB2_CREATE_DURABLE_HANDLE_REQUEST
{
  ULONGLONG  DurableRequest[2];
} REQ_CREATE_DURABLE_HANDLE_REQUEST;
typedef REQ_CREATE_DURABLE_HANDLE_REQUEST *PREQ_CREATE_DURABLE_HANDLE_REQUEST;

typedef struct _REQ_SMB2_CREATE_DURABLE_HANDLE_RECONNECT
{
  ULONGLONG  Data[2];
} REQ_CREATE_DURABLE_HANDLE_RECONNECT;
typedef REQ_CREATE_DURABLE_HANDLE_RECONNECT *PREQ_CREATE_DURABLE_HANDLE_RECONNECT;

typedef struct _REQ_SMB2_CREATE_QUERY_MAXIMAL_ACCESS
{
  ULONGLONG  Timestamp;
} REQ_SMB2_CREATE_QUERY_MAXIMAL_ACCESS;
typedef REQ_SMB2_CREATE_QUERY_MAXIMAL_ACCESS *PREQ_SMB2_CREATE_QUERY_MAXIMAL_ACCESS;

typedef struct _REQ_SMB2_CREATE_ALLOCATION_SIZE
{
  ULONGLONG  AllocationSize;
} REQ_SMB2_CREATE_ALLOCATION_SIZE;
typedef REQ_SMB2_CREATE_ALLOCATION_SIZE *PREQ_SMB2_CREATE_ALLOCATION_SIZE;

typedef struct _REQ_SMB2_TIMEWARP_TOKEN
{
  ULONGLONG  Timestamp;
} REQ_SMB2_TIMEWARP_TOKEN;
typedef REQ_SMB2_TIMEWARP_TOKEN *PREQ_SMB2_TIMEWARP_TOKEN;

#define SMB2_OPLOCK_LEVEL_NONE 0x00 //No oplock was granted. 
#define SMB2_OPLOCK_LEVEL_II 0x01 //A level II oplock was granted. 
#define SMB2_OPLOCK_LEVEL_EXCLUSIVE 0x08 //An exclusive oplock was granted. 
#define SMB2_OPLOCK_LEVEL_BATCH 0x09 //A batch oplock was granted.

#define FILE_SUPERSEDED 0x00000000 //An existing file was deleted and a new file was created in its place. 
#define FILE_OPENED 0x00000001 //An existing file was opened. 
#define FILE_CREATED 0x00000002 //A new file was created.
#define FILE_OVERWRITTEN 0x00000003 //An existing file was overwritten.

typedef struct _SMB2_FILEID
{
  ULONGLONG Persistent;
  ULONGLONG Volatile;
} SMB2_FILEID;
typedef SMB2_FILEID* PSMB2_FILEID;

typedef struct _RESP_SMB2_CREATE
{
  USHORT  StructureSize;
  UCHAR  OplockLevel;
  UCHAR  Flags;
  ULONG  CreateAction;
  ULONGLONG  CreationTime;
  ULONGLONG  LastAccessTime;
  ULONGLONG  LastWriteTime;
  ULONGLONG  ChangeTime;
  ULONGLONG  AllocationSize;
  ULONGLONG  EndOfFile;
  ULONG  FileAttributes;
  ULONG  Reserved2;
  SMB2_FILEID  FileId;
  ULONG  CreateContextsOffset;
  ULONG  CreateContextsLength;
} RESP_SMB2_CREATE;
typedef RESP_SMB2_CREATE *PRESP_SMB2_CREATE;

#define SMB2_CLOSE_FLAG_POSTQUERY_ATTRIB 0x0001 //If set, the server MUST set the attribute fields in the response, as specified in section 2.2.16, to valid values. If not set, the client MUST NOT use the values that are returned in the response.

typedef struct _REQ_SMB2_CLOSE
{
  USHORT  StructureSize;
  USHORT  Flags;
  ULONG  Reserved;
  SMB2_FILEID FileId;
} REQ_SMB2_CLOSE;
typedef REQ_SMB2_CLOSE *PREQ_SMB2_CLOSE;

typedef struct _RESP_SMB2_CLOSE
{
  USHORT  StructureSize;
  USHORT  Flags;
  ULONG  Reserved;
  ULONGLONG  CreationTime;
  ULONGLONG  LastAccessTime;
  ULONGLONG  LastWriteTime;
  ULONGLONG  ChangeTime;
  ULONGLONG  AllocationSize;
  ULONGLONG  EndOfFile;
  ULONG  FileAttributes;
} RESP_SMB2_CLOSE;
typedef RESP_SMB2_CLOSE *PRESP_SMB2_CLOSE;

typedef struct _REQ_SMB2_FLUSH
{
  USHORT  StructureSize;
  USHORT  Reserved1;
  ULONG  Reserved2;
  SMB2_FILEID FileId;
} REQ_SMB2_FLUSH;
typedef REQ_SMB2_FLUSH *PREQ_SMB2_FLUSH;

typedef struct _RESP_SMB2_FLUSH
{
  USHORT  StructureSize;
  USHORT  Reserved;
} RESP_SMB2_FLUSH;
typedef RESP_SMB2_FLUSH *PRESP_SMB2_FLUSH;

typedef struct _REQ_SMB2_READ
{
  USHORT  StructureSize;
  UCHAR  Padding;
  UCHAR  Reserved;
  ULONG  Length;
  ULONGLONG  Offset;
  SMB2_FILEID FileId;
  ULONG  MinimumCount;
  ULONG  Channel;
  ULONG  RemainingBytes;
  USHORT  ReadChannelInfoOffset;
  USHORT  ReadChannelInfoLength;
} REQ_SMB2_READ;
typedef REQ_SMB2_READ *PREQ_SMB2_READ;

typedef struct _RESP_SMB2_READ
{
  USHORT  StructureSize;
  UCHAR  DataOffset;
  UCHAR  Reserved;
  ULONG  DataLength;
  ULONG  DataRemaining;
  ULONG  Reserved2;
} RESP_SMB2_READ;
typedef RESP_SMB2_READ *PRESP_SMB2_READ;

typedef struct _REQ_SMB2_WRITE
{
  USHORT  StructureSize;
  USHORT  DataOffset;
  ULONG  Length;
  ULONGLONG  Offset;
  SMB2_FILEID FileId;
  ULONG  Channel;
  ULONG  RemainingBytes;
  USHORT  WriteChannelInfoOffset;
  USHORT  WriteChannelInfoLength;
  ULONG  Flags;
} REQ_SMB2_WRITE;
typedef REQ_SMB2_WRITE *PREQ_SMB2_WRITE;

typedef struct _RESP_SMB2_WRITE
{
  USHORT  StructureSize;
  USHORT  Reserved;
  ULONG  Count;
  ULONG  Remaining;
  USHORT  WriteChannelInfoOffset;
  USHORT  WriteChannelInfoLength;
} RESP_SMB2_WRITE;
typedef RESP_SMB2_WRITE *PRESP_SMB2_WRITE;

typedef struct _RESP_SMB2_OPLOCK_BREAK_NOTIFY
{
  USHORT  StructureSize;
  UCHAR  OplockLevel;
  UCHAR  Reserved;
  ULONG  Reserved2;
  SMB2_FILEID FileId;
} RESP_SMB2_OPLOCK_BREAK_NOTIFY;
typedef RESP_SMB2_OPLOCK_BREAK_NOTIFY *PRESP_SMB2_OPLOCK_BREAK_NOTIFY;

typedef struct _RESP_SMB2_OPLOCK_BREAK_ACK
{
  USHORT  StructureSize;
  UCHAR  OplockLevel;
  UCHAR  Reserved;
  ULONG  Reserved2;
  SMB2_FILEID  FileId;
} RESP_SMB2_OPLOCK_BREAK_ACK;
typedef RESP_SMB2_OPLOCK_BREAK_ACK *PRESP_SMB2_OPLOCK_BREAK_ACK;

typedef struct _RESP_SMB2_OPLOCK_BREAK_RESP
{
  USHORT  StructureSize;
  UCHAR  OplockLevel;
  UCHAR  Reserved;
  ULONG  Reserved2;
  SMB2_FILEID  FileId;
} RESP_SMB2_OPLOCK_BREAK_RESP;
typedef RESP_SMB2_OPLOCK_BREAK_RESP *PRESP_SMB2_OPLOCK_BREAK_RESP;

typedef struct _REQ_SMB2_LOCK
{
  USHORT  StructureSize;
  USHORT  LockCount;
  ULONG  Reserved1;
  SMB2_FILEID  FileId;
  ULONGLONG  Offset;
  ULONGLONG  Length;
  ULONG  Flags;
  ULONG  Reserved2;
} REQ_SMB2_LOCK;
typedef REQ_SMB2_LOCK *PREQ_SMB2_LOCK;

typedef struct _RESP_SMB2_LOCK
{
  USHORT  StructureSize;
  USHORT  Reserved;
} RESP_SMB2_LOCK;
typedef RESP_SMB2_LOCK *PRESP_SMB2_LOCK;

typedef struct _REQ_SMB2_ECHO
{
  USHORT  StructureSize;
  USHORT  Reserved;
} REQ_SMB2_ECHO;
typedef REQ_SMB2_ECHO *PREQ_SMB2_ECHO;
typedef struct _RESP_SMB2_ECHO
{
  USHORT  StructureSize;
  USHORT  Reserved;
} RESP_SMB2_ECHO;
typedef RESP_SMB2_ECHO *PRESP_SMB2_ECHO;

typedef struct _REQ_SMB2_CANCEL
{
  USHORT  StructureSize;
  USHORT  Reserved;
} REQ_SMB2_CANCEL;
typedef REQ_SMB2_CANCEL *PREQ_SMB2_CANCEL;

typedef struct _REQ_SMB2_IOCTL
{
  USHORT  StructureSize;
  USHORT  Reserved;
  ULONG  CtlCode;
  SMB2_FILEID  FileId;
  ULONG  InputOffset;
  ULONG  InputCount;
  ULONG  MaxInputResponse;
  ULONG  OutputOffset;
  ULONG  OutputCount;
  ULONG  MaxOutputResponse;
  ULONG  Flags;
  ULONG  Reserved2;
} REQ_SMB2_IOCTL;
typedef REQ_SMB2_IOCTL *PREQ_SMB2_IOCTL;

#define SMB2_0_IOCTL_IS_FSCTL 0x00000001 //If Flags is set to this value, the request is an FSCTL request.

typedef struct _SRV_COPYCHUNK_COPY
{
  ULONGLONG  SourceKey[3];
  ULONG  ChunkCount;
  ULONG  Reserved;
  ULONGLONG  SourceOffset;
  ULONGLONG  TargetOffset;
  ULONG  Length;
} SRV_COPYCHUNK_COPY;
typedef SRV_COPYCHUNK_COPY *PSRV_COPYCHUNK_COPY;

typedef struct _RESP_SMB2_IOCTL
{
  USHORT  StructureSize;
  USHORT  Reserved;
  ULONG  CtlCode;
  SMB2_FILEID  FileId;
  ULONG  InputOffset;
  ULONG  InputCount;
  ULONG  OutputOffset;
  ULONG  OutputCount;
  ULONG  Flags;
  ULONG  Reserved2;
} RESP_SMB2_IOCTL;
typedef RESP_SMB2_IOCTL *PRESP_SMB2_IOCTL;

typedef struct _SRV_COPYCHUNK_COPY_RESPONSE
{
  ULONG  ChunksWritten;
  ULONG  ChunksBytesWritten;
  ULONG  TotalBytesWritten;
} SRV_COPYCHUNK_COPY_RESPONSE;
typedef SRV_COPYCHUNK_COPY_RESPONSE *PSRV_COPYCHUNK_COPY_RESPONSE;

typedef struct _SRV_SNAPSHOT_ARRAY
{
  ULONG  NumberOfSnapShots;
  ULONG  NumberOfSnapShotsReturned;
  ULONG  SnapShotArraySize;
} SRV_SNAPSHOT_ARRAY;
typedef SRV_SNAPSHOT_ARRAY *PSRV_SNAPSHOT_ARRAY;

typedef struct _SRV_REQUEST_RESUME_KEY
{
  ULONGLONG  ResumeKey[3];
  ULONG  ContextLength;
  ULONG  than;
} SRV_REQUEST_RESUME_KEY;
typedef SRV_REQUEST_RESUME_KEY *PSRV_REQUEST_RESUME_KEY;

//Flags
#define SMB2_RESTART_SCANS 0x01 //The server MUST restart the enumeration from the beginning, but the search pattern is not changed. 
#define SMB2_RETURN_SINGLE_ENTRY 0x02 //The server MUST only return the first entry of the search results. 
#define SMB2_INDEX_SPECIFIED 0x04 //The server SHOULD return entries beginning at the byte number specified by FileIndex.<29> 
#define SMB2_REOPEN 0x10 //The server MUST restart the enumeration from the beginning, and the search pattern MUST be changed to the provided value. This often involves silently closing and reopening the directory on the server side.

typedef struct _REQ_SMB2_QUERY_DIRECTORY
{
  USHORT  StructureSize;
  UCHAR  FileInformationClass;
  UCHAR  Flags;
  ULONG  FileIndex;
  SMB2_FILEID  FileId;
  USHORT  FileNameOffset;
  USHORT  FileNameLength;
  ULONG  OutputBufferLength;
} REQ_SMB2_QUERY_DIRECTORY;
typedef REQ_SMB2_QUERY_DIRECTORY *PREQ_SMB2_QUERY_DIRECTORY;

typedef struct _RESP_SMB2_QUERY_DIRECTORY
{
  USHORT  StructureSize;
  USHORT  OutputBufferOffset;
  ULONG  OutputBufferLength;
} RESP_SMB2_QUERY_DIRECTORY;
typedef RESP_SMB2_QUERY_DIRECTORY *PRESP_SMB2_QUERY_DIRECTORY;

typedef struct _REQ_SMB2_CHANGE_NOTIFY
{
  USHORT  StructureSize;
  USHORT  Flags;
  ULONG  OutputBufferLength;
  SMB2_FILEID  FileId;
  ULONG  CompletionFilter;
  ULONG  Reserved;
} REQ_SMB2_CHANGE_NOTIFY;
typedef REQ_SMB2_CHANGE_NOTIFY *PREQ_SMB2_CHANGE_NOTIFY;

typedef struct _RESP_SMB2_CHANGE_NOTIFY
{
  USHORT  StructureSize;
  USHORT  OutputBufferOffset;
  ULONG  OutputBufferLength;
} RESP_SMB2_CHANGE_NOTIFY;
typedef RESP_SMB2_CHANGE_NOTIFY *PRESP_SMB2_CHANGE_NOTIFY;

typedef struct _SMB2_FILE_NOTIFY_INFORMATION
{
  ULONG  NextEntryOffset;
  ULONG  Action;
  ULONG  FileNameLength;
} SMB2_FILE_NOTIFY_INFORMATION;
typedef SMB2_FILE_NOTIFY_INFORMATION *PSMB2_FILE_NOTIFY_INFORMATION;

#define SMB2_0_INFO_FILE 0x01 //The file information is requested. 
#define SMB2_0_INFO_FILESYSTEM 0x02 //The file system information is requested. 
#define SMB2_0_INFO_SECURITY 0x03 //The security information is requested. 
#define SMB2_0_INFO_QUOTA 0x04 //The file system quota information is requested.

typedef struct _REQ_SMB2_QUERY_INFO
{
  USHORT  StructureSize;
  UCHAR  InfoType;
  UCHAR  FileInfoClass;
  ULONG  OutputBufferLength;
  USHORT  InputBufferOffset;
  USHORT  Reserved1;
  ULONG  InputBufferLength;
  ULONG  AdditionalInformation;
  ULONG  Flags;
  SMB2_FILEID  FileId;
  UCHAR  ReturnSingle;
  UCHAR  RestartScan;
  USHORT  Reserved2;
  ULONG  SidListLength;
  ULONG  StartSidLength;
  ULONG  StartSidOffset;
} REQ_SMB2_QUERY_INFO;
typedef REQ_SMB2_QUERY_INFO *PREQ_SMB2_QUERY_INFO;

typedef struct _RESP_SMB2_QUERY_INFO
{
  USHORT  StructureSize;
  USHORT  OutputBufferOffset;
  ULONG  OutputBufferLength;
} RESP_SMB2_QUERY_INFO;
typedef RESP_SMB2_QUERY_INFO *PRESP_SMB2_QUERY_INFO;

typedef struct _REQ_SMB2_SET_INFO
{
  USHORT  StructureSize;
  UCHAR  InfoType;
  UCHAR  FileInfoClass;
  ULONG  BufferLength;
  USHORT  BufferOffset;
  USHORT  Reserved;
  ULONG  AdditionalInformation;
  SMB2_FILEID  FileId;
} REQ_SMB2_SET_INFO;
typedef REQ_SMB2_SET_INFO *PREQ_SMB2_SET_INFO;

typedef struct _RESP_SMB2_SET_INFO
{
  USHORT  StructureSize;
} RESP_SMB2_SET_INFO;
typedef RESP_SMB2_SET_INFO *PRESP_SMB2_SET_INFO;


typedef struct _RESP_VALIDATE_NEGOTIATE_INFO
{
  ULONG  Capabilities;
  ULONGLONG ServerGuid[2];
  USHORT  SecurityMode; 
  USHORT  DialectRevision;
} RESP_VALIDATE_NEGOTIATE_INFO;

typedef struct _RESP_NETWORK_INTERFACE_INFO
{
  ULONG Next;
  ULONG Ifindex;
  ULONG Capability;
  ULONG Reservered;
  ULONGLONG LinkSpeed;
  UCHAR SockAddress[128];
} RESP_NETWORK_INTERFACE_INFO;

#define RSS_CAPABLE 0x00000001
#define RDMA_CAPABLE 0x00000002

typedef struct _RESP_NETWORK_INTERFACE_INFO_SOCKADDR_IN
{
  USHORT Family;
  USHORT Port;
  ULONG IPAddress;
  ULONGLONG Resesrved;
} RESP_NETWORK_INTERFACE_INFO_SOCKADDR_IN;

#define InterNetwork 0x0002
#define InterNetworkV6 0x0017

typedef struct _REQ_FSCTL_PIPE_WAIT
{
  ULONGLONG Timeout;
  ULONG NameLength;
  UCHAR TimeoutSpecified;
  UCHAR Pad1;
  wchar_t Name[];

} REQ_FSCTL_PIPE_WAIT;
typedef REQ_FSCTL_PIPE_WAIT* PREQ_FSCTL_PIPE_WAIT;


typedef struct _NTLMv2_RESPONSE
{
  UCHAR Response[16];
  UCHAR RespType;
  UCHAR RespTypeHigh;
  USHORT Reserved1;
  ULONG Reserved2;
  ULONGLONG TimeStamp;
  ULONGLONG Challenge;
  ULONG Reserved3;

} NTLMv2_RESPONSE;
typedef  NTLMv2_RESPONSE  *PNTLMv2_RESPONSE;

typedef struct _AV_PAIR
{
  USHORT ID;
  USHORT Length;

} AV_PAIR;
typedef  AV_PAIR  *PAV_PAIR;


#pragma pack(pop)
