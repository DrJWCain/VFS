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

//spnego.h
// http://tools.ietf.org/html/rfc4178 - The Simple and Protected Generic Security Service Application Program Interface (GSS-API) Negotiation Mechanism [#4]
// [MS-SPNG] - v20080207 [#5]
#pragma once

#include "ASN1DER.h"
#include "gss_constants.h"

static bool isSPNEGOTokenInit(const PUCHAR pSPNEGOPayload)
{
  return kSPNEGOTokInit == pSPNEGOPayload[0];
}

static bool isSPNEGOTokenTarg(const PUCHAR pSPNEGOPayload)
{
  return kSPNEGOTokTarg == pSPNEGOPayload[0];
}

static PUCHAR getSPNEGOmechTypes(const PUCHAR pSPNEGOPayload)
{
  if (isSPNEGOTokenInit(pSPNEGOPayload))
    return getDEROffset(pSPNEGOPayload);
  return NULL;
}

static bool isSeq(const PUCHAR pBuf)
{
  return kSeqSig == pBuf[0];
}

static bool isMechType(const PUCHAR pBuf)
{
  return kSeqElemSig[0] == pBuf[0];
}

static bool isMechToken(const PUCHAR pBuf)
{
  return kSeqElemSig[2] == pBuf[0];
}

static UCHAR mechTypeLength(const PUCHAR pMechTypes)
{
  if (isSeq(pMechTypes))
    return pMechTypes[1];
  return 0;
}

typedef std::pair<PUCHAR, UCHAR> tMechTypeDesc;
typedef std::list<tMechTypeDesc> tMechTypeList;

static PUCHAR getMechTypes(const PUCHAR pBuf, tMechTypeList& mechTypes)
{
  if (isSeq(pBuf))
    return getMechTypes(&pBuf[2], mechTypes);
  else if (isMechType(pBuf))
  {
    mechTypes.push_back(tMechTypeList::value_type(&pBuf[2], pBuf[1]));
    getMechTypes(&pBuf[2] + pBuf[1], mechTypes);
  }
  else if (isMechToken(pBuf))
    return pBuf;
  return NULL;
}

static bool isOctetString(const PUCHAR pBuf)
{
  return (kOctetSig == pBuf[0]);
}

static PUCHAR getSPNEGOPayloadOctets(const PUCHAR pBuf)
{
  if (isOctetString(pBuf))
    return getDEROffset(pBuf);
  if (isSeq(pBuf) || isMechToken(pBuf))
    return getSPNEGOPayloadOctets(getDEROffset(pBuf));
  return getSPNEGOPayloadOctets(getDEROffset(pBuf) + getDERLen(pBuf));
}

static bool isSPNEGO(const PUCHAR pBuf)
{
  return 0 == memcmp(pBuf, SPNEGO_OID, sizeof(SPNEGO_OID));
}

static PUCHAR getSPNEGOPayload(const PUCHAR pBuf)
{
  if (isSPNEGO(pBuf))
    return getSPNEGOPayloadOctets(getDEROffset(pBuf + sizeof(SPNEGO_OID))); //offset from SPNEGO token at beginning of SPNEGO
  return NULL;
}

template <typename T>
static PUCHAR writeField(T* const pBuf, const T signature, const ULONGLONG length)
{
  pBuf[0] = signature;
  return writeDERLen(&pBuf[1], length - (sizeof(T) * (calcDERLenFieldSize(length) + 1))); //+1 for signature
}

static PUCHAR writeSPNEGOInit(const PUCHAR pBuf)
{
  //mech tok
  const ULONGLONG OIDLen = calcDERLenSize(sizeof(NTLMSSP_OID)) + sizeof(kOIDSig);
  const ULONGLONG mechSeqLen = calcDERLenSize(OIDLen) + sizeof(kSeqSig);
  const ULONGLONG mechTokLen = calcDERLenSize(mechSeqLen) + sizeof(kSeqElemSig[0]);
  const ULONGLONG respSeqLen = calcDERLenSize(mechTokLen) + sizeof(kSeqSig);
  const ULONGLONG SPNEGOLen = calcDERLenSize(respSeqLen) + sizeof(kSPNEGOTokInit);
  //GSS
  const ULONG gssHeaderLen = static_cast<ULONG>(calcDERLenSize(sizeof(SPNEGO_OID)) + sizeof(kOIDSig));

  const ULONGLONG gssLen = gssHeaderLen + SPNEGOLen;

  //write buffer
  const PUCHAR pGSSAPIs = writeField(pBuf, kTokGSSAPI, gssLen);
  memcpy(pGSSAPIs, SPNEGO_OID, sizeof(SPNEGO_OID));
  const PUCHAR pRespSeqLen = writeField(pGSSAPIs + sizeof(SPNEGO_OID), kSPNEGOTokInit, SPNEGOLen);
  const PUCHAR pMechTypeSeq = writeField(pRespSeqLen, kSeqSig, respSeqLen);
  const PUCHAR pMechSeq = writeField(pMechTypeSeq, kSeqElemSig[0], mechTokLen);
  const PUCHAR pOID = writeField(pMechSeq, kSeqSig, mechSeqLen);
  const PUCHAR pPayLoad = writeField(pOID, kOIDSig, OIDLen);
  memcpy(pPayLoad, NTLMSSP_OID, sizeof(NTLMSSP_OID));
  return pPayLoad + sizeof(NTLMSSP_OID);
}

