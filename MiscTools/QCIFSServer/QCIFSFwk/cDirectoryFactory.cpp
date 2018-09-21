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

//cDirectoryFactory.cpp

#include "StdAfx.h"
#include ".\cdirectoryfactory.h"

#include "iDirLocation.h"
#include "iNameMangler.h"
#include "wildcardPattern.h"

using namespace vfs;
using namespace std;

cDirectoryFactory::cDirectoryFactory(vfs::cPtr<cDirectoryFactory> pDirectoryFactory)
: m_pRoot(pDirectoryFactory->m_pRoot)
, m_pNameMangler(pDirectoryFactory->m_pNameMangler)
{
}

cDirectoryFactory::cDirectoryFactory(cPtr<cSearchLocation> pRoot)
: m_pRoot(pRoot)
{
}

cDirectoryFactory::cDirectoryFactory(vfs::cPtr<cSearchLocation> pRoot
  , vfs::cPtr<iNameMangler> pNameMangler)
: m_pRoot(pRoot)
, m_pNameMangler(pNameMangler)
{
}

cDirectoryFactory::~cDirectoryFactory(void)
{
}

cPtr<iDirLocation> cDirectoryFactory::get(const String& sPath) const
{
  tSrchList searchList;
  parsePath(sPath, searchList);
  return getOnlyDirLocation(searchList);
}

cPtr<iDirLocation> cDirectoryFactory::getParent(const vfs::String& sPath) const
{
  return get(removeEndName(sPath));
}

DWORD cDirectoryFactory::getSearchResults(const vfs::String& sSearchPath
  , tFileInfoList& fileInfoList) const
{
  tSrchList dirList;
  parsePath(sSearchPath, dirList);
  for (tSrchList::const_iterator cit = dirList.begin()
    ; dirList.end() != cit
    ; ++cit)
  {
    const cPtr<cSearchLocation>& pSearchLocation(*cit);
    if (pSearchLocation.isNull())
    {
      if (sSearchPath.find_last_of(L"._") == String::npos)
        QSOS((L"cDirectoryFactory::getSearchResults() - Null pointer: %s", sSearchPath.c_str()));
      return ERROR_ARENA_TRASHED;
    }

    cPtr<iDirLocation> pDirLocation = pSearchLocation->getDirLocation();
    if (pDirLocation.isNull())
    {
      QSOS((L"cDirectoryFactory::getSearchResults() - Null pointer for dir location: %s", sSearchPath.c_str()));
      return ERROR_ARENA_TRASHED;
    }

    fileInfo fi;
    pDirLocation->writeInfo(&fi, 0);
    fileInfoList.push_back(fi);
  }

  return ERROR_SUCCESS;
}

static const boost::wregex PathTokenizer(L"\\\\([^\\\\]+)(.*)$");

void cDirectoryFactory::parsePath(const String& sPath, tSrchList& dirList) const
{
  if (sPath.empty() || L"\\" == sPath)
  {
    makeRootIntoSearchList(dirList);
    return;
  }

  String lowerPath = toLower(sPath); //CIFS is Case PrEsRvInG, case insensitive :-)

  boost::wsmatch matches;
  std::vector<String> replacePatterns;
  replacePatterns.push_back(L" ");
  String sConsumePath(m_pNameMangler.isValid() ? m_pNameMangler->makeMangledName(lowerPath, replacePatterns) : lowerPath);
  //static const boost::char_separator<String::value_type> sep(L"\\");
  //typedef boost::tokenizer<boost::char_separator<String::value_type>, String::const_iterator, String> tokenizer;
  //static const tokenizer tokens(m_pNameMangler.isValid() ? m_pNameMangler->makeMangledName(lowerPath) : lowerPath
  //    , sep);
  bool bFirstIteration(true);

  while (boost::regex_search(sConsumePath, matches, PathTokenizer))
  //for (tokenizer::const_iterator tokIt = tokens.begin()
  //  ; tokens.end() != tokIt
  //  ; ++tokIt)
  {
    if (dirList.empty())
    {
      if (bFirstIteration)
        makeRootIntoSearchList(dirList);
      else
        break; //terminate early
    }

    //try and find an exact match first
    //const String sMatchPattern(*tokIt);
    const String sMatchPattern(matches[1]);
    sConsumePath = matches[2];

    tSrchList childList;
    getMatchingChildren(dirList, sMatchPattern, childList);
    dirList.swap(childList);
    bFirstIteration = false;
  }
}

