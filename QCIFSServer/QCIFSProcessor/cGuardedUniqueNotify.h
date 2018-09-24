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

//cGuardedUniqueNotify.h

#pragma once

#include "cNotificationID.h"

class cGuardedUniqueNotify : public vfs::cRefCount
{
public:
  cGuardedUniqueNotify(ULONGLONG ullID
    , const cNotificationID& rNID
    , const vfs::cPtr<vfs::iGuardCreator>& pGuardCreator);
  ~cGuardedUniqueNotify(void);

  //public member functions
  ULONGLONG getUniqueId() const {return m_ullID;}

private:
  const vfs::cPtr<vfs::cGuard> m_NIDGuard;
  const ULONGLONG m_ullID;
};
