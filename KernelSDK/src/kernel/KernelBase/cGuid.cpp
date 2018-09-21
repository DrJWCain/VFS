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
#include <objbase.h>
#include "fatal.h"

using namespace vfs;


// =======================================================================================
// === Constructors ======================================================================
// =======================================================================================
//{{{
cGuid::cGuid(
  const UInt32        Data1,
  const UInt16        Data2,
  const UInt16        Data3,
  const UInt8         Data50,
  const UInt8         Data51,
  const UInt8         Data52,
  const UInt8         Data53,
  const UInt8         Data54,
  const UInt8         Data55,
  const UInt8         Data56,
  const UInt8         Data57)
{
  m_Data1 = Data1;
  m_Data2 = Data2;
  m_Data3 = Data3;
  m_Data4[0] = Data50;
  m_Data4[1] = Data51;
  m_Data4[2] = Data52;
  m_Data4[3] = Data53;
  m_Data4[4] = Data54;
  m_Data4[5] = Data55;
  m_Data4[6] = Data56;
  m_Data4[7] = Data57;
}
// ---------------------------------------------------------------------------------------
//}}}
//{{{
cGuid::cGuid(
  const UInt32        Data1,
  const UInt16        Data2,
  const UInt16        Data3,
  const UInt8         Data50,
  const UInt8         Data51,
  const UInt8         Data52,
  const UInt8         Data53,
  const UInt8         Data54,
  const UInt8         Data55,
  const UInt8         Data56,
  const UInt8         Data57,
  const String &      Comment)
{
  m_Data1 = Data1;
  m_Data2 = Data2;
  m_Data3 = Data3;
  m_Data4[0] = Data50;
  m_Data4[1] = Data51;
  m_Data4[2] = Data52;
  m_Data4[3] = Data53;
  m_Data4[4] = Data54;
  m_Data4[5] = Data55;
  m_Data4[6] = Data56;
  m_Data4[7] = Data57;
  m_Comment = Comment;
}
// ---------------------------------------------------------------------------------------
//}}}
//{{{
cGuid::cGuid(const UInt64 Data0, const UInt64 Data1)
{
  m_Data64[0] = Data0;
  m_Data64[1] = Data1;
}
// ---------------------------------------------------------------------------------------
//}}}
//{{{
cGuid::cGuid(const UInt64 Data0, const UInt64 Data1, const String & Comment)
{
  m_Data64[0] = Data0;
  m_Data64[1] = Data1;
  m_Comment = Comment;
}
// ---------------------------------------------------------------------------------------
//}}}
//{{{
cGuid::cGuid(const String & Serialised) throw(cBadFormat)
{
  _deserialise(Serialised.c_str());
}
// ---------------------------------------------------------------------------------------
//}}}
//{{{
cGuid::cGuid(const Char * const Serialised) throw(cBadFormat)
{
  _deserialise(Serialised);
}
//}}}
//{{{
cGuid::cGuid(const GUID& Win32Guid)
{
  m_Data1    = Win32Guid.Data1;
  m_Data2    = Win32Guid.Data2;
  m_Data3    = Win32Guid.Data3;
  m_Data4[0] = Win32Guid.Data4[0];
  m_Data4[1] = Win32Guid.Data4[1];
  m_Data4[2] = Win32Guid.Data4[2];
  m_Data4[3] = Win32Guid.Data4[3];
  m_Data4[4] = Win32Guid.Data4[4];
  m_Data4[5] = Win32Guid.Data4[5];
  m_Data4[6] = Win32Guid.Data4[6];
  m_Data4[7] = Win32Guid.Data4[7];
}
// ---------------------------------------------------------------------------------------
//}}}
//{{{
cGuid::cGuid(const GUID& Win32Guid, const String& Comment)
{
  m_Data1    = Win32Guid.Data1;
  m_Data2    = Win32Guid.Data2;
  m_Data3    = Win32Guid.Data3;
  m_Data4[0] = Win32Guid.Data4[0];
  m_Data4[1] = Win32Guid.Data4[1];
  m_Data4[2] = Win32Guid.Data4[2];
  m_Data4[3] = Win32Guid.Data4[3];
  m_Data4[4] = Win32Guid.Data4[4];
  m_Data4[5] = Win32Guid.Data4[5];
  m_Data4[6] = Win32Guid.Data4[6];
  m_Data4[7] = Win32Guid.Data4[7];
  m_Comment = Comment;
}
//}}}
//{{{
// =======================================================================================
// === createUnique ======================================================================
// =======================================================================================

cGuid QAPI cGuid::createUnique (const String& Comment)
{
  GUID ComGUID;
#if defined(_DEBUG)
  const HRESULT hr = CoCreateGuid(&ComGUID);
  QKERNELBASEVERIFY(!FAILED(hr));
#else
  (void)CoCreateGuid(&ComGUID);
#endif
  return cGuid (ComGUID, Comment);
}
//}}}

