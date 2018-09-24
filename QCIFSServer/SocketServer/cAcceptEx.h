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

//cAcceptEx.h

#pragma once

#include "iacceptex.h"

class cAcceptEx : public iAcceptEx, public vfs::cRefCount
{
public:
  QDEFINE_SINGLETON_IMPL(cAcceptEx, L"{F968E230-B3AD-45f3-9483-622C0AF7524E} // cAcceptEx", iAcceptEx);

  cAcceptEx(void);
  ~cAcceptEx(void);

  void QAPI logSummary() {QTRACE((L"cAcceptEx - wraps WSA AcceptEx and GetAcceptExSockaddrs"));}
  void QAPI logDetailedSummary() {logSummary();}

  BOOL AcceptEx(const SOCKET sListenSocket
    , cSocketServer* const pSocketServer
    , const vfs::cPtr<cListeningSocket> socket
    , const UINT_PTR associatedSessionID);

  VOID GetAcceptExSockaddrs(PVOID lpOutputBuffer
    , DWORD dwReceiveDataLength
    , DWORD dwLocalAddressLength
    , DWORD dwRemoteAddressLength
    , sockaddr **LocalSockaddr
    , LPINT LocalSockaddrLength
    , sockaddr **RemoteSockaddr
    , LPINT RemoteSockaddrLength);

private:  //private member functions
  template <typename func>
  bool assignFunc(SOCKET sock, func& fn, GUID fnGuid);

private:  //private member variables
  LPFN_ACCEPTEX m_pfnAcceptEx;
  LPFN_GETACCEPTEXSOCKADDRS m_pfnGetAcceptExSockaddrs;
  vfs::cLock m_acceptExLock;
  vfs::cLock m_acceptExLockExSockaddrs;
};