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

//cFileIOCompletionStatus.cpp

#include "StdAfx.h"

#include ".\cFileIOCompletionStatus.h"

#include "..\Common\iCommonFactory.h"
#include "..\Common\primatives.h"

cFileIOCompletionStatus::cFileIOCompletionStatus(void)
: m_statusSet(iCommonFactory::singleton().createEvent())
, m_dwBytes(0)
{
}

cFileIOCompletionStatus::~cFileIOCompletionStatus(void)
{
}

void cFileIOCompletionStatus::deliver(ULONG_PTR pCompletionKey, const DWORD dwBytes)
{
  m_dwStatus = ERROR_SUCCESS;
  m_dwBytes = dwBytes;
  m_statusSet->set();
}

void cFileIOCompletionStatus::err(ULONG_PTR pCompletionKey, const DWORD errCode)
{
  m_dwStatus = errCode;
  m_statusSet->set();
}

DWORD cFileIOCompletionStatus::getStatus()
{
  m_statusSet->wait();
  return m_dwStatus;
};

DWORD cFileIOCompletionStatus::getBytes()
{
  return m_dwBytes;
}