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

//cIOCompletionPort.cpp

#include "StdAfx.h"

#include "ciocompletionport.h"
#include "ciocompletionthread.h"

#include "..\..\..\KernelSDK\include\QWinBase.h"

cIOCompletionPort::cIOCompletionPort(const unsigned int threads)
{
  // create IO completion port
  m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
  if(!m_hCompletionPort)
  {
      QSOS((L"Cqp Server: CreateIoCompletionPort() failed with error %d", GetLastError()));
  }

  // creat worker threads
  for (unsigned int i=0; i<threads; i++)
  {
    vfs::cPtr<cIOCompletionThread> pIOCompletionThread = new cIOCompletionThread(m_hCompletionPort, i);
    m_IOCompletionThreads.push_back(pIOCompletionThread);
    pIOCompletionThread->startLoop();
    m_threadIDs.push_back(pIOCompletionThread->getThreadID());
  }
}

cIOCompletionPort::~cIOCompletionPort(void)
{
  //PRINT(L"cIOCompletionPort::~cIOCompletionPort(void)\n");
  // need to kill worker threads before releasing handle to IOCompletionPort
  for(std::list<cPtr<cIOCompletionThread> >::iterator it = m_IOCompletionThreads.begin(); it != m_IOCompletionThreads.end(); ++it)
  {
    (*it)->signalLoopStop();
  }
  m_IOCompletionThreads.clear();
  CloseHandle(m_hCompletionPort);
}

HANDLE *cIOCompletionPort::AddFileHandle(const HANDLE hFile)
{
  // return completion key as a pointer to variable holding the handle value
  HANDLE *pHandle = new HANDLE;
  *pHandle = hFile;

  if (CreateIoCompletionPort(hFile
    , m_hCompletionPort
    , reinterpret_cast<ULONG_PTR>(pHandle)
    , 0) == NULL)
  {
    DWORD dwErr = GetLastError();
    QSOS((L"Failed to add Handle to completion ports with error %d - %s"
      , dwErr
      , win32ErrorToString(dwErr).c_str()));

    delete(pHandle);
    pHandle = NULL;
  }

  return pHandle;
}

bool cIOCompletionPort::AddCompletionKey(ULONG_PTR pCompletionKey)
{
  bool bRet(false);
  if (CreateIoCompletionPort(*reinterpret_cast<HANDLE *>(pCompletionKey)
    , m_hCompletionPort
    , pCompletionKey
    , 0) == NULL)
  {
    DWORD dwErr = GetLastError();
    QSOS((L"Failed to add Completion Key to completion ports with error %d - %s"
      , dwErr
      , win32ErrorToString(dwErr).c_str()));
  }
  else
    bRet = true;

  return bRet;
}

bool cIOCompletionPort::PostMessage(cPtr<iCompletionMsg> pMsg)
{
  //PRINT(L"cIOCompletionPort::PostMessage()\n");
  bool bRet(false);
  IO_DATA *pData = new IO_DATA;
  pData->overlapped.Internal = 0;
  pData->overlapped.InternalHigh = 0;
  pData->overlapped.Offset = 0;
  pData->overlapped.OffsetHigh = 0;
  pData->overlapped.hEvent = 0;
  pData->pMsg = pMsg;
  if (PostQueuedCompletionStatus(m_hCompletionPort
    , sizeof(pMsg)
    , reinterpret_cast<ULONG_PTR>(this)
    , reinterpret_cast<LPOVERLAPPED>(pData)) == FALSE)
  {
    DWORD dwErr = GetLastError();
    QSOS((L"cIOCompletionPort::PostMessage() - failed with error %d - %s"
      , dwErr
      , win32ErrorToString(dwErr).c_str()));
    delete(pData);
  }
  else
    bRet = true;

  return bRet;
}
