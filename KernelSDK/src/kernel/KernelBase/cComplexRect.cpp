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
#include "cComplexRectImpl.h"
#include "fatal.h"

using namespace vfs;




// =======================================================================================
// === getReadImpl/getWriteImpl ==========================================================
// =======================================================================================

#if IS_COMPLEXRECT_COW

  #define getReadImpl(IT)   static_cast<const cComplexRectImpl &>((IT).m_Pimpl.read())
  #define getWriteImpl(IT)  static_cast<cComplexRectImpl &>((IT).m_Pimpl.write())

#else

  #define getReadImpl(IT)   reinterpret_cast<const cComplexRectImpl &>((IT).m_Pimpl[0])
  #define getWriteImpl(IT)  reinterpret_cast<cComplexRectImpl &>((IT).m_Pimpl[0])

#endif




// =======================================================================================
// === ADD ===============================================================================
// =======================================================================================

#define ADD(__X, __Y, __W, __H) NewFragmentsPimpl.append(cRect(__X, __Y, __W, __H))




// =======================================================================================
// === Construction ======================================================================
// =======================================================================================

cComplexRect::cComplexRect()
#if IS_COMPLEXRECT_COW
  : m_Pimpl(new cComplexRectImpl)
#endif
{
#if !IS_COMPLEXRECT_COW
  new (m_Pimpl /* dst */, sizeof(m_Pimpl)) cComplexRectImpl();
#endif
}

// ---------------------------------------------------------------------------------------

cComplexRect::cComplexRect(const cRect & InitialFragment)
#if IS_COMPLEXRECT_COW
  : m_Pimpl(new cComplexRectImpl(InitialFragment))
#endif
{
#if !IS_COMPLEXRECT_COW
  new (m_Pimpl /* dst */, sizeof(m_Pimpl)) cComplexRectImpl(InitialFragment);
#endif
}

// ---------------------------------------------------------------------------------------

cComplexRect::cComplexRect(const std::vector<cRect> & InitialFragments)
#if IS_COMPLEXRECT_COW
  : m_Pimpl(new cComplexRectImpl(InitialFragments))
#endif
{
#if !IS_COMPLEXRECT_COW
  new (m_Pimpl /* dst */, sizeof(m_Pimpl)) cComplexRectImpl(InitialFragments);
#endif
}

// ---------------------------------------------------------------------------------------

cComplexRect::cComplexRect(const cComplexRect & Other)
{
  *this = Other;
}

// ---------------------------------------------------------------------------------------

cComplexRect::~cComplexRect()
{
#if !IS_COMPLEXRECT_COW
  reinterpret_cast<cComplexRectImpl &>(m_Pimpl[0]).~cComplexRectImpl();
#endif
}





// =======================================================================================
// === operator= =========================================================================
// =======================================================================================

cComplexRect & QAPI cComplexRect::operator=(const cComplexRect & Other)
{
#if IS_COMPLEXRECT_COW
  m_Pimpl = Other.m_Pimpl;
#else
  new (m_Pimpl /* dst */, sizeof(m_Pimpl)) cComplexRectImpl(reinterpret_cast<const cComplexRectImpl &>(Other.m_Pimpl[0]));
#endif
  return *this;
}

cComplexRect & QAPI cComplexRect::operator=(const cRect & Other)
{
  cComplexRectImpl & Pimpl = getWriteImpl(*this);
  Pimpl.m_Fragments.clear();
  Pimpl.m_Fragments.push_back(Other);
  Pimpl.m_IsBoundsOutOfDate   = false;
  Pimpl.m_IsBoundsDegenerate  = Other.isDegenerate();
  Pimpl.m_Bounds              = Other;
  return *this;
}




// =======================================================================================
// === serialise =========================================================================
// =======================================================================================

String QAPI cComplexRect::serialise(void) const
{
  const cComplexRectImpl & Pimpl = getReadImpl(*this);

  StringStream Buffer;

  Buffer << L"ComplexRect : " << (unsigned int)Pimpl.m_Fragments.size() << L" fragments {";

  cComplexRectImpl::RectList::const_iterator end(Pimpl.m_Fragments.end());
  for (cComplexRectImpl::RectList::const_iterator i = Pimpl.m_Fragments.begin(); i != end; ++i)
  {
    Buffer << L" { " << i->org.x << L", " << i->org.y << L", " << i->len.x << L", " << i->len.y << L" }";
  }

  Buffer << L" }";

  return Buffer.str();
}




// =======================================================================================
// === calcBounds ========================================================================
// =======================================================================================

cRect QAPI cComplexRect::calcBounds(void) const
{
  return getReadImpl(*this).calcBounds();
}




// =======================================================================================
// === isDegenerate ======================================================================
// =======================================================================================

bool QAPI cComplexRect::isDegenerate(void) const
{
  const cComplexRectImpl & Pimpl = getReadImpl(*this);
  if (Pimpl.m_IsBoundsOutOfDate) (void)Pimpl.calcBounds();
  return Pimpl.m_IsBoundsDegenerate;
}




// =======================================================================================
// === countFragments ====================================================================
// =======================================================================================

unsigned int QAPI cComplexRect::countFragments(void) const
{
  return (unsigned int)getReadImpl(*this).m_Fragments.size();
}




// =======================================================================================
// === clearFragments ====================================================================
// =======================================================================================

bool QAPI cComplexRect::clearFragments(void)
{
  const unsigned int PrevSize = (unsigned int)getReadImpl(*this).m_Fragments.size();
  if (PrevSize > 0)
  {
    cComplexRectImpl & Pimpl = getWriteImpl(*this);
    Pimpl.m_Fragments.clear();
    Pimpl.m_IsBoundsOutOfDate   = false;
    Pimpl.m_IsBoundsDegenerate  = true;
//    Pimpl.m_IsOptimised         = true;
    Pimpl.m_Bounds              = cRect(0, 0, 0, 0);
    return true;
  }
  else
  {
    return false;
  }
}





