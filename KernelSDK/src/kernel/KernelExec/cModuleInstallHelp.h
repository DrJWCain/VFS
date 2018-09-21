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
/*
**  
*/

#include "QKernelExec.h"
#include "cProcessHelp.h"

using namespace std;
using namespace vfs;




// =======================================================================================
// === cModuleInstallHelp ================================================================
// =======================================================================================

class cModuleInstallHelp : public iModuleInstallHelp, public cRefCount
{
	public :

    QDEFINE_SMARTPTR(cModuleInstallHelp);

    // iModuleInstallHelp
    virtual void QAPI regsvr32(const String & DllFilePath) throw(cNotFound, cWin32Error);
    virtual void QAPI unregsvr32(const String & DllFilePath) throw(cNotFound, cWin32Error);
    virtual void QAPI executeProcess(const String &  ExePath, const String &  ExeFileName, const String &  ExeParams, const int Flags) throw(cWin32Error);
    virtual String QAPI getWin32RootFolder() const { return m_SystemRootFolder; }
    virtual void QAPI copyFileOrFolder(const String & SrcFilePath, const String & DstFilePath) throw(cBadFormat, cFileError);
    virtual void QAPI killProcess(const String & ProcessName, const unsigned TimeoutMillisecs) throw(cNotFound, cWin32Error);
    virtual bool QAPI isProcessRunning(const String & ProcessName) throw(cNotFound, cWin32Error);
    virtual void QAPI installSharedDll(const String & SrcDllPath) throw(cBadFormat, cFileError, cWin32Error);
    virtual void QAPI deprecated0() {}
    void QAPI uninstallSharedDll(const String & DllFilename) throw(cBadFormat, cFileError);
    virtual void QAPI executeProcessWait(const String &  ExePath, const String &  ExeFileName, const String &  ExeParams, const unsigned int TimeoutMillisecs, const int Flags) throw(cWin32Error, cTimedOut);
    // deprecated
    virtual cPtr<iRegistryHelp> QAPI createRegistryHelp() { return ::createRegistryHelp(); }
    virtual cPtr<iRegistryHelp> QAPI createCurrentUserRegistryHelp() { return ::createCurrentUserRegistryHelp(); }

    // cModuleInstallHelp
		cModuleInstallHelp();

	private :

    String            m_SystemRootFolder;
    cProcessHelp::Ptr m_ProcessHelp;
};
