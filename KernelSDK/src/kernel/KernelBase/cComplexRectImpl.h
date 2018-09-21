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


#if 0
#include "cComplexRectImpl_2.h"
#else
#pragma once
/*
**  
*/

#include <list>
#include "QKernelBase.h"
#include <new>

using namespace std;

namespace vfs
{


#define IS_USING_COMPLEXRECT_FAST_LIST

#if defined(_DEBUG)
  #define IS_COMPLEXRECT_ASSERTING
#endif

// =======================================================================================
// === iFastContainer ====================================================================
// =======================================================================================

#if defined(IS_USING_COMPLEXRECT_FAST_LIST)

class cFastIterator;

class iFastContainer
{
  public :
    virtual ~iFastContainer() {}
    virtual cFastIterator & incIterator(cFastIterator & Iterator) const = 0;
    virtual cFastIterator & decIterator(cFastIterator & Iterator) const = 0;
};

#endif




// =======================================================================================
// === cFastIterator/ ====================================================================
// =======================================================================================

#if defined(IS_USING_COMPLEXRECT_FAST_LIST)

class cFastIterator
{
  public :

    cFastIterator() {}
    cFastIterator(iFastContainer * const Container, const unsigned int e, cRect * const Element) : m_Container(Container), m_Fast(e), m_Element(Element) {}
    explicit cFastIterator(iFastContainer * const Container, const list<cRect>::iterator & i, cRect * const Element) : m_Container(Container), m_Fast(UINT_MAX), m_Slow(i), m_Element(Element) {}
    cFastIterator(const cFastIterator & other) : m_Container(other.m_Container), m_Slow(other.m_Slow), m_Fast(other.m_Fast), m_Element(other.m_Element) {}
    cFastIterator & operator=(const cFastIterator & other) { m_Container = other.m_Container; m_Slow = other.m_Slow; m_Fast = other.m_Fast; m_Element = other.m_Element; return *this; }
    bool isSlow() const { return m_Fast == UINT_MAX; }
    bool isFast() const { return m_Fast != UINT_MAX; }
    cRect * operator->() const { return m_Element; }
    cRect & operator*() const { return *m_Element; }
    bool operator==(const cFastIterator & other) const { return m_Element == other.m_Element; }
    bool operator!=(const cFastIterator & other) const { return !(*this == other); }
    cFastIterator & operator++() const /* pre */ { return m_Container->incIterator(*const_cast<cFastIterator *>(this)); }
    cFastIterator & operator--() const /* pre */ { return m_Container->decIterator(*const_cast<cFastIterator *>(this)); }
    cFastIterator operator++(int /* not used */) const /* post */ { const cFastIterator Copy(*this); m_Container->incIterator(*const_cast<cFastIterator *>(this)); return Copy; }
    cFastIterator operator--(int /* not used */) const /* post */ { const cFastIterator Copy(*this); m_Container->decIterator(*const_cast<cFastIterator *>(this)); return Copy; }
    String serialise() const
    {
      Char tmp[128];
      (void)wsprintf(tmp, L"0x%I64x (rect @ 0x%p (%d, %d, %d, %d))", m_Fast == UINT_MAX ? (UInt64)m_Slow.operator->() : (UInt64)m_Fast, m_Element, m_Element->org.x, m_Element->org.y, m_Element->len.x, m_Element->len.y);
      return tmp;
    }

    void setElementPtr(cRect * const NewPtr) { m_Element = NewPtr; }
    list<cRect>::iterator & getSlowIterator() const { return m_Slow; }
    unsigned int & getFastIterator() const { return m_Fast; }

  private :

    iFastContainer *                            m_Container;
    mutable unsigned int                        m_Fast; // If UINT_MAX then m_Slow should be used instead
    mutable list<cRect>::iterator               m_Slow;
    mutable cRect *                             m_Element;
};

#endif




// =======================================================================================
// === cFastRectList =====================================================================
// =======================================================================================

#if defined(IS_USING_COMPLEXRECT_FAST_LIST)

const unsigned int kNumPreallocatedFrags = 10;

class cFastRectList : public iFastContainer
{
  public :

    typedef cFastIterator       iterator;
    typedef const cFastIterator const_iterator;

    cFastRectList() :
      m_Size(0),
      m_NextFastToUse(0)
    {
    }

    void clear()
    {
      m_Slow.clear();
      m_Size = 0;
      m_NextFastToUse = 0;
    }

    void push_back(const cRect & a)
    {
      #if defined(IS_COMPLEXRECT_ASSERTING)
        assert(m_NextFastToUse + m_Slow.size() == m_Size); // Size tally?
      #endif

      if (m_NextFastToUse < kNumPreallocatedFrags)
      {
        #if defined(IS_COMPLEXRECT_ASSERTING)
          assert(m_Slow.empty()); // Slow must be empty
        #endif
        m_Fast[m_NextFastToUse] = a;
        m_NextFastToUse++;
      }
      else
      {
        #if defined(IS_COMPLEXRECT_ASSERTING)
          assert(m_NextFastToUse == kNumPreallocatedFrags); // Fast must be full
        #endif
        m_Slow.push_back(a);
      }

      m_Size++;

      #if defined(IS_COMPLEXRECT_ASSERTING)
        assert(m_NextFastToUse + m_Slow.size() == m_Size); // Size tally?
      #endif
    }

