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

//cSMB2Session.h

#pragma once

#define SECURITY_WIN32
#include <sspi.h>

#include "iTreeChangeListener.h"

class cSMB2Response;
class cSMB2Tree;
class iTreeResourceFactory;

namespace Kerr {
  namespace Security {
    namespace Cryptography {
      class Buffer;
    }
  }
}


class cSMB2Session : public iTreeChangeListener, public vfs::cRefCount
{
public:
  cSMB2Session(bool useSMB3Signing = false);
  ~cSMB2Session(void);

  PBYTE getNTLMServerResponce(bool fNewConversation, const PUCHAR pGSSdata, DWORD cbIn, DWORD& cbOut);
  PBYTE VerifyThis(PBYTE pDataBuffer, DWORD dataLen, PBYTE pSigBuffer, DWORD sigLen);
  PBYTE SignThis(PBYTE pDataBuffer, DWORD dataLen, PBYTE pSigBuffer, DWORD sigLen);

  //iTreeChangeListener
  void replaceTree(const vfs::String& sName
    , const vfs::cPtr<iTreeResourceFactory>& pTreeResourceFactory);

  //public member functions
  ULONG ConnectTree(const vfs::String& sShare
    , const vfs::cPtr<cSMB2Response>& response
    , const vfs::cPtr<iTreeResourceFactory> pTreeResourceFactory);
  void DisconnectTree(const ULONG Tid);
  vfs::cPtr<cSMB2Tree> getTree(const ULONG Tid) const;
  void CancelNotifications(const ULONGLONG& messageId, bool synchronous);

  vfs::String loginName()
  {
    if(MasterSession.isValid())
      return MasterSession->loginName();
    return m_sLoginName;
  }

  void setLoginName(const vfs::String& sLoginName)
  {
    if(m_sLoginName.empty())
      m_sLoginName = sLoginName;
    else
    {
      QTRACE((L"setLoginName %s %s", m_sLoginName.c_str(), sLoginName.c_str()));
    }
  }

  void addChannelKey(vfs::cPtr<cSMB2Session> previousSession);
  void clearChannelKey();

  Kerr::Security::Cryptography::Buffer calculateSessionKey(const PUCHAR pGSSdata);
  void setChallenge(ULONGLONG challenge);
  void setDomainAndWorkstation(const vfs::String& domain, const vfs::String& workstation);
  void setTargetInfo(DWORD length, PBYTE buffer);

  void setLmChallengeResponse(DWORD length, PBYTE buffer);
  void setNtChallengeResponse(DWORD length, PBYTE buffer);
  void setEncryptedRandomSessionKey(DWORD length, PBYTE buffer);

private:  //private member variables
  vfs::String m_sLoginName;
  typedef std::map<LONG, vfs::cPtr<cSMB2Tree> > tTreeMap; 
  mutable vfs::cLock m_TreeAccess;
  tTreeMap m_treeMap;
  volatile LONG m_nTreeCount;

  //Security support
  CredHandle hcred;
  CtxtHandle hctxt;

  PBYTE pInBuf; 
  PBYTE pOutBuf;
  DWORD cbMaxMessage;
  char lpPackageName[1024];

  void initSecurity();
  BOOL GenServerContext(BYTE *pIn, DWORD cbIn, BYTE *pOut,
                       DWORD *pcbOut, BOOL *pfDone, BOOL fNewConversation);

  std::vector<unsigned char> SessionKey;
  std::vector<unsigned char> SignKey;
  std::vector<unsigned char> PrevousSignKey;

  ULONGLONG mChallenge;
  vfs::String mDomain;
  vfs::String mWorkstation;
  std::vector<unsigned char> mTargetInfo;
  std::vector<unsigned char> mLmChallengeResponse;
  std::vector<unsigned char> mNtChallengeResponse;
  std::vector<unsigned char> mEncryptedRandomSessionKey;

  const bool UseSMB3Signing;
  vfs::cPtr<cSMB2Session> MasterSession;
};

class cAccessDeniedErrorValue : public vfs::cAccessDenied
{
public :
  virtual ~cAccessDeniedErrorValue() {}
  cAccessDeniedErrorValue(const char* const  SrcFilename, const unsigned long SrcLineNumber, unsigned int errorValue, const vfs::Char* MsgFormat, ...) :
    vfs::cAccessDenied(SrcFilename, SrcLineNumber, MsgFormat, (va_list)&MsgFormat + _INTSIZEOF(MsgFormat)), ErrorValue(errorValue)
  {
  }
  unsigned int ErrorValue;
};
