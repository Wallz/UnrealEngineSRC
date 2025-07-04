/*++

Copyright (c) Microsoft Corporation. All rights reserved.

Module Name:

    rpcndr.h

Abstract:

    Definitions for stub data structures and prototypes of helper functions.

--*/

// This version of the rpcndr.h file corresponds to MIDL version 5.0.+
// used with Windows 2000/XP build 1700+


#ifndef __RPCNDR_H_VERSION__
#define __RPCNDR_H_VERSION__        ( 475 )
#endif // __RPCNDR_H_VERSION__


#ifndef __RPCNDR_H__
#define __RPCNDR_H__

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef __REQUIRED_RPCNDR_H_VERSION__
    #if ( __RPCNDR_H_VERSION__ < __REQUIRED_RPCNDR_H_VERSION__ )
        #error incorrect <rpcndr.h> version. Use the header that matches with the MIDL compiler.
    #endif
#endif


#include <pshpack8.h>
#include <basetsd.h>
#include <rpcnsip.h>


#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************

     Network Computing Architecture (NCA) definition:

     Network Data Representation: (NDR) Label format:
     An unsigned long (32 bits) with the following layout:

     3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
     1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
    +---------------+---------------+---------------+-------+-------+
    |   Reserved    |   Reserved    |Floating point | Int   | Char  |
    |               |               |Representation | Rep.  | Rep.  |
    +---------------+---------------+---------------+-------+-------+

     Where

         Reserved:

             Must be zero (0) for NCA 1.5 and NCA 2.0.

         Floating point Representation is:

             0 - IEEE
             1 - VAX
             2 - Cray
             3 - IBM

         Int Rep. is Integer Representation:

             0 - Big Endian
             1 - Little Endian

         Char Rep. is Character Representation:

             0 - ASCII
             1 - EBCDIC

     The Microsoft Local Data Representation (for all platforms which are
     of interest currently is edefined below:

 ****************************************************************************/

#define NDR_CHAR_REP_MASK               (unsigned long)0X0000000FL
#define NDR_INT_REP_MASK                (unsigned long)0X000000F0L
#define NDR_FLOAT_REP_MASK              (unsigned long)0X0000FF00L

#define NDR_LITTLE_ENDIAN               (unsigned long)0X00000010L
#define NDR_BIG_ENDIAN                  (unsigned long)0X00000000L

#define NDR_IEEE_FLOAT                  (unsigned long)0X00000000L
#define NDR_VAX_FLOAT                   (unsigned long)0X00000100L
#define NDR_IBM_FLOAT                   (unsigned long)0X00000300L

#define NDR_ASCII_CHAR                  (unsigned long)0X00000000L
#define NDR_EBCDIC_CHAR                 (unsigned long)0X00000001L

#if defined(__RPC_MAC__)
#define NDR_LOCAL_DATA_REPRESENTATION   (unsigned long)0X00000000L
#define NDR_LOCAL_ENDIAN                NDR_BIG_ENDIAN
#else
#define NDR_LOCAL_DATA_REPRESENTATION   (unsigned long)0X00000010L
#define NDR_LOCAL_ENDIAN                NDR_LITTLE_ENDIAN
#endif


/****************************************************************************
 *  Macros for targeted platforms
 ****************************************************************************/

#if (0x501 <= _WIN32_WINNT)
#define TARGET_IS_NT51_OR_LATER                   1
#else
#define TARGET_IS_NT51_OR_LATER                   0
#endif

#if (0x500 <= _WIN32_WINNT)
#define TARGET_IS_NT50_OR_LATER                   1
#else
#define TARGET_IS_NT50_OR_LATER                   0
#endif

#if (defined(_WIN32_DCOM) || 0x400 <= _WIN32_WINNT)
#define TARGET_IS_NT40_OR_LATER                   1
#else
#define TARGET_IS_NT40_OR_LATER                   0
#endif

#if (0x400 <= WINVER)
#define TARGET_IS_NT351_OR_WIN95_OR_LATER         1
#else
#define TARGET_IS_NT351_OR_WIN95_OR_LATER         0
#endif

/****************************************************************************
 *  Other MIDL base types / predefined types:
 ****************************************************************************/

#define small char
typedef unsigned char byte;
typedef byte cs_byte;
typedef unsigned char boolean;

#ifndef _HYPER_DEFINED
#define _HYPER_DEFINED

#if (!defined(_M_IX86) || (defined(_INTEGRAL_MAX_BITS) && _INTEGRAL_MAX_BITS >= 64))
#define  hyper           __int64
#define MIDL_uhyper  unsigned __int64
#else
typedef double  hyper;
typedef double MIDL_uhyper;
#endif

#endif // _HYPER_DEFINED

#ifndef _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED
#endif

#ifndef _SIZE_T_DEFINED
#ifdef __RPC_WIN64__
typedef unsigned __int64 size_t;
#else
typedef unsigned int     size_t;
#endif
#define _SIZE_T_DEFINED
#endif

#ifdef __RPC_WIN32__
#if   (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
#define __RPC_CALLEE       __stdcall
#else
#define __RPC_CALLEE
#endif
#endif

#ifndef __MIDL_USER_DEFINED
#define midl_user_allocate MIDL_user_allocate
#define midl_user_free     MIDL_user_free
#define __MIDL_USER_DEFINED
#endif

void  * __RPC_USER MIDL_user_allocate(size_t);
void             __RPC_USER MIDL_user_free( void  * );

#define RPC_VAR_ENTRY __cdecl


/* winnt only */
#if defined(_M_IX86) || defined(_M_AMD64) || defined(_M_IA64)
#define __MIDL_DECLSPEC_DLLIMPORT   __declspec(dllimport)
#define __MIDL_DECLSPEC_DLLEXPORT   __declspec(dllexport)
#else
#define __MIDL_DECLSPEC_DLLIMPORT
#define __MIDL_DECLSPEC_DLLEXPORT
#endif




/****************************************************************************
 * Context handle management related definitions:
 *
 * Client and Server Contexts.
 *
 ****************************************************************************/

typedef void  * NDR_CCONTEXT;

typedef struct
    {
    void  * pad[2];
    void  * userContext;
    }  * NDR_SCONTEXT;

#define NDRSContextValue(hContext) (&(hContext)->userContext)

#define cbNDRContext 20         /* size of context on WIRE */

typedef void (__RPC_USER  * NDR_RUNDOWN)(void  * context);

typedef void (__RPC_USER  * NDR_NOTIFY_ROUTINE)(void);
typedef void (__RPC_USER  * NDR_NOTIFY2_ROUTINE)(boolean flag);

typedef struct _SCONTEXT_QUEUE {
    unsigned long   NumberOfObjects;
    NDR_SCONTEXT  * ArrayOfObjects;
    } SCONTEXT_QUEUE,  * PSCONTEXT_QUEUE;

RPCRTAPI
RPC_BINDING_HANDLE
RPC_ENTRY
NDRCContextBinding (
    IN NDR_CCONTEXT     CContext
    );

RPCRTAPI
void
RPC_ENTRY
NDRCContextMarshall (
    IN  NDR_CCONTEXT    CContext,
    OUT void  *pBuff
    );

RPCRTAPI
void
RPC_ENTRY
NDRCContextUnmarshall (
    OUT NDR_CCONTEXT        *   pCContext,
    IN  RPC_BINDING_HANDLE      hBinding,
    IN  void                *   pBuff,
    IN  unsigned long           DataRepresentation
    );

RPCRTAPI
void
RPC_ENTRY
NDRSContextMarshall (
    IN  NDR_SCONTEXT    CContext,
    OUT void          * pBuff,
    IN  NDR_RUNDOWN     userRunDownIn
    );

RPCRTAPI
NDR_SCONTEXT
RPC_ENTRY
NDRSContextUnmarshall (
    IN  void          * pBuff,
    IN  unsigned long   DataRepresentation
    );

RPCRTAPI
void
RPC_ENTRY
NDRSContextMarshallEx (
    IN  RPC_BINDING_HANDLE  BindingHandle,
    IN  NDR_SCONTEXT        CContext,
    OUT void              * pBuff,
    IN  NDR_RUNDOWN         userRunDownIn
    );

RPCRTAPI
void
RPC_ENTRY
NDRSContextMarshall2 (
    IN  RPC_BINDING_HANDLE  BindingHandle,
    IN  NDR_SCONTEXT        CContext,
    OUT void              * pBuff,
    IN  NDR_RUNDOWN         userRunDownIn,
    IN  void              * CtxGuard,
    IN unsigned long        Flags
    );

RPCRTAPI
NDR_SCONTEXT
RPC_ENTRY
NDRSContextUnmarshallEx (
    IN  RPC_BINDING_HANDLE  BindingHandle,
    IN  void              * pBuff,
    IN  unsigned long       DataRepresentation
    );

RPCRTAPI
NDR_SCONTEXT
RPC_ENTRY
NDRSContextUnmarshall2(
    IN  RPC_BINDING_HANDLE  BindingHandle,
    IN  void              * pBuff,
    IN  unsigned long       DataRepresentation,
    IN  void              * CtxGuard,
    IN unsigned long        Flags
    );

RPCRTAPI
void
RPC_ENTRY
RpcSsDestroyClientContext (
    IN void  *  * ContextHandle
    );


/****************************************************************************
    NDR conversion related definitions.
 ****************************************************************************/

#define byte_from_ndr(source, target) \
    { \
    *(target) = *(*(char  *  *)&(source)->Buffer)++; \
    }

#define byte_array_from_ndr(Source, LowerIndex, UpperIndex, Target) \
    { \
    NDRcopy ( \
        (((char  *)(Target))+(LowerIndex)), \
        (Source)->Buffer, \
        (unsigned int)((UpperIndex)-(LowerIndex))); \
    *(unsigned long  *)&(Source)->Buffer += ((UpperIndex)-(LowerIndex)); \
    }

#define boolean_from_ndr(source, target) \
    { \
    *(target) = *(*(char  *  *)&(source)->Buffer)++; \
    }

#define boolean_array_from_ndr(Source, LowerIndex, UpperIndex, Target) \
    { \
    NDRcopy ( \
        (((char  *)(Target))+(LowerIndex)), \
        (Source)->Buffer, \
        (unsigned int)((UpperIndex)-(LowerIndex))); \
    *(unsigned long  *)&(Source)->Buffer += ((UpperIndex)-(LowerIndex)); \
    }

#define small_from_ndr(source, target) \
    { \
    *(target) = *(*(char  *  *)&(source)->Buffer)++; \
    }

#define small_from_ndr_temp(source, target, format) \
    { \
    *(target) = *(*(char  *  *)(source))++; \
    }

#define small_array_from_ndr(Source, LowerIndex, UpperIndex, Target) \
    { \
    NDRcopy ( \
        (((char  *)(Target))+(LowerIndex)), \
        (Source)->Buffer, \
        (unsigned int)((UpperIndex)-(LowerIndex))); \
    *(unsigned long  *)&(Source)->Buffer += ((UpperIndex)-(LowerIndex)); \
    }

/****************************************************************************
    Platform specific mapping of c-runtime functions.
 ****************************************************************************/

#if defined(__RPC_WIN32__) || defined(__RPC_WIN64__)
#define MIDL_ascii_strlen(string) \
    strlen(string)
#define MIDL_ascii_strcpy(target,source) \
    strcpy(target,source)
#define MIDL_memset(s,c,n) \
    memset(s,c,n)
#endif

/****************************************************************************
    MIDL 2.0 ndr definitions.
 ****************************************************************************/

typedef unsigned long error_status_t;

#define _midl_ma1( p, cast )    *(*( cast **)&p)++
#define _midl_ma2( p, cast )    *(*( cast **)&p)++
#define _midl_ma4( p, cast )    *(*( cast **)&p)++
#define _midl_ma8( p, cast )    *(*( cast **)&p)++

#define _midl_unma1( p, cast )  *(( cast *)p)++
#define _midl_unma2( p, cast )  *(( cast *)p)++
#define _midl_unma3( p, cast )  *(( cast *)p)++
#define _midl_unma4( p, cast )  *(( cast *)p)++

// Some alignment specific macros.

// RKK64
// these appear to be used in fossils inside MIDL
#define _midl_fa2( p )          (p = (RPC_BUFPTR )((ULONG_PTR)(p+1) & ~0x1))
#define _midl_fa4( p )          (p = (RPC_BUFPTR )((ULONG_PTR)(p+3) & ~0x3))
#define _midl_fa8( p )          (p = (RPC_BUFPTR )((ULONG_PTR)(p+7) & ~0x7))

#define _midl_addp( p, n )      (p += n)

// Marshalling macros

#define _midl_marsh_lhs( p, cast )  *(*( cast **)&p)++
#define _midl_marsh_up( mp, p )     *(*(unsigned long **)&mp)++ = (unsigned long)p
#define _midl_advmp( mp )           *(*(unsigned long **)&mp)++
#define _midl_unmarsh_up( p )       (*(*(unsigned long **)&p)++)


////////////////////////////////////////////////////////////////////////////
// Ndr macros.
////////////////////////////////////////////////////////////////////////////

// RKK64
// these appear to be used in fossils inside MIDL
#define NdrMarshConfStringHdr( p, s, l )    (_midl_ma4( p, unsigned long) = s, \
                                            _midl_ma4( p, unsigned long) = 0, \
                                            _midl_ma4( p, unsigned long) = l)

