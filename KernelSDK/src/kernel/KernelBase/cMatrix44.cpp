/*
**  Copyright Bullet Continuous Collision Detection and Physics Library
This code is licensed under the zlib license.
http://bulletphysics.org

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "stdafx.h"
#include "QKernelBase.h"
//#include <d3dx9.h>
#if defined(QSWITCH_SME)
  #pragma warning(disable: 4239)  // Ignore "conversion from 'class cfXYZW' to 'class cfXYZW &'"
#else
  #error "Must use compiler that supports processors' Streaming Media Extensions (SME)!"
#endif
#include "fatal.h"

using namespace vfs;





// =======================================================================================
// === Globals ===========================================================================
// =======================================================================================

const cMatrix44 vfs::kIdentityMatrix(
  1.0f,	0.0f,	0.0f,	0.0f,
  0.0f,	1.0f,	0.0f,	0.0f,
  0.0f,	0.0f,	1.0f,	0.0f,
  0.0f, 0.0f, 0.0f, 1.0f);

const cMatrix44 vfs::kZeroMatrix(
	0.0f,	0.0f,	0.0f,	0.0f,
	0.0f,	0.0f,	0.0f,	0.0f,
	0.0f,	0.0f,	0.0f,	0.0f,
	0.0f, 0.0f, 0.0f, 0.0f);




// =======================================================================================
// === _setFromString ====================================================================
// =======================================================================================

void _setFromString(cMatrix44 * const It, const Char * const Serialised) throw(cBadFormat)
{
  String CopyOfInput(Serialised);

  //----------------------------------------------------------
  // WARNING! swscanf will temporarily modify the src string!
  //----------------------------------------------------------

  const unsigned int nNumFieldsConverted = swscanf(
    CopyOfInput.c_str(),
    L"{ { %f, %f, %f, %f }, { %f, %f, %f, %f }, { %f, %f, %f, %f }, { %f, %f, %f, %f } }",
    &It->m00, &It->m01, &It->m02, &It->m03,
    &It->m10, &It->m11, &It->m12, &It->m13,
    &It->m20, &It->m21, &It->m22, &It->m23,
    &It->m30, &It->m31, &It->m32, &It->m33);
  if (nNumFieldsConverted < 16)
  {
    throw cBadFormat(__FILE__, __LINE__, L"Tried to construct a cMatrix44 from string \"%s\"", Serialised);
  }
}




// =======================================================================================
// === Construction ======================================================================
// =======================================================================================

cMatrix44::cMatrix44(const float * const ArrayToCopy)
{
  m00 = ArrayToCopy[ 0]; m01 = ArrayToCopy[ 1]; m02 = ArrayToCopy[ 2]; m03 = ArrayToCopy[ 3]; 
  m10 = ArrayToCopy[ 4]; m11 = ArrayToCopy[ 5]; m12 = ArrayToCopy[ 6]; m13 = ArrayToCopy[ 7]; 
  m20 = ArrayToCopy[ 8]; m21 = ArrayToCopy[ 9]; m22 = ArrayToCopy[10]; m23 = ArrayToCopy[11]; 
  m30 = ArrayToCopy[12]; m31 = ArrayToCopy[13]; m32 = ArrayToCopy[14]; m33 = ArrayToCopy[15]; 
}

// ---------------------------------------------------------------------------------------

cMatrix44::cMatrix44(const cMatrix44 & Other)
{
  m00 = Other.m00; m01 = Other.m01; m02 = Other.m02; m03 = Other.m03; 
  m10 = Other.m10; m11 = Other.m11; m12 = Other.m12; m13 = Other.m13; 
  m20 = Other.m20; m21 = Other.m21; m22 = Other.m22; m23 = Other.m23; 
  m30 = Other.m30; m31 = Other.m31; m32 = Other.m32; m33 = Other.m33; 
}

// ---------------------------------------------------------------------------------------

cMatrix44::cMatrix44(
  const float m00b, const float m01b, const float m02b, const float m03b,
  const float m10b, const float m11b, const float m12b, const float m13b,
  const float m20b, const float m21b, const float m22b, const float m23b,
  const float m30b, const float m31b, const float m32b, const float m33b) :
    m00(m00b), m01(m01b), m02(m02b), m03(m03b), 
    m10(m10b), m11(m11b), m12(m12b), m13(m13b), 
    m20(m20b), m21(m21b), m22(m22b), m23(m23b), 
    m30(m30b), m31(m31b), m32(m32b), m33(m33b)
{
}

// ---------------------------------------------------------------------------------------

cMatrix44::cMatrix44(const String & Serialised) throw(cBadFormat)
{
  _setFromString(this, Serialised.c_str());
}

// ---------------------------------------------------------------------------------------

cMatrix44::cMatrix44(const Char * const Serialised) throw(cBadFormat)
{
  _setFromString(this, Serialised);
}




// =======================================================================================
// === serialise =========================================================================
// =======================================================================================

String QAPI cMatrix44::serialise() const
{
  StringStream tmp;
  tmp << L"{ ";
  tmp << L"{ " << m00 << L", " << m01 << L", " << m02 << L", " << m03 << L" }, ";
  tmp << L"{ " << m10 << L", " << m11 << L", " << m12 << L", " << m13 << L" }, ";
  tmp << L"{ " << m20 << L", " << m21 << L", " << m22 << L", " << m23 << L" }, ";
  tmp << L"{ " << m30 << L", " << m31 << L", " << m32 << L", " << m33 << L" } ";
  tmp << L"}";
/*
  Char Buffer[256];
  wsprintf(
    Buffer,
    L"  %f, %f, %f, %f , { %f, %f, %f, %f }, { %f, %f, %f, %f }, { %f, %f, %f, %f } }",
    1.0f, 2.0f, 3.0f, 4.0f,//m00, m01, m02, m03,
    m10, m11, m12, m13,
    m20, m21, m22, m23,
    m30, m31, m32, m33);
*/
  return tmp.str();
}



