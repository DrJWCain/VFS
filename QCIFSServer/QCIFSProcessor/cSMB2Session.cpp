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

//cSMB2Session.cpp

#include "StdAfx.h"
#include ".\csmb2session.h"
#include "iTreeResourceFactory.h"
#include "cSMB2Tree.h"
#include "..\iTreeResource.h"

#include "Bcrypt.h"

#define NT_SUCCESS(Status)          (((NTSTATUS)(Status)) >= 0)
#define ASSERT(X)
#define NT_VERIFY(x) {NTSTATUS result = x; if(!NT_SUCCESS(result))QTRACE((L"%08x", result)); }
#define SEC_SUCCESS(Status) ((Status) >= 0)

#pragma comment(lib, "Secur32.lib")

//From MSDN Social: The x64 and IA64 versions of bcrypt.lib were included in the Windows SDK starting with Server 2008 (SDK 6.1).
#include "KerrSecurityCryptography.h"
using namespace Kerr::Security::Cryptography;
#pragma comment(lib, "Bcrypt.lib")

using namespace vfs;

void dump(std::vector<unsigned char> data, int lineCount);
void PrintHexDump(DWORD length, PBYTE buffer);

cSMB2Session::cSMB2Session(bool useSMB3Signing)
: m_nTreeCount(0), pInBuf(NULL), pOutBuf(NULL), cbMaxMessage(0), UseSMB3Signing(useSMB3Signing)
{
  memset(&hcred, 0, sizeof(hcred));
  memset(&hctxt, 0, sizeof(hctxt));
  initSecurity();

  //Default settings ...
  //const char sess[] = {0x53, 0x79, 0x73, 0x74, 0x65, 0x6d, 0x4c, 0x69, 0x62, 0x72, 0x61, 0x72, 0x79, 0x44, 0x54, 0x43};
  //SessionKey = std::vector<unsigned char>(&sess[0], &sess[0x10]);
  //SignKey = SessionKey;
}

cSMB2Session::~cSMB2Session(void)
{
  QTRACE((L"cSMB2Session Tidy security"));
  DeleteSecurityContext(&hctxt);
  FreeCredentialsHandle(&hcred);

  if (pInBuf)
    free(pInBuf);

  if (pOutBuf)
    free(pOutBuf);
}

void cSMB2Session::initSecurity()
{
  //QTRACE((L"cSMB2Session Init security"));
  strcpy_s(lpPackageName, 1024, NTLMSP_NAME_A);

  SECURITY_STATUS ss;
  PSecPkgInfoA pkgInfo;

  ss = QuerySecurityPackageInfoA (
    lpPackageName, 
    &pkgInfo);

  if (!SEC_SUCCESS(ss)) 
  {
  }

  cbMaxMessage = pkgInfo->cbMaxToken;
  //QTRACE((L"%08x, %d", pkgInfo->fCapabilities, cbMaxMessage));

  FreeContextBuffer(pkgInfo);

  pInBuf = (PBYTE) malloc (cbMaxMessage);
  pOutBuf = (PBYTE) malloc (cbMaxMessage);
}

namespace
{
  void hashVal(PCWSTR algorithm, void* input, ULONG size, Buffer& hashValue)
  {
      AlgorithmProvider algorithmProvider;

      NT_VERIFY(algorithmProvider.Open(algorithm,
        0, // implementation
        0)); // flags

      Hash hash;    
      NT_VERIFY(hash.Create(algorithmProvider,
        0, // secret
        0, // secret size
        0)); // flags
      NT_VERIFY(hash.HashData(input, 
        size,
        0)); // flags

      ULONG hashValueSize = 0;
      NT_VERIFY(hash.GetHashSize(hashValueSize));

      NT_VERIFY(hashValue.Create(hashValueSize));

      NT_VERIFY(hash.FinishHash(hashValue.GetData(),
        hashValue.GetSize(),
        0)); // flags
  }
  void hashVal(PCWSTR algorithm, const String& value, Buffer& hashValue)
  {  
    QTRACE((L"Calculating hash for %s", value.c_str()));
    hashVal(algorithm, (void*)&value[0], value.size(), hashValue);
    QTRACE((L"DONE Calculating hash for %s", value.c_str()));
  }
  void hashVal(PCWSTR algorithm, Buffer& value, Buffer& hashValue)
  {  
    //QTRACE((L"Calculating hash for value"));
    hashVal(algorithm, (void*)value.GetData(), value.GetSize(), hashValue);
    //QTRACE((L"DONE Calculating hash for value"));
  }