// =======================================================================================
// === cFragmentIterator =================================================================
// =======================================================================================

cComplexRect::cFragmentIterator::cFragmentIterator(const cComplexRect * const ComplexRect)
{
#if defined(_DEBUG)
  QKERNELBASEVERIFY(sizeof(m_Pimpl) >= sizeof(cComplexRectFragmentIteratorImpl));
#endif

  // Use placement new to create an impl using pre-allocated memory.  Need to remember that we 
  // bceome responsible for manually calling the destructors of classes contained within
  // cComplexRectFragmentIteratorImpl.  See ~cFragmentIterator() below.
  cComplexRectFragmentIteratorImpl* ThisImpl = new (m_Pimpl) cComplexRectFragmentIteratorImpl();

  ThisImpl->m_ComplexRectImpl  = &getReadImpl(*ComplexRect);
  ThisImpl->m_Iterator         = ThisImpl->m_ComplexRectImpl->m_Fragments.begin();
  ThisImpl->m_IteratorPosition = 0;
}

cComplexRect::cFragmentIterator::~cFragmentIterator()
{
  cComplexRectFragmentIteratorImpl & ThisImpl = reinterpret_cast<cComplexRectFragmentIteratorImpl &>(*m_Pimpl);

  // Need to manually call destructors for classes allocated using placement new!!
  ThisImpl.~cComplexRectFragmentIteratorImpl();
}

bool /* is valid? */ cComplexRect::cFragmentIterator::gotoFragment(const unsigned int WhichFragment)
{
  cComplexRectFragmentIteratorImpl & ThisImpl = reinterpret_cast<cComplexRectFragmentIteratorImpl &>(*m_Pimpl);
  const bool IsValid = WhichFragment < ThisImpl.m_ComplexRectImpl->m_Fragments.size();
  if (IsValid)
  {
    ThisImpl.m_Iterator = ThisImpl.m_ComplexRectImpl->m_Fragments.begin();
    for (unsigned int j = 0; j < WhichFragment; ++j) ThisImpl.m_Iterator++;
  }
  ThisImpl.m_IteratorPosition = WhichFragment;
  return IsValid;
}

bool /* is valid? */ cComplexRect::cFragmentIterator::nextFragment()
{
  cComplexRectFragmentIteratorImpl & ThisImpl = reinterpret_cast<cComplexRectFragmentIteratorImpl &>(*m_Pimpl);
  ThisImpl.m_IteratorPosition++;
  const bool IsValid = ThisImpl.m_IteratorPosition < ThisImpl.m_ComplexRectImpl->m_Fragments.size();
  if (IsValid) ThisImpl.m_Iterator++;
  return IsValid;
}

bool /* is valid? */ cComplexRect::cFragmentIterator::prevFragment()
{
  cComplexRectFragmentIteratorImpl & ThisImpl = reinterpret_cast<cComplexRectFragmentIteratorImpl &>(*m_Pimpl);
  ThisImpl.m_IteratorPosition--;
  const bool IsValid = ThisImpl.m_IteratorPosition >= 0;
  if (IsValid) ThisImpl.m_Iterator--;
  return IsValid;
}

cRect * cComplexRect::cFragmentIterator::operator->() const
{
  const cComplexRectFragmentIteratorImpl & ThisImpl = reinterpret_cast<const cComplexRectFragmentIteratorImpl &>(*m_Pimpl);
  return ThisImpl.m_Iterator.operator->();
}

cRect & cComplexRect::cFragmentIterator::operator*() const
{
  const cComplexRectFragmentIteratorImpl & ThisImpl = reinterpret_cast<const cComplexRectFragmentIteratorImpl &>(*m_Pimpl);
  return ThisImpl.m_Iterator.operator*();
}




// =======================================================================================
// === optimise ==========================================================================
// =======================================================================================

bool QAPI cComplexRect::optimise(void)
{
  bool WasAnyOptimisationDone = false;

  const cComplexRectImpl & ReadPimpl = getReadImpl(*this);

  if (ReadPimpl.m_Fragments.size() > 1 && !ReadPimpl.isDegenerate()/* && !ReadPimpl.m_IsOptimised*/)
  {
    cComplexRectImpl & Pimpl = getWriteImpl(*this);
    cComplexRectImpl::RectList & Fragments = Pimpl.m_Fragments;
    cComplexRectImpl::RectList::iterator i, j, end = Fragments.end();

    // Turn rects from Internal Top Left + Size to Internal Top Left + External Bottom Right.
    for (i = Fragments.begin(); i != end; ++i)
    {
      i->len += i->org; // len is now External Bottom Right
    }

    bool WasAnyOptimisationDoneOnThisPass;
    do
    {
      WasAnyOptimisationDoneOnThisPass = false;

      // Merges pairs sharing same vert edge.
      for (i = Fragments.begin(); i != end; ++i)
      {
        if (*i != cRect(0, 0, 0, 0) /* ignore if marked for removal */)
        {
          for (j = Fragments.begin(); j != end; ++j)
          {
            if (*j != cRect(0, 0, 0, 0) /* ignore if marked for removal */)
            {
              if (*j != *i &&
                  i->len.x == j->org.x /* Is I right same as J left? Remember len is bottom right NOT size */ &&
                  i->org.y == j->org.y && i->len.y == j->len.y /* Are we vertically aligned? */)
              {
                // Excellent, we can merge these two fragments.
                i->len.x = j->len.x;
                WasAnyOptimisationDoneOnThisPass = true;
                *j = cRect(0, 0, 0, 0); // Mark for removal
              }
            }
          }
        }
      }

      // Merges pairs sharing same horiz edge.
      for (i = Fragments.begin(); i != end; ++i)
      {
        if (*i != cRect(0, 0, 0, 0) /* ignore if marked for removal */)
        {
          for (j = Fragments.begin(); j != end; ++j)
          {
            if (*j != cRect(0, 0, 0, 0) /* ignore if marked for removal */)
            {
              if (j != i &&
                  i->len.y == j->org.y /* Is I bottom same as J top? Remember len is bottom right NOT size */ &&
                  i->org.x == j->org.x && i->len.x == j->len.x /* Are we horiz aligned? */)
              {
                // Excellent, we can merge these two fragments.
                i->len.y = j->len.y;
                WasAnyOptimisationDoneOnThisPass = true;
                *j = cRect(0, 0, 0, 0); // Mark for removal
              }
            }
          }
        }
      }

      if (WasAnyOptimisationDoneOnThisPass)
      {
        // Remove all rects that are marked for removal
        for (i = Fragments.begin(); i != Fragments.end();)
        {
          if (*i == cRect(0, 0, 0, 0))
          {
            i = Fragments.erase(i);
          }
          else
          {
            ++i;
          }
        }
        end = Fragments.end();

        Pimpl.m_IsBoundsOutOfDate= true;
      }
      
      if (WasAnyOptimisationDoneOnThisPass) WasAnyOptimisationDone = true;
    } while (WasAnyOptimisationDoneOnThisPass);

    // Turn rects from Internal Top Left + External Bottom Right back to Internal Top Left + Size.
    end = Fragments.end();
    for (i = Fragments.begin(); i != end; ++i)
    {
      i->len -= i->org; // len is now Size
    }

    Pimpl.m_IsOptimised = true;
  }

  return WasAnyOptimisationDone;
}