// =======================================================================================
// === Construction/Destruction ==========================================================
// =======================================================================================

cMatrix44 & cMatrix44::operator*=(const cMatrix44 & Other)
{
  cMatrix44 Tmp(*this);
  *this = Tmp * Other;
  return *this;
}

// ---------------------------------------------------------------------------------------

cMatrix44 & cMatrix44::operator+=(const cMatrix44 & Other)
{
  m00 += Other.m00; m01 += Other.m01; m02 += Other.m02; m03 += Other.m03; 
  m10 += Other.m10; m11 += Other.m11; m12 += Other.m12; m13 += Other.m13; 
  m20 += Other.m20; m21 += Other.m21; m22 += Other.m22; m23 += Other.m23; 
  m30 += Other.m30; m31 += Other.m31; m32 += Other.m32; m33 += Other.m33; 
  return *this;
}

// ---------------------------------------------------------------------------------------

cMatrix44 & cMatrix44::operator-=(const cMatrix44 & Other)
{
  m00 -= Other.m00; m01 -= Other.m01; m02 -= Other.m02; m03 -= Other.m03; 
  m10 -= Other.m10; m11 -= Other.m11; m12 -= Other.m12; m13 -= Other.m13; 
  m20 -= Other.m20; m21 -= Other.m21; m22 -= Other.m22; m23 -= Other.m23; 
  m30 -= Other.m30; m31 -= Other.m31; m32 -= Other.m32; m33 -= Other.m33; 
  return *this;
}

// ---------------------------------------------------------------------------------------

cMatrix44 & QAPI cMatrix44::operator*=(const float Multiplier)
{
  m00 *= Multiplier; m01 *= Multiplier; m02 *= Multiplier; m03 *= Multiplier; 
  m10 *= Multiplier; m11 *= Multiplier; m12 *= Multiplier; m13 *= Multiplier; 
  m20 *= Multiplier; m21 *= Multiplier; m22 *= Multiplier; m23 *= Multiplier; 
  m30 *= Multiplier; m31 *= Multiplier; m32 *= Multiplier; m33 *= Multiplier; 
  return *this;
}




// =======================================================================================
// === Construction/Destruction ==========================================================
// =======================================================================================

