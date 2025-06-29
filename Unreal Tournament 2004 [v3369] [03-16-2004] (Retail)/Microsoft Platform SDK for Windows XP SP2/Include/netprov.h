
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0347 */
/* Compiler settings for netprov.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __netprov_h__
#define __netprov_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IProvisioningDomain_FWD_DEFINED__
#define __IProvisioningDomain_FWD_DEFINED__
typedef interface IProvisioningDomain IProvisioningDomain;
#endif 	/* __IProvisioningDomain_FWD_DEFINED__ */


#ifndef __IProvisioningProfileWireless_FWD_DEFINED__
#define __IProvisioningProfileWireless_FWD_DEFINED__
typedef interface IProvisioningProfileWireless IProvisioningProfileWireless;
#endif 	/* __IProvisioningProfileWireless_FWD_DEFINED__ */


#ifndef __IFlashConfig_FWD_DEFINED__
#define __IFlashConfig_FWD_DEFINED__
typedef interface IFlashConfig IFlashConfig;
#endif 	/* __IFlashConfig_FWD_DEFINED__ */


#ifndef __NetProvisioning_FWD_DEFINED__
#define __NetProvisioning_FWD_DEFINED__

#ifdef __cplusplus
typedef class NetProvisioning NetProvisioning;
#else
typedef struct NetProvisioning NetProvisioning;
#endif /* __cplusplus */

#endif 	/* __NetProvisioning_FWD_DEFINED__ */


#ifndef __FlashConfig_FWD_DEFINED__
#define __FlashConfig_FWD_DEFINED__

#ifdef __cplusplus
typedef class FlashConfig FlashConfig;
#else
typedef struct FlashConfig FlashConfig;
#endif /* __cplusplus */

#endif 	/* __FlashConfig_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "prsht.h"
#include "msxml.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

/* interface __MIDL_itf_netprov_0000 */
/* [local] */ 

//+-------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (c) Microsoft Corporation. All rights reserved.
//
//--------------------------------------------------------------------------
#if ( _MSC_VER >= 800 )
#pragma warning(disable:4201)
#endif




extern RPC_IF_HANDLE __MIDL_itf_netprov_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_netprov_0000_v0_0_s_ifspec;

#ifndef __IProvisioningDomain_INTERFACE_DEFINED__
#define __IProvisioningDomain_INTERFACE_DEFINED__

