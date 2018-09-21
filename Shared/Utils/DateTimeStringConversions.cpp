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

/////////////////////////////////////////////////////////////////////
// Useful utilities for various projects to share
/////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "DateTimeStringConversions.h"

#include <time.h>
#include <atlcomtime.h>

using namespace vfs;

static const COleDateTime g_NineteenSeventy (1970, 1, 1, 0, 0, 0);

//{{{ // this routine will take a string containing a longlong and populate a datetime object
void numericStringToDateTime (const vfs::String& strMillisecondTicsSince1970, vfs::cDateTime& dt)
{
  __int64 _n = 0;
  int fieldsConverted = swscanf(strMillisecondTicsSince1970.c_str(),L"%I64d",&_n);
  if (fieldsConverted != 1)
    return;

  if (_n < 0) //protect against -ve strings
    return;

  COleDateTimeSpan secs (0, 0, 0, (_n / 1000));
  COleDateTime date = g_NineteenSeventy + secs;

  // now populate the result
  cDateTime tempDT (date.GetYear (), date.GetMonth (), date.GetDay (), 
                    date.GetHour (), date.GetMinute (), date.GetSecond (), 0);
  dt = tempDT;
}
//}}}

//{{{ // this routine will take a fully formatted date/time string and populate a string with a 'long long'
void XMLDateTimeStringToNumericString (const vfs::String& strPrettyDateTime, vfs::String& strMillisecondTicsSince1970)
{
  strMillisecondTicsSince1970 = strPrettyDateTime;
  COleDateTime oledt;
  int year=0,month=0,day=0,hour=0,mins=0,secs=0;
  try
  {
    // Parse the pretty date/time - we are expecting an 'XML' style string : CCYY-MM-DDThh:mm:ss
    //                                         Y   M   D   H   M   S
    if(swscanf(strPrettyDateTime.c_str (), L"%4d-%2d-%2dT%2d:%2d:%2d", &year, &month, &day, &hour, &mins, &secs) != 6)
    {
      // problem parsing original string
      QSOS((L"XMLDateTimeStringToNumericString () - Error during string conversion."));
      return;
    }

    // find the difference between the pretty date and 1970
    oledt.SetDateTime (year, month, day, hour, mins, secs);
    COleDateTimeSpan Difference = oledt - g_NineteenSeventy;
    unsigned __int64 Milliseconds = (unsigned __int64) Difference.GetTotalSeconds () * 1000;

    // and format the string
    strMillisecondTicsSince1970 = lexical_cast<vfs::String, unsigned long long>(Milliseconds);

///////////// TEST ///////////////
#if(_DEBUG)

    vfs::String testMSstr (strMillisecondTicsSince1970);

    vfs::cDateTime testDT;
    numericStringToDateTime (testMSstr, testDT);
    dateTimeToXMLString (testDT, testMSstr);

    QVERIFY((testMSstr.compare (strPrettyDateTime) == 0));

#endif // TEST

  }
  catch (...)
  {
    // problem parsing original string
    QSOS((L"XMLDateTimeStringToNumericString () - Error during string conversion."));
  }
}
//}}}

//{{{ // this routine will take a datetime and produce a pretty string
void dateTimeToXMLString (const vfs::cDateTime& dt, vfs::String& rPrettyStr)
{
  // produce an 'XML' style string in the form of : "CCYY-MM-DDThh:mm:ss"
  COleDateTime formattedDT (dt.year (), dt.month (), dt.day (), dt.hour (), dt.minute (), dt.second ());
  rPrettyStr = formattedDT.Format (L"%Y-%m-%dT%H:%M:%S").GetBuffer ();
}
//}}}

//{{{ // this routine will take a datetime and produce a pretty string
void dateTimeToJSONString (const vfs::cDateTime& dt, vfs::String& rPrettyStr)
{
  // produce an 'XML' style string in the form of : "CCYY-MM-DDThh:mm:ss"
  COleDateTime formattedDT (dt.year (), dt.month (), dt.day (), dt.hour (), dt.minute (), dt.second ());
  rPrettyStr = formattedDT.Format (L"%Y-%m-%dT%H:%M:%S").GetBuffer ();
  StringStream extra;
  extra << L"." << std::setw(3) << std::setfill(L'0') << dt.millisec() << L"Z";
  rPrettyStr += extra.str();
}
//}}}