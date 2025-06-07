

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Tue Aug 17 12:54:40 2004
 */
/* Compiler settings for netfw.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if !defined(_M_IA64) && !defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */
#pragma warning( disable: 4211 )  /* redefine extent to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "netfw.h"

#define TYPE_FORMAT_STRING_SIZE   1387                              
#define PROC_FORMAT_STRING_SIZE   3043                              
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   2            

typedef struct _MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } MIDL_PROC_FORMAT_STRING;


static RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INetFwRemoteAdminSettings_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INetFwRemoteAdminSettings_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INetFwIcmpSettings_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INetFwIcmpSettings_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INetFwOpenPort_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INetFwOpenPort_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INetFwOpenPorts_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INetFwOpenPorts_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INetFwService_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INetFwService_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INetFwServices_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INetFwServices_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INetFwAuthorizedApplication_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INetFwAuthorizedApplication_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INetFwAuthorizedApplications_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INetFwAuthorizedApplications_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INetFwProfile_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INetFwProfile_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INetFwPolicy_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INetFwPolicy_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INetFwMgr_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INetFwMgr_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need a Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure get_Type */


	/* Procedure get_IpVersion */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
/*  8 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x22 ),	/* 34 */
/* 14 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 16 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 18 */	NdrFcShort( 0x0 ),	/* 0 */
/* 20 */	NdrFcShort( 0x0 ),	/* 0 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter type */


	/* Parameter ipVersion */

/* 24 */	NdrFcShort( 0x2010 ),	/* Flags:  out, srv alloc size=8 */
/* 26 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 28 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */


	/* Return value */

/* 30 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 32 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 34 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_IpVersion */

/* 36 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 38 */	NdrFcLong( 0x0 ),	/* 0 */
/* 42 */	NdrFcShort( 0x8 ),	/* 8 */
/* 44 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 46 */	NdrFcShort( 0x6 ),	/* 6 */
/* 48 */	NdrFcShort( 0x8 ),	/* 8 */
/* 50 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 52 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 54 */	NdrFcShort( 0x0 ),	/* 0 */
/* 56 */	NdrFcShort( 0x0 ),	/* 0 */
/* 58 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ipVersion */

/* 60 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 62 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 64 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Return value */

/* 66 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 68 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 70 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_IpVersion */


	/* Procedure get_Scope */

/* 72 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 74 */	NdrFcLong( 0x0 ),	/* 0 */
/* 78 */	NdrFcShort( 0x9 ),	/* 9 */
/* 80 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 82 */	NdrFcShort( 0x0 ),	/* 0 */
/* 84 */	NdrFcShort( 0x22 ),	/* 34 */
/* 86 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 88 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 90 */	NdrFcShort( 0x0 ),	/* 0 */
/* 92 */	NdrFcShort( 0x0 ),	/* 0 */
/* 94 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ipVersion */


	/* Parameter scope */

/* 96 */	NdrFcShort( 0x2010 ),	/* Flags:  out, srv alloc size=8 */
/* 98 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 100 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */


	/* Return value */

/* 102 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 104 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 106 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_IpVersion */


	/* Procedure put_Scope */

/* 108 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 110 */	NdrFcLong( 0x0 ),	/* 0 */
/* 114 */	NdrFcShort( 0xa ),	/* 10 */
/* 116 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 118 */	NdrFcShort( 0x6 ),	/* 6 */
/* 120 */	NdrFcShort( 0x8 ),	/* 8 */
/* 122 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 124 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 126 */	NdrFcShort( 0x0 ),	/* 0 */
/* 128 */	NdrFcShort( 0x0 ),	/* 0 */
/* 130 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ipVersion */


	/* Parameter scope */

/* 132 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 134 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 136 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 138 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 140 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 142 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_RemoteAddresses */

/* 144 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 146 */	NdrFcLong( 0x0 ),	/* 0 */
/* 150 */	NdrFcShort( 0xb ),	/* 11 */
/* 152 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 154 */	NdrFcShort( 0x0 ),	/* 0 */
/* 156 */	NdrFcShort( 0x8 ),	/* 8 */
/* 158 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 160 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 162 */	NdrFcShort( 0x1 ),	/* 1 */
/* 164 */	NdrFcShort( 0x0 ),	/* 0 */
/* 166 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter remoteAddrs */

/* 168 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 170 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 172 */	NdrFcShort( 0x24 ),	/* Type Offset=36 */

	/* Return value */

/* 174 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 176 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 178 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_RemoteAddresses */

/* 180 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 182 */	NdrFcLong( 0x0 ),	/* 0 */
/* 186 */	NdrFcShort( 0xc ),	/* 12 */
/* 188 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 190 */	NdrFcShort( 0x0 ),	/* 0 */
/* 192 */	NdrFcShort( 0x8 ),	/* 8 */
/* 194 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 196 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 198 */	NdrFcShort( 0x0 ),	/* 0 */
/* 200 */	NdrFcShort( 0x1 ),	/* 1 */
/* 202 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter remoteAddrs */

/* 204 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 206 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 208 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Return value */

/* 210 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 212 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 214 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_AllowOutboundTimeExceeded */


	/* Procedure get_Enabled */

/* 216 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 218 */	NdrFcLong( 0x0 ),	/* 0 */
/* 222 */	NdrFcShort( 0xd ),	/* 13 */
/* 224 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 226 */	NdrFcShort( 0x0 ),	/* 0 */
/* 228 */	NdrFcShort( 0x22 ),	/* 34 */
/* 230 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 232 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 234 */	NdrFcShort( 0x0 ),	/* 0 */
/* 236 */	NdrFcShort( 0x0 ),	/* 0 */
/* 238 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter allow */


	/* Parameter enabled */

/* 240 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 242 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 244 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 246 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 248 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 250 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_AllowOutboundTimeExceeded */


	/* Procedure put_Enabled */

/* 252 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 254 */	NdrFcLong( 0x0 ),	/* 0 */
/* 258 */	NdrFcShort( 0xe ),	/* 14 */
/* 260 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 262 */	NdrFcShort( 0x6 ),	/* 6 */
/* 264 */	NdrFcShort( 0x8 ),	/* 8 */
/* 266 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 268 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 270 */	NdrFcShort( 0x0 ),	/* 0 */
/* 272 */	NdrFcShort( 0x0 ),	/* 0 */
/* 274 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter allow */


	/* Parameter enabled */

/* 276 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 278 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 280 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 282 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 284 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 286 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_AllowOutboundDestinationUnreachable */

/* 288 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 290 */	NdrFcLong( 0x0 ),	/* 0 */
/* 294 */	NdrFcShort( 0x7 ),	/* 7 */
/* 296 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 298 */	NdrFcShort( 0x0 ),	/* 0 */
/* 300 */	NdrFcShort( 0x22 ),	/* 34 */
/* 302 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 304 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 306 */	NdrFcShort( 0x0 ),	/* 0 */
/* 308 */	NdrFcShort( 0x0 ),	/* 0 */
/* 310 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter allow */

/* 312 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 314 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 316 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 318 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 320 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 322 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_AllowOutboundDestinationUnreachable */

/* 324 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 326 */	NdrFcLong( 0x0 ),	/* 0 */
/* 330 */	NdrFcShort( 0x8 ),	/* 8 */
/* 332 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 334 */	NdrFcShort( 0x6 ),	/* 6 */
/* 336 */	NdrFcShort( 0x8 ),	/* 8 */
/* 338 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 340 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 342 */	NdrFcShort( 0x0 ),	/* 0 */
/* 344 */	NdrFcShort( 0x0 ),	/* 0 */
/* 346 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter allow */

/* 348 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 350 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 352 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 354 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 356 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 358 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Customized */


	/* Procedure get_AllowRedirect */

/* 360 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 362 */	NdrFcLong( 0x0 ),	/* 0 */
/* 366 */	NdrFcShort( 0x9 ),	/* 9 */
/* 368 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 370 */	NdrFcShort( 0x0 ),	/* 0 */
/* 372 */	NdrFcShort( 0x22 ),	/* 34 */
/* 374 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 376 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 378 */	NdrFcShort( 0x0 ),	/* 0 */
/* 380 */	NdrFcShort( 0x0 ),	/* 0 */
/* 382 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter customized */


	/* Parameter allow */

/* 384 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 386 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 388 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 390 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 392 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 394 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_AllowRedirect */

/* 396 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 398 */	NdrFcLong( 0x0 ),	/* 0 */
/* 402 */	NdrFcShort( 0xa ),	/* 10 */
/* 404 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 406 */	NdrFcShort( 0x6 ),	/* 6 */
/* 408 */	NdrFcShort( 0x8 ),	/* 8 */
/* 410 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 412 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 414 */	NdrFcShort( 0x0 ),	/* 0 */
/* 416 */	NdrFcShort( 0x0 ),	/* 0 */
/* 418 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter allow */

/* 420 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 422 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 424 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 426 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 428 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 430 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_AllowInboundEchoRequest */

/* 432 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 434 */	NdrFcLong( 0x0 ),	/* 0 */
/* 438 */	NdrFcShort( 0xb ),	/* 11 */
/* 440 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 442 */	NdrFcShort( 0x0 ),	/* 0 */
/* 444 */	NdrFcShort( 0x22 ),	/* 34 */
/* 446 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 448 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 450 */	NdrFcShort( 0x0 ),	/* 0 */
/* 452 */	NdrFcShort( 0x0 ),	/* 0 */
/* 454 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter allow */

/* 456 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 458 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 460 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 462 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 464 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 466 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_AllowInboundEchoRequest */

/* 468 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 470 */	NdrFcLong( 0x0 ),	/* 0 */
/* 474 */	NdrFcShort( 0xc ),	/* 12 */
/* 476 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 478 */	NdrFcShort( 0x6 ),	/* 6 */
/* 480 */	NdrFcShort( 0x8 ),	/* 8 */
/* 482 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 484 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 486 */	NdrFcShort( 0x0 ),	/* 0 */
/* 488 */	NdrFcShort( 0x0 ),	/* 0 */
/* 490 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter allow */

/* 492 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 494 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 496 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 498 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 500 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 502 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_AllowOutboundParameterProblem */

/* 504 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 506 */	NdrFcLong( 0x0 ),	/* 0 */
/* 510 */	NdrFcShort( 0xf ),	/* 15 */
/* 512 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 514 */	NdrFcShort( 0x0 ),	/* 0 */
/* 516 */	NdrFcShort( 0x22 ),	/* 34 */
/* 518 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 520 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 522 */	NdrFcShort( 0x0 ),	/* 0 */
/* 524 */	NdrFcShort( 0x0 ),	/* 0 */
/* 526 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter allow */

/* 528 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 530 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 532 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 534 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 536 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 538 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_AllowOutboundParameterProblem */

/* 540 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 542 */	NdrFcLong( 0x0 ),	/* 0 */
/* 546 */	NdrFcShort( 0x10 ),	/* 16 */
/* 548 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 550 */	NdrFcShort( 0x6 ),	/* 6 */
/* 552 */	NdrFcShort( 0x8 ),	/* 8 */
/* 554 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 556 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 558 */	NdrFcShort( 0x0 ),	/* 0 */
/* 560 */	NdrFcShort( 0x0 ),	/* 0 */
/* 562 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter allow */

/* 564 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 566 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 568 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 570 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 572 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 574 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Enabled */


	/* Procedure get_AllowOutboundSourceQuench */

/* 576 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 578 */	NdrFcLong( 0x0 ),	/* 0 */
/* 582 */	NdrFcShort( 0x11 ),	/* 17 */
/* 584 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 586 */	NdrFcShort( 0x0 ),	/* 0 */
/* 588 */	NdrFcShort( 0x22 ),	/* 34 */
/* 590 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 592 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 594 */	NdrFcShort( 0x0 ),	/* 0 */
/* 596 */	NdrFcShort( 0x0 ),	/* 0 */
/* 598 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter enabled */


	/* Parameter allow */

/* 600 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 602 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 604 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 606 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 608 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 610 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Enabled */


	/* Procedure put_AllowOutboundSourceQuench */

/* 612 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 614 */	NdrFcLong( 0x0 ),	/* 0 */
/* 618 */	NdrFcShort( 0x12 ),	/* 18 */
/* 620 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 622 */	NdrFcShort( 0x6 ),	/* 6 */
/* 624 */	NdrFcShort( 0x8 ),	/* 8 */
/* 626 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 628 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 630 */	NdrFcShort( 0x0 ),	/* 0 */
/* 632 */	NdrFcShort( 0x0 ),	/* 0 */
/* 634 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter enabled */


	/* Parameter allow */

/* 636 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 638 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 640 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 642 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 644 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 646 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Enabled */


	/* Procedure get_AllowInboundRouterRequest */

/* 648 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 650 */	NdrFcLong( 0x0 ),	/* 0 */
/* 654 */	NdrFcShort( 0x13 ),	/* 19 */
/* 656 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 658 */	NdrFcShort( 0x0 ),	/* 0 */
/* 660 */	NdrFcShort( 0x22 ),	/* 34 */
/* 662 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 664 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 666 */	NdrFcShort( 0x0 ),	/* 0 */
/* 668 */	NdrFcShort( 0x0 ),	/* 0 */
/* 670 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter enabled */


	/* Parameter allow */

/* 672 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 674 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 676 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 678 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 680 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 682 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Enabled */


	/* Procedure put_AllowInboundRouterRequest */

/* 684 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 686 */	NdrFcLong( 0x0 ),	/* 0 */
/* 690 */	NdrFcShort( 0x14 ),	/* 20 */
/* 692 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 694 */	NdrFcShort( 0x6 ),	/* 6 */
/* 696 */	NdrFcShort( 0x8 ),	/* 8 */
/* 698 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 700 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 702 */	NdrFcShort( 0x0 ),	/* 0 */
/* 704 */	NdrFcShort( 0x0 ),	/* 0 */
/* 706 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter enabled */


	/* Parameter allow */

/* 708 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 710 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 712 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 714 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 716 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 718 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_BuiltIn */


	/* Procedure get_AllowInboundTimestampRequest */

/* 720 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 722 */	NdrFcLong( 0x0 ),	/* 0 */
/* 726 */	NdrFcShort( 0x15 ),	/* 21 */
/* 728 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 730 */	NdrFcShort( 0x0 ),	/* 0 */
/* 732 */	NdrFcShort( 0x22 ),	/* 34 */
/* 734 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 736 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 738 */	NdrFcShort( 0x0 ),	/* 0 */
/* 740 */	NdrFcShort( 0x0 ),	/* 0 */
/* 742 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter builtIn */


	/* Parameter allow */

/* 744 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 746 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 748 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 750 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 752 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 754 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_AllowInboundTimestampRequest */

/* 756 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 758 */	NdrFcLong( 0x0 ),	/* 0 */
/* 762 */	NdrFcShort( 0x16 ),	/* 22 */
/* 764 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 766 */	NdrFcShort( 0x6 ),	/* 6 */
/* 768 */	NdrFcShort( 0x8 ),	/* 8 */
/* 770 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 772 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 774 */	NdrFcShort( 0x0 ),	/* 0 */
/* 776 */	NdrFcShort( 0x0 ),	/* 0 */
/* 778 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter allow */

/* 780 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 782 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 784 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 786 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 788 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 790 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_AllowInboundMaskRequest */

/* 792 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 794 */	NdrFcLong( 0x0 ),	/* 0 */
/* 798 */	NdrFcShort( 0x17 ),	/* 23 */
/* 800 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 802 */	NdrFcShort( 0x0 ),	/* 0 */
/* 804 */	NdrFcShort( 0x22 ),	/* 34 */
/* 806 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 808 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 810 */	NdrFcShort( 0x0 ),	/* 0 */
/* 812 */	NdrFcShort( 0x0 ),	/* 0 */
/* 814 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter allow */

/* 816 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 818 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 820 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 822 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 824 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 826 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_AllowInboundMaskRequest */

/* 828 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 830 */	NdrFcLong( 0x0 ),	/* 0 */
/* 834 */	NdrFcShort( 0x18 ),	/* 24 */
/* 836 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 838 */	NdrFcShort( 0x6 ),	/* 6 */
/* 840 */	NdrFcShort( 0x8 ),	/* 8 */
/* 842 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 844 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 846 */	NdrFcShort( 0x0 ),	/* 0 */
/* 848 */	NdrFcShort( 0x0 ),	/* 0 */
/* 850 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter allow */

/* 852 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 854 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 856 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 858 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 860 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 862 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_AllowOutboundPacketTooBig */

/* 864 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 866 */	NdrFcLong( 0x0 ),	/* 0 */
/* 870 */	NdrFcShort( 0x19 ),	/* 25 */
/* 872 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 874 */	NdrFcShort( 0x0 ),	/* 0 */
/* 876 */	NdrFcShort( 0x22 ),	/* 34 */
/* 878 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 880 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 882 */	NdrFcShort( 0x0 ),	/* 0 */
/* 884 */	NdrFcShort( 0x0 ),	/* 0 */
/* 886 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter allow */

/* 888 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 890 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 892 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 894 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 896 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 898 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_AllowOutboundPacketTooBig */

/* 900 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 902 */	NdrFcLong( 0x0 ),	/* 0 */
/* 906 */	NdrFcShort( 0x1a ),	/* 26 */
/* 908 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 910 */	NdrFcShort( 0x6 ),	/* 6 */
/* 912 */	NdrFcShort( 0x8 ),	/* 8 */
/* 914 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 916 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 918 */	NdrFcShort( 0x0 ),	/* 0 */
/* 920 */	NdrFcShort( 0x0 ),	/* 0 */
/* 922 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter allow */

/* 924 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 926 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 928 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 930 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 932 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 934 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Name */


	/* Procedure get_Name */


	/* Procedure get_Name */

/* 936 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 938 */	NdrFcLong( 0x0 ),	/* 0 */
/* 942 */	NdrFcShort( 0x7 ),	/* 7 */
/* 944 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 946 */	NdrFcShort( 0x0 ),	/* 0 */
/* 948 */	NdrFcShort( 0x8 ),	/* 8 */
/* 950 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 952 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 954 */	NdrFcShort( 0x1 ),	/* 1 */
/* 956 */	NdrFcShort( 0x0 ),	/* 0 */
/* 958 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter name */


	/* Parameter name */


	/* Parameter name */

/* 960 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 962 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 964 */	NdrFcShort( 0x24 ),	/* Type Offset=36 */

	/* Return value */


	/* Return value */


	/* Return value */

/* 966 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 968 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 970 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Name */


	/* Procedure put_Name */

/* 972 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 974 */	NdrFcLong( 0x0 ),	/* 0 */
/* 978 */	NdrFcShort( 0x8 ),	/* 8 */
/* 980 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 982 */	NdrFcShort( 0x0 ),	/* 0 */
/* 984 */	NdrFcShort( 0x8 ),	/* 8 */
/* 986 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 988 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 990 */	NdrFcShort( 0x0 ),	/* 0 */
/* 992 */	NdrFcShort( 0x1 ),	/* 1 */
/* 994 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter name */


	/* Parameter name */

/* 996 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 998 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1000 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Return value */


	/* Return value */

/* 1002 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1004 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1006 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_IpVersion */


	/* Procedure get_Protocol */

/* 1008 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1010 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1014 */	NdrFcShort( 0xb ),	/* 11 */
/* 1016 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1018 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1020 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1022 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1024 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1026 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1028 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1030 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ipVersion */


	/* Parameter ipProtocol */

/* 1032 */	NdrFcShort( 0x2010 ),	/* Flags:  out, srv alloc size=8 */
/* 1034 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1036 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */


	/* Return value */

/* 1038 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1040 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1042 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_IpVersion */


	/* Procedure put_Protocol */

/* 1044 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1046 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1050 */	NdrFcShort( 0xc ),	/* 12 */
/* 1052 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1054 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1056 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1058 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1060 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1062 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1064 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1066 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ipVersion */


	/* Parameter ipProtocol */

/* 1068 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1070 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1072 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 1074 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1076 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1078 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Port */

/* 1080 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1082 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1086 */	NdrFcShort( 0xd ),	/* 13 */
/* 1088 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1090 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1092 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1094 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1096 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1098 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1100 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1102 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter portNumber */

/* 1104 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1106 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1108 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1110 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1112 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1114 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Port */

/* 1116 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1118 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1122 */	NdrFcShort( 0xe ),	/* 14 */
/* 1124 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1126 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1128 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1130 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1132 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1134 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1136 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1138 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter portNumber */

/* 1140 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1142 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1144 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1146 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1148 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1150 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Scope */

/* 1152 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1154 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1158 */	NdrFcShort( 0xf ),	/* 15 */
/* 1160 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1162 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1164 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1166 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1168 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1170 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1172 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1174 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter scope */

/* 1176 */	NdrFcShort( 0x2010 ),	/* Flags:  out, srv alloc size=8 */
/* 1178 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1180 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */

/* 1182 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1184 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1186 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Scope */

/* 1188 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1190 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1194 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1196 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1198 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1200 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1202 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1204 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1206 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1208 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1210 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter scope */

/* 1212 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1214 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1216 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Return value */

/* 1218 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1220 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1222 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_RemoteAddresses */

/* 1224 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1226 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1230 */	NdrFcShort( 0x11 ),	/* 17 */
/* 1232 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1234 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1236 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1238 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1240 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 1242 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1244 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1246 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter remoteAddrs */

/* 1248 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 1250 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1252 */	NdrFcShort( 0x24 ),	/* Type Offset=36 */

	/* Return value */

/* 1254 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1256 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1258 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_RemoteAddresses */

/* 1260 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1262 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1266 */	NdrFcShort( 0x12 ),	/* 18 */
/* 1268 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1270 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1272 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1274 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1276 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1278 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1280 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1282 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter remoteAddrs */

/* 1284 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1286 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1288 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Return value */

/* 1290 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1292 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1294 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Count */


	/* Procedure get_Count */


	/* Procedure get_Count */

/* 1296 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1298 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1302 */	NdrFcShort( 0x7 ),	/* 7 */
/* 1304 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1306 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1308 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1310 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1312 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1314 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1316 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1318 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter count */


	/* Parameter count */


	/* Parameter count */

/* 1320 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1322 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1324 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */


	/* Return value */

/* 1326 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1328 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1330 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Add */

/* 1332 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1334 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1338 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1340 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1342 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1344 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1346 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1348 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1350 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1352 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1354 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter port */

/* 1356 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1358 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1360 */	NdrFcShort( 0x44 ),	/* Type Offset=68 */

	/* Return value */

/* 1362 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1364 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1366 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Remove */

/* 1368 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1370 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1374 */	NdrFcShort( 0x9 ),	/* 9 */
/* 1376 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1378 */	NdrFcShort( 0xe ),	/* 14 */
/* 1380 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1382 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 1384 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1386 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1388 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1390 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter portNumber */

