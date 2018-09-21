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

/*
**  
*/

#include "stdafx.h"
#include "QKernelBase.h"

using namespace std;
using namespace vfs;

String QAPI vfs::trim(const String & ToTrim)
{
  if (ToTrim.size() > 0)
  {
    const size_t numBytesInStrBuffer = (1u + ToTrim.size()) * sizeof(Char);
    Char * str = (Char *)_alloca(numBytesInStrBuffer);
    wcsncpy(str, ToTrim.c_str(), ToTrim.size());
    str[ToTrim.size()] = L'\0';
    Char *ibuf, *obuf;
    for (ibuf = obuf = str; *ibuf;)
    {
      while (*ibuf && (iswspace (*ibuf)))  ibuf++;
      if (*ibuf && (obuf != str))          *(obuf++) = L' ';
      while (*ibuf && (!iswspace (*ibuf))) *(obuf++) = *(ibuf++);
    }
    *obuf = L'\0';
    return str;
  }
  else
  {
    return L"";
  }
}

String QAPI vfs::toUpper (const String & str)
{
  String ret = str;
  CharUpper((LPWSTR)ret.c_str());
  return ret;
}


String QAPI vfs::toLower (const String & str)
{
  String ret = str;
  CharLower((LPWSTR)ret.c_str());
  return ret;
}


String QAPI vfs::widen(const std::string & str)
{
  const ctype<wchar_t>& char_facet = _USE(locale(),ctype<wchar_t>);
  String ret;
  ret.reserve(str.size());//avoid incremental allocation
  for(std::string::const_iterator it = str.begin(); it != str.end(); ++it)
  {
    ret += char_facet.widen(*it);
  }
  return ret;
}

std::string QAPI vfs::narrow(const String & str)
{
  const ctype<wchar_t>& char_facet = _USE(locale(),ctype<wchar_t>);
  std::string ret;
  ret.reserve(str.size());//avoid incremental allocation
  for(String::const_iterator it = str.begin(); it != str.end(); ++it)
  {
    ret += char_facet.narrow(*it);
  }
  return ret;
}

#if defined(_DEBUG)
  bool QAPI vfs::checkPointer2(const void * Pointer, unsigned int NumBytes)
  {
    if (!Pointer) return true;//zero is ok

    bool ret = !IsBadReadPtr(Pointer, NumBytes);
    if(!ret && IsDebuggerPresent())
      __debugbreak();
    return ret;
  }
#endif

void QAPI vfs::safeCopy(void* dest, size_t dest_sz, const void* src, size_t src_sz)
{
  memset(dest,0,dest_sz);//blank the dest structure
  memcpy(dest,src,Min(dest_sz,src_sz));//copy the correct amount of data
}
