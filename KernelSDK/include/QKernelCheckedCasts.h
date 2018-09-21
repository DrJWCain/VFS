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

// vfs Casts
#pragma once
#include <limits>

//{{{  Floating Point and Integer Limits
// =======================================================================================
// All integer containing Float64 values between 0 and FLOAT64_UINT64_MAX, inclusive, map to unique unsigned __int64 values
#define FLOAT64_UINT64_MAX 9007199254740992.0L

// All integer containing Float64 values between FLOAT64_INT64_MIN and FLOAT64_INT64_MAX, inclusive, map to unique unsigned __int64 values
#define FLOAT64_INT64_MIN (-9007199254740992.0L)
#define FLOAT64_INT64_MAX 9007199254740992.0L

// All integer containing Float32 values between 0 and FLOAT32_UINT64_MAX, inclusive, map to unique unsigned __int64 values
#define FLOAT32_UINT64_MAX 16777216.0L

// All integer containing Float32 values between FLOAT32_INT64_MIN and FLOAT32_INT64_MAX, inclusive, map to unique unsigned __int64 values
#define FLOAT32_INT64_MIN (-16777216.0L)
#define FLOAT32_INT64_MAX 16777216.0L

// All integer containing Float32 values between 0 and FLOAT32_UINT32_MAX, inclusive, map to unique unsigned __int32 values
#define FLOAT32_UINT32_MAX 16777216.0L

