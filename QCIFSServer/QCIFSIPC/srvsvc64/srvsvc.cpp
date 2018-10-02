// srvsvc.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <string>
#include <sstream>

//#include "Rpcproxy.h"
#define CINTERFACE 
#include <ObjIdlbase.h>
#include <objidl.h> 
#include <RpcProxy.h> 
#undef CINTERFACE 

#pragma comment (lib, "Rpcrt4.lib")
#pragma warning(disable: 4100)

extern "C" {
  #include "srvsvc.h"
  extern const MIDL_STUB_DESC srvsvc_StubDesc;
};


extern "C"
{
  void
    RPC_ENTRY
    MyNdrServerCall2(
      PRPC_MESSAGE                pRpcMsg
    )
  {
    return NdrServerCall2(pRpcMsg);
  }
}

NET_API_STATUS Opnum0NotUsedOnWire(/* [unique][string][in] */ SRVSVC_HANDLE ServerName) { return ERROR_CALL_NOT_IMPLEMENTED; }


NET_API_STATUS Opnum1NotUsedOnWire(/* [unique][string][in] */ SRVSVC_HANDLE ServerName) { return ERROR_CALL_NOT_IMPLEMENTED; }
NET_API_STATUS Opnum2NotUsedOnWire(/* [unique][string][in] */ SRVSVC_HANDLE ServerName) { return ERROR_CALL_NOT_IMPLEMENTED; }
NET_API_STATUS Opnum3NotUsedOnWire(/* [unique][string][in] */ SRVSVC_HANDLE ServerName) { return ERROR_CALL_NOT_IMPLEMENTED; }
NET_API_STATUS Opnum4NotUsedOnWire(/* [unique][string][in] */ SRVSVC_HANDLE ServerName) { return ERROR_CALL_NOT_IMPLEMENTED; }
NET_API_STATUS Opnum5NotUsedOnWire(/* [unique][string][in] */ SRVSVC_HANDLE ServerName) { return ERROR_CALL_NOT_IMPLEMENTED; }
NET_API_STATUS Opnum6NotUsedOnWire(/* [unique][string][in] */ SRVSVC_HANDLE ServerName) { return ERROR_CALL_NOT_IMPLEMENTED; }
NET_API_STATUS Opnum7NotUsedOnWire(/* [unique][string][in] */ SRVSVC_HANDLE ServerName) { return ERROR_CALL_NOT_IMPLEMENTED; }

NET_API_STATUS NetrConnectionEnum(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [unique][string][in] */ WCHAR *Qualifier,
  /* [out][in] */ LPCONNECT_ENUM_STRUCT InfoStruct,
  /* [in] */ DWORD PreferedMaximumLength,
  /* [out] */ DWORD *TotalEntries,
  /* [unique][out][in] */ DWORD *ResumeHandle)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrFileEnum(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [unique][string][in] */ WCHAR *BasePath,
  /* [unique][string][in] */ WCHAR *UserName,
  /* [out][in] */ PFILE_ENUM_STRUCT InfoStruct,
  /* [in] */ DWORD PreferedMaximumLength,
  /* [out] */ DWORD *TotalEntries,
  /* [unique][out][in] */ DWORD *ResumeHandle)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrFileGetInfo(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [in] */ DWORD FileId,
  /* [in] */ DWORD Level,
  /* [switch_is][out] */ LPFILE_INFO InfoStruct)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrFileClose(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [in] */ DWORD FileId)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrSessionEnum(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [unique][string][in] */ WCHAR *ClientName,
  /* [unique][string][in] */ WCHAR *UserName,
  /* [out][in] */ PSESSION_ENUM_STRUCT InfoStruct,
  /* [in] */ DWORD PreferedMaximumLength,
  /* [out] */ DWORD *TotalEntries,
  /* [unique][out][in] */ DWORD *ResumeHandle)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrSessionDel(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [unique][string][in] */ WCHAR *ClientName,
  /* [unique][string][in] */ WCHAR *UserName)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrShareAdd(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [in] */ DWORD Level,
  /* [switch_is][in] */ LPSHARE_INFO InfoStruct,
  /* [unique][out][in] */ DWORD *ParmErr)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}


