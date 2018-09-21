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

// [RFD-148/AJS] Need to access the dump participant interface
#include "KernelDumpInterface.h"

namespace vfs
{
  // [RFD-148/AJS] Add dump participant interface
  class cBaseLogDriver : public iLogDriver, public cRefCount, public iKernelDumpManager::iKernelDumpParticipant
  {
  public:
    QDEFINE_CLASSINFO(L"{124A22A4-9DE8-4075-8A39-131A68AB6379} // Advanced Log Driver", iLogDriver);
    virtual vfs::iFactory::iClassInfo::ConstPtr QAPI getClassInfo() const { return new cBaseLogDriver::class_info; }

    virtual void init(const size_t maxFileSize);

    // These functions are implemented by derived classes
    virtual const String getExtension() const { return L""; }
    virtual void writeHeader(const String& prevFile) const { }
    virtual void writeFooter(const String& nextFile) const { }
    virtual void handleMsg(const iLogDriver::cMsg& msg) const = 0;

    // [RFD-148/AJS] We need to implement dump callbacks
    virtual void    QAPI onDump(iKernelDumpProcessor * const mgr);
    virtual String  QAPI getParticipantID() const { return L"File Log Driver"; }

  protected:
    std::string mStartTime;
    String mLogFolder;
    String mFileName;
    UInt32 mFilePartNo;
    String mFullName; // L"mFileName (Part mFilePartNo).html"

    void newFile(); // Creates a new log, either at startup or because we hit the limit
    void checkFileSize(); // Checks that we haven't hit our file size limit

    void write(const String& data) const; // converts to utf8 and then calls...
    void write(const std::string& data) const; // ... which actually writes to the file

  private:
    iFile::Ptr mFile;
    size_t mMaxFileSize;
    mutable size_t mFileSize; // Since the only way to write to the file is through write() this size is always correct

    // [RFD-148/AJS] We need to hold on to our dump manager registration to participate
    //               in any required dumps
    cGuard::ConstPtr   mDumpRegistration;

    // There is no need for this many functions
    virtual void QAPI writeMsg(iLogDriver::cMsg::ConstPtr msg) { handleMsg(*msg); checkFileSize(); }
    virtual void QAPI writeTrace(iLogDriver::cMsg::ConstPtr msg) { handleMsg(*msg); checkFileSize(); }
    virtual void QAPI writeOutdent(iLogDriver::cMsg::ConstPtr msg) { handleMsg(*msg); checkFileSize(); }
    virtual void QAPI writeCollapsedIndent(iLogDriver::cMsg::ConstPtr msg) { handleMsg(*msg); checkFileSize(); }
    virtual void QAPI writeExpandedIndent(iLogDriver::cMsg::ConstPtr msg) { handleMsg(*msg); checkFileSize(); }
    virtual void QAPI writeLazyIndent(iLogDriver::cMsg::ConstPtr msg) { handleMsg(*msg); checkFileSize(); }
    virtual void QAPI writeBreak(iLogDriver::cMsg::ConstPtr msg) { handleMsg(*msg); checkFileSize(); }
  };
}
