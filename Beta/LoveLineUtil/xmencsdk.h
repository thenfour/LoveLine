

#pragma once


#define XING_ALGORITHM_MP3   0x00000003


#include <windows.h>


// {477a3788-2d4d-11d3-b244-444553540000}
static const GUID IID_IMP3Encoder3 = 
{ 0x477a3788, 0x2d4d, 0x11d3, { 0xb2, 0x44, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00 } };

// {477A3782-2D4D-11D3-B244-444553540000}
static const GUID IID_IEncodeMP3Mem2 = 
{ 0x477a3782, 0x2d4d, 0x11d3, { 0xb2, 0x44, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00 } };

// {477A3784-2D4D-11D3-B244-444553540000}
static const GUID IID_IQueryWavFile2 = 
{ 0x477a3784, 0x2d4d, 0x11d3, { 0xb2, 0x44, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00 } };

// {477A3786-2D4D-11D3-B244-444553540000}
static const GUID IID_IQueryMPEGAudioFile2 = 
{ 0x477a3786, 0x2d4d, 0x11d3, { 0xb2, 0x44, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00 } };

// {477A3783-2D4D-11D3-B244-444553540000}
static const GUID CLSID_EncodeMP3Mem2 = 
{ 0x477a3783, 0x2d4d, 0x11d3, { 0xb2, 0x44, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00 } };

// {477A3785-2D4D-11D3-B244-444553540000}
static const GUID CLSID_QueryWavFile2 = 
{ 0x477a3785, 0x2d4d, 0x11d3, { 0xb2, 0x44, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00 } };

// {477A3787-2D4D-11D3-B244-444553540000}
static const GUID CLSID_QueryMPEGAudioFile2 = 
{ 0x477a3787, 0x2d4d, 0x11d3, { 0xb2, 0x44, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00 } };

// {477A3789-2D4D-11D3-B244-444553540000}
static const GUID CLSID_MP3Encoder3 = 
{ 0x477a3789, 0x2d4d, 0x11d3, { 0xb2, 0x44, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00 } };


/*
    This is what's passed into InitAuthentication().
*/
class XingKey
{
public:
    XingKey();
    ~XingKey();

    operator PBYTE();// So you can just call InitAuthentication(key);

private:
    BOOL bHaveKey;
    BOOL bGeneratedKey;

    BYTE pSrcKey[16];
    BYTE pDestKey[16];
};







