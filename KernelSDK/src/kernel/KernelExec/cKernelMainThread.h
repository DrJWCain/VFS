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
/*
**  
*/

#include "QKernelExec.h"

using namespace vfs;
using namespace std;




// =======================================================================================
// === cKernelMainThread =================================================================
// =======================================================================================

namespace vfs
{

class cKernelMainThread : public iKernelMainThread, public cBasicThread, public iGuardCreator
{
  public :

    QDEFINE_SMARTPTR(cKernelMainThread);

    // iThread
    void QAPI signalLoopStop() {} // Can't abort once started

    // cKernelMainThread
    cKernelMainThread();
    cGuard::ConstPtr start();
    void waitDevicesReady();
    void waitBootedForever();
    void waitBooted(const unsigned int timeoutMs);

  private :

    cManualResetEvent m_SignalDevicesReady;
    cManualResetEvent m_SignalBooted;

    cGuard::ConstPtr m_ThreadExeGuard;

    // iThread
    void QAPI runLoop();

    // iGuardCreator
    void QAPI onGuardDestroyed(iGuarded::Ptr Guarded);
};

}