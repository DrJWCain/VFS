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

// SocketServer.cpp : Defines the entry point for the dll application.
//

#include "stdafx.h"

#include "cSocketServer.h"

#include "iPacketProcessor.h"
#include "iSocketServer.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved)
{
  WSADATA              wsaData;
  int                  Ret;

  switch (ul_reason_for_call)
  {
  case DLL_PROCESS_ATTACH:
    // Initialize Winsock version 2.2
    if ((Ret = WSAStartup(MAKEWORD(2,2), &wsaData)) != 0)
    {
      // NOTE: Since Winsock failed to load we cannot use WSAGetLastError 
      // to determine the error code as is normally done when a Winsock 
      // API fails. We have to report the return status of the function.

      QSOS((L"WSAStartup failed with error %d", Ret));
    }
    break;
  case DLL_PROCESS_DETACH:
    WSACleanup();
    break;
  //case DLL_THREAD_ATTACH:
  //  break;
  //case DLL_THREAD_DETACH:
  //  break;
  default:
    break;
  }

  return TRUE;
}
