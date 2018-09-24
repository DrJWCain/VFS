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

//cDFSDatabase.cpp

#include "StdAfx.h"
#include ".\cdfsdatabase.h"

using namespace vfs;
using namespace std;

const String cDFSDatabase::m_shareFinder(L"\\\\?([^\\\\]+)");

cDFSDatabase::cDFSDatabase(void)
{
}

cDFSDatabase::~cDFSDatabase(void)
{
}

void cDFSDatabase::addDFSPath(const String& sDisplayPath, const String& sDFSPath)
{
  const String sRoot = pathRoot(sDisplayPath);
  const String sSubPath = subPath(sDisplayPath);
  if (sSubPath.empty())
  {
    cLockGuard lg(&m_namesAccess);
    m_nameMap[sRoot] = sDFSPath;
  }
  else
  {
    const cPtr<cDFSDatabase> pDFSDatabase = getOrCreateDatabase(sRoot);
    if (pDFSDatabase.isValid())
      pDFSDatabase->addDFSPath(sSubPath, sDFSPath);
  }
}

String cDFSDatabase::getDFSPath(const String& sDisplayPath) const
{
  const String sRoot = pathRoot(sDisplayPath);
  const String sSubPath = subPath(sDisplayPath);
  if (sSubPath.empty())
  {
    cLockGuard lg(&m_namesAccess);
    const tNameMap::const_iterator finder = m_nameMap.find(sRoot);
    if (m_nameMap.end() != finder)
      return finder->second;
  }
  else
  {
    const cPtr<cDFSDatabase> pDFSDatabase = getDatabase(sRoot);
    if (pDFSDatabase.isValid())
      return pDFSDatabase->getDFSPath(sSubPath);
  }

  return L"";
}

void cDFSDatabase::removeDFSPath(const String& sDisplayPath)
{
  const String sRoot = pathRoot(sDisplayPath);
  const String sSubPath = subPath(sDisplayPath);
  if (sSubPath.empty())
  {
    cLockGuard lg(&m_namesAccess);
    const tNameMap::iterator finder = m_nameMap.find(sRoot);
    if (m_nameMap.end() != finder)
      m_nameMap.erase(finder);
  }
  else
  {
    const cPtr<cDFSDatabase> pDFSDatabase = getDatabase(sRoot);
    if (pDFSDatabase.isValid())
    {
      pDFSDatabase->removeDFSPath(sSubPath);
      if (pDFSDatabase->empty())
      {
        cLockGuard lg(&m_childAccess);
        for (tDatabaseMap::iterator cit = m_childDatabase.begin()
          ; m_childDatabase.end() != cit
          ; ++cit)
        {
          if (pDFSDatabase == cit->second)
          {
            m_childDatabase.erase(cit);
            cit = m_childDatabase.begin();  //iterator will become invalid after erase
          }
        }
      }
    }
  }
}

inline String cDFSDatabase::subPath(const String& sPath) const
{
  static const boost::wregex shareFinder(m_shareFinder + L"(\\\\.*)");
  boost::wsmatch matches;
  try
  {
    if (boost::regex_match(sPath, matches, shareFinder))
      return matches[2];
  }
  catch (const exception&)
  {
  }
  return L"";
}

inline String cDFSDatabase::pathRoot(const String& sPath) const
{
  static const boost::wregex shareFinder(m_shareFinder + L"(\\\\.*)?");
  boost::wsmatch matches;
  try
  {
    if (boost::regex_match(sPath, matches, shareFinder))
      return matches[1];
  }
  catch (const exception&)
  {
  }
  return L"";
}

inline cPtr<cDFSDatabase> cDFSDatabase::getDatabase(const String& sPath) const
{
  cLockGuard lg(&m_childAccess);
  const tDatabaseMap::const_iterator finder = m_childDatabase.find(sPath);
  if (m_childDatabase.end() != finder && finder->second.isValid())
    return finder->second;

  return cPtr<cDFSDatabase>();
}

inline cPtr<cDFSDatabase> cDFSDatabase::getOrCreateDatabase(const String& sPath)
{
  cLockGuard lg(&m_childAccess);
  const tDatabaseMap::const_iterator finder = m_childDatabase.find(sPath);
  if (m_childDatabase.end() != finder && finder->second.isValid())
    return finder->second;

  const cPtr<cDFSDatabase> pDFSDatabase = new cDFSDatabase();
  m_childDatabase[sPath] = pDFSDatabase;

  return pDFSDatabase;
}

inline bool cDFSDatabase::empty() const
{
  cLockGuard lgChild(&m_childAccess);
  cLockGuard lgNames(&m_namesAccess);
  return m_childDatabase.empty() && m_nameMap.empty();
}