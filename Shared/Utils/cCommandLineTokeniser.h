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


#include <list>

namespace vfs
{

  // cCommandLineTokeniser
  //
  // Splits a string into tokens at whitespace, handling quotes (both double and single).
  // Quote characters may be escaped to be treated as literal characters instead of opening/closing quotes.
  // Throws cBadFormat if there is an unmatched ' or ".
  //
  // Examples (strings delimited by {} for clarity, [] represents a list with items comma-separated):
  //
  // Input -> Tokens
  // {abc} -> [{abc}]
  // {a b c} -> [{a},{b},{c}]
  // {a "b c"} -> [{a},{b c}]
  // {a"b c"} -> [{ab c}]
  // {a \"b c} -> [{a},{"b},{c}]
  // {"'a b' c"} -> [{'a b' c}]
  // {C:\mydir} -> [{C:\mydir}]   * Note that escape character only affects quotes and whitespace
  // {a\ b\c} -> [{a b\c}]
  // {'a"\'"b'"} -> [{a"\b'}]     * No special handling of escape character inside single quotes
  class cCommandLineTokeniser
  {
  public:
    typedef std::list<String> container_type;
    typedef String value_type;
    typedef container_type::const_iterator const_iterator;

    cCommandLineTokeniser(const String& cmdLine, Char esc = L'\\')
    {
      StringStream tok;
      bool escape = false;
      bool inSquote = false;
      bool inDquote = false;
      bool endQuote = false;
      for (String::const_iterator ch = cmdLine.begin(); ch != cmdLine.end(); ++ch)
      {
        if (iswspace(*ch) && !escape && !inSquote && !inDquote)
        {
          // discard, capture token
          if (!tok.str().empty() || endQuote)
          {
            tokens_.push_back(tok.str());
            tok.str(L"");
          }
          endQuote = false;
        }
        else
        {
          switch (*ch)
          {
          case L'\'':
            if (escape)
            {
              escape = false;
              tok << *ch;
            }
            else if (inSquote)
            {
              inSquote = false;
              endQuote = true;
            }
            else if (inDquote)
              tok << *ch;
            else
              inSquote = true;
            break;

          case L'"':
            if (escape)
            {
              escape = false;
              tok << *ch;
            }
            else if (inDquote)
            {
              inDquote = false;
              endQuote = true;
            }
            else if (inSquote)
              tok << *ch;
            else
              inDquote = true;
            break;

          default:
            if (*ch == esc && !escape && !inSquote)
            {
              escape = true;
            }
            else
            {
              if (escape && *ch != esc && !iswspace(*ch))
                tok << L'\\';
              tok << *ch;
              escape = false;
            }
            endQuote = false;
            break;
          }
        }
      }

      if (escape)
        tok << L'\\';
      if (inSquote)
        throw cBadFormat(__FILE__, __LINE__, L"Unmatched '");
      if (inDquote)
        throw cBadFormat(__FILE__, __LINE__, L"Unmatched \"");
      if (!tok.str().empty() || endQuote)
        tokens_.push_back(tok.str());
    }

    bool empty() const              { return tokens_.empty(); }
    size_t size() const             { return tokens_.size(); }
    const_iterator begin() const    { return tokens_.begin(); }
    const_iterator end() const      { return tokens_.end(); }
    const value_type& front() const { return tokens_.front(); }
    void pop_front()                { tokens_.pop_front(); }

    const container_type& tokens() const { return tokens_; }

  private:
    container_type tokens_;
  };

}
