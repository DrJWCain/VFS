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

using namespace std;
using namespace vfs;




// =======================================================================================
// === cMsgQueue =========================================================================
// =======================================================================================

#define MSGQUEUE_USING_SEMAPHORE 0

class cMsgQueue : public iMsgQueue, public cRefCount
{
  public :

    QDEFINE_SMARTPTR(cMsgQueue);

    // iMsgQueue
    virtual void QAPI postMsg(const iMsg::Ptr & newMsg);
//    virtual void QAPI postMsgWait(const iMsg::Ptr & newMsg, const unsigned int timeoutMs);
    virtual unsigned int /* num culled */ QAPI clearMsgs();
    virtual unsigned int QAPI countMsgs() const;
    virtual bool QAPI isMsgWaiting() const;
    virtual void QAPI waitMsgForever() const;
    virtual bool /* signalled? */ QAPI waitMsg(const unsigned int timeoutMs) const;
    virtual bool /* has a msg? */ QAPI getMsg(iMsg::Ptr * const optionalResult /* could be null! */);
    virtual void QAPI postMsgFront(const iMsg::Ptr & newMsg);
    virtual bool /* visited all? see fMsgVisitor_Abort */ visitMsgsOldestFirst(iMsgVisitor & visitor);
    virtual bool /* visited all? see fMsgVisitor_Abort */ visitMsgsNewestFirst(iMsgVisitor & visitor);

#if kKernelSDKIfaceVersion_Int < 0400000
    virtual unsigned int /* num culled */ QAPI clearMessages(const type_info& messageType);
    virtual unsigned int /* num culled */ QAPI clearMessages(const type_info& messageType, const unsigned int leavingMinNumMsgsIntact);
    virtual unsigned int /* num culled */ clearNewestMessagesIfMoreThan(const unsigned int maxNumMsgs);
    virtual unsigned int /* num culled */ clearOldestMessagesIfMoreThan(const unsigned int maxNumMsgs);
#endif

    // cThreadImpl
    cMsgQueue();
    iWaitable::Ptr getNotEmptyWaitable() const { return m_SignalMsgQueueNotEmpty; }

  private :

    cManualResetEvent::Ptr      m_SignalMsgQueueNotEmpty;

#if MSGQUEUE_USING_SEMAPHORE
    cSemaphore                  m_HasSpace;
#endif

    mutable cLock               m_MsgQueueThreadSafety;
    list<iMsgQueue::iMsg::Ptr>  m_MsgQueue;
};
