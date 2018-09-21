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

//cNotificationThread.h

#pragma once

class iNotificationReceiver;
class cWindowsComposite;

class cNotificationThread : public vfs::cThread
{
public:
  cNotificationThread(const HANDLE hChange
    , vfs::cPtr<iNotificationReceiver> pNotification
    , cWindowsComposite* pWindowsComposite);
  ~cNotificationThread(void);

  void QAPI runLoop();
  void QAPI signalLoopStop() {}

private:
  const HANDLE m_hChange;
  const HANDLE m_hCancelled;
  vfs::cPtr<iNotificationReceiver> m_pNotification;
  cWindowsComposite* m_pWindowsComposite;
  bool m_bCancelled;
};
