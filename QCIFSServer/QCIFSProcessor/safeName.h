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

//safeName.h

#pragma once

static vfs::String makeSafeName(const PUCHAR pStrStart, const ULONG len)
{
  return (0 == len)
    ? String(L"")
    : String(reinterpret_cast<PWCHAR>(pStrStart), len/sizeof(WCHAR));
}

static vfs::String makeSafeNameFromNarrow(const PUCHAR pStrStart, const ULONG len)
{
  return widen((0 == len)
    ? std::string("")
    : std::string(reinterpret_cast<PCHAR>(pStrStart), len));
}