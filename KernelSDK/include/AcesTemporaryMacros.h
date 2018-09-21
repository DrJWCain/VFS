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
 *  These macros control which interfaces are exposed and which
 *  implementation is built into KernelMedia. For more information,
 *  consult Ben Russell's "ACES Project Plan, Nov 2014".
 *  
 *  The ACES_ENABLE_OLD_IFACE and HDR_ENABLE_OLD_IFACE macros are
 *  for the client code, so we can slowly migrate code towards
 *  the new interface.
 *  
 *  The ACES_ENABLE_NEW_IMPL macro switches KernelMedia to the new,
 *  full ACES implementation rather than the legacy (SD and HD)
 *  implementation. In addition, it disables legacy features in
 *  the client code (e.g. PicStore) that are no longer supported.
 *  
 *  Regardless of implementation, both old and new interfaces are
 *  implemented in KernelMedia and either emulate legacy behaviour
 *  or future behaviour depending on which implementation is in use.
 *  
 *  During ACES project:
 *
 *    - main            (old impl, with old iface enabled)
 *
 *      Work happens on main latest to port code to the new iface,
 *      while the old impl's emulation layer maintains the software's
 *      behaviour.
 *
 *    - aces_branch     (new impl, with old iface enabled)
 *
 *      This branch contains the new implementation in KernelMedia
 *      and QDatabase, and allows testing of the ACES project before
 *      the main software is complete. It also contains a modified
 *      iQ.sln, removing DLLs which will no longer be supported.
 *
 *    - aces_progess    (new impl, with old iface disabled)
 *
 *      This branch is only used to generate a burndown chart, showing
 *      how much work is left to do to remove the old iface. It is
 *      never intended to be used, just to show when we're done.
 *
 *  Completion of ACES project:
 *
 *  The aces_branch is merged into main latest, enabling the new
 *  implementation and merging the new QDatabase schema. This should
 *  include a major version change to GenQ. The old interface's
 *  emulation layer can also be removed, along with any client code
 *  which is no longer built.
 *
 */

#ifndef ACES_DISABLE_OLD_IFACE
#define ACES_ENABLE_OLD_IFACE
#endif

#ifndef HDR_DISABLE_OLD_IFACE
#define HDR_ENABLE_OLD_IFACE
#endif

#define ACES_ENABLE_NEW_IMPL

/*
 *  KernelMedia builds both interfaces, regardless of the above setting.
 */

#if defined(KERNELMEDIA_EXPORTS)
#ifndef ACES_ENABLE_OLD_IFACE
#define ACES_ENABLE_OLD_IFACE
#endif
#ifndef HDR_ENABLE_OLD_IFACE
#define HDR_ENABLE_OLD_IFACE
#endif
#endif
