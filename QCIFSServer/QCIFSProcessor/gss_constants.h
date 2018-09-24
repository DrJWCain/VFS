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

//gss_constants.h

#pragma once

static const UCHAR kTokGSSAPI = 0x60;

//signatures
static const UCHAR kSPNEGOTokInit = 0xa0;
static const UCHAR kSPNEGOTokTarg = 0xa1;
static const UCHAR kSeqElemSig[4] = {0xa0, 0xa1, 0xa2, 0xa3};
static const UCHAR kOctetSig = 0x4;
static const UCHAR kOIDSig = 0x06;
static const UCHAR kNegResultSig = 0x0a;
static const UCHAR kSeqSig = 0x30;

//constants
static const UCHAR kNegResultComplete = 0;
static const UCHAR kNegResultIncomplete = 1;
static const UCHAR kNegResultRejected = 2;

static const UCHAR SPNEGO_OID[8] = {0x06, 0x06, 0x2b, 0x06, 0x01, 0x05, 0x05, 0x02};
static const UCHAR NTLMSSP_OID[10] = {0x2b, 0x06, 0x01, 0x04, 0x01, 0x82, 0x37, 0x02, 0x02, 0x0a};