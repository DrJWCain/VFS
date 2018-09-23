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

//cCurlWrapper.h

#pragma once

vfs::cMemoryView::Ptr HTTPGet_basic(const std::string& url, bool log, std::list<std::string>* pHeaders);

vfs::cMemoryView::Ptr HTTPPostJSONWithReply(const std::string& url, vfs::cMemoryView::Ptr payload, bool log);
vfs::cMemoryView::Ptr HTTPPutJSONWithReply(const std::string& url, vfs::cMemoryView::Ptr payload, bool log);


class cHTTPError : public vfs::cRecoverable
{
public:
  unsigned int ResponseCode;
  vfs::cMemoryView::Ptr Payload;
  virtual ~cHTTPError() {};
  cHTTPError(const char* const  SrcFilename, const unsigned long SrcLineNumber, unsigned int responseCode, vfs::cMemoryView::Ptr payload, const vfs::Char* MsgFormat, ...) :
    cRecoverable(SrcFilename, SrcLineNumber, MsgFormat, (va_list)&MsgFormat + _INTSIZEOF(MsgFormat)), ResponseCode(responseCode), Payload(payload)
  {
  }
};