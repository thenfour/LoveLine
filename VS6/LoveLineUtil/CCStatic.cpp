

#include "pch.h"
#include "CCStatic.h"


HRESULT CCStatic::Create(
    CCString Text, HWND hParent, HINSTANCE hInstance, RECT prect, RECT orect)
{
	CopyRect(&this->orect, &orect);
	CopyRect(&this->prect, &prect);

	this->hParent = hParent;
	this->dwStyle = WS_CHILD;

    this->hWnd = CreateWindowExW(
		0, L"STATIC", Text, this->dwStyle, 0, 0, 1, 1,
		hParent, (HMENU)this->GetId(), hInstance, 0);

	this->Size();

	SendMessage(
		this->hWnd, WM_SETFONT,(WPARAM) GetStockObject(DEFAULT_GUI_FONT), 0);

    Show();

	return S_OK;
}