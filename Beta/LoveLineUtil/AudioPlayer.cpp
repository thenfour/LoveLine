

#include "AudioPlayer.h"
#include <UUIDS.h>// CLSID_FilterGraph


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
    CloseHandle(this->hNotificationThread);
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
    DWORD dwId;

    this->hExitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    this->hNotificationThread = CreateThread(NULL, 0, AudioPlayer::NotificationProc,
        this, 0, &dwId);

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


DWORD WINAPI AudioPlayer::NotificationProc(PVOID dwUser)
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
        return -1;
    }

    if(FAILED(hr = pEvent->GetEventHandle((OAEVENT*)&hEvents[1])))
    {
        odsf(L"AudioPlayer::NotificationProc", L"Could not get event handle");
        return -1;
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
                return 0;
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
                    case EC_SYSTEMBASE: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_SYSTEMBASE"); break;}
                    case EC_USER: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_USER"); break;}
                    case EC_COMPLETE:
                        {
                            pThis->bPlaying = TRUE;
                            pThis->bUseLastPosition = TRUE;
                            pThis->GetLength(&pThis->dwLastPosition, 0);
                            odsf(L"AudioPlayer::NotificationProc", L"Event: EC_COMPLETE");
                            pThis->_SendMessage(CCM_COMPLETE);
                            break;
                        }
                    case EC_USERABORT: { odsf(L"AudioPlayer::NotificationProc", L"Event: EC_USERABORT"); break;}
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
                }

                break;
            }
        }// switch(dwResult)
    }// while(1)

    return 0;
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

