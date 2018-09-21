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

#pragma once

#include "QKernelBaseEnviron.h"
#include "QKernelBaseMacros.h"

#if defined(_MSC_VER)
  #pragma pack(push, 8)
#endif

#if (_MSC_VER >= 1300) // VC 7.0 and beyond
#define _L(X) L##X
#endif

//{{{  includes
#include <limits.h>
#include <float.h>
#include <math.h>

#include <crtdbg.h>
#include <assert.h>
#include <stdarg.h>

#include <string>

#include <map>
#include <set>
#include <list>
#include <deque>
#include <vector>

#include <iostream>
#include <iomanip>
#include <sstream>

#if defined(QSWITCH_SME)
  // Include xmmintrin.h from MSVC 7 (or processor pack) if you want inline Streaming
  // Media Extensions (SME) when using some math classes (e.g. cfXYZW, cMatrix44).
  #define _INC_MALLOC
  #include <mmintrin.h>
  #include <xmmintrin.h>
#endif

#if defined(_MSC_VER)
  #ifdef _M_X64
    #include <windows.h>
    #include <intrin.h>
    #include <emmintrin.h>
    #define _INTSIZEOF(x) _SLOTSIZEOF(x)
  #else
    #include <intrin.h>
  #endif
#endif
//}}}

namespace vfs
{
  //{{{  Integer Types
  // =======================================================================================
  #if defined(_MSC_VER)

    typedef signed   __int8     Int8;
    typedef unsigned __int8     UInt8;
    typedef signed   __int16    Int16;
    typedef unsigned __int16    UInt16;
    typedef signed   __int32    Int32;
    typedef unsigned __int32    UInt32;
    typedef signed   __int64    Int64;
    typedef unsigned __int64    UInt64;

    // The IntPtr is guaranteed to be the same size as a pointer.  Its
    // size will change with pointer size (32/64).  It should be used
    // anywhere that a pointer is cast to an integer type. UIntPtr is
    // the unsigned variation.
    #if defined(_WIN64)
    typedef __int64 IntPtr;
    typedef unsigned __int64 UIntPtr;
    #else
    typedef _W64 int IntPtr;
    typedef _W64 unsigned int UIntPtr;
    #endif

    // The reverse of size_t (As copied from crtdefs.h)
    #if defined(_WIN64)
    typedef _W64 unsigned int   not_size_t;
    #else
    typedef unsigned __int64    not_size_t;
    #endif

  #else

    #error Compiler not supported!

  #endif
  //}}}
  //{{{  Floating Point Types
  // =======================================================================================
  typedef float  Float32;
  #define FLOAT32_EPSILON FLT_EPSILON // Smallest such that 1.0 + FLOAT32_EPSILON != 1.0
  #define FLOAT32_MAX     FLT_MAX
  #define FLOAT32_MIN     FLT_MIN

  typedef double Float64;
  #define FLOAT64_EPSILON DBL_EPSILON // Smallest such that 1.0 + FLOAT64_EPSILON != 1.0
  #define FLOAT64_MAX     DBL_MAX
  #define FLOAT64_MIN     DBL_MIN
  //}}}
  //{{{
  inline int signedRound (const double value) {
  // return int rounded from a double, handles -ve values
    return (value > 0.0) ? int(value + 0.5) : -int(-value + 0.5); }
  //}}}
  //{{{  more float->int
  // =======================================================================================
  #if defined(_MSC_VER)
  /*
  We can't use 'inline __declspec(naked)' functions here because Visual C won't actually
  inline them.
  */

  // Using this instead of integer cast will avoid implicit call to _ftol which is slow.
  __forceinline int fastRoundNearest(const float v)
  {
    return _mm_cvtss_si32( _mm_load_ss( &v ) );
  }
  __forceinline int fastRoundNearest(const double v)
  {
    return _mm_cvtsd_si32( _mm_load_pd( &v ) );
  }

  #endif
  //}}}
  //{{{  Trig constants
  // =======================================================================================
  const double kDblPi       = 3.1415926535897932384626433832795;
  const float  kPi          = 3.1415926535f;
  const float  kTwoPi       = 2.0f * kPi;
  const float  kPiTwo       = kPi / 2.0f;
  const float  kThreePiTwo  = (3.0f * kPi) / 2.0f;
  //}}}
  //{{{  Char, String, StringStream
  // =======================================================================================
  // The vfs Kernel uses Unicode characters thoughout. A Char is an unsigned short
  // representing one Unicode character. To define a Unicode string literal use the syntax
  // L"Hello" - note the L prefix. This works for single characters too, e.g. L'z'.
  typedef std::basic_string<wchar_t>       String;

  // The vfs Kernel uses the Standard Template Library "wstring" template class.
  typedef std::basic_stringstream<wchar_t> StringStream;
  typedef wchar_t Char;
  //}}}
  //{{{  String widen/narrow
  // =======================================================================================
  // Use widen function to ensure that all strings are wide. For ease of coding, there is no
  // cost (in release build) of calling widen on a wide string. Note: these functions can also
  // be passed const char/Char* as well.
  inline String QAPI widen(const String& ToWiden) { return ToWiden; }
  extern String QAPI widen(const std::string& ToWiden);
  extern std::string QAPI narrow(const String& ToNarrow);
  extern String QAPI trim(const String& ToTrim);

  extern String QAPI toUpper (const String& ToUpper);
  extern String QAPI toLower (const String& ToLower);
  //}}}
  //{{{
  // Helper function: safeCopy.
  // Zeros the whole of dest, then copies Min(src_sz,dest_sz) bytes from src to dest
  extern void QAPI safeCopy (void* dest, size_t dest_sz, const void* src, size_t src_sz);
  //}}}

  //{{{  lexical_cast
  template <typename DEST, typename SRC> DEST lexical_cast (SRC input)
  {
    DEST output = DEST();
    StringStream str;
    str << input;
    str >> output;
    return output;
  }

  template <typename DEST> DEST lexical_cast (std::string input)
  {
    DEST output = DEST();
    std::stringstream str;
    str << input;
    str >> output;
    return output;
  }

  // For (Ss)tring to (Ss)tring 'casts' std::basic_stringstream will split at white space points
  // so we specialise specifically for these conversions.
  // NOTE they are inline to stop multiple defs against KernelBase.
  template <> inline String      lexical_cast (String input)           { return input; }
  template <> inline String      lexical_cast (std::string input)      { return widen(input); }
  template <> inline std::string lexical_cast (String input)           { return narrow(input); }
  template <> inline std::string lexical_cast (std::string input)      { return input; }
  //}}}

  //{{{
  // =======================================================================================
  // Returns the larger of two values. The Max template function compares two values and
  // returns the value of the larger one. The arguments can be of any type with a '<'
  // operator. Both arguments and the return value must be of the same type.
  //
  // <b>Example Source</b>
  //
  //  #include <stdlib.h>
  //  #include <stdio.h>
  //  #include <QKernelSDK.h>
  //
  //  void main()
  //  {
  //      int a = 10;
  //      int b = 21;
  //      printf("The larger  of %d and %d is %d\n", a, b, vfs::Max( a, b ));
  //      printf("The smaller of %d and %d is %d\n", a, b, vfs::Min( a, b ));
  //  }
  //
  // <b>Example Output</b>
  //
  //  The larger of 10 and 21 is 21
  //  The smaller of 10 and 21 is 10

  template<typename T> inline T QAPI Max(T a, T b)
  {
    return b < a ? a : b;
  }

  template<typename T> inline T QAPI Max(T a, T b, T cs...)
  {
    return Max<T>(a, Max<T>(b, cs));
  }
  //}}}
  //{{{
  // =======================================================================================
  // Returns the smaller of two values. The Min template function compares two values and
  // returns the value of the smaller one. The arguments can be of any type with a '<'
  // operator. Both arguments and the return value must be of the same type.
  //
  // <b>Example Source</b>
  //
  //  #include <stdlib.h>
  //  #include <stdio.h>
  //  #include <QKernelSDK.h>
  //
  //  void main()
  //  {
  //      int a = 10;
  //      int b = 21;
  //      printf("The larger  of %d and %d is %d\n", a, b, vfs::Max( a, b ));
  //      printf("The smaller of %d and %d is %d\n", a, b, vfs::Min( a, b ));
  //  }
  //
  // <b>Example Output</b>
  //
  //  The larger of 10 and 21 is 21
  //  The smaller of 10 and 21 is 10

  template<typename T> inline T QAPI Min(T a, T b)
  {
    return a < b ? a : b;
  }

  template<typename T> inline T QAPI Min(T a, T b, T cs...)
  {
    return Min<T>(a, Min<T>(b, cs));
  }
  //}}}
  //{{{
  // =======================================================================================
  // Swaps two values. The Swap template function compares two values and
  // returns the value of the smaller one. The arguments can be of any type with a '<'
  // operator. Both arguments and the return value must be of the same type.
  //
  // <b>Example Source</b>
  //
  //  #include <stdlib.h>
  //  #include <stdio.h>
  //  #include <kernel.h>
  //
  //  void main()
  //  {
  //      int a = 10;
  //      int b = 21;
  //      printf("The original values are %d and %d\n", a, b);
  //      vfs::Swap(&a, &b);
  //      printf("The values swapped are %d and %d\n", a, b);
  //  }
  //
  // <b>Example Output</b>
  //
  // The original values are 10 and 21
  // The values swapped are 21 and 10

  template<typename T> inline void QAPI Swap(
    T * const a,
    T * const b)
  {
    const T tmp = *a; *a = *b; *b = tmp;
  }
  //}}}

  // Exceptions
  //{{{
  // =======================================================================================
  class cRecoverable
  {
  protected :
    cRecoverable(
      const char* const  SrcFilename,
      const unsigned long SrcLineNumber,
      const Char* const  MsgFormat,
      ...);
    cRecoverable(
      const char* const  SrcFilename,
      const unsigned long SrcLineNumber,
      const Char* const  MsgFormat,
      va_list             VArgs);

  public :
    virtual ~cRecoverable() {}

    cRecoverable();
    cRecoverable(const cRecoverable& other);

    void QAPI operator=(const cRecoverable& other);
    bool QAPI operator==(const cRecoverable& other) const;
    bool QAPI operator!=(const cRecoverable& other) const;

    String QAPI getSrcFilename() const;
    unsigned long QAPI getSrcLineNumber() const;

    String QAPI getMsg() const;
    void QAPI setMsg(const String& NewMsg);

  public :
    const unsigned long m_Magic;

  private :
    String          m_SrcFilename;
    unsigned long   m_SrcLineNumber;
    String          m_Msg;
  };

  const unsigned long kRecoverableMagic = 0xfee1badd;
  //}}}
  //{{{
  // =======================================================================================
  class cNoRoom : public cRecoverable
  {
  public :
    virtual ~cNoRoom() {}
    cNoRoom(const char* const  SrcFilename, const unsigned long SrcLineNumber, const Char* MsgFormat, ...) :
      cRecoverable(SrcFilename, SrcLineNumber, MsgFormat, (va_list)&MsgFormat + _INTSIZEOF(MsgFormat))
    {
    }
  };
  //}}}
  //{{{
  // =======================================================================================
  class cOSError : public cRecoverable
  {
  public :
    virtual ~cOSError() {}
    cOSError(const char* const  SrcFilename, const unsigned long SrcLineNumber, const Char* MsgFormat, ...) :
      cRecoverable(SrcFilename, SrcLineNumber, MsgFormat, (va_list)&MsgFormat + _INTSIZEOF(MsgFormat))
    {
    }
  };
  //}}}
  //{{{
  // =======================================================================================
  class cBadFormat : public cRecoverable
  {
  public :
    virtual ~cBadFormat() {}
    cBadFormat(const char* const  SrcFilename, const unsigned long SrcLineNumber, const Char* MsgFormat, ...) :
      cRecoverable(SrcFilename, SrcLineNumber, MsgFormat, (va_list)&MsgFormat + _INTSIZEOF(MsgFormat))
    {
    }
  };
  //}}}
  //{{{
  // =======================================================================================
  class cNotSupported : public cRecoverable
  {
  public :
    virtual ~cNotSupported() {}
    cNotSupported(const char* const  SrcFilename, const unsigned long SrcLineNumber, const Char* MsgFormat, ...) :
      cRecoverable(SrcFilename, SrcLineNumber, MsgFormat, (va_list)&MsgFormat + _INTSIZEOF(MsgFormat))
    {
    }
  };
  //}}}
  //{{{
  // =======================================================================================
  class cNotAvailable : public cRecoverable
  {
  public :
    virtual ~cNotAvailable() {};
    cNotAvailable(const char* const  SrcFilename, const unsigned long SrcLineNumber, const Char* MsgFormat, ...) :
      cRecoverable(SrcFilename, SrcLineNumber, MsgFormat, (va_list)&MsgFormat + _INTSIZEOF(MsgFormat))
    {
    }
  };
  //}}}
  //{{{
  // =======================================================================================
  class cNotImplemented : public cRecoverable
  {
  public :
    virtual ~cNotImplemented() {};
    cNotImplemented(const char* const  SrcFilename, const unsigned long SrcLineNumber, const Char* MsgFormat, ...) :
      cRecoverable(SrcFilename, SrcLineNumber, MsgFormat, (va_list)&MsgFormat + _INTSIZEOF(MsgFormat))
    {
    }
  };
  //}}}
  //{{{
  // =======================================================================================
  class cNotFound : public cRecoverable
  {
  public :
    virtual ~cNotFound() {}
    cNotFound(const char* const  SrcFilename, const unsigned long SrcLineNumber, const Char* MsgFormat, ...) :
      cRecoverable(SrcFilename, SrcLineNumber, MsgFormat, (va_list)&MsgFormat + _INTSIZEOF(MsgFormat))
    {
    }
  };
  //}}}
  //{{{
  // =======================================================================================
  class cTimedOut : public cRecoverable
  {
  public :
    virtual ~cTimedOut() {}
    cTimedOut(const char* const  SrcFilename, const unsigned long SrcLineNumber, const Char* MsgFormat, ...) :
      cRecoverable(SrcFilename, SrcLineNumber, MsgFormat, (va_list)&MsgFormat + _INTSIZEOF(MsgFormat))
    {
    }
  };
  //}}}
  //{{{
  // =======================================================================================
  class cAccessDenied : public cRecoverable
  {
  public :
    virtual ~cAccessDenied() {}
    cAccessDenied(const char* const  SrcFilename, const unsigned long SrcLineNumber, const Char* MsgFormat, ...) :
      cRecoverable(SrcFilename, SrcLineNumber, MsgFormat, (va_list)&MsgFormat + _INTSIZEOF(MsgFormat))
    {
    }
  };
  //}}}
  //{{{
  // =======================================================================================
  class cAborted : public cRecoverable
  {
  public :
    virtual ~cAborted() {}
    cAborted(const char* const  SrcFilename, const unsigned long SrcLineNumber, const Char* MsgFormat, ...) :
      cRecoverable(SrcFilename, SrcLineNumber, MsgFormat, (va_list)&MsgFormat + _INTSIZEOF(MsgFormat))
    {
    }
  };
  //}}}

  // Vectors
  //{{{
  class cXY
  {
  public:
    int x; //!< Position on the X axis.
    int y; //!< Position on the Y axis.
    // Use cXYZ if you need a Z axis.

    inline cXY() : x(0), y(0)                         {}
    inline cXY (int xx, int yy): x(xx), y(yy)         {}

    inline cXY (const cXY& p) : x(p.x), y(p.y)        {}
    inline cXY& QAPI operator = (const cXY& p)        { x = p.x; y = p.y; return *this; }

    inline cXY QAPI operator - () const               { return cXY(-x, -y); }

    inline cXY QAPI operator * (const int p) const    { return cXY(x * p, y * p); }
    inline cXY QAPI operator / (const int p) const    { return cXY(x / p, y / p); }
    inline cXY QAPI operator * (const cXY& p) const   { return cXY(x * p.x, y * p.y); }
    inline cXY QAPI operator / (const cXY& p) const   { return cXY(x / p.x, y / p.y); }
    inline cXY QAPI operator + (const cXY& p) const   { return cXY(x + p.x, y + p.y); }
    inline cXY QAPI operator - (const cXY& p) const   { return cXY(x - p.x, y - p.y); }

