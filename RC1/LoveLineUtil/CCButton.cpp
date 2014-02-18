

#include "CCButton.h"
#include <Windowsx.h>// For GetWindowStyle();


//////////////////////////////////////////////////////////////////////////////
HRESULT CCButton::Click()
{
	return SendMessage(this->hWnd, BM_CLICK, 0, 0);
}


//////////////////////////////////////////////////////////////////////////////
CCButton::CCButton()
{
    this->hBitmap = NULL;
}


//////////////////////////////////////////////////////////////////////////////
CCButton::~CCButton()
{
    if(this->hBitmap)
    {
        DeleteObject(this->hBitmap);
    }
}


//////////////////////////////////////////////////////////////////////////////
HRESULT CCButton::Create(PCWSTR wszText, HWND hParent, HINSTANCE hInstance, 
						 RECT prect, RECT orect)
{
	CopyRect(&this->orect, &orect);
	CopyRect(&this->prect, &prect);

	this->hParent = hParent;
	this->dwStyle = WS_CHILD;
 
	this->hWnd = CreateWindowExW(
		0, L"BUTTON", wszText, this->dwStyle, 0, 0, 1, 1,
		hParent, (HMENU)this->GetId(), hInstance, 0);

	//this->Size();

	SendMessage(
		this->hWnd, WM_SETFONT,(WPARAM) GetStockObject(DEFAULT_GUI_FONT), 0);

    this->AddStyle(WS_VISIBLE);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////////
HRESULT CCButton::CreateGroup(PCWSTR wszText, HWND hParent, 
							  HINSTANCE hInstance, 
							  RECT prect, RECT orect)
{
	CopyRect(&this->orect, &orect);
	CopyRect(&this->prect, &prect);

	this->hParent = hParent;
	this->dwStyle = WS_CHILD | BS_GROUPBOX;
 
	this->hWnd = CreateWindowExW(
		0, L"BUTTON", wszText, this->dwStyle, 0, 0, 1, 1, 
		hParent, (HMENU)this->GetId(), hInstance, 0);

	//this->Size();

	SendMessage(this->hWnd, WM_SETFONT,(WPARAM) GetStockObject(DEFAULT_GUI_FONT), 0);

    this->AddStyle(WS_VISIBLE);

	return S_OK;
}


HRESULT CCButton::SetActive(BOOL b)
{
    DWORD dwStyle = GetWindowStyle(this->hWnd);

    if(b == FALSE)
    {
        this->AddStyle(WS_DISABLED);
    }
    else
    {
        this->RemoveStyle(WS_DISABLED);
    }

    this->Invalidate();

    return S_OK;
}


HRESULT CCButton::SetBitmap(HINSTANCE hInst, UINT uResid)
{
    // First destroy our own stuff.
    if(this->hBitmap)
    {
        DeleteObject(this->hBitmap);
    }

    // Set our state so that we don't have borders and stuff.
    this->AddStyle(BS_BITMAP | BS_FLAT);
    this->RemoveStyle(WS_BORDER | BS_TEXT);

    this->hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(uResid));

    SendMessage(this->hWnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)this->hBitmap);

    return S_OK;
}



