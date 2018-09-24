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

#pragma pack(push)
#pragma pack(1)

const ULONG REQUEST_OWNER = 0x00000001;
const ULONG REQUEST_GROUP = 0x00000002;
const ULONG REQUEST_DACL = 0x00000004;
const ULONG REQUEST_SACL = 0x00000008;

struct BufferFullError
{
  ULONG RequiredBufferSize;
};

#pragma pack(pop)