/*
    Carl Corcoran
    010309
*/


#pragma once


#include <windows.h>
#include "global.h"
#include "CCString.h"
#include "CCLog.h"


class CCEXPORT WAVReader
{
public:

    WAVReader();
    ~WAVReader();

    HRESULT Open(CCString s, BOOL bCircular, CCLog* pLog);
    HRESULT Close();
    HRESULT GetFormat(WAVEFORMATEX * pFormat);
    HRESULT Read(PVOID pBuf, DWORD dwSize, DWORD* pBr, CCLog* pLog);
    DWORD GetDataSize();

    /*
        Sets the file pointer to the beginning of the data
        so it can be reread.
    */
    HRESULT ReturnToData();

private:

    /*
        If bCircular is FALSE, the wav will play fine.
        If it's TRUE, then the wav will loop over
        and over again.
    */
    BOOL bCircular;
    DWORD dwDataPos;//Place in the file where the data begins (for circular)

    WAVEFORMATEX Format;
    HANDLE hFile;
    DWORD dwSize;//Size of the wave data, in bytes.
};



