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

//cConstMemComposite.cpp

#include "StdAfx.h"
#include ".\cconstmemcomposite.h"

#include "writeFileSize.h"
#include "iFileEvent.h"
#include "iRenameHandler.h"
#include "cMemFileRename.h"
#include "cConstNamed.h"

using namespace vfs;

cConstMemComposite::cConstMemComposite(const fileInfo& fi
  , const cPtr<cSearchListener> pSearchListener
  , const cConstPtr<cMemoryView> pMem
  , cPtr<iFileEvent> fileEvent
  , const bool bDeletable
  , const cPtr<iRenameHandler> pRenameHandler)
: cDeletableComposite(fi, pSearchListener, fileEvent, bDeletable)
, m_pMem(pMem)
, m_pRenameHandler(pRenameHandler)
{
}

cConstMemComposite::~cConstMemComposite(void)
{
}

DWORD cConstMemComposite::writeInfo(LPBY_HANDLE_FILE_INFORMATION pFileInfo, ULONGLONG fid)
{
  const DWORD dwRet = cAttributeComposite::writeInfo(pFileInfo, fid);
  if (ERROR_SUCCESS == dwRet)
    writeFileSize<LPBY_HANDLE_FILE_INFORMATION, cConstPtr<cMemoryView> >(pFileInfo, m_pMem);

  return dwRet;
}

DWORD cConstMemComposite::writeInfo(PfileInfo pFileInfo, ULONGLONG fid)
{
  const DWORD dwRet = cAttributeComposite::writeInfo(pFileInfo, fid);
  if (ERROR_SUCCESS == dwRet)
    writeFileSize<WIN32_FIND_DATAW*, cConstPtr<cMemoryView> >(&(pFileInfo->w32Data), m_pMem);

  return dwRet;
}

DWORD cConstMemComposite::Read(tTransmitList &krTPM, DWORD& nBytes, const LARGE_INTEGER &nOffset, const int sessionID, ULONGLONG fid)
{
  if (0 == nBytes)
    return ERROR_SUCCESS;

  cAttributeComposite::access();
  if (m_pMem.isNull() || nOffset.QuadPart >= m_pMem->getSize())
  {
    nBytes = 0;
    return ERROR_ACCESS_DENIED;
  }

  const cConstPtr<cMemoryView> pMemRead = m_pMem->rangeConst((size_t)nOffset.LowPart
    , min((size_t)nBytes, m_pMem->getSize() - nOffset.LowPart));

  if (pMemRead.isValid())
  {
    SMART_TPE tpe;
    tpe.pMem = pMemRead;
    tpe.tpe.dwElFlags = TP_ELEMENT_MEMORY;
    tpe.tpe.pBuffer = (PVOID)pMemRead->getConstBytes();
    tpe.tpe.cLength = pMemRead->getSize();
    nBytes = tpe.tpe.cLength;
    krTPM.push_back(tpe);
    return ERROR_SUCCESS;
  }

  nBytes = 0;
  return ERROR_ARENA_TRASHED;
}

DWORD cConstMemComposite::Rename( const String& sNewName )
{
  return m_pRenameHandler->Rename(
    new cMemFileRename<cConstMemComposite>(cPtr<cConstMemComposite>(this, this)
      , new cConstNamed(Name())
      , m_pParentSearchListener)
    , sNewName);
}