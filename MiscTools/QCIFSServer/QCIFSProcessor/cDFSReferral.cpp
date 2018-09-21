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

//cDFSReferral.cpp

#include "StdAfx.h"
#include ".\cdfsreferral.h"

cDFSReferral::cDFSReferral(const std::wstring &sDFSReferralPath)
: m_sDFSReferralPath(sDFSReferralPath)
{
  // reads in and creates the referral database
  generateReferralBase();
}

cDFSReferral::~cDFSReferral(void)
{
}

bool cDFSReferral::resolveDFSPath(std::wstring &sPath)
{
  stripReferralBase(sPath);

  cLockGuard lg(&m_AccessLock);

  // look for the path in our referral database
  std::map<std::wstring, std::wstring>::iterator it = m_ReferralMap.find(sPath);
  bool bRet(true);
  if (it != m_ReferralMap.end())
  {
    bRet = (it->first.compare(L"") == 0);
  }

  return bRet;
}

std::wstring cDFSReferral::findReferral(const std::wstring &krsPath)
{
  // return the referral path, given the source path
  std::wstring sPath(krsPath);
  stripReferralBase(sPath);

  std::wstring sRetPath;

  cLockGuard lg(&m_AccessLock);
  std::map<std::wstring, std::wstring>::iterator it = m_ReferralMap.find(sPath);
  if (it != m_ReferralMap.end())
  {
    sRetPath = m_sReferralBase + it->second;
  }

  return sRetPath;
}

void cDFSReferral::setReferralBase(const std::wstring &refBase)
{
  // set the root of our referral paths
  cLockGuard lg(&m_AccessLock);
  m_sReferralBase = refBase;
}

void cDFSReferral::resetReferralBase()
{
  // reset the root of our referral path and read in the database again
  {
    cLockGuard lg(&m_AccessLock);
    m_sReferralBase.clear();
    m_ReferralMap.clear();
  }
  generateReferralBase();
}

void cDFSReferral::generateReferralBase()
{
  // read the referral database from the file at m_sDFSReferralPath
  // format is alternate lines of source and destination path
  {
    cLockGuard lg(&m_AccessLock);
    m_ReferralMap.insert(std::map<std::wstring, std::wstring>::value_type(L"", L""));
  }

  // read referral file
  FILE *pFile = _wfopen(m_sDFSReferralPath.c_str(), L"r");

  if (pFile != NULL)
  {
    wchar_t pathBuffer[MAX_PATH];

    {
      cLockGuard lg(&m_AccessLock);
      // read in a line from the file
      while (fgetws(pathBuffer, MAX_PATH, pFile) != NULL)
      {
        std::wstring sPath(pathBuffer);
        // strip trailing newline characters
        size_t pos = sPath.find(L"\n");
        if (pos != std::wstring::npos)
          sPath = sPath.substr(0, pos);
        // there should be a partnering referral line
        if (fgetws(pathBuffer, MAX_PATH, pFile) != NULL)
        {
          std::wstring sRef(pathBuffer);
          pos = sRef.find(L"\n");
          if (pos != std::wstring::npos)
            sPath = sRef.substr(0, pos);

          m_ReferralMap.insert(std::map<std::wstring, std::wstring>::value_type(sPath, sRef));
        }
        else
          // set referral to root
          m_ReferralMap.insert(std::map<std::wstring, std::wstring>::value_type(sPath, L""));
      }
    }

    fclose(pFile);
  }
}

void cDFSReferral::stripReferralBase(std::wstring &rsPath)
{
  // remove the DFS root from the beginning of the path
  cLockGuard lg(&m_AccessLock);
  size_t pos = rsPath.find(m_sReferralBase);
  if (pos == 0)
  {
    rsPath = rsPath.substr(pos + m_sReferralBase.size());
  }
}