cMatrix44 QAPI cMatrix44::operator*(const cMatrix44 & Other) const
{
#if QSWITCH_SME

  if ((PtrToUlong(this) & 0x0000000f) == 0)
  {
    // Aligned - we can do op using SME

    cMatrix44 Result;
	  cfXYZW Tmp;
	  cfXYZW B1 = cfXYZW(r0), B2 = cfXYZW(r1), B3 = cfXYZW(r2), B4 = cfXYZW(r3);

	  Tmp  = cfXYZW(Other.m00) * B1;
	  Tmp += cfXYZW(Other.m01) * B2;
	  Tmp += cfXYZW(Other.m02) * B3;
	  Tmp += cfXYZW(Other.m03) * B4;
	  Result.r0 = Tmp;

	  Tmp  = cfXYZW(Other.m10) * B1;
	  Tmp += cfXYZW(Other.m11) * B2;
	  Tmp += cfXYZW(Other.m12) * B3;
	  Tmp += cfXYZW(Other.m13) * B4;
	  Result.r1 = Tmp;

	  Tmp  = cfXYZW(Other.m20) * B1;
	  Tmp += cfXYZW(Other.m21) * B2;
	  Tmp += cfXYZW(Other.m22) * B3;
	  Tmp += cfXYZW(Other.m23) * B4;
	  Result.r2 = Tmp;

	  Tmp  = cfXYZW(Other.m30) * B1;
	  Tmp += cfXYZW(Other.m31) * B2;
	  Tmp += cfXYZW(Other.m32) * B3;
	  Tmp += cfXYZW(Other.m33) * B4;
	  Result.r3 = Tmp;

    return Result;

  }
  else

#endif

  {
    cMatrix44 Result;
    for (unsigned int r = 0; r < 4; r++)
    {
      Result.m[r][0] = m00 * Other.m[r][0] + m10 * Other.m[r][1] + m20 * Other.m[r][2] + m30 * Other.m[r][3];
      Result.m[r][1] = m01 * Other.m[r][0] + m11 * Other.m[r][1] + m21 * Other.m[r][2] + m31 * Other.m[r][3];
      Result.m[r][2] = m02 * Other.m[r][0] + m12 * Other.m[r][1] + m22 * Other.m[r][2] + m32 * Other.m[r][3];
      Result.m[r][3] = m03 * Other.m[r][0] + m13 * Other.m[r][1] + m23 * Other.m[r][2] + m33 * Other.m[r][3];
    }
    return Result;
  }
}

// ---------------------------------------------------------------------------------------

cMatrix44 QAPI cMatrix44::operator+(const cMatrix44 & Other) const
{
  cMatrix44 Result(*this);
  Result += Other;
  return Result;
}

// ---------------------------------------------------------------------------------------

cMatrix44 QAPI cMatrix44::operator-(const cMatrix44 & Other) const
{
  cMatrix44 Result(*this);
  Result -= Other;
  return Result;
}

// ---------------------------------------------------------------------------------------

cMatrix44 QAPI cMatrix44::operator*(const float Multiplier) const
{
  cMatrix44 Result(*this);
  Result *= Multiplier;
  return Result;
}




// =======================================================================================
// === Construction/Destruction ==========================================================
// =======================================================================================

cMatrix44 QAPI cMatrix44::operator-() const
{
  cMatrix44 Result(*this);
  return Result.operator-();
}

// ---------------------------------------------------------------------------------------

cMatrix44 & QAPI cMatrix44::operator-()
{
  m00 = -m00; m01 = -m01; m02 = -m02; m03 = -m03;
  m10 = -m10; m11 = -m11; m12 = -m12; m13 = -m13;
  m20 = -m20; m21 = -m21; m22 = -m22; m23 = -m23;
  m30 = -m30; m31 = -m31; m32 = -m32; m33 = -m33;
  return *this;
}




// =======================================================================================
// === Construction/Destruction ==========================================================
// =======================================================================================

bool cMatrix44::operator==(const cMatrix44 & Other) const
{
  return m00 == Other.m00 && m01 == Other.m01 && m02 == Other.m02 && m03 == Other.m03 &&
         m10 == Other.m10 && m11 == Other.m11 && m12 == Other.m12 && m13 == Other.m13 &&
         m20 == Other.m20 && m21 == Other.m21 && m22 == Other.m22 && m23 == Other.m23 &&
         m30 == Other.m30 && m31 == Other.m31 && m32 == Other.m32 && m33 == Other.m33;
}

// ---------------------------------------------------------------------------------------

namespace
{

bool _isAlmostEqual(const float a, const float b, const float error)
{
  if (a < b)
  {
    return (b - a) <= error;
  }
  else
  {
    return (a - b) <= error;
  }
}

}
/*
bool cMatrix44::isAlmostEqual(const cMatrix44 & Other, const float Error) const
{
  return _isAlmostEqual(m00, Other.m00, Error) &&_isAlmostEqual(m01, Other.m01, Error) && _isAlmostEqual(m02, Other.m02, Error) && _isAlmostEqual(m03, Other.m03, Error) &&
         _isAlmostEqual(m10, Other.m10, Error) &&_isAlmostEqual(m11, Other.m11, Error) && _isAlmostEqual(m12, Other.m12, Error) && _isAlmostEqual(m13, Other.m13, Error) &&
         _isAlmostEqual(m20, Other.m20, Error) &&_isAlmostEqual(m21, Other.m21, Error) && _isAlmostEqual(m22, Other.m22, Error) && _isAlmostEqual(m23, Other.m23, Error) &&
         _isAlmostEqual(m30, Other.m30, Error) &&_isAlmostEqual(m31, Other.m31, Error) && _isAlmostEqual(m32, Other.m32, Error) && _isAlmostEqual(m33, Other.m33, Error);
}
*/
// ---------------------------------------------------------------------------------------

