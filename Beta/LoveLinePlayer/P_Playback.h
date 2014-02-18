

#pragma once


#include <Windows.h>
#include "..\\LoveLineUtil\\CCStatic.h"
#include "..\\LoveLineUtil\\LLSlider.h"
#include "..\\LoveLineUtil\\CCButton.h"
#include "..\\LoveLineUtil\\CCCustomWnd.h"
#include "..\\LoveLineUtil\\CCFader.h"
#include "..\\LoveLineUtil\\CCListBox.h"
#include "..\\LoveLineUtil\\CCImageList.h"
#include "LLPlugin.h"
#include "LLNavigation.h"
#include "StandardPlugins.h"
#include "LLPlayerCore.h"


class Plugin_Playback
{
public:

    Plugin_Playback();
    ~Plugin_Playback();

    static LPARAM CALLBACK WndProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static void CALLBACK NotificationProc(PVOID pUser, DWORD dwMsg, PVOID pParams);

    HRESULT Create();
    HRESULT Destroy();
    HRESULT Refresh();

private:

    // This handles notifications
    void NotifySeekCurrent();

    static HRESULT CALLBACK EnumCommercials(LLCommercial* pc, PVOID pUser);
    static HRESULT CALLBACK EnumBookmarks(LLLink* pl, LLFeature* pf, PVOID pUser);

    // Commands
    void OnAddCommercial();

    void UpdateSliderPositionAndLCD();
    CCString sLastLCDText;// For making the display pretty.

    /*
        When the user is dragging stuff other than the cursor,
        this has to be TRUE so that UpdateSliderPositionAndLCD() knows
        that the current position in playback ISNT the cursor.  Thus the
        cursor will just stay put.
    */
    BOOL bIsDragging;

    void StopTimer();
    void StartTimer();

    LLPlayerCore* pCore;
    StandardPlugins* pPlugins;

    LLNavigation Nav;
    BOOL bUseNavSeek;

    CCStatic   cStaticDate;
    CCStatic   cStaticGuests;
    CCButton   cPlay;
    CCButton   cStop;
    CCButton   cBack;
    CCButton   cForward;
    CCButton   cAddCommercial;
    CCStatic   cLCD;
    LLSlider   cSlider;
    CCFader    cVol;

    CCCustomWnd cMainWindow;
};




