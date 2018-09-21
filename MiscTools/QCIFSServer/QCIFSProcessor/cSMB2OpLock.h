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


#pragma once

#include "cSMB2Response.h"

class cSMB2OpLock : public vfs::cRefCount
{
public:
  cSMB2OpLock(const ULONGLONG& fid, const ULONGLONG& durableFileID, cPtr<cSMB2Response> response);
  ~cSMB2OpLock();

  bool released() const;
  void Release();
  void Break();

private:
  const ULONGLONG m_Fid;
  const ULONGLONG m_DurableFid;
  cPtr<cSMB2Response> m_Response;
  bool m_Locked;
};