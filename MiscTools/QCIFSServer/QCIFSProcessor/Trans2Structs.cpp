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

//Trans2Structs.h

#include "stdafx.h"
#include "Trans2Structs.h"

// turns hi and low ints into a single 64 bit integer
__int64 makeLg(int hiPart, int loPart)
{
  LARGE_INTEGER temp;
  temp.HighPart = hiPart;
  temp.LowPart = loPart;
  return temp.QuadPart;
}

//convert FILETIME to __int64
__int64 makeLg(FILETIME ft)
{
  return makeLg(ft.dwHighDateTime, ft.dwLowDateTime);
}