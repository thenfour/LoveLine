

#pragma once


#include <Windows.h>
#include "..\\LoveLineUtil\\CCStatic.h"
#include "..\\LoveLineUtil\\CCImageList.h"
#include "..\\LoveLineUtil\\CCListBox.h"
#include "..\\LoveLineUtil\\CCEditBox.h"
#include "..\\LoveLineUtil\\CCButton.h"
#include "..\\LoveLineUtil\\CCCustomWnd.h"
#include "LLPlugin.h"
#include "StandardPlugins.h"
#include "LLPlayerCore.h"


class Plugin_Features
{
public:

    Plugin_Features();
    ~Plugin_Features();

    static LPARAM CALLBACK WndProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static void CALLBACK NotificationProc(PVOID pUser, DWORD dwMsg, PVOID pParams);

    HRESULT Create();
    HRESULT Destroy();
    HRESULT Refresh();

private:

    LLPlayerCore* pCore;

    StandardPlugins* pPlugins;

    static int CALLBACK SortEpisodeProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

    static HRESULT CALLBACK EnumFeaturesProc(LLFeature* pFeature, PVOID pUser);
    static HRESULT CALLBACK EnumEpisodesProc(LLLink* pl, LLEpisode* pe, PVOID pUser);
    CCString sidFeatureSelected;
    int nFeatureSelected;

    void OnSelectFeature();
    void OnEditFeature();
    void OnNewFeature();
    void OnRemoveFeature();
    void OnRemoveLink();
    void OnPlay();
    void OnNewNow();
    void OnAddNow();
    void OnSearchChange();

    CCString sSearchString;

    CCButton  cGroupFeatures;
    CCButton  cGroupLinks;

    CCImageList cImages;
    int iEpisodeImage;
    int iFeatureImage;

    CCStatic  cStaticSearch;
    CCEditBox cEditSearch;
    CCListBox cListFeatures;
    CCButton  cNewFeature;
    CCButton  cRemoveFeature;
    CCButton  cEditFeature;
    CCButton  cNewNow;
    CCButton  cAddNow;

    CCEditBox cEditResults;
    CCButton  cPlay;
    CCButton  cRemoveLink;
    CCListBox cListLinks;

    CCCustomWnd cMainWindow;
};