// =======================================================================================
// === gets ==============================================================================
// =======================================================================================

UInt32 QAPI cGuid::getData1() const  { return m_Data1; }
UInt16 QAPI cGuid::getData2() const { return m_Data2; }
UInt16 QAPI cGuid::getData3() const { return m_Data3; }
void QAPI cGuid::getData4 (UInt8 * const data4_0, UInt8 * const data4_1, UInt8 * const data4_2, UInt8 * const data4_3, UInt8 * const data4_4, UInt8 * const data4_5, UInt8 * const data4_6, UInt8 * const data4_7) const
{
  *data4_0 = m_Data4[0];
  *data4_1 = m_Data4[1];
  *data4_2 = m_Data4[2];
  *data4_3 = m_Data4[3];
  *data4_4 = m_Data4[4];
  *data4_5 = m_Data4[5];
  *data4_6 = m_Data4[6];
  *data4_7 = m_Data4[7];
}
UInt8 QAPI cGuid::getData4_0() const  { return m_Data4[0]; }
UInt8 QAPI cGuid::getData4_1() const  { return m_Data4[1]; }
UInt8 QAPI cGuid::getData4_2() const  { return m_Data4[2]; }
UInt8 QAPI cGuid::getData4_3() const  { return m_Data4[3]; }
UInt8 QAPI cGuid::getData4_4() const  { return m_Data4[4]; }
UInt8 QAPI cGuid::getData4_5() const  { return m_Data4[5]; }
UInt8 QAPI cGuid::getData4_6() const  { return m_Data4[6]; }
UInt8 QAPI cGuid::getData4_7() const  { return m_Data4[7]; }
UInt64 QAPI cGuid::getData64_0() const  { return m_Data64[0]; }
UInt64 QAPI cGuid::getData64_1() const  { return m_Data64[1]; }
void QAPI cGuid::getData64 (UInt64 * const data64_0, UInt64 * const data64_1) const
{
  *data64_0 = m_Data64[0];
  *data64_1 = m_Data64[1];
}
//{{{
// =======================================================================================
// === serialise =========================================================================
// =======================================================================================

String QAPI cGuid::serialise (const bool IncludeComment) const
{
  const unsigned int MaxGuidSize    = 39;
  const unsigned int MaxCommentSize = 255;

  Char Buffer[MaxGuidSize + MaxCommentSize + 10]; // Plus an extra 10 for good measure.

  wsprintf(
    Buffer,
    L"{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
    m_Data1,
    m_Data2,
    m_Data3,
    m_Data4[0],
    m_Data4[1],
    m_Data4[2],
    m_Data4[3],
    m_Data4[4],
    m_Data4[5],
    m_Data4[6],
    m_Data4[7]);

  if (IncludeComment)
  {
    if (m_Comment.size() > 0)
    {
      wcscat(Buffer, L" // ");
      wcsncat(Buffer, m_Comment.c_str(), MaxCommentSize);
    }
  }
  return Buffer;
}
//}}}
//{{{
// =======================================================================================
// === toString ==========================================================================
// =======================================================================================

String QAPI cGuid::toString (const bool IncludeComment) const
{
  return serialise (IncludeComment);
}
//}}}
// =======================================================================================
// === privates ==========================================================================
// =======================================================================================
//{{{

namespace
{

bool _isHex(const Char c)
{
  return (c >= L'0' && c <= L'9') || (c >= L'a' && c <= L'f') || (c >= L'A' && c <= L'F');
}

unsigned int _hexToDec(const Char c) throw(cBadFormat)
{
  if (c >= L'0' && c <= L'9')
  {
    return c - L'0';
  }
  else if (c >= L'a' && c <= L'f')
  {
    return 10 + (c - L'a');
  }
  else if (c >= L'A' && c <= L'F')
  {
    return 10 + (c - L'A');
  }
  else
  {
    throw cBadFormat(__FILE__, __LINE__, L"Expected hex char (\"%c\")", c);
  }
}

bool _isEnd(const Char c)
{
  return c == L'\0';
}

bool _isWhite(const Char c)
{
  return c == L' ' || c == L'\t';
}

const Char * _nextHex(const Char * p) throw(cNotFound)
{
  for (; !_isHex(*p); p++)
  {
    if (_isEnd(*p))
      throw cNotFound(__FILE__, __LINE__, L"End reach before hex digit found");
  }
  return p;
}

bool /* is found? */ _nextFwdSlash(const Char * p, const Char ** const rp /* result p */)
{
  bool IsFound = false;
  Char c = *p;
  while (!_isEnd(c))
  {
    if (c == L'/') { IsFound = true; break; }
    p++;
    c = *p;
  }
  *rp = p;
  return IsFound;
}

const Char * _skipWhite(const Char * p)
{
  while (_isWhite(*p))
  {
    p++;
  }
  return p;
}

const Char * _getHex16(const Char * p, UInt16 * const Result)
{
  *Result = 0;
  Char c = *p;
  unsigned int i = 0;
  while (_isHex(c))
  {
    *Result *= 16;
    *Result = *Result + (UInt16)_hexToDec(c);
    p++;
    c = *p;
    ++i;
    if (i == 4) break; // 4 chars is enough for a 32 bit value
  }
  return p;
}

const Char * _getHex32(const Char * p, UInt32 * const Result)
{
  *Result = 0;
  Char c = *p;
  unsigned int i = 0;
  while (_isHex(c))
  {
    *Result *= 16;
    *Result += _hexToDec(c);
    p++;
    c = *p;
    ++i;
    if (i == 8) break; // 8 chars is enough for a 32 bit value
  }
  return p;
}

}

