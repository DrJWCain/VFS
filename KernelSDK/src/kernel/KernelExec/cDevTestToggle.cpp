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
#include "LogCategories.h"

#include "cFile.h"

using namespace vfs; 

namespace
{
  const Char* const kDevTestRegistryKey = L"Software\\vfs\\_DevTest"; //!< \brief When running a T version we get our toggles from the registry
  const Char* const kDevTestFileName = L"Toggles.sys";                    //!< \brief When running an S version we get our toggles from a file
  
  bool kTrueBool = true; //!< \brief When using file toggles, they all point to this boolean

  //{{{
  class cDevTestToggleManager
  {
  public:
    //{{{
    cDevTestToggleManager()
      : mInitialised(false), mRegTogglesAvailable(false), mForceTogglesOn(false), mForceTogglesOff(false), mFileTogglesAvailable(false)
    {
      
    }
    //}}}
    //{{{
    bool isAvailable()
    {
      if (!mInitialised)
        initialise();

      return (mRegTogglesAvailable || mFileTogglesAvailable);
    }
    //}}}
    //{{{
    bool* getBoolForToggle(bool* toggle, String variableName, String description)
    {
      cLockGuard lg(&mLock); // take the lock while constructing a toggle (std::map not thread safe!)

      tTogglesMap::const_iterator it = mToggles.find(variableName);

      if (it != mToggles.end())
        toggle = it->second.first;
      else
      {
        if (mRegTogglesAvailable)
        {
          mToggleStorage.push_back(false);
          toggle = &mToggleStorage.back();

          mToggles[variableName] = std::make_pair(toggle, description);
          createRegistryToggle(variableName, description);

          *toggle = getRegistryToggleValue(variableName);
        }
      }

      return toggle; // return the bool*
    }
    //}}}
    //{{{
    void updateAll()
    {
      if (!mInitialised)
        initialise();

      if (mRegTogglesAvailable)
      {
        cLockGuard lg(&mLock); // take the lock while updating toggles (std::map not thread safe!)

        // Check the state of ForceOn or ForceOff
        checkRegistryTogglesForced();

        // Update all toggles from the registry
        for (tTogglesMap::iterator it = mToggles.begin(); it != mToggles.end(); ++it)
        {
          const String& variableName = it->first;
          bool* toggleState = it->second.first;

          *toggleState = getRegistryToggleValue(variableName);
        }
      }
    }
    //}}}
    //{{{
    size_t logAll()
    {
      cLockGuard lg(&mLock); // take the lock while checking toggles (std::map not thread safe!)

      typedef std::set<std::pair<String, String> > tTogglesSet;
      tTogglesSet togglesEnabled;
      tTogglesSet togglesDisabled;

      // Group toggles into Enabled and Disabled
      for (tTogglesMap::iterator it = mToggles.begin(); it != mToggles.end(); ++it)
      {
        const String& variableName = it->first;
        bool* toggleState = it->second.first;
        const String& description = it->second.second;

        if (*toggleState)
          togglesEnabled.insert(tTogglesSet::value_type(variableName, description));
        else
          togglesDisabled.insert(tTogglesSet::value_type(variableName, description));
      }

      // Only log if we actually have toggles enabled
      if (mRegTogglesAvailable && (!togglesEnabled.empty() || !togglesDisabled.empty()))
      {
        cLogIndentGuard Indent(kCtgFeatureToggles, L"");

        if (mForceTogglesOn)
          QMSG((0, L"DevTest Toggles set to ForceOn"));
        else if (mForceTogglesOff)
          QMSG((0, L"DevTest Toggles set to ForceOff"));

        if (!togglesEnabled.empty())
        {
          cLogIndentGuard IndentEnabled(0, L"DevTest Toggles enabled:");
          for (tTogglesSet::const_iterator it = togglesEnabled.begin(); it != togglesEnabled.end(); ++it)
            QMSG((0, L"%-20s - %s", it->first.c_str(), it->second.c_str()));
        }
        if (!togglesDisabled.empty())
        {
          cLogIndentGuard IndentDisabled(0, L"DevTest Toggles disabled:");
          for (tTogglesSet::const_iterator it = togglesDisabled.begin(); it != togglesDisabled.end(); ++it)
            QMSG((0, L"%-20s - %s", it->first.c_str(), it->second.c_str()));
        }
      }

      return togglesEnabled.size();
    }
    //}}}