// =======================================================================================
// === amalgamate ========================================================================
// =======================================================================================

bool QAPI cComplexRect::amalgamate(
  const float        BigAreaRatio,
  const unsigned int SmallAreaThreshold)
{
  bool WasAnyAmalgamationDone = false;

  const cComplexRectImpl & ReadPimpl = getReadImpl(*this);

  if (ReadPimpl.m_Fragments.size() > 1 && !ReadPimpl.isDegenerate())
  {
    cComplexRectImpl & Pimpl = getWriteImpl(*this);
    cComplexRectImpl::RectList & Fragments = Pimpl.m_Fragments;
    cComplexRectImpl::RectList::iterator end(Fragments.end());

    for (cComplexRectImpl::RectList::iterator i = Fragments.begin(); i != end; ++i)
    {
      for (cComplexRectImpl::RectList::iterator j = i; j != end; ++j)
      {
        if (j != i)
        {
          const unsigned int AreaOfFragmentI = i->len.x * i->len.y;
          const unsigned int AreaOfFragmentJ = j->len.x * j->len.y;
          const cRect        Union           (i->Union(*j));
          const unsigned int AreaOfUnion     = Union.len.x * Union.len.y;
          const unsigned int UnionLongestEdgeLength = Max<unsigned int>(Union.len.x, Union.len.y);

          if (UnionLongestEdgeLength < SmallAreaThreshold)
          {
            *i = Union;
            *j = cRect(0, 0, 0, 0); // Mark for removal
            WasAnyAmalgamationDone = true;
          }
          else
          {
            const float RatioA = (float)AreaOfUnion / (float)(AreaOfFragmentI + AreaOfFragmentJ);
            if (RatioA > 0.0f)
            {
              const float RatioB = 100.0f / RatioA;

              if (RatioB > BigAreaRatio)
              {
                *i = Union;
                *j = cRect(0, 0, 0, 0); // Mark for removal
                WasAnyAmalgamationDone = true;
              }
            }
          }
        }
      }
    }
    
    if (WasAnyAmalgamationDone)
    {
      // Remove all rects that are marked for removal
      for (cComplexRectImpl::RectList::iterator i = Fragments.begin(); i != Fragments.end();)
      {
        if (*i == cRect(0, 0, 0, 0))
        {
          i = Fragments.erase(i);
        }
        else
        {
          ++i;
        }
      }
    }
  }
  
  return WasAnyAmalgamationDone;
}




// =======================================================================================
// === isContaining ======================================================================
// =======================================================================================

bool QAPI cComplexRect::isContaining(const cRect & Other) const
{
  bool IsContained = false;

  const cComplexRectImpl & Pimpl = getReadImpl(*this);

  if (!Pimpl.isDegenerate())
  {
    if (Pimpl.calcBounds().isOverlapped(Other))
    {
      cComplexRect Remainder(Other);
      Remainder.subtract(*this);
      IsContained = Remainder.isDegenerate();
    }
  }

  return IsContained;
}

// ---------------------------------------------------------------------------------------

bool QAPI cComplexRect::isContaining(const cComplexRect & Other) const
{
  bool IsContained = false;

  const cComplexRectImpl & Pimpl      = getReadImpl(*this);
  const cComplexRectImpl & OtherPimpl = getReadImpl(Other);
  if (!Pimpl.isDegenerate() && !OtherPimpl.isDegenerate())
  {
    if (Pimpl.calcBounds().isOverlapped(OtherPimpl.calcBounds()))
    {
      IsContained = true;
      const cComplexRectImpl::RectList::const_iterator end(OtherPimpl.m_Fragments.end());
      for (cComplexRectImpl::RectList::const_iterator i = OtherPimpl.m_Fragments.begin(); i != end; ++i)
      {
        if (!isContaining(*i))
        {
          IsContained = false;
          break;
        }
      }
    }
  }

  return IsContained;
}

// ---------------------------------------------------------------------------------------

