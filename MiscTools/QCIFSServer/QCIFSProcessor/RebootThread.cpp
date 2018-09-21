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

/////////////////////////////////////////////////////////////////////////////
// RebootThread.cpp

#include "stdafx.h"

//#include "RebootThread.h"
//
//#include "cQCIFSProcessor.h"
//#include "ATLComTime.h" // for coledatetime
//
////#include "..\..\MiscPlugins\QOptions\QOptions.h"
//
//using namespace vfs;
//
///////////////////////////////////////////////////////////////////////////////
//// externals
//
////extern iModuleContext::Ptr* gPluginContext;
//extern iModuleContext::Ptr gModuleContext;
//
///////////////////////////////////////////////////////////////////////////////
//// subroutines
//
////bool testFCPGateway()
////{
////  static bool Initialised = false;
////  static bool Result = false;
////  static cLock Safety;
////
////  if(!Initialised)//Double checked locking pattern 
////  {
////    cLockGuard g(&Safety);
////    if(!Initialised)
////    {
////      try
////      {
////        iOptionManager::singleton().checkOut (L"FCP Gateway Folders");
////        Result = true;
////      }
////      catch (cRecoverable&)
////      {
////      }
////      Initialised = true;
////    }
////  }
////  return Result;
////}
//
////bool testQTubeTransformer()
////{
////  static bool Initialised = false;
////  static bool Result = false;
////  static cLock Safety;
////
////  if(!Initialised)//Double checked locking pattern 
////  {
////    cLockGuard g(&Safety);
////    if(!Initialised)
////    {
////      try
////      {
////        iOptionManager::singleton().checkOut (L"QTube Transformer Product");        
////        Result = true;
////      }
////      catch (cRecoverable&)
////      {
////      }
////      Initialised = true;
////    }
////  }
////  return Result;
////}
////
//
//cTime getLocalTime (void)
//{
//  __time64_t time;
//
//  ::_time64 (&time);
//  struct tm* pNow = ::_localtime64 (&time);
//
//  return cTime (pNow->tm_hour, pNow->tm_min, pNow->tm_sec, 0); // no millisecs
//}
//
//cDate getLocalDate (void)
//{
//  __time64_t time;
//
//  ::_time64 (&time);
//  struct tm* pNow = ::_localtime64 (&time);
//
//  return cDate (pNow->tm_year, pNow->tm_mon + 1, pNow->tm_mday);
//}
//
///////////////////////////////////////////////////////////////////////////////
//// class cRebootThread
//
//cRebootThread::cRebootThread (cQCIFSProcessor *const pQCIFSP)
//: cThread (L"Reboot Thread", iThread::eNormal)
//, m_QCIFSP (pQCIFSP)
//, m_bDoNotReboot (true)
//, m_bUserRequest (false)
//, m_bForcedRestart (false)
//{
//  cTime rebootTime (16, 3, 0, 0);
//  if (1)
//    m_bDoNotReboot = true;
//  else
//    try
//  {
//    String sRebootTime = gModuleContext->getCustomSettingString (L"Config", L"Reboot Time (24hh:mm:ss)");
//    
//    if (sRebootTime.compare (L"99:99:99") == 0)
//    {
//      m_bDoNotReboot = true;
//    }
//    else
//    {
//      int nHours;
//      int nMinutes;
//      int nSeconds;
//      int nTimeArgs = ::swscanf (sRebootTime.c_str (), L"%d:%d:%d", &nHours, &nMinutes, &nSeconds);
//
//      if (nTimeArgs == 3)
//      {
//        m_bDoNotReboot = false;
//        rebootTime = cTime (nHours, nMinutes, nSeconds, 0);
//
//      }
//      else
//      {
//        QSOS((L"cRebootThread::cRebootThread () - registry setting \"Reboot Time (24hh:mm:ss)\" has invalid format. Resetting to 00:00:00."));
//      }
//    }
//  }
//  catch (cNotFound&)
//  {
//    QSOS((L"cRebootThread::cRebootThread () - registry setting \"Reboot Time (24hh:mm:ss)\" not found. Resetting to 00:00:00."));
//  }
//
//  if (m_bDoNotReboot)
//    QTRACE((L"cRebootThread::cRebootThread () - Daily reboot Disabled"));
//  else
//    QTRACE((L"cRebootThread::cRebootThread () - Daily reboot set to %02d:%02d:%02d",rebootTime.getHour(), rebootTime.getMinute(), rebootTime.getSecond()));
//
//m_RebootDateTime = cDateTime (getLocalDate (), rebootTime);
//if (!m_bDoNotReboot)
//  setNextReboot ();
//}
//
//cRebootThread::~cRebootThread ()
//{
//}
//
//void QAPI cRebootThread::signalLoopStop (void)
//{ }
//
//void QAPI cRebootThread::runLoop (void)
//{
//  do
//  {
//    if (/*m_pGM->getCountOpenSessions () == 0 &&*/ m_bDoNotReboot == false)
//    {
//      cDateTime now (getLocalDate (), getLocalTime ());
//      if (now > m_RebootDateTime)
//      {
//        // do not open any sessions while we shutdown
//        /*m_pGM->suspendService();*/      
//
//        if (m_bUserRequest)
//        {
//          iExceptionManager::singleton().fatal (
//            0, /* flags */
//            iExceptionManager::cFatalDiagnostics (
//            EXIT_SUCCESS, "" /* src file */, 0 /* src line */, L"" /* heading */, L"Closing"));
//        }
//        else
//        {
//          if(!m_bForcedRestart)
//          {
//            QSOS((L"cRebootThread::runLoop () - reboot time reached at %s. Rebooting..."
//              , now.toString ().c_str ()));
//
//            // return all checked out licenses            
//            iOptionManager::singleton().onForcedCleanup();
//            Sleep(5000);
//          }                   
//
//          QVERIFY(0);
//        }
//      }
//    }
//
//    ::MsgWaitForMultipleObjects (0, 0, TRUE, 10000, 0); // 10 second idle
//
//  } while (true);
//}
//
//void QAPI cRebootThread::setNextReboot ()
//{
//  cDateTime now (getLocalDate (), getLocalTime ());
//
//  // next reboot should be at the next occurence of 'rebootTime'
//  if (m_RebootDateTime < now)
//  {
//    __time64_t time;
//
//    ::_time64 (&time);
//    struct tm* pNow = ::_localtime64 (&time);
//    COleDateTime dt(pNow->tm_year, pNow->tm_mon + 1, pNow->tm_mday, m_RebootDateTime.hour(), m_RebootDateTime.minute(), m_RebootDateTime.second());
//    dt += COleDateTimeSpan(1,0,0,0);
//
//    m_RebootDateTime = cDateTime (cDate (dt.GetYear(), dt.GetMonth(), dt.GetDay()), m_RebootDateTime.time());
//  }
//}
//
//void QAPI cRebootThread::forceRebootAsap (bool bIsUserRequest /*= false*/)
//{
//  __time64_t time;
//  ::_time64 (&time);
//  struct tm* pNow = ::_localtime64 (&time);
//
//  m_bUserRequest = bIsUserRequest;
//
//  if (m_bUserRequest)
//  {
//    QSOS((L"cRebootThread::forceRebootAsap () - user requested to exit."));
//  }
//  else
//  {
//    m_bForcedRestart = true;
//    QSOS((L"cRebootThread::forceRebootAsap () - fatal internal error, forcing a restart."));
//  }
//
//  // clear the 'avoid reboot' switch to avoid not handling the reboot request
//  m_bDoNotReboot = false;
//
//  // fake the last reboot DateTime to yesterday at midnight
//  // we are converting between zero based and one based, so we roll back to yesterday by not adding 1 to the day here
//  m_RebootDateTime = cDateTime (cDate (pNow->tm_year, pNow->tm_mon + 1, pNow->tm_mday), cTime(pNow->tm_hour, pNow->tm_min, pNow->tm_sec, 0));
//}
//
//String QAPI cRebootThread::autoRebootTime (void)
//{
//
//  Char mRTStr [10];
//
//  if (m_bDoNotReboot)
//    swprintf (mRTStr, L"99:99:99");
//  else
//    swprintf (mRTStr, L"%02d:%02d:%02d", m_RebootDateTime.hour(), m_RebootDateTime.minute(), m_RebootDateTime.second());
//
//  return mRTStr ;
//}
//
//
//void QAPI cRebootThread::setAutoRebootTime (vfs::String newT)
//{
//  cTime rebootTime (4, 30, 0, 0);
//  int nHours;
//  int nMinutes;
//  int nSeconds;
//  int nTimeArgs = ::swscanf (newT.c_str (), L"%d:%d:%d", &nHours, &nMinutes, &nSeconds);
//  Char newTStr [10];
//
//  if (newT.compare (L"99:99:99") == 0)
//  {
//    m_bDoNotReboot = true;
//    swprintf (newTStr, L"99:99:99");
//  }
//  else if (nTimeArgs == 3)
//  {
//    m_bDoNotReboot = false;
//    rebootTime = cTime (nHours, nMinutes, nSeconds, 0);
//    cDateTime now (getLocalDate (), getLocalTime ());
//
//    m_RebootDateTime = cDateTime (getLocalDate (), rebootTime);
//
//    swprintf (newTStr, L"%02d:%02d:%02d", rebootTime.getHour(), rebootTime.getMinute(), rebootTime.getSecond());
//
//  }
//  else
//  {
//    QSOS((L"cRebootThread::cRebootThread () - registry setting \"Reboot Time (24hh:mm:ss)\" has invalid format. Resetting to 00:00:00."));
//  }
//
//
//
//  gModuleContext->setCustomSettingString (L"Config", L"Reboot Time (24hh:mm:ss)", newTStr);
//  if (!m_bDoNotReboot)
//    setNextReboot ();
//
//}
