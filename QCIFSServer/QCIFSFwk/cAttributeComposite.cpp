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

//cAttributeComposite.cpp

#include "StdAfx.h"

#include "cAttributeComposite.h"
#include "iWriteInfo.h"
//#include "..\..\MiscPlugins\QOptions\QOptions.h"

using namespace vfs;

bool dynamicTimestamps()
{
  return true;
  //static bool firstTime = true;
  //static bool pDynamicTimestamps = true;

  //if(firstTime)
  //{
  //  try // check for special case
  //  {
  //    iOptionManager::singleton().checkOut(L"Files Have Static Times");
  //    pDynamicTimestamps = false;  // inverse of register value - which is the exception case
  //  }
  //  catch(cRecoverable&)
  //  {
  //  }
  //  firstTime = false;
  //}
  //return pDynamicTimestamps;
}


cAttributeComposite::cAttributeComposite(const fileInfo& fi)
: m_fileInfo(fi)
{
}

cAttributeComposite::~cAttributeComposite(void)
{
}

String cAttributeComposite::Name() const
{
  cLockGuard lg(&m_fiAccess);
  return m_fileInfo.w32Data.cFileName;
}

DWORD cAttributeComposite::writeInfo(LPBY_HANDLE_FILE_INFORMATION pFileInfo, ULONGLONG fid)
{
  cLockGuard lg(&m_fiAccess);
  iWriteInfo::singleton().writeFileInfo(pFileInfo, &m_fileInfo);
  return ERROR_SUCCESS;
}

DWORD cAttributeComposite::writeInfo(PfileInfo pFileInfo, ULONGLONG fid)
{
  cLockGuard lg(&m_fiAccess);
  *pFileInfo = m_fileInfo;

  return ERROR_SUCCESS;
}

DWORD cAttributeComposite::SetFileInfo(const FILETIME& creationTime
  , const FILETIME& modifiedTime
  , const FILETIME& accessTime
  , const DWORD attributes)
{
  cLockGuard lg(&m_fiAccess);
  if (0 != modifiedTime.dwHighDateTime
    && 0 != modifiedTime.dwLowDateTime)
    m_fileInfo.w32Data.ftLastWriteTime = modifiedTime;
  if (0 != accessTime.dwHighDateTime
    && 0 != accessTime.dwLowDateTime)
    m_fileInfo.w32Data.ftLastAccessTime = accessTime;
  return ERROR_SUCCESS;
}

void cAttributeComposite::access()
{
  cLockGuard lg(&m_fiAccess);
  GetSystemTimeAsFileTime(&(m_fileInfo.w32Data.ftLastAccessTime));
}

void cAttributeComposite::modify()
{
  if(dynamicTimestamps())
  {
    cLockGuard lg(&m_fiAccess);
    GetSystemTimeAsFileTime(&(m_fileInfo.w32Data.ftLastAccessTime));
    m_fileInfo.w32Data.ftLastWriteTime = m_fileInfo.w32Data.ftLastAccessTime;
  }
}

DWORD cAttributeComposite::atts() const
{
  cLockGuard lg(&m_fiAccess);
  return m_fileInfo.w32Data.dwFileAttributes;
}

vfs::cPtr<iTransactCallback> cAttributeComposite::getTransactInterface() { return vfs::cPtr<iTransactCallback>(); }