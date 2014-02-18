

#pragma once


#include <Windows.h>
#include <wtypes.h>
#include "wmsdk.h"
#include "CCstring.h"
#include "CCLog.h"


#ifndef CCEXPORT
#define CCEXPORT __declspec(dllexport)
#endif


class CCEXPORT WMAWriter
{
public:
    WMAWriter();
    ~WMAWriter();

    HRESULT Open(WAVEFORMATEX* pFormat, PCWSTR wszFileName, int nBitRate, BOOL bLive, CCLog* pLog);
    HRESULT Close(CCLog* pLog);

    HRESULT WriteBits(PVOID pData, DWORD dwSize, CCLog* pLog);

private:

    HRESULT SetOutputFormat(WAVEFORMATEX* pFormat, int nBitRate, CCLog* pLog);

    BOOL bLive;

    int nBytesPerSecond;
    DWORD dwAudioTime;// Milliseconds of current write operation.
    IWMWriter* pWriter;
    IWMWriterAdvanced* pAdv;
};

