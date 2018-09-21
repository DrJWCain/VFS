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
#include "cConsoleLogDriver.h"
#include "Main.h"
#include "LogCategories.h"
#include "..\..\..\kernel\KernelExec\cModuleInstance.h"

using namespace vfs;
using namespace std;




// =======================================================================================
// === gSignalShowLog ====================================================================
// =======================================================================================

cAutoResetEvent gSignalShowLog(false /* initially set? */);


/*
class cTrayMenuItem : public cThread
{
  public :

    QDEFINE_SMARTPTR(cTrayMenuItem);

    cTrayMenuItem(
      const String &          Text,
      cConsole::Ptr           Console) throw(cNotAvailable) :
        m_Console(Console),
        cThread(L"Console log driver tray menu item", iBasicThread::ePriority_Normal)
    {
      HRESULT hr = CoInitialize(NULL);
      if (FAILED(hr)) throw cNotAvailable(__FILE__, __LINE__, L"Tried to CoInitialize(NULL) - returned %x", hr);

      hr = m_Item.CoCreateInstance(CLSID_TrayMenuItem);
      if (FAILED(hr)) throw cNotAvailable(__FILE__, __LINE__, L"Tried to CoCreateInstance(CLSID_TrayMenuItem) - returned %x", hr);

      hr = m_Callback.CoCreateInstance(CLSID_TrayMenuItemCallback);
      if (FAILED(hr)) throw cNotAvailable(__FILE__, __LINE__, L"Tried to CoCreateInstance(CLSID_TrayMenuItemCallback) - returned %x", hr);

      hr = m_Item->init(_bstr_t(Text.c_str()), m_Callback);
      if (FAILED(hr)) throw cNotAvailable(__FILE__, __LINE__, L"Tried to init() TrayMenuItemCallback - returned %x", hr);

      m_Item->check(true);
    }

    void QAPI signalLoopStop()
    {
      // Can't stop!
    }

    void QAPI runLoop()
    {
      while (true)
      {
        gSignalShowLog.waitForever();

        BOOL CurrCheckedState;
        (void)m_Item->isChecked(&CurrCheckedState);

        const BOOL NewCheckedState = !CurrCheckedState;
        m_Item->check(NewCheckedState);

        if (NewCheckedState)
        {
          m_Console->show(false); // topmost?
        }
        else
        {
          m_Console->hide();
        }
      }
    }

  private :

    cConsole::Ptr                   m_Console;
    CComPtr<ITrayMenuItem>          m_Item;
    CComPtr<ITrayMenuItemCallback>  m_Callback;
};
*/



// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

class cRegistryWatch : public cThread
{
  public :
    cRegistryWatch(cConsole::Ptr Console) :
      cThread(L"Console log driver visibility", iBasicThread::ePriority_Normal),
      m_SignalVisibilityChanged(false /* initially signalled */),
      m_Running(true),
      m_Console(Console)
    {
    }
    void QAPI signalLoopStop()
    {
      m_Running = false;
      m_SignalVisibilityChanged.set();
    }
  private :
    cConsole::Ptr       m_Console;
    bool                m_Running;
    cAutoResetEvent     m_SignalVisibilityChanged;
    void QAPI runLoop()
    {
      while (m_Running)
      {
        const bool IsVisible = gModuleContext->getPublicSettingNumber(L"User Interface: Tweaks\\Log console visible 2") != 0;
        QMSG((kCtgVisibility, L"Console log window visibility changed (is visible = %d)", IsVisible));
        if (IsVisible)
        {
          const bool IsTopmost = gModuleContext->getPublicSettingNumber(L"User Interface: Tweaks\\Log console always on top") != 0;
          m_Console->show(IsTopmost);
        }
        else
        {
          m_Console->hide();
        }

        cGuard::ConstPtr RegWatch = createRegistryHelp()->watchKey(
          gModuleContext->getRegistryPublicSettingsPath() + L"User Interface: Tweaks\\Log console visible 2",
          &m_SignalVisibilityChanged);
        m_SignalVisibilityChanged.waitForever();
      }
    }
};

