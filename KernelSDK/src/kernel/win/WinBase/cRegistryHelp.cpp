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
#include <QWinBase.h>
#include "cRegistryHelp.h"
#include <shlwapi.h>

using namespace std;
using namespace vfs;




// =======================================================================================
// === createRegistryHelp ================================================================
// =======================================================================================

iRegistryHelp::Ptr vfs::createRegistryHelp(iRegistryHelp::eRegistryView registryView)
{
  return new cRegistryHelp(HKEY_LOCAL_MACHINE, registryView);
}

iRegistryHelp::Ptr vfs::createCurrentUserRegistryHelp(iRegistryHelp::eRegistryView registryView)
{
  return new cRegistryHelp(HKEY_CURRENT_USER, registryView);
}


// =======================================================================================
// === cRegEdit4FileParser ===============================================================
// =======================================================================================

cRegistryHelp::cRegEdit4FileParser::cRegEdit4FileParser(const String & Items) :
  m_Buffer(Items),
  m_CurrItemType(eUnknown),
  m_CurrItemString(),
  m_CurrItemNumber(0),
  m_CurrItemName(),
  m_CurrKeyName(),
  m_NextLinePtr(0)
{
  m_NextLinePtr = &m_Buffer[0];
  _nextLine();
  m_CurrLine = m_Buffer;
}

// ---------------------------------------------------------------------------------------

bool cRegistryHelp::cRegEdit4FileParser::nextKey()
{
  m_CurrItemType = eUnknown;
  bool hasFoundNewKey = false;
  while (_nextLine())
  {
    Char tmpKeyName[512];
    if (swscanf(m_CurrLine.c_str(), L"[HKEY_LOCAL_MACHINE\\%[^]]", &tmpKeyName[0]) == 1)
    {
      hasFoundNewKey = true;
      m_CurrKeyName = tmpKeyName;
      break;
    }
  }
  return hasFoundNewKey;
}

// ---------------------------------------------------------------------------------------

#define ANSI_CR       0xd
#define ANSI_NEWLINE  0xa

bool cRegistryHelp::cRegEdit4FileParser::nextItem()
{
  m_CurrItemType = eUnknown;
  bool hasFoundNewItem = false;
  bool IsMultiLineString = false;
  while (_nextLine())
  {
    String copyOfCurrLine(m_CurrLine);
    Char          TmpItemName[256];
    Char          TmpItemString[1024];
    unsigned long TmpItemNumber;
    if (copyOfCurrLine.size() == 0)
    {
      // End of items
      break;
    }
    else if (IsMultiLineString)
    {
      if (swscanf(copyOfCurrLine.c_str(), L"%[^\"]", &TmpItemString) == 1)
      {
        m_CurrItemString += String(L"\n") + TmpItemString;
      }
      if (copyOfCurrLine[copyOfCurrLine.size() - 1] == L'\"')
      {
        break;
      }
    }
    else if (swscanf(copyOfCurrLine.c_str(), L"\"%[^\"]\"=\"%[^\"]", &TmpItemName, &TmpItemString) == 2)
    {
      hasFoundNewItem = true;
      m_CurrItemType = eString;
      m_CurrItemName = TmpItemName;
      m_CurrItemString = expandTokens(TmpItemString);
      if (copyOfCurrLine[copyOfCurrLine.size() - 1] != L'\"')
      {
        // Multi-line string!
        IsMultiLineString = true;
      }
      else
      {
        break;
      }
    }
    else if (swscanf(copyOfCurrLine.c_str(), L"\"%[^\"]\"=dword:%x", &TmpItemName, &TmpItemNumber) == 2)
    {
      hasFoundNewItem = true;
      m_CurrItemType = eNumber;
      m_CurrItemName = TmpItemName;
      m_CurrItemNumber = TmpItemNumber;
      break;
    }
    else if (swscanf(copyOfCurrLine.c_str(), L"\"%[^\"]\"=\"\"", &TmpItemName) == 1)
    {
      hasFoundNewItem = true;
      m_CurrItemType = eString;
      m_CurrItemName = TmpItemName;
      m_CurrItemString = L"";
      break;
    }
  }
  return hasFoundNewItem;
}

// ---------------------------------------------------------------------------------------

namespace
{

  String _trimWhitespace(const String & a)
  {
    if (a.empty())
    {
      return a;
    }
    else
    {
      unsigned int c = (unsigned int)a.size();
      const Char * p = &a[c - 1];
      while (c > 0)
      {
        if (*p != ANSI_CR && *p != L' ')
        {
          break;
        }
        c--;
        p--;
      }
      return a.substr(0, c);
    }
  }

}

bool cRegistryHelp::cRegEdit4FileParser::_nextLine()
{
  bool hasFoundNextLine = false;
  if (m_NextLinePtr)
  {
    hasFoundNextLine = true;
    Char * startOfLine = m_NextLinePtr;
    while (*m_NextLinePtr != ANSI_NEWLINE && *m_NextLinePtr != L'\0')
    {
      m_NextLinePtr++;
    }
    if (*m_NextLinePtr == ANSI_NEWLINE)
    {
      *m_NextLinePtr = L'\0';
      m_NextLinePtr++;
    }
    else if (*m_NextLinePtr == L'\0')
    {
      m_NextLinePtr = 0;
    }
    m_CurrLine = _trimWhitespace(startOfLine);
  }
  else
  {
    m_CurrLine = L"";
  }
  return hasFoundNextLine;
}




// =======================================================================================
// === getItemType =======================================================================
// =======================================================================================

