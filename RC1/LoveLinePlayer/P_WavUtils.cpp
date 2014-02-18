

#include "P_WavUtils.h"


Plugin_WavUtils::Plugin_WavUtils()
{
    this->pCore = NULL;
    this->pPlugins = NULL;
}


Plugin_WavUtils::~Plugin_WavUtils()
{
}


LPARAM CALLBACK Plugin_WavUtils::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Plugin_WavUtils* pThis = (Plugin_WavUtils*)GetWindowLongPtr(hWnd, GWL_USERDATA);

    switch(uMsg)
    {
    case WM_CREATE:
        {
            CREATESTRUCT* pcs = (CREATESTRUCT*)lParam;
            pThis = (Plugin_WavUtils*)pcs->lpCreateParams;
            SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)pThis);
            return 0;
        }
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


void CALLBACK Plugin_WavUtils::NotificationProc(PVOID pUser, DWORD dwMsg, PVOID pParams)
{
    Plugin_WavUtils* pThis = (Plugin_WavUtils*)pUser;
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
            pThis = (Plugin_WavUtils*)psp->pWavUtils;

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


HRESULT Plugin_WavUtils::Create()
{
    return S_OK;
}


HRESULT Plugin_WavUtils::Destroy()
{
    return S_OK;
}


HRESULT Plugin_WavUtils::Refresh()
{
    return S_OK;
}
