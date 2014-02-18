//  Microsoft Windows Media Technologies
//  Copyright (C) Microsoft Corporation, 1999 - 2001.  All rights reserved.
//

#if !defined(__SAMPLE_GENPROFILE_H)
#define __SAMPLE_GENPROFILE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


///////////////////////////////////////////////////////////////////////////////
#include "wmsdk.h"


///////////////////////////////////////////////////////////////////////////////
#define CODEC_AUDIO_MSAUDIO                 353   // Microsoft WMAudio
#define CODEC_VIDEO_ACELP                   304   // Sipro.net ACELP

#define CODEC_VIDEO_MPEG43                  MAKEFOURCC( 'M', 'P', '4', '3' )
#define CODEC_VIDEO_WMV1                    MAKEFOURCC( 'W', 'M', 'V', '1' )

#define DEFAULT_SCRIPT_STREAM_BANDWIDTH     2048

struct AUDIO_PARAMS
{
    DWORD dwBitrate;

    DWORD dwFormatTag;

    DWORD dwSampleRate;
    DWORD dwChannels;
};

struct VIDEO_PARAMS
{
    DWORD dwFourCC;

    DWORD dwBitrate;

    DWORD dwHeight;
    DWORD dwWidth;

    DWORD dwFPS;
    DWORD dwQuality;
    DWORD dwSecPerKey;
};

struct SCRIPT_PARAMS
{
    DWORD dwBitrate;
};


///////////////////////////////////////////////////////////////////////////////
STDMETHODIMP GenerateProfile( AUDIO_PARAMS *pAudioParams,
                              SCRIPT_PARAMS *pScriptParams,
                              WORD cVideoStreams,
                              VIDEO_PARAMS *prgVideoParams,
                              IWMProfile **ppIWMProfile );

#endif // __SAMPLE_GENPROFILE_H                              

