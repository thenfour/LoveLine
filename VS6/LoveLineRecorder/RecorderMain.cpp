

#include <windows.h>
#include <CommCtrl.h>
#include "LLApp.h"
#include "..\\LoveLineUtil\\global.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR wszCmd, int)
{
    LLApp* pApp = NULL;
    INITCOMMONCONTROLSEX iccx;

    long l;
    WAVEFORMATEX wfx;
    wfx.wBitsPerSample = 16;
    signed __int16 data[5] = { 32767,32768,-32767,-32768,0 };
    CAudioLevels x;
    x.SetFormat(wfx);
    x.SetReturnExtents();
    x.GetLevel((BYTE*)data, 10, &l);

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
