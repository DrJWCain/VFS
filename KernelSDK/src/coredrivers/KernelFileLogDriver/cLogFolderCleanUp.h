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

#pragma once

namespace vfs
{
  class cLogFolderCleanUp
  {
  public:
    cLogFolderCleanUp(const String& logFolder, const UInt32 fileLimit);

    void renameFile(const String& oldFile, const String& newFile) const;
    void renameGroup(const std::map<int, String>& files, const String& ext) const;

  private:
    const String mLogFolder;
    const UInt32 mFileLimit;
  };
}
