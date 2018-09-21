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

//cTrans2QueryFileInfo.h

#pragma once

#include "mycifs.h"
#include "cTrans2Response.h"
#include "cSizeOnDisk.h"
#include "cPathInfoErr.h"

class iTreeResource;
class iComposite;

class cTrans2QueryFileInfo : cTrans2Response, cSizeOnDisk, cPathInfoErr
{
public:
  cTrans2QueryFileInfo(const vfs::cPtr<iTreeResource> pTreeResource
    , const vfs::cPtr<iComposite> pComposite
    , PREQ_QUERY_FILE_INFORMATION pReq
    , vfs::cPtr<cSMBResponse> rResponse);
  ~cTrans2QueryFileInfo(void);

  DWORD processUnhandled(PUCHAR &pBuffer) const;
  DWORD Basic(PUCHAR &pBuffer) const;
  DWORD Standard(PUCHAR &pBuffer) const;
  DWORD EA(PUCHAR &pBuffer) const;
  DWORD Stream(PUCHAR &pBuffer) const;
  DWORD All(PUCHAR &pBuffer) const;
  DWORD Compression(PUCHAR &pBuffer) const;
  DWORD UnixBasic(PUCHAR &pBuffer) const;

private:  //private member variables
  const vfs::cPtr<iComposite> m_pComposite;
};