bool cMatrix44::operator!=(const cMatrix44 & Other) const
{
  return !operator==(Other);
}




// =======================================================================================
// === getScale ==========================================================================
// =======================================================================================

cfXYZ QAPI cMatrix44::getScale(void) const
{
  cfXYZ Result;

	if (m01 == 0.0f && m02 == 0.0f)
	{
		Result.x = m00;
	}
	else
	{
		Result.x = cfXYZ(&m00).magnitude();
	}

	if (m10 == 0.0f && m12 == 0.0f)
	{
		Result.y = m11;
	}
	else
	{
		Result.y = cfXYZ(&m10).magnitude();
	}

	if (m20 == 0.0f && m21 == 0.0f)
	{
		Result.z = m22;
	}
	else
	{
		Result.z = cfXYZ(&m20).magnitude();
	}

  return Result;
}




// =======================================================================================
// === getShear ==========================================================================
// =======================================================================================

cfXYZ QAPI cMatrix44::getShear(void) const
{
  // Not implemented yet!
  QKERNELBASEVERIFY(false);
  return cfXYZ();
}




// =======================================================================================
// === setIsotropicAndScale/getIsotropic/getIsotropicAndScale ============================
// =======================================================================================

cMatrix44 & QAPI cMatrix44::setIsotropicAndScale(
  const cMatrix44 & NewIsotropic,
  const cfXYZ &     NewScale)
{
  setRow(0, NewIsotropic.getRow(0) * NewScale.x);
  setRow(1, NewIsotropic.getRow(1) * NewScale.x);
  setRow(2, NewIsotropic.getRow(2) * NewScale.x);

  return *this;
}

// ---------------------------------------------------------------------------------------

cMatrix44 QAPI cMatrix44::getIsotropic(void) const
{
  cMatrix44 Result(kZeroMatrix);
  Result.setRow(0, getRow(0).normalise());
  Result.setRow(1, getRow(1).normalise());
  Result.setRow(2, getRow(2).normalise());
  return Result;
}

// ---------------------------------------------------------------------------------------

void QAPI cMatrix44::getIsotropicAndScale(
  cMatrix44 * const ResultMatrix,
  cfXYZ * const     ResultScale) const
{
  cMatrix44 TmpMatrix(*this);
  TmpMatrix.transpose33();

  cfXYZ TmpScale(TmpMatrix.getRow(0).magnitudeSquared(), TmpMatrix.getRow(1).magnitudeSquared(), TmpMatrix.getRow(2).magnitudeSquared());

  if (TmpScale == cfXYZ(1, 1, 1))
  {
    *ResultScale = cfXYZ(1, 1, 1);

    ResultMatrix->setRow(0, getRow(0));
    ResultMatrix->setRow(1, getRow(1));
    ResultMatrix->setRow(2, getRow(2));
  }
  else
  {
    *ResultScale = cfXYZ((float)sqrt((double)TmpScale.x), (float)sqrt((double)TmpScale.y), (float)sqrt((double)TmpScale.z));

    ResultMatrix->setRow(0, getRow(0).normalise());
    ResultMatrix->setRow(1, getRow(1).normalise());
    ResultMatrix->setRow(2, getRow(2).normalise());
  }
}




// =======================================================================================
// === setRotationFromAxes ===============================================================
// =======================================================================================

cMatrix44 & QAPI cMatrix44::setRotationFromAxes(
  const cfXYZ & NewFrontNormal,
  const cfXYZ & NewUpNormal)
{
#if defined(_DEBUG)
	// Make sure vectors are not parallel.
	QKERNELBASEVERIFY(!areParallel(NewFrontNormal, NewUpNormal));
#endif

	// New front normal...
  setRow(2, NewFrontNormal);

	// Force up normal to be orthogonal with front normal.
	if (!arePerpendicular(NewFrontNormal, NewUpNormal))
	{
		// New up normal...
    setRow(1, orthogonalise(NewFrontNormal, NewUpNormal).normalise());
	}
	else
	{
		// New up normal...
    setRow(1, NewUpNormal);
	}

	// New right normal...
  setRow(0, crossProduct(getRow(1), NewFrontNormal).normalise());

  return *this;
}




