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

/*
** 
**/

#include "stdafx.h"
#include "cLogOutputThread.h"
#include "cLogManager.h"
#include "cSystemInfo.h"

using namespace vfs;




// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

cLogOutputThread::cLogOutputThread() :
  cBasicThread(L"Log", iBasicThread::ePriority_BelowNormal),
  m_IsRunning(false),
  m_NewRecord(false /* initially set? */),
  m_BufferEmpty(true /* initially set? */),
  m_IsBuffered(true)
{
  m_FrontBuffer = &m_Buffers[0];
  m_BackBuffer  = &m_Buffers[1];

  iRegistryHelp::Ptr rh = createRegistryHelp();
  m_IsBuffered = rh->getItemNumber(cSystemInfo::singleton().getKernelExecRegistryPath() + L"Custom Settings\\Log\\", L"Is msg log buffered") != 0;
}

// ---------------------------------------------------------------------------------------

cLogOutputThread::~cLogOutputThread()
{
  stopDrivers();
}




// =======================================================================================
// === lockBuffer/releaseBuffer ==========================================================
// =======================================================================================

void cLogOutputThread::lockBuffer(void)
{
  m_BufferLock.lock();
  m_WriteLock.lock();
}

// ---------------------------------------------------------------------------------------

void cLogOutputThread::releaseBuffer(void)
{
  m_WriteLock.unlock();
  m_BufferLock.unlock();
}




// =======================================================================================
// === signalLoopStop ====================================================================
// =======================================================================================

void cLogOutputThread::signalLoopStop()
{
  m_IsRunning = false;
  m_NewRecord.set();
  // Do nothing.
}




// =======================================================================================
// === pushRecord ========================================================================
// =======================================================================================

void cLogOutputThread::pushRecord(iLogDriver::cMsg::ConstPtr NewRecord)
{
  {
    cLockGuard LockGuard(&m_BufferLock);
    m_FrontBuffer->push_back(NewRecord);
    m_NewRecord.set();
    m_BufferEmpty.reset();
  }
  if (!m_IsBuffered) flushBuffer();
}



// =======================================================================================
// === flushBuffer =======================================================================
// =======================================================================================

void cLogOutputThread::flushBuffer(void)
{
  if (m_IsRunning)
  {
    bool IsBufferOccupied = false;
    {
      cLockGuard LockGuard(&m_BufferLock);
      IsBufferOccupied = m_FrontBuffer->size() > 0;
    }

    if (IsBufferOccupied)
    {
      if (m_BufferEmpty.wait(5000) == cManualResetEvent::eTimedOut)
      {
        OutputDebugString(L"Kernel : Timed out waiting for message log to flush!\n");
      }
    }
  }
}




// =======================================================================================
// === runLoop ===========================================================================
// =======================================================================================

void cLogOutputThread::runLoop()
{
  m_IsRunning = true;

  while (m_IsRunning)
  {
    m_NewRecord.waitForever();

    {
      cLockGuard LockGuard(&m_BufferLock);
      Swap<RecordBuffer *>(&m_FrontBuffer, &m_BackBuffer);
    }

    {
      // Protect output from being interrupt by new output.
      cLockGuard MemberGuard(&m_WriteLock);

      for (RecordBuffer::iterator i = m_BackBuffer->begin(); i != m_BackBuffer->end(); ++i)
      {
        for (DriversList::iterator j = m_Drivers.begin(); j != m_Drivers.end(); ++j)
        {
//          try
          {
            switch ((*i)->m_Type)
            {
              case iLogDriver::cMsg::eErrorMsg :
              case iLogDriver::cMsg::ePlainMsg :        { (*j)->writeMsg(*i); break; }
              case iLogDriver::cMsg::eCollapsedIndent : { (*j)->writeCollapsedIndent(*i); break; }
              case iLogDriver::cMsg::eExpandedIndent :  { (*j)->writeExpandedIndent(*i); break; }
              case iLogDriver::cMsg::eLazyIndent :      { (*j)->writeLazyIndent(*i); break; }
              case iLogDriver::cMsg::eOutdent :         { (*j)->writeOutdent(*i); break; }
              case iLogDriver::cMsg::eTrace :           { (*j)->writeTrace(*i); break; }
              case iLogDriver::cMsg::eBreak :           { (*j)->writeBreak(*i); break; }
              default :                                 { QSOS((L"LogOutputThread : Bad record type %d in log buffer", (long)(*i)->m_Type)); break; }
            }
          }
/*
          catch (...)
          {
            // A log driver crashed! Remove it from list and never try it again...
            j = m_Drivers.erase(j);
            QSOS((L"Log driver \"%s\" crashed during output! - ignoring driver from now on", (*j)->getClassInfo()->getName().c_str()));
          }
*/
        }
      }
    }

    m_BackBuffer->clear();
    m_BufferEmpty.set();
  }
}




// =======================================================================================
// === startDrivers/stopDrivers ==========================================================
// =======================================================================================

bool /* has a driver? */ cLogOutputThread::startDrivers()
{
  std::list<iFactory::iConstructor::Ptr> Constructors = gFactory->getConstructors(iLogDriver::class_info().getGuid());
  for (std::list<iFactory::iConstructor::Ptr>::iterator i = Constructors.begin(); i != Constructors.end(); ++i)
  {
    QMSG((0, L"Starting driver \"%s\"", (*i)->getClassInfo()->getName().c_str()));

    try
    {
      m_Drivers.push_back(ptr_cast<iLogDriver::Ptr>(gFactory->createInstance((*i)->getClassInfo()->getGuid())));
    }
    catch (cNotAvailable & Reason)
    {
      QSOS((L"Failed to start log driver \"%s\" (%s)", (*i)->getClassInfo()->getName().c_str(), Reason.getMsg().c_str()));
    }
  }
  return !m_Drivers.empty();
}

// ---------------------------------------------------------------------------------------

void cLogOutputThread::stopDrivers()
{
  m_Drivers.clear();
}
