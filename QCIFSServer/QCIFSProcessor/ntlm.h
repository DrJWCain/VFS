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

//ntlm.h info harvested from [MS-NLMP] - v20080207 - [#3]
//http://msdn.microsoft.com/en-us/library/ms995330.aspx

#pragma once

static char sNTLMSSP[] = "NTLMSSP";
static char sNEGOEXTS[] = "NEGOEXTS";

#define NTLMSP_NAME_A   "NTLM"
#define NEGOSSP_NAME_A  "Negotiate"

typedef ULONG NTLM_NEGOTIATE_FLAGS;

static const ULONG NT_LM_NEGOTIATE = 0x00000001;
static const ULONG NT_LM_CHALLENGE = 0x00000002;
static const ULONG NT_LM_AUTHENTICATE = 0x00000003;

//Flags
static const ULONG NTLMSSP_NEGOTIATE_UNICODE = 0x00000001;
static const ULONG NTLMSSP_NEGOTIATE_OEM = 0x00000002;
static const ULONG NTLMSSP_NEGOTIATE_NETWARE = 0x00000100;
static const ULONG NTLMSSP_NEGOTIATE_NTLM = 0x00000200;
static const ULONG NTLMSSP_NEGOTIATE_OEM_DOMAIN_SUPPLIED = 0x00001000;
static const ULONG NTLMSSP_NEGOTIATE_OEM_WORKSTATION_SUPPLIED = 0x00002000;
static const ULONG NTLMSSP_NEGOTIATE_ALWAYS_SIGN = 0x00008000;
static const ULONG NTLMSSP_V2 = 0x00080000;
static const ULONG NTLMSSP_NEGOTIATE_56 = 0x80000000;
static const ULONG NTLMSSP_NEGOTIATE_KEY_EXCH = 0x40000000;
static const ULONG NTLMSSP_NEGOTIATE_128 = 0x20000000;
static const ULONG NTLMSSP_TARGET_INFO = 0x00800000;
static const ULONG NTLMSSP_NEGOTIATE_ACCEPT = 0x00020000;
static const ULONG NTLMSSP_LAN_MANAGER_KEY = 0x00000080;

//AVLen ID values
static const USHORT kMsvAvEOL = 0;// NONE; AvLen MUST be 0. 
static const USHORT kMsvAvNbComputerName = 1;// The server's NetBIOS computer name. The name MUST be in Unicode, and is not null-terminated. 
static const USHORT kMsvAvNbDomainName = 2;// The server's NetBIOS domain name. The name MUST be in Unicode, and is not null-terminated.
static const USHORT kMsvAvDnsComputerName = 3;// The server's Active Directory (AD) DNS computer name. The name MUST be in Unicode, and is not null-terminated.
static const USHORT kMsvAvDnsDomainName = 4;// The server's Active Directory (AD) DNS domain name. The name MUST be in Unicode, and is not null-terminated. 
static const USHORT kMsvAvDnsTreeName = 5;// The server's Active Directory (AD) DNS forest tree name. The name MUST be in Unicode, and is not null-terminated.
static const USHORT kMsvAvFlags = 6;// An optional field containing a 32-bit value indicating server configuration. The only defined value is 0x00000001, which indicates the server forces all authentication to the guest account. This indicator MAY be used by a client application to indicate forced authentication to the guest account. 
static const USHORT kMsvAvTimestamp = 7;// A FILETIME structure (as specified in [MS-DTYP] section 2.3.1) in little-endian byte order that contains the server local time. <12> 
static const USHORT kMsAvRestrictions = 8;// A Restrictions Encoding structure, as defined in section 2.2.2.2. The Restrictions field contains a structure representing the integrity level of the security principal, as well as a MachineID used to detect when

#pragma pack(push)
#pragma pack(1)

typedef struct _NTLM_FIELD_DESCRIPTOR
{
  USHORT NameLen;
  USHORT NameMaxLen;
  ULONG Offset;
} NTLM_FIELD_DESCRIPTOR;
typedef NTLM_FIELD_DESCRIPTOR* PNTLM_FIELD_DESCRIPTOR;

typedef struct _NTLM_MSG //[#3] p.13
{
  ULONGLONG Signature;
  ULONG MessageType;
} NTLM_MSG;
typedef NTLM_MSG* PNTLM_MSG;

typedef struct _NTLM_VERSION
{
  UCHAR Major;
  UCHAR Minor;
  USHORT Build;
  UCHAR Reserved[3];
  UCHAR Revision;
} NTLM_VERSION;
typedef NTLM_VERSION* PNTLM_VERSION;

