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

#include "cScanFiles.h"

using namespace vfs;

//{{{
cLogFolderCleanUp::cLogFolderCleanUp(const String& logFolder, const UInt32 fileLimit)
  : mLogFolder(logFolder), mFileLimit(fileLimit)
{
  // List all files in the folder
  std::list<String> files;
  iFileManager::singleton().enumerateFiles(0, mLogFolder, new cScanFiles(files));

  std::map<std::string, std::set<String> > groupsOfFiles;
  std::map<int, String> recentHtmlFiles;
  std::map<int, String> recentTextFiles;

  // Only collect logs which have been created by us, ignore renamed ones
  for (std::list<String>::const_iterator it = files.begin(); it != files.end(); ++it)
  {
    int p = 1;
    if (wcsncmp(it->c_str(), L"Recent.", 7) == 0
        || swscanf(it->c_str(), L"Recent (Part %d).", &p) == 1)
    {
      // scanf doesn't read past the last % so we have to deduce extension ourself
      if (it->find(L".html") != String::npos)
        recentHtmlFiles.insert(std::pair<int, String>(p, *it));
      else if (it->find(L".txt") != String::npos)
        recentTextFiles.insert(std::pair<int, String>(p, *it));
    }
    else
    {
      int y, m, d, h, n, s;
      if (swscanf(it->c_str(), L"%4d-%2d-%2d %2d-%2d-%2d (Part %d).", &y, &m, &d, &h, &n, &s, &p) == 7
          || swscanf(it->c_str(), L"%4d-%2d-%2d %2d-%2d-%2d.", &y, &m, &d, &h, &n, &s) == 6
          || swscanf(it->c_str(), L"%dy%dm%dd - %dh%dm%ds (Part %d).", &y, &m, &d, &h, &n, &s, &p) == 7
          || swscanf(it->c_str(), L"%dy%dm%dd - %dh%dm%ds", &y, &m, &d, &h, &n, &s) == 6) // backwards compatible with KernelFileLogDriver
      {
        std::string key = isoTime(y, m, d, h, n, s);
        if (groupsOfFiles.find(key) == groupsOfFiles.end())
          groupsOfFiles.insert(std::pair<std::string, std::set<String> >(key, std::set<String>()));
        groupsOfFiles[key].insert(*it);
      }
    }
  }

  // Delete groups of logs if we go over the limit
  size_t fileCount = 0;
  for (std::map<std::string, std::set<String> >::const_reverse_iterator rit = groupsOfFiles.rbegin(); rit != groupsOfFiles.rend(); ++rit)
  {
    fileCount += rit->second.size();
    if (fileCount > mFileLimit)
    {
      for (std::set<String>::const_iterator it = rit->second.begin(); it != rit->second.end(); ++it)
      {
        try
        {
          iFileManager::singleton().deleteFileOrFolder(mLogFolder + *it, fDeleteFileOrFolder_DeleteReadOnly);
        }
        catch (cRecoverable&)
        {
          // What now?
        }
      }
    }
  }

  // Rename the recent files to timestamped versions
  renameGroup(recentHtmlFiles, L"html");
  renameGroup(recentTextFiles, L"txt");
}
//}}}
//{{{
void cLogFolderCleanUp::renameFile(const String& oldFile, const String& newFile) const
{
  try
  {
    iFileManager::singleton().renameFile(mLogFolder + oldFile, mLogFolder + newFile);
  }
  catch (cRecoverable&)
  {
    // What now?
  }
}
//}}}
//{{{
void cLogFolderCleanUp::renameGroup(const std::map<int, String>& files, const String& ext) const
{
  if (files.empty())
    return;

  iFile::Ptr ptr = iFileManager::singleton().openFile(mLogFolder + files.begin()->second, 0);
  cDate date = ptr->getModificationDate();
  cTime time = ptr->getModificationTime();
  ptr.invalidate();

  const String newNamePrefix = isoTimeW(date.getYear(), date.getMonth(), date.getDay(), time.getHour(), time.getMinute(), time.getSecond());

  if (files.size() == 1)
  {
    renameFile(files.begin()->second, newNamePrefix + L"." + ext);
    return;
  }

  std::map<int, String>::const_iterator cit;
  for (cit = files.begin(); cit != files.end(); ++cit)
  {
    StringStream newName;
    newName << newNamePrefix << L" (Part " << cit->first << L")." << ext;
    renameFile(cit->second, newName.str());
  }
}
//}}}