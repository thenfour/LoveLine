/*
    Nov. 18, 2001
*/


#pragma once


#include <Windows.h>
#include "Global.h"
#include "CCLog.h"
#include "CCString.h"
#include "wmsdk.h"
#include "CAudioReaderCallback.h"
#include "CAudioReaderBuffer.h"
//
//
//extern const GUID CLSID_MPEGStreamSplitter;
//extern const GUID CLSID_MP3Decoder;
//extern const GUID CLSID_DefaultDSoundDevice;
//extern const GUID CLSID_FileSourceAsync;


/*

OPEN:
    Create a reader.
    Open a file.
    Set it to use a user-provided clock.

    Need IWMReaderAdvancedCallback to drive the clock (Calling DeliverTime for each OnTime call)
    Need IWMReaderCallback to grab samples.

READ:
    Send a Deliver time message to the reader.
    On OnTime(), return with the data.

*/

class CCEXPORT AudioReader
{
    friend class CAudioReaderCallback;
public:

    AudioReader();
    ~AudioReader();

    HRESULT Open(CCString FileName, WAVEFORMATEX* pFormat, CCLog* pLog);
    void SetTimeout(DWORD dwMS);

    // The buffer is valid until the next READ operation or until this is deleted.
    //HRESULT Read(DWORD dwMS, DWORD* pdwSize, PBYTE* ppBuffer, CCLog* pLog);
    HRESULT Read(DWORD dwMS, CCLog* pLog);
    HRESULT Close(CCLog* pLog);

    HRESULT LockAndFlush(DWORD* pdwSize, PBYTE* ppBuffer, CCLog* pLog);
    HRESULT Unlock();

private:

    HRESULT hrAsync;// Used for both OnSample and OnOpen
    BOOL bAsyncEOF;// Will be set to TRUE by the callback when the EOF is called.

    HANDLE hEventOpened;

    DWORD m_dwTimeout;

    HANDLE hEventOnTime;
    CAudioReaderBuffer Buffer;// Holds audio data passed in by the NSSSamples.

    QWORD qwCursor;
    CAudioReaderCallback Callback;
    IWMReader* pReader;
    IWMReaderAdvanced* pAdv;
};