const int STYPE_DISKTREE = 0;
NET_API_STATUS NetrShareEnum(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [out][in] */ LPSHARE_ENUM_STRUCT InfoStruct,
  /* [in] */ DWORD PreferedMaximumLength,
  /* [out] */ DWORD *TotalEntries,
  /* [unique][out][in] */ DWORD *ResumeHandle)
{
  printf("%s %S, %d %p %p %d %p\n", __FUNCTION__, ServerName, InfoStruct->Level, InfoStruct, InfoStruct->ShareInfo.Level1, InfoStruct->ShareInfo.Level1->EntriesRead, InfoStruct->ShareInfo.Level1->Buffer);
  if(1 == InfoStruct->Level)
  {
    InfoStruct->ShareInfo.Level1->EntriesRead = 1;
    InfoStruct->ShareInfo.Level1->Buffer = (LPSHARE_INFO_1)midl_user_allocate(sizeof(SHARE_INFO_1));
    memset(InfoStruct->ShareInfo.Level1->Buffer, 0, sizeof(SHARE_INFO_1));

    InfoStruct->ShareInfo.Level1->Buffer[0].shi1_netname = (WCHAR *)midl_user_allocate(20);
    wcscpy_s(InfoStruct->ShareInfo.Level1->Buffer[0].shi1_netname, 20, L"test");

    InfoStruct->ShareInfo.Level1->Buffer[0].shi1_type = STYPE_DISKTREE;

    InfoStruct->ShareInfo.Level1->Buffer[0].shi1_remark = (WCHAR *)midl_user_allocate(40);
    wcscpy_s(InfoStruct->ShareInfo.Level1->Buffer[0].shi1_remark, 40, L"fred was here");
  }
  if(502 == InfoStruct->Level)
  {
    InfoStruct->ShareInfo.Level502->EntriesRead = 1;
    InfoStruct->ShareInfo.Level502->Buffer = (LPSHARE_INFO_502_I)midl_user_allocate(sizeof(SHARE_INFO_502_I));
    memset(InfoStruct->ShareInfo.Level502->Buffer, 0, sizeof(SHARE_INFO_502_I));

    InfoStruct->ShareInfo.Level502->Buffer[0].shi502_netname = (WCHAR *)midl_user_allocate(20);
    wcscpy_s(InfoStruct->ShareInfo.Level502->Buffer[0].shi502_netname, 20, L"test");

    InfoStruct->ShareInfo.Level502->Buffer[0].shi502_type = STYPE_DISKTREE;

    InfoStruct->ShareInfo.Level502->Buffer[0].shi502_remark = (WCHAR *)midl_user_allocate(40);
    wcscpy_s(InfoStruct->ShareInfo.Level502->Buffer[0].shi502_remark, 40, L"fred was here");

    InfoStruct->ShareInfo.Level502->Buffer[0].shi502_path = (WCHAR *)midl_user_allocate(50);
    wcscpy_s(InfoStruct->ShareInfo.Level502->Buffer[0].shi502_path, 40, L"VFS magic path");

    InfoStruct->ShareInfo.Level502->Buffer[0].shi502_max_uses = 0xffffffff;//unlimited.

    InfoStruct->ShareInfo.Level502->Buffer[0].shi502_reserved = sizeof(SECURITY_DESCRIPTOR);
    InfoStruct->ShareInfo.Level502->Buffer[0].shi502_security_descriptor = (unsigned char*)midl_user_allocate(sizeof(SECURITY_DESCRIPTOR));
    InitializeSecurityDescriptor(InfoStruct->ShareInfo.Level502->Buffer[0].shi502_security_descriptor, SECURITY_DESCRIPTOR_REVISION);

  }
  printf("%s %S, %d %p %p %d %p - OK\n", __FUNCTION__, ServerName, InfoStruct->Level, InfoStruct, InfoStruct->ShareInfo.Level1, InfoStruct->ShareInfo.Level1->EntriesRead, InfoStruct->ShareInfo.Level1->Buffer);

  return ERROR_SUCCESS;
}

