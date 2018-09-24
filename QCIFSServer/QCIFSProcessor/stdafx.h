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

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define CIFS 0 // set to 1 to build SMB1 - requires cifs.h from WDK include folder

#include <iostream>
#include <tchar.h>
#include <list>
#include <vector>
#include <string>
#include <map>
#include <set>


#define _USING_V110_SDK71_

#define NTDDI_VERSION NTDDI_VISTA 
#define _WIN32_WINNT _WIN32_WINNT_VISTA 
#define NT_ERROR(Status)                ((((ULONG)(Status)) >> 30) == 3)

#define WIN32_NO_STATUS
#include <winsock2.h>
#include <Mswsock.h>
#undef WIN32_NO_STATUS
#include <ntstatus.h>
#include <ntsecapi.h>
#include <process.h>
#include <winioctl.h>

#include "..\..\..\KernelSDK\Include\QKernelSDK.h"
#include "mycifs.h"


extern vfs::iModuleContext::Ptr gModuleContext;

struct cActivityScope
{
  cActivityScope (vfs::iActivity::Ptr activity, int id) 
  : Activity (activity)
  {
    Activity->activityStart (id);
  }
  
  cActivityScope (vfs::iActivity::Ptr activity, int id, int value) 
  : Activity (activity)
  {
    Activity->activityStart (id, value);
  }
  
  cActivityScope (vfs::iActivity::Ptr activity, int id, const vfs::String& comment) 
  : Activity (activity)
  {
    Activity->activityStart (id, vfs::narrow (comment).c_str ());
  }
  
  ~cActivityScope ()
  {
    Activity->activityDone ();
  }

private:
  vfs::iActivity::Ptr Activity;
};

enum eActivityMsg { kCompositeRead
  , kTreeRead
  , kTreeClose
  , kTreeCloseRemove
  , kTreeCloseCallComposite
  , kProcessClose
  , kGetHeader
  , kSMBBuffer
  , kCreateBuffer
  , kRXBuffer
  , kTXBuffer
  , kBufferFree
  , kCreateResponse
  , kGetBlocks
  , kGetDispatchCommand
  , kGetResponses
  , kSendResponses };
typedef std::map<DWORD, vfs::iActivity::Ptr> tActivityMap;

extern vfs::iActivity::Ptr getActivity();

//#define JC_EXPERIMENTAL_RPC_TEST

static const int gkBufferSize = 0x10000 + 0x100;//max size required by SMB2
//static const int gkBufferSize = 0x10000 + 4;//stresses SMB2 when reading writing - buffer size is 64K payload + 0x70 byte SMB2 header + 4 byte NetBIOS length
static const int gkSMB2BufferSize = 0x10000;


