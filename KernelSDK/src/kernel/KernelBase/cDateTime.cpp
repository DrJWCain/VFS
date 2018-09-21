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
// cDateTime.cpp

#include "stdafx.h"

#include "QKernelBase.h"

using namespace vfs;

cDateTime::cDateTime ()      
{ 
}

cDateTime::cDateTime (const unsigned int year, const unsigned int month, const unsigned int day
  , const int hour, const int minute, const int second, const int millisec)
: Date (year, month, day)
, Time (hour, minute, second, millisec)
{ 
}

cDateTime::cDateTime (const cDate& date, const cTime& time)
: Date (date)
, Time (time)
{ 
}

cDateTime::cDateTime (const cDateTime& other)
: Date (other.Date)
, Time (other.Time)
{ 
}

cTime cDateTime::time (void) const
{
  return Time;
}

cDate cDateTime::date (void) const
{
  return Date;
}

unsigned int QAPI cDateTime::year (void) const
{
  return Date.getYear ();
}

unsigned int QAPI cDateTime::month (void) const
{
  return Date.getMonth ();
}

unsigned int QAPI cDateTime::day (void) const
{
  return Date.getDay ();
}

int QAPI cDateTime::hour (void) const
{
  return Time.getHour ();
}

int QAPI cDateTime::minute (void) const
{
  return Time.getMinute ();
}

int QAPI cDateTime::second (void) const
{
  return Time.getSecond ();
}

int QAPI cDateTime::millisec (void) const
{
  return Time.getMillisec ();
}

String QAPI cDateTime::serialise (void) const
{
  return toString ();
}

String QAPI cDateTime::toString (void) const
{
  return Date.toString() + L" " + Time.toString();
}

bool QAPI cDateTime::operator== (const cDateTime& other) const
{
  return (Time == other.Time) && (Date == other.Date);
}

bool QAPI cDateTime::operator!= (const cDateTime& other) const
{
  return !(*this == other);
}

bool QAPI cDateTime::operator< (const cDateTime& other) const
{
  bool lessThan = false;

  if (Date < other.Date)
    lessThan = true;  // regardless of time setting
  else if ((Date == other.Date) && (Time < other.Time))
    lessThan = true;

  return lessThan;
}

bool QAPI cDateTime::operator> (const cDateTime& other) const
{
  return (other < *this);
}