NET_API_STATUS NetrShareGetInfo(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [string][in] */ WCHAR *NetName,
  /* [in] */ DWORD Level,
  /* [switch_is][out] */ LPSHARE_INFO InfoStruct)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrShareSetInfo(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [string][in] */ WCHAR *NetName,
  /* [in] */ DWORD Level,
  /* [switch_is][in] */ LPSHARE_INFO ShareInfo,
  /* [unique][out][in] */ DWORD *ParmErr)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrShareDel(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [string][in] */ WCHAR *NetName,
  /* [in] */ DWORD Reserved)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrShareDelSticky(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [string][in] */ WCHAR *NetName,
  /* [in] */ DWORD Reserved)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrShareCheck(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [string][in] */ WCHAR *Device,
  /* [out] */ DWORD *Type)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrServerGetInfo(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [in] */ DWORD Level,
  /* [switch_is][out] */ LPSERVER_INFO InfoStruct)
{
  printf("%s %d\n", __FUNCTION__, Level);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrServerSetInfo(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [in] */ DWORD Level,
  /* [switch_is][in] */ LPSERVER_INFO ServerInfo,
  /* [unique][out][in] */ DWORD *ParmErr)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrServerDiskEnum(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [in] */ DWORD Level,
  /* [out][in] */ DISK_ENUM_CONTAINER *DiskInfoStruct,
  /* [in] */ DWORD PreferedMaximumLength,
  /* [out] */ DWORD *TotalEntries,
  /* [unique][out][in] */ DWORD *ResumeHandle)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrServerStatisticsGet(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [unique][string][in] */ WCHAR *Service,
  /* [in] */ DWORD Level,
  /* [in] */ DWORD Options,
  /* [out] */ LPSTAT_SERVER_0 *InfoStruct)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrServerTransportAdd(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [in] */ DWORD Level,
  /* [in] */ LPSERVER_TRANSPORT_INFO_0 Buffer)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrServerTransportEnum(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [out][in] */ LPSERVER_XPORT_ENUM_STRUCT InfoStruct,
  /* [in] */ DWORD PreferedMaximumLength,
  /* [out] */ DWORD *TotalEntries,
  /* [unique][out][in] */ DWORD *ResumeHandle)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrServerTransportDel(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [in] */ DWORD Level,
  /* [in] */ LPSERVER_TRANSPORT_INFO_0 Buffer)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrRemoteTOD(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [out] */ LPTIME_OF_DAY_INFO *BufferPtr)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetprPathType(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [string][in] */ WCHAR *PathName,
  /* [out] */ DWORD *PathType,
  /* [in] */ DWORD Flags)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetprPathCanonicalize(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [string][in] */ WCHAR *PathName,
  /* [size_is][out] */ unsigned char *Outbuf,
  /* [range][in] */ DWORD OutbufLen,
  /* [string][in] */ WCHAR *Prefix,
  /* [out][in] */ DWORD *PathType,
  /* [in] */ DWORD Flags)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

long NetprPathCompare(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [string][in] */ WCHAR *PathName1,
  /* [string][in] */ WCHAR *PathName2,
  /* [in] */ DWORD PathType,
  /* [in] */ DWORD Flags)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetprNameValidate(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [string][in] */ WCHAR *Name,
  /* [in] */ DWORD NameType,
  /* [in] */ DWORD Flags)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetprNameCanonicalize(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [string][in] */ WCHAR *Name,
  /* [size_is][out] */ WCHAR *Outbuf,
  /* [range][in] */ DWORD OutbufLen,
  /* [in] */ DWORD NameType,
  /* [in] */ DWORD Flags)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

long NetprNameCompare(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [string][in] */ WCHAR *Name1,
  /* [string][in] */ WCHAR *Name2,
  /* [in] */ DWORD NameType,
  /* [in] */ DWORD Flags)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrShareEnumSticky(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [out][in] */ LPSHARE_ENUM_STRUCT InfoStruct,
  /* [in] */ DWORD PreferedMaximumLength,
  /* [out] */ DWORD *TotalEntries,
  /* [unique][out][in] */ DWORD *ResumeHandle)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrShareDelStart(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [string][in] */ WCHAR *NetName,
  /* [in] */ DWORD Reserved,
  /* [out] */ PSHARE_DEL_HANDLE ContextHandle)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrShareDelCommit(
  /* [out][in] */ PSHARE_DEL_HANDLE ContextHandle)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

DWORD NetrpGetFileSecurity(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [unique][string][in] */ WCHAR *ShareName,
  /* [string][in] */ WCHAR *lpFileName,
  /* [in] */ SECURITY_INFORMATION RequestedInformation,
  /* [out] */ PADT_SECURITY_DESCRIPTOR *SecurityDescriptor)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

DWORD NetrpSetFileSecurity(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [unique][string][in] */ WCHAR *ShareName,
  /* [string][in] */ WCHAR *lpFileName,
  /* [in] */ SECURITY_INFORMATION SecurityInformation,
  /* [in] */ PADT_SECURITY_DESCRIPTOR SecurityDescriptor)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrServerTransportAddEx(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [in] */ DWORD Level,
  /* [switch_is][in] */ LPTRANSPORT_INFO Buffer)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrDfsGetVersion(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [out] */ DWORD *Version)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrDfsCreateLocalPartition(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [string][in] */ WCHAR *ShareName,
  /* [in] */ GUID *EntryUid,
  /* [string][in] */ WCHAR *EntryPrefix,
  /* [string][in] */ WCHAR *ShortName,
  /* [in] */ LPNET_DFS_ENTRY_ID_CONTAINER RelationInfo,
  /* [in] */ int Force)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrDfsDeleteLocalPartition(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [in] */ GUID *Uid,
  /* [string][in] */ WCHAR *Prefix)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrDfsSetLocalVolumeState(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [in] */ GUID *Uid,
  /* [string][in] */ WCHAR *Prefix,
  /* [in] */ unsigned long State)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrDfsCreateExitPoint(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [in] */ GUID *Uid,
  /* [string][in] */ WCHAR *Prefix,
  /* [in] */ unsigned long Type,
  /* [range][in] */ DWORD ShortPrefixLen,
  /* [size_is][out] */ WCHAR *ShortPrefix)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrDfsDeleteExitPoint(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [in] */ GUID *Uid,
  /* [string][in] */ WCHAR *Prefix,
  /* [in] */ unsigned long Type)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrDfsModifyPrefix(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [in] */ GUID *Uid,
  /* [string][in] */ WCHAR *Prefix)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrDfsFixLocalVolume(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [string][in] */ WCHAR *VolumeName,
  /* [in] */ unsigned long EntryType,
  /* [in] */ unsigned long ServiceType,
  /* [string][in] */ WCHAR *StgId,
  /* [in] */ GUID *EntryUid,
  /* [string][in] */ WCHAR *EntryPrefix,
  /* [in] */ LPNET_DFS_ENTRY_ID_CONTAINER RelationInfo,
  /* [in] */ unsigned long CreateDisposition)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrDfsManagerReportSiteInfo(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [unique][out][in] */ LPDFS_SITELIST_INFO *ppSiteInfo)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrServerTransportDelEx(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [in] */ DWORD Level,
  /* [switch_is][in] */ LPTRANSPORT_INFO Buffer)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrServerAliasAdd(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [in] */ DWORD Level,
  /* [switch_is][in] */ LPSERVER_ALIAS_INFO InfoStruct)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrServerAliasEnum(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [out][in] */ LPSERVER_ALIAS_ENUM_STRUCT InfoStruct,
  /* [in] */ DWORD PreferedMaximumLength,
  /* [out] */ LPDWORD TotalEntries,
  /* [unique][out][in] */ LPDWORD ResumeHandle)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrServerAliasDel(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [in] */ DWORD Level,
  /* [switch_is][in] */ LPSERVER_ALIAS_INFO InfoStruct)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}