  private:
    //{{{
    void initialise()
    {
      cLockGuard lg(&mLock); // take the lock while reading from the registry
      if (mInitialised)
        return;
      mInitialised = true;

      // Get the path of the current executable
      String path;
      {
        std::vector<Char> filename(MAX_PATH);
        DWORD filenameSize = GetModuleFileName(NULL, &filename.at(0), (DWORD) filename.size());
        if (filenameSize == 0 || filenameSize == filename.size())
          return;
        path = String(&filename.at(0), filenameSize);
        path = path.substr(0, path.rfind(L"\\") + 1);
        if (path.size() < 2)
          return;
      }

      // Get the software version to decide if we are registry- (T builds) or file- (S builds) based
      const Char versionLetter = iSystemInfo::singleton().getProductVersionLetter();
      const String version = iSystemInfo::singleton().getProductVersion();

      if (versionLetter == L'D' || versionLetter == L'T') // D/T builds have registry toggles
      {
        mRegHelp = createRegistryHelp();
        if (mRegHelp->isKeyCreated(kDevTestRegistryKey))
        {
          try
          {
            // Check we have permission to write to the registry
            mRegHelp->setItemString(kDevTestRegistryKey, L"!", L"!");
            mRegHelp->deleteItem(kDevTestRegistryKey, L"!");

            mRegTogglesAvailable = true;
          }
          catch (cRecoverable&)
          {
            QSOS((L"Error attempting to write to \"%s\". DevTest toggles will not function. Check registry permissions", kDevTestRegistryKey));
          }

          // Check the state of ForceOn or ForceOff
          if (mRegTogglesAvailable)
            checkRegistryTogglesForced();
        }
      }

      if (versionLetter == L'S') // S builds have file based toggles
      {
        // Read the encrypted contents of the file
        cMemory::Ptr encryptedContents;
        try
        {
          encryptedContents = cFile(path + kDevTestFileName, fFileAccess_Read, 0).read();
        }
        catch (cNotFound)
        {
        }
        if (encryptedContents.isNull() || encryptedContents->getSize() == 0)
          return;

        // Decrypt the contents into a vector of strings
        std::vector<String> contents;

        const char* bytes = encryptedContents->getConstBytes();
        String line;
        for (size_t i = 0; i < encryptedContents->getSize(); i++)
        {
          char byte = ~bytes[i];
          if (byte == '\0')
          {
            contents.push_back(line);
            line.clear();
          }
          else
            line += byte;
        }
        if (!line.empty())
          contents.push_back(line);

        // The first parameter must match the version of software
        if (!contents.empty() && !contents[0].empty() && version.substr(0, contents[0].size()) == contents[0])
        {
          mFileTogglesAvailable = true;

          // The remaining parameters are the toggles to enable
          for (std::vector<String>::const_iterator it = contents.begin() + 1; it != contents.end(); ++it)
          {
            mToggles.insert(std::make_pair(*it, std::make_pair(&kTrueBool, String())));
          }
        }
      }
    }
    //}}}
    //{{{
    void createRegistryToggle(const String& variableName, const String& description)
    {
      if (!mRegHelp->isItemCreated(kDevTestRegistryKey, variableName))
        mRegHelp->setItemNumber(kDevTestRegistryKey, variableName, 0);
      mRegHelp->setItemString(kDevTestRegistryKey, variableName + L"-Desc", description);
    }
    //}}}
    //{{{
    void checkRegistryTogglesForced()
    {
      try
      {
        String defaultValue = mRegHelp->getItemString(kDevTestRegistryKey, L"");
        mForceTogglesOn = (defaultValue == L"ForceOn");
        mForceTogglesOff = (defaultValue == L"ForceOff");
      }
      catch (cRecoverable&)
      {
      }
    }
    //}}}
    //{{{
    bool getRegistryToggleValue(const String& variableName)
    {
      if (mForceTogglesOn)
        return true;
      if (mForceTogglesOff)
        return false;

      try
      {
        return (mRegHelp->getItemNumber(kDevTestRegistryKey, variableName) != 0);
      }
      catch (cRecoverable&)
      {
        return false;
      }
    }
    //}}}
    
    bool mInitialised;                //!< \brief Have we loaded our registry settings yet?
    bool mRegTogglesAvailable;        //!< \brief Are registry toggles available? Does the registry key exist?
    bool mFileTogglesAvailable;       //!< \brief Are file toggles available? Does "Toggles.sys" exist?

    typedef std::map<String, std::pair<bool*, String> > tTogglesMap;
    cLock mLock;                      //!< \brief Lock needs to be held for initialisation and updating of cDevTestToggle instances
    tTogglesMap mToggles;             //!< \brief Map from feature toggle Name to (Value, Description) pair
    bool mForceTogglesOn;             //!< \brief Have all toggles been forced on?
    bool mForceTogglesOff;            //!< \brief Have all toggles been forced off?

    std::deque<bool> mToggleStorage;  //!< \brief The actual storage for the toggles

    iRegistryHelp::Ptr mRegHelp;
  };
  //}}}

  //! \brief The only cDevTestToggleManager in existence
  cDevTestToggleManager gDevTestToggleManager;
}

//{{{
cDevTestToggle::cDevTestToggle(const char* const variableName, const Char* const description)
  : mToggleState(&mBoolStorage), mBoolStorage(false)
{
  if (gDevTestToggleManager.isAvailable())
    mToggleState = gDevTestToggleManager.getBoolForToggle(mToggleState, widen(variableName), description);
}
//}}}
//{{{
size_t cDevTestToggle::logAll()
{
  if (gDevTestToggleManager.isAvailable())
    return gDevTestToggleManager.logAll();
  return 0;
}
//}}}
//{{{
void cDevTestToggle::updateAll()
{
  gDevTestToggleManager.updateAll();
}
//}}}
