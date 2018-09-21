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

class iQCIFSIPC : public vfs::iManager
{
public:
  QDEFINE_SINGLETON_IFACE(iQCIFSIPC, L"{0A8EAEB6-FB67-488b-833A-1861EB9C3D0E} //iQCIFSIPC", vfs::iManager);

  virtual ~iQCIFSIPC(){}

  virtual vfs::cPtr<iTreeResourceFactory> getTreeResourceFactory() = 0;

  virtual void addShare(const vfs::String& sShare) = 0;
  virtual bool hasShare(const vfs::String& sShare) = 0;

  virtual void addDFSPath(const vfs::String& sDisplayPath, const vfs::String& sDFSPath) = 0;
  virtual vfs::String getDFSPath(const vfs::String& sDisplayPath) const = 0;
  virtual void removeDFSPath(const vfs::String& sDisplayPath) = 0;

  virtual vfs::String removeDFSPart(const vfs::String& psPath) const = 0;
};