

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Sat Jan 12 17:43:19 2013
 */
/* Compiler settings for ATLExeCOMServer.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


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

#ifndef __ATLExeCOMServer_i_h__
#define __ATLExeCOMServer_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ISimpleObject_FWD_DEFINED__
#define __ISimpleObject_FWD_DEFINED__
typedef interface ISimpleObject ISimpleObject;
#endif 	/* __ISimpleObject_FWD_DEFINED__ */


#ifndef ___ISimpleObjectEvents_FWD_DEFINED__
#define ___ISimpleObjectEvents_FWD_DEFINED__
typedef interface _ISimpleObjectEvents _ISimpleObjectEvents;
#endif 	/* ___ISimpleObjectEvents_FWD_DEFINED__ */


#ifndef __SimpleObject_FWD_DEFINED__
#define __SimpleObject_FWD_DEFINED__

#ifdef __cplusplus
typedef class SimpleObject SimpleObject;
#else
typedef struct SimpleObject SimpleObject;
#endif /* __cplusplus */

#endif 	/* __SimpleObject_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ISimpleObject_INTERFACE_DEFINED__
#define __ISimpleObject_INTERFACE_DEFINED__

/* interface ISimpleObject */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ISimpleObject;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("47E764FE-D065-4BDE-8943-30F46664B02C")
    ISimpleObject : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FloatProperty( 
            /* [retval][out] */ FLOAT *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_FloatProperty( 
            /* [in] */ FLOAT newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE HelloWorld( 
            /* [retval][out] */ BSTR *pRet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetProcessThreadID( 
            /* [out] */ LONG *pdwProcessId,
            /* [out] */ LONG *pdwThreadId) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISimpleObjectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISimpleObject * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISimpleObject * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISimpleObject * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISimpleObject * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISimpleObject * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISimpleObject * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISimpleObject * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FloatProperty )( 
            ISimpleObject * This,
            /* [retval][out] */ FLOAT *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_FloatProperty )( 
            ISimpleObject * This,
            /* [in] */ FLOAT newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *HelloWorld )( 
            ISimpleObject * This,
            /* [retval][out] */ BSTR *pRet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetProcessThreadID )( 
            ISimpleObject * This,
            /* [out] */ LONG *pdwProcessId,
            /* [out] */ LONG *pdwThreadId);
        
        END_INTERFACE
    } ISimpleObjectVtbl;

    interface ISimpleObject
    {
        CONST_VTBL struct ISimpleObjectVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISimpleObject_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISimpleObject_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISimpleObject_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISimpleObject_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISimpleObject_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISimpleObject_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISimpleObject_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISimpleObject_get_FloatProperty(This,pVal)	\
    ( (This)->lpVtbl -> get_FloatProperty(This,pVal) ) 

#define ISimpleObject_put_FloatProperty(This,newVal)	\
    ( (This)->lpVtbl -> put_FloatProperty(This,newVal) ) 

#define ISimpleObject_HelloWorld(This,pRet)	\
    ( (This)->lpVtbl -> HelloWorld(This,pRet) ) 

#define ISimpleObject_GetProcessThreadID(This,pdwProcessId,pdwThreadId)	\
    ( (This)->lpVtbl -> GetProcessThreadID(This,pdwProcessId,pdwThreadId) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISimpleObject_INTERFACE_DEFINED__ */



#ifndef __ATLExeCOMServerLib_LIBRARY_DEFINED__
#define __ATLExeCOMServerLib_LIBRARY_DEFINED__

/* library ATLExeCOMServerLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_ATLExeCOMServerLib;

#ifndef ___ISimpleObjectEvents_DISPINTERFACE_DEFINED__
#define ___ISimpleObjectEvents_DISPINTERFACE_DEFINED__

/* dispinterface _ISimpleObjectEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__ISimpleObjectEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("6EE998B7-61C8-4D54-B27A-F623E8C1EA64")
    _ISimpleObjectEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _ISimpleObjectEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _ISimpleObjectEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _ISimpleObjectEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _ISimpleObjectEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _ISimpleObjectEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _ISimpleObjectEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _ISimpleObjectEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _ISimpleObjectEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _ISimpleObjectEventsVtbl;

    interface _ISimpleObjectEvents
    {
        CONST_VTBL struct _ISimpleObjectEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ISimpleObjectEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _ISimpleObjectEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _ISimpleObjectEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _ISimpleObjectEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _ISimpleObjectEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _ISimpleObjectEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _ISimpleObjectEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___ISimpleObjectEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_SimpleObject;

#ifdef __cplusplus

class DECLSPEC_UUID("9465BE08-C9FA-4DDF-A56E-57B92BCDEEB0")
SimpleObject;
#endif
#endif /* __ATLExeCOMServerLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


