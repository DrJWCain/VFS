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

// MsgPump.cpp
//{{{  includes, using namespace
// 
#include "stdafx.h"

#include <QWinBase.h>

using namespace vfs;
//}}}

//{{{
cWinKeybdAccelerator::Ptr vfs::createKeybdAccelerator() /* throw cWin32Error */
{
  return new cWinKeybdAccelerator;
}
//}}}

//{{{
cWinAccelGuard::Ptr cWinKeybdAccelerator::getAccelHandle() const
{
  cLockGuard threadSafety(&m_ThreadSafety);

  return m_AccelHandle;
}
//}}}
//{{{
void cWinKeybdAccelerator::setAccelerators (const std::vector<ACCEL> & newAccels) /* throw cWin32Error */
{
  cLockGuard threadSafety(&m_ThreadSafety);

  m_Accels = newAccels;
  m_AccelHandle = new cWinAccelGuard(CreateAcceleratorTable((ACCEL *)&m_Accels[0], (int)m_Accels.size()));

  if (m_AccelHandle->it == NULL)
  {
    const DWORD lastError = GetLastError();
    throw cWin32Error(__FILE__, __LINE__, L"Tried to CreateAcceleratorTable - returned %d (%s)", lastError, win32ErrorToString(lastError).c_str());
  }
}
//}}}

//{{{
bool/*is quit?*/ vfs::pumpCurrThreadWinMsgsUntilEventOrQuit (const HANDLE hEvent, const HWND hOptionalKeybdAccelWnd, const cWinKeybdAccelerator::Ptr & optionalKeybdAccel) /* throw cWin32Error */
{
  // wait for a window msg or a drawing msg
  const HANDLE drawingQueueEventHandle[1] = { hEvent };
  const DWORD Result = MsgWaitForMultipleObjectsEx (
    1,
    (HANDLE *)&drawingQueueEventHandle,
    INFINITE,
    QS_ALLINPUT | QS_ALLPOSTMESSAGE | QS_INPUT | QS_RAWINPUT,
    MWMO_INPUTAVAILABLE /* return immediately if already win32msgs in queue */);

  switch (Result)
  {
    case WAIT_OBJECT_0 + 0 : { break; }
    case WAIT_OBJECT_0 + 1 : { break; }
    case WAIT_OBJECT_0 + 2 : { break; }
    case WAIT_FAILED :       { throw cWin32Error(__FILE__, __LINE__, L"MsgWaitForMultipleObjectsEx() failed"); }
    default :                { /* wait abandoned? */ break; }
  }

  MSG msg;
  bool isQuit = false;
  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
  {
    if (msg.message == WM_QUIT)
    {
      isQuit = true;
      break;
    }

    bool isTranslateNecessary = true;
    if (optionalKeybdAccel.isValid())
    {
      if (optionalKeybdAccel->getAccelHandle().isValid())
      {
        isTranslateNecessary = !TranslateAccelerator (hOptionalKeybdAccelWnd, optionalKeybdAccel->getAccelHandle()->it, &msg);
      }
    }

    if (isTranslateNecessary)
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return isQuit;
}
//}}}
