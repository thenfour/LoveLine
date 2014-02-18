

#pragma once


#include <Windows.h>
#include "..\\LoveLineUtil\\CCCustomWnd.h"
#include "..\\LoveLineUtil\\CCListBox.h"
#include "..\\LoveLineUtil\\CCButton.h"
#include "LLPlugin.h"
#include "StandardPlugins.h"
#include "LLPlayerCore.h"


class Plugin_Commercial
{
public:

    Plugin_Commercial();
    ~Plugin_Commercial();

    static LPARAM CALLBACK WndProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static void CALLBACK NotificationProc(PVOID pUser, DWORD dwMsg, PVOID pParams);

    HRESULT Create();
    HRESULT Destroy();
    HRESULT Refresh();

private:

    void OnAdd();
    void OnRemove();
    void OnEdit();

    LLPlayerCore* pCore;
    StandardPlugins* pPlugins;

    CCButton cAdd;
    CCButton cRemove;
    CCButton cEdit;
    CCListBox cRanges;
    CCImageList cImages;
    CCCustomWnd cMainWindow;
};




