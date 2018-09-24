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

//ASN1DER.h
// ASN.1 encoding rules: Specification of Basic Encoding Rules
//             (BER), Canonical Encoding Rules (CER) and Distinguished
//             Encoding Rules (DER), ITU-T Recommendation X.690 (1997) |
//            ISO/IEC International Standard 8825-1:1998.
// http://msdn.microsoft.com/en-us/library/ms995330.aspx  -  HTTP-Based Cross-Platform Authentication via the Negotiate Protocol [#6]

//determine offset of payload from identifier in ASN.1 DER encoding

#pragma once

static UCHAR getDERLenSize(const PUCHAR pBuf)
{
  if (0x80 == (0x80 & pBuf[1]))
    return (pBuf[1] & 0xf) + 1;
  return sizeof(UCHAR);
}

static PUCHAR getDEROffset(const PUCHAR pBuf) //[#6] - ASN.1 DER Lengths
{
  return pBuf + sizeof(UCHAR) + getDERLenSize(pBuf);
}

static ULONGLONG getDERLen(const PUCHAR pBuf)
{
  ULONGLONG ret(0);
  const UCHAR derLenSize(getDERLenSize(pBuf));
  if (1 == derLenSize)
    ret = pBuf[1];
  else
  {
    for (int i=derLenSize; i>=0; i--)
      ret |= pBuf[2+derLenSize-i] << (8*i);
  }
  return ret;
}

static PUCHAR writeDERLen(const PUCHAR pBuf, ULONGLONG len)
{
  const PUCHAR pLen = reinterpret_cast<PUCHAR>(&len);
  if (len <= 0x7f)
  {
    pBuf[0] = pLen[0];  //pLen[0];
    return &pBuf[1];
  }

  pBuf[0] = 0x80;
  UCHAR nBytes(0);
  for (UCHAR i=sizeof(ULONGLONG); i>0; i--)
  {
    if (0 == nBytes && pLen[i-1])
      nBytes = i;
    if (0 != nBytes)
      pBuf[nBytes-i+1] = pLen[i-1];
  }

  pBuf[0] |= nBytes;

  return &pBuf[nBytes+1];
}

static ULONGLONG calcDERLenFieldSize(ULONGLONG nBytes)
{
  if (nBytes <= 0x7f)
    return 1;
  const PUCHAR pnBytes = reinterpret_cast<PUCHAR>(&nBytes);
  for (ULONGLONG i=sizeof(ULONGLONG); i>0; i--)
  {
    if (pnBytes[i-1])
      return i + 1; //+1 for 0x8$
  }
  return 0;
}

static ULONGLONG calcDERLenSize(ULONGLONG nBytes)
{
  return nBytes + calcDERLenFieldSize(nBytes);
}