bool QAPI cComplexRect::isContaining(const cComplexRect & Other, const cXY & OrgOffset) const
{
  bool IsContained = false;

  const cComplexRectImpl & Pimpl      = getReadImpl(*this);
  const cComplexRectImpl & OtherPimpl = getReadImpl(Other);
  if (!Pimpl.isDegenerate() && !OtherPimpl.isDegenerate())
  {
    const cRect OtherBounds(OtherPimpl.calcBounds());
    if (Pimpl.calcBounds().isOverlapped(cRect(OtherBounds.org + OrgOffset, OtherBounds.len)))
    {
      IsContained = true;
      const cComplexRectImpl::RectList::const_iterator end(OtherPimpl.m_Fragments.end());
      for (cComplexRectImpl::RectList::const_iterator i = OtherPimpl.m_Fragments.begin(); i != end; ++i)
      {
        if (!isContaining(cRect(i->org + OrgOffset, i->len)))
        {
          IsContained = false;
          break;
        }
      }
    }
  }

  return IsContained;
}




// =======================================================================================
// === isOverlapping =====================================================================
// =======================================================================================

bool QAPI cComplexRect::isOverlapping(const cRect & Other) const
{
  bool IsOverlapped = false;

  const cComplexRectImpl & Pimpl = getReadImpl(*this);
  if (!Pimpl.isDegenerate() && !Other.isDegenerate())
  {
    if (Pimpl.calcBounds().isOverlapped(Other))
    {
      if (Pimpl.m_Fragments.size() == 1)
      {
        IsOverlapped = true;
      }
      else
      {
        const cComplexRectImpl::RectList::const_iterator end(Pimpl.m_Fragments.end());
        for (cComplexRectImpl::RectList::const_iterator i = Pimpl.m_Fragments.begin(); i != end; ++i)
        {
          if (i->isOverlapped(Other))
          {
            IsOverlapped = true;
            break;
          }
        }
      }
    }
  }

  return IsOverlapped;
}

// ---------------------------------------------------------------------------------------

bool QAPI cComplexRect::isOverlapping(const cComplexRect & Other) const
{
  bool IsOverlapped = false;

  const cComplexRectImpl & Pimpl      = getReadImpl(*this);
  const cComplexRectImpl & OtherPimpl = getReadImpl(Other);
  if (!Pimpl.isDegenerate() && !OtherPimpl.isDegenerate())
  {
    if (Pimpl.calcBounds().isOverlapped(OtherPimpl.calcBounds()))
    {
      if (Pimpl.m_Fragments.size() == 1 && OtherPimpl.m_Fragments.size() == 1)
      {
        IsOverlapped = true;
      }
      else
      {
        const cComplexRectImpl::RectList::const_iterator end(OtherPimpl.m_Fragments.end());
        for (cComplexRectImpl::RectList::const_iterator i = OtherPimpl.m_Fragments.begin(); i != end; ++i)
        {
          if (isOverlapping(*i))
          {
            IsOverlapped = true;
            break;
          }
        }
      }
    }
  }

  return IsOverlapped;
}

// ---------------------------------------------------------------------------------------

bool QAPI cComplexRect::isOverlapping(const cComplexRect & Other, const cXY & OrgOffset) const
{
  bool IsOverlapped = false;

  const cComplexRectImpl & Pimpl      = getReadImpl(*this);
  const cComplexRectImpl & OtherPimpl = getReadImpl(Other);
  if (!Pimpl.isDegenerate() && !OtherPimpl.isDegenerate())
  {
    const cRect OtherBounds(OtherPimpl.calcBounds());
    if (Pimpl.calcBounds().isOverlapped(cRect(OtherBounds.org + OrgOffset, OtherBounds.len)))
    {
      if (Pimpl.m_Fragments.size() == 1 && OtherPimpl.m_Fragments.size() == 1)
      {
        IsOverlapped = true;
      }
      else
      {
        const cComplexRectImpl::RectList::const_iterator end(OtherPimpl.m_Fragments.end());
        for (cComplexRectImpl::RectList::const_iterator i = OtherPimpl.m_Fragments.begin(); i != end; ++i)
        {
          if (isOverlapping(cRect(i->org + OrgOffset, i->len)))
          {
            IsOverlapped = true;
            break;
          }
        }
      }
    }
  }

  return IsOverlapped;
}




// =======================================================================================
// === or ================================================================================
// =======================================================================================

cComplexRect & QAPI cComplexRect::or(const cRect & Other)
{
  if (!Other.isDegenerate())// && !isContaining(Other))
  {
    cComplexRectImpl & Pimpl = getWriteImpl(*this);

    subtract(Other);
    Pimpl.append(Other);
  }
  return *this;
}

// ---------------------------------------------------------------------------------------

cComplexRect & QAPI cComplexRect::orNotOverlapping(const cRect & Other)
{
#if defined(_DEBUG)
  QKERNELBASEVERIFY(!isOverlapping(Other));
#endif
  if (!Other.isDegenerate())// && !isContaining(Other))
  {
    cComplexRectImpl & Pimpl = getWriteImpl(*this);

    Pimpl.append(Other);
  }
  return *this;
}

// ---------------------------------------------------------------------------------------

cComplexRect & QAPI cComplexRect::or(const cComplexRect & Other)
{
  if (!Other.isDegenerate())// && !isContaining(Other))
  {
    subtract(Other);
    getWriteImpl(*this).append(getReadImpl(Other));
  }
  return *this;
}

// ---------------------------------------------------------------------------------------

cComplexRect & QAPI cComplexRect::or(const cComplexRect & Other, const cXY & OrgOffset)
{
  if (!Other.isDegenerate())// && !isContaining(Other, OrgOffset))
  {
    subtract(Other, OrgOffset);
    getWriteImpl(*this).append(getReadImpl(Other), OrgOffset);
  }
  return *this;
}




// =======================================================================================
// === subtract ==========================================================================
// =======================================================================================

