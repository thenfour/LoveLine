

#include <Windows.h>
#include "LLPlayerApp.h"
#include "resource.h"
#include "StandardPlugins.h"
#include "..\\LoveLineUtil\\LLUtils.h"


LLPlayerApp::LLPlayerApp()
{
    this->hInstance = NULL;
    this->pPlugins = NULL;
}


LLPlayerApp::~LLPlayerApp()
{
    // Destroy the plugins.
    LL_PLUGIN_INFORMATION* p = this->pPlugins;
    LL_PLUGIN_INFORMATION* pTemp = NULL;

    while(p)
    {
        pTemp = p->pNext;
        delete p;
        p = pTemp;
    }

    this->pPlugins = NULL;
}


HRESULT LLPlayerApp::Close()
{
    /*
        Save window position.
    */
    WINDOWPLACEMENT wp;
    wp.length = sizeof(wp);
    GetWindowPlacement(this->MainWindow, &wp);
    CCString Value;

    Value.FromBinary(&wp, sizeof(wp));
    this->Core.db.SetSetting(L"WindowPlacement", Value, &this->Core.Log);

    /*
        Let the plugins know they're being destroyed.
    */
    this->SendPluginMessage(LL_DETACH, 0);

    // Close up shop.
    this->Core.Uninitialize();

    this->Tabs.Destroy();
    this->MainWindow.Destroy();

    PostQuitMessage(0);

    return S_OK;
}


HRESULT LLPlayerApp::Entry(HINSTANCE hInstance, PCWSTR wszCmdLine)
{
    /*
        Show the splash screen.
    */
    POINT pStatic;
    pStatic.x = 25;
    pStatic.y = 220;
    this->Splash.Create(pStatic, RGB(255,255,180), IDB_SPLASH, hInstance);

    /*
        Set up our window.
    */

    this->Splash.SetStatusText(L"Initializing Windowing Engine");

    RECT orect;
    RECT prect;

    /*
        Default window positions.
    */
    SetRect(&prect, 0, 0, 0, 0);
    SetRect(&orect, 0, 0, 600, 400);

    this->hInstance = hInstance;

    this->MainWindow.Create(L"LoveLine Player", LLPlayerApp::WndProc,
        hInstance, prect, orect, WS_OVERLAPPEDWINDOW, (PVOID)this, IDI_ICON1);

    this->hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_APPMENU));
    SetMenu(this->MainWindow, this->hMenu);

    // Create our tab control.
    SetRect(&prect, 0, 0, 100, 100);
    SetRect(&orect, 6, 6, -6, -6);
    this->Tabs.Create(this->MainWindow, prect, orect, 0, NULL);

    // Now initialize our plugins
    this->Splash.SetStatusText(L"Initializing Plugins Pass 1");
    this->InitPlugins();

    this->Core.Initialize(this, &this->Splash);
    this->Core.SetPlugins(this->pPlugins);

    this->SendPluginMessage(LL_INITIALIZE, 0);

    this->Splash.SetStatusText(L"Initializing Plugins Pass 2");

    /*
        Now select the correct plugin.
    */
    CCString val;
    GUID id;
    LL_PLUGIN_INFORMATION* pSel = this->pPlugins;

    if(SUCCEEDED(this->Core.db.GetSetting(L"SelectedPlugin", &val, &this->Core.Log)))
    {
        val.ToGuid(&id);
        pSel = this->_GetPluginById(id);
    }

    // Select the appropriate plugin.
    this->Tabs.SelectLParam((DWORD)pSel);
    this->_ActivatePlugin(pSel);

    this->Splash.SetStatusText(L"Closing Splash Window");
    this->Splash.Destroy();

    /*
        Set window placement.
    */
    CCString sWindowPlacement;
    if(SUCCEEDED(this->Core.db.GetSetting(L"WindowPlacement", &sWindowPlacement, &this->Core.Log)))
    {
        WINDOWPLACEMENT wp;
        sWindowPlacement.ToBinary(&wp, sizeof(wp));
        SetWindowPlacement(this->MainWindow, &wp);
    }
    else
    {
        this->MainWindow.Size();
    }
    
    this->MainWindow.AddStyle(WS_VISIBLE);
    SetActiveWindow(this->MainWindow);

    MSG msg;
    while(GetMessage(&msg, 0, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return S_OK;
}


LRESULT CALLBACK LLPlayerApp::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LLPlayerApp* pThis = (LLPlayerApp*)GetWindowLong(hWnd, GWL_USERDATA);

    switch(uMsg)
    {
    case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
            case ID_APPLICATION_EXIT:
                {
                    pThis->Close();
                    return 0;
                }
            case ID_DATABASE_SAVE:
                {
                    pThis->Core.db.Save(&pThis->Core.Log);
                    return 0;
                }
            }
            break;
        }
    case WM_SIZING:
        {
            POINT p;
            p.x = 557;
            p.y = 324;
            return pThis->MainWindow.Sizing(wParam, lParam, &p);
        }
    case WM_SIZE:
        {
            pThis->Tabs.Size();
            break;
        }
    case WM_CLOSE:
        {
            pThis->Close();
            break;
        }
    case WM_CREATE:
        {
            CREATESTRUCT* pcs = (CREATESTRUCT*)lParam;
            SetWindowLong(hWnd, GWL_USERDATA, (LONG)pcs->lpCreateParams);
            pThis = (LLPlayerApp*)pcs->lpCreateParams;
            break;
        }
	case WM_NOTIFY:
		{
			LPNMHDR phdr = (LPNMHDR) lParam;

            if(phdr->idFrom == pThis->Tabs.GetId())
            {
                if(phdr->code == TCN_SELCHANGE)
                {
                    int i = TabCtrl_GetCurSel(pThis->Tabs);
                    DWORD lParam = pThis->Tabs.GetLParam(i);
                    LL_PLUGIN_INFORMATION* pSelected = (LL_PLUGIN_INFORMATION*)lParam;

                    pThis->_ActivatePlugin(pSelected);

                    return 0;
                }
            }
            break;
        }// case WM_NOTIFY:
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


