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

//cOpLock.h

#pragma once

#if CIFS 

#include "mycifs.h"



class cOpLock : public vfs::cRefCount
{
public:
  cOpLock(const std::wstring &sPath, const PSMB_HEADER pHeader, const USHORT Fid, const SOCKET socket);
  ~cOpLock(void);

  void P();
  void V();
  int compare(const std::wstring sCmpPath);
  PSMB_HEADER Header();
  USHORT Fid();
  SOCKET Connection();
  USHORT getTid() { return m_header.Tid; };

private:
  std::wstring m_sPath;
  SMB_HEADER m_header;
  USHORT m_Fid;
  HANDLE m_Semaphore;
  const SOCKET m_socket;
};

#endif // CIFS 