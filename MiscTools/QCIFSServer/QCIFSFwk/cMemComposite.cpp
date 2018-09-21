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

//cMemComposite.cpp

#include "stdafx.h"

#include ".\cmemcomposite.h"
#include "iChildLoaderVisitor.h"
#include "writeFileSize.h"
#include "cQCIFSFwkHelper.h"
#include "cNotificationHandler.h"
#include "iRenameHandler.h"
#include "cMemFileRename.h"
#include "cConstNamed.h"

using namespace vfs;

static iActivity::Ptr pActivityGroup;
static volatile LONG reads(0);

void initactivities(iActivityGroup::Ptr activityGroup)
{
  pActivityGroup = activityGroup->addSignal (L"mem composite");
  pActivityGroup->addActivity(kMemCompositeClose, L"close");
}

cMemComposite::cMemComposite(const fileInfo& fi
  , const cPtr<cSearchListener> pSearchListener
  , const cPtr<cNotificationHandler> pNotificationHandler
  , const bool bDeletable
  , const tFrameVector& frameVector
  , const cPtr<iFileLoader> pMemLoader
  , cPtr<iFileEvent> pFileEvent  
  , const cPtr<iRenameHandler> pRenameHandler
  , bool hasBatchOpLock)
: cDeletableComposite(fi, pSearchListener, pFileEvent, bDeletable)
, m_pNotificationHandler(pNotificationHandler)
, m_pMemLoader(pMemLoader)
, m_bWritten(false)
, m_frameVector(frameVector)
, m_uiHeaderBytes(0)
, m_uiBytesRead(0)
, m_pRenameHandler(pRenameHandler)
, m_bBatchOpLock(hasBatchOpLock)
{
  //QTRACE((L"cMemComposite::cMemComposite(%p)", this));
}

cMemComposite::~cMemComposite(void)
{
  //QTRACE((L"cMemComposite::~cMemComposite(%p)", this));
}

DWORD cMemComposite::writeInfo(LPBY_HANDLE_FILE_INFORMATION pFileInfo, ULONGLONG fid)
{
  cAttributeComposite::writeInfo(pFileInfo, fid);
  return writeInfoTemplate<LPBY_HANDLE_FILE_INFORMATION>(pFileInfo);
}

DWORD cMemComposite::writeInfo(PfileInfo pFileInfo, ULONGLONG fid)
{
  cAttributeComposite::writeInfo(pFileInfo, fid);
  return writeInfoTemplate<WIN32_FIND_DATAW*>(&(pFileInfo->w32Data));
}