  void kxkey(Buffer& sessionBaseKey, Buffer& bLMChallengeResponse, ULONGLONG serverChallenge, Buffer& keyExchangeKey)
  {
    //keyExchangeKey = HMAC_MD5(sessionBaseKey, serverChallenge + bLMChallengeResponse);
    
    StringStream signDataStr;
    signDataStr << serverChallenge << String(bLMChallengeResponse.m_ptr[0], bLMChallengeResponse.m_ptr[7]);
    String signDataString = signDataStr.str();
    QTRACE((L"Got kxkey signDataStr %s", signDataString.c_str()));
    //std::vector<unsigned char> signData = serverChallenge + bLMChallengeResponse  
    
      AlgorithmProvider algorithmProvider;

      NT_VERIFY(algorithmProvider.Open(BCRYPT_MD5_ALGORITHM,
        0, // implementation
        BCRYPT_ALG_HANDLE_HMAC_FLAG)); // flags

      Hash hash;

      NT_VERIFY(hash.Create(algorithmProvider,
        sessionBaseKey.GetData(), // secret
        sessionBaseKey.GetSize(), // secret size
        0)); // flags

      NT_VERIFY(hash.HashData(signDataString.c_str(), 
        16,
        0)); // flags

      ULONG hashValueSize = 0;
      NT_VERIFY(hash.GetHashSize(hashValueSize));

      NT_VERIFY(keyExchangeKey.Create(hashValueSize));

      NT_VERIFY(hash.FinishHash(keyExchangeKey.GetData(),
        keyExchangeKey.GetSize(),
        0)); // flags
  }
  void rc4k(Buffer& keyExchangeKey, Buffer& encryptedRandomSessionKey, Buffer& exportedSessionKey)
  {
    AlgorithmProvider algorithmProvider;

    NT_VERIFY(algorithmProvider.Open(BCRYPT_RC4_ALGORITHM,
      0, // implementation
      0)); // flags

    Key key;

    NT_VERIFY(key.GenerateSymmetricKey(algorithmProvider,
      keyExchangeKey.GetData(), // secret
      keyExchangeKey.GetSize(), // secret size
      0)); // flags

    UCHAR text[100];
    ULONG sizeCopied = 0;
    NT_VERIFY(key.Decrypt(encryptedRandomSessionKey.GetData(),
      encryptedRandomSessionKey.GetSize(),
      0,
      0,
      0,
      text,
      100,
      sizeCopied,
      0)); // flags
    QTRACE((L"sizeCopied %d", sizeCopied));

    NT_VERIFY(exportedSessionKey.Create(sizeCopied));
    memcpy(exportedSessionKey.GetData(), text, sizeCopied);
  }

  //void rc4k(Buffer& keyExchangeKey, Buffer& encryptedRandomSessionKey, Buffer& exportedSessionKey)
  //{
  //  {
  //    AlgorithmProvider algorithmProvider;

  //    NT_VERIFY(algorithmProvider.Open(BCRYPT_RC4_ALGORITHM,
  //      0, // implementation
  //      0)); // flags

  //    Hash hash;

  //    NT_VERIFY(hash.Create(algorithmProvider,
  //      keyExchangeKey.GetData(), // secret
  //      keyExchangeKey.GetSize(), // secret size
  //      0)); // flags

  //    NT_VERIFY(hash.HashData(encryptedRandomSessionKey.GetData(), 
  //      encryptedRandomSessionKey.GetSize(),
  //      0)); // flags

  //    ULONG hashValueSize = 0;
  //    NT_VERIFY(hash.GetHashSize(hashValueSize));

  //    NT_VERIFY(exportedSessionKey.Create(hashValueSize));

  //    NT_VERIFY(hash.FinishHash(exportedSessionKey.GetData(),
  //      exportedSessionKey.GetSize(),
  //      0)); // flags
  //  }
  //}
  void signKey(NTLM_NEGOTIATE_FLAGS negFlg, Buffer& exportedSessionKey, const String& mode, Buffer& signingKey)
  {
    if (negFlg & NTLMSSP_V2)
    {   
      StringStream hashValueStr;
      if (mode == L"Client")
      {
        // hashValue = exportedSessionKey + L"session key to client-to-server signing key magic constant")
        hashValueStr << String(exportedSessionKey.m_ptr[0], exportedSessionKey.m_ptr[exportedSessionKey.GetSize()]) << L"session key to client-to-server signing key magic constant";
      }
      else
      {
        // hashValue = exportedSessionKey + L"session key to server-to-client signing key magic constant")      
        hashValueStr << String(exportedSessionKey.m_ptr[0], exportedSessionKey.m_ptr[exportedSessionKey.GetSize()]) << L"session key to server-to-client signing key magic constant";
      }
      String hashValueString = hashValueStr.str();
      Buffer hashValue;
      hashValue.m_ptr = (PUCHAR)hashValueString.c_str();
      hashValue.m_size = hashValueString.size();
      QTRACE((L"SignKey for mode %s generates hashValue %s", mode.c_str(), hashValueString.c_str()));
      
      hashVal(BCRYPT_MD5_ALGORITHM, hashValue, signingKey);

      hashValue.m_ptr = 0;
      hashValue.m_size = 0;
    }
  }
};


