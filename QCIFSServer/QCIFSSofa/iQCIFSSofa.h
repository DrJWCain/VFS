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

//iQCIFSIPC.h

#pragma once

class iTreeResourceFactory;
class iChildLoader;

class iQCIFSSofa : public vfs::iManager
{
public:
  QDEFINE_SINGLETON_IFACE(iQCIFSSofa, L"{20C9F775-A0EE-4c0c-B8FC-D3458F5FAE04} //iQCIFSSofa", vfs::iManager);

  virtual ~iQCIFSSofa(){}

  virtual vfs::String shareName() = 0;
  virtual vfs::cPtr<iChildLoader> createRootChildLoader() = 0;
};