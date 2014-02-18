

#include "pch.h"
#include "AudioReader.h"
#include "uuids.h"


#pragma comment(lib, "wmvcore.lib")
//#pragma comment(lib, "wmstub.lib")



AudioReader::AudioReader()
{
    m_dwTimeout = 3000;
    this->pAdv = 0;
    this->hEventOpened = 0;
    this->pReader = 0;
    this->hEventOnTime = 0;
}


AudioReader::~AudioReader()
{
    this->Close(NULL);
}


void AudioReader::SetTimeout(DWORD dwMS)
{
    m_dwTimeout = dwMS;
}


HRESULT AudioReader::Open(CCString FileName, WAVEFORMATEX* pFormat, CCLog* pLog)
{
    HRESULT hr;

    this->Close(pLog);

    if(FAILED(hr = WMCreateReader(NULL, 0, &this->pReader)))
    {
        pLog->AddEntry(L"Could not create a Windows Media reader object.");
        pLog->AddEntry(hr, L"WMCreateReader");
        return E_FAIL;
    }

    if(FAILED(hr = this->pReader->QueryInterface(IID_IWMReaderAdvanced, (PVOID*)&this->pAdv)))
    {
        pLog->AddEntry(L"Error working with Windows Media objects.");
        pLog->AddEntry(hr, L"QI for IWMReaderAdvanced");
        SAFE_RELEASE(this->pReader);
        return E_FAIL;
    }

    // Read() calls are also asynch. so this is for that.
    this->hEventOnTime = CreateEvent(NULL, FALSE, FALSE, NULL);

    // Because the Open() call is asynchronous, create an event
    // that will be signaled when the open is actually finished.
    this->hEventOpened = CreateEvent(NULL, FALSE, FALSE, NULL);

    if(FAILED(hr = this->pReader->Open(FileName, &this->Callback, (PVOID)this)))
    {
        pLog->AddEntry(L"Could not open the specified Windows Media file.  Only Windows Media or MPEG files are supported.");
        pLog->AddEntry(hr, L"IWMReader::Open");
        this->Close(pLog);
        return E_FAIL;
    }

    /*
        The previous call completes immediately, so we have to wait for it to
        be opened completely to get the wave format stuff.
    */
    if(WAIT_TIMEOUT == WaitForSingleObject(this->hEventOpened, m_dwTimeout))
    {
        pLog->AddEntry(L"Operation timed out while initializing audio reader.");
        this->Close(pLog);
        return E_FAIL;
    }

    if(FAILED(this->hrAsync))
    {
        pLog->AddEntry(L"Could not open the specified Windows Media file.  Only Windows Media or MPEG files are supported.");
        pLog->AddEntry(hr, L"WMT_OPENED");
        this->Close(pLog);
        return E_FAIL;
    }

    // Set the clock so we can grab samples faster than real-time.
    if(FAILED(hr = this->pAdv->SetUserProvidedClock(TRUE)))
    {
        pLog->AddEntry(L"Could not specify Windows Media properties.");
        pLog->AddEntry(hr, L"IWMReaderAdvanced::SetUserProvidedClock");
        this->Close(pLog);
        return E_FAIL;
    }

    // Now we are free to grab the output format.
    //DWORD dwFormats;
    //if(FAILED(this->pReader->GetOutputFormatCount(0, &dwFormats)))
    //{
    //    this->Close(pLog);
    //}

    IWMOutputMediaProps* pmp = NULL;
    WM_MEDIA_TYPE* mt = NULL;
    WAVEFORMATEX* wfx = NULL;
    DWORD dwSize = 0;

    if(FAILED(hr = this->pReader->GetOutputProps(0, &pmp)))
    {
        pLog->AddEntry(L"Couldn't get the output format.");
        pLog->AddEntry(hr, L"IWMReader::GetOutputProps");
        this->Close(pLog);
        return E_FAIL;
    }

    if(FAILED(hr = pmp->GetMediaType(NULL, &dwSize)))
    {
        pLog->AddEntry(L"Couldn't get the output format.");
        pLog->AddEntry(hr, L"IWMMediaProps::GetMediaType");
        SAFE_RELEASE(pmp);
        this->Close(pLog);
        return E_FAIL;
    }

    mt = (WM_MEDIA_TYPE*)malloc(dwSize);
    
    if(FAILED(hr = pmp->GetMediaType(mt, &dwSize)))
    {
        pLog->AddEntry(L"Couldn't get the output format.");
        pLog->AddEntry(hr, L"IWMMediaProps::GetMediaType");
        SAFE_RELEASE(pmp);
        free(mt);
        this->Close(pLog);
        return E_FAIL;
    }

    if(mt->formattype != FORMAT_WaveFormatEx)
    {
        pLog->AddEntry(L"Media format is unsupported.");
        SAFE_RELEASE(pmp);
        free(mt);
        this->Close(pLog);
        return E_FAIL;
    }

    wfx = (WAVEFORMATEX*)mt->pbFormat;
    memcpy(pFormat, wfx, sizeof(WAVEFORMATEX));

    free(mt);
    SAFE_RELEASE(pmp);

    // Set the reader to read.
    this->qwCursor = 0;
    this->bAsyncEOF = FALSE;

    if(FAILED(hr = this->pReader->Start(0, 0, 1.0, 0)))
    {
        pLog->AddEntry(L"Error reading from the file.");
        pLog->AddEntry(L"IWMReader::Start()");
        this->Close(pLog);
        return E_FAIL;
    }

    return S_OK;
}