#define NdrUnMarshConfStringHdr(p, s, l)    ((s=_midl_unma4(p,unsigned long),\
                                            (_midl_addp(p,4)),               \
                                            (l=_midl_unma4(p,unsigned long))

#define NdrMarshCCtxtHdl(pc,p)  (NDRCContextMarshall( (NDR_CCONTEXT)pc, p ),p+20)

#define NdrUnMarshCCtxtHdl(pc,p,h,drep) \
        (NDRCContextUnmarshall((NDR_CONTEXT)pc,h,p,drep), p+20)

#define NdrUnMarshSCtxtHdl(pc, p,drep)  (pc = NdrSContextUnMarshall(p,drep ))


#define NdrMarshSCtxtHdl(pc,p,rd)   (NdrSContextMarshall((NDR_SCONTEXT)pc,p, (NDR_RUNDOWN)rd)

// end of unused

#define NdrFieldOffset(s,f)     (LONG_PTR)(& (((s  *)0)->f))
#define NdrFieldPad(s,f,p,t)    ((unsigned long)(NdrFieldOffset(s,f) - NdrFieldOffset(s,p)) - sizeof(t))

#define NdrFcShort(s)   (unsigned char)(s & 0xff), (unsigned char)(s >> 8)
#define NdrFcLong(s)    (unsigned char)(s & 0xff), (unsigned char)((s & 0x0000ff00) >> 8), \
                        (unsigned char)((s & 0x00ff0000) >> 16), (unsigned char)(s >> 24)

//
// On the server side, the following exceptions are mapped to
// the bad stub data exception if -error stub_data is used.
//

#define RPC_BAD_STUB_DATA_EXCEPTION_FILTER  \
                 ( (RpcExceptionCode() == STATUS_ACCESS_VIOLATION)  || \
                   (RpcExceptionCode() == STATUS_DATATYPE_MISALIGNMENT) || \
                   (RpcExceptionCode() == RPC_X_BAD_STUB_DATA) || \
                   (RpcExceptionCode() == RPC_S_INVALID_BOUND) )

/////////////////////////////////////////////////////////////////////////////
// Some stub helper functions.
/////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// Stub helper structures.
////////////////////////////////////////////////////////////////////////////

struct _MIDL_STUB_MESSAGE;
struct _MIDL_STUB_DESC;
struct _FULL_PTR_XLAT_TABLES;

typedef unsigned char  *    RPC_BUFPTR;
typedef unsigned long       RPC_LENGTH;

// Expression evaluation callback routine prototype.
typedef void (__RPC_USER  * EXPR_EVAL)( struct _MIDL_STUB_MESSAGE  * );

typedef const unsigned char  * PFORMAT_STRING;

/*
 * Multidimensional conformant/varying array struct.
 */
typedef struct
    {
    long              Dimension;

    /* These fields MUST be (unsigned long *) */
    unsigned long  *  BufferConformanceMark;
    unsigned long  *  BufferVarianceMark;

    /* Count arrays, used for top level arrays in -Os stubs */
    unsigned long  *  MaxCountArray;
    unsigned long  *  OffsetArray;
    unsigned long  *  ActualCountArray;
    } ARRAY_INFO,  *PARRAY_INFO;


typedef struct _NDR_ASYNC_MESSAGE *   PNDR_ASYNC_MESSAGE;
typedef struct _NDR_CORRELATION_INFO *PNDR_CORRELATION_INFO;

/*
 * cs_char info in the stub message
 */

typedef struct
    {
    unsigned long   WireCodeset;
    unsigned long   DesiredReceivingCodeset;
    void           *CSArrayInfo;
    } CS_STUB_INFO;

/*
 * MIDL Stub Message
 */

typedef const unsigned char  * PFORMAT_STRING;
typedef struct _MIDL_SYNTAX_INFO MIDL_SYNTAX_INFO, *PMIDL_SYNTAX_INFO;

struct NDR_ALLOC_ALL_NODES_CONTEXT;
struct NDR_POINTER_QUEUE_STATE;
struct _NDR_PROC_CONTEXT;