/* 1392 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1394 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1396 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ipProtocol */

/* 1398 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1400 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1402 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Return value */

/* 1404 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1406 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1408 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Item */

/* 1410 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1412 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1416 */	NdrFcShort( 0xa ),	/* 10 */
/* 1418 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1420 */	NdrFcShort( 0xe ),	/* 14 */
/* 1422 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1424 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x4,		/* 4 */
/* 1426 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1428 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1430 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1432 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter portNumber */

/* 1434 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1436 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1438 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ipProtocol */

/* 1440 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1442 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1444 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Parameter openPort */

/* 1446 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1448 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1450 */	NdrFcShort( 0x56 ),	/* Type Offset=86 */

	/* Return value */

/* 1452 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1454 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1456 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get__NewEnum */

/* 1458 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1460 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1464 */	NdrFcShort( 0xb ),	/* 11 */
/* 1466 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1468 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1470 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1472 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1474 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1476 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1478 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1480 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newEnum */

/* 1482 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1484 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1486 */	NdrFcShort( 0x5a ),	/* Type Offset=90 */

	/* Return value */

/* 1488 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1490 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1492 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_CurrentProfileType */


	/* Procedure get_Type */

/* 1494 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1496 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1500 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1502 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1504 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1506 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1508 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1510 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1512 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1514 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1516 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter profileType */


	/* Parameter type */

/* 1518 */	NdrFcShort( 0x2010 ),	/* Flags:  out, srv alloc size=8 */
/* 1520 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1522 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */


	/* Return value */

/* 1524 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1526 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1528 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_IpVersion */

/* 1530 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1532 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1536 */	NdrFcShort( 0xa ),	/* 10 */
/* 1538 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1540 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1542 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1544 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1546 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1548 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1550 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1552 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ipVersion */

/* 1554 */	NdrFcShort( 0x2010 ),	/* Flags:  out, srv alloc size=8 */
/* 1556 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1558 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */

/* 1560 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1562 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1564 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_IpVersion */

/* 1566 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1568 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1572 */	NdrFcShort( 0xb ),	/* 11 */
/* 1574 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1576 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1578 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1580 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1582 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1584 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1586 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1588 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ipVersion */

/* 1590 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1592 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1594 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Return value */

/* 1596 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1598 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1600 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Scope */

/* 1602 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1604 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1608 */	NdrFcShort( 0xc ),	/* 12 */
/* 1610 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1612 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1614 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1616 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1618 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1620 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1622 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1624 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter scope */

/* 1626 */	NdrFcShort( 0x2010 ),	/* Flags:  out, srv alloc size=8 */
/* 1628 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1630 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */

/* 1632 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1634 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1636 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Scope */

/* 1638 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1640 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1644 */	NdrFcShort( 0xd ),	/* 13 */
/* 1646 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1648 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1650 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1652 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1654 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1656 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1658 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1660 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter scope */

/* 1662 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1664 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1666 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Return value */

/* 1668 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1670 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1672 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_RemoteAddresses */

/* 1674 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1676 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1680 */	NdrFcShort( 0xe ),	/* 14 */
/* 1682 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1684 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1686 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1688 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1690 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 1692 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1694 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1696 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter remoteAddrs */

/* 1698 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 1700 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1702 */	NdrFcShort( 0x24 ),	/* Type Offset=36 */

	/* Return value */

/* 1704 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1706 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1708 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_RemoteAddresses */

/* 1710 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1712 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1716 */	NdrFcShort( 0xf ),	/* 15 */
/* 1718 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1720 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1722 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1724 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1726 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1728 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1730 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1732 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter remoteAddrs */

/* 1734 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1736 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1738 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Return value */

/* 1740 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1742 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1744 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Enabled */

/* 1746 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1748 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1752 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1754 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1756 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1758 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1760 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1762 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1764 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1766 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1768 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter enabled */

/* 1770 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1772 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1774 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1776 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1778 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1780 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Enabled */

/* 1782 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1784 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1788 */	NdrFcShort( 0x11 ),	/* 17 */
/* 1790 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1792 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1794 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1796 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1798 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1800 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1802 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1804 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter enabled */

/* 1806 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1808 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1810 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1812 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1814 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1816 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_GloballyOpenPorts */

/* 1818 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1820 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1824 */	NdrFcShort( 0x12 ),	/* 18 */
/* 1826 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1828 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1830 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1832 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1834 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1836 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1838 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1840 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter openPorts */

/* 1842 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1844 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1846 */	NdrFcShort( 0x70 ),	/* Type Offset=112 */

	/* Return value */

/* 1848 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1850 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1852 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Item */

/* 1854 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1856 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1860 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1862 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1864 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1866 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1868 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 1870 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1872 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1874 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1876 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter svcType */

/* 1878 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1880 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1882 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Parameter service */

/* 1884 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1886 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1888 */	NdrFcShort( 0x86 ),	/* Type Offset=134 */

	/* Return value */

/* 1890 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1892 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1894 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get__NewEnum */

/* 1896 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1898 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1902 */	NdrFcShort( 0x9 ),	/* 9 */
/* 1904 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1906 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1908 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1910 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1912 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1914 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1916 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1918 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newEnum */

/* 1920 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1922 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1924 */	NdrFcShort( 0x9c ),	/* Type Offset=156 */

	/* Return value */

/* 1926 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1928 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1930 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_ProcessImageFileName */

/* 1932 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1934 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1938 */	NdrFcShort( 0x9 ),	/* 9 */
/* 1940 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1942 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1944 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1946 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1948 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 1950 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1952 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1954 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter imageFileName */

/* 1956 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 1958 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1960 */	NdrFcShort( 0x24 ),	/* Type Offset=36 */

	/* Return value */

/* 1962 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1964 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1966 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_ProcessImageFileName */

/* 1968 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1970 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1974 */	NdrFcShort( 0xa ),	/* 10 */
/* 1976 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1978 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1980 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1982 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1984 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1986 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1988 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1990 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter imageFileName */

/* 1992 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1994 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1996 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Return value */

/* 1998 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2000 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2002 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Scope */

/* 2004 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2006 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2010 */	NdrFcShort( 0xd ),	/* 13 */
/* 2012 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2014 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2016 */	NdrFcShort( 0x22 ),	/* 34 */
/* 2018 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2020 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2022 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2024 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2026 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter scope */

/* 2028 */	NdrFcShort( 0x2010 ),	/* Flags:  out, srv alloc size=8 */
/* 2030 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2032 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */

/* 2034 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2036 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2038 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Scope */

/* 2040 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2042 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2046 */	NdrFcShort( 0xe ),	/* 14 */
/* 2048 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2050 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2052 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2054 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2056 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2058 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2060 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2062 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter scope */

/* 2064 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2066 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2068 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Return value */

/* 2070 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2072 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2074 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_RemoteAddresses */

/* 2076 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2078 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2082 */	NdrFcShort( 0xf ),	/* 15 */
/* 2084 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2086 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2088 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2090 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2092 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 2094 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2096 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2098 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter remoteAddrs */

/* 2100 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 2102 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2104 */	NdrFcShort( 0x24 ),	/* Type Offset=36 */

	/* Return value */

/* 2106 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2108 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2110 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_RemoteAddresses */

/* 2112 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2114 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2118 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2120 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2122 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2124 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2126 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 2128 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 2130 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2132 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2134 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter remoteAddrs */

/* 2136 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2138 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2140 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Return value */

/* 2142 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2144 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2146 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Add */

/* 2148 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2150 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2154 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2156 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2158 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2160 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2162 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 2164 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2166 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2168 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2170 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter app */

/* 2172 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2174 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2176 */	NdrFcShort( 0xb2 ),	/* Type Offset=178 */

	/* Return value */

/* 2178 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2180 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2182 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Remove */

/* 2184 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2186 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2190 */	NdrFcShort( 0x9 ),	/* 9 */
/* 2192 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2194 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2196 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2198 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 2200 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 2202 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2204 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2206 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter imageFileName */

/* 2208 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2210 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2212 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Return value */

/* 2214 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2216 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2218 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Item */

/* 2220 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2222 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2226 */	NdrFcShort( 0xa ),	/* 10 */
/* 2228 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2230 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2232 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2234 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 2236 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 2238 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2240 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2242 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter imageFileName */

/* 2244 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2246 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2248 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Parameter app */

/* 2250 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2252 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2254 */	NdrFcShort( 0xc4 ),	/* Type Offset=196 */

	/* Return value */

/* 2256 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2258 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2260 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get__NewEnum */

/* 2262 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2264 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2268 */	NdrFcShort( 0xb ),	/* 11 */
/* 2270 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2272 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2274 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2276 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2278 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2280 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2282 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2284 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newEnum */

/* 2286 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2288 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2290 */	NdrFcShort( 0x9c ),	/* Type Offset=156 */

	/* Return value */

/* 2292 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2294 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2296 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_FirewallEnabled */

/* 2298 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2300 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2304 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2306 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2308 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2310 */	NdrFcShort( 0x22 ),	/* 34 */
/* 2312 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2314 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2316 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2318 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2320 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter enabled */

/* 2322 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2324 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2326 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2328 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2330 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2332 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_FirewallEnabled */

/* 2334 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2336 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2340 */	NdrFcShort( 0x9 ),	/* 9 */
/* 2342 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2344 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2346 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2348 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2350 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2352 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2354 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2356 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter enabled */

/* 2358 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2360 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2362 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2364 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2366 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2368 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_ExceptionsNotAllowed */

/* 2370 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2372 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2376 */	NdrFcShort( 0xa ),	/* 10 */
/* 2378 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2380 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2382 */	NdrFcShort( 0x22 ),	/* 34 */
/* 2384 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2386 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2388 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2390 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2392 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter notAllowed */

/* 2394 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2396 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2398 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2400 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2402 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2404 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_ExceptionsNotAllowed */

/* 2406 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2408 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2412 */	NdrFcShort( 0xb ),	/* 11 */
/* 2414 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2416 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2418 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2420 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2422 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2424 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2426 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2428 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter notAllowed */

/* 2430 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2432 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2434 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2436 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2438 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2440 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_NotificationsDisabled */

/* 2442 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2444 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2448 */	NdrFcShort( 0xc ),	/* 12 */
/* 2450 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2452 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2454 */	NdrFcShort( 0x22 ),	/* 34 */
/* 2456 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2458 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2460 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2462 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2464 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter disabled */

/* 2466 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2468 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2470 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2472 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2474 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2476 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_NotificationsDisabled */

/* 2478 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2480 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2484 */	NdrFcShort( 0xd ),	/* 13 */
/* 2486 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2488 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2490 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2492 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2494 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2496 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2498 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2500 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter disabled */

/* 2502 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2504 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2506 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2508 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2510 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2512 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_UnicastResponsesToMulticastBroadcastDisabled */

/* 2514 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2516 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2520 */	NdrFcShort( 0xe ),	/* 14 */
/* 2522 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2524 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2526 */	NdrFcShort( 0x22 ),	/* 34 */
/* 2528 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2530 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2532 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2534 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2536 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter disabled */

/* 2538 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2540 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2542 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2544 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2546 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2548 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_UnicastResponsesToMulticastBroadcastDisabled */

/* 2550 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2552 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2556 */	NdrFcShort( 0xf ),	/* 15 */
/* 2558 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2560 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2562 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2564 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2566 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2568 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2570 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2572 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter disabled */

/* 2574 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2576 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2578 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2580 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2582 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2584 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_RemoteAdminSettings */

/* 2586 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2588 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2592 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2594 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2596 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2598 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2600 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2602 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2604 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2606 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2608 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter remoteAdminSettings */

/* 2610 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2612 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2614 */	NdrFcShort( 0xc8 ),	/* Type Offset=200 */

	/* Return value */

/* 2616 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2618 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2620 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_IcmpSettings */

/* 2622 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2624 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2628 */	NdrFcShort( 0x11 ),	/* 17 */
/* 2630 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2632 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2634 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2636 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2638 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2640 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2642 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2644 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter icmpSettings */

/* 2646 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2648 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2650 */	NdrFcShort( 0xde ),	/* Type Offset=222 */

	/* Return value */

/* 2652 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2654 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2656 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_GloballyOpenPorts */

/* 2658 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2660 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2664 */	NdrFcShort( 0x12 ),	/* 18 */
/* 2666 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2668 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2670 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2672 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2674 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2676 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2678 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2680 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter openPorts */

/* 2682 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2684 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2686 */	NdrFcShort( 0xf4 ),	/* Type Offset=244 */

	/* Return value */

/* 2688 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2690 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2692 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Services */

/* 2694 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2696 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2700 */	NdrFcShort( 0x13 ),	/* 19 */
/* 2702 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2704 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2706 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2708 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2710 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2712 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2714 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2716 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter services */

/* 2718 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2720 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2722 */	NdrFcShort( 0x10a ),	/* Type Offset=266 */

	/* Return value */

/* 2724 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2726 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2728 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_AuthorizedApplications */

/* 2730 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2732 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2736 */	NdrFcShort( 0x14 ),	/* 20 */
/* 2738 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2740 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2742 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2744 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2746 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2748 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2750 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2752 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter apps */

/* 2754 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2756 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2758 */	NdrFcShort( 0x120 ),	/* Type Offset=288 */

	/* Return value */

/* 2760 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2762 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2764 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_CurrentProfile */

/* 2766 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2768 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2772 */	NdrFcShort( 0x7 ),	/* 7 */
/* 2774 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2776 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2778 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2780 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2782 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2784 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2786 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2788 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter profile */

/* 2790 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2792 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2794 */	NdrFcShort( 0x136 ),	/* Type Offset=310 */

	/* Return value */

/* 2796 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2798 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2800 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetProfileByType */

/* 2802 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2804 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2808 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2810 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2812 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2814 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2816 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 2818 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2820 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2822 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2824 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter profileType */

/* 2826 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2828 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2830 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Parameter profile */

/* 2832 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2834 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2836 */	NdrFcShort( 0x136 ),	/* Type Offset=310 */

	/* Return value */

/* 2838 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2840 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2842 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_LocalPolicy */

/* 2844 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2846 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2850 */	NdrFcShort( 0x7 ),	/* 7 */
/* 2852 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2854 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2856 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2858 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2860 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2862 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2864 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2866 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter localPolicy */

/* 2868 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2870 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2872 */	NdrFcShort( 0x14c ),	/* Type Offset=332 */

	/* Return value */

/* 2874 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2876 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2878 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RestoreDefaults */

/* 2880 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2882 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2886 */	NdrFcShort( 0x9 ),	/* 9 */
/* 2888 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2890 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2892 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2894 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 2896 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2898 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2900 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2902 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 2904 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2906 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2908 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure IsPortAllowed */

/* 2910 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2912 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2916 */	NdrFcShort( 0xa ),	/* 10 */
/* 2918 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 2920 */	NdrFcShort( 0x14 ),	/* 20 */
/* 2922 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2924 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x8,		/* 8 */
/* 2926 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 2928 */	NdrFcShort( 0x40 ),	/* 64 */
/* 2930 */	NdrFcShort( 0x2 ),	/* 2 */
/* 2932 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter imageFileName */

/* 2934 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2936 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2938 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Parameter ipVersion */

/* 2940 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2942 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2944 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Parameter portNumber */

/* 2946 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2948 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2950 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter localAddress */

/* 2952 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2954 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2956 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Parameter ipProtocol */

/* 2958 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2960 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2962 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Parameter allowed */

/* 2964 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
/* 2966 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2968 */	NdrFcShort( 0x560 ),	/* Type Offset=1376 */

	/* Parameter restricted */

/* 2970 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
/* 2972 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2974 */	NdrFcShort( 0x560 ),	/* Type Offset=1376 */

	/* Return value */

/* 2976 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2978 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 2980 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure IsIcmpTypeAllowed */

/* 2982 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2984 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2988 */	NdrFcShort( 0xb ),	/* 11 */
/* 2990 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2992 */	NdrFcShort( 0xb ),	/* 11 */
/* 2994 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2996 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 2998 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 3000 */	NdrFcShort( 0x40 ),	/* 64 */
/* 3002 */	NdrFcShort( 0x1 ),	/* 1 */
/* 3004 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ipVersion */

/* 3006 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3008 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3010 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Parameter localAddress */

/* 3012 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 3014 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3016 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Parameter type */

/* 3018 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3020 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3022 */	0x1,		/* FC_BYTE */
			0x0,		/* 0 */

	/* Parameter allowed */

/* 3024 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
/* 3026 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3028 */	NdrFcShort( 0x560 ),	/* Type Offset=1376 */

	/* Parameter restricted */

/* 3030 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
/* 3032 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3034 */	NdrFcShort( 0x560 ),	/* Type Offset=1376 */

	/* Return value */

/* 3036 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3038 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 3040 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/*  4 */	0xd,		/* FC_ENUM16 */
			0x5c,		/* FC_PAD */
/*  6 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/*  8 */	NdrFcShort( 0x1c ),	/* Offset= 28 (36) */
/* 10 */	
			0x13, 0x0,	/* FC_OP */
/* 12 */	NdrFcShort( 0xe ),	/* Offset= 14 (26) */
/* 14 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 16 */	NdrFcShort( 0x2 ),	/* 2 */
/* 18 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 20 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 22 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 24 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 26 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 28 */	NdrFcShort( 0x8 ),	/* 8 */
/* 30 */	NdrFcShort( 0xfff0 ),	/* Offset= -16 (14) */
/* 32 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 34 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 36 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 38 */	NdrFcShort( 0x0 ),	/* 0 */
/* 40 */	NdrFcShort( 0x4 ),	/* 4 */
/* 42 */	NdrFcShort( 0x0 ),	/* 0 */
/* 44 */	NdrFcShort( 0xffde ),	/* Offset= -34 (10) */
/* 46 */	
			0x12, 0x0,	/* FC_UP */
/* 48 */	NdrFcShort( 0xffea ),	/* Offset= -22 (26) */
/* 50 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 52 */	NdrFcShort( 0x0 ),	/* 0 */
/* 54 */	NdrFcShort( 0x4 ),	/* 4 */
/* 56 */	NdrFcShort( 0x0 ),	/* 0 */
/* 58 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (46) */
/* 60 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 62 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 64 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 66 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 68 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 70 */	NdrFcLong( 0xe0483ba0 ),	/* -532137056 */
/* 74 */	NdrFcShort( 0x47ff ),	/* 18431 */
/* 76 */	NdrFcShort( 0x4d9c ),	/* 19868 */
/* 78 */	0xa6,		/* 166 */
			0xd6,		/* 214 */
/* 80 */	0x77,		/* 119 */
			0x41,		/* 65 */
/* 82 */	0xd0,		/* 208 */
			0xb1,		/* 177 */
/* 84 */	0x95,		/* 149 */
			0xf7,		/* 247 */
/* 86 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 88 */	NdrFcShort( 0xffec ),	/* Offset= -20 (68) */
/* 90 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 92 */	NdrFcShort( 0x2 ),	/* Offset= 2 (94) */
/* 94 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 96 */	NdrFcLong( 0x0 ),	/* 0 */
/* 100 */	NdrFcShort( 0x0 ),	/* 0 */
/* 102 */	NdrFcShort( 0x0 ),	/* 0 */
/* 104 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 106 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 108 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 110 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 112 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 114 */	NdrFcShort( 0x2 ),	/* Offset= 2 (116) */
/* 116 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 118 */	NdrFcLong( 0xc0e9d7fa ),	/* -1058416646 */
/* 122 */	NdrFcShort( 0xe07e ),	/* -8066 */
/* 124 */	NdrFcShort( 0x430a ),	/* 17162 */
/* 126 */	0xb1,		/* 177 */
			0x9a,		/* 154 */
/* 128 */	0x9,		/* 9 */
			0xc,		/* 12 */
/* 130 */	0xe8,		/* 232 */
			0x2d,		/* 45 */
/* 132 */	0x92,		/* 146 */
			0xe2,		/* 226 */
/* 134 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 136 */	NdrFcShort( 0x2 ),	/* Offset= 2 (138) */
/* 138 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 140 */	NdrFcLong( 0x79fd57c8 ),	/* 2046646216 */
/* 144 */	NdrFcShort( 0x908e ),	/* -28530 */
/* 146 */	NdrFcShort( 0x4a36 ),	/* 18998 */
/* 148 */	0x98,		/* 152 */
			0x88,		/* 136 */
/* 150 */	0xd5,		/* 213 */
			0xb3,		/* 179 */
/* 152 */	0xf0,		/* 240 */
			0xa4,		/* 164 */
/* 154 */	0x44,		/* 68 */
			0xcf,		/* 207 */
/* 156 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 158 */	NdrFcShort( 0x2 ),	/* Offset= 2 (160) */
/* 160 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 162 */	NdrFcLong( 0x0 ),	/* 0 */
/* 166 */	NdrFcShort( 0x0 ),	/* 0 */
/* 168 */	NdrFcShort( 0x0 ),	/* 0 */
/* 170 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 172 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 174 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 176 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 178 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 180 */	NdrFcLong( 0xb5e64ffa ),	/* -1243197446 */
/* 184 */	NdrFcShort( 0xc2c5 ),	/* -15675 */
/* 186 */	NdrFcShort( 0x444e ),	/* 17486 */
/* 188 */	0xa3,		/* 163 */
			0x1,		/* 1 */
/* 190 */	0xfb,		/* 251 */
			0x5e,		/* 94 */
/* 192 */	0x0,		/* 0 */
			0x1,		/* 1 */
/* 194 */	0x80,		/* 128 */
			0x50,		/* 80 */
/* 196 */	0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 198 */	NdrFcShort( 0xffec ),	/* Offset= -20 (178) */
/* 200 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 202 */	NdrFcShort( 0x2 ),	/* Offset= 2 (204) */
/* 204 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 206 */	NdrFcLong( 0xd4becddf ),	/* -725692961 */
/* 210 */	NdrFcShort( 0x6f73 ),	/* 28531 */
/* 212 */	NdrFcShort( 0x4a83 ),	/* 19075 */
/* 214 */	0xb8,		/* 184 */
			0x32,		/* 50 */
/* 216 */	0x9c,		/* 156 */
			0x66,		/* 102 */
/* 218 */	0x87,		/* 135 */
			0x4c,		/* 76 */
/* 220 */	0xd2,		/* 210 */
			0xe,		/* 14 */
/* 222 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 224 */	NdrFcShort( 0x2 ),	/* Offset= 2 (226) */
/* 226 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 228 */	NdrFcLong( 0xa6207b2e ),	/* -1507820754 */
/* 232 */	NdrFcShort( 0x7cdd ),	/* 31965 */
/* 234 */	NdrFcShort( 0x426a ),	/* 17002 */
/* 236 */	0x95,		/* 149 */
			0x1e,		/* 30 */
