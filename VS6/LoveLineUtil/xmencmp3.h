
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 5.03.0280 */
/* at Fri Nov 09 16:34:26 2001
 */
/* Compiler settings for xmencmp3.idl:
    Os (OptLev=s), W1, Zp8, env=Win32 (32b run), ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
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

#ifndef __xmencmp3_h__
#define __xmencmp3_h__

/* Forward Declarations */ 

#ifndef __EncodeMP3Mem2_FWD_DEFINED__
#define __EncodeMP3Mem2_FWD_DEFINED__

#ifdef __cplusplus
typedef class EncodeMP3Mem2 EncodeMP3Mem2;
#else
typedef struct EncodeMP3Mem2 EncodeMP3Mem2;
#endif /* __cplusplus */

#endif 	/* __EncodeMP3Mem2_FWD_DEFINED__ */


#ifndef __IEncodeMP3Mem2_FWD_DEFINED__
#define __IEncodeMP3Mem2_FWD_DEFINED__
typedef interface IEncodeMP3Mem2 IEncodeMP3Mem2;
#endif 	/* __IEncodeMP3Mem2_FWD_DEFINED__ */


#ifndef __QueryWavFile2_FWD_DEFINED__
#define __QueryWavFile2_FWD_DEFINED__

#ifdef __cplusplus
typedef class QueryWavFile2 QueryWavFile2;
#else
typedef struct QueryWavFile2 QueryWavFile2;
#endif /* __cplusplus */

#endif 	/* __QueryWavFile2_FWD_DEFINED__ */


#ifndef __IQueryWavFile2_FWD_DEFINED__
#define __IQueryWavFile2_FWD_DEFINED__
typedef interface IQueryWavFile2 IQueryWavFile2;
#endif 	/* __IQueryWavFile2_FWD_DEFINED__ */


#ifndef __QueryMPEGAudioFile2_FWD_DEFINED__
#define __QueryMPEGAudioFile2_FWD_DEFINED__

#ifdef __cplusplus
typedef class QueryMPEGAudioFile2 QueryMPEGAudioFile2;
#else
typedef struct QueryMPEGAudioFile2 QueryMPEGAudioFile2;
#endif /* __cplusplus */

#endif 	/* __QueryMPEGAudioFile2_FWD_DEFINED__ */


#ifndef __IQueryMPEGAudioFile2_FWD_DEFINED__
#define __IQueryMPEGAudioFile2_FWD_DEFINED__
typedef interface IQueryMPEGAudioFile2 IQueryMPEGAudioFile2;
#endif 	/* __IQueryMPEGAudioFile2_FWD_DEFINED__ */


#ifndef __MP3Encoder3_FWD_DEFINED__
#define __MP3Encoder3_FWD_DEFINED__

#ifdef __cplusplus
typedef class MP3Encoder3 MP3Encoder3;
#else
typedef struct MP3Encoder3 MP3Encoder3;
#endif /* __cplusplus */

#endif 	/* __MP3Encoder3_FWD_DEFINED__ */


#ifndef __IMP3Encoder3_FWD_DEFINED__
#define __IMP3Encoder3_FWD_DEFINED__
typedef interface IMP3Encoder3 IMP3Encoder3;
#endif 	/* __IMP3Encoder3_FWD_DEFINED__ */


#ifndef __EncodeMP3File2_FWD_DEFINED__
#define __EncodeMP3File2_FWD_DEFINED__

#ifdef __cplusplus
typedef class EncodeMP3File2 EncodeMP3File2;
#else
typedef struct EncodeMP3File2 EncodeMP3File2;
#endif /* __cplusplus */

#endif 	/* __EncodeMP3File2_FWD_DEFINED__ */


#ifndef __IEncodeMP3File2_FWD_DEFINED__
#define __IEncodeMP3File2_FWD_DEFINED__
typedef interface IEncodeMP3File2 IEncodeMP3File2;
#endif 	/* __IEncodeMP3File2_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_xmencmp3_0000 */
/* [local] */ 








extern RPC_IF_HANDLE __MIDL_itf_xmencmp3_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_xmencmp3_0000_v0_0_s_ifspec;

#ifndef __IEncodeMP3Mem2_INTERFACE_DEFINED__
#define __IEncodeMP3Mem2_INTERFACE_DEFINED__

/* interface IEncodeMP3Mem2 */
/* [object][helpstring][uuid] */ 


