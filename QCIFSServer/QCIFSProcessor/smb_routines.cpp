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

//smb_routines.cpp

#include "StdAfx.h"

#if CIFS 

#include ".\smb_routines.h"

void SMBTimeToFiletime(const SMB_DATE& smbDate, const SMB_TIME& smbTime, LPFILETIME pFileTime)
{
  SYSTEMTIME sysTime;
  sysTime.wYear = smbDate.Struct.Year;
  sysTime.wMonth = smbDate.Struct.Month;
  sysTime.wDay = smbDate.Struct.Day;
  sysTime.wHour = smbTime.Struct.Hours;
  sysTime.wMinute = smbTime.Struct.Minutes;
  sysTime.wSecond = smbTime.Struct.TwoSeconds * 2;
  SystemTimeToFileTime(&sysTime, pFileTime);
}

void FiletimeToSMBTime(const LPFILETIME pFiletime, PSMB_DATE pSMBDate, PSMB_TIME pSMBTime)
{
  SYSTEMTIME sysTime;
  FileTimeToSystemTime(pFiletime, &sysTime);
  pSMBDate->Struct.Day = sysTime.wDay;
  pSMBDate->Struct.Month = sysTime.wMonth;
  pSMBDate->Struct.Year = sysTime.wYear;
  pSMBTime->Struct.Hours = sysTime.wHour;
  pSMBTime->Struct.Minutes = sysTime.wMinute;
  pSMBTime->Struct.TwoSeconds = sysTime.wSecond/2;
}

#endif // CIFS 