    inline cXY& QAPI operator *= (const int p)        { x *= p; y *= p; return *this; }
    inline cXY& QAPI operator /= (const int p)        { x /= p; y /= p; return *this; }
    inline cXY& QAPI operator *= (const cXY& p)       { x *= p.x; y *= p.y; return *this; }
    inline cXY& QAPI operator /= (const cXY& p)       { x /= p.x; y /= p.y; return *this; }
    inline cXY& QAPI operator += (const cXY& p)       { x += p.x; y += p.y; return *this; }
    inline cXY& QAPI operator -= (const cXY& p)       { x -= p.x; y -= p.y; return *this; }

    inline bool QAPI operator == (const cXY& p) const { return (x == p.x && y == p.y); }
    inline bool QAPI operator != (const cXY& p) const { return !(*this == p); }
    inline bool QAPI operator < (const cXY& p) const  { return ((x < p.x) || (x == p.x && y < p.y)); }

    inline int QAPI area() const                      { return x * y; }
    inline int QAPI magnitudeSquared() const          { return x * x + y * y; }
    inline int QAPI magnitude() const                 { return (int) sqrt((double) magnitudeSquared()); }

    explicit cXY (const String& serialised);
    String QAPI toString() const;
  };
  //}}}
  //{{{
  class cfXY
  {
  public:
    float x; //!< Position on the X axis.
    float y; //!< Position on the Y axis.
    // Use cfXYZ if you need a Z axis.

    inline cfXY() : x(0.0f), y(0.0f)                   {}
    inline cfXY (float xx, float yy) : x(xx), y(yy)    {}

    inline cfXY (const cfXY& p) : x(p.x), y(p.y)       {}
    inline cfXY& QAPI operator = (const cfXY& p)       { x = p.x; y = p.y; return *this; }

    inline cfXY QAPI operator - () const               { return cfXY (-x, -y); }

    inline cfXY QAPI operator * (const float p) const  { return cfXY(x * p, y * p); }
    inline cfXY QAPI operator / (const float p) const  { return cfXY(x / p, y / p); }
    inline cfXY QAPI operator * (const cfXY& p) const  { return cfXY(x * p.x, y * p.y); }
    inline cfXY QAPI operator / (const cfXY& p) const  { return cfXY(x / p.x, y / p.y); }
    inline cfXY QAPI operator + (const cfXY& p) const  { return cfXY(x + p.x, y + p.y); }
    inline cfXY QAPI operator - (const cfXY& p) const  { return cfXY(x - p.x, y - p.y); }

    inline cfXY& QAPI operator *= (const float p)      { x *= p; y *= p; return *this; }
    inline cfXY& QAPI operator /= (const float p)      { x /= p; y /= p; return *this; }
    inline cfXY& QAPI operator *= (const cfXY& p)      { x *= p.x; y *= p.y; return *this; }
    inline cfXY& QAPI operator /= (const cfXY& p)      { x /= p.x; y /= p.y; return *this; }
    inline cfXY& QAPI operator += (const cfXY& p)      { x += p.x; y += p.y; return *this; }
    inline cfXY& QAPI operator -= (const cfXY& p)      { x -= p.x; y -= p.y; return *this; }

    inline bool QAPI operator == (const cfXY& p) const { return (x == p.x && y == p.y); }
    inline bool QAPI operator != (const cfXY& p) const { return !(*this == p); }
    inline bool QAPI operator < (const cfXY& p) const  { return ((x < p.x) || (x == p.x && y < p.y)); }

    inline float QAPI area() const                     { return x * y; }
    inline float QAPI magnitudeSquared() const         { return x * x + y * y; }
    inline float QAPI magnitude() const                { return (float) sqrt((double) magnitudeSquared()); }

    explicit cfXY (const String& serialised);
    String QAPI serialise() const;
  };
  //}}}
  //{{{
  class cdXY
  {
  public:
    double x; //!< Position on the X axis.
    double y; //!< Position on the Y axis.
    // Use cdXYZ if you need a Z axis.

    inline cdXY() : x(0.0), y(0.0)                     {}
    inline cdXY (double xx, double yy) : x(xx), y(yy)  {}

    inline cdXY (const cdXY& p) : x(p.x), y(p.y)       {}
    inline cdXY& QAPI operator = (const cdXY& p)       { x = p.x; y = p.y; return *this; }

    inline cdXY QAPI operator - () const               { return cdXY (-x, -y); }

    inline cdXY QAPI operator * (const double p) const  { return cdXY(x * p, y * p); }
    inline cdXY QAPI operator / (const double p) const  { return cdXY(x / p, y / p); }
    inline cdXY QAPI operator * (const cdXY& p) const  { return cdXY(x * p.x, y * p.y); }
    inline cdXY QAPI operator / (const cdXY& p) const  { return cdXY(x / p.x, y / p.y); }
    inline cdXY QAPI operator + (const cdXY& p) const  { return cdXY(x + p.x, y + p.y); }
    inline cdXY QAPI operator - (const cdXY& p) const  { return cdXY(x - p.x, y - p.y); }

    inline cdXY& QAPI operator *= (const double p)      { x *= p; y *= p; return *this; }
    inline cdXY& QAPI operator /= (const double p)      { x /= p; y /= p; return *this; }
    inline cdXY& QAPI operator *= (const cdXY& p)      { x *= p.x; y *= p.y; return *this; }
    inline cdXY& QAPI operator /= (const cdXY& p)      { x /= p.x; y /= p.y; return *this; }
    inline cdXY& QAPI operator += (const cdXY& p)      { x += p.x; y += p.y; return *this; }
    inline cdXY& QAPI operator -= (const cdXY& p)      { x -= p.x; y -= p.y; return *this; }

    inline bool QAPI operator == (const cdXY& p) const { return (x == p.x && y == p.y); }
    inline bool QAPI operator != (const cdXY& p) const { return !(*this == p); }
    inline bool QAPI operator < (const cdXY& p) const  { return ((x < p.x) || (x == p.x && y < p.y)); }

    inline double QAPI area() const                    { return x * y; }
    inline double QAPI magnitudeSquared() const        { return x * x + y * y; }
    inline double QAPI magnitude() const               { return sqrt(magnitudeSquared()); }

    explicit cdXY (const String& serialised);
    String QAPI serialise() const;
  };
  //}}}
  //{{{
  class cXYZ
  {
  public:
    int x; //!< Position on the X axis.
    int y; //!< Position on the Y axis.
    int z; //!< Position on the Z axis.

    inline cXYZ() : x(0), y(0), z(0)                     {}
    inline cXYZ (int xx, int yy, int zz) : x(xx), y(yy), z(zz) {}
    inline explicit cXYZ (const int * const Values) : x(Values[0]), y(Values[1]), z(Values[2]) {}
    inline explicit cXYZ (int v) : x(v), y(v), z(v)      {}

    inline cXYZ (const cXYZ& p) : x(p.x), y(p.y), z(p.z) {}
    inline cXYZ QAPI operator = (const cXYZ& p)          { x = p.x; y = p.y; z= p.z; return *this; }

    inline cXYZ QAPI operator - () const                 { return cXYZ (-x, -y, -z); }

    inline cXYZ QAPI operator * (const int p) const      { return cXYZ (x * p, y * p, z * p); }
    inline cXYZ QAPI operator / (const int p) const      { return cXYZ (x / p, y / p, z / p); }
    inline cXYZ QAPI operator * (const cXYZ& p) const    { return cXYZ (x * p.x, y * p.y, z * p.z); }
    inline cXYZ QAPI operator / (const cXYZ& p) const    { return cXYZ (x / p.x, y / p.y, z / p.z); }
    inline cXYZ QAPI operator + (const cXYZ& p) const    { return cXYZ (x + p.x, y + p.y, z + p.z); }
    inline cXYZ QAPI operator - (const cXYZ& p) const    { return cXYZ (x - p.x, y - p.y, z - p.z); }

    inline cXYZ& QAPI operator *= (const int p)          { x *= p; y *= p; z *= p; return *this; }
    inline cXYZ& QAPI operator /= (const int p)          { x /= p; y /= p; z /= p; return *this; }
    inline cXYZ& QAPI operator *= (const cXYZ& p)        { x *= p.x; y *= p.y; z *= p.z; return *this; }
    inline cXYZ& QAPI operator /= (const cXYZ& p)        { x /= p.x; y /= p.y; z /= p.z; return *this; }
    inline cXYZ& QAPI operator += (const cXYZ& p)        { x += p.x; y += p.y; z += p.z; return *this; }
    inline cXYZ& QAPI operator -= (const cXYZ& p)        { x -= p.x; y -= p.y; z -= p.z; return *this; }

    inline bool QAPI operator == (const cXYZ& p) const   { return (x == p.x && y == p.y && z == p.z); }
    inline bool QAPI operator != (const cXYZ& p) const   { return !(*this == p); }
    inline bool QAPI operator < (const cXYZ& p) const    { return ((x < p.x) || (x == p.x && y < p.y) || (x == p.x && y == p.y && z < p.z)); }

    inline int QAPI volume() const                       { return x * y * z; }
    inline int QAPI magnitudeSquared() const             { return x * x + y * y + z * z; }
    inline int QAPI magnitude() const                    { return (int) sqrt((double) magnitudeSquared()); }

    explicit cXYZ (const String& serialised);
    String QAPI serialise() const;
  };
  //}}}
  //{{{
  class cfXYZ
  {
  public:
    float x; //!< Position on the X axis.
    float y; //!< Position on the Y axis.
    float z; //!< Position on the Z axis.

    inline cfXYZ() : x(0.0f), y(0.0f), z(0.0f)             {}
    inline cfXYZ (float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}
    inline /*explicit*/ cfXYZ (const float * const Values) : x(Values[0]), y(Values[1]), z(Values[2]) {}
    inline explicit cfXYZ (float v) : x(v), y(v), z(v)     {}

    inline cfXYZ (const cfXYZ& p) : x(p.x), y(p.y), z(p.z) {}
    inline cfXYZ& QAPI operator = (const cfXYZ& p)         { x = p.x; y = p.y; z= p.z; return *this; }

    inline cfXYZ QAPI operator - () const                  { return cfXYZ(-x, -y, -z); }

    inline cfXYZ QAPI operator * (const float p) const     { return cfXYZ (x * p, y * p, z * p); }
    inline cfXYZ QAPI operator / (const float p) const     { return cfXYZ (x / p, y / p, z / p); }
    inline cfXYZ QAPI operator * (const cfXYZ& p) const    { return cfXYZ (x * p.x, y * p.y, z * p.z); }
    inline cfXYZ QAPI operator / (const cfXYZ& p) const    { return cfXYZ (x / p.x, y / p.y, z / p.z); }
    inline cfXYZ QAPI operator + (const cfXYZ& p) const    { return cfXYZ (x + p.x, y + p.y, z + p.z); }
    inline cfXYZ QAPI operator - (const cfXYZ& p) const    { return cfXYZ (x - p.x, y - p.y, z - p.z); }

    inline cfXYZ& QAPI operator *= (const float p)         { x *= p; y *= p; z *= p; return *this; }
    inline cfXYZ& QAPI operator /= (const float p)         { x /= p; y /= p; z /= p; return *this; }
    inline cfXYZ& QAPI operator *= (const cfXYZ& p)        { x *= p.x; y *= p.y; z *= p.z; return *this; }
    inline cfXYZ& QAPI operator /= (const cfXYZ& p)        { x /= p.x; y /= p.y; z /= p.z; return *this; }
    inline cfXYZ& QAPI operator += (const cfXYZ& p)        { x += p.x; y += p.y; z += p.z; return *this; }
    inline cfXYZ& QAPI operator -= (const cfXYZ& p)        { x -= p.x; y -= p.y; z -= p.z; return *this; }

    inline bool QAPI operator == (const cfXYZ& p) const    { return (x == p.x && y == p.y && z == p.z); }
    inline bool QAPI operator != (const cfXYZ& p) const    { return !(*this == p); }
    inline bool QAPI operator < (const cfXYZ& p) const     { return ((x < p.x) || (x == p.x && y < p.y) || (x == p.x && y == p.y && z < p.z)); }

    inline float QAPI volume() const { return x * y * z; }
    inline float QAPI magnitudeSquared() const { return x * x + y * y + z * z; }
    inline float QAPI magnitude() const { return (float) sqrt((double) magnitudeSquared()); }

    /*
    Calculates a new W vector that is at a right angle to V on the plane formed by V W.

      E.g.  Result
          |
          |   W
          |  /
          | /
          |/______ V

    - Note that the magnitude of the W will NOT be preserved.
    - Returns pResult.
    - V & W should not be parallel.
    */
    friend cfXYZ QAPI orthogonalise (const cfXYZ& V, const cfXYZ& W);

    /*
    Calculates the cross product of two normals/vectors. The result is a vector
    perpendicular to the plane formed by V W in a left-handed coordinate system.

      E.g.  Result
          |
          |
          |____ V
           /
         W/

    - Returns pResult.
    - The magnitude of the result is the area of the parallelogram defined by V W.
    - V & W should not be parallel.
    */
    friend cfXYZ QAPI crossProduct (const cfXYZ& V, const cfXYZ& W);

    /*
    Scales a vector so that its magnitude is 1.
    */
    cfXYZ& QAPI normalise();

    friend inline bool QAPI areParallel(const cfXYZ& V, const cfXYZ& W) { return V == W || V == -W; }

    /*
    Calculates the dot product of two normals, this and other.
    */
    friend inline float QAPI dotProduct(const cfXYZ& V, const cfXYZ& W) { return V.x * W.x + V.y * W.y + V.z * W.z; }

    friend inline bool QAPI arePerpendicular(const cfXYZ& V, const cfXYZ& W) { return dotProduct(V, W) == 0.0f; }

    explicit cfXYZ(const String& serialised);
    String QAPI serialise() const;
  };
  //}}}
  //{{{  class cfXYZW
  #if defined(QSWITCH_SME)
  class __declspec(align(16)) cfXYZW
  {
  public:
    #pragma warning(push)
    #pragma warning(disable: 4201)  // Ignore "nonstandard extension used : nameless struct/union"
    union
    {
      float m[4]; // As a column vector
      struct
      {
        float x; //!< Position on the X axis.
        float y; //!< Position on the Y axis.
        float z; //!< Position on the Z axis.
        float w; //!< Position on the W axis.
      };
      struct
      {
        __m128 v;
      };
    };
    #pragma warning(pop)

    inline cfXYZW() : v(_mm_set1_ps(0.0f))                {}
    inline cfXYZW(const float xx, const float yy, const float zz, const float ww) : v(_mm_set_ps(ww,zz,yy,xx)) {}
    inline explicit cfXYZW(const float * const Values) : x(Values[0]), y(Values[1]), z(Values[2]), w(Values[3]) {}
    inline explicit cfXYZW(const float a) : v(_mm_set1_ps(a)) {}

    inline cfXYZW(const __m128 p) : v(p)                  {}
    inline QAPI operator __m128() const                   { return v; }

    inline cfXYZW(const cfXYZW& p) : v(p.v)               {}
    inline cfXYZW QAPI operator = (const cfXYZW& p)       { v = p.v; return *this; }

    inline cfXYZW QAPI operator - () const                { return cfXYZW(-x, -y, -z, -w); }

    inline cfXYZW QAPI operator * (const float n) const   { return _mm_mul_ps(v, _mm_set1_ps(n)); }
    inline cfXYZW QAPI operator / (const float n) const   { return _mm_div_ps(v, _mm_set1_ps(n)); }
    inline cfXYZW QAPI operator * (const cfXYZW& p) const { return _mm_mul_ps(v, p.v); }
    inline cfXYZW QAPI operator / (const cfXYZW& p) const { return _mm_div_ps(v, p.v); }
    inline cfXYZW QAPI operator + (const cfXYZW& p) const { return _mm_add_ps(v, p.v); }
    inline cfXYZW QAPI operator - (const cfXYZW& p) const { return _mm_sub_ps(v, p.v); }

    inline cfXYZW& QAPI operator *= (const float n)       { v = _mm_mul_ps(v, _mm_set1_ps(n)); return *this; }
    inline cfXYZW& QAPI operator /= (const float n)       { v = _mm_div_ps(v, _mm_set1_ps(n)); return *this; }
    inline cfXYZW& QAPI operator *= (const cfXYZW& p)     { v = _mm_mul_ps(v, p.v); return *this; }
    inline cfXYZW& QAPI operator /= (const cfXYZW& p)     { v = _mm_div_ps(v, p.v); return *this; }
    inline cfXYZW& QAPI operator += (const cfXYZW& p)     { v = _mm_add_ps(v, p.v); return *this; }
    inline cfXYZW& QAPI operator -= (const cfXYZW& p)     { v = _mm_sub_ps(v, p.v); return *this; }