EXTERN_C const IID IID_IEncodeMP3Mem2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("477A3782-2D4D-11D3-B244-444553540000")
    IEncodeMP3Mem2 : public IDispatch
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Algorithm( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_BitRate( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_BitRate( 
            /* [in] */ long rhs) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_SamplesPerSecond( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_SamplesPerSecond( 
            /* [in] */ long rhs) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_ChannelMode( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_ChannelMode( 
            /* [in] */ long rhs) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_StereoBands( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_StereoBands( 
            /* [in] */ long rhs) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_VariableBitRateScaleFactor( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_VariableBitRateScaleFactor( 
            /* [in] */ long rhs) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_DownMixToMono( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_DownMixToMono( 
            /* [in] */ long rhs) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_VariableBitRateHeader( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_VariableBitRateHeader( 
            /* [in] */ long rhs) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_VariableBitRateTOC( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_VariableBitRateTOC( 
            /* [in] */ long rhs) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_HighFrequencyMode( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_HighFrequencyMode( 
            /* [in] */ long rhs) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_CopyrightBit( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_CopyrightBit( 
            /* [in] */ long rhs) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_WarningLowDiskSpace( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_WarningChangedProperties( void) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_OriginalBit( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_OriginalBit( 
            /* [in] */ long rhs) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_ChooseBestSamplesPerSecond( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_ChooseBestSamplesPerSecond( 
            /* [in] */ long rhs) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PutStandardProperties( 
            /* [in] */ long lStandardMPEGType) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE InitSourceAudioProperties( 
            /* [in] */ long lSrcDataType,
            /* [in] */ long lSrcSamplesPerSec,
            /* [in] */ long lSrcChannels,
            /* [in] */ long lSrcBitDepth) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EncodeBegin( 
            /* [in] */ BSTR strMPEGAudFile,
            /* [in] */ long bMemoryOutput,
            /* [in] */ long lSizeOutputToMemBuf,
            /* [in] */ long __RPC_FAR *pKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE InputAudio( 
            /* [out][in] */ unsigned long __RPC_FAR *plCurDataLen,
            /* [in] */ long __RPC_FAR *pWAVAudBuf) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EncodeAudio( 
            /* [in] */ long bLastTime,
            /* [out][in] */ long __RPC_FAR *plCurDataLenForMem,
            /* [out] */ long __RPC_FAR *pOutputBufForMem) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EncodeEnd( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE InitAuthentication( 
            unsigned char __RPC_FAR *pKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE UnlockSDK( 
            unsigned char __RPC_FAR *pKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE VerifyEncode( 
            /* [in] */ BSTR strSrcWAVFile,
            /* [in] */ BSTR strMPEGAudioFile) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_WarningUnsupportedSource( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_MP3Frames( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_VBRHeaderBytes( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE VBRHeaderUpdate( 
            /* [in] */ long lFrames,
            /* [in] */ long lCumOutBytes,
            /* [in] */ long lVBRLevel,
            /* [out][in] */ unsigned char __RPC_FAR *bs_buffer) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE VBRSetCustomBuffer( 
            /* [in] */ unsigned long ulFlags,
            /* [in] */ unsigned char __RPC_FAR *pCustBuf) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEncodeMP3Mem2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IEncodeMP3Mem2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IEncodeMP3Mem2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Algorithm )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_BitRate )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_BitRate )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [in] */ long rhs);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SamplesPerSecond )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SamplesPerSecond )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [in] */ long rhs);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ChannelMode )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ChannelMode )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [in] */ long rhs);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_StereoBands )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_StereoBands )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [in] */ long rhs);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_VariableBitRateScaleFactor )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_VariableBitRateScaleFactor )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [in] */ long rhs);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DownMixToMono )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_DownMixToMono )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [in] */ long rhs);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_VariableBitRateHeader )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_VariableBitRateHeader )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [in] */ long rhs);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_VariableBitRateTOC )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_VariableBitRateTOC )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [in] */ long rhs);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_HighFrequencyMode )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_HighFrequencyMode )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [in] */ long rhs);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CopyrightBit )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_CopyrightBit )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [in] */ long rhs);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WarningLowDiskSpace )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WarningChangedProperties )( 
            IEncodeMP3Mem2 __RPC_FAR * This);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_OriginalBit )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_OriginalBit )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [in] */ long rhs);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ChooseBestSamplesPerSecond )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ChooseBestSamplesPerSecond )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [in] */ long rhs);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PutStandardProperties )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [in] */ long lStandardMPEGType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InitSourceAudioProperties )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [in] */ long lSrcDataType,
            /* [in] */ long lSrcSamplesPerSec,
            /* [in] */ long lSrcChannels,
            /* [in] */ long lSrcBitDepth);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EncodeBegin )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [in] */ BSTR strMPEGAudFile,
            /* [in] */ long bMemoryOutput,
            /* [in] */ long lSizeOutputToMemBuf,
            /* [in] */ long __RPC_FAR *pKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InputAudio )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [out][in] */ unsigned long __RPC_FAR *plCurDataLen,
            /* [in] */ long __RPC_FAR *pWAVAudBuf);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EncodeAudio )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [in] */ long bLastTime,
            /* [out][in] */ long __RPC_FAR *plCurDataLenForMem,
            /* [out] */ long __RPC_FAR *pOutputBufForMem);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EncodeEnd )( 
            IEncodeMP3Mem2 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InitAuthentication )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            unsigned char __RPC_FAR *pKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnlockSDK )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            unsigned char __RPC_FAR *pKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *VerifyEncode )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [in] */ BSTR strSrcWAVFile,
            /* [in] */ BSTR strMPEGAudioFile);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WarningUnsupportedSource )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MP3Frames )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_VBRHeaderBytes )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *VBRHeaderUpdate )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [in] */ long lFrames,
            /* [in] */ long lCumOutBytes,
            /* [in] */ long lVBRLevel,
            /* [out][in] */ unsigned char __RPC_FAR *bs_buffer);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *VBRSetCustomBuffer )( 
            IEncodeMP3Mem2 __RPC_FAR * This,
            /* [in] */ unsigned long ulFlags,
            /* [in] */ unsigned char __RPC_FAR *pCustBuf);
        
        END_INTERFACE
    } IEncodeMP3Mem2Vtbl;

    interface IEncodeMP3Mem2
    {
        CONST_VTBL struct IEncodeMP3Mem2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEncodeMP3Mem2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEncodeMP3Mem2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEncodeMP3Mem2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEncodeMP3Mem2_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IEncodeMP3Mem2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IEncodeMP3Mem2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IEncodeMP3Mem2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IEncodeMP3Mem2_get_Algorithm(This,l)	\
    (This)->lpVtbl -> get_Algorithm(This,l)

#define IEncodeMP3Mem2_get_BitRate(This,l)	\
    (This)->lpVtbl -> get_BitRate(This,l)

#define IEncodeMP3Mem2_put_BitRate(This,rhs)	\
    (This)->lpVtbl -> put_BitRate(This,rhs)

#define IEncodeMP3Mem2_get_SamplesPerSecond(This,l)	\
    (This)->lpVtbl -> get_SamplesPerSecond(This,l)

#define IEncodeMP3Mem2_put_SamplesPerSecond(This,rhs)	\
    (This)->lpVtbl -> put_SamplesPerSecond(This,rhs)

#define IEncodeMP3Mem2_get_ChannelMode(This,l)	\
    (This)->lpVtbl -> get_ChannelMode(This,l)

#define IEncodeMP3Mem2_put_ChannelMode(This,rhs)	\
    (This)->lpVtbl -> put_ChannelMode(This,rhs)

#define IEncodeMP3Mem2_get_StereoBands(This,l)	\
    (This)->lpVtbl -> get_StereoBands(This,l)

#define IEncodeMP3Mem2_put_StereoBands(This,rhs)	\
    (This)->lpVtbl -> put_StereoBands(This,rhs)

#define IEncodeMP3Mem2_get_VariableBitRateScaleFactor(This,l)	\
    (This)->lpVtbl -> get_VariableBitRateScaleFactor(This,l)

#define IEncodeMP3Mem2_put_VariableBitRateScaleFactor(This,rhs)	\
    (This)->lpVtbl -> put_VariableBitRateScaleFactor(This,rhs)

#define IEncodeMP3Mem2_get_DownMixToMono(This,l)	\
    (This)->lpVtbl -> get_DownMixToMono(This,l)

#define IEncodeMP3Mem2_put_DownMixToMono(This,rhs)	\
    (This)->lpVtbl -> put_DownMixToMono(This,rhs)

#define IEncodeMP3Mem2_get_VariableBitRateHeader(This,l)	\
    (This)->lpVtbl -> get_VariableBitRateHeader(This,l)

#define IEncodeMP3Mem2_put_VariableBitRateHeader(This,rhs)	\
    (This)->lpVtbl -> put_VariableBitRateHeader(This,rhs)

#define IEncodeMP3Mem2_get_VariableBitRateTOC(This,l)	\
    (This)->lpVtbl -> get_VariableBitRateTOC(This,l)

#define IEncodeMP3Mem2_put_VariableBitRateTOC(This,rhs)	\
    (This)->lpVtbl -> put_VariableBitRateTOC(This,rhs)

#define IEncodeMP3Mem2_get_HighFrequencyMode(This,l)	\
    (This)->lpVtbl -> get_HighFrequencyMode(This,l)

#define IEncodeMP3Mem2_put_HighFrequencyMode(This,rhs)	\
    (This)->lpVtbl -> put_HighFrequencyMode(This,rhs)

#define IEncodeMP3Mem2_get_CopyrightBit(This,l)	\
    (This)->lpVtbl -> get_CopyrightBit(This,l)

#define IEncodeMP3Mem2_put_CopyrightBit(This,rhs)	\
    (This)->lpVtbl -> put_CopyrightBit(This,rhs)

#define IEncodeMP3Mem2_get_WarningLowDiskSpace(This,l)	\
    (This)->lpVtbl -> get_WarningLowDiskSpace(This,l)

#define IEncodeMP3Mem2_get_WarningChangedProperties(This)	\
    (This)->lpVtbl -> get_WarningChangedProperties(This)

#define IEncodeMP3Mem2_get_OriginalBit(This,l)	\
    (This)->lpVtbl -> get_OriginalBit(This,l)

#define IEncodeMP3Mem2_put_OriginalBit(This,rhs)	\
    (This)->lpVtbl -> put_OriginalBit(This,rhs)

#define IEncodeMP3Mem2_get_ChooseBestSamplesPerSecond(This,l)	\
    (This)->lpVtbl -> get_ChooseBestSamplesPerSecond(This,l)

#define IEncodeMP3Mem2_put_ChooseBestSamplesPerSecond(This,rhs)	\
    (This)->lpVtbl -> put_ChooseBestSamplesPerSecond(This,rhs)

#define IEncodeMP3Mem2_PutStandardProperties(This,lStandardMPEGType)	\
    (This)->lpVtbl -> PutStandardProperties(This,lStandardMPEGType)

#define IEncodeMP3Mem2_InitSourceAudioProperties(This,lSrcDataType,lSrcSamplesPerSec,lSrcChannels,lSrcBitDepth)	\
    (This)->lpVtbl -> InitSourceAudioProperties(This,lSrcDataType,lSrcSamplesPerSec,lSrcChannels,lSrcBitDepth)

#define IEncodeMP3Mem2_EncodeBegin(This,strMPEGAudFile,bMemoryOutput,lSizeOutputToMemBuf,pKey)	\
    (This)->lpVtbl -> EncodeBegin(This,strMPEGAudFile,bMemoryOutput,lSizeOutputToMemBuf,pKey)

#define IEncodeMP3Mem2_InputAudio(This,plCurDataLen,pWAVAudBuf)	\
    (This)->lpVtbl -> InputAudio(This,plCurDataLen,pWAVAudBuf)

#define IEncodeMP3Mem2_EncodeAudio(This,bLastTime,plCurDataLenForMem,pOutputBufForMem)	\
    (This)->lpVtbl -> EncodeAudio(This,bLastTime,plCurDataLenForMem,pOutputBufForMem)

#define IEncodeMP3Mem2_EncodeEnd(This)	\
    (This)->lpVtbl -> EncodeEnd(This)

#define IEncodeMP3Mem2_InitAuthentication(This,pKey)	\
    (This)->lpVtbl -> InitAuthentication(This,pKey)

#define IEncodeMP3Mem2_UnlockSDK(This,pKey)	\
    (This)->lpVtbl -> UnlockSDK(This,pKey)

#define IEncodeMP3Mem2_VerifyEncode(This,strSrcWAVFile,strMPEGAudioFile)	\
    (This)->lpVtbl -> VerifyEncode(This,strSrcWAVFile,strMPEGAudioFile)

#define IEncodeMP3Mem2_get_WarningUnsupportedSource(This,l)	\
    (This)->lpVtbl -> get_WarningUnsupportedSource(This,l)

#define IEncodeMP3Mem2_get_MP3Frames(This,l)	\
    (This)->lpVtbl -> get_MP3Frames(This,l)

#define IEncodeMP3Mem2_get_VBRHeaderBytes(This,l)	\
    (This)->lpVtbl -> get_VBRHeaderBytes(This,l)

#define IEncodeMP3Mem2_VBRHeaderUpdate(This,lFrames,lCumOutBytes,lVBRLevel,bs_buffer)	\
    (This)->lpVtbl -> VBRHeaderUpdate(This,lFrames,lCumOutBytes,lVBRLevel,bs_buffer)

#define IEncodeMP3Mem2_VBRSetCustomBuffer(This,ulFlags,pCustBuf)	\
    (This)->lpVtbl -> VBRSetCustomBuffer(This,ulFlags,pCustBuf)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_get_Algorithm_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3Mem2_get_Algorithm_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_get_BitRate_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3Mem2_get_BitRate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_put_BitRate_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [in] */ long rhs);


void __RPC_STUB IEncodeMP3Mem2_put_BitRate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_get_SamplesPerSecond_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3Mem2_get_SamplesPerSecond_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_put_SamplesPerSecond_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [in] */ long rhs);


void __RPC_STUB IEncodeMP3Mem2_put_SamplesPerSecond_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_get_ChannelMode_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3Mem2_get_ChannelMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_put_ChannelMode_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [in] */ long rhs);


void __RPC_STUB IEncodeMP3Mem2_put_ChannelMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_get_StereoBands_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3Mem2_get_StereoBands_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_put_StereoBands_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [in] */ long rhs);


void __RPC_STUB IEncodeMP3Mem2_put_StereoBands_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_get_VariableBitRateScaleFactor_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3Mem2_get_VariableBitRateScaleFactor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_put_VariableBitRateScaleFactor_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [in] */ long rhs);