typedef struct _MIDL_STUB_MESSAGE
    {
    /* RPC message structure. */
    PRPC_MESSAGE            RpcMsg;

    /* Pointer into RPC message buffer. */
    unsigned char       *   Buffer;

    /*
     * These are used internally by the Ndr routines to mark the beginning
     * and end of an incoming RPC buffer.
     */
    unsigned char       *   BufferStart;
    unsigned char       *   BufferEnd;

    /*
     * Used internally by the Ndr routines as a place holder in the buffer.
     * On the marshalling side it's used to mark the location where conformance
     * size should be marshalled.
     * On the unmarshalling side it's used to mark the location in the buffer
     * used during pointer unmarshalling to base pointer offsets off of.
     */
    unsigned char       *   BufferMark;

    /* Set by the buffer sizing routines. */
    unsigned long           BufferLength;

    /* Set by the memory sizing routines. */
    unsigned long           MemorySize;

    /* Pointer to user memory. */
    unsigned char       *   Memory;

    /* Is the Ndr routine begin called from a client side stub. */
    int                     IsClient;

    /* Can the buffer be re-used for memory on unmarshalling. */
    int                     ReuseBuffer;

    /* Hold the context for allocate all nodes */
    struct NDR_ALLOC_ALL_NODES_CONTEXT *pAllocAllNodesContext;
    struct NDR_POINTER_QUEUE_STATE     *pPointerQueueState;

    /*
     * Stuff needed while handling complex structures
     */

    /* Ignore imbeded pointers while computing buffer or memory sizes. */
    int                     IgnoreEmbeddedPointers;

    /*
     * This marks the location in the buffer where pointees of a complex
     * struct reside.
     */
    unsigned char       *   PointerBufferMark;

    /*
     * Used to catch errors in SendReceive.
     */
    unsigned char           fBufferValid;

    unsigned char           uFlags;
    unsigned short          Unused2;

    /*
     * Used internally by the Ndr routines.  Holds the max counts for
     * a conformant array.
     */
    ULONG_PTR               MaxCount;

    /*
     * Used internally by the Ndr routines.  Holds the offsets for a varying
     * array.
     */
    unsigned long           Offset;

    /*
     * Used internally by the Ndr routines.  Holds the actual counts for
     * a varying array.
     */
    unsigned long           ActualCount;

    /* Allocation and Free routine to be used by the Ndr routines. */
    void  *                 ( __RPC_API * pfnAllocate)(size_t);
    void                    ( __RPC_API * pfnFree)(void  *);

    /*
     * Top of parameter stack.  Used for "single call" stubs during marshalling
     * to hold the beginning of the parameter list on the stack.  Needed to
     * extract parameters which hold attribute values for top level arrays and
     * pointers.
     */
    unsigned char       *   StackTop;

    /*
     *  Fields used for the transmit_as and represent_as objects.
     *  For represent_as the mapping is: presented=local, transmit=named.
     */
    unsigned char       *   pPresentedType;
    unsigned char       *   pTransmitType;

    /*
     * When we first construct a binding on the client side, stick it
     * in the rpcmessage and later call RpcGetBuffer, the handle field
     * in the rpcmessage is changed. That's fine except that we need to
     * have that original handle for use in unmarshalling context handles
     * (the second argument in NDRCContextUnmarshall to be exact). So
     * stash the contructed handle here and extract it when needed.
     */
    handle_t                SavedHandle;

    /*
     * Pointer back to the stub descriptor.  Use this to get all handle info.
     */
    const struct _MIDL_STUB_DESC  * StubDesc;

    /*
     * Full pointer stuff.
     */
    struct _FULL_PTR_XLAT_TABLES  * FullPtrXlatTables;
    unsigned long                   FullPtrRefId;

    unsigned long                   PointerLength;

    int                             fInDontFree       :1;
    int                             fDontCallFreeInst :1;
    int                             fInOnlyParam      :1;
    int                             fHasReturn        :1;
    int                             fHasExtensions    :1;
    int                             fHasNewCorrDesc   :1;
    int                             fIsOicfServer    :1;
    int                             fUnused           :9;
    int                             fUnused2          :16;


    unsigned long                   dwDestContext;
    void  *                         pvDestContext;

    NDR_SCONTEXT *                  SavedContextHandles;

    long                            ParamNumber;

    struct IRpcChannelBuffer    *   pRpcChannelBuffer;

    PARRAY_INFO                     pArrayInfo;
    unsigned long           *       SizePtrCountArray;
    unsigned long           *       SizePtrOffsetArray;
    unsigned long           *       SizePtrLengthArray;

    /*
     * Interpreter argument queue.  Used on server side only.
     */
    void                    *       pArgQueue;

    unsigned long                   dwStubPhase;

    void                    *       LowStackMark;

    /*
     *  Async message pointer, correlation data - NT 5.0 features.
     */
    PNDR_ASYNC_MESSAGE              pAsyncMsg;
    PNDR_CORRELATION_INFO           pCorrInfo;
    unsigned char *                 pCorrMemory;

    void *                          pMemoryList;

    /*
     *  Reserved fields up to this point present since the 3.50 release.
     *  Reserved fields below were introduced for Windows 2000 release.
     *  (but not used).
     */

    /*
     * International character support information - NT 5.1 feature.
     */

    CS_STUB_INFO *                  pCSInfo;

    unsigned char *                 ConformanceMark;
    unsigned char *                 VarianceMark;

#if defined(IA64)
    void                   *        BackingStoreLowMark;
#else
    INT_PTR                         Unused;
#endif

    struct _NDR_PROC_CONTEXT *      pContext;

    /*
     *  Reserved fields up to this point present since Windows 2000 release.
     *  Fields added for NT5.1
     */

    void *                             pUserMarshalList;
    INT_PTR                         Reserved51_2;
    INT_PTR                         Reserved51_3;
    INT_PTR                         Reserved51_4;
    INT_PTR                         Reserved51_5;


    /*
     *  Reserved fields up to this point present since NT5.1 release.
     */
    } MIDL_STUB_MESSAGE,  *PMIDL_STUB_MESSAGE;


typedef  struct _MIDL_STUB_MESSAGE MIDL_STUB_MESSAGE,   *PMIDL_STUB_MESSAGE;

/*
 * Generic handle bind/unbind routine pair.
 */
typedef void  *
        ( __RPC_API * GENERIC_BINDING_ROUTINE)
        (void  *);
typedef void
        ( __RPC_API * GENERIC_UNBIND_ROUTINE)
        (void  *, unsigned char  *);

typedef struct _GENERIC_BINDING_ROUTINE_PAIR
    {
    GENERIC_BINDING_ROUTINE     pfnBind;
    GENERIC_UNBIND_ROUTINE      pfnUnbind;
    } GENERIC_BINDING_ROUTINE_PAIR,  *PGENERIC_BINDING_ROUTINE_PAIR;

typedef struct __GENERIC_BINDING_INFO
    {
    void  *            pObj;
    unsigned int                Size;
    GENERIC_BINDING_ROUTINE     pfnBind;
    GENERIC_UNBIND_ROUTINE      pfnUnbind;
    } GENERIC_BINDING_INFO,  *PGENERIC_BINDING_INFO;

// typedef EXPR_EVAL - see above
// typedefs for xmit_as

#if (defined(_MSC_VER)) && !defined(MIDL_PASS)
// a Microsoft C++ compiler
#define NDR_SHAREABLE __inline
#else
#define NDR_SHAREABLE static
#endif


typedef void ( __RPC_USER * XMIT_HELPER_ROUTINE)
    ( PMIDL_STUB_MESSAGE );

typedef struct _XMIT_ROUTINE_QUINTUPLE
    {
    XMIT_HELPER_ROUTINE     pfnTranslateToXmit;
    XMIT_HELPER_ROUTINE     pfnTranslateFromXmit;
    XMIT_HELPER_ROUTINE     pfnFreeXmit;
    XMIT_HELPER_ROUTINE     pfnFreeInst;
    } XMIT_ROUTINE_QUINTUPLE,  *PXMIT_ROUTINE_QUINTUPLE;

typedef unsigned long
( __RPC_USER * USER_MARSHAL_SIZING_ROUTINE)
    (unsigned long  *,
     unsigned long,
     void  * );

typedef unsigned char  *
( __RPC_USER * USER_MARSHAL_MARSHALLING_ROUTINE)
    (unsigned long  *,
     unsigned char  * ,
     void  * );

typedef unsigned char  *
( __RPC_USER * USER_MARSHAL_UNMARSHALLING_ROUTINE)
    (unsigned long  *,
     unsigned char  *,
     void  * );

typedef void ( __RPC_USER * USER_MARSHAL_FREEING_ROUTINE)
    (unsigned long  *,
     void  * );

typedef struct _USER_MARSHAL_ROUTINE_QUADRUPLE
    {
    USER_MARSHAL_SIZING_ROUTINE          pfnBufferSize;
    USER_MARSHAL_MARSHALLING_ROUTINE     pfnMarshall;
    USER_MARSHAL_UNMARSHALLING_ROUTINE   pfnUnmarshall;
    USER_MARSHAL_FREEING_ROUTINE         pfnFree;
    } USER_MARSHAL_ROUTINE_QUADRUPLE;

#define USER_MARSHAL_CB_SIGNATURE 'USRC'

typedef enum _USER_MARSHAL_CB_TYPE
{
    USER_MARSHAL_CB_BUFFER_SIZE,
    USER_MARSHAL_CB_MARSHALL,
    USER_MARSHAL_CB_UNMARSHALL,
    USER_MARSHAL_CB_FREE
} USER_MARSHAL_CB_TYPE;

typedef struct _USER_MARSHAL_CB
{
    unsigned long           Flags;
    PMIDL_STUB_MESSAGE      pStubMsg;
    PFORMAT_STRING          pReserve;
    unsigned long           Signature;
    USER_MARSHAL_CB_TYPE    CBType;
    PFORMAT_STRING          pFormat;
    PFORMAT_STRING          pTypeFormat;
} USER_MARSHAL_CB;


#define USER_CALL_CTXT_MASK(f)  ((f) & 0x00ff)
#define USER_CALL_AUX_MASK(f)   ((f) & 0xff00)
#define GET_USER_DATA_REP(f)    ((f) >> 16)

#define USER_CALL_IS_ASYNC              0x0100  /* aux flag: in an [async] call */
#define USER_CALL_NEW_CORRELATION_DESC  0x0200

typedef struct _MALLOC_FREE_STRUCT
    {
    void  *     ( __RPC_USER * pfnAllocate)(size_t);
    void        ( __RPC_USER * pfnFree)(void  *);
    } MALLOC_FREE_STRUCT;

typedef struct _COMM_FAULT_OFFSETS
    {
    short       CommOffset;
    short       FaultOffset;
    } COMM_FAULT_OFFSETS;

/*
 * International character support definitions
 */

typedef enum _IDL_CS_CONVERT
    {
    IDL_CS_NO_CONVERT,
    IDL_CS_IN_PLACE_CONVERT,
    IDL_CS_NEW_BUFFER_CONVERT
    } IDL_CS_CONVERT;

typedef void
( __RPC_USER * CS_TYPE_NET_SIZE_ROUTINE)
    (RPC_BINDING_HANDLE     hBinding,
     unsigned long          ulNetworkCodeSet,
     unsigned long          ulLocalBufferSize,
     IDL_CS_CONVERT     *   conversionType,
     unsigned long      *   pulNetworkBufferSize,
     error_status_t     *   pStatus);

