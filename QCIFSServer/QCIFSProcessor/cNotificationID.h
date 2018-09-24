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

//cNotificationID.h

#pragma once

class cNotificationID
{
  const ULONGLONG m_Sid;
  const ULONG m_Tid;
  const ULONGLONG m_Mid;
  cNotificationID() : m_Sid(0), m_Tid(0), m_Mid(0){}  //private default constructor

public:
  cNotificationID(const ULONGLONG Sid
    , const ULONG Tid
    , const ULONGLONG Mid)
    : m_Sid(Sid)
    , m_Tid(Tid)
    , m_Mid(Mid)
  {}

  //bool operator= (const cNotificationID& rRHS)
  //{
  //  m_Sid = rRHS.m_Sid;
  //  m_Tid = rRHS.m_Tid;
  //  m_Mid = rRHS.m_Mid;
  //}

  bool operator== (const cNotificationID& rRHS) const
  {
    return m_Sid == rRHS.m_Sid
      && m_Tid == rRHS.m_Tid
      && m_Mid == rRHS.m_Mid;
  }

  bool operator< (const cNotificationID& rRHS) const
  {
    if(m_Sid == rRHS.m_Sid && m_Tid == rRHS.m_Tid)
      return m_Mid < rRHS.m_Mid;
    if(m_Sid == rRHS.m_Sid)
      return m_Tid < rRHS.m_Tid;
    return m_Sid < rRHS.m_Sid;
  }

  vfs::String toString() const
  {
    std::wstringstream strm;
    strm << m_Sid << "_" << m_Tid << "_" << m_Mid;
    return strm.str();
  }
};

class cGuardNotificationID : public vfs::iGuarded, public vfs::cRefCount
{
public:
  cGuardNotificationID(const cNotificationID& nid) : m_nid(nid){}

  cNotificationID getNid() const {return m_nid;}

private:
  const cNotificationID m_nid;
};