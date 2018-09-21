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
#include "fatal.h"

using namespace vfs;




// =======================================================================================
// === isValid ===========================================================================
// =======================================================================================

bool QAPI cRect::isValid() const
{
  // To avoid overflow problems the ints used by a cRect should never be larger than
  // RECTINT_MAX.
  const bool IsFloorOk   = (org.x >= RECTINT_MIN && org.y >= RECTINT_MIN && len.x >= RECTINT_MIN && len.y >= RECTINT_MIN);
  const bool IsCeilingOk = (org.x <= RECTINT_MAX && org.y <= RECTINT_MAX && len.x <= RECTINT_MAX && len.y <= RECTINT_MAX);
  return IsFloorOk && IsCeilingOk;
}




// =======================================================================================
// === _normalise/_clip ==================================================================
// =======================================================================================

void QAPI _clip(cRect & it)
{
  if (it.org.x < RECTINT_MIN) it.org.x = RECTINT_MIN;
  if (it.org.y < RECTINT_MIN) it.org.y = RECTINT_MIN;

  if (it.len.x < RECTINT_MIN) it.len.x = RECTINT_MIN;
  if (it.len.y < RECTINT_MIN) it.len.y = RECTINT_MIN;

  if (it.org.x > RECTINT_MAX) it.org.x = RECTINT_MAX;
  if (it.org.y > RECTINT_MAX) it.org.y = RECTINT_MAX;

  if (it.len.x > RECTINT_MAX) it.len.x = RECTINT_MAX;
  if (it.len.y > RECTINT_MAX) it.len.y = RECTINT_MAX;
}

// ---------------------------------------------------------------------------------------

void QAPI _normalise(cRect & it)
{
  // sort out negative lengths - adjust coords and make lengths positive
  if (it.len.x < 0)
  {
    // STEFANO CASILLO. The sign was wrong
	//it.org.x -= it.len.x;
	it.org.x += it.len.x;
    it.len.x = - it.len.x;
  }
  if (it.len.y < 0)
  {
    // STEFANO CASILLO. The sign was wrong
	//it.org.y -= it.len.y;
	it.org.y += it.len.y;
    it.len.y = - it.len.y;
  }
}




// =======================================================================================
// === Construction ======================================================================
// =======================================================================================

// Normally inline for a release build.
#if defined(_DEBUG)

cRect::cRect(cXY xyOrg, cXY xyLen) :
  org(xyOrg),
  len(xyLen)
{
  #if defined(_DEBUG)
    QKERNELBASEVERIFY(isValid());
  #endif
}

#endif

// ---------------------------------------------------------------------------------------

// Normally inline for a release build.
#if defined(_DEBUG)

cRect::cRect(int xorg, int yorg, cXY xyLen) :
  org(cXY(xorg, yorg)),
  len(xyLen)
{
  #if defined(_DEBUG)
    QKERNELBASEVERIFY(isValid());
  #endif
}

#endif

// ---------------------------------------------------------------------------------------

// Normally inline for a release build.
#if defined(_DEBUG)

cRect::cRect(cXY xyOrg, int xlen, int ylen) :
  org(xyOrg),
  len(cXY(xlen, ylen))
{
  #if defined(_DEBUG)
    QKERNELBASEVERIFY(isValid());
  #endif
}

#endif

// ---------------------------------------------------------------------------------------

// Normally inline for a release build.
#if defined(_DEBUG)

cRect::cRect(int xorg, int yorg, int xlen, int ylen) :
  org(cXY(xorg, yorg)),
  len(cXY(xlen, ylen))
{
  #if defined(_DEBUG)
    QKERNELBASEVERIFY(isValid());
  #endif
}

#endif

// ---------------------------------------------------------------------------------------

cRect::cRect(const String & SerialisedRect)
{
  double nTmpX, nTmpY, nTmpW, nTmpH;
  (void)swscanf(SerialisedRect.c_str(), L"%lf, %lf, %lf, %lf", &nTmpX, &nTmpY, &nTmpW, &nTmpH);
  org.x = (int)nTmpX;
  org.y = (int)nTmpY;
  len.x = (int)nTmpW;
  len.y = (int)nTmpH;

  #if defined(_DEBUG)
    QKERNELBASEVERIFY(isValid());
  #endif
}


String QAPI cRect::serialise() const
{
  Char buffer[256];
  _snwprintf(buffer,256,L"%d, %d, %d, %d", org.x, org.y, len.x, len.y);
  return String(buffer);
}




// =======================================================================================
// === intersect =========================================================================
// =======================================================================================

