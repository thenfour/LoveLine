

#pragma once


#include <windows.h>
#include <commctrl.h>
#include "global.h"


//////////////////////////////////////////////////////////////////////////////
class CCEXPORT CCControl
{
public:
	CCControl();
	~CCControl();

    operator HWND();
    operator UINT();

    void Destroy();

    /*
        Resizes the control based on the placement rects.
    */
	HRESULT Size();
	HRESULT GetPlacement(RECT* prect, RECT* orect);

    /*
        Sizing() ensures that the window doesn't go out of bounds.

        case WM_SIZING:
	    	POINT p;
	    	p.x = 360;
	    	p.y = 344;
	    	return pThis->cWindow.Sizing(wParam, lParam, &p);
    */
    HRESULT Sizing(WPARAM wParam, LPARAM lParam, LPPOINT pMin);

	UINT GetId();
	HWND GethWnd();
	HWND GetParentHWND();

	WNDPROC Subclass(PVOID pUser, WNDPROC NewProc);

    HRESULT SetRedraw(BOOL);
	HRESULT SetFont(PCWSTR, int);
    HRESULT SetFont(HFONT hFont);

    void SetText(PCWSTR wsz);

    void Invalidate();
    void Invalidate(const RECT* rc);

    void RemoveStyle(DWORD s);
    void AddStyle(DWORD s);
    void RemoveExStyle(DWORD s);
    void AddExStyle(DWORD s);
    DWORD GetStyle();
    DWORD GetExStyle();
    void SetStyle(DWORD dw);
    void SetExStyle(DWORD dw);

    void SetColorKey(COLORREF rgb);
    void SetTransparency(BYTE b);

protected:

	static UINT NextId;

    // Control ID.
	UINT id;

    // This may or may not be valid.
	HFONT hFont;
	HINSTANCE hInstance;

	/*
		the rect that was last used in a call to MoveWindow().
		This is useful for finding out what offset in the client area
		this control is.
	*/
	RECT ControlRect;

	RECT orect;
	RECT prect;

	HWND hWnd;
	HWND hParent;
	DWORD dwStyle;

	HRESULT ControlRectToRect(PRECT, PRECT, PRECT, PRECT);
};