DWORD cMemComposite::Read(tTransmitList &krTPM, DWORD& nBytes, const LARGE_INTEGER &nOffset, const int sessionID, ULONGLONG fid)
{
  cAttributeComposite::access();
  if (0 == nBytes)
    return ERROR_SUCCESS;

//  QTRACE((L"cMemComposite::Read() - (%p), offset: %I64d", this, nOffset.QuadPart));
  
  m_access.lock();
  if (m_frameVector.empty())
  {
    if (m_pMemLoader.isNull())
    {
      m_access.unlock();
      return ERROR_READ_FAULT;
    }

    /*
    if (nOffset.QuadPart == 0)  //read header only
    {
      const cPtr<cMemoryView> buffer = m_pMemLoader->loadMem(Name(), sessionID, true);
      if (buffer.isNull())
      {
        m_access.unlock();
        return ERROR_READ_FAULT;
      }

      SMART_TPE smart_tpe;
      smart_tpe.tpe.dwElFlags = TP_ELEMENT_MEMORY;
      smart_tpe.tpe.pBuffer = buffer->getBytes();
      smart_tpe.tpe.cLength = min(nBytes, buffer->getSize());
      smart_tpe.pMem = buffer;

      nBytes = smart_tpe.tpe.cLength;
      krTPM.push_back(smart_tpe);
      m_uiHeaderBytes = nBytes;

      m_access.unlock();
      return ERROR_SUCCESS;
    }*/
    
    if (m_frameVector.empty())
    {
      m_uiBytesRead = 0;
      const cPtr<cMemoryView> buffer = m_pMemLoader->loadMem(Name(), sessionID, false);
      m_frameVector.push_back(tFrameVector::value_type(0, buffer));
    }
  }

  //copy frame vector so that we don't need to hold a lock for this whole routine  
  tFrameVector copyVector(m_frameVector);
  m_access.unlock();

  const unsigned int frameVectorSize = iQCIFSFwkHelper::singleton().getFrameVectorSize(copyVector);

  if ((nOffset.QuadPart >= frameVectorSize) && m_pMemLoader.isValid())
  {
    const __int64 expectedSize = m_pMemLoader->getExpectedSize(Name());
    if (expectedSize > nOffset.QuadPart)
    {
      // return cMemView of 0s since we advertised this much mnemory but haven't been supplied any data for it
      const unsigned int memAmount = static_cast<unsigned int>(min(nBytes, expectedSize - nOffset.LowPart));
      const cConstPtr<cMemoryView> pMem = new cMemoryView(new cMemory((size_t)memAmount, cMemory::eVirtual));
      SMART_TPE smart_tpe;
      smart_tpe.tpe.dwElFlags = TP_ELEMENT_MEMORY;
      smart_tpe.tpe.pBuffer = (PVOID)pMem->getConstBytes();
      smart_tpe.tpe.cLength = memAmount;
      smart_tpe.pMem = pMem;

      nBytes = memAmount;
      krTPM.push_back(smart_tpe);
    }
    else
    {
      nBytes = 0;
      cLockGuard lg(&m_access);
      if (!m_bWritten)  //read past the end of the frame, free memory if not written to (caching optimisation)
        m_frameVector.clear();
    }

    return ERROR_SUCCESS;
  }

  std::vector<cConstPtr<cMemoryView> > memViews;

  iQCIFSFwkHelper::singleton().flattenFrameVector(copyVector, memViews, nOffset.LowPart, nOffset.LowPart + nBytes);

  nBytes = 0;
  for (std::vector<cConstPtr<cMemoryView> >::const_iterator cit = memViews.begin()
    ; memViews.end() != cit
    ; ++cit)
  {
    const cConstPtr<cMemoryView> pMem = *cit;
    SMART_TPE tpe;
    tpe.pMem = pMem;
    tpe.tpe.dwElFlags = TP_ELEMENT_MEMORY;
    tpe.tpe.pBuffer = (PVOID)pMem->getConstBytes();
    tpe.tpe.cLength = pMem->getSize();
    nBytes += tpe.tpe.cLength;
    krTPM.push_back(tpe);
  }
  
  {
    cLockGuard lg(&m_access);
    m_uiBytesRead += nBytes;  

    if ((m_uiHeaderBytes + m_uiBytesRead >= frameVectorSize) && m_pMemLoader.isValid())
    {      
      if (!m_bWritten)  //read past the end of the frame, free memory if not written to (caching optimisation)
        m_frameVector.clear();
    }
  }

  return ERROR_SUCCESS;
}

#ifndef ERROR_INCORRECT_SIZE
#define ERROR_INCORRECT_SIZE 1462
#endif

DWORD cMemComposite::Write(cConstPtr<cMemoryView> buffer, DWORD& nBytes, const LARGE_INTEGER &offset, const int sessionID, ULONGLONG fid)
{
  if (offset.HighPart > 0)
    return ERROR_INCORRECT_SIZE;

  if (0 == nBytes)
    return ERROR_SUCCESS;

  if (cQCIFSFwkHelper::singleton().lowMemory(95))
  {
    //RFHRFH - (95% picked out of the air: might be too cautious/not cautious enough)
    QSOS((L"Refusing write request - low memory."));
    nBytes = 0;
    return ERROR_NOT_ENOUGH_MEMORY;
  }

  std::pair<unsigned int,cConstPtr<cMemoryView> > entry(offset.LowPart, buffer);
  {
    cLockGuard lg(&m_access);
    m_frameVector.push_back(entry);
    m_bWritten = true;
  }
  try
  {
    doNotify(fid);

    InterlockedExchange(&m_sessionID, sessionID);
  }
  //catch (const exception& e)
  //{
  //  QSOS((L"cMemComposite::Write - %S", e.what()));
  //  return ERROR_ACCESS_DENIED;
  //}
  catch (const cBadFormat& e)
  {
    QSOS((L"cMemComposite::Write - %s", e.getMsg().c_str()));
    return ERROR_ACCESS_DENIED;
  }


  return ERROR_SUCCESS;
}