cRegistryHelp::eItemType QAPI cRegistryHelp::getItemType(
  const String & KeyPath,
  const String & ItemName) throw(cNotFound, cNotSupported)
{
  // Protect member variables from changes by another thread.
  cLockGuard ThreadSafety(&m_ThreadSafety);

  HKEY hKey = NULL;

  eItemType Result;

  try
  {
    LONG nRegResult;
    DWORD nValueType;
    unsigned long nKeyValueSize;

    nRegResult = RegOpenKeyEx(mHkey, KeyPath.c_str(), 0, KEY_READ | m_RegistryView, &hKey);
    if (nRegResult != ERROR_SUCCESS)
    {
      throw cNotFound(
        __FILE__, __LINE__,
        L"Tried to RegOpenKeyEx() at \"%s\" (item \"%s\") - returned \"%s\"",
        KeyPath.c_str(), ItemName.c_str(), win32ErrorToString(nRegResult).c_str());
    }

    // Get size and format of key
    nRegResult = RegQueryValueEx(
      hKey,                 // Handle of key to query
      ItemName.c_str(),     // -> name of value to query
      0,                    // Reserved
      &nValueType,          // -> buffer for value type
      (unsigned char *)0,   // -> data buffer
      &nKeyValueSize);      // Data buffer size
    if (nRegResult != ERROR_SUCCESS)
    {
      throw cNotFound(
        __FILE__, __LINE__,
        L"Tried to RegQueryValueEx() at \"%s\" (item \"%s\") - returned \"%s\"",
        KeyPath.c_str(), ItemName.c_str(), win32ErrorToString(nRegResult).c_str());
    }

    switch (nValueType)
    {
    case REG_SZ: { Result = eItemType_String; break; }
    case REG_DWORD: { Result = eItemType_Number; break; }
    case REG_MULTI_SZ: { Result = eItemType_MultiString; break; }
    default: { throw cNotSupported(__FILE__, __LINE__, L"Unexpected item \"%s\" type %d", ItemName.c_str(), nValueType); }
    }

    (void)RegCloseKey(hKey);
    hKey = NULL;
  }
  catch (cRecoverable &)
  {
    // Clean up
    if (hKey) (void)RegCloseKey(hKey);
    throw;
  }

  return Result;
}




// =======================================================================================
// === getKeyNumbers/getKeyStrings/getSubKeys ============================================
// =======================================================================================

class cGetKeyNumbersEnumerator : public iRegistryHelp::iItemIterator, public cRefCount
{
public:
  QDEFINE_SMARTPTR(cGetKeyNumbersEnumerator);
  cRegistryHelp * const  m_RegBuilder;
  map<String, unsigned long> m_Result;
  cGetKeyNumbersEnumerator(cRegistryHelp * const RegBuilder) :
    m_RegBuilder(RegBuilder)
  {
  }
  bool QAPI onSubKey(const String & KeyPath, const list<String> & SubKeyStack, const iRegistryHelp::Ptr & rh)
  {
    return false; /* go into */
  }
  void QAPI onItem(const String & KeyPath, const String & ItemName, const list<String> & SubKeyStack, const iRegistryHelp::Ptr & rh)
  {
    m_Result.insert(make_pair(ItemName, m_RegBuilder->getItemNumber(KeyPath, ItemName)));
  }
};

map<String, unsigned long> QAPI cRegistryHelp::getKeyNumbers(const String & KeyPath) throw(cNotFound)
{
  cGetKeyNumbersEnumerator::Ptr Enumerator = new cGetKeyNumbersEnumerator(this);
  enumerateTree(KeyPath, Enumerator);
  return Enumerator->m_Result;
}

// ---------------------------------------------------------------------------------------

class cGetKeyStringsEnumerator : public iRegistryHelp::iItemIterator, public cRefCount
{
public:
  QDEFINE_SMARTPTR(cGetKeyStringsEnumerator);
  cRegistryHelp * const  m_RegBuilder;
  map<String, String>        m_Result;
  cGetKeyStringsEnumerator(cRegistryHelp * const RegBuilder) :
    m_RegBuilder(RegBuilder)
  {
  }
  bool QAPI onSubKey(const String & KeyPath, const list<String> & SubKeyStack, const iRegistryHelp::Ptr & rh)
  {
    return false; /* go into */
  }
  void QAPI onItem(const String & KeyPath, const String & ItemName, const list<String> & SubKeyStack, const iRegistryHelp::Ptr & rh)
  {
    m_Result.insert(make_pair(ItemName, m_RegBuilder->getItemString(KeyPath, ItemName)));
  }
};

map<String, String> QAPI cRegistryHelp::getKeyStrings(const String & KeyPath) throw(cNotFound)
{
  cGetKeyStringsEnumerator::Ptr Enumerator = new cGetKeyStringsEnumerator(this);
  enumerateTree(KeyPath, Enumerator);
  return Enumerator->m_Result;
}

// ---------------------------------------------------------------------------------------

class cGetSubKeysEnumerator : public iRegistryHelp::iItemIterator, public cRefCount
{
public:
  QDEFINE_SMARTPTR(cGetSubKeysEnumerator);
  const cRegistryHelp * const  m_RegBuilder;
  list<String>                    m_Result;
  cGetSubKeysEnumerator(const cRegistryHelp * const RegBuilder) :
    m_RegBuilder(RegBuilder)
  {
  }
  bool QAPI onSubKey(const String & KeyPath, const list<String> & SubKeyStack, const iRegistryHelp::Ptr & rh)
  {
    m_Result.push_back(*SubKeyStack.begin());
    return false; /* go into */
  }
  void QAPI onItem(const String & KeyPath, const String & ItemName, const list<String> & SubKeyStack, const iRegistryHelp::Ptr & rh)
  {
  }
};

list<String> QAPI cRegistryHelp::getSubKeys(const String & KeyPath) throw(cNotFound)
{
  cGetSubKeysEnumerator::Ptr Enumerator = new cGetSubKeysEnumerator(this);
  enumerateTree(KeyPath, Enumerator);
  return Enumerator->m_Result;
}




// =======================================================================================
// === deleteSubKey ======================================================================
// =======================================================================================

