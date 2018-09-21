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

namespace vfs
{
  //{{{
  inline const std::string isoTime(const UInt32 y, const UInt32 m, const UInt32 d, const UInt32 h, const UInt32 n, const UInt32 s)
  {
    char buf[32];
    sprintf(buf, "%04dy%02dm%02dd - %02dh%02dm%02ds", y, m, d, h, n, s);
    return std::string(buf);
  }
  //}}}
  //{{{
  inline const String isoTimeW(const UInt32 y, const UInt32 m, const UInt32 d, const UInt32 h, const UInt32 n, const UInt32 s)
  {
    Char buf[32];
    swprintf(buf, L"%04dy%02dm%02dd - %02dh%02dm%02ds", y, m, d, h, n, s);
    return String(buf);
  }
  //}}}
}