    inline bool QAPI operator == (const cfXYZW& p) const  { return (x == p.x && y == p.y && z == p.z && w == p.w); }
    inline bool QAPI operator != (const cfXYZW& p) const  { return !(*this == p); }
    inline bool QAPI operator < (const cfXYZW& p) const   { return ((x < p.x) || (x == p.x && y < p.y) || (x == p.x && y == p.y && z < p.z) || (x == p.x && y == p.y && z == p.z && w < p.w)); }

    inline float QAPI volume() const { return x * y * z * w; }
    inline float QAPI magnitudeSquared() const { return x * x + y * y + z * z + w * w; }
    inline float QAPI magnitude() const { return (float) sqrt((double) magnitudeSquared()); }

    /*
    Calculates a new W vector that is at a right angle to V on the plane formed by V W.

      E.g.  Result
          |
          |   W
          |  /
          | /
          |/______ V

    - Note that the magnitude of the W will NOT be preserved.
    - Returns pResult.
    - V & W should not be parallel.
    */
    friend cfXYZW QAPI orthogonalise (const cfXYZW& V, const cfXYZW& W);

    /*
    Calculates the cross product of two normals/vectors. The result is a vector
    perpendicular to the plane formed by V W in a left-handed coordinate system.

      E.g.  Result
          |
          |
          |____ V
           /
         W/

    - Returns pResult.
    - The magnitude of the result is the area of the parallelogram defined by V W.
    - V & W should not be parallel.
    */
    friend cfXYZW QAPI crossProduct (const cfXYZW& V, const cfXYZW& W);

    /*
    Scales a vector so that its magnitude is 1.
    */
    cfXYZW& QAPI normalise();

    friend inline bool QAPI areParallel(const cfXYZW& V, const cfXYZW& W) { return V == W || V == -W; }

    /*
    Calculates the dot product of two normals, this and other.
    */
    friend inline float QAPI dotProduct(const cfXYZW& V, const cfXYZW& W) { return V.x * W.x + V.y * W.y + V.z * W.z + V.w * W.w; }

    friend inline bool QAPI arePerpendicular(const cfXYZW& V, const cfXYZW& W) { return dotProduct(V, W) == 0.0f; }

    explicit cfXYZW(const String& Serialised);
    String QAPI serialise() const;
  };

  #else
  class cfXYZW;
  #endif
  //}}}

  // Rect
  //{{{  class cRect

  // We won't allow any of the ints used by cRect to get larger than this to avoid overflow.
  // Alternate solution is to use __in64 intermediates but this would slow down the
  // majority of cRect operations.
  #define RECTINT_MAX (LONG_MAX / 2)
  #define RECTINT_MIN (LONG_MIN / 2)
  #define RECTINT_BIG (65535)         // Build large rects using this rather than RECTINT_MAX to give calculations some headroom.

  class cRect
  {
  public:
    cXY org; //!< Top-left origin of the rectangle.
    cXY len; //!< Size of the rectangle, both width and height must both be greater than or equal to zero.

    inline cRect() : org(), len()                           {}

    #if defined(_DEBUG)
    cRect(cXY xyOrg, cXY xyLen);
    cRect(int xorg, int yorg, cXY xyLen);
    cRect(cXY xyOrg, int xlen, int ylen);
    cRect(int xorg, int yorg, int xlen, int ylen);
    #else
    inline cRect(cXY xyOrg, cXY xyLen) : org(xyOrg), len(xyLen) {}
    inline cRect(int xorg, int yorg, cXY xyLen) : org(xorg, yorg), len(xyLen) {}
    inline cRect(cXY xyOrg, int xlen, int ylen) : org(xyOrg), len(xlen, ylen) {}
    inline cRect(int xorg, int yorg, int xlen, int ylen) : org(xorg, yorg), len(xlen, ylen) {}
    #endif

    inline cRect(const cRect& other) : org(other.org), len(other.len)  { }
    inline cRect& QAPI operator = (const cRect& other)      { org = other.org; len = other.len; return *this; }

    inline cRect QAPI operator * (int p) const              { return cRect(org * p, len * p); }
    inline cRect QAPI operator / (int p) const              { return cRect(org / p, len / p); }
    inline cRect QAPI operator * (const cXY& p) const       { return cRect(org * p, len * p); }
    inline cRect QAPI operator / (const cXY& p) const       { return cRect(org / p, len / p); }
    inline cRect QAPI operator + (const cRect& p) const     { return cRect(org + p.org, len + p.len); }
    inline cRect QAPI operator - (const cRect& p) const     { return cRect(org - p.org, len - p.len); }

    inline cRect& QAPI operator *= (int p)                  { org *= p; len *= p; return *this; }
    inline cRect& QAPI operator /= (int p)                  { org /= p; len /= p; return *this; }
    inline cRect& QAPI operator *= (const cXY& p)           { org *= p; len *= p; return *this; }
    inline cRect& QAPI operator /= (const cXY& p)           { org /= p; len /= p; return *this; }
    inline cRect& QAPI operator += (const cRect& p)         { org += p.org; len += p.len; return *this; }
    inline cRect& QAPI operator -= (const cRect& p)         { org -= p.org; len -= p.len; return *this; }

    inline bool QAPI operator == (const cRect& other) const { return (org == other.org && len == other.len); }
    inline bool QAPI operator != (const cRect& other) const { return !(*this == other); }
    inline bool QAPI operator < (const cRect& other) const  { return ((org < other.org) || (org == other.org && len < other.len)); }

    // normalise x and y lengths to be >= 0
    cRect QAPI normalise(const cRect& a) const;

    /*
    Intersection

         this +---T---+
              |       |
              |   ####|##             In this function we are clipping
              L   ####R##             'this' rectangle against 'a'.
              |   ####|##
              |   ####|## a
              +---B---+##
                  #######
                  #######
    */
    cRect QAPI intersect(cRect a) const;

    cRect QAPI Union(cRect a) const;

    #if defined(_DEBUG)
    bool QAPI isContainedBy(const cRect& a) const;
    bool QAPI isContaining(const cXY& a) const;
    #else
    //{{{
    inline bool QAPI isContainedBy(const cRect& a) const
    {
      return org.x         >= a.org.x &&
             org.x + len.x <= a.org.x + a.len.x &&
             org.y         >= a.org.y &&
             org.y + len.y <= a.org.y + a.len.y;
    }
    //}}}
    //{{{
    inline bool QAPI isContaining(const cXY& a) const
    {
      return a.x >= org.x &&
             a.x <  org.x + len.x &&
             a.y >= org.y &&
             a.y <  org.y + len.y;
    }
    //}}}
    #endif

    //{{{
    inline bool QAPI isDegenerate() const
    {
      return (len.x <= 0 || len.y <= 0);
    }
    //}}}
    //{{{
    inline bool QAPI isOverlapped(const cRect& a) const
    {
      return (org.x < a.org.x + a.len.x) && (org.x + len.x > a.org.x) &&
             (org.y < a.org.y + a.len.y) && (org.y + len.y > a.org.y);
    }
    //}}}

    explicit cRect(const String& SerialisedRect);
    String QAPI serialise() const;

    bool QAPI isValid() const;
  };
  //}}}
  //{{{  class cfRect
  // =======================================================================================
  class cfRect
  {
  public:
    cfXY org; //!< Top-left origin of the rectangle.
    cfXY len; //!< Size of the rectangle, both width and height must both be greater than or equal to zero.

    inline cfRect() : org(), len()                            {}

    inline cfRect(cfXY xyOrg, cfXY xyLen) : org(xyOrg), len(xyLen) {}
    inline cfRect(float xorg, float yorg, cfXY xyLen) : org(xorg, yorg), len(xyLen) {}
    inline cfRect(cfXY xyOrg, float xlen, float ylen) : org(xyOrg), len(xlen, ylen) {}
    inline cfRect(float xorg, float yorg, float xlen, float ylen) : org(xorg, yorg), len(xlen, ylen) {}

    inline cfRect(const cfRect& r) : org(r.org), len(r.len)   {}
    inline cfRect& QAPI operator = (const cfRect& other)      { org = other.org; len = other.len; return *this; }

    inline cfRect QAPI operator * (float p) const             { return cfRect(org * p, len * p); }
    inline cfRect QAPI operator / (float p) const             { return cfRect(org / p, len / p); }
    inline cfRect QAPI operator * (const cfXY& p) const       { return cfRect(org * p, len * p); }
    inline cfRect QAPI operator / (const cfXY& p) const       { return cfRect(org / p, len / p); }
    inline cfRect QAPI operator + (const cfRect& p) const     { return cfRect(org + p.org, len + p.len); }
    inline cfRect QAPI operator - (const cfRect& p) const     { return cfRect(org - p.org, len - p.len); }

    inline cfRect& QAPI operator *= (float p)                 { org *= p; len *= p; return *this; }
    inline cfRect& QAPI operator /= (float p)                 { org /= p; len /= p; return *this; }
    inline cfRect& QAPI operator *= (const cfXY& p)           { org *= p; len *= p; return *this; }
    inline cfRect& QAPI operator /= (const cfXY& p)           { org /= p; len /= p; return *this; }
    inline cfRect& QAPI operator += (const cfRect& p)         { org += p.org; len += p.len; return *this; }
    inline cfRect& QAPI operator -= (const cfRect& p)         { org -= p.org; len -= p.len; return *this; }

    inline bool QAPI operator == (const cfRect& other) const  { return (org == other.org && len == other.len); }
    inline bool QAPI operator != (const cfRect& other) const  { return !(*this == other); }
    inline bool QAPI operator < (const cfRect& other) const   { return ((org < other.org) || (org == other.org && len < other.len)); }

    /*
    Intersection

         this +---T---+
              |       |
              |   ####|##             In this function we are clipping
              L   ####R##             'this' rectangle against 'a'.
              |   ####|##
              |   ####|## a
              +---B---+##
                  #######
                  #######
    */
    cfRect QAPI intersect(const cfRect& a) const;

    cfRect QAPI Union(const cfRect& a) const;

    //{{{
    inline bool QAPI isContainedBy(const cfRect& a) const
    {
      return org.x         >= a.org.x &&
             org.x + len.x <= a.org.x + a.len.x &&
             org.y         >= a.org.y &&
             org.y + len.y <= a.org.y + a.len.y;
    }
    //}}}
    //{{{
    inline bool QAPI isContainedBy(const cfXY& a) const
    {
      return org.x         >= 0 &&
             org.x + len.x <= a.x &&
             org.y         >= 0 &&
             org.y + len.y <= a.y;
    }
    //}}}
    //{{{
    inline bool QAPI isContaining(const cfXY& a) const
    {
      return a.x >= org.x &&
             a.x <  org.x + len.x &&
             a.y >= org.y &&
             a.y <  org.y + len.y;
    }
    //}}}
    //{{{
    inline bool QAPI isDegenerate() const
    {
      return len.x <= 0 || len.y <= 0;
    }
    //}}}
    //{{{
    inline bool QAPI isOverlapped(const cfRect& a) const
    {
      return (org.x < a.org.x + a.len.x) && (org.x + len.x > a.org.x) &&
             (org.y < a.org.y + a.len.y) && (org.y + len.y > a.org.y);
    }
    //}}}

    cfRect QAPI round() const;

    explicit cfRect(const String& SerialisedRect);
    String QAPI serialise() const;

    bool QAPI isValid() const;
  };
  //}}}

  // Matrices
  //{{{  class cMatrix44
  // =======================================================================================
  #if defined(QSWITCH_SME)

  enum eEulerOrder
  {
    eEulerXYZ,      // Only this at the moment.
  };

  // 4x4 matrix of floats. Makes use of Streaming SIMD Extensions (SME) for better performance.
  class __declspec(align(16)) cMatrix44
  {
  public :
    #pragma warning(push)
    #pragma warning(disable: 4201)  // Ignore "nonstandard extension used : nameless struct/union"
    union
    {
      float m[4][4];
      struct
      {
        float m00, m01, m02, m03;
        float m10, m11, m12, m13;
        float m20, m21, m22, m23;
        float m30, m31, m32, m33;
      };
      struct
      {
        __m128 r0, r1, r2, r3;
      };
    };
    #pragma warning(pop)

    // --- Construction & Destruction ----------------------------------------------------
    cMatrix44() {};
    cMatrix44(const float * const ArrayToCopy);
    cMatrix44(const cMatrix44& other);
    cMatrix44(const float m00, const float m01, const float m02, const float m03,
              const float m10, const float m11, const float m12, const float m13,
              const float m20, const float m21, const float m22, const float m23,
              const float m30, const float m31, const float m32, const float m33);

    cMatrix44(const Char* const Serialised) throw(cBadFormat);
    cMatrix44(const String& Serialised) throw(cBadFormat);

    // --- Element Access ----------------------------------------------------------------
    inline float& QAPI operator()(const unsigned int Row, const unsigned int Column) { return m[Row][Column]; }
    inline float QAPI operator()(const unsigned int Row, const unsigned int Column) const { return m[Row][Column]; }
    inline cfXYZ QAPI getRow(const unsigned int Row) const { return cfXYZ(&m[Row][0]); }
    inline cfXYZ QAPI getColumn(const unsigned int Column) const { return cfXYZ(m[0][Column], m[1][Column], m[2][Column]); }
    inline cMatrix44& QAPI setRow(const unsigned int Row, const cfXYZ& v) { m[Row][0] = v.x; m[Row][1] = v.y; m[Row][2] = v.z; return *this; }
    inline cMatrix44& QAPI setColumn(const unsigned int Column, const cfXYZ& v) { m[0][Column] = v.x; m[1][Column] = v.y; m[2][Column] = v.z; return *this; }

    // --- Casting -----------------------------------------------------------------------
    QAPI operator float*() { return &m[0][0]; }
    QAPI operator const float*() const { return &m[0][0]; }

    #if defined(__D3DXMATH_H__)
    QAPI operator D3DXMATRIX*() { return (D3DXMATRIX *)m; }
    #endif
    #if defined(__D3DXMATH_H__)
    QAPI operator const D3DXMATRIX*() const { return (D3DXMATRIX *)m; }
    #endif

    // --- Assignment --------------------------------------------------------------------
    cMatrix44& operator*=(const cMatrix44& other);
    cMatrix44& operator+=(const cMatrix44& other);
    cMatrix44& operator-=(const cMatrix44& other);
    cMatrix44& QAPI operator*=(const float Multiplier);

    // --- Unary Operators ---------------------------------------------------------------
    cMatrix44 QAPI operator-() const;
    cMatrix44& QAPI operator-();

    // --- Binary Operators --------------------------------------------------------------
    cMatrix44 QAPI operator*(const cMatrix44& other) const;
    cMatrix44 QAPI operator+(const cMatrix44& other) const;
    cMatrix44 QAPI operator-(const cMatrix44& other) const;
    cMatrix44 QAPI operator*(const float Multiplier) const;

    // --- Comparision --------------------------------------------------------------------
    bool operator==(const cMatrix44& other) const;
    bool operator!=(const cMatrix44& other) const;

    // --- Translation -------------------------------------------------------------------
    // Overwrites first 3 elements of row 3 - others are unaltered.
    //{{{
    inline cMatrix44& QAPI setTranslation(const cfXYZ& NewTranslation)
    {
      m30 = NewTranslation.x; m31 = NewTranslation.y; m32 = NewTranslation.z;
      return *this;
    }
    //}}}
    //{{{
    inline cfXYZ QAPI getTranslation() const
    {
      return cfXYZ(m30, m31, m32);
    }
    //}}}

    // --- Scale -------------------------------------------------------------------------
    // Sets the scaling of the individual X, Y& Z axes.
    // Overwrites diagonal m00, m11 and m22 - others are unaltered.
    // - None of scaling components should be zero.
    //{{{
    inline cMatrix44& QAPI setScale(const cfXYZ& NewScale)
    {
      #if defined(_DEBUG)
      assert(NewScale.x != 0.0f);
      assert(NewScale.y != 0.0f);
      assert(NewScale.z != 0.0f);
      #endif
      m00 = NewScale.x; m11 = NewScale.y; m22 = NewScale.z;
      return *this;
    }
    //}}}

