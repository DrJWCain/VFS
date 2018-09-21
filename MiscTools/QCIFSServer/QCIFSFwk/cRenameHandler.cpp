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

//cRenameHandler .cpp

#include "StdAfx.h"

#include ".\crenamehandler.h"
#include "iDirLocation.h"
#include "cTreeResourceFactory.h"
#include "cSearchLocation.h"

using namespace vfs;

cRenameHandler::cRenameHandler(const cPtr<cTreeResourceFactory> pTreeResourceFactory
  , const String& sLoginName)
: m_pTreeResourceFactory(pTreeResourceFactory)
, m_sLoginName(sLoginName)
{
}

cRenameHandler::~cRenameHandler(void)
{
}

DWORD cRenameHandler::Rename( const cPtr<iRename> pSourceRename, const String &newName)
{
  if (m_pTreeResourceFactory.isNull())
    return ERROR_ACCESS_DENIED;

  cPtr<iDirLocation> pNewLocation = m_pTreeResourceFactory->get(newName, m_sLoginName);
  if (pNewLocation.isValid())
    return ERROR_FILE_EXISTS;

  pNewLocation = m_pTreeResourceFactory->getParent(newName, m_sLoginName);
  try
  {
    if (pNewLocation.isValid())
      return pNewLocation->Rename(pSourceRename, newName, m_sLoginName);
  }
  catch (cNotFound& err)
  {
    QSOS((L"cRenameHandler::Rename() Exception caugth:%s", err.getMsg().c_str()));
  }
  return ERROR_ACCESS_DENIED;
}