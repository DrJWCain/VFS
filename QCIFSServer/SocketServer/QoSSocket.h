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

//QoSSocket.h

#pragma once

// from http://msdn.microsoft.com/en-us/magazine/bb985821.aspx

// Create a QoS Socket. The only socket types allowed are SOCK_STREAM and 
// SOCK_DGRAM.
//
// Required for creating QoS TCP connecting socket and listening socket.  
// A socket handle returned from accept call is automatically created 
// from RSVPSP if the listening socket is a QoS socket.
// 
// Required for creating QoS UDP socket.
// 
static SOCKET OpenQoSSocket(INT iSocketType)
{
  DWORD bufferSize = 0;
  DWORD numProtocols = WSAEnumProtocols(NULL, NULL, &bufferSize);
  if((numProtocols != SOCKET_ERROR) && (WSAGetLastError() != WSAENOBUFS))
  {
    QSOS (( L"WSAEnumProtocols Failed [%d]", WSAGetLastError() )); 
    return(INVALID_SOCKET);  
  }
  else
  {
    LPWSAPROTOCOL_INFO qosProtocol;
    LPWSAPROTOCOL_INFO installedProtocols = (LPWSAPROTOCOL_INFO)malloc(bufferSize);

    numProtocols = WSAEnumProtocols(NULL,
      installedProtocols,
      &bufferSize);
    if(numProtocols == SOCKET_ERROR)
    {
      QSOS (( L"WSAEnumProtocols Failed [%d]", WSAGetLastError())); 
      return(INVALID_SOCKET);
    } 
    else 
    {
      qosProtocol = installedProtocols;

      for(DWORD i=0 ; i< numProtocols ; qosProtocol++, i++)
      {
        if  ((qosProtocol->dwServiceFlags1 & XP1_QOS_SUPPORTED)
          && (qosProtocol->iSocketType == iSocketType)
          && (qosProtocol->iAddressFamily == AF_INET)
          && (qosProtocol->iProtocol == IPPROTO_TCP))
        {
          break;
        }              
      }
    }

    //
    // Now open the socket.
    //
    SOCKET fd = WSASocket(0, 
      0, 
      0, 
      qosProtocol,          // Use the RSVPSP we found
      0, 
      WSA_FLAG_OVERLAPPED); // *MUST* be overlapped!

    //
    // De-allocate protocol info buffer
    //
    free(installedProtocols);

    return(fd);
  }        
}
