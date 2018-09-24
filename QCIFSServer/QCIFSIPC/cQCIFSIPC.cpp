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

#include "stdafx.h"

#include ".\cqcifsipc.h"
#include "cIPC.h"
#include "cDFSDatabase.h"

#include "..\SocketServer\iSocketServer.h"
#include "..\QCIFSProcessor\iTreeResourceFactory.h"

using namespace vfs;
using namespace std;

static const cPtr<iTreeResource> pTreeResource = new cIPC();

class cTreeResourceFactory : public iTreeResourceFactory, public cRefCount
{
public:
  cPtr<iTreeResource> getTreeResource(const String& sLoginName){return pTreeResource;}
  void removeTreeResource(const vfs::String& sLoginName){};
};

static const cPtr<iTreeResourceFactory> gpIPCFactory = new cTreeResourceFactory();

cPtr<iTreeResourceFactory> cQCIFSIPC::getTreeResourceFactory(void)
{
  return gpIPCFactory;
}

const String cQCIFSIPC::sIPAddrRegex(L"[[:digit:]]{1,3}\\.[[:digit:]]{1,3}\\.[[:digit:]]{1,3}\\.[[:digit:]]{1,3}");

cQCIFSIPC::cQCIFSIPC()
: m_pDFSDatabase(new cDFSDatabase)
{
}

void cQCIFSIPC::addShare(const String& sShare)
{
  const boost::wregex sPathRegex(L"\\\\" + sIPAddrRegex + L"\\\\(.*)");
  boost::wsmatch matches;
  try
  {
    if (boost::regex_match(sShare, matches, sPathRegex))
    {
      cLockGuard lg(&m_ShareAccess);
      m_Shares.insert(toLower(sShare));
      return;
    }
  }
  catch (const exception&)
  {
  }

  const std::vector<String> IPAddresses = iSocketServerFactory::singleton().getIpAddresses();
  cLockGuard lg(& m_ShareAccess);
  for (std::vector<String>::const_iterator cit = IPAddresses.begin()
    ; IPAddresses.end() != cit
    ; ++cit)
  {
    const String& sIPAddr(*cit);
    m_Shares.insert(L"\\" + sIPAddr + L"\\" + toLower(sShare));
  }
}

bool cQCIFSIPC::hasShare(const String& sShare)
{
  cLockGuard lg(&m_ShareAccess);
  return m_Shares.end() != m_Shares.find(toLower(sShare));
}

void cQCIFSIPC::addDFSPath(const String& sDisplayPath, const String& sDFSPath)
{
  m_pDFSDatabase->addDFSPath(sDisplayPath, sDFSPath);
}

String cQCIFSIPC::getDFSPath(const String& sDisplayPath) const
{
  const String sIPAddress = findIPAddress(sDisplayPath);
  if (!sIPAddress.empty())
  {
    const String sSharePath = removeIP(sDisplayPath);
    const String sDFSPath = m_pDFSDatabase->getDFSPath(sSharePath);
    if (!sDFSPath.empty())
      return L"\\" + sIPAddress + sDFSPath;
  }
  return L"";
}

void cQCIFSIPC::removeDFSPath(const String& sDisplayPath)
{
  m_pDFSDatabase->removeDFSPath(sDisplayPath);
}

vfs::String cQCIFSIPC::removeDFSPart(const String& psPath) const
{
  throw cNotFound(__FILE__, __LINE__, L"cQCIFSIPC::removeDFSPart() - RFH big bodge!!!!");

  QTRACE((L"cQCIFSIPC::removeDFSPart() - %s", psPath.c_str()));
  const String minusIP = removeIP(psPath);
  const boost::wregex sPathRegex(String(L"\\\\[^\\\\]+") + String(L"(\\\\.*)"));
  boost::wsmatch matches;
  try
  {
    if (boost::regex_match(minusIP, matches, sPathRegex))
    {
      int i = 0;
      for (boost::wsmatch::const_iterator cit = matches.begin()
        ; matches.end() != cit
        ; ++cit)
      {
        const String sMatch(*cit);
        QTRACE((L"cQCIFSIPC::removeDFSPart() - %d, %s", i, sMatch.c_str()));
        i++;
      }
      return matches[1];
    }
  }
  catch (const exception&)
  {
  }
  return L"";
}

vfs::String cQCIFSIPC::findIPAddress(const vfs::String& sDisplayPath) const
{
  try
  {
    const size_t firstSlash = sDisplayPath.find_first_of(L"\\");
    if (String::npos != firstSlash)
    {
      const size_t secondSlash = sDisplayPath.substr(firstSlash+1).find_first_of(L"\\");
      if (String::npos != secondSlash)
      {
        const String sPathIPAddress = sDisplayPath.substr(firstSlash+1, firstSlash + secondSlash);
        const std::vector<String> sIPAddresses = iSocketServerFactory::singleton().getIpAddresses();
        for (std::vector<String>::const_iterator cit = sIPAddresses.begin()
          ; cit != sIPAddresses.end()
          ; ++cit)
        {
          const String& sIPAddress(*cit);
          if (0 == sPathIPAddress.compare(sIPAddress))
            return sPathIPAddress;
        }
      }
    }
  }
  catch (const exception&)
  {
  }
  catch (const cNotFound&)
  {
  }

  return L"";
}

inline vfs::String cQCIFSIPC::removeIP(const vfs::String& sPath) const
{
  const boost::wregex sPathRegex(L"\\\\" + sIPAddrRegex + L"(\\\\.*)");
  boost::wsmatch matches;
  try
  {
    if (boost::regex_match(sPath, matches, sPathRegex))
      return matches[1];
  }
  catch (const exception&)
  {
  }
  return L"";
}