

#include "stdafx.h"
#include "P_Log.h"


Plugin_Log::Plugin_Log()
{
    this->pCore = NULL;
    this->pPlugins = NULL;
}


Plugin_Log::~Plugin_Log()
{
}


LPARAM CALLBACK Plugin_Log::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Plugin_Log* pThis = (Plugin_Log*)GetWindowLongPtr(hWnd, GWL_USERDATA);

    switch(uMsg)
    {
    case WM_COMMAND:
        {
            WORD uID = LOWORD(wParam);

            if(uID == pThis->m_cClear)
            {
                pThis->m_cLog.SetText(L"");
                return 0;
            }

            break;
        }
    case WM_SIZE:
        {
            pThis->m_cLog.Size();
            pThis->m_cClear.Size();
            return 0;
        }
    case WM_CREATE:
        {
            CREATESTRUCT* pcs = (CREATESTRUCT*)lParam;
            pThis = (Plugin_Log*)pcs->lpCreateParams;
            SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)pThis);
            return 0;
        }
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


void CALLBACK Plugin_Log::NotificationProc(PVOID pUser, DWORD dwMsg, PVOID pParams)
{
    Plugin_Log* pThis = (Plugin_Log*)pUser;
    switch(dwMsg)
    {
    case LL_DBUPDATE:
        {
            break;
        }
    case LL_SEEKEPISODE:
        {
            break;
        }
    case LL_ATTACH:
        {
            LL_ATTACH_STRUCT* pas = (LL_ATTACH_STRUCT*)pParams;
            StandardPlugins* psp = (StandardPlugins*)pas->pUserData;
            pThis = (Plugin_Log*)psp->pLog;

            pThis->pPlugins = psp;
            pThis->pCore = pas->pCore;

            pThis->Create();

            pas->hWnd = pThis->cMainWindow;
            pas->pUserData = pThis;

            break;
        }
    case LL_INITIALIZE:
        {
            pThis->pCore->Log.SetWindow(pThis->m_cLog);
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


HRESULT Plugin_Log::Create()
{
    RECT orect, prect;
    SetRect(&prect, 0, 0, 100, 100);
    SetRect(&orect, 0, 0, 0, 0);
    cMainWindow.Create(L"", Plugin_Log::WndProc, GetModuleHandle(0), prect, orect, this);

    SetRect(&prect, 0, 0, 100, 100);
    SetRect(&orect, 0, 0, 0, -29);
    m_cLog.CreateMultiline(cMainWindow, GetModuleHandle(0), prect, orect, true);
    m_cLog.SetFont(L"Courier", 10);

    SetRect(&prect, 0, 100, 0, 100);
    SetRect(&orect, 0, -23, 75, 0);
    m_cClear.Create(L"Clear", cMainWindow, GetModuleHandle(0), prect, orect);

    return S_OK;
}


HRESULT Plugin_Log::Destroy()
{
    return S_OK;
}


HRESULT Plugin_Log::Refresh()
{
    return S_OK;
}
