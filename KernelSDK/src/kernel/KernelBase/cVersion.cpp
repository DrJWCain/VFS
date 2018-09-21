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
#include "fatal.h"

using namespace vfs;




// =======================================================================================
// === Construction ======================================================================
// =======================================================================================

cVersion::cVersion()
{
  m_Release = 0;
  m_MajorVersion = 0;
  m_MinorVersion = 0;
  m_Build = 0;
}

// ---------------------------------------------------------------------------------------

cVersion::cVersion(
  const unsigned int  nRelease,
  const unsigned int  nMajorVersion,
  const unsigned int  nMinorVersion,
  const unsigned int  nBuild)
{
  m_Release = nRelease;
  m_MajorVersion = nMajorVersion;
  m_MinorVersion = nMinorVersion;
  m_Build = nBuild;
}

// ---------------------------------------------------------------------------------------

cVersion::cVersion(const String & Serialised) throw(cBadFormat)
{
  String CopyOfInput(Serialised.c_str()  /* Thread safe deep copy */);

  /*
  WARNING! swscanf will temporarily modify the src string!
  */
  unsigned int nNumFieldsConverted = swscanf(
    CopyOfInput.c_str(),
    L"%d.%d.%d.%d",
    &m_Release,
    &m_MajorVersion,
    &m_MinorVersion,
    &m_Build);   

  if (nNumFieldsConverted != 4)
  {
    nNumFieldsConverted = swscanf(
      CopyOfInput.c_str(),
      L"%d,%d,%d,%d",
      &m_Release,
      &m_MajorVersion,
      &m_MinorVersion,
      &m_Build);
    if (nNumFieldsConverted != 4) throw cBadFormat(__FILE__, __LINE__, L"Unable to parse \"%s\" in cVersion", Serialised.c_str());
  }
}

// ---------------------------------------------------------------------------------------

cVersion::cVersion(const Char * const Serialised) throw(cBadFormat)
{
  QKERNELBASEVERIFY(Serialised);

  String CopyOfInput(Serialised);

  /*
  WARNING! swscanf will temporarily modify the src string!
  */
  unsigned int nNumFieldsConverted = swscanf(
    CopyOfInput.c_str(),
    L"%d.%d.%d.%d",
    &m_Release,
    &m_MajorVersion,
    &m_MinorVersion,
    &m_Build);   

  if (nNumFieldsConverted != 4)
  {
    nNumFieldsConverted = swscanf(
      CopyOfInput.c_str(),
      L"%d,%d,%d,%d",
      &m_Release,
      &m_MajorVersion,
      &m_MinorVersion,
      &m_Build);
    if (nNumFieldsConverted != 4) throw cBadFormat(__FILE__, __LINE__, L"Unable to parse \"%s\" in cVersion", Serialised);
  }
}




// =======================================================================================
// === cVersion Operators ================================================================
// =======================================================================================

bool QAPI cVersion::operator!=(const cVersion & Other) const
{
  return !(*this == Other);
}

// ---------------------------------------------------------------------------------------

bool QAPI cVersion::operator==(const cVersion & Other) const
{
  return m_Release == Other.m_Release &&
       m_MajorVersion == Other.m_MajorVersion &&
       m_MinorVersion == Other.m_MinorVersion &&
       m_Build == Other.m_Build;
}

// ---------------------------------------------------------------------------------------

bool QAPI cVersion::operator<(const cVersion & Other) const
{
  if (m_Release == Other.m_Release)
  {
    if (m_MajorVersion == Other.m_MajorVersion)
    {
      if (m_MinorVersion == Other.m_MinorVersion)
      {
        return m_Build < Other.m_Build;
      }
      return m_MinorVersion < Other.m_MinorVersion;
    }
    return m_MajorVersion < Other.m_MajorVersion;
  }
  return m_Release < Other.m_Release;
}

// ---------------------------------------------------------------------------------------

bool QAPI cVersion::operator>(const cVersion & Other) const
{
  if (m_Release == Other.m_Release)
  {
    if (m_MajorVersion == Other.m_MajorVersion)
    {
      if (m_MinorVersion == Other.m_MinorVersion)
      {
        return m_Build > Other.m_Build;
      }
      return m_MinorVersion > Other.m_MinorVersion;
    }
    return m_MajorVersion > Other.m_MajorVersion;
  }
  return m_Release > Other.m_Release;
}




// =======================================================================================
// === serialise =========================================================================
// =======================================================================================

String QAPI cVersion::serialise(void) const
{
  Char Buffer[80] = L"";
  (void)wsprintf(Buffer, L"%d.%d.%d.%d", m_Release, m_MajorVersion, m_MinorVersion, m_Build);
  return Buffer;
}




// =======================================================================================
// === toString ==========================================================================
// =======================================================================================

String QAPI cVersion::toString(void) const
{
  return serialise();
}
