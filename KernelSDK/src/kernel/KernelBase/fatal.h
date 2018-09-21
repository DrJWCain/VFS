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
/*
**  
*/

#include "QKernelBase.h"

using namespace vfs;


#define QKERNELBASEVERIFY(__EXP)           { if (!(__EXP)) fatal(__FILE__, __LINE__, L"QKERNELBASEVERIFY expression \"%s\" failed!", L#__EXP); }
#define QKERNELBASEVERIFYMSG(__EXP, __MSG) { if (!(__EXP)) fatal(__FILE__, __LINE__, L"QKERNELBASEVERIFY expression \"%s\" failed! (%s)", L#__EXP, __MSG); }


__declspec(noreturn) void fatal(
  const char * const srcFilename,
  const unsigned long srcLineNumber,
  const Char * const msgFormat,
  ...);
