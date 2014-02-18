

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