/* interface IProvisioningDomain */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IProvisioningDomain;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("c96fbd50-24dd-11d8-89fb-00904b2ea9c6")
    IProvisioningDomain : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Add( 
            /* [string][in] */ LPCWSTR pszwPathToFolder) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Query( 
            /* [string][in] */ LPCWSTR pszwDomain,
            /* [string][in] */ LPCWSTR pszwLanguage,
            /* [string][in] */ LPCWSTR pszwXPathQuery,
            /* [out] */ IXMLDOMNodeList **Nodes) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IProvisioningDomainVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IProvisioningDomain * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IProvisioningDomain * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IProvisioningDomain * This);
        
        HRESULT ( STDMETHODCALLTYPE *Add )( 
            IProvisioningDomain * This,
            /* [string][in] */ LPCWSTR pszwPathToFolder);
        
        HRESULT ( STDMETHODCALLTYPE *Query )( 
            IProvisioningDomain * This,
            /* [string][in] */ LPCWSTR pszwDomain,
            /* [string][in] */ LPCWSTR pszwLanguage,
            /* [string][in] */ LPCWSTR pszwXPathQuery,
            /* [out] */ IXMLDOMNodeList **Nodes);
        
        END_INTERFACE
    } IProvisioningDomainVtbl;

    interface IProvisioningDomain
    {
        CONST_VTBL struct IProvisioningDomainVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IProvisioningDomain_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IProvisioningDomain_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IProvisioningDomain_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IProvisioningDomain_Add(This,pszwPathToFolder)	\
    (This)->lpVtbl -> Add(This,pszwPathToFolder)

#define IProvisioningDomain_Query(This,pszwDomain,pszwLanguage,pszwXPathQuery,Nodes)	\
    (This)->lpVtbl -> Query(This,pszwDomain,pszwLanguage,pszwXPathQuery,Nodes)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IProvisioningDomain_Add_Proxy( 
    IProvisioningDomain * This,
    /* [string][in] */ LPCWSTR pszwPathToFolder);


void __RPC_STUB IProvisioningDomain_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IProvisioningDomain_Query_Proxy( 
    IProvisioningDomain * This,
    /* [string][in] */ LPCWSTR pszwDomain,
    /* [string][in] */ LPCWSTR pszwLanguage,
    /* [string][in] */ LPCWSTR pszwXPathQuery,
    /* [out] */ IXMLDOMNodeList **Nodes);


void __RPC_STUB IProvisioningDomain_Query_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IProvisioningDomain_INTERFACE_DEFINED__ */


#ifndef __IProvisioningProfileWireless_INTERFACE_DEFINED__
#define __IProvisioningProfileWireless_INTERFACE_DEFINED__

/* interface IProvisioningProfileWireless */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IProvisioningProfileWireless;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("c96fbd51-24dd-11d8-89fb-00904b2ea9c6")
    IProvisioningProfileWireless : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CreateProfile( 
            /* [in] */ BSTR bstrXMLWirelessConfigProfile,
            /* [in] */ BSTR bstrXMLConnectionConfigProfile,
            /* [in] */ GUID *pAdapterInstanceGuid,
            /* [out] */ ULONG *pulStatus) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IProvisioningProfileWirelessVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IProvisioningProfileWireless * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IProvisioningProfileWireless * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IProvisioningProfileWireless * This);
        
        HRESULT ( STDMETHODCALLTYPE *CreateProfile )( 
            IProvisioningProfileWireless * This,
            /* [in] */ BSTR bstrXMLWirelessConfigProfile,
            /* [in] */ BSTR bstrXMLConnectionConfigProfile,
            /* [in] */ GUID *pAdapterInstanceGuid,
            /* [out] */ ULONG *pulStatus);
        
        END_INTERFACE
    } IProvisioningProfileWirelessVtbl;

    interface IProvisioningProfileWireless
    {
        CONST_VTBL struct IProvisioningProfileWirelessVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IProvisioningProfileWireless_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IProvisioningProfileWireless_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IProvisioningProfileWireless_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IProvisioningProfileWireless_CreateProfile(This,bstrXMLWirelessConfigProfile,bstrXMLConnectionConfigProfile,pAdapterInstanceGuid,pulStatus)	\
    (This)->lpVtbl -> CreateProfile(This,bstrXMLWirelessConfigProfile,bstrXMLConnectionConfigProfile,pAdapterInstanceGuid,pulStatus)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IProvisioningProfileWireless_CreateProfile_Proxy( 
    IProvisioningProfileWireless * This,
    /* [in] */ BSTR bstrXMLWirelessConfigProfile,
    /* [in] */ BSTR bstrXMLConnectionConfigProfile,
    /* [in] */ GUID *pAdapterInstanceGuid,
    /* [out] */ ULONG *pulStatus);


void __RPC_STUB IProvisioningProfileWireless_CreateProfile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IProvisioningProfileWireless_INTERFACE_DEFINED__ */


#ifndef __IFlashConfig_INTERFACE_DEFINED__
#define __IFlashConfig_INTERFACE_DEFINED__

/* interface IFlashConfig */
/* [object][version][uuid] */ 

typedef 
enum tagFLASHCONFIG_FLAGS
    {	FCF_INFRASTRUCTURE	= 0,
	FCF_ADHOC	= 1
    } 	FLASHCONFIG_FLAGS;


EXTERN_C const IID IID_IFlashConfig;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8244dedf-cf04-41fa-812f-e151f492c5aa")
    IFlashConfig : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE RunWizard( 
            /* [in] */ HWND hwndParent,
            /* [in] */ FLASHCONFIG_FLAGS eFlags) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IFlashConfigVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IFlashConfig * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IFlashConfig * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IFlashConfig * This);
        
        HRESULT ( STDMETHODCALLTYPE *RunWizard )( 
            IFlashConfig * This,
            /* [in] */ HWND hwndParent,
            /* [in] */ FLASHCONFIG_FLAGS eFlags);
        
        END_INTERFACE
    } IFlashConfigVtbl;

    interface IFlashConfig
    {
        CONST_VTBL struct IFlashConfigVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFlashConfig_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFlashConfig_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFlashConfig_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IFlashConfig_RunWizard(This,hwndParent,eFlags)	\
    (This)->lpVtbl -> RunWizard(This,hwndParent,eFlags)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IFlashConfig_RunWizard_Proxy( 
    IFlashConfig * This,
    /* [in] */ HWND hwndParent,
    /* [in] */ FLASHCONFIG_FLAGS eFlags);


void __RPC_STUB IFlashConfig_RunWizard_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IFlashConfig_INTERFACE_DEFINED__ */



#ifndef __NETPROVLib_LIBRARY_DEFINED__
#define __NETPROVLib_LIBRARY_DEFINED__

/* library NETPROVLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_NETPROVLib;

EXTERN_C const CLSID CLSID_NetProvisioning;

#ifdef __cplusplus

class DECLSPEC_UUID("2aa2b5fe-b846-4d07-810c-b21ee45320e3")
NetProvisioning;
#endif

EXTERN_C const CLSID CLSID_FlashConfig;

#ifdef __cplusplus

class DECLSPEC_UUID("9f63805a-42a7-4472-babc-642466c91d59")
FlashConfig;
#endif
#endif /* __NETPROVLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  HWND_UserSize(     unsigned long *, unsigned long            , HWND * ); 
unsigned char * __RPC_USER  HWND_UserMarshal(  unsigned long *, unsigned char *, HWND * ); 
unsigned char * __RPC_USER  HWND_UserUnmarshal(unsigned long *, unsigned char *, HWND * ); 
void                      __RPC_USER  HWND_UserFree(     unsigned long *, HWND * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