//     this +---T---+
//          |       |
//          |   ####|##             In this function we are clipping
//          L   ####R##             'this' rectangle against 'a'.
//          |   ####|##
//          |   ####|## a
//          +---B---+##
//              #######
//              #######
cRect QAPI cRect::intersect(cRect a) const
{
  #if defined(_DEBUG)
    QKERNELBASEVERIFY(isValid());
    QKERNELBASEVERIFY(a.len.x >= 0 && a.len.y >= 0);
    QKERNELBASEVERIFY(len.x   >= 0 &&   len.y >= 0);
  #endif

  const int & tx = org.x;
  const int & ty = org.y;
  const int & tw = len.x;
  const int & th = len.y;

  const int & ax = a.org.x;
  const int & ay = a.org.y;
  const int & aw = a.len.x;
  const int & ah = a.len.y;

  const int tr = tx + tw;
  const int ar = ax + aw;
  if (tr > ax && tx < ar)     // Overlapping horizontally?
  {
    const int tb = ty + th;
    const int ab = ay + ah;
    if (tb > ay && ty < ab)   // Overlapping veritically?
    {
      const unsigned int OverlapType =
        ( (ay <= ty) ? 1 << 3 : 0 ) |
        ( (ab >= tb) ? 1 << 2 : 0 ) |
        ( (ax <= tx) ? 1 << 1 : 0 ) |
        ( (ar >= tr) ? 1 << 0 : 0 );

      switch (OverlapType)
      {
        case 0: // KEEPS centre portion           
        {
          return cRect(                               //  t---+ 
            ax,            ay,                        //  |a##| 
            aw,            ah);                       //  |###|
                                                      //  +---+
        }
        case 1: // KEEPS right middle portion.
        {
          return cRect(                               //  t---+
            ax,            ay,                        //  |  a##
            tr - ax,       ah);                       //  |  ###
                                                      //  +---+
        }
        case 2: // KEEPS left middle portion.
        {
          return cRect(                               //  t---+
            tx,            ay,                        // a##  |
            ar - tx,       ah);                       // ###  |
                                                      //  +---+
        }
        case 3: // KEEPS all middle horiz portion.
        {
          return cRect(                               //  t---+
            tx,            ay,                        // a######
            tw,            ah);                       // #######
                                                      //  +---+
        }
        case 4: // KEEPS bottom middle portion.
        {
          return cRect(                               //  t---+
            ax,            ay,                        //  |   |
            aw,            tb - ay);                  //  |a##|
                                                      //  +###+
        }                                             //   ###
        case 5: // KEEPS bottom right portion.
        {
          return cRect(                               //  t---+
            ax,            ay,                        //  |   |
            tr - ax,       tb - ay);                  //  |  a##
                                                      //  +--###
        }                                             //     ###
        case 6: // KEEPS bottom left portion.
        {
          return cRect(                               //  t---+
            tx,            ay,                        //  |   |
            ar - tx,       tb - ay);                  // a##  |
                                                      // ###--+
        }                                             // ###
        case 7: // KEEPS all bottom portion.
        {
          return cRect(                               //  t---+
            tx,            ay,                        //  |   |
            tw,            tb - ay);                  // a######
                                                      // #######
        }
        case 8: // KEEPS top middle portion.
        {
          return cRect(                               //   a##
            ax,            ty,                        //  t###+
            aw,            ab - ty);                  //  |###|
                                                      //  |   |
        }                                             //  +---+
        case 9: // KEEPS top right corner.
        {
          return cRect(                               //     a##
            ax,            ty,                        //  t--###
            tr - ax,       ab - ty);                  //  |  ###
                                                      //  |   |
        }                                             //  +---+
        case 10: // KEEPS top left corner.
        {
          return cRect(                               // a##
            tx,            ty,                        // ###--+
            ar - tx,       ab - ty);                  // ###  |
                                                      //  +---+
        }                                                       
        case 11: // KEEPS all top edge.
        {
          return cRect(                               // a######
            tx,            ty,                        // #######
            tw,            ab - ty);                  //  |   | 
                                                      //  +---+          
        }                                                       
        case 12: // KEEPS vertical portion down middle.
        {
          return cRect(                               //   a##
            ax,            ty,                        // t-###-+
            aw,            th);                       // | ### |
                                                      // +-###-+
        }                                             //   ###
        case 13: // KEEPS all right edge.
        {
          return cRect(                               //      a##
            ax,            ty,                        // t----###
            tr - ax,       th);                       // |    ###
                                                      // +----###
        }                                             //      ###
        case 14: // KEEPS all left edge.
        {
          return cRect(                               // a##
            tx,            ty,                        // ###----+
            ar - tx,       th);                       // ###    |
                                                      // ###----+          
        }                                             // ###          
        case 15: // KEEPS all.
        {
          return cRect(                               // a#######
            tx,            ty,                        // ########
            tw,            th);                       // ########
        }                                                       
      }
    }
  }

  // No overlap at all.
  return cRect(0, 0, 0, 0);
}






