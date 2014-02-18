

#include "pch.h"
#include "AudioPlayer.h"
#include <UUIDS.h>// CLSID_FilterGraph
#include <process.h>


#define SAFE_REMOVE_FILTER(g, f) { if(f){ if(g) { (g)->RemoveFilter((f));  (f)->Release();  (f) = 0;} } }


HRESULT CreateFilterAndAdd(IGraphBuilder* pgb, const GUID& clsid, PCWSTR wszName, IBaseFilter** ppOut);
HRESULT AddMP3Filters(IGraphBuilder* pgb, CCString& FileName, 
                      IBaseFilter** pSource, IBaseFilter** pSplitter, 
                      IBaseFilter** pMP3, IBaseFilter** pOutput);
HRESULT ConnectFilters(IGraphBuilder* pgb, IBaseFilter* pLeft, IBaseFilter* pRight);
HRESULT ConnectMP3Filters(IGraphBuilder* pgb, IBaseFilter* pSource, 
                          IBaseFilter* pSplitter, IBaseFilter* pMP3, 
                          IBaseFilter* pOutput);
IPin* FindPinByDirection(IBaseFilter* pFilter, PIN_DIRECTION Criteria);


//"FriendlyName"="MPEG-I Stream Splitter"
//"CLSID"="{336475D0-942A-11CE-A870-00AA002FEAB5}"
const GUID CLSID_MPEGStreamSplitter = \
    {0x336475D0,0x942A,0x11ce,0xA8,0x70,0x00,0xAA,0x00,0x2F,0xEA,0xB5};

//"FriendlyName"="MPEG Layer-3 Decoder"
//"CLSID"="{38BE3000-DBF4-11D0-860E-00A024CFEF6D}"
const GUID CLSID_MP3Decoder = \
    {0x38BE3000,0xDBF4,0x11D0,0x86,0x0E,0x00,0xA0,0x24,0xCF,0xEF,0x6D};

//"FriendlyName"="Default DirectSound Device"
//"CLSID"="{79376820-07D0-11CF-A24D-0020AFD79767}"
const GUID CLSID_DefaultDSoundDevice = \
    {0x79376820,0x07D0,0x11CF,0xA2,0x4D,0x00,0x20,0xAF,0xD7,0x97,0x67};

//"FriendlyName"="File Source (Async.)"
//"CLSID"="{E436EBB5-524F-11CE-9F53-0020AF0BA770}"
const GUID CLSID_FileSourceAsync = \
    {0xE436EBB5,0x524F,0x11CE,0x9F,0x53,0x00,0x20,0xAF,0x0B,0xA7,0x70};


AudioPlayer::AudioPlayer()
{
    this->bPlaying = FALSE;
    this->bUseLastPosition = FALSE;
    this->hWnd = NULL;
    this->pControl = NULL;
    this->pAudio = NULL;
    this->pSeeking = NULL;
    this->hExitEvent = NULL;
    this->hNotificationThread = NULL;
    this->dwLastPosition = 0;
    this->dwVolume = 10000;
}


AudioPlayer::~AudioPlayer()
{
    this->Close();
}


HRESULT AudioPlayer::Close()
{
    CCLog Log;

    if(this->hExitEvent == NULL) return S_OK;

    BOOL bPlaying = this->bPlaying;
    this->Stop(&Log);
    this->bPlaying = bPlaying;
    this->bUseLastPosition = TRUE;
    this->dwLastPosition = 0;

    // Stop our notification thread.
    SetEvent(this->hExitEvent);
    WaitForSingleObject(this->hNotificationThread, INFINITE);
    //CloseHandle(this->hNotificationThread);
    CloseHandle(this->hExitEvent);

    this->hNotificationThread = NULL;
    this->hExitEvent = NULL;

    SAFE_RELEASE(this->pSeeking);
    SAFE_RELEASE(this->pAudio);
    SAFE_RELEASE(this->pControl);

    return S_OK;
}


