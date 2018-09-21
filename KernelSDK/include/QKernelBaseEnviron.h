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

#if !defined(__CF7B5C84A592491a89492424EFFA98ED)
#define __CF7B5C84A592491a89492424EFFA98ED
/*
**  
*/

// =======================================================================================
// === Compiler Warning Disables =========================================================
// =======================================================================================

#define QSWITCH_LV1_WARNINGS_DISABLED
#define QSWITCH_LV2_WARNINGS_DISABLED
#define QSWITCH_LV3_WARNINGS_DISABLED
#define QSWITCH_LV4_WARNINGS_DISABLED


// =======================================================================================
// === Environment Checks ================================================================
// =======================================================================================
// Look out for missing preprocesser defines, unsupported compiler settings, etc.

#if !defined(_WIN32)
  #error Platform not supported!
#endif

#if !defined(__cplusplus)
  #error This is a C++ header only!
#endif

#if defined(_MSC_VER) // Is Visual C++ ?
  //{{{
  #if !(_MSC_VER >= 1500) // 1500 is Visual C++ 9.0
    #error Requires at least Microsoft Visual C++ 9.0 (2008)
  #endif

  #if (!defined(_MT) || !defined(_DLL)) && !defined(QKERNEL_IS_USING_NON_DLL_CRT)
    #error Please use the multithreaded DLL version of the C/C++ runtime library!
  #endif

  #if defined(_CHAR_UNSIGNED)
    #error Char is expected to be signed!
  #endif

  #if defined(_DEBUG)
    #if defined(_NDEBUG) || defined(NDEBUG)
      #error Only one of _DEBUG or _NDEBUG (or NDEBUG) can be defined!
    #endif
  #else
    #if !defined(_NDEBUG)
      #define _NDEBUG
    #endif
    #if !defined(NDEBUG)
      #define NDEBUG
    #endif
  #endif

  // set the UNICODE flags up so that it if one is set then the other is too
  #if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
  #endif

  #if defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
  #endif

  // this should never be true
  #if defined(UNICODE) && defined(_MBCS)
    #error Either _MBCS or UNICODE/_UNICODE may be defined - KernelBase expects UNICODE only!
  #endif

  #include <float.h>
  #if FLT_MANT_DIG != 24
    #error "Floating point support has not been tested with this mantissa accuracy!"
  #endif

  #if FLT_MAX_EXP != 128
    #error "Floating point support has not been tested with this exponent accuracy!"
  #endif

  //}}}

#else
  #error Compiler not supported!
#endif

// =======================================================================================
// === Warning Disables ==================================================================
// =======================================================================================

#if defined(_MSC_VER) // Is Visual C++ ?

  //{{{
  #if defined(QSWITCH_LV1_WARNINGS_DISABLED)
    //#pragma warning(disable: 4995)  // Ignore "..name was marked as #pragma deprecated"
    #pragma warning(disable: 4996)  // Ignore "This function or variable may be unsafe." or "..has been changed to conform with the ISO C standard"
    //#pragma warning(disable: 4297)  // Ignore "function assumed not to throw an exception but does"
  #endif
  //}}}

  //{{{
  #if defined(QSWITCH_LV2_WARNINGS_DISABLED)
    #pragma warning(disable: 4275)  // Ignore "non DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier'"
    #pragma warning(disable: 4127)  // Ignore "conditional expression is constant"
  #endif
  //}}}

  //{{{
  #if defined(QSWITCH_LV3_WARNINGS_DISABLED)
    // Visual Studio does not support the C++ exception specification
    #pragma warning(disable: 4290)  // Ignore "C++ Exception Specification ignored"

    // The standard C++ library templates create huge decorated names.
    #pragma warning(disable: 4786)  // Ignore "Identifier truncated...in the debug information"

    #if defined(_DEBUG)
      //When compiling with /Wp64, or when compiling on a 64-bit operating system, type is 32 bits but size_t is 64 bits when compiling for 64-bit targets. 
      #pragma warning(disable: 4267)  // Ignore "conversion from 'size_t' to 'type', possible loss of data"
    #endif
  #endif
  //}}}

  //{{{
  #if defined(QSWITCH_LV4_WARNINGS_DISABLED)
    #pragma warning(disable: 4100)  // Ignore "unreferenced formal parameter"
    #pragma warning(disable: 4201)  // Ignore "nonstandard extension used : nameless struct/union"
    //#pragma warning(disable: 4510)  // Ignore "default constructor could not be generated"
    //#pragma warning(disable: 4511)  // Ignore "copy constructor could not be generated"
    #pragma warning(disable: 4512)  // Ignore "assignment operator could not be generated"
    #pragma warning(disable: 4514)  // Ignore "unreferenced inline function has been removed"
    //#pragma warning(disable: 4610)  // Ignore "can never be instantiated - user defined constructor required"
    #pragma warning(disable: 4710)  // Ignore "function not inlined"
    #pragma warning(disable: 4702)  // Ignore "unreachable code"
    #pragma warning(disable: 4711)  // Ignore "function selected for automatic inline expansion"
  #endif
  //}}}

  // This is being promoted because, if ignored, this *will* cause memory leaks.
  #pragma warning (error: 150) // convert warning to error "deletion of pointer to incomplete type '<typename>'; no destructor called"

#endif

#endif //!defined(__CF7B5C84A592491a89492424EFFA98ED)