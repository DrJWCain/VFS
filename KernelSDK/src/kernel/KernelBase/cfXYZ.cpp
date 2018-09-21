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
// === Construction ======================================================================
// =======================================================================================

cXY::cXY (const String & serialised)
{
  String CopyOfInput(serialised.c_str());
 
  //WARNING! swscanf will temporarily modify the src string!
 
  int nTmpX, nTmpY;
  (void)swscanf(CopyOfInput.c_str(), L"%d, %d", &nTmpX, &nTmpY);
  x = (int)nTmpX;
  y = (int)nTmpY;
};

String QAPI cXY::toString() const //used for serialising
{
  const int kBufferSize(256);
  Char buffer [kBufferSize];
  _snwprintf(buffer, kBufferSize, L"%d, %d", x, y);
  return String(buffer);
}

cdXY::cdXY(const String & Serialised)
{
  String CopyOfInput(Serialised.c_str());

  /*
  WARNING! swscanf will temporarily modify the src string!
  */
  double nTmpX, nTmpY;
  (void)swscanf(CopyOfInput.c_str(), L"%lf, %lf", &nTmpX, &nTmpY);
  x = (double)nTmpX;
  y = (double)nTmpY;
}
cXYZ::cXYZ(const String & Serialised)
{
  String CopyOfInput(Serialised.c_str()  /* Thread safe deep copy */);

  /*
  WARNING! swscanf will temporarily modify the src string!
  */
  double nTmpX, nTmpY, nTmpZ;
  (void)swscanf(CopyOfInput.c_str(), L"%lf, %lf, %lf", &nTmpX, &nTmpY, &nTmpZ);
  x = (int)nTmpX;
  y = (int)nTmpY;
  z = (int)nTmpZ;
}
cfXYZ::cfXYZ(const String & Serialised)
{
  String CopyOfInput(Serialised.c_str() /* Thread safe deep copy */);

  /*
  WARNING! swscanf will temporarily modify the src string!
  */
  double nTmpX, nTmpY, nTmpZ;
  (void)swscanf(CopyOfInput.c_str(), L"%lf, %lf, %lf", &nTmpX, &nTmpY, &nTmpZ);
  x = (float)nTmpX;
  y = (float)nTmpY;
  y = (float)nTmpZ;
}




// =======================================================================================
// === normalise =========================================================================
// =======================================================================================

cfXYZ & QAPI cfXYZ::normalise(void) throw()
{
  const float MagnitudeReciprocal = 1.0f / magnitude();
  x *= MagnitudeReciprocal;
  y *= MagnitudeReciprocal;
  z *= MagnitudeReciprocal;
  return *this;
}




// =======================================================================================
// === orthogonalise =====================================================================
// =======================================================================================

cfXYZ QAPI vfs::orthogonalise(
  const cfXYZ & V,
  const cfXYZ & W) throw()
{
	// Vectors should not be parallel!
	assert(!areParallel(V, W));

	const cfXYZ TmpAccuratePerp(crossProduct(W, V));
	return crossProduct(V, TmpAccuratePerp);
}




// =======================================================================================
// === crossProduct ======================================================================
// =======================================================================================

cfXYZ QAPI vfs::crossProduct(
	const cfXYZ & V,
  const cfXYZ & W) throw()
{
  return cfXYZ(
     (V.y * W.z) - (V.z * W.y),
	  -(V.x * W.z) + (V.z * W.x),
	   (V.x * W.y) - (V.y * W.x));
}

