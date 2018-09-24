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

//dumpers.cpp

#include "stdafx.h"
#include "dumpers.h"

using namespace vfs;

void dump(cMemoryView::Ptr text)
{
  if(text.isNull())
    return;
  std::string str(text->getConstBytes(), text->getSize());
  std::stringstream strStream(str);
  while(!strStream.eof())
  {
    std::string line;
    std::getline(strStream, line);
    QTRACE((L"%S", line.c_str()));
  }
}

void dump(const std::string& str)
{
  std::stringstream strStream(str);
  while(!strStream.eof())
  {
    std::string line;
    std::getline(strStream, line);
    QTRACE((L"%S", line.c_str()));
  }
}

void dump16(cMemoryView::Ptr text)
{
  if(text.isNull())
    return;
  String str((wchar_t*)text->getConstBytes(), text->getSize() / 2);
  StringStream strStream(str);
  while(!strStream.eof())
  {
    String line;
    std::getline(strStream, line);
    QTRACE((L"%s", line.c_str()));
  }
}

void dumpBuffer(Uint8* ptr, Uint32 lengthToPrint)
{
  const Uint8* lastLine = 0;
  Uint32 byte = 0;
  Uint32 i = 0;
  wchar_t line[80], *linePtr;

  linePtr = line;

  QTRACE((L"memory %p, lengthToPrint 0x%08x", ptr, lengthToPrint));

  while(i<lengthToPrint)
  {
    if(!(i & 15)) // line break every mod 16
    {
      if(lastLine)
      {
        QTRACE((L"%04x %s", i - 16, line));
      }

      //printf("\n ");
      lastLine = ptr;
      linePtr = line;
      *line = 0;
    }

    wsprintf(linePtr, L"%02x ", *ptr++);
    i++;
    byte++;
    linePtr += 3;
  }

  if(lastLine)
  {
    QTRACE((L"%04x %s", i - (i % 16), line));
  }
}


