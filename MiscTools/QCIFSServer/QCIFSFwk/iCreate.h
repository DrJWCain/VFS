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

//iCreate.h

#pragma once

#include "..\iFileProcessingCallback.h"

class iCreate
{
public:
  virtual ~iCreate(){}

  virtual DWORD Directory(const vfs::String& sName, LPSECURITY_ATTRIBUTES lpAtts) = 0;
  virtual DWORD File(const vfs::String& sName, LPSECURITY_ATTRIBUTES lpAtts, const DWORD dwAtts, vfs::cPtr<iFileProcessingCallback> pCallback) = 0;
};