HRESULT AudioPlayer::Open(CCString FileName, CCLog* pLog)
{
    HRESULT hr;

    this->Close();

    if(FAILED(hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
        IID_IMediaControl, (PVOID*)&this->pControl)))
    {
        pLog->AddEntry(L"Error working with DShow objects.");
        pLog->AddEntry(hr, L"CoCreateInstance");
        return E_FAIL;
    }

    // Now get IMediaSeeking interface - this is REQUIRED.
    if(FAILED(hr = this->pControl->QueryInterface(IID_IMediaSeeking, (PVOID*)&this->pSeeking)))
    {
        pLog->AddEntry(L"Error working with DShow objects.");
        pLog->AddEntry(hr, L"QI for IID_IMediaSeeking");
        SAFE_RELEASE(this->pControl);
        return E_FAIL;
    }

    // Now get the IBasicAudio interface - this is PREFERRED.
    if(FAILED(hr = this->pControl->QueryInterface(IID_IBasicAudio, (PVOID*)&this->pAudio)))
    {
        pLog->AddEntry(L"DShow player does not support basic audio controls such as volume or balance.");
    }

    if(FAILED(this->AttemptMP3Graph(FileName, pLog)))
    {
        // Render the file now that we know we're good to go.
        if(FAILED(hr = this->pControl->RenderFile(FileName.bstr())))
        {
            pLog->AddEntry(L"Error rendering the file. The file may be corrupt.");
            pLog->AddEntry(hr, L"IMediaControl::RenderFile");
            SAFE_RELEASE(this->pControl);
            SAFE_RELEASE(this->pSeeking);
            SAFE_RELEASE(this->pAudio);
            return E_FAIL;
        }
    }

    // Make sure we can use TIME_FORMAT_MEDIA_TIME for seeking... otherwise
    // We won't know how to seek.
    if(FAILED(hr = this->pSeeking->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME)))
    {
        pLog->AddEntry(L"The audio rendering software does not support seeking using time units.  This is unrecoverable.");
        pLog->AddEntry(hr, L"IMediaSeeking::SetTimeFormat");
        SAFE_RELEASE(this->pControl);
        SAFE_RELEASE(this->pSeeking);
        SAFE_RELEASE(this->pAudio);
        return E_FAIL;
    }

    // Now set up our notifications and begin our notification thread to handle them.

    this->hExitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	this->hNotificationThread = (HANDLE)_beginthread(AudioPlayer::NotificationProc, 0, this);

    //this->hNotificationThread = CreateThread(NULL, 0, AudioPlayer::NotificationProc,
    //    this, 0, &dwId);

    /*
        Make sure that our old properties propogate to this file.
    */
    this->SetVolume(this->dwVolume, pLog);
    if(this->bPlaying == TRUE) this->Play(pLog);

    // We made it this far!  Hooray!

    return S_OK;
}


HRESULT AudioPlayer::Seek(DWORD dwMS, CCLog* pLog)
{
    HRESULT hr;
    LONGLONG qwCurrentPos;

    if(this->pSeeking == NULL)
    {
        //pLog->AddEntry(L"Error attempting to seek on uninitialized objects.");
        return E_FAIL;
    }

    // Determine if we're seeking PAST the end of the file.
    DWORD dwLength;
    this->GetLength(&dwLength, pLog);
    if(dwMS > dwLength) dwMS = dwLength;

    qwCurrentPos = (LONGLONG)dwMS * 10000;

    if(FAILED(hr = this->pSeeking->SetPositions(
        &qwCurrentPos, AM_SEEKING_AbsolutePositioning,
        NULL, AM_SEEKING_NoPositioning)))
    {
        pLog->AddEntry(L"Could not seek in the media file.");
        pLog->AddEntry(hr, L"IMediaSeeking::SetPositions()");
        return E_FAIL;
    }

    /*
        And if we're theoretically playing, start playing again.
    */
    if(this->bPlaying == TRUE)
    {
        this->Play(pLog);
    }

    this->dwLastPosition = dwMS;

    return S_OK;
}


