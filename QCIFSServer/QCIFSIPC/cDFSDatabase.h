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

//cDFSDatabase.h

#pragma once

class cDFSDatabase : public vfs::cRefCount
{
public:
  cDFSDatabase(void);
  ~cDFSDatabase(void);

  void addDFSPath(const vfs::String& sDisplayPath, const vfs::String& sDFSPath);
  vfs::String getDFSPath(const vfs::String& sDisplayPath) const;
  void removeDFSPath(const vfs::String& sDisplayPath);

private:  //private member functions
  inline vfs::String subPath(const vfs::String& sPath) const;
  inline vfs::String pathRoot(const vfs::String& sPath) const;
  inline vfs::cPtr<cDFSDatabase> getDatabase(const vfs::String& sPath) const;
  inline vfs::cPtr<cDFSDatabase> getOrCreateDatabase(const vfs::String& sPath);
  inline bool empty() const;

private:  //private member variables
  typedef std::map<vfs::String, vfs::cPtr<cDFSDatabase> > tDatabaseMap;
  mutable vfs::cLock m_childAccess;
  tDatabaseMap m_childDatabase;

  typedef std::map<vfs::String, vfs::String> tNameMap;
  mutable vfs::cLock m_namesAccess;
  tNameMap m_nameMap;

  static const vfs::String m_shareFinder;
};