/*
    This will perform all the discovery of the plugins.
    Actual attaching will be done by the Entry() function.
*/
HRESULT LLPlayerApp::InitPlugins()
{
    LLPluginEnumerator(LLPlayerApp::LLPluginEnumProc, this);

    /*
        Now this will attach them if they are enabled.
    */
    LL_PLUGIN_INFORMATION* p = this->pPlugins;
    while(p)
    {
        this->AttachPlugin(p);
        p = p->pNext;
    }

    return S_OK;
}


HRESULT LLPlayerApp::AttachPlugin(LL_PLUGIN_INFORMATION* pInfo)
{
    TAB_ITEM         t;
    LL_ATTACH_STRUCT as;
    RECT             prect;
    RECT             orect;

    pInfo->bEnabled = TRUE;

    // Add a new tab.
    SetRect(&prect, 0, 0, 100, 100);
    SetRect(&orect, 0, 0, 0, 0);

    pInfo->Container.Create(this->Tabs, this->hInstance, prect, orect);
    t.wszTitle = (PCWSTR)pInfo->Name;
    t.dwUser = (DWORD)pInfo;

    this->Tabs.Attach(&pInfo->Container);
    this->Tabs.AddTabs(&t, 1);

    // Now tell the plugin to create itself.
    as.hWnd = NULL;
    as.pUserData = pInfo->pUserAttach;
    as.pCore = &this->Core;

    pInfo->pNotify(pInfo->pUser, LL_ATTACH, &as);

    pInfo->pUser = as.pUserData;

    // Now put that window inside its container by modifying styles and stuff.
    SetWindowLong(as.hWnd, GWL_STYLE, WS_CHILD | WS_VISIBLE);
    SetWindowLong(as.hWnd, GWL_EXSTYLE, 0);
    pInfo->Container.SetChild(as.hWnd);
    SetParent(as.hWnd, pInfo->Container);

    this->Tabs.Size();

    return S_OK;
}


void CALLBACK LLPlayerApp::LLPluginEnumProc(
    PVOID pPrivate, PCWSTR wszTitle, const GUID* pGuid,
    LL_PLUGIN_NOTIFICATION pNotification, PVOID pUser)
{
    LLPlayerApp* pThis = (LLPlayerApp*)pPrivate;
    LL_PLUGIN_INFORMATION* pInfo = NULL;

    pInfo = new LL_PLUGIN_INFORMATION;

    pInfo->bEnabled = TRUE;
    pInfo->id = *pGuid;
    pInfo->Name = wszTitle;
    pInfo->pNotify = pNotification;
    pInfo->pOwner = pThis;
    pInfo->pUser = 0;// This gets filled in after it's been attached (created).
    pInfo->pUserAttach = pUser;

    // Add it to the beginning of the list.
    pInfo->pPrevious = NULL;
    pInfo->pNext = pThis->pPlugins;

    if(pThis->pPlugins)
    {
        pThis->pPlugins->pPrevious = pInfo;
    }

    pThis->pPlugins = pInfo;
}


HRESULT LLPlayerApp::SendPluginMessage(DWORD dwMsg, PVOID pParam)
{
    LL_PLUGIN_INFORMATION* p = this->pPlugins;
    CCString s;

    while(p)
    {
        s.FormatW(L"Initializing %s", (PCWSTR)p->Name);
        this->Splash.SetStatusText(s);
        p->pNotify(p->pUser, dwMsg, pParam);
        p = p->pNext;
    }

    return S_OK;
}


LL_PLUGIN_INFORMATION* LLPlayerApp::_GetPluginById(GUID id)
{
    LL_PLUGIN_INFORMATION* p = this->pPlugins;

    while(p)
    {
        if(memcmp(&id, &p->id, sizeof(GUID)) == 0)
        {
            return p;
        }

        p = p->pNext;
    }

    return NULL;
}


void LLPlayerApp::_ActivatePlugin(LL_PLUGIN_INFORMATION* pSelected)
{
    LL_PLUGIN_INFORMATION* p = this->pPlugins;

    if(pSelected == NULL) return;

    pSelected->Container.Show(TRUE);

    // Send a message to the plugin that it is being selected.
    pSelected->pNotify(pSelected->pUser, LL_SETFOCUS, 0);

    // And save the setting.
    CCString sGuid;
    sGuid.FromGuid(&pSelected->id);
    this->Core.db.SetSetting(L"SelectedPlugin", sGuid, &this->Core.Log);

    while(p)
    {
        if(p != pSelected)
        {
            p->Container.Show(FALSE);
            pSelected->pNotify(pSelected->pUser, LL_KILLFOCUS, 0);
        }

        p = p->pNext;
    }

    return;
}



