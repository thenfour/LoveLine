

#pragma once


#include <Windows.h>
#include "..\\LoveLineUtil\\CCStatic.h"
#include "..\\LoveLineUtil\\CCListBox.h"
#include "..\\LoveLineUtil\\CCImageList.h"
#include "..\\LoveLineUtil\\CCEditBox.h"
#include "..\\LoveLineUtil\\CCButton.h"
#include "..\\LoveLineUtil\\CCCustomWnd.h"
#include "LLPlugin.h"
#include "StandardPlugins.h"
#include "LLPlayerCore.h"


class Plugin_Episode
{
public:

    Plugin_Episode();
    ~Plugin_Episode();

    static LPARAM CALLBACK WndProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static void CALLBACK NotificationProc(PVOID pUser, DWORD dwMsg, PVOID pParams);

    HRESULT Create();
    HRESULT Destroy();
    HRESULT Refresh();

private:

    static HRESULT CALLBACK EnumCommercials(LLCommercial* pc, PVOID pUser);
    static HRESULT CALLBACK EnumBookmarks(LLLink* pl, LLFeature* pf, PVOID pUser);

    static int CALLBACK SortCommercialProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
    static int CALLBACK SortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
    int iColumn;
    DWORD dwSortBy;//SORT_IN, SORT_OUT, SORT_LENGTH

    void _OnBrowse();
    void _OnPlayLink();
    void _OnEdit();
    void _OnSave();
    void _OnCancel();
    void _OnRemoveCommercial();
    void _OnRemoveLink();

    void _SetControlsReadOnly();
    void _SetControlsEdit();

    LLPlayerCore* pCore;

    StandardPlugins* pPlugins;

    CCStatic cStaticDate;
    CCStatic cStaticGuests;
    CCStatic cStaticFile;
    CCStatic cStaticDescription;

    CCEditBox cDate;
    CCEditBox cGuests;
    CCEditBox cFile;
    CCEditBox cDescription;

    CCButton cBrowse;
    CCButton cEpisodeInfo;
    CCButton cEdit;
    CCButton cCancel;
    CCButton cSave;
    CCButton cRemoveCommercial;
    CCButton cRemoveLink;

    CCImageList cImages;
    CCListBox cListCommercials;
    CCListBox cListLinks;

    CCCustomWnd cMainWindow;
};