void __RPC_STUB IEncodeMP3Mem2_put_VariableBitRateScaleFactor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_get_DownMixToMono_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3Mem2_get_DownMixToMono_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_put_DownMixToMono_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [in] */ long rhs);


void __RPC_STUB IEncodeMP3Mem2_put_DownMixToMono_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_get_VariableBitRateHeader_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3Mem2_get_VariableBitRateHeader_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_put_VariableBitRateHeader_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [in] */ long rhs);


void __RPC_STUB IEncodeMP3Mem2_put_VariableBitRateHeader_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_get_VariableBitRateTOC_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3Mem2_get_VariableBitRateTOC_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_put_VariableBitRateTOC_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [in] */ long rhs);


void __RPC_STUB IEncodeMP3Mem2_put_VariableBitRateTOC_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_get_HighFrequencyMode_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3Mem2_get_HighFrequencyMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_put_HighFrequencyMode_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [in] */ long rhs);


void __RPC_STUB IEncodeMP3Mem2_put_HighFrequencyMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_get_CopyrightBit_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3Mem2_get_CopyrightBit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_put_CopyrightBit_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [in] */ long rhs);


void __RPC_STUB IEncodeMP3Mem2_put_CopyrightBit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_get_WarningLowDiskSpace_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3Mem2_get_WarningLowDiskSpace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_get_WarningChangedProperties_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This);


void __RPC_STUB IEncodeMP3Mem2_get_WarningChangedProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_get_OriginalBit_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3Mem2_get_OriginalBit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_put_OriginalBit_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [in] */ long rhs);


void __RPC_STUB IEncodeMP3Mem2_put_OriginalBit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_get_ChooseBestSamplesPerSecond_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3Mem2_get_ChooseBestSamplesPerSecond_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_put_ChooseBestSamplesPerSecond_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [in] */ long rhs);


void __RPC_STUB IEncodeMP3Mem2_put_ChooseBestSamplesPerSecond_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_PutStandardProperties_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [in] */ long lStandardMPEGType);


void __RPC_STUB IEncodeMP3Mem2_PutStandardProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_InitSourceAudioProperties_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [in] */ long lSrcDataType,
    /* [in] */ long lSrcSamplesPerSec,
    /* [in] */ long lSrcChannels,
    /* [in] */ long lSrcBitDepth);


void __RPC_STUB IEncodeMP3Mem2_InitSourceAudioProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_EncodeBegin_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [in] */ BSTR strMPEGAudFile,
    /* [in] */ long bMemoryOutput,
    /* [in] */ long lSizeOutputToMemBuf,
    /* [in] */ long __RPC_FAR *pKey);


void __RPC_STUB IEncodeMP3Mem2_EncodeBegin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_InputAudio_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [out][in] */ unsigned long __RPC_FAR *plCurDataLen,
    /* [in] */ long __RPC_FAR *pWAVAudBuf);


void __RPC_STUB IEncodeMP3Mem2_InputAudio_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_EncodeAudio_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [in] */ long bLastTime,
    /* [out][in] */ long __RPC_FAR *plCurDataLenForMem,
    /* [out] */ long __RPC_FAR *pOutputBufForMem);


void __RPC_STUB IEncodeMP3Mem2_EncodeAudio_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_EncodeEnd_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This);


void __RPC_STUB IEncodeMP3Mem2_EncodeEnd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_InitAuthentication_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    unsigned char __RPC_FAR *pKey);


void __RPC_STUB IEncodeMP3Mem2_InitAuthentication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_UnlockSDK_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    unsigned char __RPC_FAR *pKey);


void __RPC_STUB IEncodeMP3Mem2_UnlockSDK_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_VerifyEncode_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [in] */ BSTR strSrcWAVFile,
    /* [in] */ BSTR strMPEGAudioFile);


void __RPC_STUB IEncodeMP3Mem2_VerifyEncode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_get_WarningUnsupportedSource_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3Mem2_get_WarningUnsupportedSource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_get_MP3Frames_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3Mem2_get_MP3Frames_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_get_VBRHeaderBytes_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3Mem2_get_VBRHeaderBytes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_VBRHeaderUpdate_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [in] */ long lFrames,
    /* [in] */ long lCumOutBytes,
    /* [in] */ long lVBRLevel,
    /* [out][in] */ unsigned char __RPC_FAR *bs_buffer);


void __RPC_STUB IEncodeMP3Mem2_VBRHeaderUpdate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3Mem2_VBRSetCustomBuffer_Proxy( 
    IEncodeMP3Mem2 __RPC_FAR * This,
    /* [in] */ unsigned long ulFlags,
    /* [in] */ unsigned char __RPC_FAR *pCustBuf);


void __RPC_STUB IEncodeMP3Mem2_VBRSetCustomBuffer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEncodeMP3Mem2_INTERFACE_DEFINED__ */


#ifndef __IQueryWavFile2_INTERFACE_DEFINED__
#define __IQueryWavFile2_INTERFACE_DEFINED__

/* interface IQueryWavFile2 */
/* [object][helpstring][uuid] */ 


