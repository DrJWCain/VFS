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

#include <QKernelExecDDK.h>
#include "cLogThreadData.h"

using namespace vfs;

class cLogOutputThread : public cBasicThread
{
  friend class cLogDriverEnumerator;

  public :
    QDEFINE_SMARTPTR(cLogOutputThread);

    cLogOutputThread();
    ~cLogOutputThread();

    bool /* has a driver? */ startDrivers();
    void stopDrivers();
    void QAPI signalLoopStop();
    void QAPI runLoop();
    void pushRecord(iLogDriver::cMsg::ConstPtr NewRecord);
    void flushBuffer();
    void lockBuffer();
    void releaseBuffer();

  private :

    typedef std::list<iLogDriver::Ptr> DriversList;
    DriversList             m_Drivers;

    bool                    m_IsRunning;

    typedef std::vector<iLogDriver::cMsg::ConstPtr> RecordBuffer;
    RecordBuffer            m_Buffers[2];
    RecordBuffer *          m_FrontBuffer;
    RecordBuffer *          m_BackBuffer;
    cLock                   m_BufferLock;
    cAutoResetEvent         m_NewRecord;
    cManualResetEvent       m_BufferEmpty;

    cLock                   m_WriteLock;

    bool                    m_IsBuffered;
};
