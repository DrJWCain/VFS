

/* this ALWAYS GENERATED file contains the RPC server stubs */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Mon Jan 18 19:14:07 2038
 */
/* Compiler settings for srvsvc.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0622 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if defined(_M_AMD64)


#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/

#include <string.h>
#include "srvsvc.h"

#define TYPE_FORMAT_STRING_SIZE   3795                              
#define PROC_FORMAT_STRING_SIZE   3327                              
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _srvsvc_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } srvsvc_MIDL_TYPE_FORMAT_STRING;

typedef struct _srvsvc_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } srvsvc_MIDL_PROC_FORMAT_STRING;

typedef struct _srvsvc_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } srvsvc_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};

static const RPC_SYNTAX_IDENTIFIER  _NDR64_RpcTransferSyntax = 
{{0x71710533,0xbeba,0x4937,{0x83,0x19,0xb5,0xdb,0xef,0x9c,0xcc,0x36}},{1,0}};


extern const srvsvc_MIDL_TYPE_FORMAT_STRING srvsvc__MIDL_TypeFormatString;
extern const srvsvc_MIDL_PROC_FORMAT_STRING srvsvc__MIDL_ProcFormatString;
extern const srvsvc_MIDL_EXPR_FORMAT_STRING srvsvc__MIDL_ExprFormatString;

/* Standard interface: srvsvc, ver. 3.0,
   GUID={0x4B324FC8,0x1670,0x01D3,{0x12,0x78,0x5A,0x47,0xBF,0x6E,0xE1,0x88}} */


extern const MIDL_SERVER_INFO srvsvc_ServerInfo;

extern const RPC_DISPATCH_TABLE srvsvc_v3_0_DispatchTable;