EXTERN_C const IID IID_IQueryWavFile2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("477A3784-2D4D-11D3-B244-444553540000")
    IQueryWavFile2 : public IDispatch
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_SamplesPerSecond( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Channels( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_BitsPerSample( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_FileMilliSeconds( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_FileSize( 
            /* [retval][out] */ double __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE QueryFile( 
            /* [in] */ BSTR bstrWavFile) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_IsUnsupportedWAVFile( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_IsValidWAVFileForEncode( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_OffsetToAudioData( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IQueryWavFile2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IQueryWavFile2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IQueryWavFile2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IQueryWavFile2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IQueryWavFile2 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IQueryWavFile2 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IQueryWavFile2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IQueryWavFile2 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SamplesPerSecond )( 
            IQueryWavFile2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Channels )( 
            IQueryWavFile2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_BitsPerSample )( 
            IQueryWavFile2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FileMilliSeconds )( 
            IQueryWavFile2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FileSize )( 
            IQueryWavFile2 __RPC_FAR * This,
            /* [retval][out] */ double __RPC_FAR *l);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryFile )( 
            IQueryWavFile2 __RPC_FAR * This,
            /* [in] */ BSTR bstrWavFile);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsUnsupportedWAVFile )( 
            IQueryWavFile2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsValidWAVFileForEncode )( 
            IQueryWavFile2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_OffsetToAudioData )( 
            IQueryWavFile2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        END_INTERFACE
    } IQueryWavFile2Vtbl;

    interface IQueryWavFile2
    {
        CONST_VTBL struct IQueryWavFile2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IQueryWavFile2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IQueryWavFile2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IQueryWavFile2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IQueryWavFile2_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IQueryWavFile2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IQueryWavFile2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IQueryWavFile2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IQueryWavFile2_get_SamplesPerSecond(This,l)	\
    (This)->lpVtbl -> get_SamplesPerSecond(This,l)

#define IQueryWavFile2_get_Channels(This,l)	\
    (This)->lpVtbl -> get_Channels(This,l)

#define IQueryWavFile2_get_BitsPerSample(This,l)	\
    (This)->lpVtbl -> get_BitsPerSample(This,l)

#define IQueryWavFile2_get_FileMilliSeconds(This,l)	\
    (This)->lpVtbl -> get_FileMilliSeconds(This,l)

#define IQueryWavFile2_get_FileSize(This,l)	\
    (This)->lpVtbl -> get_FileSize(This,l)

#define IQueryWavFile2_QueryFile(This,bstrWavFile)	\
    (This)->lpVtbl -> QueryFile(This,bstrWavFile)

#define IQueryWavFile2_get_IsUnsupportedWAVFile(This,l)	\
    (This)->lpVtbl -> get_IsUnsupportedWAVFile(This,l)

#define IQueryWavFile2_get_IsValidWAVFileForEncode(This,l)	\
    (This)->lpVtbl -> get_IsValidWAVFileForEncode(This,l)

#define IQueryWavFile2_get_OffsetToAudioData(This,l)	\
    (This)->lpVtbl -> get_OffsetToAudioData(This,l)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IQueryWavFile2_get_SamplesPerSecond_Proxy( 
    IQueryWavFile2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IQueryWavFile2_get_SamplesPerSecond_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IQueryWavFile2_get_Channels_Proxy( 
    IQueryWavFile2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IQueryWavFile2_get_Channels_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IQueryWavFile2_get_BitsPerSample_Proxy( 
    IQueryWavFile2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IQueryWavFile2_get_BitsPerSample_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IQueryWavFile2_get_FileMilliSeconds_Proxy( 
    IQueryWavFile2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IQueryWavFile2_get_FileMilliSeconds_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IQueryWavFile2_get_FileSize_Proxy( 
    IQueryWavFile2 __RPC_FAR * This,
    /* [retval][out] */ double __RPC_FAR *l);


void __RPC_STUB IQueryWavFile2_get_FileSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IQueryWavFile2_QueryFile_Proxy( 
    IQueryWavFile2 __RPC_FAR * This,
    /* [in] */ BSTR bstrWavFile);


void __RPC_STUB IQueryWavFile2_QueryFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IQueryWavFile2_get_IsUnsupportedWAVFile_Proxy( 
    IQueryWavFile2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IQueryWavFile2_get_IsUnsupportedWAVFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IQueryWavFile2_get_IsValidWAVFileForEncode_Proxy( 
    IQueryWavFile2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IQueryWavFile2_get_IsValidWAVFileForEncode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IQueryWavFile2_get_OffsetToAudioData_Proxy( 
    IQueryWavFile2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IQueryWavFile2_get_OffsetToAudioData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IQueryWavFile2_INTERFACE_DEFINED__ */


#ifndef __IQueryMPEGAudioFile2_INTERFACE_DEFINED__
#define __IQueryMPEGAudioFile2_INTERFACE_DEFINED__

/* interface IQueryMPEGAudioFile2 */
/* [object][helpstring][uuid] */ 


EXTERN_C const IID IID_IQueryMPEGAudioFile2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("477A3786-2D4D-11D3-B244-444553540000")
    IQueryMPEGAudioFile2 : public IDispatch
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Algorithm( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_BitRate( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_SamplesPerSecond( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_ChannelMode( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_StereoBands( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_FileMilliSeconds( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE QueryFile( 
            /* [in] */ BSTR bstrFile) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_FileSize( 
            /* [retval][out] */ double __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_VariableBitRateLevel( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_IsVariableBitRateHeader( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_IsVariableBitRateTOC( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CopyVBRTableOfContents( void) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_IsCopyrightBit( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_IsVariableBitRateFile( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_IsValidMPEGAudioFile( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_IsValidMP3File( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Channels( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_IsOriginalBit( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetVBRByteOffsetFromPercent( 
            /* [in] */ float fPercent) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IQueryMPEGAudioFile2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IQueryMPEGAudioFile2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IQueryMPEGAudioFile2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IQueryMPEGAudioFile2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IQueryMPEGAudioFile2 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IQueryMPEGAudioFile2 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IQueryMPEGAudioFile2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IQueryMPEGAudioFile2 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Algorithm )( 
            IQueryMPEGAudioFile2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_BitRate )( 
            IQueryMPEGAudioFile2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SamplesPerSecond )( 
            IQueryMPEGAudioFile2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ChannelMode )( 
            IQueryMPEGAudioFile2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_StereoBands )( 
            IQueryMPEGAudioFile2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FileMilliSeconds )( 
            IQueryMPEGAudioFile2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryFile )( 
            IQueryMPEGAudioFile2 __RPC_FAR * This,
            /* [in] */ BSTR bstrFile);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FileSize )( 
            IQueryMPEGAudioFile2 __RPC_FAR * This,
            /* [retval][out] */ double __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_VariableBitRateLevel )( 
            IQueryMPEGAudioFile2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsVariableBitRateHeader )( 
            IQueryMPEGAudioFile2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsVariableBitRateTOC )( 
            IQueryMPEGAudioFile2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CopyVBRTableOfContents )( 
            IQueryMPEGAudioFile2 __RPC_FAR * This);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsCopyrightBit )( 
            IQueryMPEGAudioFile2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsVariableBitRateFile )( 
            IQueryMPEGAudioFile2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsValidMPEGAudioFile )( 
            IQueryMPEGAudioFile2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsValidMP3File )( 
            IQueryMPEGAudioFile2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Channels )( 
            IQueryMPEGAudioFile2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsOriginalBit )( 
            IQueryMPEGAudioFile2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetVBRByteOffsetFromPercent )( 
            IQueryMPEGAudioFile2 __RPC_FAR * This,
            /* [in] */ float fPercent);
        
        END_INTERFACE
    } IQueryMPEGAudioFile2Vtbl;

    interface IQueryMPEGAudioFile2
    {
        CONST_VTBL struct IQueryMPEGAudioFile2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IQueryMPEGAudioFile2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IQueryMPEGAudioFile2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IQueryMPEGAudioFile2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IQueryMPEGAudioFile2_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IQueryMPEGAudioFile2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IQueryMPEGAudioFile2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IQueryMPEGAudioFile2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IQueryMPEGAudioFile2_get_Algorithm(This,l)	\
    (This)->lpVtbl -> get_Algorithm(This,l)

#define IQueryMPEGAudioFile2_get_BitRate(This,l)	\
    (This)->lpVtbl -> get_BitRate(This,l)

#define IQueryMPEGAudioFile2_get_SamplesPerSecond(This,l)	\
    (This)->lpVtbl -> get_SamplesPerSecond(This,l)

#define IQueryMPEGAudioFile2_get_ChannelMode(This,l)	\
    (This)->lpVtbl -> get_ChannelMode(This,l)

#define IQueryMPEGAudioFile2_get_StereoBands(This,l)	\
    (This)->lpVtbl -> get_StereoBands(This,l)

#define IQueryMPEGAudioFile2_get_FileMilliSeconds(This,l)	\
    (This)->lpVtbl -> get_FileMilliSeconds(This,l)

#define IQueryMPEGAudioFile2_QueryFile(This,bstrFile)	\
    (This)->lpVtbl -> QueryFile(This,bstrFile)

#define IQueryMPEGAudioFile2_get_FileSize(This,l)	\
    (This)->lpVtbl -> get_FileSize(This,l)

#define IQueryMPEGAudioFile2_get_VariableBitRateLevel(This,l)	\
    (This)->lpVtbl -> get_VariableBitRateLevel(This,l)

#define IQueryMPEGAudioFile2_get_IsVariableBitRateHeader(This,l)	\
    (This)->lpVtbl -> get_IsVariableBitRateHeader(This,l)

#define IQueryMPEGAudioFile2_get_IsVariableBitRateTOC(This,l)	\
    (This)->lpVtbl -> get_IsVariableBitRateTOC(This,l)

#define IQueryMPEGAudioFile2_CopyVBRTableOfContents(This)	\
    (This)->lpVtbl -> CopyVBRTableOfContents(This)

#define IQueryMPEGAudioFile2_get_IsCopyrightBit(This,l)	\
    (This)->lpVtbl -> get_IsCopyrightBit(This,l)

#define IQueryMPEGAudioFile2_get_IsVariableBitRateFile(This,l)	\
    (This)->lpVtbl -> get_IsVariableBitRateFile(This,l)

#define IQueryMPEGAudioFile2_get_IsValidMPEGAudioFile(This,l)	\
    (This)->lpVtbl -> get_IsValidMPEGAudioFile(This,l)

#define IQueryMPEGAudioFile2_get_IsValidMP3File(This,l)	\
    (This)->lpVtbl -> get_IsValidMP3File(This,l)

#define IQueryMPEGAudioFile2_get_Channels(This,l)	\
    (This)->lpVtbl -> get_Channels(This,l)

#define IQueryMPEGAudioFile2_get_IsOriginalBit(This,l)	\
    (This)->lpVtbl -> get_IsOriginalBit(This,l)

#define IQueryMPEGAudioFile2_GetVBRByteOffsetFromPercent(This,fPercent)	\
    (This)->lpVtbl -> GetVBRByteOffsetFromPercent(This,fPercent)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IQueryMPEGAudioFile2_get_Algorithm_Proxy( 
    IQueryMPEGAudioFile2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IQueryMPEGAudioFile2_get_Algorithm_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IQueryMPEGAudioFile2_get_BitRate_Proxy( 
    IQueryMPEGAudioFile2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IQueryMPEGAudioFile2_get_BitRate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IQueryMPEGAudioFile2_get_SamplesPerSecond_Proxy( 
    IQueryMPEGAudioFile2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IQueryMPEGAudioFile2_get_SamplesPerSecond_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IQueryMPEGAudioFile2_get_ChannelMode_Proxy( 
    IQueryMPEGAudioFile2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IQueryMPEGAudioFile2_get_ChannelMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IQueryMPEGAudioFile2_get_StereoBands_Proxy( 
    IQueryMPEGAudioFile2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IQueryMPEGAudioFile2_get_StereoBands_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IQueryMPEGAudioFile2_get_FileMilliSeconds_Proxy( 
    IQueryMPEGAudioFile2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IQueryMPEGAudioFile2_get_FileMilliSeconds_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IQueryMPEGAudioFile2_QueryFile_Proxy( 
    IQueryMPEGAudioFile2 __RPC_FAR * This,
    /* [in] */ BSTR bstrFile);


void __RPC_STUB IQueryMPEGAudioFile2_QueryFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IQueryMPEGAudioFile2_get_FileSize_Proxy( 
    IQueryMPEGAudioFile2 __RPC_FAR * This,
    /* [retval][out] */ double __RPC_FAR *l);


void __RPC_STUB IQueryMPEGAudioFile2_get_FileSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IQueryMPEGAudioFile2_get_VariableBitRateLevel_Proxy( 
    IQueryMPEGAudioFile2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IQueryMPEGAudioFile2_get_VariableBitRateLevel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IQueryMPEGAudioFile2_get_IsVariableBitRateHeader_Proxy( 
    IQueryMPEGAudioFile2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IQueryMPEGAudioFile2_get_IsVariableBitRateHeader_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IQueryMPEGAudioFile2_get_IsVariableBitRateTOC_Proxy( 
    IQueryMPEGAudioFile2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IQueryMPEGAudioFile2_get_IsVariableBitRateTOC_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IQueryMPEGAudioFile2_CopyVBRTableOfContents_Proxy( 
    IQueryMPEGAudioFile2 __RPC_FAR * This);


void __RPC_STUB IQueryMPEGAudioFile2_CopyVBRTableOfContents_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IQueryMPEGAudioFile2_get_IsCopyrightBit_Proxy( 
    IQueryMPEGAudioFile2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IQueryMPEGAudioFile2_get_IsCopyrightBit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IQueryMPEGAudioFile2_get_IsVariableBitRateFile_Proxy( 
    IQueryMPEGAudioFile2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IQueryMPEGAudioFile2_get_IsVariableBitRateFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IQueryMPEGAudioFile2_get_IsValidMPEGAudioFile_Proxy( 
    IQueryMPEGAudioFile2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IQueryMPEGAudioFile2_get_IsValidMPEGAudioFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IQueryMPEGAudioFile2_get_IsValidMP3File_Proxy( 
    IQueryMPEGAudioFile2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IQueryMPEGAudioFile2_get_IsValidMP3File_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IQueryMPEGAudioFile2_get_Channels_Proxy( 
    IQueryMPEGAudioFile2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IQueryMPEGAudioFile2_get_Channels_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IQueryMPEGAudioFile2_get_IsOriginalBit_Proxy( 
    IQueryMPEGAudioFile2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IQueryMPEGAudioFile2_get_IsOriginalBit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IQueryMPEGAudioFile2_GetVBRByteOffsetFromPercent_Proxy( 
    IQueryMPEGAudioFile2 __RPC_FAR * This,
    /* [in] */ float fPercent);


void __RPC_STUB IQueryMPEGAudioFile2_GetVBRByteOffsetFromPercent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IQueryMPEGAudioFile2_INTERFACE_DEFINED__ */


#ifndef __IMP3Encoder3_INTERFACE_DEFINED__
#define __IMP3Encoder3_INTERFACE_DEFINED__

/* interface IMP3Encoder3 */
/* [object][helpstring][uuid] */ 


EXTERN_C const IID IID_IMP3Encoder3;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("477A3788-2D4D-11D3-B244-444553540000")
    IMP3Encoder3 : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Init( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetOutputStereoMode( 
            int iMode) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetOutputNumberOfSubbands( 
            int iNumSubbands) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetOutputChannelBitrate( 
            int iBitRate) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetInputSampleRate( 
            int iSampleRate) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetEncodingLayer( 
            int iLayer) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ConvertFile( 
            unsigned char __RPC_FAR *szWavFile,
            unsigned char __RPC_FAR *szMP3File,
            unsigned char __RPC_FAR *pKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetOutputDownMixToMono( 
            long bDownMixToMono) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetEncodingAlgorithm( 
            int iAlgorithm) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartMemoryEncode( 
            int __RPC_FAR *pMinBytesInInputBuffer,
            unsigned char __RPC_FAR *pKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetInputSampleSize( 
            int iSampleSize) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EncodeSamples( 
            unsigned char __RPC_FAR *pPCMBuffer,
            unsigned char __RPC_FAR *pMPEGBuffer,
            int __RPC_FAR *pBytesRead,
            int __RPC_FAR *pBytesWritten) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EndMemoryEncode( 
            unsigned char __RPC_FAR *pMPEGBuffer,
            int __RPC_FAR *pBytesWritten) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetInputNumberOfChannels( 
            int iNumChannels) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetInputSampleRate( 
            int __RPC_FAR *pSampleRate) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetInputNumberOfChannels( 
            int __RPC_FAR *pNumChannels) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetInputSampleSize( 
            int __RPC_FAR *pSampleSize) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetInputDownMixToMono( 
            long __RPC_FAR *pDownMixToMono) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetOutputStereoMode( 
            int __RPC_FAR *pMode) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetOutputNumberOfSubbands( 
            int __RPC_FAR *pNumSubbands) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetOutputChannelBitrate( 
            int __RPC_FAR *pBitrate) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetEncodingAlgorithm( 
            int __RPC_FAR *pAlgorithm) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetEncodingLayer( 
            int __RPC_FAR *pLayer) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE VerifySettings( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetErrorString( 
            HRESULT hrErrorCode,
            unsigned char __RPC_FAR *szErrorText,
            int iMaxTextLen) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE InitAuthentication( 
            unsigned char __RPC_FAR *pKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE UnlockSDK( 
            unsigned char __RPC_FAR *pKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetHighFrequencyMode( 
            int iFrequencyMode) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetCopyrightBit( 
            long bSet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetVariableBitRateScale( 
            int iScale) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetVariableBitRateEncode( 
            long bSet,
            long bCreateTOC) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WriteVariableBitRateHeader( 
            /* [in] */ unsigned char __RPC_FAR *szMP3File) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetOriginalBit( 
            long bSet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetOutputTotalChannelBitrate( 
            int iBitRate) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetOutputSampleRate( 
            int nOutSampleRate) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetOutputSampleRate( 
            int __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetChooseBestOutputSampleRate( 
            long bChooseRate) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetHighFrequencyMode( 
            int __RPC_FAR *pFreqMode) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetVariableBitRateScale( 
            int __RPC_FAR *pFreqMode) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMP3Encoder3Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMP3Encoder3 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMP3Encoder3 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMP3Encoder3 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IMP3Encoder3 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IMP3Encoder3 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IMP3Encoder3 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IMP3Encoder3 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Init )( 
            IMP3Encoder3 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetOutputStereoMode )( 
            IMP3Encoder3 __RPC_FAR * This,
            int iMode);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetOutputNumberOfSubbands )( 
            IMP3Encoder3 __RPC_FAR * This,
            int iNumSubbands);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetOutputChannelBitrate )( 
            IMP3Encoder3 __RPC_FAR * This,
            int iBitRate);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetInputSampleRate )( 
            IMP3Encoder3 __RPC_FAR * This,
            int iSampleRate);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEncodingLayer )( 
            IMP3Encoder3 __RPC_FAR * This,
            int iLayer);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ConvertFile )( 
            IMP3Encoder3 __RPC_FAR * This,
            unsigned char __RPC_FAR *szWavFile,
            unsigned char __RPC_FAR *szMP3File,
            unsigned char __RPC_FAR *pKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetOutputDownMixToMono )( 
            IMP3Encoder3 __RPC_FAR * This,
            long bDownMixToMono);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEncodingAlgorithm )( 
            IMP3Encoder3 __RPC_FAR * This,
            int iAlgorithm);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StartMemoryEncode )( 
            IMP3Encoder3 __RPC_FAR * This,
            int __RPC_FAR *pMinBytesInInputBuffer,
            unsigned char __RPC_FAR *pKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetInputSampleSize )( 
            IMP3Encoder3 __RPC_FAR * This,
            int iSampleSize);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EncodeSamples )( 
            IMP3Encoder3 __RPC_FAR * This,
            unsigned char __RPC_FAR *pPCMBuffer,
            unsigned char __RPC_FAR *pMPEGBuffer,
            int __RPC_FAR *pBytesRead,
            int __RPC_FAR *pBytesWritten);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EndMemoryEncode )( 
            IMP3Encoder3 __RPC_FAR * This,
            unsigned char __RPC_FAR *pMPEGBuffer,
            int __RPC_FAR *pBytesWritten);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetInputNumberOfChannels )( 
            IMP3Encoder3 __RPC_FAR * This,
            int iNumChannels);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetInputSampleRate )( 
            IMP3Encoder3 __RPC_FAR * This,
            int __RPC_FAR *pSampleRate);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetInputNumberOfChannels )( 
            IMP3Encoder3 __RPC_FAR * This,
            int __RPC_FAR *pNumChannels);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetInputSampleSize )( 
            IMP3Encoder3 __RPC_FAR * This,
            int __RPC_FAR *pSampleSize);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetInputDownMixToMono )( 
            IMP3Encoder3 __RPC_FAR * This,
            long __RPC_FAR *pDownMixToMono);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetOutputStereoMode )( 
            IMP3Encoder3 __RPC_FAR * This,
            int __RPC_FAR *pMode);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetOutputNumberOfSubbands )( 
            IMP3Encoder3 __RPC_FAR * This,
            int __RPC_FAR *pNumSubbands);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetOutputChannelBitrate )( 
            IMP3Encoder3 __RPC_FAR * This,
            int __RPC_FAR *pBitrate);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetEncodingAlgorithm )( 
            IMP3Encoder3 __RPC_FAR * This,
            int __RPC_FAR *pAlgorithm);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetEncodingLayer )( 
            IMP3Encoder3 __RPC_FAR * This,
            int __RPC_FAR *pLayer);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *VerifySettings )( 
            IMP3Encoder3 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetErrorString )( 
            IMP3Encoder3 __RPC_FAR * This,
            HRESULT hrErrorCode,
            unsigned char __RPC_FAR *szErrorText,
            int iMaxTextLen);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InitAuthentication )( 
            IMP3Encoder3 __RPC_FAR * This,
            unsigned char __RPC_FAR *pKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnlockSDK )( 
            IMP3Encoder3 __RPC_FAR * This,
            unsigned char __RPC_FAR *pKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetHighFrequencyMode )( 
            IMP3Encoder3 __RPC_FAR * This,
            int iFrequencyMode);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetCopyrightBit )( 
            IMP3Encoder3 __RPC_FAR * This,
            long bSet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetVariableBitRateScale )( 
            IMP3Encoder3 __RPC_FAR * This,
            int iScale);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetVariableBitRateEncode )( 
            IMP3Encoder3 __RPC_FAR * This,
            long bSet,
            long bCreateTOC);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WriteVariableBitRateHeader )( 
            IMP3Encoder3 __RPC_FAR * This,
            /* [in] */ unsigned char __RPC_FAR *szMP3File);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetOriginalBit )( 
            IMP3Encoder3 __RPC_FAR * This,
            long bSet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetOutputTotalChannelBitrate )( 
            IMP3Encoder3 __RPC_FAR * This,
            int iBitRate);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetOutputSampleRate )( 
            IMP3Encoder3 __RPC_FAR * This,
            int nOutSampleRate);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetOutputSampleRate )( 
            IMP3Encoder3 __RPC_FAR * This,
            int __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetChooseBestOutputSampleRate )( 
            IMP3Encoder3 __RPC_FAR * This,
            long bChooseRate);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetHighFrequencyMode )( 
            IMP3Encoder3 __RPC_FAR * This,
            int __RPC_FAR *pFreqMode);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetVariableBitRateScale )( 
            IMP3Encoder3 __RPC_FAR * This,
            int __RPC_FAR *pFreqMode);
        
        END_INTERFACE
    } IMP3Encoder3Vtbl;

    interface IMP3Encoder3
    {
        CONST_VTBL struct IMP3Encoder3Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMP3Encoder3_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMP3Encoder3_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMP3Encoder3_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMP3Encoder3_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMP3Encoder3_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMP3Encoder3_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMP3Encoder3_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMP3Encoder3_Init(This)	\
    (This)->lpVtbl -> Init(This)

#define IMP3Encoder3_SetOutputStereoMode(This,iMode)	\
    (This)->lpVtbl -> SetOutputStereoMode(This,iMode)

#define IMP3Encoder3_SetOutputNumberOfSubbands(This,iNumSubbands)	\
    (This)->lpVtbl -> SetOutputNumberOfSubbands(This,iNumSubbands)

#define IMP3Encoder3_SetOutputChannelBitrate(This,iBitRate)	\
    (This)->lpVtbl -> SetOutputChannelBitrate(This,iBitRate)

#define IMP3Encoder3_SetInputSampleRate(This,iSampleRate)	\
    (This)->lpVtbl -> SetInputSampleRate(This,iSampleRate)

#define IMP3Encoder3_SetEncodingLayer(This,iLayer)	\
    (This)->lpVtbl -> SetEncodingLayer(This,iLayer)

#define IMP3Encoder3_ConvertFile(This,szWavFile,szMP3File,pKey)	\
    (This)->lpVtbl -> ConvertFile(This,szWavFile,szMP3File,pKey)

#define IMP3Encoder3_SetOutputDownMixToMono(This,bDownMixToMono)	\
    (This)->lpVtbl -> SetOutputDownMixToMono(This,bDownMixToMono)

#define IMP3Encoder3_SetEncodingAlgorithm(This,iAlgorithm)	\
    (This)->lpVtbl -> SetEncodingAlgorithm(This,iAlgorithm)

#define IMP3Encoder3_StartMemoryEncode(This,pMinBytesInInputBuffer,pKey)	\
    (This)->lpVtbl -> StartMemoryEncode(This,pMinBytesInInputBuffer,pKey)

#define IMP3Encoder3_SetInputSampleSize(This,iSampleSize)	\
    (This)->lpVtbl -> SetInputSampleSize(This,iSampleSize)

#define IMP3Encoder3_EncodeSamples(This,pPCMBuffer,pMPEGBuffer,pBytesRead,pBytesWritten)	\
    (This)->lpVtbl -> EncodeSamples(This,pPCMBuffer,pMPEGBuffer,pBytesRead,pBytesWritten)

#define IMP3Encoder3_EndMemoryEncode(This,pMPEGBuffer,pBytesWritten)	\
    (This)->lpVtbl -> EndMemoryEncode(This,pMPEGBuffer,pBytesWritten)

#define IMP3Encoder3_SetInputNumberOfChannels(This,iNumChannels)	\
    (This)->lpVtbl -> SetInputNumberOfChannels(This,iNumChannels)

#define IMP3Encoder3_GetInputSampleRate(This,pSampleRate)	\
    (This)->lpVtbl -> GetInputSampleRate(This,pSampleRate)

#define IMP3Encoder3_GetInputNumberOfChannels(This,pNumChannels)	\
    (This)->lpVtbl -> GetInputNumberOfChannels(This,pNumChannels)

#define IMP3Encoder3_GetInputSampleSize(This,pSampleSize)	\
    (This)->lpVtbl -> GetInputSampleSize(This,pSampleSize)

#define IMP3Encoder3_GetInputDownMixToMono(This,pDownMixToMono)	\
    (This)->lpVtbl -> GetInputDownMixToMono(This,pDownMixToMono)

#define IMP3Encoder3_GetOutputStereoMode(This,pMode)	\
    (This)->lpVtbl -> GetOutputStereoMode(This,pMode)

#define IMP3Encoder3_GetOutputNumberOfSubbands(This,pNumSubbands)	\
    (This)->lpVtbl -> GetOutputNumberOfSubbands(This,pNumSubbands)

#define IMP3Encoder3_GetOutputChannelBitrate(This,pBitrate)	\
    (This)->lpVtbl -> GetOutputChannelBitrate(This,pBitrate)

#define IMP3Encoder3_GetEncodingAlgorithm(This,pAlgorithm)	\
    (This)->lpVtbl -> GetEncodingAlgorithm(This,pAlgorithm)

#define IMP3Encoder3_GetEncodingLayer(This,pLayer)	\
    (This)->lpVtbl -> GetEncodingLayer(This,pLayer)

#define IMP3Encoder3_VerifySettings(This)	\
    (This)->lpVtbl -> VerifySettings(This)

#define IMP3Encoder3_GetErrorString(This,hrErrorCode,szErrorText,iMaxTextLen)	\
    (This)->lpVtbl -> GetErrorString(This,hrErrorCode,szErrorText,iMaxTextLen)

#define IMP3Encoder3_InitAuthentication(This,pKey)	\
    (This)->lpVtbl -> InitAuthentication(This,pKey)

#define IMP3Encoder3_UnlockSDK(This,pKey)	\
    (This)->lpVtbl -> UnlockSDK(This,pKey)

#define IMP3Encoder3_SetHighFrequencyMode(This,iFrequencyMode)	\
    (This)->lpVtbl -> SetHighFrequencyMode(This,iFrequencyMode)

#define IMP3Encoder3_SetCopyrightBit(This,bSet)	\
    (This)->lpVtbl -> SetCopyrightBit(This,bSet)

#define IMP3Encoder3_SetVariableBitRateScale(This,iScale)	\
    (This)->lpVtbl -> SetVariableBitRateScale(This,iScale)

#define IMP3Encoder3_SetVariableBitRateEncode(This,bSet,bCreateTOC)	\
    (This)->lpVtbl -> SetVariableBitRateEncode(This,bSet,bCreateTOC)

#define IMP3Encoder3_WriteVariableBitRateHeader(This,szMP3File)	\
    (This)->lpVtbl -> WriteVariableBitRateHeader(This,szMP3File)

#define IMP3Encoder3_SetOriginalBit(This,bSet)	\
    (This)->lpVtbl -> SetOriginalBit(This,bSet)

#define IMP3Encoder3_SetOutputTotalChannelBitrate(This,iBitRate)	\
    (This)->lpVtbl -> SetOutputTotalChannelBitrate(This,iBitRate)

#define IMP3Encoder3_SetOutputSampleRate(This,nOutSampleRate)	\
    (This)->lpVtbl -> SetOutputSampleRate(This,nOutSampleRate)

#define IMP3Encoder3_GetOutputSampleRate(This,pVal)	\
    (This)->lpVtbl -> GetOutputSampleRate(This,pVal)

#define IMP3Encoder3_SetChooseBestOutputSampleRate(This,bChooseRate)	\
    (This)->lpVtbl -> SetChooseBestOutputSampleRate(This,bChooseRate)

#define IMP3Encoder3_GetHighFrequencyMode(This,pFreqMode)	\
    (This)->lpVtbl -> GetHighFrequencyMode(This,pFreqMode)

#define IMP3Encoder3_GetVariableBitRateScale(This,pFreqMode)	\
    (This)->lpVtbl -> GetVariableBitRateScale(This,pFreqMode)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_Init_Proxy( 
    IMP3Encoder3 __RPC_FAR * This);


void __RPC_STUB IMP3Encoder3_Init_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_SetOutputStereoMode_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    int iMode);


void __RPC_STUB IMP3Encoder3_SetOutputStereoMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_SetOutputNumberOfSubbands_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    int iNumSubbands);


void __RPC_STUB IMP3Encoder3_SetOutputNumberOfSubbands_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_SetOutputChannelBitrate_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    int iBitRate);


void __RPC_STUB IMP3Encoder3_SetOutputChannelBitrate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_SetInputSampleRate_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    int iSampleRate);


void __RPC_STUB IMP3Encoder3_SetInputSampleRate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_SetEncodingLayer_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    int iLayer);


void __RPC_STUB IMP3Encoder3_SetEncodingLayer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_ConvertFile_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    unsigned char __RPC_FAR *szWavFile,
    unsigned char __RPC_FAR *szMP3File,
    unsigned char __RPC_FAR *pKey);


void __RPC_STUB IMP3Encoder3_ConvertFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_SetOutputDownMixToMono_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    long bDownMixToMono);


void __RPC_STUB IMP3Encoder3_SetOutputDownMixToMono_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_SetEncodingAlgorithm_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    int iAlgorithm);


void __RPC_STUB IMP3Encoder3_SetEncodingAlgorithm_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_StartMemoryEncode_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    int __RPC_FAR *pMinBytesInInputBuffer,
    unsigned char __RPC_FAR *pKey);


void __RPC_STUB IMP3Encoder3_StartMemoryEncode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_SetInputSampleSize_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    int iSampleSize);


void __RPC_STUB IMP3Encoder3_SetInputSampleSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_EncodeSamples_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    unsigned char __RPC_FAR *pPCMBuffer,
    unsigned char __RPC_FAR *pMPEGBuffer,
    int __RPC_FAR *pBytesRead,
    int __RPC_FAR *pBytesWritten);


void __RPC_STUB IMP3Encoder3_EncodeSamples_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_EndMemoryEncode_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    unsigned char __RPC_FAR *pMPEGBuffer,
    int __RPC_FAR *pBytesWritten);


void __RPC_STUB IMP3Encoder3_EndMemoryEncode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_SetInputNumberOfChannels_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    int iNumChannels);


void __RPC_STUB IMP3Encoder3_SetInputNumberOfChannels_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_GetInputSampleRate_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    int __RPC_FAR *pSampleRate);


void __RPC_STUB IMP3Encoder3_GetInputSampleRate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_GetInputNumberOfChannels_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    int __RPC_FAR *pNumChannels);


void __RPC_STUB IMP3Encoder3_GetInputNumberOfChannels_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_GetInputSampleSize_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    int __RPC_FAR *pSampleSize);


void __RPC_STUB IMP3Encoder3_GetInputSampleSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_GetInputDownMixToMono_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    long __RPC_FAR *pDownMixToMono);


void __RPC_STUB IMP3Encoder3_GetInputDownMixToMono_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_GetOutputStereoMode_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    int __RPC_FAR *pMode);


void __RPC_STUB IMP3Encoder3_GetOutputStereoMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_GetOutputNumberOfSubbands_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    int __RPC_FAR *pNumSubbands);


void __RPC_STUB IMP3Encoder3_GetOutputNumberOfSubbands_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_GetOutputChannelBitrate_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    int __RPC_FAR *pBitrate);