    // Sets the scaling of all the axes.
    // - You would usually have set this matrix to identity first.
    // - nNewScale should not be zero.
    // - Returns true if the new scale is different from the current one.
    //{{{
    inline cMatrix44& QAPI setUniformScale(const float NewScale)
    {
      #if defined(_DEBUG)
      assert(NewScale != 0.0f);
      #endif
      m00 = NewScale; m11 = NewScale; m22 = NewScale;
      return *this;
    }
    //}}}

    cfXYZ QAPI getScale() const;

    // --- Shear -------------------------------------------------------------------------
    // See also SetRotationAndScale below.
    // Overwrites m01, m02, m10, m12, m20 and m21 - others are unaltered.
    //{{{
    cMatrix44& QAPI setShear(const cfXYZ& NewShear)
    {
      m01 = NewShear.x;
      m02 = NewShear.x;
      m10 = NewShear.y;
      m12 = NewShear.y;
      m20 = NewShear.z;
      m21 = NewShear.z;
      return *this;
    }
    //}}}

    // See also SetRotationAndScale below.
    cfXYZ QAPI getShear() const;

    // --- Rotation isotropic ------------------------------------------------------------
    // Overwrites first 3 elements from first 3 rows - others are unaltered.
    //{{{
    cMatrix44& QAPI setIsotropicAndScale(
      const cMatrix44& NewIsotropic,
      const cfXYZ&     NewScale);
    //}}}

    cMatrix44 QAPI getIsotropic() const;

    //{{{
    void QAPI getIsotropicAndScale(
      cMatrix44 * const ResultIsotropic,
      cfXYZ * const     ResultScale) const;
    //}}}

    // --- Rotation axes -----------------------------------------------------------------

    /*
    Set the rotation so that the Z-axis points along pNewFront and Y points
    along the perpendicular on the plane formed by pNewFront & pNewUp.
    - Overwrites first 3 elements from first 3 rows - others are unaltered.
    - Normals do not _have_ to be orthogonal, the up axis will be recalculated as
      othogonal if necessary.
    - Normals should not be parallel - if this is the case then it is likely you've
      encountered a gimbal lock problem in your code, try
      setRotationFromAxisAngles instead.
    */
    cMatrix44& QAPI setRotationFromAxes(
      const cfXYZ& NewFrontNormal,
      const cfXYZ& NewUpNormal);

    // * Returns pResultFront.
    //{{{
    inline void QAPI getRotationAsAxes(
     cfXYZ  * const ResultFrontNormal,
     cfXYZ  * const ResultUpNormal) const
    {
      *ResultFrontNormal = cfXYZ(m20, m21, m22);
      *ResultUpNormal    = cfXYZ(m10, m11, m12);
    }
    //}}}

    // --- Rotation euler ----------------------------------------------------------------

    /*
    Sets the rotation so that the axes are the result, if EULERORDER_XYZ is set, of
    sucessive rotations on X (pitch), Y (yaw) then Z (roll).
    - Overwrites first 3 elements from first 3 rows - others are unaltered.
    */
    //{{{
    cMatrix44& QAPI setRotationFromEulerAngles(
      const cfXYZ&     NewAngles,
      const eEulerOrder WhichOrder);
    //}}}
    //{{{
    void QAPI getRotationAsEulerAngles(
      cfXYZ * const     ResultAngles,
      const eEulerOrder WhichOrder) const;
    //}}}

    // --- Rotation axis & angle ---------------------------------------------------------

    /*
    Aligns the rotation so that the Z axis points along pNewAxis with an anti-
    clockwise twist of nNewRadians about it.
    - Overwrites first 3 elements from first 3 rows - others are unaltered.
    */
    //{{{
    cMatrix44& QAPI setRotationFromAxisAngle(
      const cfXYZ& NewAxisNormal,
      const float   NewRadians);
    //}}}

    //{{{
    void QAPI getRotationAsAxisAngle(
      cfXYZ * const ResultAxisNormal,
      float * const ResultRadians) const;
    //}}}

    // --- Transform Vertices/Rect -------------------------------------------------------

    cfXYZ QAPI transformVertex (const cfXYZ& Vertex) const;

    void QAPI transformVertices (
      std::vector<cfXYZ> * const DestVertices,
      const std::vector<cfXYZ>& SrcVertices) const;

    cfRect QAPI transformRect(
      const cfRect& Rect) const;

    // --- Misc --------------------------------------------------------------------------

    /*
    Returns the 'parity' of the matrix. If one axis of the matrix is scaled
    negatively this switches the 'parity'. However if you scale two axes it will
    flip it back. Three times switches it again. When rendering a mesh, if you
    scale something along one axis, it turns 'inside out'. That is the direction
    of the normals are reversed.
    */
    bool QAPI getParity() const;

    /*
    The transpose of a matrix has its elements mirrored across its major diagonal.
    If you are only using this matrix for rotations and is always isotropic (no
    scaling) then the transpose of the matrix is also its inverse
    */
    //{{{
    inline cMatrix44& QAPI transpose33()
    {
      Swap<float>(&m10, &m01);
      Swap<float>(&m20, &m02);
      Swap<float>(&m30, &m03);
      Swap<float>(&m12, &m21);
      Swap<float>(&m13, &m31);
      Swap<float>(&m23, &m32);
      return *this;
    }
    //}}}

    /*
    Calculates the inverse of a matrix. The inverse effectively undoes the transformation of the original.
    A matrix may not always be invertible. If the matrix reduces dimensionality (e.g. a
    3D projection matrix) then it is not invertible, these are called 'singular' matrices.
    Otherwise it is said to have 'rank n' or 'full rank'

    The inversion routine which uses Gaussian reduction.
    */
    cMatrix44 QAPI inverse() const throw(cNotSupported);

    float QAPI getDeterminant() const;
    String QAPI serialise() const;
  };

  extern const cMatrix44 kIdentityMatrix;
  extern const cMatrix44 kZeroMatrix;

  #else
  class cMatrix44;
  #endif
  //}}}

  //{{{  toFloat/toInt conversions

  // Convert toFloat/toDouble
  inline cfXY QAPI toFloat(const cXY& r) { return cfXY((float) r.x, (float) r.y); }
  inline cfXY QAPI toFloat(const cdXY& r) { return cfXY((float) r.x, (float) r.y); }
  inline cdXY QAPI toDouble(const cXY& r) { return cdXY((double) r.x, (double) r.y); }
  inline cdXY QAPI toDouble(const cfXY& r) { return cdXY((double) r.x, (double) r.y); }
  inline cfRect QAPI toFloat(const cRect& r) { return cfRect(toFloat(r.org), toFloat(r.len)); }

  // Convert toIntTruncated, result is truncated and will be <= original
  inline cXY QAPI toIntTruncated(const cfXY& r) { return cXY((int) r.x, (int) r.y); }
  inline cXY QAPI toIntTruncated(const cdXY& r) { return cXY((int) r.x, (int) r.y); }
  inline cRect QAPI toIntTruncated(const cfRect& r) { return cRect(toIntTruncated(r.org), toIntTruncated(r.len)); }

  // Convert toIntRounded, result is rounded to nearest
  inline cXY QAPI toIntRounded(const cfXY& r) { return cXY(fastRoundNearest(r.x), fastRoundNearest(r.y)); }
  inline cXY QAPI toIntRounded(const cdXY& r) { return cXY(fastRoundNearest(r.x), fastRoundNearest(r.y)); }
  inline cRect QAPI toIntRounded(const cfRect& r) { return cRect(toIntRounded(r.org), toIntRounded(r.len)); }

  // toInt == toIntTruncated
  inline cXY QAPI toInt(const cfXY& r) { return toIntTruncated(r); }
  inline cXY QAPI toInt(const cdXY& r) { return toIntTruncated(r); }
  inline cRect QAPI toInt(const cfRect& r) { return toIntTruncated(r); }
  //}}}

  //{{{  threadSafeAdd/getCurrentThreadStackBase/getCurrentThreadStackLimit
  // =======================================================================================
  #if defined(_MSC_VER)
    inline int __cdecl threadSafeAdd(volatile int * const Dest, const int ToAdd)
    {
      return InterlockedAdd((volatile long *)Dest, ToAdd) - ToAdd;
    }

    inline void * __cdecl getCurrentThreadStackBase()
    {
      return (void *)__readgsqword(0x08);
    }

    inline void * __cdecl getCurrentThreadStackLimit()
    {
      return (void *)__readgsqword(0x10);
    }
  #else
    #error Compiler not supported!
  #endif
  //}}}
  //{{{  getFuncCallerAddr
  // =======================================================================================
  #if defined(_MSC_VER)
    inline void * __cdecl getFuncCallerAddr()
    {
      return _ReturnAddress();
    }
  #else
    #error Compiler not supported!
  #endif
  //}}}
  //{{{  checkPointer2
  // =======================================================================================
  /*
  Returns true if the pointer is valid for length bytes. When the debugger is present this
  will assert.
  */
  #if defined(_DEBUG)
    extern bool QAPI checkPointer2(const void * Pointer, unsigned int NumBytes);
  #else
    #define checkPointer2(PTR, NUMBYTES) true
  #endif
  //}}}
  //{{{
  // =======================================================================================
  /*
  */
  inline bool isPointerToStack (const void* const Pointer)
  {
    return (Pointer < getCurrentThreadStackBase()) &&
       (Pointer > getCurrentThreadStackLimit());
  }
  //}}}

  // refCount, smartPtr
  //{{{  cRefCount
  // =======================================================================================
  // Expensive way of tracking who is keeping alive a smart pointable object. Define this
  // then paste in the following code into your object. Then call verifyRefCount(1) on it
  // when you expect the ref count to be 1 - the outstanding refs will be listed.
  // Base class for all ref countable objects.
  class cRefCount
  {
  public:
    //{{{  ~cRefCount
    /*
    */
    // ---------------------------------------------------------------------------------------
    //  virtual ~cRefCount ();
    //}}}
    //{{{
    // ---------------------------------------------------------------------------------------
    /*
    Primarily for use during a smart pointed objects constructor to protect it from premature
    destruction caused by temporarily making smart pointers to itself.
    */
      class cAddRefGuard
      {
      public :

        cAddRefGuard(cRefCount * const RefCount);
        ~cAddRefGuard();

      private :

        class cRefCount * const m_RefCount;
      };
    //}}}

    //{{{  inline void addRef(
    // ---------------------------------------------------------------------------------------
    /*
    */
      #if QSWITCH_TRACKPTR
      inline int /* prev count */ addRef(const void * const PtrID, const void * const ModuleAddr) const
      #else
      inline int /* prev count */addRef() const
      #endif
      {
        int prevCount = InterlockedIncrement((volatile long *)&m_RefCount) - 1;
        #if QSWITCH_TRACKPTR
          if (m_IsTracking) onAfterAddRef(PtrID, ModuleAddr, prevCount + 1);
        #endif
        return prevCount;
      }
    //}}}
    //{{{  inline bool removeRef(
    // ---------------------------------------------------------------------------------------
    /*
    Returns true on removing the last reference
    */
      #if QSWITCH_TRACKPTR
      inline bool removeRef(const void * const PtrID, const void * const ModuleAddr) const
      #else
      inline bool removeRef() const
      #endif
      {
      #if QSWITCH_TRACKPTR
        if (m_IsTracking) onBeforeRemoveRef(PtrID, ModuleAddr, m_RefCount);
      #endif
        return InterlockedDecrement((volatile long *)&m_RefCount) == 0;
      }
    //}}}
    //{{{
    // ---------------------------------------------------------------------------------------
    /*
    */
      inline unsigned int getNumRefs() const
      {
        return m_RefCount;
      }
    //}}}
    //{{{  verifyRefCount (const unsigned int RefCount)
    // ---------------------------------------------------------------------------------------
    /*
    */
      #if QSWITCH_TRACKPTR
      virtual void QAPI verifyRefCount(const unsigned int RefCount) const;
      virtual void QAPI setTracking(const bool IsTracking);
      #endif
    //}}}

  protected:
    cRefCount() :
    #if QSWITCH_TRACKPTR
      m_IsTracking(false),
    #endif
    m_RefCount(0)
    {}
    //do not copy the original count
    cRefCount(const cRefCount&) : m_RefCount(0) {}

    #if QSWITCH_TRACKPTR
    virtual void QAPI onAfterAddRef(const void * const PtrID, const void * const ModuleAddr, const unsigned int RefCount) const;
    virtual void QAPI onBeforeRemoveRef(const void * const PtrID, const void * const ModuleAddr, const unsigned int RefCount) const;
    #endif

  private:
    //{{{
    /*
    The count of how many references are held to this object.  it doesn't need to be
    volatile if I only access it by address.
    */
    mutable unsigned int m_RefCount;
    #if QSWITCH_TRACKPTR
    bool               m_IsTracking;
    #endif
    //}}}
  };
  //}}}
  //{{{  template <typename T> class cPtr
  // =======================================================================================
  #if QSWITCH_TRACKPTR
    #define INC_COUNTER() (myCounter ? myCounter->addRef(this, _ReturnAddress()) : 0)
    #define DEC_COUNTER() { if (myCounter && myCounter->removeRef(this, _ReturnAddress())) destroyObject(); }
    #define DEC_COUNTER_NODESTROY() { if (myCounter) myCounter->removeRef(this, _ReturnAddress()); }
  #else
    #define INC_COUNTER() (myCounter ? myCounter->addRef() : 0)
    #define DEC_COUNTER() { if (myCounter && myCounter->removeRef()) destroyObject(); }
    #define DEC_COUNTER_NODESTROY() { if (myCounter) myCounter->removeRef(); }
  #endif

