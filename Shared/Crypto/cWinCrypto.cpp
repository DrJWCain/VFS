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

#include "stdafx.h"
#include "cWinCrypto.h"
#include "../Lexical/Base64Coder.h"

#include <wincrypt.h>      // CryptoAPI definitions
#include <winsock2.h>
#include <sddl.h>

#include <time.h>

static const bool kUseAlternateMapping(true);
vfs::String kSignature(L"{B62E21E9-97BB-41db-8966-D3FC8DEFD51A}");

namespace
{
  //{{{
  void OutputDebugMessage (const wchar_t * const format, ...)
  {
    // Validate the parameters and throw an exception if one fails...
    if (format == 0)
      return;
    // ...so we will never get here if one of the above fails.
    va_list ap;
    va_start(ap, format);
    const int kBufferSize (1024);
    wchar_t Buffer[kBufferSize];
    if (_vsnwprintf(Buffer, kBufferSize - 1, format, ap) == kBufferSize - 1)
    {
      Buffer[kBufferSize - 1] = L'\0';
    }
    OutputDebugString (Buffer);
    wprintf(L"%s", Buffer);
    va_end(ap);
  }
  //}}}
  //{{{
  void DisplayLastError (LPWSTR lpszText, DWORD error=0)
  {
    DWORD dwError (0);
    if (error == 0)
      dwError = GetLastError();
    LPVOID lpMessageBuffer = NULL;

    if (!FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                         NULL, dwError, LANG_NEUTRAL, (LPTSTR)&lpMessageBuffer, 0, NULL))
    {
      OutputDebugMessage(L"%s because 0x%x : FormatMessage failed (%d)\n", lpszText, dwError, GetLastError());
    }
    else
    {
      OutputDebugMessage(L"%s because 0x%x : %s\n", lpszText, dwError, lpMessageBuffer);
    }

    if (lpMessageBuffer)
        LocalFree ((HLOCAL)lpMessageBuffer);
  }
  //}}}
  //{{{
  std::string machineName ()
  {
    WSADATA  wsaData;
    char     szHostname[100];
    HOSTENT *pHostEnt;
    struct   sockaddr_in sAddr;
    //{{{
    //struct   in_addr {
        //union   {
             //struct{
                 //unsigned  char   s_b1,
                                  //s_b2,
                                  //s_b3,
                                  //s_b4;
            //}  S_un_b;
                 //struct  {
                 //unsigned  short  s_w1,
                                  //s_w2;
                  //}  S_un_w;
                   //unsigned long  S_addr;
         //} S_un;
    //};
    //}}}

    std::string result("31turnpike"); //default in case things go wrong

    unsigned int nAdapter=0;
    if (WSAStartup(0x0101, &wsaData))
    {
      return result;
    }

    gethostname (szHostname, sizeof(szHostname));
    pHostEnt = gethostbyname (szHostname);

    if (pHostEnt->h_addr_list[nAdapter])
    {
      // pHostEnt->h_addr_list[nAdapter] is the current address in host
      // order.

      // Copy the address information from the pHostEnt to a sockaddr_in
      // structure.
      ::memcpy (&sAddr.sin_addr.s_addr, pHostEnt->h_addr_list[nAdapter], pHostEnt->h_length);

      // machine's IP Address
      result = std::string(pHostEnt->h_name);
    }

    WSACleanup ();
    return result;
  }
  //}}}
  //{{{
  std::wstring GetUserSIDFromPID(DWORD dwProcessID)
  {
    std::wstring result;
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcessID);
    if(hProcess)
    {
      HANDLE token = NULL;
      if (!OpenProcessToken(hProcess, TOKEN_QUERY, &token))
      {
        printf("OpenProcessToken failed. GetLastError=%d", GetLastError());
      }
      else
      {
        DWORD iTokenInfLength = 0;
        if(!GetTokenInformation(token, TokenUser, 0, iTokenInfLength, &iTokenInfLength))
        {
          if(GetLastError() != ERROR_INSUFFICIENT_BUFFER)
          {
            printf("GetTokenInformation failed. GetLastError=%d", GetLastError());
          }
          else
          {
            PVOID pTokenInfo = new char[iTokenInfLength];
            if(!GetTokenInformation(token, TokenUser, pTokenInfo, iTokenInfLength, &iTokenInfLength))
            {
              printf("GetTokenInformation failed. GetLastError=%d", GetLastError());
              CloseHandle(token);
              token = NULL;
            }
            else
            {
              CloseHandle(token);
              token = NULL;
              TOKEN_USER* pTokenUser = (TOKEN_USER*)pTokenInfo;
              LPWSTR pstrSid = NULL;
              ConvertSidToStringSidW(pTokenUser->User.Sid, &pstrSid);
              if(pstrSid != NULL)
              {
                result = pstrSid;
              }
              LocalFree(pstrSid);
            }
            delete pTokenInfo;
          }
        }
        CloseHandle(token);
        token = NULL;
      }
      CloseHandle(hProcess);
    }
    return result;
  }
  //}}}
  //{{{
  __int64 getNumberRoughlyBasedOnDate()
  {
      struct tm newtime;
      __time64_t long_time;
      errno_t err;

      // Get time as 64-bit integer.
      _time64( &long_time );
      // Convert to local time.
      err = _localtime64_s( &newtime, &long_time );
      if (err)
      {
        return 1103515245; //what else can we do?
      }

      // multipliers are all prime just to mix things up a bit.
      __int64 timeNow = (newtime.tm_year + 1900) * 4363 + newtime.tm_mon * 3221 + newtime.tm_mday * 2269
        +  newtime.tm_wday * 1667 + newtime.tm_yday * 7879;
      return timeNow;
  }
  //}}}
};