void __RPC_STUB IMP3Encoder3_GetOutputChannelBitrate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_GetEncodingAlgorithm_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    int __RPC_FAR *pAlgorithm);


void __RPC_STUB IMP3Encoder3_GetEncodingAlgorithm_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_GetEncodingLayer_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    int __RPC_FAR *pLayer);


void __RPC_STUB IMP3Encoder3_GetEncodingLayer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_VerifySettings_Proxy( 
    IMP3Encoder3 __RPC_FAR * This);


void __RPC_STUB IMP3Encoder3_VerifySettings_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_GetErrorString_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    HRESULT hrErrorCode,
    unsigned char __RPC_FAR *szErrorText,
    int iMaxTextLen);


void __RPC_STUB IMP3Encoder3_GetErrorString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_InitAuthentication_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    unsigned char __RPC_FAR *pKey);


void __RPC_STUB IMP3Encoder3_InitAuthentication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_UnlockSDK_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    unsigned char __RPC_FAR *pKey);


void __RPC_STUB IMP3Encoder3_UnlockSDK_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_SetHighFrequencyMode_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    int iFrequencyMode);


void __RPC_STUB IMP3Encoder3_SetHighFrequencyMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_SetCopyrightBit_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    long bSet);


void __RPC_STUB IMP3Encoder3_SetCopyrightBit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_SetVariableBitRateScale_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    int iScale);