// =======================================================================================
// === setRotationFromEulerAngles/getRotationAsEulerAngles ===============================
// =======================================================================================

cMatrix44 & QAPI cMatrix44::setRotationFromEulerAngles(
  const cfXYZ &     NewUnorderedAngles,
  const eEulerOrder WhichOrder)
{
  cfXYZ NewAngles;

  switch (WhichOrder)
  {
    case eEulerXYZ :  { NewAngles = NewUnorderedAngles; break; }
  }

  // Pitch is rotation about X...
  const float CosPitch       = (float)cos(NewAngles.x);
  const float SinPitch       = (float)sin(NewAngles.x);
  // ...followed by yaw which is rotation about Y...
  const float CosYaw         = (float)cos(NewAngles.y);
  const float AccurateSinYaw = (float)sin(NewAngles.y);
  const float SinYaw         = AccurateSinYaw;
  // ...then finally roll which is rotation about Z.
  const float CosRoll        = (float)cos(NewAngles.z);
  const float SinRoll        = (float)sin(NewAngles.z);

  const float CosPitchSinYaw = CosPitch * SinYaw;
  const float SinPitchSinYaw = SinPitch * SinYaw;

  // New right vector...
  setRow(0, cfXYZ(CosYaw * CosRoll, -CosYaw * SinRoll, -AccurateSinYaw));

  // New up vector...
  setRow(1, cfXYZ((-SinPitchSinYaw * CosRoll) + (CosPitch * SinRoll),
                  ( SinPitchSinYaw * SinRoll) + (CosPitch * CosRoll),
                  -SinPitch  * CosYaw));

  // New Front vector...
  setRow(2, cfXYZ(( CosPitchSinYaw * CosRoll) + (SinPitch * SinRoll),
                  (-CosPitchSinYaw * SinRoll) + (SinPitch * CosRoll),
                  CosPitch * CosYaw));

  return *this;
}

// ---------------------------------------------------------------------------------------

void QAPI cMatrix44::getRotationAsEulerAngles(
  cfXYZ * const     ResultAngles,
  const eEulerOrder Order) const
{
  // Not implemented yet!
  QKERNELBASEVERIFY(false);
}




// =======================================================================================
// === setRotationFromAxisAngle/getRotationAsAxisAngle ===================================
// =======================================================================================

cMatrix44 & QAPI cMatrix44::setRotationFromAxisAngle(
  const cfXYZ & NewAxisNormal,
  const float   NewRadians)
{
  const float AccurateCos = (float)cos(NewRadians);
  const float InvCos      = 1.0f - AccurateCos;
  const float Sin         = (float)sin(NewRadians);

  const cfXYZ SinAxis   (NewAxisNormal * Sin);
  const cfXYZ InvCosAxis(NewAxisNormal * InvCos);

  const cfXYZ Right1(  AccurateCos,  SinAxis.z,   -SinAxis.y  );
  const cfXYZ Up1   ( -SinAxis.z,    AccurateCos,  SinAxis.x   );
  const cfXYZ Front1(  SinAxis.y,   -SinAxis.x,    AccurateCos );

  const cfXYZ Right2( InvCosAxis.x * NewAxisNormal.x, InvCosAxis.y * NewAxisNormal.x, InvCosAxis.z * NewAxisNormal.x );
  const cfXYZ Up2   ( InvCosAxis.x * NewAxisNormal.y, InvCosAxis.y * NewAxisNormal.y, InvCosAxis.z * NewAxisNormal.y );
  const cfXYZ Front2( InvCosAxis.x * NewAxisNormal.z, InvCosAxis.y * NewAxisNormal.z, InvCosAxis.z * NewAxisNormal.z );

  setRow(0, Right1 + Right2);
  setRow(1, Up1    + Up2);
  setRow(2, Front1 + Front2);

  return *this;
}

// ---------------------------------------------------------------------------------------

void QAPI cMatrix44::getRotationAsAxisAngle(
  cfXYZ * const ResultAxisNormal,
  float * const ResultRadians) const
{
  // Not implemented yet!
  QKERNELBASEVERIFY(false);
}




// =======================================================================================
// === transformVertex/transformVertices =================================================
// =======================================================================================

