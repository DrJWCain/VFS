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

#include "QKernelExec.h"




// =======================================================================================
// === Logging Categories ================================================================
// =======================================================================================

const vfs::iLogManager::Category kCtgDllMain                 = 1 << 4;

const vfs::iLogManager::Category kCtgAcquireDeviceDetail     = 1 << 5;
const vfs::iLogManager::Category kCtgCreateManagerDetail     = 1 << 7;
const vfs::iLogManager::Category kCtgBootPhaseBooted         = 1 << 8;
const vfs::iLogManager::Category kCtgKernelBootComms         = 1 << 9;

const vfs::iLogManager::Category kCtgFatalCallbacks          = 1 << 10;

const vfs::iLogManager::Category kCtgLoadModule              = 1 << 11;
const vfs::iLogManager::Category kCtgInitDriver              = 1 << 14;

const vfs::iLogManager::Category kCtgThreadRunLoop           = 1 << 15;

const vfs::iLogManager::Category kCtgCmdOutput               = 1 << 18;

const vfs::iLogManager::Category kCtgSummaryDetail           = 1 << 19;
const vfs::iLogManager::Category kCtgLoadModuleDetail        = 1 << 20;

const vfs::iLogManager::Category kCtgPresentStatus           = 1 << 21;
const vfs::iLogManager::Category kCtgPresentWarning          = 1 << 22;

const vfs::iLogManager::Category kCtgBkgndActivitySuspension = 1 << 23;

const vfs::iLogManager::Category kCtgLoadLoggingCategories   = 1 << 24;

const vfs::iLogManager::Category kCtgHandleLeakWarning       = 1 << 25;

const vfs::iLogManager::Category kCtgFeatureToggles          = 1 << 26;
