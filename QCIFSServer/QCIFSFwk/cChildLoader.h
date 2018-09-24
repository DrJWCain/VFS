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

//cChildLoader.h

#pragma once

#include "iChildLoader.h"
#include "iChildLoaderVisitor.h"

class cChildLoader : public iChildLoader
{
  vfs::cLock m_access;
  vfs::cPtr<iChildLoaderVisitor> m_pChildListener;

public:
  void registerListener(const vfs::cPtr<iChildLoaderVisitor> pChildListener)
  {
    if (pChildListener.isValid())
    {
      vfs::cLockGuard lg(&m_access);
      if (m_pChildListener.isNull() || pChildListener != m_pChildListener)
        m_pChildListener = pChildListener;
    }
  }

  void clear() {if (m_pChildListener.isValid()) m_pChildListener->clear();}

protected:
  vfs::cPtr<iChildLoaderVisitor> getChildListener() {return m_pChildListener;}
};