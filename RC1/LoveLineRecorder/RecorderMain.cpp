

#include <windows.h>
#include <CommCtrl.h>
#include "LLApp.h"
#include "..\\LoveLineUtil\\global.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR wszCmd, int)
{
    LLApp* pApp = NULL;
    INITCOMMONCONTROLSEX iccx;

    CoInitialize(0);

    iccx.dwSize = sizeof(iccx);
    iccx.dwICC = ICC_PROGRESS_CLASS;
    InitCommonControlsEx(&iccx);

    pApp = new LLApp;

    pApp->Entry(hInstance, wszCmd);

    delete pApp;

    CoUninitialize();

    return 0;
}
