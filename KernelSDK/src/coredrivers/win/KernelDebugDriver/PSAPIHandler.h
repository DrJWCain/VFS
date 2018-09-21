/*----------------------------------------------------------------------
 John Robbins - Microsoft Systems Journal Bugslayer Column - August '98
 Copyright 1998, Microsoft Systems Journal.
----------------------------------------------------------------------*/

//PSAPIHandler.h

BOOL GetLoadedModules ( DWORD     dwPID        ,
                        UINT      uiCount      ,
                        HMODULE * paModArray   ,
                        LPUINT    puiRealCount  );

DWORD NTGetModuleBaseName ( HANDLE  hProcess   ,
                            HMODULE hModule    ,
                            LPSTR  lpBaseName  ,
                            DWORD   nSize       );


