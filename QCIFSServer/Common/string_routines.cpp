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

//string_routines.cpp

#include "stdafx.h"

#include "string_routines.h"

using namespace vfs;

String toStringWin32ErrCode(const long Win32ErrCode)
{
  String ErrorMessage;
  Char * FormatMessageBuffer = 0;
	if (FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
		NULL,
		Win32ErrCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&FormatMessageBuffer,
		0,
		NULL) != 0)
  {
    ErrorMessage = FormatMessageBuffer;
    LocalFree(FormatMessageBuffer);
  }
  else
  {
    ErrorMessage = L"Unknown Win32 error code";
  }
  return ErrorMessage;
}