HRESULT AudioPlayer::GetLength(DWORD* pdwMS, CCLog* pLog)
{
    LONGLONG qwDuration;
    HRESULT hr;

    if(pdwMS == NULL)
    {
        //if(pLog) pLog->AddEntry(L"AudioPlayer::GetLength was called with a bad pointer");
        return E_FAIL;
    }

    if(this->pSeeking == NULL) return E_FAIL;

    if(FAILED(hr = this->pSeeking->GetDuration(&qwDuration)))
    {
        if(pLog) pLog->AddEntry(L"Could not get the length of the media.");
        if(pLog) pLog->AddEntry(hr, L"IMediaSeeking::GetDuration");
        return E_FAIL;
    }

    *pdwMS = (DWORD)(qwDuration / 10000);

    return S_OK;
}


HRESULT AudioPlayer::GetPos(DWORD* pdwMS, CCLog* pLog)
{
    LONGLONG qwPos;
    HRESULT hr;

    if(pdwMS == NULL)
    {
        pLog->AddEntry(L"AudioPlayer::GetLength was called with a bad pointer");
        return E_FAIL;
    }

    if(this->bUseLastPosition == TRUE)
    {
        *pdwMS = this->dwLastPosition;
        return S_OK;
    }

    if(this->pSeeking == NULL) return E_FAIL;

    if(FAILED(hr = this->pSeeking->GetCurrentPosition(&qwPos)))
    {
        pLog->AddEntry(L"Could not get the current position in the media.");
        pLog->AddEntry(hr, L"IMediaSeeking::GetCurrentPosition");
        return E_FAIL;
    }

    *pdwMS = (DWORD)(qwPos / 10000);

    return S_OK;
}


HRESULT AudioPlayer::Stop(CCLog* pLog)
{
    HRESULT hr;

    this->GetPos(&this->dwLastPosition, pLog);
    this->bPlaying = FALSE;
    this->bUseLastPosition = TRUE;

    if(this->pControl == NULL)
    {
        return S_OK;
    }

    if(FAILED(hr = this->pControl->Stop()))
    {
        pLog->AddEntry(L"Could not stop the media.");
        pLog->AddEntry(hr, L"IMediaControl::Stop");
        return E_FAIL;
    }

    this->_SendMessage(CMM_STOP);

    return S_OK;
}


HRESULT AudioPlayer::Play(CCLog* pLog)
{
    HRESULT hr;

    this->bPlaying = TRUE;

    if(this->pControl == NULL)
    {
        //pLog->AddEntry(L"Error attempting to operate on uninitialized objects.");
        return E_FAIL;
    }

    if(FAILED(hr = this->pControl->Run()))
    {
        pLog->AddEntry(L"Could not play the media.");
        pLog->AddEntry(hr, L"IMediaControl::Run");
        return E_FAIL;
    }

    this->bUseLastPosition = FALSE;

    this->_SendMessage(CMM_PLAY);

    return S_OK;
}


HRESULT AudioPlayer::SetVolume(DWORD dwVol, CCLog* pLog)
{
    HRESULT hr;

    this->dwVolume = dwVol;

    if(this->pAudio == NULL)
    {
        //pLog->AddEntry(L"Error attempting to operate on uninitialized objects.");
        return S_OK;
    }

    long l = dwVol - 10000;

    if(FAILED(hr = this->pAudio->put_Volume(l)))
    {
        pLog->AddEntry(L"Could not set the volume.");
        pLog->AddEntry(hr, L"put_Volume");
        return E_FAIL;
    }

    return S_OK;
}


HRESULT AudioPlayer::GetVolume(DWORD* pdwVol, CCLog* pLog)
{
    HRESULT hr;

    if(this->bUseLastPosition == TRUE)
    {
        *pdwVol = this->dwVolume;
        return S_OK;
    }

    if(this->pAudio == NULL)
    {
        //pLog->AddEntry(L"Error attempting to operate on uninitialized objects.");
        return S_OK;
    }

    long l;

    if(FAILED(hr = this->pAudio->get_Volume(&l)))
    {
        pLog->AddEntry(L"Could not get the volume.");
        pLog->AddEntry(hr, L"get_Volume");
        return E_FAIL;
    }

    *pdwVol = l + 10000;

    return S_OK;
}