static PUCHAR writeSPNEGOChallenge(const PUCHAR pBuf, const ULONGLONG respSize)
{
  //calculate the length of the GSS-API packet up front
  //mech tok len
  const ULONGLONG respOctetLen = calcDERLenSize(respSize) + sizeof(kOctetSig);
  const ULONGLONG respMechTokLen = calcDERLenSize(respOctetLen) + sizeof(kSeqElemSig[2]);
  //mech len
  const ULONGLONG OIDLen = calcDERLenSize(sizeof(NTLMSSP_OID)) + sizeof(kOIDSig);
  const ULONGLONG supportedMechLen = calcDERLenSize(OIDLen) + sizeof(kSeqElemSig[1]);
  //neg result len
  const ULONGLONG negLen = calcDERLenSize(sizeof(UCHAR)) + sizeof(kNegResultSig);
  const ULONGLONG negResult = calcDERLenSize(negLen) + sizeof(kSeqElemSig[0]);
  //sequence len
  const ULONGLONG seqLen = calcDERLenSize(negResult + supportedMechLen + respMechTokLen) + sizeof(kSeqSig);
  //GSS len
  const ULONGLONG gssLen = calcDERLenSize(seqLen) + sizeof(kSPNEGOTokTarg);

  //write the buffer
  const PUCHAR pNegTokenArgSeq = writeField(pBuf, kSPNEGOTokTarg, gssLen);
  //negResult
  const PUCHAR pSeq0 = writeField(pNegTokenArgSeq, kSeqSig, seqLen);
  const PUCHAR pNegResult = writeField(pSeq0, kSeqElemSig[0], negResult);
  const PUCHAR pNegFlag = writeField(pNegResult, kNegResultSig, negLen);
  *pNegFlag = kNegResultIncomplete;
  //supportedMech
  const PUCHAR pSeq1 = pNegFlag + 1;
  const PUCHAR pSupportedMechOID = writeField(pSeq1, kSeqElemSig[1], supportedMechLen);
  const PUCHAR pSupportedMech = writeField(pSupportedMechOID, kOIDSig, OIDLen);
  memcpy(pSupportedMech, NTLMSSP_OID, sizeof(NTLMSSP_OID));
  //mekToken
  const PUCHAR pSeq2 = pSupportedMech + sizeof(NTLMSSP_OID);
  const PUCHAR pOctet = writeField(pSeq2, kSeqElemSig[2], respMechTokLen);
  return writeField(pOctet, kOctetSig, respOctetLen);
}

static PUCHAR writeSPNEGOAccept(const PUCHAR pBuf)
{
  const ULONGLONG negLen = calcDERLenSize(sizeof(UCHAR)) + sizeof(kNegResultSig);
  const ULONGLONG negResult = calcDERLenSize(negLen) + sizeof(kSeqElemSig[0]);
  const ULONGLONG seqLen = calcDERLenSize(negResult) + sizeof(kSeqSig);
  const ULONGLONG gssLen = calcDERLenSize(seqLen) + sizeof(kSPNEGOTokTarg);
  const PUCHAR pNegTokenArgSeq = writeField(pBuf, kSPNEGOTokTarg, gssLen);
  //negResult
  const PUCHAR pSeq0 = writeField(pNegTokenArgSeq, kSeqSig, seqLen);
  const PUCHAR pNegResult = writeField(pSeq0, kSeqElemSig[0], negResult);
  const PUCHAR pNegFlag = writeField(pNegResult, kNegResultSig, negLen);
  *pNegFlag = kNegResultComplete;
  return pNegFlag+1;
}