

#pragma once


#include <Windows.h>
#include "..\\LoveLineUtil\\global.h"
#include "..\\LoveLineUtil\\CCString.h"
#include "..\\LoveLineUtil\\LLSlider.h"
#include "..\\LoveLineUtil\\CCCustomWnd.h"
#include "..\\LoveLineUtil\\CCTab.h"
#include "..\\LoveLineUtil\\Splash.h"
#include "..\\LoveLineUtil\\PluginContainer.h"
#include "LLPlayerCore.h"
#include "LLPlugin.h"
#include "LLPluginInformation.h"

class LLPlayerApp
{
public:

    LLPlayerApp();
    ~LLPlayerApp();

    HRESULT Entry(HINSTANCE hInstance, PCWSTR wszCmdLine);

    /*
        This message gets sent to all plugins
    */
    HRESULT SendPluginMessage(DWORD dwMsg, PVOID pParam);

private:

    LL_PLUGIN_INFORMATION* _GetPluginById(GUID id);
    void _ActivatePlugin(LL_PLUGIN_INFORMATION* p);// Sends the SETFOCUS notification to the plugin

    HINSTANCE hInstance;

    HRESULT Close();

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    // Plugins
    HRESULT InitPlugins();
    HRESULT AttachPlugin(LL_PLUGIN_INFORMATION* pInfo);

    static void CALLBACK LLPluginEnumProc(
         PVOID pPrivate, PCWSTR wszTitle, const GUID* pGuid,
         LL_PLUGIN_NOTIFICATION pNotification, PVOID pUser);

    // The different components (linked-list)
    LL_PLUGIN_INFORMATION* pPlugins;

    LLPlayerCore Core;

    CCTab       Tabs;
    CCCustomWnd MainWindow;
    CCSplash    Splash;
};


