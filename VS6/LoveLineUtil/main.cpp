
#include "pch.h"
#include <windows.h>
#include <strmif.h>
#include <uuids.h>
#include <control.h>


#define XDEFINE_GUID(a, b, c, d, e, f, g, h, i, j, k, l) \
    const GUID a =  \
    { b, c, d, e, f, g, h, i, j, k, l };

XDEFINE_GUID(CLSID_FilterGraph,0xe436ebb3,0x524f,0x11ce,0x9f,0x53,0x00,0x20,0xaf,0x0b,0xa7,0x70)
XDEFINE_GUID(IID_IMediaControl,0x56a868b1,0x0ad4,0x11ce,0xb0,0x3a,0x00,0x20,0xaf,0x0b,0xa7,0x70)
XDEFINE_GUID(IID_IMediaEvent,0x56a868b6,0x0ad4,0x11ce,0xb0,0x3a,0x00,0x20,0xaf,0x0b,0xa7,0x70)
XDEFINE_GUID(IID_IMediaEventEx,0x56a868c0,0x0ad4,0x11ce,0xb0,0x3a,0x00,0x20,0xaf,0x0b,0xa7,0x70)
XDEFINE_GUID(IID_IMediaPosition,0x56a868b2,0x0ad4,0x11ce,0xb0,0x3a,0x00,0x20,0xaf,0x0b,0xa7,0x70)
XDEFINE_GUID(IID_IBasicAudio,0x56a868b3,0x0ad4,0x11ce,0xb0,0x3a,0x00,0x20,0xaf,0x0b,0xa7,0x70)
XDEFINE_GUID(FORMAT_WaveFormatEx,0x05589f81, 0xc356, 0x11ce, 0xbf, 0x01, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a)
XDEFINE_GUID(IID_IMediaSeeking,  0x36b73880, 0xc2c8, 0x11cf, 0x8b, 0x46, 0x00, 0x80, 0x5f, 0x6c, 0xef, 0x60)
XDEFINE_GUID(IID_IDirectSoundNotify, 0xb0210783, 0x89cd, 0x11d0, 0xaf, 0x8, 0x0, 0xa0, 0xc9, 0x25, 0xcd, 0x16)
XDEFINE_GUID(TIME_FORMAT_MEDIA_TIME, 0x7b785574,0x8c82,0x11cf,0xbc,0x0c,0x00,0xaa,0x00,0xac,0x74,0xf6)
XDEFINE_GUID(IID_IBaseFilter, 0x56a86895, 0x0ad4, 0x11ce, 0xb0, 0x3a, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
XDEFINE_GUID(IID_IGraphBuilder, 0x56a868a9, 0x0ad4, 0x11ce, 0xb0, 0x3a, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)

HINSTANCE g_hInstance;


BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, PVOID)
{
    switch(dwReason)
    {
    case DLL_PROCESS_ATTACH:
        {
            g_hInstance = hInstance;
            break;
        }
    }
    return TRUE;
}


