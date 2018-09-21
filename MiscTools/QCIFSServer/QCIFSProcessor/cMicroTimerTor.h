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

//cMicroTimerTor.h

#pragma once

//{{{
class cMicroTimer
{
  LARGE_INTEGER StartTime;
public:
  void start()
  {
    QueryPerformanceCounter(&StartTime);
  }
  int stop()
  {
    LARGE_INTEGER stopTime;
    QueryPerformanceCounter(&stopTime);
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);

    LARGE_INTEGER result;
    result.QuadPart = ((stopTime.QuadPart - StartTime.QuadPart)*1000000)/freq.QuadPart;
    return result.LowPart;
  }
};
//}}}
//{{{
class cMicroTimerTor : private cMicroTimer
{
  vfs::String Message;
public:
  cMicroTimerTor(const vfs::String& message) : Message(message)
  {
    start();
  }
  ~cMicroTimerTor()
  {
    int time = stop();
    QTRACE((L"%s %dus",Message.c_str(),time));
  }
};
//}}}