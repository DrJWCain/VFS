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

//cDFSReferral.h

#pragma once

using namespace vfs;

class cDFSReferral : public cRefCount
{
public:
  cDFSReferral(const std::wstring &sDFSReferralPath);
  ~cDFSReferral(void);

  bool resolveDFSPath(std::wstring &sPath);
  std::wstring findReferral(const std::wstring &sPath);
  bool isReferralBaseSet() { return !m_sReferralBase.empty(); }
  void setReferralBase(const std::wstring &refBase);
  std::wstring getReferralBase() { return m_sReferralBase; }
  void resetReferralBase();

private:
  void generateReferralBase();
  void stripReferralBase(std::wstring &rsPath);

private:
  cLock m_AccessLock;
  std::wstring m_sReferralBase;
  std::map<std::wstring, std::wstring> m_ReferralMap;
  const std::wstring m_sDFSReferralPath;
};
