

#include <windows.h>
#include "..\\LoveLineUtil\\global.h"
#include "..\\LoveLineUtil\\LLDatabase.h"
#include "..\\LoveLineUtil\\AudioPlayer.h"
#include "LLPlayerApp.h"

#include "..\\LoveLineUtil\\llutils.h"

HINSTANCE g_hInstance;


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR wszCmd, int)
{
    g_hInstance = hInstance;
    LLPlayerApp* pApp = NULL;

    CoInitialize(0);

    pApp = new LLPlayerApp;

    pApp->Entry(hInstance, wszCmd);

    delete pApp;

    CoUninitialize();

    return 0;
}