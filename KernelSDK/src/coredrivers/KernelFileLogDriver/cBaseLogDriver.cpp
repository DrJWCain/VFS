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
#include <tchar.h>

using namespace vfs;

//{{{
void cBaseLogDriver::init(const size_t maxFileSize)
{
  mMaxFileSize = maxFileSize;

  const cTime time = iSystemInfo::singleton().getCurrentLocalTime();
  const cDate date = iSystemInfo::singleton().getCurrentLocalDate();
  mStartTime = isoTime(date.getYear(), date.getMonth(), date.getDay(), time.getHour(), time.getMinute(), time.getSecond());

  mFileName = L"Recent";
  mFilePartNo = 0;
  newFile();

  // [RFD-148/AJS] Check for the existance of the dump manager and if found register
  if (iKernelDumpManager::isSingletonRegistered())
    mDumpRegistration = iKernelDumpManager::singleton().Register(this);
}
//}}}

//{{{
void cBaseLogDriver::newFile()
{
  mFilePartNo++;

  StringStream filepath;
  filepath << mFileName;
  if (mMaxFileSize > 0)
    filepath << L" (Part " << mFilePartNo << L')';
  filepath << L'.' << getExtension();

  // If we already have a file open, tell it to write a footer and pass along the location of the new file.
  String oldPath;
  if (mFile.isValid())
  {
    oldPath = mFullName;
    writeFooter(filepath.str());
  }

  mFullName = filepath.str();

  // Delete the previous file just in case
  try
  {
    iFileManager::singleton().deleteFileOrFolder(cBaseLogDriverCtor::getLogFolder() + mFullName, 0);
  }
  catch (cRecoverable &)
  {

  }

  mFile = iFileManager::singleton().openFile(cBaseLogDriverCtor::getLogFolder() + mFullName, fFileAccess_Write);
  if (mFile.isNull())
    throw cNotAvailable(__FILE__, __LINE__, L"Unable to create log file \"%s\".", (cBaseLogDriverCtor::getLogFolder() + mFullName).c_str());

  mFileSize = 0;

  writeHeader(oldPath); // pass along the location of the old file, if there was one
}
//}}}

//{{{
void cBaseLogDriver::checkFileSize()
{
  // If we've gone over our target size then start a new file
  if (mMaxFileSize > 0 && mFileSize > mMaxFileSize)
    newFile();
}
//}}}

//{{{
void cBaseLogDriver::write(const String& utf16_data) const
{
  if (!utf16_data.empty())
  {
    std::string utf8_data;
    try
    {
      utf8::utf16to8(utf16_data.begin(), utf16_data.end(), std::back_inserter(utf8_data));
    }
    catch (utf8::exception&)
    {
      utf8_data += "[INVALID UNICODE CHARACTER SEQUENCE]";
    }
    write(utf8_data);
  }
}
//}}}
//{{{
void cBaseLogDriver::write(const std::string& utf8_data) const
{
  const std::string::size_type size = utf8_data.size();
  if (size > 0)
  {
    mFile->write((UInt64) size, utf8_data.c_str());
    mFileSize += size;
  }
}
//}}}

//{{{
/* Method "onDump" provides the callback interface for dump production when the system
                   fails or a user requests a dump. We will copy all the log files from
                   this run into the dump.

   Inputs:  1) The interface for dump information addition

   Outputs: None

   Returns: None
*/

void QAPI cBaseLogDriver::onDump(vfs::iKernelDumpProcessor *const mgr)
{
  // Use a stack based buffer as we may be in a low-memory state and unable
  // to create new strings on the heap. If we've run out of stuck we have a
  // big problem!
  TCHAR fileName[MAX_PATH+1];

  // Are we running a multi-part file
  if (mMaxFileSize > 0)
    for (int lfNum = mFilePartNo; lfNum > 0; --lfNum)
    {
      _sntprintf(fileName, MAX_PATH, L"%s%s (Part %d).%s",
            cBaseLogDriverCtor::getLogFolder().c_str(), mFileName.c_str(), lfNum, getExtension().c_str());
      mgr->AddFile(fileName);
    }
  else
  {
    _sntprintf(fileName, MAX_PATH, L"%s%s.%s", cBaseLogDriverCtor::getLogFolder().c_str(),
                    mFileName.c_str(), getExtension().c_str());
    mgr->AddFile(fileName);
  }
}
//}}}