HRESULT AudioPlayer::SetBalance(int nBal, CCLog* pLog)
{
    HRESULT hr;

    if(this->pAudio == NULL)
    {
        //pLog->AddEntry(L"Error attempting to operate on uninitialized objects.");
        return E_FAIL;
    }

    if(FAILED(hr = this->pAudio->put_Balance((long)nBal)))
    {
        pLog->AddEntry(L"Could not set the balance.");
        pLog->AddEntry(hr, L"put_Balance");
        return E_FAIL;
    }

    return S_OK;
}


HRESULT AudioPlayer::GetBalance(int* pnBal, CCLog* pLog)
{
    HRESULT hr;

    if(this->pAudio == NULL)
    {
        //pLog->AddEntry(L"Error attempting to operate on uninitialized objects.");
        return E_FAIL;
    }

    if(FAILED(hr = this->pAudio->get_Balance((long*)pnBal)))
    {
        pLog->AddEntry(L"Could not get the balance.");
        pLog->AddEntry(hr, L"get_Balance");
        return E_FAIL;
    }

    return S_OK;
}


HRESULT AudioPlayer::SetNotificationWindow(HWND hWnd)
{
    this->hWnd = hWnd;
    return S_OK;
}


void AudioPlayer::NotificationProc(PVOID dwUser)
{
    AudioPlayer* pThis = (AudioPlayer*)dwUser;

    IMediaEvent* pEvent = NULL;
    HANDLE hEvents[2];//hEvents[0] == The Exit event.  hEvents[1] means there is a media event.
    HRESULT hr;
    DWORD dwResult;
    long lEvent;
    long lParam1;
    long lParam2;

    if(FAILED(hr = pThis->pControl->QueryInterface(IID_IMediaEvent, (PVOID*)&pEvent)))
    {
        odsf(L"AudioPlayer::NotificationProc", L"Could not obtain an IMediaEvent object");
        return;
    }

    if(FAILED(hr = pEvent->GetEventHandle((OAEVENT*)&hEvents[1])))
    {
        odsf(L"AudioPlayer::NotificationProc", L"Could not get event handle");
        return;
    }

    hEvents[0] = pThis->hExitEvent;

    while(1)
    {
        dwResult = WaitForMultipleObjects(2, hEvents, FALSE, INFINITE) - WAIT_OBJECT_0;

        switch(dwResult)
        {
        case 0:
            {
                // We are supposed to exit.
                SAFE_RELEASE(pEvent);
                return;
            }
        case 1:
            {
                // We have events in the queue.  Let's handle them.
                if(FAILED(hr = pEvent->GetEvent(&lEvent, &lParam1, &lParam2, 0)))
                {
                    odsf(L"AudioPlayer::NotificationProc", L"Could not get event data");
                    break;
                }

                // Handle the event.
                switch(lEvent)
                {
                    //case EC_SYSTEMBASE: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_SYSTEMBASE"); break;}
                    //case EC_USER: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_USER"); break;}
                    case EC_COMPLETE:
                        {
                            pThis->bPlaying = TRUE;
                            pThis->bUseLastPosition = TRUE;
                            pThis->GetLength(&pThis->dwLastPosition, 0);
                            //odsf(L"AudioPlayer::NotificationProc", L"Event: EC_COMPLETE");
                            pThis->_SendMessage(CCM_COMPLETE);
                            break;
                        }
/*                    case EC_USERABORT: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_USERABORT"); break;}
                    case EC_ERRORABORT: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_ERRORABORT"); break;}
                    case EC_TIME: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_TIME"); break;}
                    case EC_REPAINT: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_REPAINT"); break;}
                    case EC_STREAM_ERROR_STOPPED: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_STREAM_ERROR_STOPPED"); break;}
                    case EC_STREAM_ERROR_STILLPLAYING: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_STREAM_ERROR_STILLPLAYING"); break;}
                    case EC_ERROR_STILLPLAYING: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_ERROR_STILLPLAYING"); break;}
                    case EC_PALETTE_CHANGED: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_PALETTE_CHANGED"); break;}
                    case EC_VIDEO_SIZE_CHANGED: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_VIDEO_SIZE_CHANGED"); break;}
                    case EC_QUALITY_CHANGE: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_QUALITY_CHANGE"); break;}
                    case EC_SHUTTING_DOWN: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_SHUTTING_DOWN"); break;}
                    case EC_CLOCK_CHANGED: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_CLOCK_CHANGED"); break;}
                    //case EC_PAUSED: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_PAUSED"); break;}
                    case EC_OPENING_FILE	: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_OPENING_FILE	"); break;}
                    //case EC_BUFFERING_DATA: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_BUFFERING_DATA"); break;}
                    case EC_FULLSCREEN_LOST: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_FULLSCREEN_LOST"); break;}
                    case EC_ACTIVATE: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_ACTIVATE"); break;}
                    case EC_NEED_RESTART: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_NEED_RESTART"); break;}
                    case EC_WINDOW_DESTROYED: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_WINDOW_DESTROYED"); break;}
                    case EC_DISPLAY_CHANGED: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_DISPLAY_CHANGED"); break;}
                    case EC_STARVATION: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_STARVATION"); break;}
                    case EC_OLE_EVENT			: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_OLE_EVENT"); break;}
                    case EC_NOTIFY_WINDOW: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_NOTIFY_WINDOW"); break;}
                    case EC_STREAM_CONTROL_STOPPED	: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_STREAM_CONTROL_STOPPED"); break;}
                    case EC_STREAM_CONTROL_STARTED	: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_STREAM_CONTROL_STARTED"); break;}
                    case EC_END_OF_SEGMENT: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_END_OF_SEGMENT"); break;}
                    case EC_SEGMENT_STARTED: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_SEGMENT_STARTED"); break;}
                    case EC_LENGTH_CHANGED: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_LENGTH_CHANGED"); break;}
                    case EC_TIMECODE_AVAILABLE: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_TIMECODE_AVAILABLE"); break;}
                    case EC_EXTDEVICE_MODE_CHANGE: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_EXTDEVICE_MODE_CHANGE"); break;}
*/
                }

                break;
            }
        }// switch(dwResult)
    }// while(1)

    return;
}


