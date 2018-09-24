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

//cQCIFSTest.h

#pragma once

#include "iQCIFSTest.h"

class cQCIFSTest : public iQCIFSTest, public vfs::cRefCount
{
public:
  QDEFINE_SINGLETON_IMPL(cQCIFSTest, L"{0B59BA3E-2BA7-4a1c-9892-E2014A82F625} // cQCIFSTest", iQCIFSTest);

  cQCIFSTest(){}
  ~cQCIFSTest(void){}

  virtual void QAPI logSummary(void) {QTRACE((L"cQCIFSTest"));}
  virtual void QAPI logDetailedSummary(void) {QTRACE((L"cQCIFSTest - simple Test harness for QCifs"));}

  virtual vfs::String shareName(){return L"test";}
  virtual vfs::cPtr<iChildLoader> createRootChildLoader();
};