cComplexRect & QAPI cComplexRect::subtract(const cComplexRect & Other)
{
  if (isOverlapping(Other))
  {
    const cComplexRectImpl & OtherPimpl = getReadImpl(Other);

    const cComplexRectImpl::RectList::const_iterator end(OtherPimpl.m_Fragments.end());
    for (cComplexRectImpl::RectList::const_iterator i = OtherPimpl.m_Fragments.begin(); i != end; ++i)
    {
      subtract(*i);
    }
  }
  return *this;
}

// ---------------------------------------------------------------------------------------

cComplexRect & QAPI cComplexRect::subtract(const cComplexRect & Other, const cXY & OrgOffset)
{
  if (isOverlapping(Other, OrgOffset))
  {
    const cComplexRectImpl & OtherPimpl = getReadImpl(Other);
    const cComplexRectImpl::RectList::const_iterator end(OtherPimpl.m_Fragments.end());
    for (cComplexRectImpl::RectList::const_iterator i = OtherPimpl.m_Fragments.begin(); i != end; ++i)
    {
      (void)subtract(cRect(i->org + OrgOffset, i->len));
      if (getReadImpl(*this).isDegenerate()) break;
    }
  }
  return *this;
}

// ---------------------------------------------------------------------------------------

cComplexRect & QAPI cComplexRect::subtract(const cRect & Other)
{
  // If fragment to clear or this Complex Rect is degenerate then no point doing anything.
  if (getReadImpl(*this).isDegenerate() || Other.isDegenerate())
  {
    // Do nothing.
  }
  else
  {
    cComplexRectImpl & Pimpl = getWriteImpl(*this);

    const int & cx = Other.org.x;
    const int & cy = Other.org.y;
    const int & cw = Other.len.x;
    const int & ch = Other.len.y;

    cComplexRect NewFragments;
    cComplexRectImpl & NewFragmentsPimpl = getWriteImpl(NewFragments);
/*
    bool trace = false;
    if (Pimpl.m_Fragments.size() == 102)
    {
      trace = true;
    }
    if (trace) QTRACE((L"end %s", Pimpl.m_Fragments.end().serialise().c_str()));
*/
    int n = 0;
    for (cComplexRectImpl::RectList::iterator i = Pimpl.m_Fragments.begin(); i != Pimpl.m_Fragments.end();)
    {
//      if (trace) QTRACE((L"%d i %s          End (%s)", n, i.serialise().c_str(), Pimpl.m_Fragments.end().serialise().c_str()));
      n++;

      bool IsRemoving = false;

      int & ix = i->org.x;
      int & iy = i->org.y;
      int & iw = i->len.x;
      int & ih = i->len.y;

      const int cr = cx + cw;
      const int ir = ix + iw;
      if (cr > ix && cx < ir)     // Overlapping horizontally?
      {
        const int cb = cy + ch;
        const int ib = iy + ih;
        if (cb > iy && cy < ib)   // Overlapping veritically?
        {
          const unsigned int OverlapType =
            ( (cy <= iy) ? 1 << 3 : 0 ) |
            ( (cb >= ib) ? 1 << 2 : 0 ) |
            ( (cx <= ix) ? 1 << 1 : 0 ) |
            ( (cr >= ir) ? 1 << 0 : 0 );

          // The macros cx, cy, cw, ch, cr, cb  are        // I## Fragment to clear
          // defined as being the dimensions of the        // ###
          // fragment to clear and ix, iy, iw, ih, ir and  // ###
          // ib as being the dimensions of an existing
          // fragment in the list.                         // C-+ Current exisiting
                                                           // | |  fragment
                                                           // +-+ 
          switch (OverlapType)
          {
            case 0: // REMOVES internal portion.         // Sub- C-------+
            {                                            // div. |   0   |
                //Pimpl.m_IsOptimised = false;
                ADD(ix, cy, cx - ix, ch);                //  1   +-I####-+
                ADD(cr, cy, ir - cr, ch);                //  2   |1#####2|
                ADD(ix, cb, iw, ib - cb);                //  3   +-#####-+
                ih = cy - iy;                            //  0   |   3   |
                break;                                   //      +-------+
            }
            case 1: // REMOVES right middle portion.
            {   
                //Pimpl.m_IsOptimised = false;
                                                         //      C-------+
                                                         //      |   0   |
                ADD(ix, cy, cx - ix, ch);                //      +----I####
                ADD(ix, cb, iw, ib - cb);                //  2   |  1 #####
                ih = cy - iy;                            //  0   +----#####
                                                         //      |   2   |
                break;                                   //      +-------+
            }
            case 2: // REMOVES left middle portion.
            {
                //Pimpl.m_IsOptimised = false;
                                                         //      C-------+
                                                         //      |   0   |
                ADD(cr, cy, ir - cr, ch);                //  1  I####----+
                ADD(ix, cb, iw, ib - cb);                //  2  ##### 1  |
                ih = cy - iy;                            //  0  #####----+
                                                         //      |   2   |
                break;                                   //      +-------+
            }
            case 3: // REMOVES middle horiz portion.
            {
                //Pimpl.m_IsOptimised = false;
                                                         //      C-------+
                                                         //      |   0   |
                ADD(ix, cb, iw, ib - cb);                //  1  I##########
                ih = cy - iy;                            //  0  ###########
                                                         //     ###########
                                                         //      |   1   |
                break;                                   //      +-------+
            }
            case 4: // REMOVES bottom middle portion.
            {
                //Pimpl.m_IsOptimised = false;
                                                         //      C-------+
                                                         //      |       |
                ADD(ix, cy, cx - ix, ib - cy);           //  1   |   0   |
                ADD(cr, cy, ir - cr, ib - cy);           //  2   |       |
                ih = cy - iy;                            //  0   +-I####-+
                                                         //      |1#####2|
                break;                                   //      +-#####-+
            }
            case 5: // REMOVES bottom right portion.
            {
                //Pimpl.m_IsOptimised = false;
                                                         //      C-------+
                                                         //      |       |
                ADD(ix, cy, cx - ix, ib - cy);           //  1   |   0   |
                ih = cy - iy;                            //  0   |       |
                                                         //      +----I####
                                                         //      |  1 #####
                break;                                   //      +----#####
            }
            case 6: // REMOVES bottom left portion.
            {
                //Pimpl.m_IsOptimised = false;
                                                         //      C-------+
                                                         //      |       |
                ADD(cr, cy, ir - cr, ib - cy);           //  1   |   0   |
                ih = cy - iy;                            //  0   |       |
                                                         //     I####----+
                                                         //     ##### 1  |
                break;                                   //     #####----+
            }
            case 7: // REMOVES all bottom portion.
            {
                //Pimpl.m_IsOptimised = false;
                                                         //      C-------+
                                                         //      |       |
                ih = cy - iy;                            //  0   |   0   |
                                                         //      |       |
                                                         //     I##########
                                                         //     ###########
                break;                                   //     ###########
            }
            case 8: // REMOVES top middle portion.
            {
                //Pimpl.m_IsOptimised = false;
                                                         //      C-I####-+
                                                         //      |1#####2|
                ADD(ix, iy, cx - ix, cb - iy);           //  1   +-#####-+
                ADD(cr, iy, ir - cr, cb - iy);           //  2   |       |
                ih = ib - cb;       iy = cb;             //  0   |   0   |
                                                         //      |       |
                break;                                   //      +-------+
            }
            case 9: // REMOVES top right corner.
            {
                //Pimpl.m_IsOptimised = false;
                                                         //      C----I####
                                                         //      |  1 #####
                ADD(ix, iy, cx - ix, cb - iy);           //  1   +----#####
                ih = ib - cb;       iy = cb;             //  0   |       |
                                                         //      |   0   |
                                                         //      |       |
                break;                                   //      +-------+
            }
            case 10: // REMOVES top left corner.
            {
                //Pimpl.m_IsOptimised = false;
                                                         //     I####----+
                                                         //     ##### 1  |
                ADD(cr, iy, ir - cr, cb - iy);           //  1  #####----+
                ih = ib - cb;       iy = cb;             //  0   |       |
                                                         //      |   0   |
                                                         //      |       |
                break;                                   //      +-------+
            }
            case 11:// REMOVES all top edge.
            {
                //Pimpl.m_IsOptimised = false;
                                                         //     I##########
                                                         //     ###########
                ih = ib - cb;       iy = cb;             //  0  ###########
                                                         //      |       |
                                                         //      |   0   |
                                                         //      |       |
                break;                                   //      +-------+
            }
            case 12: // REMOVES vertical down middle.
            {
                //Pimpl.m_IsOptimised = false;
                                                         //      C-I####-+
                                                         //      | ##### |
                ADD(cr, iy, ir - cr, ih);                //  1   | ##### |
                iw = cx - ix;                            //  0   |0#####1|
                                                         //      | ##### |
                                                         //      | ##### |
                break;                                   //      +-#####-+
            }
            case 13:// REMOVES all right edge.
            {
                //Pimpl.m_IsOptimised = false;
                                                         //      C----I####
                                                         //      |    #####
                iw = cx - ix;                            //  0   |    #####
                                                         //      |  0 #####
                                                         //      |    #####
                                                         //      |    #####
                break;                                   //      +----#####
            }
            case 14: // REMOVES all left edge.
            {
                //Pimpl.m_IsOptimised = false;
                                                         //     I####----+
                                                         //     #####    |
                iw = ir - cr;       ix = cr;             //  0  #####    |
                                                         //     ##### 0  |
                                                         //     #####    |
                                                         //     #####    |
                break;                                   //     #####----+
            }
            case 15: // REMOVES all.
            {
                                                         //     I##########
                                                         //     ###########
                IsRemoving = true;                       //     ###########
                                                         //     ###########
                                                         //     ###########
                                                         //     ###########
                break;                                   //     ###########
            }
          }
        }

        Pimpl.m_IsBoundsOutOfDate = true;
      }

      if (IsRemoving)
      {
        i = Pimpl.m_Fragments.erase(i);
      }
      else
      {
        ++i;
      }
    }

    if (NewFragmentsPimpl.m_Fragments.size() > 0)
    {
      Pimpl.append(NewFragmentsPimpl);
    }
  }

  return *this;
}




