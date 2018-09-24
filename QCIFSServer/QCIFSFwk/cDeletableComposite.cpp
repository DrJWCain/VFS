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

//cDeletableComposite.cpp

#include "StdAfx.h"
#include ".\cdeletablecomposite.h"

#include "iChildLoaderVisitor.h"

using namespace vfs;

cDeletableComposite::cDeletableComposite(const fileInfo& fi
  , const cPtr<cSearchListener> pParentSearchListener
  , const vfs::cPtr<iFileEvent> pFileEvent
  , const bool bDeletable)
: cAttributeComposite(fi)
, m_pParentSearchListener(pParentSearchListener)
, m_pFileEvent(pFileEvent)
, m_bDeletable(bDeletable)
{
}

cDeletableComposite::~cDeletableComposite(void)
{
}

DWORD cDeletableComposite::Delete()
{
  return Delete(m_bDeletable);
}

DWORD cDeletableComposite::Delete(const bool bDeletable)
{
  if (bDeletable && m_pParentSearchListener.isValid())
  {
    m_pParentSearchListener->remove(cAttributeComposite::Name());
    if (m_pFileEvent.isValid())
      return m_pFileEvent->notifyDelete();
    return ERROR_SUCCESS;
  }
  return ERROR_ACCESS_DENIED;
}