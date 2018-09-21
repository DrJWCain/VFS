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

// cTrans2Response.h
// wraps generic functionality of forming a transaction 2 response

#pragma once

class cSMBResponse;

class cTrans2Response
{
  friend class cNotificationReceiver;
public:
  cTrans2Response(const vfs::cPtr<cSMBResponse> pResponse);
  virtual ~cTrans2Response(void);

  vfs::cPtr<cSMBResponse> processUnhandled() const;

  void setParams(PUCHAR pParams, USHORT byteCount) const;
  void setData(PUCHAR pData, USHORT byteCount) const;

private:
  const vfs::cPtr<cSMBResponse> m_pResponse;
};