cfXYZ QAPI cMatrix44::transformVertex(const cfXYZ & Vertex) const
{
#if QSWITCH_SME

  if ((PtrToUlong(this) & 0x0000000f) == 0)
  {
    // Aligned - we can do op using SME

    cfXYZW Result; 
    Result =  cfXYZW(Vertex.x) * r0;
    Result += cfXYZW(Vertex.y) * r1;
    Result += cfXYZW(Vertex.z) * r2;
    Result += cfXYZW(1.0f)     * r3;
    return cfXYZ((float *)&Result);
  }
  else

#endif

  {
	  return cfXYZ(
		  ( Vertex.x * m00 ) + ( Vertex.y * m10 ) + ( Vertex.z * m20 ) + m30,
		  ( Vertex.x * m01 ) + ( Vertex.y * m11 ) + ( Vertex.z * m21 ) + m31,
		  ( Vertex.x * m02 ) + ( Vertex.y * m12 ) + ( Vertex.z * m22 ) + m32 );
  }
}

// ---------------------------------------------------------------------------------------

void QAPI cMatrix44::transformVertices(
  std::vector<cfXYZ> * const DestVertices,
  const std::vector<cfXYZ> & SrcVertices) const
{
  const std::vector<cfXYZ>::size_type NumVertices = SrcVertices.size();

  if (DestVertices->size() < NumVertices) DestVertices->resize(NumVertices);

  for (std::vector<cfXYZ>::size_type i = 0; i < NumVertices; ++i)
	{
		const cfXYZ v = SrcVertices[i];

		(*DestVertices)[i] = cfXYZ(
			( v.x * m00 ) + ( v.y * m10 ) + ( v.z * m20 ) + m30,
			( v.x * m01 ) + ( v.y * m11 ) + ( v.z * m21 ) + m31,
			( v.x * m02 ) + ( v.y * m12 ) + ( v.z * m22 ) + m32 );
	}
}




// =======================================================================================
// === getParity =========================================================================
// =======================================================================================

bool QAPI cMatrix44::getParity(void) const
{
  return dotProduct(crossProduct(getRow(0), getRow(1)), getRow(2)) < 0.0f;
}




// =======================================================================================
// === inverse ===========================================================================
// =======================================================================================

#define _mm_ror_ps(vec,i)	\
	(((i)%4) ? (_mm_shuffle_ps(vec,vec, _MM_SHUFFLE((unsigned char)(i+3)%4,(unsigned char)(i+2)%4,(unsigned char)(i+1)%4,(unsigned char)(i+0)%4))) : (vec))

