

#pragma once


#include <Windows.h>
#include "LLPlugin.h"
#include "StandardPlugins.h"
#include "LLPlayerCore.h"

#include "..\\LoveLineUtil\\CCStatic.h"
#include "..\\LoveLineUtil\\CCButton.h"
#include "..\\LoveLineUtil\\CCListBox.h"
#include "..\\LoveLineUtil\\CCEditBox.h"
#include "..\\LoveLineUtil\\CCImageList.h"
#include "..\\LoveLineUtil\\CCCustomWnd.h"


class Plugin_Library
{
public:

    Plugin_Library();
    ~Plugin_Library();

    static LPARAM CALLBACK WndProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static void CALLBACK NotificationProc(PVOID pUser, DWORD dwMsg, PVOID pParams);

    HRESULT Create();
    HRESULT Destroy();
    HRESULT Refresh();

private:

    static HRESULT CALLBACK EnumFeaturesProc(LLLink* pl, LLFeature* pf, PVOID pUser);
    static HRESULT CALLBACK EnumEpisodesProc(LLEpisode* pEpisode, PVOID pUser);
    static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
    void OnAddFile();
    void OnRemove();
    void OnRefresh();
    void OnPlay();
    void OnSelect();
    void OnPlayLink();

    /*
        This is all for the initial check for stale or new episodes.
    */
    void ScoutEpisodes();

    BOOL _CheckForNewEpisodes(HWND hWnd);
    void _AddAllEpisodes(HWND hWnd);
    static INT_PTR CALLBACK NewEpisodesProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    CCListBox cNewListEpisodes;

    BOOL _CheckForStaleEpisodes(HWND hWnd);
    void _RemoveAllEpisodes(HWND hWnd);
    static INT_PTR CALLBACK StaleEpisodesProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static HRESULT CALLBACK EnumStaleEpisodesProc(LLEpisode* pEpisode, PVOID pUser);
    CCListBox cStaleListEpisodes;


    LLPlayerCore* pCore;
    StandardPlugins* pPlugins;

    CCImageList cImages;
    int iImageEpisode;
    int iImageLink;

    CCStatic cStaticDate;
    CCStatic cStaticLinks;
    CCStatic cStaticGuests;
    CCStatic cStaticFile;
    CCStatic cStaticDescription;

    CCEditBox cDate;
    CCEditBox cGuests;
    CCEditBox cFile;
    CCEditBox cDescription;

    CCButton cAddFile;
    CCButton cRemove;
    CCButton cPlay;
    CCButton cGroupEpisodes;
    CCButton cGroupEpisodeDetails;

    CCListBox cListEpisodes;
    CCListBox cListLinks;

    CCCustomWnd cMainWindow;
};




