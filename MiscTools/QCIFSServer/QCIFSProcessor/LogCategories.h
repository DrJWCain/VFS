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

//{{{  includes and headers
#pragma once
/*
**  
*/

// --- Headers ---------------------------------------------------------------------------

// Windows headers

// Standard C library headers

// Standard C++ library headers

// vfs Kernel headers
#include <QKernelSDK.h>

// Local headers

//}}}

// =======================================================================================
// === Logging Categories ================================================================
// =======================================================================================

// Do not extract the comments and generate documentation for these definitions.
#if !defined(_DOCUMENTATION)

  // These constants define the meaning of the log category bits defined for this module.
  // They are stored per module in the registry and are used to selectively control the type
  // of logging information produced from this module.
                                                              // no Fatal category. Use QSOS for that.
    const vfs::iLogManager::Category kCtgWarn   = 1u << 0;         // warning messages
    const vfs::iLogManager::Category kCtgNotify = 1u << 1;         // useful notification messages
    const vfs::iLogManager::Category kCtgInfo   = 1u << 2;         // low level informational messages, e.g. debug
    const vfs::iLogManager::Category kCtgLogSMB = 1u << 3;
    const vfs::iLogManager::Category kCtgLogFID = 1u << 4;
    const vfs::iLogManager::Category kCtgLogIO  = 1u << 5;
    const vfs::iLogManager::Category kCtgUser6  = 1u << 6;
    const vfs::iLogManager::Category kCtgUser7  = 1u << 7;
    const vfs::iLogManager::Category kCtgUser8  = 1u << 8;
    const vfs::iLogManager::Category kCtgUser9  = 1u << 9;
    const vfs::iLogManager::Category kCtgUser10 = 1u << 10;
    const vfs::iLogManager::Category kCtgUser11 = 1u << 11;
    const vfs::iLogManager::Category kCtgUser12 = 1u << 12;
    const vfs::iLogManager::Category kCtgUser13 = 1u << 13;
    const vfs::iLogManager::Category kCtgUser14 = 1u << 14;
    const vfs::iLogManager::Category kCtgUser15 = 1u << 15;
    const vfs::iLogManager::Category kCtgUser16 = 1u << 16;
    const vfs::iLogManager::Category kCtgUser17 = 1u << 17;
    const vfs::iLogManager::Category kCtgUser18 = 1u << 18;
    const vfs::iLogManager::Category kCtgUser19 = 1u << 19;
    const vfs::iLogManager::Category kCtgUser20 = 1u << 20;
    const vfs::iLogManager::Category kCtgUser21 = 1u << 21;
    const vfs::iLogManager::Category kCtgUser22 = 1u << 22;
    const vfs::iLogManager::Category kCtgUser23 = 1u << 23;
    const vfs::iLogManager::Category kCtgUser24 = 1u << 24;
    const vfs::iLogManager::Category kCtgUser25 = 1u << 25;
    const vfs::iLogManager::Category kCtgUser26 = 1u << 26;
    const vfs::iLogManager::Category kCtgUser27 = 1u << 27;
    const vfs::iLogManager::Category kCtgUser28 = 1u << 28;
    const vfs::iLogManager::Category kCtgUser29 = 1u << 29;
    const vfs::iLogManager::Category kCtgUser30 = 1u << 30;
    const vfs::iLogManager::Category kCtgUser31 = 1u << 31;


#endif