/* 238 */	0x5e,		/* 94 */
			0x1c,		/* 28 */
/* 240 */	0xbc,		/* 188 */
			0x5a,		/* 90 */
/* 242 */	0xfe,		/* 254 */
			0xad,		/* 173 */
/* 244 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 246 */	NdrFcShort( 0x2 ),	/* Offset= 2 (248) */
/* 248 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 250 */	NdrFcLong( 0xc0e9d7fa ),	/* -1058416646 */
/* 254 */	NdrFcShort( 0xe07e ),	/* -8066 */
/* 256 */	NdrFcShort( 0x430a ),	/* 17162 */
/* 258 */	0xb1,		/* 177 */
			0x9a,		/* 154 */
/* 260 */	0x9,		/* 9 */
			0xc,		/* 12 */
/* 262 */	0xe8,		/* 232 */
			0x2d,		/* 45 */
/* 264 */	0x92,		/* 146 */
			0xe2,		/* 226 */
/* 266 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 268 */	NdrFcShort( 0x2 ),	/* Offset= 2 (270) */
/* 270 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 272 */	NdrFcLong( 0x79649bb4 ),	/* 2036636596 */
/* 276 */	NdrFcShort( 0x903e ),	/* -28610 */
/* 278 */	NdrFcShort( 0x421b ),	/* 16923 */
/* 280 */	0x94,		/* 148 */
			0xc9,		/* 201 */
/* 282 */	0x79,		/* 121 */
			0x84,		/* 132 */
/* 284 */	0x8e,		/* 142 */
			0x79,		/* 121 */
/* 286 */	0xf6,		/* 246 */
			0xee,		/* 238 */
/* 288 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 290 */	NdrFcShort( 0x2 ),	/* Offset= 2 (292) */
/* 292 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 294 */	NdrFcLong( 0x644efd52 ),	/* 1682898258 */
/* 298 */	NdrFcShort( 0xccf9 ),	/* -13063 */
/* 300 */	NdrFcShort( 0x486c ),	/* 18540 */
/* 302 */	0x97,		/* 151 */
			0xa2,		/* 162 */
/* 304 */	0x39,		/* 57 */
			0xf3,		/* 243 */
/* 306 */	0x52,		/* 82 */
			0x57,		/* 87 */
/* 308 */	0xb,		/* 11 */
			0x30,		/* 48 */
/* 310 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 312 */	NdrFcShort( 0x2 ),	/* Offset= 2 (314) */
/* 314 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 316 */	NdrFcLong( 0x174a0dda ),	/* 390729178 */
/* 320 */	NdrFcShort( 0xe9f9 ),	/* -5639 */
/* 322 */	NdrFcShort( 0x449d ),	/* 17565 */
/* 324 */	0x99,		/* 153 */
			0x3b,		/* 59 */
/* 326 */	0x21,		/* 33 */
			0xab,		/* 171 */
/* 328 */	0x66,		/* 102 */
			0x7c,		/* 124 */
/* 330 */	0xa4,		/* 164 */
			0x56,		/* 86 */
/* 332 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 334 */	NdrFcShort( 0x2 ),	/* Offset= 2 (336) */
/* 336 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 338 */	NdrFcLong( 0xd46d2478 ),	/* -731044744 */
/* 342 */	NdrFcShort( 0x9ac9 ),	/* -25911 */
/* 344 */	NdrFcShort( 0x4008 ),	/* 16392 */
/* 346 */	0x9d,		/* 157 */
			0xc7,		/* 199 */
/* 348 */	0x55,		/* 85 */
			0x63,		/* 99 */
/* 350 */	0xce,		/* 206 */
			0x55,		/* 85 */
/* 352 */	0x36,		/* 54 */
			0xcc,		/* 204 */
/* 354 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 356 */	NdrFcShort( 0x3fc ),	/* Offset= 1020 (1376) */
/* 358 */	
			0x13, 0x0,	/* FC_OP */
/* 360 */	NdrFcShort( 0x3e4 ),	/* Offset= 996 (1356) */
/* 362 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 364 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 366 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 368 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 370 */	NdrFcShort( 0x2 ),	/* Offset= 2 (372) */
/* 372 */	NdrFcShort( 0x10 ),	/* 16 */
/* 374 */	NdrFcShort( 0x2f ),	/* 47 */
/* 376 */	NdrFcLong( 0x14 ),	/* 20 */
/* 380 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 382 */	NdrFcLong( 0x3 ),	/* 3 */
/* 386 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 388 */	NdrFcLong( 0x11 ),	/* 17 */
/* 392 */	NdrFcShort( 0x8001 ),	/* Simple arm type: FC_BYTE */
/* 394 */	NdrFcLong( 0x2 ),	/* 2 */
/* 398 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 400 */	NdrFcLong( 0x4 ),	/* 4 */
/* 404 */	NdrFcShort( 0x800a ),	/* Simple arm type: FC_FLOAT */
/* 406 */	NdrFcLong( 0x5 ),	/* 5 */
/* 410 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 412 */	NdrFcLong( 0xb ),	/* 11 */
/* 416 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 418 */	NdrFcLong( 0xa ),	/* 10 */
/* 422 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 424 */	NdrFcLong( 0x6 ),	/* 6 */
/* 428 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (660) */
/* 430 */	NdrFcLong( 0x7 ),	/* 7 */
/* 434 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 436 */	NdrFcLong( 0x8 ),	/* 8 */
/* 440 */	NdrFcShort( 0xfe52 ),	/* Offset= -430 (10) */
/* 442 */	NdrFcLong( 0xd ),	/* 13 */
/* 446 */	NdrFcShort( 0xdc ),	/* Offset= 220 (666) */
/* 448 */	NdrFcLong( 0x9 ),	/* 9 */
/* 452 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (684) */
/* 454 */	NdrFcLong( 0x2000 ),	/* 8192 */
/* 458 */	NdrFcShort( 0xf4 ),	/* Offset= 244 (702) */
/* 460 */	NdrFcLong( 0x24 ),	/* 36 */
/* 464 */	NdrFcShort( 0x332 ),	/* Offset= 818 (1282) */
/* 466 */	NdrFcLong( 0x4024 ),	/* 16420 */
/* 470 */	NdrFcShort( 0x32c ),	/* Offset= 812 (1282) */
/* 472 */	NdrFcLong( 0x4011 ),	/* 16401 */
/* 476 */	NdrFcShort( 0x32a ),	/* Offset= 810 (1286) */
/* 478 */	NdrFcLong( 0x4002 ),	/* 16386 */
/* 482 */	NdrFcShort( 0x328 ),	/* Offset= 808 (1290) */
/* 484 */	NdrFcLong( 0x4003 ),	/* 16387 */
/* 488 */	NdrFcShort( 0x326 ),	/* Offset= 806 (1294) */
/* 490 */	NdrFcLong( 0x4014 ),	/* 16404 */
/* 494 */	NdrFcShort( 0x324 ),	/* Offset= 804 (1298) */
/* 496 */	NdrFcLong( 0x4004 ),	/* 16388 */
/* 500 */	NdrFcShort( 0x322 ),	/* Offset= 802 (1302) */
/* 502 */	NdrFcLong( 0x4005 ),	/* 16389 */
/* 506 */	NdrFcShort( 0x320 ),	/* Offset= 800 (1306) */
/* 508 */	NdrFcLong( 0x400b ),	/* 16395 */
/* 512 */	NdrFcShort( 0x30a ),	/* Offset= 778 (1290) */
/* 514 */	NdrFcLong( 0x400a ),	/* 16394 */
/* 518 */	NdrFcShort( 0x308 ),	/* Offset= 776 (1294) */
/* 520 */	NdrFcLong( 0x4006 ),	/* 16390 */
/* 524 */	NdrFcShort( 0x312 ),	/* Offset= 786 (1310) */
/* 526 */	NdrFcLong( 0x4007 ),	/* 16391 */
/* 530 */	NdrFcShort( 0x308 ),	/* Offset= 776 (1306) */
/* 532 */	NdrFcLong( 0x4008 ),	/* 16392 */
/* 536 */	NdrFcShort( 0x30a ),	/* Offset= 778 (1314) */
/* 538 */	NdrFcLong( 0x400d ),	/* 16397 */
/* 542 */	NdrFcShort( 0x308 ),	/* Offset= 776 (1318) */
/* 544 */	NdrFcLong( 0x4009 ),	/* 16393 */
/* 548 */	NdrFcShort( 0x306 ),	/* Offset= 774 (1322) */
/* 550 */	NdrFcLong( 0x6000 ),	/* 24576 */
/* 554 */	NdrFcShort( 0x304 ),	/* Offset= 772 (1326) */
/* 556 */	NdrFcLong( 0x400c ),	/* 16396 */
/* 560 */	NdrFcShort( 0x302 ),	/* Offset= 770 (1330) */
/* 562 */	NdrFcLong( 0x10 ),	/* 16 */
/* 566 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 568 */	NdrFcLong( 0x12 ),	/* 18 */
/* 572 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 574 */	NdrFcLong( 0x13 ),	/* 19 */
/* 578 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 580 */	NdrFcLong( 0x15 ),	/* 21 */
/* 584 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 586 */	NdrFcLong( 0x16 ),	/* 22 */
/* 590 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 592 */	NdrFcLong( 0x17 ),	/* 23 */
/* 596 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 598 */	NdrFcLong( 0xe ),	/* 14 */
/* 602 */	NdrFcShort( 0x2e0 ),	/* Offset= 736 (1338) */
/* 604 */	NdrFcLong( 0x400e ),	/* 16398 */
/* 608 */	NdrFcShort( 0x2e4 ),	/* Offset= 740 (1348) */
/* 610 */	NdrFcLong( 0x4010 ),	/* 16400 */
/* 614 */	NdrFcShort( 0x2e2 ),	/* Offset= 738 (1352) */
/* 616 */	NdrFcLong( 0x4012 ),	/* 16402 */
/* 620 */	NdrFcShort( 0x29e ),	/* Offset= 670 (1290) */
/* 622 */	NdrFcLong( 0x4013 ),	/* 16403 */
/* 626 */	NdrFcShort( 0x29c ),	/* Offset= 668 (1294) */
/* 628 */	NdrFcLong( 0x4015 ),	/* 16405 */
/* 632 */	NdrFcShort( 0x29a ),	/* Offset= 666 (1298) */
/* 634 */	NdrFcLong( 0x4016 ),	/* 16406 */
/* 638 */	NdrFcShort( 0x290 ),	/* Offset= 656 (1294) */
/* 640 */	NdrFcLong( 0x4017 ),	/* 16407 */
/* 644 */	NdrFcShort( 0x28a ),	/* Offset= 650 (1294) */
/* 646 */	NdrFcLong( 0x0 ),	/* 0 */
/* 650 */	NdrFcShort( 0x0 ),	/* Offset= 0 (650) */
/* 652 */	NdrFcLong( 0x1 ),	/* 1 */
/* 656 */	NdrFcShort( 0x0 ),	/* Offset= 0 (656) */
/* 658 */	NdrFcShort( 0xffff ),	/* Offset= -1 (657) */
/* 660 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 662 */	NdrFcShort( 0x8 ),	/* 8 */
/* 664 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 666 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 668 */	NdrFcLong( 0x0 ),	/* 0 */
/* 672 */	NdrFcShort( 0x0 ),	/* 0 */
/* 674 */	NdrFcShort( 0x0 ),	/* 0 */
/* 676 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 678 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 680 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 682 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 684 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 686 */	NdrFcLong( 0x20400 ),	/* 132096 */
/* 690 */	NdrFcShort( 0x0 ),	/* 0 */
/* 692 */	NdrFcShort( 0x0 ),	/* 0 */
/* 694 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 696 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 698 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 700 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 702 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 704 */	NdrFcShort( 0x2 ),	/* Offset= 2 (706) */
/* 706 */	
			0x13, 0x0,	/* FC_OP */
/* 708 */	NdrFcShort( 0x22c ),	/* Offset= 556 (1264) */
/* 710 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x49,		/* 73 */
/* 712 */	NdrFcShort( 0x18 ),	/* 24 */
/* 714 */	NdrFcShort( 0xa ),	/* 10 */
/* 716 */	NdrFcLong( 0x8 ),	/* 8 */
/* 720 */	NdrFcShort( 0x5a ),	/* Offset= 90 (810) */
/* 722 */	NdrFcLong( 0xd ),	/* 13 */
/* 726 */	NdrFcShort( 0x7e ),	/* Offset= 126 (852) */
/* 728 */	NdrFcLong( 0x9 ),	/* 9 */
/* 732 */	NdrFcShort( 0x9e ),	/* Offset= 158 (890) */
/* 734 */	NdrFcLong( 0xc ),	/* 12 */
/* 738 */	NdrFcShort( 0xc8 ),	/* Offset= 200 (938) */
/* 740 */	NdrFcLong( 0x24 ),	/* 36 */
/* 744 */	NdrFcShort( 0x124 ),	/* Offset= 292 (1036) */
/* 746 */	NdrFcLong( 0x800d ),	/* 32781 */
/* 750 */	NdrFcShort( 0x156 ),	/* Offset= 342 (1092) */
/* 752 */	NdrFcLong( 0x10 ),	/* 16 */
/* 756 */	NdrFcShort( 0x170 ),	/* Offset= 368 (1124) */
/* 758 */	NdrFcLong( 0x2 ),	/* 2 */
/* 762 */	NdrFcShort( 0x18a ),	/* Offset= 394 (1156) */
/* 764 */	NdrFcLong( 0x3 ),	/* 3 */
/* 768 */	NdrFcShort( 0x1a4 ),	/* Offset= 420 (1188) */
/* 770 */	NdrFcLong( 0x14 ),	/* 20 */
/* 774 */	NdrFcShort( 0x1be ),	/* Offset= 446 (1220) */
/* 776 */	NdrFcShort( 0xffff ),	/* Offset= -1 (775) */
/* 778 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 780 */	NdrFcShort( 0x4 ),	/* 4 */
/* 782 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 784 */	NdrFcShort( 0x0 ),	/* 0 */
/* 786 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 788 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 790 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 792 */	NdrFcShort( 0x4 ),	/* 4 */
/* 794 */	NdrFcShort( 0x0 ),	/* 0 */
/* 796 */	NdrFcShort( 0x1 ),	/* 1 */
/* 798 */	NdrFcShort( 0x0 ),	/* 0 */
/* 800 */	NdrFcShort( 0x0 ),	/* 0 */
/* 802 */	0x13, 0x0,	/* FC_OP */
/* 804 */	NdrFcShort( 0xfcf6 ),	/* Offset= -778 (26) */
/* 806 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 808 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 810 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 812 */	NdrFcShort( 0x8 ),	/* 8 */
/* 814 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 816 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 818 */	NdrFcShort( 0x4 ),	/* 4 */
/* 820 */	NdrFcShort( 0x4 ),	/* 4 */
/* 822 */	0x11, 0x0,	/* FC_RP */
/* 824 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (778) */
/* 826 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 828 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 830 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 832 */	NdrFcShort( 0x0 ),	/* 0 */
/* 834 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 836 */	NdrFcShort( 0x0 ),	/* 0 */
/* 838 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 840 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 844 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 846 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 848 */	NdrFcShort( 0xfd0e ),	/* Offset= -754 (94) */
/* 850 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 852 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 854 */	NdrFcShort( 0x8 ),	/* 8 */
/* 856 */	NdrFcShort( 0x0 ),	/* 0 */
/* 858 */	NdrFcShort( 0x6 ),	/* Offset= 6 (864) */
/* 860 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 862 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 864 */	
			0x11, 0x0,	/* FC_RP */
/* 866 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (830) */
/* 868 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 870 */	NdrFcShort( 0x0 ),	/* 0 */
/* 872 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 874 */	NdrFcShort( 0x0 ),	/* 0 */
/* 876 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 878 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 882 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 884 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 886 */	NdrFcShort( 0xff36 ),	/* Offset= -202 (684) */
/* 888 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 890 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 892 */	NdrFcShort( 0x8 ),	/* 8 */
/* 894 */	NdrFcShort( 0x0 ),	/* 0 */
/* 896 */	NdrFcShort( 0x6 ),	/* Offset= 6 (902) */
/* 898 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 900 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 902 */	
			0x11, 0x0,	/* FC_RP */
/* 904 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (868) */
/* 906 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 908 */	NdrFcShort( 0x4 ),	/* 4 */
/* 910 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 912 */	NdrFcShort( 0x0 ),	/* 0 */
/* 914 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 916 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 918 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 920 */	NdrFcShort( 0x4 ),	/* 4 */
/* 922 */	NdrFcShort( 0x0 ),	/* 0 */
/* 924 */	NdrFcShort( 0x1 ),	/* 1 */
/* 926 */	NdrFcShort( 0x0 ),	/* 0 */
/* 928 */	NdrFcShort( 0x0 ),	/* 0 */
/* 930 */	0x13, 0x0,	/* FC_OP */
/* 932 */	NdrFcShort( 0x1a8 ),	/* Offset= 424 (1356) */
/* 934 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 936 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 938 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 940 */	NdrFcShort( 0x8 ),	/* 8 */
/* 942 */	NdrFcShort( 0x0 ),	/* 0 */
/* 944 */	NdrFcShort( 0x6 ),	/* Offset= 6 (950) */
/* 946 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 948 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 950 */	
			0x11, 0x0,	/* FC_RP */
/* 952 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (906) */
/* 954 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 956 */	NdrFcLong( 0x2f ),	/* 47 */
/* 960 */	NdrFcShort( 0x0 ),	/* 0 */
/* 962 */	NdrFcShort( 0x0 ),	/* 0 */
/* 964 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 966 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 968 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 970 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 972 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 974 */	NdrFcShort( 0x1 ),	/* 1 */
/* 976 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 978 */	NdrFcShort( 0x4 ),	/* 4 */
/* 980 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 982 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 984 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 986 */	NdrFcShort( 0x10 ),	/* 16 */
/* 988 */	NdrFcShort( 0x0 ),	/* 0 */
/* 990 */	NdrFcShort( 0xa ),	/* Offset= 10 (1000) */
/* 992 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 994 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 996 */	NdrFcShort( 0xffd6 ),	/* Offset= -42 (954) */
/* 998 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 1000 */	
			0x13, 0x0,	/* FC_OP */
/* 1002 */	NdrFcShort( 0xffe2 ),	/* Offset= -30 (972) */
/* 1004 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 1006 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1008 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1010 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1012 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1014 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 1016 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 1018 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1020 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1022 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1024 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1026 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1028 */	0x13, 0x0,	/* FC_OP */
/* 1030 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (984) */
/* 1032 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 1034 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1036 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1038 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1040 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1042 */	NdrFcShort( 0x6 ),	/* Offset= 6 (1048) */
/* 1044 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 1046 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1048 */	
			0x11, 0x0,	/* FC_RP */
/* 1050 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (1004) */
/* 1052 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 1054 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1056 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 1058 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 1060 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1062 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 1064 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 1066 */	0x0,		/* 0 */
			NdrFcShort( 0xfff1 ),	/* Offset= -15 (1052) */
			0x5b,		/* FC_END */
/* 1070 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 1072 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1074 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1076 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1078 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1080 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 1084 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 1086 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1088 */	NdrFcShort( 0xfe5a ),	/* Offset= -422 (666) */
/* 1090 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1092 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1094 */	NdrFcShort( 0x18 ),	/* 24 */
/* 1096 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1098 */	NdrFcShort( 0xa ),	/* Offset= 10 (1108) */
/* 1100 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 1102 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1104 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (1058) */
/* 1106 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1108 */	
			0x11, 0x0,	/* FC_RP */
/* 1110 */	NdrFcShort( 0xffd8 ),	/* Offset= -40 (1070) */
/* 1112 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 1114 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1116 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1118 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1120 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1122 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 1124 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 1126 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1128 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 1130 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 1132 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1134 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1136 */	0x13, 0x0,	/* FC_OP */
/* 1138 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (1112) */
/* 1140 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 1142 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1144 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 1146 */	NdrFcShort( 0x2 ),	/* 2 */
/* 1148 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1150 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1152 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1154 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 1156 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 1158 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1160 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 1162 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 1164 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1166 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1168 */	0x13, 0x0,	/* FC_OP */
/* 1170 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (1144) */
/* 1172 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 1174 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1176 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 1178 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1180 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1182 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1184 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1186 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1188 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 1190 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1192 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 1194 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 1196 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1198 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1200 */	0x13, 0x0,	/* FC_OP */
/* 1202 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (1176) */
/* 1204 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 1206 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1208 */	
			0x1b,		/* FC_CARRAY */
			0x7,		/* 7 */
/* 1210 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1212 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1214 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1216 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1218 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 1220 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 1222 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1224 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 1226 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 1228 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1230 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1232 */	0x13, 0x0,	/* FC_OP */
/* 1234 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (1208) */
/* 1236 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 1238 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1240 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 1242 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1244 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1246 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1248 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 1250 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1252 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 1254 */	NdrFcShort( 0xffd8 ),	/* -40 */
/* 1256 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1258 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1260 */	NdrFcShort( 0xffec ),	/* Offset= -20 (1240) */
/* 1262 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1264 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1266 */	NdrFcShort( 0x28 ),	/* 40 */
/* 1268 */	NdrFcShort( 0xffec ),	/* Offset= -20 (1248) */
/* 1270 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1270) */
/* 1272 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1274 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1276 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1278 */	NdrFcShort( 0xfdc8 ),	/* Offset= -568 (710) */
/* 1280 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1282 */	
			0x13, 0x0,	/* FC_OP */
/* 1284 */	NdrFcShort( 0xfed4 ),	/* Offset= -300 (984) */
/* 1286 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1288 */	0x1,		/* FC_BYTE */
			0x5c,		/* FC_PAD */
/* 1290 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1292 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 1294 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1296 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 1298 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1300 */	0xb,		/* FC_HYPER */
			0x5c,		/* FC_PAD */
/* 1302 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1304 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 1306 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1308 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 1310 */	
			0x13, 0x0,	/* FC_OP */
/* 1312 */	NdrFcShort( 0xfd74 ),	/* Offset= -652 (660) */
/* 1314 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1316 */	NdrFcShort( 0xfae6 ),	/* Offset= -1306 (10) */
/* 1318 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1320 */	NdrFcShort( 0xfd72 ),	/* Offset= -654 (666) */
/* 1322 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1324 */	NdrFcShort( 0xfd80 ),	/* Offset= -640 (684) */
/* 1326 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1328 */	NdrFcShort( 0xfd8e ),	/* Offset= -626 (702) */
/* 1330 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1332 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1334) */
/* 1334 */	
			0x13, 0x0,	/* FC_OP */