void QAPI cRegistryHelp::deleteSubKey(const String & KeyPath, const String & SubKeyName) throw(cNotFound, cAccessDenied, cWin32Error)
{
  // Protect member variables from changes by another thread.
  cLockGuard ThreadSafety(&m_ThreadSafety);

  HKEY hKey = NULL;

  try
  {
    LONG nRegResult;

    nRegResult = RegOpenKeyEx(mHkey, KeyPath.c_str(), 0, KEY_SET_VALUE | m_RegistryView, &hKey);
    if (nRegResult != ERROR_SUCCESS)
    {
      throw cNotFound(
        __FILE__, __LINE__,
        L"Tried to RegOpenKeyEx() at \"%s\" (subkey \"%s\") - returned \"%s\"", KeyPath.c_str(), SubKeyName.c_str(), win32ErrorToString(nRegResult).c_str());
    }

    nRegResult = SHDeleteKey(hKey, SubKeyName.c_str());
    if (nRegResult != ERROR_SUCCESS)
    {
      throw cAccessDenied(
        __FILE__, __LINE__,
        L"Tried to RegDeleteKey() at \"%s\" (subkey \"%s\") - returned \"%s\"",
        KeyPath.c_str(), SubKeyName.c_str(), win32ErrorToString(nRegResult).c_str());
    }

    nRegResult = RegCloseKey(hKey);
    if (nRegResult != ERROR_SUCCESS)
    {
      throw cWin32Error(__FILE__, __LINE__, L"Tried to RegCloseKey() on \"%s\" (subkey \"%s\") - returned \"%s\"", KeyPath.c_str(), SubKeyName.c_str(), win32ErrorToString(nRegResult).c_str());
    }
  }
  catch (cRecoverable &)
  {
    // Clean up if there was an exception.
    if (hKey) (void)RegCloseKey(hKey);
    throw;
  }
}




// =======================================================================================
// === deleteItem ========================================================================
// =======================================================================================

void QAPI cRegistryHelp::deleteItem(const String & KeyPath, const String & ItemName) throw(cNotFound, cAccessDenied, cWin32Error)
{
  // Protect member variables from changes by another thread.
  cLockGuard ThreadSafety(&m_ThreadSafety);

  HKEY hKey = NULL;

  try
  {
    LONG nRegResult;

    nRegResult = RegOpenKeyEx(mHkey, KeyPath.c_str(), 0, KEY_SET_VALUE | m_RegistryView, &hKey);
    if (nRegResult != ERROR_SUCCESS)
    {
      throw cNotFound(
        __FILE__, __LINE__,
        L"Tried to RegOpenKeyEx() at \"%s\" - returned \"%s\"", KeyPath.c_str(), win32ErrorToString(nRegResult).c_str());
    }

    nRegResult = RegDeleteValue(hKey, ItemName.c_str());
    if (nRegResult != ERROR_SUCCESS)
    {
      throw cAccessDenied(
        __FILE__, __LINE__,
        L"Tried to RegDeleteValue() at \"%s\" - returned \"%s\"", KeyPath.c_str(), win32ErrorToString(nRegResult).c_str());
    }

    nRegResult = RegCloseKey(hKey);
    if (nRegResult != ERROR_SUCCESS)
    {
      throw cWin32Error(__FILE__, __LINE__, L"Tried to RegCloseKey() on \"%s\" - returned \"%s\"", KeyPath.c_str(), win32ErrorToString(nRegResult).c_str());
    }
  }
  catch (cRecoverable &)
  {
    // Clean up if there was an exception.
    if (hKey) (void)RegCloseKey(hKey);
    throw;
  }
}




// =======================================================================================
// === expandTokens ======================================================================
// =======================================================================================

String cRegistryHelp::cRegEdit4FileParser::expandTokens(const String & Line)
{
  String NewLine;
  bool HasFoundToken = false;
  for (String::const_iterator i = Line.begin(); i != Line.end(); ++i)
  {
    if (HasFoundToken)
    {
      NewLine.append(1, *i);
      HasFoundToken = false;
    }
    else if (*i == L'\\')
    {
      HasFoundToken = true;
    }
    else
    {
      NewLine.append(1, *i);
    }
  }
  return NewLine;
}




// =======================================================================================
// === substituteSymbol ==================================================================
// =======================================================================================

String QAPI cRegistryHelp::substituteSymbol(
  const String & TextToSearch,
  const String & Symbol,
  const String & Replacement)
{
  // Protect member variables from changes by another thread.
  cLockGuard ThreadSafety(&m_ThreadSafety);

  const Char * const FirstOccurence = wcsstr(TextToSearch.c_str(), Symbol.c_str());
  if (FirstOccurence == NULL)
  {
    return TextToSearch;
  }
  else
  {
    String Result;
    const unsigned int FirstOccurenceIndex = PtrToUlong((const void *)(FirstOccurence - TextToSearch.c_str()));
    if (FirstOccurenceIndex > 0)
    {
      Result.append(TextToSearch.c_str(), FirstOccurenceIndex);
    }
    Result.append(Replacement);
    if (FirstOccurenceIndex + Symbol.size() < TextToSearch.size())
    {
      Result.append(&FirstOccurence[Symbol.size()]);
    }
    return Result;
  }
}




// =======================================================================================
// === substituteSymbols =================================================================
// =======================================================================================

String QAPI cRegistryHelp::substituteSymbols(const String & TextToSearch)
{
  // Protect member variables from changes by another thread.
  cLockGuard ThreadSafety(&m_ThreadSafety);

  String Result(TextToSearch);
  Result = substituteSymbol(Result, L"%MODULEPATH%\\", m_SymbolValues[eModulePath]);

  return Result;
}




// =======================================================================================
// === setSymbolValue ====================================================================
// =======================================================================================

void QAPI cRegistryHelp::setSymbolValue(const eSymbol WhichSymbol, const String & NewValue)
{
  // Protect member variables from changes by another thread.
  cLockGuard ThreadSafety(&m_ThreadSafety);

  m_SymbolValues[WhichSymbol] = NewValue;
}