void __RPC_STUB IMP3Encoder3_SetVariableBitRateScale_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_SetVariableBitRateEncode_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    long bSet,
    long bCreateTOC);


void __RPC_STUB IMP3Encoder3_SetVariableBitRateEncode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_WriteVariableBitRateHeader_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    /* [in] */ unsigned char __RPC_FAR *szMP3File);


void __RPC_STUB IMP3Encoder3_WriteVariableBitRateHeader_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_SetOriginalBit_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    long bSet);


void __RPC_STUB IMP3Encoder3_SetOriginalBit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_SetOutputTotalChannelBitrate_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    int iBitRate);


void __RPC_STUB IMP3Encoder3_SetOutputTotalChannelBitrate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_SetOutputSampleRate_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    int nOutSampleRate);


void __RPC_STUB IMP3Encoder3_SetOutputSampleRate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_GetOutputSampleRate_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    int __RPC_FAR *pVal);


void __RPC_STUB IMP3Encoder3_GetOutputSampleRate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_SetChooseBestOutputSampleRate_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    long bChooseRate);


void __RPC_STUB IMP3Encoder3_SetChooseBestOutputSampleRate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_GetHighFrequencyMode_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    int __RPC_FAR *pFreqMode);


void __RPC_STUB IMP3Encoder3_GetHighFrequencyMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMP3Encoder3_GetVariableBitRateScale_Proxy( 
    IMP3Encoder3 __RPC_FAR * This,
    int __RPC_FAR *pFreqMode);


