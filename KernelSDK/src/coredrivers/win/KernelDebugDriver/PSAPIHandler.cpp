/*----------------------------------------------------------------------
 John Robbins - Microsoft Systems Journal Bugslayer Column - August '98
 Copyright 1998, Microsoft Systems Journal.
----------------------------------------------------------------------*/


// --- Headers ---------------------------------------------------------

#include "StdAfx.h"

// --- Use Namespace ---------------------------------------------------

using namespace vfs;

// --- Headers ---------------------------------------------------------

#include "PSAPIHandler.h"

/*//////////////////////////////////////////////////////////////////////
                                Typedefs
//////////////////////////////////////////////////////////////////////*/
// The typedefs for the PSAPI.DLL functions used by this module.
typedef BOOL (WINAPI *ENUMPROCESSMODULES) ( HANDLE    hProcess   ,
                                            HMODULE * lphModule  ,
                                            DWORD     cb         ,
                                            LPDWORD   lpcbNeeded  ) ;

typedef DWORD (WINAPI *GETMODULEBASENAME) ( HANDLE  hProcess   ,
                                            HMODULE hModule    ,
                                            LPSTR  lpBaseName ,
                                            DWORD   nSize       ) ;




/*//////////////////////////////////////////////////////////////////////
                            File Static Data
//////////////////////////////////////////////////////////////////////*/
// Has the function stuff here been initialized?  This is only to be
//  used by the InitPSAPI function and nothing else.
static BOOL g_bInitialized = FALSE ;
// The pointer to EnumProcessModules.
static ENUMPROCESSMODULES g_pEnumProcessModules = NULL ;
// The pointer to GetModuleBaseName.
static GETMODULEBASENAME g_pGetModuleBaseName = NULL ;

/*----------------------------------------------------------------------
FUNCTION        :   InitPSAPI
DISCUSSION      :
    Loads PSAPI.DLL and initializes all the pointers needed by this
file.  If BugslayerUtil.DLL statically linked to PSAPI.DLL, it would not
work on Windows95.
    Note that I conciously chose to allow the resource leak on loading
PSAPI.DLL.
PARAMETERS      :
    None.
RETURNS         :
    TRUE  - Everything initialized properly.
    FALSE - There was a problem.
----------------------------------------------------------------------*/
static BOOL InitPSAPI ( void )
{
    if ( TRUE == g_bInitialized )
    {
        return ( TRUE ) ;
    }

    // Load up PSAPI.DLL.
    HINSTANCE hInst = LoadLibraryA ( "PSAPI.DLL" ) ;
    QVERIFY ( NULL != hInst ) ;
    if ( NULL == hInst )
    {
        QSOS (( L"Unable to load PSAPI.DLL!\n" )) ;
        return ( FALSE ) ;
    }

    // Now do the GetProcAddress stuff.
    g_pEnumProcessModules =
           (ENUMPROCESSMODULES)GetProcAddress ( hInst ,
                                                "EnumProcessModules" ) ;
    QVERIFY ( NULL != g_pEnumProcessModules ) ;
    if ( NULL == g_pEnumProcessModules )
    {
        QSOS (( L"GetProcAddress failed on EnumProcessModules!" )) ;
        return ( FALSE ) ;
    }

    g_pGetModuleBaseName =
        (GETMODULEBASENAME)GetProcAddress ( hInst ,
                                            "GetModuleBaseNameA" ) ;
    QVERIFY ( NULL != g_pGetModuleBaseName ) ;
    if ( NULL == g_pGetModuleBaseName )
    {
        QSOS (( L"GetProcAddress failed on GetModuleBaseNameA!" )) ;
        return ( FALSE ) ;
    }

    // All OK, Jumpmaster!
    g_bInitialized = TRUE ;
    return ( TRUE ) ;
}