// =======================================================================================
// === importRegEdit4File/getRegEdit4Values ==============================================
// =======================================================================================

void QAPI cRegistryHelp::importRegEdit4File(const cMemoryView::ConstPtr & Buffer) throw(cAccessDenied, cWin32Error)
{
  tRegEdit4Values values = getRegEdit4Values(Buffer);

  for (tRegEdit4Values::const_iterator it = values.begin(); it != values.end(); ++it)
  {
    const String keyName = it->first.first;
    const String valueName = it->first.second;
    switch (it->second.first)
    {
    case eItemType_String:
    {
      setItemString(keyName, valueName, it->second.second);
      break;
    }
    case eItemType_Number:
    {
      setItemNumber(keyName, valueName, lexical_cast<unsigned long>(it->second.second));
      break;
    }
    }
  }
}

iRegistryHelp::tRegEdit4Values QAPI cRegistryHelp::getRegEdit4Values(const cMemoryView::ConstPtr & Buffer) throw(cAccessDenied, cWin32Error)
{
  iRegistryHelp::tRegEdit4Values values;

  cLockGuard ThreadSafety(&m_ThreadSafety);
  {
    String WideBuffer;
    const bool IsUnicode = (*(Char *)Buffer->getConstBytes() == 0xfeff);
    if (!IsUnicode)
    {
      WideBuffer = widen(Buffer->getConstBytes());
    }
    else
    {
      WideBuffer = String((Char *)&(Buffer->getConstBytes()[2]));
    }

    cRegEdit4FileParser i(WideBuffer);
    while (i.nextKey())
    {
      while (i.nextItem())
      {
        std::pair<String, String> ExpandedKeyItemNames(substituteSymbols(i.getKeyName()), substituteSymbols(i.getItemName()));

        switch (i.getItemType())
        {
        case cRegEdit4FileParser::eString:
        {
          values.insert(tRegEdit4Values::value_type(ExpandedKeyItemNames, std::pair<eItemType, String>(eItemType_String, i.getItemString())));
          break;
        }
        case cRegEdit4FileParser::eNumber:
        {
          values.insert(tRegEdit4Values::value_type(ExpandedKeyItemNames, std::pair<eItemType, String>(eItemType_Number, lexical_cast<String>(i.getItemNumber()))));
          break;
        }
        default:
        {
          throw cWin32Error(__FILE__, __LINE__, L"Unsupported item type in RegEdit4 file");
        }
        }
      }
    }
  }

  return values;
}




// =======================================================================================
// === isKeyCreated ======================================================================
// =======================================================================================

bool QAPI cRegistryHelp::isKeyCreated(const String & KeyPath)
{
  // Protect member variables from changes by another thread.
  cLockGuard ThreadSafety(&m_ThreadSafety);

  bool Result = false;

  HKEY hKey = NULL;
  Result = RegOpenKeyEx(mHkey, KeyPath.c_str(), 0, KEY_READ | m_RegistryView, &hKey) == ERROR_SUCCESS;
  if (hKey) (void)RegCloseKey(hKey);

  return Result;
}




// =======================================================================================
// === isItemCreated =====================================================================
// =======================================================================================

bool QAPI cRegistryHelp::isItemCreated(const String & KeyPath, const String & ItemName)
{
  // Protect member variables from changes by another thread.
  cLockGuard ThreadSafety(&m_ThreadSafety);

  bool Result = false;

  HKEY          hKey = NULL;
  DWORD         nValueType;
  unsigned long nKeyValueSize;

  Result = RegOpenKeyEx(mHkey, KeyPath.c_str(), 0, KEY_READ | m_RegistryView, &hKey) == ERROR_SUCCESS;
  if (!Result || hKey == NULL) goto Abort;

  Result = RegQueryValueEx(
    hKey,                             // Handle of key to query
    ItemName.c_str(),                 // -> name of value to query
    0,                                // Reserved
    &nValueType,                      // -> buffer for value type
    (unsigned char *)0,               // -> data buffer
    &nKeyValueSize) == ERROR_SUCCESS; // Data buffer size
  if (!Result) goto Abort;

Abort:
  if (hKey)
  {
    (void)RegCloseKey(hKey);
  }

  return Result;
}




// =======================================================================================
// === setItemString =====================================================================
// =======================================================================================

void QAPI cRegistryHelp::setItemString(
  const String & KeyPath,
  const String & ItemName,
  const String & NewValue) throw(cAccessDenied, cWin32Error)
{
  // Protect member variables from changes by another thread.
  cLockGuard ThreadSafety(&m_ThreadSafety);
  HKEY hKey;
  LONG nRegResult;

  nRegResult = RegCreateKeyEx(
    mHkey,         // Key
    KeyPath.c_str(),            // SubKey
    0,                          // Reserved - always zero
    0,                          // Class - always zero
    REG_OPTION_NON_VOLATILE,    // Options
    KEY_WRITE | m_RegistryView, // Desired access
    0,                          // Default secruity
    &hKey,                      // -> Result
    0);                         // Disposition - always zero
  if (nRegResult != ERROR_SUCCESS)
  {
    switch (nRegResult)
    {
    case ERROR_ACCESS_DENIED: { throw cAccessDenied(__FILE__, __LINE__, L"Access denied to registry when writing string to \"%s\"", KeyPath.c_str()); }
    default: { throw cWin32Error(__FILE__, __LINE__, L"Tried to RegCreateKeyEx() on \"%s\" - returned \"%s\"", KeyPath.c_str(), win32ErrorToString(nRegResult).c_str()); }
    }
  }
  cKeyGuard RegKeyGuard(hKey);

  nRegResult = RegSetValueEx(
    hKey,                                  // Handle of key to query
    ItemName.c_str(),                      // -> name of value to set
    0,                                     // Reserved
    REG_SZ,                                // Value type
    (const unsigned char *)NewValue.c_str(),// Data buffer
    (unsigned int((NewValue.size() + 1) * sizeof(Char)))); // Data buffer size
  if (nRegResult != ERROR_SUCCESS)
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to RegSetValueEx() on \"%s\\%s\" - returned \"%s\"", KeyPath.c_str(), ItemName.c_str(), win32ErrorToString(nRegResult).c_str());
  }
}




