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

//cVirtualComposite.cpp

#include "StdAfx.h"
#include ".\cvirtualcomposite.h"

#include "cConstNamed.h"
#include "wildcardPattern.h"
#include "iDirLocation.h"
#include "cSearchLocation.h"
#include "iChildLoader.h"
#include "iRenameHandler.h"

using namespace vfs;

cVirtualComposite::cVirtualComposite(const fileInfo& fi
  , const cPtr<cSearchListener> pSearchListener
  , const cPtr<cSearchListener> pParentSearchListener
  , const cPtr<iChildLoader> pChildLoader
  , const cPtr<iNotifyTree> pNotifyTree
  , const cPtr<cNotificationHandler> pNotificationReceiver
  , const cPtr<iFileEvent> fileEvent
  , const cPtr<iRename> pRename
  , const cPtr<iRenameHandler> pRenameHandler)
: cNotifyComposite(fi, pParentSearchListener, pNotifyTree, pNotificationReceiver, fileEvent)
, m_pSearchListener(pSearchListener)
, m_pChildLoader(pChildLoader)
, m_pRename(pRename)
, m_pRenameHandler(pRenameHandler)
{
}

cVirtualComposite::~cVirtualComposite(void)
{
}

DWORD cVirtualComposite::Read(tTransmitList &krTPM
  , DWORD& nBytes
  , const LARGE_INTEGER &nOffset
  , const int sessionID
  , ULONGLONG fid)
{
  nBytes = 0;
  return ERROR_ACCESS_DENIED;
}

DWORD cVirtualComposite::Write(vfs::cConstPtr<vfs::cMemoryView> buffer
  , DWORD& nBytes
  , const LARGE_INTEGER &offset
  , const int sessionID
  , ULONGLONG fid)
{
  return ERROR_ACCESS_DENIED;
}

DWORD cVirtualComposite::SetFileSize(const PLARGE_INTEGER pFileSize)
{
  return ERROR_ACCESS_DENIED;
}

DWORD cVirtualComposite::Delete()
{
  if (FILE_ATTRIBUTE_READONLY & atts())
    return ERROR_ACCESS_DENIED;

  return cDeletableComposite::Delete();
}

DWORD cVirtualComposite::registerNotification(const ULONGLONG requestID
  , vfs::cPtr<iNotificationReceiver> pNotification)
{
  if (FILE_ATTRIBUTE_DIRECTORY & atts())
    return cNotifyComposite::registerNotification(requestID, pNotification);

  return ERROR_ACCESS_DENIED;
}

bool cVirtualComposite::find(const vfs::String& name, tFileInfoList& searchResults)
{
  if (m_pSearchListener.isNull())
    return false;

  cPtr<cSearchLocation> pSearchLocation = m_pSearchListener->getChild(name, m_pChildLoader);
  if(pSearchLocation.isNull())
    return false;

  const cPtr<iDirLocation> pDirLocation = pSearchLocation->getDirLocation();
  if (pDirLocation.isValid())
  {
    fileInfo fi;
    pDirLocation->writeInfo(&fi, 0);
    searchResults.push_back(fi);
    return true;
  }

  return false;
}
void cVirtualComposite::getChildren(const vfs::String& sSearchMask, tFileInfoList& searchResults )
{
  if (m_pSearchListener.isNull())
    return;

  const cSearchListener::tDirList dirList = m_pSearchListener->getChildren(m_pChildLoader);
  static std::vector<String> replacePatterns;
  if (replacePatterns.empty())
    replacePatterns.push_back(L" ");
  if (String::npos != sSearchMask.find(L"*")) //wildcard search
    searchResults.reserve(dirList.size());
  const boost::wregex wildCardPattern(makeWildcardPattern(m_pSearchListener->makeMangledName(sSearchMask, replacePatterns)));

  for (cSearchListener::tDirList::const_iterator cit = dirList.begin()
    ; dirList.end() != cit
    ; ++cit)
  {
    boost::wsmatch tmpMatches;
    const String& sThisName(cit->first);
    if (boost::regex_match(sThisName, tmpMatches, wildCardPattern))
    {
      const cPtr<cSearchLocation>& pSearchLocation(cit->second);
      if (pSearchLocation.isValid())
      {
        const cPtr<iDirLocation> pDirLocation = pSearchLocation->getDirLocation();
        if (pDirLocation.isValid())
        {
          fileInfo fi;
          pDirLocation->writeInfo(&fi, 0);
          searchResults.push_back(fi);
        }
      }
    }
  }
}

DWORD cVirtualComposite::Rename( const vfs::String& sNewName )
{
  if (m_pRenameHandler.isValid() && m_pRename.isValid())
    return m_pRenameHandler->Rename(m_pRename, sNewName);
  return ERROR_ACCESS_DENIED;
}