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

// cNetManServiceStart.h
//

#pragma once
#include "Resource.h"

namespace vfs
{
  class iThread;
};

class cNetManServiceStart : public vfs::iGuardCreator, public vfs::cRefCount
{
public:
  vfs::cGuard::ConstPtr start (const vfs::String& cmdLine) throw (vfs::cRecoverable);

private:
  vfs::cGuard::ConstPtr mMyWorkerThreadGuard;
  vfs::cPtr<vfs::iBasicThread> mQThread;

  // iGuardCreator
  void QAPI onGuardDestroyed (vfs::iGuarded::Ptr guarded);
};
