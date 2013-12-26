

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Dec 26 17:45:49 2013
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


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_ISimpleObject,0x47E764FE,0xD065,0x4BDE,0x89,0x43,0x30,0xF4,0x66,0x64,0xB0,0x2C);


MIDL_DEFINE_GUID(IID, LIBID_ATLExeCOMServerLib,0xC7902493,0xE23D,0x4487,0x82,0x4F,0x4A,0xB9,0x7B,0xD8,0xB8,0x6D);


MIDL_DEFINE_GUID(IID, DIID__ISimpleObjectEvents,0x6EE998B7,0x61C8,0x4D54,0xB2,0x7A,0xF6,0x23,0xE8,0xC1,0xEA,0x64);


MIDL_DEFINE_GUID(CLSID, CLSID_SimpleObject,0x9465BE08,0xC9FA,0x4DDF,0xA5,0x6E,0x57,0xB9,0x2B,0xCD,0xEE,0xB0);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