typedef void
( __RPC_USER * CS_TYPE_LOCAL_SIZE_ROUTINE)
    (RPC_BINDING_HANDLE     hBinding,
     unsigned long          ulNetworkCodeSet,
     unsigned long          ulNetworkBufferSize,
     IDL_CS_CONVERT     *   conversionType,
     unsigned long      *   pulLocalBufferSize,
     error_status_t     *   pStatus);

typedef void
( __RPC_USER * CS_TYPE_TO_NETCS_ROUTINE)
    (RPC_BINDING_HANDLE     hBinding,
     unsigned long          ulNetworkCodeSet,
     void               *   pLocalData,
     unsigned long          ulLocalDataLength,
     byte               *   pNetworkData,
     unsigned long      *   pulNetworkDataLength,
     error_status_t     *   pStatus);

typedef void
( __RPC_USER * CS_TYPE_FROM_NETCS_ROUTINE)
    (RPC_BINDING_HANDLE     hBinding,
     unsigned long          ulNetworkCodeSet,
     byte               *   pNetworkData,
     unsigned long          ulNetworkDataLength,
     unsigned long          ulLocalBufferSize,
     void               *   pLocalData,
     unsigned long      *   pulLocalDataLength,
     error_status_t     *   pStatus);

typedef void
( __RPC_USER * CS_TAG_GETTING_ROUTINE)
    (RPC_BINDING_HANDLE     hBinding,
     int                    fServerSide,
     unsigned long      *   pulSendingTag,
     unsigned long      *   pulDesiredReceivingTag,
     unsigned long      *   pulReceivingTag,
     error_status_t     *   pStatus);

void __RPC_USER
RpcCsGetTags(
     RPC_BINDING_HANDLE     hBinding,
     int                    fServerSide,
     unsigned long      *   pulSendingTag,
     unsigned long      *   pulDesiredReceivingTag,
     unsigned long      *   pulReceivingTag,
     error_status_t     *   pStatus);

typedef struct _NDR_CS_SIZE_CONVERT_ROUTINES
    {
    CS_TYPE_NET_SIZE_ROUTINE    pfnNetSize;
    CS_TYPE_TO_NETCS_ROUTINE    pfnToNetCs;
    CS_TYPE_LOCAL_SIZE_ROUTINE  pfnLocalSize;
    CS_TYPE_FROM_NETCS_ROUTINE  pfnFromNetCs;
    } NDR_CS_SIZE_CONVERT_ROUTINES;

typedef struct _NDR_CS_ROUTINES
    {
    NDR_CS_SIZE_CONVERT_ROUTINES   *pSizeConvertRoutines;
    CS_TAG_GETTING_ROUTINE         *pTagGettingRoutines;
    } NDR_CS_ROUTINES;

/*
 * MIDL Stub Descriptor
 */

typedef struct _MIDL_STUB_DESC
    {
    void  *    RpcInterfaceInformation;

    void  *    ( __RPC_API * pfnAllocate)(size_t);
    void       ( __RPC_API * pfnFree)(void  *);

    union
        {
        handle_t  *             pAutoHandle;
        handle_t  *             pPrimitiveHandle;
        PGENERIC_BINDING_INFO   pGenericBindingInfo;
        } IMPLICIT_HANDLE_INFO;

    const NDR_RUNDOWN  *                    apfnNdrRundownRoutines;
    const GENERIC_BINDING_ROUTINE_PAIR  *   aGenericBindingRoutinePairs;
    const EXPR_EVAL  *                      apfnExprEval;
    const XMIT_ROUTINE_QUINTUPLE  *         aXmitQuintuple;

    const unsigned char  *                  pFormatTypes;

    int                                     fCheckBounds;

    /* Ndr library version. */
    unsigned long                           Version;

    MALLOC_FREE_STRUCT  *                   pMallocFreeStruct;

    long                                    MIDLVersion;

    const COMM_FAULT_OFFSETS  *    CommFaultOffsets;

    // New fields for version 3.0+
    const USER_MARSHAL_ROUTINE_QUADRUPLE  * aUserMarshalQuadruple;

    // Notify routines - added for NT5, MIDL 5.0
    const NDR_NOTIFY_ROUTINE  *             NotifyRoutineTable;

    /*
     * Reserved for future use.
     */

    ULONG_PTR                               mFlags;

    // International support routines - added for 64bit post NT5
    const NDR_CS_ROUTINES *                 CsRoutineTables;

    void *                                  Reserved4;
    ULONG_PTR                               Reserved5;

    // Fields up to now present in win2000 release.

    } MIDL_STUB_DESC;


typedef const MIDL_STUB_DESC  * PMIDL_STUB_DESC;

typedef void  * PMIDL_XMIT_TYPE;

/*
 * MIDL Stub Format String.  This is a const in the stub.
 */
#if !defined( RC_INVOKED )
#if _MSC_VER >= 1200
#pragma warning(push)
#endif
#pragma warning( disable:4200 )
#endif
typedef struct _MIDL_FORMAT_STRING
    {
    short               Pad;
    unsigned char       Format[];
    } MIDL_FORMAT_STRING;
#if !defined( RC_INVOKED )
#if _MSC_VER >= 1200
#pragma warning(pop)
#else
#pragma warning( default:4200 )
#endif
#endif

/*
 * Stub thunk used for some interpreted server stubs.
 */
typedef void ( __RPC_API * STUB_THUNK)( PMIDL_STUB_MESSAGE );

typedef long ( __RPC_API * SERVER_ROUTINE)();

/*
 * Server Interpreter's information strucuture.
 */
typedef struct  _MIDL_SERVER_INFO_
    {
    PMIDL_STUB_DESC                     pStubDesc;
    const SERVER_ROUTINE     *          DispatchTable;
    PFORMAT_STRING                      ProcString;
    const unsigned short *              FmtStringOffset;
    const STUB_THUNK *                  ThunkTable;
    PRPC_SYNTAX_IDENTIFIER              pTransferSyntax;
    ULONG_PTR                           nCount;
    PMIDL_SYNTAX_INFO                   pSyntaxInfo;
    } MIDL_SERVER_INFO, *PMIDL_SERVER_INFO;

#undef _MIDL_STUBLESS_PROXY_INFO

/*
 * Stubless object proxy information structure.
 */
typedef struct _MIDL_STUBLESS_PROXY_INFO
    {
    PMIDL_STUB_DESC                     pStubDesc;
    PFORMAT_STRING                      ProcFormatString;
    const unsigned short            *   FormatStringOffset;
    PRPC_SYNTAX_IDENTIFIER              pTransferSyntax;
    ULONG_PTR                           nCount;
    PMIDL_SYNTAX_INFO                   pSyntaxInfo;
    } MIDL_STUBLESS_PROXY_INFO;

typedef MIDL_STUBLESS_PROXY_INFO  * PMIDL_STUBLESS_PROXY_INFO;

/*
 *  Multiple transfer syntax information.
 */
typedef struct _MIDL_SYNTAX_INFO
{
RPC_SYNTAX_IDENTIFIER               TransferSyntax;
RPC_DISPATCH_TABLE *                DispatchTable;
PFORMAT_STRING                      ProcString;
const unsigned short *              FmtStringOffset;
PFORMAT_STRING                      TypeString;
const void           *              aUserMarshalQuadruple;
ULONG_PTR                           pReserved1;
ULONG_PTR                           pReserved2;
} MIDL_SYNTAX_INFO, *PMIDL_SYNTAX_INFO;

typedef unsigned short * PARAM_OFFSETTABLE, *PPARAM_OFFSETTABLE;

/*
 * This is the return value from NdrClientCall.
 */
typedef union _CLIENT_CALL_RETURN
    {
    void  *         Pointer;
    LONG_PTR        Simple;
    } CLIENT_CALL_RETURN;

/*
 * Full pointer data structures.
 */

typedef enum
        {
        XLAT_SERVER = 1,
        XLAT_CLIENT
        } XLAT_SIDE;

/*
 * Stores the translation for the conversion from a full pointer into it's
 * corresponding ref id.
 */
typedef struct _FULL_PTR_TO_REFID_ELEMENT
    {
    struct _FULL_PTR_TO_REFID_ELEMENT  *  Next;

    void  *             Pointer;
    unsigned long       RefId;
    unsigned char       State;
    } FULL_PTR_TO_REFID_ELEMENT,  *PFULL_PTR_TO_REFID_ELEMENT;

/*
 * Full pointer translation tables.
 */
typedef struct _FULL_PTR_XLAT_TABLES
    {
    /*
     * Ref id to pointer translation information.
     */
    struct
        {
        void  * *           XlatTable;
        unsigned char  *    StateTable;
        unsigned long       NumberOfEntries;
        } RefIdToPointer;

    /*
     * Pointer to ref id translation information.
     */
    struct
        {
        PFULL_PTR_TO_REFID_ELEMENT  *   XlatTable;
        unsigned long                   NumberOfBuckets;
        unsigned long                   HashMask;
        } PointerToRefId;

    /*
     * Next ref id to use.
     */
    unsigned long           NextRefId;

    /*
     * Keep track of the translation size we're handling : server or client.
     * This tells us when we have to do reverse translations when we insert
     * new translations.  On the server we must insert a pointer-to-refid
     * translation whenever we insert a refid-to-pointer translation, and
     * vica versa for the client.
     */
    XLAT_SIDE               XlatSide;
    } FULL_PTR_XLAT_TABLES,  *PFULL_PTR_XLAT_TABLES;