cConsoleLogDriver::cConsoleLogDriver() throw(cNotAvailable)
{
  try
  {
    (void)iSystemInfo::singleton().getCmdLineOption(L"install");
    throw cNotAvailable(__FILE__, __LINE__, L"Quiet during install");
  }
  catch (cNotFound &)
  {
  }

  m_NumColumns = gModuleContext->getCustomSettingNumber(L"", L"Num Columns");

  try
  {
    m_Console = new cConsole(
      gModuleContext->getCustomSettingNumber(L"", L"Is showing stdout") != 0,
      m_NumColumns,
      gModuleContext->getCustomSettingNumber(L"", L"Num Rows"));
  }
  catch (cRecoverable & reason)
  {
    throw cNotAvailable(__FILE__, __LINE__, L"Could not create console window (%s)", reason.getMsg().c_str());
  }

  m_Unwind.push_back(iThreadManager::singleton().startThread(new cRegistryWatch(m_Console)));

}

cConsoleLogDriver::~cConsoleLogDriver()
{
//  iThreadManager::singleton().stopThread(m_Unwind);
}




// =======================================================================================
// === _StringUppercase ==================================================================
// =======================================================================================

static void QAPI _StringUppercase(Char * pBuffer)
{

  QVERIFY(pBuffer != 0);


  while (*pBuffer)
  {
    *pBuffer = (Char)toupper(*pBuffer);
    pBuffer++;
  }
}




// =======================================================================================
// === Write =============================================================================
// =======================================================================================

#define ANSI_CR       0xd
#define ANSI_NEWLINE  0xa

void QAPI cConsoleLogDriver::write(
  const cConsole::WriteFlag Flags,
  const unsigned int        Indent,
  const String &            Msg)
{
  Char * WriteBuffer = (Char *)_alloca((Msg.size() + 100 /* Room for indent */) * sizeof(Char));
  Char * pNextBufferChar = WriteBuffer;

  WriteBuffer[0] = '\0';

  // Indent?
  if (Indent > 0 && Indent < 50)
  {
    unsigned int nIndentCount = Indent;
    while (nIndentCount--)
    {
      *pNextBufferChar++ = _L(' ');
      *pNextBufferChar++ = _L(' ');
    }
  }

  // Body
  const Char * pNextMsgChar = &Msg[0];
  if (Flags & LWF_STRIPCR)
  {
    // Don't copy carriage returns / newlines from message.
    Char NextMsgChar = *(pNextMsgChar++);
    while (NextMsgChar)
    {
      const bool IsACR = (NextMsgChar == ANSI_CR) || (NextMsgChar == ANSI_NEWLINE);
      if (!IsACR) *pNextBufferChar++ = NextMsgChar;
      NextMsgChar = *(pNextMsgChar++);
    }
  }
  else
  {
    // Copy everything including carriage returns / newlines from message.
    while (*pNextMsgChar) *pNextBufferChar++ = *(pNextMsgChar++);
  }

  // Append newline?
  if (Flags & LWF_APPENDNEWLINE)
  {
    *pNextBufferChar++ = ANSI_CR;
    *pNextBufferChar++ = ANSI_NEWLINE;
  }

  // Terminate string HERE before possible uppercase and debug output
  *pNextBufferChar = 0; // IMPORTANT: Don't autoinc

  // All uppercase?
  if (Flags & LWF_UPPERCASE) _StringUppercase(WriteBuffer);

  const unsigned int NumCharsToWrite = ((char *)pNextBufferChar - (char *)WriteBuffer) /  sizeof(Char);
  if (NumCharsToWrite > 0) m_Console->write(Flags, WriteBuffer);
}




// =======================================================================================
// === writeOutdent ======================================================================
// =======================================================================================

void QAPI cConsoleLogDriver::writeOutdent(iLogDriver::cMsg::ConstPtr Record)
{
  writeInject(Record);

  if (Record->m_ThreadName.substr(0, 4) == L"Krnl")
  {
    write(LWF_APPENDNEWLINE | LWF_BLUE | LWF_INTENSITY, 0, L" }");
  }
  else
  {
    write(LWF_APPENDNEWLINE | LWF_GREEN, 0, L" }");
  }
}