/* 1336 */	NdrFcShort( 0x14 ),	/* Offset= 20 (1356) */
/* 1338 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 1340 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1342 */	0x6,		/* FC_SHORT */
			0x1,		/* FC_BYTE */
/* 1344 */	0x1,		/* FC_BYTE */
			0x8,		/* FC_LONG */
/* 1346 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 1348 */	
			0x13, 0x0,	/* FC_OP */
/* 1350 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (1338) */
/* 1352 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1354 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 1356 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 1358 */	NdrFcShort( 0x20 ),	/* 32 */
/* 1360 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1362 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1362) */
/* 1364 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1366 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1368 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1370 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1372 */	NdrFcShort( 0xfc0e ),	/* Offset= -1010 (362) */
/* 1374 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1376 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1378 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1380 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1382 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1384 */	NdrFcShort( 0xfbfe ),	/* Offset= -1026 (358) */

			0x0
        }
    };

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            },
            {
            VARIANT_UserSize
            ,VARIANT_UserMarshal
            ,VARIANT_UserUnmarshal
            ,VARIANT_UserFree
            }

        };



/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: INetFwRemoteAdminSettings, ver. 0.0,
   GUID={0xD4BECDDF,0x6F73,0x4A83,{0xB8,0x32,0x9C,0x66,0x87,0x4C,0xD2,0x0E}} */

#pragma code_seg(".orpc")
static const unsigned short INetFwRemoteAdminSettings_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    36,
    72,
    108,
    144,
    180,
    216,
    252
    };

static const MIDL_STUBLESS_PROXY_INFO INetFwRemoteAdminSettings_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &INetFwRemoteAdminSettings_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO INetFwRemoteAdminSettings_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &INetFwRemoteAdminSettings_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(15) _INetFwRemoteAdminSettingsProxyVtbl = 
{
    &INetFwRemoteAdminSettings_ProxyInfo,
    &IID_INetFwRemoteAdminSettings,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* INetFwRemoteAdminSettings::get_IpVersion */ ,
    (void *) (INT_PTR) -1 /* INetFwRemoteAdminSettings::put_IpVersion */ ,
    (void *) (INT_PTR) -1 /* INetFwRemoteAdminSettings::get_Scope */ ,
    (void *) (INT_PTR) -1 /* INetFwRemoteAdminSettings::put_Scope */ ,
    (void *) (INT_PTR) -1 /* INetFwRemoteAdminSettings::get_RemoteAddresses */ ,
    (void *) (INT_PTR) -1 /* INetFwRemoteAdminSettings::put_RemoteAddresses */ ,
    (void *) (INT_PTR) -1 /* INetFwRemoteAdminSettings::get_Enabled */ ,
    (void *) (INT_PTR) -1 /* INetFwRemoteAdminSettings::put_Enabled */
};


static const PRPC_STUB_FUNCTION INetFwRemoteAdminSettings_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _INetFwRemoteAdminSettingsStubVtbl =
{
    &IID_INetFwRemoteAdminSettings,
    &INetFwRemoteAdminSettings_ServerInfo,
    15,
    &INetFwRemoteAdminSettings_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: INetFwIcmpSettings, ver. 0.0,
   GUID={0xA6207B2E,0x7CDD,0x426A,{0x95,0x1E,0x5E,0x1C,0xBC,0x5A,0xFE,0xAD}} */

#pragma code_seg(".orpc")
static const unsigned short INetFwIcmpSettings_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    288,
    324,
    360,
    396,
    432,
    468,
    216,
    252,
    504,
    540,
    576,
    612,
    648,
    684,
    720,
    756,
    792,
    828,
    864,
    900
    };

static const MIDL_STUBLESS_PROXY_INFO INetFwIcmpSettings_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &INetFwIcmpSettings_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO INetFwIcmpSettings_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &INetFwIcmpSettings_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(27) _INetFwIcmpSettingsProxyVtbl = 
{
    &INetFwIcmpSettings_ProxyInfo,
    &IID_INetFwIcmpSettings,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::get_AllowOutboundDestinationUnreachable */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::put_AllowOutboundDestinationUnreachable */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::get_AllowRedirect */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::put_AllowRedirect */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::get_AllowInboundEchoRequest */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::put_AllowInboundEchoRequest */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::get_AllowOutboundTimeExceeded */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::put_AllowOutboundTimeExceeded */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::get_AllowOutboundParameterProblem */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::put_AllowOutboundParameterProblem */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::get_AllowOutboundSourceQuench */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::put_AllowOutboundSourceQuench */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::get_AllowInboundRouterRequest */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::put_AllowInboundRouterRequest */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::get_AllowInboundTimestampRequest */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::put_AllowInboundTimestampRequest */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::get_AllowInboundMaskRequest */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::put_AllowInboundMaskRequest */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::get_AllowOutboundPacketTooBig */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::put_AllowOutboundPacketTooBig */
};


static const PRPC_STUB_FUNCTION INetFwIcmpSettings_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _INetFwIcmpSettingsStubVtbl =
{
    &IID_INetFwIcmpSettings,
    &INetFwIcmpSettings_ServerInfo,
    27,
    &INetFwIcmpSettings_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: INetFwOpenPort, ver. 0.0,
   GUID={0xE0483BA0,0x47FF,0x4D9C,{0xA6,0xD6,0x77,0x41,0xD0,0xB1,0x95,0xF7}} */

#pragma code_seg(".orpc")
static const unsigned short INetFwOpenPort_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    936,
    972,
    72,
    108,
    1008,
    1044,
    1080,
    1116,
    1152,
    1188,
    1224,
    1260,
    648,
    684,
    720
    };

static const MIDL_STUBLESS_PROXY_INFO INetFwOpenPort_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &INetFwOpenPort_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO INetFwOpenPort_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &INetFwOpenPort_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(22) _INetFwOpenPortProxyVtbl = 
{
    &INetFwOpenPort_ProxyInfo,
    &IID_INetFwOpenPort,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::get_Name */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::put_Name */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::get_IpVersion */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::put_IpVersion */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::get_Protocol */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::put_Protocol */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::get_Port */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::put_Port */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::get_Scope */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::put_Scope */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::get_RemoteAddresses */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::put_RemoteAddresses */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::get_Enabled */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::put_Enabled */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::get_BuiltIn */
};


static const PRPC_STUB_FUNCTION INetFwOpenPort_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _INetFwOpenPortStubVtbl =
{
    &IID_INetFwOpenPort,
    &INetFwOpenPort_ServerInfo,
    22,
    &INetFwOpenPort_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: INetFwOpenPorts, ver. 0.0,
   GUID={0xC0E9D7FA,0xE07E,0x430A,{0xB1,0x9A,0x09,0x0C,0xE8,0x2D,0x92,0xE2}} */

#pragma code_seg(".orpc")
static const unsigned short INetFwOpenPorts_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    1296,
    1332,
    1368,
    1410,
    1458
    };

static const MIDL_STUBLESS_PROXY_INFO INetFwOpenPorts_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &INetFwOpenPorts_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO INetFwOpenPorts_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &INetFwOpenPorts_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(12) _INetFwOpenPortsProxyVtbl = 
{
    &INetFwOpenPorts_ProxyInfo,
    &IID_INetFwOpenPorts,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPorts::get_Count */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPorts::Add */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPorts::Remove */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPorts::Item */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPorts::get__NewEnum */
};


static const PRPC_STUB_FUNCTION INetFwOpenPorts_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _INetFwOpenPortsStubVtbl =
{
    &IID_INetFwOpenPorts,
    &INetFwOpenPorts_ServerInfo,
    12,
    &INetFwOpenPorts_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: INetFwService, ver. 0.0,
   GUID={0x79FD57C8,0x908E,0x4A36,{0x98,0x88,0xD5,0xB3,0xF0,0xA4,0x44,0xCF}} */

#pragma code_seg(".orpc")
static const unsigned short INetFwService_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    936,
    1494,
    360,
    1530,
    1566,
    1602,
    1638,
    1674,
    1710,
    1746,
    1782,
    1818
    };

static const MIDL_STUBLESS_PROXY_INFO INetFwService_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &INetFwService_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO INetFwService_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &INetFwService_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(19) _INetFwServiceProxyVtbl = 
{
    &INetFwService_ProxyInfo,
    &IID_INetFwService,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* INetFwService::get_Name */ ,
    (void *) (INT_PTR) -1 /* INetFwService::get_Type */ ,
    (void *) (INT_PTR) -1 /* INetFwService::get_Customized */ ,
    (void *) (INT_PTR) -1 /* INetFwService::get_IpVersion */ ,
    (void *) (INT_PTR) -1 /* INetFwService::put_IpVersion */ ,
    (void *) (INT_PTR) -1 /* INetFwService::get_Scope */ ,
    (void *) (INT_PTR) -1 /* INetFwService::put_Scope */ ,
    (void *) (INT_PTR) -1 /* INetFwService::get_RemoteAddresses */ ,
    (void *) (INT_PTR) -1 /* INetFwService::put_RemoteAddresses */ ,
    (void *) (INT_PTR) -1 /* INetFwService::get_Enabled */ ,
    (void *) (INT_PTR) -1 /* INetFwService::put_Enabled */ ,
    (void *) (INT_PTR) -1 /* INetFwService::get_GloballyOpenPorts */
};


static const PRPC_STUB_FUNCTION INetFwService_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _INetFwServiceStubVtbl =
{
    &IID_INetFwService,
    &INetFwService_ServerInfo,
    19,
    &INetFwService_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: INetFwServices, ver. 0.0,
   GUID={0x79649BB4,0x903E,0x421B,{0x94,0xC9,0x79,0x84,0x8E,0x79,0xF6,0xEE}} */

#pragma code_seg(".orpc")
static const unsigned short INetFwServices_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    1296,
    1854,
    1896
    };

static const MIDL_STUBLESS_PROXY_INFO INetFwServices_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &INetFwServices_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO INetFwServices_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &INetFwServices_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(10) _INetFwServicesProxyVtbl = 
{
    &INetFwServices_ProxyInfo,
    &IID_INetFwServices,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* INetFwServices::get_Count */ ,
    (void *) (INT_PTR) -1 /* INetFwServices::Item */ ,
    (void *) (INT_PTR) -1 /* INetFwServices::get__NewEnum */
};


static const PRPC_STUB_FUNCTION INetFwServices_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _INetFwServicesStubVtbl =
{
    &IID_INetFwServices,
    &INetFwServices_ServerInfo,
    10,
    &INetFwServices_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: INetFwAuthorizedApplication, ver. 0.0,
   GUID={0xB5E64FFA,0xC2C5,0x444E,{0xA3,0x01,0xFB,0x5E,0x00,0x01,0x80,0x50}} */

#pragma code_seg(".orpc")
static const unsigned short INetFwAuthorizedApplication_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    936,
    972,
    1932,
    1968,
    1008,
    1044,
    2004,
    2040,
    2076,
    2112,
    576,
    612
    };

static const MIDL_STUBLESS_PROXY_INFO INetFwAuthorizedApplication_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &INetFwAuthorizedApplication_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO INetFwAuthorizedApplication_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &INetFwAuthorizedApplication_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(19) _INetFwAuthorizedApplicationProxyVtbl = 
{
    &INetFwAuthorizedApplication_ProxyInfo,
    &IID_INetFwAuthorizedApplication,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplication::get_Name */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplication::put_Name */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplication::get_ProcessImageFileName */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplication::put_ProcessImageFileName */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplication::get_IpVersion */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplication::put_IpVersion */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplication::get_Scope */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplication::put_Scope */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplication::get_RemoteAddresses */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplication::put_RemoteAddresses */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplication::get_Enabled */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplication::put_Enabled */
};


static const PRPC_STUB_FUNCTION INetFwAuthorizedApplication_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _INetFwAuthorizedApplicationStubVtbl =
{
    &IID_INetFwAuthorizedApplication,
    &INetFwAuthorizedApplication_ServerInfo,
    19,
    &INetFwAuthorizedApplication_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: INetFwAuthorizedApplications, ver. 0.0,
   GUID={0x644EFD52,0xCCF9,0x486C,{0x97,0xA2,0x39,0xF3,0x52,0x57,0x0B,0x30}} */

#pragma code_seg(".orpc")
static const unsigned short INetFwAuthorizedApplications_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    1296,
    2148,
    2184,
    2220,
    2262
    };

static const MIDL_STUBLESS_PROXY_INFO INetFwAuthorizedApplications_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &INetFwAuthorizedApplications_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO INetFwAuthorizedApplications_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &INetFwAuthorizedApplications_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(12) _INetFwAuthorizedApplicationsProxyVtbl = 
{
    &INetFwAuthorizedApplications_ProxyInfo,
    &IID_INetFwAuthorizedApplications,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplications::get_Count */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplications::Add */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplications::Remove */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplications::Item */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplications::get__NewEnum */
};


static const PRPC_STUB_FUNCTION INetFwAuthorizedApplications_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _INetFwAuthorizedApplicationsStubVtbl =
{
    &IID_INetFwAuthorizedApplications,
    &INetFwAuthorizedApplications_ServerInfo,
    12,
    &INetFwAuthorizedApplications_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: INetFwProfile, ver. 0.0,
   GUID={0x174A0DDA,0xE9F9,0x449D,{0x99,0x3B,0x21,0xAB,0x66,0x7C,0xA4,0x56}} */

#pragma code_seg(".orpc")
static const unsigned short INetFwProfile_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    2298,
    2334,
    2370,
    2406,
    2442,
    2478,
    2514,
    2550,
    2586,
    2622,
    2658,
    2694,
    2730
    };

static const MIDL_STUBLESS_PROXY_INFO INetFwProfile_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &INetFwProfile_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO INetFwProfile_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &INetFwProfile_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(21) _INetFwProfileProxyVtbl = 
{
    &INetFwProfile_ProxyInfo,
    &IID_INetFwProfile,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* INetFwProfile::get_Type */ ,
    (void *) (INT_PTR) -1 /* INetFwProfile::get_FirewallEnabled */ ,
    (void *) (INT_PTR) -1 /* INetFwProfile::put_FirewallEnabled */ ,
    (void *) (INT_PTR) -1 /* INetFwProfile::get_ExceptionsNotAllowed */ ,
    (void *) (INT_PTR) -1 /* INetFwProfile::put_ExceptionsNotAllowed */ ,
    (void *) (INT_PTR) -1 /* INetFwProfile::get_NotificationsDisabled */ ,
    (void *) (INT_PTR) -1 /* INetFwProfile::put_NotificationsDisabled */ ,
    (void *) (INT_PTR) -1 /* INetFwProfile::get_UnicastResponsesToMulticastBroadcastDisabled */ ,
    (void *) (INT_PTR) -1 /* INetFwProfile::put_UnicastResponsesToMulticastBroadcastDisabled */ ,
    (void *) (INT_PTR) -1 /* INetFwProfile::get_RemoteAdminSettings */ ,
    (void *) (INT_PTR) -1 /* INetFwProfile::get_IcmpSettings */ ,
    (void *) (INT_PTR) -1 /* INetFwProfile::get_GloballyOpenPorts */ ,
    (void *) (INT_PTR) -1 /* INetFwProfile::get_Services */ ,
    (void *) (INT_PTR) -1 /* INetFwProfile::get_AuthorizedApplications */
};


static const PRPC_STUB_FUNCTION INetFwProfile_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _INetFwProfileStubVtbl =
{
    &IID_INetFwProfile,
    &INetFwProfile_ServerInfo,
    21,
    &INetFwProfile_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: INetFwPolicy, ver. 0.0,
   GUID={0xD46D2478,0x9AC9,0x4008,{0x9D,0xC7,0x55,0x63,0xCE,0x55,0x36,0xCC}} */

#pragma code_seg(".orpc")
static const unsigned short INetFwPolicy_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    2766,
    2802
    };

static const MIDL_STUBLESS_PROXY_INFO INetFwPolicy_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &INetFwPolicy_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO INetFwPolicy_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &INetFwPolicy_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(9) _INetFwPolicyProxyVtbl = 
{
    &INetFwPolicy_ProxyInfo,
    &IID_INetFwPolicy,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* INetFwPolicy::get_CurrentProfile */ ,
    (void *) (INT_PTR) -1 /* INetFwPolicy::GetProfileByType */
};


static const PRPC_STUB_FUNCTION INetFwPolicy_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _INetFwPolicyStubVtbl =
{
    &IID_INetFwPolicy,
    &INetFwPolicy_ServerInfo,
    9,
    &INetFwPolicy_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: INetFwMgr, ver. 0.0,
   GUID={0xF7898AF5,0xCAC4,0x4632,{0xA2,0xEC,0xDA,0x06,0xE5,0x11,0x1A,0xF2}} */

#pragma code_seg(".orpc")
static const unsigned short INetFwMgr_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    2844,
    1494,
    2880,
    2910,
    2982
    };

static const MIDL_STUBLESS_PROXY_INFO INetFwMgr_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &INetFwMgr_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO INetFwMgr_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &INetFwMgr_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(12) _INetFwMgrProxyVtbl = 
{
    &INetFwMgr_ProxyInfo,
    &IID_INetFwMgr,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* INetFwMgr::get_LocalPolicy */ ,
    (void *) (INT_PTR) -1 /* INetFwMgr::get_CurrentProfileType */ ,
    (void *) (INT_PTR) -1 /* INetFwMgr::RestoreDefaults */ ,
    (void *) (INT_PTR) -1 /* INetFwMgr::IsPortAllowed */ ,
    (void *) (INT_PTR) -1 /* INetFwMgr::IsIcmpTypeAllowed */
};


static const PRPC_STUB_FUNCTION INetFwMgr_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _INetFwMgrStubVtbl =
{
    &IID_INetFwMgr,
    &INetFwMgr_ServerInfo,
    12,
    &INetFwMgr_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    __MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x6000169, /* MIDL Version 6.0.361 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0   /* Reserved5 */
    };

const CInterfaceProxyVtbl * _netfw_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_INetFwIcmpSettingsProxyVtbl,
    ( CInterfaceProxyVtbl *) &_INetFwAuthorizedApplicationsProxyVtbl,
    ( CInterfaceProxyVtbl *) &_INetFwPolicyProxyVtbl,
    ( CInterfaceProxyVtbl *) &_INetFwOpenPortProxyVtbl,
    ( CInterfaceProxyVtbl *) &_INetFwServicesProxyVtbl,
    ( CInterfaceProxyVtbl *) &_INetFwServiceProxyVtbl,
    ( CInterfaceProxyVtbl *) &_INetFwProfileProxyVtbl,
    ( CInterfaceProxyVtbl *) &_INetFwRemoteAdminSettingsProxyVtbl,
    ( CInterfaceProxyVtbl *) &_INetFwMgrProxyVtbl,
    ( CInterfaceProxyVtbl *) &_INetFwAuthorizedApplicationProxyVtbl,
    ( CInterfaceProxyVtbl *) &_INetFwOpenPortsProxyVtbl,
    0
};

const CInterfaceStubVtbl * _netfw_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_INetFwIcmpSettingsStubVtbl,
    ( CInterfaceStubVtbl *) &_INetFwAuthorizedApplicationsStubVtbl,
    ( CInterfaceStubVtbl *) &_INetFwPolicyStubVtbl,
    ( CInterfaceStubVtbl *) &_INetFwOpenPortStubVtbl,
    ( CInterfaceStubVtbl *) &_INetFwServicesStubVtbl,
    ( CInterfaceStubVtbl *) &_INetFwServiceStubVtbl,
    ( CInterfaceStubVtbl *) &_INetFwProfileStubVtbl,
    ( CInterfaceStubVtbl *) &_INetFwRemoteAdminSettingsStubVtbl,
    ( CInterfaceStubVtbl *) &_INetFwMgrStubVtbl,
    ( CInterfaceStubVtbl *) &_INetFwAuthorizedApplicationStubVtbl,
    ( CInterfaceStubVtbl *) &_INetFwOpenPortsStubVtbl,
    0
};

PCInterfaceName const _netfw_InterfaceNamesList[] = 
{
    "INetFwIcmpSettings",
    "INetFwAuthorizedApplications",
    "INetFwPolicy",
    "INetFwOpenPort",
    "INetFwServices",
    "INetFwService",
    "INetFwProfile",
    "INetFwRemoteAdminSettings",
    "INetFwMgr",
    "INetFwAuthorizedApplication",
    "INetFwOpenPorts",
    0
};

const IID *  _netfw_BaseIIDList[] = 
{
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    0
};


#define _netfw_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _netfw, pIID, n)

int __stdcall _netfw_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _netfw, 11, 8 )
    IID_BS_LOOKUP_NEXT_TEST( _netfw, 4 )
    IID_BS_LOOKUP_NEXT_TEST( _netfw, 2 )
    IID_BS_LOOKUP_NEXT_TEST( _netfw, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _netfw, 11, *pIndex )
    
}

const ExtendedProxyFileInfo netfw_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _netfw_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _netfw_StubVtblList,
    (const PCInterfaceName * ) & _netfw_InterfaceNamesList,
    (const IID ** ) & _netfw_BaseIIDList,
    & _netfw_IID_Lookup, 
    11,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/



/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Tue Aug 17 12:54:40 2004
 */
/* Compiler settings for netfw.idl:
    Oicf, W1, Zp8, env=Win64 (32b run,appending)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if defined(_M_IA64) || defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extent to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "netfw.h"

#define TYPE_FORMAT_STRING_SIZE   1333                              
#define PROC_FORMAT_STRING_SIZE   3207                              
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   2            

typedef struct _MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } MIDL_PROC_FORMAT_STRING;


static RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INetFwRemoteAdminSettings_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INetFwRemoteAdminSettings_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INetFwIcmpSettings_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INetFwIcmpSettings_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INetFwOpenPort_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INetFwOpenPort_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INetFwOpenPorts_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INetFwOpenPorts_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INetFwService_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INetFwService_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INetFwServices_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INetFwServices_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INetFwAuthorizedApplication_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INetFwAuthorizedApplication_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INetFwAuthorizedApplications_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INetFwAuthorizedApplications_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INetFwProfile_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INetFwProfile_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INetFwPolicy_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INetFwPolicy_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INetFwMgr_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INetFwMgr_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif

static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure get_Type */


	/* Procedure get_IpVersion */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
/*  8 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x22 ),	/* 34 */
/* 14 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 16 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 18 */	NdrFcShort( 0x0 ),	/* 0 */
/* 20 */	NdrFcShort( 0x0 ),	/* 0 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */
/* 24 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter type */


	/* Parameter ipVersion */

/* 26 */	NdrFcShort( 0x2010 ),	/* Flags:  out, srv alloc size=8 */
/* 28 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 30 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */


	/* Return value */

/* 32 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 34 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 36 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_IpVersion */