DWORD cMemComposite::Close(ULONGLONG fid)
{
  cActivityScope as1(pActivityGroup,kMemCompositeClose);

  if (m_pMemLoader.isValid())
  {
    try
    {
      m_pMemLoader->notifyClosed(copyFrameVector(), Name(), m_sessionID, cPtr<iFileEventCallback>(this, this), fid);
    }
    catch(cRecoverable& err)
    {
      QSOS((L"ERR - cMemComposite::Close - Caught: %s", err.getMsg().c_str()));
      return ERROR_HANDLE_DISK_FULL;
    }
  }
  return ERROR_SUCCESS;
}

DWORD cMemComposite::SetFileSize(const PLARGE_INTEGER pFileSize)
{
  {
    cLockGuard lg(&m_access);
    std::vector<cConstPtr<cMemoryView> > memViews;
    iQCIFSFwkHelper::singleton().flattenFrameVector(m_frameVector, memViews, 0, pFileSize->LowPart);

    {
      __int64 count(0);
      cLockGuard lg(&m_access);
      m_frameVector.clear();
      for (std::vector<cConstPtr<cMemoryView> >::const_iterator cit = memViews.begin()
        ; memViews.end() != cit && (count < pFileSize->QuadPart)
        ; ++cit)
      {
        const cConstPtr<cMemoryView> pMem = *cit;
        m_frameVector.push_back(tFrameVector::value_type(static_cast<unsigned int>(count)
          , pMem->firstConst(min(pMem->getSize(), static_cast<unsigned int>(pFileSize->QuadPart - count)))));
        count += pMem->getSize();
      }
      m_bWritten = true;
    }
  }

  doNotify(0);

  return ERROR_SUCCESS;
}

void cMemComposite::clearMemCache()
{
  {
    cLockGuard lg(&m_access);
    m_frameVector.clear();
    m_bWritten = false;
  }
  sendValidNotifications();
}

void cMemComposite::errorRemoveFromDirectory()
{
  cDeletableComposite::Delete(true);
}

void cMemComposite::artificialChange()
{
  if (m_pMemLoader.isValid())
    m_pMemLoader->notifyChange(copyFrameVector(), Name(), cPtr<iFileEventCallback>(this, this), 0);
}

template <typename T>
DWORD cMemComposite::writeInfoTemplate(T pFileInfo)
{
  cLockGuard lg(&m_access);
  if (m_frameVector.empty() && m_pMemLoader.isValid())
  {
    LARGE_INTEGER lg;
    lg.QuadPart = m_pMemLoader->getExpectedSize(Name());
    pFileInfo->nFileSizeHigh = lg.HighPart;
    pFileInfo->nFileSizeLow = lg.LowPart;
  }
  else
    writeFileSize<T>(pFileInfo, iQCIFSFwkHelper::singleton().getFrameVectorSize(m_frameVector));

  return ERROR_SUCCESS;
}

void cMemComposite::doNotify(ULONGLONG fid)
{
  cAttributeComposite::modify();

  if (m_pMemLoader.isValid())
    m_pMemLoader->notifyChange(copyFrameVector(), Name(), cPtr<iFileEventCallback>(this, this), fid);

  sendValidNotifications();
}

void cMemComposite::sendValidNotifications()
{
  if (m_pNotificationHandler.isValid())
  {
    iNotificationTarget::tNotifyList notifyList;
    notifyList.push_back(iNotificationTarget::tNotifyList::value_type(FILE_ACTION_MODIFIED, Name()));
    m_pNotificationHandler->Notify(notifyList);
  }
}

inline tFrameVector cMemComposite::copyFrameVector() const
{
  // return value optimisation should make this perform quickly since it is inlined
  cLockGuard lg(&m_access);
  return m_frameVector;
}

DWORD cMemComposite::Rename( const String& sNewName )
{
  return m_pRenameHandler->Rename(
    new cMemFileRename<cMemComposite>(cPtr<cMemComposite>(this, this)
      , new cConstNamed(Name())
      , m_pParentSearchListener)
    , sNewName);
}