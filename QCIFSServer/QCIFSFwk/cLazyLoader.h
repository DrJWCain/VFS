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

//cLazyLoader.h

#pragma once

#include "iFactory.h"

template <typename T>
class cLazyLoader : public vfs::cRefCount
{
  const vfs::cPtr<QCifsFwk::iFactory<T> > m_pFactory;
  mutable vfs::cLock m_access;
  mutable T m_T;

public:
  cLazyLoader()
  {}

  cLazyLoader(const T& rT) : m_T(rT)
  {}

  cLazyLoader(const vfs::cPtr<cLazyLoader> pOther)
  : m_pFactory(pOther.isValid() ? pOther->m_pFactory : cPtr<iFactory<T> >())
  , m_T(pOther.isValid() ? pOther->m_T : T())
  {}

  cLazyLoader(const vfs::cPtr<QCifsFwk::iFactory<T> > pFactory) : m_pFactory(pFactory)
  {}

  T load() const
  {
    if (m_T.isNull() && m_pFactory.isValid())
    {
      vfs::cLockGuard lg(&m_access);
      if (m_T.isNull())
        m_T = m_pFactory->create();
    }

    return m_T;
  }
};