// =======================================================================================
// === writeInject =======================================================================
// =======================================================================================

void QAPI cConsoleLogDriver::writeInject(iLogDriver::cMsg::ConstPtr Record)
{
/*
  if (Record->m_NumSuppressedSinceLast > 0)
  {
    Char Tmp[16] = L" >>>";
    (void)_ltow(Record->m_NumSuppressedSinceLast, &Tmp[4], 10);
    (void)wcscat(Tmp, L"<<<");
    write(LWF_APPENDNEWLINE | LWF_BLUE, Record->m_NumIndents + 3, Tmp);
  }
*/
  const int Colour = Record->m_ThreadName.substr(0, 4) == L"Krnl" ? LWF_BLUE| LWF_INTENSITY : LWF_GREEN;

  Char Buffer[128];
  const cTime& localTime = Record->m_LocalTime;
  swprintf (Buffer, L"%02d:%02d:%02d.%03d ", localTime.getHour(), localTime.getMinute(), localTime.getSecond(), localTime.getMillisec());
  write(Colour, 0, Buffer);

  if (Record->m_NumInjectedIndents > 0)
  {
    Char Tmp[10];
    (void)_snwprintf(Tmp, sizeof(Tmp), L"%5d-> ", Record->m_ThreadID);
    write(Colour, 0, Tmp);
  }
  else
  {
    Char Tmp[10];
    (void)_snwprintf(Tmp, sizeof(Tmp), L"%5d   ", Record->m_ThreadID);
    write(Colour, 0, Tmp);
  }

  write(0, Record->m_NumIndents, L"");
}




// =======================================================================================
// === writePrefix =======================================================================
// =======================================================================================

void QAPI cConsoleLogDriver::writePrefix(iLogDriver::cMsg::ConstPtr Record, const Char ExpandButton)
{
  Char Buffer[128];

  Buffer[0] = ExpandButton;
  Buffer[1] = '\0';

  if (Record->m_ThreadName.substr(0, 4) == L"Krnl")
  {
    write(LWF_UPPERCASE | LWF_BLUE | LWF_INTENSITY, 0, Buffer);
    //Char * Dot = wcschr(Record->m_ModuleName.c_str(), L'.'); // Don't want to show file extension
    //if (Dot != NULL) *Dot = 0;                               // in module name - wastes space!
    const String::size_type DotPos = Record->m_ModuleName.find_first_of (L'.'); // Don't want to show file extension
    write(LWF_UPPERCASE | LWF_BLUE | LWF_INTENSITY, 0, Record->m_ModuleName.substr (0, DotPos));
  }
  else
  {
    write(LWF_UPPERCASE | LWF_GREEN, 0, Buffer);
    write(LWF_UPPERCASE | LWF_GREEN, 0, Record->m_ThreadName.c_str());
  }

  Buffer[0] = '(';
  Buffer[1] = '\0';

  if (wcslen(Buffer) > 1)
  {
    (void)wcscat(Buffer, L") ");
  }
  else
  {
    (void)wcscpy(Buffer, L" ");
  }

  write(LWF_STRIPCR | LWF_RED | LWF_GREEN, 0, Buffer);
}




// =======================================================================================
// === writeLazyIndent ===================================================================
// =======================================================================================

void QAPI cConsoleLogDriver::writeLazyIndent(iLogDriver::cMsg::ConstPtr Record) throw()
{
  writeInject(Record);

  const int Colour = Record->m_ThreadName.substr(0, 4) == L"Krnl" ? LWF_BLUE| LWF_INTENSITY : LWF_GREEN;
  write(LWF_APPENDNEWLINE | Colour, 0, L" {");
}




// =======================================================================================
// === writeExpandedIndent ===============================================================
// =======================================================================================

