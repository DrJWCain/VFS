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

/////////////////////////////////////////////////////////////////////////////
// RebootThread.h

#pragma once

class cQCIFSProcessor;

//class cRebootThread : public vfs::cThread
//{
//public:
//  cRebootThread (cQCIFSProcessor *const pQCIFSP);
//  ~cRebootThread ();
//
//  void QAPI signalLoopStop (void);
//
//  void QAPI forceRebootAsap (bool bIsUserRequest = false); // called when there is a problem and we want to restart the app asap
//  vfs::String QAPI autoRebootTime (void);
//  void QAPI setAutoRebootTime (vfs::String newT);
//
//protected:
//  void QAPI runLoop (void);
//
//private:
//  void QAPI setNextReboot ();
//
//  cQCIFSProcessor* const m_QCIFSP;
//  vfs::cDateTime m_RebootDateTime;
//  bool m_bDoNotReboot;
//  bool m_bUserRequest;
//  bool m_bForcedRestart;
//};
