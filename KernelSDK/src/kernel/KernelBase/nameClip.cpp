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

/*
**  
*/

// --- Pre-Compiled Headers --------------------------------------------------------------

#include "stdafx.h"
#include "QKernelBase.h"
#include <malloc.h>
using namespace std;
using namespace vfs;

//nameClip will make a new clip name.
//It takes an old clip name and makes a new one by adding or incrementing a version number.

namespace
{

String nameClip(const String & oldName)
{
  String ret;

  String::size_type p = oldName.rfind(L'-');
  if(p == String::npos)//no hyphen, so concatenate a number
  {
    ret = oldName + L"-001";
  }
  else
  {
    ++p;//skip the hyphen

    String::size_type numStart = p;
    bool isnum = false;
    while(p < oldName.size())
    {
      if(iswdigit(oldName[p]))
      {
        isnum = true;
      }
      else
      {
        isnum = false;
        break;
      }
      ++p;
    }
    if(!isnum)//we found a hyphen but not a valid number
    {
      ret = oldName + L"-001";
    }
    else //we found a hyphen and a number
    {
      #if defined(_DEBUG)
        assert(p >= oldName.size());//the last part of the string is a number
      #endif

      String numAsString(&(oldName[numStart]),&(oldName[oldName.size()]));
      StringStream numAsStream(numAsString);

      int version;
      numAsStream >> version;
      ++version;//up the version number

      String clipBaseName(&(oldName[0]),&(oldName[numStart]));//get the base name of the string (including the hyphen)

      StringStream result;
      result << clipBaseName << setfill(L'0') << setw(3) << version;//concatenate the version number
      ret = result.str();
    }
  }

  return ret;
}

}