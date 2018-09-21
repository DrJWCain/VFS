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

#ifndef WINSCOPEDHANDLEDEF_H
#define WINSCOPEDHANDLEDEF_H

#include <Windows.h>

#include "scoped_handle.h"

typedef scoped_handle<BOOL, HANDLE, stdcall_close_func,  CloseHandle, NULL > tKernelHandle;
BOOL CloseHandle(tKernelHandle);

typedef scoped_handle<LONG, HKEY, stdcall_close_func,  RegCloseKey, NULL > tRegKeyHandle;
LONG RegCloseKey(tRegKeyHandle);

typedef scoped_handle<int, FILE*, cdecl_close_func,  ::fclose, NULL > tFileHandle;
int fclose(tFileHandle);

#endif // WINSCOPEDHANDLEDEF_H

