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

// --- Pre-Compiled Headers --------------------------------------------------------------

#include "stdafx.h"

// --- Headers ---------------------------------------------------------------------------

#include "QKernelBase.h"
#include <float.h>

// --- Start of Namespace ----------------------------------------------------------------

using namespace vfs;






// =======================================================================================
// === isValid ===========================================================================
// =======================================================================================

bool QAPI cfRect::isValid() const
{
  return toIntTruncated(*this).isValid();
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
cfRect QAPI cfRect::intersect(const cfRect & a) const
{
  cfRect r = *this;

  // Clip left edge of 'this' (L) against left edge of 'other'.
  if (r.org.x < a.org.x)
  {
    const float MinOverlap = r.len.x - (a.org.x - r.org.x);
    const float PosWidth   = Max<float>(MinOverlap, 0); // Width must not be neg.
    r.len.x = PosWidth;
    r.org.x = a.org.x;
   }
  // Clip top edge of 'this' (T) against top edge of 'other'.
  if (r.org.y < a.org.y)
  {
    const float MinOverlap = r.len.y - (a.org.y - r.org.y);
    const float PosHeight  = Max<float>(MinOverlap, 0); // Height must not be neg.
    r.len.y = PosHeight;
    r.org.y = a.org.y;
  }
  // Clip right edge of 'this' (R) against right edge of 'other' (R). Size never goes negative.
  const float ar = a.org.x + a.len.x;
  const float rr = r.org.x + r.len.x;
  if (rr > ar)
  {
    const float MinOverlap = Min<float>(rr, ar) - r.org.x;
    const float PosWidth   = Max<float>(MinOverlap, 0); // Width must not be neg.
    r.len.x = PosWidth;
  }
  // Clip bottom edge of 'this' (T) against bottom edge of 'other' (B). Size never goes negative.
  const float ab = a.org.y + a.len.y;
  const float rb = r.org.y + r.len.y;
  if (rb > ab)
  {
    const float MinOverlap = Min<float>(rb, ab) - r.org.y;
    const float PosHeight  = Max<float>(MinOverlap, 0); // Height must not be neg.
    r.len.y = PosHeight;
  }

  // Result can be a degenerate rectangle where w or h is zero!
  return r;
}




// =======================================================================================
// === Union =============================================================================
// =======================================================================================

cfRect QAPI cfRect::Union(const cfRect & a) const
{
  cfRect r = *this;

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
      const float Diff = r.org.x - a.org.x;
      r.len.x += Diff;
      r.org.x = a.org.x;
    }
    // Use nearest y to origin.
    if (a.org.y < r.org.y)
    {
      const float Diff = r.org.y - a.org.y;
      r.len.y += Diff;
      r.org.y = a.org.y;
    }

    // Use furthest right from origin.
    const float ar = a.org.x + a.len.x;
    const float rr = r.org.x + r.len.x;
    if (ar > rr)
    {
      const float Diff = ar - rr;
      r.len.x += Diff;
    }

    // Use furthest bottom from origin.
    const float ab = a.org.y + a.len.y;
    const float rb = r.org.y + r.len.y;
    if (ab > rb)
    {
      const float Diff = ab - rb;
      r.len.y += Diff;
    }
  }

  // Result can be a degenerate rectangle where w or h is zero!
  return r;
}




// =======================================================================================
// === round =============================================================================
// =======================================================================================

#define ROUND 0.5f//(1.0f - FLT_EPSILON)

cfRect cfRect::round() const
{
  // This routine is slightly more compilcated because it has to handle a negative origin.
  //
  //        |
  //        |           Consider a rectangle starting at 'o'. It's origin is in the
  //        |           PosXPosY quadrant. To guarantee that the resulting integer
  //        |           rect encloses the original floating point one we need to round
  //  ------+------X+   the values in of rect origin down and the size up. Potentially
  //        |o---+      moving x & y 'outwards'.
  //        ||   |
  //        |+---+
  //        |
  //        Y+
  //
  //        |
  //        |+---+      However if we conside the situation when the rect is in the PosXNegY
  //        ||   |      quadrant then rounding the rect origin values down potentially
  //        |0---+      moves x 'outwards' BUT y 'inwards'!. To fix this we have to
  //  ------+------X+   examine which quadrant the rect is in and do a different combination
  //        |           of rounding in each case.
  //        |
  //        |
  //        |
  //        Y+

  cfXY tl = this->org;
  cfXY br = this->org + this->len;

  // Work out which quadrant around the coord point is in.
  if (tl.y < 0.0f)
  { // NegY
    if (tl.x < 0.0f)
    { // NegXNegY 
      tl.x = (float)(int)(tl.x - ROUND); tl.y = (float)(int)(tl.y - ROUND);
    }
    else
    { // PosXNegY
      tl.x = (float)(int)(tl.x); tl.y = (float)(int)(tl.y - ROUND);
    }
  }
  else
  { // PosY
    if (tl.x < 0.0f)
    { // NegXPosY
      tl.x = (float)(int)(tl.x - ROUND); tl.y = (float)(int)(tl.y);
    }
    else
    { // PosXPosY
      tl.x = (float)(int)(tl.x); tl.y = (float)(int)(tl.y);
    }
  }

  // Work out which quadrant around the coord point is in.
  if (br.y < 0.0f)
  { // NegY
    if (br.x < 0.0f)
    { // NegXNegY
      br.x = (float)(int)(br.x); br.y = (float)(int)(br.y);
    }
    else
    { // PosXNegY
      br.x = (float)(int)(br.x + ROUND); br.y = (float)(int)(br.y);
    }
  }
  else
  { // PosY
    if (br.x < 0.0f)
    { // NegXPosY
      br.x = (float)(int)(br.x); br.y = (float)(int)(br.y + ROUND);
    }
    else
    { // PosXPosY
      br.x = (float)(int)(br.x + ROUND); br.y = (float)(int)(br.y + ROUND);
    }
  }

//  r.len.x = (float)(int)(r.len.x + FLT_EPSILON); r.len.y = (float)(int)(r.len.y + FLT_EPSILON);

  return cfRect(tl, br - tl);
}