  template <typename T> class cPtr
  {
  public:
    //{{{
    //Default construct a null smart-pointer.
    cPtr()
    : myPointer(0), myCounter(0)
    {
    // do nothing
    }
    //}}}
    //{{{
    /**
     * construct a smart-pointer from a cRefCount.
     *
     * it might not be obvious why this constructor is a member template.  consider:
     *
     * SomeInterface::Ptr ptr = new SomeClass();
     *
     * if SomeClass implements SomeInterface this is fine, but without the member template pointer's
     * type would be SomeInterface* which can't be assigned to cRefCount* without a dynamic-cast
     * so using the member template pointer can always be assigned to both cRefCount* and T*
     *
     * @param pointer the smart-pointable.
     *
     *                this only compiles if pointer's type is a Class, ie, extends cRefCount.  This will be true if Interfaces
     *                never have instance methods (and so can't pass 'this' with a static Interface type), which they
     *                don't, and Class raw pointers are never assigned to Interface raw pointers.  eg,
     *
     *                SomeClass          *cPtr1 = new SomeClass();  // fine, but use a smart-pointer if you can
     *                SomeClass::Ptr      cPrt2 = cPtr1;            // fine
     *                SomeInterface      *iPtr1 = cPtr1;            // works, but dangerous because you have an Interface raw pointer
     *                SomeInterface::Ptr  iPtr2 = iPtr1;            // won't compile because iPtr1 is an interface raw pointer
     *                SomeInterface::Ptr  iPtr3 = cPtr1;            // fine, cPtr1 is a statically typed Class not Interface
     */
    template <class B> cPtr(B * pointer)  //non const pointer
    : myPointer(pointer), myCounter(pointer)
    {
    #if defined(_DEBUG)
      assert(isPointerValid());
      assert(!isPointerToStack(myPointer)); // Object must _not_ be on the stack!
    #endif
    INC_COUNTER();
    }
    //}}}
    //{{{
    //This constructor is used exclusively by ptr_cast.
    cPtr(T * pointer, cRefCount *counter)
    : myPointer(pointer), myCounter(pointer ? counter : 0) // if the cast failed i don't want this smart-pointer to have the reference count
    {
    #if defined(_DEBUG)
      assert(isPointerValid());
      assert(!isPointerToStack(myPointer)); // Object must _not_ be on the stack!
    #endif
    INC_COUNTER();
    }
    //}}}
    //{{{
    //copy-construct a smart-pointer from another smart-pointer.
    template <class B> cPtr(const cPtr<B>& other)
    : myPointer(other.getRaw()), myCounter(other.getCounter())
    {
    #if defined(_DEBUG)
      assert(isPointerValid());
      assert(!isPointerToStack(myPointer)); // Object must _not_ be on the stack!
    #endif
    INC_COUNTER();
    }
    //}}}
    //{{{
    //self-copy-constructor.  I have to specialize this or the compiler will ignore it and use the bit-wise copy.
    cPtr(const cPtr& other)
    : myPointer(other.myPointer), myCounter(other.myCounter)
    {
    #if defined(_DEBUG)
      assert(isPointerValid());
      assert(!isPointerToStack(myPointer)); // Object must _not_ be on the stack!
    #endif
    INC_COUNTER();
    }
    //}}}
    //{{{
    inline ~cPtr()
    {
    DEC_COUNTER();
    }
    //}}}
    //{{{
    //Get the smart-pointable's reference counter.  Use this with care.
    cRefCount * QAPI getCounter() const
    {
    return myCounter;
    }
    //}}}
    //{{{
    //Get the raw pointer. This will be null if the smart-pointer is null.
    T * QAPI getRaw() const
    {
    #if defined(_DEBUG)
      assert(isPointerValid());
    #endif
    return myPointer;
    }
    //}}}
    //{{{
    // Safely assign this ptr from a raw ptr. Avoids double delete if raw object is in the middle
    // of destruction. Result in that case is a null ptr.
    cPtr<T> & QAPI fromRaw(T * newObject)
    {
    if (myPointer != newObject)
    {
      DEC_COUNTER();
      myPointer = newObject;
      myCounter = newObject;
      if (isPointerValid())
      {
      if (INC_COUNTER() == 0 /* count before inc */)
      {
        // Must have just inc'd count from zero - object was being destroyed (or brand new)!!!  Dec the
        // count and avoid double deleting. Return a null ptr to client.
        DEC_COUNTER_NODESTROY();
        myPointer = 0;
        myCounter = 0;
      }
      }
    }
    return *this; // null ptr if object is destroying itself or was newly created (e.g. fromRaw(new cMyClass))
    }
    //}}}
    //{{{
    //Dereference the smart-pointer. Asserts if the smart-pointer is null.
    inline T * QAPI operator->() const
    {
    #if defined(_DEBUG)
      assert(!isNull()); // Dereferenced uninitialised smart pointer?
      assert(isPointerValid());
    #endif
    return myPointer;
    }
    //}}}
    //{{{
    //Dereference the smart-pointer. throws if the smart-pointer is null.
    inline T& QAPI operator*() const
    {
    #if defined(_DEBUG)
      assert(!isNull()); // Dereferenced uninitialised smart pointer?
      assert(isPointerValid());
    #endif
    return *myPointer;
    }
    //}}}
    //{{{
    //True if the smart pointer is null.
    inline bool QAPI isNull() const
    {
    #if defined(_DEBUG)
      assert(isPointerValid());
    #endif
    return myPointer == 0;
    }
    //}}}
    //{{{
    //True if the smart-pointer is non-null.
    inline bool QAPI isValid() const
    {
    #if defined(_DEBUG)
      assert(isPointerValid());
    #endif
    return myPointer != 0;
    }
    //}}}
    //{{{
    //Null the smart-pointer.
    inline void QAPI invalidate()
    {
    DEC_COUNTER();
    myPointer = 0;
    myCounter = 0;
    }
    //}}}
    //{{{
    //Null the smart-pointer.
    inline void QAPI setNull()
    {
    invalidate();
    }
    //}}}
    //{{{
    cPtr<T>& QAPI operator=(const cPtr<T> & other)
    {
    if (this != &other)
    {
      //Interesting lifetime issue - for the duration of this function the 'other' will have a ref count of at least 1 -> thread safe.
      DEC_COUNTER();
      myPointer = other.myPointer;
      myCounter = other.myCounter;
      INC_COUNTER();
    }
    return *this;
    }
    //}}}
    //{{{
    //allows cPtr to be added to STL Trees. Makes no demands upon the pointed object.
    inline bool QAPI operator < (const cPtr<T> & other) const
    {
    return getRaw() < other.getRaw();
    }
    //}}}
    //{{{
    //the type of the smart-pointable.
    typedef T *Type;
    typedef T *NonConstType;
    //}}}

  protected :
    //{{{
    // This is seperate function which isn't usually inlined which allows decrementCounter and
    // incrementCounter to be smaller and more likely to be inlined.
    void QAPI destroyObject() const
    {
    delete myPointer;
    }
    //}}}
    //{{{
    bool QAPI isPointerValid() const
    {
    return checkPointer2(myPointer, sizeof(T));
    }
    //}}}
    //{{{  pointers
    mutable T *         myPointer;
    mutable cRefCount * myCounter;
    //}}}
  };
  //}}}
  //{{{
  // =======================================================================================
  template <typename T> class cConstPtr
  {
  public:
    //{{{
    //Default construct a null smart-pointer.
    cConstPtr()
    : myPointer(0), myCounter(0)
    {
    // do nothing
    }
    //}}}
    //{{{
    /**
     * construct a smart-pointer from a cRefCount.
     *
     * it might not be obvious why this constructor is a member template.  consider:
     *
     * SomeInterface::ConstPtr ptr = new SomeClass();
     *
     * if SomeClass implements SomeInterface this is fine, but without the member template pointer's
     * type would be SomeInterface* which can't be assigned to cRefCount* without a dynamic-cast
     * so using the member template pointer can always be assigned to both cRefCount* and T*
     *
     * @param pointer the smart-pointable.
     *
     *                this only compiles if pointer's type is a Class, ie, extends cRefCount.  This will be true if Interfaces
     *                never have instance methods (and so can't pass 'this' with a static Interface type), which they
     *                don't, and Class raw pointers are never assigned to Interface raw pointers.  eg,
     *
     *                SomeClass          *cPtr1 = new SomeClass();  // fine, but use a smart-pointer if you can
     *                SomeClass::Ptr      cPrt2 = cPtr1;            // fine
     *                SomeInterface      *iPtr1 = cPtr1;            // works, but dangerous because you have an Interface raw pointer
     *                SomeInterface::Ptr  iPtr2 = iPtr1;            // won't compile because iPtr1 is an interface raw pointer
     *                SomeInterface::Ptr  iPtr3 = cPtr1;            // fine, cPtr1 is a statically typed Class not Interface
     */
    template <class B> cConstPtr(const B * pointer) //const pointer
    : myPointer(pointer), myCounter(pointer)
    {
    #if defined(_DEBUG)
      assert(isPointerValid());
      assert(!isPointerToStack(myPointer)); // Object must _not_ be on the stack!
    #endif
    INC_COUNTER();
    }
    //}}}
    //{{{
    //This constructor is used exclusively by ptr_cast.
    cConstPtr(T * pointer, cRefCount *counter)
    : myPointer(pointer), myCounter(pointer ? counter : 0) // if the cast failed i don't want this smart-pointer to have the reference count
    {
    #if defined(_DEBUG)
      assert(isPointerValid());
      assert(!isPointerToStack(myPointer)); // Object must _not_ be on the stack!
    #endif
    INC_COUNTER();
    }
    //}}}
    //{{{
    //copy-construct a smart-pointer from another smart-pointer.
    template <class B>
    cConstPtr(const cConstPtr<B>& other)
    : myPointer(other.getRaw()), myCounter(other.getCounter())
    {
    #if defined(_DEBUG)
      assert(isPointerValid());
      assert(!isPointerToStack(myPointer)); // Object must _not_ be on the stack!
    #endif
    INC_COUNTER();
    }
    //}}}
    //{{{
    //self-copy-constructor.  I have to specialize this or the compiler will ignore it and use the bit-wise copy.
    cConstPtr(const cConstPtr& other)
    : myPointer(other.myPointer), myCounter(other.myCounter)
    {
    #if defined(_DEBUG)
      assert(isPointerValid());
      assert(!isPointerToStack(myPointer)); // Object must _not_ be on the stack!
    #endif
    INC_COUNTER();
    }
    //}}}
    //{{{
    //copy-construct a const-smart-pointer from another non-const-smart-pointer.
    template <class B>
    cConstPtr(const cPtr<B>& other)
    : myPointer(other.getRaw()), myCounter(other.getCounter())
    {
    INC_COUNTER();
    }
    //}}}
    //{{{
    inline ~cConstPtr()
    {
    DEC_COUNTER();
    }
    //}}}
    //{{{
    //Get the smart-pointable's reference counter.  Use this with care.
    const cRefCount * QAPI getCounter() const
    {
    return myCounter;
    }
    //}}}
    //{{{
    //Get the raw pointer. This will be null if the smart-pointer is null.
    const T * QAPI getRaw() const
    {
    #if defined(_DEBUG)
      assert(isPointerValid());
    #endif
    return myPointer;
    }
    //}}}
    //{{{
    // Safely assign this ptr from a raw ptr. Avoids double delete if raw object is in the middle
    // of destruction. Result in that case is a null ptr.
    cConstPtr<T> & QAPI fromRaw(const T * newObject)
    {
    if (myPointer != newObject)
    {
      DEC_COUNTER();
      myPointer = newObject;
      myCounter = newObject;
      if (isPointerValid())
      {
      if (INC_COUNTER() == 0 /* count before inc */)
      {
        // Must have just inc'd count from zero - object was being destroyed (or brand new)!!!  Dec the
        // count and avoid double deleting. Return a null ptr to client.
        DEC_COUNTER_NODESTROY();
        myPointer = 0;
        myCounter = 0;
      }
      }
    }
    return *this; // null ptr if object is destroying itself or was newly created (e.g. fromRaw(new cMyClass))
    }
    //}}}
    //{{{
    //Dereference the smart-pointer. Asserts if the smart-pointer is null.
    inline const T * QAPI operator->() const
    {
    #if defined(_DEBUG)
      assert(!isNull()); // Dereferenced uninitialised smart pointer
      assert(isPointerValid());
    #endif
    return myPointer;
    }
    //}}}
    //{{{
    //Dereference the smart-pointer. throws if the smart-pointer is null.
    inline const T & QAPI operator*() const
    {
    #if defined(_DEBUG)
      assert(!isNull()); // Dereferenced uninitialised smart pointer
      assert(isPointerValid());
    #endif
    return *myPointer;
    }
    //}}}
    //{{{
    //True if the smart pointer is null.
    inline bool QAPI isNull() const
    {
    #if defined(_DEBUG)
      assert(isPointerValid());
    #endif
    return myPointer == 0;
    }
    //}}}
    //{{{
    //True if the smart-pointer is non-null.
    inline bool QAPI isValid() const
    {
    #if defined(_DEBUG)
      assert(isPointerValid());
    #endif
    return myPointer != 0;
    }
    //}}}
    //{{{
    //Null the smart-pointer.
    inline void QAPI invalidate()
    {
    DEC_COUNTER();
    myPointer = 0;
    myCounter = 0;
    }
    //}}}
    //{{{
    //Null the smart-pointer.
    inline void QAPI setNull()
    {
    invalidate();
    }
    //}}}
    //{{{
    cConstPtr<T>& QAPI operator=(const cConstPtr<T> & other)
    {
    if (this != &other)
    {
      //Interesting lifetime issue - for the duration of this function the 'other' will have a ref count of at least 1 -> thread safe.
      DEC_COUNTER();
      myPointer = other.myPointer;
      myCounter = other.myCounter;
      INC_COUNTER();
    }
    return *this;
    }
    //}}}
    //{{{
    //allows cPtr to be added to STL Trees. Makes no demands upon the pointed object.
    inline bool QAPI operator < (const cConstPtr<T> & other) const
    {
    return getRaw() < other.getRaw();
    }
    //}}}
    //{{{
    // the type of the smart-pointable.
    typedef const T *Type;
    typedef T *NonConstType;
    //}}}

  protected:
    //{{{
    // This is seperate function which isn't usually inlined which allows decrementCounter and
    // incrementCounter to be smaller and more likely to be inlined.
    void QAPI destroyObject() const
    {
    delete const_cast<T*>(myPointer);
    }
    //}}}
    //{{{
    bool QAPI isPointerValid() const
    {
    return checkPointer2(myPointer, sizeof(T));
    }
    //}}}
    //{{{  pointers
    mutable const T *         myPointer;
    mutable const cRefCount * myCounter;
    //}}}
  };
  //}}}
  //{{{
  //equivalent to dynamic_cast.
  // FROM can be inferred, e.g. ptr_cast<a::ptr, b::ptr> == ptr_cast<a::ptr>
  template <class TO, class FROM> TO QAPI ptr_cast(const FROM & a)
  throw()
  {
    // the dynamic cast won't compile if A is const and B isn't
    return TO(const_cast<TO::NonConstType>(dynamic_cast<TO::Type>(a.getRaw())), const_cast<cRefCount*>(a.getCounter()));
  }

  template <class TO, class FROM> TO QAPI ptr_const_cast(const FROM & a)
  throw()
  {
    return TO(const_cast<TO::NonConstType>(const_cast<TO::NonConstType>(a.getRaw())), const_cast<cRefCount*>(a.getCounter()));
  }

  template <class TO, class FROM> TO QAPI ptr_static_cast(const FROM & a)
  throw()
  {
    return TO(const_cast<TO::NonConstType>(static_cast<TO::Type>(a.getRaw())), const_cast<cRefCount*>(a.getCounter()));
  }

  template <class A, class B> inline bool QAPI operator ==(const cPtr<A> & a, const B * const b)      { return (void *)a.getRaw() == (void *)b; }
  template <class A, class B> inline bool QAPI operator ==(const cConstPtr<A> & a, const B * const b) { return (void *)a.getRaw() == (void *)b; }
  template <class A, class B> inline bool QAPI operator ==(const cPtr<A> & a, const cPtr<B> & b)      { return (void *)a.getRaw() == (void *)b.getRaw(); }
  template <class A, class B> inline bool QAPI operator ==(const cPtr<A> & a, const cConstPtr<B> & b) { return (void *)a.getRaw() == (void *)b.getRaw(); }
  template <class A, class B> inline bool QAPI operator ==(const cConstPtr<A> & a, const cPtr<B> & b) { return (void *)a.getRaw() == (void *)b.getRaw(); }
  template <class A, class B> inline bool QAPI operator ==(const cConstPtr<A> & a, const cConstPtr<B> & b) { return (void *)a.getRaw() == (void *)b.getRaw(); }
  template <class A, class B> inline bool QAPI operator !=(const cPtr<A> & a, const B * const b)      { return (void *)a.getRaw() != (void *)b; }
  template <class A, class B> inline bool QAPI operator !=(const cConstPtr<A> & a, const B * const b) { return (void *)a.getRaw() != (void *)b; }
  template <class A, class B> inline bool QAPI operator !=(const cPtr<A> & a, const cPtr<B> & b)      { return (void *)a.getRaw() != (void *)b.getRaw(); }
  template <class A, class B> inline bool QAPI operator !=(const cPtr<A> & a, const cConstPtr<B> & b) { return (void *)a.getRaw() != (void *)b.getRaw(); }
  template <class A, class B> inline bool QAPI operator !=(const cConstPtr<A> & a, const cPtr<B> & b) { return (void *)a.getRaw() != (void *)b.getRaw(); }
  template <class A, class B> inline bool QAPI operator !=(const cConstPtr<A> & a, const cConstPtr<B> & b) { return (void *)a.getRaw() != (void *)b.getRaw(); }
  //}}}

  // Date & Time classes
  //{{{
  class cDate
  {
  public :
    cDate();
    cDate (const cDate& other);
    cDate (const unsigned int Year // 0 - USHRT_MAX
    , const unsigned int Month // 1 - 12
    , const unsigned int Day); // 1 - 365

    ~cDate();

    unsigned int QAPI getYear() const;
    unsigned int QAPI getMonth() const;
    unsigned int QAPI getDay() const;

    vfs::String QAPI serialise() const;

    vfs::String QAPI toString() const;

    cDate& QAPI operator= (const cDate& other);

    bool QAPI operator== (const cDate& other) const;
    bool QAPI operator!= (const cDate& other) const;

    bool QAPI operator< (const cDate& other) const;
    bool QAPI operator> (const cDate& other) const;

  private :
    unsigned short m_Year;
    unsigned char m_Month;
    unsigned char m_Day;
  };
  //}}}
  //{{{
  class cTime
  {
  public :
    cTime();
    cTime (const cTime& other);
    cTime (const int Hour // 0 - UINT_MAX
    , const int Minute // 0 - 59
    , const int Second // 0 - 59
    , const int Millisec); // 0 - 999

    ~cTime();

    int QAPI getHour() const;
    int QAPI getMinute() const;
    int QAPI getSecond() const;
    int QAPI getMillisec() const;

