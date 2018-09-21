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
// cTime.cpp

/*
 *  
 */

#include "stdafx.h"

#include "QKernelBase.h"

#include <time.h>
#include <sys/timeb.h>

using namespace vfs;

cTime::cTime () 
: m_Hour (0)
, m_Minute (0)
, m_Second (0)
, m_Millisec (0)
{
}

cTime::cTime (const int Hour, const int Minute, const int Second, const int Millisec)
{
  // limit using MOD
  m_Hour = Hour % 24;
  m_Minute = (char)(Minute % 60);
  m_Second = (char)(Second % 60);
  m_Millisec = (short)(Millisec % 1000);
}

cTime::cTime (const cTime& Other) 
{
  *this = Other;
}

cTime::~cTime ()
{
}

String QAPI cTime::serialise (void) const
{
  return toString ();
}

String QAPI cTime::toString (void) const
{
  // SJSJSJ Localise this eventually.
  Char Buffer[48] = L"";
  (void)_snwprintf(Buffer, sizeof(Buffer), L"%02d:%02d:%02d.%03d"
    , m_Hour, m_Minute, m_Second, m_Millisec);
  
  return Buffer;
}

cTime& QAPI cTime::operator= (const cTime& Other)
{
  if (this != &Other)
  {
    m_Hour = Other.m_Hour;
    m_Minute = Other.m_Minute;
    m_Second = Other.m_Second;
    m_Millisec = Other.m_Millisec;
  }

  return *this;
}

bool QAPI cTime::operator== (const cTime& Other) const
{
  return m_Hour == Other.m_Hour 
    && m_Minute == Other.m_Minute 
    && m_Second == Other.m_Second 
    && m_Millisec == Other.m_Millisec;
}

bool QAPI cTime::operator!= (const cTime& Other) const
{
  return !(*this == Other);
}

bool QAPI cTime::operator< (const cTime& Other) const
{
  if (m_Hour == Other.m_Hour)
  {
    if (m_Minute == Other.m_Minute)
    {
      if (m_Second == Other.m_Second)
        return m_Millisec < Other.m_Millisec;

      return m_Second < Other.m_Second;
    }

    return m_Minute < Other.m_Minute;
  }

  return m_Hour < Other.m_Hour;
}

bool QAPI cTime::operator> (const cTime& Other) const
{
  return (Other < *this);
}

int QAPI cTime::getHour (void) const
{
  return m_Hour;
}

int QAPI cTime::getMinute (void) const
{
  return m_Minute;
}

int QAPI cTime::getSecond (void) const
{
  return m_Second;
}

int QAPI cTime::getMillisec (void) const
{
  return m_Millisec;
}

__int64 QAPI cTime::getDurationInMinutes (void) const
{
  __int64 Sum (m_Minute);
  Sum += m_Hour * 60;

  return Sum;
}

__int64 QAPI cTime::getDurationInSeconds (void) const
{
  __int64 Sum (m_Second);
  Sum += m_Minute * 60;
  Sum += m_Hour * 60 * 60;

  return Sum;
}

__int64 QAPI cTime::getDurationInMillisecs (void) const
{
  __int64 Sum (m_Millisec);
  Sum += m_Second * 1000;
  Sum += m_Minute * 1000 * 60;
  Sum += m_Hour * 1000 * 60 * 60;
  
  return Sum;
}

void QAPI cTime::setDurationFromMinutes (const __int64 NumMinutes)
{
  __int64 Count (NumMinutes);

  m_Hour = (unsigned long)(Count / (60));
  Count -= m_Hour * (60);

  m_Minute = (unsigned char)Count;

  m_Second = 0;

  m_Millisec = 0;
}

void QAPI cTime::setDurationFromSeconds (const __int64 NumSeconds)
{
  __int64 Count (NumSeconds);

  m_Hour = (unsigned char)(Count / (60 * 60));
  Count -= m_Hour * (60 * 60);

  m_Minute = (unsigned char)(Count / (60));
  Count -= m_Minute * (60);

  m_Second = (unsigned char)Count;

  m_Millisec = 0;
}

void QAPI cTime::setDurationFromMillisecs (const __int64 NumMillisecs)
{
  __int64 Count (NumMillisecs);

  m_Hour = (unsigned char)(Count / (1000 * 60 * 60));
  Count -= m_Hour * (1000 * 60 * 60);

  m_Minute = (unsigned char)(Count / (1000 * 60));
  Count -= m_Minute * (1000 * 60);

  m_Second = (unsigned char)(Count / (1000));
  Count -= m_Second * (1000);

  m_Millisec = (unsigned short)Count;
}

void QAPI cTime::operator-= (const cTime & Other)
{
  setDurationFromMillisecs (getDurationInMillisecs () - Other.getDurationInMillisecs ());
}

cTime QAPI cTime::operator- (const cTime & Other) const
{
  cTime Result (*this);
  Result -= Other;
  return Result;
}
