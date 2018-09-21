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

//
// Definition of the dump manager interfaces used for creating support information. 
// Defined to be a singleton. 
//
// Any parts of the system that wish to participate in the 
// provision of information for later analysis of incidents or problems can
// register to be visited when a dump is required.
//
// The processor interface will be provided to participants on callback to allow
// files to be created or added to the current dump.
//


#pragma once
#ifndef KERNELDUMPINTERFACE_H
#define KERNELDUMPINTERFACE_H

#include "QKernelExecFactory.h"

namespace vfs
{
  class iKernelDumpProcessor;
  
  // Interface for the manager which records interested participants
  class iKernelDumpManager : public iFactory::iInstance
  {
    public:
      QDEFINE_SINGLETON_IFACE(iKernelDumpManager, L"{4557A085-FF8B-46cc-84B7-1D638D200FE5} // Kernel Dump Info iface", iFactory::iInstance);

      // Interface required by participants for call back when data is
      // being dumped
      class iKernelDumpParticipant : public iGuarded
      {
        public:
          QDEFINE_SMARTPTR(iKernelDumpParticipant);

          virtual void          QAPI onDump(iKernelDumpProcessor * const) = 0;
          virtual String        QAPI getParticipantID() const = 0;
          
          // Callback for cleaning up when all other participants have completed.
          // A default null implementation is provided as there are few cases
          // where this will be needed
          virtual void          QAPI onCleanUp() const { }
      };

      // Register/Unregister to take part in dump production
      virtual cGuard::ConstPtr  QAPI  Register(iKernelDumpParticipant::Ptr) = 0;
      virtual void              QAPI  UnRegister(iKernelDumpParticipant::Ptr) = 0;

      // Check the global enable for dump production
      virtual bool              QAPI  isDumpEnabled() const = 0;

      // Initiate dump
      virtual void              QAPI  GenDump() = 0;

      // Get path of the dump location
      virtual String            QAPI  GetDumpLocation() = 0;
  };
  
  // Interface for dumping info on request/crash for support and sustaining use in 
  // problem reproduction. Passed to participants when they are called to perform
  // the dump

  class iKernelDumpProcessor
  {
    public:
      // Add an existing file to the dump (makes a copy)
      virtual void QAPI AddFile(LPCTSTR path) = 0;

      // Create a file in the dump with the specified name. Data can then
      // be added to the file by calls to Append and finally closed. Only
      // one file can be in use at a time in this version.
      virtual void QAPI GenFile(LPCTSTR name, bool binary = false) = 0;
      virtual void QAPI AppendS(LPCTSTR msg) const = 0;
      virtual void QAPI AppendF(LPCTSTR fmt, ...) = 0;
      virtual void QAPI Close() = 0;

      virtual void QAPI AppendB(void *data, DWORD len) const = 0;

      virtual const TCHAR * QAPI GetPath() const = 0;

      // Provide a method by which a participant can register to be called
      // again once all other dumps have completed.
      virtual void QAPI RegisterCleanup(iKernelDumpManager::iKernelDumpParticipant::Ptr) = 0;
      
      // Maximum line length for formatted output
      static const int kMaxLineLength = _MAX_PATH;
  };
}

#endif