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


// Insert your headers here

#include <vector>
#include <map>
#include <list>
#include <set>
#include <memory>

#include "..\..\KernelSDK\Include\QKernelSDK.h"

#include <winsock2.h>
#include <Mswsock.h>
#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <iphlpapi.h>


#include "..\..\KernelSDK\include\QWinBase.h"

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

struct cActivityScopeId
{
  cActivityScopeId (vfs::iActivity::Ptr activity, int act, int id) 
  : Activity (activity)
  {
    Activity->activityStartId (act, id);
  }
  
  ~cActivityScopeId ()
  {
    Activity->activityDone ();
  }

private:
  vfs::iActivity::Ptr Activity;
};

enum eActivityMsg { kSent
  , kRecvNotify
  , kRecvInterlockedDec
  , kRecvDo
  , kRecvWSARecv
  , kRecvInterlocked
  , kRecvBufferAssign
  , kRecvIOBufferBytes
  , kRecvIOBufferSize
  , kWaitSend
  , kSend };