void AudioPlayer::_SendMessage(UINT uMsg)
{
    if(this->hWnd)
    {
        PostMessage(this->hWnd, uMsg, 0, 0);
    }
}


BOOL AudioPlayer::IsPlaying()
{
    return this->bPlaying;
}


/*
    This function will take our graph builder and attempt to create an MP3
    graph to play the file.  If it fails, we'll try the old intelligent
    connect method.  This step is necessary to try to circumvent unexpected
    badly behaved filters that get associated with the MP3 and play a bunch
    of garbage.
*/
HRESULT AudioPlayer::AttemptMP3Graph(CCString& FileName, CCLog* pLog)
{
    IGraphBuilder* pgb = 0;
    IBaseFilter* pSource = 0;
    IBaseFilter* pSplitter = 0;
    IBaseFilter* pMP3 = 0;
    IBaseFilter* pOutput = 0;
    HRESULT hr = E_FAIL;
    CCString s;

    if(FAILED(this->pControl->QueryInterface(IID_IGraphBuilder, (void**)&pgb)))
    {
        return E_FAIL;
    }

    if(SUCCEEDED(AddMP3Filters(pgb, FileName, &pSource, &pSplitter, &pMP3, &pOutput)))
    {
        if(SUCCEEDED(ConnectMP3Filters(pgb, pSource, pSplitter, pMP3, pOutput)))
        {
            hr = S_OK;
        }
    }

    // Release all the stuff.
    if(FAILED(hr))
    {
        SAFE_REMOVE_FILTER(pgb, pSource);
        SAFE_REMOVE_FILTER(pgb, pSplitter);
        SAFE_REMOVE_FILTER(pgb, pMP3);
        SAFE_REMOVE_FILTER(pgb, pOutput);
    }
    SAFE_RELEASE(pSource);
    SAFE_RELEASE(pSplitter);
    SAFE_RELEASE(pMP3);
    SAFE_RELEASE(pOutput);
    SAFE_RELEASE(pgb);

    return hr;
}