void cGuid::_deserialise(const Char * const Serialised) throw(cBadFormat)
{
#if 1
    // This should be faster than swscanf method

    try
    {
      // Format = L"{%8x-%4hx-%4hx-%4hx-%4hx%4hx%4hx} // %255[^\n]"
      const Char * p = Serialised;
      p = _nextHex(p);
      p = _getHex32(p, &m_Data1);
      if (*p++ != '-') // Skip '-'
        throw cBadFormat(__FILE__, __LINE__, L"Expected '-' separator");
      p = _getHex16(p, &m_Data2);
      if (*p++ != '-') // Skip '-'
        throw cBadFormat(__FILE__, __LINE__, L"Expected '-' separator");
      p = _getHex16(p, &m_Data3);
      if (*p++ != '-') // Skip '-'
        throw cBadFormat(__FILE__, __LINE__, L"Expected '-' separator");
      p = _getHex16(p, (UInt16 *)&m_Data4[0]);
      if (*p++ != '-') // Skip '-'
        throw cBadFormat(__FILE__, __LINE__, L"Expected '-' separator");
      p = _getHex16(p, (UInt16 *)&m_Data4[2]);
      p = _getHex16(p, (UInt16 *)&m_Data4[4]);
      p = _getHex16(p, (UInt16 *)&m_Data4[6]);
      if (_nextFwdSlash(p, &p)) // Has comment?
      {
        p++; // Skip '/'
        p++; // Skip '/'
        p = _skipWhite(p);
        m_Comment = p;
      }
      else
      {
        m_Comment.clear();
      }
    }
    catch (cRecoverable & Reason)
    {
      throw cBadFormat(__FILE__, __LINE__, L"Tried to construct a cGuid from string \"%s\" (%s)", Serialised, Reason.getMsg().c_str());
    }
/*
    {
      cGuid tmp;
    
      Char Comment[256] = L"";

      String CopyOfString(Serialised); // Deep copy because swscanf will temporarily modify the src string!

      const unsigned int nNumFieldsConverted = swscanf(
        CopyOfString.c_str(),
        L"{%8x-%4hx-%4hx-%4hx-%4hx%4hx%4hx} // %255[^\n]",
        &tmp.m_Data1, &tmp.m_Data2, &tmp.m_Data3, &tmp.m_Data4[0], &tmp.m_Data4[2], &tmp.m_Data4[4], &tmp.m_Data4[6], Comment);
      if (nNumFieldsConverted < 7)
      {
        throw cBadFormat(__FILE__, __LINE__, L"Tried to construct a cGuid from string \"%s\"", m_Comment.c_str());
      }

      tmp.m_Comment = Comment;
      
      assert(m_Comment == tmp.m_Comment);
      assert(m_Data64[0] == tmp.m_Data64[0]);
      assert(m_Data64[1] == tmp.m_Data64[1]);
    }
*/
#else
    Char Comment[256] = L"";

    String CopyOfString(m_Comment.c_str()); // Deep copy because swscanf will temporarily modify the src string!

    const unsigned int nNumFieldsConverted = swscanf(
      CopyOfString.c_str(),
      L"{%8x-%4hx-%4hx-%4hx-%4hx%4hx%4hx} // %255[^\n]",
      &m_Data1, &m_Data2, &m_Data3, &m_Data4[0], &m_Data4[2], &m_Data4[4], &m_Data4[6], Comment);
    if (nNumFieldsConverted < 7)
    {
      throw cBadFormat(__FILE__, __LINE__, L"Tried to construct a cGuid from string \"%s\"", m_Comment.c_str());
    }

    m_Comment = Comment;
    
//    QKERNELBASEVERIFY(m_Comment.size() <= 55); // SJSJSJ Want to replace String with Char[31] plus 1 byte for size. Want to know if this will be big enough.
#endif

    // Swap 'endianness' of words for intel processors.
#if defined(_M_IX86) || defined(_M_X64)
    Swap<UInt8>(&m_Data4[0], &m_Data4[1]);
    Swap<UInt8>(&m_Data4[2], &m_Data4[3]);
    Swap<UInt8>(&m_Data4[4], &m_Data4[5]);
    Swap<UInt8>(&m_Data4[6], &m_Data4[7]);
#endif
}
//}}}