// =======================================================================================
// === and ===============================================================================
// =======================================================================================

cComplexRect & QAPI cComplexRect::and(const cComplexRect & Other)
{
  if (getReadImpl(*this).isDegenerate())
  {
    clearFragments();
  }
  else
  {
    const cComplexRectImpl & OtherPimpl = getReadImpl(Other);

    const cComplexRect OriginalBackup(*this);
    cComplexRect &     Accumulator = *this;

    Accumulator.clearFragments();
    const cComplexRectImpl::RectList::const_iterator end(OtherPimpl.m_Fragments.end());
    for (cComplexRectImpl::RectList::const_iterator i = OtherPimpl.m_Fragments.begin(); i != end; ++i)
    {
      cComplexRect Scratch(OriginalBackup);
      Scratch.and(*i);
      getWriteImpl(Accumulator).append(getReadImpl(Scratch));
    }
  }
  return *this;
}

// ---------------------------------------------------------------------------------------

cComplexRect & QAPI cComplexRect::and(const cComplexRect & Other, const cXY & OrgOffset)
{
  if (getReadImpl(*this).isDegenerate())
  {
    clearFragments();
  }
  else
  {
    const cComplexRectImpl & OtherPimpl = getReadImpl(Other);

    const cComplexRect  OriginalBackup(*this);
    cComplexRect &      Accumulator = *this;

    Accumulator.clearFragments();
    const cComplexRectImpl::RectList::const_iterator end(OtherPimpl.m_Fragments.end());
    for (cComplexRectImpl::RectList::const_iterator i = OtherPimpl.m_Fragments.begin(); i != end; ++i)
    {
      cComplexRect Scratch(OriginalBackup);
      Scratch.and(cRect(i->org + OrgOffset, i->len));
      getWriteImpl(Accumulator).append(getReadImpl(Scratch));
    }
  }
  return *this;
}