HRESULT CreateFilterAndAdd(IGraphBuilder* pgb, const GUID& clsid, PCWSTR wszName, IBaseFilter** ppOut)
{
    CCString Name = wszName;
    HRESULT hr = 0;

    *ppOut = 0;

    if(FAILED(hr = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER,
        IID_IBaseFilter, (void**)ppOut)))
    {
        return E_FAIL;
    }

    if(FAILED(pgb->AddFilter(*ppOut, Name.bstr())))
    {
        (*ppOut)->Release();
        *ppOut = 0;
        return E_FAIL;
    }

    return S_OK;
}


HRESULT AddMP3Filters(IGraphBuilder* pgb, CCString& FileName, 
                      IBaseFilter** pSource, IBaseFilter** pSplitter, 
                      IBaseFilter** pMP3, IBaseFilter** pOutput)
{
    CCString s;
    HRESULT hr = E_FAIL;

    // Set the source!
    s = L"Source";
    if(SUCCEEDED(hr = pgb->AddSourceFilter(FileName.bstr(), s.bstr(), pSource)))
    {
        // Create the other filters and add them to the graph.
        if(SUCCEEDED(CreateFilterAndAdd(pgb, CLSID_MPEGStreamSplitter, L"MPEG Stream Splitter", pSplitter)))
        {
            if(SUCCEEDED(CreateFilterAndAdd(pgb, CLSID_MP3Decoder, L"MP3 decoder", pMP3)))
            {
                if(SUCCEEDED(CreateFilterAndAdd(pgb, CLSID_DefaultDSoundDevice, L"Output", pOutput)))
                {
                    hr = S_OK;
                }
            }
        }
    }

    if(FAILED(hr))
    {
        // Release all the stuff.
        SAFE_REMOVE_FILTER(pgb, *pSource);
        SAFE_REMOVE_FILTER(pgb, *pSplitter);
        SAFE_REMOVE_FILTER(pgb, *pMP3);
        SAFE_REMOVE_FILTER(pgb, *pOutput);
    }

    return hr;
}


HRESULT ConnectMP3Filters(IGraphBuilder* pgb, IBaseFilter* pSource, 
                          IBaseFilter* pSplitter, IBaseFilter* pMP3, 
                          IBaseFilter* pOutput)
{
    HRESULT hr = E_FAIL;

    if(SUCCEEDED(ConnectFilters(pgb, pSource, pSplitter)))
    {
        if(SUCCEEDED(ConnectFilters(pgb, pSplitter, pMP3)))
        {
            if(SUCCEEDED(ConnectFilters(pgb, pMP3, pOutput)))
            {
                hr = S_OK;
            }
        }
    }

    return hr;
}


HRESULT ConnectFilters(IGraphBuilder* pgb, IBaseFilter* pLeft, IBaseFilter* pRight)
{
    IPin* pOutput = 0;
    IPin* pInput = 0;
    HRESULT hr = E_FAIL;

    // Find the LEFT filter's first OUTPUT pin.
    pOutput = FindPinByDirection(pLeft, PINDIR_OUTPUT);
    pInput = FindPinByDirection(pRight, PINDIR_INPUT);

    if(pOutput && pInput)
    {
        if(SUCCEEDED(pgb->Connect(pOutput, pInput)))
        {
            hr = S_OK;
        }
    }

    SAFE_RELEASE(pInput);
    SAFE_RELEASE(pOutput);

    return hr;
}


IPin* FindPinByDirection(IBaseFilter* pFilter, PIN_DIRECTION Criteria)
{
    IPin* pPin = 0;
    IEnumPins* pEnum = 0;
    ULONG ul = 0;
    PIN_DIRECTION TempDir;

    pFilter->EnumPins(&pEnum);
    if(!pEnum) return 0;

    while(1)
    {
        pEnum->Next(1, &pPin, &ul);
        if(ul != 1)
        {
            pPin = 0;
            break;
        }

        // Examine this pin.
        pPin->QueryDirection(&TempDir);
        if(TempDir == Criteria)
        {
            break;
        }

        pPin->Release();
    }

    SAFE_RELEASE(pEnum);

    return pPin;
}

