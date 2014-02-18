

#pragma once

#define INITGUID
#include <Windows.h>
#include <dsound.h>
#include "CCString.h"
#include "CCLog.h"


#ifndef CCEXPORT
#define CCEXPORT __declspec(dllexport)
#endif


/*
    This is the callback function that will be called each time the capturer
    needs to give data.
*/
typedef HRESULT (CALLBACK* WAVECAPTUREPROC)(PBYTE pData, DWORD dwDataLen, PVOID pUser);


class CCEXPORT WaveCapture
{
public:
    WaveCapture();
    ~WaveCapture();

    HRESULT Initialize(WAVEFORMATEX* pFormat, WAVECAPTUREPROC pProc, PVOID pUser, int nInterval, CCLog* pLog);
    HRESULT Uninitialize();
    HRESULT Start(CCLog* pLog);
    HRESULT Stop(CCLog* pLog);

    HRESULT Flush(CCLog* pLog);// If there is more data, this will call the user proc.

private:

    static DWORD WINAPI RecordingThread(PVOID pThis);

    IDirectSoundCapture* pdsc;
    IDirectSoundCaptureBuffer* pdsb;

    /*
        hEvent[0] - Capture is at beginning of buffer
        hEvent[1] - Capture is at first interval
        hEvent[2] - Capture is at last interval
        hEvent[3] - Uninitialize and exit
    */
    HANDLE hEvents[4];
    HANDLE hThread;
    WAVEFORMATEX Format;

    /*
        When we pass audio from the DS buffer to the user-defined function,
        we have to pass in a complete buffer, so we'll allocate it to the maximum size.
    */
    int nPassesLeft;// How many times have we hit a notification - this gets reset by Initialize().
    DWORD dwLastPosition;// Offset, in bytes, of the DS Buffer where we last grabbed data.
    PBYTE pBuffer;
    int nBufferSize;

    WAVECAPTUREPROC pProc;
    PVOID           pUser;

    int nInterval;// In Seconds.
};