cMatrix44 QAPI cMatrix44::inverse(void) const throw(cNotSupported)
{
#if QSWITCH_SME

  if ((PtrToUlong(this) & 0x0000000f) == 0)
  {
    // Aligned - we can do op using SME

    cfXYZW Sign_PNPN;	// + - + -
    cfXYZW Sign_NPNP;	// - + - +
    cfXYZW _ZERONE_;		// 1 0 0 1
    const unsigned long Sign_PNPN_Src[] = { 0x00000000, 0x80000000, 0x00000000, 0x80000000 };
    const unsigned long Sign_NPNP_Src[] = { 0x80000000, 0x00000000, 0x80000000, 0x00000000 };
    const float         _ZERONE_Src[]   = { 1.0f, 0.0f, 0.0f, 1.0f };
    memcpy((void *)&(Sign_PNPN), &(Sign_PNPN_Src), sizeof(__m128));
    memcpy((void *)&(Sign_NPNP), &(Sign_NPNP_Src), sizeof(__m128));
    memcpy((void *)&(_ZERONE_),  &(_ZERONE_Src),   sizeof(__m128));

	  __m128 Va,Vb,Vc;
	  __m128 R1,R2,R3,tt,tt2;
	  __m128 sum,Det,RDet;
	  cMatrix44 Minterms;
	  __m128 trns0,trns1,trns2,trns3;

	  // Calculating the minterms for the first line.

	  // _mm_ror_ps is just a macro using _mm_shuffle_ps().
	  tt = r3; tt2 = _mm_ror_ps(r2,1); 
	  Vc = _mm_mul_ps(tt2,_mm_ror_ps(tt,0));					// V3'�V4
	  Va = _mm_mul_ps(tt2,_mm_ror_ps(tt,2));					// V3'�V4"
	  Vb = _mm_mul_ps(tt2,_mm_ror_ps(tt,3));					// V3'�V4^

	  R1 = _mm_sub_ps(_mm_ror_ps(Va,1),_mm_ror_ps(Vc,2));		// V3"�V4^ - V3^�V4"
	  R2 = _mm_sub_ps(_mm_ror_ps(Vb,2),_mm_ror_ps(Vb,0));		// V3^�V4' - V3'�V4^
	  R3 = _mm_sub_ps(_mm_ror_ps(Va,0),_mm_ror_ps(Vc,1));		// V3'�V4" - V3"�V4'

	  tt = r1;
	  Va = _mm_ror_ps(tt,1);		sum = _mm_mul_ps(Va,R1);
	  Vb = _mm_ror_ps(tt,2);		sum = _mm_add_ps(sum,_mm_mul_ps(Vb,R2));
	  Vc = _mm_ror_ps(tt,3);		sum = _mm_add_ps(sum,_mm_mul_ps(Vc,R3));

	  // Calculating the determinant.
	  Det = _mm_mul_ps(sum,r0);
	  Det = _mm_add_ps(Det,_mm_movehl_ps(Det,Det));

	  Minterms.r0 = _mm_xor_ps(sum,Sign_PNPN);

	  // Calculating the minterms of the second line (using previous results).
	  tt = _mm_ror_ps(r0,1);		sum = _mm_mul_ps(tt,R1);
	  tt = _mm_ror_ps(tt,1);		sum = _mm_add_ps(sum,_mm_mul_ps(tt,R2));
	  tt = _mm_ror_ps(tt,1);		sum = _mm_add_ps(sum,_mm_mul_ps(tt,R3));
	  Minterms.r1 = _mm_xor_ps(sum,Sign_NPNP);

	  // Testing the determinant.
	  Det = _mm_sub_ss(Det,_mm_shuffle_ps(Det,Det,1));
	  #ifdef ZERO_SINGULAR
		  int flag = _mm_comieq_ss(Det,_mm_sub_ss(tt,tt));
		  // Using _mm_sub_ss, as only the first element has to be zeroed.
	  #endif

	  // Calculating the minterms of the third line.
	  tt = _mm_ror_ps(r0,1);
	  Va = _mm_mul_ps(tt,Vb);									// V1'�V2"
	  Vb = _mm_mul_ps(tt,Vc);									// V1'�V2^
	  Vc = _mm_mul_ps(tt,r1);								// V1'�V2

	  R1 = _mm_sub_ps(_mm_ror_ps(Va,1),_mm_ror_ps(Vc,2));		// V1"�V2^ - V1^�V2"
	  R2 = _mm_sub_ps(_mm_ror_ps(Vb,2),_mm_ror_ps(Vb,0));		// V1^�V2' - V1'�V2^
	  R3 = _mm_sub_ps(_mm_ror_ps(Va,0),_mm_ror_ps(Vc,1));		// V1'�V2" - V1"�V2'

	  tt = _mm_ror_ps(r3,1);		sum = _mm_mul_ps(tt,R1);
	  tt = _mm_ror_ps(tt,1);		sum = _mm_add_ps(sum,_mm_mul_ps(tt,R2));
	  tt = _mm_ror_ps(tt,1);		sum = _mm_add_ps(sum,_mm_mul_ps(tt,R3));
	  Minterms.r2 = _mm_xor_ps(sum,Sign_PNPN);

	  // Dividing is FASTER than rcp_nr! (Because rcp_nr causes many register-memory RWs).
	  RDet = _mm_div_ss(_mm_load_ss((float *)&_ZERONE_), Det);
	  RDet = _mm_shuffle_ps(RDet,RDet,0x00);

	  // Devide the first 12 minterms with the determinant.
	  Minterms.r0 = _mm_mul_ps(Minterms.r0, RDet);
	  Minterms.r1 = _mm_mul_ps(Minterms.r1, RDet);
	  Minterms.r2 = _mm_mul_ps(Minterms.r2, RDet);

	  // Calculate the minterms of the forth line and devide by the determinant.
	  tt = _mm_ror_ps(r2,1);		sum = _mm_mul_ps(tt,R1);
	  tt = _mm_ror_ps(tt,1);		sum = _mm_add_ps(sum,_mm_mul_ps(tt,R2));
	  tt = _mm_ror_ps(tt,1);		sum = _mm_add_ps(sum,_mm_mul_ps(tt,R3));
	  Minterms.r3 = _mm_xor_ps(sum,Sign_NPNP);
	  Minterms.r3 = _mm_mul_ps(Minterms.r3, RDet);

	  #ifdef ZERO_SINGULAR
	  // Check if the matrix is inversable.
	  // Uses a delayed branch here, so the test would not interfere the calculations.
	  // Assuming most of the matrices are inversable, the previous calculations are 
	  // not wasted. It is faster this way.
		  if (flag) {
			  ZeroMatrix();
			  return 0.0f;
		  }
	  #endif

	  // Now we just have to transpose the minterms matrix.
	  trns0 = _mm_unpacklo_ps(Minterms.r0,Minterms.r1);
	  trns1 = _mm_unpacklo_ps(Minterms.r2,Minterms.r3);
	  trns2 = _mm_unpackhi_ps(Minterms.r0,Minterms.r1);
	  trns3 = _mm_unpackhi_ps(Minterms.r2,Minterms.r3);

    cMatrix44 Result;
	  Result.r0 = _mm_movelh_ps(trns0,trns1);
	  Result.r1 = _mm_movehl_ps(trns1,trns0);
	  Result.r2 = _mm_movelh_ps(trns2,trns3);
	  Result.r3 = _mm_movehl_ps(trns3,trns2);

    return Result;
  }
  else

#endif

  {
    // Not implemented yet
    QKERNELBASEVERIFY(false);
    return cMatrix44();
  }
}




