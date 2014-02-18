/*
    AudioPlayer---------------------------
    This class plays audio by abstracting the DirectShow interfaces and stuff.

    Nov. 3, 2001
*/



#pragma once


/*
    This message is sent when the audio file is done.  Then the handler
    can move on to the next file or someething.
*/
#define CCM_COMPLETE    WM_APP + 682

/*
    This is sent when the audio is either playing or paused.
*/
#define CMM_PLAY        WM_APP + 683

/*
    This is sent when the audio STOPS.  not on a pause command.
*/
#define CMM_STOP        WM_APP + 684


#include <Windows.h>
#include "Global.h"
#include "CCLog.h"
#include "CCString.h"

#include <mmsystem.h>
#include <control.h>
#include <Strmif.h>// AM_SEEKING_XxxXXxxxxxxx
#include <Evcode.h>


class CCEXPORT AudioPlayer
{
public:

    AudioPlayer();
    ~AudioPlayer();

    HRESULT Seek(DWORD dwMS, CCLog* pLog);
    HRESULT GetLength(DWORD* pdwMS, CCLog* pLog);
    HRESULT GetPos(DWORD* pdwMS, CCLog* pLog);

    BOOL    IsPlaying();

    HRESULT Stop(CCLog* pLog);
    HRESULT Play(CCLog* pLog);

    HRESULT SetNotificationWindow(HWND hWnd);
    HRESULT Open(CCString FileName, CCLog* pLog);
    HRESULT Close();

    HRESULT SetVolume(DWORD dwVol, CCLog* pLog);// 0-10000
    HRESULT GetVolume(DWORD* pdwVol, CCLog* pLog);// 0-10000
    HRESULT SetBalance(int nBal, CCLog* pLog);// -10000 - 10000
    HRESULT GetBalance(int* pnBal, CCLog* pLog);// -10000 - 10000

private:

    /*
        This is TRUE when the player is *THEORETICALLY* playing.
        In other words, when the media completes by itself, this is still
        TRUE so that when the user seeks to a different part of the
        media, it will automatically be playing.
    */
    BOOL bPlaying;
    BOOL bUseLastPosition;// TRUE when we should look to the next member for position data.
    DWORD dwLastPosition;// When bPlaying is TRUE, we need to store the theoretic position.
    DWORD dwVolume;

    void _SendMessage(UINT uMsg);

    static DWORD WINAPI NotificationProc(PVOID dwUser);
    HANDLE hExitEvent;
    HANDLE hNotificationThread;

    HWND hWnd;
    IMediaSeeking* pSeeking;
    IBasicAudio* pAudio;
    IMediaControl* pControl;

};