void QAPI cConsoleLogDriver::writeExpandedIndent(iLogDriver::cMsg::ConstPtr Record) throw()
{
  writeInject(Record);

  unsigned int Colour;
  if (Record->m_NumSOSIndents > 0)
  {
    Colour = LWF_RED | LWF_INTENSITY;
  }
  else
  {
    Colour = Record->m_IsHighlighted ? LWF_WHITE | LWF_INTENSITY : LWF_WHITE;
  }

  writePrefix(Record, L'-');
  write(LWF_STRIPCR | LWF_APPENDNEWLINE | Colour, 0, Record->m_Text);
}




// =======================================================================================
// === writeCollapsedIndent ==============================================================
// =======================================================================================

void QAPI cConsoleLogDriver::writeCollapsedIndent(iLogDriver::cMsg::ConstPtr Record) throw()
{
  writeInject(Record);

  unsigned int Colour;
  if (Record->m_NumSOSIndents > 0)
  {
    Colour = LWF_RED | LWF_INTENSITY;
  }
  else
  {
    Colour = Record->m_IsHighlighted ? LWF_WHITE | LWF_INTENSITY : LWF_WHITE;
  }

  writePrefix(Record, L'+');
  write(LWF_STRIPCR | Colour, 0, Record->m_Text);
  write(LWF_STRIPCR | LWF_APPENDNEWLINE | LWF_BLUE | LWF_INTENSITY, 0, L"...");
}




// =======================================================================================
// === writeMsg ==========================================================================
// =======================================================================================

void QAPI cConsoleLogDriver::writeTrace(iLogDriver::cMsg::ConstPtr Record) throw()
{
  writeInject(Record);
  writePrefix(Record, L' ');
  write(LWF_STRIPCR | LWF_APPENDNEWLINE | LWF_CYAN | LWF_INTENSITY, 0, Record->m_Text);
}




// =======================================================================================
// === writeBreak ========================================================================
// =======================================================================================

void QAPI cConsoleLogDriver::writeBreak(iLogDriver::cMsg::ConstPtr Record) throw()
{
  const unsigned int Style     = LWF_WHITE | LWF_BLUEBKGND | LWF_INTENSITY;// | LWF_INTENSITYBKGND;
  const unsigned int LineWidth = Min<unsigned int>(m_NumColumns, 120);

  write(LWF_APPENDNEWLINE, 0, L"");
  write(LWF_APPENDNEWLINE, 0, L"");
  write(LWF_APPENDNEWLINE, 0, L"");
  write(LWF_APPENDNEWLINE, 0, L"");

  unsigned int i;
  for (i = 0; i < LineWidth; ++i) write(Style, 0, L" ");
  write(LWF_APPENDNEWLINE, 0, L"");

  write(Style, 0, L"       ");
  write(LWF_STRIPCR | Style, 0, Record->m_Text);
  const unsigned j = LineWidth - 7 - Record->m_Text.size();
  for (i = 0; i < j; ++i) write(Style, 0, L" ");
  write(LWF_APPENDNEWLINE, 0, L"");

  for (i = 0; i < LineWidth; ++i) write(Style, 0, L" ");
  write(LWF_APPENDNEWLINE, 0, L"");

  write(LWF_APPENDNEWLINE, 0, L"");
}




// =======================================================================================
// === writeMsg ==========================================================================
// =======================================================================================

void QAPI cConsoleLogDriver::writeMsg(iLogDriver::cMsg::ConstPtr Record) throw()
{
  writeInject(Record);

  if (Record->m_Type == iLogDriver::cMsg::eErrorMsg) // Show console on errors
  {
    m_Console->show(gModuleContext->getPublicSettingNumber(L"User Interface: Tweaks\\Log console always on top") != 0);
  }

  unsigned int Colour;
  if (Record->m_Type == iLogDriver::cMsg::eErrorMsg)
  {
    Colour = LWF_REDBKGND | LWF_INTENSITYBKGND;
  }
  else
  {
    if (Record->m_NumSOSIndents > 0)
    {
      Colour = LWF_RED | LWF_INTENSITY;
    }
    else
    {
      Colour = Record->m_IsHighlighted ? LWF_WHITE | LWF_INTENSITY : LWF_WHITE;
    }
  }

  writePrefix(Record, L' ');
  write(LWF_STRIPCR | LWF_APPENDNEWLINE | Colour, 0, Record->m_Text);
}
