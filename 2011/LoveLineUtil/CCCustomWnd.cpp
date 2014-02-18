

#include "pch.h"
#include "CCCustomWnd.h"
#include <stdio.h>


//////////////////////////////////////////////////////////////////////////////
CCCustomWnd::~CCCustomWnd()
{
}

//////////////////////////////////////////////////////////////////////////////
CCCustomWnd::CCCustomWnd()
{
	this->Class[0]=0;
}

//////////////////////////////////////////////////////////////////////////////
HRESULT CCCustomWnd::Create(PCWSTR wszName, WNDPROC WndProc, HINSTANCE hInstance, 
						 RECT prect, RECT orect, LPVOID lparam)
{
	return this->Create(
		wszName, WndProc, hInstance, prect, orect,
		0,
        lparam, 0);
}

//////////////////////////////////////////////////////////////////////////////
HRESULT CCCustomWnd::Destroy()
{
	DestroyWindow(this->hWnd);

	UnregisterClassW(this->Class, this->hInstance);
	this->Class[0]=0;
	this->hWnd = 0;

	return 1;
}

//////////////////////////////////////////////////////////////////////////////
HRESULT CCCustomWnd::Create(PCWSTR wszName, WNDPROC WndProc, HINSTANCE hInstance, 
						 RECT prect, RECT orect, DWORD Style, 
						 PVOID lparam, int iIcon)
{
	WNDCLASSW wc;

    this->hInstance  = hInstance;

	CopyRect(&this->orect, &orect);
	CopyRect(&this->prect, &prect);

	this->hParent = GetDesktopWindow();
	this->dwStyle = Style;

 	swprintf(this->Class, L"CCCustomWnd 0x%X", this->GetId());
	wc.cbClsExtra= 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = 0;
	if(iIcon) wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(iIcon));
	wc.hInstance = hInstance;
	wc.lpfnWndProc = (WndProc == 0) ? DefWindowProc : WndProc;
	wc.lpszClassName = this->Class;
	wc.lpszMenuName = 0;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	RegisterClassW(&wc);

	this->hWnd = CreateWindowW(this->Class, wszName, this->dwStyle,
		0, 0, 1, 1,
		(HWND)this->hParent,
		(HMENU)0, hInstance, lparam);

	//this->CCControl::Size();
	//this->CCControl::SetFont(L"Courier New", 14);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////////
HRESULT CCCustomWnd::SetTitle(PCWSTR Name)
{
	return SetWindowTextW(this->GethWnd(), Name);
}


//////////////////////////////////////////////////////////////////////////////
HRESULT CCCustomWnd::GetTitle(PWSTR Name, int Max)
{
	return GetWindowTextW(this->GethWnd(), Name, Max);
}


//////////////////////////////////////////////////////////////////////////////
HRESULT CCCustomWnd::SetIcon(HICON hIcon)
{
	SetClassLong(this->hWnd, GCL_HICON, (LONG)hIcon);
	return S_OK;
}