Buffer HMAC_MD5(Buffer& secret, Buffer& message)
{
  AlgorithmProvider algorithmProvider;
  NT_VERIFY(algorithmProvider.Open(BCRYPT_MD5_ALGORITHM,
    0, // implementation
    BCRYPT_ALG_HANDLE_HMAC_FLAG)); // flags

  Hash hash;
  NT_VERIFY(hash.Create(algorithmProvider,
    secret.GetData(), // secret
    secret.GetSize(), // secret size
    0)); // flags

  NT_VERIFY(hash.HashData(message.GetData(),
    message.GetSize(),
    0)); // flags

  ULONG hashValueSize = 0;
  NT_VERIFY(hash.GetHashSize(hashValueSize));

  Buffer _result;
  NT_VERIFY(_result.Create(hashValueSize));

  NT_VERIFY(hash.FinishHash(_result.GetData(),
    _result.GetSize(),
    0)); // flags

         //QTRACE((L"result ... "));
         //_result.print();
         //QTRACE((L"... result"));

  return _result;
}

Buffer NTOWFv2(Buffer& password, Buffer& user, Buffer& domain)
{

  Buffer md4Psswrd;
  hashVal(BCRYPT_MD4_ALGORITHM, password, md4Psswrd);
  //QTRACE((L"md4Psswrd ... "));
  //md4Psswrd.print();
  //QTRACE((L"... md4Psswrd"));

  Buffer userAndDom;
  NT_VERIFY(userAndDom.Create(user.GetSize() + domain.GetSize()));
  memcpy(userAndDom.GetData(), user.GetData(), user.GetSize());
  memcpy(userAndDom.GetData() + user.GetSize(), domain.GetData(), domain.GetSize());

  //QTRACE((L"userAndDom ... "));
  //PrintHexDump(userAndDom.GetSize(), userAndDom.GetData());
  //QTRACE((L"... userAndDom"));




  return HMAC_MD5(md4Psswrd, userAndDom);
}

Buffer getBufferFromString(const String& str)
{
  Buffer result;
  result.Create(str.size() * 2);
  memcpy(result.GetData(), &str[0], str.size() * 2);
  return result;
}


/*

NTLMv2 Session key calculation

-- Explanation of message fields and variables: 
 --   NegFlg, User, UserDom - Defined in section 3.1.1. 
 --   Passwd - Password of the user. 
 --   LmChallengeResponse - The LM response to the server challenge. 
      Computed by the client.  
 --   NTChallengeResponse - The NT response to the server challenge. 
      Computed  by the client. 
 --   ClientChallenge - The 8-byte challenge message generated by the 
      client.  
 --   CHALLENGE_MESSAGE.ServerChallenge - The 8-byte challenge message 
      generated by the server.  
 --   ResponseKeyNT - Temporary variable to hold the results of 
      calling NTOWF(). 
 --   ResponseKeyLM - Temporary variable to hold the results of 
      calling LMGETKEY. 
 --   ServerName - The  
      NtChallengeResponseFields.NTLMv2_RESPONSE.NTLMv2_CLIENT_CHALLENGE.AvPairs  
      field structure of the AUTHENTICATE_MESSAGE payload. 
 --   KeyExchangeKey - Temporary variable to hold the results of 
      calling KXKEY.  
 --   HiResponserversion - The 1-byte highest response version 
      understood by the client.  Currently set to 1. 
 --   Responserversion - The 1-byte response version. Currently set 
      to 1. 
 --  Time - The 8-byte little-endian time in GMT. 
 -- 
 -- Functions Used: 
 --   Z(M) - Defined in section 6. 
 
 
Define NTOWFv2(Passwd, User, UserDom) as HMAC_MD5(  
 MD4(UNICODE(Passwd)), UNICODE(ConcatenationOf( Uppercase(User),  
 UserDom ) ) ) 

EndDefine 
 
Set ResponseKeyNT to NTOWFv2(Passwd, User, UserDom) 
 
 
Define ComputeResponse(NegFlg, ResponseKeyNT, ResponseKeyLM, 
 CHALLENGE_MESSAGE.ServerChallenge, ClientChallenge, Time, ServerName) 
 As 
 If (User is set to "" && Passwd is set to "") 
     -- Special case for anonymous authentication 
     Set NtChallengeResponseLen to 0 
     Set NtChallengeResponseMaxLen to 0 
     Set NtChallengeResponseBufferOffset to 0 
     Set LmChallengeResponse to Z(1) 
 Else 
     Set temp to ConcatenationOf(Responserversion, HiResponserversion, 
     Z(6), Time, ClientChallenge, Z(4), ServerName, Z(4)) 
     Set NTProofStr to HMAC_MD5(ResponseKeyNT,  
     ConcatenationOf(CHALLENGE_MESSAGE.ServerChallenge,temp)) 
     Set NtChallengeResponse to ConcatenationOf(NTProofStr, temp) 
     Set LmChallengeResponse to ConcatenationOf(HMAC_MD5(ResponseKeyLM,  
     ConcatenationOf(CHALLENGE_MESSAGE.ServerChallenge, ClientChallenge)), 
     ClientChallenge ) 
 EndIf 
 
 
 
Set SessionBaseKey to HMAC_MD5(ResponseKeyNT, NTProofStr) 
 EndDefine 
*/


