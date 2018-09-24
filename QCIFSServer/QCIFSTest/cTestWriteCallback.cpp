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

//cTestWriteCallback.cpp

#include "StdAfx.h"

#include "cTestWriteCallback.h"
#include "cTestLoader.h"



#include "..\QCifsFwk\iQCIFSFwkHelper.h"

using namespace vfs;

cTestWriteCallback::cTestWriteCallback(const String& name, cTestLoader* parent) : Name(name), FileSize(0), Parent(parent)
{
}

unsigned __int64 cTestWriteCallback::getSize(ULONGLONG fid)
{
  return FileSize;
}

DWORD cTestWriteCallback::readBytes(tTransmitList &krTPM, DWORD& nBytes, const LARGE_INTEGER &nOffset, const int sessionID, ULONGLONG fid)
{
  if (m_frameVector.empty())
    return ERROR_READ_FAULT;

  if(nOffset.QuadPart >= (__int64)FileSize)
    return ERROR_HANDLE_EOF;

  if(nOffset.QuadPart + nBytes > (__int64)FileSize)
    nBytes = FileSize - nOffset.QuadPart;

  m_access.lock();
  tFrameVector copyVector(m_frameVector);
  m_access.unlock();

  std::vector<cConstPtr<cMemoryView> > memViews;

  iQCIFSFwkHelper::singleton().flattenFrameVector(copyVector, memViews, nOffset.LowPart, nOffset.LowPart + nBytes);

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
    krTPM.push_back(tpe);
  }
  

  return ERROR_SUCCESS;
}

DWORD cTestWriteCallback::setSize(unsigned __int64 newSize)
{
  FileSize = newSize;
  return ERROR_SUCCESS;
}

DWORD cTestWriteCallback::writeBytes(vfs::cConstPtr<vfs::cMemoryView> buffer, const LARGE_INTEGER &offset, const int sessionID)
{
  //QTRACE((L"cTestWriteCallback::writeBytes %I64d:%d", offset.QuadPart, buffer->getSize()));
  if(!buffer->getSize())
  {
    //JCJCJC - MS writes zero byte payloads to set EOF
    return setSize(offset.QuadPart);
  }

  if(offset.QuadPart + buffer->getSize() > (__int64)FileSize)
    FileSize = offset.QuadPart + buffer->getSize();

  if (iQCIFSFwkHelper::singleton().lowMemory(95))
  {
    //RFHRFH - (95% picked out of the air: might be too cautious/not cautious enough)
    QSOS((L"Refusing write request - low memory."));
    return ERROR_NOT_ENOUGH_MEMORY;
  }

  if(offset.LowPart < 0x10000)// only store 1st 64 k
  {
    std::pair<unsigned int,cConstPtr<cMemoryView> > entry(offset.LowPart, buffer);
    {
      cLockGuard lg(&m_access);
      m_frameVector.push_back(entry);
    }
  }

  return ERROR_SUCCESS;
  //return ERROR_ARENA_TRASHED;
}

DWORD cTestWriteCallback::close(ULONGLONG fid)
{
  return ERROR_SUCCESS;
}

DWORD cTestWriteCallback::From(vfs::cPtr<iRename> pRenameSource
    , const vfs::String& sNewName
    , const vfs::String& sUserName)
{
  return ERROR_SUCCESS;
}

DWORD cTestWriteCallback::notifyDelete()
{
  Parent->removeFile(Name);
  return ERROR_SUCCESS;
}

void cTestWriteCallback::rename(const vfs::String& newName, cTestLoader* newParent) 
{
  Parent->removeFile(Name);
  Name=newName;
  Parent = newParent;
}