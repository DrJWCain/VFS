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

#include "iQCIFSSofa.h"

class cQCIFSSofa : public iQCIFSSofa, public vfs::cRefCount
{
public:
  QDEFINE_SINGLETON_IMPL(cQCIFSSofa, L"{62937E9B-E21E-41f9-B310-4C5D55DD5227} // cQCIFSSofa", iQCIFSSofa);

  cQCIFSSofa(){}
  ~cQCIFSSofa(void){}

  virtual void QAPI logSummary(void) {QTRACE((L"cQCIFSSofa"));}
  virtual void QAPI logDetailedSummary(void) {QTRACE((L"cQCIFSSofa - example plugin for QCifs"));}

  virtual vfs::String shareName(){return L"sofa";}
  virtual vfs::cPtr<iChildLoader> createRootChildLoader();
};