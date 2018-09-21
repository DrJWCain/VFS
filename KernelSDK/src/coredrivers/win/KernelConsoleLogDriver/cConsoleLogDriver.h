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

#pragma once
/*
**  
*/

#include <QKernelExec.h>
#include "cConsole.h"

using namespace vfs;
using namespace std;




// =======================================================================================
// === gSignalShowLog ====================================================================
// =======================================================================================

extern cAutoResetEvent gSignalShowLog;

/*
// =======================================================================================
// === cTrayMenuItemListener =============================================================
// =======================================================================================

DEFINE_GUID(CLSID_TrayMenuItemListener2,
	0xB2FF1F0E, 0x8B6C, 0x474A, 0xAB, 0x67, 0xF5, 0x32, 0x71, 0x4C, 0x98, 0xA6);

class ATL_NO_VTABLE cTrayMenuItemListener : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<cTrayMenuItemListener, &CLSID_TrayMenuItemListener>
{
  public:
	  cTrayMenuItemListener() :
      m_SignalSelected(0, 1)
    {
    }

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(cTrayMenuItemListener)
	    COM_INTERFACE_ENTRY(ITrayMenuItemListener)
    END_COM_MAP()

  // ITrayMenuItemListener
  public:

    STDMETHODIMP onSelected(fMenuItemEventMask EventsToSignal)
    {
      m_SignalSelected.permitNextThread();
	    return S_OK;
    }

    void waitSelected(void) const
    {
      m_SignalSelected.waitForever();
    }

  private:

    cSemaphore m_SignalSelected;
};

*/

// =======================================================================================
// === cConsoleLogDriver =================================================================
// =======================================================================================

class cConsoleLogDriver : public iLogDriver, public cRefCount
{
  public :

    QDEFINE_CLASS_IMPL(cConsoleLogDriver, L"{C76C801E-513A-4699-86F0-2736A837DADD} // cConsoleLogDriver", iLogDriver);

    cConsoleLogDriver() throw(cNotAvailable);
    ~cConsoleLogDriver();

    void QAPI writeOutdent(iLogDriver::cMsg::ConstPtr Record) throw();
    void QAPI writeMsg(iLogDriver::cMsg::ConstPtr Record) throw();
    void QAPI writeCollapsedIndent(iLogDriver::cMsg::ConstPtr Record) throw();
    void QAPI writeExpandedIndent(iLogDriver::cMsg::ConstPtr Record) throw();
    void QAPI writeLazyIndent(iLogDriver::cMsg::ConstPtr Record) throw();
    void QAPI writeTrace(iLogDriver::cMsg::ConstPtr Record) throw();
    void QAPI writeBreak(iLogDriver::cMsg::ConstPtr Record) throw();

  private :

    cConsole::Ptr           m_Console;
    unsigned int            m_NumColumns;

    list<cGuard::ConstPtr>  m_Unwind;

    void QAPI writePrefix(iLogDriver::cMsg::ConstPtr Record, const Char ExpandButton);
    void QAPI writeInject(iLogDriver::cMsg::ConstPtr Record);
    void QAPI write(const cConsole::WriteFlag Flags, const unsigned int Indent, const String & Msg);

    void QAPI writeBytes(const void * const Bytes, const unsigned long NumBytes);
    void QAPI writeHTML(const String & UnicodeString);
    void QAPI writeUnicode(const Char * const Unicode, const unsigned long NumChars);
};