typedef struct _NTLM_MSG_NEGOTIATE  //[#3] p.15
{
  ULONGLONG Signature;
  ULONG MessageType;
  NTLM_NEGOTIATE_FLAGS Flags;
  NTLM_FIELD_DESCRIPTOR Domain;
  NTLM_FIELD_DESCRIPTOR Workstation;
  NTLM_VERSION Version;
  //USHORT Buffer[1];
} NTLM_MSG_NEGOTIATE;
typedef NTLM_MSG_NEGOTIATE* PNTLM_MSG_NEGOTIATE;

typedef struct _NTLM_MSG_CHALLENGE  //[#3] p.17
{
  ULONGLONG Signature;
  ULONG MessageType;
  NTLM_FIELD_DESCRIPTOR TargetName;
  NTLM_NEGOTIATE_FLAGS Flags;
  ULONGLONG ServerChallenge;
  ULONGLONG Reserved;
  NTLM_FIELD_DESCRIPTOR TargetInfo;
  NTLM_VERSION Version;
//  USHORT Payload[1];
} NTLM_MSG_CHALLENGE;
typedef NTLM_MSG_CHALLENGE* PNTLM_MSG_CHALLENGE;

typedef struct _NTLM_MSG_AUTHENTICATE  //[#3] p.20
{
  ULONGLONG Signature;
  ULONG MessageType;
  NTLM_FIELD_DESCRIPTOR LmChallengeResponse;
  NTLM_FIELD_DESCRIPTOR NtChallengeResponse;
  NTLM_FIELD_DESCRIPTOR Domain;
  NTLM_FIELD_DESCRIPTOR UserName;
  NTLM_FIELD_DESCRIPTOR Workstation;
  NTLM_FIELD_DESCRIPTOR EncryptedRandomSessionKey;
  NTLM_NEGOTIATE_FLAGS Flags;
  NTLM_VERSION Version;
  ULONGLONG ServerChallenge;
  ULONGLONG Reserved;
  //USHORT Payload[1];
} NTLM_MSG_AUTHENTICATE;
typedef NTLM_MSG_AUTHENTICATE* PNTLM_MSG_AUTHENTICATE;

typedef struct _NTLM_AV_PAIR
{
  USHORT Id;
  USHORT Len;
} NTLM_AV_PAIR;
typedef NTLM_AV_PAIR* PNTLM_AV_PAIR;

static USHORT getAVPairsLen(const vfs::String& sAVName)
{
  const USHORT sAVPairLen(sAVName.size()*sizeof(WCHAR) + sizeof(NTLM_AV_PAIR));
  return 4*sAVPairLen  //the AV_Pairs
    + sizeof(NTLM_AV_PAIR) + sizeof(FILETIME) //server time
    + sizeof(NTLM_AV_PAIR); //end of list
}

static PNTLM_AV_PAIR writeAVPair(const vfs::String& sVal, const USHORT id, const PNTLM_AV_PAIR pAvPair)
{
  pAvPair->Id = id;
  pAvPair->Len = (sVal.size())*sizeof(WCHAR);
  const PUCHAR pPayload(reinterpret_cast<PUCHAR>(pAvPair+1));
  memcpy(pPayload, sVal.c_str(), pAvPair->Len);
  return reinterpret_cast<PNTLM_AV_PAIR>(pPayload + pAvPair->Len);
}

static PUCHAR writeAVPairs(const vfs::String& sVal, const PUCHAR pAVPairStart)
{
  PNTLM_AV_PAIR pAVPair = reinterpret_cast<PNTLM_AV_PAIR>(pAVPairStart);
  pAVPair = writeAVPair(sVal, kMsvAvNbDomainName, pAVPair);
  pAVPair = writeAVPair(sVal, kMsvAvNbComputerName, pAVPair);
  pAVPair = writeAVPair(sVal, kMsvAvDnsDomainName, pAVPair);
  pAVPair = writeAVPair(sVal, kMsvAvDnsComputerName, pAVPair);
  pAVPair->Id = kMsvAvTimestamp;
  pAVPair->Len = sizeof(FILETIME);
  GetSystemTimeAsFileTime(reinterpret_cast<LPFILETIME>(pAVPair+1));
  pAVPair = reinterpret_cast<PNTLM_AV_PAIR>(reinterpret_cast<PUCHAR>(pAVPair+1) + sizeof(FILETIME));
  pAVPair->Id = kMsvAvEOL;
  pAVPair->Len = 0;
  return reinterpret_cast<PUCHAR>(pAVPair+1);
}

#pragma pack(pop)
