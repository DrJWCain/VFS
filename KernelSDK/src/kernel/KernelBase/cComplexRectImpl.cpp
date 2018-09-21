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

/*
**  
*/

#include "stdafx.h"
#include "QKernelBase.h"
#include "cComplexRectImpl.h"
#include "fatal.h"

using namespace vfs;




// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

cComplexRectImpl::cComplexRectImpl() :
  m_IsBoundsOutOfDate(false),
  m_IsBoundsDegenerate(true),
  m_IsOptimised(true),
  m_Bounds(0, 0, 0, 0),
  m_ClassVersion(kComplexRectImplClassVersion)
{
  // Do nothing.
}

// ---------------------------------------------------------------------------------------

cComplexRectImpl::cComplexRectImpl(const cRect & InitialFragment) :
  m_IsBoundsOutOfDate(false),
  m_IsBoundsDegenerate(InitialFragment.isDegenerate()),
  m_Bounds(InitialFragment),
  m_IsOptimised(true),
  m_ClassVersion(kComplexRectImplClassVersion)
{
  if (!m_IsBoundsDegenerate) m_Fragments.push_back(InitialFragment);
}

// ---------------------------------------------------------------------------------------

cComplexRectImpl::cComplexRectImpl(const std::vector<cRect> & InitialFragments) :
  m_IsBoundsOutOfDate(false),
  m_IsBoundsDegenerate(true),
  m_Bounds(0, 0, 0, 0),
  m_IsOptimised(false),
  m_ClassVersion(kComplexRectImplClassVersion)
{
  const std::vector<cRect>::size_type NumInitialFragments = InitialFragments.size();
  if (NumInitialFragments > 0)
  {
    m_Bounds = InitialFragments[0];
    m_Fragments.push_back(m_Bounds);
    for (std::vector<cRect>::size_type i = 1; i < NumInitialFragments; ++i)
    {
      const cRect & ThisFragment = InitialFragments[i];
      m_Bounds = m_Bounds.Union(ThisFragment);
      m_Fragments.push_back(ThisFragment);
    }
    m_IsBoundsDegenerate = m_Bounds.isDegenerate();
  }
}

// ---------------------------------------------------------------------------------------

cComplexRectImpl::cComplexRectImpl(const cComplexRectImpl & Other) :
  m_ClassVersion(kComplexRectImplClassVersion)
{
  operator=(Other);
}




// =======================================================================================
// === calcBounds ========================================================================
// =======================================================================================

const cRect & cComplexRectImpl::calcBounds() const
{
  if (m_IsBoundsOutOfDate)
  {
    switch (m_Fragments.size())
    {
      case 0 :
      {
        m_Bounds = cRect(0, 0, 0, 0);
        break;
      }
      case 1 :
      {
        m_Bounds = *getFragments().begin();
        break;
      }
      default :
      {
        RectList::const_iterator i = getFragments().begin();
        const RectList::const_iterator end = getFragments().end();
        m_Bounds = *i;
        ++i;
        do
        {
          m_Bounds = m_Bounds.Union(*i);
          ++i;
        } while (i != end);
        break;
      }
    }
    m_IsBoundsOutOfDate  = false;
    m_IsBoundsDegenerate = m_Bounds.isDegenerate();
  }

  return m_Bounds;
}




// =======================================================================================
// === operator= =========================================================================
// =======================================================================================

cComplexRectImpl & cComplexRectImpl::operator=(const cComplexRectImpl & Other)
{
#if defined(_DEBUG)
  QKERNELBASEVERIFY(Other.m_ClassVersion == kComplexRectImplClassVersion);
#endif
  m_IsBoundsOutOfDate  = Other.m_IsBoundsOutOfDate;
  m_IsBoundsDegenerate = Other.m_IsBoundsDegenerate;
  m_IsOptimised        = Other.m_IsOptimised;
  m_Bounds             = Other.m_Bounds;
  m_Fragments          = Other.m_Fragments;
  return *this;
}




// =======================================================================================
// === append ============================================================================
// =======================================================================================

void cComplexRectImpl::append(const cRect & NewFragment)
{
  // No point appending useless fragments to our list.
#if defined(_DEBUG)
  QKERNELBASEVERIFY(!NewFragment.isDegenerate());
#endif

  if (m_Fragments.size() == 0)
  {
    m_Bounds             = NewFragment;
    m_IsBoundsOutOfDate  = false;
    m_IsOptimised        = true;
  }
  else
  {
    m_IsBoundsOutOfDate  = true;
    m_IsOptimised        = false;
  }

  m_IsBoundsDegenerate = false;
  m_Fragments.push_back(NewFragment);
}

// ---------------------------------------------------------------------------------------

void cComplexRectImpl::append(const cComplexRectImpl & NewFragments)
{
#if defined(_DEBUG)
  QKERNELBASEVERIFY(NewFragments.m_ClassVersion == kComplexRectImplClassVersion);
#endif
  const RectList::const_iterator end(NewFragments.m_Fragments.end());
  for (RectList::const_iterator i = NewFragments.m_Fragments.begin(); i != end; ++i)
  {
    append(*i);
  }
}

// ---------------------------------------------------------------------------------------

void cComplexRectImpl::append(const cComplexRectImpl & NewFragments, const cXY & OrgOffset)
{
#if defined(_DEBUG)
  QKERNELBASEVERIFY(NewFragments.m_ClassVersion == kComplexRectImplClassVersion);
#endif
  const RectList::const_iterator end(NewFragments.m_Fragments.end());
  for (RectList::const_iterator i = NewFragments.m_Fragments.begin(); i != end; ++i)
  {
    append(cRect(i->org + OrgOffset, i->len));
  }
}