/***************************************************************************
 ** New MIDL 2.0 Ndr routine templates
 ***************************************************************************/

RPC_STATUS RPC_ENTRY
NdrClientGetSupportedSyntaxes(
    IN RPC_CLIENT_INTERFACE * pInf,
    OUT unsigned long       * pCount,
    OUT MIDL_SYNTAX_INFO   ** pArr );


RPC_STATUS RPC_ENTRY
NdrServerGetSupportedSyntaxes(
    IN RPC_SERVER_INTERFACE * pInf,
    OUT unsigned long       * pCount,
    OUT MIDL_SYNTAX_INFO   ** pArr,
    OUT unsigned long       * pPreferSyntaxIndex);

/*
 * Marshall routines
 */

RPCRTAPI
void
RPC_ENTRY
NdrSimpleTypeMarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    unsigned char           FormatChar
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrPointerMarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrCsArrayMarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrCsTagMarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

/* Structures */

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrSimpleStructMarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrConformantStructMarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrConformantVaryingStructMarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrComplexStructMarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

/* Arrays */

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrFixedArrayMarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrConformantArrayMarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrConformantVaryingArrayMarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrVaryingArrayMarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrComplexArrayMarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

/* Strings */

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrNonConformantStringMarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrConformantStringMarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

/* Unions */

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrEncapsulatedUnionMarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrNonEncapsulatedUnionMarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

/* Byte count pointer */

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrByteCountPointerMarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

/* Transmit as and represent as*/

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrXmitOrRepAsMarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

/* User_marshal */

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrUserMarshalMarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

/* Interface pointer */

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrInterfacePointerMarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

/* Context handles */

RPCRTAPI
void
RPC_ENTRY
NdrClientContextMarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    NDR_CCONTEXT            ContextHandle,
    int                     fCheck
    );

RPCRTAPI
void
RPC_ENTRY
NdrServerContextMarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    NDR_SCONTEXT            ContextHandle,
    NDR_RUNDOWN             RundownRoutine
    );

RPCRTAPI
void
RPC_ENTRY
NdrServerContextNewMarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    NDR_SCONTEXT            ContextHandle,
    NDR_RUNDOWN             RundownRoutine,
    PFORMAT_STRING          pFormat
    );

/*
 * Unmarshall routines
 */

RPCRTAPI
void
RPC_ENTRY
NdrSimpleTypeUnmarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    unsigned char           FormatChar
    );

RPCRTAPI
unsigned char * RPC_ENTRY
RPC_ENTRY
NdrCsArrayUnmarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char **        ppMemory,
    PFORMAT_STRING          pFormat,
    unsigned char           fMustAlloc
    );

RPCRTAPI
unsigned char * RPC_ENTRY
RPC_ENTRY
NdrCsTagUnmarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char **        ppMemory,
    PFORMAT_STRING          pFormat,
    unsigned char           fMustAlloc
    );

RPCRTAPI
unsigned char * RPC_ENTRY
NdrRangeUnmarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char **        ppMemory,
    PFORMAT_STRING          pFormat,
    unsigned char           fMustAlloc
    );

RPCRTAPI
void
RPC_ENTRY
NdrCorrelationInitialize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    void  *                 pMemory,
    unsigned long           CacheSize,
    unsigned long           flags
    );

RPCRTAPI
void
RPC_ENTRY
NdrCorrelationPass(
    PMIDL_STUB_MESSAGE      pStubMsg
    );

RPCRTAPI
void
RPC_ENTRY
NdrCorrelationFree(
    PMIDL_STUB_MESSAGE      pStubMsg
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrPointerUnmarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *  *     ppMemory,
    PFORMAT_STRING          pFormat,
    unsigned char           fMustAlloc
    );

/* Structures */

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrSimpleStructUnmarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *  *     ppMemory,
    PFORMAT_STRING          pFormat,
    unsigned char           fMustAlloc
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrConformantStructUnmarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *  *     ppMemory,
    PFORMAT_STRING          pFormat,
    unsigned char           fMustAlloc
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrConformantVaryingStructUnmarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *  *     ppMemory,
    PFORMAT_STRING          pFormat,
    unsigned char           fMustAlloc
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrComplexStructUnmarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *  *     ppMemory,
    PFORMAT_STRING          pFormat,
    unsigned char           fMustAlloc
    );

/* Arrays */

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrFixedArrayUnmarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *  *     ppMemory,
    PFORMAT_STRING          pFormat,
    unsigned char           fMustAlloc
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrConformantArrayUnmarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *  *     ppMemory,
    PFORMAT_STRING          pFormat,
    unsigned char           fMustAlloc
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrConformantVaryingArrayUnmarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *  *     ppMemory,
    PFORMAT_STRING          pFormat,
    unsigned char           fMustAlloc
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrVaryingArrayUnmarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *  *     ppMemory,
    PFORMAT_STRING          pFormat,
    unsigned char           fMustAlloc
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrComplexArrayUnmarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *  *     ppMemory,
    PFORMAT_STRING          pFormat,
    unsigned char           fMustAlloc
    );

/* Strings */

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrNonConformantStringUnmarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *  *     ppMemory,
    PFORMAT_STRING          pFormat,
    unsigned char           fMustAlloc
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrConformantStringUnmarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *  *     ppMemory,
    PFORMAT_STRING          pFormat,
    unsigned char           fMustAlloc
    );

/* Unions */

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrEncapsulatedUnionUnmarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *  *     ppMemory,
    PFORMAT_STRING          pFormat,
    unsigned char           fMustAlloc
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrNonEncapsulatedUnionUnmarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *  *     ppMemory,
    PFORMAT_STRING          pFormat,
    unsigned char           fMustAlloc
    );

/* Byte count pointer */

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrByteCountPointerUnmarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *  *     ppMemory,
    PFORMAT_STRING          pFormat,
    unsigned char           fMustAlloc
    );

/* Transmit as and represent as*/

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrXmitOrRepAsUnmarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *  *     ppMemory,
    PFORMAT_STRING          pFormat,
    unsigned char           fMustAlloc
    );

/* User_marshal */

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrUserMarshalUnmarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *  *     ppMemory,
    PFORMAT_STRING          pFormat,
    unsigned char           fMustAlloc
    );

/* Interface pointer */

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrInterfacePointerUnmarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *  *     ppMemory,
    PFORMAT_STRING          pFormat,
    unsigned char           fMustAlloc
    );

/* Context handles */

RPCRTAPI
void
RPC_ENTRY
NdrClientContextUnmarshall(
    PMIDL_STUB_MESSAGE      pStubMsg,
    NDR_CCONTEXT        *   pContextHandle,
    RPC_BINDING_HANDLE      BindHandle
    );

RPCRTAPI
NDR_SCONTEXT
RPC_ENTRY
NdrServerContextUnmarshall(
    PMIDL_STUB_MESSAGE      pStubMsg
    );

/* New context handle flavors */

RPCRTAPI
NDR_SCONTEXT
RPC_ENTRY
NdrContextHandleInitialize(
    IN  PMIDL_STUB_MESSAGE  pStubMsg,
    IN  PFORMAT_STRING      pFormat
    );

RPCRTAPI
NDR_SCONTEXT
RPC_ENTRY
NdrServerContextNewUnmarshall(
    IN  PMIDL_STUB_MESSAGE  pStubMsg,
    IN  PFORMAT_STRING      pFormat
    );

/*
 * Buffer sizing routines
 */

RPCRTAPI
void
RPC_ENTRY
NdrPointerBufferSize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
void
RPC_ENTRY
NdrCsArrayBufferSize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
void
RPC_ENTRY
NdrCsTagBufferSize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

/* Structures */

RPCRTAPI
void
RPC_ENTRY
NdrSimpleStructBufferSize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
void
RPC_ENTRY
NdrConformantStructBufferSize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
void
RPC_ENTRY
NdrConformantVaryingStructBufferSize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
void
RPC_ENTRY
NdrComplexStructBufferSize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

/* Arrays */

RPCRTAPI
void
RPC_ENTRY
NdrFixedArrayBufferSize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
void
RPC_ENTRY
NdrConformantArrayBufferSize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
void
RPC_ENTRY
NdrConformantVaryingArrayBufferSize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
void
RPC_ENTRY
NdrVaryingArrayBufferSize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
void
RPC_ENTRY
NdrComplexArrayBufferSize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

/* Strings */

RPCRTAPI
void
RPC_ENTRY
NdrConformantStringBufferSize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
void
RPC_ENTRY
NdrNonConformantStringBufferSize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

/* Unions */

RPCRTAPI
void
RPC_ENTRY
NdrEncapsulatedUnionBufferSize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
void
RPC_ENTRY
NdrNonEncapsulatedUnionBufferSize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

/* Byte count pointer */

RPCRTAPI
void
RPC_ENTRY
NdrByteCountPointerBufferSize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

/* Transmit as and represent as*/

RPCRTAPI
void
RPC_ENTRY
NdrXmitOrRepAsBufferSize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

/* User_marshal */

RPCRTAPI
void
RPC_ENTRY
NdrUserMarshalBufferSize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

/* Interface pointer */

RPCRTAPI
void
RPC_ENTRY
NdrInterfacePointerBufferSize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

// Context Handle size
//
RPCRTAPI
void
RPC_ENTRY
NdrContextHandleSize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

/*
 * Memory sizing routines
 */

RPCRTAPI
unsigned long
RPC_ENTRY
NdrPointerMemorySize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    PFORMAT_STRING          pFormat
    );

/* cs_char things */