static const RPC_SERVER_INTERFACE srvsvc___RpcServerInterface =
    {
    sizeof(RPC_SERVER_INTERFACE),
    {{0x4B324FC8,0x1670,0x01D3,{0x12,0x78,0x5A,0x47,0xBF,0x6E,0xE1,0x88}},{3,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    (RPC_DISPATCH_TABLE*)&srvsvc_v3_0_DispatchTable,
    0,
    0,
    0,
    &srvsvc_ServerInfo,
    0x06000000
    };
RPC_IF_HANDLE srvsvc_v3_0_s_ifspec = (RPC_IF_HANDLE)& srvsvc___RpcServerInterface;

extern const MIDL_STUB_DESC srvsvc_StubDesc;

extern const NDR_RUNDOWN RundownRoutines[];

#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif

static const srvsvc_MIDL_PROC_FORMAT_STRING srvsvc__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure Opnum0NotUsedOnWire */

			0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x0 ),	/* 0 */
/*  8 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 10 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 12 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 14 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 16 */	NdrFcShort( 0x0 ),	/* 0 */
/* 18 */	NdrFcShort( 0x8 ),	/* 8 */
/* 20 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 22 */	0xa,		/* 10 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 24 */	NdrFcShort( 0x0 ),	/* 0 */
/* 26 */	NdrFcShort( 0x0 ),	/* 0 */
/* 28 */	NdrFcShort( 0x0 ),	/* 0 */
/* 30 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 32 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 34 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 36 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */

/* 38 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 40 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 42 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Opnum1NotUsedOnWire */

/* 44 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 46 */	NdrFcLong( 0x0 ),	/* 0 */
/* 50 */	NdrFcShort( 0x1 ),	/* 1 */
/* 52 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 54 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 56 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 58 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 60 */	NdrFcShort( 0x0 ),	/* 0 */
/* 62 */	NdrFcShort( 0x8 ),	/* 8 */
/* 64 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 66 */	0xa,		/* 10 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 68 */	NdrFcShort( 0x0 ),	/* 0 */
/* 70 */	NdrFcShort( 0x0 ),	/* 0 */
/* 72 */	NdrFcShort( 0x0 ),	/* 0 */
/* 74 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 76 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 78 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 80 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */

/* 82 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 84 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 86 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Opnum2NotUsedOnWire */

/* 88 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 90 */	NdrFcLong( 0x0 ),	/* 0 */
/* 94 */	NdrFcShort( 0x2 ),	/* 2 */
/* 96 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 98 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 100 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 102 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 104 */	NdrFcShort( 0x0 ),	/* 0 */
/* 106 */	NdrFcShort( 0x8 ),	/* 8 */
/* 108 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 110 */	0xa,		/* 10 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 112 */	NdrFcShort( 0x0 ),	/* 0 */
/* 114 */	NdrFcShort( 0x0 ),	/* 0 */
/* 116 */	NdrFcShort( 0x0 ),	/* 0 */
/* 118 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 120 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 122 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 124 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */

/* 126 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 128 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 130 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Opnum3NotUsedOnWire */

/* 132 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 134 */	NdrFcLong( 0x0 ),	/* 0 */
/* 138 */	NdrFcShort( 0x3 ),	/* 3 */
/* 140 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 142 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 144 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 146 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 148 */	NdrFcShort( 0x0 ),	/* 0 */
/* 150 */	NdrFcShort( 0x8 ),	/* 8 */
/* 152 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 154 */	0xa,		/* 10 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 156 */	NdrFcShort( 0x0 ),	/* 0 */
/* 158 */	NdrFcShort( 0x0 ),	/* 0 */
/* 160 */	NdrFcShort( 0x0 ),	/* 0 */
/* 162 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 164 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 166 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 168 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */

/* 170 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 172 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 174 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Opnum4NotUsedOnWire */

/* 176 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 178 */	NdrFcLong( 0x0 ),	/* 0 */
/* 182 */	NdrFcShort( 0x4 ),	/* 4 */
/* 184 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 186 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 188 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 190 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 192 */	NdrFcShort( 0x0 ),	/* 0 */
/* 194 */	NdrFcShort( 0x8 ),	/* 8 */
/* 196 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 198 */	0xa,		/* 10 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 200 */	NdrFcShort( 0x0 ),	/* 0 */
/* 202 */	NdrFcShort( 0x0 ),	/* 0 */
/* 204 */	NdrFcShort( 0x0 ),	/* 0 */
/* 206 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 208 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 210 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 212 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */

/* 214 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 216 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 218 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Opnum5NotUsedOnWire */

/* 220 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 222 */	NdrFcLong( 0x0 ),	/* 0 */
/* 226 */	NdrFcShort( 0x5 ),	/* 5 */
/* 228 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 230 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 232 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 234 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 236 */	NdrFcShort( 0x0 ),	/* 0 */
/* 238 */	NdrFcShort( 0x8 ),	/* 8 */
/* 240 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 242 */	0xa,		/* 10 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 244 */	NdrFcShort( 0x0 ),	/* 0 */
/* 246 */	NdrFcShort( 0x0 ),	/* 0 */
/* 248 */	NdrFcShort( 0x0 ),	/* 0 */
/* 250 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 252 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 254 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 256 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */

/* 258 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 260 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 262 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Opnum6NotUsedOnWire */

/* 264 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 266 */	NdrFcLong( 0x0 ),	/* 0 */
/* 270 */	NdrFcShort( 0x6 ),	/* 6 */
/* 272 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 274 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 276 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 278 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 280 */	NdrFcShort( 0x0 ),	/* 0 */
/* 282 */	NdrFcShort( 0x8 ),	/* 8 */
/* 284 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 286 */	0xa,		/* 10 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 288 */	NdrFcShort( 0x0 ),	/* 0 */
/* 290 */	NdrFcShort( 0x0 ),	/* 0 */
/* 292 */	NdrFcShort( 0x0 ),	/* 0 */
/* 294 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 296 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 298 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 300 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */

/* 302 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 304 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 306 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Opnum7NotUsedOnWire */

/* 308 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 310 */	NdrFcLong( 0x0 ),	/* 0 */
/* 314 */	NdrFcShort( 0x7 ),	/* 7 */
/* 316 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 318 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 320 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 322 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 324 */	NdrFcShort( 0x0 ),	/* 0 */
/* 326 */	NdrFcShort( 0x8 ),	/* 8 */
/* 328 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 330 */	0xa,		/* 10 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 332 */	NdrFcShort( 0x0 ),	/* 0 */
/* 334 */	NdrFcShort( 0x0 ),	/* 0 */
/* 336 */	NdrFcShort( 0x0 ),	/* 0 */
/* 338 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 340 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 342 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 344 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */

/* 346 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 348 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 350 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrConnectionEnum */

/* 352 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 354 */	NdrFcLong( 0x0 ),	/* 0 */
/* 358 */	NdrFcShort( 0x8 ),	/* 8 */
/* 360 */	NdrFcShort( 0x38 ),	/* X64 Stack size/offset = 56 */
/* 362 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 364 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 366 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 368 */	NdrFcShort( 0x24 ),	/* 36 */
/* 370 */	NdrFcShort( 0x40 ),	/* 64 */
/* 372 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 374 */	0xa,		/* 10 */
			0x47,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, has range on conformance */
/* 376 */	NdrFcShort( 0x1 ),	/* 1 */
/* 378 */	NdrFcShort( 0x1 ),	/* 1 */
/* 380 */	NdrFcShort( 0x0 ),	/* 0 */
/* 382 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 384 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 386 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 388 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter Qualifier */

/* 390 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 392 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 394 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter InfoStruct */

/* 396 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
/* 398 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 400 */	NdrFcShort( 0xcc ),	/* Type Offset=204 */

	/* Parameter PreferedMaximumLength */

/* 402 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 404 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 406 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter TotalEntries */

/* 408 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 410 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 412 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ResumeHandle */

/* 414 */	NdrFcShort( 0x1a ),	/* Flags:  must free, in, out, */
/* 416 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 418 */	NdrFcShort( 0xe0 ),	/* Type Offset=224 */

	/* Return value */

/* 420 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 422 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 424 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrFileEnum */

/* 426 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 428 */	NdrFcLong( 0x0 ),	/* 0 */
/* 432 */	NdrFcShort( 0x9 ),	/* 9 */
/* 434 */	NdrFcShort( 0x40 ),	/* X64 Stack size/offset = 64 */
/* 436 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 438 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 440 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 442 */	NdrFcShort( 0x24 ),	/* 36 */
/* 444 */	NdrFcShort( 0x40 ),	/* 64 */
/* 446 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x8,		/* 8 */
/* 448 */	0xa,		/* 10 */
			0x47,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, has range on conformance */
/* 450 */	NdrFcShort( 0x1 ),	/* 1 */
/* 452 */	NdrFcShort( 0x1 ),	/* 1 */
/* 454 */	NdrFcShort( 0x0 ),	/* 0 */
/* 456 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 458 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 460 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 462 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter BasePath */

/* 464 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 466 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 468 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter UserName */

/* 470 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 472 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 474 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter InfoStruct */

/* 476 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
/* 478 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 480 */	NdrFcShort( 0x178 ),	/* Type Offset=376 */

	/* Parameter PreferedMaximumLength */

/* 482 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 484 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 486 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter TotalEntries */

/* 488 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 490 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 492 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ResumeHandle */

/* 494 */	NdrFcShort( 0x1a ),	/* Flags:  must free, in, out, */
/* 496 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 498 */	NdrFcShort( 0xe0 ),	/* Type Offset=224 */

	/* Return value */

/* 500 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 502 */	NdrFcShort( 0x38 ),	/* X64 Stack size/offset = 56 */
/* 504 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrFileGetInfo */

/* 506 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 508 */	NdrFcLong( 0x0 ),	/* 0 */
/* 512 */	NdrFcShort( 0xa ),	/* 10 */
/* 514 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 516 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 518 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 520 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 522 */	NdrFcShort( 0x10 ),	/* 16 */
/* 524 */	NdrFcShort( 0x8 ),	/* 8 */
/* 526 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 528 */	0xa,		/* 10 */
			0x43,		/* Ext Flags:  new corr desc, clt corr check, has range on conformance */
/* 530 */	NdrFcShort( 0x1 ),	/* 1 */
/* 532 */	NdrFcShort( 0x0 ),	/* 0 */
/* 534 */	NdrFcShort( 0x0 ),	/* 0 */
/* 536 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 538 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 540 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 542 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter FileId */

/* 544 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 546 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 548 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter Level */

/* 550 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 552 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 554 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter InfoStruct */

/* 556 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 558 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 560 */	NdrFcShort( 0x18c ),	/* Type Offset=396 */

	/* Return value */

/* 562 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 564 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 566 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrFileClose */

/* 568 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 570 */	NdrFcLong( 0x0 ),	/* 0 */
/* 574 */	NdrFcShort( 0xb ),	/* 11 */
/* 576 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 578 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 580 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 582 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 584 */	NdrFcShort( 0x8 ),	/* 8 */
/* 586 */	NdrFcShort( 0x8 ),	/* 8 */
/* 588 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 590 */	0xa,		/* 10 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 592 */	NdrFcShort( 0x0 ),	/* 0 */
/* 594 */	NdrFcShort( 0x0 ),	/* 0 */
/* 596 */	NdrFcShort( 0x0 ),	/* 0 */
/* 598 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 600 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 602 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 604 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter FileId */

/* 606 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 608 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 610 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 612 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 614 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 616 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrSessionEnum */

/* 618 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 620 */	NdrFcLong( 0x0 ),	/* 0 */
/* 624 */	NdrFcShort( 0xc ),	/* 12 */
/* 626 */	NdrFcShort( 0x40 ),	/* X64 Stack size/offset = 64 */
/* 628 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 630 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 632 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 634 */	NdrFcShort( 0x24 ),	/* 36 */
/* 636 */	NdrFcShort( 0x40 ),	/* 64 */
/* 638 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x8,		/* 8 */
/* 640 */	0xa,		/* 10 */
			0x47,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, has range on conformance */
/* 642 */	NdrFcShort( 0x1 ),	/* 1 */
/* 644 */	NdrFcShort( 0x1 ),	/* 1 */
/* 646 */	NdrFcShort( 0x0 ),	/* 0 */
/* 648 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 650 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 652 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 654 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter ClientName */

/* 656 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 658 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 660 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter UserName */

/* 662 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 664 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 666 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter InfoStruct */

/* 668 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
/* 670 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 672 */	NdrFcShort( 0x3a6 ),	/* Type Offset=934 */

	/* Parameter PreferedMaximumLength */

/* 674 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 676 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 678 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter TotalEntries */

/* 680 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 682 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 684 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ResumeHandle */

/* 686 */	NdrFcShort( 0x1a ),	/* Flags:  must free, in, out, */
/* 688 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 690 */	NdrFcShort( 0xe0 ),	/* Type Offset=224 */

	/* Return value */

/* 692 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 694 */	NdrFcShort( 0x38 ),	/* X64 Stack size/offset = 56 */
/* 696 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrSessionDel */

/* 698 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 700 */	NdrFcLong( 0x0 ),	/* 0 */
/* 704 */	NdrFcShort( 0xd ),	/* 13 */
/* 706 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 708 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 710 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 712 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 714 */	NdrFcShort( 0x0 ),	/* 0 */
/* 716 */	NdrFcShort( 0x8 ),	/* 8 */
/* 718 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 720 */	0xa,		/* 10 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 722 */	NdrFcShort( 0x0 ),	/* 0 */
/* 724 */	NdrFcShort( 0x0 ),	/* 0 */
/* 726 */	NdrFcShort( 0x0 ),	/* 0 */
/* 728 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 730 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 732 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 734 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter ClientName */

/* 736 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 738 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 740 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter UserName */

/* 742 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 744 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 746 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */

/* 748 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 750 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 752 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrShareAdd */

/* 754 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 756 */	NdrFcLong( 0x0 ),	/* 0 */
/* 760 */	NdrFcShort( 0xe ),	/* 14 */
/* 762 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 764 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 766 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 768 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 770 */	NdrFcShort( 0x24 ),	/* 36 */
/* 772 */	NdrFcShort( 0x24 ),	/* 36 */
/* 774 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 776 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 778 */	NdrFcShort( 0x0 ),	/* 0 */
/* 780 */	NdrFcShort( 0x1 ),	/* 1 */
/* 782 */	NdrFcShort( 0x0 ),	/* 0 */
/* 784 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 786 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 788 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 790 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter Level */

/* 792 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 794 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 796 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter InfoStruct */

/* 798 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 800 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 802 */	NdrFcShort( 0x3ba ),	/* Type Offset=954 */

	/* Parameter ParmErr */

/* 804 */	NdrFcShort( 0x1a ),	/* Flags:  must free, in, out, */
/* 806 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 808 */	NdrFcShort( 0xe0 ),	/* Type Offset=224 */

	/* Return value */

/* 810 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 812 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 814 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrShareEnum */

/* 816 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 818 */	NdrFcLong( 0x0 ),	/* 0 */
/* 822 */	NdrFcShort( 0xf ),	/* 15 */
/* 824 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 826 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 828 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 830 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 832 */	NdrFcShort( 0x24 ),	/* 36 */
/* 834 */	NdrFcShort( 0x40 ),	/* 64 */
/* 836 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 838 */	0xa,		/* 10 */
			0x47,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, has range on conformance */
/* 840 */	NdrFcShort( 0x1 ),	/* 1 */
/* 842 */	NdrFcShort( 0x1 ),	/* 1 */
/* 844 */	NdrFcShort( 0x0 ),	/* 0 */
/* 846 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 848 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 850 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 852 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter InfoStruct */

/* 854 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
/* 856 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 858 */	NdrFcShort( 0x700 ),	/* Type Offset=1792 */

	/* Parameter PreferedMaximumLength */

/* 860 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 862 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 864 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter TotalEntries */

/* 866 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 868 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 870 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ResumeHandle */

/* 872 */	NdrFcShort( 0x1a ),	/* Flags:  must free, in, out, */
/* 874 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 876 */	NdrFcShort( 0xe0 ),	/* Type Offset=224 */

	/* Return value */

/* 878 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 880 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 882 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrShareGetInfo */

/* 884 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 886 */	NdrFcLong( 0x0 ),	/* 0 */
/* 890 */	NdrFcShort( 0x10 ),	/* 16 */
/* 892 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 894 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 896 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 898 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 900 */	NdrFcShort( 0x8 ),	/* 8 */
/* 902 */	NdrFcShort( 0x8 ),	/* 8 */
/* 904 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 906 */	0xa,		/* 10 */
			0x43,		/* Ext Flags:  new corr desc, clt corr check, has range on conformance */
/* 908 */	NdrFcShort( 0x1 ),	/* 1 */
/* 910 */	NdrFcShort( 0x0 ),	/* 0 */
/* 912 */	NdrFcShort( 0x0 ),	/* 0 */
/* 914 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 916 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 918 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 920 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter NetName */

/* 922 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 924 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 926 */	NdrFcShort( 0x712 ),	/* Type Offset=1810 */

	/* Parameter Level */

/* 928 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 930 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 932 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter InfoStruct */

/* 934 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 936 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 938 */	NdrFcShort( 0x718 ),	/* Type Offset=1816 */

	/* Return value */

/* 940 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 942 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 944 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrShareSetInfo */

/* 946 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 948 */	NdrFcLong( 0x0 ),	/* 0 */
/* 952 */	NdrFcShort( 0x11 ),	/* 17 */
/* 954 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 956 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 958 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 960 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 962 */	NdrFcShort( 0x24 ),	/* 36 */
/* 964 */	NdrFcShort( 0x24 ),	/* 36 */
/* 966 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 968 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 970 */	NdrFcShort( 0x0 ),	/* 0 */
/* 972 */	NdrFcShort( 0x1 ),	/* 1 */
/* 974 */	NdrFcShort( 0x0 ),	/* 0 */
/* 976 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 978 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 980 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 982 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter NetName */

/* 984 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 986 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 988 */	NdrFcShort( 0x712 ),	/* Type Offset=1810 */

	/* Parameter Level */

/* 990 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 992 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 994 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ShareInfo */

/* 996 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 998 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1000 */	NdrFcShort( 0x730 ),	/* Type Offset=1840 */

	/* Parameter ParmErr */

/* 1002 */	NdrFcShort( 0x1a ),	/* Flags:  must free, in, out, */
/* 1004 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 1006 */	NdrFcShort( 0xe0 ),	/* Type Offset=224 */

	/* Return value */

/* 1008 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1010 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 1012 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrShareDel */

/* 1014 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 1016 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1020 */	NdrFcShort( 0x12 ),	/* 18 */
/* 1022 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 1024 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 1026 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1028 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 1030 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1032 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1034 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 1036 */	0xa,		/* 10 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 1038 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1040 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1042 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1044 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 1046 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1048 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1050 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter NetName */

/* 1052 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1054 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1056 */	NdrFcShort( 0x712 ),	/* Type Offset=1810 */

	/* Parameter Reserved */

/* 1058 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1060 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1062 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1064 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1066 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1068 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrShareDelSticky */

/* 1070 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 1072 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1076 */	NdrFcShort( 0x13 ),	/* 19 */
/* 1078 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 1080 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 1082 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1084 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 1086 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1088 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1090 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 1092 */	0xa,		/* 10 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 1094 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1096 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1098 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1100 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 1102 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1104 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1106 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter NetName */

/* 1108 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1110 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1112 */	NdrFcShort( 0x712 ),	/* Type Offset=1810 */

	/* Parameter Reserved */

/* 1114 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1116 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1118 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1120 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1122 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1124 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrShareCheck */

/* 1126 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 1128 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1132 */	NdrFcShort( 0x14 ),	/* 20 */
/* 1134 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 1136 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 1138 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1140 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 1142 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1144 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1146 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 1148 */	0xa,		/* 10 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 1150 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1152 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1154 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1156 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 1158 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1160 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1162 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter Device */

/* 1164 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1166 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1168 */	NdrFcShort( 0x712 ),	/* Type Offset=1810 */

	/* Parameter Type */

/* 1170 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1172 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1174 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1176 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1178 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1180 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrServerGetInfo */

/* 1182 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 1184 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1188 */	NdrFcShort( 0x15 ),	/* 21 */
/* 1190 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 1192 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 1194 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1196 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 1198 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1200 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1202 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 1204 */	0xa,		/* 10 */
			0x43,		/* Ext Flags:  new corr desc, clt corr check, has range on conformance */
/* 1206 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1208 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1210 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1212 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 1214 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1216 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1218 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter Level */

/* 1220 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1222 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1224 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter InfoStruct */

/* 1226 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 1228 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1230 */	NdrFcShort( 0x748 ),	/* Type Offset=1864 */

	/* Return value */

/* 1232 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1234 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1236 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrServerSetInfo */

/* 1238 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 1240 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1244 */	NdrFcShort( 0x16 ),	/* 22 */
/* 1246 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 1248 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 1250 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1252 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 1254 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1256 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1258 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 1260 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 1262 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1264 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1266 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1268 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 1270 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1272 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1274 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter Level */

/* 1276 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1278 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1280 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ServerInfo */

/* 1282 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1284 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1286 */	NdrFcShort( 0x9d0 ),	/* Type Offset=2512 */

	/* Parameter ParmErr */

/* 1288 */	NdrFcShort( 0x1a ),	/* Flags:  must free, in, out, */
/* 1290 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1292 */	NdrFcShort( 0xe0 ),	/* Type Offset=224 */

	/* Return value */

/* 1294 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1296 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 1298 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrServerDiskEnum */

/* 1300 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 1302 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1306 */	NdrFcShort( 0x17 ),	/* 23 */
/* 1308 */	NdrFcShort( 0x38 ),	/* X64 Stack size/offset = 56 */
/* 1310 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 1312 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1314 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 1316 */	NdrFcShort( 0x2c ),	/* 44 */
/* 1318 */	NdrFcShort( 0x40 ),	/* 64 */
/* 1320 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 1322 */	0xa,		/* 10 */
			0x47,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, has range on conformance */
/* 1324 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1326 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1328 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1330 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 1332 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1334 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1336 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter Level */

/* 1338 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1340 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1342 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter DiskInfoStruct */

/* 1344 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
/* 1346 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1348 */	NdrFcShort( 0xa24 ),	/* Type Offset=2596 */

	/* Parameter PreferedMaximumLength */

/* 1350 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1352 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1354 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter TotalEntries */

/* 1356 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1358 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 1360 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ResumeHandle */

/* 1362 */	NdrFcShort( 0x1a ),	/* Flags:  must free, in, out, */
/* 1364 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 1366 */	NdrFcShort( 0xe0 ),	/* Type Offset=224 */

	/* Return value */

/* 1368 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1370 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 1372 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrServerStatisticsGet */

/* 1374 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 1376 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1380 */	NdrFcShort( 0x18 ),	/* 24 */
/* 1382 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 1384 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 1386 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1388 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 1390 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1392 */	NdrFcShort( 0x84 ),	/* 132 */
/* 1394 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 1396 */	0xa,		/* 10 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 1398 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1400 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1402 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1404 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 1406 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1408 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1410 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter Service */

/* 1412 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1414 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1416 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter Level */

/* 1418 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1420 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1422 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter Options */

/* 1424 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1426 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1428 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter InfoStruct */

/* 1430 */	NdrFcShort( 0x2012 ),	/* Flags:  must free, out, srv alloc size=8 */
/* 1432 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 1434 */	NdrFcShort( 0xa34 ),	/* Type Offset=2612 */

	/* Return value */

/* 1436 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1438 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 1440 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrServerTransportAdd */

/* 1442 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 1444 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1448 */	NdrFcShort( 0x19 ),	/* 25 */
/* 1450 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 1452 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 1454 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1456 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 1458 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1460 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1462 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 1464 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 1466 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1468 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1470 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1472 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 1474 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1476 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1478 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter Level */

/* 1480 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1482 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1484 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter Buffer */

/* 1486 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1488 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1490 */	NdrFcShort( 0xa6c ),	/* Type Offset=2668 */

	/* Return value */

/* 1492 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1494 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1496 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrServerTransportEnum */

/* 1498 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 1500 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1504 */	NdrFcShort( 0x1a ),	/* 26 */
/* 1506 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 1508 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 1510 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1512 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 1514 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1516 */	NdrFcShort( 0x40 ),	/* 64 */
/* 1518 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 1520 */	0xa,		/* 10 */
			0x47,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, has range on conformance */
/* 1522 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1524 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1526 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1528 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 1530 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1532 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1534 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter InfoStruct */

/* 1536 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
/* 1538 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1540 */	NdrFcShort( 0xc2a ),	/* Type Offset=3114 */

	/* Parameter PreferedMaximumLength */

/* 1542 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1544 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1546 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter TotalEntries */

/* 1548 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1550 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1552 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ResumeHandle */

/* 1554 */	NdrFcShort( 0x1a ),	/* Flags:  must free, in, out, */
/* 1556 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 1558 */	NdrFcShort( 0xe0 ),	/* Type Offset=224 */

	/* Return value */

/* 1560 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1562 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 1564 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrServerTransportDel */

/* 1566 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 1568 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1572 */	NdrFcShort( 0x1b ),	/* 27 */
/* 1574 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 1576 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 1578 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1580 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 1582 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1584 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1586 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 1588 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 1590 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1592 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1594 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1596 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 1598 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1600 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1602 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter Level */

/* 1604 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1606 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1608 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter Buffer */

/* 1610 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1612 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1614 */	NdrFcShort( 0xa6c ),	/* Type Offset=2668 */

	/* Return value */

/* 1616 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1618 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1620 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrRemoteTOD */

/* 1622 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 1624 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1628 */	NdrFcShort( 0x1c ),	/* 28 */
/* 1630 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1632 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 1634 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1636 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 1638 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1640 */	NdrFcShort( 0x70 ),	/* 112 */
/* 1642 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 1644 */	0xa,		/* 10 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 1646 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1648 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1650 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1652 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 1654 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1656 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1658 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter BufferPtr */

/* 1660 */	NdrFcShort( 0x2012 ),	/* Flags:  must free, out, srv alloc size=8 */
/* 1662 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1664 */	NdrFcShort( 0xc3a ),	/* Type Offset=3130 */

	/* Return value */

/* 1666 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1668 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1670 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetprPathType */

/* 1672 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 1674 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1678 */	NdrFcShort( 0x1d ),	/* 29 */
/* 1680 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 1682 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 1684 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1686 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 1688 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1690 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1692 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 1694 */	0xa,		/* 10 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 1696 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1698 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1700 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1702 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 1704 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1706 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1708 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter PathName */

/* 1710 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1712 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1714 */	NdrFcShort( 0x712 ),	/* Type Offset=1810 */

	/* Parameter PathType */

/* 1716 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1718 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1720 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter Flags */

/* 1722 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1724 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1726 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1728 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1730 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 1732 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetprPathCanonicalize */

/* 1734 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 1736 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1740 */	NdrFcShort( 0x1e ),	/* 30 */
/* 1742 */	NdrFcShort( 0x40 ),	/* X64 Stack size/offset = 64 */
/* 1744 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 1746 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1748 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 1750 */	NdrFcShort( 0x2c ),	/* 44 */
/* 1752 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1754 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x8,		/* 8 */
/* 1756 */	0xa,		/* 10 */
			0x43,		/* Ext Flags:  new corr desc, clt corr check, has range on conformance */
/* 1758 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1760 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1762 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1764 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 1766 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1768 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1770 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter PathName */

/* 1772 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1774 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1776 */	NdrFcShort( 0x712 ),	/* Type Offset=1810 */

	/* Parameter Outbuf */

/* 1778 */	NdrFcShort( 0x113 ),	/* Flags:  must size, must free, out, simple ref, */
/* 1780 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1782 */	NdrFcShort( 0xc58 ),	/* Type Offset=3160 */

	/* Parameter OutbufLen */

/* 1784 */	NdrFcShort( 0x88 ),	/* Flags:  in, by val, */
/* 1786 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1788 */	NdrFcShort( 0xc6e ),	/* 3182 */

	/* Parameter Prefix */

/* 1790 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1792 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 1794 */	NdrFcShort( 0x712 ),	/* Type Offset=1810 */

	/* Parameter PathType */

/* 1796 */	NdrFcShort( 0x158 ),	/* Flags:  in, out, base type, simple ref, */
/* 1798 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 1800 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter Flags */

/* 1802 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1804 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 1806 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1808 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1810 */	NdrFcShort( 0x38 ),	/* X64 Stack size/offset = 56 */
/* 1812 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetprPathCompare */

/* 1814 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 1816 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1820 */	NdrFcShort( 0x1f ),	/* 31 */
/* 1822 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 1824 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 1826 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1828 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 1830 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1832 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1834 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 1836 */	0xa,		/* 10 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 1838 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1840 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1842 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1844 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 1846 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1848 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1850 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter PathName1 */

/* 1852 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1854 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1856 */	NdrFcShort( 0x712 ),	/* Type Offset=1810 */

	/* Parameter PathName2 */

/* 1858 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1860 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1862 */	NdrFcShort( 0x712 ),	/* Type Offset=1810 */

	/* Parameter PathType */

/* 1864 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1866 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1868 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter Flags */

/* 1870 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1872 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 1874 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1876 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1878 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 1880 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetprNameValidate */

/* 1882 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 1884 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1888 */	NdrFcShort( 0x20 ),	/* 32 */
/* 1890 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 1892 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 1894 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1896 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 1898 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1900 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1902 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 1904 */	0xa,		/* 10 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 1906 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1908 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1910 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1912 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 1914 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1916 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1918 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter Name */

/* 1920 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1922 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1924 */	NdrFcShort( 0x712 ),	/* Type Offset=1810 */

	/* Parameter NameType */

/* 1926 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1928 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1930 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter Flags */

/* 1932 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1934 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1936 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1938 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1940 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 1942 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetprNameCanonicalize */

/* 1944 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 1946 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1950 */	NdrFcShort( 0x21 ),	/* 33 */
/* 1952 */	NdrFcShort( 0x38 ),	/* X64 Stack size/offset = 56 */
/* 1954 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 1956 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1958 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 1960 */	NdrFcShort( 0x18 ),	/* 24 */
/* 1962 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1964 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 1966 */	0xa,		/* 10 */
			0x43,		/* Ext Flags:  new corr desc, clt corr check, has range on conformance */
/* 1968 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1970 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1972 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1974 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 1976 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1978 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 1980 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter Name */

/* 1982 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1984 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1986 */	NdrFcShort( 0x712 ),	/* Type Offset=1810 */

	/* Parameter Outbuf */

/* 1988 */	NdrFcShort( 0x113 ),	/* Flags:  must size, must free, out, simple ref, */
/* 1990 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1992 */	NdrFcShort( 0xc80 ),	/* Type Offset=3200 */

	/* Parameter OutbufLen */

/* 1994 */	NdrFcShort( 0x88 ),	/* Flags:  in, by val, */
/* 1996 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1998 */	NdrFcShort( 0xc96 ),	/* 3222 */

	/* Parameter NameType */

/* 2000 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2002 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 2004 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter Flags */

/* 2006 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2008 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 2010 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2012 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2014 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 2016 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetprNameCompare */

/* 2018 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 2020 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2024 */	NdrFcShort( 0x22 ),	/* 34 */
/* 2026 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 2028 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 2030 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2032 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 2034 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2036 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2038 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 2040 */	0xa,		/* 10 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 2042 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2044 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2046 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2048 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 2050 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2052 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2054 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter Name1 */

/* 2056 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2058 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 2060 */	NdrFcShort( 0x712 ),	/* Type Offset=1810 */

	/* Parameter Name2 */

/* 2062 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2064 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 2066 */	NdrFcShort( 0x712 ),	/* Type Offset=1810 */

	/* Parameter NameType */

/* 2068 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2070 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 2072 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter Flags */

/* 2074 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2076 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 2078 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2080 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2082 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 2084 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrShareEnumSticky */

/* 2086 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 2088 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2092 */	NdrFcShort( 0x23 ),	/* 35 */
/* 2094 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 2096 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 2098 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2100 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 2102 */	NdrFcShort( 0x24 ),	/* 36 */
/* 2104 */	NdrFcShort( 0x40 ),	/* 64 */
/* 2106 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 2108 */	0xa,		/* 10 */
			0x47,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, has range on conformance */
/* 2110 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2112 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2114 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2116 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 2118 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2120 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2122 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter InfoStruct */

/* 2124 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
/* 2126 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 2128 */	NdrFcShort( 0x700 ),	/* Type Offset=1792 */

	/* Parameter PreferedMaximumLength */

/* 2130 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2132 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 2134 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter TotalEntries */

/* 2136 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2138 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 2140 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ResumeHandle */

/* 2142 */	NdrFcShort( 0x1a ),	/* Flags:  must free, in, out, */
/* 2144 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 2146 */	NdrFcShort( 0xe0 ),	/* Type Offset=224 */

	/* Return value */

/* 2148 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2150 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 2152 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrShareDelStart */

/* 2154 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 2156 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2160 */	NdrFcShort( 0x24 ),	/* 36 */
/* 2162 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 2164 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 2166 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2168 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 2170 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2172 */	NdrFcShort( 0x40 ),	/* 64 */
/* 2174 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 2176 */	0xa,		/* 10 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 2178 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2180 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2182 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2184 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 2186 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2188 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2190 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter NetName */

/* 2192 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2194 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 2196 */	NdrFcShort( 0x712 ),	/* Type Offset=1810 */

	/* Parameter Reserved */

/* 2198 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2200 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 2202 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ContextHandle */

/* 2204 */	NdrFcShort( 0x110 ),	/* Flags:  out, simple ref, */
/* 2206 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 2208 */	NdrFcShort( 0xca4 ),	/* Type Offset=3236 */

	/* Return value */

/* 2210 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2212 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 2214 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrShareDelCommit */

/* 2216 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 2218 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2222 */	NdrFcShort( 0x25 ),	/* 37 */
/* 2224 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 2226 */	0x30,		/* FC_BIND_CONTEXT */
			0xe0,		/* Ctxt flags:  via ptr, in, out, */
/* 2228 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2230 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 2232 */	NdrFcShort( 0x38 ),	/* 56 */
/* 2234 */	NdrFcShort( 0x40 ),	/* 64 */
/* 2236 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2238 */	0xa,		/* 10 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 2240 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2242 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2244 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2246 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ContextHandle */

/* 2248 */	NdrFcShort( 0x118 ),	/* Flags:  in, out, simple ref, */
/* 2250 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2252 */	NdrFcShort( 0xcac ),	/* Type Offset=3244 */

	/* Return value */

/* 2254 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2256 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 2258 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrpGetFileSecurity */

/* 2260 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 2262 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2266 */	NdrFcShort( 0x26 ),	/* 38 */
/* 2268 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 2270 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 2272 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2274 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 2276 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2278 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2280 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 2282 */	0xa,		/* 10 */
			0x43,		/* Ext Flags:  new corr desc, clt corr check, has range on conformance */
/* 2284 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2286 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2288 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2290 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 2292 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2294 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2296 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter ShareName */

/* 2298 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2300 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 2302 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter lpFileName */

/* 2304 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2306 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 2308 */	NdrFcShort( 0x712 ),	/* Type Offset=1810 */

	/* Parameter RequestedInformation */

/* 2310 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2312 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 2314 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter SecurityDescriptor */

/* 2316 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 2318 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 2320 */	NdrFcShort( 0xcb0 ),	/* Type Offset=3248 */

	/* Return value */

/* 2322 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2324 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 2326 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrpSetFileSecurity */

/* 2328 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 2330 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2334 */	NdrFcShort( 0x27 ),	/* 39 */
/* 2336 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 2338 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 2340 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2342 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 2344 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2346 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2348 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 2350 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 2352 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2354 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2356 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2358 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 2360 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2362 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2364 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter ShareName */

/* 2366 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2368 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 2370 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter lpFileName */

/* 2372 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2374 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 2376 */	NdrFcShort( 0x712 ),	/* Type Offset=1810 */

	/* Parameter SecurityInformation */

/* 2378 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2380 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 2382 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter SecurityDescriptor */

/* 2384 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2386 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 2388 */	NdrFcShort( 0xcb8 ),	/* Type Offset=3256 */

	/* Return value */

/* 2390 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2392 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 2394 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrServerTransportAddEx */

/* 2396 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 2398 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2402 */	NdrFcShort( 0x28 ),	/* 40 */
/* 2404 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 2406 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 2408 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2410 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 2412 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2414 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2416 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 2418 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 2420 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2422 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2424 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2426 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 2428 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2430 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2432 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter Level */

/* 2434 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2436 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 2438 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter Buffer */

/* 2440 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2442 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 2444 */	NdrFcShort( 0xcd0 ),	/* Type Offset=3280 */

	/* Return value */

/* 2446 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2448 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 2450 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrDfsGetVersion */

/* 2452 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 2454 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2458 */	NdrFcShort( 0x29 ),	/* 41 */
/* 2460 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 2462 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 2464 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2466 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 2468 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2470 */	NdrFcShort( 0x24 ),	/* 36 */
/* 2472 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 2474 */	0xa,		/* 10 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 2476 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2478 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2480 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2482 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 2484 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2486 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2488 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter Version */

/* 2490 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2492 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 2494 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2496 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2498 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 2500 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrDfsCreateLocalPartition */

/* 2502 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 2504 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2508 */	NdrFcShort( 0x2a ),	/* 42 */
/* 2510 */	NdrFcShort( 0x40 ),	/* X64 Stack size/offset = 64 */
/* 2512 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 2514 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2516 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 2518 */	NdrFcShort( 0x4c ),	/* 76 */
/* 2520 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2522 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x8,		/* 8 */
/* 2524 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 2526 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2528 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2530 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2532 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 2534 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2536 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2538 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter ShareName */

/* 2540 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2542 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 2544 */	NdrFcShort( 0x712 ),	/* Type Offset=1810 */

	/* Parameter EntryUid */

/* 2546 */	NdrFcShort( 0x10a ),	/* Flags:  must free, in, simple ref, */
/* 2548 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 2550 */	NdrFcShort( 0xd0c ),	/* Type Offset=3340 */

	/* Parameter EntryPrefix */

/* 2552 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2554 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 2556 */	NdrFcShort( 0x712 ),	/* Type Offset=1810 */

	/* Parameter ShortName */

/* 2558 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2560 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 2562 */	NdrFcShort( 0x712 ),	/* Type Offset=1810 */

	/* Parameter RelationInfo */

/* 2564 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2566 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 2568 */	NdrFcShort( 0xd58 ),	/* Type Offset=3416 */

	/* Parameter Force */

/* 2570 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2572 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 2574 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2576 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2578 */	NdrFcShort( 0x38 ),	/* X64 Stack size/offset = 56 */
/* 2580 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrDfsDeleteLocalPartition */

/* 2582 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 2584 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2588 */	NdrFcShort( 0x2b ),	/* 43 */
/* 2590 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 2592 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 2594 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2596 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 2598 */	NdrFcShort( 0x44 ),	/* 68 */
/* 2600 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2602 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 2604 */	0xa,		/* 10 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 2606 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2608 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2610 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2612 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 2614 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2616 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2618 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter Uid */

/* 2620 */	NdrFcShort( 0x10a ),	/* Flags:  must free, in, simple ref, */
/* 2622 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 2624 */	NdrFcShort( 0xd0c ),	/* Type Offset=3340 */

	/* Parameter Prefix */

/* 2626 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2628 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 2630 */	NdrFcShort( 0x712 ),	/* Type Offset=1810 */

	/* Return value */

/* 2632 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2634 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 2636 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrDfsSetLocalVolumeState */

/* 2638 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 2640 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2644 */	NdrFcShort( 0x2c ),	/* 44 */
/* 2646 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 2648 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 2650 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2652 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 2654 */	NdrFcShort( 0x4c ),	/* 76 */
/* 2656 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2658 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 2660 */	0xa,		/* 10 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 2662 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2664 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2666 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2668 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 2670 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2672 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2674 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter Uid */

/* 2676 */	NdrFcShort( 0x10a ),	/* Flags:  must free, in, simple ref, */
/* 2678 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 2680 */	NdrFcShort( 0xd0c ),	/* Type Offset=3340 */

	/* Parameter Prefix */

/* 2682 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2684 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 2686 */	NdrFcShort( 0x712 ),	/* Type Offset=1810 */

	/* Parameter State */

/* 2688 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2690 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 2692 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2694 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2696 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 2698 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrDfsCreateExitPoint */

/* 2700 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 2702 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2706 */	NdrFcShort( 0x2d ),	/* 45 */
/* 2708 */	NdrFcShort( 0x38 ),	/* X64 Stack size/offset = 56 */
/* 2710 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 2712 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2714 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 2716 */	NdrFcShort( 0x54 ),	/* 84 */
/* 2718 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2720 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 2722 */	0xa,		/* 10 */
			0x43,		/* Ext Flags:  new corr desc, clt corr check, has range on conformance */
/* 2724 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2726 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2728 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2730 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 2732 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2734 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2736 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter Uid */

/* 2738 */	NdrFcShort( 0x10a ),	/* Flags:  must free, in, simple ref, */
/* 2740 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 2742 */	NdrFcShort( 0xd0c ),	/* Type Offset=3340 */

	/* Parameter Prefix */

/* 2744 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2746 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 2748 */	NdrFcShort( 0x712 ),	/* Type Offset=1810 */

	/* Parameter Type */

/* 2750 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2752 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 2754 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ShortPrefixLen */

/* 2756 */	NdrFcShort( 0x88 ),	/* Flags:  in, by val, */
/* 2758 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 2760 */	NdrFcShort( 0xd68 ),	/* 3432 */

	/* Parameter ShortPrefix */

/* 2762 */	NdrFcShort( 0x113 ),	/* Flags:  must size, must free, out, simple ref, */
/* 2764 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 2766 */	NdrFcShort( 0xd76 ),	/* Type Offset=3446 */

	/* Return value */

/* 2768 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2770 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 2772 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrDfsDeleteExitPoint */

/* 2774 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 2776 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2780 */	NdrFcShort( 0x2e ),	/* 46 */
/* 2782 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 2784 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 2786 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2788 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 2790 */	NdrFcShort( 0x4c ),	/* 76 */
/* 2792 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2794 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 2796 */	0xa,		/* 10 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 2798 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2800 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2802 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2804 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 2806 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2808 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2810 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter Uid */

/* 2812 */	NdrFcShort( 0x10a ),	/* Flags:  must free, in, simple ref, */
/* 2814 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 2816 */	NdrFcShort( 0xd0c ),	/* Type Offset=3340 */

	/* Parameter Prefix */

/* 2818 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2820 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 2822 */	NdrFcShort( 0x712 ),	/* Type Offset=1810 */

	/* Parameter Type */

/* 2824 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2826 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 2828 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2830 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2832 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 2834 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrDfsModifyPrefix */

/* 2836 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 2838 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2842 */	NdrFcShort( 0x2f ),	/* 47 */
/* 2844 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 2846 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 2848 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2850 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 2852 */	NdrFcShort( 0x44 ),	/* 68 */
/* 2854 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2856 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 2858 */	0xa,		/* 10 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 2860 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2862 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2864 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2866 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 2868 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2870 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2872 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter Uid */

/* 2874 */	NdrFcShort( 0x10a ),	/* Flags:  must free, in, simple ref, */
/* 2876 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 2878 */	NdrFcShort( 0xd0c ),	/* Type Offset=3340 */

	/* Parameter Prefix */

/* 2880 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2882 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 2884 */	NdrFcShort( 0x712 ),	/* Type Offset=1810 */

	/* Return value */

/* 2886 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2888 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 2890 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrDfsFixLocalVolume */

/* 2892 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 2894 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2898 */	NdrFcShort( 0x30 ),	/* 48 */
/* 2900 */	NdrFcShort( 0x50 ),	/* X64 Stack size/offset = 80 */
/* 2902 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 2904 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2906 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 2908 */	NdrFcShort( 0x5c ),	/* 92 */
/* 2910 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2912 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0xa,		/* 10 */
/* 2914 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 2916 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2918 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2920 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2922 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 2924 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2926 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2928 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter VolumeName */

/* 2930 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2932 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 2934 */	NdrFcShort( 0x712 ),	/* Type Offset=1810 */

	/* Parameter EntryType */

/* 2936 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2938 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 2940 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ServiceType */

/* 2942 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2944 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 2946 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter StgId */

/* 2948 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2950 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 2952 */	NdrFcShort( 0x712 ),	/* Type Offset=1810 */

	/* Parameter EntryUid */

/* 2954 */	NdrFcShort( 0x10a ),	/* Flags:  must free, in, simple ref, */
/* 2956 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 2958 */	NdrFcShort( 0xd0c ),	/* Type Offset=3340 */

	/* Parameter EntryPrefix */

/* 2960 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2962 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 2964 */	NdrFcShort( 0x712 ),	/* Type Offset=1810 */

	/* Parameter RelationInfo */

/* 2966 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2968 */	NdrFcShort( 0x38 ),	/* X64 Stack size/offset = 56 */
/* 2970 */	NdrFcShort( 0xd58 ),	/* Type Offset=3416 */

	/* Parameter CreateDisposition */

/* 2972 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2974 */	NdrFcShort( 0x40 ),	/* X64 Stack size/offset = 64 */
/* 2976 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2978 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2980 */	NdrFcShort( 0x48 ),	/* X64 Stack size/offset = 72 */
/* 2982 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrDfsManagerReportSiteInfo */

/* 2984 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 2986 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2990 */	NdrFcShort( 0x31 ),	/* 49 */
/* 2992 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 2994 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 2996 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 2998 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 3000 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3002 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3004 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 3006 */	0xa,		/* 10 */
			0x47,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, has range on conformance */
/* 3008 */	NdrFcShort( 0x1 ),	/* 1 */
/* 3010 */	NdrFcShort( 0x1 ),	/* 1 */
/* 3012 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3014 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 3016 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 3018 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 3020 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter ppSiteInfo */

/* 3022 */	NdrFcShort( 0x201b ),	/* Flags:  must size, must free, in, out, srv alloc size=8 */
/* 3024 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 3026 */	NdrFcShort( 0xd8c ),	/* Type Offset=3468 */

	/* Return value */

/* 3028 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3030 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 3032 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrServerTransportDelEx */

/* 3034 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 3036 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3040 */	NdrFcShort( 0x32 ),	/* 50 */
/* 3042 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 3044 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 3046 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 3048 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 3050 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3052 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3054 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 3056 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 3058 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3060 */	NdrFcShort( 0x1 ),	/* 1 */
/* 3062 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3064 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 3066 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 3068 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 3070 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter Level */

/* 3072 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3074 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 3076 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter Buffer */

/* 3078 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3080 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 3082 */	NdrFcShort( 0xdde ),	/* Type Offset=3550 */

	/* Return value */

/* 3084 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3086 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 3088 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrServerAliasAdd */

/* 3090 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 3092 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3096 */	NdrFcShort( 0x33 ),	/* 51 */
/* 3098 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 3100 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 3102 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 3104 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 3106 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3108 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3110 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 3112 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 3114 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3116 */	NdrFcShort( 0x1 ),	/* 1 */
/* 3118 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3120 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 3122 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 3124 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 3126 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter Level */

/* 3128 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3130 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 3132 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter InfoStruct */

/* 3134 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3136 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 3138 */	NdrFcShort( 0xdf6 ),	/* Type Offset=3574 */

	/* Return value */

/* 3140 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3142 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 3144 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrServerAliasEnum */

/* 3146 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 3148 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3152 */	NdrFcShort( 0x34 ),	/* 52 */
/* 3154 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 3156 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 3158 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 3160 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 3162 */	NdrFcShort( 0x24 ),	/* 36 */
/* 3164 */	NdrFcShort( 0x40 ),	/* 64 */
/* 3166 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 3168 */	0xa,		/* 10 */
			0x47,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, has range on conformance */
/* 3170 */	NdrFcShort( 0x1 ),	/* 1 */
/* 3172 */	NdrFcShort( 0x1 ),	/* 1 */
/* 3174 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3176 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 3178 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 3180 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 3182 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter InfoStruct */

/* 3184 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
/* 3186 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 3188 */	NdrFcShort( 0xe92 ),	/* Type Offset=3730 */

	/* Parameter PreferedMaximumLength */

/* 3190 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3192 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 3194 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter TotalEntries */

/* 3196 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 3198 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 3200 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ResumeHandle */

/* 3202 */	NdrFcShort( 0x1a ),	/* Flags:  must free, in, out, */
/* 3204 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 3206 */	NdrFcShort( 0xe0 ),	/* Type Offset=224 */

	/* Return value */

/* 3208 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3210 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 3212 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrServerAliasDel */

/* 3214 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 3216 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3220 */	NdrFcShort( 0x35 ),	/* 53 */
/* 3222 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 3224 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 3226 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 3228 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 3230 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3232 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3234 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 3236 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 3238 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3240 */	NdrFcShort( 0x1 ),	/* 1 */
/* 3242 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3244 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 3246 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 3248 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 3250 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter Level */

/* 3252 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3254 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 3256 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter InfoStruct */

/* 3258 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3260 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 3262 */	NdrFcShort( 0xea6 ),	/* Type Offset=3750 */

	/* Return value */

/* 3264 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3266 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 3268 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NetrShareDelEx */

/* 3270 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 3272 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3276 */	NdrFcShort( 0x36 ),	/* 54 */
/* 3278 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 3280 */	0x31,		/* FC_BIND_GENERIC */
			0x8,		/* 8 */
/* 3282 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 3284 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 3286 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3288 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3290 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 3292 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 3294 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3296 */	NdrFcShort( 0x1 ),	/* 1 */
/* 3298 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3300 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ServerName */

/* 3302 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 3304 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 3306 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter Level */

/* 3308 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3310 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 3312 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ShareInfo */

/* 3314 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3316 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 3318 */	NdrFcShort( 0xebe ),	/* Type Offset=3774 */

	/* Return value */

/* 3320 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3322 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 3324 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const srvsvc_MIDL_TYPE_FORMAT_STRING srvsvc__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/*  4 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/*  6 */	
			0x11, 0x0,	/* FC_RP */
/*  8 */	NdrFcShort( 0xc4 ),	/* Offset= 196 (204) */
/* 10 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 12 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 14 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 16 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 18 */	0x0 , 
			0x0,		/* 0 */
/* 20 */	NdrFcLong( 0x0 ),	/* 0 */
/* 24 */	NdrFcLong( 0x0 ),	/* 0 */
/* 28 */	NdrFcShort( 0x2 ),	/* Offset= 2 (30) */
/* 30 */	NdrFcShort( 0x8 ),	/* 8 */
/* 32 */	NdrFcShort( 0x3002 ),	/* 12290 */
/* 34 */	NdrFcLong( 0x0 ),	/* 0 */
/* 38 */	NdrFcShort( 0xa ),	/* Offset= 10 (48) */
/* 40 */	NdrFcLong( 0x1 ),	/* 1 */
/* 44 */	NdrFcShort( 0x48 ),	/* Offset= 72 (116) */
/* 46 */	NdrFcShort( 0xffff ),	/* Offset= -1 (45) */
/* 48 */	
			0x12, 0x0,	/* FC_UP */
/* 50 */	NdrFcShort( 0x32 ),	/* Offset= 50 (100) */
/* 52 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 54 */	NdrFcShort( 0x4 ),	/* 4 */
/* 56 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 58 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 60 */	NdrFcShort( 0x0 ),	/* 0 */
/* 62 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 64 */	NdrFcShort( 0x0 ),	/* 0 */
/* 66 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 68 */	0x0 , 
			0x0,		/* 0 */
/* 70 */	NdrFcLong( 0x0 ),	/* 0 */
/* 74 */	NdrFcLong( 0x0 ),	/* 0 */
/* 78 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 82 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 84 */	0x0 , 
			0x0,		/* 0 */
/* 86 */	NdrFcLong( 0x0 ),	/* 0 */
/* 90 */	NdrFcLong( 0x0 ),	/* 0 */
/* 94 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 96 */	NdrFcShort( 0xffd4 ),	/* Offset= -44 (52) */
/* 98 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 100 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 102 */	NdrFcShort( 0x10 ),	/* 16 */
/* 104 */	NdrFcShort( 0x0 ),	/* 0 */
/* 106 */	NdrFcShort( 0x6 ),	/* Offset= 6 (112) */
/* 108 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 110 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 112 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 114 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (58) */
/* 116 */	
			0x12, 0x0,	/* FC_UP */
/* 118 */	NdrFcShort( 0x46 ),	/* Offset= 70 (188) */
/* 120 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 122 */	NdrFcShort( 0x28 ),	/* 40 */
/* 124 */	NdrFcShort( 0x0 ),	/* 0 */
/* 126 */	NdrFcShort( 0xc ),	/* Offset= 12 (138) */
/* 128 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 130 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 132 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 134 */	0x36,		/* FC_POINTER */
			0x36,		/* FC_POINTER */
/* 136 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 138 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 140 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 142 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 144 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 146 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 148 */	NdrFcShort( 0x0 ),	/* 0 */
/* 150 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 152 */	NdrFcShort( 0x0 ),	/* 0 */
/* 154 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 156 */	0x0 , 
			0x0,		/* 0 */
/* 158 */	NdrFcLong( 0x0 ),	/* 0 */
/* 162 */	NdrFcLong( 0x0 ),	/* 0 */
/* 166 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 170 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 172 */	0x0 , 
			0x0,		/* 0 */
/* 174 */	NdrFcLong( 0x0 ),	/* 0 */
/* 178 */	NdrFcLong( 0x0 ),	/* 0 */
/* 182 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 184 */	NdrFcShort( 0xffc0 ),	/* Offset= -64 (120) */
/* 186 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 188 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 190 */	NdrFcShort( 0x10 ),	/* 16 */
/* 192 */	NdrFcShort( 0x0 ),	/* 0 */
/* 194 */	NdrFcShort( 0x6 ),	/* Offset= 6 (200) */
/* 196 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 198 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 200 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 202 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (146) */
/* 204 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 206 */	NdrFcShort( 0x10 ),	/* 16 */
/* 208 */	NdrFcShort( 0x0 ),	/* 0 */
/* 210 */	NdrFcShort( 0x0 ),	/* Offset= 0 (210) */
/* 212 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 214 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 216 */	NdrFcShort( 0xff32 ),	/* Offset= -206 (10) */
/* 218 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 220 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 222 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 224 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 226 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 228 */	
			0x11, 0x0,	/* FC_RP */
/* 230 */	NdrFcShort( 0x92 ),	/* Offset= 146 (376) */
/* 232 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 234 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 236 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 238 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 240 */	0x0 , 
			0x0,		/* 0 */
/* 242 */	NdrFcLong( 0x0 ),	/* 0 */
/* 246 */	NdrFcLong( 0x0 ),	/* 0 */
/* 250 */	NdrFcShort( 0x2 ),	/* Offset= 2 (252) */
/* 252 */	NdrFcShort( 0x8 ),	/* 8 */
/* 254 */	NdrFcShort( 0x3002 ),	/* 12290 */
/* 256 */	NdrFcLong( 0x2 ),	/* 2 */
/* 260 */	NdrFcShort( 0xa ),	/* Offset= 10 (270) */
/* 262 */	NdrFcLong( 0x3 ),	/* 3 */
/* 266 */	NdrFcShort( 0x18 ),	/* Offset= 24 (290) */
/* 268 */	NdrFcShort( 0xffff ),	/* Offset= -1 (267) */
/* 270 */	
			0x12, 0x0,	/* FC_UP */
/* 272 */	NdrFcShort( 0x2 ),	/* Offset= 2 (274) */
/* 274 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 276 */	NdrFcShort( 0x10 ),	/* 16 */
/* 278 */	NdrFcShort( 0x0 ),	/* 0 */
/* 280 */	NdrFcShort( 0x6 ),	/* Offset= 6 (286) */
/* 282 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 284 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 286 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 288 */	NdrFcShort( 0xff1a ),	/* Offset= -230 (58) */
/* 290 */	
			0x12, 0x0,	/* FC_UP */
/* 292 */	NdrFcShort( 0x44 ),	/* Offset= 68 (360) */
/* 294 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 296 */	NdrFcShort( 0x20 ),	/* 32 */
/* 298 */	NdrFcShort( 0x0 ),	/* 0 */
/* 300 */	NdrFcShort( 0xa ),	/* Offset= 10 (310) */
/* 302 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 304 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 306 */	0x36,		/* FC_POINTER */
			0x36,		/* FC_POINTER */
/* 308 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 310 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 312 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 314 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 316 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 318 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 320 */	NdrFcShort( 0x0 ),	/* 0 */
/* 322 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 324 */	NdrFcShort( 0x0 ),	/* 0 */
/* 326 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 328 */	0x0 , 
			0x0,		/* 0 */
/* 330 */	NdrFcLong( 0x0 ),	/* 0 */
/* 334 */	NdrFcLong( 0x0 ),	/* 0 */
/* 338 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 342 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 344 */	0x0 , 
			0x0,		/* 0 */
/* 346 */	NdrFcLong( 0x0 ),	/* 0 */
/* 350 */	NdrFcLong( 0x0 ),	/* 0 */
/* 354 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 356 */	NdrFcShort( 0xffc2 ),	/* Offset= -62 (294) */
/* 358 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 360 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 362 */	NdrFcShort( 0x10 ),	/* 16 */
/* 364 */	NdrFcShort( 0x0 ),	/* 0 */
/* 366 */	NdrFcShort( 0x6 ),	/* Offset= 6 (372) */
/* 368 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 370 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 372 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 374 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (318) */
/* 376 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 378 */	NdrFcShort( 0x10 ),	/* 16 */
/* 380 */	NdrFcShort( 0x0 ),	/* 0 */
/* 382 */	NdrFcShort( 0x0 ),	/* Offset= 0 (382) */
/* 384 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 386 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 388 */	NdrFcShort( 0xff64 ),	/* Offset= -156 (232) */
/* 390 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 392 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 394 */	NdrFcShort( 0x2 ),	/* Offset= 2 (396) */
/* 396 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 398 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 400 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 402 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 404 */	0x0 , 
			0x0,		/* 0 */
/* 406 */	NdrFcLong( 0x0 ),	/* 0 */
/* 410 */	NdrFcLong( 0x0 ),	/* 0 */
/* 414 */	NdrFcShort( 0x2 ),	/* Offset= 2 (416) */
/* 416 */	NdrFcShort( 0x8 ),	/* 8 */
/* 418 */	NdrFcShort( 0x3002 ),	/* 12290 */
/* 420 */	NdrFcLong( 0x2 ),	/* 2 */
/* 424 */	NdrFcShort( 0xa ),	/* Offset= 10 (434) */
/* 426 */	NdrFcLong( 0x3 ),	/* 3 */
/* 430 */	NdrFcShort( 0x8 ),	/* Offset= 8 (438) */
/* 432 */	NdrFcShort( 0xffff ),	/* Offset= -1 (431) */
/* 434 */	
			0x12, 0x0,	/* FC_UP */
/* 436 */	NdrFcShort( 0xfe80 ),	/* Offset= -384 (52) */
/* 438 */	
			0x12, 0x0,	/* FC_UP */
/* 440 */	NdrFcShort( 0xff6e ),	/* Offset= -146 (294) */
/* 442 */	
			0x11, 0x0,	/* FC_RP */
/* 444 */	NdrFcShort( 0x1ea ),	/* Offset= 490 (934) */
/* 446 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 448 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 450 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 452 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 454 */	0x0 , 
			0x0,		/* 0 */
/* 456 */	NdrFcLong( 0x0 ),	/* 0 */
/* 460 */	NdrFcLong( 0x0 ),	/* 0 */
/* 464 */	NdrFcShort( 0x2 ),	/* Offset= 2 (466) */
/* 466 */	NdrFcShort( 0x8 ),	/* 8 */
/* 468 */	NdrFcShort( 0x3005 ),	/* 12293 */
/* 470 */	NdrFcLong( 0x0 ),	/* 0 */
/* 474 */	NdrFcShort( 0x1c ),	/* Offset= 28 (502) */
/* 476 */	NdrFcLong( 0x1 ),	/* 1 */
/* 480 */	NdrFcShort( 0x62 ),	/* Offset= 98 (578) */
/* 482 */	NdrFcLong( 0x2 ),	/* 2 */
/* 486 */	NdrFcShort( 0xb2 ),	/* Offset= 178 (664) */
/* 488 */	NdrFcLong( 0xa ),	/* 10 */
/* 492 */	NdrFcShort( 0x106 ),	/* Offset= 262 (754) */
/* 494 */	NdrFcLong( 0x1f6 ),	/* 502 */
/* 498 */	NdrFcShort( 0x154 ),	/* Offset= 340 (838) */
/* 500 */	NdrFcShort( 0xffff ),	/* Offset= -1 (499) */
/* 502 */	
			0x12, 0x0,	/* FC_UP */
/* 504 */	NdrFcShort( 0x3a ),	/* Offset= 58 (562) */
/* 506 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 508 */	NdrFcShort( 0x8 ),	/* 8 */
/* 510 */	NdrFcShort( 0x0 ),	/* 0 */
/* 512 */	NdrFcShort( 0x4 ),	/* Offset= 4 (516) */
/* 514 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 516 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 518 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 520 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 522 */	NdrFcShort( 0x0 ),	/* 0 */
/* 524 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 526 */	NdrFcShort( 0x0 ),	/* 0 */
/* 528 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 530 */	0x0 , 
			0x0,		/* 0 */
/* 532 */	NdrFcLong( 0x0 ),	/* 0 */
/* 536 */	NdrFcLong( 0x0 ),	/* 0 */
/* 540 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 544 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 546 */	0x0 , 
			0x0,		/* 0 */
/* 548 */	NdrFcLong( 0x0 ),	/* 0 */
/* 552 */	NdrFcLong( 0x0 ),	/* 0 */
/* 556 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 558 */	NdrFcShort( 0xffcc ),	/* Offset= -52 (506) */
/* 560 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 562 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 564 */	NdrFcShort( 0x10 ),	/* 16 */
/* 566 */	NdrFcShort( 0x0 ),	/* 0 */
/* 568 */	NdrFcShort( 0x6 ),	/* Offset= 6 (574) */
/* 570 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 572 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 574 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 576 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (520) */
/* 578 */	
			0x12, 0x0,	/* FC_UP */
/* 580 */	NdrFcShort( 0x44 ),	/* Offset= 68 (648) */
/* 582 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 584 */	NdrFcShort( 0x20 ),	/* 32 */
/* 586 */	NdrFcShort( 0x0 ),	/* 0 */
/* 588 */	NdrFcShort( 0xa ),	/* Offset= 10 (598) */
/* 590 */	0x36,		/* FC_POINTER */
			0x36,		/* FC_POINTER */
/* 592 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 594 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 596 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 598 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 600 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 602 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 604 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 606 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 608 */	NdrFcShort( 0x0 ),	/* 0 */
/* 610 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 612 */	NdrFcShort( 0x0 ),	/* 0 */
/* 614 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 616 */	0x0 , 
			0x0,		/* 0 */
/* 618 */	NdrFcLong( 0x0 ),	/* 0 */
/* 622 */	NdrFcLong( 0x0 ),	/* 0 */
/* 626 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 630 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 632 */	0x0 , 
			0x0,		/* 0 */
/* 634 */	NdrFcLong( 0x0 ),	/* 0 */
/* 638 */	NdrFcLong( 0x0 ),	/* 0 */
/* 642 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 644 */	NdrFcShort( 0xffc2 ),	/* Offset= -62 (582) */
/* 646 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 648 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 650 */	NdrFcShort( 0x10 ),	/* 16 */
/* 652 */	NdrFcShort( 0x0 ),	/* 0 */
/* 654 */	NdrFcShort( 0x6 ),	/* Offset= 6 (660) */
/* 656 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 658 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 660 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 662 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (606) */
/* 664 */	
			0x12, 0x0,	/* FC_UP */
/* 666 */	NdrFcShort( 0x48 ),	/* Offset= 72 (738) */
/* 668 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 670 */	NdrFcShort( 0x28 ),	/* 40 */
/* 672 */	NdrFcShort( 0x0 ),	/* 0 */
/* 674 */	NdrFcShort( 0xa ),	/* Offset= 10 (684) */
/* 676 */	0x36,		/* FC_POINTER */
			0x36,		/* FC_POINTER */
/* 678 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 680 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 682 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 684 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 686 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 688 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 690 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 692 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 694 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 696 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 698 */	NdrFcShort( 0x0 ),	/* 0 */
/* 700 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 702 */	NdrFcShort( 0x0 ),	/* 0 */
/* 704 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 706 */	0x0 , 
			0x0,		/* 0 */
/* 708 */	NdrFcLong( 0x0 ),	/* 0 */
/* 712 */	NdrFcLong( 0x0 ),	/* 0 */
/* 716 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 720 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 722 */	0x0 , 
			0x0,		/* 0 */
/* 724 */	NdrFcLong( 0x0 ),	/* 0 */
/* 728 */	NdrFcLong( 0x0 ),	/* 0 */
/* 732 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 734 */	NdrFcShort( 0xffbe ),	/* Offset= -66 (668) */
/* 736 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 738 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 740 */	NdrFcShort( 0x10 ),	/* 16 */
/* 742 */	NdrFcShort( 0x0 ),	/* 0 */
/* 744 */	NdrFcShort( 0x6 ),	/* Offset= 6 (750) */
/* 746 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 748 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 750 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 752 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (696) */
/* 754 */	
			0x12, 0x0,	/* FC_UP */
/* 756 */	NdrFcShort( 0x42 ),	/* Offset= 66 (822) */
/* 758 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 760 */	NdrFcShort( 0x18 ),	/* 24 */
/* 762 */	NdrFcShort( 0x0 ),	/* 0 */
/* 764 */	NdrFcShort( 0x8 ),	/* Offset= 8 (772) */
/* 766 */	0x36,		/* FC_POINTER */
			0x36,		/* FC_POINTER */
/* 768 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 770 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 772 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 774 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 776 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 778 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 780 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 782 */	NdrFcShort( 0x0 ),	/* 0 */
/* 784 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 786 */	NdrFcShort( 0x0 ),	/* 0 */
/* 788 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 790 */	0x0 , 
			0x0,		/* 0 */
/* 792 */	NdrFcLong( 0x0 ),	/* 0 */
/* 796 */	NdrFcLong( 0x0 ),	/* 0 */
/* 800 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 804 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 806 */	0x0 , 
			0x0,		/* 0 */
/* 808 */	NdrFcLong( 0x0 ),	/* 0 */
/* 812 */	NdrFcLong( 0x0 ),	/* 0 */
/* 816 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 818 */	NdrFcShort( 0xffc4 ),	/* Offset= -60 (758) */
/* 820 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 822 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 824 */	NdrFcShort( 0x10 ),	/* 16 */
/* 826 */	NdrFcShort( 0x0 ),	/* 0 */
/* 828 */	NdrFcShort( 0x6 ),	/* Offset= 6 (834) */
/* 830 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 832 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 834 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 836 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (780) */
/* 838 */	
			0x12, 0x0,	/* FC_UP */
/* 840 */	NdrFcShort( 0x4e ),	/* Offset= 78 (918) */
/* 842 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 844 */	NdrFcShort( 0x30 ),	/* 48 */
/* 846 */	NdrFcShort( 0x0 ),	/* 0 */
/* 848 */	NdrFcShort( 0xc ),	/* Offset= 12 (860) */
/* 850 */	0x36,		/* FC_POINTER */
			0x36,		/* FC_POINTER */
/* 852 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 854 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 856 */	0x36,		/* FC_POINTER */
			0x36,		/* FC_POINTER */
/* 858 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 860 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 862 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 864 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 866 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 868 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 870 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 872 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 874 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 876 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 878 */	NdrFcShort( 0x0 ),	/* 0 */
/* 880 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 882 */	NdrFcShort( 0x0 ),	/* 0 */
/* 884 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 886 */	0x0 , 
			0x0,		/* 0 */
/* 888 */	NdrFcLong( 0x0 ),	/* 0 */
/* 892 */	NdrFcLong( 0x0 ),	/* 0 */
/* 896 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 900 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 902 */	0x0 , 
			0x0,		/* 0 */
/* 904 */	NdrFcLong( 0x0 ),	/* 0 */
/* 908 */	NdrFcLong( 0x0 ),	/* 0 */
/* 912 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 914 */	NdrFcShort( 0xffb8 ),	/* Offset= -72 (842) */
/* 916 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 918 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 920 */	NdrFcShort( 0x10 ),	/* 16 */
/* 922 */	NdrFcShort( 0x0 ),	/* 0 */
/* 924 */	NdrFcShort( 0x6 ),	/* Offset= 6 (930) */
/* 926 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 928 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 930 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 932 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (876) */
/* 934 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 936 */	NdrFcShort( 0x10 ),	/* 16 */
/* 938 */	NdrFcShort( 0x0 ),	/* 0 */
/* 940 */	NdrFcShort( 0x0 ),	/* Offset= 0 (940) */
/* 942 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 944 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 946 */	NdrFcShort( 0xfe0c ),	/* Offset= -500 (446) */
/* 948 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 950 */	
			0x11, 0x0,	/* FC_RP */
/* 952 */	NdrFcShort( 0x2 ),	/* Offset= 2 (954) */
/* 954 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 956 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 958 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 960 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 962 */	0x0 , 
			0x0,		/* 0 */
/* 964 */	NdrFcLong( 0x0 ),	/* 0 */
/* 968 */	NdrFcLong( 0x0 ),	/* 0 */
/* 972 */	NdrFcShort( 0x2 ),	/* Offset= 2 (974) */
/* 974 */	NdrFcShort( 0x8 ),	/* 8 */
/* 976 */	NdrFcShort( 0x300a ),	/* 12298 */
/* 978 */	NdrFcLong( 0x0 ),	/* 0 */
/* 982 */	NdrFcShort( 0x3a ),	/* Offset= 58 (1040) */
/* 984 */	NdrFcLong( 0x1 ),	/* 1 */
/* 988 */	NdrFcShort( 0x46 ),	/* Offset= 70 (1058) */
/* 990 */	NdrFcLong( 0x2 ),	/* 2 */
/* 994 */	NdrFcShort( 0x5a ),	/* Offset= 90 (1084) */
/* 996 */	NdrFcLong( 0x1f6 ),	/* 502 */
/* 1000 */	NdrFcShort( 0x7c ),	/* Offset= 124 (1124) */
/* 1002 */	NdrFcLong( 0x3ec ),	/* 1004 */
/* 1006 */	NdrFcShort( 0xba ),	/* Offset= 186 (1192) */
/* 1008 */	NdrFcLong( 0x3ee ),	/* 1006 */
/* 1012 */	NdrFcShort( 0xfdbe ),	/* Offset= -578 (434) */
/* 1014 */	NdrFcLong( 0x5dd ),	/* 1501 */
/* 1018 */	NdrFcShort( 0xc0 ),	/* Offset= 192 (1210) */
/* 1020 */	NdrFcLong( 0x3ed ),	/* 1005 */
/* 1024 */	NdrFcShort( 0xfdb2 ),	/* Offset= -590 (434) */
/* 1026 */	NdrFcLong( 0x1f5 ),	/* 501 */
/* 1030 */	NdrFcShort( 0xde ),	/* Offset= 222 (1252) */
/* 1032 */	NdrFcLong( 0x1f7 ),	/* 503 */
/* 1036 */	NdrFcShort( 0xf4 ),	/* Offset= 244 (1280) */
/* 1038 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1038) */
/* 1040 */	
			0x12, 0x0,	/* FC_UP */
/* 1042 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1044) */
/* 1044 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1046 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1048 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1050 */	NdrFcShort( 0x4 ),	/* Offset= 4 (1054) */
/* 1052 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 1054 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1056 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 1058 */	
			0x12, 0x0,	/* FC_UP */
/* 1060 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1062) */
/* 1062 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1064 */	NdrFcShort( 0x18 ),	/* 24 */
/* 1066 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1068 */	NdrFcShort( 0x8 ),	/* Offset= 8 (1076) */
/* 1070 */	0x36,		/* FC_POINTER */
			0x8,		/* FC_LONG */
/* 1072 */	0x40,		/* FC_STRUCTPAD4 */
			0x36,		/* FC_POINTER */
/* 1074 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1076 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1078 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 1080 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1082 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 1084 */	
			0x12, 0x0,	/* FC_UP */
/* 1086 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1088) */
/* 1088 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1090 */	NdrFcShort( 0x38 ),	/* 56 */
/* 1092 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1094 */	NdrFcShort( 0xe ),	/* Offset= 14 (1108) */
/* 1096 */	0x36,		/* FC_POINTER */
			0x8,		/* FC_LONG */
/* 1098 */	0x40,		/* FC_STRUCTPAD4 */
			0x36,		/* FC_POINTER */
/* 1100 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1102 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 1104 */	0x36,		/* FC_POINTER */
			0x36,		/* FC_POINTER */
/* 1106 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1108 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1110 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 1112 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1114 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 1116 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1118 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 1120 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1122 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 1124 */	
			0x12, 0x0,	/* FC_UP */
/* 1126 */	NdrFcShort( 0x18 ),	/* Offset= 24 (1150) */
/* 1128 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 1130 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1132 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1134 */	NdrFcShort( 0x38 ),	/* 56 */
/* 1136 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1138 */	0x0 , 
			0x0,		/* 0 */
/* 1140 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1144 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1148 */	0x2,		/* FC_CHAR */
			0x5b,		/* FC_END */
/* 1150 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1152 */	NdrFcShort( 0x48 ),	/* 72 */
/* 1154 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1156 */	NdrFcShort( 0x10 ),	/* Offset= 16 (1172) */
/* 1158 */	0x36,		/* FC_POINTER */
			0x8,		/* FC_LONG */
/* 1160 */	0x40,		/* FC_STRUCTPAD4 */
			0x36,		/* FC_POINTER */
/* 1162 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1164 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 1166 */	0x36,		/* FC_POINTER */
			0x36,		/* FC_POINTER */
/* 1168 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 1170 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 1172 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1174 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 1176 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1178 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 1180 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1182 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 1184 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1186 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 1188 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 1190 */	NdrFcShort( 0xffc2 ),	/* Offset= -62 (1128) */
/* 1192 */	
			0x12, 0x0,	/* FC_UP */
/* 1194 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1196) */
/* 1196 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1198 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1200 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1202 */	NdrFcShort( 0x4 ),	/* Offset= 4 (1206) */
/* 1204 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 1206 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1208 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 1210 */	
			0x12, 0x0,	/* FC_UP */
/* 1212 */	NdrFcShort( 0x18 ),	/* Offset= 24 (1236) */
/* 1214 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 1216 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1218 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1220 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1222 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1224 */	0x0 , 
			0x0,		/* 0 */
/* 1226 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1230 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1234 */	0x2,		/* FC_CHAR */
			0x5b,		/* FC_END */
/* 1236 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1238 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1240 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1242 */	NdrFcShort( 0x6 ),	/* Offset= 6 (1248) */
/* 1244 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 1246 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 1248 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 1250 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (1214) */
/* 1252 */	
			0x12, 0x0,	/* FC_UP */
/* 1254 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1256) */
/* 1256 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1258 */	NdrFcShort( 0x20 ),	/* 32 */
/* 1260 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1262 */	NdrFcShort( 0xa ),	/* Offset= 10 (1272) */
/* 1264 */	0x36,		/* FC_POINTER */
			0x8,		/* FC_LONG */
/* 1266 */	0x40,		/* FC_STRUCTPAD4 */
			0x36,		/* FC_POINTER */
/* 1268 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 1270 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1272 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1274 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 1276 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1278 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 1280 */	
			0x12, 0x0,	/* FC_UP */
/* 1282 */	NdrFcShort( 0x18 ),	/* Offset= 24 (1306) */
/* 1284 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 1286 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1288 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1290 */	NdrFcShort( 0x40 ),	/* 64 */
/* 1292 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1294 */	0x0 , 
			0x0,		/* 0 */
/* 1296 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1300 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1304 */	0x2,		/* FC_CHAR */
			0x5b,		/* FC_END */
/* 1306 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1308 */	NdrFcShort( 0x50 ),	/* 80 */
/* 1310 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1312 */	NdrFcShort( 0x12 ),	/* Offset= 18 (1330) */
/* 1314 */	0x36,		/* FC_POINTER */
			0x8,		/* FC_LONG */
/* 1316 */	0x40,		/* FC_STRUCTPAD4 */
			0x36,		/* FC_POINTER */
/* 1318 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1320 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 1322 */	0x36,		/* FC_POINTER */
			0x36,		/* FC_POINTER */
/* 1324 */	0x36,		/* FC_POINTER */
			0x8,		/* FC_LONG */
/* 1326 */	0x40,		/* FC_STRUCTPAD4 */
			0x36,		/* FC_POINTER */
/* 1328 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1330 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1332 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 1334 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1336 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 1338 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1340 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 1342 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1344 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 1346 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1348 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 1350 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 1352 */	NdrFcShort( 0xffbc ),	/* Offset= -68 (1284) */
/* 1354 */	
			0x11, 0x0,	/* FC_RP */
/* 1356 */	NdrFcShort( 0x1b4 ),	/* Offset= 436 (1792) */
/* 1358 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 1360 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 1362 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 1364 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1366 */	0x0 , 
			0x0,		/* 0 */
/* 1368 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1372 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1376 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1378) */
/* 1378 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1380 */	NdrFcShort( 0x3006 ),	/* 12294 */
/* 1382 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1386 */	NdrFcShort( 0x22 ),	/* Offset= 34 (1420) */
/* 1388 */	NdrFcLong( 0x1 ),	/* 1 */
/* 1392 */	NdrFcShort( 0x5a ),	/* Offset= 90 (1482) */
/* 1394 */	NdrFcLong( 0x2 ),	/* 2 */
/* 1398 */	NdrFcShort( 0x92 ),	/* Offset= 146 (1544) */
/* 1400 */	NdrFcLong( 0x1f5 ),	/* 501 */
/* 1404 */	NdrFcShort( 0xca ),	/* Offset= 202 (1606) */
/* 1406 */	NdrFcLong( 0x1f6 ),	/* 502 */
/* 1410 */	NdrFcShort( 0x102 ),	/* Offset= 258 (1668) */
/* 1412 */	NdrFcLong( 0x1f7 ),	/* 503 */
/* 1416 */	NdrFcShort( 0x13a ),	/* Offset= 314 (1730) */
/* 1418 */	NdrFcShort( 0xffff ),	/* Offset= -1 (1417) */
/* 1420 */	
			0x12, 0x0,	/* FC_UP */
/* 1422 */	NdrFcShort( 0x2c ),	/* Offset= 44 (1466) */
/* 1424 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 1426 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1428 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1430 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1432 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1434 */	0x0 , 
			0x0,		/* 0 */
/* 1436 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1440 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1444 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 1448 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 1450 */	0x0 , 
			0x0,		/* 0 */
/* 1452 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1456 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1460 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1462 */	NdrFcShort( 0xfe5e ),	/* Offset= -418 (1044) */
/* 1464 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1466 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1468 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1470 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1472 */	NdrFcShort( 0x6 ),	/* Offset= 6 (1478) */
/* 1474 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 1476 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 1478 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 1480 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (1424) */
/* 1482 */	
			0x12, 0x0,	/* FC_UP */
/* 1484 */	NdrFcShort( 0x2c ),	/* Offset= 44 (1528) */
/* 1486 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 1488 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1490 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1492 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1494 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1496 */	0x0 , 
			0x0,		/* 0 */
/* 1498 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1502 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1506 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 1510 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 1512 */	0x0 , 
			0x0,		/* 0 */
/* 1514 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1518 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1522 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1524 */	NdrFcShort( 0xfe32 ),	/* Offset= -462 (1062) */
/* 1526 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1528 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1530 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1532 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1534 */	NdrFcShort( 0x6 ),	/* Offset= 6 (1540) */
/* 1536 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 1538 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 1540 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 1542 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (1486) */
/* 1544 */	
			0x12, 0x0,	/* FC_UP */
/* 1546 */	NdrFcShort( 0x2c ),	/* Offset= 44 (1590) */
/* 1548 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 1550 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1552 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1554 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1556 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1558 */	0x0 , 
			0x0,		/* 0 */
/* 1560 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1564 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1568 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 1572 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 1574 */	0x0 , 
			0x0,		/* 0 */
/* 1576 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1580 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1584 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1586 */	NdrFcShort( 0xfe0e ),	/* Offset= -498 (1088) */
/* 1588 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1590 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1592 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1594 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1596 */	NdrFcShort( 0x6 ),	/* Offset= 6 (1602) */
/* 1598 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 1600 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 1602 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 1604 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (1548) */
/* 1606 */	
			0x12, 0x0,	/* FC_UP */
/* 1608 */	NdrFcShort( 0x2c ),	/* Offset= 44 (1652) */
/* 1610 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 1612 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1614 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1616 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1618 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1620 */	0x0 , 
			0x0,		/* 0 */
/* 1622 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1626 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1630 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 1634 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 1636 */	0x0 , 
			0x0,		/* 0 */
/* 1638 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1642 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1646 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1648 */	NdrFcShort( 0xfe78 ),	/* Offset= -392 (1256) */
/* 1650 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1652 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1654 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1656 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1658 */	NdrFcShort( 0x6 ),	/* Offset= 6 (1664) */
/* 1660 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 1662 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 1664 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 1666 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (1610) */
/* 1668 */	
			0x12, 0x0,	/* FC_UP */
/* 1670 */	NdrFcShort( 0x2c ),	/* Offset= 44 (1714) */
/* 1672 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 1674 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1676 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1678 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1680 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1682 */	0x0 , 
			0x0,		/* 0 */
/* 1684 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1688 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1692 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 1696 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 1698 */	0x0 , 
			0x0,		/* 0 */
/* 1700 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1704 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1708 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1710 */	NdrFcShort( 0xfdd0 ),	/* Offset= -560 (1150) */
/* 1712 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1714 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1716 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1718 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1720 */	NdrFcShort( 0x6 ),	/* Offset= 6 (1726) */
/* 1722 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 1724 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 1726 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 1728 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (1672) */
/* 1730 */	
			0x12, 0x0,	/* FC_UP */
/* 1732 */	NdrFcShort( 0x2c ),	/* Offset= 44 (1776) */
/* 1734 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 1736 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1738 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1740 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1742 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1744 */	0x0 , 
			0x0,		/* 0 */
/* 1746 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1750 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1754 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 1758 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 1760 */	0x0 , 
			0x0,		/* 0 */
/* 1762 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1766 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1770 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1772 */	NdrFcShort( 0xfe2e ),	/* Offset= -466 (1306) */
/* 1774 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1776 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1778 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1780 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1782 */	NdrFcShort( 0x6 ),	/* Offset= 6 (1788) */
/* 1784 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 1786 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 1788 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 1790 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (1734) */
/* 1792 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1794 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1796 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1798 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1798) */
/* 1800 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 1802 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1804 */	NdrFcShort( 0xfe42 ),	/* Offset= -446 (1358) */
/* 1806 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1808 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/* 1810 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 1812 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 1814 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1816) */
/* 1816 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 1818 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 1820 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1822 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1824 */	0x0 , 
			0x0,		/* 0 */
/* 1826 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1830 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1834 */	NdrFcShort( 0xfca4 ),	/* Offset= -860 (974) */
/* 1836 */	
			0x11, 0x0,	/* FC_RP */
/* 1838 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1840) */
/* 1840 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 1842 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 1844 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1846 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1848 */	0x0 , 
			0x0,		/* 0 */
/* 1850 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1854 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1858 */	NdrFcShort( 0xfc8c ),	/* Offset= -884 (974) */
/* 1860 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 1862 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1864) */
/* 1864 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 1866 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 1868 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1870 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1872 */	0x0 , 
			0x0,		/* 0 */
/* 1874 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1878 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1882 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1884) */
/* 1884 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1886 */	NdrFcShort( 0x3033 ),	/* 12339 */
/* 1888 */	NdrFcLong( 0x64 ),	/* 100 */
/* 1892 */	NdrFcShort( 0x130 ),	/* Offset= 304 (2196) */
/* 1894 */	NdrFcLong( 0x65 ),	/* 101 */
/* 1898 */	NdrFcShort( 0x13e ),	/* Offset= 318 (2216) */
/* 1900 */	NdrFcLong( 0x66 ),	/* 102 */
/* 1904 */	NdrFcShort( 0x156 ),	/* Offset= 342 (2246) */
/* 1906 */	NdrFcLong( 0x67 ),	/* 103 */
/* 1910 */	NdrFcShort( 0x178 ),	/* Offset= 376 (2286) */
/* 1912 */	NdrFcLong( 0x1f6 ),	/* 502 */
/* 1916 */	NdrFcShort( 0x19c ),	/* Offset= 412 (2328) */
/* 1918 */	NdrFcLong( 0x1f7 ),	/* 503 */
/* 1922 */	NdrFcShort( 0x1b2 ),	/* Offset= 434 (2356) */
/* 1924 */	NdrFcLong( 0x257 ),	/* 599 */
/* 1928 */	NdrFcShort( 0x1e8 ),	/* Offset= 488 (2416) */
/* 1930 */	NdrFcLong( 0x3ed ),	/* 1005 */
/* 1934 */	NdrFcShort( 0x22c ),	/* Offset= 556 (2490) */
/* 1936 */	NdrFcLong( 0x453 ),	/* 1107 */
/* 1940 */	NdrFcShort( 0xfa1e ),	/* Offset= -1506 (434) */
/* 1942 */	NdrFcLong( 0x3f2 ),	/* 1010 */
/* 1946 */	NdrFcShort( 0xfa18 ),	/* Offset= -1512 (434) */
/* 1948 */	NdrFcLong( 0x3f8 ),	/* 1016 */
/* 1952 */	NdrFcShort( 0xfa12 ),	/* Offset= -1518 (434) */
/* 1954 */	NdrFcLong( 0x3f9 ),	/* 1017 */
/* 1958 */	NdrFcShort( 0xfa0c ),	/* Offset= -1524 (434) */
/* 1960 */	NdrFcLong( 0x3fa ),	/* 1018 */
/* 1964 */	NdrFcShort( 0xfa06 ),	/* Offset= -1530 (434) */
/* 1966 */	NdrFcLong( 0x5dd ),	/* 1501 */
/* 1970 */	NdrFcShort( 0xfa00 ),	/* Offset= -1536 (434) */
/* 1972 */	NdrFcLong( 0x5de ),	/* 1502 */
/* 1976 */	NdrFcShort( 0xf9fa ),	/* Offset= -1542 (434) */
/* 1978 */	NdrFcLong( 0x5df ),	/* 1503 */
/* 1982 */	NdrFcShort( 0xf9f4 ),	/* Offset= -1548 (434) */
/* 1984 */	NdrFcLong( 0x5e2 ),	/* 1506 */
/* 1988 */	NdrFcShort( 0xf9ee ),	/* Offset= -1554 (434) */
/* 1990 */	NdrFcLong( 0x5e6 ),	/* 1510 */
/* 1994 */	NdrFcShort( 0xf9e8 ),	/* Offset= -1560 (434) */
/* 1996 */	NdrFcLong( 0x5e7 ),	/* 1511 */
/* 2000 */	NdrFcShort( 0xf9e2 ),	/* Offset= -1566 (434) */
/* 2002 */	NdrFcLong( 0x5e8 ),	/* 1512 */
/* 2006 */	NdrFcShort( 0xf9dc ),	/* Offset= -1572 (434) */
/* 2008 */	NdrFcLong( 0x5e9 ),	/* 1513 */
/* 2012 */	NdrFcShort( 0xf9d6 ),	/* Offset= -1578 (434) */
/* 2014 */	NdrFcLong( 0x5ea ),	/* 1514 */
/* 2018 */	NdrFcShort( 0xf9d0 ),	/* Offset= -1584 (434) */
/* 2020 */	NdrFcLong( 0x5eb ),	/* 1515 */
/* 2024 */	NdrFcShort( 0xf9ca ),	/* Offset= -1590 (434) */
/* 2026 */	NdrFcLong( 0x5ec ),	/* 1516 */
/* 2030 */	NdrFcShort( 0xf9c4 ),	/* Offset= -1596 (434) */
/* 2032 */	NdrFcLong( 0x5ee ),	/* 1518 */
/* 2036 */	NdrFcShort( 0xf9be ),	/* Offset= -1602 (434) */
/* 2038 */	NdrFcLong( 0x5f3 ),	/* 1523 */
/* 2042 */	NdrFcShort( 0xf9b8 ),	/* Offset= -1608 (434) */
/* 2044 */	NdrFcLong( 0x5f8 ),	/* 1528 */
/* 2048 */	NdrFcShort( 0xf9b2 ),	/* Offset= -1614 (434) */
/* 2050 */	NdrFcLong( 0x5f9 ),	/* 1529 */
/* 2054 */	NdrFcShort( 0xf9ac ),	/* Offset= -1620 (434) */
/* 2056 */	NdrFcLong( 0x5fa ),	/* 1530 */
/* 2060 */	NdrFcShort( 0xf9a6 ),	/* Offset= -1626 (434) */
/* 2062 */	NdrFcLong( 0x5fd ),	/* 1533 */
/* 2066 */	NdrFcShort( 0xf9a0 ),	/* Offset= -1632 (434) */
/* 2068 */	NdrFcLong( 0x5fe ),	/* 1534 */
/* 2072 */	NdrFcShort( 0xf99a ),	/* Offset= -1638 (434) */
/* 2074 */	NdrFcLong( 0x5ff ),	/* 1535 */
/* 2078 */	NdrFcShort( 0xf994 ),	/* Offset= -1644 (434) */
/* 2080 */	NdrFcLong( 0x600 ),	/* 1536 */
/* 2084 */	NdrFcShort( 0xf98e ),	/* Offset= -1650 (434) */
/* 2086 */	NdrFcLong( 0x602 ),	/* 1538 */
/* 2090 */	NdrFcShort( 0xf988 ),	/* Offset= -1656 (434) */
/* 2092 */	NdrFcLong( 0x603 ),	/* 1539 */
/* 2096 */	NdrFcShort( 0xf982 ),	/* Offset= -1662 (434) */
/* 2098 */	NdrFcLong( 0x604 ),	/* 1540 */
/* 2102 */	NdrFcShort( 0xf97c ),	/* Offset= -1668 (434) */
/* 2104 */	NdrFcLong( 0x605 ),	/* 1541 */
/* 2108 */	NdrFcShort( 0xf976 ),	/* Offset= -1674 (434) */
/* 2110 */	NdrFcLong( 0x606 ),	/* 1542 */
/* 2114 */	NdrFcShort( 0xf970 ),	/* Offset= -1680 (434) */
/* 2116 */	NdrFcLong( 0x607 ),	/* 1543 */
/* 2120 */	NdrFcShort( 0xf96a ),	/* Offset= -1686 (434) */
/* 2122 */	NdrFcLong( 0x608 ),	/* 1544 */
/* 2126 */	NdrFcShort( 0xf964 ),	/* Offset= -1692 (434) */
/* 2128 */	NdrFcLong( 0x609 ),	/* 1545 */
/* 2132 */	NdrFcShort( 0xf95e ),	/* Offset= -1698 (434) */
/* 2134 */	NdrFcLong( 0x60a ),	/* 1546 */
/* 2138 */	NdrFcShort( 0xf958 ),	/* Offset= -1704 (434) */
/* 2140 */	NdrFcLong( 0x60b ),	/* 1547 */
/* 2144 */	NdrFcShort( 0xf952 ),	/* Offset= -1710 (434) */
/* 2146 */	NdrFcLong( 0x60c ),	/* 1548 */
/* 2150 */	NdrFcShort( 0xf94c ),	/* Offset= -1716 (434) */
/* 2152 */	NdrFcLong( 0x60d ),	/* 1549 */
/* 2156 */	NdrFcShort( 0xf946 ),	/* Offset= -1722 (434) */
/* 2158 */	NdrFcLong( 0x60e ),	/* 1550 */
/* 2162 */	NdrFcShort( 0xf940 ),	/* Offset= -1728 (434) */
/* 2164 */	NdrFcLong( 0x610 ),	/* 1552 */
/* 2168 */	NdrFcShort( 0xf93a ),	/* Offset= -1734 (434) */
/* 2170 */	NdrFcLong( 0x611 ),	/* 1553 */
/* 2174 */	NdrFcShort( 0xf934 ),	/* Offset= -1740 (434) */
/* 2176 */	NdrFcLong( 0x612 ),	/* 1554 */
/* 2180 */	NdrFcShort( 0xf92e ),	/* Offset= -1746 (434) */
/* 2182 */	NdrFcLong( 0x613 ),	/* 1555 */
/* 2186 */	NdrFcShort( 0xf928 ),	/* Offset= -1752 (434) */
/* 2188 */	NdrFcLong( 0x614 ),	/* 1556 */
/* 2192 */	NdrFcShort( 0xf922 ),	/* Offset= -1758 (434) */
/* 2194 */	NdrFcShort( 0xffff ),	/* Offset= -1 (2193) */
/* 2196 */	
			0x12, 0x0,	/* FC_UP */
/* 2198 */	NdrFcShort( 0x2 ),	/* Offset= 2 (2200) */
/* 2200 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 2202 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2204 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2206 */	NdrFcShort( 0x6 ),	/* Offset= 6 (2212) */
/* 2208 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 2210 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 2212 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 2214 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 2216 */	
			0x12, 0x0,	/* FC_UP */
/* 2218 */	NdrFcShort( 0x2 ),	/* Offset= 2 (2220) */
/* 2220 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 2222 */	NdrFcShort( 0x28 ),	/* 40 */
/* 2224 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2226 */	NdrFcShort( 0xc ),	/* Offset= 12 (2238) */
/* 2228 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 2230 */	0x36,		/* FC_POINTER */
			0x8,		/* FC_LONG */
/* 2232 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2234 */	0x40,		/* FC_STRUCTPAD4 */
			0x36,		/* FC_POINTER */
/* 2236 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 2238 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 2240 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 2242 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 2244 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 2246 */	
			0x12, 0x0,	/* FC_UP */
/* 2248 */	NdrFcShort( 0x2 ),	/* Offset= 2 (2250) */
/* 2250 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 2252 */	NdrFcShort( 0x48 ),	/* 72 */
/* 2254 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2256 */	NdrFcShort( 0x12 ),	/* Offset= 18 (2274) */
/* 2258 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 2260 */	0x36,		/* FC_POINTER */
			0x8,		/* FC_LONG */
/* 2262 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2264 */	0x40,		/* FC_STRUCTPAD4 */
			0x36,		/* FC_POINTER */
/* 2266 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2268 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2270 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2272 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 2274 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 2276 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 2278 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 2280 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 2282 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 2284 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 2286 */	
			0x12, 0x0,	/* FC_UP */
/* 2288 */	NdrFcShort( 0x2 ),	/* Offset= 2 (2290) */
/* 2290 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 2292 */	NdrFcShort( 0x50 ),	/* 80 */
/* 2294 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2296 */	NdrFcShort( 0x14 ),	/* Offset= 20 (2316) */
/* 2298 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 2300 */	0x36,		/* FC_POINTER */
			0x8,		/* FC_LONG */
/* 2302 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2304 */	0x40,		/* FC_STRUCTPAD4 */
			0x36,		/* FC_POINTER */
/* 2306 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2308 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2310 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2312 */	0x36,		/* FC_POINTER */
			0x8,		/* FC_LONG */
/* 2314 */	0x40,		/* FC_STRUCTPAD4 */
			0x5b,		/* FC_END */
/* 2316 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 2318 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 2320 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 2322 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 2324 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 2326 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 2328 */	
			0x12, 0x0,	/* FC_UP */
/* 2330 */	NdrFcShort( 0x2 ),	/* Offset= 2 (2332) */
/* 2332 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 2334 */	NdrFcShort( 0x48 ),	/* 72 */
/* 2336 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2338 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2340 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2342 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2344 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2346 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2348 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2350 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2352 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2354 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 2356 */	
			0x12, 0x0,	/* FC_UP */
/* 2358 */	NdrFcShort( 0x2 ),	/* Offset= 2 (2360) */
/* 2360 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 2362 */	NdrFcShort( 0xb0 ),	/* 176 */
/* 2364 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2366 */	NdrFcShort( 0x2e ),	/* Offset= 46 (2412) */
/* 2368 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2370 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2372 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2374 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2376 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2378 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2380 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2382 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2384 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2386 */	0x36,		/* FC_POINTER */
			0x8,		/* FC_LONG */
/* 2388 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2390 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2392 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2394 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2396 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2398 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2400 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2402 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2404 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2406 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2408 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2410 */	0x40,		/* FC_STRUCTPAD4 */
			0x5b,		/* FC_END */
/* 2412 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 2414 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 2416 */	
			0x12, 0x0,	/* FC_UP */
/* 2418 */	NdrFcShort( 0x2 ),	/* Offset= 2 (2420) */
/* 2420 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 2422 */	NdrFcShort( 0xe8 ),	/* 232 */
/* 2424 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2426 */	NdrFcShort( 0x3c ),	/* Offset= 60 (2486) */
/* 2428 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2430 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2432 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2434 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2436 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2438 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2440 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2442 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2444 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2446 */	0x36,		/* FC_POINTER */
			0x8,		/* FC_LONG */
/* 2448 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2450 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2452 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2454 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2456 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2458 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2460 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2462 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2464 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2466 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2468 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2470 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2472 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2474 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2476 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2478 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2480 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2482 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2484 */	0x40,		/* FC_STRUCTPAD4 */
			0x5b,		/* FC_END */
/* 2486 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 2488 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 2490 */	
			0x12, 0x0,	/* FC_UP */
/* 2492 */	NdrFcShort( 0x2 ),	/* Offset= 2 (2494) */
/* 2494 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 2496 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2498 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2500 */	NdrFcShort( 0x4 ),	/* Offset= 4 (2504) */
/* 2502 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 2504 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 2506 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 2508 */	
			0x11, 0x0,	/* FC_RP */
/* 2510 */	NdrFcShort( 0x2 ),	/* Offset= 2 (2512) */
/* 2512 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 2514 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 2516 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 2518 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 2520 */	0x0 , 
			0x0,		/* 0 */
/* 2522 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2526 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2530 */	NdrFcShort( 0xfd7a ),	/* Offset= -646 (1884) */
/* 2532 */	
			0x11, 0x0,	/* FC_RP */
/* 2534 */	NdrFcShort( 0x3e ),	/* Offset= 62 (2596) */
/* 2536 */	
			0x29,		/* FC_WSTRING */
			0x5c,		/* FC_PAD */
/* 2538 */	NdrFcShort( 0x3 ),	/* 3 */
/* 2540 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x1,		/* 1 */
/* 2542 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2544 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2546 */	NdrFcShort( 0x0 ),	/* Offset= 0 (2546) */
/* 2548 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 2550 */	NdrFcShort( 0xfff2 ),	/* Offset= -14 (2536) */
/* 2552 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 2554 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x1,		/* 1 */
/* 2556 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2558 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 2560 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2562 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 2564 */	0x0 , 
			0x0,		/* 0 */
/* 2566 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2570 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2574 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 2576 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2578 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 2580 */	0x0 , 
			0x0,		/* 0 */
/* 2582 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2586 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2590 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 2592 */	NdrFcShort( 0xffcc ),	/* Offset= -52 (2540) */
/* 2594 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 2596 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 2598 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2600 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2602 */	NdrFcShort( 0x6 ),	/* Offset= 6 (2608) */
/* 2604 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 2606 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 2608 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 2610 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (2554) */
/* 2612 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 2614 */	NdrFcShort( 0x2 ),	/* Offset= 2 (2616) */
/* 2616 */	
			0x12, 0x0,	/* FC_UP */
/* 2618 */	NdrFcShort( 0x2 ),	/* Offset= 2 (2620) */
/* 2620 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 2622 */	NdrFcShort( 0x44 ),	/* 68 */
/* 2624 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2626 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2628 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2630 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2632 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2634 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2636 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2638 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 2640 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 2642 */	
			0x11, 0x0,	/* FC_RP */
/* 2644 */	NdrFcShort( 0x18 ),	/* Offset= 24 (2668) */
/* 2646 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 2648 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2650 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 2652 */	NdrFcShort( 0x18 ),	/* 24 */
/* 2654 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 2656 */	0x0 , 
			0x0,		/* 0 */
/* 2658 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2662 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2666 */	0x2,		/* FC_CHAR */
			0x5b,		/* FC_END */
/* 2668 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 2670 */	NdrFcShort( 0x28 ),	/* 40 */
/* 2672 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2674 */	NdrFcShort( 0xa ),	/* Offset= 10 (2684) */
/* 2676 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 2678 */	0x36,		/* FC_POINTER */
			0x36,		/* FC_POINTER */
/* 2680 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 2682 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 2684 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 2686 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 2688 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 2690 */	NdrFcShort( 0xffd4 ),	/* Offset= -44 (2646) */
/* 2692 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 2694 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 2696 */	
			0x11, 0x0,	/* FC_RP */
/* 2698 */	NdrFcShort( 0x1a0 ),	/* Offset= 416 (3114) */
/* 2700 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 2702 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 2704 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 2706 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 2708 */	0x0 , 
			0x0,		/* 0 */
/* 2710 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2714 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2718 */	NdrFcShort( 0x2 ),	/* Offset= 2 (2720) */
/* 2720 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2722 */	NdrFcShort( 0x3004 ),	/* 12292 */
/* 2724 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2728 */	NdrFcShort( 0x16 ),	/* Offset= 22 (2750) */
/* 2730 */	NdrFcLong( 0x1 ),	/* 1 */
/* 2734 */	NdrFcShort( 0x4e ),	/* Offset= 78 (2812) */
/* 2736 */	NdrFcLong( 0x2 ),	/* 2 */
/* 2740 */	NdrFcShort( 0xa8 ),	/* Offset= 168 (2908) */
/* 2742 */	NdrFcLong( 0x3 ),	/* 3 */
/* 2746 */	NdrFcShort( 0x104 ),	/* Offset= 260 (3006) */
/* 2748 */	NdrFcShort( 0xffff ),	/* Offset= -1 (2747) */
/* 2750 */	
			0x12, 0x0,	/* FC_UP */
/* 2752 */	NdrFcShort( 0x2c ),	/* Offset= 44 (2796) */
/* 2754 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 2756 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2758 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 2760 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2762 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 2764 */	0x0 , 
			0x0,		/* 0 */
/* 2766 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2770 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2774 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 2778 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 2780 */	0x0 , 
			0x0,		/* 0 */
/* 2782 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2786 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2790 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 2792 */	NdrFcShort( 0xff84 ),	/* Offset= -124 (2668) */
/* 2794 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 2796 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 2798 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2800 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2802 */	NdrFcShort( 0x6 ),	/* Offset= 6 (2808) */
/* 2804 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 2806 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 2808 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 2810 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (2754) */
/* 2812 */	
			0x12, 0x0,	/* FC_UP */
/* 2814 */	NdrFcShort( 0x4e ),	/* Offset= 78 (2892) */
/* 2816 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 2818 */	NdrFcShort( 0x30 ),	/* 48 */
/* 2820 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2822 */	NdrFcShort( 0xc ),	/* Offset= 12 (2834) */
/* 2824 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 2826 */	0x36,		/* FC_POINTER */
			0x36,		/* FC_POINTER */
/* 2828 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 2830 */	0x36,		/* FC_POINTER */
			0x36,		/* FC_POINTER */
/* 2832 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 2834 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 2836 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 2838 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 2840 */	NdrFcShort( 0xff3e ),	/* Offset= -194 (2646) */
/* 2842 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 2844 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 2846 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 2848 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 2850 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 2852 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2854 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 2856 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2858 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 2860 */	0x0 , 
			0x0,		/* 0 */
/* 2862 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2866 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2870 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 2874 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 2876 */	0x0 , 
			0x0,		/* 0 */
/* 2878 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2882 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2886 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 2888 */	NdrFcShort( 0xffb8 ),	/* Offset= -72 (2816) */
/* 2890 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 2892 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 2894 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2896 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2898 */	NdrFcShort( 0x6 ),	/* Offset= 6 (2904) */
/* 2900 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 2902 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 2904 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 2906 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (2850) */
/* 2908 */	
			0x12, 0x0,	/* FC_UP */
/* 2910 */	NdrFcShort( 0x50 ),	/* Offset= 80 (2990) */
/* 2912 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 2914 */	NdrFcShort( 0x38 ),	/* 56 */
/* 2916 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2918 */	NdrFcShort( 0xe ),	/* Offset= 14 (2932) */
/* 2920 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 2922 */	0x36,		/* FC_POINTER */
			0x36,		/* FC_POINTER */
/* 2924 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 2926 */	0x36,		/* FC_POINTER */
			0x36,		/* FC_POINTER */
/* 2928 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 2930 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 2932 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 2934 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 2936 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 2938 */	NdrFcShort( 0xfedc ),	/* Offset= -292 (2646) */
/* 2940 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 2942 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 2944 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 2946 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 2948 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 2950 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2952 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 2954 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2956 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 2958 */	0x0 , 
			0x0,		/* 0 */
/* 2960 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2964 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2968 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 2972 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 2974 */	0x0 , 
			0x0,		/* 0 */
/* 2976 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2980 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2984 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 2986 */	NdrFcShort( 0xffb6 ),	/* Offset= -74 (2912) */
/* 2988 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 2990 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 2992 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2994 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2996 */	NdrFcShort( 0x6 ),	/* Offset= 6 (3002) */
/* 2998 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 3000 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 3002 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 3004 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (2948) */
/* 3006 */	
			0x12, 0x0,	/* FC_UP */
/* 3008 */	NdrFcShort( 0x5a ),	/* Offset= 90 (3098) */
/* 3010 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 3012 */	NdrFcShort( 0x100 ),	/* 256 */
/* 3014 */	0x2,		/* FC_CHAR */
			0x5b,		/* FC_END */
/* 3016 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 3018 */	NdrFcShort( 0x138 ),	/* 312 */
/* 3020 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3022 */	NdrFcShort( 0x12 ),	/* Offset= 18 (3040) */
/* 3024 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 3026 */	0x36,		/* FC_POINTER */
			0x36,		/* FC_POINTER */
/* 3028 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 3030 */	0x36,		/* FC_POINTER */
			0x36,		/* FC_POINTER */
/* 3032 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 3034 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 3036 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (3010) */
/* 3038 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 3040 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 3042 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 3044 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 3046 */	NdrFcShort( 0xfe70 ),	/* Offset= -400 (2646) */
/* 3048 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 3050 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 3052 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 3054 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 3056 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 3058 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3060 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 3062 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3064 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 3066 */	0x0 , 
			0x0,		/* 0 */
/* 3068 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3072 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3076 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 3080 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 3082 */	0x0 , 
			0x0,		/* 0 */
/* 3084 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3088 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3092 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 3094 */	NdrFcShort( 0xffb2 ),	/* Offset= -78 (3016) */
/* 3096 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 3098 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 3100 */	NdrFcShort( 0x10 ),	/* 16 */
/* 3102 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3104 */	NdrFcShort( 0x6 ),	/* Offset= 6 (3110) */
/* 3106 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 3108 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 3110 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 3112 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (3056) */
/* 3114 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 3116 */	NdrFcShort( 0x10 ),	/* 16 */
/* 3118 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3120 */	NdrFcShort( 0x0 ),	/* Offset= 0 (3120) */
/* 3122 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 3124 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 3126 */	NdrFcShort( 0xfe56 ),	/* Offset= -426 (2700) */
/* 3128 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 3130 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 3132 */	NdrFcShort( 0x2 ),	/* Offset= 2 (3134) */
/* 3134 */	
			0x12, 0x0,	/* FC_UP */
/* 3136 */	NdrFcShort( 0x2 ),	/* Offset= 2 (3138) */
/* 3138 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 3140 */	NdrFcShort( 0x30 ),	/* 48 */
/* 3142 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 3144 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 3146 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 3148 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 3150 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 3152 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 3154 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 3156 */	
			0x11, 0x0,	/* FC_RP */
/* 3158 */	NdrFcShort( 0x2 ),	/* Offset= 2 (3160) */
/* 3160 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 3162 */	NdrFcShort( 0x1 ),	/* 1 */
/* 3164 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 3166 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 3168 */	NdrFcShort( 0x10 ),	/* Corr flags:  */
/* 3170 */	0x1 , /* correlation range */
			0x0,		/* 0 */
/* 3172 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3176 */	NdrFcLong( 0xfa00 ),	/* 64000 */
/* 3180 */	0x2,		/* FC_CHAR */
			0x5b,		/* FC_END */
/* 3182 */	0xb7,		/* FC_RANGE */
			0x8,		/* 8 */
/* 3184 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3188 */	NdrFcLong( 0xfa00 ),	/* 64000 */
/* 3192 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/* 3194 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 3196 */	
			0x11, 0x0,	/* FC_RP */
/* 3198 */	NdrFcShort( 0x2 ),	/* Offset= 2 (3200) */
/* 3200 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 3202 */	NdrFcShort( 0x2 ),	/* 2 */
/* 3204 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 3206 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 3208 */	NdrFcShort( 0x10 ),	/* Corr flags:  */
/* 3210 */	0x1 , /* correlation range */
			0x0,		/* 0 */
/* 3212 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3216 */	NdrFcLong( 0xfa00 ),	/* 64000 */
/* 3220 */	0x5,		/* FC_WCHAR */
			0x5b,		/* FC_END */
/* 3222 */	0xb7,		/* FC_RANGE */
			0x8,		/* 8 */
/* 3224 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3228 */	NdrFcLong( 0xfa00 ),	/* 64000 */
/* 3232 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 3234 */	NdrFcShort( 0x2 ),	/* Offset= 2 (3236) */
/* 3236 */	0x30,		/* FC_BIND_CONTEXT */
			0xa0,		/* Ctxt flags:  via ptr, out, */
/* 3238 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 3240 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 3242 */	NdrFcShort( 0x2 ),	/* Offset= 2 (3244) */
/* 3244 */	0x30,		/* FC_BIND_CONTEXT */
			0xe1,		/* Ctxt flags:  via ptr, in, out, can't be null */
/* 3246 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 3248 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 3250 */	NdrFcShort( 0x2 ),	/* Offset= 2 (3252) */
/* 3252 */	
			0x12, 0x0,	/* FC_UP */
/* 3254 */	NdrFcShort( 0x2 ),	/* Offset= 2 (3256) */
/* 3256 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 3258 */	NdrFcShort( 0x10 ),	/* 16 */
/* 3260 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3262 */	NdrFcShort( 0x6 ),	/* Offset= 6 (3268) */
/* 3264 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 3266 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 3268 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 3270 */	NdrFcShort( 0xf7f8 ),	/* Offset= -2056 (1214) */
/* 3272 */	
			0x11, 0x0,	/* FC_RP */
/* 3274 */	NdrFcShort( 0xffee ),	/* Offset= -18 (3256) */
/* 3276 */	
			0x11, 0x0,	/* FC_RP */
/* 3278 */	NdrFcShort( 0x2 ),	/* Offset= 2 (3280) */
/* 3280 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 3282 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 3284 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 3286 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 3288 */	0x0 , 
			0x0,		/* 0 */
/* 3290 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3294 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3298 */	NdrFcShort( 0x2 ),	/* Offset= 2 (3300) */
/* 3300 */	NdrFcShort( 0x138 ),	/* 312 */
/* 3302 */	NdrFcShort( 0x3004 ),	/* 12292 */
/* 3304 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3308 */	NdrFcShort( 0xfd80 ),	/* Offset= -640 (2668) */
/* 3310 */	NdrFcLong( 0x1 ),	/* 1 */
/* 3314 */	NdrFcShort( 0xfe0e ),	/* Offset= -498 (2816) */
/* 3316 */	NdrFcLong( 0x2 ),	/* 2 */
/* 3320 */	NdrFcShort( 0xfe68 ),	/* Offset= -408 (2912) */
/* 3322 */	NdrFcLong( 0x3 ),	/* 3 */
/* 3326 */	NdrFcShort( 0xfeca ),	/* Offset= -310 (3016) */
/* 3328 */	NdrFcShort( 0xffff ),	/* Offset= -1 (3327) */
/* 3330 */	
			0x11, 0x0,	/* FC_RP */
/* 3332 */	NdrFcShort( 0x8 ),	/* Offset= 8 (3340) */
/* 3334 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 3336 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3338 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 3340 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 3342 */	NdrFcShort( 0x10 ),	/* 16 */
/* 3344 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 3346 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 3348 */	0x0,		/* 0 */
			NdrFcShort( 0xfff1 ),	/* Offset= -15 (3334) */
			0x5b,		/* FC_END */
/* 3352 */	
			0x11, 0x0,	/* FC_RP */
/* 3354 */	NdrFcShort( 0x3e ),	/* Offset= 62 (3416) */
/* 3356 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 3358 */	NdrFcShort( 0x18 ),	/* 24 */
/* 3360 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3362 */	NdrFcShort( 0x8 ),	/* Offset= 8 (3370) */
/* 3364 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 3366 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (3340) */
/* 3368 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 3370 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 3372 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 3374 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 3376 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3378 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 3380 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3382 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 3384 */	0x0 , 
			0x0,		/* 0 */
/* 3386 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3390 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3394 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 3398 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 3400 */	0x0 , 
			0x0,		/* 0 */
/* 3402 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3406 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3410 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 3412 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (3356) */
/* 3414 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 3416 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 3418 */	NdrFcShort( 0x10 ),	/* 16 */
/* 3420 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3422 */	NdrFcShort( 0x6 ),	/* Offset= 6 (3428) */
/* 3424 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 3426 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 3428 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 3430 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (3374) */
/* 3432 */	0xb7,		/* FC_RANGE */
			0x8,		/* 8 */
/* 3434 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3438 */	NdrFcLong( 0x20 ),	/* 32 */
/* 3442 */	
			0x11, 0x0,	/* FC_RP */
/* 3444 */	NdrFcShort( 0x2 ),	/* Offset= 2 (3446) */
/* 3446 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 3448 */	NdrFcShort( 0x2 ),	/* 2 */
/* 3450 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 3452 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 3454 */	NdrFcShort( 0x11 ),	/* Corr flags:  early, */
/* 3456 */	0x1 , /* correlation range */
			0x0,		/* 0 */
/* 3458 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3462 */	NdrFcLong( 0x20 ),	/* 32 */
/* 3466 */	0x5,		/* FC_WCHAR */
			0x5b,		/* FC_END */
/* 3468 */	
			0x12, 0x14,	/* FC_UP [alloced_on_stack] [pointer_deref] */
/* 3470 */	NdrFcShort( 0x2 ),	/* Offset= 2 (3472) */
/* 3472 */	
			0x12, 0x0,	/* FC_UP */
/* 3474 */	NdrFcShort( 0x3c ),	/* Offset= 60 (3534) */
/* 3476 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 3478 */	NdrFcShort( 0x10 ),	/* 16 */
/* 3480 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3482 */	NdrFcShort( 0x6 ),	/* Offset= 6 (3488) */
/* 3484 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 3486 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 3488 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 3490 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 3492 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 3494 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3496 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 3498 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 3500 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 3502 */	0x0 , 
			0x0,		/* 0 */
/* 3504 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3508 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3512 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 3516 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 3518 */	0x0 , 
			0x0,		/* 0 */
/* 3520 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3524 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3528 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 3530 */	NdrFcShort( 0xffca ),	/* Offset= -54 (3476) */
/* 3532 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 3534 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 3536 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3538 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (3492) */
/* 3540 */	NdrFcShort( 0x0 ),	/* Offset= 0 (3540) */
/* 3542 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 3544 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 3546 */	
			0x11, 0x0,	/* FC_RP */
/* 3548 */	NdrFcShort( 0x2 ),	/* Offset= 2 (3550) */
/* 3550 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 3552 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 3554 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 3556 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 3558 */	0x0 , 
			0x0,		/* 0 */
/* 3560 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3564 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3568 */	NdrFcShort( 0xfef4 ),	/* Offset= -268 (3300) */
/* 3570 */	
			0x11, 0x0,	/* FC_RP */
/* 3572 */	NdrFcShort( 0x2 ),	/* Offset= 2 (3574) */
/* 3574 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 3576 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 3578 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 3580 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 3582 */	0x0 , 
			0x0,		/* 0 */
/* 3584 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3588 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3592 */	NdrFcShort( 0x2 ),	/* Offset= 2 (3594) */
/* 3594 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3596 */	NdrFcShort( 0x3001 ),	/* 12289 */
/* 3598 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3602 */	NdrFcShort( 0x4 ),	/* Offset= 4 (3606) */
/* 3604 */	NdrFcShort( 0xffff ),	/* Offset= -1 (3603) */
/* 3606 */	
			0x12, 0x0,	/* FC_UP */
/* 3608 */	NdrFcShort( 0x2 ),	/* Offset= 2 (3610) */
/* 3610 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 3612 */	NdrFcShort( 0x18 ),	/* 24 */
/* 3614 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3616 */	NdrFcShort( 0x8 ),	/* Offset= 8 (3624) */
/* 3618 */	0x36,		/* FC_POINTER */
			0x36,		/* FC_POINTER */
/* 3620 */	0x2,		/* FC_CHAR */
			0x3f,		/* FC_STRUCTPAD3 */
/* 3622 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 3624 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 3626 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 3628 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 3630 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 3632 */	
			0x11, 0x0,	/* FC_RP */
/* 3634 */	NdrFcShort( 0x60 ),	/* Offset= 96 (3730) */
/* 3636 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 3638 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 3640 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 3642 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 3644 */	0x0 , 
			0x0,		/* 0 */
/* 3646 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3650 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3654 */	NdrFcShort( 0x2 ),	/* Offset= 2 (3656) */
/* 3656 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3658 */	NdrFcShort( 0x3001 ),	/* 12289 */
/* 3660 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3664 */	NdrFcShort( 0x4 ),	/* Offset= 4 (3668) */
/* 3666 */	NdrFcShort( 0xffff ),	/* Offset= -1 (3665) */
/* 3668 */	
			0x12, 0x0,	/* FC_UP */
/* 3670 */	NdrFcShort( 0x2c ),	/* Offset= 44 (3714) */
/* 3672 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 3674 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3676 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 3678 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3680 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 3682 */	0x0 , 
			0x0,		/* 0 */
/* 3684 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3688 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3692 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 3696 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 3698 */	0x0 , 
			0x0,		/* 0 */
/* 3700 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3704 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3708 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 3710 */	NdrFcShort( 0xff9c ),	/* Offset= -100 (3610) */
/* 3712 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 3714 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 3716 */	NdrFcShort( 0x10 ),	/* 16 */
/* 3718 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3720 */	NdrFcShort( 0x6 ),	/* Offset= 6 (3726) */
/* 3722 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 3724 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 3726 */	
			0x12, 0x20,	/* FC_UP [maybenull_sizeis] */
/* 3728 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (3672) */
/* 3730 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 3732 */	NdrFcShort( 0x10 ),	/* 16 */
/* 3734 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3736 */	NdrFcShort( 0x0 ),	/* Offset= 0 (3736) */
/* 3738 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 3740 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 3742 */	NdrFcShort( 0xff96 ),	/* Offset= -106 (3636) */
/* 3744 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 3746 */	
			0x11, 0x0,	/* FC_RP */
/* 3748 */	NdrFcShort( 0x2 ),	/* Offset= 2 (3750) */
/* 3750 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 3752 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 3754 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 3756 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 3758 */	0x0 , 
			0x0,		/* 0 */
/* 3760 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3764 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3768 */	NdrFcShort( 0xff52 ),	/* Offset= -174 (3594) */
/* 3770 */	
			0x11, 0x0,	/* FC_RP */
/* 3772 */	NdrFcShort( 0x2 ),	/* Offset= 2 (3774) */
/* 3774 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 3776 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 3778 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 3780 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 3782 */	0x0 , 
			0x0,		/* 0 */
/* 3784 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3788 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3792 */	NdrFcShort( 0xf4fe ),	/* Offset= -2818 (974) */

			0x0
        }
    };