// ---------------------------------------------------------------------------------------

cComplexRect & QAPI cComplexRect::and(const cRect & Other)
{
  // If framgment to and or this Complex Rect is degenerate then we end up with a
  // degenerate result.
  if (getReadImpl(*this).isDegenerate() || Other.isDegenerate())
  {
    clearFragments();
  }
  else
  {
    cComplexRectImpl & Pimpl = getWriteImpl(*this);

    const int & cx = Other.org.x;
    const int & cy = Other.org.y;
    const int & cw = Other.len.x;
    const int & ch = Other.len.y;

    for (cComplexRectImpl::RectList::iterator i = Pimpl.m_Fragments.begin(); i != Pimpl.m_Fragments.end();)
    {
      bool IsRemoving = false;

      int & ix = i->org.x;
      int & iy = i->org.y;
      int & iw = i->len.x;
      int & ih = i->len.y;

      // We'll define cx, cy, cw, ch, cr, cb as being the dimensions of the 'keep' rect
      // and ix, iy, iw, ih, ir and ib as being the dimensions of the current fragment.

      const int cr = cx + cw;
      const int ir = ix + iw;

      // If Rect we're ANDing overlaps this Rect aleady in the ComplexRect then
      // we'll have to subdivide the the exiting one. Else, just discard it!
      if (cr > ix && cx < ir)
      {
        const int cb = cy + ch;
        const int ib = iy + ih;

        if (cb > iy && cy < ib)
        {
          // But first we have to work out _how_ the new Rect overlaps the
          // existing one.
          const unsigned OverlapType =
            ( (cy <= iy) ? 1 << 3 : 0 ) |
            ( (cb >= ib) ? 1 << 2 : 0 ) |
            ( (cx <= ix) ? 1 << 1 : 0 ) |
            ( (cr >= ir) ? 1 << 0 : 0 );

          switch (OverlapType)
          {
            case 0: // KEEPS centre portion           
            {
                Pimpl.m_IsBoundsOutOfDate = true;
                //Pimpl.m_IsOptimised = false;
                                                                    //  I---+ 
                ix = cx;            iy = cy;                        //  |C##| 
                iw = cw;            ih = ch;                        //  |###|
                break;                                              //  +---+
            }
            case 1: // KEEPS right middle portion.
            {
                Pimpl.m_IsBoundsOutOfDate = true;
                //Pimpl.m_IsOptimised = false;
                                                                    //  I---+
                ix = cx;            iy = cy;                        //  |  C##
                iw = ir - cx;       ih = ch;                        //  |  ###
                break;                                              //  +---+
            }
            case 2: // KEEPS left middle portion.
            {
                 Pimpl.m_IsBoundsOutOfDate = true;
                 //Pimpl.m_IsOptimised = false;
                                                                   //  I---+
                                    iy = cy;                        // C##  |
                iw = cr - ix;       ih = ch;                        // ###  |
                break;                                              //  +---+
            }
            case 3: // KEEPS all middle horiz portion.
            {
                Pimpl.m_IsBoundsOutOfDate = true;
                //Pimpl.m_IsOptimised = false;
                                                                   //  I---+
                                    iy = cy;                        // C######
                                    ih = ch;                        // #######
                break;                                              //  +---+
            }
            case 4: // KEEPS bottom middle portion.
            {
                Pimpl.m_IsBoundsOutOfDate = true;
                //Pimpl.m_IsOptimised = false;
                                                                    //  I---+
                ix = cx;            iy = cy;                        //  |   |
                iw = cw;            ih = ib - cy;                   //  |C##|
                break;                                              //  +###+
            }                                                       //   ###
            case 5: // KEEPS bottom right portion.
            {
                Pimpl.m_IsBoundsOutOfDate = true;
                //Pimpl.m_IsOptimised = false;
                                                                    //  I---+
                ix = cx;            iy = cy;                        //  |   |
                iw = ir - cx;       ih = ib - cy;                   //  |  C##
                break;                                              //  +--###
            }                                                       //     ###
            case 6: // KEEPS bottom left portion.
            {
                Pimpl.m_IsBoundsOutOfDate = true;
                //Pimpl.m_IsOptimised = false;
                                                                    //  I---+
                                    iy = cy;                        //  |   |
                iw = cr - ix;       ih = ib - cy;                   // C##  |
                break;                                              // ###--+
            }                                                       // ###
            case 7: // KEEPS all bottom portion.
            {
                Pimpl.m_IsBoundsOutOfDate = true;
                //Pimpl.m_IsOptimised = false;
                                                                    //  I---+
                                    iy = cy;                        //  |   |
                                    ih = ib - cy;                   // C######
                break;                                              // #######
            }
            case 8: // KEEPS top middle portion.
            {
                Pimpl.m_IsBoundsOutOfDate = true;
                //Pimpl.m_IsOptimised = false;
                                                                    //   C##
                ix = cx;                                            //  T###+
                iw = cw;            ih = cb - iy;                   //  |###|
                break;                                              //  |   |
            }                                                       //  +---+
            case 9: // KEEPS top right corner.
            {
                Pimpl.m_IsBoundsOutOfDate = true;
                //Pimpl.m_IsOptimised = false;
                                                                    //     C##
                ix = cx;                                            //  I--###
                iw = ir - cx;       ih = cb - iy;                   //  |  ###
                break;                                              //  |   |
            }                                                       //  +---+
            case 10: // KEEPS top left corner.
            {
                Pimpl.m_IsBoundsOutOfDate = true;
                //Pimpl.m_IsOptimised = false;
                                                                    // C##
                iw = cr - ix;       ih = cb - iy;                   // ###--+
                break;                                              // ###  |
            }                                                       //  +---+
            case 11: // KEEPS all top edge.
            {
                Pimpl.m_IsBoundsOutOfDate = true;
                //Pimpl.m_IsOptimised = false;
                                                                    // C######
                                    ih = cb - iy;                   // #######
                break;                                              //  |   |
            }                                                       //  +---+
            case 12: // KEEPS vertical portion down middle.
            {
                Pimpl.m_IsBoundsOutOfDate = true;
                //Pimpl.m_IsOptimised = false;
                                                                    //   C##
                ix = cx;                                            // I-###-+
                iw = cw;                                            // | ### |
                break;                                              // +-###-+
            }                                                       //   ###
            case 13: // KEEPS all right edge.
            {
                Pimpl.m_IsBoundsOutOfDate = true;
                //Pimpl.m_IsOptimised = false;
                                                                    //      C##
                ix = cx;                                            // I----###
                iw = ir - cx;                                       // |    ###
                break;                                              // +----###
            }                                                       //      ###
            case 14: // KEEPS all left edge.
            {
                Pimpl.m_IsBoundsOutOfDate = true;
                //Pimpl.m_IsOptimised = false;
                                                                    // C##
                iw = cr - ix;                                       // ###----+
                break;                                              // ###    |
            }                                                       // ###----+
                                                                    // ###
            case 15: // KEEPS all.
            {
                                                                    // C#######
                break;                                              // ########
            }                                                       // ########
          }
         }
        else
        {
          IsRemoving = true;
        }
      }
      else
      {
        IsRemoving = true;
      }

      if (IsRemoving)
      {
        Pimpl.m_IsBoundsOutOfDate = true;

        i = Pimpl.m_Fragments.erase(i);
      }
      else
      {
        ++i;
      }
    }
  }

  return *this;
}