    __int64 QAPI getDurationInMinutes() const;
    void QAPI setDurationFromMinutes (const __int64 NumMinutes);

    __int64 QAPI getDurationInSeconds() const;
    void QAPI setDurationFromSeconds (const __int64 NumSeconds);

    __int64 QAPI getDurationInMillisecs() const;
    void QAPI setDurationFromMillisecs (const __int64 NumMillisecs);

    String QAPI serialise() const;
    String QAPI toString() const;

    cTime&  QAPI operator= (const cTime& other);

    bool QAPI operator== (const cTime& other) const;
    bool QAPI operator!= (const cTime& other) const;

    bool QAPI operator< (const cTime& other) const;
    bool QAPI operator> (const cTime& other) const;

    cTime QAPI operator- (const cTime& other) const;
    void QAPI operator-= (const cTime& other);

  private:
    long  m_Hour;
    char  m_Minute;
    char  m_Second;
    short m_Millisec;
  };
  //}}}
  //{{{
  class cDateTime
  {
  public:
    cDateTime();
    cDateTime (const vfs::cDate& date, const vfs::cTime& time);
    cDateTime (const cDateTime& other);
    cDateTime (const unsigned int year, const unsigned int month, const unsigned int day
    , const int hour, const int minute, const int second, const int millisec);

    vfs::cTime time() const;
    vfs::cDate date() const;

    unsigned int QAPI year() const;
    unsigned int QAPI month() const;
    unsigned int QAPI day() const;

    int QAPI hour() const;
    int QAPI minute() const;
    int QAPI second() const;
    int QAPI millisec() const;

    vfs::String QAPI serialise () const;
    vfs::String QAPI toString () const;

    bool QAPI operator!= (const cDateTime& other) const;
    bool QAPI operator== (const cDateTime& other) const;
    bool QAPI operator< (const cDateTime& other) const;
    bool QAPI operator> (const cDateTime& other) const;

  private:
    vfs::cDate Date;
    vfs::cTime Time;
  };
  //}}}

  //{{{  Rational
  //{{{
  // =======================================================================================
  template <typename T> class cRational
  {
  public :
    cRational() {}
    cRational(const T Numerator, const T Denominator) : m_N(Numerator), m_D(Denominator) { _fixSigns(); _reduce(); }
    explicit cRational(const T Value) : m_N(Value), m_D(1) {}
    explicit cRational(const String& Serialised) throw(cBadFormat) { _fromString(Serialised.c_str()); }
    explicit cRational(const Char* const Serialised) throw(cBadFormat) { _fromString(Serialised); }
    //{{{
    explicit cRational(const double Value, const T DenominatorToUse, double * const Remainder) :
      m_N((T)(Value * (double)DenominatorToUse)),
      m_D(DenominatorToUse)
    {
      *Remainder = Value - (double)(m_N / m_D);
      _reduce();
    }
    //}}}
    //{{{
    explicit cRational(const double Value, const unsigned int PreserveNumBitsPrecision, bool * const IsPrecise)
    {
      double x(Value);
      m_N = 0;
      m_D = 1;
      if (x != 0.0)
      {
      const int sign = x < 0;
      if (sign) x = -x;

      int tmp;
      double Mantissa = frexp(x, &tmp);
      T Exponent = tmp;

      unsigned int NumBitsPrecisionToDo = PreserveNumBitsPrecision;
      while (Mantissa != 0.0 && NumBitsPrecisionToDo > 0)
      {
        const unsigned int MaxNumBitsToDoPerStep = 15;
        const unsigned int Shift = Min<unsigned int>(MaxNumBitsToDoPerStep, NumBitsPrecisionToDo);
        const double       Width = (double)(1 << Shift);  // = 2^shift
        Mantissa *= Width;
        double intpart;
        Mantissa = modf(Mantissa, &intpart);
        m_N <<= Shift;
        m_N += (T)intpart;
        Exponent -= Shift;
        NumBitsPrecisionToDo -= Shift;
      }

      *IsPrecise = (Mantissa == 0.0);

      if (Exponent > 0)      m_N <<= Exponent;
      else if (Exponent < 0) m_D <<= -Exponent;

      if (sign) m_N = -m_N;
      }
      _fixSigns();
      _reduce();
    }
    //}}}

    // Copy construction
    cRational(const cRational<T> & other) : m_N(other.m_N), m_D(other.m_D) {}

    // Conversion
    operator double() const         { return (double)m_N / (double)m_D; }
    operator T() const              { return m_N / m_D; }

    // Assignment
    void QAPI operator=(const cRational<T> & other) { m_N= other.m_N; m_D = other.m_D; }

    // Comparison
    //{{{
    friend bool QAPI operator==(const cRational<T> & x, const cRational<T> & y)
    {
      if (x.m_N != y.m_N || x.m_D != y.m_D)
      {
      const_cast<cRational<T> &>(x)._reduce();
      if (x.m_N != y.m_N || x.m_D != y.m_D)
      {
        const_cast<cRational<T> &>(y)._reduce();
        if (x.m_N != y.m_N || x.m_D != y.m_D)
        {
        return false;
        }
      }
      }
      return true;
    }
    //}}}
    friend bool QAPI operator!=(const cRational<T> & x, const cRational<T> & y) { return !(x == y); }
    friend bool QAPI operator< (const cRational<T> & x, const cRational<T> & y) { return cRational<T>(x - y).m_N < 0; }
    friend bool QAPI operator<=(const cRational<T> & x, const cRational<T> & y) { return x == y || x < y; }
    friend bool QAPI operator> (const cRational<T> & x, const cRational<T> & y) { return cRational<T>(x - y).m_N > 0; }
    friend bool QAPI operator>=(const cRational<T> & x, const cRational<T> & y) { return x == y || x > y; }

    // Unary
    cRational<T> QAPI operator-() const { return cRational<T>(-m_N, m_D); }

    // Arithmetic
    friend cRational<T> QAPI operator+(const cRational<T> & x, const cRational<T> & y) { cRational<T> tmp((x.m_N * y.m_D) + (y.m_N * x.m_D), x.m_D * y.m_D); tmp._reduce(); return tmp; }
    friend cRational<T> QAPI operator-(const cRational<T> & x, const cRational<T> & y) { cRational<T> tmp((x.m_N * y.m_D) - (y.m_N * x.m_D), x.m_D * y.m_D); tmp._reduce(); return tmp; }
    friend cRational<T> QAPI operator*(const cRational<T> & x, const cRational<T> & y) { cRational<T> tmp(x.m_N * y.m_N, x.m_D * y.m_D); tmp._reduce(); return tmp; }
    friend cRational<T> QAPI operator/(const cRational<T> & x, const cRational<T> & y) { cRational<T> tmp(x.m_N * y.m_D, x.m_D * y.m_N); tmp._reduce(); return tmp; }
    cRational<T> & QAPI operator+=(const cRational<T> & x) { *this = (*this) + x; return *this; }
    cRational<T> & QAPI operator-=(const cRational<T> & x) { *this = (*this) - x; return *this; }
    cRational<T> & QAPI operator*=(const cRational<T> & x) { *this = (*this) * x; return *this; }
    cRational<T> & QAPI operator/=(const cRational<T> & x) { *this = (*this) / x; return *this; }

    // Serialisation
    //{{{
    String QAPI serialise() const
    {
      StringStream tmp;
      tmp << std::setiosflags(std::ios::fixed) << std::setprecision(1) << (double)(Int64)((double)m_N / (double)m_D * 10.0L) / 10.0L << L"(" << m_N << L"/" << m_D << L")";
      return tmp.str();
    }
    //}}}

    // Misc
    cRational<T> QAPI abs() const { return cRational<T>(m_N < 0 ? -m_N : m_N, m_D); }
    friend cRational<T> QAPI mod(const cRational<T> & x, const cRational<T> & y) { const T tmp((T)(x / y)); return x - Rational64(tmp) * y; }
    bool isInteger() const { return m_D == (T)1; }
    bool isNegative() const { return m_N < 0; }
    bool isNonNegative() const { return m_N >= 0; }
    bool isPositive() const { return m_N > 0; }
    //{{{

    friend size_t calcNumIntermediateBitsRequired(const cRational<T> & x, const cRational<T> & y)
    {
      // Work out number of bits needed to calculate "x + y"
      const cRational<T> xAbs(x.abs());
      const cRational<T> yAbs(y.abs());
      const size_t xN = _calcNumBitsUsed(xAbs.m_N);
      const size_t xD = _calcNumBitsUsed(xAbs.m_D);
      const size_t yN = _calcNumBitsUsed(yAbs.m_N);
      const size_t yD = _calcNumBitsUsed(yAbs.m_D);
      const size_t tmp0 = xN + yD;
      const size_t tmp1 = yN + xD;
      const size_t tmp2 = xD + yD;
      return Max<size_t>(tmp2, Max<size_t>(tmp0, tmp1)) + (1 /* to accomodate addition/subtraction */);
    }
    //}}}

    cRational<T> QAPI floor() const { return cRational<T>(m_N >= 0 ? (m_N / m_D) : (m_N - (m_D - 1)) / m_D); }
    cRational<T> QAPI floor(const Int64 ToNearest) const { return (*this * Rational64((Int64)ToNearest)).floor() / Rational64((Int64)ToNearest); }
    cRational<T> QAPI ceil() const { return cRational<T>(m_N >= 0 ? (m_N + (m_D - 1)) / m_D : (m_N / m_D)); }
    cRational<T> QAPI ceil(const Int64 ToNearest) const { return (*this * Rational64((Int64)ToNearest)).ceil() / Rational64((Int64)ToNearest); }
    cRational<T> QAPI sign() const /* 1 = pos, -1 = neg */ { return m_N < 0 ? cRational<T>(-1, 1) : cRational<T>(1, 1); }

    T QAPI getNumerator() const { return m_N; }
    T QAPI getDenominator() const { return m_D; }

  private :
    T m_N;
    T m_D;

    //{{{
    T _gcd(const T x, const T y) const
    {
      const T AbsX(x < 0 ? -x : x);
      const T AbsY(y < 0 ? -y : y);
      return AbsX > AbsY ? _gcd_helper(AbsX, AbsY) : _gcd_helper(AbsY, AbsX);
    }
    //}}}
    //{{{
    T _gcd_helper(const T x, const T y) const
    {
      return y == 0 ? x : _gcd_helper(y, x%y);
    }
    //}}}

    //{{{
    void _reduce()
    {
      assert(m_D >= 0); // Should have called _fixSigns() first
      const T GreatestCommonDenominator = _gcd(m_N, m_D);
      m_N /= GreatestCommonDenominator;
      m_D /= GreatestCommonDenominator;
    }
    //}}}
    //{{{
    void _fixSigns()
    {
      if (m_D < 0) { m_N = -m_N; m_D = -m_D; }
    }
    //}}}
    //{{{
    void _fromString(const Char* const Serialised) throw(cBadFormat)
    {
      String CopyOf(Serialised); // Need to copy because swscanf will temporarily modify it!
      double Unused;
      const unsigned int NumFieldsConverted = swscanf(CopyOf.c_str(), L"%lf(%I64d/%I64d)", &Unused, &m_N, &m_D);
      if (NumFieldsConverted < 3)
        throw cBadFormat(__FILE__, __LINE__, L"Tried to construct a cRational from string \"%s\"", CopyOf.c_str());
    }
    //}}}

    //{{{
    static size_t _calcNumBitsUsed(const T Value)
    {
      const size_t MaxNumBits = sizeof(T) * 8;
      const size_t MaxNumBitsExcSign = MaxNumBits - 1;
      size_t i = MaxNumBitsExcSign;
      T BitMask = (T)1 << (i - 1);
      do
      {
      if ((Value & BitMask) != 0) return i;
      BitMask = BitMask >> 1;
      i--;
      } while (BitMask != 0);
      return 0;
    }
    //}}}
  };
  //}}}

  typedef cRational<Int32> Rational32;
  typedef cRational<Int64> Rational64;

  //{{{
  inline Rational64 to64(const Rational32& a)
  {
    return Rational64((Int64)a.getNumerator(), (Int64)a.getDenominator());
  }
  //}}}
  //{{{
  inline Rational32 to32(const Rational64& a) {
    return Rational32((Int32)a.getNumerator(), (Int32)a.getDenominator());
  }
  //}}}
  //}}}

  // Multi-Thread support
  //{{{
  // =======================================================================================
  class iWaitable
  {
  public :
    QDEFINE_SMARTPTR(iWaitable);
    virtual ~iWaitable() {}

    enum eWakeupReason  // deprecated, use eWaitResult
    {
      eSignalled = 0, // deprecated, use eWaitResult_Set
      eWaitResult_Set = 0,
      eTimedOut = 1, // deprecated, use eWaitResult_TimedOut
      eWaitResult_TimedOut = 1
    };
    typedef eWakeupReason eWaitResult;

    virtual eWaitResult QAPI wait(const unsigned int timeoutMs) const = 0;
    virtual void QAPI waitForever() const = 0;
    virtual void QAPI set() = 0;
    virtual void QAPI reset() = 0;

    virtual String QAPI getName() const throw(cNotFound) = 0;

  #if defined(_WIN32)
    #if defined(_WINDOWS_) // is windows header included?
    virtual HANDLE QAPI getWin32EventHandle() const = 0;
    #else
    virtual void * QAPI getWin32EventHandle() const = 0;
    #endif

  #endif
  };
  //}}}
  //{{{
  // =======================================================================================
  class cManualResetEvent : public iWaitable, public cRefCount
  {
  public :
    QDEFINE_SMARTPTR(cManualResetEvent);

    // iWaitable
    virtual eWaitResult QAPI wait(const unsigned int timeoutMs) const;
    virtual void QAPI waitForever() const;
    virtual void QAPI set();
    virtual void QAPI reset();
    virtual String QAPI getName() const throw(cNotFound);

  #if defined(_WIN32)
    #if defined(_WINDOWS_) // is windows header included?
    virtual HANDLE QAPI getWin32EventHandle() const;
    #else
    virtual void * QAPI getWin32EventHandle() const;
    #endif
  #endif

    // cManualResetEvent
    cManualResetEvent(const bool IsInitiallySet) throw(cOSError);
    cManualResetEvent(const bool IsInitiallySet, const String& OSGlobalName) throw(cOSError);
    virtual ~cManualResetEvent();

  private :
    // Not permitted to do these!
    cManualResetEvent(const cManualResetEvent&);
    cManualResetEvent& operator=(const cManualResetEvent&);

  #if defined(_WIN32)
    #if defined(_WINDOWS_) // is windows header included?
    HANDLE m_hEvent;
    #else
    int m_hEvent;
    #endif
  #endif

    const String m_Name;
  };
  //}}}
  //{{{
  // =======================================================================================
  class cAutoResetEvent : public iWaitable, public cRefCount
  {
  public :
    QDEFINE_SMARTPTR(cAutoResetEvent);

    // iWaitable
    virtual eWakeupReason QAPI wait(const unsigned int timeoutMs) const;
    virtual void QAPI waitForever() const;
    virtual void QAPI set();
    virtual void QAPI reset();

    virtual String QAPI getName() const throw(cNotFound);

  #if defined(_WIN32)
    #if defined(_WINDOWS_) // is windows header included?
    virtual HANDLE QAPI getWin32EventHandle() const;
    #else
    virtual void * QAPI getWin32EventHandle() const;
    #endif
  #endif

    // cAutoResetEvent
    cAutoResetEvent(const bool IsInitiallySet) throw(cOSError);
    cAutoResetEvent(const bool IsInitiallySet, const String& OSGlobalName) throw(cOSError);
    virtual ~cAutoResetEvent();

  private :
    // Not permitted to do these!
    cAutoResetEvent(const cAutoResetEvent&);
    cAutoResetEvent& operator=(const cAutoResetEvent&);

  #if defined(_WIN32)
    #if defined(_WINDOWS_) // is windows header included?
    HANDLE m_hEvent;
    #else
    int m_hEvent;
    #endif
  #endif

    const String m_Name;
  };
  //}}}
  //{{{  class cSemaphore
  // =======================================================================================
  const unsigned int kMaxSemaphoreThreadsPermitted = INT_MAX;

  class cSemaphore : public iWaitable, public cRefCount
  {
  public:
    QDEFINE_SMARTPTR(cSemaphore);
    // iWaitable
    // Wait a maximum period of time for the semaphore signaled count > 0 then decrements the count.
    virtual eWaitResult QAPI wait(const unsigned int timeoutMs) const;

