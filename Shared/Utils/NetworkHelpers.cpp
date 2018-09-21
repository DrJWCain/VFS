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

#include "stdafx.h"

#include "NetworkHelpers.h"

#include <iphlpapi.h> // for GetIpAddrTable


// function to obtain the (first adapter's) network address.
// populates the string parameter in the form : "127.0.0.1"
bool getFirstAdapterAddress(vfs::String& rAddress)
{
  bool bRet = false;

  // figure out the buffersize we need to allocate to receive the data
  unsigned long iSize (0);
  DWORD res = GetIpAddrTable (NULL, &iSize, FALSE);
  QVERIFY (res == ERROR_INSUFFICIENT_BUFFER);
  QVERIFY (iSize > 0);

  // now allocate space...
  MIB_IPADDRTABLE* pTable = (MIB_IPADDRTABLE*) calloc (1, iSize);
  if (pTable)
  {
    // go get the data and use the first entry in the table 
    // (there may be multiple adapters on this machine - this function will only return the first one it finds)
    if ( (GetIpAddrTable (pTable, &iSize, FALSE) == NO_ERROR) 
      && (pTable->dwNumEntries >= 1) )
    {
      // print out the IP4 bytes in a nice pretty manner. default to port 21.
      unsigned char* pBytes = (unsigned char*)&(pTable->table[0].dwAddr);
      wchar_t strAddress[64];
      swprintf (strAddress, L"%d.%d.%d.%d", pBytes[0], pBytes[1], pBytes[2], pBytes[3]); 

      rAddress = strAddress;
      bRet = true;
    }

    free (pTable);
  }

  return bRet;
}

