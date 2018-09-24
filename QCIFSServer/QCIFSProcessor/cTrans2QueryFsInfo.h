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

// cTrans2QueryFSInfo.h
// handle Transaction2 query FS Info command

#pragma once

#include "mycifs.h"
#include "cTrans2Response.h"

class iTreeResource;
class cSMBResponse;

class cTrans2QueryFsInfo : cTrans2Response
{
public:
  cTrans2QueryFsInfo(const vfs::cPtr<iTreeResource> pTreeResource
    , const PREQ_QUERY_FS_INFORMATION pReq
    , vfs::cPtr<cSMBResponse> response);
  ~cTrans2QueryFsInfo(void);

//private:
//  typedef vfs::cPtr<cSMBResponse> (cTrans2QueryFsInfo::* tProcessQueryFSInfo)(cSMBResponse &data) const;
//  static tProcessQueryFSInfo QueryFSInfoDispatch[USHRT_MAX];

private:
  vfs::cPtr<cSMBResponse> processUnhandled(vfs::cPtr<cSMBResponse> response) const;
  vfs::cPtr<cSMBResponse> Allocation(vfs::cPtr<cSMBResponse> response) const;
  vfs::cPtr<cSMBResponse> VolumeInfo(vfs::cPtr<cSMBResponse> response) const;
  vfs::cPtr<cSMBResponse> SizeInfo(vfs::cPtr<cSMBResponse> response) const;
  vfs::cPtr<cSMBResponse> AttributeInfo(vfs::cPtr<cSMBResponse> response) const;
  vfs::cPtr<cSMBResponse> DeviceInfo(vfs::cPtr<cSMBResponse> response) const;
  vfs::cPtr<cSMBResponse> UnixInfo(vfs::cPtr<cSMBResponse> response) const;

private:
  const vfs::cPtr<iTreeResource> m_pTreeResource;
};