// =======================================================================================
// === setItemString =====================================================================
// =======================================================================================

void QAPI cRegistryHelp::setItemMultiString(const String & KeyPath, const String & ItemName, const vector<String> & NewValues) throw(cAccessDenied, cWin32Error)
{
  setItemMultiString(mHkey, KeyPath, ItemName, NewValues);
}




// =======================================================================================
// === setItemNumber =====================================================================
// =======================================================================================

void QAPI cRegistryHelp::setItemNumber(const String & KeyPath, const String & ItemName, const unsigned long  NewValue)  throw(cAccessDenied)
{
  setItemNumber(mHkey, KeyPath, ItemName, NewValue);
}




// =======================================================================================
// === watchKey ==========================================================================
// =======================================================================================

class cWatchedKeyGuard : public iGuarded, public cRefCount
{
public:
  QDEFINE_SMARTPTR(cWatchedKeyGuard);
  const String  m_KeyPath;
  cKeyGuard     m_KeyHandle;
  cWatchedKeyGuard(const String & KeyPath, const HKEY KeyHandle) : m_KeyPath(KeyPath), m_KeyHandle(KeyHandle) {}
};

cGuard::ConstPtr QAPI cRegistryHelp::watchKey(const String & KeyPath, cAutoResetEvent * const EventToSet) throw(cNotFound)
{
  // Protect member variables from changes by another thread.
  cLockGuard ThreadSafety(&m_ThreadSafety);

  HKEY hKey = NULL;
  LONG nRegResult = RegOpenKeyEx(mHkey, KeyPath.c_str(), 0, KEY_NOTIFY | m_RegistryView, &hKey);
  if (nRegResult != ERROR_SUCCESS)
  {
    throw cNotFound(__FILE__, __LINE__, L"Tried to RegOpenKeyEx() at \"%s\" - returned \"%s\"", KeyPath.c_str(), win32ErrorToString(nRegResult).c_str());
  }

  nRegResult = RegNotifyChangeKeyValue(hKey, FALSE /* watch subtree? */, REG_NOTIFY_CHANGE_LAST_SET /* filter */, (HANDLE)EventToSet->getWin32EventHandle(), TRUE /* async? */);
  if (nRegResult != ERROR_SUCCESS)
  {
    throw cNotFound(__FILE__, __LINE__, L"Tried to RegNotifyChangeKeyValue() at \"%s\" - returned \"%s\"", KeyPath.c_str(), win32ErrorToString(nRegResult).c_str());
  }

  return new cGuard(this, new cWatchedKeyGuard(KeyPath, hKey));
}

void QAPI cRegistryHelp::onGuardDestroyed(iGuarded::Ptr Guarded)
{
  cWatchedKeyGuard::Ptr WatchedKey = ptr_cast<cWatchedKeyGuard::Ptr>(Guarded);
  assert(Guarded.isValid());
}




// =======================================================================================
// === getItemString/getItemNumber =======================================================
// =======================================================================================

String QAPI cRegistryHelp::getItemString(const String & KeyPath, const String & ItemName)  throw(cNotFound, cAccessDenied)
{
  // Protect member variables from changes by another thread.
  cLockGuard ThreadSafety(&m_ThreadSafety);

  HKEY hKey = NULL;

  String Result;
  LONG nRegResult;
  DWORD nValueType;
  unsigned long nKeyValueSize;

  nRegResult = RegOpenKeyEx(mHkey, KeyPath.c_str(), 0, KEY_READ | m_RegistryView, &hKey);
  if (nRegResult != ERROR_SUCCESS)
  {
    throw cNotFound(__FILE__, __LINE__, L"Tried to RegOpenKeyEx() at \"%s\" (item \"%s\") - returned \"%s\"", KeyPath.c_str(), ItemName.c_str(), win32ErrorToString(nRegResult).c_str());
  }
  cKeyGuard RegKeyGuard(hKey);

  // Get size and format of key
  nRegResult = RegQueryValueEx(
    hKey,                 // Handle of key to query
    ItemName.c_str(),     // -> name of value to query
    0,                    // Reserved
    &nValueType,          // -> buffer for value type
    (unsigned char *)0,   // -> data buffer
    &nKeyValueSize);      // Data buffer size
  if (nRegResult != ERROR_SUCCESS)
  {
    throw cNotFound(__FILE__, __LINE__, L"Tried to RegQueryValueEx() at \"%s\" (item \"%s\") - returned \"%s\"", KeyPath.c_str(), ItemName.c_str(), win32ErrorToString(nRegResult).c_str());
  }

  // Key must contain a string!
  if (nValueType != REG_SZ)
  {
    throw cWin32Error(__FILE__, __LINE__, L"Expected REG_SZ from RegQueryValueEx() on \"%s\\%s\"", KeyPath.c_str(), ItemName.c_str());
  }

  vector<Char> buffer(nKeyValueSize + 1);
  Char * pBuffer = &buffer[0];

  // Get copy of key for real
  nRegResult = RegQueryValueEx(
    hKey,                     // Handle of key to query
    ItemName.c_str(),         // -> name of value to query
    0,                        // Reserved
    &nValueType,              // -> buffer for value type
    (unsigned char *)pBuffer, // -> data buffer
    &nKeyValueSize);          // Data buffer size
  if (nRegResult != ERROR_SUCCESS)
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to RegQueryValueEx() on \"%s\\%s\" - returned \"%s\"", KeyPath.c_str(), ItemName.c_str(), win32ErrorToString(nRegResult).c_str());
  }

  Result = pBuffer;

  return Result;
}

