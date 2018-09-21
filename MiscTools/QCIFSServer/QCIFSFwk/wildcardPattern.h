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

//wildcardPattern.h

#pragma once

static boost::wregex makeWildcardPattern(vfs::String sCIFSPattern) //make a copy of the string
{
  sCIFSPattern = boost::regex_replace(sCIFSPattern, boost::wregex(L"\\?"), L".");
  sCIFSPattern = boost::regex_replace(sCIFSPattern, boost::wregex(L"\\*"), L".*");
  sCIFSPattern = boost::regex_replace(sCIFSPattern, boost::wregex(L">"), L"."); //[#1] p.25
  sCIFSPattern = boost::regex_replace(sCIFSPattern, boost::wregex(L"<"), L".*");
  return boost::wregex(sCIFSPattern, boost::regex::perl|boost::regex::icase);  //perl, case insensitive
}