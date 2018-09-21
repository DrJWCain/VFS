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

/** @file   avceperformance.h
*   @brief  This file contains function declerations related to Performance measurement
*   @author AVC Intra Encoder Team
*   @date   06-03-2008
*/

#ifndef _AVCE_PERFORMANCE_
#define _AVCE_PERFORMANCE_

#include <windows.h>

#include <boost/noncopyable.hpp>
#include <memory>

static inline LARGE_INTEGER GetFreq()
{
  LARGE_INTEGER freq;

  QueryPerformanceFrequency(&freq);

  return freq;
}

static const LARGE_INTEGER gFreq = GetFreq();


class CScopedTimeDiff : boost::noncopyable
{
    unsigned m_acc;
    unsigned& m_milliSecAccumulator;
    LARGE_INTEGER m_start;
    LONGLONG& m_microSecAccumulator;
public:
  explicit CScopedTimeDiff(unsigned& milliSecAccumulator) : m_milliSecAccumulator(milliSecAccumulator), m_microSecAccumulator(m_start.QuadPart)
  {
    QueryPerformanceCounter(&m_start);
  }
  explicit CScopedTimeDiff(LARGE_INTEGER& microSecAccumulator) : m_milliSecAccumulator(m_acc), m_microSecAccumulator(microSecAccumulator.QuadPart) 
  {
    QueryPerformanceCounter(&m_start);
  }

  explicit CScopedTimeDiff(LONGLONG& microSecAccumulator) : m_milliSecAccumulator(m_acc), m_microSecAccumulator(microSecAccumulator) 
  {
    QueryPerformanceCounter(&m_start);
  }

  CScopedTimeDiff(unsigned& milliSecAccumulator, LONGLONG& microSecAccumulator) : m_milliSecAccumulator(milliSecAccumulator), m_microSecAccumulator(microSecAccumulator)
  {
    QueryPerformanceCounter(&m_start);
  }
 
  ~CScopedTimeDiff()
  {
      LARGE_INTEGER stopTime;
      QueryPerformanceCounter(&stopTime);
      LONGLONG result;
      result = ((stopTime.QuadPart - m_start.QuadPart)*1000000)/gFreq.QuadPart; 
      m_microSecAccumulator += result; 
      m_milliSecAccumulator += (unsigned) result/1000;
  }

};

class CStopWatch //: boost::noncopyable
{
  CStopWatch(const CStopWatch&);
  CStopWatch& operator = (const CStopWatch&);

  char m_memoryHolder[sizeof(CScopedTimeDiff)];
  CScopedTimeDiff* m_scopedTimeDiff;
  unsigned m_accMilliSec;
  LONGLONG m_accMicroSec;
  
  void Stop()
  {
    m_scopedTimeDiff->~CScopedTimeDiff();
  }
  void Start() 
  { 
    m_accMilliSec = 0;
    m_accMicroSec = 0;
    m_scopedTimeDiff = new (m_scopedTimeDiff) CScopedTimeDiff(m_accMilliSec, m_accMicroSec); 
  }
public:
  //CStopWatch() : m_scopedTimeDiff((CScopedTimeDiff*) m_memoryHolder), m_acc(0) { Start(); }
  CStopWatch() : m_scopedTimeDiff((CScopedTimeDiff*) m_memoryHolder), m_accMilliSec(0), m_accMicroSec(0)  { Start(); }

  unsigned GetMilliSecs()
  {
    Stop();
    unsigned acc = m_accMilliSec;
    Start();
    return acc;
  }
  
  LONGLONG GetMicroSecs()
  {
    Stop();
    LONGLONG acc = m_accMicroSec;
    Start();
    return acc;
  }
};



#endif