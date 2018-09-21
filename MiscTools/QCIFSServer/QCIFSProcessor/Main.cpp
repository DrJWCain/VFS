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

//////////////////////////////////////////////////////////////////////////////
// Main.cpp

#include "stdafx.h"
#include "Main.h"

#include "cQCIFSProcessor.h"
#include "cUniqueNotifyID.h"
#include "LogCategories.h"
#include "..\..\..\MiscPlugins\QCoSpace\cServiceHelp.h"

// =======================================================================================
// === moduleInit/moduleCleanUp ==========================================================
// =======================================================================================
using namespace vfs;

typedef std::list<cGuard::ConstPtr> tImplList;
static tImplList gRegdClasses;
extern const String kNetManServiceName (L"vfs NetManService");
extern const String kNetManEvent (L"Global\\NetManCompleteEvent");

iModuleContext::Ptr gModuleContext;

namespace
{
  USHORT gSMBCredits = 40;
  bool gUseClassicSMBHandler = false;
}

bool useClassicSMBHandler()
{
  return gUseClassicSMBHandler;
}

USHORT getSMBCredits()
{
  return gSMBCredits;
}
// ---------------------------------------------------------------------------------------

DWORD getRegistrySetting(String name, unsigned int min, unsigned int max, unsigned int defaultValue)
{
  DWORD setting;

  try
  {
    setting = gModuleContext->getCustomSettingNumber(L"", name.c_str());
  }
  catch (cNotFound&)
  {
    QSOS((L"Using default value (%i) for option %s", (int)defaultValue, name.c_str()));
    setting = defaultValue;
  }

  if (setting < min)
  {
    setting = min;
    QSOS((L"%s set to minimum allowable value %u", name.c_str(), min));
  }
  else if (setting > max)
  {
    setting = max;
    QSOS((L"%s set to maximum allowable value %u", name.c_str(), max));
  }

  return setting;
}

FILETIME BootFileTime;

namespace
{
  const Char* kDelimiters = L" ,\t";       // delimiter char set

  // parse utilities
  //{{{
  bool strEquals (const Char* str1, const Char* str2)
  {
    if (str1 && str2)
        // uses MS case insensitive comparison routine
      return _wcsicmp (str1, str2) ? false : true;
    else
      return (str1 == str2) ? true : false;   // check for both being null
  }
  //}}}
  //{{{
  bool onlyDigits (const Char* str)
  // check that the string has only valid numbers
  {
    const Char *s = str;
    while (*s)
      {
      if ((*s < L'0') || (*s > L'9'))
        return false;                       // not a digit
      s++;
      }
    return true;
  }
  //}}}
  //{{{
  bool validNum (Char* str, int* num, bool displayNoNumWarning) throw (cBadFormat)
  // check number
  {
    if (displayNoNumWarning && !str)
      throw cBadFormat (__FILE__, __LINE__, L"WARNING: No number specified");

    if (!str)
      return false;                         // no num specified

    if (strEquals (str, L"all"))
      return false;                         // all is allowed to flag no num specified

    if (!onlyDigits (str))
      throw cBadFormat (__FILE__, __LINE__, L"WARNING: Bad number %s", str);

    if (swscanf (str, L"%d", num) == 0)
      throw cBadFormat (__FILE__, __LINE__, L"WARNING: Bad number %s", str);

    return true;
  }
  //}}}
  //{{{
  bool getNum (int* numGot)
  {
    Char* numStr = wcstok (NULL, kDelimiters);   // get number in string form
    return validNum (numStr, numGot, false);
  }
  //}}}
  //{{{
  bool getBool() throw (cBadFormat)
  {
    Char* boolStr = wcstok (NULL, kDelimiters);   // get new boolean state in string form
    if (strEquals (boolStr, L"on") || strEquals (boolStr, L"1") || strEquals (boolStr, L"true"))
      return true;
    else if (strEquals (boolStr, L"off") || strEquals (boolStr, L"0") || strEquals (boolStr, L"false"))
      return false;
    else
      throw cBadFormat (__FILE__, __LINE__, L"Bad binary state %s", boolStr);
  }
  //}}}
  //{{{
  bool getString (Char** strGot)
  {
    *strGot = wcstok (NULL, kDelimiters);
    return true;
  }
  //}}}
};


class cDumpPacketsTestCmd : public vfs::iCmdConsoleManager::iCommand, public vfs::cRefCount
{
public:
  vfs::String QAPI getName() const;

  void QAPI logOneLineDescription() const;
  void QAPI logDetailedDescription() const;
  void QAPI logOneLineParams() const;
  void QAPI logDetailedParams() const;
  void QAPI execute (const vfs::String& Parameters) const;
};
//}}}
vfs::cGuard::ConstPtr DumpPacketsTestCmd;


class cAsyncTestCmd : public vfs::iCmdConsoleManager::iCommand, public vfs::cRefCount
{
public:
  vfs::String QAPI getName() const;

  void QAPI logOneLineDescription() const;
  void QAPI logDetailedDescription() const;
  void QAPI logOneLineParams() const;
  void QAPI logDetailedParams() const;
  void QAPI execute (const vfs::String& Parameters) const;
};
//}}}
vfs::cGuard::ConstPtr AsyncTestCmd;

