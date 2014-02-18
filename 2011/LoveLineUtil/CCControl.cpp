

#include "pch.h"
#include "CCControl.h"


UINT CCControl::NextId=500;


//////////////////////////////////////////////////////////////////////////////
HRESULT CCControl::SetRedraw(BOOL b)
{
    SendMessage(this->hWnd, WM_SETREDRAW, b, 0);
    return S_OK;
}


void CCControl::Show()
{
    ShowWindow(this->hWnd, SW_SHOWNORMAL);
}



//////////////////////////////////////////////////////////////////////////////
HRESULT CCControl::Sizing(WPARAM wParam, LPARAM lParam, LPPOINT pMin)
{
	PRECT pRect = (PRECT)lParam;
	int w = pRect->right - pRect->left;
	int h = pRect->bottom - pRect->top;

	if(w < pMin->x)
    {
		switch(wParam)
        {
		case WMSZ_BOTTOMLEFT:
		case WMSZ_LEFT:
		case WMSZ_TOPLEFT:
			//adjust the left side.
			pRect->left = pRect->right - pMin->x;
			break;

		case WMSZ_BOTTOMRIGHT:
		case WMSZ_RIGHT:
		case WMSZ_TOPRIGHT:
			//adjust the left side.
			pRect->right = pRect->left + pMin->x;
			break;
		}
	}

	if(h < pMin->y)
    {
		switch(wParam)
        {
		case WMSZ_TOPRIGHT:
		case WMSZ_TOP:
		case WMSZ_TOPLEFT:
			//adjust the left side.
			pRect->top = pRect->bottom - pMin->y;
			break;

		case WMSZ_BOTTOMLEFT:
		case WMSZ_BOTTOM:
		case WMSZ_BOTTOMRIGHT:
			//adjust the left side.
			pRect->bottom = pRect->top + pMin->y;
			break;
		}
	}

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////
CCControl::CCControl()
{
	this->hWnd = NULL;
	this->hParent = NULL;
	this->dwStyle = 0;
	this->hFont = NULL;
	ZeroMemory(&this->prect, sizeof(RECT));
	ZeroMemory(&this->orect, sizeof(RECT));

	this->id = CCControl::NextId;
	CCControl::NextId++;

	InitCommonControls();
}



//////////////////////////////////////////////////////////////////////////////
UINT CCControl::GetId()
{
	return this->id;
}



//////////////////////////////////////////////////////////////////////////////
HWND CCControl::GethWnd()
{
	return this->hWnd;
}



//////////////////////////////////////////////////////////////////////////////
HWND CCControl::GetParentHWND()
{
	return this->hParent;
}


HRESULT CCControl::SetFont(HFONT hFont)
{
	if(this->hFont != NULL)
    {
		SendMessage(this->hWnd, WM_SETFONT, 0, FALSE);
		DeleteObject(this->hFont);
		this->hFont = 0;
	}

    SendMessage(this->hWnd, WM_SETFONT, (WPARAM)hFont, TRUE);

    return S_OK;
}


//////////////////////////////////////////////////////////////////////////////
HRESULT CCControl::SetFont(PCWSTR FontName, int Size)
{
	if(this->hFont != NULL)
    {
		SendMessage(this->hWnd, WM_SETFONT, 0, FALSE);
		DeleteObject(this->hFont);
		this->hFont = 0;
	}

    HFONT hFont;

    if(FontName)
    {
        HDC hDC = GetDC(this->hWnd);

        int nHeight = -MulDiv(Size, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	    this->hFont = CreateFontW(
		    nHeight,0,0,0,0,0,0,0,DEFAULT_CHARSET,0,0,0,0,FontName);
        hFont = this->hFont;

        ReleaseDC(this->hWnd, hDC);
    }
    else
    {
        hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
    }

	SendMessage(this->hWnd, WM_SETFONT, (WPARAM)hFont, TRUE);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////////
HRESULT CCControl::Size()
{
	if(this->hWnd != 0)
    {

		RECT clientrect;
		DWORD height, width;

		if(GetClientRect(this->hParent, &clientrect)==0) return E_FAIL;

		RECT newrect;
		this->ControlRectToRect(
			&newrect, &this->prect, &this->orect, &clientrect);

        /*
            If we actually need to size the window, do it.
        */
        int nDiff = 0;
        nDiff += (this->ControlRect.left - newrect.left);
        nDiff += (this->ControlRect.bottom - newrect.bottom);
        nDiff += (this->ControlRect.top - newrect.top);
        nDiff += (this->ControlRect.right - newrect.right);

        if(nDiff == 0)
        {
            return S_OK;
        }

		height = newrect.bottom - newrect.top;
		width = newrect.right - newrect.left;

		CopyRect(&this->ControlRect, &newrect);

		if(MoveWindow(
			this->hWnd, newrect.left, newrect.top, width, height, TRUE)
			==0)
        {
            //__asm int 3;
			return E_FAIL;
		}

	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////////
CCControl::~CCControl()
{
	if(this->hFont)
    {
        DeleteObject(this->hFont);
    }

    if(this->hWnd)
    {
        //DestroyWindow(this->hWnd);
		this->hWnd = 0;

    }
}


/*
    Converts a control's placement to actual positioning.
    Uses the parent's client rect, a percentag-rect, and an offset-rect to
    calculate the new client coords for the control.
*/
HRESULT CCControl::ControlRectToRect(LPRECT pnewrect, LPRECT pprect, 
									 LPRECT porect, LPRECT pclientrect)
{
	pnewrect->top    =
		(int)((((float)pprect->top)/100)    *((float)(pclientrect->bottom - pclientrect->top)));
	pnewrect->left   =
		(int)((((float)pprect->left)/100)   *((float)(pclientrect->right - pclientrect->left)));
	pnewrect->bottom =
		(int)((((float)pprect->bottom)/100) *((float)(pclientrect->bottom - pclientrect->top)));
	pnewrect->right  =
		(int)((((float)pprect->right)/100)  *((float)(pclientrect->right - pclientrect->left)));

	pnewrect->top += porect->top + pclientrect->top;
	pnewrect->left += porect->left + pclientrect->left;
	pnewrect->bottom += porect->bottom +pclientrect->top;
	pnewrect->right += porect->right + pclientrect->left;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////////
HRESULT CCControl::GetPlacement(RECT * prect, RECT * orect)
{
	if(prect)
    {
		CopyRect(prect, &this->prect);
	}

	if(orect)
    {
		CopyRect(orect, &this->orect);
	}

	return S_OK;
}


CCControl::operator HWND()
{
    return this->hWnd;
}


CCControl::operator UINT()
{
    return this->id;
}


void CCControl::Destroy()
{
    if(this->hWnd) DestroyWindow(this->hWnd);
	this->hWnd = 0;
}


DWORD CCControl::GetStyle()
{
    return GetWindowLong(this->hWnd, GWL_STYLE);
}


DWORD CCControl::GetExStyle()
{
    return GetWindowLong(this->hWnd, GWL_EXSTYLE);
}


void CCControl::SetStyle(DWORD dw)
{
    SetWindowLong(this->hWnd, GWL_STYLE, dw);
}


void CCControl::SetExStyle(DWORD dw)
{
    SetWindowLong(this->hWnd, GWL_EXSTYLE, dw);
}


void CCControl::Invalidate()
{
    if(this->hWnd == NULL) return;

    InvalidateRect(this->hWnd, NULL, FALSE);
}


void CCControl::RemoveStyle(DWORD s)
{
    DWORD dw = this->GetStyle();
    dw &= ~s;
    this->SetStyle(dw);
}


void CCControl::RemoveExStyle(DWORD s)
{
    DWORD dw = this->GetExStyle();
    dw &= ~s;
    this->SetExStyle(dw);
}


void CCControl::AddExStyle(DWORD s)
{
    this->SetExStyle(this->GetExStyle() | s);
}


void CCControl::AddStyle(DWORD s)
{
    this->SetStyle(this->GetStyle() | s);
}


void CCControl::SetColorKey(COLORREF rgb)
{
    this->AddExStyle(WS_EX_LAYERED);

    SetLayeredWindowAttributes(this->hWnd, rgb, NULL, LWA_COLORKEY);
}


void CCControl::SetTransparency(BYTE b)
{
    this->AddExStyle(WS_EX_LAYERED);

    SetLayeredWindowAttributes(this->hWnd, NULL, b, LWA_ALPHA);
}


void CCControl::Invalidate(const RECT* rc)
{
    InvalidateRect(this->hWnd, rc, FALSE);
}


void CCControl::SetText(PCWSTR wsz)
{
    SetWindowText(this->hWnd, wsz);
}
