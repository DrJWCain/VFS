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

#if !defined(__7720FA5729384237BBAD3F85EA42EBDB)
#define __7720FA5729384237BBAD3F85EA42EBDB
/*
**  
*/

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
    #if !defined(_CPPUNWIND)
        #error C++ exception handling must be enabled - see compiler option /GX.
    #endif
    #if !defined(_CPPRTTI)
        #error Run Time Type Identification (RTTI) is required - see compiler option /GR.
    #endif
    #include <float.h>
    #if FLT_MANT_DIG != 24
        #error "Floating point support has not been tested with this mantissa accuracy!"
    #endif
    #if FLT_MAX_EXP != 128
        #error "Floating point support has not been tested with this exponent accuracy!"
    #endif
#else
    #error Compiler not supported!
#endif




#endif