//{{{
class cWinCrypto
{
  public:
    cWinCrypto(const std::string& keystring); //will throw if initialisation fails
    ~cWinCrypto();

    bool encrypt (
      BYTE* pData,                      //pointer to plaintext, owned by caller
      unsigned long dataLen,            //length of plaintext
      BYTE*& pCipherData,               //pointer to ciphertext, owner by caller
      unsigned long& cipherDataLen);    //length of ciphertext

    bool decrypt (BYTE* pData, unsigned long& dataLen); //the ciphertext is overwritten by the plaintext


    bool encrypt (vfs::cPtr<vfs::cMemory> plaindata,        //pointer to plaintext, owned by caller
      vfs::cPtr<vfs::cMemory>& cipherData);                 //pointer to ciphertext, owner by caller

    bool decrypt (vfs::cPtr<vfs::cMemory> pData);

  private:
    HCRYPTPROV hProv;
    HCRYPTHASH hHash;
    HCRYPTKEY hKey;
    std::string mKeystring;
};
//}}}

int counter(1);

//{{{
cWinCrypto::cWinCrypto(const std::string& keystring) : mKeystring(keystring)
{
  BOOL bResult;
  //printf("instance %d\n", counter++);

  // Attempt to acquire a handle to the default key container.
  bResult = CryptAcquireContext(
            &hProv,            // Variable to hold returned handle.
            NULL,              // Use default key container.
            MS_DEF_PROV,       // Use default CSP.
            PROV_RSA_FULL,     // Type of provider to acquire.
            0);
  if (!bResult)
    throw vfs::cNotAvailable(__FILE__,__LINE__,L"failed to initialise cryptography system-1");

            // Obtain handle to hash object.
  bResult = CryptCreateHash(
            hProv,             // Handle to CSP obtained earlier
            CALG_MD5,          // Hashing algorithm
            0,                 // Non-keyed hash
            0,                 // Should be zero
            &hHash);           // Variable to hold hash object handle
  if (!bResult)
    throw vfs::cNotAvailable(__FILE__,__LINE__,L"failed to initialise cryptography system-2");

  // Hash data, input:password, output:hashed-password
  bResult = CryptHashData(
            hHash,             // Handle to hash object
            (const BYTE*)mKeystring.c_str(),         // Pointer to password
            mKeystring.length(),  // Length of data
            0);                // No special flags
  if (!bResult)
    throw vfs::cNotAvailable(__FILE__,__LINE__,L"failed to initialise cryptography system-3");


  // Create key from specified password.
  bResult = CryptDeriveKey(
            hProv,               // Handle to CSP obtained earlier.
            CALG_RC4,            // Use a stream cipher.
            hHash,               // Handle to hashed password.
            CRYPT_EXPORTABLE,    // Make key exportable.
            &hKey);              // Variable to hold handle of key.
  if (!bResult)
    throw vfs::cNotAvailable(__FILE__,__LINE__,L"failed to initialise cryptography system-4");

}
//}}}
//{{{
cWinCrypto::~cWinCrypto()
{
  // Release hash object.
  CryptDestroyHash(hHash);

  // Release key object.
  CryptDestroyKey(hKey);

  // Release handle to container.
  CryptReleaseContext(hProv, 0);
}
//}}}

const bool NullEncryption(false);

