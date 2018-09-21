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

#ifndef __BASICFILE__
#define __BASICFILE__

#include "..\QCIFSFwk\iChildLoaderVisitor.h"

namespace vfs
{
  class cBasicFile : public iReadCallback, public cRefCount
  {
  public:
      QDEFINE_SMARTPTR(cBasicFile);
      cBasicFile(const String& filename);
      ~cBasicFile();

      //iReadCallback
      unsigned __int64 getSize(ULONGLONG fid) { return mSize; }
      DWORD readBytes(tTransmitList &krTPM, DWORD& nBytes, const LARGE_INTEGER &nOffset, const int sessionID, ULONGLONG fid);
      bool canExecute() { return false; }
      DWORD registerChangeListener(vfs::cPtr<iCallbackChangeListener> listener) { return ERROR_NOT_SUPPORTED; }
      WIN32_FILE_ATTRIBUTE_DATA getFileAttributes();

  private:
    unsigned __int64 mSize;
    String mFilepath;
  };
};

#endif // __BASICFILE__
