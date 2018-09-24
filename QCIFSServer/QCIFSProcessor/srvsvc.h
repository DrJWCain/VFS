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

//srvsvc.h

//[#10] MS-SRVS v20080207

#pragma once

#pragma pack(push)
#pragma pack(1)

#define STYPE_DISKTREE 0x00000000//Disk drive 
#define STYPE_PRINTQ 0x00000001 //Print queue 
#define STYPE_DEVICE 0x00000002 //Communication device 
#define STYPE_IPC 0x00000003 //Interprocess communication (IPC) Zero or more of the following flags MAY be combined with the values in the previous table to further specify the characteristics of a shared resource. 
#define STYPE_SPECIAL 0x80000000 //Special share reserved for interprocess communication (IPC$) or remote administration of the server (ADMIN$). Can also refer to administrative shares such as C$, D$, E$, and so forth.
#define STYPE_TEMPORARY 0x40000000 //A temporary share that is not persisted for creation each time the file server initializes
typedef struct _SRV_SVC_RESPONSE
{
  ULONG TotalEntries;
  ULONG ResumeHandle;
  ULONG ReturnValue;
}SRV_SVC_RESPONSE, *PSRV_SVC_RESPONSE;

typedef struct _SHARE_ENUM_STRUCT 
{ 
  DWORD Level;
  DWORD SwitchValue;
  //SHARE_ENUM_UNION ShareInfo;
} SHARE_ENUM_STRUCT, *PSHARE_ENUM_STRUCT, *LPSHARE_ENUM_STRUCT;

typedef struct _SHARE_INFO_1_CONTAINER
{
  ULONG Pointer;
  DWORD EntriesRead;
  //LPSHARE_INFO_1 Buffer;
} SHARE_INFO_1_CONTAINER;
typedef SHARE_INFO_1_CONTAINER* PSHARE_INFO_1_CONTAINER;

typedef struct _SHARE_INFO_1
{
  wchar_t* shi1_netname;
  DWORD shi1_type;
  wchar_t* shi1_remark;
} SHARE_INFO_1, *PSHARE_INFO_1, *LPSHARE_INFO_1;

typedef struct _SHARE_INFO_1_BUFFER
{
  ULONG MaxCount;
  ULONG Offset;
  ULONG ActualCount;
} SHARE_INFO_1_BUFFER, *PSHARE_INFO_1_BUFFER;

#pragma pack(pop)