//{{{
bool cWinCrypto::encrypt (
  PBYTE pData,                    //pointer to plaintext, owned by caller
  unsigned long dataLen,          //length of plaintext
  PBYTE& pCipherData,             //pointer to ciphertext, owner by caller
  unsigned long& cipherDataLen    //length of ciphertext
  )
{
  if (NullEncryption)
  {
    pCipherData = (PBYTE)new char[dataLen];
    memcpy (pCipherData, pData, dataLen);
    cipherDataLen = dataLen;
    return true;
  }

  bool result(false);

  // Set variable to length of data in buffer.
  cipherDataLen = dataLen;

  // Have API return us the required buffer size.
  BOOL bResult = CryptEncrypt(
            hKey,            // Key obtained earlier
            0,               // No hashing of data
            TRUE,            // Final or only buffer of data
            0,               // Must be zero
            NULL,            // No data yet, simply return size
            &cipherDataLen,         // Size of data
            cipherDataLen);         // Size of block

  if (!bResult)
  {
    DisplayLastError(L"encrypt", GetLastError());
    pCipherData = 0;
    cipherDataLen = 0;
  }
  else
  {
    // We now have a size for the output buffer, so create buffer.
    unsigned int bufferSize (cipherDataLen);
    if (dataLen > cipherDataLen)
      bufferSize = dataLen;

    pCipherData = new BYTE[bufferSize];
    memcpy(pCipherData, pData, dataLen);

    unsigned long dataLength(dataLen);

    // Now encrypt data.
    if (!CryptEncrypt(
    /*1*/          hKey,            // Key obtained earlier
    /*2*/          0,               // No hashing of data
    /*3*/          TRUE,            // Final or only buffer of data
    /*4*/          0,               // Must be zero
    /*5*/          pCipherData,     // Data buffer                - data to be encrypted (or decrypted, below)
    /*6*/          &dataLength,     // Size of data               - in:size of data to be encrypted, out:size of encrypted data?
    /*7*/          cipherDataLen))  // Size of block              - out: size of encrypted block - not always the same length as the plaintext
    {
      DisplayLastError(L"encrypt", GetLastError());
    }
    else
      result = true;
  }
  return result;
}
//}}}
//{{{
bool cWinCrypto::encrypt (
  vfs::cPtr<vfs::cMemory> plainData,      //pointer to plaintext, owned by caller
  vfs::cPtr<vfs::cMemory>& cipherData     //pointer to ciphertext, owner by caller
  )
{
  if (NullEncryption)
  {
    cipherData = plainData;
    return true;
  }

  bool result(false);
  cipherData = vfs::cPtr<vfs::cMemory>();

  // Set variable to length of data in buffer.
  DWORD cipherDataLen = plainData->getSize();

  // Have API return us the required buffer size.
  BOOL bResult = CryptEncrypt(
            hKey,            // Key obtained earlier
            0,               // No hashing of data
            TRUE,            // Final or only buffer of data
            0,               // Must be zero
            NULL,            // No data yet, simply return size
            &cipherDataLen,         // Size of data
            cipherDataLen);         // Size of block

  if (!bResult)
  {
    DisplayLastError(L"encrypt", GetLastError());
  }
  else
  {
    // We now have a size for the output buffer, so create buffer.
    cipherData = new vfs::cMemory ((size_t)cipherDataLen, vfs::cMemory::eHeap);
    memcpy(cipherData->getBytes(), plainData->getBytes(), plainData->getSize());

    unsigned long dataLength(plainData->getSize());

    // Now encrypt data.
    if (!CryptEncrypt(
    /*1*/          hKey,            // Key obtained earlier
    /*2*/          0,               // No hashing of data
    /*3*/          TRUE,            // Final or only buffer of data
    /*4*/          0,               // Must be zero
    /*5*/          (PBYTE)cipherData->getBytes(),     // Data buffer                - data to be encrypted (or decrypted, below)
    /*6*/          &dataLength,     // Size of data               - in:size of data to be encrypted, out:size of encrypted data?
    /*7*/          cipherDataLen))  // Size of block              - out: size of encrypted block - not always the same length as the plaintext
    {
      DisplayLastError(L"encrypt", GetLastError());
    }
    else
    {
      QVERIFY(cipherDataLen == cipherData->getSize());
      result = true;
    }
  }
  return result;
}
//}}}