/* 38 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 40 */	NdrFcLong( 0x0 ),	/* 0 */
/* 44 */	NdrFcShort( 0x8 ),	/* 8 */
/* 46 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 48 */	NdrFcShort( 0x6 ),	/* 6 */
/* 50 */	NdrFcShort( 0x8 ),	/* 8 */
/* 52 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 54 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 56 */	NdrFcShort( 0x0 ),	/* 0 */
/* 58 */	NdrFcShort( 0x0 ),	/* 0 */
/* 60 */	NdrFcShort( 0x0 ),	/* 0 */
/* 62 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ipVersion */

/* 64 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 66 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 68 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Return value */

/* 70 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 72 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 74 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_IpVersion */


	/* Procedure get_Scope */

/* 76 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 78 */	NdrFcLong( 0x0 ),	/* 0 */
/* 82 */	NdrFcShort( 0x9 ),	/* 9 */
/* 84 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 86 */	NdrFcShort( 0x0 ),	/* 0 */
/* 88 */	NdrFcShort( 0x22 ),	/* 34 */
/* 90 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 92 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 94 */	NdrFcShort( 0x0 ),	/* 0 */
/* 96 */	NdrFcShort( 0x0 ),	/* 0 */
/* 98 */	NdrFcShort( 0x0 ),	/* 0 */
/* 100 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ipVersion */


	/* Parameter scope */

/* 102 */	NdrFcShort( 0x2010 ),	/* Flags:  out, srv alloc size=8 */
/* 104 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 106 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */


	/* Return value */

/* 108 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 110 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 112 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_IpVersion */


	/* Procedure put_Scope */

/* 114 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 116 */	NdrFcLong( 0x0 ),	/* 0 */
/* 120 */	NdrFcShort( 0xa ),	/* 10 */
/* 122 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 124 */	NdrFcShort( 0x6 ),	/* 6 */
/* 126 */	NdrFcShort( 0x8 ),	/* 8 */
/* 128 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 130 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 132 */	NdrFcShort( 0x0 ),	/* 0 */
/* 134 */	NdrFcShort( 0x0 ),	/* 0 */
/* 136 */	NdrFcShort( 0x0 ),	/* 0 */
/* 138 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ipVersion */


	/* Parameter scope */

/* 140 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 142 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 144 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 146 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 148 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 150 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_RemoteAddresses */

/* 152 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 154 */	NdrFcLong( 0x0 ),	/* 0 */
/* 158 */	NdrFcShort( 0xb ),	/* 11 */
/* 160 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 162 */	NdrFcShort( 0x0 ),	/* 0 */
/* 164 */	NdrFcShort( 0x8 ),	/* 8 */
/* 166 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 168 */	0xa,		/* 10 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 170 */	NdrFcShort( 0x1 ),	/* 1 */
/* 172 */	NdrFcShort( 0x0 ),	/* 0 */
/* 174 */	NdrFcShort( 0x0 ),	/* 0 */
/* 176 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter remoteAddrs */

/* 178 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 180 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 182 */	NdrFcShort( 0x24 ),	/* Type Offset=36 */

	/* Return value */

/* 184 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 186 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 188 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_RemoteAddresses */

/* 190 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 192 */	NdrFcLong( 0x0 ),	/* 0 */
/* 196 */	NdrFcShort( 0xc ),	/* 12 */
/* 198 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 200 */	NdrFcShort( 0x0 ),	/* 0 */
/* 202 */	NdrFcShort( 0x8 ),	/* 8 */
/* 204 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 206 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 208 */	NdrFcShort( 0x0 ),	/* 0 */
/* 210 */	NdrFcShort( 0x1 ),	/* 1 */
/* 212 */	NdrFcShort( 0x0 ),	/* 0 */
/* 214 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter remoteAddrs */

/* 216 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 218 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 220 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Return value */

/* 222 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 224 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 226 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_AllowOutboundTimeExceeded */


	/* Procedure get_Enabled */

/* 228 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 230 */	NdrFcLong( 0x0 ),	/* 0 */
/* 234 */	NdrFcShort( 0xd ),	/* 13 */
/* 236 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 238 */	NdrFcShort( 0x0 ),	/* 0 */
/* 240 */	NdrFcShort( 0x22 ),	/* 34 */
/* 242 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 244 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 246 */	NdrFcShort( 0x0 ),	/* 0 */
/* 248 */	NdrFcShort( 0x0 ),	/* 0 */
/* 250 */	NdrFcShort( 0x0 ),	/* 0 */
/* 252 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter allow */


	/* Parameter enabled */

/* 254 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 256 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 258 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 260 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 262 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 264 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_AllowOutboundTimeExceeded */


	/* Procedure put_Enabled */

/* 266 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 268 */	NdrFcLong( 0x0 ),	/* 0 */
/* 272 */	NdrFcShort( 0xe ),	/* 14 */
/* 274 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 276 */	NdrFcShort( 0x6 ),	/* 6 */
/* 278 */	NdrFcShort( 0x8 ),	/* 8 */
/* 280 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 282 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 284 */	NdrFcShort( 0x0 ),	/* 0 */
/* 286 */	NdrFcShort( 0x0 ),	/* 0 */
/* 288 */	NdrFcShort( 0x0 ),	/* 0 */
/* 290 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter allow */


	/* Parameter enabled */

/* 292 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 294 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 296 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 298 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 300 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 302 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_AllowOutboundDestinationUnreachable */

/* 304 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 306 */	NdrFcLong( 0x0 ),	/* 0 */
/* 310 */	NdrFcShort( 0x7 ),	/* 7 */
/* 312 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 314 */	NdrFcShort( 0x0 ),	/* 0 */
/* 316 */	NdrFcShort( 0x22 ),	/* 34 */
/* 318 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 320 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 322 */	NdrFcShort( 0x0 ),	/* 0 */
/* 324 */	NdrFcShort( 0x0 ),	/* 0 */
/* 326 */	NdrFcShort( 0x0 ),	/* 0 */
/* 328 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter allow */

/* 330 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 332 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 334 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 336 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 338 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 340 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_AllowOutboundDestinationUnreachable */

/* 342 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 344 */	NdrFcLong( 0x0 ),	/* 0 */
/* 348 */	NdrFcShort( 0x8 ),	/* 8 */
/* 350 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 352 */	NdrFcShort( 0x6 ),	/* 6 */
/* 354 */	NdrFcShort( 0x8 ),	/* 8 */
/* 356 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 358 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 360 */	NdrFcShort( 0x0 ),	/* 0 */
/* 362 */	NdrFcShort( 0x0 ),	/* 0 */
/* 364 */	NdrFcShort( 0x0 ),	/* 0 */
/* 366 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter allow */

/* 368 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 370 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 372 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 374 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 376 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 378 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Customized */


	/* Procedure get_AllowRedirect */

/* 380 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 382 */	NdrFcLong( 0x0 ),	/* 0 */
/* 386 */	NdrFcShort( 0x9 ),	/* 9 */
/* 388 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 390 */	NdrFcShort( 0x0 ),	/* 0 */
/* 392 */	NdrFcShort( 0x22 ),	/* 34 */
/* 394 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 396 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 398 */	NdrFcShort( 0x0 ),	/* 0 */
/* 400 */	NdrFcShort( 0x0 ),	/* 0 */
/* 402 */	NdrFcShort( 0x0 ),	/* 0 */
/* 404 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter customized */


	/* Parameter allow */

/* 406 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 408 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 410 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 412 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 414 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 416 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_AllowRedirect */

/* 418 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 420 */	NdrFcLong( 0x0 ),	/* 0 */
/* 424 */	NdrFcShort( 0xa ),	/* 10 */
/* 426 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 428 */	NdrFcShort( 0x6 ),	/* 6 */
/* 430 */	NdrFcShort( 0x8 ),	/* 8 */
/* 432 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 434 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 436 */	NdrFcShort( 0x0 ),	/* 0 */
/* 438 */	NdrFcShort( 0x0 ),	/* 0 */
/* 440 */	NdrFcShort( 0x0 ),	/* 0 */
/* 442 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter allow */

/* 444 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 446 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 448 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 450 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 452 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 454 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_AllowInboundEchoRequest */

/* 456 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 458 */	NdrFcLong( 0x0 ),	/* 0 */
/* 462 */	NdrFcShort( 0xb ),	/* 11 */
/* 464 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 466 */	NdrFcShort( 0x0 ),	/* 0 */
/* 468 */	NdrFcShort( 0x22 ),	/* 34 */
/* 470 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 472 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 474 */	NdrFcShort( 0x0 ),	/* 0 */
/* 476 */	NdrFcShort( 0x0 ),	/* 0 */
/* 478 */	NdrFcShort( 0x0 ),	/* 0 */
/* 480 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter allow */

/* 482 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 484 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 486 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 488 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 490 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 492 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_AllowInboundEchoRequest */

/* 494 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 496 */	NdrFcLong( 0x0 ),	/* 0 */
/* 500 */	NdrFcShort( 0xc ),	/* 12 */
/* 502 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 504 */	NdrFcShort( 0x6 ),	/* 6 */
/* 506 */	NdrFcShort( 0x8 ),	/* 8 */
/* 508 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 510 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 512 */	NdrFcShort( 0x0 ),	/* 0 */
/* 514 */	NdrFcShort( 0x0 ),	/* 0 */
/* 516 */	NdrFcShort( 0x0 ),	/* 0 */
/* 518 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter allow */

/* 520 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 522 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 524 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 526 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 528 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 530 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_AllowOutboundParameterProblem */

/* 532 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 534 */	NdrFcLong( 0x0 ),	/* 0 */
/* 538 */	NdrFcShort( 0xf ),	/* 15 */
/* 540 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 542 */	NdrFcShort( 0x0 ),	/* 0 */
/* 544 */	NdrFcShort( 0x22 ),	/* 34 */
/* 546 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 548 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 550 */	NdrFcShort( 0x0 ),	/* 0 */
/* 552 */	NdrFcShort( 0x0 ),	/* 0 */
/* 554 */	NdrFcShort( 0x0 ),	/* 0 */
/* 556 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter allow */

/* 558 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 560 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 562 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 564 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 566 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 568 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_AllowOutboundParameterProblem */

/* 570 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 572 */	NdrFcLong( 0x0 ),	/* 0 */
/* 576 */	NdrFcShort( 0x10 ),	/* 16 */
/* 578 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 580 */	NdrFcShort( 0x6 ),	/* 6 */
/* 582 */	NdrFcShort( 0x8 ),	/* 8 */
/* 584 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 586 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 588 */	NdrFcShort( 0x0 ),	/* 0 */
/* 590 */	NdrFcShort( 0x0 ),	/* 0 */
/* 592 */	NdrFcShort( 0x0 ),	/* 0 */
/* 594 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter allow */

/* 596 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 598 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 600 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 602 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 604 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 606 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Enabled */


	/* Procedure get_AllowOutboundSourceQuench */

/* 608 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 610 */	NdrFcLong( 0x0 ),	/* 0 */
/* 614 */	NdrFcShort( 0x11 ),	/* 17 */
/* 616 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 618 */	NdrFcShort( 0x0 ),	/* 0 */
/* 620 */	NdrFcShort( 0x22 ),	/* 34 */
/* 622 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 624 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 626 */	NdrFcShort( 0x0 ),	/* 0 */
/* 628 */	NdrFcShort( 0x0 ),	/* 0 */
/* 630 */	NdrFcShort( 0x0 ),	/* 0 */
/* 632 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter enabled */


	/* Parameter allow */

/* 634 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 636 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 638 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 640 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 642 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 644 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Enabled */


	/* Procedure put_AllowOutboundSourceQuench */

/* 646 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 648 */	NdrFcLong( 0x0 ),	/* 0 */
/* 652 */	NdrFcShort( 0x12 ),	/* 18 */
/* 654 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 656 */	NdrFcShort( 0x6 ),	/* 6 */
/* 658 */	NdrFcShort( 0x8 ),	/* 8 */
/* 660 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 662 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 664 */	NdrFcShort( 0x0 ),	/* 0 */
/* 666 */	NdrFcShort( 0x0 ),	/* 0 */
/* 668 */	NdrFcShort( 0x0 ),	/* 0 */
/* 670 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter enabled */


	/* Parameter allow */

/* 672 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 674 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 676 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 678 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 680 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 682 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Enabled */


	/* Procedure get_AllowInboundRouterRequest */

/* 684 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 686 */	NdrFcLong( 0x0 ),	/* 0 */
/* 690 */	NdrFcShort( 0x13 ),	/* 19 */
/* 692 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 694 */	NdrFcShort( 0x0 ),	/* 0 */
/* 696 */	NdrFcShort( 0x22 ),	/* 34 */
/* 698 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 700 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 702 */	NdrFcShort( 0x0 ),	/* 0 */
/* 704 */	NdrFcShort( 0x0 ),	/* 0 */
/* 706 */	NdrFcShort( 0x0 ),	/* 0 */
/* 708 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter enabled */


	/* Parameter allow */

/* 710 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 712 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 714 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 716 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 718 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 720 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Enabled */


	/* Procedure put_AllowInboundRouterRequest */

/* 722 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 724 */	NdrFcLong( 0x0 ),	/* 0 */
/* 728 */	NdrFcShort( 0x14 ),	/* 20 */
/* 730 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 732 */	NdrFcShort( 0x6 ),	/* 6 */
/* 734 */	NdrFcShort( 0x8 ),	/* 8 */
/* 736 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 738 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 740 */	NdrFcShort( 0x0 ),	/* 0 */
/* 742 */	NdrFcShort( 0x0 ),	/* 0 */
/* 744 */	NdrFcShort( 0x0 ),	/* 0 */
/* 746 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter enabled */


	/* Parameter allow */

/* 748 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 750 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 752 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 754 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 756 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 758 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_BuiltIn */


	/* Procedure get_AllowInboundTimestampRequest */

/* 760 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 762 */	NdrFcLong( 0x0 ),	/* 0 */
/* 766 */	NdrFcShort( 0x15 ),	/* 21 */
/* 768 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 770 */	NdrFcShort( 0x0 ),	/* 0 */
/* 772 */	NdrFcShort( 0x22 ),	/* 34 */
/* 774 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 776 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 778 */	NdrFcShort( 0x0 ),	/* 0 */
/* 780 */	NdrFcShort( 0x0 ),	/* 0 */
/* 782 */	NdrFcShort( 0x0 ),	/* 0 */
/* 784 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter builtIn */


	/* Parameter allow */

/* 786 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 788 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 790 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 792 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 794 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 796 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_AllowInboundTimestampRequest */

/* 798 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 800 */	NdrFcLong( 0x0 ),	/* 0 */
/* 804 */	NdrFcShort( 0x16 ),	/* 22 */
/* 806 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 808 */	NdrFcShort( 0x6 ),	/* 6 */
/* 810 */	NdrFcShort( 0x8 ),	/* 8 */
/* 812 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 814 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 816 */	NdrFcShort( 0x0 ),	/* 0 */
/* 818 */	NdrFcShort( 0x0 ),	/* 0 */
/* 820 */	NdrFcShort( 0x0 ),	/* 0 */
/* 822 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter allow */

/* 824 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 826 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 828 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 830 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 832 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 834 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_AllowInboundMaskRequest */

/* 836 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 838 */	NdrFcLong( 0x0 ),	/* 0 */
/* 842 */	NdrFcShort( 0x17 ),	/* 23 */
/* 844 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 846 */	NdrFcShort( 0x0 ),	/* 0 */
/* 848 */	NdrFcShort( 0x22 ),	/* 34 */
/* 850 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 852 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 854 */	NdrFcShort( 0x0 ),	/* 0 */
/* 856 */	NdrFcShort( 0x0 ),	/* 0 */
/* 858 */	NdrFcShort( 0x0 ),	/* 0 */
/* 860 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter allow */

/* 862 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 864 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 866 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 868 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 870 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 872 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_AllowInboundMaskRequest */

/* 874 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 876 */	NdrFcLong( 0x0 ),	/* 0 */
/* 880 */	NdrFcShort( 0x18 ),	/* 24 */
/* 882 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 884 */	NdrFcShort( 0x6 ),	/* 6 */
/* 886 */	NdrFcShort( 0x8 ),	/* 8 */
/* 888 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 890 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 892 */	NdrFcShort( 0x0 ),	/* 0 */
/* 894 */	NdrFcShort( 0x0 ),	/* 0 */
/* 896 */	NdrFcShort( 0x0 ),	/* 0 */
/* 898 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter allow */

/* 900 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 902 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 904 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 906 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 908 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 910 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_AllowOutboundPacketTooBig */

/* 912 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 914 */	NdrFcLong( 0x0 ),	/* 0 */
/* 918 */	NdrFcShort( 0x19 ),	/* 25 */
/* 920 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 922 */	NdrFcShort( 0x0 ),	/* 0 */
/* 924 */	NdrFcShort( 0x22 ),	/* 34 */
/* 926 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 928 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 930 */	NdrFcShort( 0x0 ),	/* 0 */
/* 932 */	NdrFcShort( 0x0 ),	/* 0 */
/* 934 */	NdrFcShort( 0x0 ),	/* 0 */
/* 936 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter allow */

/* 938 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 940 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 942 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 944 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 946 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 948 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_AllowOutboundPacketTooBig */

/* 950 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 952 */	NdrFcLong( 0x0 ),	/* 0 */
/* 956 */	NdrFcShort( 0x1a ),	/* 26 */
/* 958 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 960 */	NdrFcShort( 0x6 ),	/* 6 */
/* 962 */	NdrFcShort( 0x8 ),	/* 8 */
/* 964 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 966 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 968 */	NdrFcShort( 0x0 ),	/* 0 */
/* 970 */	NdrFcShort( 0x0 ),	/* 0 */
/* 972 */	NdrFcShort( 0x0 ),	/* 0 */
/* 974 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter allow */

/* 976 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 978 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 980 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 982 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 984 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 986 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Name */


	/* Procedure get_Name */


	/* Procedure get_Name */

/* 988 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 990 */	NdrFcLong( 0x0 ),	/* 0 */
/* 994 */	NdrFcShort( 0x7 ),	/* 7 */
/* 996 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 998 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1000 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1002 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1004 */	0xa,		/* 10 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 1006 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1008 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1010 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1012 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter name */


	/* Parameter name */


	/* Parameter name */

/* 1014 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 1016 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1018 */	NdrFcShort( 0x24 ),	/* Type Offset=36 */

	/* Return value */


	/* Return value */


	/* Return value */

/* 1020 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1022 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1024 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Name */


	/* Procedure put_Name */

/* 1026 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1028 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1032 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1034 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1036 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1038 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1040 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1042 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1044 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1046 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1048 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1050 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter name */


	/* Parameter name */

/* 1052 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1054 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1056 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Return value */


	/* Return value */

/* 1058 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1060 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1062 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_IpVersion */


	/* Procedure get_Protocol */

/* 1064 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1066 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1070 */	NdrFcShort( 0xb ),	/* 11 */
/* 1072 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1074 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1076 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1078 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1080 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1082 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1084 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1086 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1088 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ipVersion */


	/* Parameter ipProtocol */

/* 1090 */	NdrFcShort( 0x2010 ),	/* Flags:  out, srv alloc size=8 */
/* 1092 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1094 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */


	/* Return value */

/* 1096 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1098 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1100 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_IpVersion */


	/* Procedure put_Protocol */

/* 1102 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1104 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1108 */	NdrFcShort( 0xc ),	/* 12 */
/* 1110 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1112 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1114 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1116 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1118 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1120 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1122 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1124 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1126 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ipVersion */


	/* Parameter ipProtocol */

/* 1128 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1130 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1132 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 1134 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1136 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1138 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Port */

/* 1140 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1142 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1146 */	NdrFcShort( 0xd ),	/* 13 */
/* 1148 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1150 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1152 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1154 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1156 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1158 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1160 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1162 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1164 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter portNumber */

/* 1166 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1168 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1170 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1172 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1174 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1176 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Port */

/* 1178 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1180 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1184 */	NdrFcShort( 0xe ),	/* 14 */
/* 1186 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1188 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1190 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1192 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1194 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1196 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1198 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1200 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1202 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter portNumber */

/* 1204 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1206 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1208 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1210 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1212 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1214 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Scope */

/* 1216 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1218 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1222 */	NdrFcShort( 0xf ),	/* 15 */
/* 1224 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1226 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1228 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1230 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1232 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1234 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1236 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1238 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1240 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter scope */

/* 1242 */	NdrFcShort( 0x2010 ),	/* Flags:  out, srv alloc size=8 */
/* 1244 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1246 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */

/* 1248 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1250 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1252 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Scope */

/* 1254 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1256 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1260 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1262 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1264 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1266 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1268 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1270 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1272 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1274 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1276 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1278 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter scope */

/* 1280 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1282 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1284 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Return value */

/* 1286 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1288 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1290 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_RemoteAddresses */

/* 1292 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1294 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1298 */	NdrFcShort( 0x11 ),	/* 17 */
/* 1300 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1302 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1304 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1306 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1308 */	0xa,		/* 10 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 1310 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1312 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1314 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1316 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter remoteAddrs */

/* 1318 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 1320 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1322 */	NdrFcShort( 0x24 ),	/* Type Offset=36 */

	/* Return value */

/* 1324 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1326 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1328 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_RemoteAddresses */

/* 1330 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1332 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1336 */	NdrFcShort( 0x12 ),	/* 18 */
/* 1338 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1340 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1342 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1344 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1346 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1348 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1350 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1352 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1354 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter remoteAddrs */

/* 1356 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1358 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1360 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Return value */

/* 1362 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1364 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1366 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Count */


	/* Procedure get_Count */


	/* Procedure get_Count */

/* 1368 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1370 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1374 */	NdrFcShort( 0x7 ),	/* 7 */
/* 1376 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1378 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1380 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1382 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1384 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1386 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1388 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1390 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1392 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter count */


	/* Parameter count */


	/* Parameter count */

/* 1394 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1396 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1398 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */


	/* Return value */

/* 1400 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1402 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1404 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Add */

/* 1406 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1408 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1412 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1414 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1416 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1418 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1420 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1422 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1424 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1426 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1428 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1430 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter port */

/* 1432 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1434 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1436 */	NdrFcShort( 0x44 ),	/* Type Offset=68 */

	/* Return value */

/* 1438 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1440 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1442 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Remove */

/* 1444 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1446 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1450 */	NdrFcShort( 0x9 ),	/* 9 */
/* 1452 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 1454 */	NdrFcShort( 0xe ),	/* 14 */
/* 1456 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1458 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 1460 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1462 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1464 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1466 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1468 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter portNumber */

/* 1470 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1472 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1474 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ipProtocol */

/* 1476 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1478 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1480 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Return value */

/* 1482 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1484 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1486 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Item */

