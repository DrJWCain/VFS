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


class Base64Coder {
  // Internal bucket class.
  class TempBucket {
  public:
    BYTE    nData[4];
    BYTE    nSize;
    void    Clear() { ::ZeroMemory(nData, 4); nSize = 0; };
    };

  PBYTE         m_pDBuffer;
  PBYTE         m_pEBuffer;
  DWORD         m_nDBufLen;
  DWORD         m_nEBufLen;
  DWORD         m_nDDataLen;
  DWORD         m_nEDataLen;
  bool          m_useAlternateMapping;
  LONG          m_nDecodedLength;

public:
  Base64Coder(bool useAlternateMapping=false);
  ~Base64Coder();

public:
  void    Encode (const PBYTE, DWORD);
  void    Decode (const PBYTE, DWORD);
  void    Encode (const char* sMessage);
  void    Decode (const char* sMessage);

  unsigned char* DecodedMessage() const;
  unsigned char* EncodedMessage() const;
  unsigned int EncodedLength() const;
  unsigned int DecodedLength() const;

  void    AllocEncode (DWORD);
  void    AllocDecode (DWORD);
  void    SetEncodeBuffer (const PBYTE pBuffer, DWORD nBufLen);
  void    SetDecodeBuffer (const PBYTE pBuffer, DWORD nBufLen);

protected:
  void    _EncodeToBuffer (const TempBucket &Decode, PBYTE pBuffer);
  ULONG   _DecodeToBuffer (const TempBucket &Decode, PBYTE pBuffer);
  void    _EncodeRaw (TempBucket &, const TempBucket &);
  void    _DecodeRaw (TempBucket &, const TempBucket &);
  BOOL    _IsBadMimeChar (BYTE);

  static  char    m_DecodeTable[256];
  static  BOOL    m_Init;
  void          _Init();
};