    cFastIterator erase(const cFastIterator & i)
    {
      #if defined(IS_COMPLEXRECT_ASSERTING)
        assert(m_NextFastToUse + m_Slow.size() == m_Size); // Size tally?
      #endif

      #if defined(IS_COMPLEXRECT_ASSERTING)
        assert(m_Size > 0); // Must not be empty
      #endif
      m_Size--;
      if (i.isFast())
      {
        const unsigned int ElementToErase(i.getFastIterator());
        #if defined(IS_COMPLEXRECT_ASSERTING)
          assert(ElementToErase < kNumPreallocatedFrags); // Within range?
        #endif
        // Shift succeeding elements forward
        unsigned int j;
        for (j = ElementToErase + 1; j < m_NextFastToUse; ++j)
        {
          m_Fast[j - 1] = m_Fast[j];
        }
        if (m_Slow.empty())
        {
          m_NextFastToUse = j - 1;
        }
        else
        {
          #if defined(IS_COMPLEXRECT_ASSERTING)
            assert(j == kNumPreallocatedFrags); // Fast must be full
          #endif
          // Shift first from slow into last in fast
          list<cRect>::iterator FirstSlow(m_Slow.begin());
          m_Fast[kNumPreallocatedFrags - 1] = *FirstSlow;
          (void)m_Slow.erase(FirstSlow);
        }
        #if defined(IS_COMPLEXRECT_ASSERTING)
          assert(m_NextFastToUse + m_Slow.size() == m_Size); // Size tally?
        #endif
        return i;
      }
      else
      {
        #if defined(IS_COMPLEXRECT_ASSERTING)
          assert(m_NextFastToUse == kNumPreallocatedFrags); // Fast must be full
        #endif
        if (m_Size == kNumPreallocatedFrags)
        {
          #if defined(IS_COMPLEXRECT_ASSERTING)
            assert(m_Slow.size() == 1);
          #endif
          m_Slow.clear();
          #if defined(IS_COMPLEXRECT_ASSERTING)
            assert(m_NextFastToUse + m_Slow.size() == m_Size); // Size tally?
          #endif
          return cFastIterator(this, m_NextFastToUse, &m_Fast[m_NextFastToUse]);
        }
        else
        {
          list<cRect>::iterator tmp = m_Slow.erase(i.getSlowIterator());            
          
          #if defined(IS_COMPLEXRECT_ASSERTING)
            assert(m_NextFastToUse + m_Slow.size() == m_Size); // Size tally?
          #endif
          
          if (tmp == m_Slow.end())
            return cFastIterator(this, tmp, NULL);
          else
            return cFastIterator(this, tmp, tmp.operator->());
        }
      }
    }

    unsigned int size() const
    {
      #if defined(IS_COMPLEXRECT_ASSERTING)
        assert(m_NextFastToUse + m_Slow.size() == m_Size); // Size tally?
      #endif
      return m_Size;
    }

    cFastIterator begin()
    {
      #if defined(IS_COMPLEXRECT_ASSERTING)
        assert(m_NextFastToUse + m_Slow.size() == m_Size); // Size tally?
      #endif
      return cFastIterator(this, 0, &m_Fast[0]);
    }

    cFastIterator begin() const
    {
      #if defined(IS_COMPLEXRECT_ASSERTING)
        assert(m_NextFastToUse + m_Slow.size() == m_Size); // Size tally?
      #endif
      cFastRectList * const NonConstThis = const_cast<cFastRectList *>(this);
      return cFastIterator(NonConstThis, 0, &NonConstThis->m_Fast[0]);
    }

    cFastIterator end() const
    {
      #if defined(IS_COMPLEXRECT_ASSERTING)
        assert(m_NextFastToUse + m_Slow.size() == m_Size); // Size tally?
      #endif
      cFastRectList * const NonConstThis = const_cast<cFastRectList *>(this);
      if (m_Slow.empty())
      {
        return cFastIterator(NonConstThis, m_NextFastToUse, &NonConstThis->m_Fast[m_NextFastToUse]);
      }
      else
      {
        list<cRect>::iterator tmp(NonConstThis->m_Slow.end());
        return cFastIterator(NonConstThis, tmp, NULL);
      }
    }

  private :

    cRect                   m_Fast[kNumPreallocatedFrags];
    list<cRect>             m_Slow;
    unsigned int            m_Size;
    unsigned int            m_NextFastToUse;

