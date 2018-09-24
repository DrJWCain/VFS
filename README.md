# SMBServer VFS

Welcome to the Windows SMBServer Virtual File System environment!
This code base offers an SMB2/3 server running on Windows in User mode.
This code has the _potential_ to support appications on Windows in the style of Fuse in the *nix world.

## Build
We use the Microsoft Vsiual Studio C++ build environment. 
The code is configured for VS 2015 or later.
This is a 64 bit project.

### Dependencies
The code only has few dependenies:
1. Windows SDK
Comes with Visual Studio.

2. Boost
Available from [boost](https://www.boost.org/)

To configure boost, download the latest relase of boost headers and libaries built for Windows 64 bit.
Update the props file in \props folder to have the correct path.

So in props\Boost64.props edit this line:
> &lt;BOOST_DIR&gt;D:\build\Boost\boost_1_65_1&lt;/BOOST_DIR&gt;

Now, open __vfs.sln__ in Visual Studio, and build _release 64-bit_.
There are some warnings, but all dlls and one exe should build error free.

## Config
SMB2/3 runs over TCP on well known port numbers. The main port (now-a-days) is port 445. The Windows implementaions do not allow deviating from this port, so the 1st thing to do in order to run a Windows User mode SMB2/3 server is to free port 445 from the clutches of the Windows Kernel.

This code base offers two ways to achive this.

### 1. Free445
There is a stand alone subproject called Free445. Build this, and then run it as local admin on your machine.

It will disconnect the SMB server built into Windows from the Netbios transport.
This has the effect of freeing the 445 port, so that we can then bind our server to it (The Windows Server service is still running - we'll be using this later).
The port 445 will remain free for the duration of this boot of Windows. 
On re-booting, port 445 will again be bound to the Kernel mode MS supplied SMB server.

### 2. Windows Service
Running a local admin program each time we want to run our SMBServer is not a good apprach, so the SMBserver code also come supplied with a Windows Service called NetManService.
This contains the same code as the Free445, but wrapped in a Windows service, so that after initial installation (that requires elevated permissions), the service can be run from a non-elevated executable to achieve the same result - freeing port 445.

To get this working, open NetManService\NetManService.sln in a new copy of Visual Studio, and build 64bit release. This makes a new exe under Data\x64\DLL_Data\vfs\QCIFSProcessor\NetManService. 
In order to install the service we need to run with elevated privileges. We also need to clear a reg key, to get the QCIFSProcessot.dll to re-install itself.
Therefore open regedit at: HKEY_LOCAL_MACHINE\SOFTWARE\vfs\SMB3 Server\Vfs. Find and delete the key called 'QCIFSProcessor'. This will cause the QCIFSProcessor.dll to re-install, and install the service as a consequence.

## Running
RUNME.bat runs KernelServer.exe built in the **build** step. This exe has no knowledge of SMB, it just loads dlls from pre-configured directorties, and offers simple services to register factory functions to allow services to be called at runtime. This is an example of the inversion of control design principle [IoC](https://en.wikipedia.org/wiki/Inversion_of_control).

On booting successfully the set of DLLs supplied should load some logging facilities (both console and file based logging are built in), and it loads the set of DLLs that implement our SMB2/3 server.

All being well, the console should look something like this:

![Booted OK](Docs/booted.PNG)

## Validating the boot
Now, check the boot log, by scolling back in the console window.

We need to establish if we managed to bind to port 445.
All being well, you'll see this:

![Happy](Docs/Happy.PNG)

However, if for some reason port 445 cannot be bound to, you'll see something like this:

![Sad](Docs/Sad.PNG)

# Architecture
See the presentation first given at SNIA SDC 2018 for architecture details, available in the root directory of this code base. 

# Contributing
Please read CONTRIBUTING.md for details on our code of conduct, and the process for submitting pull requests to us.

# Versioning
We use [SemVer](https://semver.org/) for versioning. For the versions available, see the tags on this repository.

# Authors
James Cain - Initial work - Grass Valley, A Belden Brand

# Acknowledgments
Thanks to (current and former) colleagues for helping build this code.

Special mentions go out to:
- Michael Weaver
- Fang Ren
- Rob Hinchliff 

Additional thanks are due to the following:
- All the sponsors and attendees of the SNIA SDC SMB plugfest over the years
- The Grass Valley Management for having the forsight to support this open source development.
