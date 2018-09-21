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

using namespace vfs;

//{{{
const std::string cTextLogDriver::getPrefix(const iLogDriver::cMsg& msg) const
{
  if (msg.m_Type == iLogDriver::cMsg::eBreak)
    return "\r\n\r\n\r\n\r\n\r\n";

  std::stringstream prefix;

  // output the time
  {
    char buffer[32];
    sprintf(buffer, "%02d:%02d:%02d.%03d %5d", msg.m_LocalTime.getHour(), msg.m_LocalTime.getMinute(), msg.m_LocalTime.getSecond(), msg.m_LocalTime.getMillisec(), msg.m_ThreadID);
    prefix << buffer;
  }

  prefix << (msg.m_NumInjectedIndents > 0 ? "-> " : "   ");
  for (UInt32 i = 0; i < msg.m_NumIndents; ++i)
    prefix << "  ";

  // output the module info
  char singleChar[2] = " "; // the + - or space for thread indenting

  switch (msg.m_Type)
  {
  case iLogDriver::cMsg::eLazyIndent:
    return prefix.str() + " {";

  case iLogDriver::cMsg::eOutdent:
    return prefix.str() + " }";

  case iLogDriver::cMsg::eCollapsedIndent:
    singleChar[0] = '+';
    break;

  case iLogDriver::cMsg::eExpandedIndent:
    singleChar[0] = '-';
    break;
  }

  {
    const bool isMainThread = (msg.m_ThreadName.substr(0, 4) == L"Krnl"); // backwards compatible with KernelFileLogDriver
    std::string name = narrow(isMainThread ?
      msg.m_ModuleName.substr(0, msg.m_ModuleName.find_first_of(L'.')) : // remove the ".dll"
      msg.m_ThreadName);
    std::transform(name.begin(), name.end(), name.begin(), ::toupper);

    prefix << singleChar << name << " ";
  }

  return prefix.str();
}

const String cTextLogDriver::getBody(const iLogDriver::cMsg& msg) const
{
  switch (msg.m_Type)
  {
  case iLogDriver::cMsg::eBreak:
    return L"             " + msg.m_Text + L"\r\n\r\n";

  case iLogDriver::cMsg::eLazyIndent:
  case iLogDriver::cMsg::eOutdent:
    return L"";

  default:
    return msg.m_Text + (msg.m_Type == iLogDriver::cMsg::eCollapsedIndent ? L"..." : L"");
  }
}
//}}}
//{{{
void cTextLogDriver::handleMsg(const iLogDriver::cMsg& msg) const
{
  // The reason for the getPrefix and getBody functions was that cHtmlLogDriver
  // was originally going derive from cTextLogDriver to reduce duplication.
  write(getPrefix(msg));
  write(getBody(msg));
  write("\r\n");
}
//}}}
