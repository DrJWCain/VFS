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

//cTrans2Find.h

#pragma once

#if CIFS 

#include "mycifs.h"

#include "cSizeOnDisk.h"

#include "..\iTreeResource.h"

class cSMBResponse;
class cSMBSession;

class cTrans2Find : cSizeOnDisk
{
public:
  cTrans2Find(const vfs::cPtr<iTreeResource> pTreeResource
    , const int maxData
    , const PREQ_FIND_FIRST2 pReq
    , const vfs::cPtr<cSMBResponse> pResponse);
  cTrans2Find(const vfs::cPtr<iTreeResource> pTreeResource, cTrans2Find *pOther);
  ~cTrans2Find(void);

  bool EndOfSearch() const;
protected:
  vfs::cPtr<cSMBResponse> doFind(const USHORT level, PUCHAR &pData, const vfs::cPtr<cSMBResponse> pResponse) const;

private:
  vfs::cPtr<cSMBResponse> processUnhandled(PUCHAR &pData, const vfs::cPtr<cSMBResponse> pResponse) const;
  vfs::cPtr<cSMBResponse> FileBothDirectory(PUCHAR &pData, const vfs::cPtr<cSMBResponse> pResponse) const;
  vfs::cPtr<cSMBResponse> FileFullDirectory(PUCHAR &pData, const vfs::cPtr<cSMBResponse> pResponse) const;
  vfs::cPtr<cSMBResponse> FileDirectory(PUCHAR &pData, const vfs::cPtr<cSMBResponse> pResponse) const;
  vfs::cPtr<cSMBResponse> FileIDFullDirectory(PUCHAR &pData, const vfs::cPtr<cSMBResponse> pResponse) const;
  vfs::cPtr<cSMBResponse> FileIDBothDirectory(PUCHAR &pData, const vfs::cPtr<cSMBResponse> pResponse) const;
  vfs::cPtr<cSMBResponse> FileUnix(PUCHAR &pData, const vfs::cPtr<cSMBResponse> pResponse) const;

protected:
  mutable unsigned int *m_pFileIndex;
  tFileInfoList *m_pFileInfoList;
  mutable tFileInfoList::const_iterator *m_pSearchPos;

private:
  const unsigned int m_maxData;
  const unsigned int m_maxSearch;
};

#endif // CIFS 