// =======================================================================================
// === getDeterminant ====================================================================
// =======================================================================================

float QAPI cMatrix44::getDeterminant(void) const
{
#if QSWITCH_SME

  if ((PtrToUlong(this) & 0x0000000f) == 0)
  {
    // Aligned - we can do op using SME

	  __m128 Va,Vb,Vc;
	  __m128 R1,R2,R3,t1,t2,sum;
	  cfXYZW Det;

	  // First, Let's calculate the first four minterms of the first line
	  t1 = r3; t2 = _mm_ror_ps(r2,1); 
	  Vc = _mm_mul_ps(t2,_mm_ror_ps(t1,0));					// V3'�V4
	  Va = _mm_mul_ps(t2,_mm_ror_ps(t1,2));					// V3'�V4"
	  Vb = _mm_mul_ps(t2,_mm_ror_ps(t1,3));					// V3'�V4^

	  R1 = _mm_sub_ps(_mm_ror_ps(Va,1),_mm_ror_ps(Vc,2));		// V3"�V4^ - V3^�V4"
	  R2 = _mm_sub_ps(_mm_ror_ps(Vb,2),_mm_ror_ps(Vb,0));		// V3^�V4' - V3'�V4^
	  R3 = _mm_sub_ps(_mm_ror_ps(Va,0),_mm_ror_ps(Vc,1));		// V3'�V4" - V3"�V4'

	  Va = _mm_ror_ps(r1,1);		sum = _mm_mul_ps(Va,R1);
	  Vb = _mm_ror_ps(Va,1);		sum = _mm_add_ps(sum,_mm_mul_ps(Vb,R2));
	  Vc = _mm_ror_ps(Vb,1);		sum = _mm_add_ps(sum,_mm_mul_ps(Vc,R3));

	  // Now we can calculate the determinant:
	  Det = cfXYZW(_mm_mul_ps(sum, r0));
	  Det = cfXYZW(_mm_add_ps(Det,_mm_movehl_ps(Det,Det)));
	  Det = cfXYZW(_mm_sub_ss(Det,_mm_shuffle_ps(Det,Det,1)));

	  return Det.m[0];
  }
  else

#endif

  {
    // Not implemented yet
    QKERNELBASEVERIFY(false);
    return 0.0f;
  }
}




// =======================================================================================
// === transformRect =====================================================================
// =======================================================================================

cfRect QAPI cMatrix44::transformRect(const cfRect & Rect) const
{
  return cfRect(
    Rect.org.x * m00 + m30,
    Rect.org.y * m11 + m31,
    Rect.len.x * m00,
    Rect.len.y * m11);
/*
  cfXYZW ResultOrg(cfXYZW(Rect.org.x) * r0);
  ResultOrg += cfXYZW(Rect.org.y) * r1;
  ResultOrg += cfXYZW(1.0f)       * r3;

  cfXYZW ResultLen(cfXYZW(Rect.len.x) * r0);
  ResultLen += cfXYZW(Rect.len.y) * r1;

  return cfRect(cfXY(ResultOrg[0], ResultOrg[1]), cfXY(ResultLen[0], ResultLen[1]));
*/
}