RPCRTAPI
unsigned long
RPC_ENTRY
NdrCsArrayMemorySize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
unsigned long
RPC_ENTRY
NdrCsTagMemorySize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    PFORMAT_STRING          pFormat
    );

/* Structures */

RPCRTAPI
unsigned long
RPC_ENTRY
NdrSimpleStructMemorySize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
unsigned long
RPC_ENTRY
NdrConformantStructMemorySize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
unsigned long
RPC_ENTRY
NdrConformantVaryingStructMemorySize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
unsigned long
RPC_ENTRY
NdrComplexStructMemorySize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    PFORMAT_STRING          pFormat
    );

/* Arrays */

RPCRTAPI
unsigned long
RPC_ENTRY
NdrFixedArrayMemorySize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
unsigned long
RPC_ENTRY
NdrConformantArrayMemorySize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
unsigned long
RPC_ENTRY
NdrConformantVaryingArrayMemorySize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
unsigned long
RPC_ENTRY
NdrVaryingArrayMemorySize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
unsigned long
RPC_ENTRY
NdrComplexArrayMemorySize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    PFORMAT_STRING          pFormat
    );

/* Strings */

RPCRTAPI
unsigned long
RPC_ENTRY
NdrConformantStringMemorySize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
unsigned long
RPC_ENTRY
NdrNonConformantStringMemorySize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    PFORMAT_STRING          pFormat
    );

/* Unions */

RPCRTAPI
unsigned long
RPC_ENTRY
NdrEncapsulatedUnionMemorySize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
unsigned long
RPC_ENTRY
NdrNonEncapsulatedUnionMemorySize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    PFORMAT_STRING          pFormat
    );

/* Transmit as and represent as*/

RPCRTAPI
unsigned long
RPC_ENTRY
NdrXmitOrRepAsMemorySize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    PFORMAT_STRING          pFormat
    );

/* User_marshal */

RPCRTAPI
unsigned long
RPC_ENTRY
NdrUserMarshalMemorySize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    PFORMAT_STRING          pFormat
    );

/* Interface pointer */

RPCRTAPI
unsigned long
RPC_ENTRY
NdrInterfacePointerMemorySize(
    PMIDL_STUB_MESSAGE      pStubMsg,
    PFORMAT_STRING          pFormat
    );

/*
 * Freeing routines
 */