// ---------------------------------------------------------------------------------------

unsigned long QAPI cRegistryHelp::getItemNumber(const String & KeyPath, const String & ItemName) throw(cNotFound, cAccessDenied)
{
  return getItemNumber(mHkey, KeyPath, ItemName);
}




// =======================================================================================
// === getItemMultiString ================================================================
// =======================================================================================

vector<String> QAPI cRegistryHelp::getItemMultiString(const String & KeyPath, const String & ItemName)  throw(cNotFound, cAccessDenied)
{
  return getItemMultiString(mHkey, KeyPath, ItemName);
}




// =======================================================================================
// === Helpers ===========================================================================
// =======================================================================================

namespace
{

  String _formatAsPath(const String & ToFormat)
  {
    String Result(ToFormat);
    if (Result.size() > 0)
    {
      const Char LastChar = *Result.rbegin();
      if (LastChar != L'\\' && LastChar != L'/') Result += L"\\";
    }
    return Result;
  }

}




// =======================================================================================
// === enumerateTree =====================================================================
// =======================================================================================

void QAPI cRegistryHelp::enumerateTree(const String & RootKeyPath, iItemIterator::Ptr Iterator) throw(cNotFound, cWin32Error)
{
  list<String> SubKeyStack;
  _enumerateTreeHelp(_formatAsPath(RootKeyPath), Iterator, &SubKeyStack);
}

// ---------------------------------------------------------------------------------------

void cRegistryHelp::_enumerateTreeHelp(const String & RootKeyPath, iItemIterator::Ptr Iterator, list<String> * const SubKeyStack) throw(cNotFound, cWin32Error)
{
  // Protect member variables from changes by another thread.
  cLockGuard ThreadSafety(&m_ThreadSafety);

  HKEY hKey = NULL;
  LONG nRegResult = RegOpenKeyEx(mHkey, RootKeyPath.c_str(), 0, KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS | m_RegistryView, &hKey);
  if (nRegResult != ERROR_SUCCESS)
  {
    throw cNotFound(__FILE__, __LINE__, L"Tried to RegOpenKeyEx() at \"%s\"", RootKeyPath.c_str());
  }
  cKeyGuard RegKeyGuard(hKey);

  // Items
  unsigned int nItemIndex = 0;
  do
  {
    Char      ItemName[256];
    DWORD     nNumCharsInItemName = sizeof(ItemName) / sizeof(Char);
    nRegResult = RegEnumValue(
      hKey,                     // Handle to key to enumerate
      nItemIndex++,             // Index of value to enumerate
      ItemName,                 // -> Buffer for value name
      &nNumCharsInItemName,     // -> Size of value buffer
      NULL,                     // -> Reserved (must be NULL)
      NULL,                     // -> buffer for item type
      NULL,                     // -> item data buffer
      NULL);                    // Data buffer size
    if (nRegResult == ERROR_SUCCESS)
    {
      Iterator->onItem(RootKeyPath, ItemName, *SubKeyStack, this);
    }
  } while (nRegResult == ERROR_SUCCESS);
  if (nRegResult != ERROR_NO_MORE_ITEMS)
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to RegEnumValue() at \"%s\" - returned \"%s\"", RootKeyPath.c_str(), win32ErrorToString(nRegResult).c_str());
  }

  // Subkeys
  unsigned int nSubKeyIndex = 0;
  do
  {
    Char    SubKeyName[256];
    FILETIME  SubKeyLastWritten;
    DWORD   nNumCharsInSubKeyName = sizeof(SubKeyName) / sizeof(Char);
    nRegResult = RegEnumKeyEx(
      hKey,                   // Handle to key to enumerate
      nSubKeyIndex++,         // Index of SubKey to enumerate
      SubKeyName,             // -> Buffer for SubKey name
      &nNumCharsInSubKeyName, // -> Size of SubKey buffer
      NULL,                   // -> Reserved (must be NULL)
      NULL,                   // -> Buffer for class string (igorned)
      NULL,                   // -> Size of class buffer (ignored)
      &SubKeyLastWritten);    // -> Time key last written to
    if (nRegResult == ERROR_SUCCESS)
    {
      SubKeyStack->push_front(SubKeyName);
      if (Iterator->onSubKey(RootKeyPath, *SubKeyStack, this))
      {
        _enumerateTreeHelp(RootKeyPath + SubKeyName + L"\\", Iterator, SubKeyStack);
      }
      SubKeyStack->pop_front();
    }
  } while (nRegResult == ERROR_SUCCESS);

  if (nRegResult != ERROR_NO_MORE_ITEMS)
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to RegEnumValue() at \"%s\" - returned \"%s\"", RootKeyPath.c_str(), win32ErrorToString(nRegResult).c_str());
  }
}




// =======================================================================================
// === copyTree ==========================================================================
// =======================================================================================

class cCopyTreeEnumerator : public iRegistryHelp::iItemIterator, public cRefCount
{
public:
  QDEFINE_SMARTPTR(cCopyTreeEnumerator);
  const cRegistryHelp * const  m_RegBuilder;
  const String                    m_DstRootKeyPath;
  cCopyTreeEnumerator(const String & DstRootKeyPath, const cRegistryHelp * const RegBuilder) :
    m_RegBuilder(RegBuilder),
    m_DstRootKeyPath(DstRootKeyPath)
  {
  }
  bool QAPI onSubKey(const String & KeyPath, const list<String> & SubKeyStack, const iRegistryHelp::Ptr & rh)
  {
    return true; /* descend? */
  }
  void QAPI onItem(const String & KeyPath, const String & ItemName, const list<String> & SubKeyStack, const iRegistryHelp::Ptr & rh)
  {
    StringStream SubKeyPath;
    { for (list<String>::const_reverse_iterator i = SubKeyStack.rbegin(); i != SubKeyStack.rend(); ++i) { SubKeyPath << *i << L"\\"; } }
    switch (rh->getItemType(KeyPath, ItemName))
    {
    case iRegistryHelp::eItemType_String: { rh->setItemString(m_DstRootKeyPath + SubKeyPath.str(), ItemName, rh->getItemString(KeyPath, ItemName)); break; }
    case iRegistryHelp::eItemType_Number: { rh->setItemNumber(m_DstRootKeyPath + SubKeyPath.str(), ItemName, rh->getItemNumber(KeyPath, ItemName)); break; }
    case iRegistryHelp::eItemType_MultiString: { rh->setItemMultiString(m_DstRootKeyPath + SubKeyPath.str(), ItemName, rh->getItemMultiString(KeyPath, ItemName)); break; }
    }
  }
};

