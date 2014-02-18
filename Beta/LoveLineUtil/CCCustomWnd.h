

#pragma once


#include "CCControl.h"
#include "global.h"


//////////////////////////////////////////////////////////////////////////////
class CCEXPORT CCCustomWnd : public CCControl
{
public:
	CCCustomWnd();
	~CCCustomWnd();

	HRESULT Create(
		PCWSTR Name, WNDPROC WndProc, HINSTANCE hInstance, RECT prect,
		RECT orect, PVOID lparam);

	HRESULT Create(
		PCWSTR Name, WNDPROC WndProc, HINSTANCE hInstance, RECT prect,
		RECT orect, DWORD Style, LPVOID lparam, int iIcon);

	HRESULT SetTitle(PCWSTR Name);
	HRESULT GetTitle(PWSTR Name, int Max);
	HRESULT SetIcon(HICON hIcon);
	HRESULT Destroy();

private:

	WCHAR Class[MAX_PATH];
};