extern "C" QPLUGIN void QARGS_STACK moduleInit(iModuleContext::Ptr pContext) throw( cRecoverable)
{
   // gPluginContext = new iModuleContext::Ptr (pContext);
  gModuleContext = pContext;

  SYSTEMTIME st;
  GetSystemTime(&st);
  SystemTimeToFileTime(&st, &BootFileTime);

  gRegdClasses.push_back(gFactory->registerClass(new cQCIFSProcessor::class_ctor));
  gRegdClasses.push_back(gFactory->registerClass(new cUniqueNotifyID::class_ctor));

  gSMBCredits = gModuleContext->getCustomSettingNumber (L"Config", L"SMB credits");
  gUseClassicSMBHandler = gModuleContext->getCustomSettingNumber(L"Config", L"Use Classic SMB Handler");

  DumpPacketsTestCmd = iCmdConsoleManager::singleton().addCommand (new cDumpPacketsTestCmd, false, false);
  AsyncTestCmd = iCmdConsoleManager::singleton().addCommand (new cAsyncTestCmd, false, false);
}

extern "C" QPLUGIN void QARGS_STACK moduleCleanUp (void) throw() 
{
  gRegdClasses.clear();
}

extern "C" QPLUGIN void QARGS_STACK moduleInstall(const int Flags, iModuleContext::Ptr Incoming, iModuleContext::Ptr OptionalOutgoing, iModuleInstallHelp::Ptr Help) throw(cRecoverable)
{
  OSVERSIONINFOW osVer;
  osVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFOW);
  if(GetVersionEx(&osVer))
  {
    if(osVer.dwMajorVersion >= 6) // Only for Vista, Win7 or Win2k8 or better
    {
      try
      {
        if(isServiceRegistered(kNetManServiceName))
        {
          try
          {
            const String path = getServicePath(kNetManServiceName);
            removeService(Help, kNetManServiceName, path, L"NetManService.exe");
          }
          catch(cRecoverable& err)
          {
            QSOS((L"Failed to remove NetManService (service) - %s", err.getMsg().c_str()));
            // carry on anyway...
          }
        }

        installService(Help, kNetManServiceName, Incoming->getDataFolder() + L"NetManService\\", L"NetManService.exe",
                       L"Manages Net Server Transport Functions for vfs generationQ");
      }
      catch(cRecoverable& err)
      {
        QSOS((L"Error during NetManService install - %s", err.getMsg().c_str()));
      }
    }
  }
  else QSOS((L"GetVersionEx failed, error %d", GetLastError()));
}

extern "C" QPLUGIN void QARGS_STACK moduleUninstall(iModuleContext::Ptr Outgoing, iModuleInstallHelp::Ptr Help, const bool IsRemovingSettingsAndData) throw(cRecoverable, cIgnoreInstallFailed)
{
  if(isServiceRegistered(kNetManServiceName))
  {
    const String path = getServicePath(kNetManServiceName);

    if(Outgoing->getDataFolder() + L"NetManService\\" == path)
    {
      try
      {
        removeService(Help, kNetManServiceName, Outgoing->getDataFolder() + L"NetManService\\", L"NetManService.exe");
      }
      catch(cRecoverable& err)
      {
        QSOS((L"Failed to uninstall NetManService (service) - %s", err.getMsg().c_str()));
      }
    }
  }
}



//{{{
vfs::String QAPI cDumpPacketsTestCmd::getName() const
{
  return L"smb2dump";
}
//}}}

void QAPI cDumpPacketsTestCmd::logOneLineDescription() const
{
}
void QAPI cDumpPacketsTestCmd::logDetailedDescription() const
{
}

//{{{
void QAPI cDumpPacketsTestCmd::logOneLineParams() const
{
}
//}}}

void QAPI cDumpPacketsTestCmd::logDetailedParams() const
{
}

void QAPI cDumpPacketsTestCmd::execute (const vfs::String& Parameters) const
{
  QTRACE((L"cmd = %s", Parameters.c_str()));

  iQCIFSProcessor::singleton().dumpBuffers();
}


vfs::String QAPI cAsyncTestCmd::getName() const
{
  return L"async";
}
//}}}

void QAPI cAsyncTestCmd::logOneLineDescription() const
{
}
void QAPI cAsyncTestCmd::logDetailedDescription() const
{
}

//{{{
void QAPI cAsyncTestCmd::logOneLineParams() const
{
}
//}}}

void QAPI cAsyncTestCmd::logDetailedParams() const
{
}

extern bool kAsync;
extern unsigned int kMsDelay;
void sendQueuedReadRequests();
void sendQueuedHoldingResponces();
void QAPI cAsyncTestCmd::execute (const vfs::String& Parameters) const
{
  QTRACE((L"cmd = %s", Parameters.c_str()));

  Char* commandStr = (Char*) Parameters.c_str();       // get writable pointer
  String cmd = wcstok (commandStr, kDelimiters);     // get first command token

  if(L"pause" == cmd)
  {
    if(kAsync)
      sendQueuedHoldingResponces();
    kAsync = true;
  }
  if(L"resume" == cmd)
  {
    kAsync = false;
    sendQueuedReadRequests();
  }
  if(L"delay" == cmd)
  {
    int delay = 0;
    if(getNum(&delay))
      kMsDelay = delay;
  }
}