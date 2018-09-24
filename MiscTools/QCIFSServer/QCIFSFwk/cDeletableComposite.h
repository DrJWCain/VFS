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

//cDeletableComposite.h

#pragma once

#include "cAttributeComposite.h"
#include "cSearchListener.h"

class cDeletableComposite : public cAttributeComposite
{
  const vfs::cPtr<iFileEvent> m_pFileEvent;

protected:
  const vfs::cPtr<cSearchListener> m_pParentSearchListener;

public:
  cDeletableComposite(const fileInfo& fi
    , const vfs::cPtr<cSearchListener> pParentSearchListener
    , const vfs::cPtr<iFileEvent> pFileEvent
    , const bool bDeletable);
  ~cDeletableComposite(void);

  DWORD Delete();
  void triggerNotifications(){/*NOP*/}
  DWORD registerNotification(const ULONGLONG requestID
    , vfs::cPtr<iNotificationReceiver> pNotification) {return ERROR_ACCESS_DENIED;}
  DWORD cancelNotification(const ULONGLONG requestID, bool synchronous) {return ERROR_ACCESS_DENIED;}

  virtual bool canBeDeleted() { return canDelete(); }

protected:
  DWORD Delete(const bool bDeletable);
  virtual bool canDelete() const {return m_bDeletable;}

private:
  const bool m_bDeletable;
};