void __RPC_STUB IMP3Encoder3_GetVariableBitRateScale_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMP3Encoder3_INTERFACE_DEFINED__ */


#ifndef __IEncodeMP3File2_INTERFACE_DEFINED__
#define __IEncodeMP3File2_INTERFACE_DEFINED__

/* interface IEncodeMP3File2 */
/* [object][helpstring][uuid] */ 


EXTERN_C const IID IID_IEncodeMP3File2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A97BBEAF-2D4C-11D3-B244-444553540000")
    IEncodeMP3File2 : public IDispatch
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Algorithm( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_BitRate( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_BitRate( 
            /* [in] */ long rhs) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_SamplesPerSecond( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_SamplesPerSecond( 
            /* [in] */ long rhs) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_ChannelMode( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_ChannelMode( 
            /* [in] */ long rhs) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_StereoBands( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_StereoBands( 
            /* [in] */ long rhs) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_VariableBitRateScaleFactor( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_VariableBitRateScaleFactor( 
            /* [in] */ long rhs) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_DownMixToMono( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_DownMixToMono( 
            /* [in] */ long rhs) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PutStandardProperties( 
            /* [in] */ long lStandardMPEGType) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EncodeWAVToMP3( 
            /* [in] */ BSTR strSrcWAVFile,
            /* [in] */ BSTR strMPEGAudioFile,
            /* [in] */ long __RPC_FAR *pKey,
            /* [in] */ IUnknown __RPC_FAR *pIProgress) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_IsEncodeDone( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_VariableBitRateHeader( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_VariableBitRateHeader( 
            /* [in] */ long rhs) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_VariableBitRateTOC( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_VariableBitRateTOC( 
            /* [in] */ long rhs) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_HighFrequencyMode( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_HighFrequencyMode( 
            /* [in] */ long rhs) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_CopyrightBit( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_CopyrightBit( 
            /* [in] */ long rhs) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE InitAuthentication( 
            unsigned char __RPC_FAR *pKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE UnlockSDK( 
            unsigned char __RPC_FAR *pKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE VerifyEncode( 
            /* [in] */ BSTR strSrcWAVFile,
            /* [in] */ BSTR strMPEGAudioFile) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_WarningLowDiskSpace( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_WarningChangedProperties( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_PercentDone( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_OriginalBit( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_OriginalBit( 
            /* [in] */ long rhs) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_ChooseBestSamplesPerSecond( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_ChooseBestSamplesPerSecond( 
            /* [in] */ long rhs) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_WarningUnsupportedSource( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_EncodeError( 
            /* [retval][out] */ long __RPC_FAR *l) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEncodeMP3File2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IEncodeMP3File2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IEncodeMP3File2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Algorithm )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_BitRate )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_BitRate )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [in] */ long rhs);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SamplesPerSecond )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SamplesPerSecond )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [in] */ long rhs);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ChannelMode )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ChannelMode )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [in] */ long rhs);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_StereoBands )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_StereoBands )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [in] */ long rhs);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_VariableBitRateScaleFactor )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_VariableBitRateScaleFactor )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [in] */ long rhs);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DownMixToMono )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_DownMixToMono )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [in] */ long rhs);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PutStandardProperties )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [in] */ long lStandardMPEGType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EncodeWAVToMP3 )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [in] */ BSTR strSrcWAVFile,
            /* [in] */ BSTR strMPEGAudioFile,
            /* [in] */ long __RPC_FAR *pKey,
            /* [in] */ IUnknown __RPC_FAR *pIProgress);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsEncodeDone )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_VariableBitRateHeader )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_VariableBitRateHeader )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [in] */ long rhs);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_VariableBitRateTOC )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_VariableBitRateTOC )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [in] */ long rhs);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_HighFrequencyMode )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_HighFrequencyMode )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [in] */ long rhs);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CopyrightBit )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_CopyrightBit )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [in] */ long rhs);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InitAuthentication )( 
            IEncodeMP3File2 __RPC_FAR * This,
            unsigned char __RPC_FAR *pKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnlockSDK )( 
            IEncodeMP3File2 __RPC_FAR * This,
            unsigned char __RPC_FAR *pKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *VerifyEncode )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [in] */ BSTR strSrcWAVFile,
            /* [in] */ BSTR strMPEGAudioFile);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WarningLowDiskSpace )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WarningChangedProperties )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PercentDone )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_OriginalBit )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_OriginalBit )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [in] */ long rhs);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ChooseBestSamplesPerSecond )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ChooseBestSamplesPerSecond )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [in] */ long rhs);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WarningUnsupportedSource )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_EncodeError )( 
            IEncodeMP3File2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *l);
        
        END_INTERFACE
    } IEncodeMP3File2Vtbl;

    interface IEncodeMP3File2
    {
        CONST_VTBL struct IEncodeMP3File2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEncodeMP3File2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEncodeMP3File2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEncodeMP3File2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEncodeMP3File2_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IEncodeMP3File2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IEncodeMP3File2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IEncodeMP3File2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IEncodeMP3File2_get_Algorithm(This,l)	\
    (This)->lpVtbl -> get_Algorithm(This,l)

#define IEncodeMP3File2_get_BitRate(This,l)	\
    (This)->lpVtbl -> get_BitRate(This,l)

#define IEncodeMP3File2_put_BitRate(This,rhs)	\
    (This)->lpVtbl -> put_BitRate(This,rhs)

#define IEncodeMP3File2_get_SamplesPerSecond(This,l)	\
    (This)->lpVtbl -> get_SamplesPerSecond(This,l)

#define IEncodeMP3File2_put_SamplesPerSecond(This,rhs)	\
    (This)->lpVtbl -> put_SamplesPerSecond(This,rhs)

#define IEncodeMP3File2_get_ChannelMode(This,l)	\
    (This)->lpVtbl -> get_ChannelMode(This,l)

#define IEncodeMP3File2_put_ChannelMode(This,rhs)	\
    (This)->lpVtbl -> put_ChannelMode(This,rhs)

#define IEncodeMP3File2_get_StereoBands(This,l)	\
    (This)->lpVtbl -> get_StereoBands(This,l)

#define IEncodeMP3File2_put_StereoBands(This,rhs)	\
    (This)->lpVtbl -> put_StereoBands(This,rhs)

#define IEncodeMP3File2_get_VariableBitRateScaleFactor(This,l)	\
    (This)->lpVtbl -> get_VariableBitRateScaleFactor(This,l)

#define IEncodeMP3File2_put_VariableBitRateScaleFactor(This,rhs)	\
    (This)->lpVtbl -> put_VariableBitRateScaleFactor(This,rhs)

#define IEncodeMP3File2_get_DownMixToMono(This,l)	\
    (This)->lpVtbl -> get_DownMixToMono(This,l)

#define IEncodeMP3File2_put_DownMixToMono(This,rhs)	\
    (This)->lpVtbl -> put_DownMixToMono(This,rhs)

#define IEncodeMP3File2_PutStandardProperties(This,lStandardMPEGType)	\
    (This)->lpVtbl -> PutStandardProperties(This,lStandardMPEGType)

#define IEncodeMP3File2_EncodeWAVToMP3(This,strSrcWAVFile,strMPEGAudioFile,pKey,pIProgress)	\
    (This)->lpVtbl -> EncodeWAVToMP3(This,strSrcWAVFile,strMPEGAudioFile,pKey,pIProgress)

#define IEncodeMP3File2_get_IsEncodeDone(This,l)	\
    (This)->lpVtbl -> get_IsEncodeDone(This,l)

#define IEncodeMP3File2_get_VariableBitRateHeader(This,l)	\
    (This)->lpVtbl -> get_VariableBitRateHeader(This,l)

#define IEncodeMP3File2_put_VariableBitRateHeader(This,rhs)	\
    (This)->lpVtbl -> put_VariableBitRateHeader(This,rhs)

#define IEncodeMP3File2_get_VariableBitRateTOC(This,l)	\
    (This)->lpVtbl -> get_VariableBitRateTOC(This,l)

#define IEncodeMP3File2_put_VariableBitRateTOC(This,rhs)	\
    (This)->lpVtbl -> put_VariableBitRateTOC(This,rhs)

#define IEncodeMP3File2_get_HighFrequencyMode(This,l)	\
    (This)->lpVtbl -> get_HighFrequencyMode(This,l)

#define IEncodeMP3File2_put_HighFrequencyMode(This,rhs)	\
    (This)->lpVtbl -> put_HighFrequencyMode(This,rhs)

#define IEncodeMP3File2_get_CopyrightBit(This,l)	\
    (This)->lpVtbl -> get_CopyrightBit(This,l)

#define IEncodeMP3File2_put_CopyrightBit(This,rhs)	\
    (This)->lpVtbl -> put_CopyrightBit(This,rhs)

#define IEncodeMP3File2_InitAuthentication(This,pKey)	\
    (This)->lpVtbl -> InitAuthentication(This,pKey)

#define IEncodeMP3File2_UnlockSDK(This,pKey)	\
    (This)->lpVtbl -> UnlockSDK(This,pKey)

#define IEncodeMP3File2_VerifyEncode(This,strSrcWAVFile,strMPEGAudioFile)	\
    (This)->lpVtbl -> VerifyEncode(This,strSrcWAVFile,strMPEGAudioFile)

#define IEncodeMP3File2_get_WarningLowDiskSpace(This,l)	\
    (This)->lpVtbl -> get_WarningLowDiskSpace(This,l)

#define IEncodeMP3File2_get_WarningChangedProperties(This,l)	\
    (This)->lpVtbl -> get_WarningChangedProperties(This,l)

#define IEncodeMP3File2_get_PercentDone(This,l)	\
    (This)->lpVtbl -> get_PercentDone(This,l)

#define IEncodeMP3File2_get_OriginalBit(This,l)	\
    (This)->lpVtbl -> get_OriginalBit(This,l)

#define IEncodeMP3File2_put_OriginalBit(This,rhs)	\
    (This)->lpVtbl -> put_OriginalBit(This,rhs)

#define IEncodeMP3File2_get_ChooseBestSamplesPerSecond(This,l)	\
    (This)->lpVtbl -> get_ChooseBestSamplesPerSecond(This,l)

#define IEncodeMP3File2_put_ChooseBestSamplesPerSecond(This,rhs)	\
    (This)->lpVtbl -> put_ChooseBestSamplesPerSecond(This,rhs)

#define IEncodeMP3File2_get_WarningUnsupportedSource(This,l)	\
    (This)->lpVtbl -> get_WarningUnsupportedSource(This,l)

#define IEncodeMP3File2_get_EncodeError(This,l)	\
    (This)->lpVtbl -> get_EncodeError(This,l)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_get_Algorithm_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3File2_get_Algorithm_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_get_BitRate_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3File2_get_BitRate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_put_BitRate_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [in] */ long rhs);


