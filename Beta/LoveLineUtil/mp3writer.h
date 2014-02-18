

#pragma once


#include <Windows.h>
#include <wtypes.h>
#include "xmencsdk.h"
#include "xmencmp3.h"
#include "CCstring.h"
#include "CCLog.h"


class CCEXPORT MP3Writer
{
public:
    MP3Writer();
    ~MP3Writer();

    HRESULT Open(WAVEFORMATEX* pFormat, PCWSTR wszFileName, int nBitRate, BOOL bLive, CCLog* pLog);
    HRESULT Close(CCLog* pLog);

    HRESULT WriteBits(PVOID pData, DWORD dwSize, CCLog* pLog);

private:

    int nMinBytes;// Minimum size of bytes until an EncodeSamples() call.

    int nLastSize;
    BOOL bPending;// When pBuffer needs to be written later.
    int nBufferSize;// Size of the buffer, bytes
    PBYTE pBuffer;

    IMP3Encoder3* pEnc;
    HANDLE hFile;

};

