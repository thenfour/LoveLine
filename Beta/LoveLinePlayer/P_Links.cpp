

#include "P_Links.h"
#include "..\\LoveLineUtil\\LLUtils.h"


#define COLUMN_TITLE    1
#define COLUMN_DESCRIPTION 2
#define COLUMN_TIME 0
#define COLUMN_ID 3


Plugin_Links::Plugin_Links()
{
    this->pPlugins = NULL;
    this->pCore = NULL;
}


Plugin_Links::~Plugin_Links()
{
}


LPARAM CALLBACK Plugin_Links::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Plugin_Links* pThis = (Plugin_Links*)GetWindowLongPtr(hWnd, GWL_USERDATA);

    switch(uMsg)
    {
    case WM_NOTIFY:
        {
            break;
        }
    case WM_COMMAND:
        {
            UINT uId = (UINT)((DWORD)wParam & 0xFFFF);
            if(uId == pThis->cPlay)
            {
                // On play.
            }

            break;
        }
    case WM_SIZE:
        {
            pThis->cStaticSearch.Size();
            pThis->cEditSearch.Size();
            pThis->cEditAddNow.Size();
            pThis->cAddNow.Size();
            pThis->cPlay.Size();
            pThis->cEdit.Size();
            pThis->cNewNow.Size();
            pThis->cRemove.Size();
            pThis->cGroupFeatures.Size();
            pThis->cGroupLinks.Size();
            pThis->cListFeatures.Size();
            pThis->cListLinks.Size();
            return 0;
        }
    case WM_CREATE:
        {
            CREATESTRUCT* pcs = (CREATESTRUCT*)lParam;
            pThis = (Plugin_Links*)pcs->lpCreateParams;
            SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)pThis);
            return 0;
        }
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


void CALLBACK Plugin_Links::NotificationProc(PVOID pUser, DWORD dwMsg, PVOID pParams)
{
    Plugin_Links* pThis = (Plugin_Links*)pUser;

    switch(dwMsg)
    {
    case LL_SETFOCUS:
        {
            break;
        }
    case LL_INITIALIZE:
        {
            break;
        }
    case LL_DBUPDATE:
        {
            break;
        }
    case LL_ATTACH:
        {
            LL_ATTACH_STRUCT* pas = (LL_ATTACH_STRUCT*)pParams;
            StandardPlugins* psp = (StandardPlugins*)pas->pUserData;
            pThis = (Plugin_Links*)psp->pLinks;

            pThis->pPlugins = psp;
            pThis->pCore = pas->pCore;

            pThis->Create();

            pas->hWnd = pThis->cMainWindow;
            pas->pUserData = pThis;

            break;
        }
    case LL_DETACH:
        {
            pThis->Destroy();
            break;
        }
    }

    return;
}


HRESULT Plugin_Links::Create()
{
    HINSTANCE hInstance;
    RECT orect;
    RECT prect;
    LIST_COLUMN c[10];

    hInstance = GetModuleHandle(NULL);

    SetRect(&prect, 0, 0, 100, 100);
    SetRect(&orect, 0, 0, 0, 0);

    this->cMainWindow.Create(L"Plugin_Links",
        Plugin_Links::WndProc, hInstance, prect, orect, this);

    // The "Features" Group box
    SetRect(&prect, 0, 0, 50, 100);
    SetRect(&orect, 6, 6, -3, -6);
    this->cGroupFeatures.CreateGroup(
        L"Features", this->cMainWindow, hInstance, prect, orect);

    // The "Links" group box
    SetRect(&prect, 50, 0, 100, 100);
    SetRect(&orect, 3, 6, -6, -6);
    this->cGroupLinks.CreateGroup(
        L"Links", this->cMainWindow, hInstance, prect, orect);

    // "Search" static box
    SetRect(&prect, 0, 0, 0, 0);
    SetRect(&orect, 15, 26, 72, 42);
    this->cStaticSearch.Create(L"Search", this->cMainWindow, hInstance, prect, orect);

    // The "Search" edit box
    SetRect(&prect, 0, 0, 50, 0);
    SetRect(&orect, 72, 21, -12, 42);
    this->cEditSearch.Create(this->cMainWindow, hInstance, prect, orect);
    this->cEditSearch.SetReadOnly(FALSE);

    // Features ListView
    SetRect(&prect, 0, 0, 50, 100);
    SetRect(&orect, 15, 48, -12, -42);
    this->cListFeatures.Create(this->cMainWindow, hInstance, prect, orect);

    c[0].Set(L"Name", LIST_LEFT, 100);
    c[1].Set(L"Description", LIST_LEFT, 100);
    c[2].Set(L"ID", LIST_LEFT, 100);
    this->cListFeatures.ShowHeader(3, c);

    this->cListFeatures.SetStyle(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_CHILD | WS_VISIBLE);
    this->cListFeatures.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);

    // Add Now Button
    SetRect(&prect, 50, 100, 50, 100);
    SetRect(&orect, -87, -36, -12, -15);
    this->cAddNow.Create(L"Add Now", this->cMainWindow, hInstance, prect, orect);

    // The "Results" edit box
    SetRect(&prect, 0, 100, 50, 100);
    SetRect(&orect, 15, -36, -93, -15);
    this->cEditAddNow.Create(this->cMainWindow, hInstance, prect, orect);

    // Links ListView
    SetRect(&prect, 50, 0, 100, 100);
    SetRect(&orect, 12, 21, -15, -69);
    this->cListLinks.Create(this->cMainWindow, hInstance, prect, orect);

    c[COLUMN_TITLE].Set(L"Title", LIST_LEFT, 100);
    c[COLUMN_DESCRIPTION].Set(L"Description", LIST_LEFT, 100);
    c[COLUMN_TIME].Set(L"Time", LIST_LEFT, 100);
    c[COLUMN_ID].Set(L"ID", LIST_LEFT, 100);
    this->cListLinks.ShowHeader(4, c);

    this->cListLinks.SetStyle(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_CHILD | WS_VISIBLE);
    this->cListLinks.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);

    // New Now, Play, Edit, Remove buttons
    SetRect(&prect, 100, 100, 100, 100);
    SetRect(&orect, -90, -63, -15, -42);
    this->cNewNow.Create(L"New Now", this->cMainWindow, hInstance, prect, orect);

    SetRect(&prect, 100, 100, 100, 100);
    SetRect(&orect, -171, -63, -96, -42);
    this->cPlay.Create(L"Play", this->cMainWindow, hInstance, prect, orect);

    SetRect(&prect, 100, 100, 100, 100);
    SetRect(&orect, -171, -36, -96, -15);
    this->cEdit.Create(L"Edit", this->cMainWindow, hInstance, prect, orect);

    SetRect(&prect, 100, 100, 100, 100);
    SetRect(&orect, -90, -36, -15, -15);
    this->cRemove.Create(L"Remove", this->cMainWindow, hInstance, prect, orect);

    return S_OK;
}


HRESULT Plugin_Links::Destroy()
{
    this->cStaticSearch.Destroy();
    this->cEditSearch.Destroy();
    this->cEditAddNow.Destroy();
    this->cAddNow.Destroy();
    this->cPlay.Destroy();
    this->cEdit.Destroy();
    this->cNewNow.Destroy();
    this->cRemove.Destroy();
    this->cGroupFeatures.Destroy();
    this->cGroupLinks.Destroy();
    this->cListFeatures.Destroy();
    this->cListLinks.Destroy();
    this->cMainWindow.Destroy();

    return S_OK;
}


HRESULT Plugin_Links::Refresh()
{
    return S_OK;
}