RPCRTAPI
void
RPC_ENTRY
NdrPointerFree(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
void
RPC_ENTRY
NdrCsArrayFree(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

/* Structures */

RPCRTAPI
void
RPC_ENTRY
NdrSimpleStructFree(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
void
RPC_ENTRY
NdrConformantStructFree(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
void
RPC_ENTRY
NdrConformantVaryingStructFree(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
void
RPC_ENTRY
NdrComplexStructFree(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

/* Arrays */

RPCRTAPI
void
RPC_ENTRY
NdrFixedArrayFree(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
void
RPC_ENTRY
NdrConformantArrayFree(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
void
RPC_ENTRY
NdrConformantVaryingArrayFree(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
void
RPC_ENTRY
NdrVaryingArrayFree(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
void
RPC_ENTRY
NdrComplexArrayFree(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

/* Unions */

RPCRTAPI
void
RPC_ENTRY
NdrEncapsulatedUnionFree(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

RPCRTAPI
void
RPC_ENTRY
NdrNonEncapsulatedUnionFree(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

/* Byte count */

RPCRTAPI
void
RPC_ENTRY
NdrByteCountPointerFree(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

/* Transmit as and represent as*/

RPCRTAPI
void
RPC_ENTRY
NdrXmitOrRepAsFree(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

/* User_marshal */

RPCRTAPI
void
RPC_ENTRY
NdrUserMarshalFree(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

/* Interface pointer */

RPCRTAPI
void
RPC_ENTRY
NdrInterfacePointerFree(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pMemory,
    PFORMAT_STRING          pFormat
    );

/*
 * Endian conversion routine.
 */

RPCRTAPI
void
RPC_ENTRY
NdrConvert2(
    PMIDL_STUB_MESSAGE      pStubMsg,
    PFORMAT_STRING          pFormat,
    long                    NumberParams
    );

RPCRTAPI
void
RPC_ENTRY
NdrConvert(
    PMIDL_STUB_MESSAGE      pStubMsg,
    PFORMAT_STRING          pFormat
    );

#define USER_MARSHAL_FC_BYTE         1
#define USER_MARSHAL_FC_CHAR         2
#define USER_MARSHAL_FC_SMALL        3
#define USER_MARSHAL_FC_USMALL       4
#define USER_MARSHAL_FC_WCHAR        5
#define USER_MARSHAL_FC_SHORT        6
#define USER_MARSHAL_FC_USHORT       7
#define USER_MARSHAL_FC_LONG         8
#define USER_MARSHAL_FC_ULONG        9
#define USER_MARSHAL_FC_FLOAT       10
#define USER_MARSHAL_FC_HYPER       11
#define USER_MARSHAL_FC_DOUBLE      12

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrUserMarshalSimpleTypeConvert(
    unsigned long *         pFlags,
    unsigned char *         pBuffer,
    unsigned char           FormatChar
    );

/*
 * Auxilary routines
 */

RPCRTAPI
void
RPC_ENTRY
NdrClientInitializeNew(
    PRPC_MESSAGE            pRpcMsg,
    PMIDL_STUB_MESSAGE      pStubMsg,
    PMIDL_STUB_DESC         pStubDescriptor,
    unsigned int            ProcNum
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrServerInitializeNew(
    PRPC_MESSAGE            pRpcMsg,
    PMIDL_STUB_MESSAGE      pStubMsg,
    PMIDL_STUB_DESC         pStubDescriptor
    );

RPCRTAPI
void
RPC_ENTRY
NdrServerInitializePartial(
    PRPC_MESSAGE            pRpcMsg,
    PMIDL_STUB_MESSAGE      pStubMsg,
    PMIDL_STUB_DESC         pStubDescriptor,
    unsigned long           RequestedBufferSize
    );

RPCRTAPI
void
RPC_ENTRY
NdrClientInitialize(
    PRPC_MESSAGE            pRpcMsg,
    PMIDL_STUB_MESSAGE      pStubMsg,
    PMIDL_STUB_DESC         pStubDescriptor,
    unsigned int            ProcNum
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrServerInitialize(
    PRPC_MESSAGE            pRpcMsg,
    PMIDL_STUB_MESSAGE      pStubMsg,
    PMIDL_STUB_DESC         pStubDescriptor
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrServerInitializeUnmarshall (
    PMIDL_STUB_MESSAGE      pStubMsg,
    PMIDL_STUB_DESC         pStubDescriptor,
    PRPC_MESSAGE            pRpcMsg
    );

RPCRTAPI
void
RPC_ENTRY
NdrServerInitializeMarshall (
    PRPC_MESSAGE            pRpcMsg,
    PMIDL_STUB_MESSAGE      pStubMsg
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrGetBuffer(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned long           BufferLength,
    RPC_BINDING_HANDLE      Handle
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrNsGetBuffer(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned long           BufferLength,
    RPC_BINDING_HANDLE      Handle
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrSendReceive(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char *         pBufferEnd
    );

RPCRTAPI
unsigned char  *
RPC_ENTRY
NdrNsSendReceive(
    PMIDL_STUB_MESSAGE      pStubMsg,
    unsigned char  *        pBufferEnd,
    RPC_BINDING_HANDLE  *   pAutoHandle
    );

RPCRTAPI
void
RPC_ENTRY
NdrFreeBuffer(
    PMIDL_STUB_MESSAGE      pStubMsg
    );

RPCRTAPI
RPC_STATUS
RPC_ENTRY
NdrGetDcomProtocolVersion(
    PMIDL_STUB_MESSAGE      pStubMsg,
    RPC_VERSION *           pVersion );


/*
 * Interpreter calls.
 */

/* client */

CLIENT_CALL_RETURN RPC_VAR_ENTRY
NdrClientCall2(
    PMIDL_STUB_DESC         pStubDescriptor,
    PFORMAT_STRING          pFormat,
    ...
    );

#if _MSC_FULL_VER >= 13103270
#ifndef NDR_IMPL
#ifdef __cplusplus
extern "C"
#endif
void * _AddressOfReturnAddress(void);
#define NdrClientCall2(x, y, z) NdrClientCall2(x, y, (unsigned char *)_AddressOfReturnAddress()+4)
#endif
#endif

CLIENT_CALL_RETURN RPC_VAR_ENTRY
NdrClientCall(
    PMIDL_STUB_DESC         pStubDescriptor,
    PFORMAT_STRING          pFormat,
    ...
    );

CLIENT_CALL_RETURN RPC_VAR_ENTRY
NdrAsyncClientCall(
    PMIDL_STUB_DESC         pStubDescriptor,
    PFORMAT_STRING          pFormat,
    ...
    );

CLIENT_CALL_RETURN RPC_VAR_ENTRY
NdrDcomAsyncClientCall(
    PMIDL_STUB_DESC         pStubDescriptor,
    PFORMAT_STRING          pFormat,
    ...
    );

/* server */
typedef enum {
    STUB_UNMARSHAL,
    STUB_CALL_SERVER,
    STUB_MARSHAL,
    STUB_CALL_SERVER_NO_HRESULT
}STUB_PHASE;

typedef enum {
    PROXY_CALCSIZE,
    PROXY_GETBUFFER,
    PROXY_MARSHAL,
    PROXY_SENDRECEIVE,
    PROXY_UNMARSHAL
}PROXY_PHASE;

struct IRpcStubBuffer;      // Forward declaration

// Raw RPC only
RPCRTAPI
void
RPC_ENTRY
NdrAsyncServerCall(
    PRPC_MESSAGE                pRpcMsg
    );

// old dcom async scheme
RPCRTAPI
long
RPC_ENTRY
NdrAsyncStubCall(
    struct IRpcStubBuffer *     pThis,
    struct IRpcChannelBuffer *  pChannel,
    PRPC_MESSAGE                pRpcMsg,
    unsigned long *             pdwStubPhase
    );

// async uuid
RPCRTAPI
long
RPC_ENTRY
NdrDcomAsyncStubCall(
    struct IRpcStubBuffer    *  pThis,
    struct IRpcChannelBuffer *  pChannel,
    PRPC_MESSAGE                pRpcMsg,
    unsigned long            *  pdwStubPhase
    );

RPCRTAPI
long
RPC_ENTRY
NdrStubCall2(
    struct IRpcStubBuffer  *    pThis,
    struct IRpcChannelBuffer  * pChannel,
    PRPC_MESSAGE                pRpcMsg,
    unsigned long  *            pdwStubPhase
    );

RPCRTAPI
void
RPC_ENTRY
NdrServerCall2(
    PRPC_MESSAGE                pRpcMsg
    );

RPCRTAPI
long
RPC_ENTRY
NdrStubCall (
    struct IRpcStubBuffer  *    pThis,
    struct IRpcChannelBuffer  * pChannel,
    PRPC_MESSAGE                pRpcMsg,
    unsigned long  *            pdwStubPhase
    );

RPCRTAPI
void
RPC_ENTRY
NdrServerCall(
    PRPC_MESSAGE                pRpcMsg
    );

RPCRTAPI
int
RPC_ENTRY
NdrServerUnmarshall(
    struct IRpcChannelBuffer  * pChannel,
    PRPC_MESSAGE                pRpcMsg,
    PMIDL_STUB_MESSAGE          pStubMsg,
    PMIDL_STUB_DESC             pStubDescriptor,
    PFORMAT_STRING              pFormat,
    void  *                     pParamList
    );

RPCRTAPI
void
RPC_ENTRY
NdrServerMarshall(
    struct IRpcStubBuffer  *    pThis,
    struct IRpcChannelBuffer  * pChannel,
    PMIDL_STUB_MESSAGE          pStubMsg,
    PFORMAT_STRING              pFormat
    );

/* Comm and Fault status */

RPCRTAPI
RPC_STATUS
RPC_ENTRY
NdrMapCommAndFaultStatus(
    PMIDL_STUB_MESSAGE          pStubMsg,
    unsigned long  *            pCommStatus,
    unsigned long  *            pFaultStatus,
    RPC_STATUS                  Status
    );

/* Helper routines */

RPCRTAPI
int
RPC_ENTRY
NdrSH_UPDecision(
    PMIDL_STUB_MESSAGE          pStubMsg,
    unsigned char          * *  pPtrInMem,
    RPC_BUFPTR                  pBuffer
    );

RPCRTAPI
int
RPC_ENTRY
NdrSH_TLUPDecision(
    PMIDL_STUB_MESSAGE          pStubMsg,
    unsigned char          * *  pPtrInMem
    );

RPCRTAPI
int
RPC_ENTRY
NdrSH_TLUPDecisionBuffer(
    PMIDL_STUB_MESSAGE          pStubMsg,
    unsigned char          * *  pPtrInMem
    );

RPCRTAPI
int
RPC_ENTRY
NdrSH_IfAlloc(
    PMIDL_STUB_MESSAGE          pStubMsg,
    unsigned char          * *  pPtrInMem,
    unsigned long               Count
    );

RPCRTAPI
int
RPC_ENTRY
NdrSH_IfAllocRef(
    PMIDL_STUB_MESSAGE          pStubMsg,
    unsigned char          * *  pPtrInMem,
    unsigned long               Count
    );

RPCRTAPI
int
RPC_ENTRY
NdrSH_IfAllocSet(
    PMIDL_STUB_MESSAGE          pStubMsg,
    unsigned char          * *  pPtrInMem,
    unsigned long               Count
    );

RPCRTAPI
RPC_BUFPTR
RPC_ENTRY
NdrSH_IfCopy(
    PMIDL_STUB_MESSAGE          pStubMsg,
    unsigned char          * *  pPtrInMem,
    unsigned long               Count
    );

RPCRTAPI
RPC_BUFPTR
RPC_ENTRY
NdrSH_IfAllocCopy(
    PMIDL_STUB_MESSAGE          pStubMsg,
    unsigned char          * *  pPtrInMem,
    unsigned long               Count
    );

RPCRTAPI
unsigned long
RPC_ENTRY
NdrSH_Copy(
    unsigned char            *  pStubMsg,
    unsigned char            *  pPtrInMem,
    unsigned long               Count
    );

RPCRTAPI
void
RPC_ENTRY
NdrSH_IfFree(
    PMIDL_STUB_MESSAGE          pMessage,
    unsigned char            *  pPtr );


RPCRTAPI
RPC_BUFPTR
RPC_ENTRY
NdrSH_StringMarshall(
    PMIDL_STUB_MESSAGE          pMessage,
    unsigned char            *  pMemory,
    unsigned long               Count,
    int                         Size );

RPCRTAPI
RPC_BUFPTR
RPC_ENTRY
NdrSH_StringUnMarshall(
    PMIDL_STUB_MESSAGE          pMessage,
    unsigned char          * *  pMemory,
    int                         Size );

/****************************************************************************
    MIDL 2.0 memory package: rpc_ss_* rpc_sm_*
 ****************************************************************************/

typedef void  * RPC_SS_THREAD_HANDLE;

typedef void  * __RPC_API
RPC_CLIENT_ALLOC (
    IN size_t Size
    );

typedef void __RPC_API
RPC_CLIENT_FREE (
    IN void  * Ptr
    );

/*++
     RpcSs* package
--*/

RPCRTAPI
void  *
RPC_ENTRY
RpcSsAllocate (
    IN size_t Size
    );

RPCRTAPI
void
RPC_ENTRY
RpcSsDisableAllocate (
    void
    );

RPCRTAPI
void
RPC_ENTRY
RpcSsEnableAllocate (
    void
    );

RPCRTAPI
void
RPC_ENTRY
RpcSsFree (
    IN void  * NodeToFree
    );

RPCRTAPI
RPC_SS_THREAD_HANDLE
RPC_ENTRY
RpcSsGetThreadHandle (
    void
    );

RPCRTAPI
void
RPC_ENTRY
RpcSsSetClientAllocFree (
    IN RPC_CLIENT_ALLOC  * ClientAlloc,
    IN RPC_CLIENT_FREE   * ClientFree
    );

RPCRTAPI
void
RPC_ENTRY
RpcSsSetThreadHandle (
    IN RPC_SS_THREAD_HANDLE Id
    );

RPCRTAPI
void
RPC_ENTRY
RpcSsSwapClientAllocFree (
    IN RPC_CLIENT_ALLOC     * ClientAlloc,
    IN RPC_CLIENT_FREE      * ClientFree,
    OUT RPC_CLIENT_ALLOC *  * OldClientAlloc,
    OUT RPC_CLIENT_FREE  *  * OldClientFree
    );

/*++
     RpcSm* package
--*/

RPCRTAPI
void  *
RPC_ENTRY
RpcSmAllocate (
    IN  size_t          Size,
    OUT RPC_STATUS  *   pStatus
    );

RPCRTAPI
RPC_STATUS
RPC_ENTRY
RpcSmClientFree (
    IN  void        *   pNodeToFree
    );

RPCRTAPI
RPC_STATUS
RPC_ENTRY
RpcSmDestroyClientContext (
    IN void         * * ContextHandle
    );

RPCRTAPI
RPC_STATUS
RPC_ENTRY
RpcSmDisableAllocate (
    void
    );

RPCRTAPI
RPC_STATUS
RPC_ENTRY
RpcSmEnableAllocate (
    void
    );

RPCRTAPI
RPC_STATUS
RPC_ENTRY
RpcSmFree (
    IN void         *   NodeToFree
    );

RPCRTAPI
RPC_SS_THREAD_HANDLE
RPC_ENTRY
RpcSmGetThreadHandle (
    OUT RPC_STATUS  *   pStatus
    );

RPCRTAPI
RPC_STATUS
RPC_ENTRY
RpcSmSetClientAllocFree (
    IN RPC_CLIENT_ALLOC * ClientAlloc,
    IN RPC_CLIENT_FREE  * ClientFree
    );

RPCRTAPI
RPC_STATUS
RPC_ENTRY
RpcSmSetThreadHandle (
    IN RPC_SS_THREAD_HANDLE Id
    );

RPCRTAPI
RPC_STATUS
RPC_ENTRY
RpcSmSwapClientAllocFree (
    IN RPC_CLIENT_ALLOC     *   ClientAlloc,
    IN RPC_CLIENT_FREE      *   ClientFree,
    OUT RPC_CLIENT_ALLOC    * * OldClientAlloc,
    OUT RPC_CLIENT_FREE     * * OldClientFree
    );

/*++
     Ndr stub entry points
--*/

RPCRTAPI
void
RPC_ENTRY
NdrRpcSsEnableAllocate(
    PMIDL_STUB_MESSAGE      pMessage );

RPCRTAPI
void
RPC_ENTRY
NdrRpcSsDisableAllocate(
    PMIDL_STUB_MESSAGE      pMessage );

RPCRTAPI
void
RPC_ENTRY
NdrRpcSmSetClientToOsf(
    PMIDL_STUB_MESSAGE      pMessage );

RPCRTAPI
void  *
RPC_ENTRY
NdrRpcSmClientAllocate (
    IN size_t Size
    );

RPCRTAPI
void
RPC_ENTRY
NdrRpcSmClientFree (
    IN void  * NodeToFree
    );

RPCRTAPI
void  *
RPC_ENTRY
NdrRpcSsDefaultAllocate (
    IN size_t Size
    );

RPCRTAPI
void
RPC_ENTRY
NdrRpcSsDefaultFree (
    IN void  * NodeToFree
    );

/****************************************************************************
    end of memory package: rpc_ss_* rpc_sm_*
 ****************************************************************************/

/****************************************************************************
 * Full Pointer APIs
 ****************************************************************************/

RPCRTAPI
PFULL_PTR_XLAT_TABLES
RPC_ENTRY
NdrFullPointerXlatInit(
    unsigned long           NumberOfPointers,
    XLAT_SIDE               XlatSide
    );

RPCRTAPI
void
RPC_ENTRY
NdrFullPointerXlatFree(
    PFULL_PTR_XLAT_TABLES   pXlatTables
    );

RPCRTAPI
int
RPC_ENTRY
NdrFullPointerQueryPointer(
    PFULL_PTR_XLAT_TABLES   pXlatTables,
    void  *                 pPointer,
    unsigned char           QueryType,
    unsigned long  *        pRefId
    );

RPCRTAPI
int
RPC_ENTRY
NdrFullPointerQueryRefId(
    PFULL_PTR_XLAT_TABLES   pXlatTables,
    unsigned long           RefId,
    unsigned char           QueryType,
    void  * *               ppPointer
    );

RPCRTAPI
void
RPC_ENTRY
NdrFullPointerInsertRefId(
    PFULL_PTR_XLAT_TABLES   pXlatTables,
    unsigned long           RefId,
    void  *                 pPointer
    );

RPCRTAPI
int
RPC_ENTRY
NdrFullPointerFree(
    PFULL_PTR_XLAT_TABLES   pXlatTables,
    void  *                 Pointer
    );

RPCRTAPI
void  *
RPC_ENTRY
NdrAllocate(
    PMIDL_STUB_MESSAGE      pStubMsg,
    size_t                  Len
    );

RPCRTAPI
void
RPC_ENTRY
NdrClearOutParameters(
    PMIDL_STUB_MESSAGE      pStubMsg,
    PFORMAT_STRING          pFormat,
    void  *                 ArgAddr
    );


/****************************************************************************
 * Proxy APIs
 ****************************************************************************/

RPCRTAPI
void  *
RPC_ENTRY
NdrOleAllocate (
    IN size_t Size
    );

RPCRTAPI
void
RPC_ENTRY
NdrOleFree (
    IN void  * NodeToFree
    );

#ifdef CONST_VTABLE
#define CONST_VTBL const
#else
#define CONST_VTBL
#endif

/****************************************************************************
 * VC COM support 
 ****************************************************************************/

#ifndef DECLSPEC_SELECTANY
#if (_MSC_VER >= 1100)
#define DECLSPEC_SELECTANY __declspec(selectany)
#else
#define DECLSPEC_SELECTANY
#endif
#endif

#ifndef DECLSPEC_NOVTABLE
#if (_MSC_VER >= 1100) && defined(__cplusplus)
#define DECLSPEC_NOVTABLE __declspec(novtable)
#else
#define DECLSPEC_NOVTABLE
#endif
#endif

#ifndef DECLSPEC_UUID
#if (_MSC_VER >= 1100) && defined(__cplusplus)
#define DECLSPEC_UUID(x) __declspec(uuid(x))
#else
#define DECLSPEC_UUID(x)
#endif
#endif

#define MIDL_INTERFACE(x)   struct DECLSPEC_UUID(x) DECLSPEC_NOVTABLE

#if _MSC_VER >= 1100
#define EXTERN_GUID(itf,l1,s1,s2,c1,c2,c3,c4,c5,c6,c7,c8)  \
  EXTERN_C const IID DECLSPEC_SELECTANY itf = {l1,s1,s2,{c1,c2,c3,c4,c5,c6,c7,c8}}
#else
#define EXTERN_GUID(itf,l1,s1,s2,c1,c2,c3,c4,c5,c6,c7,c8) EXTERN_C const IID itf
#endif

/****************************************************************************
 * UserMarshal information
 ****************************************************************************/

typedef struct _NDR_USER_MARSHAL_INFO_LEVEL1
{
    void *                      Buffer;
    unsigned long               BufferSize;
    void *(__RPC_API * pfnAllocate)(size_t);
    void (__RPC_API * pfnFree)(void *);
    struct IRpcChannelBuffer *  pRpcChannelBuffer;
    ULONG_PTR                   Reserved[5];
} NDR_USER_MARSHAL_INFO_LEVEL1;

#if !defined( RC_INVOKED )
#if _MSC_VER >= 1200
#pragma warning(push)
#endif
#pragma warning(disable:4201)
#endif

typedef struct _NDR_USER_MARSHAL_INFO
{
    unsigned long InformationLevel;
    union {
        NDR_USER_MARSHAL_INFO_LEVEL1 Level1;
    };
} NDR_USER_MARSHAL_INFO;

#if !defined( RC_INVOKED )
#if _MSC_VER >= 1200
#pragma warning(pop)
#else
#pragma warning(default:4201)
#endif
#endif

RPC_STATUS
RPC_ENTRY
NdrGetUserMarshalInfo (
    IN unsigned long        *   pFlags,
    IN unsigned long            InformationLevel,
    OUT NDR_USER_MARSHAL_INFO * pMarshalInfo
    );

/****************************************************************************
 * 64bit APIs
 ****************************************************************************/
RPC_STATUS RPC_ENTRY
NdrCreateServerInterfaceFromStub(
            IN struct IRpcStubBuffer* pStub,
            IN OUT RPC_SERVER_INTERFACE *pServerIf );

/*
 * Interpreter calls
 */
CLIENT_CALL_RETURN RPC_VAR_ENTRY
NdrClientCall3(
    MIDL_STUBLESS_PROXY_INFO   *pProxyInfo,
    unsigned long               nProcNum,
    void *                      pReturnValue,
    ...
    );

CLIENT_CALL_RETURN RPC_VAR_ENTRY
Ndr64AsyncClientCall(
    MIDL_STUBLESS_PROXY_INFO   *pProxyInfo,
    unsigned long               nProcNum,
    void *                      pReturnValue,
    ...
    );

CLIENT_CALL_RETURN RPC_VAR_ENTRY
Ndr64DcomAsyncClientCall(
    MIDL_STUBLESS_PROXY_INFO   *pProxyInfo,
    unsigned long               nProcNum,
    void *                      pReturnValue,
    ...
    );

struct IRpcStubBuffer;      // Forward declaration

RPCRTAPI
void
RPC_ENTRY
Ndr64AsyncServerCall(
    PRPC_MESSAGE                pRpcMsg
    );

RPCRTAPI
void
RPC_ENTRY
Ndr64AsyncServerCall64(
    PRPC_MESSAGE                pRpcMsg
    );

RPCRTAPI
void
RPC_ENTRY
Ndr64AsyncServerCallAll(
    PRPC_MESSAGE                pRpcMsg
    );

RPCRTAPI
long
RPC_ENTRY
Ndr64AsyncStubCall(
    struct IRpcStubBuffer *     pThis,
    struct IRpcChannelBuffer *  pChannel,
    PRPC_MESSAGE                pRpcMsg,
    unsigned long *             pdwStubPhase
    );

/* async uuid */
RPCRTAPI
long
RPC_ENTRY
Ndr64DcomAsyncStubCall(
    struct IRpcStubBuffer    *  pThis,
    struct IRpcChannelBuffer *  pChannel,
    PRPC_MESSAGE                pRpcMsg,
    unsigned long            *  pdwStubPhase
    );

RPCRTAPI
long
RPC_ENTRY
NdrStubCall3 (
    struct IRpcStubBuffer  *    pThis,
    struct IRpcChannelBuffer  * pChannel,
    PRPC_MESSAGE                pRpcMsg,
    unsigned long  *            pdwStubPhase
    );

RPCRTAPI
void
RPC_ENTRY
NdrServerCallAll(
    PRPC_MESSAGE                pRpcMsg
    );

RPCRTAPI
void
RPC_ENTRY
NdrServerCallNdr64(
    PRPC_MESSAGE                pRpcMsg
    );


RPCRTAPI
void
RPC_ENTRY
NdrServerCall3(
    PRPC_MESSAGE                pRpcMsg
    );


/* [partial_ignore] functions*/
RPCRTAPI
void
RPC_ENTRY
NdrPartialIgnoreClientMarshall(
    PMIDL_STUB_MESSAGE          pStubMsg,
    void *                      pMemory
    );

RPCRTAPI
void
RPC_ENTRY
NdrPartialIgnoreServerUnmarshall(
    PMIDL_STUB_MESSAGE          pStubMsg,
    void **                     ppMemory
    );

RPCRTAPI
void
RPC_ENTRY
NdrPartialIgnoreClientBufferSize(
    PMIDL_STUB_MESSAGE          pStubMsg,
    void *                      pMemory
    );

RPCRTAPI
void
RPC_ENTRY
NdrPartialIgnoreServerInitialize(
    PMIDL_STUB_MESSAGE          pStubMsg,
    void **                     ppMemory,
    PFORMAT_STRING              pFormat
    );


void RPC_ENTRY
RpcUserFree( handle_t AsyncHandle, void * pBuffer );

#ifdef __cplusplus
}
#endif

#include <poppack.h>

#endif /* __RPCNDR_H__ */