void QAPI cRegistryHelp::copyTree(const String & SrcRootKeyPath, const String & DstRootKeyPath) throw(cNotFound, cWin32Error)
{
  enumerateTree(_formatAsPath(SrcRootKeyPath), new cCopyTreeEnumerator(_formatAsPath(DstRootKeyPath), this));
}

cWinHKeyGuard::Ptr QAPI cRegistryHelp::connectRegistryByName(const String & FQName) throw(cAccessDenied, cOSError)
{
  HKEY hKey = NULL;
  String regName = L"\\\\" + FQName;
  LONG nRegResult = RegConnectRegistry(regName.c_str(), mHkey, &hKey);
  if (nRegResult != ERROR_SUCCESS)
  {
    throw cOSError(__FILE__, __LINE__, L"Tried to RegConnectRegistry() at \"%s\" - returned \"%s\"", FQName.c_str(), win32ErrorToString(nRegResult).c_str());
  }

  return new cWinHKeyGuard(hKey);
}

unsigned long QAPI cRegistryHelp::getItemNumber(const HKEY hKey, const String & KeyPath, const String & ItemName) throw(cNotFound, cAccessDenied, cOSError)
{
  // Protect member variables from changes by another thread.
  cLockGuard ThreadSafety(&m_ThreadSafety);

  HKEY hKeyItem = NULL;
  LONG nRegResult;
  nRegResult = RegOpenKeyEx(hKey, KeyPath.c_str(), 0, KEY_READ | m_RegistryView, &hKeyItem);
  if (nRegResult != ERROR_SUCCESS)
  {
    throw cNotFound(__FILE__, __LINE__, L"Tried to RegOpenKeyEx() at \"%s\" (item \"%s\") - returned \"%s\"", KeyPath.c_str(), ItemName.c_str(), win32ErrorToString(nRegResult).c_str());
  }

  cKeyGuard RegKeyGuard(hKeyItem);

  unsigned long nValue = 0;
  DWORD nValueType = 0;
  unsigned long nKeyValueSize;

  // Get size and format of key
  nRegResult = RegQueryValueEx(
    hKeyItem,               // Handle of key to query
    ItemName.c_str(),       // -> name of value to query
    NULL,                   // Reserved
    &nValueType,            // -> buffer for value type
    (unsigned char *)0,     // -> data buffer
    &nKeyValueSize);        // Data buffer size
  if (nRegResult != ERROR_SUCCESS)
  {
    throw cNotFound(__FILE__, __LINE__, L"Tried to RegQueryValueEx() at \"%s\" (item \"%s\") - returned \"%s\"", KeyPath.c_str(), ItemName.c_str(), win32ErrorToString(nRegResult).c_str());
  }

  // Key must contain a number!
  if (nValueType != REG_DWORD)
  {
    throw cWin32Error(__FILE__, __LINE__, L"Expected REG_DWORD from RegQueryValueEx() on \"%s\\%s\"", KeyPath.c_str(), ItemName.c_str());
  }

  // Get copy of key for real
  nRegResult = RegQueryValueEx(
    hKeyItem,                 // Handle of key to query
    ItemName.c_str(),         // -> name of value to query
    NULL,                     // Reserved
    &nValueType,              // -> buffer for value type
    (unsigned char *)&nValue, // -> data buffer
    &nKeyValueSize);          // Data buffer size
  if (nRegResult != ERROR_SUCCESS)
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to RegQueryValueEx() on \"%s\\%s\" - returned \"%s\"", KeyPath.c_str(), ItemName.c_str(), win32ErrorToString(nRegResult).c_str());
  }

  return nValue;
}

std::vector<String> QAPI cRegistryHelp::getItemMultiString(const HKEY hKey, const String & KeyPath, const String & ItemName) throw(cNotFound, cAccessDenied, cOSError)
{
  // Protect member variables from changes by another thread.
  cLockGuard ThreadSafety(&m_ThreadSafety);

  HKEY hKeyItem = NULL;
  LONG nRegResult;
  nRegResult = RegOpenKeyEx(hKey, KeyPath.c_str(), 0, KEY_READ | m_RegistryView, &hKeyItem);
  if (nRegResult != ERROR_SUCCESS)
  {
    throw cNotFound(__FILE__, __LINE__, L"Tried to RegOpenKeyEx() at \"%s\" (item \"%s\") - returned \"%s\"", KeyPath.c_str(), ItemName.c_str(), win32ErrorToString(nRegResult).c_str());
  }

  cKeyGuard RegKeyGuard(hKeyItem);

  DWORD nValueType;
  unsigned long nKeyValueSize;

  // Get size and format of key
  nRegResult = RegQueryValueEx(
    hKeyItem,             // Handle of key to query
    ItemName.c_str(),     // -> name of value to query
    0,                    // Reserved
    &nValueType,          // -> buffer for value type
    (unsigned char *)0,   // -> data buffer
    &nKeyValueSize);      // Data buffer size
  if (nRegResult != ERROR_SUCCESS)
  {
    throw cNotFound(__FILE__, __LINE__, L"Tried to RegQueryValueEx() at \"%s\" (item \"%s\") - returned \"%s\"", KeyPath.c_str(), ItemName.c_str(), win32ErrorToString(nRegResult).c_str());
  }

  // Key must contain a string!
  if (nValueType != REG_MULTI_SZ)
  {
    throw cWin32Error(__FILE__, __LINE__, L"Expected REG_SZ from RegQueryValueEx() on \"%s\\%s\"", KeyPath.c_str(), ItemName.c_str());
  }

  vector<Char> buffer(nKeyValueSize + 1);
  Char * pBuffer = &buffer[0];

  // Get copy of key for real
  nRegResult = RegQueryValueEx(
    hKeyItem,                 // Handle of key to query
    ItemName.c_str(),         // -> name of value to query
    0,                        // Reserved
    &nValueType,              // -> buffer for value type
    (unsigned char *)pBuffer, // -> data buffer
    &nKeyValueSize);          // Data buffer size
  if (nRegResult != ERROR_SUCCESS)
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to RegQueryValueEx() on \"%s\\%s\" - returned \"%s\"", KeyPath.c_str(), ItemName.c_str(), win32ErrorToString(nRegResult).c_str());
  }

  vector<String> Result;
  const Char * CurrPos = pBuffer;
  const Char * StartOfCurrentValue = pBuffer;
  while (true)
  {
    CurrPos++;
    if (*CurrPos == L'\0')
    {
      if (*StartOfCurrentValue == L'\0') break;
      Result.push_back(StartOfCurrentValue);
      CurrPos++;
      StartOfCurrentValue = CurrPos;
      if (*StartOfCurrentValue == L'\0') break;
    }
  }

  return Result;
}