/* 1488 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1490 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1494 */	NdrFcShort( 0xa ),	/* 10 */
/* 1496 */	NdrFcShort( 0x28 ),	/* ia64 Stack size/offset = 40 */
/* 1498 */	NdrFcShort( 0xe ),	/* 14 */
/* 1500 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1502 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x4,		/* 4 */
/* 1504 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1506 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1508 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1510 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1512 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter portNumber */

/* 1514 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1516 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1518 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ipProtocol */

/* 1520 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1522 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1524 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Parameter openPort */

/* 1526 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1528 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1530 */	NdrFcShort( 0x56 ),	/* Type Offset=86 */

	/* Return value */

/* 1532 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1534 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 1536 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get__NewEnum */

/* 1538 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1540 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1544 */	NdrFcShort( 0xb ),	/* 11 */
/* 1546 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1548 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1550 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1552 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1554 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1556 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1558 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1560 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1562 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newEnum */

/* 1564 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1566 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1568 */	NdrFcShort( 0x5a ),	/* Type Offset=90 */

	/* Return value */

/* 1570 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1572 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1574 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_CurrentProfileType */


	/* Procedure get_Type */

/* 1576 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1578 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1582 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1584 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1586 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1588 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1590 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1592 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1594 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1596 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1598 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1600 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter profileType */


	/* Parameter type */

/* 1602 */	NdrFcShort( 0x2010 ),	/* Flags:  out, srv alloc size=8 */
/* 1604 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1606 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */


	/* Return value */

/* 1608 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1610 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1612 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_IpVersion */

/* 1614 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1616 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1620 */	NdrFcShort( 0xa ),	/* 10 */
/* 1622 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1624 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1626 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1628 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1630 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1632 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1634 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1636 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1638 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ipVersion */

/* 1640 */	NdrFcShort( 0x2010 ),	/* Flags:  out, srv alloc size=8 */
/* 1642 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1644 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */

/* 1646 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1648 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1650 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_IpVersion */

/* 1652 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1654 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1658 */	NdrFcShort( 0xb ),	/* 11 */
/* 1660 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1662 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1664 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1666 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1668 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1670 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1672 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1674 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1676 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ipVersion */

/* 1678 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1680 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1682 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Return value */

/* 1684 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1686 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1688 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Scope */

/* 1690 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1692 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1696 */	NdrFcShort( 0xc ),	/* 12 */
/* 1698 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1700 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1702 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1704 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1706 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1708 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1710 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1712 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1714 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter scope */

/* 1716 */	NdrFcShort( 0x2010 ),	/* Flags:  out, srv alloc size=8 */
/* 1718 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1720 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */

/* 1722 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1724 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1726 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Scope */

/* 1728 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1730 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1734 */	NdrFcShort( 0xd ),	/* 13 */
/* 1736 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1738 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1740 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1742 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1744 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1746 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1748 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1750 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1752 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter scope */

/* 1754 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1756 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1758 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Return value */

/* 1760 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1762 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1764 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_RemoteAddresses */

/* 1766 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1768 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1772 */	NdrFcShort( 0xe ),	/* 14 */
/* 1774 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1776 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1778 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1780 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1782 */	0xa,		/* 10 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 1784 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1786 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1788 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1790 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter remoteAddrs */

/* 1792 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 1794 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1796 */	NdrFcShort( 0x24 ),	/* Type Offset=36 */

	/* Return value */

/* 1798 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1800 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1802 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_RemoteAddresses */

/* 1804 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1806 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1810 */	NdrFcShort( 0xf ),	/* 15 */
/* 1812 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1814 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1816 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1818 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1820 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1822 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1824 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1826 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1828 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter remoteAddrs */

/* 1830 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1832 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1834 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Return value */

/* 1836 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1838 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1840 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Enabled */

/* 1842 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1844 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1848 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1850 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1852 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1854 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1856 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1858 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1860 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1862 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1864 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1866 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter enabled */

/* 1868 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1870 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1872 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1874 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1876 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1878 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Enabled */

/* 1880 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1882 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1886 */	NdrFcShort( 0x11 ),	/* 17 */
/* 1888 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1890 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1892 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1894 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1896 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1898 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1900 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1902 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1904 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter enabled */

/* 1906 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1908 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1910 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1912 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1914 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1916 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_GloballyOpenPorts */

/* 1918 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1920 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1924 */	NdrFcShort( 0x12 ),	/* 18 */
/* 1926 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1928 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1930 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1932 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1934 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1936 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1938 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1940 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1942 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter openPorts */

/* 1944 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1946 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1948 */	NdrFcShort( 0x70 ),	/* Type Offset=112 */

	/* Return value */

/* 1950 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1952 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1954 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Item */

/* 1956 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1958 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1962 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1964 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 1966 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1968 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1970 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 1972 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1974 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1976 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1978 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1980 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter svcType */

/* 1982 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1984 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 1986 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Parameter service */

/* 1988 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1990 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 1992 */	NdrFcShort( 0x86 ),	/* Type Offset=134 */

	/* Return value */

/* 1994 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1996 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 1998 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get__NewEnum */

/* 2000 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2002 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2006 */	NdrFcShort( 0x9 ),	/* 9 */
/* 2008 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2010 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2012 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2014 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2016 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2018 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2020 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2022 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2024 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newEnum */

/* 2026 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2028 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2030 */	NdrFcShort( 0x9c ),	/* Type Offset=156 */

	/* Return value */

/* 2032 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2034 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2036 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_ProcessImageFileName */

/* 2038 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2040 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2044 */	NdrFcShort( 0x9 ),	/* 9 */
/* 2046 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2048 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2050 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2052 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2054 */	0xa,		/* 10 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 2056 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2058 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2060 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2062 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter imageFileName */

/* 2064 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 2066 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2068 */	NdrFcShort( 0x24 ),	/* Type Offset=36 */

	/* Return value */

/* 2070 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2072 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2074 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_ProcessImageFileName */

/* 2076 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2078 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2082 */	NdrFcShort( 0xa ),	/* 10 */
/* 2084 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2086 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2088 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2090 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 2092 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 2094 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2096 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2098 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2100 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter imageFileName */

/* 2102 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2104 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2106 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Return value */

/* 2108 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2110 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2112 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Scope */

/* 2114 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2116 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2120 */	NdrFcShort( 0xd ),	/* 13 */
/* 2122 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2124 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2126 */	NdrFcShort( 0x22 ),	/* 34 */
/* 2128 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2130 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2132 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2134 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2136 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2138 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter scope */

/* 2140 */	NdrFcShort( 0x2010 ),	/* Flags:  out, srv alloc size=8 */
/* 2142 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2144 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */

/* 2146 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2148 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2150 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Scope */

/* 2152 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2154 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2158 */	NdrFcShort( 0xe ),	/* 14 */
/* 2160 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2162 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2164 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2166 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2168 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2170 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2172 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2174 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2176 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter scope */

/* 2178 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2180 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2182 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Return value */

/* 2184 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2186 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2188 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_RemoteAddresses */

/* 2190 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2192 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2196 */	NdrFcShort( 0xf ),	/* 15 */
/* 2198 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2200 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2202 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2204 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2206 */	0xa,		/* 10 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 2208 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2210 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2212 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2214 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter remoteAddrs */

/* 2216 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 2218 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2220 */	NdrFcShort( 0x24 ),	/* Type Offset=36 */

	/* Return value */

/* 2222 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2224 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2226 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_RemoteAddresses */

/* 2228 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2230 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2234 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2236 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2238 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2240 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2242 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 2244 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 2246 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2248 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2250 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2252 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter remoteAddrs */

/* 2254 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2256 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2258 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Return value */

/* 2260 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2262 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2264 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Add */

/* 2266 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2268 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2272 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2274 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2276 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2278 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2280 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 2282 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2284 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2286 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2288 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2290 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter app */

/* 2292 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2294 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2296 */	NdrFcShort( 0xb2 ),	/* Type Offset=178 */

	/* Return value */

/* 2298 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2300 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2302 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Remove */

/* 2304 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2306 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2310 */	NdrFcShort( 0x9 ),	/* 9 */
/* 2312 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2314 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2316 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2318 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 2320 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 2322 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2324 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2326 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2328 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter imageFileName */

/* 2330 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2332 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2334 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Return value */

/* 2336 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2338 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2340 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Item */

/* 2342 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2344 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2348 */	NdrFcShort( 0xa ),	/* 10 */
/* 2350 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 2352 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2354 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2356 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 2358 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 2360 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2362 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2364 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2366 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter imageFileName */

/* 2368 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2370 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2372 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Parameter app */

/* 2374 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2376 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2378 */	NdrFcShort( 0xc4 ),	/* Type Offset=196 */

	/* Return value */

/* 2380 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2382 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2384 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get__NewEnum */

/* 2386 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2388 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2392 */	NdrFcShort( 0xb ),	/* 11 */
/* 2394 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2396 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2398 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2400 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2402 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2404 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2406 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2408 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2410 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newEnum */

/* 2412 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2414 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2416 */	NdrFcShort( 0x9c ),	/* Type Offset=156 */

	/* Return value */

/* 2418 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2420 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2422 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_FirewallEnabled */

/* 2424 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2426 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2430 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2432 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2434 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2436 */	NdrFcShort( 0x22 ),	/* 34 */
/* 2438 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2440 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2442 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2444 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2446 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2448 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter enabled */

/* 2450 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2452 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2454 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2456 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2458 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2460 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_FirewallEnabled */

/* 2462 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2464 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2468 */	NdrFcShort( 0x9 ),	/* 9 */
/* 2470 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2472 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2474 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2476 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2478 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2480 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2482 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2484 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2486 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter enabled */

/* 2488 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2490 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2492 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2494 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2496 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2498 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_ExceptionsNotAllowed */

/* 2500 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2502 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2506 */	NdrFcShort( 0xa ),	/* 10 */
/* 2508 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2510 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2512 */	NdrFcShort( 0x22 ),	/* 34 */
/* 2514 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2516 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2518 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2520 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2522 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2524 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter notAllowed */

/* 2526 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2528 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2530 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2532 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2534 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2536 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_ExceptionsNotAllowed */

/* 2538 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2540 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2544 */	NdrFcShort( 0xb ),	/* 11 */
/* 2546 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2548 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2550 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2552 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2554 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2556 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2558 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2560 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2562 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter notAllowed */

/* 2564 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2566 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2568 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2570 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2572 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2574 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_NotificationsDisabled */

/* 2576 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2578 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2582 */	NdrFcShort( 0xc ),	/* 12 */
/* 2584 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2586 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2588 */	NdrFcShort( 0x22 ),	/* 34 */
/* 2590 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2592 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2594 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2596 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2598 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2600 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter disabled */

/* 2602 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2604 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2606 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2608 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2610 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2612 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_NotificationsDisabled */

/* 2614 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2616 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2620 */	NdrFcShort( 0xd ),	/* 13 */
/* 2622 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2624 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2626 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2628 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2630 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2632 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2634 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2636 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2638 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter disabled */

/* 2640 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2642 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2644 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2646 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2648 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2650 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_UnicastResponsesToMulticastBroadcastDisabled */

/* 2652 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2654 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2658 */	NdrFcShort( 0xe ),	/* 14 */
/* 2660 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2662 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2664 */	NdrFcShort( 0x22 ),	/* 34 */
/* 2666 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2668 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2670 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2672 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2674 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2676 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter disabled */

/* 2678 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2680 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2682 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2684 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2686 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2688 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_UnicastResponsesToMulticastBroadcastDisabled */

/* 2690 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2692 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2696 */	NdrFcShort( 0xf ),	/* 15 */
/* 2698 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2700 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2702 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2704 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2706 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2708 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2710 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2712 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2714 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter disabled */

/* 2716 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2718 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2720 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2722 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2724 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2726 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_RemoteAdminSettings */

/* 2728 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2730 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2734 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2736 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2738 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2740 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2742 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2744 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2746 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2748 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2750 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2752 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter remoteAdminSettings */

/* 2754 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2756 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2758 */	NdrFcShort( 0xc8 ),	/* Type Offset=200 */

	/* Return value */

/* 2760 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2762 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2764 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_IcmpSettings */

/* 2766 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2768 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2772 */	NdrFcShort( 0x11 ),	/* 17 */
/* 2774 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2776 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2778 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2780 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2782 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2784 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2786 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2788 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2790 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter icmpSettings */

/* 2792 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2794 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2796 */	NdrFcShort( 0xde ),	/* Type Offset=222 */

	/* Return value */

/* 2798 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2800 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2802 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_GloballyOpenPorts */

/* 2804 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2806 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2810 */	NdrFcShort( 0x12 ),	/* 18 */
/* 2812 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2814 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2816 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2818 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2820 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2822 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2824 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2826 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2828 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter openPorts */

/* 2830 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2832 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2834 */	NdrFcShort( 0xf4 ),	/* Type Offset=244 */

	/* Return value */

/* 2836 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2838 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2840 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Services */

/* 2842 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2844 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2848 */	NdrFcShort( 0x13 ),	/* 19 */
/* 2850 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2852 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2854 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2856 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2858 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2860 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2862 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2864 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2866 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter services */

/* 2868 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2870 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2872 */	NdrFcShort( 0x10a ),	/* Type Offset=266 */

	/* Return value */

/* 2874 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2876 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2878 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_AuthorizedApplications */

/* 2880 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2882 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2886 */	NdrFcShort( 0x14 ),	/* 20 */
/* 2888 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2890 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2892 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2894 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2896 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2898 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2900 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2902 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2904 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter apps */

/* 2906 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2908 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2910 */	NdrFcShort( 0x120 ),	/* Type Offset=288 */

	/* Return value */

/* 2912 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2914 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2916 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_CurrentProfile */

/* 2918 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2920 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2924 */	NdrFcShort( 0x7 ),	/* 7 */
/* 2926 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2928 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2930 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2932 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2934 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2936 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2938 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2940 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2942 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter profile */

/* 2944 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2946 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2948 */	NdrFcShort( 0x136 ),	/* Type Offset=310 */

	/* Return value */

/* 2950 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2952 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2954 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetProfileByType */

/* 2956 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2958 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2962 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2964 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 2966 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2968 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2970 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 2972 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2974 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2976 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2978 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2980 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter profileType */

/* 2982 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2984 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 2986 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Parameter profile */

/* 2988 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2990 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 2992 */	NdrFcShort( 0x136 ),	/* Type Offset=310 */

	/* Return value */

/* 2994 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2996 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 2998 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_LocalPolicy */

/* 3000 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3002 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3006 */	NdrFcShort( 0x7 ),	/* 7 */
/* 3008 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 3010 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3012 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3014 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 3016 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3018 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3020 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3022 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3024 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter localPolicy */

/* 3026 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 3028 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 3030 */	NdrFcShort( 0x14c ),	/* Type Offset=332 */

	/* Return value */

/* 3032 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3034 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 3036 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RestoreDefaults */

/* 3038 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3040 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3044 */	NdrFcShort( 0x9 ),	/* 9 */
/* 3046 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 3048 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3050 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3052 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 3054 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3056 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3058 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3060 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3062 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 3064 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3066 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 3068 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure IsPortAllowed */

/* 3070 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3072 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3076 */	NdrFcShort( 0xa ),	/* 10 */
/* 3078 */	NdrFcShort( 0x48 ),	/* ia64 Stack size/offset = 72 */
/* 3080 */	NdrFcShort( 0x14 ),	/* 20 */
/* 3082 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3084 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x8,		/* 8 */
/* 3086 */	0xa,		/* 10 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 3088 */	NdrFcShort( 0x40 ),	/* 64 */
/* 3090 */	NdrFcShort( 0x2 ),	/* 2 */
/* 3092 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3094 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter imageFileName */

/* 3096 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 3098 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 3100 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Parameter ipVersion */

/* 3102 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3104 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 3106 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Parameter portNumber */

/* 3108 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3110 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 3112 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter localAddress */

/* 3114 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 3116 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 3118 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Parameter ipProtocol */

/* 3120 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3122 */	NdrFcShort( 0x28 ),	/* ia64 Stack size/offset = 40 */
/* 3124 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Parameter allowed */