#include "safeName.h"
Buffer cSMB2Session::calculateSessionKey(const PUCHAR pGSSdata)
{
  //const PNTLM_MSG pNTLMMsg = reinterpret_cast<PNTLM_MSG>(pGSSdata);
  const PNTLM_MSG_AUTHENTICATE pNTLMMsgAuth = reinterpret_cast<PNTLM_MSG_AUTHENTICATE>(pGSSdata);
  bool NTLMv2 = pNTLMMsgAuth->Flags & NTLMSSP_V2;
  QTRACE((L"*****calculateSessionKey**** Flags: %08x. NTLMv2 %d", pNTLMMsgAuth->Flags, NTLMv2));

  String password = L"";
  if((pNTLMMsgAuth->UserName.NameLen > 0) && (pNTLMMsgAuth->NtChallengeResponse.NameLen > 0))
  {
    password = L"blunder1";
  }

  QTRACE((L"NTOWFv2(\"Password\", \"User\", \"Domain\") is   0c 86 8a 40 3b fd 7a 93 a3 00 1e f2 2e f0 2e 3f"));
  String _password = L"Password";
  String _user = toUpper(L"User");
  String _domain = L"Domain";
  Buffer test = NTOWFv2(getBufferFromString(_password), getBufferFromString(_user), getBufferFromString(_domain));
  test.print();

  //Buffer temp = createTemp(pNTLMMsgAuth);
  QTRACE((L"mTargetInfo %d", mTargetInfo.size()));
  PrintHexDump(mTargetInfo.size(), &mTargetInfo[0]);

  QTRACE((L"mLmChallengeResponse %d", mLmChallengeResponse.size()));
  PrintHexDump(mLmChallengeResponse.size(), &mLmChallengeResponse[0]);

  QTRACE((L"mEncryptedRandomSessionKey %d", mEncryptedRandomSessionKey.size()));
  PrintHexDump(mEncryptedRandomSessionKey.size(), &mEncryptedRandomSessionKey[0]);
  Buffer _EncryptedRandomSessionKey;
  _EncryptedRandomSessionKey.Create(mEncryptedRandomSessionKey.size());
  memcpy(_EncryptedRandomSessionKey.GetData(), &mEncryptedRandomSessionKey[0], mEncryptedRandomSessionKey.size());

  Buffer ResponseKeyNT = NTOWFv2(getBufferFromString(password), getBufferFromString(toUpper(m_sLoginName)), getBufferFromString(mDomain));
  QTRACE((L"ResponseKeyNT ... "));
  ResponseKeyNT.print();

  UCHAR Response[16];
  memset(Response, 0, 16);
  QTRACE((L"mNtChallengeResponse %d", mNtChallengeResponse.size()));
  if(mNtChallengeResponse.size())
  {

    PrintHexDump(mNtChallengeResponse.size(), &mNtChallengeResponse[0]);
    PNTLMv2_RESPONSE pChallenge = (PNTLMv2_RESPONSE)&mNtChallengeResponse[0];
    PrintHexDump(8, (PBYTE)&pChallenge->Challenge);
    memcpy(Response, pChallenge->Response, 16);

    PUCHAR pAvPairs = &mNtChallengeResponse[sizeof(NTLMv2_RESPONSE)];
    PUCHAR pAvPairsStart = pAvPairs;
    PAV_PAIR _pAvPairs = (PAV_PAIR)pAvPairs;
    while(_pAvPairs->Length)
    {
      QTRACE((L"%d %d", _pAvPairs->ID, _pAvPairs->Length));
      pAvPairs += _pAvPairs->Length + sizeof(AV_PAIR);
      _pAvPairs = (PAV_PAIR)pAvPairs;
    }
    QTRACE((L"%d %d", _pAvPairs->ID, _pAvPairs->Length));
    PUCHAR pAvPairsEnd = pAvPairs + _pAvPairs->Length + sizeof(AV_PAIR);

    Buffer temp;
    temp.Create(pAvPairsEnd - &pChallenge->RespType);
    memcpy(temp.GetData(), &pChallenge->RespType, pAvPairsEnd - &pChallenge->RespType);
    QTRACE((L"temp"));
    PrintHexDump(temp.GetSize(), temp.GetData());

    Buffer NTProofStr = HMAC_MD5(ResponseKeyNT, temp);
    QTRACE((L"NTProofStr ... "));
    NTProofStr.print();

    Buffer SessionBaseKey = HMAC_MD5(ResponseKeyNT, NTProofStr);
    QTRACE((L"SessionBaseKey ... "));
    SessionBaseKey.print();

    Buffer exportedSessionKey;
    rc4k(SessionBaseKey, _EncryptedRandomSessionKey, exportedSessionKey);
    QTRACE((L"exportedSessionKey ... "));
    exportedSessionKey.print();
  }

  Buffer NTProofStr2;
  NTProofStr2.Create(16);
  memcpy(NTProofStr2.GetData(), Response, 16);
  QTRACE((L"NTProofStr2 ... "));
  NTProofStr2.print();

  Buffer SessionBaseKey2 = HMAC_MD5(ResponseKeyNT, NTProofStr2);
  QTRACE((L"SessionBaseKey2 ... "));
  SessionBaseKey2.print();

  Buffer exportedSessionKey2;
  rc4k(SessionBaseKey2, _EncryptedRandomSessionKey, exportedSessionKey2);
  QTRACE((L"exportedSessionKey2 ... "));
  exportedSessionKey2.print();


  QTRACE((L"Done calculating sessionkey"));

  return exportedSessionKey2;
}

