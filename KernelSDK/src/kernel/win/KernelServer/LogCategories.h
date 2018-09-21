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

// These constants define the meaning of the log category bits defined for this module.
// They are stored per module in the registry and are used to selectively control the type
// of logging information produced from this module.

const vfs::iLogManager::Category kCtgKernelBootComms = 1 << 0;
