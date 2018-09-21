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

#include "QWinBase.h"

namespace vfs
{
  struct
  {
    DWORD flag; const char* name;
  }
  const kGFlagsTable[] = {
    {0x00000001, "FLG_STOP_ON_EXCEPTION"},
    {0x00000002, "FLG_SHOW_LDR_SNAPS"},
    {0x00000004, "FLG_DEBUG_INITIAL_COMMAND"},
    {0x00000008, "FLG_STOP_ON_HUNG_GUI"},
    {0x00000010, "FLG_HEAP_ENABLE_TAIL_CHECK"},
    {0x00000020, "FLG_HEAP_ENABLE_FREE_CHECK"},
    {0x00000040, "FLG_HEAP_VALIDATE_PARAMETERS"},
    {0x00000080, "FLG_HEAP_VALIDATE_ALL"},
    {0x00000100, "FLG_APPLICATION_VERIFIER"},
    {0x00000200, "FLG_MONITOR_SILENT_PROCESS_EXIT"},
    {0x00000400, "FLG_POOL_ENABLE_TAGGING"},
    {0x00000800, "FLG_HEAP_ENABLE_TAGGING"},
    {0x00001000, "FLG_USER_STACK_TRACE_DB"},
    {0x00002000, "FLG_KERNEL_STACK_TRACE_DB"},
    {0x00004000, "FLG_MAINTAIN_OBJECT_TYPELIST"},
    {0x00008000, "FLG_HEAP_ENABLE_TAG_BY_DLL"},
    {0x00010000, "FLG_DISABLE_STACK_EXTENSION"},
    {0x00020000, "FLG_ENABLE_CSRDEBUG"},
    {0x00040000, "FLG_ENABLE_KDEBUG_SYMBOL_LOAD"},
    {0x00080000, "FLG_DISABLE_PAGE_KERNEL_STACKS"},
    {0x00100000, "FLG_ENABLE_SYSTEM_CRIT_BREAKS"},
    {0x00200000, "FLG_HEAP_DISABLE_COALESCING"},
    {0x00400000, "FLG_ENABLE_CLOSE_EXCEPTIONS"},
    {0x00800000, "FLG_ENABLE_EXCEPTION_LOGGING"},
    {0x01000000, "FLG_ENABLE_HANDLE_TYPE_TAGGING"},
    {0x02000000, "FLG_HEAP_PAGE_ALLOCS"},
    {0x04000000, "FLG_DEBUG_INITIAL_COMMAND_EX"},
    {0x08000000, "FLG_DISABLE_DBGPRINT"},
    {0x10000000, "FLG_CRITSEC_EVENT_CREATION"},
    {0x20000000, "FLG_LDR_TOP_DOWN"},
    {0x40000000, "FLG_ENABLE_HANDLE_EXCEPTIONS"},
    {0x80000000, "FLG_DISABLE_PROTDLLS"},
  };

  void printGFlags(DWORD flags)
  {
    for (auto& entry : kGFlagsTable)
    {
      if ((flags & entry.flag) != 0)
        QMSG((0, L"  0x%08x %S", entry.flag, entry.name));
    }
  }

  class cGetSubValuesEnumerator : public iRegistryHelp::iItemIterator, public cRefCount
  {
  public:
    std::vector<String> m_Found;

    bool QAPI onSubKey(const String & KeyPath, const list<String> & SubKeyStack, const iRegistryHelp::Ptr & rh) { return false; }
    void QAPI onItem(const String & KeyPath, const String & ItemName, const list<String> & SubKeyStack, const iRegistryHelp::Ptr & rh)
    {
      if (ItemName != L"GlobalFlag")
        m_Found.push_back(ItemName);
    }
  };

  void detectGFlags()
  {
    String exeName;
    {
      std::vector<Char> exePath(MAX_PATH);
      GetModuleFileName(NULL, exePath.data(), exePath.size());
      exeName = exePath.data();

      String::size_type i = exeName.rfind(L'\\');
      if (i != String::npos)
        exeName = toLower(exeName.substr(i + 1));
    }

    auto rh = createRegistryHelp();
    
    const String ifeoRoot = L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options";
    String ifeoKey;
    for (auto subkey : rh->getSubKeys(ifeoRoot))
      if (toLower(subkey) == exeName)
        ifeoKey = ifeoRoot + L"\\" + subkey;

    DWORD SystemFlags = 0;
    DWORD ImageFlags = 0;
    cPtr<cGetSubValuesEnumerator> subValues = new cGetSubValuesEnumerator();

    try
    {
      SystemFlags = rh->getItemNumber(L"SYSTEM\\CurrentControlSet\\Control\\Session Manager", L"GlobalFlag");
    }
    catch (cRecoverable&)
    {}
    try
    {
      StringStream ss;
      ss << std::hex << rh->getItemString(L"SYSTEM\\CurrentControlSet\\Control\\Session Manager", L"GlobalFlag");
      ss >> SystemFlags;
    }
    catch (cRecoverable&)
    {}
    
    if (!ifeoKey.empty())
    {
      try
      {
        ImageFlags = rh->getItemNumber(ifeoKey, L"GlobalFlag");
      }
      catch (cRecoverable&)
      {}
      try
      {
        StringStream ss;
        ss << std::hex << rh->getItemString(ifeoKey, L"GlobalFlag");
        ss >> ImageFlags;
      }
      catch (cRecoverable&)
      {}
      try
      {
        rh->enumerateTree(ifeoKey, subValues);
      }
      catch (cRecoverable&)
      {}
    }

    SystemFlags &= ~0x400; // ignore FLG_POOL_ENABLE_TAGGING
    ImageFlags &= ~0x400;

    if (SystemFlags != 0 || ImageFlags != 0 || !subValues->m_Found.empty())
    {
      cLogIndentSOSGuard IndentGuard(L"!!! Windows Debugging Flags Detected !!!");

      if (SystemFlags != 0)
      {
        QSOS((L"System-wide Flags Enabled: 0x%08x", SystemFlags));
        printGFlags(SystemFlags);
      }
      if (ImageFlags != 0)
      {
        QSOS((L"Image-specific Flags Enabled: 0x%08x", ImageFlags));
        printGFlags(ImageFlags);
      }
      if (!subValues->m_Found.empty())
      {
        for (auto subVal : subValues->m_Found)
          QSOS((L"Unknown debug flag \"%s\" detected.", subVal.c_str()));
      }
    }
  }
}
