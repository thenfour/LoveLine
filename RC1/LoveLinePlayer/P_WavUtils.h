

#pragma once


#include <Windows.h>
#include "..\\LoveLineUtil\\CCCustomWnd.h"
#include "LLPlugin.h"
#include "StandardPlugins.h"
#include "LLPlayerCore.h"


class Plugin_WavUtils
{
public:

    Plugin_WavUtils();
    ~Plugin_WavUtils();

    static LPARAM CALLBACK WndProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static void CALLBACK NotificationProc(PVOID pUser, DWORD dwMsg, PVOID pParams);

    HRESULT Create();
    HRESULT Destroy();
    HRESULT Refresh();

private:

    LLPlayerCore* pCore;
    StandardPlugins* pPlugins;

    CCCustomWnd cMainWindow;
};




