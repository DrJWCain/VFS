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

/////////////////////////////////////////////////////////////////////////////
// cDate.cpp

/*
 *  
 */

#include "stdafx.h"

#include "QKernelBase.h"

#include <time.h>
#include <sys/timeb.h>

using namespace vfs;

cDate::cDate () 
: m_Year (0)
, m_Month (1)
, m_Day (1)
{
}

cDate::cDate (const unsigned int Year, const unsigned int Month
  , const unsigned int Day)
{
  m_Year = (unsigned short) Year;

  m_Month = (unsigned char) Month;
  if (m_Month < 1) 
    m_Month = 1;
  else if (m_Month > 12) 
    m_Month = 12;

  // should do a check against months rather than generic 31 - YUK
  m_Day = (unsigned char) Day;
  if (m_Day < 1) 
    m_Day = 1;
  else if (m_Day > 31) 
    m_Day = 31;
}

cDate::cDate (const cDate & Other) 
: m_Year (Other.m_Year)
, m_Month (Other.m_Month)
, m_Day (Other.m_Day)
{
}

cDate::~cDate ()
{
}

unsigned int QAPI cDate::getYear (void) const
{
  return m_Year;
}

unsigned int QAPI cDate::getMonth (void) const
{
  return m_Month;
}

unsigned int QAPI cDate::getDay (void) const
{
  return m_Day;
}

String QAPI cDate::serialise (void) const
{
  return toString();
}

String QAPI cDate::toString (void) const
{
  // SJSJSJ Localise this eventually.

  Char Buffer[32] = L"";
  (void)_snwprintf(Buffer, sizeof(Buffer), L"%02u/%02u/%04u"
    , (unsigned long)m_Day, (unsigned long)m_Month, (unsigned long)m_Year);
  
  return Buffer;
}

bool QAPI cDate::operator== (const cDate& Other) const
{
  return m_Day == Other.m_Day
    && m_Month == Other.m_Month
    && m_Year == Other.m_Year;
}

bool QAPI cDate::operator!= (const cDate& Other) const
{
  return !(*this == Other);
}

bool QAPI cDate::operator< (const cDate& Other) const
{
  if (m_Year == Other.m_Year)
  {
    if (m_Month == Other.m_Month)
      return m_Day < Other.m_Day;

    return m_Month < Other.m_Month;
  }

  return m_Year < Other.m_Year;
}

bool QAPI cDate::operator> (const cDate& Other) const
{
  return (Other < *this);
}

cDate& QAPI cDate::operator= (const cDate& Other)
{
  if (this != &Other)
  {
    m_Year  = Other.m_Year;
    m_Month = Other.m_Month;
    m_Day   = Other.m_Day;
  }

  return *this;
}
