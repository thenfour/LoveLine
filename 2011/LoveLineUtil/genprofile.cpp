//  Microsoft Windows Media Technologies
//  Copyright (C) Microsoft Corporation, 1999 - 2001.  All rights reserved.
//

#include "pch.h"
#include "wmsdk.h"
#include <stdio.h>
#include <string.h>
#include "genprofile.h"
#include "util.h"


///////////////////////////////////////////////////////////////////////////////
STDMETHODIMP GenerateProfile( AUDIO_PARAMS *pAudioParams,
                              SCRIPT_PARAMS *pScriptParams,
                              WORD cVideoStreams,
                              VIDEO_PARAMS *prgVideoParams,
                              IWMProfile **ppIWMProfile )
{
    if( ( NULL == pAudioParams ) || 
        ( NULL == ppIWMProfile ) ||
        ( cVideoStreams > WM_MAX_VIDEO_STREAMS ) ||
        ( cVideoStreams < 0 ) ||
        ( (0 < cVideoStreams ) && (NULL == prgVideoParams ) ) )
    {
        return( E_INVALIDARG );
    }

    HRESULT hr = S_OK;
    IWMProfileManager *pIWMProfileMgr = NULL;
    IWMProfile *pIWMProfile = NULL;

    do
    {
        hr = WMCreateProfileManager( &pIWMProfileMgr );
        if( FAILED( hr ) )
        {
            break;
        }

        hr = pIWMProfileMgr->CreateEmptyProfile( WMT_VER_7_0, &pIWMProfile );
        if( FAILED( hr ) )
        {
            break;
        }

        hr = pIWMProfile->SetName( L"Custom Profile" );
        if( FAILED( hr ) )
        {
            break;
        }

#ifdef _SET_PROFILE_DESCRIPTION_
        hr = pIWMProfile->SetDescription( L"Custom Profile Description" );
        if( FAILED( hr ) )
        {
            break;
        }
#endif

        DWORD dwFlags = 0;
        if( cVideoStreams )
        {
            dwFlags |= AUDIO_FLAGS_AUDIOVIDEO;
        }

        hr = AddAudioStream( pAudioParams->dwFormatTag, 
                             pAudioParams->dwBitrate, 
                             pAudioParams->dwSampleRate, 
                             pAudioParams->dwChannels, 
                             dwFlags,
                             pIWMProfile,
                             pIWMProfileMgr );
        if( FAILED( hr ) )
        {
            break;
        }

        if( cVideoStreams > 0)
        {
            for( WORD i = 0; ( i < cVideoStreams ) && SUCCEEDED( hr ); i++ )
            {
                hr = AddVideoStream( pIWMProfile, 
                                     pIWMProfileMgr,
                                     BASE_STREAM_NUMBER_VIDEO + i,
                                     prgVideoParams[i].dwFourCC,
                                     prgVideoParams[i].dwBitrate,
                                     prgVideoParams[i].dwWidth,
                                     prgVideoParams[i].dwHeight,
                                     prgVideoParams[i].dwFPS,
                                     prgVideoParams[i].dwQuality,
                                     prgVideoParams[i].dwSecPerKey );
            }
            if( FAILED( hr ) )
            {
                break;
            }

            if( cVideoStreams > 1 )
            {
                hr = AddMutexObject( pIWMProfile, cVideoStreams );
                if( FAILED( hr ) )
                {
                    break;
                }
            }
        }

        if( pScriptParams )
        {
            hr = AddScriptStream( pIWMProfile, 
                                  BASE_STREAM_NUMBER_VIDEO + cVideoStreams,
                                  pScriptParams->dwBitrate );
            if( FAILED( hr ) )
            {
                break;
            }
        }
    }
    while( FALSE );

    if( SUCCEEDED( hr ) )
    {
        *ppIWMProfile = pIWMProfile;
    }
    else
    {
        SAFE_RELEASE( pIWMProfile );
    }
    
    SAFE_RELEASE( pIWMProfileMgr );

    return( hr );
}
