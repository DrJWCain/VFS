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


#include "free445.h"
#include <windows.h>
#include <shlobj.h>
#include <Lm.h>
#include <string>



#pragma comment(lib, "NetApi32.lib")

typedef std::wstring String;

// Function to disconnect a specified transport protocol from the host
// - see MSDN NetServerTransportEnum function
//
bool disconnectTransport(const String &transport)
{
  LPSERVER_TRANSPORT_INFO_0 pBuf = NULL;
  LPSERVER_TRANSPORT_INFO_0 pTmpBuf;
  DWORD dwLevel = 0;
  DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
  DWORD dwEntriesRead = 0;
  DWORD dwTotalEntries = 0;
  DWORD dwResumeHandle = 0;
  DWORD dwTotalCount = 0;
  NET_API_STATUS nStatus;
  LPTSTR pszServerName = NULL;
  DWORD i;
  bool found = false;

  do
  {
    // Call the NetServerTransportEnum function; specify level 0.
    nStatus = NetServerTransportEnum(pszServerName,
      dwLevel,
      (LPBYTE *)&pBuf,
      dwPrefMaxLen,
      &dwEntriesRead,
      &dwTotalEntries,
      &dwResumeHandle);

    if((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA))
    {
      if((pTmpBuf = pBuf) != NULL)
      {
        // Loop through the entries; process access errors
        for(i = 0; i < dwEntriesRead; i++)
        {
          if(pTmpBuf == NULL)
          {
            wprintf(L"An access violation has occurred\n");
            break;
          }

          // Print the transport protocol name 
          wprintf(L"\tFound Transport: %s\n", pTmpBuf->svti0_transportname);

          // Delete it if it's the one we want
          if(0 == wcscmp(pTmpBuf->svti0_transportname, transport.c_str()))
          {
            NET_API_STATUS status = NetServerTransportDel(pszServerName, dwLevel, (LPBYTE)pTmpBuf);
            if(NERR_Success == status)
            {
              wprintf(L"\tDeleted %s\n", transport.c_str());
              found = true;
            }
            else wprintf(L"\tError %d while deleting %s\n", status, transport.c_str());
          }

          pTmpBuf++;
          dwTotalCount++;
        }
      }
    }
    else wprintf(L"A system error has occurred: %d\n", nStatus);

    // Free the allocated buffer
    if(pBuf != NULL)
    {
      NetApiBufferFree(pBuf);
      pBuf = NULL;
    }
  } while(nStatus == ERROR_MORE_DATA); // Continue to call NetServerTransportEnum while there are more entries

                                       // Check again for an allocated buffer.
  if(pBuf != NULL) NetApiBufferFree(pBuf);

  wprintf(L"Total of %d entries enumerated\n", dwTotalCount);

  return found;
}

void free445()
{
  const String kNetManEvent(L"Global\\NetManCompleteEvent");
  HANDLE event = CreateEvent(NULL, TRUE, FALSE, kNetManEvent.c_str());

  const String kNetBiosSmb(L"\\Device\\NetbiosSmb");
  disconnectTransport(kNetBiosSmb);

  if(event)
  {
    SetEvent(event);
  }
}
