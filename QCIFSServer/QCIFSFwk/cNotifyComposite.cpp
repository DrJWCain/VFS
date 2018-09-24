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

//cNotifyComposite.cpp
// partial implementation of iComposite interface to handle notifications

#include "StdAfx.h"

#include ".\cnotifycomposite.h"

#include "iChildLoaderVisitor.h"
#include "iNotifyTree.h"
#include "cNotificationHandler.h"
#include "iNamed.h"

using namespace vfs;

cNotifyComposite::cNotifyComposite(const fileInfo& fi
  , const cPtr<cSearchListener> pParentSearchListener
  , const cPtr<iNotifyTree> pNotifyTree
  , const cPtr<cNotificationHandler> pNotificationHandler
  , const cPtr<iFileEvent> fileEvent)
: cDeletableComposite(fi, pParentSearchListener, fileEvent, pParentSearchListener.isValid())
, m_pNotifyTree(pNotifyTree)
, m_pNotificationHandler(pNotificationHandler)
, RequestID(0)
{
}

cNotifyComposite::~cNotifyComposite(void)
{
}

DWORD cNotifyComposite::registerNotification(const ULONGLONG requestID
  , cPtr<iNotificationReceiver> pNotification)
{
  RequestID = requestID;
  //QTRACE((L"cNotifyComposite::registerNotification %d", RequestID));

  const DWORD dwRet = m_pNotificationHandler.isValid()
    ? m_pNotificationHandler->registerNotification(requestID, pNotification)
    : ERROR_NOT_SUPPORTED;

  if (ERROR_SUCCESS == dwRet && FALSE != pNotification->WatchSubtree() && m_pNotifyTree.isValid())
    m_pNotifyTree->addSubtreeWatch(pNotification);

  return dwRet;
}

void cNotifyComposite::triggerNotifications()
{
  QTRACE((L"cNotifyComposite::triggerNotifications %d", RequestID));
  if (m_pNotificationHandler.isValid())
    m_pNotificationHandler->Notify();
}

DWORD cNotifyComposite::cancelNotification(const ULONGLONG requestID, bool synchronous)
{
  if(m_pNotificationHandler.isValid())
  {
    return m_pNotificationHandler->cancelNotification(requestID, synchronous);
  }
  return ERROR_SUCCESS;
}