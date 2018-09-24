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

cIPCWriteCallback::cIPCWriteCallback(const String& name) : FileSize(0), Name(name)
{
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


//SNIA SDC plugfest 2011 ... 
//Bytes from "ms bind" acording to netmon ...
//0000   05 00 0c 03 10 00 00 00 74 00 00 00 02 00 00 00
//0010   b8 10 b8 10 3f 71 00 00 0d 00 5c 50 49 50 45 5c
//0020   73 72 76 73 76 63 00 00 03 00 00 00 02 00 02 00
//0030   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
//0040   00 00 00 00 00 00 00 00 33 05 71 71 ba be 37 49
//0050   83 19 b5 db ef 9c cc 36 01 00 00 00 03 00 03 00
//0060   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
//0070   00 00 00 00
//
//
//  std::string fred("05000c03100000007400000002000000b810b8103f7100000d005c504950455c7372767376630000030000000200020000000000000000000000000000000000000000000000000033057171babe37498319b5dbef9ccc3601000000030003000000000000000000000000000000000000000000");
//
//  int i = 0;
//  cPtr<cMemoryView> mem = new cMemoryView(new cMemory(fred.size()/2, cMemory::eHeap));
//  char* bytes = mem->getBytes();
//  for(std::string::iterator it = fred.begin(); it != fred.end(); ++it)
//  {
//    unsigned char val = (unsigned char)*it;
//    
//    unsigned char valAsUChar = convertToNibble(val);
//    valAsUChar <<= 4;
//    ++it;
//    val = (unsigned char)*it;
//    valAsUChar += convertToNibble(val);
//
//    bytes[i] = valAsUChar;
//    ++i;
//  }
//
//  SMART_TPE tpe;
//  tpe.pMem = mem;
//  tpe.tpe.dwElFlags = TP_ELEMENT_MEMORY;
//  tpe.tpe.pBuffer = (PVOID)mem->getConstBytes();
//  tpe.tpe.cLength = mem->getSize();
//  nBytes = tpe.tpe.cLength;
//  krTPM.push_back(tpe);
//


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

DWORD cIPCWriteCallback::setSize(unsigned __int64 newSize)
{
  FileSize = newSize;
  return ERROR_SUCCESS;
}

DWORD cIPCWriteCallback::writeBytes(vfs::cConstPtr<vfs::cMemoryView> buffer, const LARGE_INTEGER &offset, const int sessionID)
{
  //QTRACE((L"cIPCWriteCallback::writeBytes %s %I64d:%Iu", Name.c_str(), offset.QuadPart, buffer->getSize()));
  if(offset.QuadPart + buffer->getSize() > (__int64)FileSize)
    FileSize = offset.QuadPart + buffer->getSize();

  if (iQCIFSFwkHelper::singleton().lowMemory(95))
  {
    //RFHRFH - (95% picked out of the air: might be too cautious/not cautious enough)
    QSOS((L"Refusing write request - low memory."));
    return ERROR_NOT_ENOUGH_MEMORY;
  }

  std::pair<unsigned int,cConstPtr<cMemoryView> > entry(offset.LowPart, buffer);
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