HRESULT AudioReader::Read(DWORD dwMS, CCLog* pLog)
{
    HRESULT hr;

    QWORD qwSampleDuration = 10000 * dwMS;
    QWORD qwGoal = this->qwCursor + qwSampleDuration;

    ResetEvent(this->hEventOnTime);

    /*
        this->qwCursor will be set by the callback function.
    */

    IWMReaderStreamClock* pClock = 0;
    pAdv->QueryInterface(IID_IWMReaderStreamClock, (void**)&pClock);
    if(pClock)
    {
        QWORD current;
        pClock->GetTime(&current);
        DWORD dw = (DWORD)(current / 10000);
        dw = dw;
    }

    if(FAILED(hr = this->pAdv->DeliverTime(qwGoal)))
    {
        pLog->AddEntry(L"Could not deliver a destination time to the Windows Media reader object.");
        pLog->AddEntry(hr, L"IWMReaderAdvanced::DeliverTime");
        return E_FAIL;
    }

    if(WAIT_TIMEOUT == WaitForSingleObject(this->hEventOnTime, m_dwTimeout))
    {
        pLog->AddEntry(L"Operation timed out while reading audio data.");
        return E_FAIL;
    }

    if(FAILED(this->hrAsync))
    {
        pLog->AddEntry(L"Could not read from the Windows Media reader object.");
        pLog->AddEntry(hr, L"OnSample() returned");
        return E_FAIL;
    }

    if(this->bAsyncEOF == TRUE)
    {
        return ERROR_END_OF_MEDIA;
    }

    return S_OK;
}


HRESULT AudioReader::Unlock()
{
    this->Buffer.UnlockBuffer();
    return S_OK;
}


HRESULT AudioReader::LockAndFlush(DWORD* pdwSize, PBYTE* ppBuffer, CCLog* pLog)
{
    *ppBuffer = this->Buffer.LockBuffer();
    *pdwSize = this->Buffer.GetLength();

    this->Buffer.Clear();// this doesnt' clear any data... it just sets the "good" portion to 0...

    return S_OK;
}


//HRESULT AudioReader::Read(DWORD dwMS, DWORD* dwSize, PBYTE* ppBuffer, CCLog* pLog)
//{
//    HRESULT hr;
//
//    QWORD qwSampleDuration = 10000 * dwMS;
//    QWORD qwGoal = this->qwCursor + qwSampleDuration;
//
//    ResetEvent(this->hEventOnTime);
//
//    this->Buffer.Clear();
//
//    /*
//        this->qwCursor will be set by the callback function.
//    */
//
//    if(FAILED(hr = this->pAdv->DeliverTime(qwGoal)))
//    {
//        pLog->AddEntry(L"Could not deliver a destination time to the Windows Media reader object.");
//        pLog->AddEntry(hr, L"IWMReaderAdvanced::DeliverTime");
//        return E_FAIL;
//    }
//
//    if(WAIT_TIMEOUT == WaitForSingleObject(this->hEventOnTime, m_dwTimeout))
//    {
//        pLog->AddEntry(L"Operation timed out while reading audio data.");
//        return E_FAIL;
//    }
//
//    if(FAILED(this->hrAsync))
//    {
//        pLog->AddEntry(L"Could not read from the Windows Media reader object.");
//        pLog->AddEntry(hr, L"OnSample() returned");
//        return E_FAIL;
//    }
//
//    *ppBuffer = this->Buffer.GetBuffer();
//    *dwSize = this->Buffer.GetLength();
//
//    if(this->bAsyncEOF == TRUE)
//    {
//        return ERROR_END_OF_MEDIA;
//    }
//
//    return S_OK;
//}


HRESULT AudioReader::Close(CCLog* pLog)
{
    SAFE_RELEASE(this->pAdv);
    SAFE_RELEASE(this->pReader);

    if(this->hEventOpened)
    {
        CloseHandle(this->hEventOpened);
        this->hEventOpened = NULL;
    }

    if(this->hEventOnTime)
    {
        CloseHandle(this->hEventOnTime);
        this->hEventOnTime = NULL;
    }

    this->bAsyncEOF = FALSE;

    return S_OK;
}





