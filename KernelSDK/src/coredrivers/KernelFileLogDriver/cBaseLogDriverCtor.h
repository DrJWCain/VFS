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

#pragma once

namespace vfs
{
  class cBaseLogDriverCtor : public iFactory::iConstructor, public cRefCount
  {
  public:
    // Constructor creates an instance of cBaseLogDriver (mSingleton) depending on registry settings
    cBaseLogDriverCtor();

    // Emulates singleton pattern
    iFactory::iClassInfo::ConstPtr QAPI getClassInfo() const { return new cBaseLogDriver::class_info; }
    iFactory::iInstance::Ptr QAPI createInstance() { return mSingleton; }
    iFactory::iInstance::Ptr QAPI createInstanceWithParams(const iFactory::iConstructor::iParameters & Params) { throw cNotSupported(__FILE__, __LINE__, L"No parameterised \"cBaseLogDriver\" ctor"); }

    // Helper function to discover where logs are stored
    static const String& getLogFolder();

  private:
    cPtr<cBaseLogDriver> mSingleton;
  };
}
