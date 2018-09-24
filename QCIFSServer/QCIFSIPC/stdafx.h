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

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <stdio.h>

#include <iostream>
#include <tchar.h>
#include <list>
#include <vector>
#include <string>
#include <map>
#include <set>

#include <winsock2.h>
#include <Mswsock.h>
#include <windows.h>
#include <winioctl.h>

#include "..\..\..\KernelSDK\Include\QKernelSDK.h"

#include <boost/regex.hpp>

#pragma warning (error: 150) // report warning C4150 as error
#include "..\..\QCIFSServer\QCIFSFwk\iChildLoaderVisitor.h"