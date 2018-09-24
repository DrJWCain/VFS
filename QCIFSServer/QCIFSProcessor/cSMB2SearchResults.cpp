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

//cSMB2SearchResults.cpp

#include "StdAfx.h"

#include ".\csmb2searchresults.h"
#include "align.h"
#include "SMB2FileInfoLevel.h"
#include "cSMB2Request.h"

#include "..\QCIFSFwk\iWriteInfo.h"
#include "..\iTreeResource.h"
#include "..\QCIFSFwk\iChildLoaderVisitor.h"

using namespace vfs;

cSMB2SearchResults::cSMB2SearchResults(const cPtr<iComposite> pComposite
  , const String& sSearchStr
  , bool singleEntry
  , const vfs::String& path
  , const vfs::cPtr<iTreeResource>& treeResource)
{
  fileInfo fi;
  String sSearch = sSearchStr;
  memset(&fi, 0, sizeof(fi));
  fi.w32Data.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
  if (sSearchStr == L"<.dll") // IIS 8 Search pattern
  {
    sSearch = L"*";
  }
  if (pComposite.isValid())
  {
    if(singleEntry && sSearch != L"*")
    {
      pComposite->find(sSearch, m_fileInfoList);
      if(m_fileInfoList.empty())
      {
        //maybe lazy create required?
        QTRACE((L"Lazy create? %s %s", path.c_str(), sSearch.c_str()));
        tFileInfoList fileInfoList;
        //This call will fail to find the 'sSearchStr' an then call lazyDirectoryCheck to try to make the file lazily ...
        treeResource->Search(path + L"\\" + sSearchStr, fileInfoList);
      }
    }
    else
    {
      pComposite->getChildren(sSearch, m_fileInfoList);
    }
    pComposite->writeInfo(&fi, 0);
  }
  
  if(L"*" != sSearch && m_fileInfoList.size())
  {
    wcscpy(fi.w32Data.cFileName, L".");
    m_fileInfoList.push_back(fi);
    wcscpy(fi.w32Data.cFileName, L"..");
    m_fileInfoList.push_back(fi);
  }

  m_searchIt = m_fileInfoList.begin();
}

cSMB2SearchResults::~cSMB2SearchResults(void)
{
}

DWORD cSMB2SearchResults::getSearchResults( const PREQ_SMB2_QUERY_DIRECTORY pReq
  , const PUCHAR pRespHeader
  , const PUCHAR pRespStart
  , ULONG& respLength) const
{
  //QTRACE((L"cSMB2SearchResults::getSearchResults %p", this));

  initialiseFileInfoHandlers();
  if (SMB2_RESTART_SCANS == (pReq->Flags & SMB2_RESTART_SCANS))
  {
    //QTRACE((L"cSMB2SearchResults::getSearchResults SMB2_RESTART_SCANS"));
    m_searchIt = m_fileInfoList.begin();
  }

  if (m_fileInfoList.begin() == m_fileInfoList.end())
  {
    return static_cast<DWORD>(STATUS_NO_SUCH_FILE);
  }

  if (m_fileInfoList.end() == m_searchIt)
  {
    //QTRACE((L"cSMB2SearchResults::getSearchResults STATUS_NO_MORE_FILES"));
    return static_cast<DWORD>(STATUS_NO_MORE_FILES);
  }

  const tSMB2FileInfoFuncPtr pFn(SMB2FileInfoDispatch[pReq->FileInformationClass]);
  if (SMB2_RETURN_SINGLE_ENTRY == (pReq->Flags & SMB2_RETURN_SINGLE_ENTRY))
  {
    //QTRACE((L"cSMB2SearchResults::getSearchResults SMB2_RETURN_SINGLE_ENTRY"));
    respLength = (*pFn)(pRespStart, *m_searchIt);
    ++m_searchIt; //RFHRFH - don't know if we're supposed to increment the search or not
    return ERROR_SUCCESS;
  }

  int allowedSpace = gkSMB2BufferSize - ((pRespStart-pRespHeader) + sizeof(SMB2_HEADER));
  allowedSpace = min(allowedSpace, pReq->OutputBufferLength);
  if(allowedSpace > 65400)
    allowedSpace = 65400;

  //QTRACE((L"cSMB2SearchResults::getSearchResults m_fileInfoList = %Iu, OutputBufferLength %d, allowedSpace %d", m_fileInfoList.size(), pReq->OutputBufferLength, allowedSpace));

  if(pReq->OutputBufferLength == 128) //There seems to be a chained command case, where MS asks for an extra find in the chain. Its an optimisation to allow us to return STATUS_NO_MORE_FILES for smaller directories.
  {                                   //However, if we get here, there are more files to list, so return OK & MS will aks for more.
    //QTRACE((L"cSMB2SearchResults::getSearchResults 128"));
    return static_cast<DWORD>(STATUS_SUCCESS);
  }

  ULONG prevRespLength(0);

  for (
    ; (m_fileInfoList.end() != m_searchIt)
    ; ++m_searchIt)
  {
    if((respLength + wcslen(m_searchIt->w32Data.cFileName)*sizeof(WCHAR) + sizeof(FileIdBothDirectoryInformation))>= allowedSpace)
      break;

    respLength += getAlignment<ULONGLONG>(pRespStart + respLength, pRespHeader);
    const PUCHAR pRespBuf(pRespStart + respLength);
    const PULONG pPrevRespBuf(reinterpret_cast<PULONG>(pRespStart + prevRespLength));
    *pPrevRespBuf = respLength - prevRespLength;
    prevRespLength = respLength;

    respLength += (*pFn)(pRespBuf, *m_searchIt);

    //QTRACE((L"%s. %d", m_searchIt->w32Data.cFileName, respLength));
  }

  //QTRACE((L"cSMB2SearchResults::getSearchResults() - %d (%d)", respLength, allowedSpace));

  return STATUS_SUCCESS;
}