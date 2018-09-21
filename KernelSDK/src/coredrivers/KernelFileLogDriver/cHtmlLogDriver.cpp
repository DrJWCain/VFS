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

namespace
{
  //{{{
  class html_escape_iterator // based on std::back_insert_iterator
  {
  public:
    explicit html_escape_iterator(std::string& _Cont) : container(&_Cont) { }

    html_escape_iterator& operator=(std::string::const_reference _Val)
    {
      // escape invalid characters in html
      if (_Val == '<')
        container->append("&lt;");
      else if (_Val == '>')
        container->append("&gt;");
      else if (_Val == '&')
        container->append("&amp;");
      else if (_Val == '\n')
        container->append("<br>");
      else if (_Val != '\r') 
        container->push_back(_Val);
      return (*this);
    }

    html_escape_iterator& operator*()
    { // pretend to return designated value
      return (*this);
    }

    html_escape_iterator& operator++()
    { // pretend to preincrement
      return (*this);
    }

    html_escape_iterator& operator++(int)
    { // pretend to postincrement
      return (*this);
    }

  protected:
    std::string* container;
  };
  //}}}
}

//{{{
void cHtmlLogDriver::htmlWrite(const String& utf16_data) const
{
  if (!utf16_data.empty())
  {
    std::string utf8_data;
    try
    {
      utf8::utf16to8(utf16_data.begin(), utf16_data.end(), html_escape_iterator(utf8_data));
    }
    catch (utf8::exception&)
    {
      utf8_data += "[INVALID UNICODE CHARACTER SEQUENCE]";
    }
    write(utf8_data);
  }
}
//}}}
//{{{
void cHtmlLogDriver::handleMsg(const iLogDriver::cMsg& msg) const
{
  if (msg.m_Type == iLogDriver::cMsg::eBreak)
  {
    write("<div>");
    htmlWrite(msg.m_Text);
    write("</div>\r\n");
    return;
  }

  const bool isMainThread = (msg.m_ThreadName.substr(0, 4) == L"Krnl"); // backwards compatible with KernelFileLogDriver

  // output the HTML <p> tag with a class so that it can be styled
  {
    std::stringstream line;
    line << "<p class=\"" << (isMainThread ? "m" : "o");

    if (msg.m_Type == iLogDriver::cMsg::eErrorMsg) // exceptions, etc.
      line << " e";
    else if (msg.m_NumSOSIndents > 0) // QSOS
      line << " s";
    else if (msg.m_Type == iLogDriver::cMsg::eTrace) // QTRACE
      line << " t";

    if (msg.m_IsHighlighted)
      line << " h";

    line << "\">";
    write(line.str());
  }

  // output the time
  {
    char buffer[32];
    sprintf(buffer, "%02d:%02d:%02d.%03d %5d", msg.m_LocalTime.getHour(), msg.m_LocalTime.getMinute(), msg.m_LocalTime.getSecond(), msg.m_LocalTime.getMillisec(), msg.m_ThreadID);
    write(buffer);
  }

  write(msg.m_NumInjectedIndents > 0 ? "-&gt; " : "   ");
  for (UInt32 i = 0; i < msg.m_NumIndents; ++i)
    write("  ");

  // output the module info
  char singleChar[2] = " "; // either '+', '-' or ' ' for thread indenting

  switch (msg.m_Type)
  {
  case iLogDriver::cMsg::eLazyIndent:
    write(" {</p>\r\n");
    return;

  case iLogDriver::cMsg::eOutdent:
    write(" }</p>\r\n");
    return;

  case iLogDriver::cMsg::eCollapsedIndent:
    singleChar[0] = '+';
    break;

  case iLogDriver::cMsg::eExpandedIndent:
    singleChar[0] = '-';
    break;
  }

  // output module name or thread name as appropriate
  {
    std::string name = narrow(isMainThread ?
      msg.m_ModuleName.substr(0, msg.m_ModuleName.find_first_of(L'.')) : // remove the ".dll"
      msg.m_ThreadName);
    std::transform(name.begin(), name.end(), name.begin(), ::toupper);

    std::stringstream line;
    line << singleChar << name << " <span>";
    write(line.str());
  }

  // htmlWrite replaces \n with html <br> and escapes character entities
  htmlWrite(msg.m_Text);

  if (msg.m_Type == iLogDriver::cMsg::eCollapsedIndent)
    write("...");

  write("</span></p>\r\n");
}
//}}}
//{{{
void cHtmlLogDriver::writeHeader(const String& prevFile) const
{
  std::stringstream header;
  header << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">\r\n"
            "<html>\r\n<head>\r\n"
            "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\r\n";

  // Our Stylesheet:
  header << "<style type=\"text/css\">/*<![CDATA[*/\r\n"
              "body { font-family: \"FixedSys\", \"Courier\", monospace; color: silver; background-color: black; padding: 1em 0; }\r\n"
              "p { margin: 0px; white-space: pre; }\r\n"
              "div { margin: 4em 0 1em 0; padding: 1em 0em 1em 8em; background-color: navy; font-weight: bold; color: white; }\r\n"
              "#head { margin-bottom: 2em; }\r\n"
              "#foot { margin-top: 2em; }\r\n"
              "#head, #foot { color: orange; text-align: center; }\r\n"
              "a { color: orange; background-color: black; text-decoration: none; }\r\n"
              "a:hover { color: black; background-color: orange; }\r\n"
              ".m { color: #00f; }\r\n"
              ".o { color: #0c0; }\r\n"
              "span { color: silver; background-color: black; }\r\n"
              ".t span { color: cyan; }\r\n"
              ".s span { color: red; }\r\n"
              ".e span { color: black; background-color: red; }\r\n"
            "/*]]>*/</style>\r\n";

  // Our Navigation Script:
  header << "<script type=\"text/javascript\">//<![CDATA[\r\n"
              "function go(delta)\r\n"
              "{\r\n"
                "var url = window.location.href.replace(/%20/g, \" \").match(/^(.*?)(-?[0-9]*)([^0-9#]*)(#[^0-9]*)?$/);\r\n"
                "window.location.href = url[1] + (parseInt(url[2]) + delta) + url[3] + (delta < 0 ? \"#foot\" : \"\");\r\n"
                "return false;\r\n"
              "}\r\n"
            "//]]></script>\r\n";

  header << "</head>\r\n<body>\r\n";
  write(header.str());

  if (!prevFile.empty())
  {
    // A bit of a bodge, but upshot is that the Next/Prev buttons will only appear
    // if javascript is enabled, which is good since they need javascript to work.
    write("<script type=\"text/javascript\"> document.write(\"<p id=\\\"head\\\"><a href=\\\"#\\\" onclick=\\\"return go(-1);\\\">&laquo; Prev Log</a></p>\"); </script>\r\n");
  }

  // The following line tests that the UTF16 -> UTF8 conversion works:
  //write(L"<div>Ni hao (\x4f60\x597d) is the Mandarin Chinese word for 'hello'.</div>\r\n");
}
//}}}
//{{{
void cHtmlLogDriver::writeFooter(const String& nextFile) const
{
  if (!nextFile.empty())
  {
    // A bit of a bodge, but upshot is that the Next/Prev buttons will only appear
    // if javascript is enabled, which is good since they need javascript to work.
    write("<script type=\"text/javascript\"> document.write(\"<p id=\\\"foot\\\"><a href=\\\"#\\\" onclick=\\\"return go(1);\\\">Next Log &raquo;</a></p>\"); </script>\r\n");
  }

  write("</body>\r\n</html>\r\n");
}
//}}}