void cSMB2Session::setChallenge(ULONGLONG challenge)
{
  mChallenge = challenge;
}

void cSMB2Session::setDomainAndWorkstation(const String& domain, const String& workstation)
{
  if(!domain.empty())
    mDomain = domain;
  if(!workstation.empty())
    mWorkstation = workstation;
}

void cSMB2Session::setTargetInfo(DWORD length, PBYTE buffer)
{
  if(length)
  {
    mTargetInfo = std::vector<unsigned char>(&buffer[0], &buffer[length]);
  }
}

void cSMB2Session::setLmChallengeResponse(DWORD length, PBYTE buffer)
{
  if(length)
  {
    mLmChallengeResponse = std::vector<unsigned char>(&buffer[0], &buffer[length]);
  }
}

void cSMB2Session::setNtChallengeResponse(DWORD length, PBYTE buffer)
{
  if(length)
  {
    mNtChallengeResponse = std::vector<unsigned char>(&buffer[0], &buffer[length]);
  }
}


void cSMB2Session::setEncryptedRandomSessionKey(DWORD length, PBYTE buffer)
{
  if(length)
  {
    mEncryptedRandomSessionKey = std::vector<unsigned char>(&buffer[0], &buffer[length]);
  }
}

BOOL cSMB2Session::GenServerContext (
                       BYTE *pIn,
                       DWORD cbIn,
                       BYTE *pOut,
                       DWORD *pcbOut,
                       BOOL *pfDone,
                       BOOL fNewConversation)
{
  SECURITY_STATUS   ss;
  TimeStamp         Lifetime;
  SecBufferDesc     OutBuffDesc;
  SecBuffer         OutSecBuff;
  SecBufferDesc     InBuffDesc;
  SecBuffer         InSecBuff;
  ULONG             Attribs = 0;

  //----------------------------------------------------------------
  //  Prepare output buffers.

  OutBuffDesc.ulVersion = 0;
  OutBuffDesc.cBuffers = 1;
  OutBuffDesc.pBuffers = &OutSecBuff;

  OutSecBuff.cbBuffer = *pcbOut;
  OutSecBuff.BufferType = SECBUFFER_TOKEN;
  OutSecBuff.pvBuffer = pOut;

  //----------------------------------------------------------------
  //  Prepare input buffers.

  InBuffDesc.ulVersion = 0;
  InBuffDesc.cBuffers = 1;
  InBuffDesc.pBuffers = &InSecBuff;

  InSecBuff.cbBuffer = cbIn;
  InSecBuff.BufferType = SECBUFFER_TOKEN;
  InSecBuff.pvBuffer = pIn;

  //QTRACE((L"Token buffer received (%lu bytes):\n", InSecBuff.cbBuffer));
  //PrintHexDump (InSecBuff.cbBuffer, (PBYTE)InSecBuff.pvBuffer);

  ss = AcceptSecurityContext (
    &hcred,
    fNewConversation ? NULL : &hctxt,
    &InBuffDesc,
    Attribs, 
    SECURITY_NATIVE_DREP,
    &hctxt,
    &OutBuffDesc,
    &Attribs,
    &Lifetime);

  if (!SEC_SUCCESS (ss))  
  {
    //Some of the errors we get here during session channle binding ...
    //SEC_E_INVALID_HANDLE 0x80090301
    //SEC_E_INVALID_TOKEN 0x80090308
    //SEC_E_LOGON_DENIED 0x8009030c
    //SEC_E_OUT_OF_SEQUENCE 0x80090310 
    QSOS((L"AcceptSecurityContext failed: 0x%08x\n", ss));
    throw cAccessDeniedErrorValue(__FILE__, __LINE__, ss, L"AcceptSecurityContext failed: 0x%08x\n", ss);
  }
  else
  {
    QTRACE((L"AcceptSecurityContext success: 0x%08x\n", ss));
  }

  //----------------------------------------------------------------
  //  Complete token if applicable.

  if ((SEC_I_COMPLETE_NEEDED == ss) 
    || (SEC_I_COMPLETE_AND_CONTINUE == ss))  
  {
    ss = CompleteAuthToken (&hctxt, &OutBuffDesc);
    if (!SEC_SUCCESS(ss))  
    {
      QSOS((L"CompleteAuthToken failed: 0x%08x\n", ss));
      throw cAccessDeniedErrorValue(__FILE__, __LINE__, ss, L"CompleteAuthToken failed: 0x%08x\n", ss);
    }
  }

  *pcbOut = OutSecBuff.cbBuffer;

  //QTRACE((L"Token buffer generated (%lu bytes):\n", 
  //  OutSecBuff.cbBuffer));
  //PrintHexDump (
  //  OutSecBuff.cbBuffer, 
  //  (PBYTE)OutSecBuff.pvBuffer);

  *pfDone = !((SEC_I_CONTINUE_NEEDED == ss) 
    || (SEC_I_COMPLETE_AND_CONTINUE == ss));

  //QTRACE((L"AcceptSecurityContext result = 0x%08x\n", ss));

  return TRUE;

}  // end GenServerContext