NET_API_STATUS NetrShareDelEx(
  /* [unique][string][in] */ SRVSVC_HANDLE ServerName,
  /* [in] */ DWORD Level,
  /* [switch_is][in] */ LPSHARE_INFO ShareInfo)
{
  printf("%s\n", __FUNCTION__);
  return ERROR_CALL_NOT_IMPLEMENTED;
}


// The RPC runtime will call this function if the connection to the client
// is lost.
void __RPC_USER SHARE_DEL_HANDLE_rundown(PCONTEXT_HANDLE hContext)
{
  printf("SHARE_DEL_HANDLE_rundown");
  //std::clog << "CONTEXT_HANDLE_rundown: Context = 
  //  " << hContext << std::endl;
  //Close(&hContext);
}



// Memory allocation function for RPC.
// The runtime uses these two functions for allocating/deallocating
// enough memory to pass the string to the server.
void* __RPC_USER midl_user_allocate(size_t size)
{
  auto p = malloc(size);
  //wprintf(L"malloc %p for %I64d bytes\n", p, size);
  return p;
}

// Memory deallocation function for RPC.
void __RPC_USER midl_user_free(void* p)
{
  //wprintf(L"free %p\n", p);
  free(p);
}

//Main entry point - blocking call to run RPC server
long runSrvSvc()
{
  RPC_STATUS status;
  const unsigned int    cMinCalls = 1;
  const unsigned int    cMaxCalls = 20;
  const unsigned int    fDontWait = FALSE;//This function is blocking ...
  unsigned int          fRegistered = 0;

  status = RpcServerUseProtseqEpA(
    reinterpret_cast<unsigned char*>("ncacn_np"), // Use Named Pipe LPC protocol.
    RPC_C_PROTSEQ_MAX_REQS_DEFAULT, // Backlog queue length for TCP/IP.
    reinterpret_cast<unsigned char*>("\\pipe\\vfs-srvsvc"), // TCP/IP port to use.
    NULL); // No security.
  printf_s("RpcServerUseProtseqEp returned 0x%x\n", status);

  if(status) {
    goto cleanup;
  }

  status = RpcServerRegisterIfEx(srvsvc_v3_0_s_ifspec,
    NULL,
    NULL,
    RPC_IF_ALLOW_CALLBACKS_WITH_NO_AUTH, 
    RPC_C_LISTEN_MAX_CALLS_DEFAULT,
    NULL);

  printf_s("RpcServerRegisterIfEx returned 0x%x\n", status);
  if(status) {
    goto cleanup;
  }
  else
    fRegistered = 1;


  printf_s("Calling RpcServerListen\n");
  status = RpcServerListen(cMinCalls,
    cMaxCalls,
    fDontWait);
  printf_s("RpcServerListen returned: 0x%x\n", status);
  if(status) {
    goto cleanup;
  }

  if(fDontWait) {
    printf_s("Calling RpcMgmtWaitServerListen\n");
    status = RpcMgmtWaitServerListen();  // wait operation
    printf_s("RpcMgmtWaitServerListen returned: 0x%x\n", status);
  }

cleanup:

  if(fRegistered)
  {
    printf_s("Calling RpcServerUnregisterIf\n");
    status = RpcServerUnregisterIf(NULL, NULL, FALSE);
    printf_s("RpcServerUnregisterIf returned 0x%x\n", status);
  }

  return status;
}

