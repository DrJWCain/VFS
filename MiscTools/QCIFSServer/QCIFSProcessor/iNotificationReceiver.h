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

//iNotificationReceiver.h

#pragma once

//CIFS Notify flags
#define FILE_ACTION_ADDED             0x00000001
#define FILE_ACTION_REMOVED           0x00000002
#define FILE_ACTION_MODIFIED          0x00000003
#define FILE_ACTION_RENAMED_OLD_NAME  0x00000004
#define FILE_ACTION_RENAMED_NEW_NAME  0x00000005
#define FILE_ACTION_ADDED_STREAM      0x00000006
#define FILE_ACTION_REMOVED_STREAM    0x00000007
#define FILE_ACTION_MODIFIED_STREAM   0x00000008

//SMB2 Notify flags
#define FILE_NOTIFY_CHANGE_FILE_NAME 0x00000001 //The client is notified if a file-name changes. 
#define FILE_NOTIFY_CHANGE_DIR_NAME 0x00000002 //The client is notified if a directory name changes. 
#define FILE_NOTIFY_CHANGE_ATTRIBUTES 0x00000004 //The client is notified if a file's attributes change. Possible file attribute values are specified in [MS-FSCC] 2.6. 
#define FILE_NOTIFY_CHANGE_SIZE 0x00000008 //The client is notified if a file's size changes. 
#define FILE_NOTIFY_CHANGE_LAST_WRITE 0x00000010 //The client is notified if the last write time of a file changes. 
#define FILE_NOTIFY_CHANGE_LAST_ACCESS 0x00000020 //The client is notified if the last access time of a file changes. 
#define FILE_NOTIFY_CHANGE_CREATION 0x00000040 //The client is notified if the creation time of a file changes. 
#define FILE_NOTIFY_CHANGE_EA 0x00000080 //The client is notified if a file's extended attributes (EAs) change. 
#define FILE_NOTIFY_CHANGE_SECURITY 0x00000100 //The client is notified of a file's access control list (ACL) settings change. 
#define FILE_NOTIFY_CHANGE_STREAM_NAME 0x00000200 //The client is notified if a named stream is added to a file. 
#define FILE_NOTIFY_CHANGE_STREAM_SIZE 0x00000400 //The client is notified if the size of a named stream is changed. 
#define FILE_NOTIFY_CHANGE_STREAM_WRITE 0x00000800 //The client is notified if a named stream is modified.

class iNotificationTarget
{
public:
  typedef std::pair<DWORD, vfs::String> tNotifyInfo;
  typedef std::vector<tNotifyInfo> tNotifyList;

  virtual ~iNotificationTarget() {}

  virtual DWORD Notify() = 0;
  virtual DWORD Notify(const tNotifyList& notifyInfo) = 0;
  virtual DWORD CancelNotification(bool synchronous) = 0;
};

class iNotificationReceiver : public iNotificationTarget
{
public:
  virtual ~iNotificationReceiver() {}

  virtual BOOL WatchSubtree() = 0;
  virtual DWORD SearchFilter() = 0;
};