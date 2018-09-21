=========================================================================
    EXTENDED STORED PROCEDURE : QCIFSSrv Project Overview
========================================================================


AppWizard has created QCIFSSrv.dll for you.  

This file contains a summary of what you will find in each of the files that
make up your QCIFSSrv application.

QCIFSSrv.vcproj
    This is the main project file for VC++ projects generated using an Application Wizard. 
    It contains information about the version of Visual C++ that generated the file, and 
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

QCIFSSrv.cpp
    This is the main dll source file.

proc.cpp
    This file contains the stored procedure xp_proc

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named QCIFSSrv.pch and a precompiled types file named StdAfx.obj.


/////////////////////////////////////////////////////////////////////////////
Other notes:

After completing this Wizard, copy the QCIFSSrv.dll over to your SQL Server 
\Binn directory.

Add your new Extended Stored Procedure from a Visual Studio Data Project, 
or using the SQL Server Enterprise Manager, or by executing the following 
SQL command:
  sp_addextendedproc 'xp_proc', 'QCIFSSrv.DLL'

You may drop the extended stored procedure by using the SQL command:
  sp_dropextendedproc 'xp_proc'

You may release the DLL from the Server (to delete or replace the file), by 
using the SQL command:
  DBCC xp_proc(FREE)


/////////////////////////////////////////////////////////////////////////////
