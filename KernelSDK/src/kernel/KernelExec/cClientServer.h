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

// --- Headers ---------------------------------------------------------------------------

#include <String>
#include <QKernelBase.h>

// --- Use Namespace ---------------------------------------------------------------------

using namespace std;
using namespace vfs;




// =======================================================================================
// === cClientServer =====================================================================
// =======================================================================================

class cClientServer
{
  protected :

    cClientServer(const String & Name) throw(String);

    char *        getBuffer() const;
    unsigned int  getBufferSize() const;

    cLock                       m_ThreadSafety;
    cAutoResetEvent             m_SignalServerSent;
    cAutoResetEvent             m_SignalClientReceived;
    cWin32HandleGuard::Ptr      m_SharedFile;
    cWin32MappedFileGuard::Ptr  m_SharedBuffer;
};




// =======================================================================================
// === cClient ===========================================================================
// =======================================================================================

class cClient : public ::cClientServer
{
  public :

    cClient(const String & Name) throw(String);

    // Wait indefinitely for next message then return.
    String        getStringWaitForever();

    // Wait until either message was received or time out period is reached. Returns false
    // if timed out and message was not received.
    bool          getStringWait(const unsigned long TimeoutMillisecs, String * const Result);

    // Force getStringWaitForever to abandon
    void          spike();
};




// =======================================================================================
// === cServer ===========================================================================
// =======================================================================================

class cServer : public ::cClientServer
{
  public :

    cServer(const String & Name) throw(String);

    // Wait until either message was sent and received or time out period is reached. Returns false
    // if timed out and message was not sent.
    bool          sendStringWait(const String & StringToSend, const unsigned long TimeoutMillisecs);
    void          sendStringWaitForever(const String & StringToSend);

    // Force getStringWaitForever to abandon
    void          spike();
};