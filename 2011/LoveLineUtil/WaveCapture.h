

#pragma once


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

    // sDevice can be a GUID, or a DLL or a device description.  Partial matches are OK.
    HRESULT Initialize(CCString& sDevice, WAVEFORMATEX* pFormat, WAVECAPTUREPROC pProc, PVOID pUser, float nInterval, CCLog* pLog);
    HRESULT Uninitialize();
    HRESULT Start(CCLog* pLog);
    HRESULT Stop(CCLog* pLog);

    HRESULT Flush(CCLog* pLog);// If there is more data, this will call the user proc.

    bool GetMatchingDevice(CCString& s, GUID& id, CCString& desc);

private:

    static UINT _stdcall RecordingThread(PVOID pThis);

    bool MatchesFormat(int nChannels, int nSamplesPerSecond, int nBits);
    bool HandleCaps(DWORD dwFormats, int nSamplesPerSecond, DWORD dw8m, DWORD dw8s, DWORD dw16m, DWORD dw16s);

    CCString m_sDeviceCriteria;
    CCString m_sDeviceResult;
    GUID m_idDeviceResult;
    int m_nResult;
    static BOOL CALLBACK DSEnumCallback(LPGUID lpGuid, LPCWSTR lpcstrDescription, LPCWSTR lpcstrModule, LPVOID lpContext);

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

    float nInterval;// In Seconds.
};