/* 3126 */	NdrFcShort( 0x6113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=24 */
/* 3128 */	NdrFcShort( 0x30 ),	/* ia64 Stack size/offset = 48 */
/* 3130 */	NdrFcShort( 0x52a ),	/* Type Offset=1322 */

	/* Parameter restricted */

/* 3132 */	NdrFcShort( 0x6113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=24 */
/* 3134 */	NdrFcShort( 0x38 ),	/* ia64 Stack size/offset = 56 */
/* 3136 */	NdrFcShort( 0x52a ),	/* Type Offset=1322 */

	/* Return value */

/* 3138 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3140 */	NdrFcShort( 0x40 ),	/* ia64 Stack size/offset = 64 */
/* 3142 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure IsIcmpTypeAllowed */

/* 3144 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3146 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3150 */	NdrFcShort( 0xb ),	/* 11 */
/* 3152 */	NdrFcShort( 0x38 ),	/* ia64 Stack size/offset = 56 */
/* 3154 */	NdrFcShort( 0xb ),	/* 11 */
/* 3156 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3158 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 3160 */	0xa,		/* 10 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 3162 */	NdrFcShort( 0x40 ),	/* 64 */
/* 3164 */	NdrFcShort( 0x1 ),	/* 1 */
/* 3166 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3168 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ipVersion */

/* 3170 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3172 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 3174 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Parameter localAddress */

/* 3176 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 3178 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 3180 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Parameter type */

/* 3182 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3184 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 3186 */	0x1,		/* FC_BYTE */
			0x0,		/* 0 */

	/* Parameter allowed */

/* 3188 */	NdrFcShort( 0x6113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=24 */
/* 3190 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 3192 */	NdrFcShort( 0x52a ),	/* Type Offset=1322 */

	/* Parameter restricted */

/* 3194 */	NdrFcShort( 0x6113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=24 */
/* 3196 */	NdrFcShort( 0x28 ),	/* ia64 Stack size/offset = 40 */
/* 3198 */	NdrFcShort( 0x52a ),	/* Type Offset=1322 */

	/* Return value */

/* 3200 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3202 */	NdrFcShort( 0x30 ),	/* ia64 Stack size/offset = 48 */
/* 3204 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/*  4 */	0xd,		/* FC_ENUM16 */
			0x5c,		/* FC_PAD */
/*  6 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/*  8 */	NdrFcShort( 0x1c ),	/* Offset= 28 (36) */
/* 10 */	
			0x13, 0x0,	/* FC_OP */
/* 12 */	NdrFcShort( 0xe ),	/* Offset= 14 (26) */
/* 14 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 16 */	NdrFcShort( 0x2 ),	/* 2 */
/* 18 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 20 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 22 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 24 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 26 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 28 */	NdrFcShort( 0x8 ),	/* 8 */
/* 30 */	NdrFcShort( 0xfff0 ),	/* Offset= -16 (14) */
/* 32 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 34 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 36 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 38 */	NdrFcShort( 0x0 ),	/* 0 */
/* 40 */	NdrFcShort( 0x8 ),	/* 8 */
/* 42 */	NdrFcShort( 0x0 ),	/* 0 */
/* 44 */	NdrFcShort( 0xffde ),	/* Offset= -34 (10) */
/* 46 */	
			0x12, 0x0,	/* FC_UP */
/* 48 */	NdrFcShort( 0xffea ),	/* Offset= -22 (26) */
/* 50 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 52 */	NdrFcShort( 0x0 ),	/* 0 */
/* 54 */	NdrFcShort( 0x8 ),	/* 8 */
/* 56 */	NdrFcShort( 0x0 ),	/* 0 */
/* 58 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (46) */
/* 60 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 62 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 64 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 66 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 68 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 70 */	NdrFcLong( 0xe0483ba0 ),	/* -532137056 */
/* 74 */	NdrFcShort( 0x47ff ),	/* 18431 */
/* 76 */	NdrFcShort( 0x4d9c ),	/* 19868 */
/* 78 */	0xa6,		/* 166 */
			0xd6,		/* 214 */
/* 80 */	0x77,		/* 119 */
			0x41,		/* 65 */
/* 82 */	0xd0,		/* 208 */
			0xb1,		/* 177 */
/* 84 */	0x95,		/* 149 */
			0xf7,		/* 247 */
/* 86 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 88 */	NdrFcShort( 0xffec ),	/* Offset= -20 (68) */
/* 90 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 92 */	NdrFcShort( 0x2 ),	/* Offset= 2 (94) */
/* 94 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 96 */	NdrFcLong( 0x0 ),	/* 0 */
/* 100 */	NdrFcShort( 0x0 ),	/* 0 */
/* 102 */	NdrFcShort( 0x0 ),	/* 0 */
/* 104 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 106 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 108 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 110 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 112 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 114 */	NdrFcShort( 0x2 ),	/* Offset= 2 (116) */
/* 116 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 118 */	NdrFcLong( 0xc0e9d7fa ),	/* -1058416646 */
/* 122 */	NdrFcShort( 0xe07e ),	/* -8066 */
/* 124 */	NdrFcShort( 0x430a ),	/* 17162 */
/* 126 */	0xb1,		/* 177 */
			0x9a,		/* 154 */
/* 128 */	0x9,		/* 9 */
			0xc,		/* 12 */
/* 130 */	0xe8,		/* 232 */
			0x2d,		/* 45 */
/* 132 */	0x92,		/* 146 */
			0xe2,		/* 226 */
/* 134 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 136 */	NdrFcShort( 0x2 ),	/* Offset= 2 (138) */
/* 138 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 140 */	NdrFcLong( 0x79fd57c8 ),	/* 2046646216 */
/* 144 */	NdrFcShort( 0x908e ),	/* -28530 */
/* 146 */	NdrFcShort( 0x4a36 ),	/* 18998 */
/* 148 */	0x98,		/* 152 */
			0x88,		/* 136 */
/* 150 */	0xd5,		/* 213 */
			0xb3,		/* 179 */
/* 152 */	0xf0,		/* 240 */
			0xa4,		/* 164 */
/* 154 */	0x44,		/* 68 */
			0xcf,		/* 207 */
/* 156 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 158 */	NdrFcShort( 0x2 ),	/* Offset= 2 (160) */
/* 160 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 162 */	NdrFcLong( 0x0 ),	/* 0 */
/* 166 */	NdrFcShort( 0x0 ),	/* 0 */
/* 168 */	NdrFcShort( 0x0 ),	/* 0 */
/* 170 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 172 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 174 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 176 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 178 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 180 */	NdrFcLong( 0xb5e64ffa ),	/* -1243197446 */
/* 184 */	NdrFcShort( 0xc2c5 ),	/* -15675 */
/* 186 */	NdrFcShort( 0x444e ),	/* 17486 */
/* 188 */	0xa3,		/* 163 */
			0x1,		/* 1 */
/* 190 */	0xfb,		/* 251 */
			0x5e,		/* 94 */
/* 192 */	0x0,		/* 0 */
			0x1,		/* 1 */
/* 194 */	0x80,		/* 128 */
			0x50,		/* 80 */
/* 196 */	0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 198 */	NdrFcShort( 0xffec ),	/* Offset= -20 (178) */
/* 200 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 202 */	NdrFcShort( 0x2 ),	/* Offset= 2 (204) */
/* 204 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 206 */	NdrFcLong( 0xd4becddf ),	/* -725692961 */
/* 210 */	NdrFcShort( 0x6f73 ),	/* 28531 */
/* 212 */	NdrFcShort( 0x4a83 ),	/* 19075 */
/* 214 */	0xb8,		/* 184 */
			0x32,		/* 50 */
/* 216 */	0x9c,		/* 156 */
			0x66,		/* 102 */
/* 218 */	0x87,		/* 135 */
			0x4c,		/* 76 */
/* 220 */	0xd2,		/* 210 */
			0xe,		/* 14 */
/* 222 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 224 */	NdrFcShort( 0x2 ),	/* Offset= 2 (226) */
/* 226 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 228 */	NdrFcLong( 0xa6207b2e ),	/* -1507820754 */
/* 232 */	NdrFcShort( 0x7cdd ),	/* 31965 */
/* 234 */	NdrFcShort( 0x426a ),	/* 17002 */
/* 236 */	0x95,		/* 149 */
			0x1e,		/* 30 */
/* 238 */	0x5e,		/* 94 */
			0x1c,		/* 28 */
/* 240 */	0xbc,		/* 188 */
			0x5a,		/* 90 */
/* 242 */	0xfe,		/* 254 */
			0xad,		/* 173 */
/* 244 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 246 */	NdrFcShort( 0x2 ),	/* Offset= 2 (248) */
/* 248 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 250 */	NdrFcLong( 0xc0e9d7fa ),	/* -1058416646 */
/* 254 */	NdrFcShort( 0xe07e ),	/* -8066 */
/* 256 */	NdrFcShort( 0x430a ),	/* 17162 */
/* 258 */	0xb1,		/* 177 */
			0x9a,		/* 154 */
/* 260 */	0x9,		/* 9 */
			0xc,		/* 12 */
/* 262 */	0xe8,		/* 232 */
			0x2d,		/* 45 */
/* 264 */	0x92,		/* 146 */
			0xe2,		/* 226 */
/* 266 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 268 */	NdrFcShort( 0x2 ),	/* Offset= 2 (270) */
/* 270 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 272 */	NdrFcLong( 0x79649bb4 ),	/* 2036636596 */
/* 276 */	NdrFcShort( 0x903e ),	/* -28610 */
/* 278 */	NdrFcShort( 0x421b ),	/* 16923 */
/* 280 */	0x94,		/* 148 */
			0xc9,		/* 201 */
/* 282 */	0x79,		/* 121 */
			0x84,		/* 132 */
/* 284 */	0x8e,		/* 142 */
			0x79,		/* 121 */
/* 286 */	0xf6,		/* 246 */
			0xee,		/* 238 */
/* 288 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 290 */	NdrFcShort( 0x2 ),	/* Offset= 2 (292) */
/* 292 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 294 */	NdrFcLong( 0x644efd52 ),	/* 1682898258 */
/* 298 */	NdrFcShort( 0xccf9 ),	/* -13063 */
/* 300 */	NdrFcShort( 0x486c ),	/* 18540 */
/* 302 */	0x97,		/* 151 */
			0xa2,		/* 162 */
/* 304 */	0x39,		/* 57 */
			0xf3,		/* 243 */
/* 306 */	0x52,		/* 82 */
			0x57,		/* 87 */
/* 308 */	0xb,		/* 11 */
			0x30,		/* 48 */
/* 310 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 312 */	NdrFcShort( 0x2 ),	/* Offset= 2 (314) */
/* 314 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 316 */	NdrFcLong( 0x174a0dda ),	/* 390729178 */
/* 320 */	NdrFcShort( 0xe9f9 ),	/* -5639 */
/* 322 */	NdrFcShort( 0x449d ),	/* 17565 */
/* 324 */	0x99,		/* 153 */
			0x3b,		/* 59 */
/* 326 */	0x21,		/* 33 */
			0xab,		/* 171 */
/* 328 */	0x66,		/* 102 */
			0x7c,		/* 124 */
/* 330 */	0xa4,		/* 164 */
			0x56,		/* 86 */
/* 332 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 334 */	NdrFcShort( 0x2 ),	/* Offset= 2 (336) */
/* 336 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 338 */	NdrFcLong( 0xd46d2478 ),	/* -731044744 */
/* 342 */	NdrFcShort( 0x9ac9 ),	/* -25911 */
/* 344 */	NdrFcShort( 0x4008 ),	/* 16392 */
/* 346 */	0x9d,		/* 157 */
			0xc7,		/* 199 */
/* 348 */	0x55,		/* 85 */
			0x63,		/* 99 */
/* 350 */	0xce,		/* 206 */
			0x55,		/* 85 */
/* 352 */	0x36,		/* 54 */
			0xcc,		/* 204 */
/* 354 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 356 */	NdrFcShort( 0x3c6 ),	/* Offset= 966 (1322) */
/* 358 */	
			0x13, 0x0,	/* FC_OP */
/* 360 */	NdrFcShort( 0x3ae ),	/* Offset= 942 (1302) */
/* 362 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 364 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 366 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 368 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 370 */	NdrFcShort( 0x2 ),	/* Offset= 2 (372) */
/* 372 */	NdrFcShort( 0x10 ),	/* 16 */
/* 374 */	NdrFcShort( 0x2f ),	/* 47 */
/* 376 */	NdrFcLong( 0x14 ),	/* 20 */
/* 380 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 382 */	NdrFcLong( 0x3 ),	/* 3 */
/* 386 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 388 */	NdrFcLong( 0x11 ),	/* 17 */
/* 392 */	NdrFcShort( 0x8001 ),	/* Simple arm type: FC_BYTE */
/* 394 */	NdrFcLong( 0x2 ),	/* 2 */
/* 398 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 400 */	NdrFcLong( 0x4 ),	/* 4 */
/* 404 */	NdrFcShort( 0x800a ),	/* Simple arm type: FC_FLOAT */
/* 406 */	NdrFcLong( 0x5 ),	/* 5 */
/* 410 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 412 */	NdrFcLong( 0xb ),	/* 11 */
/* 416 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 418 */	NdrFcLong( 0xa ),	/* 10 */
/* 422 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 424 */	NdrFcLong( 0x6 ),	/* 6 */
/* 428 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (660) */
/* 430 */	NdrFcLong( 0x7 ),	/* 7 */
/* 434 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 436 */	NdrFcLong( 0x8 ),	/* 8 */
/* 440 */	NdrFcShort( 0xfe52 ),	/* Offset= -430 (10) */
/* 442 */	NdrFcLong( 0xd ),	/* 13 */
/* 446 */	NdrFcShort( 0xdc ),	/* Offset= 220 (666) */
/* 448 */	NdrFcLong( 0x9 ),	/* 9 */
/* 452 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (684) */
/* 454 */	NdrFcLong( 0x2000 ),	/* 8192 */
/* 458 */	NdrFcShort( 0xf4 ),	/* Offset= 244 (702) */
/* 460 */	NdrFcLong( 0x24 ),	/* 36 */
/* 464 */	NdrFcShort( 0x2fc ),	/* Offset= 764 (1228) */
/* 466 */	NdrFcLong( 0x4024 ),	/* 16420 */
/* 470 */	NdrFcShort( 0x2f6 ),	/* Offset= 758 (1228) */
/* 472 */	NdrFcLong( 0x4011 ),	/* 16401 */
/* 476 */	NdrFcShort( 0x2f4 ),	/* Offset= 756 (1232) */
/* 478 */	NdrFcLong( 0x4002 ),	/* 16386 */
/* 482 */	NdrFcShort( 0x2f2 ),	/* Offset= 754 (1236) */
/* 484 */	NdrFcLong( 0x4003 ),	/* 16387 */
/* 488 */	NdrFcShort( 0x2f0 ),	/* Offset= 752 (1240) */
/* 490 */	NdrFcLong( 0x4014 ),	/* 16404 */
/* 494 */	NdrFcShort( 0x2ee ),	/* Offset= 750 (1244) */
/* 496 */	NdrFcLong( 0x4004 ),	/* 16388 */
/* 500 */	NdrFcShort( 0x2ec ),	/* Offset= 748 (1248) */
/* 502 */	NdrFcLong( 0x4005 ),	/* 16389 */
/* 506 */	NdrFcShort( 0x2ea ),	/* Offset= 746 (1252) */
/* 508 */	NdrFcLong( 0x400b ),	/* 16395 */
/* 512 */	NdrFcShort( 0x2d4 ),	/* Offset= 724 (1236) */
/* 514 */	NdrFcLong( 0x400a ),	/* 16394 */
/* 518 */	NdrFcShort( 0x2d2 ),	/* Offset= 722 (1240) */
/* 520 */	NdrFcLong( 0x4006 ),	/* 16390 */
/* 524 */	NdrFcShort( 0x2dc ),	/* Offset= 732 (1256) */
/* 526 */	NdrFcLong( 0x4007 ),	/* 16391 */
/* 530 */	NdrFcShort( 0x2d2 ),	/* Offset= 722 (1252) */
/* 532 */	NdrFcLong( 0x4008 ),	/* 16392 */
/* 536 */	NdrFcShort( 0x2d4 ),	/* Offset= 724 (1260) */
/* 538 */	NdrFcLong( 0x400d ),	/* 16397 */
/* 542 */	NdrFcShort( 0x2d2 ),	/* Offset= 722 (1264) */
/* 544 */	NdrFcLong( 0x4009 ),	/* 16393 */
/* 548 */	NdrFcShort( 0x2d0 ),	/* Offset= 720 (1268) */
/* 550 */	NdrFcLong( 0x6000 ),	/* 24576 */
/* 554 */	NdrFcShort( 0x2ce ),	/* Offset= 718 (1272) */
/* 556 */	NdrFcLong( 0x400c ),	/* 16396 */
/* 560 */	NdrFcShort( 0x2cc ),	/* Offset= 716 (1276) */
/* 562 */	NdrFcLong( 0x10 ),	/* 16 */
/* 566 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 568 */	NdrFcLong( 0x12 ),	/* 18 */
/* 572 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 574 */	NdrFcLong( 0x13 ),	/* 19 */
/* 578 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 580 */	NdrFcLong( 0x15 ),	/* 21 */
/* 584 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 586 */	NdrFcLong( 0x16 ),	/* 22 */
/* 590 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 592 */	NdrFcLong( 0x17 ),	/* 23 */
/* 596 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 598 */	NdrFcLong( 0xe ),	/* 14 */
/* 602 */	NdrFcShort( 0x2aa ),	/* Offset= 682 (1284) */
/* 604 */	NdrFcLong( 0x400e ),	/* 16398 */
/* 608 */	NdrFcShort( 0x2ae ),	/* Offset= 686 (1294) */
/* 610 */	NdrFcLong( 0x4010 ),	/* 16400 */
/* 614 */	NdrFcShort( 0x2ac ),	/* Offset= 684 (1298) */
/* 616 */	NdrFcLong( 0x4012 ),	/* 16402 */
/* 620 */	NdrFcShort( 0x268 ),	/* Offset= 616 (1236) */
/* 622 */	NdrFcLong( 0x4013 ),	/* 16403 */
/* 626 */	NdrFcShort( 0x266 ),	/* Offset= 614 (1240) */
/* 628 */	NdrFcLong( 0x4015 ),	/* 16405 */
/* 632 */	NdrFcShort( 0x264 ),	/* Offset= 612 (1244) */
/* 634 */	NdrFcLong( 0x4016 ),	/* 16406 */
/* 638 */	NdrFcShort( 0x25a ),	/* Offset= 602 (1240) */
/* 640 */	NdrFcLong( 0x4017 ),	/* 16407 */
/* 644 */	NdrFcShort( 0x254 ),	/* Offset= 596 (1240) */
/* 646 */	NdrFcLong( 0x0 ),	/* 0 */
/* 650 */	NdrFcShort( 0x0 ),	/* Offset= 0 (650) */
/* 652 */	NdrFcLong( 0x1 ),	/* 1 */
/* 656 */	NdrFcShort( 0x0 ),	/* Offset= 0 (656) */
/* 658 */	NdrFcShort( 0xffff ),	/* Offset= -1 (657) */
/* 660 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 662 */	NdrFcShort( 0x8 ),	/* 8 */
/* 664 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 666 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 668 */	NdrFcLong( 0x0 ),	/* 0 */
/* 672 */	NdrFcShort( 0x0 ),	/* 0 */
/* 674 */	NdrFcShort( 0x0 ),	/* 0 */
/* 676 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 678 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 680 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 682 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 684 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 686 */	NdrFcLong( 0x20400 ),	/* 132096 */
/* 690 */	NdrFcShort( 0x0 ),	/* 0 */
/* 692 */	NdrFcShort( 0x0 ),	/* 0 */
/* 694 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 696 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 698 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 700 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 702 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 704 */	NdrFcShort( 0x2 ),	/* Offset= 2 (706) */
/* 706 */	
			0x13, 0x0,	/* FC_OP */
/* 708 */	NdrFcShort( 0x1f6 ),	/* Offset= 502 (1210) */
/* 710 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x89,		/* 137 */
/* 712 */	NdrFcShort( 0x20 ),	/* 32 */
/* 714 */	NdrFcShort( 0xa ),	/* 10 */
/* 716 */	NdrFcLong( 0x8 ),	/* 8 */
/* 720 */	NdrFcShort( 0x50 ),	/* Offset= 80 (800) */
/* 722 */	NdrFcLong( 0xd ),	/* 13 */
/* 726 */	NdrFcShort( 0x70 ),	/* Offset= 112 (838) */
/* 728 */	NdrFcLong( 0x9 ),	/* 9 */
/* 732 */	NdrFcShort( 0xa2 ),	/* Offset= 162 (894) */
/* 734 */	NdrFcLong( 0xc ),	/* 12 */
/* 738 */	NdrFcShort( 0xc2 ),	/* Offset= 194 (932) */
/* 740 */	NdrFcLong( 0x24 ),	/* 36 */
/* 744 */	NdrFcShort( 0x114 ),	/* Offset= 276 (1020) */
/* 746 */	NdrFcLong( 0x800d ),	/* 32781 */
/* 750 */	NdrFcShort( 0x130 ),	/* Offset= 304 (1054) */
/* 752 */	NdrFcLong( 0x10 ),	/* 16 */
/* 756 */	NdrFcShort( 0x14a ),	/* Offset= 330 (1086) */
/* 758 */	NdrFcLong( 0x2 ),	/* 2 */
/* 762 */	NdrFcShort( 0x160 ),	/* Offset= 352 (1114) */
/* 764 */	NdrFcLong( 0x3 ),	/* 3 */
/* 768 */	NdrFcShort( 0x176 ),	/* Offset= 374 (1142) */
/* 770 */	NdrFcLong( 0x14 ),	/* 20 */
/* 774 */	NdrFcShort( 0x18c ),	/* Offset= 396 (1170) */
/* 776 */	NdrFcShort( 0xffff ),	/* Offset= -1 (775) */
/* 778 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 780 */	NdrFcShort( 0x0 ),	/* 0 */
/* 782 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 784 */	NdrFcShort( 0x0 ),	/* 0 */
/* 786 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 788 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 792 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 794 */	
			0x13, 0x0,	/* FC_OP */
/* 796 */	NdrFcShort( 0xfcfe ),	/* Offset= -770 (26) */
/* 798 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 800 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 802 */	NdrFcShort( 0x10 ),	/* 16 */
/* 804 */	NdrFcShort( 0x0 ),	/* 0 */
/* 806 */	NdrFcShort( 0x6 ),	/* Offset= 6 (812) */
/* 808 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 810 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 812 */	
			0x11, 0x0,	/* FC_RP */
/* 814 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (778) */
/* 816 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 818 */	NdrFcShort( 0x0 ),	/* 0 */
/* 820 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 822 */	NdrFcShort( 0x0 ),	/* 0 */
/* 824 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 826 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 830 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 832 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 834 */	NdrFcShort( 0xfd1c ),	/* Offset= -740 (94) */
/* 836 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 838 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 840 */	NdrFcShort( 0x10 ),	/* 16 */
/* 842 */	NdrFcShort( 0x0 ),	/* 0 */
/* 844 */	NdrFcShort( 0x6 ),	/* Offset= 6 (850) */
/* 846 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 848 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 850 */	
			0x11, 0x0,	/* FC_RP */
/* 852 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (816) */
/* 854 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 856 */	NdrFcLong( 0x20400 ),	/* 132096 */
/* 860 */	NdrFcShort( 0x0 ),	/* 0 */
/* 862 */	NdrFcShort( 0x0 ),	/* 0 */
/* 864 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 866 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 868 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 870 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 872 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 874 */	NdrFcShort( 0x0 ),	/* 0 */
/* 876 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 878 */	NdrFcShort( 0x0 ),	/* 0 */
/* 880 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 882 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 886 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 888 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 890 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (854) */
/* 892 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 894 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 896 */	NdrFcShort( 0x10 ),	/* 16 */
/* 898 */	NdrFcShort( 0x0 ),	/* 0 */
/* 900 */	NdrFcShort( 0x6 ),	/* Offset= 6 (906) */
/* 902 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 904 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 906 */	
			0x11, 0x0,	/* FC_RP */
/* 908 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (872) */
/* 910 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 912 */	NdrFcShort( 0x0 ),	/* 0 */
/* 914 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 916 */	NdrFcShort( 0x0 ),	/* 0 */
/* 918 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 920 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 924 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 926 */	
			0x13, 0x0,	/* FC_OP */
/* 928 */	NdrFcShort( 0x176 ),	/* Offset= 374 (1302) */
/* 930 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 932 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 934 */	NdrFcShort( 0x10 ),	/* 16 */
/* 936 */	NdrFcShort( 0x0 ),	/* 0 */
/* 938 */	NdrFcShort( 0x6 ),	/* Offset= 6 (944) */
/* 940 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 942 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 944 */	
			0x11, 0x0,	/* FC_RP */
/* 946 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (910) */
/* 948 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 950 */	NdrFcLong( 0x2f ),	/* 47 */
/* 954 */	NdrFcShort( 0x0 ),	/* 0 */
/* 956 */	NdrFcShort( 0x0 ),	/* 0 */
/* 958 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 960 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 962 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 964 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 966 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 968 */	NdrFcShort( 0x1 ),	/* 1 */
/* 970 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 972 */	NdrFcShort( 0x4 ),	/* 4 */
/* 974 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 976 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 978 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 980 */	NdrFcShort( 0x18 ),	/* 24 */
/* 982 */	NdrFcShort( 0x0 ),	/* 0 */
/* 984 */	NdrFcShort( 0xa ),	/* Offset= 10 (994) */
/* 986 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 988 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 990 */	NdrFcShort( 0xffd6 ),	/* Offset= -42 (948) */
/* 992 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 994 */	
			0x13, 0x0,	/* FC_OP */
/* 996 */	NdrFcShort( 0xffe2 ),	/* Offset= -30 (966) */
/* 998 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 1000 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1002 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1004 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1006 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1008 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 1012 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 1014 */	
			0x13, 0x0,	/* FC_OP */
/* 1016 */	NdrFcShort( 0xffda ),	/* Offset= -38 (978) */
/* 1018 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1020 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1022 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1024 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1026 */	NdrFcShort( 0x6 ),	/* Offset= 6 (1032) */
/* 1028 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 1030 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 1032 */	
			0x11, 0x0,	/* FC_RP */
/* 1034 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (998) */
/* 1036 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 1038 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1040 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 1042 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 1044 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1046 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 1048 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 1050 */	0x0,		/* 0 */
			NdrFcShort( 0xfff1 ),	/* Offset= -15 (1036) */
			0x5b,		/* FC_END */
/* 1054 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1056 */	NdrFcShort( 0x20 ),	/* 32 */
/* 1058 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1060 */	NdrFcShort( 0xa ),	/* Offset= 10 (1070) */
/* 1062 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 1064 */	0x36,		/* FC_POINTER */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 1066 */	0x0,		/* 0 */
			NdrFcShort( 0xffe7 ),	/* Offset= -25 (1042) */
			0x5b,		/* FC_END */
/* 1070 */	
			0x11, 0x0,	/* FC_RP */
/* 1072 */	NdrFcShort( 0xff00 ),	/* Offset= -256 (816) */
/* 1074 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 1076 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1078 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1080 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1082 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1084 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 1086 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1088 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1090 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1092 */	NdrFcShort( 0x6 ),	/* Offset= 6 (1098) */
/* 1094 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 1096 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 1098 */	
			0x13, 0x0,	/* FC_OP */
/* 1100 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (1074) */
/* 1102 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 1104 */	NdrFcShort( 0x2 ),	/* 2 */
/* 1106 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1108 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1110 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1112 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 1114 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1116 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1118 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1120 */	NdrFcShort( 0x6 ),	/* Offset= 6 (1126) */
/* 1122 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 1124 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 1126 */	
			0x13, 0x0,	/* FC_OP */
/* 1128 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (1102) */
/* 1130 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 1132 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1134 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1136 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1138 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1140 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1142 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1144 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1146 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1148 */	NdrFcShort( 0x6 ),	/* Offset= 6 (1154) */
/* 1150 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 1152 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 1154 */	
			0x13, 0x0,	/* FC_OP */
/* 1156 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (1130) */
/* 1158 */	
			0x1b,		/* FC_CARRAY */
			0x7,		/* 7 */
/* 1160 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1162 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1164 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1166 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1168 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 1170 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1172 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1174 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1176 */	NdrFcShort( 0x6 ),	/* Offset= 6 (1182) */
/* 1178 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 1180 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 1182 */	
			0x13, 0x0,	/* FC_OP */
/* 1184 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (1158) */
/* 1186 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 1188 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1190 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1192 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1194 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 1196 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1198 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 1200 */	NdrFcShort( 0xffc8 ),	/* -56 */
/* 1202 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1204 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1206 */	NdrFcShort( 0xffec ),	/* Offset= -20 (1186) */
/* 1208 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1210 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1212 */	NdrFcShort( 0x38 ),	/* 56 */
/* 1214 */	NdrFcShort( 0xffec ),	/* Offset= -20 (1194) */
/* 1216 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1216) */
/* 1218 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1220 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1222 */	0x40,		/* FC_STRUCTPAD4 */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 1224 */	0x0,		/* 0 */
			NdrFcShort( 0xfdfd ),	/* Offset= -515 (710) */
			0x5b,		/* FC_END */
/* 1228 */	
			0x13, 0x0,	/* FC_OP */
/* 1230 */	NdrFcShort( 0xff04 ),	/* Offset= -252 (978) */
/* 1232 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1234 */	0x1,		/* FC_BYTE */
			0x5c,		/* FC_PAD */
/* 1236 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1238 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 1240 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1242 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 1244 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1246 */	0xb,		/* FC_HYPER */
			0x5c,		/* FC_PAD */
/* 1248 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1250 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 1252 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1254 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 1256 */	
			0x13, 0x0,	/* FC_OP */
/* 1258 */	NdrFcShort( 0xfdaa ),	/* Offset= -598 (660) */
/* 1260 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1262 */	NdrFcShort( 0xfb1c ),	/* Offset= -1252 (10) */
/* 1264 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1266 */	NdrFcShort( 0xfb6c ),	/* Offset= -1172 (94) */
/* 1268 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1270 */	NdrFcShort( 0xfe60 ),	/* Offset= -416 (854) */
/* 1272 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1274 */	NdrFcShort( 0xfdc4 ),	/* Offset= -572 (702) */
/* 1276 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1278 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1280) */
/* 1280 */	
			0x13, 0x0,	/* FC_OP */
/* 1282 */	NdrFcShort( 0x14 ),	/* Offset= 20 (1302) */
/* 1284 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 1286 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1288 */	0x6,		/* FC_SHORT */
			0x1,		/* FC_BYTE */
/* 1290 */	0x1,		/* FC_BYTE */
			0x8,		/* FC_LONG */
/* 1292 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 1294 */	
			0x13, 0x0,	/* FC_OP */
/* 1296 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (1284) */
/* 1298 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1300 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 1302 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 1304 */	NdrFcShort( 0x20 ),	/* 32 */
/* 1306 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1308 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1308) */
/* 1310 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1312 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1314 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1316 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1318 */	NdrFcShort( 0xfc44 ),	/* Offset= -956 (362) */
/* 1320 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1322 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1324 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1326 */	NdrFcShort( 0x18 ),	/* 24 */
/* 1328 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1330 */	NdrFcShort( 0xfc34 ),	/* Offset= -972 (358) */

			0x0
        }
    };

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            },
            {
            VARIANT_UserSize
            ,VARIANT_UserMarshal
            ,VARIANT_UserUnmarshal
            ,VARIANT_UserFree
            }

        };