//{{{
bool cWinCrypto::decrypt (PBYTE pData, unsigned long& dataLen)
{
  if (NullEncryption)
  {
    return true;
  }

  bool result(false);

  if (!CryptDecrypt (hKey, 0/*hHash*/, TRUE, 0, pData, &dataLen))
  {
    DisplayLastError(L"decrypt");
  }
  else
    result = true;

  return result;
}
//}}}
//{{{
bool cWinCrypto::decrypt (vfs::cPtr<vfs::cMemory> pData)
{
  if (NullEncryption)
  {
    return true;
  }

  bool result(false);
  unsigned long dataLen(pData->getSize());
  if (!CryptDecrypt (hKey, 0, TRUE, 0, (PBYTE)pData->getBytes(), &dataLen))
  {
    DisplayLastError(L"decrypt");
  }
  else
  {
    result = true;
    QVERIFY(dataLen == pData->getSize());
  }

  return result;
}
//}}}

//{{{
std::string cWinCryptoHelper::defaultPassword()
/*
  This particular function is not meant to be military grade security by any means,
  it is just a slightly disguised password based on the current user's SID
  and the current time so it changes now and again (every hour).
*/
{
  std::string result(machineName());
  result = std::string(result.substr(result.length()/2, std::string::npos) + result.substr(0,result.length()/2)).substr(0,10);
  //or
  __int64 dateRepresentation = getNumberRoughlyBasedOnDate();
  result = vfs::narrow(GetUserSIDFromPID(GetCurrentProcessId()))+std::string("-")+vfs::narrow(vfs::lexical_cast<vfs::String>(dateRepresentation));
  return result;
}
//}}}

//{{{
bool cWinCryptoHelper::encrypt (const vfs::String& plaintext, std::string& ciphertext, const std::string& password)
{
  //wprintf(L"encrypt length %d: '%s'\n", plaintext.length(), plaintext.c_str());
  bool result(false);
  ciphertext = std::string();

  vfs::String text = vfs::String(kSignature + plaintext);

  PBYTE pCipherData(0); //this is passed to cWinCrypto::encrypt() but is owned by us.

  unsigned long cipherDataLen(0);
  try
  {
    std::string keystring = (password.length() == 0) ? cWinCryptoHelper::defaultPassword() : password;
    cWinCrypto crypto(keystring);
    result = crypto.encrypt((PBYTE)text.c_str(), text.length()*sizeof(vfs::Char), pCipherData, cipherDataLen);

    //now base64 encode the result so it can be passed around with ease.
    Base64Coder coder(kUseAlternateMapping);
    coder.Encode((PBYTE)pCipherData, cipherDataLen);
    ciphertext = std::string((char*)coder.EncodedMessage(), coder.EncodedLength());
  }
  catch (const vfs::cRecoverable& ex)
  {
    wprintf(L"cWinCryptoHelper::encrypt failure: '%s\n", ex.getMsg().c_str());
  }

  delete [] pCipherData;
  return result;
}
//}}}
//{{{
bool cWinCryptoHelper::decrypt (const std::string& ciphertext, vfs::String& plaintext, const std::string& password)
{
  bool result (false);
  plaintext = vfs::String();

  //decode from base64 to 'binary'
  Base64Coder coder(kUseAlternateMapping);
  vfs::cPtr<vfs::cMemory> encodedText = new vfs::cMemory(ciphertext.length(), vfs::cMemory::eHeap);
  memcpy(encodedText->getBytes(), ciphertext.c_str(), ciphertext.length());
  coder.Decode((PBYTE)encodedText->getBytes(), encodedText->getSize());
  std::string decoded ((char*)coder.DecodedMessage(), coder.DecodedLength());
  vfs::cPtr<vfs::cMemory> encryptedText = new vfs::cMemory((size_t)coder.DecodedLength(), vfs::cMemory::eHeap);
  memcpy(encryptedText->getBytes(), coder.DecodedMessage(), coder.DecodedLength());

  //now decrypt
  try
  {
    std::string keystring = (password.length() == 0) ? cWinCryptoHelper::defaultPassword() : password;
    cWinCrypto crypto (keystring);
    unsigned long dataLen(encryptedText->getSize());
    result = crypto.decrypt((PBYTE)encryptedText->getBytes(), dataLen);

    vfs::Char* pChar = (vfs::Char*)encryptedText->getBytes();
    plaintext = vfs::String(pChar, dataLen/sizeof(vfs::Char));

    if (kSignature == plaintext.substr(0,kSignature.length()))
    {
      plaintext = plaintext.substr(kSignature.length());
    }
    else
    {
      //wprintf(L"cWinCryptoHelper::decrypt failure: '%s\n", plaintext.c_str());
      plaintext = vfs::String();
      result = false;
    }
  }
  catch (const vfs::cRecoverable& ex)
  {
    wprintf(L"cWinCryptoHelper::decrypt failure: '%s\n", ex.getMsg().c_str());
    result = false;
  }
  return result;
}
//}}}
