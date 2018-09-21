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

//cOpLock.cpp

#include "stdafx.h"

#if CIFS 

#include ".\coplock.h"

cOpLock::cOpLock(const std::wstring &sPath, const PSMB_HEADER pHeader, const USHORT Fid, const SOCKET socket)
: m_sPath(sPath)
, m_Fid(Fid)
, m_socket(socket)
{
  memcpy(&m_header, reinterpret_cast<const void *>(pHeader), sizeof(SMB_HEADER));
  m_Semaphore = CreateSemaphore(NULL, 0, 1, NULL);
}

cOpLock::~cOpLock(void)
{
  CloseHandle(m_Semaphore);
}

void cOpLock::P()
{
  WaitForSingleObject(m_Semaphore, INFINITE);
}

void cOpLock::V()
{
  ReleaseSemaphore(m_Semaphore, 1, NULL);
}

int cOpLock::compare(const std::wstring sCmpPath)
{
  return m_sPath.compare(sCmpPath);
}

PSMB_HEADER cOpLock::Header()
{
  return &m_header;
}

USHORT cOpLock::Fid()
{
  return m_Fid;
}

SOCKET cOpLock::Connection()
{
  return m_socket;
}

#endif // CIFS 