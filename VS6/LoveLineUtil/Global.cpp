

#include "pch.h"
#include "Global.h"
#include "CCString.h"
#include <shlwapi.h>


#ifdef _DEBUG

//////////////////////////////////////////////////////////////////////////////
void __cdecl MsgBoxf(PCWSTR wszString, ...)
{
    CCString Buf;
    va_list argptr;

    va_start(argptr, wszString);
    Buf.FormatvW(wszString, argptr);
    va_end(argptr);

    MessageBoxW(0, Buf, 0, MB_OK | MB_ICONASTERISK);
}


//////////////////////////////////////////////////////////////////////////////
void _odsf(PCWSTR wszFunction, PCWSTR wszString, va_list argptr)
{
    CCString s;
    CCString s2;

    s.FormatW(L"[ %-20.20s ]  %s", wszFunction, wszString);
    s2.FormatvW(s, argptr);

    OutputDebugStringW(s2);
    OutputDebugStringW(L"\n");

    return;
}


//////////////////////////////////////////////////////////////////////////////
void __cdecl odsf(PCWSTR wszFunction, PCWSTR wszString, ...)
{
    va_list argptr;

    va_start(argptr, wszString);
    _odsf(wszFunction, wszString, argptr);
    va_end(argptr);
}


/*
	010319 Carl Corcoran
*/
void odsr(PCWSTR wszFunction, PCWSTR wszString, PRECT prc)
{
	WCHAR wszBuf[1024];
	swprintf(wszBuf, L"%s  (%d,%d)-(%d,%d)", wszString,
		prc->left, prc->top, prc->right, prc->bottom);

	odsf(wszFunction, wszBuf);
}

#endif