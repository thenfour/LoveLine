

#include "pch.h"
#include "global.h"
#include "WaveCapture.h"
#include "traceerrorsys.h"
#include <process.h>


#define XDEFINE_GUID(a, b, c, d, e, f, g, h, i, j, k, l) \
    const GUID a =  \
    { b, c, d, e, f, g, h, i, j, k, l };
XDEFINE_GUID(IID_IDirectSoundNotify, 0xb0210783, 0x89cd, 0x11d0, 0xaf, 0x8, 0x0, 0xa0, 0xc9, 0x25, 0xcd, 0x16)


WaveCapture::WaveCapture()
{
    this->pdsb = NULL;
    this->pdsc = NULL;

    this->nPassesLeft = 0;

    this->hEvents[0] = NULL;
    this->hEvents[1] = NULL;
    this->hEvents[2] = NULL;
    this->hEvents[3] = NULL;

    this->hThread = NULL;
    this->pBuffer = NULL;
    this->nBufferSize = 0;

    this->pProc = 0;
    this->nInterval = 0;
    this->pUser = 0;
    this->dwLastPosition = 0;
}


WaveCapture::~WaveCapture()
{
    CCString ErrText;
    this->Uninitialize();
}


/*
    nInterval is in SECONDS.  The buffer we create is 3 times the interval,
    so we make three notifications that will be placed at every interval.

    pProc will be called at approximately the interval specified.
*/
HRESULT WaveCapture::Initialize(CCString& sDevice, WAVEFORMATEX* pFormat, WAVECAPTUREPROC pProc, PVOID pUser, float nInterval, CCLog* pLog)
{
    DSCCAPS dsc;
    DSCBUFFERDESC1 dscbDesc;
    HRESULT hr;
    DSBPOSITIONNOTIFY NotifyPositions[3];
    IDirectSoundNotify* pdsn;

    this->Uninitialize();

    //this->m_sDeviceCriteria = sDevice;

    this->pProc = pProc;
    this->pUser = pUser;
    this->nInterval = nInterval;

    // Validation.
    if(pFormat->wFormatTag != WAVE_FORMAT_PCM)
    {
        pLog->AddEntry(
            L"The wave format specified for recording is invalid. "
            L"Only PCM wave data is supported.");
        return E_FAIL;
    }

    // Copy the wave format for our own purposes.
    memcpy(&this->Format, pFormat, sizeof(WAVEFORMATEX));

    GUID idDevice;
    CCString desc;
    if(!GetMatchingDevice(sDevice, idDevice, desc))
    {
        pLog->AddEntry(L"The device specified could not be found.  Using the default device instead.");
    }

    // Create DirectSoundCapture object.
    if(FAILED(hr = DirectSoundCaptureCreate(&idDevice, &this->pdsc, NULL)))
    {
        pLog->AddEntry(
            L"Could not create DirectSound Capture object. "
            L"Make sure that DirectX 7 or later is installed, "
            L"and make sure your sound card is installed and working properly.");
        pLog->AddEntry(hr, L"DirectSoundCaptureCreate returned");
        return E_FAIL;
    }

    /*
        Here is where we should check the capabilities of the sound card against
        the format that was passed in.
    */
    dsc.dwSize = sizeof(DSCCAPS);
    if(FAILED(hr = this->pdsc->GetCaps(&dsc)))
    {
        pLog->AddEntry(
            L"Could not get information about sound card capabilities.");
        pLog->AddEntry(hr, L"IDirectSoundCapture::GetCaps returned");
        SAFE_RELEASE(this->pdsc);
        return E_FAIL;
    }

    // Determine if the device supports the given format.

    if(!HandleCaps(dsc.dwFormats, 11025,
        WAVE_FORMAT_1M08, WAVE_FORMAT_1S08, WAVE_FORMAT_1M16, WAVE_FORMAT_1S16))
    {
        pLog->AddEntry(L"The requested format is not supported by the sound card.  The output will be undefined.");
    }
    if(!HandleCaps(dsc.dwFormats, 22050,
        WAVE_FORMAT_2M08, WAVE_FORMAT_2S08, WAVE_FORMAT_2M16, WAVE_FORMAT_2S16))
    {
        pLog->AddEntry(L"The requested format is not supported by the sound card.  The output will be undefined.");
    }
    if(!HandleCaps(dsc.dwFormats, 44100,
        WAVE_FORMAT_4M08, WAVE_FORMAT_4S08, WAVE_FORMAT_4M16, WAVE_FORMAT_4S16))
    {
        pLog->AddEntry(L"The requested format is not supported by the sound card.  The output will be undefined.");
    }

    /*
        Create the capture buffer.
    */
    dscbDesc.dwSize = sizeof(dscbDesc);
    dscbDesc.dwFlags = DSCBCAPS_WAVEMAPPED;
    dscbDesc.dwBufferBytes = (DWORD)(((float)pFormat->nAvgBytesPerSec * nInterval) * 3.0);
    dscbDesc.dwReserved = 0;
    dscbDesc.lpwfxFormat = &this->Format;

    if(FAILED(hr = this->pdsc->CreateCaptureBuffer((DSCBUFFERDESC*)&dscbDesc, &this->pdsb, NULL)))
    {
        pLog->AddEntry(
            L"Could not allocate memory for sound capture.");
        pLog->AddEntry(hr, L"IDirectSoundCapture::CreateCaptureBuffer returned");
        SAFE_RELEASE(this->pdsc);
        return E_FAIL;
    }

    /*
        Create the notifications
    */
    this->hEvents[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
    this->hEvents[1] = CreateEvent(NULL, FALSE, FALSE, NULL);
    this->hEvents[2] = CreateEvent(NULL, FALSE, FALSE, NULL);
    this->hEvents[3] = CreateEvent(NULL, FALSE, FALSE, NULL);

    NotifyPositions[0].dwOffset = 0;
    NotifyPositions[0].hEventNotify = this->hEvents[0];

    NotifyPositions[1].dwOffset = (DWORD)((float)this->Format.nAvgBytesPerSec * nInterval);
    NotifyPositions[1].hEventNotify = this->hEvents[1];

    NotifyPositions[2].dwOffset = (DWORD)(((float)this->Format.nAvgBytesPerSec * nInterval) * 2.0);
    NotifyPositions[2].hEventNotify = this->hEvents[2];

    if(FAILED(hr = this->pdsb->QueryInterface(IID_IDirectSoundNotify, (PVOID*)&pdsn)))
    {
        pLog->AddEntry(
            L"Unexpected error creating DirectSound notifications.");
        pLog->AddEntry(hr, L"QI IDirectSoundNotify returned");
        SAFE_RELEASE(this->pdsb);
        SAFE_RELEASE(this->pdsc);
        return E_FAIL;
    }

    if(FAILED(hr = pdsn->SetNotificationPositions(3, NotifyPositions)))
    {
        pLog->AddEntry(
            L"Unexpected error creating DirectSound notifications.");
        pLog->AddEntry(hr, L"IDirectSoundNotify::SetNotificationPositions returned");
        SAFE_RELEASE(pdsn);
        SAFE_RELEASE(this->pdsb);
        SAFE_RELEASE(this->pdsc);
        return E_FAIL;
    }

    SAFE_RELEASE(pdsn);

    /*
        Create our own buffer so when we pass data to pProc, the data is ALWAYS
        contiguous.
    */
    this->nBufferSize = (DWORD)(((float)pFormat->nAvgBytesPerSec * nInterval) * 3.0);
    this->pBuffer = (PBYTE)malloc(this->nBufferSize);

    /*
        Initialize our own information.
    */
    this->dwLastPosition = 0;
    this->nPassesLeft = 1;

    /*
        Now create our recording thread so life can continue.
    */
    UINT dwThreadId;
	this->hThread = (HANDLE)_beginthreadex(
        0, 0, WaveCapture::RecordingThread, this, 0, &dwThreadId);

    return S_OK;
}


bool WaveCapture::HandleCaps(DWORD dwFormats, int nSamplesPerSecond, DWORD dw8m, DWORD dw8s, DWORD dw16m, DWORD dw16s)
{
    bool r = true;

    if(MatchesFormat(1, nSamplesPerSecond, 8))
    {
        if(!(dwFormats & dw8m))
        {
            r = false;
        }
    }
    if(MatchesFormat(1, nSamplesPerSecond, 16))
    {
        if(!(dwFormats & dw16m))
        {
            r = false;
        }
    }
    if(MatchesFormat(2, nSamplesPerSecond, 8))
    {
        if(!(dwFormats & dw8s))
        {
            r = false;
        }
    }
    if(MatchesFormat(2, nSamplesPerSecond, 16))
    {
        if(!(dwFormats & dw16s))
        {
            r = false;
        }
    }

    return r;
}


bool WaveCapture::MatchesFormat(int nChannels, int nSamplesPerSecond, int nBits)
{
    bool r = false;

    if(nChannels == Format.nChannels)
    {
        if(nSamplesPerSecond == Format.nSamplesPerSec)
        {
            if(nBits == Format.wBitsPerSample)
            {
                r = true;
            }
        }
    }

    return r;
}


HRESULT WaveCapture::Uninitialize()
{
    if(this->pdsc == NULL) return S_OK;
    CCLog l;

    this->Stop(&l);

    /*
        Signal to our thread that we should quit and wait for it.
    */
    if(this->hThread)
    {
        SetEvent(this->hEvents[3]);
        WaitForSingleObject(this->hThread, INFINITE);
    }

    this->hThread = NULL;

    // Release everything
    if(this->pdsb)
    {
        this->pdsb->Release();
        this->pdsb = NULL;
    }

    if(this->pdsc)
    {
        this->pdsc->Release();
        this->pdsc = NULL;
    }

    if(this->pBuffer)
    {
        free(this->pBuffer);
        this->pBuffer = 0;
    }

    // Close the handles
    if(this->hEvents[0])
    {
        CloseHandle(this->hEvents[0]);
    }
    if(this->hEvents[1])
    {
        CloseHandle(this->hEvents[1]);
    }
    if(this->hEvents[2])
    {
        CloseHandle(this->hEvents[2]);
    }
    if(this->hEvents[3])
    {
        CloseHandle(this->hEvents[3]);
    }

    return S_OK;
}


HRESULT WaveCapture::Start(CCLog* pLog)
{
    HRESULT hr;

    if(this->pdsb == NULL) return E_FAIL;

    if(FAILED(hr = this->pdsb->Start(DSCBSTART_LOOPING)))
    {
        pLog->AddEntry(L"Error starting sound capture.");
        pLog->AddEntry(hr, L"IDirectSoundCaptureBuffer::Start");
        return E_FAIL;
    }

    return S_OK;
}


HRESULT WaveCapture::Stop(CCLog* pLog)
{
    HRESULT hr;

    // We'll return a Success if its an invalid object.
    if(this->pdsb == NULL) return S_OK;

    if(FAILED(hr = this->pdsb->Stop()))
    {
        pLog->AddEntry(L"Error while stopping sound capture.");
        pLog->AddEntry(hr, L"IDirectSoundCaptureBuffer::Stop");
        return E_FAIL;
    }

    /*
        Now we're most likely not exactly on a notification point in the buffer,
        so we'll "flush" the buffers.
    */
    this->Flush(pLog);

    return S_OK;
}


/*
    This is the recording thread.  It's created when we initialize everything.
    It's responsibility is to monitor the events and make sure that they are handled.

    This means either calling pProc with valid data for the first 3 events, or
    exiting when hEvents[3] is signaled.
*/
UINT _stdcall WaveCapture::RecordingThread(PVOID pThisx)
{
    CoInitialize(0);

    WaveCapture* pThis = (WaveCapture*)pThisx;
    DWORD dw;
    CCLog l;

    while(1)
    {
        dw = WaitForMultipleObjects(4, pThis->hEvents, FALSE, INFINITE) - WAIT_OBJECT_0;

        switch(dw)
        {
        case 0:
        case 1:
        case 2:
            /*
                We have entered a new notification place in the buffer.
                We need to initialize our own buffer with contiguous data and call the
                user proc.
            */
            if(pThis->nPassesLeft == 0)
            {
                pThis->Flush(&l);
            }
            else
            {
                pThis->nPassesLeft --;
            }
            break;
        case 3:
            // We need to exit!
            CoUninitialize();
            return 0;
        }
    }

    // No man's land:
    return 0;
}


/*
    Writes any NEW FRESH data to pProc.  It's possible that pProc won't be called
    in this function (for instance, if capturing is STOPPED and Flush() was called
    more than once in a row.)
*/
HRESULT WaveCapture::Flush(CCLog* pLog)
{
    DWORD dwLockBytes;
    DWORD dwBufferPos;
    HRESULT hr;
    PBYTE pAudio1, pAudio2;
    DWORD dwAudio1, dwAudio2;
    
    if(this->pdsb == NULL)
    {
        return E_FAIL;
    }

    if(FAILED(hr = this->pdsb->GetCurrentPosition(NULL, &dwBufferPos)))
    {
        pLog->AddEntry(L"Error while getting information about audio memory.");
        pLog->AddEntry(hr, L"IDirectSoundCaptureBuffer::GetCurrentPosition");
        return E_FAIL;
    }

    // First determine how many bytes we need to lock.  We may not need to read anything.
    if(dwBufferPos >= this->dwLastPosition)
    {
        dwLockBytes = dwBufferPos - this->dwLastPosition;
    }
    else
    {
        dwLockBytes = dwBufferPos + (this->nBufferSize - this->dwLastPosition);
    }

    if(dwLockBytes == 0)
    {
        return S_OK;
    }

    // Now lock the buffer
    if(FAILED(hr = this->pdsb->Lock(this->dwLastPosition, dwLockBytes,
        (PVOID*)&pAudio1, &dwAudio1, (PVOID*)&pAudio2, &dwAudio2, 0)))
    {
        pLog->AddEntry(L"Error while freezing audio data to be copied.");
        pLog->AddEntry(hr, L"IDirectSoundCaptureBuffer::Lock");
        return E_FAIL;
    }

    // Let's copy data into our own buffer.
    memcpy(this->pBuffer, pAudio1, dwAudio1);
    memcpy(this->pBuffer + dwAudio1, pAudio2, dwAudio2);

    this->pdsb->Unlock(pAudio1, dwAudio1, pAudio2, dwAudio2);

    // Call the user function
    this->pProc(this->pBuffer, dwLockBytes, this->pUser);

    // And update our own cursor.
    this->dwLastPosition = (this->dwLastPosition + dwLockBytes) % this->nBufferSize;

    return S_OK;
}



BOOL CALLBACK WaveCapture::DSEnumCallback(LPGUID lpGuid, LPCWSTR lpcstrDescription, LPCWSTR lpcstrModule, LPVOID lpContext)
{
    WaveCapture* pThis = (WaveCapture*)lpContext;

    if(0 == pThis->m_nResult)
    {
        pThis->m_sDeviceResult = lpcstrDescription;
    }

    CCString s;
    s = lpcstrDescription;
    if(s.FindI(pThis->m_sDeviceCriteria, 0) != -1)
    {
        memcpy(&pThis->m_idDeviceResult, lpGuid, sizeof(GUID));
        pThis->m_sDeviceResult = lpcstrDescription;
    }
    else
    {
        s = lpcstrModule;
        if(s.FindI(pThis->m_sDeviceCriteria, 0) != -1)
        {
            memcpy(&pThis->m_idDeviceResult, lpGuid, sizeof(GUID));
            pThis->m_sDeviceResult = lpcstrDescription;
        }
        else
        {
            // check guid
            if(lpGuid)
            {
                GUID sRequest;
                pThis->m_sDeviceCriteria.ToGuid(&sRequest);
                if(memcmp(&sRequest, lpGuid, sizeof(GUID)) == 0)
                {
                    memcpy(&pThis->m_idDeviceResult, lpGuid, sizeof(GUID));
                    pThis->m_sDeviceResult = lpcstrDescription;
                }
                else
                {
                    // no match at all.
                }
            }
        }
    }

    pThis->m_nResult ++;

    return TRUE;
}

static const GUID __DSDEVID_DefaultCapture =
    { 0xdef00001, 0x9c6d, 0x47ed,
        { 0xaa, 0xf1, 0x4d, 0xda, 0x8f, 0x2b, 0x5c, 0x03 }
    };


bool WaveCapture::GetMatchingDevice(CCString& s, GUID& id, CCString& desc)
{
    bool r = false;
    m_sDeviceCriteria = s;
    m_nResult = 0;
    memcpy(&m_idDeviceResult, &__DSDEVID_DefaultCapture, sizeof(GUID));

    if(0 != s.len())
    {
        DirectSoundCaptureEnumerate(WaveCapture::DSEnumCallback, this);
        r = true;
    }

    memcpy(&id, &m_idDeviceResult, sizeof(GUID));
    desc = m_sDeviceResult;

    return r;
}





