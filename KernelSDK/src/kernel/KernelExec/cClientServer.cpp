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

// --- Pre-Compiled Headers --------------------------------------------------------------

#include "stdafx.h"

// --- Headers ---------------------------------------------------------------------------

// Only include our own header - all others are inside there.
#include "cClientServer.h"
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <sstream>

// --- Namespace -------------------------------------------------------------------------

using namespace std;
using namespace vfs;




// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

cServer::cServer(const String & Name) throw(String) :
  cClientServer(Name)
{
}

// ---------------------------------------------------------------------------------------

cClient::cClient(const String & Name) throw(String) :
  cClientServer(Name)
{
}

// ---------------------------------------------------------------------------------------

cClientServer::cClientServer(const String & Name) throw(String) :
  m_SignalServerSent(false /* initially set? */, Name + L"_SignalServerSent"),
  m_SignalClientReceived(false /* initially set? */, Name + L"_SignalClientReceived")
{
  const String FileMappingName(Name + L"_FileMapping");

  SECURITY_ATTRIBUTES sa;
  SECURITY_DESCRIPTOR sd;

  sa.nLength = sizeof(SECURITY_ATTRIBUTES);
  sa.bInheritHandle = TRUE;
  sa.lpSecurityDescriptor = &sd;

  if(!InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION)) 
  {
    StringStream Error;
    Error << L"Unable to InitializeSecurityDescriptor, err == " << GetLastError();
    throw Error.str();
  }

  if(!SetSecurityDescriptorDacl(&sd, TRUE, (PACL)NULL, FALSE)) 
  {
    StringStream Error;
    Error << L"Unable to SetSecurityDescriptorDacl, err == " << GetLastError();
    throw Error.str();
  }

  m_SharedFile = new cWin32HandleGuard(CreateFileMapping(INVALID_HANDLE_VALUE, &sa, PAGE_READWRITE, 0, getBufferSize(), FileMappingName.c_str()));
  if (m_SharedFile->it == NULL) 
  {
    StringStream Error;
    Error << L"Unable to create file mapping object, err == " << GetLastError();
    throw Error.str();
  }

  m_SharedBuffer = new cWin32MappedFileGuard((char *)MapViewOfFile(m_SharedFile->it, FILE_MAP_WRITE, 0, 0, 0));
  if (m_SharedBuffer->it == NULL)
  {
    StringStream Error;
    Error << L"Unable to map shared memory, err == " << GetLastError();
    throw Error.str();
  }
}




// =======================================================================================
// === cClientServer =====================================================================
// =======================================================================================

char * cClientServer::getBuffer() const
{
  return m_SharedBuffer->it;
}

// ---------------------------------------------------------------------------------------

unsigned int cClientServer::getBufferSize() const
{
  return 4096 * 16;
}




// =======================================================================================
// === cServer ===========================================================================
// =======================================================================================

bool cServer::sendStringWait(const String & StringToSend, const unsigned long  TimeoutMillisecs)
{
  QVERIFY(StringToSend.size() * sizeof(Char) < getBufferSize());

  cLockGuard ThreadSafety(&m_ThreadSafety);
  m_SignalClientReceived.reset();
  wcscpy((WCHAR *)getBuffer(), StringToSend.c_str());
  m_SignalServerSent.set();
  return m_SignalClientReceived.wait(TimeoutMillisecs) == cManualResetEvent::eSignalled;
}

// ---------------------------------------------------------------------------------------

void cServer::sendStringWaitForever(const String & StringToSend)
{
  (void)sendStringWait(StringToSend, INFINITE);
}


// ---------------------------------------------------------------------------------------

void cServer::spike()
{
  m_SignalClientReceived.set();
}




// =======================================================================================
// === cClient ===========================================================================
// =======================================================================================

String cClient::getStringWaitForever()
{
  String Result;
  (void)getStringWait(INFINITE, &Result);
  return Result;
}

// ---------------------------------------------------------------------------------------

bool cClient::getStringWait(const unsigned long TimeoutMillisecs, String * const Result)
{
  cLockGuard ThreadSafety(&m_ThreadSafety);
  bool IsSucceeded = false;
  *Result = L"";
  IsSucceeded = m_SignalServerSent.wait(TimeoutMillisecs) == cAutoResetEvent::eSignalled;
  if (IsSucceeded) *Result = (Char *)getBuffer();
  m_SignalClientReceived.set();
  return IsSucceeded;
}

// ---------------------------------------------------------------------------------------

void cClient::spike()
{
  m_SignalServerSent.set();
}
