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
  if(INVALID_HANDLE_VALUE != HPipe)
    return;

  //This string matches the server string used in runSrvSvc() ...
  HPipe = CreateFile(L"\\\\.\\pipe\\vfs-srvsvc", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

  auto err = GetLastError();
  QTRACE((L"%S %p, %08x", __FUNCTION__, HPipe, err));

  DWORD dwMode = PIPE_READMODE_MESSAGE;
  auto fSuccess = SetNamedPipeHandleState(
    HPipe,    // pipe handle 
    &dwMode,  // new pipe mode 
    NULL,     // don't set maximum bytes 
    NULL);    // don't set maximum time 
}

void cIPCWriteCallback::closePipe()
{
  if(INVALID_HANDLE_VALUE != HPipe)
  {
    QTRACE((L"%S %s %p", __FUNCTION__, Name.c_str(), HPipe));
    CloseHandle(HPipe);
    HPipe = INVALID_HANDLE_VALUE;
  }
}


cIPCWriteCallback::cIPCWriteCallback(const String& name) : Name(name), HPipe(INVALID_HANDLE_VALUE)
{
  setUpPipe();
}

DWORD cIPCWriteCallback::close(ULONGLONG fid)
{
  QTRACE((L"%S %s", __FUNCTION__, Name.c_str()));
  closePipe();
  return ERROR_SUCCESS;
}


unsigned __int64 cIPCWriteCallback::getSize(ULONGLONG fid)
{
  return 0;
}

DWORD cIPCWriteCallback::setSize(unsigned __int64 newSize)
{
  return ERROR_SUCCESS;
}


DWORD cIPCWriteCallback::readBytes(tTransmitList &krTPM, DWORD& nBytes, const LARGE_INTEGER &nOffset, const int sessionID, ULONGLONG fid)
{
  //QTRACE((L"%S %s %I64d:%d", __FUNCTION__, Name.c_str(), nOffset.QuadPart, nBytes));

  vfs::cPtr<vfs::cMemoryView> buff = new vfs::cMemoryView(new vfs::cMemory((size_t)nBytes, cMemory::eHeap));

  DWORD read = 0;
  ReadFile(HPipe, buff->getBytes(), buff->getSize(), &read, 0);
  buff = buff->first((size_t)read);
  //PrintHexDump(buff->getSize(), (PBYTE)buff->getConstBytes());

  nBytes = buff->getSize();
  SMART_TPE tpe;
  tpe.pMem = buff;
  tpe.tpe.dwElFlags = TP_ELEMENT_MEMORY;
  tpe.tpe.pBuffer = (PVOID)buff->getConstBytes();
  tpe.tpe.cLength = buff->getSize();
  krTPM.push_back(tpe);

  return ERROR_SUCCESS;
}

DWORD cIPCWriteCallback::writeBytes(vfs::cConstPtr<vfs::cMemoryView> buffer, const LARGE_INTEGER &offset, const int sessionID, ULONGLONG fid)
{
  //QTRACE((L"%S %s %I64d:%Iu", __FUNCTION__, Name.c_str(), offset.QuadPart, buffer->getSize()));

  if (iQCIFSFwkHelper::singleton().lowMemory(95))
  {
    QSOS((L"Refusing write request - low memory."));
    return ERROR_NOT_ENOUGH_MEMORY;
  }

  //PrintHexDump(buffer->getSize(), (PBYTE)buffer->getConstBytes());

  vfs::cConstPtr<vfs::cMemoryView> buff = new vfs::cMemoryView(new vfs::cMemory((size_t)4096, cMemory::eHeap));
  DWORD read = 0;

  DWORD bytesWritten = 0;
  auto ret = WriteFile(HPipe, (PBYTE)buffer->getConstBytes(), buffer->getSize(), &bytesWritten, 0);
  auto err = GetLastError();
  //QTRACE((L"TransactNamedPipe returned %d, %08x %d", ret, err, read));
  if(!ret && err)
  {
    QTRACE((L"WriteFile returned %d, %08x %d", ret, err, read));
    closePipe();
    setUpPipe();

    auto ret = WriteFile(HPipe, (PBYTE)buffer->getConstBytes(), buffer->getSize(), &bytesWritten, 0);
    if(!ret)
    {
      err = GetLastError();
      QSOS((L"WriteFile returned %d, %08x %d", ret, err, read));
      return ERROR_ARENA_TRASHED;
    }
  }

  return ERROR_SUCCESS;
}

DWORD cIPCWriteCallback::transact(vfs::cConstPtr<vfs::cMemoryView> buffer, tTransmitList &krTPM, DWORD& nBytesRead, const int sessionID, ULONGLONG fid)
{
  //QTRACE((L"%S %s %I64d", __FUNCTION__, Name.c_str(), buffer->getSize()));

  if(iQCIFSFwkHelper::singleton().lowMemory(95))
  {
    QSOS((L"Refusing write request - low memory."));
    return ERROR_NOT_ENOUGH_MEMORY;
  }

  //PrintHexDump(buffer->getSize(), (PBYTE)buffer->getConstBytes());

  vfs::cPtr<vfs::cMemoryView> buff = new vfs::cMemoryView(new vfs::cMemory((size_t)nBytesRead, cMemory::eHeap));
  DWORD read = 0;
  auto ret = TransactNamedPipe(HPipe, (PBYTE)buffer->getConstBytes(), buffer->getSize(), (PBYTE)buff->getConstBytes(), buff->getSize(), &nBytesRead, 0);

  buff = buff->first((size_t)nBytesRead);
  //QTRACE((L"TransactNamedPipe 0x%08x, %d", ret, nBytesRead));
  //PrintHexDump(buff->getSize(), (PBYTE)buff->getConstBytes());

  nBytesRead = buff->getSize();
  SMART_TPE tpe;
  tpe.pMem = buff;
  tpe.tpe.dwElFlags = TP_ELEMENT_MEMORY;
  tpe.tpe.pBuffer = (PVOID)buff->getConstBytes();
  tpe.tpe.cLength = buff->getSize();
  krTPM.push_back(tpe);

  return ERROR_SUCCESS;
}

