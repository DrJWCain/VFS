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

//cSetFileInfo.h

#pragma once

#include "mycifs.h"
#include "..\iTreeResource.h"
#include "cTrans2Response.h"

class cSetFileInfo : public cTrans2Response
{
//private:
//  typedef DWORD (cSetFileInfo::* tProcessSetFileInfo)(const PUCHAR pData
//    , PUCHAR &pBuffer) const;
//  static tProcessSetFileInfo SetInfoDispatch[USHRT_MAX];

public:
  cSetFileInfo(const vfs::cPtr<iComposite> pComposite
    , PREQ_SET_FILE_INFORMATION pParams
    , const PUCHAR trans2Data
    , const vfs::cPtr<cSMBResponse> response);
  ~cSetFileInfo(void);

private:
  // command handlers
  DWORD processUnhandled(const PUCHAR pData, PUCHAR &pBuffer) const;
  DWORD Allocation(const PUCHAR pData, PUCHAR &pBuffer) const;
  DWORD EndOfFile(const PUCHAR pData, PUCHAR &pBuffer) const;
  DWORD Disposition(const PUCHAR pData, PUCHAR &pBuffer) const;
  DWORD Basic(const PUCHAR pData, PUCHAR &pBuffer) const;

  const vfs::cPtr<iComposite> m_pComposite;
};
