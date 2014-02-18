//  Microsoft Windows Media Technologies
//  Copyright (C) Microsoft Corporation, 1999 - 2001.  All rights reserved.
//


#if !defined(__SAMPLE_UTIL_H)
#define __SAMPLE_UTIL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


///////////////////////////////////////////////////////////////////////////////
#include "wmsdk.h"


///////////////////////////////////////////////////////////////////////////////
#define SAFE_RELEASE( x )           \
               if( x )              \
               {                    \
                   x->Release();    \
                   x = NULL;        \
               }


#define BASE_STREAM_NUMBER_AUDIO    1     // Audio stream numbers start at 1
#define BASE_STREAM_NUMBER_VIDEO    2     // Video stream numbers start at 2

#define AUDIO_FLAGS_AUDIOVIDEO      1

///////////////////////////////////////////////////////////////////////////////
STDMETHODIMP SetStreamBasics( IWMStreamConfig *pIWMStreamConfig, 
                              WORD wStreamNum, 
                              LPWSTR pwszStreamName,
                              LPWSTR pwszConnectionName,
                              DWORD dwBitrate,
                              WM_MEDIA_TYPE *pmt );

STDMETHODIMP AddAudioStream( DWORD dwFormatTag,
                             DWORD dwBitrate,
                             DWORD dwSampleRate,
                             DWORD dwChannels,
                             DWORD flags,
                             IWMProfile *pIWMProfile,
                             IWMProfileManager *pIWMProfileManager );

STDMETHODIMP AddVideoStream( IWMProfile *pIWMProfile, 
                             IWMProfileManager *pManager,
                             WORD wStreamNum,
                             DWORD dwFourCC,
                             DWORD dwBitrate,
                             DWORD dwWidth,
                             DWORD dwHeight,
                             DWORD dwFPS,
                             DWORD dwQuality,
                             DWORD dwSecPerKey );

STDMETHODIMP AddMutexObject( IWMProfile *pIWMProfile, 
                             WORD cVideoStreams );
                             
STDMETHODIMP AddScriptStream( IWMProfile *pIWMProfile,
                              WORD wStreamNum,
                              DWORD dwBitrate );


#endif // __SAMPLE_UTIL_H                              
