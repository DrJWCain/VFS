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

//cCallbackComposite.cpp

#include "StdAfx.h"
#include "cCallbackComposite.h"

#include "writeFileSize.h"
#include "iFileEvent.h"
#include "cNotificationHandler.h"
#include "cCallbackChangeListener.h"
#include "QKernelBase.h"

using namespace vfs;


cCallbackComposite::cCallbackComposite(const fileInfo& fi
  , const cPtr<cSearchListener> pSearchListener
  , vfs::cPtr<iReadCallback> readCallback
  , const vfs::cPtr<iFileEvent> pFileEvent
  , bool bDeletable)
: cDeletableComposite(fi, pSearchListener, pFileEvent, bDeletable)
, ReadCallback(readCallback)
, CurrentSize(0)
{
  //checkAttibutesValid();
}

cCallbackComposite::~cCallbackComposite(void)
{
}

void cCallbackComposite::checkAttibutesValid(ULONGLONG fid)
{
  unsigned __int64 newSize = ReadCallback->getSize(fid);
  if(CurrentSize != newSize)
  {
    modify();
    LARGE_INTEGER li;
    li.QuadPart = newSize;
    m_fileInfo.w32Data.nFileSizeHigh = li.HighPart;
    m_fileInfo.w32Data.nFileSizeLow = li.LowPart;

    //FILETIME ft;    
    //SYSTEMTIME st;    
    //GetSystemTime(&st);              // Gets the current system time
    //SystemTimeToFileTime(&st, &ft);  // Converts the current system time to file time format
    //m_fileInfo.w32Data.ftLastAccessTime = ft;
    //m_fileInfo.w32Data.ftLastWriteTime = ft;
    //m_fileInfo.w32Data.ftCreationTime = ft;
    CurrentSize = newSize;
  }
}

DWORD cCallbackComposite::writeInfo(LPBY_HANDLE_FILE_INFORMATION pFileInfo, ULONGLONG fid)
{
  //QTRACE((L"cCallbackComposite::writeInfo"));
  checkAttibutesValid(fid);
  const DWORD dwRet = cAttributeComposite::writeInfo(pFileInfo, fid);
  return dwRet;
}

DWORD cCallbackComposite::writeInfo(PfileInfo pFileInfo, ULONGLONG fid)
{
  //QTRACE((L"cCallbackComposite::writeInfo"));
  checkAttibutesValid(fid);
  const DWORD dwRet = cAttributeComposite::writeInfo(pFileInfo, fid);
  return dwRet;
}

DWORD cCallbackComposite::Read(tTransmitList &krTPM, DWORD& nBytes, const LARGE_INTEGER &nOffset, const int sessionID, ULONGLONG fid)
{
  //QTRACE((L"cCallbackComposite::Read"));
  try
  {
    return ReadCallback->readBytes(krTPM, nBytes, nOffset, sessionID, fid);
  }
  catch(cRecoverable& err)
  {
    QSOS((L"cCallbackComposite::Read - ERR: %s", err.getMsg().c_str()));
  }

  nBytes = 0;
  return ERROR_NOT_READY;
}

DWORD cCallbackComposite::Close(ULONGLONG fid)
{
  return ReadCallback->close(fid);
}

bool cCallbackComposite::hasSecurityDescriptor()
{
  //QTRACE((L"cCallbackComposite::hasSecurityDescriptor"));
  return ReadCallback->canExecute();
}

bool cCallbackComposite::hasBatchOpLock()
{
  //QTRACE((L"cCallbackComposite::hasBatchOpLock"));
  return ReadCallback->canCache();
}

DWORD cCallbackComposite::registerChangeListener(vfs::cPtr<iFileChangeListener> listener) 
{
  return ReadCallback->registerChangeListener(new cCallbackChangeListener(listener));
}



cWriteableCallbackComposite::cWriteableCallbackComposite(const fileInfo& fi
  , const cPtr<cSearchListener> pSearchListener
  , vfs::cPtr<iWriteCallback> writeCallback
  , bool bDeletable)
: cDeletableComposite(fi, pSearchListener, ptr_cast<cPtr<iFileEvent> >(writeCallback), bDeletable)
, WriteCallback(writeCallback)
{
}

cWriteableCallbackComposite::~cWriteableCallbackComposite(void)
{
}

DWORD cWriteableCallbackComposite::writeInfo(LPBY_HANDLE_FILE_INFORMATION pFileInfo, ULONGLONG fid)
{
  const DWORD dwRet = cAttributeComposite::writeInfo(pFileInfo, fid);
  if (ERROR_SUCCESS == dwRet)
    writeFileSize<LPBY_HANDLE_FILE_INFORMATION >(pFileInfo, WriteCallback->getSize(fid));
  return dwRet;
}

DWORD cWriteableCallbackComposite::writeInfo(PfileInfo pFileInfo, ULONGLONG fid)
{
  const DWORD dwRet = cAttributeComposite::writeInfo(pFileInfo, fid);
  if (ERROR_SUCCESS == dwRet)
    writeFileSize<WIN32_FIND_DATAW*>(&(pFileInfo->w32Data), WriteCallback->getSize(fid));
  return dwRet;
}

DWORD cWriteableCallbackComposite::Read(tTransmitList &krTPM, DWORD& nBytes, const LARGE_INTEGER &nOffset, const int sessionID, ULONGLONG fid)
{
  return WriteCallback->readBytes(krTPM, nBytes, nOffset, sessionID, fid);
}

DWORD cWriteableCallbackComposite::Write(vfs::cConstPtr<vfs::cMemoryView> buffer, DWORD &nBytes, const LARGE_INTEGER &offset, const int sessionID, ULONGLONG fid)
{
  //QTRACE((L"cWriteableCallbackComposite::Write %I64d, %d",offset.QuadPart, nBytes));
  return WriteCallback->writeBytes(buffer, offset, sessionID, fid);
}

DWORD cWriteableCallbackComposite::SetFileSize(const PLARGE_INTEGER pFileSize)
{
  return WriteCallback->setSize(pFileSize->QuadPart);
}

DWORD cWriteableCallbackComposite::Close(ULONGLONG fid)
{
  return WriteCallback->close(fid);
}

DWORD cWriteableCallbackComposite::registerChangeListener(vfs::cPtr<iFileChangeListener> listener) 
{
  return WriteCallback->registerChangeListener(new cCallbackChangeListener(listener));
}

vfs::cPtr<iTransactCallback> cWriteableCallbackComposite::getTransactInterface() 
{ 
  return ptr_cast<cPtr<iTransactCallback> >(WriteCallback); 
}