/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: INetFwRemoteAdminSettings, ver. 0.0,
   GUID={0xD4BECDDF,0x6F73,0x4A83,{0xB8,0x32,0x9C,0x66,0x87,0x4C,0xD2,0x0E}} */

#pragma code_seg(".orpc")
static const unsigned short INetFwRemoteAdminSettings_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    38,
    76,
    114,
    152,
    190,
    228,
    266
    };

static const MIDL_STUBLESS_PROXY_INFO INetFwRemoteAdminSettings_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &INetFwRemoteAdminSettings_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO INetFwRemoteAdminSettings_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &INetFwRemoteAdminSettings_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(15) _INetFwRemoteAdminSettingsProxyVtbl = 
{
    &INetFwRemoteAdminSettings_ProxyInfo,
    &IID_INetFwRemoteAdminSettings,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* INetFwRemoteAdminSettings::get_IpVersion */ ,
    (void *) (INT_PTR) -1 /* INetFwRemoteAdminSettings::put_IpVersion */ ,
    (void *) (INT_PTR) -1 /* INetFwRemoteAdminSettings::get_Scope */ ,
    (void *) (INT_PTR) -1 /* INetFwRemoteAdminSettings::put_Scope */ ,
    (void *) (INT_PTR) -1 /* INetFwRemoteAdminSettings::get_RemoteAddresses */ ,
    (void *) (INT_PTR) -1 /* INetFwRemoteAdminSettings::put_RemoteAddresses */ ,
    (void *) (INT_PTR) -1 /* INetFwRemoteAdminSettings::get_Enabled */ ,
    (void *) (INT_PTR) -1 /* INetFwRemoteAdminSettings::put_Enabled */
};


static const PRPC_STUB_FUNCTION INetFwRemoteAdminSettings_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _INetFwRemoteAdminSettingsStubVtbl =
{
    &IID_INetFwRemoteAdminSettings,
    &INetFwRemoteAdminSettings_ServerInfo,
    15,
    &INetFwRemoteAdminSettings_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: INetFwIcmpSettings, ver. 0.0,
   GUID={0xA6207B2E,0x7CDD,0x426A,{0x95,0x1E,0x5E,0x1C,0xBC,0x5A,0xFE,0xAD}} */

#pragma code_seg(".orpc")
static const unsigned short INetFwIcmpSettings_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    304,
    342,
    380,
    418,
    456,
    494,
    228,
    266,
    532,
    570,
    608,
    646,
    684,
    722,
    760,
    798,
    836,
    874,
    912,
    950
    };

static const MIDL_STUBLESS_PROXY_INFO INetFwIcmpSettings_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &INetFwIcmpSettings_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO INetFwIcmpSettings_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &INetFwIcmpSettings_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(27) _INetFwIcmpSettingsProxyVtbl = 
{
    &INetFwIcmpSettings_ProxyInfo,
    &IID_INetFwIcmpSettings,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::get_AllowOutboundDestinationUnreachable */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::put_AllowOutboundDestinationUnreachable */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::get_AllowRedirect */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::put_AllowRedirect */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::get_AllowInboundEchoRequest */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::put_AllowInboundEchoRequest */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::get_AllowOutboundTimeExceeded */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::put_AllowOutboundTimeExceeded */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::get_AllowOutboundParameterProblem */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::put_AllowOutboundParameterProblem */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::get_AllowOutboundSourceQuench */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::put_AllowOutboundSourceQuench */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::get_AllowInboundRouterRequest */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::put_AllowInboundRouterRequest */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::get_AllowInboundTimestampRequest */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::put_AllowInboundTimestampRequest */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::get_AllowInboundMaskRequest */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::put_AllowInboundMaskRequest */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::get_AllowOutboundPacketTooBig */ ,
    (void *) (INT_PTR) -1 /* INetFwIcmpSettings::put_AllowOutboundPacketTooBig */
};


static const PRPC_STUB_FUNCTION INetFwIcmpSettings_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _INetFwIcmpSettingsStubVtbl =
{
    &IID_INetFwIcmpSettings,
    &INetFwIcmpSettings_ServerInfo,
    27,
    &INetFwIcmpSettings_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: INetFwOpenPort, ver. 0.0,
   GUID={0xE0483BA0,0x47FF,0x4D9C,{0xA6,0xD6,0x77,0x41,0xD0,0xB1,0x95,0xF7}} */

#pragma code_seg(".orpc")
static const unsigned short INetFwOpenPort_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    988,
    1026,
    76,
    114,
    1064,
    1102,
    1140,
    1178,
    1216,
    1254,
    1292,
    1330,
    684,
    722,
    760
    };

static const MIDL_STUBLESS_PROXY_INFO INetFwOpenPort_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &INetFwOpenPort_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO INetFwOpenPort_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &INetFwOpenPort_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(22) _INetFwOpenPortProxyVtbl = 
{
    &INetFwOpenPort_ProxyInfo,
    &IID_INetFwOpenPort,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::get_Name */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::put_Name */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::get_IpVersion */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::put_IpVersion */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::get_Protocol */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::put_Protocol */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::get_Port */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::put_Port */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::get_Scope */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::put_Scope */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::get_RemoteAddresses */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::put_RemoteAddresses */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::get_Enabled */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::put_Enabled */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPort::get_BuiltIn */
};


static const PRPC_STUB_FUNCTION INetFwOpenPort_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _INetFwOpenPortStubVtbl =
{
    &IID_INetFwOpenPort,
    &INetFwOpenPort_ServerInfo,
    22,
    &INetFwOpenPort_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: INetFwOpenPorts, ver. 0.0,
   GUID={0xC0E9D7FA,0xE07E,0x430A,{0xB1,0x9A,0x09,0x0C,0xE8,0x2D,0x92,0xE2}} */

#pragma code_seg(".orpc")
static const unsigned short INetFwOpenPorts_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    1368,
    1406,
    1444,
    1488,
    1538
    };

static const MIDL_STUBLESS_PROXY_INFO INetFwOpenPorts_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &INetFwOpenPorts_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO INetFwOpenPorts_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &INetFwOpenPorts_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(12) _INetFwOpenPortsProxyVtbl = 
{
    &INetFwOpenPorts_ProxyInfo,
    &IID_INetFwOpenPorts,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPorts::get_Count */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPorts::Add */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPorts::Remove */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPorts::Item */ ,
    (void *) (INT_PTR) -1 /* INetFwOpenPorts::get__NewEnum */
};


static const PRPC_STUB_FUNCTION INetFwOpenPorts_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _INetFwOpenPortsStubVtbl =
{
    &IID_INetFwOpenPorts,
    &INetFwOpenPorts_ServerInfo,
    12,
    &INetFwOpenPorts_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: INetFwService, ver. 0.0,
   GUID={0x79FD57C8,0x908E,0x4A36,{0x98,0x88,0xD5,0xB3,0xF0,0xA4,0x44,0xCF}} */

#pragma code_seg(".orpc")
static const unsigned short INetFwService_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    988,
    1576,
    380,
    1614,
    1652,
    1690,
    1728,
    1766,
    1804,
    1842,
    1880,
    1918
    };

static const MIDL_STUBLESS_PROXY_INFO INetFwService_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &INetFwService_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO INetFwService_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &INetFwService_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(19) _INetFwServiceProxyVtbl = 
{
    &INetFwService_ProxyInfo,
    &IID_INetFwService,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* INetFwService::get_Name */ ,
    (void *) (INT_PTR) -1 /* INetFwService::get_Type */ ,
    (void *) (INT_PTR) -1 /* INetFwService::get_Customized */ ,
    (void *) (INT_PTR) -1 /* INetFwService::get_IpVersion */ ,
    (void *) (INT_PTR) -1 /* INetFwService::put_IpVersion */ ,
    (void *) (INT_PTR) -1 /* INetFwService::get_Scope */ ,
    (void *) (INT_PTR) -1 /* INetFwService::put_Scope */ ,
    (void *) (INT_PTR) -1 /* INetFwService::get_RemoteAddresses */ ,
    (void *) (INT_PTR) -1 /* INetFwService::put_RemoteAddresses */ ,
    (void *) (INT_PTR) -1 /* INetFwService::get_Enabled */ ,
    (void *) (INT_PTR) -1 /* INetFwService::put_Enabled */ ,
    (void *) (INT_PTR) -1 /* INetFwService::get_GloballyOpenPorts */
};


static const PRPC_STUB_FUNCTION INetFwService_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _INetFwServiceStubVtbl =
{
    &IID_INetFwService,
    &INetFwService_ServerInfo,
    19,
    &INetFwService_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: INetFwServices, ver. 0.0,
   GUID={0x79649BB4,0x903E,0x421B,{0x94,0xC9,0x79,0x84,0x8E,0x79,0xF6,0xEE}} */

#pragma code_seg(".orpc")
static const unsigned short INetFwServices_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    1368,
    1956,
    2000
    };

static const MIDL_STUBLESS_PROXY_INFO INetFwServices_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &INetFwServices_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO INetFwServices_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &INetFwServices_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(10) _INetFwServicesProxyVtbl = 
{
    &INetFwServices_ProxyInfo,
    &IID_INetFwServices,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* INetFwServices::get_Count */ ,
    (void *) (INT_PTR) -1 /* INetFwServices::Item */ ,
    (void *) (INT_PTR) -1 /* INetFwServices::get__NewEnum */
};


static const PRPC_STUB_FUNCTION INetFwServices_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _INetFwServicesStubVtbl =
{
    &IID_INetFwServices,
    &INetFwServices_ServerInfo,
    10,
    &INetFwServices_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: INetFwAuthorizedApplication, ver. 0.0,
   GUID={0xB5E64FFA,0xC2C5,0x444E,{0xA3,0x01,0xFB,0x5E,0x00,0x01,0x80,0x50}} */

#pragma code_seg(".orpc")
static const unsigned short INetFwAuthorizedApplication_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    988,
    1026,
    2038,
    2076,
    1064,
    1102,
    2114,
    2152,
    2190,
    2228,
    608,
    646
    };

static const MIDL_STUBLESS_PROXY_INFO INetFwAuthorizedApplication_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &INetFwAuthorizedApplication_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO INetFwAuthorizedApplication_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &INetFwAuthorizedApplication_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(19) _INetFwAuthorizedApplicationProxyVtbl = 
{
    &INetFwAuthorizedApplication_ProxyInfo,
    &IID_INetFwAuthorizedApplication,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplication::get_Name */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplication::put_Name */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplication::get_ProcessImageFileName */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplication::put_ProcessImageFileName */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplication::get_IpVersion */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplication::put_IpVersion */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplication::get_Scope */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplication::put_Scope */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplication::get_RemoteAddresses */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplication::put_RemoteAddresses */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplication::get_Enabled */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplication::put_Enabled */
};


static const PRPC_STUB_FUNCTION INetFwAuthorizedApplication_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _INetFwAuthorizedApplicationStubVtbl =
{
    &IID_INetFwAuthorizedApplication,
    &INetFwAuthorizedApplication_ServerInfo,
    19,
    &INetFwAuthorizedApplication_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: INetFwAuthorizedApplications, ver. 0.0,
   GUID={0x644EFD52,0xCCF9,0x486C,{0x97,0xA2,0x39,0xF3,0x52,0x57,0x0B,0x30}} */

#pragma code_seg(".orpc")
static const unsigned short INetFwAuthorizedApplications_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    1368,
    2266,
    2304,
    2342,
    2386
    };

static const MIDL_STUBLESS_PROXY_INFO INetFwAuthorizedApplications_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &INetFwAuthorizedApplications_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO INetFwAuthorizedApplications_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &INetFwAuthorizedApplications_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(12) _INetFwAuthorizedApplicationsProxyVtbl = 
{
    &INetFwAuthorizedApplications_ProxyInfo,
    &IID_INetFwAuthorizedApplications,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplications::get_Count */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplications::Add */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplications::Remove */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplications::Item */ ,
    (void *) (INT_PTR) -1 /* INetFwAuthorizedApplications::get__NewEnum */
};


static const PRPC_STUB_FUNCTION INetFwAuthorizedApplications_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _INetFwAuthorizedApplicationsStubVtbl =
{
    &IID_INetFwAuthorizedApplications,
    &INetFwAuthorizedApplications_ServerInfo,
    12,
    &INetFwAuthorizedApplications_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: INetFwProfile, ver. 0.0,
   GUID={0x174A0DDA,0xE9F9,0x449D,{0x99,0x3B,0x21,0xAB,0x66,0x7C,0xA4,0x56}} */

#pragma code_seg(".orpc")
static const unsigned short INetFwProfile_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    2424,
    2462,
    2500,
    2538,
    2576,
    2614,
    2652,
    2690,
    2728,
    2766,
    2804,
    2842,
    2880
    };

static const MIDL_STUBLESS_PROXY_INFO INetFwProfile_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &INetFwProfile_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO INetFwProfile_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &INetFwProfile_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(21) _INetFwProfileProxyVtbl = 
{
    &INetFwProfile_ProxyInfo,
    &IID_INetFwProfile,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* INetFwProfile::get_Type */ ,
    (void *) (INT_PTR) -1 /* INetFwProfile::get_FirewallEnabled */ ,
    (void *) (INT_PTR) -1 /* INetFwProfile::put_FirewallEnabled */ ,
    (void *) (INT_PTR) -1 /* INetFwProfile::get_ExceptionsNotAllowed */ ,
    (void *) (INT_PTR) -1 /* INetFwProfile::put_ExceptionsNotAllowed */ ,
    (void *) (INT_PTR) -1 /* INetFwProfile::get_NotificationsDisabled */ ,
    (void *) (INT_PTR) -1 /* INetFwProfile::put_NotificationsDisabled */ ,
    (void *) (INT_PTR) -1 /* INetFwProfile::get_UnicastResponsesToMulticastBroadcastDisabled */ ,
    (void *) (INT_PTR) -1 /* INetFwProfile::put_UnicastResponsesToMulticastBroadcastDisabled */ ,
    (void *) (INT_PTR) -1 /* INetFwProfile::get_RemoteAdminSettings */ ,
    (void *) (INT_PTR) -1 /* INetFwProfile::get_IcmpSettings */ ,
    (void *) (INT_PTR) -1 /* INetFwProfile::get_GloballyOpenPorts */ ,
    (void *) (INT_PTR) -1 /* INetFwProfile::get_Services */ ,
    (void *) (INT_PTR) -1 /* INetFwProfile::get_AuthorizedApplications */
};


static const PRPC_STUB_FUNCTION INetFwProfile_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _INetFwProfileStubVtbl =
{
    &IID_INetFwProfile,
    &INetFwProfile_ServerInfo,
    21,
    &INetFwProfile_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: INetFwPolicy, ver. 0.0,
   GUID={0xD46D2478,0x9AC9,0x4008,{0x9D,0xC7,0x55,0x63,0xCE,0x55,0x36,0xCC}} */

#pragma code_seg(".orpc")
static const unsigned short INetFwPolicy_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    2918,
    2956
    };

static const MIDL_STUBLESS_PROXY_INFO INetFwPolicy_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &INetFwPolicy_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO INetFwPolicy_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &INetFwPolicy_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(9) _INetFwPolicyProxyVtbl = 
{
    &INetFwPolicy_ProxyInfo,
    &IID_INetFwPolicy,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* INetFwPolicy::get_CurrentProfile */ ,
    (void *) (INT_PTR) -1 /* INetFwPolicy::GetProfileByType */
};


static const PRPC_STUB_FUNCTION INetFwPolicy_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _INetFwPolicyStubVtbl =
{
    &IID_INetFwPolicy,
    &INetFwPolicy_ServerInfo,
    9,
    &INetFwPolicy_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: INetFwMgr, ver. 0.0,
   GUID={0xF7898AF5,0xCAC4,0x4632,{0xA2,0xEC,0xDA,0x06,0xE5,0x11,0x1A,0xF2}} */

#pragma code_seg(".orpc")
static const unsigned short INetFwMgr_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    3000,
    1576,
    3038,
    3070,
    3144
    };

static const MIDL_STUBLESS_PROXY_INFO INetFwMgr_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &INetFwMgr_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO INetFwMgr_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &INetFwMgr_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(12) _INetFwMgrProxyVtbl = 
{
    &INetFwMgr_ProxyInfo,
    &IID_INetFwMgr,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* INetFwMgr::get_LocalPolicy */ ,
    (void *) (INT_PTR) -1 /* INetFwMgr::get_CurrentProfileType */ ,
    (void *) (INT_PTR) -1 /* INetFwMgr::RestoreDefaults */ ,
    (void *) (INT_PTR) -1 /* INetFwMgr::IsPortAllowed */ ,
    (void *) (INT_PTR) -1 /* INetFwMgr::IsIcmpTypeAllowed */
};


static const PRPC_STUB_FUNCTION INetFwMgr_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _INetFwMgrStubVtbl =
{
    &IID_INetFwMgr,
    &INetFwMgr_ServerInfo,
    12,
    &INetFwMgr_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    __MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x6000169, /* MIDL Version 6.0.361 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0   /* Reserved5 */
    };

const CInterfaceProxyVtbl * _netfw_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_INetFwIcmpSettingsProxyVtbl,
    ( CInterfaceProxyVtbl *) &_INetFwAuthorizedApplicationsProxyVtbl,
    ( CInterfaceProxyVtbl *) &_INetFwPolicyProxyVtbl,
    ( CInterfaceProxyVtbl *) &_INetFwOpenPortProxyVtbl,
    ( CInterfaceProxyVtbl *) &_INetFwServicesProxyVtbl,
    ( CInterfaceProxyVtbl *) &_INetFwServiceProxyVtbl,
    ( CInterfaceProxyVtbl *) &_INetFwProfileProxyVtbl,
    ( CInterfaceProxyVtbl *) &_INetFwRemoteAdminSettingsProxyVtbl,
    ( CInterfaceProxyVtbl *) &_INetFwMgrProxyVtbl,
    ( CInterfaceProxyVtbl *) &_INetFwAuthorizedApplicationProxyVtbl,
    ( CInterfaceProxyVtbl *) &_INetFwOpenPortsProxyVtbl,
    0
};

const CInterfaceStubVtbl * _netfw_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_INetFwIcmpSettingsStubVtbl,
    ( CInterfaceStubVtbl *) &_INetFwAuthorizedApplicationsStubVtbl,
    ( CInterfaceStubVtbl *) &_INetFwPolicyStubVtbl,
    ( CInterfaceStubVtbl *) &_INetFwOpenPortStubVtbl,
    ( CInterfaceStubVtbl *) &_INetFwServicesStubVtbl,
    ( CInterfaceStubVtbl *) &_INetFwServiceStubVtbl,
    ( CInterfaceStubVtbl *) &_INetFwProfileStubVtbl,
    ( CInterfaceStubVtbl *) &_INetFwRemoteAdminSettingsStubVtbl,
    ( CInterfaceStubVtbl *) &_INetFwMgrStubVtbl,
    ( CInterfaceStubVtbl *) &_INetFwAuthorizedApplicationStubVtbl,
    ( CInterfaceStubVtbl *) &_INetFwOpenPortsStubVtbl,
    0
};

PCInterfaceName const _netfw_InterfaceNamesList[] = 
{
    "INetFwIcmpSettings",
    "INetFwAuthorizedApplications",
    "INetFwPolicy",
    "INetFwOpenPort",
    "INetFwServices",
    "INetFwService",
    "INetFwProfile",
    "INetFwRemoteAdminSettings",
    "INetFwMgr",
    "INetFwAuthorizedApplication",
    "INetFwOpenPorts",
    0
};

const IID *  _netfw_BaseIIDList[] = 
{
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    0
};


#define _netfw_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _netfw, pIID, n)

int __stdcall _netfw_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _netfw, 11, 8 )
    IID_BS_LOOKUP_NEXT_TEST( _netfw, 4 )
    IID_BS_LOOKUP_NEXT_TEST( _netfw, 2 )
    IID_BS_LOOKUP_NEXT_TEST( _netfw, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _netfw, 11, *pIndex )
    
}

const ExtendedProxyFileInfo netfw_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _netfw_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _netfw_StubVtblList,
    (const PCInterfaceName * ) & _netfw_InterfaceNamesList,
    (const IID ** ) & _netfw_BaseIIDList,
    & _netfw_IID_Lookup, 
    11,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* defined(_M_IA64) || defined(_M_AMD64)*/