/*----------------------------------------------------------------------
FUNCTION        :   GetLoadedModules
DISCUSSION      :
    The NT4 specific version of GetLoadedModules.  This function assumes
that GetLoadedModules does the work to validate the parameters.
PARAMETERS      :
    dwPID        - The process ID to look into.
    uiCount      - The number of slots in the paModArray buffer.  If
                   this value is 0, then the return value will be TRUE
                   and puiRealCount will hold the number of items
                   needed.
    paModArray   - The array to place the HMODULES into.  If this buffer
                   is too small to hold the result and uiCount is not
                   zero, then FALSE is returned, but puiRealCount will
                   be the real number of items needed.
    puiRealCount - The count of items needed in paModArray, if uiCount
                   is zero, or the real number of items in paModArray.
RETURNS         :
    FALSE - There was a problem, check GetLastError.
    TRUE  - The function succeeded.  See the parameter discussion for
            the output parameters.
----------------------------------------------------------------------*/
BOOL GetLoadedModules ( DWORD     dwPID        ,
                        UINT      uiCount      ,
                        HMODULE * paModArray   ,
                        LPUINT    puiRealCount   )
{

    // Initialize PSAPI.DLL, if needed.
    if ( FALSE == InitPSAPI ( ) )
    {
        QVERIFY ( FALSE ) ;
        SetLastErrorEx ( ERROR_DLL_INIT_FAILED , SLE_ERROR ) ;
        return ( FALSE ) ;
    }

    // Convert the process ID into a process handle.
    HANDLE hProc = OpenProcess ( PROCESS_QUERY_INFORMATION |
                                    PROCESS_VM_READ         ,
                                 FALSE                      ,
                                 dwPID                       ) ;
    QVERIFY ( NULL != hProc ) ;
    if ( NULL == hProc )
    {
        QSOS (( L"Unable to OpenProcess on %08X\n" , dwPID )) ;
        return ( FALSE ) ;
    }

    // Now get the modules for the specified process.
    QVERIFY ( NULL != g_pEnumProcessModules ) ;
    // Because of possible DLL unload order differences, make sure that
    //  PSAPI.DLL is still loaded in case this function is called during
    //  shutdown.
    QVERIFY ( FALSE == IsBadCodePtr ( (FARPROC)g_pEnumProcessModules ) );
    if ( TRUE == IsBadCodePtr ( (FARPROC)g_pEnumProcessModules ) )
    {
        QSOS (( L"PSAPI.DLL has been unloaded on us!\n" )) ;

        // Close the process handle used.
        QVERIFY ( CloseHandle ( hProc ) ) ;

        SetLastErrorEx ( ERROR_INVALID_DLL , SLE_ERROR ) ;

        return ( FALSE ) ;
    }

    DWORD dwTotal = 0 ;
    BOOL bRet = g_pEnumProcessModules ( hProc                        ,
                                        paModArray                   ,
                                        uiCount * sizeof ( HMODULE ) ,
                                        &dwTotal                      );

    // Close the process handle used.
    QVERIFY ( CloseHandle ( hProc ) ) ;

    // Convert the count from bytes to HMODULE values.
    *puiRealCount = dwTotal / sizeof ( HMODULE ) ;

    // If bRet was FALSE, and the user was not just asking for the
    //  total, there was a problem.
    if ( ( ( FALSE == bRet ) && ( uiCount > 0 ) ) || ( 0 == dwTotal ) )
    {
        QVERIFY ( FALSE ) ;
        QSOS (( L"EnumProcessModules failed!\n" )) ;
        return ( FALSE ) ;
    }

    // If the total returned in puiRealCount is larger than the value in
    //  uiCount, then return an error.  If uiCount is zero, then it is
    //  not an error.
    if ( ( *puiRealCount > uiCount ) && ( uiCount > 0 ) )
    {
        QVERIFY ( FALSE ) ;
        QSOS (( L"Buffer is too small in GetLoadedModules!" )) ;
        SetLastErrorEx ( ERROR_INSUFFICIENT_BUFFER , SLE_ERROR ) ;
        return ( FALSE ) ;
    }

    // All OK, Jumpmaster!
    SetLastError ( ERROR_SUCCESS ) ;
    return ( TRUE ) ;
}


DWORD NTGetModuleBaseName ( HANDLE  hProcess   ,
                            HMODULE hModule    ,
                            LPSTR  lpBaseName ,
                            DWORD   nSize       )
{
    // Initialize PSAPI.DLL, if needed.
    if ( FALSE == InitPSAPI ( ) )
    {
        QVERIFY ( FALSE ) ;
        SetLastErrorEx ( ERROR_DLL_INIT_FAILED , SLE_ERROR ) ;
        return ( FALSE ) ;
    }
    return ( g_pGetModuleBaseName ( hProcess    ,
                                    hModule     ,
                                    lpBaseName  ,
                                    nSize        ) ) ;
}

