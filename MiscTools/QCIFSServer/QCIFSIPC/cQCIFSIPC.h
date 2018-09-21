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

//cQCIFSIPC.h

#pragma once

#include "iqcifsipc.h"

class iTreeResourceFactory;
class cDFSDatabase;

class cQCIFSIPC : public iQCIFSIPC, public vfs::cRefCount
{
public:
  QDEFINE_SINGLETON_IMPL(cQCIFSIPC, L"{A4A25B4A-D997-452f-BA5E-61CBC61F6DEB} // cQCIFSIPC", iQCIFSIPC);

  cQCIFSIPC();

  vfs::cPtr<iTreeResourceFactory> getTreeResourceFactory();

  void addShare(const vfs::String& sShare);
  bool hasShare(const vfs::String& sShare);

  void addDFSPath(const vfs::String& sDisplayPath, const vfs::String& sDFSPath);
  vfs::String getDFSPath(const vfs::String& sDisplayPath) const;
  void removeDFSPath(const vfs::String& sDisplayPath);

  vfs::String removeDFSPart(const vfs::String& psPath) const;

  void QAPI logSummary(void) throw() {QTRACE((L"iQCIFSIPC"));}
  void QAPI logDetailedSummary(void) throw() {QTRACE((L"iQCIFSIPC - IPC handler"));}

private:  //private member functions
  vfs::String findIPAddress(const vfs::String& sDisplayPath) const;
  inline vfs::String removeIP(const vfs::String& sPath) const;

private:  //private member variables
  const vfs::cPtr<cDFSDatabase> m_pDFSDatabase;
  static const vfs::String sIPAddrRegex;

  typedef std::set<vfs::String> tShares;
  vfs::cLock m_ShareAccess;
  tShares m_Shares;
};