    // iFastContainer
    cFastIterator & incIterator(cFastIterator & i) const
    {
      #if defined(IS_COMPLEXRECT_ASSERTING)
        assert(m_NextFastToUse + m_Slow.size() == m_Size); // Size tally?
      #endif

//      if (i != end()) // SJSJSJ Is this check necessary?
      {
        if (i.isFast())
        {
          unsigned int * FastIterator = &i.getFastIterator();
          if (*FastIterator == kNumPreallocatedFrags - 1 && !m_Slow.empty())
          {
            cFastRectList * const NonConstThis = const_cast<cFastRectList *>(this);
            list<cRect>::iterator SlowIterator(NonConstThis->m_Slow.begin());
            i = iterator(NonConstThis, SlowIterator, SlowIterator.operator->());
          }
          else
          {
            cFastRectList * const NonConstThis = const_cast<cFastRectList *>(this);
            (*FastIterator)++;
            i.setElementPtr(&NonConstThis->m_Fast[*FastIterator]);
          }
        }
        else
        {
           list<cRect>::iterator * SlowIterator = &i.getSlowIterator();

           if ( *SlowIterator == m_Slow.end() )
             i.setElementPtr(NULL);
           else
           {
             ++(*SlowIterator);
             
             if ( *SlowIterator == m_Slow.end() )
               i.setElementPtr(NULL);
             else
               i.setElementPtr(SlowIterator->operator->());
           }     
        }
      }
      return i;
    }

    cFastIterator & decIterator(cFastIterator & i) const
    {
      #if defined(IS_COMPLEXRECT_ASSERTING)
        assert(m_NextFastToUse + m_Slow.size() == m_Size); // Size tally?
      #endif

//      if (i != begin()) // SJSJSJ Is this check necessary?
      {
        if (i.isFast())
        {
          cFastRectList * const NonConstThis = const_cast<cFastRectList *>(this);
          unsigned int * FastIterator = &i.getFastIterator();
          (*FastIterator)--;
          i.setElementPtr(&NonConstThis->m_Fast[*FastIterator]);
        }
        else
        {
          list<cRect>::iterator * SlowIterator = &i.getSlowIterator();

          if (*SlowIterator == const_cast<cFastRectList *>(this)->m_Slow.begin())
          {
            cFastRectList * const NonConstThis = const_cast<cFastRectList *>(this);
            i = iterator(NonConstThis, kNumPreallocatedFrags - 1, &NonConstThis->m_Fast[kNumPreallocatedFrags - 1]);
          }
          else
          {
            --(*SlowIterator);
            i.setElementPtr(SlowIterator->operator->());
          }
        }
      }
      return i;
    }
};

#endif




// =======================================================================================
// === cComplexRectImpl ==================================================================
// =======================================================================================

const unsigned int kComplexRectImplClassVersion = 1;

class cComplexRectImpl
#if IS_COMPLEXRECT_COW
  : public cCowPimpl
#endif
{
  public :

#if IS_COMPLEXRECT_COW
    QDEFINE_SMARTCOWPTR(cComplexRectImpl);

    // Required for copy-on-write smart pointer.
    iCowPointable * clone() const
    {
      return new cComplexRectImpl(*this);
    }
#endif

    cComplexRectImpl();
    cComplexRectImpl(const cRect & InitialRect);
    cComplexRectImpl(const std::vector<cRect> & InitialFragments);
    cComplexRectImpl(const cComplexRectImpl & Other);
    cComplexRectImpl & operator=(const cComplexRectImpl & Other);
    bool isDegenerate(void) const
    {
      if (m_IsBoundsOutOfDate)
      {
        return calcBounds().isDegenerate();
      }
      else
      {
        return m_IsBoundsDegenerate;
      }
    }
    void append(const cRect & NewFragment);
    void append(const cComplexRectImpl & NewFragments);
    void append(const cComplexRectImpl & NewFragments, const cXY & OrgOffset);
    const cRect & calcBounds() const;
    #if defined(IS_USING_COMPLEXRECT_FAST_LIST)
          typedef cFastRectList RectList;
    #else
          typedef list<cRect> RectList;
    #endif
    const RectList & getFragments() const
    {
      #if defined(IS_COMPLEXRECT_ASSERTING)
        assert(m_ClassVersion == kComplexRectImplClassVersion);
      #endif
      return m_Fragments;
    }

    // placement new & delete
    void * operator new(size_t reqSize, void * const it, size_t availSize)
    {
#if defined(_DEBUG) 
      assert(availSize >= reqSize);
#endif     
      return it;
    }
    void operator delete(void * const, void * const it, size_t availSize) {}

    unsigned int                m_ClassVersion;

    RectList                    m_Fragments;

    mutable cRect               m_Bounds;
    mutable bool                m_IsBoundsOutOfDate;    // Helps to avoid unncessary recalc
                                                        //  of m_Bounds. Different from
                                                        //  m_IsFragmentsModified because
                                                        //  bounds do not always change
                                                        //  when fragment list is modified.
    mutable bool                m_IsBoundsDegenerate;
    mutable bool                m_IsOptimised;
};




// =======================================================================================
// === cComplexRectFragmentIteratorImpl ==================================================
// =======================================================================================

class cComplexRectFragmentIteratorImpl
{
  public :
    const cComplexRectImpl *              m_ComplexRectImpl;
    cComplexRectImpl::RectList::iterator  m_Iterator;
    unsigned int                          m_IteratorPosition;
};




// --- End of Namespace ------------------------------------------------------------------

}
#endif