static const NDR_RUNDOWN RundownRoutines[] = 
    {
    SHARE_DEL_HANDLE_rundown
    };


static const unsigned short srvsvc_FormatStringOffsetTable[] =
    {
    0,
    44,
    88,
    132,
    176,
    220,
    264,
    308,
    352,
    426,
    506,
    568,
    618,
    698,
    754,
    816,
    884,
    946,
    1014,
    1070,
    1126,
    1182,
    1238,
    1300,
    1374,
    1442,
    1498,
    1566,
    1622,
    1672,
    1734,
    1814,
    1882,
    1944,
    2018,
    2086,
    2154,
    2216,
    2260,
    2328,
    2396,
    2452,
    2502,
    2582,
    2638,
    2700,
    2774,
    2836,
    2892,
    2984,
    3034,
    3090,
    3146,
    3214,
    3270
    };


static const RPC_DISPATCH_FUNCTION srvsvc_table[] =
    {
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    0
    };
static const RPC_DISPATCH_TABLE srvsvc_v3_0_DispatchTable = 
    {
    55,
    (RPC_DISPATCH_FUNCTION*)srvsvc_table
    };


#endif /* defined(_M_AMD64)*/



/* this ALWAYS GENERATED file contains the RPC server stubs */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Mon Jan 18 19:14:07 2038
 */