// All integer containing Float32 values between FLOAT32_INT32_MIN and FLOAT32_INT32_MAX, inclusive, map to unique unsigned __int32 values
#define FLOAT32_INT32_MIN (-16777216.0L)
#define FLOAT32_INT32_MAX 16777216.0L
//}}}
//{{{ Unchecked static casts
template<class T> inline T unchecked_static_cast(unsigned __int64 x)
{
  return static_cast<T>(x);
}
template<class T> inline T unchecked_static_cast(__int64 x)
{
  return static_cast<T>(x);
}
template<class T> inline T unchecked_static_cast(unsigned __int32 x)
{
  return static_cast<T>(x);
}
template<class T> inline T unchecked_static_cast(__int32 x)
{
  return static_cast<T>(x);
}
template<class T> inline T unchecked_static_cast(unsigned long x)
{
  return static_cast<T>(x);
}
template<class T> inline T unchecked_static_cast(long x)
{
  return static_cast<T>(x);
}
template<class T> inline T unchecked_static_cast(unsigned __int16 x)
{
  return static_cast<T>(x);
}
template<class T> inline T unchecked_static_cast(__int16 x)
{
  return static_cast<T>(x);
}
template<class T> inline T unchecked_static_cast(unsigned __int8 x)
{
  return static_cast<T>(x);
}
template<class T> inline T unchecked_static_cast(__int8 x)
{
  return static_cast<T>(x);
}
template<class T> inline T unchecked_static_cast(double x)
{
  return static_cast<T>(x);
}
template<class T> inline T unchecked_static_cast(float x)
{
  return static_cast<T>(x);
}
//}}}
//{{{ Fits static casts
// unsigned __int64 -> T
template<class T> inline bool fits_static_cast(unsigned __int64 x)
{
  #pragma message( __FILE__ "(?) : not implemented : fits_static_cast<?>(unsigned __int64)" )
}
// unsigned __int64 -> unsigned __int64
template<> inline bool fits_static_cast<unsigned __int64>(unsigned __int64 x)
{
  return true;
}
// unsigned __int64 -> __int64
template<> inline bool fits_static_cast<__int64>(unsigned __int64 x)
{
  return (x <= static_cast<unsigned __int64>((std::numeric_limits<__int64>::max)()));
}
// unsigned __int64 -> unsigned __int32
template<> inline bool fits_static_cast<unsigned __int32>(unsigned __int64 x)
{
  return (x <= static_cast<unsigned __int64>((std::numeric_limits<unsigned __int32>::max)()));
}
// unsigned __int64 -> __int32
template<> inline bool fits_static_cast<__int32>(unsigned __int64 x)
{
  return (x <= static_cast<unsigned __int64>((std::numeric_limits<__int32>::max)()));
}
// unsigned __int64 -> unsigned long
template<> inline bool fits_static_cast<unsigned long>(unsigned __int64 x)
{
  return (x <= static_cast<unsigned __int64>((std::numeric_limits<unsigned long>::max)()));
}
// unsigned __int64 -> long
template<> inline bool fits_static_cast<long>(unsigned __int64 x)
{
  return (x <= static_cast<unsigned __int64>((std::numeric_limits<long>::max)()));
}
// unsigned __int64 -> unsigned __int16
template<> inline bool fits_static_cast<unsigned __int16>(unsigned __int64 x)
{
  return (x <= static_cast<unsigned __int64>((std::numeric_limits<unsigned __int16>::max)()));
}
// unsigned __int64 -> __int16
template<> inline bool fits_static_cast<__int16>(unsigned __int64 x)
{
  return (x <= static_cast<unsigned __int64>((std::numeric_limits<__int16>::max)()));
}
// unsigned __int64 -> unsigned __int8
template<> inline bool fits_static_cast<unsigned __int8>(unsigned __int64 x)
{
  return (x <= static_cast<unsigned __int64>((std::numeric_limits<unsigned __int8>::max)()));
}
// unsigned __int64 -> __int8
template<> inline bool fits_static_cast<__int8>(unsigned __int64 x)
{
  return (x <= static_cast<unsigned __int64>((std::numeric_limits<__int8>::max)()));
}
// __int64 -> T
template<class T> inline bool fits_static_cast(__int64 x)
{
  #pragma message( __FILE__ "(?) : not implemented : fits_static_cast<?>(__int64)" )
}
// __int64 -> unsigned __int64
template<> inline bool fits_static_cast<unsigned __int64>(__int64 x)
{
  return (0 <= x);
}
// __int64 -> __int64
template<> inline bool fits_static_cast<__int64>(__int64 x)
{
  return true;
}
// __int64 -> unsigned __int32
template<> inline bool fits_static_cast<unsigned __int32>(__int64 x)
{
  return ((0 <= x) && (x <= static_cast<__int64>((std::numeric_limits<unsigned __int32>::max)())));
}
// __int64 -> __int32
template<> inline bool fits_static_cast<__int32>(__int64 x)
{
  return ((static_cast<__int64>((std::numeric_limits<__int32>::min)()) <= x) && (x <= static_cast<__int64>((std::numeric_limits<__int32>::max)())));
}
// __int64 -> unsigned long
template<> inline bool fits_static_cast<unsigned long>(__int64 x)
{
  return ((0 <= x) && (x <= static_cast<__int64>((std::numeric_limits<unsigned long>::max)())));
}
// __int64 -> long
template<> inline bool fits_static_cast<long>(__int64 x)
{
  return ((static_cast<__int64>((std::numeric_limits<long>::min)()) <= x) && (x <= static_cast<__int64>((std::numeric_limits<long>::max)())));
}
// __int64 -> unsigned __int16
template<> inline bool fits_static_cast<unsigned __int16>(__int64 x)
{
  return ((0 <= x) && (x <= static_cast<__int64>((std::numeric_limits<unsigned __int16>::max)())));
}
// __int64 -> __int16
template<> inline bool fits_static_cast<__int16>(__int64 x)
{
  return ((static_cast<__int64>((std::numeric_limits<__int16>::min)()) <= x) && (x <= static_cast<__int64>((std::numeric_limits<__int16>::max)())));
}
// __int64 -> unsigned __int8
template<> inline bool fits_static_cast<unsigned __int8>(__int64 x)
{
  return ((0 <= x) && (x <= static_cast<__int64>((std::numeric_limits<unsigned __int8>::max)())));
}
// __int64 -> __int8
template<> inline bool fits_static_cast<__int8>(__int64 x)
{
  return ((static_cast<__int64>((std::numeric_limits<__int8>::min)()) <= x) && (x <= static_cast<__int64>((std::numeric_limits<__int8>::max)())));
}
// unsigned __int32 -> T
template<class T> inline bool fits_static_cast(unsigned __int32 x)
{
  #pragma message( __FILE__ "(?) : not implemented : fits_static_cast<?>(unsigned __int32)" )
}
// unsigned __int32 -> unsigned __int64
template<> inline bool fits_static_cast<unsigned __int64>(unsigned __int32 x)
{
  return true;
}
// unsigned __int32 -> __int64
template<> inline bool fits_static_cast<__int64>(unsigned __int32 x)
{
  return true;
}
// unsigned __int32 -> unsigned __int32
template<> inline bool fits_static_cast<unsigned __int32>(unsigned __int32 x)
{
  return true;
}
// unsigned __int32 -> __int32
template<> inline bool fits_static_cast<__int32>(unsigned __int32 x)
{
  return (x <= static_cast<unsigned __int32>((std::numeric_limits<__int32>::max)()));
}
// unsigned __int32 -> unsigned long
template<> inline bool fits_static_cast<unsigned long>(unsigned __int32 x)
{
  return true;
}
// unsigned __int32 -> long
template<> inline bool fits_static_cast<long>(unsigned __int32 x)
{
  return (x <= static_cast<unsigned __int32>((std::numeric_limits<long>::max)()));
}
// unsigned __int32 -> unsigned __int16
template<> inline bool fits_static_cast<unsigned __int16>(unsigned __int32 x)
{
  return (x <= static_cast<unsigned __int32>((std::numeric_limits<unsigned __int16>::max)()));
}
// unsigned __int32 -> __int16
template<> inline bool fits_static_cast<__int16>(unsigned __int32 x)
{
  return (x <= static_cast<unsigned __int32>((std::numeric_limits<__int16>::max)()));
}
// unsigned __int32 -> unsigned __int8
template<> inline bool fits_static_cast<unsigned __int8>(unsigned __int32 x)
{
  return (x <= static_cast<unsigned __int32>((std::numeric_limits<unsigned __int8>::max)()));
}
// unsigned __int32 -> __int8
template<> inline bool fits_static_cast<__int8>(unsigned __int32 x)
{
  return (x <= static_cast<unsigned __int32>((std::numeric_limits<__int8>::max)()));
}
// __int32 -> T
template<class T> inline bool fits_static_cast(__int32 x)
{
  #pragma message( __FILE__ "(?) : not implemented : fits_static_cast<?>(__int32)" )
}
// __int32 -> unsigned __int64
template<> inline bool fits_static_cast<unsigned __int64>(__int32 x)
{
  return (0 <= x);
}
// __int32 -> __int64
template<> inline bool fits_static_cast<__int64>(__int32 x)
{
  return true;
}
// __int32 -> unsigned __int32
template<> inline bool fits_static_cast<unsigned __int32>(__int32 x)
{
  return (0 <= x);
}
// __int32 -> __int32
template<> inline bool fits_static_cast<__int32>(__int32 x)
{
  return true;
}
// __int32 -> unsigned long
template<> inline bool fits_static_cast<unsigned long>(__int32 x)
{
  return (0 <= x);
}
// __int32 -> long
template<> inline bool fits_static_cast<long>(__int32 x)
{
  return true;
}
// __int32 -> unsigned __int16
template<> inline bool fits_static_cast<unsigned __int16>(__int32 x)
{
  return ((0 <= x) && (x <= static_cast<__int32>((std::numeric_limits<unsigned __int16>::max)())));
}
// __int32 -> __int16
template<> inline bool fits_static_cast<__int16>(__int32 x)
{
  return ((static_cast<__int32>((std::numeric_limits<__int16>::min)()) <= x) && (x <= static_cast<__int32>((std::numeric_limits<__int16>::max)())));
}
// __int32 -> unsigned __int8
template<> inline bool fits_static_cast<unsigned __int8>(__int32 x)
{
  return ((0 <= x) && (x <= static_cast<__int32>((std::numeric_limits<unsigned __int8>::max)())));
}
// __int32 -> __int8
template<> inline bool fits_static_cast<__int8>(__int32 x)
{
  return ((static_cast<__int32>((std::numeric_limits<__int8>::min)()) <= x) && (x <= static_cast<__int32>((std::numeric_limits<__int8>::max)())));
}
// unsigned long -> T
template<class T> inline bool fits_static_cast(unsigned long x)
{
  #pragma message( __FILE__ "(?) : not implemented : fits_static_cast<?>(unsigned long)" )
}
// unsigned long -> unsigned __int64
template<> inline bool fits_static_cast<unsigned __int64>(unsigned long x)
{
  return true;
}
// unsigned long -> __int64
template<> inline bool fits_static_cast<__int64>(unsigned long x)
{
  return true;
}
// unsigned long -> unsigned __int32
template<> inline bool fits_static_cast<unsigned __int32>(unsigned long x)
{
  return true;
}
// unsigned long -> __int32
template<> inline bool fits_static_cast<__int32>(unsigned long x)
{
  return (x <= static_cast<unsigned long>((std::numeric_limits<__int32>::max)()));
}
// unsigned long -> unsigned long
template<> inline bool fits_static_cast<unsigned long>(unsigned long x)
{
  return true;
}
// unsigned long -> long
template<> inline bool fits_static_cast<long>(unsigned long x)
{
  return (x <= static_cast<unsigned long>((std::numeric_limits<long>::max)()));
}
// unsigned long -> unsigned __int16
template<> inline bool fits_static_cast<unsigned __int16>(unsigned long x)
{
  return (x <= static_cast<unsigned long>((std::numeric_limits<unsigned __int16>::max)()));
}
// unsigned long -> __int16
template<> inline bool fits_static_cast<__int16>(unsigned long x)
{
  return (x <= static_cast<unsigned long>((std::numeric_limits<__int16>::max)()));
}
// unsigned long -> unsigned __int8
template<> inline bool fits_static_cast<unsigned __int8>(unsigned long x)
{
  return (x <= static_cast<unsigned long>((std::numeric_limits<unsigned __int8>::max)()));
}
// unsigned long -> __int8
template<> inline bool fits_static_cast<__int8>(unsigned long x)
{
  return (x <= static_cast<unsigned long>((std::numeric_limits<__int8>::max)()));
}
// long -> T
template<class T> inline bool fits_static_cast(long x)
{
  #pragma message( __FILE__ "(?) : not implemented : fits_static_cast<?>(long)" )
}
// long -> unsigned __int64
template<> inline bool fits_static_cast<unsigned __int64>(long x)
{
  return (0 <= x);
}
// long -> __int64
template<> inline bool fits_static_cast<__int64>(long x)
{
  return true;
}
// long -> unsigned __int32
template<> inline bool fits_static_cast<unsigned __int32>(long x)
{
  return (0 <= x);
}
// long -> __int32
template<> inline bool fits_static_cast<__int32>(long x)
{
  return true;
}
// long -> unsigned long
template<> inline bool fits_static_cast<unsigned long>(long x)
{
  return (0 <= x);
}
// long -> long
template<> inline bool fits_static_cast<long>(long x)
{
  return true;
}
// long -> unsigned __int16
template<> inline bool fits_static_cast<unsigned __int16>(long x)
{
  return ((0 <= x) && (x <= static_cast<long>((std::numeric_limits<unsigned __int16>::max)())));
}
// long -> __int16
template<> inline bool fits_static_cast<__int16>(long x)
{
  return ((static_cast<long>((std::numeric_limits<__int16>::min)()) <= x) && (x <= static_cast<long>((std::numeric_limits<__int16>::max)())));
}
// long -> unsigned __int8
template<> inline bool fits_static_cast<unsigned __int8>(long x)
{
  return ((0 <= x) && (x <= static_cast<long>((std::numeric_limits<unsigned __int8>::max)())));
}
// long -> __int8
template<> inline bool fits_static_cast<__int8>(long x)
{
  return ((static_cast<long>((std::numeric_limits<__int8>::min)()) <= x) && (x <= static_cast<long>((std::numeric_limits<__int8>::max)())));
}
// unsigned __int16 -> T
template<class T> inline bool fits_static_cast(unsigned __int16 x)
{
  #pragma message( __FILE__ "(?) : not implemented : fits_static_cast<?>(unsigned __int16)" )
}
// unsigned __int16 -> unsigned __int64
template<> inline bool fits_static_cast<unsigned __int64>(unsigned __int16 x)
{
  return true;
}
// unsigned __int16 -> __int64
template<> inline bool fits_static_cast<__int64>(unsigned __int16 x)
{
  return true;
}
// unsigned __int16 -> unsigned __int32
template<> inline bool fits_static_cast<unsigned __int32>(unsigned __int16 x)
{
  return true;
}
// unsigned __int16 -> __int32
template<> inline bool fits_static_cast<__int32>(unsigned __int16 x)
{
  return true;
}
// unsigned __int16 -> unsigned long
template<> inline bool fits_static_cast<unsigned long>(unsigned __int16 x)
{
  return true;
}
// unsigned __int16 -> long
template<> inline bool fits_static_cast<long>(unsigned __int16 x)
{
  return true;
}
// unsigned __int16 -> unsigned __int16
template<> inline bool fits_static_cast<unsigned __int16>(unsigned __int16 x)
{
  return true;
}
// unsigned __int16 -> __int16
template<> inline bool fits_static_cast<__int16>(unsigned __int16 x)
{
  return (x <= static_cast<unsigned __int16>((std::numeric_limits<__int16>::max)()));
}
// unsigned __int16 -> unsigned __int8
template<> inline bool fits_static_cast<unsigned __int8>(unsigned __int16 x)
{
  return (x <= static_cast<unsigned __int16>((std::numeric_limits<unsigned __int8>::max)()));
}
// unsigned __int16 -> __int8
template<> inline bool fits_static_cast<__int8>(unsigned __int16 x)
{
  return (x <= static_cast<unsigned __int16>((std::numeric_limits<__int8>::max)()));
}
// __int16 -> T
template<class T> inline bool fits_static_cast(__int16 x)
{
  #pragma message( __FILE__ "(?) : not implemented : fits_static_cast<?>(__int16)" )
}
// __int16 -> unsigned __int64
template<> inline bool fits_static_cast<unsigned __int64>(__int16 x)
{
  return (0 <= x);
}
// __int16 -> __int64
template<> inline bool fits_static_cast<__int64>(__int16 x)
{
  return true;
}
// __int16 -> unsigned __int32
template<> inline bool fits_static_cast<unsigned __int32>(__int16 x)
{
  return (0 <= x);
}
// __int16 -> __int32
template<> inline bool fits_static_cast<__int32>(__int16 x)
{
  return true;
}
// __int16 -> unsigned long
template<> inline bool fits_static_cast<unsigned long>(__int16 x)
{
  return (0 <= x);
}
// __int16 -> long
template<> inline bool fits_static_cast<long>(__int16 x)
{
  return true;
}
// __int16 -> unsigned __int16
template<> inline bool fits_static_cast<unsigned __int16>(__int16 x)
{
  return (0 <= x);
}
// __int16 -> __int16
template<> inline bool fits_static_cast<__int16>(__int16 x)
{
  return true;
}
// __int16 -> unsigned __int8
template<> inline bool fits_static_cast<unsigned __int8>(__int16 x)
{
  return ((0 <= x) && (x <= static_cast<__int16>((std::numeric_limits<unsigned __int8>::max)())));
}
// __int16 -> __int8
template<> inline bool fits_static_cast<__int8>(__int16 x)
{
  return ((static_cast<__int16>((std::numeric_limits<__int8>::min)()) <= x) && (x <= static_cast<__int16>((std::numeric_limits<__int8>::max)())));
}
// unsigned __int8 -> T
template<class T> inline bool fits_static_cast(unsigned __int8 x)
{
  #pragma message( __FILE__ "(?) : not implemented : fits_static_cast<?>(unsigned __int8)" )
}
// unsigned __int8 -> unsigned __int64
template<> inline bool fits_static_cast<unsigned __int64>(unsigned __int8 x)
{
  return true;
}
// unsigned __int8 -> __int64
template<> inline bool fits_static_cast<__int64>(unsigned __int8 x)
{
  return true;
}
// unsigned __int8 -> unsigned __int32
template<> inline bool fits_static_cast<unsigned __int32>(unsigned __int8 x)
{
  return true;
}
// unsigned __int8 -> __int32
template<> inline bool fits_static_cast<__int32>(unsigned __int8 x)
{
  return true;
}
// unsigned __int8 -> unsigned long
template<> inline bool fits_static_cast<unsigned long>(unsigned __int8 x)
{
  return true;
}
// unsigned __int8 -> long
template<> inline bool fits_static_cast<long>(unsigned __int8 x)
{
  return true;
}
// unsigned __int8 -> unsigned __int16
template<> inline bool fits_static_cast<unsigned __int16>(unsigned __int8 x)
{
  return true;
}
// unsigned __int8 -> __int16
template<> inline bool fits_static_cast<__int16>(unsigned __int8 x)
{
  return true;
}
// unsigned __int8 -> unsigned __int8
template<> inline bool fits_static_cast<unsigned __int8>(unsigned __int8 x)
{
  return true;
}
// unsigned __int8 -> __int8
template<> inline bool fits_static_cast<__int8>(unsigned __int8 x)
{
  return (x <= static_cast<unsigned __int8>((std::numeric_limits<__int8>::max)()));
}
// __int8 -> T
template<class T> inline bool fits_static_cast(__int8 x)
{
  #pragma message( __FILE__ "(?) : not implemented : fits_static_cast<?>(__int8)" )
}
// __int8 -> unsigned __int64
template<> inline bool fits_static_cast<unsigned __int64>(__int8 x)
{
  return (0 <= x);
}
// __int8 -> __int64
template<> inline bool fits_static_cast<__int64>(__int8 x)
{
  return true;
}
// __int8 -> unsigned __int32
template<> inline bool fits_static_cast<unsigned __int32>(__int8 x)
{
  return (0 <= x);
}
// __int8 -> __int32
template<> inline bool fits_static_cast<__int32>(__int8 x)
{
  return true;
}
// __int8 -> unsigned long
template<> inline bool fits_static_cast<unsigned long>(__int8 x)
{
  return (0 <= x);
}
// __int8 -> long
template<> inline bool fits_static_cast<long>(__int8 x)
{
  return true;
}
// __int8 -> unsigned __int16
template<> inline bool fits_static_cast<unsigned __int16>(__int8 x)
{
  return (0 <= x);
}
// __int8 -> __int16
template<> inline bool fits_static_cast<__int16>(__int8 x)
{
  return true;
}
// __int8 -> unsigned __int8
template<> inline bool fits_static_cast<unsigned __int8>(__int8 x)
{
  return (0 <= x);
}
// __int8 -> __int8
template<> inline bool fits_static_cast<__int8>(__int8 x)
{
  return true;
}
// double -> T
template<class T> inline bool fits_static_cast(double x)
{
  #pragma message( __FILE__ "(?) : not implemented : fits_static_cast<?>(double)" )
}
// double -> unsigned __int64
template<> inline bool fits_static_cast<unsigned __int64>(double x)
{
  return ((0 <= x) && (x <= FLOAT64_UINT64_MAX));
}
// double -> __int64
template<> inline bool fits_static_cast<__int64>(double x)
{
  return ((FLOAT64_INT64_MIN <= x) && (x <= FLOAT64_INT64_MAX));
}
// double -> unsigned __int32
template<> inline bool fits_static_cast<unsigned __int32>(double x)
{
  return ((0 <= x) && (x <= static_cast<double>((std::numeric_limits<unsigned __int32>::max)())));
}
// double -> __int32
template<> inline bool fits_static_cast<__int32>(double x)
{
  return ((static_cast<double>((std::numeric_limits<__int32>::min)()) <= x) && (x <= static_cast<double>((std::numeric_limits<__int32>::max)())));
}
// double -> unsigned __int16
template<> inline bool fits_static_cast<unsigned __int16>(double x)
{
  return ((0 <= x) && (x <= static_cast<double>((std::numeric_limits<unsigned __int16>::max)())));
}
// double -> __int16
template<> inline bool fits_static_cast<__int16>(double x)
{
  return ((static_cast<double>((std::numeric_limits<__int16>::min)()) <= x) && (x <= static_cast<double>((std::numeric_limits<__int16>::max)())));
}
// double -> unsigned __int8
template<> inline bool fits_static_cast<unsigned __int8>(double x)
{
  return ((0 <= x) && (x <= static_cast<double>((std::numeric_limits<unsigned __int8>::max)())));
}
// double -> __int8
template<> inline bool fits_static_cast<__int8>(double x)
{
  return ((static_cast<double>((std::numeric_limits<__int8>::min)()) <= x) && (x <= static_cast<double>((std::numeric_limits<__int8>::max)())));
}
// float -> T
template<class T> inline bool fits_static_cast(float x)
{
  #pragma message( __FILE__ "(?) : not implemented : fits_static_cast<?>(float)" )
}
// float -> unsigned __int64
template<> inline bool fits_static_cast<unsigned __int64>(float x)
{
  return ((0 <= x) && (x <= FLOAT32_UINT64_MAX));
}
// float -> __int64
template<> inline bool fits_static_cast<__int64>(float x)
{
  return ((FLOAT32_INT64_MIN <= x) && (x <= FLOAT32_INT64_MAX));
}
// float -> unsigned __int32
template<> inline bool fits_static_cast<unsigned __int32>(float x)
{
  return ((0 <= x) && (x <= FLOAT32_UINT32_MAX));
}
// float -> __int32
template<> inline bool fits_static_cast<__int32>(float x)
{
  return ((FLOAT32_INT32_MIN <= x) && (x <= FLOAT32_INT32_MAX));
}
// float -> unsigned __int16
template<> inline bool fits_static_cast<unsigned __int16>(float x)
{
  return ((0 <= x) && (x <= static_cast<float>((std::numeric_limits<unsigned __int16>::max)())));
}
// float -> __int16
template<> inline bool fits_static_cast<__int16>(float x)
{
  return ((static_cast<float>((std::numeric_limits<__int16>::min)()) <= x) && (x <= static_cast<float>((std::numeric_limits<__int16>::max)())));
}
// float -> unsigned __int8
template<> inline bool fits_static_cast<unsigned __int8>(float x)
{
  return ((0 <= x) && (x <= static_cast<float>((std::numeric_limits<unsigned __int8>::max)())));
}
// float -> __int8
template<> inline bool fits_static_cast<__int8>(float x)
{
  return ((static_cast<float>((std::numeric_limits<__int8>::min)()) <= x) && (x <= static_cast<float>((std::numeric_limits<__int8>::max)())));
}
//}}}
//{{{ Checked static casts
// unsigned __int64 -> T
template<class T> inline T checked_static_cast(unsigned __int64 x)
{
  if(!fits_static_cast<T>(x))
    throw vfs::cNoRoom( __FILE__,  __LINE__, L"checked_static_cast failed" );
  return static_cast<T>(x);
}
// unsigned __int64 -> unsigned __int64
template<> inline unsigned __int64 checked_static_cast<unsigned __int64>(unsigned __int64 x)
{
  return x;
}
// __int64 -> T
template<class T> inline T checked_static_cast(__int64 x)
{
  if(!fits_static_cast<T>(x))
    throw vfs::cNoRoom( __FILE__,  __LINE__, L"checked_static_cast failed" );
  return static_cast<T>(x);
}
// __int64 -> __int64
template<> inline __int64 checked_static_cast<__int64>(__int64 x)
{
  return x;
}
// unsigned __int32 -> T
template<class T> inline T checked_static_cast(unsigned __int32 x)
{
  if(!fits_static_cast<T>(x))
    throw vfs::cNoRoom( __FILE__,  __LINE__, L"checked_static_cast failed" );
  return static_cast<T>(x);
}
// unsigned __int32 -> unsigned __int64
template<> inline unsigned __int64 checked_static_cast<unsigned __int64>(unsigned __int32 x)
{
  return static_cast<unsigned __int64>(x);
}
// unsigned __int32 -> __int64
template<> inline __int64 checked_static_cast<__int64>(unsigned __int32 x)
{
  return static_cast<__int64>(x);
}
// unsigned __int32 -> unsigned __int32
template<> inline unsigned __int32 checked_static_cast<unsigned __int32>(unsigned __int32 x)
{
  return x;
}
// unsigned __int32 -> unsigned long
template<> inline unsigned long checked_static_cast<unsigned long>(unsigned __int32 x)
{
  return x;
}
// unsigned long -> T
template<class T> inline T checked_static_cast(unsigned long x)
{
  if(!fits_static_cast<T>(x))
    throw vfs::cNoRoom( __FILE__,  __LINE__, L"checked_static_cast failed" );
  return static_cast<T>(x);
}
// unsigned long -> unsigned __int64
template<> inline unsigned __int64 checked_static_cast<unsigned __int64>(unsigned long x)
{
  return static_cast<unsigned __int64>(x);
}
// unsigned long -> __int64
template<> inline __int64 checked_static_cast<__int64>(unsigned long x)
{
  return static_cast<__int64>(x);
}
// unsigned long -> unsigned __int32
template<> inline unsigned __int32 checked_static_cast<unsigned __int32>(unsigned long x)
{
  return x;
}
// unsigned long -> unsigned long
template<> inline unsigned long checked_static_cast<unsigned long>(unsigned long x)
{
  return x;
}
// __int32 -> T
template<class T> inline T checked_static_cast(__int32 x)
{
  if(!fits_static_cast<T>(x))
    throw vfs::cNoRoom( __FILE__,  __LINE__, L"checked_static_cast failed" );
  return static_cast<T>(x);
}
// __int32 -> __int64
template<> inline __int64 checked_static_cast<__int64>(__int32 x)
{
  return static_cast<__int64>(x);
}
// __int32 -> __int32
template<> inline __int32 checked_static_cast<__int32>(__int32 x)
{
  return x;
}
// __int32 -> long
template<> inline long checked_static_cast<long>(__int32 x)
{
  return x;
}
// long -> T
template<class T> inline T checked_static_cast(long x)
{
  if(!fits_static_cast<T>(x))
    throw vfs::cNoRoom( __FILE__,  __LINE__, L"checked_static_cast failed" );
  return static_cast<T>(x);
}
// long -> __int64
template<> inline __int64 checked_static_cast<__int64>(long x)
{
  return static_cast<__int64>(x);
}
// long -> __int32
template<> inline __int32 checked_static_cast<__int32>(long x)
{
  return x;
}
// long -> long
template<> inline long checked_static_cast<long>(long x)
{
  return x;
}
// unsigned __int16 -> T
template<class T> inline T checked_static_cast(unsigned __int16 x)
{
  if(!fits_static_cast<T>(x))
    throw vfs::cNoRoom( __FILE__,  __LINE__, L"checked_static_cast failed" );
  return static_cast<unsigned __int64>(x);
}
// unsigned __int16 -> unsigned __int64
template<> inline unsigned __int64 checked_static_cast<unsigned __int64>(unsigned __int16 x)
{
  return static_cast<unsigned __int64>(x);
}
// unsigned __int16 -> __int64
template<> inline __int64 checked_static_cast<__int64>(unsigned __int16 x)
{
  return static_cast<__int64>(x);
}
// unsigned __int16 -> unsigned __int32
template<> inline unsigned __int32 checked_static_cast<unsigned __int32>(unsigned __int16 x)
{
  return static_cast<unsigned __int32>(x);
}
// unsigned __int16 -> unsigned long
template<> inline unsigned long checked_static_cast<unsigned long>(unsigned __int16 x)
{
  return static_cast<unsigned long>(x);
}
// unsigned __int16 -> __int32
template<> inline __int32 checked_static_cast<__int32>(unsigned __int16 x)
{
  return static_cast<__int32>(x);
}
// unsigned __int16 -> long
template<> inline long checked_static_cast<long>(unsigned __int16 x)
{
  return static_cast<long>(x);
}
// unsigned __int16 -> unsigned __int16
template<> inline unsigned __int16 checked_static_cast<unsigned __int16>(unsigned __int16 x)
{
  return x;
}
// unsigned __int16 -> __int16
template<> inline __int16 checked_static_cast<__int16>(unsigned __int16 x)
{
  if(!fits_static_cast<__int16>(x))
    throw vfs::cNoRoom( __FILE__,  __LINE__, L"checked_static_cast failed" );
  return static_cast<__int16>(x);
}
// unsigned __int16 -> unsigned __int8
template<> inline unsigned __int8 checked_static_cast<unsigned __int8>(unsigned __int16 x)
{
  if(!fits_static_cast<unsigned __int8>(x))
    throw vfs::cNoRoom( __FILE__,  __LINE__, L"checked_static_cast failed" );
  return static_cast<unsigned __int8>(x);
}
// unsigned __int16 -> __int8
template<> inline __int8 checked_static_cast<__int8>(unsigned __int16 x)
{
  if(!fits_static_cast<__int8>(x))
    throw vfs::cNoRoom( __FILE__,  __LINE__, L"checked_static_cast failed" );
  return static_cast<__int8>(x);
}
// __int16 -> T
template<class T> inline T checked_static_cast(__int16 x)
{
  if(!fits_static_cast<T>(x))
    throw vfs::cNoRoom( __FILE__,  __LINE__, L"checked_static_cast failed" );
  return static_cast<T>(x);
}
// __int16 -> __int64
template<> inline __int64 checked_static_cast<__int64>(__int16 x)
{
  return static_cast<__int64>(x);
}
// __int16 -> __int32
template<> inline __int32 checked_static_cast<__int32>(__int16 x)
{
  return static_cast<__int32>(x);
}
// __int16 -> long
template<> inline long checked_static_cast<long>(__int16 x)
{
  return static_cast<long>(x);
}
// __int16 -> __int16
template<> inline __int16 checked_static_cast<__int16>(__int16 x)
{
  return x;
}
// unsigned __int8 -> T
template<class T> inline T checked_static_cast(unsigned __int8 x)
{
  if(!fits_static_cast<T>(x))
    throw vfs::cNoRoom( __FILE__,  __LINE__, L"checked_static_cast failed" );
  return static_cast<T>(x);
}
// unsigned __int8 -> unsigned __int64
template<> inline unsigned __int64 checked_static_cast<unsigned __int64>(unsigned __int8 x)
{
  return static_cast<unsigned __int64>(x);
}
// unsigned __int8 -> __int64
template<> inline __int64 checked_static_cast<__int64>(unsigned __int8 x)
{
  return static_cast<__int64>(x);
}
// unsigned __int8 -> unsigned __int32
template<> inline unsigned __int32 checked_static_cast<unsigned __int32>(unsigned __int8 x)
{
  return static_cast<unsigned __int32>(x);
}
// unsigned __int8 -> unsigned long
template<> inline unsigned long checked_static_cast<unsigned long>(unsigned __int8 x)
{
  return static_cast<unsigned long>(x);
}
// unsigned __int8 -> __int32
template<> inline __int32 checked_static_cast<__int32>(unsigned __int8 x)
{
  return static_cast<__int32>(x);
}
// unsigned __int8 -> unsigned __int16
template<> inline unsigned __int16 checked_static_cast<unsigned __int16>(unsigned __int8 x)
{
  return x;
}
// unsigned __int8 -> __int16
template<> inline __int16 checked_static_cast<__int16>(unsigned __int8 x)
{
  return static_cast<__int16>(x);
}
// unsigned __int8 -> unsigned __int8
template<> inline unsigned __int8 checked_static_cast<unsigned __int8>(unsigned __int8 x)
{
  return x;
}
// __int8 -> T
template<class T> inline T checked_static_cast(__int8 x)
{
  if(!fits_static_cast<T>(x))
    throw vfs::cNoRoom( __FILE__,  __LINE__, L"checked_static_cast failed" );
  return static_cast<T>(x);
}
// __int8 -> __int64
template<> inline __int64 checked_static_cast<__int64>(__int8 x)
{
  return static_cast<__int64>(x);
}
// __int8 -> __int32
template<> inline __int32 checked_static_cast<__int32>(__int8 x)
{
  return static_cast<__int32>(x);
}
// __int8 -> long
template<> inline long checked_static_cast<long>(__int8 x)
{
  return static_cast<long>(x);
}
// __int8 -> __int16
template<> inline __int16 checked_static_cast<__int16>(__int8 x)
{
  return static_cast<unsigned __int16>(x);
}
// __int8 -> __int8
template<> inline __int8 checked_static_cast<__int8>(__int8 x)
{
  return x;
}
// double -> T
template<class T> inline T checked_static_cast(double x)
{
  if(!fits_static_cast<T>(x))
    throw vfs::cNoRoom( __FILE__,  __LINE__, L"checked_static_cast failed" );
  return static_cast<T>(x);
}
// float -> T
template<class T> inline T checked_static_cast(float x)
{
  if(!fits_static_cast<T>(x))
    throw vfs::cNoRoom( __FILE__,  __LINE__, L"checked_static_cast failed" );
  return static_cast<T>(x);
}
//}}}
//{{{ Unchecked reinterpret casts
template<class T> inline T unchecked_reinterpret_cast(unsigned __int64 x)
{
  return reinterpret_cast<T>(x);
}
//}}}
//{{{ Fits reinterpret casts
// unsigned __int64 -> T*
template<class T> inline bool fits_reinterpret_cast(unsigned __int64 x)
{
  return true;
}
// unsigned __int64 -> void*
template<> inline bool fits_reinterpret_cast<void*>(unsigned __int64 x)
{
  return (sizeof(size_t) == sizeof(__int64));
}
// unsigned __int64 -> T*
template<class T> inline bool fits_reinterpret_cast(__int64 x)
{
  return true;
}
// __int64 -> void*
template<> inline bool fits_reinterpret_cast<void*>(__int64 x)
{
  return (sizeof(size_t) == sizeof(__int64));
}
// unsigned __int32 -> T*
template<class T> inline bool fits_reinterpret_cast(unsigned __int32 x)
{
  return true;
}
// unsigned __int32 -> void*
template<> inline bool fits_reinterpret_cast<void*>(unsigned __int32 x)
{
  return (sizeof(size_t) == sizeof(unsigned __int32));
}
// unsigned __int32 -> T*
template<class T> inline bool fits_reinterpret_cast(__int32 x)
{
  return true;
}
// unsigned __int32 -> void*
template<> inline bool fits_reinterpret_cast<void>(__int32 x)
{
  return (sizeof(size_t) == sizeof(__int32));
}
//}}}
//{{{ Checked reinterpret casts
template<class T> inline T checked_reinterpret_cast(unsigned __int64 x)
{
  if(!fits_reinterpret_cast<T>(x))
    throw vfs::cNoRoom( __FILE__,  __LINE__, L"checked_reinterpret_cast failed" );
  return reinterpret_cast<T>(x);
}
template<class T> inline T checked_reinterpret_cast(__int64 x)
{
  if(!fits_reinterpret_cast<T>(x))
    throw vfs::cNoRoom( __FILE__,  __LINE__, L"checked_reinterpret_cast failed" );
  return reinterpret_cast<T>(x);
}
template<class T> inline T checked_reinterpret_cast(unsigned __int32 x)
{
  if(!fits_reinterpret_cast<T>(x))
    throw vfs::cNoRoom( __FILE__,  __LINE__, L"checked_reinterpret_cast failed" );
  return reinterpret_cast<T>(x);
}
template<class T> inline T checked_reinterpret_cast(__int32 x)
{
  if(!fits_reinterpret_cast<T>(x))
    throw vfs::cNoRoom( __FILE__,  __LINE__, L"checked_reinterpret_cast failed" );
  return reinterpret_cast<T>(x);
}
//}}}
