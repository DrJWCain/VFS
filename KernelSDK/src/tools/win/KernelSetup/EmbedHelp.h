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

// --- Headers ---------------------------------------------------------------------------

#include <QKernelBase.h>

// --- Use Namespace ---------------------------------------------------------------------

using namespace std;
using namespace vfs;




// =======================================================================================
// === Marker ============================================================================
// =======================================================================================

const unsigned int kMarkerLong     = 0xda1ada1a;
const unsigned int kMarkerNumBytes = 512;




// =======================================================================================
// === _findMarker =======================================================================
// =======================================================================================

namespace
{

unsigned int _findMarker(cMemoryView::ConstPtr Buffer) throw(cBadFormat)
{
  const unsigned int InvalidOffset = UINT_MAX;
  unsigned int MarkerOffset = InvalidOffset;
  unsigned int MarkerLength = 0;

  const unsigned int * p = (const unsigned int *)Buffer->getConstBytes();
  unsigned int         i = 0;
  while (i < Buffer->getSize())
  {
    if (*p == kMarkerLong)
    {
      if (MarkerOffset == InvalidOffset) MarkerOffset = i;
      MarkerLength += sizeof(kMarkerLong);
      if (MarkerLength == kMarkerNumBytes) break;
    }
    else
    {
      MarkerOffset = InvalidOffset;
      MarkerLength = 0;
    }
    p++;
    i += sizeof(kMarkerLong);
  }
  
  if (MarkerLength != kMarkerNumBytes)
  {
    throw cBadFormat(__FILE__, __LINE__, L"Marker (%d bytes filled with 0x%x) not found", kMarkerNumBytes, kMarkerLong);
  }
  return MarkerOffset;
}

}