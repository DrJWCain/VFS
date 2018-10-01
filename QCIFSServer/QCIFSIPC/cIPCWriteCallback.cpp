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

//cIPCWriteCallback.cpp

#include "StdAfx.h"
#include "..\QCIFSFwk\iChildLoaderVisitor.h"
#include "..\QCifsFwk\iQCIFSFwkHelper.h"

#include "cIPCWriteCallback.h"

using namespace vfs;

void dump(std::vector<unsigned char> data, int lineCount)
{
  const int kLineLength = 16;
  unsigned char* bytes = (unsigned char*)&(data.front());
  int sizeLeft = data.size();
  for(int count = 0; count < lineCount && sizeLeft > 0; ++count)
  {
    StringStream str;
    str << std::hex << std::setw(4) << std::setfill(L'0') << count << L":";
    std::stringstream str2;
    for(int i = 0; i < kLineLength; ++i)
    {
      if(i >= sizeLeft)
        break;
      char buff[4];
      _itoa(bytes[i], buff, 16);
      str << std::hex << std::setw(2) << std::setfill(L'0') << widen(buff);
      unsigned char val = bytes[i];
      if(val > 0x20 && val < 127)
        str2 << val;
      else
        str2 << ".";
    }
    QTRACE((L"%s %S", str.str().c_str(), str2.str().c_str()));
    bytes += kLineLength;
    sizeLeft -= kLineLength;
  }
}

void PrintHexDump(DWORD length, PBYTE buffer)
{
  if(length)
  {
    std::vector<unsigned char> data(&buffer[0], &buffer[length]);
    dump(data, 20);
  }
}

void cIPCWriteCallback::setUpPipe()
{
  HPipe = CreateFile(L"\\\\.\\pipe\\srvsvc", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

  auto err = GetLastError();
  QTRACE((L"%S %p, %08x", __FUNCTION__, HPipe, err));

  DWORD dwMode = PIPE_READMODE_MESSAGE;
  auto fSuccess = SetNamedPipeHandleState(
    HPipe,    // pipe handle 
    &dwMode,  // new pipe mode 
    NULL,     // don't set maximum bytes 
    NULL);    // don't set maximum time 

}
cIPCWriteCallback::cIPCWriteCallback(const String& name) : FileSize(0), Name(name)
{
  setUpPipe();
}

unsigned __int64 cIPCWriteCallback::getSize(ULONGLONG fid)
{
  return FileSize;
}

unsigned char convertToNibble(unsigned char c)
{
  if(c >=0x30 && c<= 0x39)
    return c - 0x30;
  return c - 0x61 + 10;
}

DWORD cIPCWriteCallback::readBytes(tTransmitList &krTPM, DWORD& nBytes, const LARGE_INTEGER &nOffset, const int sessionID, ULONGLONG fid)
{
  //QTRACE((L"cIPCWriteCallback::readBytes %s %I64d:%d", Name.c_str(), nOffset.QuadPart, nBytes));
  if (m_frameVector.empty())
    return ERROR_READ_FAULT;

  m_access.lock();
  tFrameVector copyVector(m_frameVector);
  m_frameVector.clear();
  m_access.unlock();

  std::vector<cConstPtr<cMemoryView> > memViews;

  //QTRACE((L"copyVector %d", copyVector.size()));
  //for(auto ref : copyVector)
  //{
  //  QTRACE((L"size: %I64d-%I64d", ref.first, ref.second->getSize()));
  //}
  iQCIFSFwkHelper::singleton().flattenFrameVector(copyVector, memViews, nOffset.LowPart, nOffset.LowPart + nBytes);

  nBytes = 0;
  for (std::vector<cConstPtr<cMemoryView> >::const_iterator cit = memViews.begin()
    ; memViews.end() != cit
    ; ++cit)
  {
    const cConstPtr<cMemoryView> pMem = *cit;
    //QTRACE((L"SIZE %d", pMem->getSize()));
    nBytes += pMem->getSize();
    SMART_TPE tpe;
    tpe.pMem = pMem;
    tpe.tpe.dwElFlags = TP_ELEMENT_MEMORY;
    tpe.tpe.pBuffer = (PVOID)pMem->getConstBytes();
    tpe.tpe.cLength = pMem->getSize();
    krTPM.push_back(tpe);
  }

  if(krTPM.empty())
    return ERROR_READ_FAULT;

  return ERROR_SUCCESS;
}

DWORD cIPCWriteCallback::setSize(unsigned __int64 newSize)
{
  FileSize = newSize;
  return ERROR_SUCCESS;
}

DWORD cIPCWriteCallback::writeBytes(vfs::cConstPtr<vfs::cMemoryView> buffer, const LARGE_INTEGER &offset, const int sessionID)
{
  QTRACE((L"cIPCWriteCallback::writeBytes %s %I64d:%Iu", Name.c_str(), offset.QuadPart, buffer->getSize()));
  if(offset.QuadPart + buffer->getSize() > (__int64)FileSize)
    FileSize = offset.QuadPart + buffer->getSize();

  if (iQCIFSFwkHelper::singleton().lowMemory(95))
  {
    QSOS((L"Refusing write request - low memory."));
    return ERROR_NOT_ENOUGH_MEMORY;
  }

  //PrintHexDump(buffer->getSize(), (PBYTE)buffer->getConstBytes());

  vfs::cConstPtr<vfs::cMemoryView> buff = new vfs::cMemoryView(new vfs::cMemory((size_t)4096, cMemory::eHeap));
  DWORD read = 0;
  auto ret = TransactNamedPipe(HPipe, (PBYTE)buffer->getConstBytes(), buffer->getSize(), (PBYTE)buff->getConstBytes(), buff->getSize(), &read, 0);
  auto err = GetLastError();
  //QTRACE((L"TransactNamedPipe returned %d, %08x %d", ret, err, read));
  if(!ret && err == 0x000000E9)
  {
    CloseHandle(HPipe);
    setUpPipe();

    auto ret = TransactNamedPipe(HPipe, (PBYTE)buffer->getConstBytes(), buffer->getSize(), (PBYTE)buff->getConstBytes(), buff->getSize(), &read, 0);
   
    if(!ret)
    {
      err = GetLastError();
      QSOS((L"TransactNamedPipe returned %d, %08x %d", ret, err, read));
      return ERROR_ARENA_TRASHED;
    }
  }
  //PrintHexDump(read, (PBYTE)buff->getConstBytes());

  buff = buff->firstConst((size_t)read);
  //dump((PBYTE)buff->getConstBytes(), buff->getSize());
  //PrintHexDump(read, (PBYTE)buff->getConstBytes());

  std::pair<unsigned int,cConstPtr<cMemoryView> > entry(offset.LowPart, buff);
  {
    cLockGuard lg(&m_access);
    m_frameVector.push_back(entry);
    //PrintHexDump(buffer->getSize(), (PBYTE )buffer->getConstBytes());
  }

  return ERROR_SUCCESS;
  //return ERROR_ARENA_TRASHED;
}

DWORD cIPCWriteCallback::close(ULONGLONG fid)
{
  return ERROR_SUCCESS;
}