void QAPI cRegistryHelp::setItemNumber(const HKEY hKey, const String & KeyPath, const String & ItemName, const unsigned long NewValue) throw(cNotFound, cAccessDenied, cOSError)
{
  // Protect member variables from changes by another thread.
  cLockGuard ThreadSafety(&m_ThreadSafety);
  HKEY hKeyItem;
  LONG nRegResult;

  nRegResult = RegCreateKeyEx(
    hKey,                       // Key
    KeyPath.c_str(),            // SubKey
    0,                          // Reserved - always zero
    0,                          // Class - always zero
    REG_OPTION_NON_VOLATILE,    // Options
    KEY_WRITE | m_RegistryView, // Desired access
    0,                          // Default secruity
    &hKeyItem,                  // -> Result
    0);                         // Disposition - always zero
  if (nRegResult != ERROR_SUCCESS)
  {
    switch (nRegResult)
    {
    case ERROR_ACCESS_DENIED: { throw cAccessDenied(__FILE__, __LINE__, L"Access denied to registry when writing number to \"%s\"", KeyPath.c_str()); }
    default: { throw cWin32Error(__FILE__, __LINE__, L"Tried to RegCreateKeyEx() on \"%s\" - returned \"%s\"", KeyPath.c_str(), win32ErrorToString(nRegResult).c_str()); }
    }
  }
  cKeyGuard RegKeyGuard(hKeyItem);

  nRegResult = RegSetValueEx(
    hKeyItem,                         // Handle of key to query
    ItemName.c_str(),                 // -> name of value to set
    0,                                // Reserved
    REG_DWORD,                        // Value type
    (const unsigned char *)&NewValue, // Data buffer
    sizeof(NewValue));                // Data buffer size
  if (nRegResult != ERROR_SUCCESS)
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to RegSetValueEx() on \"%s\\%s\" - returned \"%s\"", KeyPath.c_str(), ItemName.c_str(), win32ErrorToString(nRegResult).c_str());
  }
}

void QAPI cRegistryHelp::setItemMultiString(const HKEY hKey, const String & KeyPath, const String & ItemName, const std::vector<String> & NewValues) throw(cNotFound, cAccessDenied, cOSError)
{
  // Protect member variables from changes by another thread.
  cLockGuard ThreadSafety(&m_ThreadSafety);
  HKEY hKeyItem;
  LONG nRegResult;

  nRegResult = RegCreateKeyEx(
    hKey,                       // Key
    KeyPath.c_str(),            // SubKey
    0,                          // Reserved - always zero
    0,                          // Class - always zero
    REG_OPTION_NON_VOLATILE,    // Options
    KEY_WRITE | m_RegistryView, // Desired access
    0,                          // Default secruity
    &hKeyItem,                  // -> Result
    0);                         // Disposition - always zero
  if (nRegResult != ERROR_SUCCESS)
  {
    switch (nRegResult)
    {
    case ERROR_ACCESS_DENIED: { throw cAccessDenied(__FILE__, __LINE__, L"Access denied to registry when writing multistring to \"%s\"", KeyPath.c_str()); }
    default: { throw cWin32Error(__FILE__, __LINE__, L"Tried to RegCreateKeyEx() on \"%s\" - returned \"%s\"", KeyPath.c_str(), win32ErrorToString(nRegResult).c_str()); }
    }
  }
  cKeyGuard RegKeyGuard(hKeyItem);

  StringStream ActualValue;
  for (vector<String>::const_iterator i = NewValues.begin(); i != NewValues.end(); ++i)
  {
    ActualValue << *i << L'\0';
  }
  if (NewValues.size() == 0) ActualValue << L'\0';
  ActualValue << L'\0';

  nRegResult = RegSetValueEx(
    hKeyItem,                              // Handle of key to query
    ItemName.c_str(),                      // -> name of value to set
    0,                                     // Reserved
    REG_MULTI_SZ,                          // Value type
    (const unsigned char *)ActualValue.str().c_str(),// Data buffer
    unsigned int(ActualValue.str().size() * sizeof(Char))); // Data buffer size
  if (nRegResult != ERROR_SUCCESS)
  {
    throw cWin32Error(__FILE__, __LINE__, L"Tried to RegSetValueEx() on \"%s\\%s\" - returned \"%s\"", KeyPath.c_str(), ItemName.c_str(), win32ErrorToString(nRegResult).c_str());
  }
}

