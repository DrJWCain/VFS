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

// cSizeOnDisk.h
// reusable routines for determining the size of a file on disk

#pragma once

#include "..\iTreeResource.h"

class cSizeOnDisk
{
public:
  cSizeOnDisk(const vfs::cPtr<iTreeResource> pTreeResource);
  ~cSizeOnDisk(void);

  LARGE_INTEGER getSizeOnDisk(DWORD hiSize, DWORD loSize) const;

private:
  DWORD m_dwBytesPerCluster;
};