String cDirectoryFactory::removeEndName(const String& sPath) const
{
  const size_t finder = sPath.find_last_of(L"\\");
  if (String::npos != finder)
    return sPath.substr(0, finder);

  return L"";
}

inline cPtr<iDirLocation> cDirectoryFactory::getOnlyDirLocation(const tSrchList& dirList) const
{
  if (1 != dirList.size())
    return cPtr<iDirLocation>();

  const cPtr<cSearchLocation>& pDirLocation = dirList.front();
  return pDirLocation.isValid() ? pDirLocation->getDirLocation() : cPtr<iDirLocation>();
}

inline void cDirectoryFactory::makeRootIntoSearchList(tSrchList& dirList) const
{
  dirList.push_back(m_pRoot);
}

void cDirectoryFactory::getMatchingChildren(const tSrchList& dirList
  , const String& sMatchPattern
  , tSrchList& childList) const
{
  for (tSrchList::const_iterator cit = dirList.begin()
    ; dirList.end() != cit
    ; ++cit)
  {
    const cPtr<cSearchLocation>& pSearchLocation(*cit);
    if (pSearchLocation.isNull())
    {
      //QSOS((L"cDirectoryFactory::parsePath() - null pointer in search loop"));
      return;
    }

    const cPtr<iNotificationReceiver> pWatchSubtree = pSearchLocation->getSubtreeWatch();

    if (isWildcardSearch(sMatchPattern))
    {
      // try and get a wildcard match
      tSrchList wildcardMatches;
      getWildcardSearchResults(sMatchPattern
        , pWatchSubtree
        , pSearchLocation
        , wildcardMatches);
      childList.insert(childList.end(), wildcardMatches.begin(), wildcardMatches.end());
    }
    else
    {
      const cPtr<cSearchLocation> pFoundLocation = pSearchLocation->getChild(sMatchPattern);
      childList.push_back(pFoundLocation);
      if (pWatchSubtree.isValid())
        pSearchLocation->addSubtreeWatch(pWatchSubtree);
    }
  }
}

inline bool cDirectoryFactory::isWildcardSearch(const String& sMatchPattern) const
{
  return (!(String::npos == sMatchPattern.find(L"*")
    && String::npos == sMatchPattern.find(L"?")
    && String::npos == sMatchPattern.find(L">")
    && String::npos == sMatchPattern.find(L"<")));
}

void cDirectoryFactory::getWildcardSearchResults(const String& sMatchPattern
  , const cPtr<iNotificationReceiver> pWatchSubtree
  , const cPtr<cSearchLocation> pParentSearchLocation
  , tSrchList& matchingChildren) const
{
  cSearchListener::tDirList children;
  pParentSearchLocation->getChildren(children);
  const boost::wregex wildCardPattern(makeWildcardPattern(sMatchPattern));
  for (cSearchListener::tDirList::const_iterator childIt = children.begin()
    ; children.end() != childIt
    ; ++childIt)
  {
    const cPtr<cSearchLocation> pChildSearchLocation(childIt->second);
    if (pChildSearchLocation.isValid())
    {
      const String& sName(childIt->first);
      try
      {
        boost::wsmatch tmpMatches;
        if (boost::regex_match(sName, tmpMatches, wildCardPattern))
          matchingChildren.push_back(pChildSearchLocation);

        if (pWatchSubtree.isValid())
          pChildSearchLocation->addSubtreeWatch(pWatchSubtree);
      }
      catch (const exception&)
      {
        QSOS((L"cDirectoryFactory::getWildcardSearchResults() -  caught exception sareching for %s"
          , sMatchPattern.c_str()));
      }
    }
  }
}