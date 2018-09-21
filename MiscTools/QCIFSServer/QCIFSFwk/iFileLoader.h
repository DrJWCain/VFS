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

//iFileLoader.h

#pragma once

class iFileEventCallback
{
public:
  virtual ~iFileEventCallback(){}

  virtual void clearMemCache() = 0;
  virtual void errorRemoveFromDirectory() = 0;
};

typedef std::pair<unsigned int,vfs::cConstPtr<vfs::cMemoryView> > tFramePart;
typedef std::vector<tFramePart> tFrameVector;

class iFileLoader
{
public:
  virtual ~iFileLoader(){}

  virtual void notifyChange(const tFrameVector frameVector
    , const vfs::String& sName
    , const vfs::cPtr<iFileEventCallback> pFileEventCallback
    , ULONGLONG fid) = 0;
  virtual void notifyClosed(const tFrameVector frameVector
    , const vfs::String& sTitle
    , const unsigned int sessionID
    , const vfs::cPtr<iFileEventCallback> pFileEventCallback
    , ULONGLONG fid) = 0;

  virtual vfs::cPtr<vfs::cMemoryView> loadMem(const vfs::String& sFilename, const unsigned int sessionID, const bool bHeader) = 0;
  virtual __int64 getExpectedSize(const vfs::String& sFilename) = 0;
};