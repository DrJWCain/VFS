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

//cTrans2GetDFSReferral.h

#pragma once

#include "mycifs.h"
#include "cTrans2Response.h"

class cSMBResponse;
class iTreeResource;

class cTrans2GetDFSReferral : public cTrans2Response
{
public:
  cTrans2GetDFSReferral(const vfs::cPtr<iTreeResource> pTreeResource
    , const PREQ_GET_DFS_REFERRAL pReq
    , const vfs::cPtr<cSMBResponse> response);
  ~cTrans2GetDFSReferral();

private:
  PUCHAR v1Referral(const PUCHAR pReferalSpace, const vfs::String& ksFileName);
  PUCHAR v2Referral(const PUCHAR pReferalSpace, const vfs::String& ksFileName);
  PUCHAR v3Referral(const PUCHAR pReferalSpace, const vfs::String& ksFileName);
};