std::vector<unsigned char> calcSMB3SignKey(std::vector<unsigned char> sessionKey)
{
  std::vector<unsigned char> signData;

  //Make signData for SignKey derivation.
  { //References:
    //3.2.5.3.3 in MS-SMB2.pdf
    std::string label = "SMB2AESCMAC";
    std::string context = "SmbSign";

    //3.1.4.2 in MS-SMB2.pdf: KDF in Counter Mode, 'r' of 32, 'L' of 128, PRF HAMC-SHA256.
    //NIST SP800-108 section 5.1
    unsigned char val1[4] = {0,0,0,1};   //unsigned int 1;
    unsigned char val2[1] = {0};         //unsigned char 0;
    unsigned char val3[4] = {0,0,0,128}; //unsigned int 128;

    signData.insert(signData.end(), &val1[0], &val1[4]); //unsigned int 1; [i]2, where i = n, where n = [L/h], = 1. h = 128, L = 128.

    signData.insert(signData.end(), &label[0], &label[label.size()]); //label
    signData.insert(signData.end(), &val2[0], &val2[1]); //Nul terminator for label

    signData.insert(signData.end(), &val2[0], &val2[1]); //Separator

    signData.insert(signData.end(), &context[0], &context[context.size()]); //context
    signData.insert(signData.end(), &val2[0], &val2[1]); //Nul terminator for context

    signData.insert(signData.end(), &val3[0], &val3[4]); //unsigned int 128; [L]2, where L =128
  }

  //PrintHexDump(signData.size(), &signData[0]);

  Buffer hashValue;
  {
    AlgorithmProvider algorithmProvider;

    NT_VERIFY(algorithmProvider.Open(BCRYPT_SHA256_ALGORITHM,
      0, // implementation
      BCRYPT_ALG_HANDLE_HMAC_FLAG)); // flags

    Hash hash;

    NT_VERIFY(hash.Create(algorithmProvider,
      &sessionKey[0], // secret
      sessionKey.size(), // secret size
      0)); // flags

    NT_VERIFY(hash.HashData(&signData[0], 
      signData.size(),
      0)); // flags

    ULONG hashValueSize = 0;
    NT_VERIFY(hash.GetHashSize(hashValueSize));

    NT_VERIFY(hashValue.Create(hashValueSize));

    NT_VERIFY(hash.FinishHash(hashValue.GetData(),
      hashValue.GetSize(),
      0)); // flags
  }

  QTRACE((L"SIGN KEY:"));
  hashValue.print();
  QTRACE((L"->"));

  std::vector<unsigned char> ret(16);

  memcpy(&ret[0], hashValue.m_ptr, 16);

  return ret;
}

#define BCRYPT_AES_CMAC_ALGORITHM  L"AES-CMAC" //For SMB 3.0 - Windows 8 & 2k12 only
Buffer getHash(bool useSMB3Signing, PBYTE pDataBuffer, DWORD dataLen, std::vector<unsigned char> key)
{
  Buffer hashValue;

  {
    AlgorithmProvider algorithmProvider;

    if(useSMB3Signing)
    {
      NTSTATUS st1 = 
      algorithmProvider.Open(BCRYPT_AES_CMAC_ALGORITHM,
        0, // implementation
        0); // flags
    QTRACE((L"hash open st %d", st1));
      NT_VERIFY(st1);
    }
    else
    {
      NT_VERIFY(algorithmProvider.Open(BCRYPT_SHA256_ALGORITHM,
        0, // implementation
        BCRYPT_ALG_HANDLE_HMAC_FLAG)); // flags
    }

    Hash hash;

    NTSTATUS st = 
    hash.Create(algorithmProvider,
      &key[0], // secret
      key.size(), // secret size
      0); // flags
    //QTRACE((L"hash create st %d", st));
    NT_VERIFY(st);

    st = hash.HashData(pDataBuffer, 
      dataLen,
      0); // flags
    //QTRACE((L"hash data st %d", st));
    NT_VERIFY(st);

    ULONG hashValueSize = 0;
    NT_VERIFY(hash.GetHashSize(hashValueSize));
    //QTRACE((L"hashValueSize %d", hashValueSize));

    NT_VERIFY(hashValue.Create(hashValueSize));

    NT_VERIFY(hash.FinishHash(hashValue.GetData(),
      hashValue.GetSize(),
      0)); // flags
  }

  return hashValue;
}

