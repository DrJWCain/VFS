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

// cNTTransIOCTL.h

#pragma once

#if CIFS 

#include "mycifs.h"
#include "cTrans2Response.h"
#include "..\iTreeResource.h"


class cNTTransIOCTL : public cTrans2Response
{
public:
  cNTTransIOCTL(const vfs::cPtr<iTreeResource> pTreeResource
    , const PREQ_NT_IO_CONTROL pSetupData
    , vfs::cPtr<cSMBResponse> response);
  ~cNTTransIOCTL(void);

private:
  vfs::cPtr<cSMBResponse> processUnhandled(vfs::cPtr<cSMBResponse> response, const ULONG functionCode);
  vfs::cPtr<cSMBResponse> processCreateOrGetObjectID(const USHORT Fid
    , vfs::cPtr<cSMBResponse> response);
  vfs::cPtr<cSMBResponse> processGetReparsePoint(const USHORT Fid
    , vfs::cPtr<cSMBResponse> response);

private:
  const vfs::cPtr<iTreeResource> m_pTreeResource;
};

#endif // CIFS 