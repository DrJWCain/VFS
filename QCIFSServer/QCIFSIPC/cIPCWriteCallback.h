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

//cIPCWriteCallback.h

class cIPCWriteCallback : public iWriteCallback, public vfs::cRefCount
{
  unsigned __int64 FileSize;

  mutable vfs::cLock m_access;
  mutable tFrameVector m_frameVector;
  vfs::String Name;
  HANDLE HPipe;

  void setUpPipe();

public:
  cIPCWriteCallback(const vfs::String& name);

  virtual unsigned __int64 getSize(ULONGLONG fid);
  virtual DWORD readBytes(tTransmitList &krTPM, DWORD& nBytes, const LARGE_INTEGER &nOffset, const int sessionID, ULONGLONG fid);
  virtual bool canExecute() { return false; }
  virtual DWORD registerChangeListener(vfs::cPtr<iCallbackChangeListener> listener) { return ERROR_NOT_SUPPORTED; }
  virtual DWORD setSize(unsigned __int64 newSize);
  virtual DWORD writeBytes(vfs::cConstPtr<vfs::cMemoryView> buffer, const LARGE_INTEGER &offset, const int sessionID);
  virtual DWORD close(ULONGLONG fid);
};


