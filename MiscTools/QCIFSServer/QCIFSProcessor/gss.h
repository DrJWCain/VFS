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

//gss.h
//http://tools.ietf.org/html/rfc2743#section-3.1 p.81/82

#pragma once

#include "ASN1DER.h"
#include "spnego.h"

static bool isGSSHeader(const PUCHAR pBuf)
{
  return (kTokGSSAPI == pBuf[0]) && (kOIDSig == *getDEROffset(pBuf));
}

static PUCHAR getGSSWrappedData(const PUCHAR pBuf)
{
  if (isGSSHeader(pBuf))
    return getSPNEGOPayload(getDEROffset(pBuf));
  if (isSPNEGOTokenTarg(pBuf))
    return getSPNEGOPayloadOctets(getDEROffset(pBuf));
  return pBuf;
}