/* Compiler settings for srvsvc.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0622 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if defined(_M_AMD64)



extern const NDR_RUNDOWN RundownRoutines[];

#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif


#include "ndr64types.h"
#include "pshpack8.h"


typedef 
NDR64_FORMAT_CHAR
__midl_frag949_t;
extern const __midl_frag949_t __midl_frag949;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag948_t;
extern const __midl_frag948_t __midl_frag948;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag947_t;
extern const __midl_frag947_t __midl_frag947;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag946_t;
extern const __midl_frag946_t __midl_frag946;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag945_t;
extern const __midl_frag945_t __midl_frag945;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag943_t;
extern const __midl_frag943_t __midl_frag943;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag942_t;
extern const __midl_frag942_t __midl_frag942;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag941_t;
extern const __midl_frag941_t __midl_frag941;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag940_t;
extern const __midl_frag940_t __midl_frag940;

typedef 
struct 
{
    NDR64_FORMAT_UINT32 frag1;
    struct _NDR64_EXPR_VAR frag2;
}
__midl_frag938_t;
extern const __midl_frag938_t __midl_frag938;

typedef 
struct 
{
    struct _NDR64_NON_ENCAPSULATED_UNION frag1;
    struct _NDR64_UNION_ARM_SELECTOR frag2;
    struct _NDR64_UNION_ARM frag3;
    struct _NDR64_UNION_ARM frag4;
    struct _NDR64_UNION_ARM frag5;
    struct _NDR64_UNION_ARM frag6;
    struct _NDR64_UNION_ARM frag7;
    struct _NDR64_UNION_ARM frag8;
    struct _NDR64_UNION_ARM frag9;
    struct _NDR64_UNION_ARM frag10;
    struct _NDR64_UNION_ARM frag11;
    struct _NDR64_UNION_ARM frag12;
    NDR64_UINT32 frag13;
}
__midl_frag937_t;
extern const __midl_frag937_t __midl_frag937;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag936_t;
extern const __midl_frag936_t __midl_frag936;

typedef 
struct _NDR64_CONFORMANT_STRING_FORMAT
__midl_frag934_t;
extern const __midl_frag934_t __midl_frag934;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag933_t;
extern const __midl_frag933_t __midl_frag933;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
}
__midl_frag932_t;
extern const __midl_frag932_t __midl_frag932;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag930_t;
extern const __midl_frag930_t __midl_frag930;

typedef 
struct 
{
    struct _NDR64_NON_ENCAPSULATED_UNION frag1;
    struct _NDR64_UNION_ARM_SELECTOR frag2;
    struct _NDR64_UNION_ARM frag3;
    NDR64_UINT32 frag4;
}
__midl_frag928_t;
extern const __midl_frag928_t __midl_frag928;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag927_t;
extern const __midl_frag927_t __midl_frag927;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
}
__midl_frag923_t;
extern const __midl_frag923_t __midl_frag923;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag920_t;
extern const __midl_frag920_t __midl_frag920;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag918_t;
extern const __midl_frag918_t __midl_frag918;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
}
__midl_frag916_t;
extern const __midl_frag916_t __midl_frag916;

typedef 
struct 
{
    NDR64_FORMAT_UINT32 frag1;
    struct _NDR64_EXPR_VAR frag2;
}
__midl_frag915_t;
extern const __midl_frag915_t __midl_frag915;

typedef 
struct 
{
    struct _NDR64_CONF_VAR_BOGUS_ARRAY_HEADER_FORMAT frag1;
}
__midl_frag914_t;
extern const __midl_frag914_t __midl_frag914;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
    } frag2;
}
__midl_frag913_t;
extern const __midl_frag913_t __midl_frag913;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag912_t;
extern const __midl_frag912_t __midl_frag912;

typedef 
struct 
{
    struct _NDR64_NON_ENCAPSULATED_UNION frag1;
    struct _NDR64_UNION_ARM_SELECTOR frag2;
    struct _NDR64_UNION_ARM frag3;
    NDR64_UINT32 frag4;
}
__midl_frag910_t;
extern const __midl_frag910_t __midl_frag910;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_EMBEDDED_COMPLEX_FORMAT frag3;
        struct _NDR64_BUFFER_ALIGN_FORMAT frag4;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag5;
    } frag2;
}
__midl_frag909_t;
extern const __midl_frag909_t __midl_frag909;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag908_t;
extern const __midl_frag908_t __midl_frag908;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
    struct _NDR64_PARAM_FORMAT frag8;
}
__midl_frag905_t;
extern const __midl_frag905_t __midl_frag905;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
    struct _NDR64_POINTER_FORMAT frag2;
}
__midl_frag903_t;
extern const __midl_frag903_t __midl_frag903;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_MEMPAD_FORMAT frag4;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag5;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag6;
    } frag2;
}
__midl_frag900_t;
extern const __midl_frag900_t __midl_frag900;

typedef 
struct 
{
    struct _NDR64_NON_ENCAPSULATED_UNION frag1;
    struct _NDR64_UNION_ARM_SELECTOR frag2;
    struct _NDR64_UNION_ARM frag3;
    struct _NDR64_UNION_ARM frag4;
    struct _NDR64_UNION_ARM frag5;
    struct _NDR64_UNION_ARM frag6;
    NDR64_UINT32 frag7;
}
__midl_frag889_t;
extern const __midl_frag889_t __midl_frag889;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag888_t;
extern const __midl_frag888_t __midl_frag888;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
}
__midl_frag884_t;
extern const __midl_frag884_t __midl_frag884;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
}
__midl_frag882_t;
extern const __midl_frag882_t __midl_frag882;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
    } frag2;
}
__midl_frag880_t;
extern const __midl_frag880_t __midl_frag880;

typedef 
struct 
{
    struct _NDR64_CONF_VAR_BOGUS_ARRAY_HEADER_FORMAT frag1;
}
__midl_frag878_t;
extern const __midl_frag878_t __midl_frag878;

typedef 
struct 
{
    struct _NDR64_CONF_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
    } frag2;
}
__midl_frag877_t;
extern const __midl_frag877_t __midl_frag877;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag876_t;
extern const __midl_frag876_t __midl_frag876;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag875_t;
extern const __midl_frag875_t __midl_frag875;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
}
__midl_frag872_t;
extern const __midl_frag872_t __midl_frag872;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag869_t;
extern const __midl_frag869_t __midl_frag869;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag867_t;
extern const __midl_frag867_t __midl_frag867;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag866_t;
extern const __midl_frag866_t __midl_frag866;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
    struct _NDR64_PARAM_FORMAT frag8;
    struct _NDR64_PARAM_FORMAT frag9;
    struct _NDR64_PARAM_FORMAT frag10;
    struct _NDR64_PARAM_FORMAT frag11;
    struct _NDR64_PARAM_FORMAT frag12;
}
__midl_frag857_t;
extern const __midl_frag857_t __midl_frag857;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
}
__midl_frag850_t;
extern const __midl_frag850_t __midl_frag850;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
}
__midl_frag842_t;
extern const __midl_frag842_t __midl_frag842;

typedef 
NDR64_FORMAT_CHAR
__midl_frag840_t;
extern const __midl_frag840_t __midl_frag840;

typedef 
struct 
{
    NDR64_FORMAT_UINT32 frag1;
    struct _NDR64_RANGE_FORMAT frag2;
    struct _NDR64_EXPR_VAR frag3;
}
__midl_frag839_t;
extern const __midl_frag839_t __midl_frag839;

typedef 
struct 
{
    struct _NDR64_CONF_ARRAY_HEADER_FORMAT frag1;
    struct _NDR64_ARRAY_ELEMENT_INFO frag2;
}
__midl_frag838_t;
extern const __midl_frag838_t __midl_frag838;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag837_t;
extern const __midl_frag837_t __midl_frag837;

typedef 
struct _NDR64_RANGE_FORMAT
__midl_frag836_t;
extern const __midl_frag836_t __midl_frag836;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
    struct _NDR64_PARAM_FORMAT frag8;
    struct _NDR64_PARAM_FORMAT frag9;
}
__midl_frag829_t;
extern const __midl_frag829_t __midl_frag829;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
}
__midl_frag811_t;
extern const __midl_frag811_t __midl_frag811;

typedef 
struct 
{
    struct _NDR64_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_NO_REPEAT_FORMAT frag1;
        struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT frag2;
        struct _NDR64_POINTER_FORMAT frag3;
        NDR64_FORMAT_CHAR frag4;
    } frag2;
}
__midl_frag809_t;
extern const __midl_frag809_t __midl_frag809;

typedef 
struct 
{
    struct _NDR64_CONF_ARRAY_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_REPEAT_FORMAT frag1;
        struct 
        {
            struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT frag1;
            struct _NDR64_POINTER_FORMAT frag2;
        } frag2;
        NDR64_FORMAT_CHAR frag3;
    } frag2;
    struct _NDR64_ARRAY_ELEMENT_INFO frag3;
}
__midl_frag806_t;
extern const __midl_frag806_t __midl_frag806;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
    } frag2;
}
__midl_frag805_t;
extern const __midl_frag805_t __midl_frag805;

typedef 
struct 
{
    struct _NDR64_STRUCTURE_HEADER_FORMAT frag1;
}
__midl_frag799_t;
extern const __midl_frag799_t __midl_frag799;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
    struct _NDR64_PARAM_FORMAT frag8;
    struct _NDR64_PARAM_FORMAT frag9;
    struct _NDR64_PARAM_FORMAT frag10;
}
__midl_frag793_t;
extern const __midl_frag793_t __midl_frag793;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
}
__midl_frag787_t;
extern const __midl_frag787_t __midl_frag787;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag777_t;
extern const __midl_frag777_t __midl_frag777;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
    struct _NDR64_PARAM_FORMAT frag8;
}
__midl_frag769_t;
extern const __midl_frag769_t __midl_frag769;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
}
__midl_frag767_t;
extern const __midl_frag767_t __midl_frag767;

typedef 
NDR64_FORMAT_CHAR
__midl_frag766_t;
extern const __midl_frag766_t __midl_frag766;

typedef 
struct 
{
    struct _NDR64_CONF_ARRAY_HEADER_FORMAT frag1;
    struct _NDR64_ARRAY_ELEMENT_INFO frag2;
}
__midl_frag764_t;
extern const __midl_frag764_t __midl_frag764;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
    } frag2;
}
__midl_frag763_t;
extern const __midl_frag763_t __midl_frag763;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag761_t;
extern const __midl_frag761_t __midl_frag761;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
    struct _NDR64_PARAM_FORMAT frag8;
}
__midl_frag753_t;
extern const __midl_frag753_t __midl_frag753;

typedef 
struct _NDR64_CONTEXT_HANDLE_FORMAT
__midl_frag751_t;
extern const __midl_frag751_t __midl_frag751;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag750_t;
extern const __midl_frag750_t __midl_frag750;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
}
__midl_frag749_t;
extern const __midl_frag749_t __midl_frag749;

typedef 
struct _NDR64_CONTEXT_HANDLE_FORMAT
__midl_frag747_t;
extern const __midl_frag747_t __midl_frag747;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag746_t;
extern const __midl_frag746_t __midl_frag746;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
}
__midl_frag740_t;
extern const __midl_frag740_t __midl_frag740;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag733_t;
extern const __midl_frag733_t __midl_frag733;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
    struct _NDR64_PARAM_FORMAT frag8;
}
__midl_frag730_t;
extern const __midl_frag730_t __midl_frag730;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
    struct _NDR64_PARAM_FORMAT frag8;
}
__midl_frag720_t;
extern const __midl_frag720_t __midl_frag720;

typedef 
struct _NDR64_RANGE_FORMAT
__midl_frag716_t;
extern const __midl_frag716_t __midl_frag716;

typedef 
struct 
{
    NDR64_FORMAT_UINT32 frag1;
    struct _NDR64_RANGE_FORMAT frag2;
    struct _NDR64_EXPR_VAR frag3;
}
__midl_frag714_t;
extern const __midl_frag714_t __midl_frag714;

typedef 
struct 
{
    struct _NDR64_CONF_ARRAY_HEADER_FORMAT frag1;
    struct _NDR64_ARRAY_ELEMENT_INFO frag2;
}
__midl_frag713_t;
extern const __midl_frag713_t __midl_frag713;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag712_t;
extern const __midl_frag712_t __midl_frag712;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
    struct _NDR64_PARAM_FORMAT frag8;
    struct _NDR64_PARAM_FORMAT frag9;
}
__midl_frag707_t;
extern const __midl_frag707_t __midl_frag707;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
}
__midl_frag699_t;
extern const __midl_frag699_t __midl_frag699;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag685_t;
extern const __midl_frag685_t __midl_frag685;

typedef 
struct 
{
    struct _NDR64_CONF_ARRAY_HEADER_FORMAT frag1;
    struct _NDR64_ARRAY_ELEMENT_INFO frag2;
}
__midl_frag679_t;
extern const __midl_frag679_t __midl_frag679;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag678_t;
extern const __midl_frag678_t __midl_frag678;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
    struct _NDR64_PARAM_FORMAT frag8;
    struct _NDR64_PARAM_FORMAT frag9;
    struct _NDR64_PARAM_FORMAT frag10;
}
__midl_frag673_t;
extern const __midl_frag673_t __midl_frag673;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
}
__midl_frag664_t;
extern const __midl_frag664_t __midl_frag664;

typedef 
struct 
{
    struct _NDR64_STRUCTURE_HEADER_FORMAT frag1;
}
__midl_frag662_t;
extern const __midl_frag662_t __midl_frag662;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag661_t;
extern const __midl_frag661_t __midl_frag661;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag660_t;
extern const __midl_frag660_t __midl_frag660;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
}
__midl_frag657_t;
extern const __midl_frag657_t __midl_frag657;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag655_t;
extern const __midl_frag655_t __midl_frag655;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
}
__midl_frag651_t;
extern const __midl_frag651_t __midl_frag651;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
}
__midl_frag644_t;
extern const __midl_frag644_t __midl_frag644;

typedef 
struct 
{
    struct _NDR64_FIX_ARRAY_HEADER_FORMAT frag1;
}
__midl_frag643_t;
extern const __midl_frag643_t __midl_frag643;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
    struct _NDR64_POINTER_FORMAT frag2;
    struct _NDR64_POINTER_FORMAT frag3;
    struct _NDR64_POINTER_FORMAT frag4;
}
__midl_frag642_t;
extern const __midl_frag642_t __midl_frag642;

typedef 
struct 
{
    NDR64_FORMAT_UINT32 frag1;
    struct _NDR64_EXPR_VAR frag2;
}
__midl_frag638_t;
extern const __midl_frag638_t __midl_frag638;

typedef 
struct 
{
    struct _NDR64_CONF_ARRAY_HEADER_FORMAT frag1;
    struct _NDR64_ARRAY_ELEMENT_INFO frag2;
}
__midl_frag637_t;
extern const __midl_frag637_t __midl_frag637;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag5;
        struct _NDR64_MEMPAD_FORMAT frag6;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag7;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag8;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag9;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag10;
        struct _NDR64_EMBEDDED_COMPLEX_FORMAT frag11;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag12;
    } frag2;
}
__midl_frag635_t;
extern const __midl_frag635_t __midl_frag635;

typedef 
struct 
{
    struct _NDR64_CONF_VAR_BOGUS_ARRAY_HEADER_FORMAT frag1;
}
__midl_frag633_t;
extern const __midl_frag633_t __midl_frag633;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
    } frag2;
}
__midl_frag632_t;
extern const __midl_frag632_t __midl_frag632;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag631_t;
extern const __midl_frag631_t __midl_frag631;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
}
__midl_frag630_t;
extern const __midl_frag630_t __midl_frag630;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag5;
        struct _NDR64_MEMPAD_FORMAT frag6;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag7;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag8;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag9;
        struct _NDR64_MEMPAD_FORMAT frag10;
        struct _NDR64_BUFFER_ALIGN_FORMAT frag11;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag12;
    } frag2;
}
__midl_frag622_t;
extern const __midl_frag622_t __midl_frag622;

typedef 
struct 
{
    struct _NDR64_CONF_VAR_BOGUS_ARRAY_HEADER_FORMAT frag1;
}
__midl_frag620_t;
extern const __midl_frag620_t __midl_frag620;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
    } frag2;
}
__midl_frag619_t;
extern const __midl_frag619_t __midl_frag619;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag618_t;
extern const __midl_frag618_t __midl_frag618;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
}
__midl_frag617_t;
extern const __midl_frag617_t __midl_frag617;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag5;
        struct _NDR64_MEMPAD_FORMAT frag6;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag7;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag8;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag9;
    } frag2;
}
__midl_frag609_t;
extern const __midl_frag609_t __midl_frag609;

typedef 
struct 
{
    struct _NDR64_CONF_VAR_BOGUS_ARRAY_HEADER_FORMAT frag1;
}
__midl_frag607_t;
extern const __midl_frag607_t __midl_frag607;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
    } frag2;
}
__midl_frag606_t;
extern const __midl_frag606_t __midl_frag606;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag605_t;
extern const __midl_frag605_t __midl_frag605;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
}
__midl_frag604_t;
extern const __midl_frag604_t __midl_frag604;

typedef 
struct 
{
    struct _NDR64_CONF_VAR_BOGUS_ARRAY_HEADER_FORMAT frag1;
}
__midl_frag602_t;
extern const __midl_frag602_t __midl_frag602;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
    } frag2;
}
__midl_frag601_t;
extern const __midl_frag601_t __midl_frag601;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag600_t;
extern const __midl_frag600_t __midl_frag600;

typedef 
struct 
{
    struct _NDR64_NON_ENCAPSULATED_UNION frag1;
    struct _NDR64_UNION_ARM_SELECTOR frag2;
    struct _NDR64_UNION_ARM frag3;
    struct _NDR64_UNION_ARM frag4;
    struct _NDR64_UNION_ARM frag5;
    struct _NDR64_UNION_ARM frag6;
    NDR64_UINT32 frag7;
}
__midl_frag598_t;
extern const __midl_frag598_t __midl_frag598;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_EMBEDDED_COMPLEX_FORMAT frag3;
        struct _NDR64_BUFFER_ALIGN_FORMAT frag4;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag5;
    } frag2;
}
__midl_frag597_t;
extern const __midl_frag597_t __midl_frag597;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag596_t;
extern const __midl_frag596_t __midl_frag596;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
    struct _NDR64_PARAM_FORMAT frag8;
}
__midl_frag593_t;
extern const __midl_frag593_t __midl_frag593;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
    struct _NDR64_POINTER_FORMAT frag2;
    struct _NDR64_POINTER_FORMAT frag3;
}
__midl_frag591_t;
extern const __midl_frag591_t __midl_frag591;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag5;
        struct _NDR64_MEMPAD_FORMAT frag6;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag7;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag8;
    } frag2;
}
__midl_frag585_t;
extern const __midl_frag585_t __midl_frag585;

typedef 
struct 
{
    struct _NDR64_STRUCTURE_HEADER_FORMAT frag1;
}
__midl_frag578_t;
extern const __midl_frag578_t __midl_frag578;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag577_t;
extern const __midl_frag577_t __midl_frag577;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag576_t;
extern const __midl_frag576_t __midl_frag576;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
    struct _NDR64_PARAM_FORMAT frag8;
}
__midl_frag569_t;
extern const __midl_frag569_t __midl_frag569;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
}
__midl_frag562_t;
extern const __midl_frag562_t __midl_frag562;

typedef 
struct _NDR64_NON_CONFORMANT_STRING_FORMAT
__midl_frag561_t;
extern const __midl_frag561_t __midl_frag561;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_EMBEDDED_COMPLEX_FORMAT frag1;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag2;
    } frag2;
}
__midl_frag560_t;
extern const __midl_frag560_t __midl_frag560;

typedef 
struct 
{
    NDR64_FORMAT_UINT32 frag1;
    struct _NDR64_EXPR_NOOP frag2;
    struct _NDR64_EXPR_CONST64 frag3;
}
__midl_frag559_t;
extern const __midl_frag559_t __midl_frag559;

typedef 
struct 
{
    struct _NDR64_CONF_VAR_BOGUS_ARRAY_HEADER_FORMAT frag1;
}
__midl_frag556_t;
extern const __midl_frag556_t __midl_frag556;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
    } frag2;
}
__midl_frag555_t;
extern const __midl_frag555_t __midl_frag555;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag554_t;
extern const __midl_frag554_t __midl_frag554;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
    struct _NDR64_PARAM_FORMAT frag8;
    struct _NDR64_PARAM_FORMAT frag9;
}
__midl_frag550_t;
extern const __midl_frag550_t __midl_frag550;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag502_t;
extern const __midl_frag502_t __midl_frag502;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag501_t;
extern const __midl_frag501_t __midl_frag501;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag500_t;
extern const __midl_frag500_t __midl_frag500;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag499_t;
extern const __midl_frag499_t __midl_frag499;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag498_t;
extern const __midl_frag498_t __midl_frag498;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag497_t;
extern const __midl_frag497_t __midl_frag497;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag496_t;
extern const __midl_frag496_t __midl_frag496;

typedef 
struct 
{
    struct _NDR64_NON_ENCAPSULATED_UNION frag1;
    struct _NDR64_UNION_ARM_SELECTOR frag2;
    struct _NDR64_UNION_ARM frag3;
    struct _NDR64_UNION_ARM frag4;
    struct _NDR64_UNION_ARM frag5;
    struct _NDR64_UNION_ARM frag6;
    struct _NDR64_UNION_ARM frag7;
    struct _NDR64_UNION_ARM frag8;
    struct _NDR64_UNION_ARM frag9;
    struct _NDR64_UNION_ARM frag10;
    struct _NDR64_UNION_ARM frag11;
    struct _NDR64_UNION_ARM frag12;
    struct _NDR64_UNION_ARM frag13;
    struct _NDR64_UNION_ARM frag14;
    struct _NDR64_UNION_ARM frag15;
    struct _NDR64_UNION_ARM frag16;
    struct _NDR64_UNION_ARM frag17;
    struct _NDR64_UNION_ARM frag18;
    struct _NDR64_UNION_ARM frag19;
    struct _NDR64_UNION_ARM frag20;
    struct _NDR64_UNION_ARM frag21;
    struct _NDR64_UNION_ARM frag22;
    struct _NDR64_UNION_ARM frag23;
    struct _NDR64_UNION_ARM frag24;
    struct _NDR64_UNION_ARM frag25;
    struct _NDR64_UNION_ARM frag26;
    struct _NDR64_UNION_ARM frag27;
    struct _NDR64_UNION_ARM frag28;
    struct _NDR64_UNION_ARM frag29;
    struct _NDR64_UNION_ARM frag30;
    struct _NDR64_UNION_ARM frag31;
    struct _NDR64_UNION_ARM frag32;
    struct _NDR64_UNION_ARM frag33;
    struct _NDR64_UNION_ARM frag34;
    struct _NDR64_UNION_ARM frag35;
    struct _NDR64_UNION_ARM frag36;
    struct _NDR64_UNION_ARM frag37;
    struct _NDR64_UNION_ARM frag38;
    struct _NDR64_UNION_ARM frag39;
    struct _NDR64_UNION_ARM frag40;
    struct _NDR64_UNION_ARM frag41;
    struct _NDR64_UNION_ARM frag42;
    struct _NDR64_UNION_ARM frag43;
    struct _NDR64_UNION_ARM frag44;
    struct _NDR64_UNION_ARM frag45;
    struct _NDR64_UNION_ARM frag46;
    struct _NDR64_UNION_ARM frag47;
    struct _NDR64_UNION_ARM frag48;
    struct _NDR64_UNION_ARM frag49;
    struct _NDR64_UNION_ARM frag50;
    struct _NDR64_UNION_ARM frag51;
    struct _NDR64_UNION_ARM frag52;
    struct _NDR64_UNION_ARM frag53;
    NDR64_UINT32 frag54;
}
__midl_frag494_t;
extern const __midl_frag494_t __midl_frag494;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag493_t;
extern const __midl_frag493_t __midl_frag493;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
}
__midl_frag489_t;
extern const __midl_frag489_t __midl_frag489;

typedef 
struct 
{
    struct _NDR64_STRUCTURE_HEADER_FORMAT frag1;
}
__midl_frag487_t;
extern const __midl_frag487_t __midl_frag487;

typedef 
struct 
{
    struct _NDR64_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_NO_REPEAT_FORMAT frag1;
        struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT frag2;
        struct _NDR64_POINTER_FORMAT frag3;
        NDR64_FORMAT_CHAR frag4;
    } frag2;
}
__midl_frag400_t;
extern const __midl_frag400_t __midl_frag400;

typedef 
struct 
{
    struct _NDR64_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_NO_REPEAT_FORMAT frag1;
        struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT frag2;
        struct _NDR64_POINTER_FORMAT frag3;
        NDR64_FORMAT_CHAR frag4;
    } frag2;
}
__midl_frag397_t;
extern const __midl_frag397_t __midl_frag397;

typedef 
struct 
{
    struct _NDR64_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_NO_REPEAT_FORMAT frag1;
        struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT frag2;
        struct _NDR64_POINTER_FORMAT frag3;
        NDR64_FORMAT_CHAR frag4;
    } frag2;
}
__midl_frag394_t;
extern const __midl_frag394_t __midl_frag394;

typedef 
struct 
{
    struct _NDR64_STRUCTURE_HEADER_FORMAT frag1;
}
__midl_frag392_t;
extern const __midl_frag392_t __midl_frag392;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
    struct _NDR64_POINTER_FORMAT frag2;
    struct _NDR64_POINTER_FORMAT frag3;
}
__midl_frag390_t;
extern const __midl_frag390_t __midl_frag390;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag5;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag6;
        struct _NDR64_MEMPAD_FORMAT frag7;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag8;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag9;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag10;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag11;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag12;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag13;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag14;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag15;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag16;
        struct _NDR64_MEMPAD_FORMAT frag17;
        struct _NDR64_BUFFER_ALIGN_FORMAT frag18;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag19;
    } frag2;
}
__midl_frag386_t;
extern const __midl_frag386_t __midl_frag386;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag5;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag6;
        struct _NDR64_MEMPAD_FORMAT frag7;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag8;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag9;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag10;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag11;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag12;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag13;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag14;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag15;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag16;
    } frag2;
}
__midl_frag380_t;
extern const __midl_frag380_t __midl_frag380;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag5;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag6;
        struct _NDR64_MEMPAD_FORMAT frag7;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag8;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag9;
    } frag2;
}
__midl_frag375_t;
extern const __midl_frag375_t __midl_frag375;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag367_t;
extern const __midl_frag367_t __midl_frag367;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
}
__midl_frag363_t;
extern const __midl_frag363_t __midl_frag363;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
}
__midl_frag355_t;
extern const __midl_frag355_t __midl_frag355;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
}
__midl_frag348_t;
extern const __midl_frag348_t __midl_frag348;

typedef 
struct 
{
    NDR64_FORMAT_UINT32 frag1;
    struct _NDR64_EXPR_VAR frag2;
}
__midl_frag327_t;
extern const __midl_frag327_t __midl_frag327;

typedef 
struct 
{
    struct _NDR64_NON_ENCAPSULATED_UNION frag1;
    struct _NDR64_UNION_ARM_SELECTOR frag2;
    struct _NDR64_UNION_ARM frag3;
    struct _NDR64_UNION_ARM frag4;
    struct _NDR64_UNION_ARM frag5;
    struct _NDR64_UNION_ARM frag6;
    struct _NDR64_UNION_ARM frag7;
    struct _NDR64_UNION_ARM frag8;
    struct _NDR64_UNION_ARM frag9;
    struct _NDR64_UNION_ARM frag10;
    struct _NDR64_UNION_ARM frag11;
    struct _NDR64_UNION_ARM frag12;
    NDR64_UINT32 frag13;
}
__midl_frag326_t;
extern const __midl_frag326_t __midl_frag326;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag325_t;
extern const __midl_frag325_t __midl_frag325;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
    struct _NDR64_PARAM_FORMAT frag8;
}
__midl_frag319_t;
extern const __midl_frag319_t __midl_frag319;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag305_t;
extern const __midl_frag305_t __midl_frag305;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
}
__midl_frag299_t;
extern const __midl_frag299_t __midl_frag299;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
}
__midl_frag292_t;
extern const __midl_frag292_t __midl_frag292;

typedef 
struct 
{
    struct _NDR64_CONF_VAR_BOGUS_ARRAY_HEADER_FORMAT frag1;
}
__midl_frag290_t;
extern const __midl_frag290_t __midl_frag290;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
    } frag2;
}
__midl_frag289_t;
extern const __midl_frag289_t __midl_frag289;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag288_t;
extern const __midl_frag288_t __midl_frag288;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
}
__midl_frag287_t;
extern const __midl_frag287_t __midl_frag287;

typedef 
struct 
{
    struct _NDR64_CONF_VAR_BOGUS_ARRAY_HEADER_FORMAT frag1;
}
__midl_frag285_t;
extern const __midl_frag285_t __midl_frag285;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
    } frag2;
}
__midl_frag284_t;
extern const __midl_frag284_t __midl_frag284;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag283_t;
extern const __midl_frag283_t __midl_frag283;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
}
__midl_frag282_t;
extern const __midl_frag282_t __midl_frag282;

typedef 
struct 
{
    struct _NDR64_CONF_VAR_BOGUS_ARRAY_HEADER_FORMAT frag1;
}
__midl_frag280_t;
extern const __midl_frag280_t __midl_frag280;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
    } frag2;
}
__midl_frag279_t;
extern const __midl_frag279_t __midl_frag279;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag278_t;
extern const __midl_frag278_t __midl_frag278;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
}
__midl_frag277_t;
extern const __midl_frag277_t __midl_frag277;

typedef 
struct 
{
    struct _NDR64_CONF_VAR_BOGUS_ARRAY_HEADER_FORMAT frag1;
}
__midl_frag275_t;
extern const __midl_frag275_t __midl_frag275;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
    } frag2;
}
__midl_frag274_t;
extern const __midl_frag274_t __midl_frag274;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag273_t;
extern const __midl_frag273_t __midl_frag273;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
}
__midl_frag272_t;
extern const __midl_frag272_t __midl_frag272;

typedef 
struct 
{
    struct _NDR64_CONF_VAR_BOGUS_ARRAY_HEADER_FORMAT frag1;
}
__midl_frag270_t;
extern const __midl_frag270_t __midl_frag270;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
    } frag2;
}
__midl_frag269_t;
extern const __midl_frag269_t __midl_frag269;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag268_t;
extern const __midl_frag268_t __midl_frag268;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
}
__midl_frag267_t;
extern const __midl_frag267_t __midl_frag267;

typedef 
struct 
{
    struct _NDR64_CONF_ARRAY_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_REPEAT_FORMAT frag1;
        struct 
        {
            struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT frag1;
            struct _NDR64_POINTER_FORMAT frag2;
        } frag2;
        NDR64_FORMAT_CHAR frag3;
    } frag2;
    struct _NDR64_ARRAY_ELEMENT_INFO frag3;
}
__midl_frag264_t;
extern const __midl_frag264_t __midl_frag264;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
    } frag2;
}
__midl_frag263_t;
extern const __midl_frag263_t __midl_frag263;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag262_t;
extern const __midl_frag262_t __midl_frag262;

typedef 
struct 
{
    struct _NDR64_NON_ENCAPSULATED_UNION frag1;
    struct _NDR64_UNION_ARM_SELECTOR frag2;
    struct _NDR64_UNION_ARM frag3;
    struct _NDR64_UNION_ARM frag4;
    struct _NDR64_UNION_ARM frag5;
    struct _NDR64_UNION_ARM frag6;
    struct _NDR64_UNION_ARM frag7;
    struct _NDR64_UNION_ARM frag8;
    NDR64_UINT32 frag9;
}
__midl_frag260_t;
extern const __midl_frag260_t __midl_frag260;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_EMBEDDED_COMPLEX_FORMAT frag3;
        struct _NDR64_BUFFER_ALIGN_FORMAT frag4;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag5;
    } frag2;
}
__midl_frag259_t;
extern const __midl_frag259_t __midl_frag259;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
    struct _NDR64_POINTER_FORMAT frag2;
    struct _NDR64_POINTER_FORMAT frag3;
    struct _NDR64_POINTER_FORMAT frag4;
    struct _NDR64_POINTER_FORMAT frag5;
    struct _NDR64_POINTER_FORMAT frag6;
}
__midl_frag251_t;
extern const __midl_frag251_t __midl_frag251;

typedef 
struct 
{
    NDR64_FORMAT_UINT32 frag1;
    struct _NDR64_EXPR_VAR frag2;
}
__midl_frag249_t;
extern const __midl_frag249_t __midl_frag249;

typedef 
struct 
{
    struct _NDR64_CONF_ARRAY_HEADER_FORMAT frag1;
    struct _NDR64_ARRAY_ELEMENT_INFO frag2;
}
__midl_frag248_t;
extern const __midl_frag248_t __midl_frag248;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag2;
        struct _NDR64_MEMPAD_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag5;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag6;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag7;
        struct _NDR64_MEMPAD_FORMAT frag8;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag9;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag10;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag11;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag12;
        struct _NDR64_MEMPAD_FORMAT frag13;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag14;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag15;
    } frag2;
}
__midl_frag242_t;
extern const __midl_frag242_t __midl_frag242;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag2;
        struct _NDR64_MEMPAD_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag5;
        struct _NDR64_MEMPAD_FORMAT frag6;
        struct _NDR64_BUFFER_ALIGN_FORMAT frag7;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag8;
    } frag2;
}
__midl_frag237_t;
extern const __midl_frag237_t __midl_frag237;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
    struct _NDR64_POINTER_FORMAT frag2;
    struct _NDR64_POINTER_FORMAT frag3;
    struct _NDR64_POINTER_FORMAT frag4;
    struct _NDR64_POINTER_FORMAT frag5;
}
__midl_frag222_t;
extern const __midl_frag222_t __midl_frag222;

typedef 
struct 
{
    NDR64_FORMAT_UINT32 frag1;
    struct _NDR64_EXPR_VAR frag2;
}
__midl_frag220_t;
extern const __midl_frag220_t __midl_frag220;

typedef 
struct 
{
    struct _NDR64_CONF_ARRAY_HEADER_FORMAT frag1;
    struct _NDR64_ARRAY_ELEMENT_INFO frag2;
}
__midl_frag219_t;
extern const __midl_frag219_t __midl_frag219;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag2;
        struct _NDR64_MEMPAD_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag5;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag6;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag7;
        struct _NDR64_MEMPAD_FORMAT frag8;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag9;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag10;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag11;
        struct _NDR64_MEMPAD_FORMAT frag12;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag13;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag14;
    } frag2;
}
__midl_frag214_t;
extern const __midl_frag214_t __midl_frag214;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
    struct _NDR64_POINTER_FORMAT frag2;
    struct _NDR64_POINTER_FORMAT frag3;
    struct _NDR64_POINTER_FORMAT frag4;
}
__midl_frag212_t;
extern const __midl_frag212_t __midl_frag212;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag2;
        struct _NDR64_MEMPAD_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag5;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag6;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag7;
        struct _NDR64_MEMPAD_FORMAT frag8;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag9;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag10;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag11;
    } frag2;
}
__midl_frag207_t;
extern const __midl_frag207_t __midl_frag207;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag2;
        struct _NDR64_MEMPAD_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag5;
    } frag2;
}
__midl_frag202_t;
extern const __midl_frag202_t __midl_frag202;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
}
__midl_frag191_t;
extern const __midl_frag191_t __midl_frag191;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
}
__midl_frag183_t;
extern const __midl_frag183_t __midl_frag183;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
}
__midl_frag176_t;
extern const __midl_frag176_t __midl_frag176;

typedef 
struct 
{
    struct _NDR64_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_NO_REPEAT_FORMAT frag1;
        struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT frag2;
        struct _NDR64_POINTER_FORMAT frag3;
        struct _NDR64_NO_REPEAT_FORMAT frag4;
        struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT frag5;
        struct _NDR64_POINTER_FORMAT frag6;
        struct _NDR64_NO_REPEAT_FORMAT frag7;
        struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT frag8;
        struct _NDR64_POINTER_FORMAT frag9;
        struct _NDR64_NO_REPEAT_FORMAT frag10;
        struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT frag11;
        struct _NDR64_POINTER_FORMAT frag12;
        NDR64_FORMAT_CHAR frag13;
    } frag2;
}
__midl_frag171_t;
extern const __midl_frag171_t __midl_frag171;

typedef 
struct 
{
    struct _NDR64_CONF_ARRAY_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_REPEAT_FORMAT frag1;
        struct 
        {
            struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT frag1;
            struct _NDR64_POINTER_FORMAT frag2;
            struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT frag3;
            struct _NDR64_POINTER_FORMAT frag4;
            struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT frag5;
            struct _NDR64_POINTER_FORMAT frag6;
            struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT frag7;
            struct _NDR64_POINTER_FORMAT frag8;
        } frag2;
        NDR64_FORMAT_CHAR frag3;
    } frag2;
    struct _NDR64_ARRAY_ELEMENT_INFO frag3;
}
__midl_frag165_t;
extern const __midl_frag165_t __midl_frag165;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
    } frag2;
}
__midl_frag164_t;
extern const __midl_frag164_t __midl_frag164;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag163_t;
extern const __midl_frag163_t __midl_frag163;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
}
__midl_frag162_t;
extern const __midl_frag162_t __midl_frag162;

typedef 
struct 
{
    struct _NDR64_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_NO_REPEAT_FORMAT frag1;
        struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT frag2;
        struct _NDR64_POINTER_FORMAT frag3;
        struct _NDR64_NO_REPEAT_FORMAT frag4;
        struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT frag5;
        struct _NDR64_POINTER_FORMAT frag6;
        NDR64_FORMAT_CHAR frag7;
    } frag2;
}
__midl_frag159_t;
extern const __midl_frag159_t __midl_frag159;

typedef 
struct 
{
    struct _NDR64_CONF_ARRAY_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_REPEAT_FORMAT frag1;
        struct 
        {
            struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT frag1;
            struct _NDR64_POINTER_FORMAT frag2;
            struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT frag3;
            struct _NDR64_POINTER_FORMAT frag4;
        } frag2;
        NDR64_FORMAT_CHAR frag3;
    } frag2;
    struct _NDR64_ARRAY_ELEMENT_INFO frag3;
}
__midl_frag155_t;
extern const __midl_frag155_t __midl_frag155;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
    } frag2;
}
__midl_frag154_t;
extern const __midl_frag154_t __midl_frag154;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag153_t;
extern const __midl_frag153_t __midl_frag153;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
}
__midl_frag152_t;
extern const __midl_frag152_t __midl_frag152;

typedef 
struct 
{
    struct _NDR64_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_NO_REPEAT_FORMAT frag1;
        struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT frag2;
        struct _NDR64_POINTER_FORMAT frag3;
        struct _NDR64_NO_REPEAT_FORMAT frag4;
        struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT frag5;
        struct _NDR64_POINTER_FORMAT frag6;
        struct _NDR64_NO_REPEAT_FORMAT frag7;
        struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT frag8;
        struct _NDR64_POINTER_FORMAT frag9;
        NDR64_FORMAT_CHAR frag10;
    } frag2;
}
__midl_frag148_t;
extern const __midl_frag148_t __midl_frag148;

typedef 
struct 
{
    struct _NDR64_CONF_ARRAY_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_REPEAT_FORMAT frag1;
        struct 
        {
            struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT frag1;
            struct _NDR64_POINTER_FORMAT frag2;
            struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT frag3;
            struct _NDR64_POINTER_FORMAT frag4;
            struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT frag5;
            struct _NDR64_POINTER_FORMAT frag6;
        } frag2;
        NDR64_FORMAT_CHAR frag3;
    } frag2;
    struct _NDR64_ARRAY_ELEMENT_INFO frag3;
}
__midl_frag143_t;
extern const __midl_frag143_t __midl_frag143;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
    } frag2;
}
__midl_frag142_t;
extern const __midl_frag142_t __midl_frag142;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag141_t;
extern const __midl_frag141_t __midl_frag141;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
}
__midl_frag140_t;
extern const __midl_frag140_t __midl_frag140;

typedef 
struct 
{
    struct _NDR64_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_NO_REPEAT_FORMAT frag1;
        struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT frag2;
        struct _NDR64_POINTER_FORMAT frag3;
        struct _NDR64_NO_REPEAT_FORMAT frag4;
        struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT frag5;
        struct _NDR64_POINTER_FORMAT frag6;
        NDR64_FORMAT_CHAR frag7;
    } frag2;
}
__midl_frag137_t;
extern const __midl_frag137_t __midl_frag137;

typedef 
struct 
{
    struct _NDR64_CONF_ARRAY_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_REPEAT_FORMAT frag1;
        struct 
        {
            struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT frag1;
            struct _NDR64_POINTER_FORMAT frag2;
            struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT frag3;
            struct _NDR64_POINTER_FORMAT frag4;
        } frag2;
        NDR64_FORMAT_CHAR frag3;
    } frag2;
    struct _NDR64_ARRAY_ELEMENT_INFO frag3;
}
__midl_frag133_t;
extern const __midl_frag133_t __midl_frag133;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
    } frag2;
}
__midl_frag132_t;
extern const __midl_frag132_t __midl_frag132;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag131_t;
extern const __midl_frag131_t __midl_frag131;

typedef 
struct 
{
    struct _NDR64_NON_ENCAPSULATED_UNION frag1;
    struct _NDR64_UNION_ARM_SELECTOR frag2;
    struct _NDR64_UNION_ARM frag3;
    struct _NDR64_UNION_ARM frag4;
    struct _NDR64_UNION_ARM frag5;
    struct _NDR64_UNION_ARM frag6;
    struct _NDR64_UNION_ARM frag7;
    NDR64_UINT32 frag8;
}
__midl_frag121_t;
extern const __midl_frag121_t __midl_frag121;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_EMBEDDED_COMPLEX_FORMAT frag3;
        struct _NDR64_BUFFER_ALIGN_FORMAT frag4;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag5;
    } frag2;
}
__midl_frag120_t;
extern const __midl_frag120_t __midl_frag120;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag119_t;
extern const __midl_frag119_t __midl_frag119;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
    struct _NDR64_PARAM_FORMAT frag8;
    struct _NDR64_PARAM_FORMAT frag9;
    struct _NDR64_PARAM_FORMAT frag10;
}
__midl_frag112_t;
extern const __midl_frag112_t __midl_frag112;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
}
__midl_frag107_t;
extern const __midl_frag107_t __midl_frag107;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag105_t;
extern const __midl_frag105_t __midl_frag105;

typedef 
struct 
{
    struct _NDR64_NON_ENCAPSULATED_UNION frag1;
    struct _NDR64_UNION_ARM_SELECTOR frag2;
    struct _NDR64_UNION_ARM frag3;
    struct _NDR64_UNION_ARM frag4;
    NDR64_UINT32 frag5;
}
__midl_frag102_t;
extern const __midl_frag102_t __midl_frag102;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag101_t;
extern const __midl_frag101_t __midl_frag101;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
}
__midl_frag96_t;
extern const __midl_frag96_t __midl_frag96;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
}
__midl_frag89_t;
extern const __midl_frag89_t __midl_frag89;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_MEMPAD_FORMAT frag4;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag5;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag6;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag7;
    } frag2;
}
__midl_frag85_t;
extern const __midl_frag85_t __midl_frag85;

typedef 
struct 
{
    struct _NDR64_CONF_VAR_BOGUS_ARRAY_HEADER_FORMAT frag1;
}
__midl_frag83_t;
extern const __midl_frag83_t __midl_frag83;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
    } frag2;
}
__midl_frag82_t;
extern const __midl_frag82_t __midl_frag82;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag81_t;
extern const __midl_frag81_t __midl_frag81;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
}
__midl_frag80_t;
extern const __midl_frag80_t __midl_frag80;

typedef 
struct 
{
    struct _NDR64_CONF_ARRAY_HEADER_FORMAT frag1;
    struct _NDR64_ARRAY_ELEMENT_INFO frag2;
}
__midl_frag77_t;
extern const __midl_frag77_t __midl_frag77;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
    } frag2;
}
__midl_frag76_t;
extern const __midl_frag76_t __midl_frag76;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag75_t;
extern const __midl_frag75_t __midl_frag75;

typedef 
struct 
{
    struct _NDR64_NON_ENCAPSULATED_UNION frag1;
    struct _NDR64_UNION_ARM_SELECTOR frag2;
    struct _NDR64_UNION_ARM frag3;
    struct _NDR64_UNION_ARM frag4;
    NDR64_UINT32 frag5;
}
__midl_frag73_t;
extern const __midl_frag73_t __midl_frag73;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_EMBEDDED_COMPLEX_FORMAT frag3;
        struct _NDR64_BUFFER_ALIGN_FORMAT frag4;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag5;
    } frag2;
}
__midl_frag72_t;
extern const __midl_frag72_t __midl_frag72;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag71_t;
extern const __midl_frag71_t __midl_frag71;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
    struct _NDR64_PARAM_FORMAT frag8;
    struct _NDR64_PARAM_FORMAT frag9;
    struct _NDR64_PARAM_FORMAT frag10;
}
__midl_frag64_t;
extern const __midl_frag64_t __midl_frag64;

typedef 
struct 
{
    struct _NDR64_POINTER_FORMAT frag1;
}
__midl_frag57_t;
extern const __midl_frag57_t __midl_frag57;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag5;
        struct _NDR64_MEMPAD_FORMAT frag6;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag7;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag8;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag9;
    } frag2;
}
__midl_frag53_t;
extern const __midl_frag53_t __midl_frag53;

typedef 
struct 
{
    struct _NDR64_CONF_VAR_BOGUS_ARRAY_HEADER_FORMAT frag1;
}
__midl_frag51_t;
extern const __midl_frag51_t __midl_frag51;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
    } frag2;
}
__midl_frag50_t;
extern const __midl_frag50_t __midl_frag50;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag49_t;
extern const __midl_frag49_t __midl_frag49;

typedef 
struct 
{
    struct _NDR64_NON_ENCAPSULATED_UNION frag1;
    struct _NDR64_UNION_ARM_SELECTOR frag2;
    struct _NDR64_UNION_ARM frag3;
    struct _NDR64_UNION_ARM frag4;
    NDR64_UINT32 frag5;
}
__midl_frag41_t;
extern const __midl_frag41_t __midl_frag41;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_MEMPAD_FORMAT frag2;
        struct _NDR64_EMBEDDED_COMPLEX_FORMAT frag3;
        struct _NDR64_BUFFER_ALIGN_FORMAT frag4;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag5;
    } frag2;
}
__midl_frag40_t;
extern const __midl_frag40_t __midl_frag40;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag39_t;
extern const __midl_frag39_t __midl_frag39;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
    struct _NDR64_PARAM_FORMAT frag8;
    struct _NDR64_PARAM_FORMAT frag9;
}
__midl_frag34_t;
extern const __midl_frag34_t __midl_frag34;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
}
__midl_frag30_t;
extern const __midl_frag30_t __midl_frag30;

typedef 
NDR64_FORMAT_UINT32
__midl_frag1_t;
extern const __midl_frag1_t __midl_frag1;

static const __midl_frag949_t __midl_frag949 =
0x5    /* FC64_INT32 */;

