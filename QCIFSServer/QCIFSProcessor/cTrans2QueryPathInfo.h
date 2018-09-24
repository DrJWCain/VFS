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

// cTrans2QueryPathInfo.h

#pragma once

#include "mycifs.h"
#include "cTrans2Response.h"
#include "cSizeOnDisk.h"
#include "cPathInfoErr.h"

class cSMBResponse;
class iTreeResource;

class cTrans2QueryPathInfo : cTrans2Response, cSizeOnDisk, cPathInfoErr
{
public:
  cTrans2QueryPathInfo(const vfs::cPtr<iTreeResource> pTreeResource
    , const PREQ_QUERY_PATH_INFORMATION pReq
    , const vfs::cPtr<cSMBResponse> response);
  ~cTrans2QueryPathInfo(void);

private:
  DWORD processUnhandled(const std::wstring &rsPath, PUCHAR& pBuffer) const;

  DWORD Basic(const std::wstring &rsPath, PUCHAR& pBuffer) const;
  DWORD Standard(const std::wstring &rsPath, PUCHAR &pBuffer) const;
  DWORD EA(const std::wstring &rsPath, PUCHAR &pBuffer) const;
  DWORD Name(const std::wstring &rsPath, PUCHAR &pBuffer) const;
  DWORD AttributeTag(const std::wstring &rsPath, PUCHAR &pBuffer) const;
  DWORD Stream(const std::wstring &rsPath, PUCHAR &pBuffer) const;
  DWORD All(const std::wstring &rsPath, PUCHAR &pBuffer) const;
  DWORD Compression(const std::wstring &rsPath, PUCHAR &pBuffer) const;
  DWORD UnixBasic(const std::wstring &rsPath, PUCHAR &pBuffer) const;

private:
  const vfs::cPtr<iTreeResource> m_pTreeResource;
};