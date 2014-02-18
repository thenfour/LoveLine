

#pragma once


#include <Windows.h>
#include "..\\LoveLineUtil\\CCStatic.h"
#include "..\\LoveLineUtil\\CCListBox.h"
#include "..\\LoveLineUtil\\CCEditBox.h"
#include "..\\LoveLineUtil\\CCButton.h"
#include "..\\LoveLineUtil\\CCCustomWnd.h"
#include "LLPlugin.h"
#include "StandardPlugins.h"
#include "LLPlayerCore.h"


class Plugin_Links
{
public:

    Plugin_Links();
    ~Plugin_Links();

    static LPARAM CALLBACK WndProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static void CALLBACK NotificationProc(PVOID pUser, DWORD dwMsg, PVOID pParams);

    HRESULT Create();
    HRESULT Destroy();
    HRESULT Refresh();

private:

    LLPlayerCore* pCore;

    StandardPlugins* pPlugins;

    CCStatic cStaticSearch;
    CCEditBox cEditSearch;
    CCEditBox cEditAddNow;
    CCButton cAddNow;
    CCButton cPlay;
    CCButton cEdit;
    CCButton cNewNow;
    CCButton cRemove;
    CCButton cGroupFeatures;
    CCButton cGroupLinks;

    CCListBox cListFeatures;
    CCListBox cListLinks;

    CCCustomWnd cMainWindow;
};