static const __midl_frag948_t __midl_frag948 =
{ 
/* *_SHARE_INFO_503_I */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag242
};

static const __midl_frag947_t __midl_frag947 =
{ 
/* *_SHARE_INFO_501 */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag237
};

static const __midl_frag946_t __midl_frag946 =
{ 
/* *_SHARE_INFO_1005 */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag487
};

static const __midl_frag945_t __midl_frag945 =
{ 
/* *_SHARE_INFO_1501_I */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag763
};

static const __midl_frag943_t __midl_frag943 =
{ 
/* *_SHARE_INFO_1004 */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag400
};

static const __midl_frag942_t __midl_frag942 =
{ 
/* *_SHARE_INFO_502_I */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag214
};

static const __midl_frag941_t __midl_frag941 =
{ 
/* *_SHARE_INFO_2 */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag207
};

static const __midl_frag940_t __midl_frag940 =
{ 
/* *_SHARE_INFO_1 */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag202
};

static const __midl_frag938_t __midl_frag938 =
{ 
/*  */
    (NDR64_UINT32) 1 /* 0x1 */,
    { 
    /* struct _NDR64_EXPR_VAR */
        0x3,    /* FC_EXPR_VAR */
        0x6,    /* FC64_UINT32 */
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT32) 8 /* 0x8 */  /* Offset */
    }
};

static const __midl_frag937_t __midl_frag937 =
{ 
/* _SHARE_INFO */
    { 
    /* _SHARE_INFO */
        0x51,    /* FC64_NON_ENCAPSULATED_UNION */
        (NDR64_UINT8) 7 /* 0x7 */,
        (NDR64_UINT8) 0 /* 0x0 */,
        0x6,    /* FC64_UINT32 */
        (NDR64_UINT32) 8 /* 0x8 */,
        &__midl_frag938,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM_SELECTOR */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT8) 7 /* 0x7 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT32) 10 /* 0xa */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 0 /* 0x0 */,
        &__midl_frag943,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1 /* 0x1 */,
        &__midl_frag940,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 2 /* 0x2 */,
        &__midl_frag941,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 502 /* 0x1f6 */,
        &__midl_frag942,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1004 /* 0x3ec */,
        &__midl_frag943,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1006 /* 0x3ee */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1501 /* 0x5dd */,
        &__midl_frag945,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1005 /* 0x3ed */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 501 /* 0x1f5 */,
        &__midl_frag947,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 503 /* 0x1f7 */,
        &__midl_frag948,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    (NDR64_UINT32) 0 /* 0x0 */
};

static const __midl_frag936_t __midl_frag936 =
{ 
/* *_SHARE_INFO */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag937
};

static const __midl_frag934_t __midl_frag934 =
{ 
/* *wchar_t */
    { 
    /* *wchar_t */
        0x64,    /* FC64_CONF_WCHAR_STRING */
        { 
        /* *wchar_t */
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT16) 2 /* 0x2 */
    }
};

static const __midl_frag933_t __midl_frag933 =
{ 
/* *wchar_t */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag934
};

