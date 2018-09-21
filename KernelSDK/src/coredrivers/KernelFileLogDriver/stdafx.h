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

#if !defined(STDAFX_INCLUDED)
#define STDAFX_INCLUDED

#include <QKernelSDK.h>
#include <QKernelExecDDK.h>

extern vfs::iModuleContext::Ptr gModuleContext;

#include <algorithm>

#include "..\..\..\..\Shared\utf8cpp\source\utf8.h"

#include "isoTime.h"

#include "cBaseLogDriver.h"
#include "cBaseLogDriverCtor.h"

#include "cLogFolderCleanUp.h"

#include "cTextLogDriver.h"
#include "cHtmlLogDriver.h"

#endif
