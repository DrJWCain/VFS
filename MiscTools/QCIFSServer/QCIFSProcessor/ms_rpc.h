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

//ms_rpc.h

#pragma once

#pragma pack(push)
#pragma pack(1)

//http://www.opengroup.org/onlinepubs/009629399/chap12.htm#tagtcjh_28
typedef struct _RPC_HEADER
{
  UCHAR  rpc_vers;        /* 00:01 RPC version */
  UCHAR  rpc_vers_minor;      /* 01:01 minor version */
  UCHAR  PTYPE;               /* 02:01 packet type */
  UCHAR  pfc_flags;           /* 03:01 flags (see PFC_... ) */
  ULONG packed_drep;   /* 04:04 NDR data representation format label */
  USHORT frag_length;         /* 08:02 total length of fragment */
  USHORT auth_length;         /* 10:02 length of auth_value */
  ULONG  call_id;             /* 12:04 call identifier */
} RPC_HEADER, *PRPC_HEADER;

typedef struct _RPC_BIND_ACK
{
  RPC_HEADER Common;
  USHORT max_xmit_frag;      /* 16:02 max transmit frag size */
  USHORT max_recv_frag;      /* 18:02 max receive  frag size */
  ULONG assoc_group_id;     /* 20:04 returned assoc_group_id */
} RPC_BIND_ACK, *PRPC_BIND_ACK;

typedef struct _RPC_REQ_RESPONSE
{
  RPC_HEADER Common;
  ULONG AllocHint;
  USHORT PContId;
  UCHAR CancelCount;
  UCHAR Reserved;
} RPC_REQ_RESPONSE;
typedef RPC_REQ_RESPONSE* PRPC_REQ_RESPONSE;

#pragma pack(pop)