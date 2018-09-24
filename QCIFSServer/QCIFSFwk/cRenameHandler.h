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

//cRenameHandler .h

#pragma once

#include "irenamehandler.h"

class cTreeResourceFactory;

class cRenameHandler : public iRenameHandler, public vfs::cRefCount
{
public:
  cRenameHandler(const vfs::cPtr<cTreeResourceFactory> pTreeResourceFactory
    , const vfs::String& sLoginName);
  ~cRenameHandler(void);

  //iRenameHandler
  DWORD Rename(const vfs::cPtr<iRename> pSourceRename, const vfs::String &newName);

  //public member functions
  vfs::String getLogin() const {return m_sLoginName;}

private:  //private member variables
  const vfs::cPtr<cTreeResourceFactory> m_pTreeResourceFactory;
  const vfs::String m_sLoginName;
};
