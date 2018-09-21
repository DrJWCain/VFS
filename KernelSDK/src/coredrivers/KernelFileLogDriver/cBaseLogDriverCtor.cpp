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

using namespace vfs;

cBaseLogDriverCtor::cBaseLogDriverCtor()
{
  bool missingRegistry = false; // Registry migration can lead to corrupted settings

  String logFolder = getLogFolder();
  UInt32 maxLogCount = 40; // backwards compatible with KernelFileLogDriver's old default
  try
  {
    maxLogCount = gModuleContext->getPublicSettingNumber(L"System: Log File\\Max Log Count");
  }
  catch (cNotFound&)
  {
    missingRegistry = true;
    // Redundant hyperspace logs can get pretty large so combined with the max log size 
    // this would give us 500MB of logs which should be enough headroom
    maxLogCount = 100;
  }


  // Clear out the log folder, and rename the "Recent*.*" log files to their date names
  cLogFolderCleanUp(logFolder, maxLogCount);

  // Instantiate our singleton log driver
  String logType = L"html";
  try
  {
    logType = toLower(gModuleContext->getPublicSettingString(L"System: Log File\\Log Type"));
  }
  catch (cNotFound&)
  {
    missingRegistry = true;
  }

  if (logType == L"html")
    mSingleton = new cHtmlLogDriver();
  else if (logType == L"text")
    mSingleton = new cTextLogDriver();

  if (mSingleton.isNull())
    throw cNotFound(__FILE__, __LINE__, L"Registry log type \"%s\" not recognised; try \"html\" or \"text\".", logType.c_str());

  // Initialise log driver and pass value for maximum log size
  // default to reasonable upper limit (5MB), particularly for redundant hyperspace which has no installer
  UInt32 maxFileSize = 5000 * 1024;
  try
  {
    maxFileSize = gModuleContext->getPublicSettingNumber(L"System: Log File\\Max Log Size (KB)") * 1024;
  }
  catch (cNotFound&)
  {
    missingRegistry = true;
  }

  if (maxFileSize != 0)
    maxFileSize = max(64 * 1024, maxFileSize); // limits smaller than 64KB are a bit silly

  if (missingRegistry)
    QSOS((L"KernelFileLogDriver: Registry Settings Missing/Corrupted"));

  mSingleton->init(maxFileSize);
}

const String& cBaseLogDriverCtor::getLogFolder()
{
  static String logFolder;

  if (logFolder.empty())
  {
     // backwards compatible with KernelFileLogDriver's method for discovering the log folder
    try
    {
      logFolder = iSystemInfo::singleton().getCmdLineOption(L"logfolder");
    }
    catch (cNotFound &)
    {
      logFolder = ivfsEnvironment::singleton().getLocalMachineRootFolder() + L"Logs\\";
    }
    
    if (logFolder.empty())
      throw cNotFound(__FILE__, __LINE__, L"Unable to locate log folder.");

    if (!iFileManager::singleton().isFileOrFolderCreated(logFolder))
      iFileManager::singleton().createFolder(logFolder);
  }

  return logFolder;
}
