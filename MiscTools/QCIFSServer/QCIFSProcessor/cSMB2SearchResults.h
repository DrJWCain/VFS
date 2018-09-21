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

//cSMB2SearchResults.h

#pragma once

#include "..\iComposite.h"
class iTreeResource;
class cSMB2SearchResults : public vfs::cRefCount
{
public:
  cSMB2SearchResults(const vfs::cPtr<iComposite> pComposite, const vfs::String& sSearchStr, bool singleEntry, const vfs::String& path, const vfs::cPtr<iTreeResource>& treeResource);
  ~cSMB2SearchResults(void);

  DWORD getSearchResults(const PREQ_SMB2_QUERY_DIRECTORY pReq
    , const PUCHAR pRespHeader
    , const PUCHAR pRespStart
    , ULONG& respLength) const;

private:  //private member variables
  tFileInfoList m_fileInfoList;
  mutable tFileInfoList::const_iterator m_searchIt;
};