// =======================================================================================
// === not ===============================================================================
// =======================================================================================

cComplexRect & QAPI cComplexRect::not(const cRect & Other)
{
  cComplexRectImpl & Pimpl = getWriteImpl(*this);

  cComplexRect Inverse(Other);

  const cComplexRectImpl::RectList::const_iterator end(Pimpl.m_Fragments.end());
  for (cComplexRectImpl::RectList::iterator i = Pimpl.m_Fragments.begin(); i != end; ++i)
  {
    Inverse.subtract(*i);
  }
  subtract(Other);
  Pimpl.append(getReadImpl(Inverse));

  return *this;
}

// ---------------------------------------------------------------------------------------

cComplexRect & QAPI cComplexRect::not(const cComplexRect & Other)
{
  cComplexRectImpl & Pimpl = getWriteImpl(*this);

  cComplexRect Inverse(Other);

  const cComplexRectImpl::RectList::const_iterator end(Pimpl.m_Fragments.end());
  for (cComplexRectImpl::RectList::iterator i = Pimpl.m_Fragments.begin(); i != end; ++i)
  {
    Inverse.subtract(*i);
  }
  subtract(Other);
  Pimpl.append(getReadImpl(Inverse));

  return *this;
}

// ---------------------------------------------------------------------------------------

cComplexRect & QAPI cComplexRect::not(const cComplexRect & Other, const cXY & OrgOffset)
{
  cComplexRectImpl & Pimpl = getWriteImpl(*this);

  cComplexRect Inverse(Other);

  const cComplexRectImpl::RectList::const_iterator end(Pimpl.m_Fragments.end());
  for (cComplexRectImpl::RectList::iterator i = Pimpl.m_Fragments.begin(); i != end; ++i)
  {
    Inverse.subtract(cRect(i->org + OrgOffset, i->len));
  }
  subtract(Other);
  Pimpl.append(getReadImpl(Inverse));

  return *this;
}




// =======================================================================================
// === incOrg ============================================================================
// =======================================================================================

cComplexRect & QAPI cComplexRect::incOrg(const cXY Inc)
{
  if (Inc != cXY(0, 0))
  {
    if (getReadImpl(*this).m_Fragments.size() > 0)
    {
      cComplexRectImpl & Pimpl = getWriteImpl(*this);

      const cComplexRectImpl::RectList::const_iterator end(Pimpl.m_Fragments.end());
      for (cComplexRectImpl::RectList::iterator i = Pimpl.m_Fragments.begin(); i != end; ++i)
      {
        i->org += Inc;
      }

      if (!Pimpl.m_IsBoundsOutOfDate)
      {
        Pimpl.m_Bounds.org += Inc;
      }
    }
  }
  return *this;
}




// =======================================================================================
// === getFragments ======================================================================
// =======================================================================================

cRect & QAPI cComplexRect::getFragment(const unsigned int j) const
{
  const cComplexRectImpl & Pimpl = getReadImpl(*this);

  const cComplexRectImpl::RectList::const_iterator end(Pimpl.m_Fragments.end());
  unsigned int n = 0;
  for (cComplexRectImpl::RectList::const_iterator i = Pimpl.m_Fragments.begin(); i != end; ++i)
  {
    if (n == j) return *i;
    n++;
  }
  throw cNotFound(__FILE__, __LINE__, L"getFragment j out of range %d", j);
}

std::vector<cRect> QAPI cComplexRect::getFragments(void) const
{
  const cComplexRectImpl & Pimpl = getReadImpl(*this);

  std::vector<cRect> Result;
  Result.resize(Pimpl.m_Fragments.size());

  const cComplexRectImpl::RectList::const_iterator end(Pimpl.m_Fragments.end());
  unsigned int n = 0;
  for (cComplexRectImpl::RectList::const_iterator i = Pimpl.m_Fragments.begin(); i != end; ++i)
  {
    Result.at(n++) = *i;
  }
  return Result;
}