    // Wait forever for the semaphore signaled count to be > 0 then decrements the count.
    virtual void QAPI waitForever() const;

    // Increment number of threads permitted by 1. Does not increment if count is already at maximum.
    // Wakes up any threads waiting for this latch.
    virtual void QAPI set();
    virtual void QAPI reset();
    virtual String QAPI getName() const throw(cNotFound);

  #if defined(_WIN32)
    #if defined(_WINDOWS_) // is windows header included?
    virtual HANDLE QAPI getWin32EventHandle() const;
    #else
    virtual void * QAPI getWin32EventHandle() const;
    #endif
  #endif

    // cSemaphore
    cSemaphore() throw(cOSError);
    cSemaphore(const unsigned int InitialNumThreadsPermitted, const unsigned int MaxNumThreadsPermitted /* max kMaxSemaphoreThreadsPermitted */);
    cSemaphore(const unsigned int InitialNumThreadsPermitted, const unsigned int MaxNumThreadsPermitted /* max kMaxSemaphoreThreadsPermitted */, const String& OSGlobalName);
    virtual ~cSemaphore();

    void QAPI wait() const { waitForever(); } // deprecated, use waitForever()
    void QAPI permitNextThread() { set(); } // deprecated, use set()
    void QAPI signal() { set(); } // deprecated, use set()

  private:
    // Not permitted to do these!
    cSemaphore(const cSemaphore&);
    cSemaphore& operator=(const cSemaphore&);

  #if defined(_WIN32)
    #if defined(_WINDOWS_) // is windows header included?
    HANDLE m_hSemaphore;
    #else
    int m_hSemaphore;
    #endif
  #endif

    const String m_Name;
    const unsigned int m_InitialNumThreadsPermitted;
    const unsigned int m_MaxNumThreadsPermitted;
  };

  //}}}
  //{{{
  class cSemaphoreGuard : public cRefCount
  {
  public :
    QDEFINE_SMARTPTR(cSemaphoreGuard);
    cSemaphoreGuard(cSemaphore * const Semaphore);
    cSemaphoreGuard(const cSemaphoreGuard& other);
    ~cSemaphoreGuard();

    void QAPI operator=(const cSemaphoreGuard& other);

  private :
    cSemaphore* m_Semaphore;
  };
  //}}}
  //{{{  class cLock
  //{{{  description
  // =======================================================================================
  /*
  Used to lock access to resources or data. i.e. mutual exclusion. lock() gains access to
  the resource, unlock() releases access.

  Recursion safe.

  e.g.
    a->lock();
    a->lock();
    a->unlock();
    a->unlock();

  Prefer using cLockGuard to keep code exception-safe.
  */
  //}}}
  const unsigned int kLockMaxSpinCount = 100000;

  class cLock : public cRefCount
  {
  public:
    cLock();
    cLock(const unsigned int spinCount /* >0 and <=kLockMaxSpinCount */);
    virtual ~cLock();

    // Blocks if already locked.
    void QAPI lock();
    void QAPI unlock();

    // Non blocking - true owns the lock - false failed to get lock.
    bool QAPI tryLock();

  private :
    // Not permitted!
    cLock(const cLock& other) {}
    void QAPI operator=(const cLock& other) {}

    CRITICAL_SECTION mSection;
  };
  //}}}
  // Exception safe usage of cLock, cLockGuard guard(&lock);
  //{{{
  // =======================================================================================
  class cWaitMultipleEvents : public cRefCount
  {
  public :
    QDEFINE_SMARTPTR(cWaitMultipleEvents);
    cWaitMultipleEvents(const std::vector<String>& EventIds) throw(cNotFound, cOSError);
    ~cWaitMultipleEvents();

    iWaitable::eWaitResult QAPI wait(const unsigned int timeoutMs, String * const EventId) const;
    String QAPI waitForever() const;

  private :
    std::vector<String> m_EventIds;
    std::vector<void *> m_Handles;
  };
  //}}}
  //{{{
  class cLockGuard : public cRefCount
  {
  public :
    QDEFINE_SMARTPTR(cLockGuard);
    cLockGuard(cLock * const Lock);
    cLockGuard(const cLockGuard& other);
    ~cLockGuard();

    void QAPI operator=(const cLockGuard& other);

  private :
    cLock * m_Lock;
  };
  //}}}

  //{{{
  class iCowPointable
  {
  public :
    virtual ~iCowPointable() {}
    virtual iCowPointable * clone() const = 0;
    cLock m_CloneLock;
  };
  //}}}
  //{{{
  template <typename T> class cCowPtr : public cPtr<iCowPointable>
  {
  public:
    // Default construct a null smart-pointer.
    cCowPtr() : cPtr<iCowPointable>() {}

    template<class B> cCowPtr(B * const pointer) : cPtr<iCowPointable>(pointer) {}

    // Copy-construct a smart-pointer from another smart-pointer.
    template<class B> cCowPtr(const cCowPtr<B> & other) : cPtr<iCowPointable>(other) {}

    // Self-copy-constructor.  I have to specialize this or the compiler will ignore it and use the bit-wise copy.
    cCowPtr(const cCowPtr & other) : cPtr<iCowPointable>(other) {}

    // Destructor
    ~cCowPtr() {}

    // Assignment
    //{{{
    cCowPtr<T> & QAPI operator=(const cCowPtr<T> & other)
    {
      // Don't copy m_MemberLock pointer - just do default cPtr method.
      cPtr<iCowPointable>::operator=(other);
      return *this;
    }
    //}}}

    //{{{
    inline const T & QAPI read() const
    {
      #if defined(_DEBUG)
        assert(!isNull()); // Dereferenced uninitialised smart copy-on-write pointer?
        assert(isPointerValid());
      #endif

      // Release build - this should be atomic.
      cLockGuard MemberGuard(&myPointer->m_CloneLock);
      return *static_cast<const T *>(myPointer);
    }
    //}}}
    //{{{
    inline T & QAPI write() const
    {
      cLockGuard MemberGuard(&myPointer->m_CloneLock);

      #if defined(_DEBUG)
        assert(!isNull()); // Dereferenced uninitialised smart copy-on-write pointer?
        assert(isPointerValid());
      #endif

      if (getCounter()->getNumRefs() > 1)
      {
        T * const NewObject = static_cast<T *>(myPointer->clone());
        DEC_COUNTER();
        myPointer = NewObject;
        myCounter = NewObject;
        INC_COUNTER();
      }
      return *static_cast<T *>(myPointer);
    }
    //}}}

  private :
    // Not permitted!
    inline T * QAPI operator->() const { return myPointer; }
    inline T & QAPI operator*() const { return *myPointer; }
  };
  //}}}
  //{{{
  // =======================================================================================
  //{{{  description
  /*
  Used to synchronise to specific events, e.g. field/frame interrupt. This lets all threads
  through on the sync() call. Any thread that comes after this call will wait until the next
  sync() call.
  */
  //}}}
  class cPulse : public iWaitable, public cRefCount
  {
  public :
    QDEFINE_SMARTPTR(cPulse);

    // iWaitable
    virtual eWaitResult QAPI wait(const unsigned int timeoutMs) const;
    virtual void QAPI waitForever() const;
    virtual void QAPI set();
    virtual void QAPI reset();
    virtual String QAPI getName() const throw(cNotFound);

  #if defined(_WIN32)
    #if defined(_WINDOWS_) // is windows header included?
    virtual HANDLE QAPI getWin32EventHandle() const;
    #else
    virtual void * QAPI getWin32EventHandle() const;
    #endif
  #endif

    // cPulse
    cPulse() throw(cOSError);
    cPulse(const String& OSGlobalName) throw(cOSError);
    void QAPI pulse() { set(); } // deprecated, use set()
    virtual ~cPulse();

  private :
    // Not permitted to do these!
    cPulse(const cPulse&);
    cPulse& operator=(const cPulse&);

  #if defined(_WIN32)
    #if defined(_WINDOWS_) // is windows header included?
    HANDLE m_hEvent;
    #else
    int m_hEvent;
    #endif
  #endif

    const String m_Name;
  };
  //}}}

  //{{{
  // =======================================================================================
  class cVersion
  {
  public :
    cVersion();
    cVersion(const Char* const Serialised) throw(cBadFormat);
    cVersion(const String& Serialised) throw(cBadFormat);
    cVersion(
      const unsigned int Release,
      const unsigned int MajorVersion,
      const unsigned int MinorVersion,
      const unsigned int Build);

    bool    QAPI operator!=(const cVersion& other) const;
    bool    QAPI operator==(const cVersion& other) const;
    bool    QAPI operator< (const cVersion& other) const;
    bool    QAPI operator> (const cVersion& other) const;
    String  QAPI serialise() const;
    String  QAPI toString() const;

    unsigned int m_Release;
    unsigned int m_MajorVersion;
    unsigned int m_MinorVersion;
    unsigned int m_Build;
  };
  //}}}
  //{{{
  //{{{  description
  // =======================================================================================
  /*
  Globally unique identifier. A vfs Kernel cGuid is based on the Microsoft GUID
  structure. They uniquely identify objects such as interfaces. A cGuid is a 128-bit value
  consisting of one group of eight hexadecimal digits, followed by three groups of four
  hexadecimal digits each, followed by one group of twelve hexadecimal digits. The
  following example shows the groupings of hexadecimal digits in a GUID.

    {
    const cGuid MyGuid1(
      0xdddcfda9, 0xaf0b, 0x11d3,
      0x9e, 0x36, 0x0, 0x90, 0x27, 0xa6, 0xc8, 0xa4);

    const cGuid MyGuid2(
      "{dddcfda9-af0b-11d3-9e36-009027a6c8a4}");
    }

  To create a new cGuid use Microsoft's "GuidGen" utility that ships with their Visual
  Studio environment. You must use this to generate a cGuid - DO NOT just copy an
  exisiting one.
  */
  //}}}
  class cGuid
  {
    friend class cGuid2;

  public :
    //{{{
    cGuid()
    {
      m_Data64[0] = 0;
      m_Data64[1] = 0;
    }
    //}}}
    //{{{
    cGuid(
      const UInt32    Data1,
      const UInt16    Data2,
      const UInt16    Data3,
      const UInt8     Data50,
      const UInt8     Data51,
      const UInt8     Data52,
      const UInt8     Data53,
      const UInt8     Data54,
      const UInt8     Data55,
      const UInt8     Data56,
      const UInt8     Data57);
    //}}}
    //{{{
    cGuid(
      const UInt32    Data1,
      const UInt16    Data2,
      const UInt16    Data3,
      const UInt8     Data50,
      const UInt8     Data51,
      const UInt8     Data52,
      const UInt8     Data53,
      const UInt8     Data54,
      const UInt8     Data55,
      const UInt8     Data56,
      const UInt8     Data57,
      const String& Comment);
    //}}}
    cGuid(const UInt64 Data0, const UInt64 Data1);
    cGuid(const UInt64 Data0, const UInt64 Data1, const String& Comment);

    explicit cGuid(const String& Serialised) throw(cBadFormat);
    explicit cGuid(const Char* const Serialised) throw(cBadFormat);
    //{{{
    cGuid(const cGuid& other) :
      m_Comment(other.m_Comment)
    {
      m_Data64[0] = other.m_Data64[0];
      m_Data64[1] = other.m_Data64[1];
    }
    //}}}

  #if defined(_WIN32)
    #if defined(_WINDOWS_) // is windows header included?
    cGuid(const GUID& other);
    cGuid(const GUID& other, const String& Comment);
    #endif
  #endif

    //{{{
    void operator=(const cGuid& other)
    {
      m_Data64[0] = other.m_Data64[0];
      m_Data64[1] = other.m_Data64[1];
      m_Comment = other.m_Comment;
    }
    //}}}
    //{{{
    bool QAPI operator<(const cGuid& other) const
    {
      if (m_Data64[0] == other.m_Data64[0])
      {
      return m_Data64[1] < other.m_Data64[1];
      }
      return m_Data64[0] < other.m_Data64[0];
    }
    //}}}
    //{{{
    bool QAPI operator>(const cGuid& other) const
    {
      if (m_Data64[0] == other.m_Data64[0])
      {
      return m_Data64[1] > other.m_Data64[1];
      }
      return m_Data64[0] > other.m_Data64[0];
    }
    //}}}
    //{{{
    bool QAPI operator==(const cGuid& other) const
    {
      return (m_Data64[0] == other.m_Data64[0]) && (m_Data64[1] == other.m_Data64[1]);
    }
    //}}}
    //{{{
    bool QAPI operator!=(const cGuid& other) const
    {
      return !(*this == other);
    }
    //}}}

    String QAPI serialise(const bool IncludeComment = true) const;
    String QAPI toString(const bool IncludeComment = true) const;
    UInt32 QAPI getData1() const;
    UInt16 QAPI getData2() const;
    UInt16 QAPI getData3() const;
    void   QAPI getData4(unsigned char* const data4_0, unsigned char* const data4_1,
                         unsigned char* const data4_2, unsigned char* const data4_3,
                         unsigned char* const data4_4, unsigned char* const data4_5,
                         unsigned char* const data4_6, unsigned char* const data4_7) const;
    UInt8  QAPI getData4_0() const;
    UInt8  QAPI getData4_1() const;
    UInt8  QAPI getData4_2() const;
    UInt8  QAPI getData4_3() const;
    UInt8  QAPI getData4_4() const;
    UInt8  QAPI getData4_5() const;
    UInt8  QAPI getData4_6() const;
    UInt8  QAPI getData4_7() const;
    UInt64 QAPI getData64_0() const;
    UInt64 QAPI getData64_1() const;
    void QAPI getData64(UInt64 * const data64_0, UInt64 * const data64_1) const;
    String QAPI getComment() const { return m_Comment; }

    static cGuid QAPI createUnique (const String& Comment) throw(cBadFormat);

  private :
    #pragma warning(push)
    #pragma warning(disable: 4201)  // Ignore "nonstandard extension used : nameless struct/union"
    union
    {
      struct
      {
        mutable UInt32  m_Data1;
        mutable UInt16  m_Data2;
        mutable UInt16  m_Data3;
        mutable UInt8   m_Data4[8];
      };
      struct
      {
        mutable UInt64 m_Data64[2];
      };
    };
    #pragma warning(pop)

    mutable bool m_NotUsed;
    mutable String m_Comment;

