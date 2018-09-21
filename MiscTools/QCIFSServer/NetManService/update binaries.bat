
cd ..\..\..\Data\Run


cleartool co -unreserved -c "NetManService update" DLL_Data\vfs\QCIFSProcessor\NetManService\KernelExec.dll
cleartool co -unreserved -c "NetManService update" DLL_Data\vfs\QCIFSProcessor\NetManService\KernelExec.pdb
cleartool co -unreserved -c "NetManService update" DLL_Data\vfs\QCIFSProcessor\NetManService\KernelExec.reg

cleartool co -unreserved -c "NetManService update" DLL_Data\vfs\QCIFSProcessor\NetManService\KernelModules\vfs_KernelDebugDriver.dll
cleartool co -unreserved -c "NetManService update" DLL_Data\vfs\QCIFSProcessor\NetManService\KernelModules\vfs_KernelDebugDriver.pdb
cleartool co -unreserved -c "NetManService update" DLL_Data\vfs\QCIFSProcessor\NetManService\KernelModules\vfs_KernelDebugDriver.reg

cleartool co -unreserved -c "NetManService update" DLL_Data\vfs\QCIFSProcessor\NetManService\KernelModules\vfs_KernelConsoleLogDriver.dll
cleartool co -unreserved -c "NetManService update" DLL_Data\vfs\QCIFSProcessor\NetManService\KernelModules\vfs_KernelConsoleLogDriver.pdb
cleartool co -unreserved -c "NetManService update" DLL_Data\vfs\QCIFSProcessor\NetManService\KernelModules\vfs_KernelConsoleLogDriver.reg

cleartool co -unreserved -c "NetManService update" DLL_Data\vfs\QCIFSProcessor\NetManService\KernelModules\vfs_KernelFileLogDriver.dll
cleartool co -unreserved -c "NetManService update" DLL_Data\vfs\QCIFSProcessor\NetManService\KernelModules\vfs_KernelFileLogDriver.pdb
cleartool co -unreserved -c "NetManService update" DLL_Data\vfs\QCIFSProcessor\NetManService\KernelModules\vfs_KernelFileLogDriver.reg

if not exist DLL_Data\vfs\QCIFSProcessor\NetManService\KernelModules\   mkdir DLL_Data\vfs\QCIFSProcessor\NetManService\KernelModules\

copy ..\..\KernelSDK\redist\win32\System\KernelExec.dll DLL_Data\vfs\QCIFSProcessor\NetManService\
copy ..\..\KernelSDK\redist\win32\System\KernelExec.pdb DLL_Data\vfs\QCIFSProcessor\NetManService\
copy ..\..\KernelSDK\redist\win32\System\KernelExec.reg DLL_Data\vfs\QCIFSProcessor\NetManService\

copy ..\..\KernelSDK\redist\win32\DLL_Code_Release\vfs_KernelDebugDriver.dll DLL_Data\vfs\QCIFSProcessor\NetManService\KernelModules\
copy ..\..\KernelSDK\redist\win32\DLL_Code_Release\vfs_KernelDebugDriver.pdb DLL_Data\vfs\QCIFSProcessor\NetManService\KernelModules\
copy ..\..\KernelSDK\redist\win32\DLL_Registry\vfs\KernelDebugDriver.reg DLL_Data\vfs\QCIFSProcessor\NetManService\KernelModules\vfs_KernelDebugDriver.reg

copy ..\..\KernelSDK\redist\win32\DLL_Code_Release\vfs_KernelConsoleLogDriver.dll DLL_Data\vfs\QCIFSProcessor\NetManService\KernelModules\
copy ..\..\KernelSDK\redist\win32\DLL_Code_Release\vfs_KernelConsoleLogDriver.pdb DLL_Data\vfs\QCIFSProcessor\NetManService\KernelModules\
copy ..\..\KernelSDK\redist\win32\DLL_Registry\vfs\KernelConsoleLogDriver.reg DLL_Data\vfs\QCIFSProcessor\NetManService\KernelModules\vfs_KernelConsoleLogDriver.reg

copy ..\..\KernelSDK\redist\win32\DLL_Code_Release\vfs_KernelFileLogDriver.dll DLL_Data\vfs\QCIFSProcessor\NetManService\KernelModules\
copy ..\..\KernelSDK\redist\win32\DLL_Code_Release\vfs_KernelFileLogDriver.pdb DLL_Data\vfs\QCIFSProcessor\NetManService\KernelModules\
copy ..\..\KernelSDK\redist\win32\DLL_Registry\vfs\KernelFileLogDriver.reg DLL_Data\vfs\QCIFSProcessor\NetManService\KernelModules\vfs_KernelFileLogDriver.reg

pause