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

//cNotificationThread.cpp

#include "StdAfx.h"

#include ".\cnotificationthread.h"
#include "iNotificationReceiver.h"
#include "cWindowsComposite.h"

using namespace vfs;

cNotificationThread::cNotificationThread(const HANDLE hChange
  , cPtr<iNotificationReceiver> pNotification
  , cWindowsComposite* pWindowsComposite)
: cThread(L"Notification Thread", eNormal)
, m_hChange(hChange)
, m_pNotification(pNotification)
, m_pWindowsComposite(pWindowsComposite)
, m_hCancelled(CreateEvent(NULL, TRUE, FALSE, NULL))
, m_bCancelled(false)
{
}

cNotificationThread::~cNotificationThread(void)
{
  m_bCancelled = true;
  SetEvent(m_hCancelled);
  CloseHandle(m_hCancelled);
}

void cNotificationThread::runLoop(void)
{
  HANDLE handles[2] = {m_hChange, m_hCancelled};
  WaitForMultipleObjects(sizeof(handles)/sizeof(handles[0]), handles, FALSE, INFINITE);

  if (m_pNotification.isValid())
    m_pNotification->Notify();

  if (!m_bCancelled)
    m_pWindowsComposite->forgetThread(cPtr<cNotificationThread>(this, this));
}