    void _deserialise(const Char* const Src);
  };
  //}}}
  //{{{
  // ---------------------------------------------------------------------------------------
  class cGuid2
  // faster version!
  {
  public:
    // ctors
    //{{{
    cGuid2()
    {
      m_Data64[0] = 0;
      m_Data64[1] = 0;
      m_SmallComment[0] = L'\0';
    }
    //}}}
    explicit cGuid2(const UInt32 Data1, const UInt16 Data2, const UInt16 Data3, const UInt8 Data50,
                    const UInt8 Data51, const UInt8 Data52, const UInt8 Data53, const UInt8 Data54,
                    const UInt8 Data55, const UInt8 Data56, const UInt8 Data57);
    explicit cGuid2(const UInt32 Data1, const UInt16 Data2, const UInt16 Data3, const UInt8 Data50,
                    const UInt8 Data51, const UInt8 Data52, const UInt8 Data53, const UInt8 Data54,
                    const UInt8 Data55, const UInt8 Data56, const UInt8 Data57, const Char* const comment);
    explicit cGuid2(const UInt32 Data1, const UInt16 Data2, const UInt16 Data3, const UInt8 Data50,
                    const UInt8 Data51, const UInt8 Data52, const UInt8 Data53, const UInt8 Data54,
                    const UInt8 Data55, const UInt8 Data56, const UInt8 Data57, const String& comment);
    explicit cGuid2(const UInt64 Data0, const UInt64 Data1);
    explicit cGuid2(const UInt64 Data0, const UInt64 Data1, const Char* const comment);
    explicit cGuid2(const UInt64 Data0, const UInt64 Data1, const String& comment);
    explicit cGuid2(const Char* const serialised) throw(cBadFormat);
    explicit cGuid2(const String& serialised) throw(cBadFormat);
    cGuid2(const cGuid& other) { *this = other; }
    cGuid2(const cGuid2& other) { *this = other; }

  #if defined(_WIN32)
    #if defined(_WINDOWS_) // is windows header included?
    explicit cGuid2(const GUID& other);
    explicit cGuid2(const GUID& other, const String& comment);
    #endif
  #endif

    // casting
    QAPI operator cGuid() const;

    // assigment
    void operator=(const cGuid& other);
    void operator=(const cGuid2& other);

    // comparison
    //{{{
    bool QAPI operator<(const cGuid2& other) const
    {
      if (m_Data64[0] == other.m_Data64[0])
      {
      return m_Data64[1] < other.m_Data64[1];
      }
      return m_Data64[0] < other.m_Data64[0];
    }
    //}}}
    //{{{
    bool QAPI operator>(const cGuid2& other) const
    {
      if (m_Data64[0] == other.m_Data64[0])
      {
      return m_Data64[1] > other.m_Data64[1];
      }
      return m_Data64[0] > other.m_Data64[0];
    }
    //}}}
    bool QAPI operator==(const cGuid& other) const { return (m_Data64[0] == other.m_Data64[0]) && (m_Data64[1] == other.m_Data64[1]); }
    bool QAPI operator==(const cGuid2& other) const { return (m_Data64[0] == other.m_Data64[0]) && (m_Data64[1] == other.m_Data64[1]); }
    bool QAPI operator!=(const cGuid2& other) const { return !(*this == other); }

    // gets
    UInt32 QAPI getData1() const;
    UInt16 QAPI getData2() const;
    UInt16 QAPI getData3() const;
    void   QAPI getData4(unsigned char* const data4_0, unsigned char* const data4_1,
                         unsigned char* const data4_2, unsigned char* const data4_3,
                         unsigned char* const data4_4, unsigned char* const data4_5,
                         unsigned char* const data4_6, unsigned char* const data4_7) const;
    UInt8  QAPI getData4_0() const;
    UInt8  QAPI getData4_1() const;
    UInt8  QAPI getData4_2() const;
    UInt8  QAPI getData4_3() const;
    UInt8  QAPI getData4_4() const;
    UInt8  QAPI getData4_5() const;
    UInt8  QAPI getData4_6() const;
    UInt8  QAPI getData4_7() const;
    UInt64 QAPI getData64_0() const;
    UInt64 QAPI getData64_1() const;
    void QAPI getData64(UInt64 * const data64_0, UInt64 * const data64_1) const;
    const Char* QAPI getComment() const;

    // serialisation
    String QAPI serialise(const bool includeComment = true) const;
    String QAPI toString(const bool includeComment = true) const;

    // factory
    static cGuid2 QAPI createUnique(const Char* const comment) throw(cBadFormat);
    static cGuid2 QAPI createUnique(const String& comment) throw(cBadFormat);

  private :
    #pragma warning(push)
    #pragma warning(disable: 4201)  // Ignore "nonstandard extension used : nameless struct/union"
    union
    {
      struct
      {
        mutable UInt32  m_Data1;
        mutable UInt16  m_Data2;
        mutable UInt16  m_Data3;
        mutable UInt8   m_Data4[8];
      };
      struct
      {
        mutable UInt64 m_Data64[2];
      };
    };
    #pragma warning(pop)

    mutable Char m_SmallComment[24];
    mutable String m_BigComment;

    void _deserialise(const Char* const src);
    void _setComment(const Char* const newComment, const size_t numChars);
  };
  //}}}
  //{{{
  // =======================================================================================
  class cCowPimpl : public iCowPointable, public cRefCount
  {
  public :
    QDEFINE_SMARTCOWPTR(cCowPimpl);
    virtual cCowPimpl * QAPI clone() { return 0; }
  };
  //}}}
  //{{{  class cComplexRect
  // =======================================================================================
  #define IS_COMPLEXRECT_COW 0

  #if !IS_COMPLEXRECT_COW
    class cComplexRectImpl;
  #endif

  class cComplexRect
  {
  public :
    // --- Construction & Destruction ----------------------------------------------------
    cComplexRect();
    cComplexRect(const cRect& InitialFragment);
    cComplexRect(const std::vector<cRect>& InitialFragments);
    cComplexRect(const cComplexRect& other);
    ~cComplexRect();

    // --- Assignment --------------------------------------------------------------------
    cComplexRect& QAPI operator=(const cComplexRect& other);
    cComplexRect& QAPI operator=(const cRect& other);

    // --- Logical Operations ------------------------------------------------------------
    cComplexRect& QAPI or(const cRect& other);
    cComplexRect& QAPI orNotOverlapping(const cRect& other);
    cComplexRect& QAPI or(const cComplexRect& other);
    cComplexRect& QAPI or(const cComplexRect& other, const cXY& OrgOffset);

    cComplexRect& QAPI subtract(const cRect& other);
    cComplexRect& QAPI subtract(const cComplexRect& other);
    cComplexRect& QAPI subtract(const cComplexRect & other, const cXY& OrgOffset);

    cComplexRect& QAPI and(const cRect& other);
    cComplexRect& QAPI and(const cComplexRect& other);
    cComplexRect& QAPI and(const cComplexRect& other, const cXY& OrgOffset);

    cComplexRect& QAPI not(const cRect& other);
    cComplexRect& QAPI not(const cComplexRect& other);
    cComplexRect& QAPI not(const cComplexRect& other, const cXY & OrgOffset);

    // --- Misc. -------------------------------------------------------------------------
    // Returns true if any optimisation was done. Can be done multiple times until there
    // are no more optimisations possible for the specifed coverage ratio.
    //
    // For each pair or fragments :
    //  IF the area of the union of two fragments is smaller than SmallAreaThreshold then
    //  they will be amalgamated.
    //  ELSE IF two fragments cover more than BigAreaRatio% of the area of their union then
    //  they will be amalgamated.
    // Area covered MAY be changed!
    bool QAPI amalgamate(
    const float         BigAreaRatio,
    const unsigned int  SmallAreaThreshold);

    // Neighbouring fragments will be joined. Area covered will NOT be changed!
    bool QAPI optimise();

    // Does this complex rect fully contain 'other'?
    bool QAPI isContaining(const cRect& other) const;
    bool QAPI isContaining(const cComplexRect& other) const;
    bool QAPI isContaining(const cComplexRect& other, const cXY& OrgOffset) const;

    // Does this complex rect fully overlap 'other'?
    bool QAPI isOverlapping(const cRect& other) const;
    bool QAPI isOverlapping(const cComplexRect& other) const;
    bool QAPI isOverlapping(const cComplexRect& other, const cXY& OrgOffset) const;

    bool QAPI isDegenerate() const;

    unsigned int QAPI countFragments() const;

    /*
    Returns true if complex rect changed because of operation.
    */
    bool QAPI clearFragments();

    cRect QAPI calcBounds() const;

    cComplexRect& QAPI incOrg(const cXY Inc);

    String QAPI serialise() const;

    std::vector<cRect> QAPI getFragments() const;

    //{{{
    class cFragmentIterator
    {
    public:
      cFragmentIterator() {}
      virtual ~cFragmentIterator();
      cFragmentIterator(const cComplexRect * const ComplexRect);
      bool /* is valid? */ gotoFragment(const unsigned int WhichFragment);
      bool /* is valid? */ nextFragment();
      bool /* is valid? */ prevFragment();
      cRect * operator->() const;
      cRect& operator*() const;
    private:
    #ifdef _M_X64
      unsigned char m_Pimpl[64];
    #else
      unsigned char m_Pimpl[32];
    #endif
    };
    //}}}
    friend cFragmentIterator;

    cRect& QAPI getFragment(const unsigned int i) const;

  private :
  #if IS_COMPLEXRECT_COW
    cCowPimpl::CowPtr m_Pimpl;
  #else
    #ifdef _M_X64
      unsigned char m_Pimpl[320];
    #else
      unsigned char m_Pimpl[256];
    #endif
  #endif
  };
  //}}}
  //{{{
  // =======================================================================================
  class iGuarded
  {
  public :
    QDEFINE_SMARTPTR(iGuarded);
    virtual ~iGuarded() {}
  };
  //}}}
  //{{{
  // ---------------------------------------------------------------------------------------
  class iGuardCreator
  {
  public :
    QDEFINE_SMARTPTR(iGuardCreator);
    virtual ~iGuardCreator() {}

    virtual void QAPI onGuardDestroyed(iGuarded::Ptr Guarded) = 0;
  };
  //}}}
  //{{{
  // ---------------------------------------------------------------------------------------
  class cGuard : public cRefCount
  {
  public :
    QDEFINE_SMARTPTR(cGuard);

    iGuardCreator::Ptr  m_Creator;
    iGuarded::Ptr       m_Guarded;

    cGuard(const iGuardCreator::Ptr& Creator, const iGuarded::Ptr& Guarded);
    ~cGuard();

  private :
    // No copying.
    cGuard(const cGuard & other);
    void operator=(const cGuard& other);
  };
  //}}}

  //{{{
  // =======================================================================================
  class iMemoryObserver
  {
  public :
    QDEFINE_SMARTPTR(iMemoryObserver);
    virtual ~iMemoryObserver() {}
    virtual void onRelease(void* ObjectPtr) = 0;
  };
  //}}}
  //{{{  class cMemory
  // =======================================================================================
  #if defined(_DEBUG)
    #define QSWITCH_CHECK_MEM_GUARD_BAND_ON_PTR_ACCESS 0
  #endif

  class cMemory : public cRefCount
  {
  public :
    QDEFINE_SMARTPTR(cMemory);
    ~cMemory();

    enum eType
    {
      eHeap         = 0,
      eVirtual      = 1,
      eFastIO       = 1,  // same as virtual
      ePreallocated = 2,  // internal use only!
      eNamed        = 3   // named memory is sharable with other processes
    };

    cMemory(const size_t NumBytes,      const enum eType TypeOfMemory) throw(cNoRoom);
    // Alignment: align address multiple of this and also align internal allocation size
    cMemory(const size_t NumBytes,      const enum eType TypeOfMemory, const size_t Alignment) throw(cNoRoom);
    cMemory(const String& StringToCopy, const enum eType TypeOfMemory) throw(cNoRoom);

    // Only use this to encapulated your own allocated memory - the cMemory will NOT dealloc it when
    // it is destroyed, you are responsible for delloc.
    cMemory(char* const BytesToUse, const size_t NumBytes, const size_t ByteAlignment, cPtr<iMemoryObserver> Observer = cPtr<iMemoryObserver>());

    void          QAPI resize(const size_t NewSize) throw(cNoRoom, cNotSupported);

    size_t        QAPI getSize() const;
    const char*   QAPI getConstBytes() const;
    char*         QAPI getBytes();

    cMemory::Ptr  QAPI clone() const;

    eType         QAPI getType() const;
    size_t        QAPI getAlignment() const;
    size_t        QAPI getAlignedSize() const; // this size includes alignment padding

    void          QAPI clear();
    cMemory::Ptr  QAPI cloneAppending(cMemory::ConstPtr ToAppend) const;

    String        QAPI getName() const;

  private :
    cPtr<class cMemoryImpl> m_Pimpl;

    // Not permitted!
    cMemory(cMemory& other);
    cMemory(const not_size_t NumBytes, const enum eType TypeOfMemory) throw(cNoRoom);
    cMemory(const not_size_t NumBytes, const enum eType TypeOfMemory, const not_size_t Alignment) throw(cNoRoom);
    cMemory(char* const BytesToUse, const not_size_t NumBytes, const not_size_t ByteAlignment, cPtr<iMemoryObserver> Observer = cPtr<iMemoryObserver>());
    void operator=(cMemory& other);
    void operator&();
    void operator,(cMemory& other);

  #if QSWITCH_CHECK_MEM_GUARD_BAND_ON_PTR_ACCESS
    // Use these to validate guard band on every ptr access
    void QAPI onAfterAddRef(const void * const PtrID, const void * const ModuleAddr, const unsigned int  RefCount) const;
    void QAPI onBeforeRemoveRef(const void * const PtrID, const void * const ModuleAddr, const unsigned int  RefCount) const;
  #endif
  };
  //}}}
  //{{{
  // =======================================================================================
  class iMediaStorage
  {
  public :
    QDEFINE_SMARTPTR(iMediaStorage);
    virtual ~iMediaStorage() {}
  };
  //}}}
  //{{{
  // =======================================================================================
  class cMemoryView : public iMediaStorage, public cRefCount
  {
  public :
    QDEFINE_SMARTPTR(cMemoryView);

    cMemoryView(const cMemory::Ptr& Memory);
    cMemoryView(const cMemory::Ptr& Memory, const size_t ByteOffset, const size_t NumBytes);
    cMemoryView(const cMemory::Ptr& Memory, const size_t ByteOffset, const size_t NumBytes, cPtr<iMemoryObserver> Observer);
    // an optional destruction-observer is provided, rather than making the whole class virtual which would allow a derived class to overload the destructor
    // - used by cSlotAlloctor to handout cMemoryViews for ImageMemory from a large cMemory allocation

    size_t            QAPI getSize() const;
    size_t            QAPI getOffset() const;
    const char*       QAPI getConstBytes() const;
    char*             QAPI getBytes();

    cMemoryView::Ptr      QAPI first (const size_t NumBytes);
    cMemoryView::ConstPtr QAPI firstConst (const size_t NumBytes) const;

    cMemoryView::Ptr      QAPI last (const size_t NumBytes);
    cMemoryView::ConstPtr QAPI lastConst (const size_t NumBytes) const;

    cMemoryView::Ptr      QAPI range (const size_t ByteOffset, const size_t NumBytes);
    cMemoryView::ConstPtr QAPI rangeConst (const size_t ByteOffset, const size_t NumBytes) const;

    cMemoryView::Ptr      QAPI trimmed (const size_t ByteOffsetFromStart, const size_t ByteOffsetFromEnd);
    cMemoryView::ConstPtr QAPI trimmedConst (const size_t ByteOffsetFromStart, const size_t ByteOffsetFromEnd) const;

    cMemoryView::Ptr QAPI clone() const;
    void QAPI clear();

    cMemoryView::Ptr QAPI cloneAppending(const cMemoryView::ConstPtr& ToAppend) const;
    cMemoryView::Ptr QAPI cloneAppending(const std::list<cMemoryView::Ptr>& ToAppend) const;
    cMemoryView::Ptr QAPI cloneAppendingConst(const std::list<cMemoryView::ConstPtr>& ToAppend) const;

    cMemory::ConstPtr QAPI getPrivateMemory() const; // Use with care!
    cMemory::ConstPtr QAPI getPrivateMemory(size_t * const ByteOffset /* can be null */) const; // Use with care!
    String            QAPI getName() const;

  private :
    cPtr<class cMemoryViewImpl> m_Pimpl;

    // Not permitted!
    cMemoryView(const cMemory::Ptr& Memory, const not_size_t ByteOffset, const not_size_t NumBytes);
    cMemoryView::Ptr      QAPI first (const not_size_t NumBytes);
    cMemoryView::ConstPtr QAPI firstConst (const not_size_t NumBytes) const;
    cMemoryView::Ptr      QAPI last (const not_size_t NumBytes);
    cMemoryView::ConstPtr QAPI lastConst (const not_size_t NumBytes) const;
    cMemoryView::Ptr      QAPI range (const not_size_t ByteOffset, const not_size_t NumBytes);
    cMemoryView::ConstPtr QAPI rangeConst (const not_size_t ByteOffset, const not_size_t NumBytes) const;
    cMemoryView::Ptr      QAPI trimmed (const not_size_t ByteOffsetFromStart, const not_size_t ByteOffsetFromEnd);
    cMemoryView::ConstPtr QAPI trimmedConst (const not_size_t ByteOffsetFromStart, const not_size_t ByteOffsetFromEnd) const;
    cMemory::ConstPtr QAPI getPrivateMemory(not_size_t * const ByteOffset) const;

    cMemoryView(cMemoryView& other);
    void operator=(cMemoryView& other);
    void operator&();
    void operator,(cMemoryView& other);

  #if QSWITCH_CHECK_MEM_GUARD_BAND_ON_PTR_ACCESS
    // Use these to validate guard band on every ptr access
    void QAPI onAfterAddRef(const void * const PtrID, const void * const ModuleAddr, const unsigned int  RefCount) const;
    void QAPI onBeforeRemoveRef(const void * const PtrID, const void * const ModuleAddr, const unsigned int  RefCount) const;
  #endif
  };
  //}}}
}

#include "QKernelCheckedCasts.h"

#if defined(_MSC_VER)
  #pragma pack(pop)
#endif