void __RPC_STUB IEncodeMP3File2_put_BitRate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_get_SamplesPerSecond_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3File2_get_SamplesPerSecond_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_put_SamplesPerSecond_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [in] */ long rhs);


void __RPC_STUB IEncodeMP3File2_put_SamplesPerSecond_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_get_ChannelMode_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3File2_get_ChannelMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_put_ChannelMode_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [in] */ long rhs);


void __RPC_STUB IEncodeMP3File2_put_ChannelMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_get_StereoBands_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3File2_get_StereoBands_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_put_StereoBands_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [in] */ long rhs);


void __RPC_STUB IEncodeMP3File2_put_StereoBands_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_get_VariableBitRateScaleFactor_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3File2_get_VariableBitRateScaleFactor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_put_VariableBitRateScaleFactor_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [in] */ long rhs);


void __RPC_STUB IEncodeMP3File2_put_VariableBitRateScaleFactor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_get_DownMixToMono_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3File2_get_DownMixToMono_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_put_DownMixToMono_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [in] */ long rhs);


void __RPC_STUB IEncodeMP3File2_put_DownMixToMono_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_PutStandardProperties_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [in] */ long lStandardMPEGType);


void __RPC_STUB IEncodeMP3File2_PutStandardProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_EncodeWAVToMP3_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [in] */ BSTR strSrcWAVFile,
    /* [in] */ BSTR strMPEGAudioFile,
    /* [in] */ long __RPC_FAR *pKey,
    /* [in] */ IUnknown __RPC_FAR *pIProgress);


void __RPC_STUB IEncodeMP3File2_EncodeWAVToMP3_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_get_IsEncodeDone_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3File2_get_IsEncodeDone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_get_VariableBitRateHeader_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3File2_get_VariableBitRateHeader_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_put_VariableBitRateHeader_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [in] */ long rhs);


void __RPC_STUB IEncodeMP3File2_put_VariableBitRateHeader_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_get_VariableBitRateTOC_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3File2_get_VariableBitRateTOC_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_put_VariableBitRateTOC_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [in] */ long rhs);


void __RPC_STUB IEncodeMP3File2_put_VariableBitRateTOC_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_get_HighFrequencyMode_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3File2_get_HighFrequencyMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_put_HighFrequencyMode_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [in] */ long rhs);


void __RPC_STUB IEncodeMP3File2_put_HighFrequencyMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_get_CopyrightBit_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3File2_get_CopyrightBit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_put_CopyrightBit_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [in] */ long rhs);


void __RPC_STUB IEncodeMP3File2_put_CopyrightBit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_InitAuthentication_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    unsigned char __RPC_FAR *pKey);


void __RPC_STUB IEncodeMP3File2_InitAuthentication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_UnlockSDK_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    unsigned char __RPC_FAR *pKey);


void __RPC_STUB IEncodeMP3File2_UnlockSDK_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_VerifyEncode_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [in] */ BSTR strSrcWAVFile,
    /* [in] */ BSTR strMPEGAudioFile);


void __RPC_STUB IEncodeMP3File2_VerifyEncode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_get_WarningLowDiskSpace_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3File2_get_WarningLowDiskSpace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_get_WarningChangedProperties_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3File2_get_WarningChangedProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_get_PercentDone_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3File2_get_PercentDone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_get_OriginalBit_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3File2_get_OriginalBit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_put_OriginalBit_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [in] */ long rhs);


void __RPC_STUB IEncodeMP3File2_put_OriginalBit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_get_ChooseBestSamplesPerSecond_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3File2_get_ChooseBestSamplesPerSecond_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_put_ChooseBestSamplesPerSecond_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [in] */ long rhs);


void __RPC_STUB IEncodeMP3File2_put_ChooseBestSamplesPerSecond_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_get_WarningUnsupportedSource_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3File2_get_WarningUnsupportedSource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IEncodeMP3File2_get_EncodeError_Proxy( 
    IEncodeMP3File2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *l);


void __RPC_STUB IEncodeMP3File2_get_EncodeError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEncodeMP3File2_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


