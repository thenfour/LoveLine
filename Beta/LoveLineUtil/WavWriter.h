

#pragma once


#include <windows.h>
#include "global.h"
#include "CCstring.h"
#include "CCLog.h"


class CCEXPORT WavWriter
{
public:
    WavWriter();
    ~WavWriter();

    HRESULT Open(WAVEFORMATEX* pFormat, PCWSTR wszFileName, CCLog* pLog);
    HRESULT Close(CCLog* pLog);

    HRESULT WriteBits(PVOID pData, DWORD dwSize, CCLog* pLog);

private:

    HRESULT _WriteFile(PVOID pData, DWORD dwSize, CCLog* pLog);

    DWORD dwWavSize;// Size of the wave data.  This needs to be saved so when we
                    // Close the file we can fill in some header information correctly.
    DWORD dwRIFFSizeOffset;// Offset where we need to write the RIFF chunk size.
    DWORD dwDATASizeOffset;// offset where we need to write the data chunk size.
    HANDLE hFile;

};