PBYTE cSMB2Session::getNTLMServerResponce(bool fNewConversation, const PUCHAR pGSSdata, DWORD cbIn, DWORD& cbOut)
{
  BOOL done =   FALSE;

  if(fNewConversation)
  {
    TimeStamp         Lifetime;
    SECURITY_STATUS ss = AcquireCredentialsHandleA (
      NULL, 
      lpPackageName,
      SECPKG_CRED_INBOUND,
      NULL, 
      NULL, 
      NULL, 
      NULL, 
      &hcred,
      &Lifetime);

    if (!SEC_SUCCESS (ss))
    {
      QSOS((L"AcquireCredentialsHandleA failed - 0x%08x\n", ss));
    }
    else
    {
      QTRACE((L"AcquireCredentialsHandleA success."));
    }
  }

  cbOut = cbMaxMessage;
  memcpy(pInBuf, pGSSdata, cbIn);

  if (!GenServerContext (
    pInBuf, 
    cbIn, 
    pOutBuf, 
    &cbOut, 
    &done,
    fNewConversation))
  {
    QSOS((L"GenServerContext failed.\n"));
  }

  if(done)
  {
    SecPkgContext_SessionKey sessionKey;
    Buffer calculatedSessionKey = calculateSessionKey(pGSSdata);
    //QTRACE((L"Got back here"));
    // sessionKey = Dec_RC4K(pGSSdata->EncryptedRandomSessionKey, KeyExchangeKey);
    SECURITY_STATUS ss = QueryContextAttributesA(&hctxt, SECPKG_ATTR_SESSION_KEY, (void*)&sessionKey);
    if(SEC_SUCCESS (ss))
    {
      QTRACE((L"NTLM Accepted"));
      //QTRACE((L"Session Key (%d):", sessionKey.SessionKeyLength));
      //PrintHexDump(sessionKey.SessionKeyLength, sessionKey.SessionKey);
      SessionKey = std::vector<unsigned char>(&sessionKey.SessionKey[0], &sessionKey.SessionKey[sessionKey.SessionKeyLength]);
      QTRACE((L"SECPKG_ATTR_SESSION_KEY & calculatedSessionKey"));
      PrintHexDump(sessionKey.SessionKeyLength, sessionKey.SessionKey);
      PrintHexDump(calculatedSessionKey.GetSize(), calculatedSessionKey.GetData());
      QTRACE((L"-->"));
      //SessionKey[0] = 0;
      if(UseSMB3Signing)
      {
        QTRACE((L"SMB3 - Sign Key:"));
        SignKey = ::calcSMB3SignKey(SessionKey);
      }
      else
      {
        //SMB 2.1 & previous - Sign Key same as Session Key
        SignKey = SessionKey;
      }

    }
    else
    {
      QSOS((L"QueryContextAttributes FAILED %s", ss));
    }

    SecPkgContext_KeyInfo keyInfo;
    ss = QueryContextAttributesA(&hctxt, SECPKG_ATTR_KEY_INFO, (void*)&keyInfo);
    if(SEC_SUCCESS (ss))
    {
      //QTRACE((L"sign: %S crypt:%S KeySize:%d", keyInfo.sSignatureAlgorithmName, keyInfo.sEncryptAlgorithmName, keyInfo.KeySize));
    }
    
    return 0;
  }

  return pOutBuf;
}

PBYTE cSMB2Session::VerifyThis(PBYTE pDataBuffer, DWORD dataLen, PBYTE pSigBuffer, DWORD sigLen)
{
  if(SignKey.empty() && PrevousSignKey.empty())
  {
    QSOS((L"Cannot cSMB2Session::VerifyThis - null SignKeys!!"));
    return 0;
  }

  std::vector<unsigned char> temp;
  if (!PrevousSignKey.empty())
  {
    QTRACE((L"VERIFYING WITH PREVIOUS SIGN KEY"));
    temp = PrevousSignKey;
  }
  else
  {
    QTRACE((L"VERIFYING WITH SIGN KEY"));
    temp = SignKey;
  }

  Buffer hashValue = getHash(UseSMB3Signing, pDataBuffer, dataLen, temp);//(!SignKey.empty())?SignKey:PrevousSignKey);

  //QTRACE((L"HASH:"));
  //hashValue.print();

  //QTRACE((L"SIG:"));
  //PrintHexDump(sigLen, pSigBuffer);

  return pDataBuffer;
}