static const __midl_frag932_t __midl_frag932 =
{ 
/* NetrShareDelEx */
    { 
    /* NetrShareDelEx */      /* procedure NetrShareDelEx */
        (NDR64_UINT32) 19660864 /* 0x12c0040 */,    /* explicit handle */ /* IsIntrepreted, ClientMustSize, HasReturn, ServerCorrelation, HasExtensions */
        (NDR64_UINT32) 32 /* 0x20 */ ,  /* Stack size */
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 4 /* 0x4 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* Level */      /* parameter Level */
        &__midl_frag949,
        { 
        /* Level */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* ShareInfo */      /* parameter ShareInfo */
        &__midl_frag937,
        { 
        /* ShareInfo */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    }
};

static const __midl_frag930_t __midl_frag930 =
{ 
/* *_SERVER_ALIAS_INFO_0 */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag900
};

static const __midl_frag928_t __midl_frag928 =
{ 
/* _SERVER_ALIAS_INFO */
    { 
    /* _SERVER_ALIAS_INFO */
        0x51,    /* FC64_NON_ENCAPSULATED_UNION */
        (NDR64_UINT8) 7 /* 0x7 */,
        (NDR64_UINT8) 0 /* 0x0 */,
        0x6,    /* FC64_UINT32 */
        (NDR64_UINT32) 8 /* 0x8 */,
        &__midl_frag938,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM_SELECTOR */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT8) 7 /* 0x7 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT32) 1 /* 0x1 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 0 /* 0x0 */,
        &__midl_frag930,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    (NDR64_UINT32) 4294967295 /* 0xffffffff */
};

static const __midl_frag927_t __midl_frag927 =
{ 
/* *_SERVER_ALIAS_INFO */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag928
};

static const __midl_frag923_t __midl_frag923 =
{ 
/* NetrServerAliasDel */
    { 
    /* NetrServerAliasDel */      /* procedure NetrServerAliasDel */
        (NDR64_UINT32) 19660864 /* 0x12c0040 */,    /* explicit handle */ /* IsIntrepreted, ClientMustSize, HasReturn, ServerCorrelation, HasExtensions */
        (NDR64_UINT32) 32 /* 0x20 */ ,  /* Stack size */
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 4 /* 0x4 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* Level */      /* parameter Level */
        &__midl_frag949,
        { 
        /* Level */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* InfoStruct */      /* parameter InfoStruct */
        &__midl_frag928,
        { 
        /* InfoStruct */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    }
};

static const __midl_frag920_t __midl_frag920 =
{ 
/* *long */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 8 /* 0x8 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag949
};

static const __midl_frag918_t __midl_frag918 =
{ 
/* *long */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 12 /* 0xc */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag949
};

static const __midl_frag916_t __midl_frag916 =
{ 
/*  */
    { 
    /* *_SERVER_ALIAS_INFO_0 */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 32 /* 0x20 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag914
    }
};

static const __midl_frag915_t __midl_frag915 =
{ 
/*  */
    (NDR64_UINT32) 1 /* 0x1 */,
    { 
    /* struct _NDR64_EXPR_VAR */
        0x3,    /* FC_EXPR_VAR */
        0x6,    /* FC64_UINT32 */
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT32) 0 /* 0x0 */
    }
};

static const __midl_frag914_t __midl_frag914 =
{ 
/* *_SERVER_ALIAS_INFO_0 */
    { 
    /* *_SERVER_ALIAS_INFO_0 */
        { 
        /* *_SERVER_ALIAS_INFO_0 */
            0x46,    /* FC64_FORCED_BOGUS_ARRAY */
            (NDR64_UINT8) 7 /* 0x7 */,
            { 
            /* *_SERVER_ALIAS_INFO_0 */
                0,
                1,
                0,
                0,
                0,
                0,
                0,
                0
            },
            (NDR64_UINT8) 1 /* 0x1 */,
            (NDR64_UINT32) 0 /* 0x0 */,
            &__midl_frag900
        },
        &__midl_frag915,
        0,
        0
    }
};

static const __midl_frag913_t __midl_frag913 =
{ 
/* SERVER_ALIAS_INFO_0_CONTAINER */
    { 
    /* SERVER_ALIAS_INFO_0_CONTAINER */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* SERVER_ALIAS_INFO_0_CONTAINER */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */,
        0,
        0,
        &__midl_frag916,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag912_t __midl_frag912 =
{ 
/* *SERVER_ALIAS_INFO_0_CONTAINER */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag913
};

static const __midl_frag910_t __midl_frag910 =
{ 
/* SERVER_ALIAS_ENUM_UNION */
    { 
    /* SERVER_ALIAS_ENUM_UNION */
        0x51,    /* FC64_NON_ENCAPSULATED_UNION */
        (NDR64_UINT8) 7 /* 0x7 */,
        (NDR64_UINT8) 0 /* 0x0 */,
        0x6,    /* FC64_UINT32 */
        (NDR64_UINT32) 8 /* 0x8 */,
        &__midl_frag915,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM_SELECTOR */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT8) 7 /* 0x7 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT32) 1 /* 0x1 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 0 /* 0x0 */,
        &__midl_frag912,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    (NDR64_UINT32) 4294967295 /* 0xffffffff */
};

static const __midl_frag909_t __midl_frag909 =
{ 
/* _SERVER_ALIAS_ENUM_STRUCT */
    { 
    /* _SERVER_ALIAS_ENUM_STRUCT */
        0x34,    /* FC64_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _SERVER_ALIAS_ENUM_STRUCT */
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */,
        0,
        0,
        0,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_EMBEDDED_COMPLEX_FORMAT */
            0x91,    /* FC64_EMBEDDED_COMPLEX */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            &__midl_frag910
        },
        { 
        /* _SERVER_ALIAS_ENUM_STRUCT */
            0x92,    /* FC64_BUFFER_ALIGN */
            (NDR64_UINT8) 7 /* 0x7 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag908_t __midl_frag908 =
{ 
/* *_SERVER_ALIAS_ENUM_STRUCT */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag909
};

static const __midl_frag905_t __midl_frag905 =
{ 
/* NetrServerAliasEnum */
    { 
    /* NetrServerAliasEnum */      /* procedure NetrServerAliasEnum */
        (NDR64_UINT32) 23986240 /* 0x16e0040 */,    /* explicit handle */ /* IsIntrepreted, ServerMustSize, ClientMustSize, HasReturn, ServerCorrelation, ClientCorrelation, HasExtensions */
        (NDR64_UINT32) 48 /* 0x30 */ ,  /* Stack size */
        (NDR64_UINT32) 40 /* 0x28 */,
        (NDR64_UINT32) 72 /* 0x48 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 6 /* 0x6 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* InfoStruct */      /* parameter InfoStruct */
        &__midl_frag909,
        { 
        /* InfoStruct */
            1,
            1,
            0,
            1,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], [out], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* PreferedMaximumLength */      /* parameter PreferedMaximumLength */
        &__midl_frag949,
        { 
        /* PreferedMaximumLength */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* TotalEntries */      /* parameter TotalEntries */
        &__midl_frag949,
        { 
        /* TotalEntries */
            0,
            0,
            0,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            1
        },    /* [out], Basetype, SimpleRef, UseCache */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* ResumeHandle */      /* parameter ResumeHandle */
        &__midl_frag920,
        { 
        /* ResumeHandle */
            0,
            1,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustFree, [in], [out] */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        40 /* 0x28 */,   /* Stack offset */
    }
};

static const __midl_frag903_t __midl_frag903 =
{ 
/*  */
    { 
    /* *WCHAR */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag934
    },
    { 
    /* *WCHAR */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag934
    }
};

static const __midl_frag900_t __midl_frag900 =
{ 
/* _SERVER_ALIAS_INFO_0 */
    { 
    /* _SERVER_ALIAS_INFO_0 */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _SERVER_ALIAS_INFO_0 */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 24 /* 0x18 */,
        0,
        0,
        &__midl_frag903,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x10,    /* FC64_CHAR */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 3 /* 0x3 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag889_t __midl_frag889 =
{ 
/* _TRANSPORT_INFO */
    { 
    /* _TRANSPORT_INFO */
        0x51,    /* FC64_NON_ENCAPSULATED_UNION */
        (NDR64_UINT8) 7 /* 0x7 */,
        (NDR64_UINT8) 0 /* 0x0 */,
        0x6,    /* FC64_UINT32 */
        (NDR64_UINT32) 312 /* 0x138 */,
        &__midl_frag938,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM_SELECTOR */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT8) 7 /* 0x7 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT32) 4 /* 0x4 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 0 /* 0x0 */,
        &__midl_frag585,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1 /* 0x1 */,
        &__midl_frag609,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 2 /* 0x2 */,
        &__midl_frag622,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 3 /* 0x3 */,
        &__midl_frag635,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    (NDR64_UINT32) 4294967295 /* 0xffffffff */
};

static const __midl_frag888_t __midl_frag888 =
{ 
/* *_TRANSPORT_INFO */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag889
};

static const __midl_frag884_t __midl_frag884 =
{ 
/* NetrServerTransportDelEx */
    { 
    /* NetrServerTransportDelEx */      /* procedure NetrServerTransportDelEx */
        (NDR64_UINT32) 19660864 /* 0x12c0040 */,    /* explicit handle */ /* IsIntrepreted, ClientMustSize, HasReturn, ServerCorrelation, HasExtensions */
        (NDR64_UINT32) 32 /* 0x20 */ ,  /* Stack size */
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 4 /* 0x4 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* Level */      /* parameter Level */
        &__midl_frag949,
        { 
        /* Level */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* Buffer */      /* parameter Buffer */
        &__midl_frag889,
        { 
        /* Buffer */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    }
};

static const __midl_frag882_t __midl_frag882 =
{ 
/*  */
    { 
    /* *WCHAR */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag934
    }
};

static const __midl_frag880_t __midl_frag880 =
{ 
/* DFS_SITENAME_INFO */
    { 
    /* DFS_SITENAME_INFO */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* DFS_SITENAME_INFO */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */,
        0,
        0,
        &__midl_frag882,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag878_t __midl_frag878 =
{ 
/*  */
    { 
    /* struct _NDR64_CONF_VAR_BOGUS_ARRAY_HEADER_FORMAT */
        { 
        /* struct _NDR64_CONF_VAR_BOGUS_ARRAY_HEADER_FORMAT */
            0x46,    /* FC64_FORCED_BOGUS_ARRAY */
            (NDR64_UINT8) 7 /* 0x7 */,
            { 
            /* struct _NDR64_CONF_VAR_BOGUS_ARRAY_HEADER_FORMAT */
                0,
                1,
                0,
                0,
                0,
                0,
                0,
                0
            },
            (NDR64_UINT8) 1 /* 0x1 */,
            (NDR64_UINT32) 0 /* 0x0 */,
            &__midl_frag880
        },
        &__midl_frag915,
        0,
        0
    }
};

static const __midl_frag877_t __midl_frag877 =
{ 
/* _DFS_SITELIST_INFO */
    { 
    /* _DFS_SITELIST_INFO */
        0x37,    /* FC64_FORCED_CONF_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _DFS_SITELIST_INFO */
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 1 /* 0x1 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        0,
        0,
        0,
        &__midl_frag878,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag876_t __midl_frag876 =
{ 
/* *_DFS_SITELIST_INFO */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag877
};

static const __midl_frag875_t __midl_frag875 =
{ 
/* **_DFS_SITELIST_INFO */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 20 /* 0x14 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag876
};

static const __midl_frag872_t __midl_frag872 =
{ 
/* NetrDfsManagerReportSiteInfo */
    { 
    /* NetrDfsManagerReportSiteInfo */      /* procedure NetrDfsManagerReportSiteInfo */
        (NDR64_UINT32) 23986240 /* 0x16e0040 */,    /* explicit handle */ /* IsIntrepreted, ServerMustSize, ClientMustSize, HasReturn, ServerCorrelation, ClientCorrelation, HasExtensions */
        (NDR64_UINT32) 24 /* 0x18 */ ,  /* Stack size */
        (NDR64_UINT32) 0 /* 0x0 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 3 /* 0x3 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* ppSiteInfo */      /* parameter ppSiteInfo */
        &__midl_frag875,
        { 
        /* ppSiteInfo */
            1,
            1,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            1
        },    /* MustSize, MustFree, [in], [out], UseCache */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    }
};

static const __midl_frag869_t __midl_frag869 =
{ 
/* *_NET_DFS_ENTRY_ID_CONTAINER */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag805
};

static const __midl_frag867_t __midl_frag867 =
{ 
/* *WCHAR */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag934
};

static const __midl_frag866_t __midl_frag866 =
{ 
/* *GUID */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag799
};

static const __midl_frag857_t __midl_frag857 =
{ 
/* NetrDfsFixLocalVolume */
    { 
    /* NetrDfsFixLocalVolume */      /* procedure NetrDfsFixLocalVolume */
        (NDR64_UINT32) 19660864 /* 0x12c0040 */,    /* explicit handle */ /* IsIntrepreted, ClientMustSize, HasReturn, ServerCorrelation, HasExtensions */
        (NDR64_UINT32) 80 /* 0x50 */ ,  /* Stack size */
        (NDR64_UINT32) 96 /* 0x60 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 10 /* 0xa */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* VolumeName */      /* parameter VolumeName */
        &__midl_frag934,
        { 
        /* VolumeName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* EntryType */      /* parameter EntryType */
        &__midl_frag949,
        { 
        /* EntryType */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* ServiceType */      /* parameter ServiceType */
        &__midl_frag949,
        { 
        /* ServiceType */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* StgId */      /* parameter StgId */
        &__midl_frag934,
        { 
        /* StgId */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    },
    { 
    /* EntryUid */      /* parameter EntryUid */
        &__midl_frag799,
        { 
        /* EntryUid */
            0,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        40 /* 0x28 */,   /* Stack offset */
    },
    { 
    /* EntryPrefix */      /* parameter EntryPrefix */
        &__midl_frag934,
        { 
        /* EntryPrefix */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        48 /* 0x30 */,   /* Stack offset */
    },
    { 
    /* RelationInfo */      /* parameter RelationInfo */
        &__midl_frag805,
        { 
        /* RelationInfo */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        56 /* 0x38 */,   /* Stack offset */
    },
    { 
    /* CreateDisposition */      /* parameter CreateDisposition */
        &__midl_frag949,
        { 
        /* CreateDisposition */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        64 /* 0x40 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        72 /* 0x48 */,   /* Stack offset */
    }
};

static const __midl_frag850_t __midl_frag850 =
{ 
/* NetrDfsModifyPrefix */
    { 
    /* NetrDfsModifyPrefix */      /* procedure NetrDfsModifyPrefix */
        (NDR64_UINT32) 17563712 /* 0x10c0040 */,    /* explicit handle */ /* IsIntrepreted, ClientMustSize, HasReturn, HasExtensions */
        (NDR64_UINT32) 32 /* 0x20 */ ,  /* Stack size */
        (NDR64_UINT32) 72 /* 0x48 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 4 /* 0x4 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* Uid */      /* parameter Uid */
        &__midl_frag799,
        { 
        /* Uid */
            0,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* Prefix */      /* parameter Prefix */
        &__midl_frag934,
        { 
        /* Prefix */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    }
};

static const __midl_frag842_t __midl_frag842 =
{ 
/* NetrDfsDeleteExitPoint */
    { 
    /* NetrDfsDeleteExitPoint */      /* procedure NetrDfsDeleteExitPoint */
        (NDR64_UINT32) 17563712 /* 0x10c0040 */,    /* explicit handle */ /* IsIntrepreted, ClientMustSize, HasReturn, HasExtensions */
        (NDR64_UINT32) 40 /* 0x28 */ ,  /* Stack size */
        (NDR64_UINT32) 80 /* 0x50 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 5 /* 0x5 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* Uid */      /* parameter Uid */
        &__midl_frag799,
        { 
        /* Uid */
            0,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* Prefix */      /* parameter Prefix */
        &__midl_frag934,
        { 
        /* Prefix */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* Type */      /* parameter Type */
        &__midl_frag949,
        { 
        /* Type */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    }
};

static const __midl_frag840_t __midl_frag840 =
0x11    /* FC64_WCHAR */;

static const __midl_frag839_t __midl_frag839 =
{ 
/*  */
    (NDR64_UINT32) 5 /* 0x5 */,
    { 
    /* struct _NDR64_RANGE_FORMAT */
        0xa4,    /* FC64_RANGE */
        0x5,    /* FC64_INT32 */
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_INT64) 0 /* 0x0 */,
        (NDR64_INT64) 32 /* 0x20 */
    },
    { 
    /* struct _NDR64_EXPR_VAR */
        0x3,    /* FC_EXPR_VAR */
        0x6,    /* FC64_UINT32 */
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT32) 32 /* 0x20 */  /* Offset */
    }
};

static const __midl_frag838_t __midl_frag838 =
{ 
/* *WCHAR */
    { 
    /* *WCHAR */
        0x41,    /* FC64_CONF_ARRAY */
        (NDR64_UINT8) 1 /* 0x1 */,
        { 
        /* *WCHAR */
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 2 /* 0x2 */,
        &__midl_frag839
    },
    { 
    /* struct _NDR64_ARRAY_ELEMENT_INFO */
        (NDR64_UINT32) 2 /* 0x2 */,
        &__midl_frag840
    }
};

static const __midl_frag837_t __midl_frag837 =
{ 
/* *WCHAR */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag838
};

static const __midl_frag836_t __midl_frag836 =
{ 
/* DWORD */
    0xa4,    /* FC64_RANGE */
    0x5,    /* FC64_INT32 */
    (NDR64_UINT16) 0 /* 0x0 */,
    (NDR64_INT64) 0 /* 0x0 */,
    (NDR64_INT64) 32 /* 0x20 */
};

static const __midl_frag829_t __midl_frag829 =
{ 
/* NetrDfsCreateExitPoint */
    { 
    /* NetrDfsCreateExitPoint */      /* procedure NetrDfsCreateExitPoint */
        (NDR64_UINT32) 21889088 /* 0x14e0040 */,    /* explicit handle */ /* IsIntrepreted, ServerMustSize, ClientMustSize, HasReturn, ClientCorrelation, HasExtensions */
        (NDR64_UINT32) 56 /* 0x38 */ ,  /* Stack size */
        (NDR64_UINT32) 88 /* 0x58 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 7 /* 0x7 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* Uid */      /* parameter Uid */
        &__midl_frag799,
        { 
        /* Uid */
            0,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* Prefix */      /* parameter Prefix */
        &__midl_frag934,
        { 
        /* Prefix */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* Type */      /* parameter Type */
        &__midl_frag949,
        { 
        /* Type */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* ShortPrefixLen */      /* parameter ShortPrefixLen */
        &__midl_frag836,
        { 
        /* ShortPrefixLen */
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    },
    { 
    /* ShortPrefix */      /* parameter ShortPrefix */
        &__midl_frag838,
        { 
        /* ShortPrefix */
            1,
            1,
            0,
            0,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [out], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        40 /* 0x28 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        48 /* 0x30 */,   /* Stack offset */
    }
};

static const __midl_frag811_t __midl_frag811 =
{ 
/*  */
    { 
    /* *_NET_DFS_ENTRY_ID */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 32 /* 0x20 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag806
    }
};

static const __midl_frag809_t __midl_frag809 =
{ 
/* _NET_DFS_ENTRY_ID */
    { 
    /* _NET_DFS_ENTRY_ID */
        0x31,    /* FC64_PSTRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _NET_DFS_ENTRY_ID */
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 24 /* 0x18 */
    },
    { 
    /*  */
        { 
        /* struct _NDR64_NO_REPEAT_FORMAT */
            0x80,    /* FC64_NO_REPEAT */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT */
            (NDR64_UINT32) 16 /* 0x10 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* *WCHAR */
            0x21,    /* FC64_UP */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            &__midl_frag934
        },
        0x93    /* FC64_END */
    }
};

static const __midl_frag806_t __midl_frag806 =
{ 
/* *_NET_DFS_ENTRY_ID */
    { 
    /* *_NET_DFS_ENTRY_ID */
        0x41,    /* FC64_CONF_ARRAY */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* *_NET_DFS_ENTRY_ID */
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 24 /* 0x18 */,
        &__midl_frag915
    },
    { 
    /*  */
        { 
        /* struct _NDR64_REPEAT_FORMAT */
            0x82,    /* FC64_VARIABLE_REPEAT */
            { 
            /* struct _NDR64_REPEAT_FORMAT */
                (NDR64_UINT8) 1 /* 0x1 */,
                (NDR64_UINT8) 0 /* 0x0 */
            },
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 24 /* 0x18 */,
            (NDR64_UINT32) 0 /* 0x0 */,
            (NDR64_UINT32) 1 /* 0x1 */
        },
        { 
        /*  */
            { 
            /* struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT */
                (NDR64_UINT32) 16 /* 0x10 */,
                (NDR64_UINT32) 0 /* 0x0 */
            },
            { 
            /* *WCHAR */
                0x21,    /* FC64_UP */
                (NDR64_UINT8) 0 /* 0x0 */,
                (NDR64_UINT16) 0 /* 0x0 */,
                &__midl_frag934
            }
        },
        0x93    /* FC64_END */
    },
    { 
    /* struct _NDR64_ARRAY_ELEMENT_INFO */
        (NDR64_UINT32) 24 /* 0x18 */,
        &__midl_frag809
    }
};

static const __midl_frag805_t __midl_frag805 =
{ 
/* _NET_DFS_ENTRY_ID_CONTAINER */
    { 
    /* _NET_DFS_ENTRY_ID_CONTAINER */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _NET_DFS_ENTRY_ID_CONTAINER */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */,
        0,
        0,
        &__midl_frag811,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag799_t __midl_frag799 =
{ 
/* GUID */
    { 
    /* GUID */
        0x30,    /* FC64_STRUCT */
        (NDR64_UINT8) 3 /* 0x3 */,
        { 
        /* GUID */
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */
    }
};

static const __midl_frag793_t __midl_frag793 =
{ 
/* NetrDfsCreateLocalPartition */
    { 
    /* NetrDfsCreateLocalPartition */      /* procedure NetrDfsCreateLocalPartition */
        (NDR64_UINT32) 19660864 /* 0x12c0040 */,    /* explicit handle */ /* IsIntrepreted, ClientMustSize, HasReturn, ServerCorrelation, HasExtensions */
        (NDR64_UINT32) 64 /* 0x40 */ ,  /* Stack size */
        (NDR64_UINT32) 80 /* 0x50 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 8 /* 0x8 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* ShareName */      /* parameter ShareName */
        &__midl_frag934,
        { 
        /* ShareName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* EntryUid */      /* parameter EntryUid */
        &__midl_frag799,
        { 
        /* EntryUid */
            0,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* EntryPrefix */      /* parameter EntryPrefix */
        &__midl_frag934,
        { 
        /* EntryPrefix */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* ShortName */      /* parameter ShortName */
        &__midl_frag934,
        { 
        /* ShortName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    },
    { 
    /* RelationInfo */      /* parameter RelationInfo */
        &__midl_frag805,
        { 
        /* RelationInfo */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        40 /* 0x28 */,   /* Stack offset */
    },
    { 
    /* Force */      /* parameter Force */
        &__midl_frag949,
        { 
        /* Force */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        48 /* 0x30 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        56 /* 0x38 */,   /* Stack offset */
    }
};

static const __midl_frag787_t __midl_frag787 =
{ 
/* NetrDfsGetVersion */
    { 
    /* NetrDfsGetVersion */      /* procedure NetrDfsGetVersion */
        (NDR64_UINT32) 17563712 /* 0x10c0040 */,    /* explicit handle */ /* IsIntrepreted, ClientMustSize, HasReturn, HasExtensions */
        (NDR64_UINT32) 24 /* 0x18 */ ,  /* Stack size */
        (NDR64_UINT32) 0 /* 0x0 */,
        (NDR64_UINT32) 40 /* 0x28 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 3 /* 0x3 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* Version */      /* parameter Version */
        &__midl_frag949,
        { 
        /* Version */
            0,
            0,
            0,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            1
        },    /* [out], Basetype, SimpleRef, UseCache */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    }
};

static const __midl_frag777_t __midl_frag777 =
{ 
/* *_ADT_SECURITY_DESCRIPTOR */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag763
};

static const __midl_frag769_t __midl_frag769 =
{ 
/* NetrpSetFileSecurity */
    { 
    /* NetrpSetFileSecurity */      /* procedure NetrpSetFileSecurity */
        (NDR64_UINT32) 19660864 /* 0x12c0040 */,    /* explicit handle */ /* IsIntrepreted, ClientMustSize, HasReturn, ServerCorrelation, HasExtensions */
        (NDR64_UINT32) 48 /* 0x30 */ ,  /* Stack size */
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 6 /* 0x6 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* ShareName */      /* parameter ShareName */
        &__midl_frag933,
        { 
        /* ShareName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* lpFileName */      /* parameter lpFileName */
        &__midl_frag934,
        { 
        /* lpFileName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* SecurityInformation */      /* parameter SecurityInformation */
        &__midl_frag949,
        { 
        /* SecurityInformation */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* SecurityDescriptor */      /* parameter SecurityDescriptor */
        &__midl_frag763,
        { 
        /* SecurityDescriptor */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    },
    { 
    /* DWORD */      /* parameter DWORD */
        &__midl_frag949,
        { 
        /* DWORD */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        40 /* 0x28 */,   /* Stack offset */
    }
};

static const __midl_frag767_t __midl_frag767 =
{ 
/*  */
    { 
    /* *char */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 32 /* 0x20 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag764
    }
};

static const __midl_frag766_t __midl_frag766 =
0x10    /* FC64_CHAR */;

static const __midl_frag764_t __midl_frag764 =
{ 
/* *char */
    { 
    /* *char */
        0x41,    /* FC64_CONF_ARRAY */
        (NDR64_UINT8) 0 /* 0x0 */,
        { 
        /* *char */
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 1 /* 0x1 */,
        &__midl_frag915
    },
    { 
    /* struct _NDR64_ARRAY_ELEMENT_INFO */
        (NDR64_UINT32) 1 /* 0x1 */,
        &__midl_frag766
    }
};

static const __midl_frag763_t __midl_frag763 =
{ 
/* _ADT_SECURITY_DESCRIPTOR */
    { 
    /* _ADT_SECURITY_DESCRIPTOR */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _ADT_SECURITY_DESCRIPTOR */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */,
        0,
        0,
        &__midl_frag767,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag761_t __midl_frag761 =
{ 
/* **_ADT_SECURITY_DESCRIPTOR */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 20 /* 0x14 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag945
};

static const __midl_frag753_t __midl_frag753 =
{ 
/* NetrpGetFileSecurity */
    { 
    /* NetrpGetFileSecurity */      /* procedure NetrpGetFileSecurity */
        (NDR64_UINT32) 21889088 /* 0x14e0040 */,    /* explicit handle */ /* IsIntrepreted, ServerMustSize, ClientMustSize, HasReturn, ClientCorrelation, HasExtensions */
        (NDR64_UINT32) 48 /* 0x30 */ ,  /* Stack size */
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 6 /* 0x6 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* ShareName */      /* parameter ShareName */
        &__midl_frag933,
        { 
        /* ShareName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* lpFileName */      /* parameter lpFileName */
        &__midl_frag934,
        { 
        /* lpFileName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* RequestedInformation */      /* parameter RequestedInformation */
        &__midl_frag949,
        { 
        /* RequestedInformation */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* SecurityDescriptor */      /* parameter SecurityDescriptor */
        &__midl_frag761,
        { 
        /* SecurityDescriptor */
            1,
            1,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            1
        },    /* MustSize, MustFree, [out], UseCache */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    },
    { 
    /* DWORD */      /* parameter DWORD */
        &__midl_frag949,
        { 
        /* DWORD */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        40 /* 0x28 */,   /* Stack offset */
    }
};

static const __midl_frag751_t __midl_frag751 =
{ 
/* struct _NDR64_CONTEXT_HANDLE_FORMAT */
    0x70,    /* FC64_BIND_CONTEXT */
    (NDR64_UINT8) 225 /* 0xe1 */,
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT8) 1 /* 0x1 */
};

static const __midl_frag750_t __midl_frag750 =
{ 
/* *struct _NDR64_POINTER_FORMAT */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 4 /* 0x4 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag751
};

static const __midl_frag749_t __midl_frag749 =
{ 
/* NetrShareDelCommit */
    { 
    /* NetrShareDelCommit */      /* procedure NetrShareDelCommit */
        (NDR64_UINT32) 17301568 /* 0x1080040 */,    /* explicit handle */ /* IsIntrepreted, HasReturn, HasExtensions */
        (NDR64_UINT32) 16 /* 0x10 */ ,  /* Stack size */
        (NDR64_UINT32) 60 /* 0x3c */,
        (NDR64_UINT32) 68 /* 0x44 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 2 /* 0x2 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x70,    /* FC64_BIND_CONTEXT */
            (NDR64_UINT8) 224 /* 0xe0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 0 /* 0x0 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ContextHandle */      /* parameter ContextHandle */
        &__midl_frag751,
        { 
        /* ContextHandle */
            0,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], [out], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    }
};

static const __midl_frag747_t __midl_frag747 =
{ 
/* struct _NDR64_CONTEXT_HANDLE_FORMAT */
    0x70,    /* FC64_BIND_CONTEXT */
    (NDR64_UINT8) 160 /* 0xa0 */,
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT8) 1 /* 0x1 */
};

static const __midl_frag746_t __midl_frag746 =
{ 
/* *struct _NDR64_POINTER_FORMAT */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 4 /* 0x4 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag747
};

static const __midl_frag740_t __midl_frag740 =
{ 
/* NetrShareDelStart */
    { 
    /* NetrShareDelStart */      /* procedure NetrShareDelStart */
        (NDR64_UINT32) 17563712 /* 0x10c0040 */,    /* explicit handle */ /* IsIntrepreted, ClientMustSize, HasReturn, HasExtensions */
        (NDR64_UINT32) 40 /* 0x28 */ ,  /* Stack size */
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT32) 68 /* 0x44 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 5 /* 0x5 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* NetName */      /* parameter NetName */
        &__midl_frag934,
        { 
        /* NetName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* Reserved */      /* parameter Reserved */
        &__midl_frag949,
        { 
        /* Reserved */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* ContextHandle */      /* parameter ContextHandle */
        &__midl_frag747,
        { 
        /* ContextHandle */
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    }
};

static const __midl_frag733_t __midl_frag733 =
{ 
/* *_SHARE_ENUM_STRUCT */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag259
};

static const __midl_frag730_t __midl_frag730 =
{ 
/* NetrShareEnumSticky */
    { 
    /* NetrShareEnumSticky */      /* procedure NetrShareEnumSticky */
        (NDR64_UINT32) 23986240 /* 0x16e0040 */,    /* explicit handle */ /* IsIntrepreted, ServerMustSize, ClientMustSize, HasReturn, ServerCorrelation, ClientCorrelation, HasExtensions */
        (NDR64_UINT32) 48 /* 0x30 */ ,  /* Stack size */
        (NDR64_UINT32) 40 /* 0x28 */,
        (NDR64_UINT32) 72 /* 0x48 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 6 /* 0x6 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* InfoStruct */      /* parameter InfoStruct */
        &__midl_frag259,
        { 
        /* InfoStruct */
            1,
            1,
            0,
            1,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], [out], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* PreferedMaximumLength */      /* parameter PreferedMaximumLength */
        &__midl_frag949,
        { 
        /* PreferedMaximumLength */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* TotalEntries */      /* parameter TotalEntries */
        &__midl_frag949,
        { 
        /* TotalEntries */
            0,
            0,
            0,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            1
        },    /* [out], Basetype, SimpleRef, UseCache */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* ResumeHandle */      /* parameter ResumeHandle */
        &__midl_frag920,
        { 
        /* ResumeHandle */
            0,
            1,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustFree, [in], [out] */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        40 /* 0x28 */,   /* Stack offset */
    }
};

static const __midl_frag720_t __midl_frag720 =
{ 
/* NetprNameCompare */
    { 
    /* NetprNameCompare */      /* procedure NetprNameCompare */
        (NDR64_UINT32) 17563712 /* 0x10c0040 */,    /* explicit handle */ /* IsIntrepreted, ClientMustSize, HasReturn, HasExtensions */
        (NDR64_UINT32) 48 /* 0x30 */ ,  /* Stack size */
        (NDR64_UINT32) 16 /* 0x10 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 6 /* 0x6 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* Name1 */      /* parameter Name1 */
        &__midl_frag934,
        { 
        /* Name1 */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* Name2 */      /* parameter Name2 */
        &__midl_frag934,
        { 
        /* Name2 */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* NameType */      /* parameter NameType */
        &__midl_frag949,
        { 
        /* NameType */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* Flags */      /* parameter Flags */
        &__midl_frag949,
        { 
        /* Flags */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    },
    { 
    /* long */      /* parameter long */
        &__midl_frag949,
        { 
        /* long */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        40 /* 0x28 */,   /* Stack offset */
    }
};

static const __midl_frag716_t __midl_frag716 =
{ 
/* DWORD */
    0xa4,    /* FC64_RANGE */
    0x5,    /* FC64_INT32 */
    (NDR64_UINT16) 0 /* 0x0 */,
    (NDR64_INT64) 0 /* 0x0 */,
    (NDR64_INT64) 64000 /* 0xfa00 */
};

static const __midl_frag714_t __midl_frag714 =
{ 
/*  */
    (NDR64_UINT32) 4 /* 0x4 */,
    { 
    /* struct _NDR64_RANGE_FORMAT */
        0xa4,    /* FC64_RANGE */
        0x5,    /* FC64_INT32 */
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_INT64) 0 /* 0x0 */,
        (NDR64_INT64) 64000 /* 0xfa00 */
    },
    { 
    /* struct _NDR64_EXPR_VAR */
        0x3,    /* FC_EXPR_VAR */
        0x6,    /* FC64_UINT32 */
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT32) 24 /* 0x18 */  /* Offset */
    }
};

static const __midl_frag713_t __midl_frag713 =
{ 
/* *WCHAR */
    { 
    /* *WCHAR */
        0x41,    /* FC64_CONF_ARRAY */
        (NDR64_UINT8) 1 /* 0x1 */,
        { 
        /* *WCHAR */
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 2 /* 0x2 */,
        &__midl_frag714
    },
    { 
    /* struct _NDR64_ARRAY_ELEMENT_INFO */
        (NDR64_UINT32) 2 /* 0x2 */,
        &__midl_frag840
    }
};

static const __midl_frag712_t __midl_frag712 =
{ 
/* *WCHAR */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag713
};

static const __midl_frag707_t __midl_frag707 =
{ 
/* NetprNameCanonicalize */
    { 
    /* NetprNameCanonicalize */      /* procedure NetprNameCanonicalize */
        (NDR64_UINT32) 21889088 /* 0x14e0040 */,    /* explicit handle */ /* IsIntrepreted, ServerMustSize, ClientMustSize, HasReturn, ClientCorrelation, HasExtensions */
        (NDR64_UINT32) 56 /* 0x38 */ ,  /* Stack size */
        (NDR64_UINT32) 24 /* 0x18 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 7 /* 0x7 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* Name */      /* parameter Name */
        &__midl_frag934,
        { 
        /* Name */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* Outbuf */      /* parameter Outbuf */
        &__midl_frag713,
        { 
        /* Outbuf */
            1,
            1,
            0,
            0,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [out], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* OutbufLen */      /* parameter OutbufLen */
        &__midl_frag716,
        { 
        /* OutbufLen */
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* NameType */      /* parameter NameType */
        &__midl_frag949,
        { 
        /* NameType */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    },
    { 
    /* Flags */      /* parameter Flags */
        &__midl_frag949,
        { 
        /* Flags */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        40 /* 0x28 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        48 /* 0x30 */,   /* Stack offset */
    }
};

static const __midl_frag699_t __midl_frag699 =
{ 
/* NetprNameValidate */
    { 
    /* NetprNameValidate */      /* procedure NetprNameValidate */
        (NDR64_UINT32) 17563712 /* 0x10c0040 */,    /* explicit handle */ /* IsIntrepreted, ClientMustSize, HasReturn, HasExtensions */
        (NDR64_UINT32) 40 /* 0x28 */ ,  /* Stack size */
        (NDR64_UINT32) 16 /* 0x10 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 5 /* 0x5 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* Name */      /* parameter Name */
        &__midl_frag934,
        { 
        /* Name */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* NameType */      /* parameter NameType */
        &__midl_frag949,
        { 
        /* NameType */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* Flags */      /* parameter Flags */
        &__midl_frag949,
        { 
        /* Flags */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    }
};

static const __midl_frag685_t __midl_frag685 =
{ 
/* *DWORD */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 8 /* 0x8 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag949
};

static const __midl_frag679_t __midl_frag679 =
{ 
/* *char */
    { 
    /* *char */
        0x41,    /* FC64_CONF_ARRAY */
        (NDR64_UINT8) 0 /* 0x0 */,
        { 
        /* *char */
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 1 /* 0x1 */,
        &__midl_frag714
    },
    { 
    /* struct _NDR64_ARRAY_ELEMENT_INFO */
        (NDR64_UINT32) 1 /* 0x1 */,
        &__midl_frag766
    }
};

static const __midl_frag678_t __midl_frag678 =
{ 
/* *char */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag679
};

static const __midl_frag673_t __midl_frag673 =
{ 
/* NetprPathCanonicalize */
    { 
    /* NetprPathCanonicalize */      /* procedure NetprPathCanonicalize */
        (NDR64_UINT32) 21889088 /* 0x14e0040 */,    /* explicit handle */ /* IsIntrepreted, ServerMustSize, ClientMustSize, HasReturn, ClientCorrelation, HasExtensions */
        (NDR64_UINT32) 64 /* 0x40 */ ,  /* Stack size */
        (NDR64_UINT32) 48 /* 0x30 */,
        (NDR64_UINT32) 40 /* 0x28 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 8 /* 0x8 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* PathName */      /* parameter PathName */
        &__midl_frag934,
        { 
        /* PathName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* Outbuf */      /* parameter Outbuf */
        &__midl_frag679,
        { 
        /* Outbuf */
            1,
            1,
            0,
            0,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [out], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* OutbufLen */      /* parameter OutbufLen */
        &__midl_frag716,
        { 
        /* OutbufLen */
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* Prefix */      /* parameter Prefix */
        &__midl_frag934,
        { 
        /* Prefix */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    },
    { 
    /* PathType */      /* parameter PathType */
        &__midl_frag949,
        { 
        /* PathType */
            0,
            0,
            0,
            1,
            1,
            0,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], [out], Basetype, SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        40 /* 0x28 */,   /* Stack offset */
    },
    { 
    /* Flags */      /* parameter Flags */
        &__midl_frag949,
        { 
        /* Flags */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        48 /* 0x30 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        56 /* 0x38 */,   /* Stack offset */
    }
};

static const __midl_frag664_t __midl_frag664 =
{ 
/* NetprPathType */
    { 
    /* NetprPathType */      /* procedure NetprPathType */
        (NDR64_UINT32) 17563712 /* 0x10c0040 */,    /* explicit handle */ /* IsIntrepreted, ClientMustSize, HasReturn, HasExtensions */
        (NDR64_UINT32) 40 /* 0x28 */ ,  /* Stack size */
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT32) 40 /* 0x28 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 5 /* 0x5 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* PathName */      /* parameter PathName */
        &__midl_frag934,
        { 
        /* PathName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* PathType */      /* parameter PathType */
        &__midl_frag949,
        { 
        /* PathType */
            0,
            0,
            0,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            1
        },    /* [out], Basetype, SimpleRef, UseCache */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* Flags */      /* parameter Flags */
        &__midl_frag949,
        { 
        /* Flags */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    }
};

static const __midl_frag662_t __midl_frag662 =
{ 
/* _TIME_OF_DAY_INFO */
    { 
    /* _TIME_OF_DAY_INFO */
        0x30,    /* FC64_STRUCT */
        (NDR64_UINT8) 3 /* 0x3 */,
        { 
        /* _TIME_OF_DAY_INFO */
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 48 /* 0x30 */
    }
};

static const __midl_frag661_t __midl_frag661 =
{ 
/* *_TIME_OF_DAY_INFO */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag662
};

static const __midl_frag660_t __midl_frag660 =
{ 
/* **_TIME_OF_DAY_INFO */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 20 /* 0x14 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag661
};

static const __midl_frag657_t __midl_frag657 =
{ 
/* NetrRemoteTOD */
    { 
    /* NetrRemoteTOD */      /* procedure NetrRemoteTOD */
        (NDR64_UINT32) 17563712 /* 0x10c0040 */,    /* explicit handle */ /* IsIntrepreted, ClientMustSize, HasReturn, HasExtensions */
        (NDR64_UINT32) 24 /* 0x18 */ ,  /* Stack size */
        (NDR64_UINT32) 0 /* 0x0 */,
        (NDR64_UINT32) 120 /* 0x78 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 3 /* 0x3 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* BufferPtr */      /* parameter BufferPtr */
        &__midl_frag660,
        { 
        /* BufferPtr */
            0,
            1,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            1
        },    /* MustFree, [out], UseCache */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    }
};

static const __midl_frag655_t __midl_frag655 =
{ 
/* *SERVER_TRANSPORT_INFO_0 */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag585
};

static const __midl_frag651_t __midl_frag651 =
{ 
/* NetrServerTransportDel */
    { 
    /* NetrServerTransportDel */      /* procedure NetrServerTransportDel */
        (NDR64_UINT32) 19660864 /* 0x12c0040 */,    /* explicit handle */ /* IsIntrepreted, ClientMustSize, HasReturn, ServerCorrelation, HasExtensions */
        (NDR64_UINT32) 32 /* 0x20 */ ,  /* Stack size */
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 4 /* 0x4 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* Level */      /* parameter Level */
        &__midl_frag949,
        { 
        /* Level */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* Buffer */      /* parameter Buffer */
        &__midl_frag585,
        { 
        /* Buffer */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    }
};

static const __midl_frag644_t __midl_frag644 =
{ 
/*  */
    { 
    /* *SERVER_TRANSPORT_INFO_3 */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 32 /* 0x20 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag633
    }
};

static const __midl_frag643_t __midl_frag643 =
{ 
/*  */
    { 
    /* struct _NDR64_FIX_ARRAY_HEADER_FORMAT */
        0x40,    /* FC64_FIX_ARRAY */
        (NDR64_UINT8) 0 /* 0x0 */,
        { 
        /* struct _NDR64_FIX_ARRAY_HEADER_FORMAT */
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 256 /* 0x100 */
    }
};

static const __midl_frag642_t __midl_frag642 =
{ 
/*  */
    { 
    /* *wchar_t */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag934
    },
    { 
    /* *char */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 32 /* 0x20 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag637
    },
    { 
    /* *wchar_t */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag934
    },
    { 
    /* *wchar_t */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag934
    }
};

static const __midl_frag638_t __midl_frag638 =
{ 
/*  */
    (NDR64_UINT32) 0 /* 0x0 */,
    { 
    /* struct _NDR64_EXPR_VAR */
        0x3,    /* FC_EXPR_VAR */
        0x6,    /* FC64_UINT32 */
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT32) 24 /* 0x18 */
    }
};

static const __midl_frag637_t __midl_frag637 =
{ 
/* *char */
    { 
    /* *char */
        0x41,    /* FC64_CONF_ARRAY */
        (NDR64_UINT8) 0 /* 0x0 */,
        { 
        /* *char */
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 1 /* 0x1 */,
        &__midl_frag638
    },
    { 
    /* struct _NDR64_ARRAY_ELEMENT_INFO */
        (NDR64_UINT32) 1 /* 0x1 */,
        &__midl_frag766
    }
};

static const __midl_frag635_t __midl_frag635 =
{ 
/* SERVER_TRANSPORT_INFO_3 */
    { 
    /* SERVER_TRANSPORT_INFO_3 */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* SERVER_TRANSPORT_INFO_3 */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 312 /* 0x138 */,
        0,
        0,
        &__midl_frag642,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_EMBEDDED_COMPLEX_FORMAT */
            0x91,    /* FC64_EMBEDDED_COMPLEX */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            &__midl_frag643
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag633_t __midl_frag633 =
{ 
/* *SERVER_TRANSPORT_INFO_3 */
    { 
    /* *SERVER_TRANSPORT_INFO_3 */
        { 
        /* *SERVER_TRANSPORT_INFO_3 */
            0x46,    /* FC64_FORCED_BOGUS_ARRAY */
            (NDR64_UINT8) 7 /* 0x7 */,
            { 
            /* *SERVER_TRANSPORT_INFO_3 */
                0,
                1,
                0,
                0,
                0,
                0,
                0,
                0
            },
            (NDR64_UINT8) 1 /* 0x1 */,
            (NDR64_UINT32) 0 /* 0x0 */,
            &__midl_frag635
        },
        &__midl_frag915,
        0,
        0
    }
};

static const __midl_frag632_t __midl_frag632 =
{ 
/* _SERVER_XPORT_INFO_3_CONTAINER */
    { 
    /* _SERVER_XPORT_INFO_3_CONTAINER */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _SERVER_XPORT_INFO_3_CONTAINER */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */,
        0,
        0,
        &__midl_frag644,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag631_t __midl_frag631 =
{ 
/* *_SERVER_XPORT_INFO_3_CONTAINER */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag632
};

static const __midl_frag630_t __midl_frag630 =
{ 
/*  */
    { 
    /* *SERVER_TRANSPORT_INFO_2 */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 32 /* 0x20 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag620
    }
};

static const __midl_frag622_t __midl_frag622 =
{ 
/* SERVER_TRANSPORT_INFO_2 */
    { 
    /* SERVER_TRANSPORT_INFO_2 */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* SERVER_TRANSPORT_INFO_2 */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 56 /* 0x38 */,
        0,
        0,
        &__midl_frag642,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* SERVER_TRANSPORT_INFO_2 */
            0x92,    /* FC64_BUFFER_ALIGN */
            (NDR64_UINT8) 7 /* 0x7 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag620_t __midl_frag620 =
{ 
/* *SERVER_TRANSPORT_INFO_2 */
    { 
    /* *SERVER_TRANSPORT_INFO_2 */
        { 
        /* *SERVER_TRANSPORT_INFO_2 */
            0x46,    /* FC64_FORCED_BOGUS_ARRAY */
            (NDR64_UINT8) 7 /* 0x7 */,
            { 
            /* *SERVER_TRANSPORT_INFO_2 */
                0,
                1,
                0,
                0,
                0,
                0,
                0,
                0
            },
            (NDR64_UINT8) 1 /* 0x1 */,
            (NDR64_UINT32) 0 /* 0x0 */,
            &__midl_frag622
        },
        &__midl_frag915,
        0,
        0
    }
};

static const __midl_frag619_t __midl_frag619 =
{ 
/* _SERVER_XPORT_INFO_2_CONTAINER */
    { 
    /* _SERVER_XPORT_INFO_2_CONTAINER */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _SERVER_XPORT_INFO_2_CONTAINER */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */,
        0,
        0,
        &__midl_frag630,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag618_t __midl_frag618 =
{ 
/* *_SERVER_XPORT_INFO_2_CONTAINER */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag619
};

static const __midl_frag617_t __midl_frag617 =
{ 
/*  */
    { 
    /* *SERVER_TRANSPORT_INFO_1 */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 32 /* 0x20 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag607
    }
};

static const __midl_frag609_t __midl_frag609 =
{ 
/* SERVER_TRANSPORT_INFO_1 */
    { 
    /* SERVER_TRANSPORT_INFO_1 */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* SERVER_TRANSPORT_INFO_1 */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 48 /* 0x30 */,
        0,
        0,
        &__midl_frag642,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag607_t __midl_frag607 =
{ 
/* *SERVER_TRANSPORT_INFO_1 */
    { 
    /* *SERVER_TRANSPORT_INFO_1 */
        { 
        /* *SERVER_TRANSPORT_INFO_1 */
            0x46,    /* FC64_FORCED_BOGUS_ARRAY */
            (NDR64_UINT8) 7 /* 0x7 */,
            { 
            /* *SERVER_TRANSPORT_INFO_1 */
                0,
                1,
                0,
                0,
                0,
                0,
                0,
                0
            },
            (NDR64_UINT8) 1 /* 0x1 */,
            (NDR64_UINT32) 0 /* 0x0 */,
            &__midl_frag609
        },
        &__midl_frag915,
        0,
        0
    }
};

static const __midl_frag606_t __midl_frag606 =
{ 
/* _SERVER_XPORT_INFO_1_CONTAINER */
    { 
    /* _SERVER_XPORT_INFO_1_CONTAINER */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _SERVER_XPORT_INFO_1_CONTAINER */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */,
        0,
        0,
        &__midl_frag617,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag605_t __midl_frag605 =
{ 
/* *_SERVER_XPORT_INFO_1_CONTAINER */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag606
};

static const __midl_frag604_t __midl_frag604 =
{ 
/*  */
    { 
    /* *SERVER_TRANSPORT_INFO_0 */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 32 /* 0x20 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag602
    }
};

static const __midl_frag602_t __midl_frag602 =
{ 
/* *SERVER_TRANSPORT_INFO_0 */
    { 
    /* *SERVER_TRANSPORT_INFO_0 */
        { 
        /* *SERVER_TRANSPORT_INFO_0 */
            0x46,    /* FC64_FORCED_BOGUS_ARRAY */
            (NDR64_UINT8) 7 /* 0x7 */,
            { 
            /* *SERVER_TRANSPORT_INFO_0 */
                0,
                1,
                0,
                0,
                0,
                0,
                0,
                0
            },
            (NDR64_UINT8) 1 /* 0x1 */,
            (NDR64_UINT32) 0 /* 0x0 */,
            &__midl_frag585
        },
        &__midl_frag915,
        0,
        0
    }
};

static const __midl_frag601_t __midl_frag601 =
{ 
/* _SERVER_XPORT_INFO_0_CONTAINER */
    { 
    /* _SERVER_XPORT_INFO_0_CONTAINER */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _SERVER_XPORT_INFO_0_CONTAINER */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */,
        0,
        0,
        &__midl_frag604,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag600_t __midl_frag600 =
{ 
/* *_SERVER_XPORT_INFO_0_CONTAINER */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag601
};

static const __midl_frag598_t __midl_frag598 =
{ 
/* SERVER_XPORT_ENUM_UNION */
    { 
    /* SERVER_XPORT_ENUM_UNION */
        0x51,    /* FC64_NON_ENCAPSULATED_UNION */
        (NDR64_UINT8) 7 /* 0x7 */,
        (NDR64_UINT8) 0 /* 0x0 */,
        0x6,    /* FC64_UINT32 */
        (NDR64_UINT32) 8 /* 0x8 */,
        &__midl_frag915,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM_SELECTOR */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT8) 7 /* 0x7 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT32) 4 /* 0x4 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 0 /* 0x0 */,
        &__midl_frag600,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1 /* 0x1 */,
        &__midl_frag605,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 2 /* 0x2 */,
        &__midl_frag618,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 3 /* 0x3 */,
        &__midl_frag631,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    (NDR64_UINT32) 4294967295 /* 0xffffffff */
};

static const __midl_frag597_t __midl_frag597 =
{ 
/* _SERVER_XPORT_ENUM_STRUCT */
    { 
    /* _SERVER_XPORT_ENUM_STRUCT */
        0x34,    /* FC64_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _SERVER_XPORT_ENUM_STRUCT */
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */,
        0,
        0,
        0,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_EMBEDDED_COMPLEX_FORMAT */
            0x91,    /* FC64_EMBEDDED_COMPLEX */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            &__midl_frag598
        },
        { 
        /* _SERVER_XPORT_ENUM_STRUCT */
            0x92,    /* FC64_BUFFER_ALIGN */
            (NDR64_UINT8) 7 /* 0x7 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag596_t __midl_frag596 =
{ 
/* *_SERVER_XPORT_ENUM_STRUCT */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag597
};

static const __midl_frag593_t __midl_frag593 =
{ 
/* NetrServerTransportEnum */
    { 
    /* NetrServerTransportEnum */      /* procedure NetrServerTransportEnum */
        (NDR64_UINT32) 23986240 /* 0x16e0040 */,    /* explicit handle */ /* IsIntrepreted, ServerMustSize, ClientMustSize, HasReturn, ServerCorrelation, ClientCorrelation, HasExtensions */
        (NDR64_UINT32) 48 /* 0x30 */ ,  /* Stack size */
        (NDR64_UINT32) 40 /* 0x28 */,
        (NDR64_UINT32) 72 /* 0x48 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 6 /* 0x6 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* InfoStruct */      /* parameter InfoStruct */
        &__midl_frag597,
        { 
        /* InfoStruct */
            1,
            1,
            0,
            1,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], [out], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* PreferedMaximumLength */      /* parameter PreferedMaximumLength */
        &__midl_frag949,
        { 
        /* PreferedMaximumLength */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* TotalEntries */      /* parameter TotalEntries */
        &__midl_frag949,
        { 
        /* TotalEntries */
            0,
            0,
            0,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            1
        },    /* [out], Basetype, SimpleRef, UseCache */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* ResumeHandle */      /* parameter ResumeHandle */
        &__midl_frag920,
        { 
        /* ResumeHandle */
            0,
            1,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustFree, [in], [out] */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        40 /* 0x28 */,   /* Stack offset */
    }
};

static const __midl_frag591_t __midl_frag591 =
{ 
/*  */
    { 
    /* *wchar_t */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag934
    },
    { 
    /* *char */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 32 /* 0x20 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag637
    },
    { 
    /* *wchar_t */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag934
    }
};

static const __midl_frag585_t __midl_frag585 =
{ 
/* SERVER_TRANSPORT_INFO_0 */
    { 
    /* SERVER_TRANSPORT_INFO_0 */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* SERVER_TRANSPORT_INFO_0 */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 40 /* 0x28 */,
        0,
        0,
        &__midl_frag591,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag578_t __midl_frag578 =
{ 
/* _STAT_SERVER_0 */
    { 
    /* _STAT_SERVER_0 */
        0x30,    /* FC64_STRUCT */
        (NDR64_UINT8) 3 /* 0x3 */,
        { 
        /* _STAT_SERVER_0 */
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 68 /* 0x44 */
    }
};

static const __midl_frag577_t __midl_frag577 =
{ 
/* *_STAT_SERVER_0 */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag578
};

static const __midl_frag576_t __midl_frag576 =
{ 
/* **_STAT_SERVER_0 */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 20 /* 0x14 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag577
};

static const __midl_frag569_t __midl_frag569 =
{ 
/* NetrServerStatisticsGet */
    { 
    /* NetrServerStatisticsGet */      /* procedure NetrServerStatisticsGet */
        (NDR64_UINT32) 17563712 /* 0x10c0040 */,    /* explicit handle */ /* IsIntrepreted, ClientMustSize, HasReturn, HasExtensions */
        (NDR64_UINT32) 48 /* 0x30 */ ,  /* Stack size */
        (NDR64_UINT32) 16 /* 0x10 */,
        (NDR64_UINT32) 140 /* 0x8c */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 6 /* 0x6 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* Service */      /* parameter Service */
        &__midl_frag933,
        { 
        /* Service */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* Level */      /* parameter Level */
        &__midl_frag949,
        { 
        /* Level */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* Options */      /* parameter Options */
        &__midl_frag949,
        { 
        /* Options */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* InfoStruct */      /* parameter InfoStruct */
        &__midl_frag576,
        { 
        /* InfoStruct */
            0,
            1,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            1
        },    /* MustFree, [out], UseCache */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        40 /* 0x28 */,   /* Stack offset */
    }
};

static const __midl_frag562_t __midl_frag562 =
{ 
/*  */
    { 
    /* *_DISK_INFO */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 32 /* 0x20 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag556
    }
};

static const __midl_frag561_t __midl_frag561 =
{ 
/* struct _NDR64_NON_CONFORMANT_STRING_FORMAT */
    { 
    /* struct _NDR64_NON_CONFORMANT_STRING_FORMAT */
        0x61,    /* FC64_WCHAR_STRING */
        { 
        /* struct _NDR64_NON_CONFORMANT_STRING_FORMAT */
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT16) 2 /* 0x2 */
    },
    (NDR64_UINT32) 6 /* 0x6 */
};

static const __midl_frag560_t __midl_frag560 =
{ 
/* _DISK_INFO */
    { 
    /* _DISK_INFO */
        0x34,    /* FC64_BOGUS_STRUCT */
        (NDR64_UINT8) 1 /* 0x1 */,
        { 
        /* _DISK_INFO */
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 6 /* 0x6 */,
        0,
        0,
        0,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_EMBEDDED_COMPLEX_FORMAT */
            0x91,    /* FC64_EMBEDDED_COMPLEX */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            &__midl_frag561
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag559_t __midl_frag559 =
{ 
/*  */
    (NDR64_UINT32) 1 /* 0x1 */,
    { 
    /* struct _NDR64_EXPR_NOOP */
        0x5,    /* FC_EXPR_PAD */
        (NDR64_UINT8) 4 /* 0x4 */,
        (NDR64_UINT16) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_EXPR_CONST64 */
        0x2,    /* FC_EXPR_CONST64 */
        0x7,    /* FC64_INT64 */
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT64) 0 /* 0x0 */
    }
};

static const __midl_frag556_t __midl_frag556 =
{ 
/* *_DISK_INFO */
    { 
    /* *_DISK_INFO */
        { 
        /* *_DISK_INFO */
            0x47,    /* FC64_BOGUS_ARRAY */
            (NDR64_UINT8) 1 /* 0x1 */,
            { 
            /* *_DISK_INFO */
                0,
                1,
                0,
                0,
                0,
                0,
                0,
                0
            },
            (NDR64_UINT8) 1 /* 0x1 */,
            (NDR64_UINT32) 0 /* 0x0 */,
            &__midl_frag560
        },
        &__midl_frag915,
        &__midl_frag915,
        &__midl_frag559
    }
};

static const __midl_frag555_t __midl_frag555 =
{ 
/* DISK_ENUM_CONTAINER */
    { 
    /* DISK_ENUM_CONTAINER */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* DISK_ENUM_CONTAINER */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */,
        0,
        0,
        &__midl_frag562,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag554_t __midl_frag554 =
{ 
/* *DISK_ENUM_CONTAINER */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag555
};

static const __midl_frag550_t __midl_frag550 =
{ 
/* NetrServerDiskEnum */
    { 
    /* NetrServerDiskEnum */      /* procedure NetrServerDiskEnum */
        (NDR64_UINT32) 23986240 /* 0x16e0040 */,    /* explicit handle */ /* IsIntrepreted, ServerMustSize, ClientMustSize, HasReturn, ServerCorrelation, ClientCorrelation, HasExtensions */
        (NDR64_UINT32) 56 /* 0x38 */ ,  /* Stack size */
        (NDR64_UINT32) 48 /* 0x30 */,
        (NDR64_UINT32) 72 /* 0x48 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 7 /* 0x7 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* Level */      /* parameter Level */
        &__midl_frag949,
        { 
        /* Level */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* DiskInfoStruct */      /* parameter DiskInfoStruct */
        &__midl_frag555,
        { 
        /* DiskInfoStruct */
            1,
            1,
            0,
            1,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], [out], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* PreferedMaximumLength */      /* parameter PreferedMaximumLength */
        &__midl_frag949,
        { 
        /* PreferedMaximumLength */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* TotalEntries */      /* parameter TotalEntries */
        &__midl_frag949,
        { 
        /* TotalEntries */
            0,
            0,
            0,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            1
        },    /* [out], Basetype, SimpleRef, UseCache */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    },
    { 
    /* ResumeHandle */      /* parameter ResumeHandle */
        &__midl_frag920,
        { 
        /* ResumeHandle */
            0,
            1,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustFree, [in], [out] */
        (NDR64_UINT16) 0 /* 0x0 */,
        40 /* 0x28 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        48 /* 0x30 */,   /* Stack offset */
    }
};

static const __midl_frag502_t __midl_frag502 =
{ 
/* *_SERVER_INFO_599 */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag397
};

static const __midl_frag501_t __midl_frag501 =
{ 
/* *_SERVER_INFO_503 */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag394
};

static const __midl_frag500_t __midl_frag500 =
{ 
/* *_SERVER_INFO_502 */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag392
};

static const __midl_frag499_t __midl_frag499 =
{ 
/* *_SERVER_INFO_103 */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag386
};

static const __midl_frag498_t __midl_frag498 =
{ 
/* *_SERVER_INFO_102 */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag380
};

static const __midl_frag497_t __midl_frag497 =
{ 
/* *_SERVER_INFO_101 */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag375
};

static const __midl_frag496_t __midl_frag496 =
{ 
/* *_SERVER_INFO_100 */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag880
};

static const __midl_frag494_t __midl_frag494 =
{ 
/* _SERVER_INFO */
    { 
    /* _SERVER_INFO */
        0x51,    /* FC64_NON_ENCAPSULATED_UNION */
        (NDR64_UINT8) 7 /* 0x7 */,
        (NDR64_UINT8) 0 /* 0x0 */,
        0x6,    /* FC64_UINT32 */
        (NDR64_UINT32) 8 /* 0x8 */,
        &__midl_frag938,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM_SELECTOR */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT8) 7 /* 0x7 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT32) 51 /* 0x33 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 100 /* 0x64 */,
        &__midl_frag496,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 101 /* 0x65 */,
        &__midl_frag497,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 102 /* 0x66 */,
        &__midl_frag498,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 103 /* 0x67 */,
        &__midl_frag499,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 502 /* 0x1f6 */,
        &__midl_frag500,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 503 /* 0x1f7 */,
        &__midl_frag501,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 599 /* 0x257 */,
        &__midl_frag502,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1005 /* 0x3ed */,
        &__midl_frag943,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1107 /* 0x453 */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1010 /* 0x3f2 */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1016 /* 0x3f8 */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1017 /* 0x3f9 */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1018 /* 0x3fa */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1501 /* 0x5dd */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1502 /* 0x5de */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1503 /* 0x5df */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1506 /* 0x5e2 */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1510 /* 0x5e6 */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1511 /* 0x5e7 */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1512 /* 0x5e8 */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1513 /* 0x5e9 */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1514 /* 0x5ea */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1515 /* 0x5eb */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1516 /* 0x5ec */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1518 /* 0x5ee */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1523 /* 0x5f3 */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1528 /* 0x5f8 */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1529 /* 0x5f9 */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1530 /* 0x5fa */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1533 /* 0x5fd */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1534 /* 0x5fe */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1535 /* 0x5ff */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1536 /* 0x600 */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1538 /* 0x602 */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1539 /* 0x603 */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1540 /* 0x604 */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1541 /* 0x605 */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1542 /* 0x606 */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1543 /* 0x607 */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1544 /* 0x608 */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1545 /* 0x609 */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1546 /* 0x60a */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1547 /* 0x60b */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1548 /* 0x60c */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1549 /* 0x60d */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1550 /* 0x60e */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1552 /* 0x610 */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1553 /* 0x611 */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1554 /* 0x612 */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1555 /* 0x613 */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1556 /* 0x614 */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    (NDR64_UINT32) 4294967295 /* 0xffffffff */
};

static const __midl_frag493_t __midl_frag493 =
{ 
/* *_SERVER_INFO */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag494
};

static const __midl_frag489_t __midl_frag489 =
{ 
/* NetrServerSetInfo */
    { 
    /* NetrServerSetInfo */      /* procedure NetrServerSetInfo */
        (NDR64_UINT32) 19660864 /* 0x12c0040 */,    /* explicit handle */ /* IsIntrepreted, ClientMustSize, HasReturn, ServerCorrelation, HasExtensions */
        (NDR64_UINT32) 40 /* 0x28 */ ,  /* Stack size */
        (NDR64_UINT32) 40 /* 0x28 */,
        (NDR64_UINT32) 40 /* 0x28 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 5 /* 0x5 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* Level */      /* parameter Level */
        &__midl_frag949,
        { 
        /* Level */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* ServerInfo */      /* parameter ServerInfo */
        &__midl_frag494,
        { 
        /* ServerInfo */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* ParmErr */      /* parameter ParmErr */
        &__midl_frag920,
        { 
        /* ParmErr */
            0,
            1,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustFree, [in], [out] */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    }
};

static const __midl_frag487_t __midl_frag487 =
{ 
/* _SERVER_INFO_1556 */
    { 
    /* _SERVER_INFO_1556 */
        0x30,    /* FC64_STRUCT */
        (NDR64_UINT8) 3 /* 0x3 */,
        { 
        /* _SERVER_INFO_1556 */
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 4 /* 0x4 */
    }
};

static const __midl_frag400_t __midl_frag400 =
{ 
/* _SERVER_INFO_1005 */
    { 
    /* _SERVER_INFO_1005 */
        0x31,    /* FC64_PSTRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _SERVER_INFO_1005 */
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 8 /* 0x8 */
    },
    { 
    /*  */
        { 
        /* struct _NDR64_NO_REPEAT_FORMAT */
            0x80,    /* FC64_NO_REPEAT */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT */
            (NDR64_UINT32) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* *wchar_t */
            0x21,    /* FC64_UP */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            &__midl_frag934
        },
        0x93    /* FC64_END */
    }
};

static const __midl_frag397_t __midl_frag397 =
{ 
/* _SERVER_INFO_599 */
    { 
    /* _SERVER_INFO_599 */
        0x31,    /* FC64_PSTRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _SERVER_INFO_599 */
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 232 /* 0xe8 */
    },
    { 
    /*  */
        { 
        /* struct _NDR64_NO_REPEAT_FORMAT */
            0x80,    /* FC64_NO_REPEAT */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT */
            (NDR64_UINT32) 72 /* 0x48 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* *wchar_t */
            0x21,    /* FC64_UP */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            &__midl_frag934
        },
        0x93    /* FC64_END */
    }
};

static const __midl_frag394_t __midl_frag394 =
{ 
/* _SERVER_INFO_503 */
    { 
    /* _SERVER_INFO_503 */
        0x31,    /* FC64_PSTRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _SERVER_INFO_503 */
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 176 /* 0xb0 */
    },
    { 
    /*  */
        { 
        /* struct _NDR64_NO_REPEAT_FORMAT */
            0x80,    /* FC64_NO_REPEAT */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT */
            (NDR64_UINT32) 72 /* 0x48 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* *wchar_t */
            0x21,    /* FC64_UP */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            &__midl_frag934
        },
        0x93    /* FC64_END */
    }
};

static const __midl_frag392_t __midl_frag392 =
{ 
/* _SERVER_INFO_502 */
    { 
    /* _SERVER_INFO_502 */
        0x30,    /* FC64_STRUCT */
        (NDR64_UINT8) 3 /* 0x3 */,
        { 
        /* _SERVER_INFO_502 */
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 72 /* 0x48 */
    }
};

static const __midl_frag390_t __midl_frag390 =
{ 
/*  */
    { 
    /* *wchar_t */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag934
    },
    { 
    /* *wchar_t */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag934
    },
    { 
    /* *wchar_t */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag934
    }
};

static const __midl_frag386_t __midl_frag386 =
{ 
/* _SERVER_INFO_103 */
    { 
    /* _SERVER_INFO_103 */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _SERVER_INFO_103 */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 80 /* 0x50 */,
        0,
        0,
        &__midl_frag390,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* _SERVER_INFO_103 */
            0x92,    /* FC64_BUFFER_ALIGN */
            (NDR64_UINT8) 7 /* 0x7 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag380_t __midl_frag380 =
{ 
/* _SERVER_INFO_102 */
    { 
    /* _SERVER_INFO_102 */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _SERVER_INFO_102 */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 72 /* 0x48 */,
        0,
        0,
        &__midl_frag390,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag375_t __midl_frag375 =
{ 
/* _SERVER_INFO_101 */
    { 
    /* _SERVER_INFO_101 */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _SERVER_INFO_101 */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 40 /* 0x28 */,
        0,
        0,
        &__midl_frag903,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag367_t __midl_frag367 =
{ 
/* *_SERVER_INFO */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 4 /* 0x4 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag494
};

static const __midl_frag363_t __midl_frag363 =
{ 
/* NetrServerGetInfo */
    { 
    /* NetrServerGetInfo */      /* procedure NetrServerGetInfo */
        (NDR64_UINT32) 21889088 /* 0x14e0040 */,    /* explicit handle */ /* IsIntrepreted, ServerMustSize, ClientMustSize, HasReturn, ClientCorrelation, HasExtensions */
        (NDR64_UINT32) 32 /* 0x20 */ ,  /* Stack size */
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 4 /* 0x4 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* Level */      /* parameter Level */
        &__midl_frag949,
        { 
        /* Level */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* InfoStruct */      /* parameter InfoStruct */
        &__midl_frag494,
        { 
        /* InfoStruct */
            1,
            1,
            0,
            0,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            1
        },    /* MustSize, MustFree, [out], SimpleRef, UseCache */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    }
};

static const __midl_frag355_t __midl_frag355 =
{ 
/* NetrShareCheck */
    { 
    /* NetrShareCheck */      /* procedure NetrShareCheck */
        (NDR64_UINT32) 17563712 /* 0x10c0040 */,    /* explicit handle */ /* IsIntrepreted, ClientMustSize, HasReturn, HasExtensions */
        (NDR64_UINT32) 32 /* 0x20 */ ,  /* Stack size */
        (NDR64_UINT32) 0 /* 0x0 */,
        (NDR64_UINT32) 40 /* 0x28 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 4 /* 0x4 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* Device */      /* parameter Device */
        &__midl_frag934,
        { 
        /* Device */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* Type */      /* parameter Type */
        &__midl_frag949,
        { 
        /* Type */
            0,
            0,
            0,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            1
        },    /* [out], Basetype, SimpleRef, UseCache */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    }
};

static const __midl_frag348_t __midl_frag348 =
{ 
/* NetrShareDelSticky */
    { 
    /* NetrShareDelSticky */      /* procedure NetrShareDelSticky */
        (NDR64_UINT32) 17563712 /* 0x10c0040 */,    /* explicit handle */ /* IsIntrepreted, ClientMustSize, HasReturn, HasExtensions */
        (NDR64_UINT32) 32 /* 0x20 */ ,  /* Stack size */
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 4 /* 0x4 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* NetName */      /* parameter NetName */
        &__midl_frag934,
        { 
        /* NetName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* Reserved */      /* parameter Reserved */
        &__midl_frag949,
        { 
        /* Reserved */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    }
};

static const __midl_frag327_t __midl_frag327 =
{ 
/*  */
    (NDR64_UINT32) 1 /* 0x1 */,
    { 
    /* struct _NDR64_EXPR_VAR */
        0x3,    /* FC_EXPR_VAR */
        0x6,    /* FC64_UINT32 */
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */  /* Offset */
    }
};

static const __midl_frag326_t __midl_frag326 =
{ 
/* _SHARE_INFO */
    { 
    /* _SHARE_INFO */
        0x51,    /* FC64_NON_ENCAPSULATED_UNION */
        (NDR64_UINT8) 7 /* 0x7 */,
        (NDR64_UINT8) 0 /* 0x0 */,
        0x6,    /* FC64_UINT32 */
        (NDR64_UINT32) 8 /* 0x8 */,
        &__midl_frag327,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM_SELECTOR */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT8) 7 /* 0x7 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT32) 10 /* 0xa */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 0 /* 0x0 */,
        &__midl_frag943,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1 /* 0x1 */,
        &__midl_frag940,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 2 /* 0x2 */,
        &__midl_frag941,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 502 /* 0x1f6 */,
        &__midl_frag942,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1004 /* 0x3ec */,
        &__midl_frag943,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1006 /* 0x3ee */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1501 /* 0x5dd */,
        &__midl_frag945,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1005 /* 0x3ed */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 501 /* 0x1f5 */,
        &__midl_frag947,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 503 /* 0x1f7 */,
        &__midl_frag948,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    (NDR64_UINT32) 0 /* 0x0 */
};

static const __midl_frag325_t __midl_frag325 =
{ 
/* *_SHARE_INFO */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag326
};

static const __midl_frag319_t __midl_frag319 =
{ 
/* NetrShareSetInfo */
    { 
    /* NetrShareSetInfo */      /* procedure NetrShareSetInfo */
        (NDR64_UINT32) 19660864 /* 0x12c0040 */,    /* explicit handle */ /* IsIntrepreted, ClientMustSize, HasReturn, ServerCorrelation, HasExtensions */
        (NDR64_UINT32) 48 /* 0x30 */ ,  /* Stack size */
        (NDR64_UINT32) 40 /* 0x28 */,
        (NDR64_UINT32) 40 /* 0x28 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 6 /* 0x6 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* NetName */      /* parameter NetName */
        &__midl_frag934,
        { 
        /* NetName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* Level */      /* parameter Level */
        &__midl_frag949,
        { 
        /* Level */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* ShareInfo */      /* parameter ShareInfo */
        &__midl_frag326,
        { 
        /* ShareInfo */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* ParmErr */      /* parameter ParmErr */
        &__midl_frag920,
        { 
        /* ParmErr */
            0,
            1,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustFree, [in], [out] */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        40 /* 0x28 */,   /* Stack offset */
    }
};

static const __midl_frag305_t __midl_frag305 =
{ 
/* *_SHARE_INFO */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 4 /* 0x4 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag326
};

static const __midl_frag299_t __midl_frag299 =
{ 
/* NetrShareGetInfo */
    { 
    /* NetrShareGetInfo */      /* procedure NetrShareGetInfo */
        (NDR64_UINT32) 21889088 /* 0x14e0040 */,    /* explicit handle */ /* IsIntrepreted, ServerMustSize, ClientMustSize, HasReturn, ClientCorrelation, HasExtensions */
        (NDR64_UINT32) 40 /* 0x28 */ ,  /* Stack size */
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 5 /* 0x5 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* NetName */      /* parameter NetName */
        &__midl_frag934,
        { 
        /* NetName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* Level */      /* parameter Level */
        &__midl_frag949,
        { 
        /* Level */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* InfoStruct */      /* parameter InfoStruct */
        &__midl_frag326,
        { 
        /* InfoStruct */
            1,
            1,
            0,
            0,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            1
        },    /* MustSize, MustFree, [out], SimpleRef, UseCache */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    }
};

static const __midl_frag292_t __midl_frag292 =
{ 
/*  */
    { 
    /* *_SHARE_INFO_503_I */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 32 /* 0x20 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag290
    }
};

static const __midl_frag290_t __midl_frag290 =
{ 
/* *_SHARE_INFO_503_I */
    { 
    /* *_SHARE_INFO_503_I */
        { 
        /* *_SHARE_INFO_503_I */
            0x46,    /* FC64_FORCED_BOGUS_ARRAY */
            (NDR64_UINT8) 7 /* 0x7 */,
            { 
            /* *_SHARE_INFO_503_I */
                0,
                1,
                0,
                0,
                0,
                0,
                0,
                0
            },
            (NDR64_UINT8) 1 /* 0x1 */,
            (NDR64_UINT32) 0 /* 0x0 */,
            &__midl_frag242
        },
        &__midl_frag915,
        0,
        0
    }
};

static const __midl_frag289_t __midl_frag289 =
{ 
/* SHARE_INFO_503_CONTAINER */
    { 
    /* SHARE_INFO_503_CONTAINER */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* SHARE_INFO_503_CONTAINER */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */,
        0,
        0,
        &__midl_frag292,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag288_t __midl_frag288 =
{ 
/* *SHARE_INFO_503_CONTAINER */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag289
};

static const __midl_frag287_t __midl_frag287 =
{ 
/*  */
    { 
    /* *_SHARE_INFO_502_I */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 32 /* 0x20 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag285
    }
};

static const __midl_frag285_t __midl_frag285 =
{ 
/* *_SHARE_INFO_502_I */
    { 
    /* *_SHARE_INFO_502_I */
        { 
        /* *_SHARE_INFO_502_I */
            0x46,    /* FC64_FORCED_BOGUS_ARRAY */
            (NDR64_UINT8) 7 /* 0x7 */,
            { 
            /* *_SHARE_INFO_502_I */
                0,
                1,
                0,
                0,
                0,
                0,
                0,
                0
            },
            (NDR64_UINT8) 1 /* 0x1 */,
            (NDR64_UINT32) 0 /* 0x0 */,
            &__midl_frag214
        },
        &__midl_frag915,
        0,
        0
    }
};

static const __midl_frag284_t __midl_frag284 =
{ 
/* SHARE_INFO_502_CONTAINER */
    { 
    /* SHARE_INFO_502_CONTAINER */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* SHARE_INFO_502_CONTAINER */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */,
        0,
        0,
        &__midl_frag287,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag283_t __midl_frag283 =
{ 
/* *SHARE_INFO_502_CONTAINER */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag284
};

static const __midl_frag282_t __midl_frag282 =
{ 
/*  */
    { 
    /* *_SHARE_INFO_501 */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 32 /* 0x20 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag280
    }
};

static const __midl_frag280_t __midl_frag280 =
{ 
/* *_SHARE_INFO_501 */
    { 
    /* *_SHARE_INFO_501 */
        { 
        /* *_SHARE_INFO_501 */
            0x46,    /* FC64_FORCED_BOGUS_ARRAY */
            (NDR64_UINT8) 7 /* 0x7 */,
            { 
            /* *_SHARE_INFO_501 */
                0,
                1,
                0,
                0,
                0,
                0,
                0,
                0
            },
            (NDR64_UINT8) 1 /* 0x1 */,
            (NDR64_UINT32) 0 /* 0x0 */,
            &__midl_frag237
        },
        &__midl_frag915,
        0,
        0
    }
};

static const __midl_frag279_t __midl_frag279 =
{ 
/* SHARE_INFO_501_CONTAINER */
    { 
    /* SHARE_INFO_501_CONTAINER */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* SHARE_INFO_501_CONTAINER */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */,
        0,
        0,
        &__midl_frag282,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag278_t __midl_frag278 =
{ 
/* *SHARE_INFO_501_CONTAINER */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag279
};

static const __midl_frag277_t __midl_frag277 =
{ 
/*  */
    { 
    /* *_SHARE_INFO_2 */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 32 /* 0x20 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag275
    }
};

static const __midl_frag275_t __midl_frag275 =
{ 
/* *_SHARE_INFO_2 */
    { 
    /* *_SHARE_INFO_2 */
        { 
        /* *_SHARE_INFO_2 */
            0x46,    /* FC64_FORCED_BOGUS_ARRAY */
            (NDR64_UINT8) 7 /* 0x7 */,
            { 
            /* *_SHARE_INFO_2 */
                0,
                1,
                0,
                0,
                0,
                0,
                0,
                0
            },
            (NDR64_UINT8) 1 /* 0x1 */,
            (NDR64_UINT32) 0 /* 0x0 */,
            &__midl_frag207
        },
        &__midl_frag915,
        0,
        0
    }
};

static const __midl_frag274_t __midl_frag274 =
{ 
/* SHARE_INFO_2_CONTAINER */
    { 
    /* SHARE_INFO_2_CONTAINER */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* SHARE_INFO_2_CONTAINER */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */,
        0,
        0,
        &__midl_frag277,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag273_t __midl_frag273 =
{ 
/* *SHARE_INFO_2_CONTAINER */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag274
};

static const __midl_frag272_t __midl_frag272 =
{ 
/*  */
    { 
    /* *_SHARE_INFO_1 */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 32 /* 0x20 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag270
    }
};

static const __midl_frag270_t __midl_frag270 =
{ 
/* *_SHARE_INFO_1 */
    { 
    /* *_SHARE_INFO_1 */
        { 
        /* *_SHARE_INFO_1 */
            0x46,    /* FC64_FORCED_BOGUS_ARRAY */
            (NDR64_UINT8) 7 /* 0x7 */,
            { 
            /* *_SHARE_INFO_1 */
                0,
                1,
                0,
                0,
                0,
                0,
                0,
                0
            },
            (NDR64_UINT8) 1 /* 0x1 */,
            (NDR64_UINT32) 0 /* 0x0 */,
            &__midl_frag202
        },
        &__midl_frag915,
        0,
        0
    }
};

static const __midl_frag269_t __midl_frag269 =
{ 
/* SHARE_INFO_1_CONTAINER */
    { 
    /* SHARE_INFO_1_CONTAINER */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* SHARE_INFO_1_CONTAINER */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */,
        0,
        0,
        &__midl_frag272,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag268_t __midl_frag268 =
{ 
/* *SHARE_INFO_1_CONTAINER */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag269
};

static const __midl_frag267_t __midl_frag267 =
{ 
/*  */
    { 
    /* *_SHARE_INFO_0 */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 32 /* 0x20 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag264
    }
};

static const __midl_frag264_t __midl_frag264 =
{ 
/* *_SHARE_INFO_0 */
    { 
    /* *_SHARE_INFO_0 */
        0x41,    /* FC64_CONF_ARRAY */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* *_SHARE_INFO_0 */
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        &__midl_frag915
    },
    { 
    /*  */
        { 
        /* struct _NDR64_REPEAT_FORMAT */
            0x82,    /* FC64_VARIABLE_REPEAT */
            { 
            /* struct _NDR64_REPEAT_FORMAT */
                (NDR64_UINT8) 1 /* 0x1 */,
                (NDR64_UINT8) 0 /* 0x0 */
            },
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 8 /* 0x8 */,
            (NDR64_UINT32) 0 /* 0x0 */,
            (NDR64_UINT32) 1 /* 0x1 */
        },
        { 
        /*  */
            { 
            /* struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT */
                (NDR64_UINT32) 0 /* 0x0 */,
                (NDR64_UINT32) 0 /* 0x0 */
            },
            { 
            /* *wchar_t */
                0x21,    /* FC64_UP */
                (NDR64_UINT8) 0 /* 0x0 */,
                (NDR64_UINT16) 0 /* 0x0 */,
                &__midl_frag934
            }
        },
        0x93    /* FC64_END */
    },
    { 
    /* struct _NDR64_ARRAY_ELEMENT_INFO */
        (NDR64_UINT32) 8 /* 0x8 */,
        &__midl_frag400
    }
};

static const __midl_frag263_t __midl_frag263 =
{ 
/* SHARE_INFO_0_CONTAINER */
    { 
    /* SHARE_INFO_0_CONTAINER */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* SHARE_INFO_0_CONTAINER */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */,
        0,
        0,
        &__midl_frag267,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag262_t __midl_frag262 =
{ 
/* *SHARE_INFO_0_CONTAINER */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag263
};

static const __midl_frag260_t __midl_frag260 =
{ 
/* SHARE_ENUM_UNION */
    { 
    /* SHARE_ENUM_UNION */
        0x51,    /* FC64_NON_ENCAPSULATED_UNION */
        (NDR64_UINT8) 7 /* 0x7 */,
        (NDR64_UINT8) 0 /* 0x0 */,
        0x6,    /* FC64_UINT32 */
        (NDR64_UINT32) 8 /* 0x8 */,
        &__midl_frag915,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM_SELECTOR */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT8) 7 /* 0x7 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT32) 6 /* 0x6 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 0 /* 0x0 */,
        &__midl_frag262,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1 /* 0x1 */,
        &__midl_frag268,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 2 /* 0x2 */,
        &__midl_frag273,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 501 /* 0x1f5 */,
        &__midl_frag278,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 502 /* 0x1f6 */,
        &__midl_frag283,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 503 /* 0x1f7 */,
        &__midl_frag288,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    (NDR64_UINT32) 4294967295 /* 0xffffffff */
};

static const __midl_frag259_t __midl_frag259 =
{ 
/* _SHARE_ENUM_STRUCT */
    { 
    /* _SHARE_ENUM_STRUCT */
        0x34,    /* FC64_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _SHARE_ENUM_STRUCT */
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */,
        0,
        0,
        0,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_EMBEDDED_COMPLEX_FORMAT */
            0x91,    /* FC64_EMBEDDED_COMPLEX */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            &__midl_frag260
        },
        { 
        /* _SHARE_ENUM_STRUCT */
            0x92,    /* FC64_BUFFER_ALIGN */
            (NDR64_UINT8) 7 /* 0x7 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag251_t __midl_frag251 =
{ 
/*  */
    { 
    /* *WCHAR */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag934
    },
    { 
    /* *WCHAR */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag934
    },
    { 
    /* *WCHAR */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag934
    },
    { 
    /* *WCHAR */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag934
    },
    { 
    /* *WCHAR */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag934
    },
    { 
    /* *char */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 32 /* 0x20 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag248
    }
};

static const __midl_frag249_t __midl_frag249 =
{ 
/*  */
    (NDR64_UINT32) 1 /* 0x1 */,
    { 
    /* struct _NDR64_EXPR_VAR */
        0x3,    /* FC_EXPR_VAR */
        0x6,    /* FC64_UINT32 */
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT32) 64 /* 0x40 */
    }
};

static const __midl_frag248_t __midl_frag248 =
{ 
/* *char */
    { 
    /* *char */
        0x41,    /* FC64_CONF_ARRAY */
        (NDR64_UINT8) 0 /* 0x0 */,
        { 
        /* *char */
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 1 /* 0x1 */,
        &__midl_frag249
    },
    { 
    /* struct _NDR64_ARRAY_ELEMENT_INFO */
        (NDR64_UINT32) 1 /* 0x1 */,
        &__midl_frag766
    }
};

static const __midl_frag242_t __midl_frag242 =
{ 
/* _SHARE_INFO_503_I */
    { 
    /* _SHARE_INFO_503_I */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _SHARE_INFO_503_I */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 80 /* 0x50 */,
        0,
        0,
        &__midl_frag251,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag237_t __midl_frag237 =
{ 
/* _SHARE_INFO_501 */
    { 
    /* _SHARE_INFO_501 */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _SHARE_INFO_501 */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 32 /* 0x20 */,
        0,
        0,
        &__midl_frag903,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* _SHARE_INFO_501 */
            0x92,    /* FC64_BUFFER_ALIGN */
            (NDR64_UINT8) 7 /* 0x7 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag222_t __midl_frag222 =
{ 
/*  */
    { 
    /* *WCHAR */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag934
    },
    { 
    /* *WCHAR */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag934
    },
    { 
    /* *WCHAR */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag934
    },
    { 
    /* *WCHAR */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag934
    },
    { 
    /* *char */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 32 /* 0x20 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag219
    }
};

static const __midl_frag220_t __midl_frag220 =
{ 
/*  */
    (NDR64_UINT32) 1 /* 0x1 */,
    { 
    /* struct _NDR64_EXPR_VAR */
        0x3,    /* FC_EXPR_VAR */
        0x6,    /* FC64_UINT32 */
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT32) 56 /* 0x38 */
    }
};

static const __midl_frag219_t __midl_frag219 =
{ 
/* *char */
    { 
    /* *char */
        0x41,    /* FC64_CONF_ARRAY */
        (NDR64_UINT8) 0 /* 0x0 */,
        { 
        /* *char */
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 1 /* 0x1 */,
        &__midl_frag220
    },
    { 
    /* struct _NDR64_ARRAY_ELEMENT_INFO */
        (NDR64_UINT32) 1 /* 0x1 */,
        &__midl_frag766
    }
};

static const __midl_frag214_t __midl_frag214 =
{ 
/* _SHARE_INFO_502_I */
    { 
    /* _SHARE_INFO_502_I */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _SHARE_INFO_502_I */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 72 /* 0x48 */,
        0,
        0,
        &__midl_frag222,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag212_t __midl_frag212 =
{ 
/*  */
    { 
    /* *wchar_t */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag934
    },
    { 
    /* *wchar_t */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag934
    },
    { 
    /* *wchar_t */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag934
    },
    { 
    /* *wchar_t */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag934
    }
};

static const __midl_frag207_t __midl_frag207 =
{ 
/* _SHARE_INFO_2 */
    { 
    /* _SHARE_INFO_2 */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _SHARE_INFO_2 */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 56 /* 0x38 */,
        0,
        0,
        &__midl_frag212,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag202_t __midl_frag202 =
{ 
/* _SHARE_INFO_1 */
    { 
    /* _SHARE_INFO_1 */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _SHARE_INFO_1 */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 24 /* 0x18 */,
        0,
        0,
        &__midl_frag903,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag191_t __midl_frag191 =
{ 
/* NetrShareAdd */
    { 
    /* NetrShareAdd */      /* procedure NetrShareAdd */
        (NDR64_UINT32) 19660864 /* 0x12c0040 */,    /* explicit handle */ /* IsIntrepreted, ClientMustSize, HasReturn, ServerCorrelation, HasExtensions */
        (NDR64_UINT32) 40 /* 0x28 */ ,  /* Stack size */
        (NDR64_UINT32) 40 /* 0x28 */,
        (NDR64_UINT32) 40 /* 0x28 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 5 /* 0x5 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* Level */      /* parameter Level */
        &__midl_frag949,
        { 
        /* Level */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* InfoStruct */      /* parameter InfoStruct */
        &__midl_frag937,
        { 
        /* InfoStruct */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* ParmErr */      /* parameter ParmErr */
        &__midl_frag920,
        { 
        /* ParmErr */
            0,
            1,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustFree, [in], [out] */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    }
};

static const __midl_frag183_t __midl_frag183 =
{ 
/* NetrSessionDel */
    { 
    /* NetrSessionDel */      /* procedure NetrSessionDel */
        (NDR64_UINT32) 17563712 /* 0x10c0040 */,    /* explicit handle */ /* IsIntrepreted, ClientMustSize, HasReturn, HasExtensions */
        (NDR64_UINT32) 32 /* 0x20 */ ,  /* Stack size */
        (NDR64_UINT32) 0 /* 0x0 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 4 /* 0x4 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* ClientName */      /* parameter ClientName */
        &__midl_frag933,
        { 
        /* ClientName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* UserName */      /* parameter UserName */
        &__midl_frag933,
        { 
        /* UserName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    }
};

static const __midl_frag176_t __midl_frag176 =
{ 
/*  */
    { 
    /* *_SESSION_INFO_502 */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 32 /* 0x20 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag165
    }
};

static const __midl_frag171_t __midl_frag171 =
{ 
/* _SESSION_INFO_502 */
    { 
    /* _SESSION_INFO_502 */
        0x31,    /* FC64_PSTRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _SESSION_INFO_502 */
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 48 /* 0x30 */
    },
    { 
    /*  */
        { 
        /* struct _NDR64_NO_REPEAT_FORMAT */
            0x80,    /* FC64_NO_REPEAT */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT */
            (NDR64_UINT32) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* *wchar_t */
            0x21,    /* FC64_UP */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            &__midl_frag934
        },
        { 
        /* struct _NDR64_NO_REPEAT_FORMAT */
            0x80,    /* FC64_NO_REPEAT */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT */
            (NDR64_UINT32) 8 /* 0x8 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* *wchar_t */
            0x21,    /* FC64_UP */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            &__midl_frag934
        },
        { 
        /* struct _NDR64_NO_REPEAT_FORMAT */
            0x80,    /* FC64_NO_REPEAT */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT */
            (NDR64_UINT32) 32 /* 0x20 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* *wchar_t */
            0x21,    /* FC64_UP */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            &__midl_frag934
        },
        { 
        /* struct _NDR64_NO_REPEAT_FORMAT */
            0x80,    /* FC64_NO_REPEAT */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT */
            (NDR64_UINT32) 40 /* 0x28 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* *wchar_t */
            0x21,    /* FC64_UP */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            &__midl_frag934
        },
        0x93    /* FC64_END */
    }
};

static const __midl_frag165_t __midl_frag165 =
{ 
/* *_SESSION_INFO_502 */
    { 
    /* *_SESSION_INFO_502 */
        0x41,    /* FC64_CONF_ARRAY */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* *_SESSION_INFO_502 */
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 48 /* 0x30 */,
        &__midl_frag915
    },
    { 
    /*  */
        { 
        /* struct _NDR64_REPEAT_FORMAT */
            0x82,    /* FC64_VARIABLE_REPEAT */
            { 
            /* struct _NDR64_REPEAT_FORMAT */
                (NDR64_UINT8) 1 /* 0x1 */,
                (NDR64_UINT8) 0 /* 0x0 */
            },
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 48 /* 0x30 */,
            (NDR64_UINT32) 0 /* 0x0 */,
            (NDR64_UINT32) 4 /* 0x4 */
        },
        { 
        /*  */
            { 
            /* struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT */
                (NDR64_UINT32) 0 /* 0x0 */,
                (NDR64_UINT32) 0 /* 0x0 */
            },
            { 
            /* *wchar_t */
                0x21,    /* FC64_UP */
                (NDR64_UINT8) 0 /* 0x0 */,
                (NDR64_UINT16) 0 /* 0x0 */,
                &__midl_frag934
            },
            { 
            /* struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT */
                (NDR64_UINT32) 8 /* 0x8 */,
                (NDR64_UINT32) 0 /* 0x0 */
            },
            { 
            /* *wchar_t */
                0x21,    /* FC64_UP */
                (NDR64_UINT8) 0 /* 0x0 */,
                (NDR64_UINT16) 0 /* 0x0 */,
                &__midl_frag934
            },
            { 
            /* struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT */
                (NDR64_UINT32) 32 /* 0x20 */,
                (NDR64_UINT32) 0 /* 0x0 */
            },
            { 
            /* *wchar_t */
                0x21,    /* FC64_UP */
                (NDR64_UINT8) 0 /* 0x0 */,
                (NDR64_UINT16) 0 /* 0x0 */,
                &__midl_frag934
            },
            { 
            /* struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT */
                (NDR64_UINT32) 40 /* 0x28 */,
                (NDR64_UINT32) 0 /* 0x0 */
            },
            { 
            /* *wchar_t */
                0x21,    /* FC64_UP */
                (NDR64_UINT8) 0 /* 0x0 */,
                (NDR64_UINT16) 0 /* 0x0 */,
                &__midl_frag934
            }
        },
        0x93    /* FC64_END */
    },
    { 
    /* struct _NDR64_ARRAY_ELEMENT_INFO */
        (NDR64_UINT32) 48 /* 0x30 */,
        &__midl_frag171
    }
};

static const __midl_frag164_t __midl_frag164 =
{ 
/* SESSION_INFO_502_CONTAINER */
    { 
    /* SESSION_INFO_502_CONTAINER */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* SESSION_INFO_502_CONTAINER */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */,
        0,
        0,
        &__midl_frag176,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag163_t __midl_frag163 =
{ 
/* *SESSION_INFO_502_CONTAINER */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag164
};

static const __midl_frag162_t __midl_frag162 =
{ 
/*  */
    { 
    /* *_SESSION_INFO_10 */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 32 /* 0x20 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag155
    }
};

static const __midl_frag159_t __midl_frag159 =
{ 
/* _SESSION_INFO_10 */
    { 
    /* _SESSION_INFO_10 */
        0x31,    /* FC64_PSTRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _SESSION_INFO_10 */
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 24 /* 0x18 */
    },
    { 
    /*  */
        { 
        /* struct _NDR64_NO_REPEAT_FORMAT */
            0x80,    /* FC64_NO_REPEAT */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT */
            (NDR64_UINT32) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* *wchar_t */
            0x21,    /* FC64_UP */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            &__midl_frag934
        },
        { 
        /* struct _NDR64_NO_REPEAT_FORMAT */
            0x80,    /* FC64_NO_REPEAT */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT */
            (NDR64_UINT32) 8 /* 0x8 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* *wchar_t */
            0x21,    /* FC64_UP */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            &__midl_frag934
        },
        0x93    /* FC64_END */
    }
};

static const __midl_frag155_t __midl_frag155 =
{ 
/* *_SESSION_INFO_10 */
    { 
    /* *_SESSION_INFO_10 */
        0x41,    /* FC64_CONF_ARRAY */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* *_SESSION_INFO_10 */
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 24 /* 0x18 */,
        &__midl_frag915
    },
    { 
    /*  */
        { 
        /* struct _NDR64_REPEAT_FORMAT */
            0x82,    /* FC64_VARIABLE_REPEAT */
            { 
            /* struct _NDR64_REPEAT_FORMAT */
                (NDR64_UINT8) 1 /* 0x1 */,
                (NDR64_UINT8) 0 /* 0x0 */
            },
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 24 /* 0x18 */,
            (NDR64_UINT32) 0 /* 0x0 */,
            (NDR64_UINT32) 2 /* 0x2 */
        },
        { 
        /*  */
            { 
            /* struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT */
                (NDR64_UINT32) 0 /* 0x0 */,
                (NDR64_UINT32) 0 /* 0x0 */
            },
            { 
            /* *wchar_t */
                0x21,    /* FC64_UP */
                (NDR64_UINT8) 0 /* 0x0 */,
                (NDR64_UINT16) 0 /* 0x0 */,
                &__midl_frag934
            },
            { 
            /* struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT */
                (NDR64_UINT32) 8 /* 0x8 */,
                (NDR64_UINT32) 0 /* 0x0 */
            },
            { 
            /* *wchar_t */
                0x21,    /* FC64_UP */
                (NDR64_UINT8) 0 /* 0x0 */,
                (NDR64_UINT16) 0 /* 0x0 */,
                &__midl_frag934
            }
        },
        0x93    /* FC64_END */
    },
    { 
    /* struct _NDR64_ARRAY_ELEMENT_INFO */
        (NDR64_UINT32) 24 /* 0x18 */,
        &__midl_frag159
    }
};

static const __midl_frag154_t __midl_frag154 =
{ 
/* SESSION_INFO_10_CONTAINER */
    { 
    /* SESSION_INFO_10_CONTAINER */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* SESSION_INFO_10_CONTAINER */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */,
        0,
        0,
        &__midl_frag162,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag153_t __midl_frag153 =
{ 
/* *SESSION_INFO_10_CONTAINER */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag154
};

static const __midl_frag152_t __midl_frag152 =
{ 
/*  */
    { 
    /* *_SESSION_INFO_2 */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 32 /* 0x20 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag143
    }
};

static const __midl_frag148_t __midl_frag148 =
{ 
/* _SESSION_INFO_2 */
    { 
    /* _SESSION_INFO_2 */
        0x31,    /* FC64_PSTRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _SESSION_INFO_2 */
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 40 /* 0x28 */
    },
    { 
    /*  */
        { 
        /* struct _NDR64_NO_REPEAT_FORMAT */
            0x80,    /* FC64_NO_REPEAT */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT */
            (NDR64_UINT32) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* *wchar_t */
            0x21,    /* FC64_UP */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            &__midl_frag934
        },
        { 
        /* struct _NDR64_NO_REPEAT_FORMAT */
            0x80,    /* FC64_NO_REPEAT */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT */
            (NDR64_UINT32) 8 /* 0x8 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* *wchar_t */
            0x21,    /* FC64_UP */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            &__midl_frag934
        },
        { 
        /* struct _NDR64_NO_REPEAT_FORMAT */
            0x80,    /* FC64_NO_REPEAT */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT */
            (NDR64_UINT32) 32 /* 0x20 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* *wchar_t */
            0x21,    /* FC64_UP */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            &__midl_frag934
        },
        0x93    /* FC64_END */
    }
};

static const __midl_frag143_t __midl_frag143 =
{ 
/* *_SESSION_INFO_2 */
    { 
    /* *_SESSION_INFO_2 */
        0x41,    /* FC64_CONF_ARRAY */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* *_SESSION_INFO_2 */
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 40 /* 0x28 */,
        &__midl_frag915
    },
    { 
    /*  */
        { 
        /* struct _NDR64_REPEAT_FORMAT */
            0x82,    /* FC64_VARIABLE_REPEAT */
            { 
            /* struct _NDR64_REPEAT_FORMAT */
                (NDR64_UINT8) 1 /* 0x1 */,
                (NDR64_UINT8) 0 /* 0x0 */
            },
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 40 /* 0x28 */,
            (NDR64_UINT32) 0 /* 0x0 */,
            (NDR64_UINT32) 3 /* 0x3 */
        },
        { 
        /*  */
            { 
            /* struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT */
                (NDR64_UINT32) 0 /* 0x0 */,
                (NDR64_UINT32) 0 /* 0x0 */
            },
            { 
            /* *wchar_t */
                0x21,    /* FC64_UP */
                (NDR64_UINT8) 0 /* 0x0 */,
                (NDR64_UINT16) 0 /* 0x0 */,
                &__midl_frag934
            },
            { 
            /* struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT */
                (NDR64_UINT32) 8 /* 0x8 */,
                (NDR64_UINT32) 0 /* 0x0 */
            },
            { 
            /* *wchar_t */
                0x21,    /* FC64_UP */
                (NDR64_UINT8) 0 /* 0x0 */,
                (NDR64_UINT16) 0 /* 0x0 */,
                &__midl_frag934
            },
            { 
            /* struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT */
                (NDR64_UINT32) 32 /* 0x20 */,
                (NDR64_UINT32) 0 /* 0x0 */
            },
            { 
            /* *wchar_t */
                0x21,    /* FC64_UP */
                (NDR64_UINT8) 0 /* 0x0 */,
                (NDR64_UINT16) 0 /* 0x0 */,
                &__midl_frag934
            }
        },
        0x93    /* FC64_END */
    },
    { 
    /* struct _NDR64_ARRAY_ELEMENT_INFO */
        (NDR64_UINT32) 40 /* 0x28 */,
        &__midl_frag148
    }
};

static const __midl_frag142_t __midl_frag142 =
{ 
/* SESSION_INFO_2_CONTAINER */
    { 
    /* SESSION_INFO_2_CONTAINER */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* SESSION_INFO_2_CONTAINER */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */,
        0,
        0,
        &__midl_frag152,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag141_t __midl_frag141 =
{ 
/* *SESSION_INFO_2_CONTAINER */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag142
};

static const __midl_frag140_t __midl_frag140 =
{ 
/*  */
    { 
    /* *_SESSION_INFO_1 */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 32 /* 0x20 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag133
    }
};

static const __midl_frag137_t __midl_frag137 =
{ 
/* _SESSION_INFO_1 */
    { 
    /* _SESSION_INFO_1 */
        0x31,    /* FC64_PSTRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _SESSION_INFO_1 */
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 32 /* 0x20 */
    },
    { 
    /*  */
        { 
        /* struct _NDR64_NO_REPEAT_FORMAT */
            0x80,    /* FC64_NO_REPEAT */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT */
            (NDR64_UINT32) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* *wchar_t */
            0x21,    /* FC64_UP */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            &__midl_frag934
        },
        { 
        /* struct _NDR64_NO_REPEAT_FORMAT */
            0x80,    /* FC64_NO_REPEAT */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT */
            (NDR64_UINT32) 8 /* 0x8 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* *wchar_t */
            0x21,    /* FC64_UP */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            &__midl_frag934
        },
        0x93    /* FC64_END */
    }
};

static const __midl_frag133_t __midl_frag133 =
{ 
/* *_SESSION_INFO_1 */
    { 
    /* *_SESSION_INFO_1 */
        0x41,    /* FC64_CONF_ARRAY */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* *_SESSION_INFO_1 */
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 32 /* 0x20 */,
        &__midl_frag915
    },
    { 
    /*  */
        { 
        /* struct _NDR64_REPEAT_FORMAT */
            0x82,    /* FC64_VARIABLE_REPEAT */
            { 
            /* struct _NDR64_REPEAT_FORMAT */
                (NDR64_UINT8) 1 /* 0x1 */,
                (NDR64_UINT8) 0 /* 0x0 */
            },
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 32 /* 0x20 */,
            (NDR64_UINT32) 0 /* 0x0 */,
            (NDR64_UINT32) 2 /* 0x2 */
        },
        { 
        /*  */
            { 
            /* struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT */
                (NDR64_UINT32) 0 /* 0x0 */,
                (NDR64_UINT32) 0 /* 0x0 */
            },
            { 
            /* *wchar_t */
                0x21,    /* FC64_UP */
                (NDR64_UINT8) 0 /* 0x0 */,
                (NDR64_UINT16) 0 /* 0x0 */,
                &__midl_frag934
            },
            { 
            /* struct _NDR64_POINTER_INSTANCE_HEADER_FORMAT */
                (NDR64_UINT32) 8 /* 0x8 */,
                (NDR64_UINT32) 0 /* 0x0 */
            },
            { 
            /* *wchar_t */
                0x21,    /* FC64_UP */
                (NDR64_UINT8) 0 /* 0x0 */,
                (NDR64_UINT16) 0 /* 0x0 */,
                &__midl_frag934
            }
        },
        0x93    /* FC64_END */
    },
    { 
    /* struct _NDR64_ARRAY_ELEMENT_INFO */
        (NDR64_UINT32) 32 /* 0x20 */,
        &__midl_frag137
    }
};

static const __midl_frag132_t __midl_frag132 =
{ 
/* SESSION_INFO_1_CONTAINER */
    { 
    /* SESSION_INFO_1_CONTAINER */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* SESSION_INFO_1_CONTAINER */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */,
        0,
        0,
        &__midl_frag140,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag131_t __midl_frag131 =
{ 
/* *SESSION_INFO_1_CONTAINER */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag132
};

static const __midl_frag121_t __midl_frag121 =
{ 
/* SESSION_ENUM_UNION */
    { 
    /* SESSION_ENUM_UNION */
        0x51,    /* FC64_NON_ENCAPSULATED_UNION */
        (NDR64_UINT8) 7 /* 0x7 */,
        (NDR64_UINT8) 0 /* 0x0 */,
        0x6,    /* FC64_UINT32 */
        (NDR64_UINT32) 8 /* 0x8 */,
        &__midl_frag915,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM_SELECTOR */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT8) 7 /* 0x7 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT32) 5 /* 0x5 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 0 /* 0x0 */,
        &__midl_frag262,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1 /* 0x1 */,
        &__midl_frag131,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 2 /* 0x2 */,
        &__midl_frag141,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 10 /* 0xa */,
        &__midl_frag153,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 502 /* 0x1f6 */,
        &__midl_frag163,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    (NDR64_UINT32) 4294967295 /* 0xffffffff */
};

static const __midl_frag120_t __midl_frag120 =
{ 
/* _SESSION_ENUM_STRUCT */
    { 
    /* _SESSION_ENUM_STRUCT */
        0x34,    /* FC64_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _SESSION_ENUM_STRUCT */
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */,
        0,
        0,
        0,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_EMBEDDED_COMPLEX_FORMAT */
            0x91,    /* FC64_EMBEDDED_COMPLEX */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            &__midl_frag121
        },
        { 
        /* _SESSION_ENUM_STRUCT */
            0x92,    /* FC64_BUFFER_ALIGN */
            (NDR64_UINT8) 7 /* 0x7 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag119_t __midl_frag119 =
{ 
/* *_SESSION_ENUM_STRUCT */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag120
};

static const __midl_frag112_t __midl_frag112 =
{ 
/* NetrSessionEnum */
    { 
    /* NetrSessionEnum */      /* procedure NetrSessionEnum */
        (NDR64_UINT32) 23986240 /* 0x16e0040 */,    /* explicit handle */ /* IsIntrepreted, ServerMustSize, ClientMustSize, HasReturn, ServerCorrelation, ClientCorrelation, HasExtensions */
        (NDR64_UINT32) 64 /* 0x40 */ ,  /* Stack size */
        (NDR64_UINT32) 40 /* 0x28 */,
        (NDR64_UINT32) 72 /* 0x48 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 8 /* 0x8 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* ClientName */      /* parameter ClientName */
        &__midl_frag933,
        { 
        /* ClientName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* UserName */      /* parameter UserName */
        &__midl_frag933,
        { 
        /* UserName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* InfoStruct */      /* parameter InfoStruct */
        &__midl_frag120,
        { 
        /* InfoStruct */
            1,
            1,
            0,
            1,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], [out], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* PreferedMaximumLength */      /* parameter PreferedMaximumLength */
        &__midl_frag949,
        { 
        /* PreferedMaximumLength */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    },
    { 
    /* TotalEntries */      /* parameter TotalEntries */
        &__midl_frag949,
        { 
        /* TotalEntries */
            0,
            0,
            0,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            1
        },    /* [out], Basetype, SimpleRef, UseCache */
        (NDR64_UINT16) 0 /* 0x0 */,
        40 /* 0x28 */,   /* Stack offset */
    },
    { 
    /* ResumeHandle */      /* parameter ResumeHandle */
        &__midl_frag920,
        { 
        /* ResumeHandle */
            0,
            1,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustFree, [in], [out] */
        (NDR64_UINT16) 0 /* 0x0 */,
        48 /* 0x30 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        56 /* 0x38 */,   /* Stack offset */
    }
};

static const __midl_frag107_t __midl_frag107 =
{ 
/* NetrFileClose */
    { 
    /* NetrFileClose */      /* procedure NetrFileClose */
        (NDR64_UINT32) 17563712 /* 0x10c0040 */,    /* explicit handle */ /* IsIntrepreted, ClientMustSize, HasReturn, HasExtensions */
        (NDR64_UINT32) 24 /* 0x18 */ ,  /* Stack size */
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 3 /* 0x3 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* FileId */      /* parameter FileId */
        &__midl_frag949,
        { 
        /* FileId */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    }
};

static const __midl_frag105_t __midl_frag105 =
{ 
/* *_FILE_INFO_3 */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag85
};

static const __midl_frag102_t __midl_frag102 =
{ 
/* _FILE_INFO */
    { 
    /* _FILE_INFO */
        0x51,    /* FC64_NON_ENCAPSULATED_UNION */
        (NDR64_UINT8) 7 /* 0x7 */,
        (NDR64_UINT8) 0 /* 0x0 */,
        0x6,    /* FC64_UINT32 */
        (NDR64_UINT32) 8 /* 0x8 */,
        &__midl_frag327,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM_SELECTOR */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT8) 7 /* 0x7 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT32) 2 /* 0x2 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 2 /* 0x2 */,
        &__midl_frag946,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 3 /* 0x3 */,
        &__midl_frag105,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    (NDR64_UINT32) 4294967295 /* 0xffffffff */
};

static const __midl_frag101_t __midl_frag101 =
{ 
/* *_FILE_INFO */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 4 /* 0x4 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag102
};

static const __midl_frag96_t __midl_frag96 =
{ 
/* NetrFileGetInfo */
    { 
    /* NetrFileGetInfo */      /* procedure NetrFileGetInfo */
        (NDR64_UINT32) 21889088 /* 0x14e0040 */,    /* explicit handle */ /* IsIntrepreted, ServerMustSize, ClientMustSize, HasReturn, ClientCorrelation, HasExtensions */
        (NDR64_UINT32) 40 /* 0x28 */ ,  /* Stack size */
        (NDR64_UINT32) 16 /* 0x10 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 5 /* 0x5 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* FileId */      /* parameter FileId */
        &__midl_frag949,
        { 
        /* FileId */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* Level */      /* parameter Level */
        &__midl_frag949,
        { 
        /* Level */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* InfoStruct */      /* parameter InfoStruct */
        &__midl_frag102,
        { 
        /* InfoStruct */
            1,
            1,
            0,
            0,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            1
        },    /* MustSize, MustFree, [out], SimpleRef, UseCache */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    }
};

static const __midl_frag89_t __midl_frag89 =
{ 
/*  */
    { 
    /* *_FILE_INFO_3 */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 32 /* 0x20 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag83
    }
};

static const __midl_frag85_t __midl_frag85 =
{ 
/* _FILE_INFO_3 */
    { 
    /* _FILE_INFO_3 */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _FILE_INFO_3 */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 32 /* 0x20 */,
        0,
        0,
        &__midl_frag903,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag83_t __midl_frag83 =
{ 
/* *_FILE_INFO_3 */
    { 
    /* *_FILE_INFO_3 */
        { 
        /* *_FILE_INFO_3 */
            0x46,    /* FC64_FORCED_BOGUS_ARRAY */
            (NDR64_UINT8) 7 /* 0x7 */,
            { 
            /* *_FILE_INFO_3 */
                0,
                1,
                0,
                0,
                0,
                0,
                0,
                0
            },
            (NDR64_UINT8) 1 /* 0x1 */,
            (NDR64_UINT32) 0 /* 0x0 */,
            &__midl_frag85
        },
        &__midl_frag915,
        0,
        0
    }
};

static const __midl_frag82_t __midl_frag82 =
{ 
/* FILE_INFO_3_CONTAINER */
    { 
    /* FILE_INFO_3_CONTAINER */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* FILE_INFO_3_CONTAINER */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */,
        0,
        0,
        &__midl_frag89,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag81_t __midl_frag81 =
{ 
/* *FILE_INFO_3_CONTAINER */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag82
};

static const __midl_frag80_t __midl_frag80 =
{ 
/*  */
    { 
    /* *_FILE_INFO_2 */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 32 /* 0x20 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag77
    }
};

static const __midl_frag77_t __midl_frag77 =
{ 
/* *_FILE_INFO_2 */
    { 
    /* *_FILE_INFO_2 */
        0x41,    /* FC64_CONF_ARRAY */
        (NDR64_UINT8) 3 /* 0x3 */,
        { 
        /* *_FILE_INFO_2 */
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 4 /* 0x4 */,
        &__midl_frag915
    },
    { 
    /* struct _NDR64_ARRAY_ELEMENT_INFO */
        (NDR64_UINT32) 4 /* 0x4 */,
        &__midl_frag487
    }
};

static const __midl_frag76_t __midl_frag76 =
{ 
/* FILE_INFO_2_CONTAINER */
    { 
    /* FILE_INFO_2_CONTAINER */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* FILE_INFO_2_CONTAINER */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */,
        0,
        0,
        &__midl_frag80,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag75_t __midl_frag75 =
{ 
/* *FILE_INFO_2_CONTAINER */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag76
};

static const __midl_frag73_t __midl_frag73 =
{ 
/* FILE_ENUM_UNION */
    { 
    /* FILE_ENUM_UNION */
        0x51,    /* FC64_NON_ENCAPSULATED_UNION */
        (NDR64_UINT8) 7 /* 0x7 */,
        (NDR64_UINT8) 0 /* 0x0 */,
        0x6,    /* FC64_UINT32 */
        (NDR64_UINT32) 8 /* 0x8 */,
        &__midl_frag915,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM_SELECTOR */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT8) 7 /* 0x7 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT32) 2 /* 0x2 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 2 /* 0x2 */,
        &__midl_frag75,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 3 /* 0x3 */,
        &__midl_frag81,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    (NDR64_UINT32) 4294967295 /* 0xffffffff */
};

static const __midl_frag72_t __midl_frag72 =
{ 
/* _FILE_ENUM_STRUCT */
    { 
    /* _FILE_ENUM_STRUCT */
        0x34,    /* FC64_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _FILE_ENUM_STRUCT */
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */,
        0,
        0,
        0,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_EMBEDDED_COMPLEX_FORMAT */
            0x91,    /* FC64_EMBEDDED_COMPLEX */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            &__midl_frag73
        },
        { 
        /* _FILE_ENUM_STRUCT */
            0x92,    /* FC64_BUFFER_ALIGN */
            (NDR64_UINT8) 7 /* 0x7 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag71_t __midl_frag71 =
{ 
/* *_FILE_ENUM_STRUCT */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag72
};

static const __midl_frag64_t __midl_frag64 =
{ 
/* NetrFileEnum */
    { 
    /* NetrFileEnum */      /* procedure NetrFileEnum */
        (NDR64_UINT32) 23986240 /* 0x16e0040 */,    /* explicit handle */ /* IsIntrepreted, ServerMustSize, ClientMustSize, HasReturn, ServerCorrelation, ClientCorrelation, HasExtensions */
        (NDR64_UINT32) 64 /* 0x40 */ ,  /* Stack size */
        (NDR64_UINT32) 40 /* 0x28 */,
        (NDR64_UINT32) 72 /* 0x48 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 8 /* 0x8 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* BasePath */      /* parameter BasePath */
        &__midl_frag933,
        { 
        /* BasePath */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* UserName */      /* parameter UserName */
        &__midl_frag933,
        { 
        /* UserName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* InfoStruct */      /* parameter InfoStruct */
        &__midl_frag72,
        { 
        /* InfoStruct */
            1,
            1,
            0,
            1,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], [out], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* PreferedMaximumLength */      /* parameter PreferedMaximumLength */
        &__midl_frag949,
        { 
        /* PreferedMaximumLength */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    },
    { 
    /* TotalEntries */      /* parameter TotalEntries */
        &__midl_frag949,
        { 
        /* TotalEntries */
            0,
            0,
            0,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            1
        },    /* [out], Basetype, SimpleRef, UseCache */
        (NDR64_UINT16) 0 /* 0x0 */,
        40 /* 0x28 */,   /* Stack offset */
    },
    { 
    /* ResumeHandle */      /* parameter ResumeHandle */
        &__midl_frag920,
        { 
        /* ResumeHandle */
            0,
            1,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustFree, [in], [out] */
        (NDR64_UINT16) 0 /* 0x0 */,
        48 /* 0x30 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        56 /* 0x38 */,   /* Stack offset */
    }
};

static const __midl_frag57_t __midl_frag57 =
{ 
/*  */
    { 
    /* *_CONNECTION_INFO_1 */
        0x21,    /* FC64_UP */
        (NDR64_UINT8) 32 /* 0x20 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        &__midl_frag51
    }
};

static const __midl_frag53_t __midl_frag53 =
{ 
/* _CONNECTION_INFO_1 */
    { 
    /* _CONNECTION_INFO_1 */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _CONNECTION_INFO_1 */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 40 /* 0x28 */,
        0,
        0,
        &__midl_frag903,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag51_t __midl_frag51 =
{ 
/* *_CONNECTION_INFO_1 */
    { 
    /* *_CONNECTION_INFO_1 */
        { 
        /* *_CONNECTION_INFO_1 */
            0x46,    /* FC64_FORCED_BOGUS_ARRAY */
            (NDR64_UINT8) 7 /* 0x7 */,
            { 
            /* *_CONNECTION_INFO_1 */
                0,
                1,
                0,
                0,
                0,
                0,
                0,
                0
            },
            (NDR64_UINT8) 1 /* 0x1 */,
            (NDR64_UINT32) 0 /* 0x0 */,
            &__midl_frag53
        },
        &__midl_frag915,
        0,
        0
    }
};

static const __midl_frag50_t __midl_frag50 =
{ 
/* CONNECT_INFO_1_CONTAINER */
    { 
    /* CONNECT_INFO_1_CONTAINER */
        0x35,    /* FC64_FORCED_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* CONNECT_INFO_1_CONTAINER */
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */,
        0,
        0,
        &__midl_frag57,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x14,    /* FC64_POINTER */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag49_t __midl_frag49 =
{ 
/* *CONNECT_INFO_1_CONTAINER */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag50
};

static const __midl_frag41_t __midl_frag41 =
{ 
/* CONNECT_ENUM_UNION */
    { 
    /* CONNECT_ENUM_UNION */
        0x51,    /* FC64_NON_ENCAPSULATED_UNION */
        (NDR64_UINT8) 7 /* 0x7 */,
        (NDR64_UINT8) 0 /* 0x0 */,
        0x6,    /* FC64_UINT32 */
        (NDR64_UINT32) 8 /* 0x8 */,
        &__midl_frag915,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM_SELECTOR */
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT8) 7 /* 0x7 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT32) 2 /* 0x2 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 0 /* 0x0 */,
        &__midl_frag75,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_UNION_ARM */
        (NDR64_INT64) 1 /* 0x1 */,
        &__midl_frag49,
        (NDR64_UINT32) 0 /* 0x0 */
    },
    (NDR64_UINT32) 4294967295 /* 0xffffffff */
};

static const __midl_frag40_t __midl_frag40 =
{ 
/* _CONNECT_ENUM_STRUCT */
    { 
    /* _CONNECT_ENUM_STRUCT */
        0x34,    /* FC64_BOGUS_STRUCT */
        (NDR64_UINT8) 7 /* 0x7 */,
        { 
        /* _CONNECT_ENUM_STRUCT */
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */,
        0,
        0,
        0,
    },
    { 
    /*  */
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x5,    /* FC64_INT32 */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_MEMPAD_FORMAT */
            0x90,    /* FC64_STRUCTPADN */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 4 /* 0x4 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_EMBEDDED_COMPLEX_FORMAT */
            0x91,    /* FC64_EMBEDDED_COMPLEX */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            &__midl_frag41
        },
        { 
        /* _CONNECT_ENUM_STRUCT */
            0x92,    /* FC64_BUFFER_ALIGN */
            (NDR64_UINT8) 7 /* 0x7 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        },
        { 
        /* struct _NDR64_SIMPLE_MEMBER_FORMAT */
            0x93,    /* FC64_END */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT16) 0 /* 0x0 */,
            (NDR64_UINT32) 0 /* 0x0 */
        }
    }
};

static const __midl_frag39_t __midl_frag39 =
{ 
/* *_CONNECT_ENUM_STRUCT */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag40
};

static const __midl_frag34_t __midl_frag34 =
{ 
/* NetrConnectionEnum */
    { 
    /* NetrConnectionEnum */      /* procedure NetrConnectionEnum */
        (NDR64_UINT32) 23986240 /* 0x16e0040 */,    /* explicit handle */ /* IsIntrepreted, ServerMustSize, ClientMustSize, HasReturn, ServerCorrelation, ClientCorrelation, HasExtensions */
        (NDR64_UINT32) 56 /* 0x38 */ ,  /* Stack size */
        (NDR64_UINT32) 40 /* 0x28 */,
        (NDR64_UINT32) 72 /* 0x48 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 7 /* 0x7 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* Qualifier */      /* parameter Qualifier */
        &__midl_frag933,
        { 
        /* Qualifier */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* InfoStruct */      /* parameter InfoStruct */
        &__midl_frag40,
        { 
        /* InfoStruct */
            1,
            1,
            0,
            1,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], [out], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* PreferedMaximumLength */      /* parameter PreferedMaximumLength */
        &__midl_frag949,
        { 
        /* PreferedMaximumLength */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* TotalEntries */      /* parameter TotalEntries */
        &__midl_frag949,
        { 
        /* TotalEntries */
            0,
            0,
            0,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            1
        },    /* [out], Basetype, SimpleRef, UseCache */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    },
    { 
    /* ResumeHandle */      /* parameter ResumeHandle */
        &__midl_frag920,
        { 
        /* ResumeHandle */
            0,
            1,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustFree, [in], [out] */
        (NDR64_UINT16) 0 /* 0x0 */,
        40 /* 0x28 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        48 /* 0x30 */,   /* Stack offset */
    }
};

static const __midl_frag30_t __midl_frag30 =
{ 
/* Opnum7NotUsedOnWire */
    { 
    /* Opnum7NotUsedOnWire */      /* procedure Opnum7NotUsedOnWire */
        (NDR64_UINT32) 17563712 /* 0x10c0040 */,    /* explicit handle */ /* IsIntrepreted, ClientMustSize, HasReturn, HasExtensions */
        (NDR64_UINT32) 16 /* 0x10 */ ,  /* Stack size */
        (NDR64_UINT32) 0 /* 0x0 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 2 /* 0x2 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x71,    /* FC64_BIND_GENERIC */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 8 /* 0x8 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* ServerName */      /* parameter ServerName */
        &__midl_frag933,
        { 
        /* ServerName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        0 /* 0x0 */,   /* Stack offset */
    },
    { 
    /* NET_API_STATUS */      /* parameter NET_API_STATUS */
        &__midl_frag949,
        { 
        /* NET_API_STATUS */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    }
};

static const __midl_frag1_t __midl_frag1 =
(NDR64_UINT32) 0 /* 0x0 */;


#include "poppack.h"


static const FormatInfoRef srvsvc_Ndr64ProcTable[] =
    {
    &__midl_frag30,
    &__midl_frag30,
    &__midl_frag30,
    &__midl_frag30,
    &__midl_frag30,
    &__midl_frag30,
    &__midl_frag30,
    &__midl_frag30,
    &__midl_frag34,
    &__midl_frag64,
    &__midl_frag96,
    &__midl_frag107,
    &__midl_frag112,
    &__midl_frag183,
    &__midl_frag191,
    &__midl_frag730,
    &__midl_frag299,
    &__midl_frag319,
    &__midl_frag348,
    &__midl_frag348,
    &__midl_frag355,
    &__midl_frag363,
    &__midl_frag489,
    &__midl_frag550,
    &__midl_frag569,
    &__midl_frag651,
    &__midl_frag593,
    &__midl_frag651,
    &__midl_frag657,
    &__midl_frag664,
    &__midl_frag673,
    &__midl_frag720,
    &__midl_frag699,
    &__midl_frag707,
    &__midl_frag720,
    &__midl_frag730,
    &__midl_frag740,
    &__midl_frag749,
    &__midl_frag753,
    &__midl_frag769,
    &__midl_frag884,
    &__midl_frag787,
    &__midl_frag793,
    &__midl_frag850,
    &__midl_frag842,
    &__midl_frag829,
    &__midl_frag842,
    &__midl_frag850,
    &__midl_frag857,
    &__midl_frag872,
    &__midl_frag884,
    &__midl_frag923,
    &__midl_frag905,
    &__midl_frag923,
    &__midl_frag932
    };


static const MIDL_STUB_DESC srvsvc_StubDesc = 
    {
    (void *)& srvsvc___RpcServerInterface,
    MIDL_user_allocate,
    MIDL_user_free,
    0,
    RundownRoutines,
    0,
    0,
    0,
    srvsvc__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x60003, /* Ndr library version */
    0,
    0x801026e, /* MIDL Version 8.1.622 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x2000001, /* MIDL flag */
    0, /* cs routines */
    (void *)& srvsvc_ServerInfo,   /* proxy/server info */
    0
    };

static const RPC_DISPATCH_FUNCTION srvsvc_NDR64__table[] =
    {
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    NdrServerCallAll,
    0
    };
static const RPC_DISPATCH_TABLE srvsvc_NDR64__v3_0_DispatchTable = 
    {
    55,
    (RPC_DISPATCH_FUNCTION*)srvsvc_NDR64__table
    };

static const MIDL_SYNTAX_INFO srvsvc_SyntaxInfo [  2 ] = 
    {
    {
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    (RPC_DISPATCH_TABLE*)&srvsvc_v3_0_DispatchTable,
    srvsvc__MIDL_ProcFormatString.Format,
    srvsvc_FormatStringOffsetTable,
    srvsvc__MIDL_TypeFormatString.Format,
    0,
    0,
    0
    }
    ,{
    {{0x71710533,0xbeba,0x4937,{0x83,0x19,0xb5,0xdb,0xef,0x9c,0xcc,0x36}},{1,0}},
    (RPC_DISPATCH_TABLE*)&srvsvc_NDR64__v3_0_DispatchTable,
    0 ,
    (unsigned short *) srvsvc_Ndr64ProcTable,
    0,
    0,
    0,
    0
    }
    };


static const SERVER_ROUTINE srvsvc_ServerRoutineTable[] = 
    {
    (SERVER_ROUTINE)Opnum0NotUsedOnWire,
    (SERVER_ROUTINE)Opnum1NotUsedOnWire,
    (SERVER_ROUTINE)Opnum2NotUsedOnWire,
    (SERVER_ROUTINE)Opnum3NotUsedOnWire,
    (SERVER_ROUTINE)Opnum4NotUsedOnWire,
    (SERVER_ROUTINE)Opnum5NotUsedOnWire,
    (SERVER_ROUTINE)Opnum6NotUsedOnWire,
    (SERVER_ROUTINE)Opnum7NotUsedOnWire,
    (SERVER_ROUTINE)NetrConnectionEnum,
    (SERVER_ROUTINE)NetrFileEnum,
    (SERVER_ROUTINE)NetrFileGetInfo,
    (SERVER_ROUTINE)NetrFileClose,
    (SERVER_ROUTINE)NetrSessionEnum,
    (SERVER_ROUTINE)NetrSessionDel,
    (SERVER_ROUTINE)NetrShareAdd,
    (SERVER_ROUTINE)NetrShareEnum,
    (SERVER_ROUTINE)NetrShareGetInfo,
    (SERVER_ROUTINE)NetrShareSetInfo,
    (SERVER_ROUTINE)NetrShareDel,
    (SERVER_ROUTINE)NetrShareDelSticky,
    (SERVER_ROUTINE)NetrShareCheck,
    (SERVER_ROUTINE)NetrServerGetInfo,
    (SERVER_ROUTINE)NetrServerSetInfo,
    (SERVER_ROUTINE)NetrServerDiskEnum,
    (SERVER_ROUTINE)NetrServerStatisticsGet,
    (SERVER_ROUTINE)NetrServerTransportAdd,
    (SERVER_ROUTINE)NetrServerTransportEnum,
    (SERVER_ROUTINE)NetrServerTransportDel,
    (SERVER_ROUTINE)NetrRemoteTOD,
    (SERVER_ROUTINE)NetprPathType,
    (SERVER_ROUTINE)NetprPathCanonicalize,
    (SERVER_ROUTINE)NetprPathCompare,
    (SERVER_ROUTINE)NetprNameValidate,
    (SERVER_ROUTINE)NetprNameCanonicalize,
    (SERVER_ROUTINE)NetprNameCompare,
    (SERVER_ROUTINE)NetrShareEnumSticky,
    (SERVER_ROUTINE)NetrShareDelStart,
    (SERVER_ROUTINE)NetrShareDelCommit,
    (SERVER_ROUTINE)NetrpGetFileSecurity,
    (SERVER_ROUTINE)NetrpSetFileSecurity,
    (SERVER_ROUTINE)NetrServerTransportAddEx,
    (SERVER_ROUTINE)NetrDfsGetVersion,
    (SERVER_ROUTINE)NetrDfsCreateLocalPartition,
    (SERVER_ROUTINE)NetrDfsDeleteLocalPartition,
    (SERVER_ROUTINE)NetrDfsSetLocalVolumeState,
    (SERVER_ROUTINE)NetrDfsCreateExitPoint,
    (SERVER_ROUTINE)NetrDfsDeleteExitPoint,
    (SERVER_ROUTINE)NetrDfsModifyPrefix,
    (SERVER_ROUTINE)NetrDfsFixLocalVolume,
    (SERVER_ROUTINE)NetrDfsManagerReportSiteInfo,
    (SERVER_ROUTINE)NetrServerTransportDelEx,
    (SERVER_ROUTINE)NetrServerAliasAdd,
    (SERVER_ROUTINE)NetrServerAliasEnum,
    (SERVER_ROUTINE)NetrServerAliasDel,
    (SERVER_ROUTINE)NetrShareDelEx
    };

static const MIDL_SERVER_INFO srvsvc_ServerInfo = 
    {
    &srvsvc_StubDesc,
    srvsvc_ServerRoutineTable,
    srvsvc__MIDL_ProcFormatString.Format,
    (unsigned short *) srvsvc_FormatStringOffsetTable,
    0,
    (RPC_SYNTAX_IDENTIFIER*)&_NDR64_RpcTransferSyntax,
    2,
    (MIDL_SYNTAX_INFO*)srvsvc_SyntaxInfo
    };
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* defined(_M_AMD64)*/