/*
SJSJSJ Old intersect code - roughly 30% more expensive than above in best case no clip.
Even slower if there is a clip!
*/
/*
  cRect r  = *this;

  // Validate the parameters and report if one fails. In the event of an error
  // we will modify the parameters in an effort to keep running.
  #if defined(_DEBUG)
    if (!_ASSERTE_AND_CONTINUE(_isInRange(r)))                  _clip(r);
    if (!_ASSERTE_AND_CONTINUE(a.len.x >= 0 && a.len.y >= 0)) _normalise(a);
    if (!_ASSERTE_AND_CONTINUE(r.len.x >= 0 && r.len.y >= 0))   _normalise(r);
  #endif

  // Clip left edge of 'this' (L) against left edge of 'other'.
  if (r.org.x < a.org.x)
  {
    const int MinOverlap = r.len.x - (a.org.x - r.org.x);
    const int PosWidth   = Max<int>(MinOverlap, 0); // Width must not be neg.
    r.len.x = (int)PosWidth;
    r.org.x = a.org.x;
   }
  // Clip top edge of 'this' (T) against top edge of 'other'.
  if (r.org.y < a.org.y)
  {
    const int MinOverlap = r.len.y - (a.org.y - r.org.y);
    const int PosHeight  = Max<int>(MinOverlap, 0); // Height must not be neg.
    r.len.y = (int)PosHeight;
    r.org.y = a.org.y;
  }
  // Clip right edge of 'this' (R) against right edge of 'other' (R). Size never goes negative.
  const int ar = a.org.x + a.len.x;
  const int rr = r.org.x + r.len.x;
  if (rr > ar)
  {
    const int MinOverlap = Min<int>(rr, ar) - r.org.x;
    const int PosWidth   = Max<int>(MinOverlap, 0); // Width must not be neg.
    r.len.x = (int)PosWidth;
  }
  // Clip bottom edge of 'this' (T) against bottom edge of 'other' (B). Size never goes negative.
  const int ab = a.org.y + a.len.y;
  const int rb = r.org.y + r.len.y;
  if (rb > ab)
  {
    const int MinOverlap = Min<int>(rb, ab) - r.org.y;
    const int PosHeight  = Max<int>(MinOverlap, 0); // Height must not be neg.
    r.len.y = (int)PosHeight;
  }

  // Result can be a degenerate rectangle where w or h is zero!
  return r;
}
*/



// =======================================================================================
// === normalise =========================================================================
// =======================================================================================

cRect QAPI cRect::normalise(const cRect& r) const
{
  // sort out negative lengths - adjust coords and make lengths positive
  cRect result = r;
  _normalise(result);
  return result;
}




// =======================================================================================
// === Union =============================================================================
// =======================================================================================

cRect QAPI cRect::Union(cRect a) const
{
  cRect r  = *this;

  // Validate the parameters and report if one fails. In the event of an error
  // we will modify the parameters in an effort to keep running.
  #if defined(_DEBUG)
    QKERNELBASEVERIFY(r.isValid());
    QKERNELBASEVERIFY(a.isValid());
    QKERNELBASEVERIFY(a.len.x >= 0 && a.len.y >= 0);
    QKERNELBASEVERIFY(r.len.x >= 0 && r.len.y >= 0);
  #endif

  if (isDegenerate())
  {
    // If 'this' is degenerate then 'this' has no influence in union so just return 'a';
    r = a;
  }
  else if (a.isDegenerate())
  {
    // If 'a' is degenerate then 'a' has no influence in union so just return 'this';
    r = *this;
  }
  else
  {
    // Neither parameter is degenerate so we have to calculate the union for real.

    // Use nearest x to origin.
    if (a.org.x < r.org.x)
    {
      const int Diff = r.org.x - a.org.x;
      r.len.x += Diff;
      r.org.x = a.org.x;
    }
    // Use nearest y to origin.
    if (a.org.y < r.org.y)
    {
      const int Diff = r.org.y - a.org.y;
      r.len.y += Diff;
      r.org.y = a.org.y;
    }

    // Use furthest right from origin.
    const int ar = a.org.x + a.len.x;
    const int rr = r.org.x + r.len.x;
    if (ar > rr)
    {
      const int Diff = ar - rr;
      r.len.x += Diff;
    }

    // Use furthest bottom from origin.
    const int ab = a.org.y + a.len.y;
    const int rb = r.org.y + r.len.y;
    if (ab > rb)
    {
      const int Diff = ab - rb;
      r.len.y += Diff;
    }
  }

#if defined(_DEBUG)
  QKERNELBASEVERIFY(r.isValid());
#endif

  // Result can be a degenerate rectangle where w or h is zero!
  return r;
}






// =======================================================================================
// === isContainedBy =====================================================================
// =======================================================================================

#if defined(_DEBUG)
bool QAPI cRect::isContainedBy(const cRect & a) const
{
  cRect r  = *this;
  cRect aa = a;

#if defined(_DEBUG)
  QKERNELBASEVERIFY(r.isValid());
  QKERNELBASEVERIFY(aa.isValid());
#endif

  return r.org.x           >= aa.org.x &&
         r.org.x + r.len.x <= aa.org.x + aa.len.x &&
         r.org.y           >= aa.org.y &&
         r.org.y + r.len.y <= aa.org.y + aa.len.y;
}
#endif





// =======================================================================================
// === isContaining ======================================================================
// =======================================================================================

#if defined(_DEBUG)
bool QAPI cRect::isContaining(const cXY & a) const
{
  cRect r  = *this;

#if defined(_DEBUG)
  QKERNELBASEVERIFY(r.isValid());
#endif

  return a.x >= r.org.x &&
         a.x <  r.org.x + r.len.x &&
         a.y >= r.org.y &&
         a.y <  r.org.y + r.len.y;
}
#endif