PBYTE cSMB2Session::SignThis(PBYTE pDataBuffer, DWORD dataLen, PBYTE pSigBuffer, DWORD sigLen)
{
  if(SignKey.empty() && PrevousSignKey.empty())
  {
    QSOS((L"Cannot cSMB2Session::SignThis - null SignKeys!!"));
    return 0;
  }
  std::vector<unsigned char> temp;
  if (!PrevousSignKey.empty())
  {
    QTRACE((L"SIGNING WITH PREVIOUS SIGN KEY"));
    temp = PrevousSignKey;
  }
  else
  {
    //QTRACE((L"SIGNING WITH SIGN KEY"));
    temp = SignKey;
  }
  Buffer hashValue = getHash(UseSMB3Signing, pDataBuffer, dataLen, temp);//(!PrevousSignKey.empty())?PrevousSignKey:SignKey);

  //QTRACE((L"HASH: %d", hashValue.GetSize()));
  //hashValue.print();
  QTRACE((L"->"));

  memcpy(pSigBuffer, hashValue.m_ptr, sigLen);

  return pSigBuffer;
}


void cSMB2Session::replaceTree(const String& sShare
  , const cPtr<iTreeResourceFactory>& pTreeResourceFactory)
{
  if(MasterSession.isValid())
  {
    MasterSession->replaceTree(sShare, pTreeResourceFactory);
    return;
  }
  QTRACE((L"cSMB2Session::replaceTree() - %s", sShare.c_str()));
  cLockGuard lg(&m_TreeAccess);

  for (tTreeMap::iterator cit = m_treeMap.begin()
    ; m_treeMap.end() != cit
    ; ++cit)
  {
    const cPtr<cSMB2Tree>& pTree = cit->second;
    if (pTree->NameIs(sShare))
    {
      const cPtr<cSMB2Tree> pOldTree = cit->second;
      const cPtr<iTreeResource> pNewTreeResource = pTreeResourceFactory->getTreeResource(m_sLoginName);
      const cPtr<cSMB2Tree> pNewSMBTree = new cSMB2Tree(pNewTreeResource, m_sLoginName, sShare);
      cit->second = pNewSMBTree;
      if (pOldTree.isValid())
        pOldTree->notifyRemoved(pNewSMBTree);
    }
  }
}

// connects a tree and returns the ID of its connection
ULONG cSMB2Session::ConnectTree(const String& sShare
  , const cPtr<cSMB2Response>& response
  , const cPtr<iTreeResourceFactory> pTreeResourceFactory)
{
  if(MasterSession.isValid())
  {
    return MasterSession->ConnectTree(sShare, response, pTreeResourceFactory);
  }
  const cPtr<iTreeResource> pTreeResource = pTreeResourceFactory->getTreeResource(m_sLoginName);
  if (pTreeResource.isNull())
    return 0;

  // when count wraps back round, don't return 0
  InterlockedCompareExchange(&m_nTreeCount, 2, LONG_MAX);

  const LONG Tid = InterlockedIncrement(&m_nTreeCount);

  QTRACE((L"cSMB2Session::ConnectTree() - %d", Tid));

  try
  {
    const cPtr<cSMB2Tree> pTree = new cSMB2Tree(pTreeResource, m_sLoginName, sShare);
    cLockGuard lg(&m_TreeAccess);
    m_treeMap.insert(tTreeMap::value_type(Tid, pTree));
  }
  catch (const bool &)
  {
    QSOS((L"failed to connect tree"));
  }

  return Tid;
}

void cSMB2Session::DisconnectTree(const ULONG Tid)
{
  if(MasterSession.isValid())
  {
    MasterSession->DisconnectTree(Tid);
    return;
  }
  QTRACE((L"DisconnectTree [%d]", Tid));
  cLockGuard lg(&m_TreeAccess);
  m_treeMap.erase(Tid);
}

cPtr<cSMB2Tree> cSMB2Session::getTree(const ULONG Tid) const
{
  if(MasterSession.isValid())
  {
    return MasterSession->getTree(Tid);
  }
  {
    cLockGuard lg(&m_TreeAccess);
    const tTreeMap::const_iterator finder = m_treeMap.find(Tid);
    if (m_treeMap.end() != finder)
      return finder->second;
  }

  return cPtr<cSMB2Tree>();
}

void cSMB2Session::CancelNotifications(const ULONGLONG& messageId, bool synchronous)
{
  if(MasterSession.isValid())
  {
    MasterSession->CancelNotifications(messageId, synchronous);
    return;
  }
  cLockGuard lg(&m_TreeAccess);
  for(tTreeMap::const_iterator it = m_treeMap.begin(); it != m_treeMap.end(); ++it)
  {
    it->second->CancelNotifications(messageId, synchronous);
  }
}

void cSMB2Session::addChannelKey(cPtr<cSMB2Session> previousSession)
{
  MasterSession = previousSession;
  while(MasterSession->MasterSession.isValid())
  {
    MasterSession = MasterSession->MasterSession;
  }
  PrevousSignKey = MasterSession->SignKey;
}

void cSMB2Session::clearChannelKey()
{
  PrevousSignKey.clear();
}