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

#include <QWinBase.h>

using namespace std;
using namespace vfs;




// =======================================================================================
// === cRegistryHelp =====================================================================
// =======================================================================================

class cRegistryHelp : public iRegistryHelp, public iGuardCreator, public cRefCount
{
  public :
    QDEFINE_SMARTPTR(cRegistryHelp);

    cRegistryHelp(HKEY hKey, iRegistryHelp::eRegistryView registryView) : mHkey(hKey), m_RegistryView(registryView) { }

    bool             QAPI isKeyCreated(const String & KeyPath);
    bool             QAPI isItemCreated(const String & KeyPath, const String & ItemName);
    void             QAPI enumerateTree(const String & RootKeyPath, iItemIterator::Ptr Iterator) throw(cNotFound, cWin32Error);
    String           QAPI getItemString(const String & KeyPath, const String & ItemName) throw(cNotFound, cAccessDenied, cWin32Error);
    unsigned long    QAPI getItemNumber(const String & KeyPath, const String & ItemName) throw(cNotFound, cAccessDenied, cWin32Error);
    void             QAPI setItemString(const String & KeyPath, const String & ItemName, const String & NewValue) throw(cNotFound, cAccessDenied, cWin32Error);
    void             QAPI setItemNumber(const String & KeyPath, const String & ItemName, const unsigned long NewValue) throw(cNotFound, cAccessDenied, cWin32Error);
    void             QAPI setSymbolValue(const eSymbol WhichSymbol, const String & NewValue);
    void             QAPI importRegEdit4File(const cMemoryView::ConstPtr & srcData) throw(cAccessDenied, cWin32Error);
    tRegEdit4Values  QAPI getRegEdit4Values(const cMemoryView::ConstPtr & srcData) throw(cAccessDenied, cWin32Error);
    String           QAPI substituteSymbol(const String & TextToSearch, const String & Symbol, const String & Replacement);
    String           QAPI substituteSymbols(const String & TextToSearch);
    std::map<String, unsigned long> QAPI getKeyNumbers(const String & KeyPath) throw(cNotFound);
    std::map<String, String> QAPI getKeyStrings(const String & KeyPath) throw(cNotFound);
    std::list<String> QAPI getSubKeys(const String & KeyPath) throw(cNotFound);
    void             QAPI deleteSubKey(const String & KeyPath, const String & SubKeyName) throw(cNotFound, cAccessDenied, cWin32Error);
    void             QAPI deleteItem(const String & KeyPath, const String & ItemName) throw(cNotFound, cAccessDenied, cWin32Error);
    eItemType        QAPI getItemType(const String & KeyPath, const String & ItemName) throw(cNotFound, cNotSupported);
    cGuard::ConstPtr QAPI watchKey(const String & KeyPath, cAutoResetEvent * const EventToSet) throw(cNotFound);
    vector<String>   QAPI getItemMultiString(const String & KeyPath, const String & ItemName) throw(cNotFound, cAccessDenied, cWin32Error);
    void             QAPI setItemMultiString(const String & KeyPath, const String & ItemName, const vector<String> & NewValues) throw(cNotFound, cAccessDenied, cWin32Error);
    void             QAPI copyTree(const String & SrcRootKeyPath, const String & DstRootKeyPath) throw(cNotFound, cWin32Error);

    cWinHKeyGuard::Ptr QAPI connectRegistryByName(const String & FQName) throw(cAccessDenied, cOSError);

    unsigned long QAPI getItemNumber(const HKEY hKey, const String & KeyPath, const String & ItemName) throw(cNotFound, cAccessDenied, cOSError);
    std::vector<String> QAPI getItemMultiString(const HKEY hKey, const String & KeyPath, const String & ItemName) throw(cNotFound, cAccessDenied, cOSError);

    void QAPI setItemNumber(const HKEY hKey, const String & KeyPath, const String & ItemName, const unsigned long NewValue) throw(cNotFound, cAccessDenied, cOSError);
    void QAPI setItemMultiString(const HKEY hKey, const String & KeyPath, const String & ItemName, const std::vector<String> & NewValues) throw(cNotFound, cAccessDenied, cOSError);

  private :

    mutable cLock m_ThreadSafety;

    class cRegEdit4FileParser
    {
      public :

        enum eItemType
        {
          eUnknown,
          eString,
          eNumber
        };

        cRegEdit4FileParser(const String & Items);

        bool          nextKey();
        bool          nextItem();
        eItemType getItemType() const { return m_CurrItemType; }
        String getKeyName() const { assert(m_CurrItemType != eUnknown); return m_CurrKeyName; }
        String getItemName() const { assert(m_CurrItemType != eUnknown); return m_CurrItemName; }
        String getItemString() const { assert(m_CurrItemType == eString); return m_CurrItemString; }
        unsigned long getItemNumber() const { assert(m_CurrItemType == eNumber); return m_CurrItemNumber; }

      private :

        String         m_Buffer;
        String         m_CurrLine;
        Char *         m_NextLinePtr; // null if no more lines
        eItemType      m_CurrItemType;
        String         m_CurrKeyName;
        String         m_CurrItemName;
        String         m_CurrItemString;
        unsigned long  m_CurrItemNumber;

        String        expandTokens(const String & Line);
        bool /* found next line? */ _nextLine();
        String        _getCurrLine();
    };

    void _enumerateTreeHelp(const String & RootKeyPath, iItemIterator::Ptr Iterator, list<String> * const SubKeyStack) throw(cNotFound, cWin32Error);

    String m_SymbolValues[eNumSymbols];

    // iGuardCreator
    void QAPI onGuardDestroyed(iGuarded::Ptr Guarded);

    iRegistryHelp::eRegistryView m_RegistryView;
    HKEY mHkey;
};
