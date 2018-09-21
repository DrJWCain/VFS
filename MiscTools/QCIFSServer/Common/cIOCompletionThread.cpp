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

//cIOCompletionThread.cpp

#include "StdAfx.h"

#include "ciocompletionthread.h"
#include "ciocompletionport.h"
#include "iCompletionMsg.h"

//volatile static LONG gIOQueuedCount = 0;

cIOCompletionThread::cIOCompletionThread(HANDLE hIOCompletionPort, int threadID)
: cThread(L"IOCP", eNormal)
, m_hIOCompletionPort(hIOCompletionPort)
, m_threadID(threadID)
, m_bRun(true)
, m_kernelThreadID(0)
{
}

cIOCompletionThread::~cIOCompletionThread(void)
{
  //PRINT(L"cIOCompletionPort::~cIOCompletionThread(void)\n");
  m_bRun = false;
}

DWORD cIOCompletionThread::getThreadID()
{
  threadIDSet.wait();
  return m_kernelThreadID;
}

void cIOCompletionThread::runLoop()
{
  DWORD dwBytes;
  LPOVERLAPPED pOverlapped(NULL);
  ULONG_PTR pCompletionKey(NULL);
  m_kernelThreadID = GetCurrentThreadId();
  QTRACE((L"cIOCompletionThread::runLoop() - thread ID is %d", m_kernelThreadID));
  threadIDSet.set();
  while (m_bRun)
  {
    //PRINT(L"Waiting thread %d\n", m_threadID);

    //InterlockedIncrement(&gIOQueuedCount);
    //PRINT(L"cIOCompletionThread::runLoop() - queuing %d threads for completion status\n", gIOQueuedCount);

    BOOL bRet = GetQueuedCompletionStatus(m_hIOCompletionPort, &dwBytes, &pCompletionKey, &pOverlapped, INFINITE);
    //InterlockedDecrement(&gIOQueuedCount);
    //PRINT(L"cIOCompletionThread::runLoop() - dequeued completion event %d still waiting\n", gIOQueuedCount);
    //PRINT(L"Running thread %d\n", m_threadID);

    if (pOverlapped == NULL || pCompletionKey == NULL)
    {
      DWORD err = GetLastError();
      if(!m_bRun && 735 == err)
        return;
      QSOS((L"Failed to dequeue IO operation with error %d", err));
    }
    else
    {
      IO_DATA *pIOKey = reinterpret_cast<IO_DATA *>(pOverlapped);

      if (bRet != 0)
      {
        // success
        pIOKey->pMsg->deliver(pCompletionKey, dwBytes);
      }
      else
      {
        pIOKey->pMsg->err(pCompletionKey, GetLastError());
      }

      delete(pIOKey);
    }
  }
}