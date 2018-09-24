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

#include "stdafx.h"

#include "cBasicFile.h"

using namespace vfs;

cBasicFile::cBasicFile(const String& filepath) : mFilepath(filepath), mSize(0)
{
  iFile::Ptr file = iFileManager::singleton().openFile(mFilepath, fFileAccess_Read);
  mSize = file->getSize();
  QTRACE((L"cBasicFile::cBasicFile %s. Size %08I64x", mFilepath.c_str(), mSize));
}

cBasicFile::~cBasicFile()
{
}

WIN32_FILE_ATTRIBUTE_DATA cBasicFile::getFileAttributes()
{
  WIN32_FILE_ATTRIBUTE_DATA FileAttr = { 0 };
  GetFileAttributesEx(
    mFilepath.c_str(),                  // File or directory name
    GetFileExInfoStandard,              // Attribute
    &FileAttr);                         // Attribute information
  return FileAttr;
}

DWORD cBasicFile::readBytes(tTransmitList &krTPM, DWORD &nBytes, const LARGE_INTEGER &nOffset, const int sessionID, ULONGLONG fid)
{
  DWORD ret = ERROR_SUCCESS;
  //QTRACE((L"request for file %s. Offset %08I64x, length %i", mFilepath.c_str(), nOffset.QuadPart, nBytes));
  
  if( nOffset.QuadPart < Int64(getSize(fid)) )
  {
    iFile::Ptr michaelsFile = iFileManager::singleton().openFile(mFilepath, fFileAccess_Read);
    if(michaelsFile.isValid())
    {
      cMemoryView::Ptr buf = new cMemoryView(michaelsFile->read(UInt64(nOffset.QuadPart), nBytes));
      SMART_TPE smartTpe;
      smartTpe.pMem = buf;
      smartTpe.tpe.dwElFlags = TP_ELEMENT_MEMORY;
      smartTpe.tpe.pBuffer = (PVOID)buf->getConstBytes();
      smartTpe.tpe.cLength = buf->getSize();
      krTPM.push_back(smartTpe);
      //QTRACE((L"sending packet size = %i, offset = %i", buf->getSize(), int(nOffset.QuadPart)));
    }
  }
  else
    ret = STATUS_END_OF_FILE;

  return ret;
}
