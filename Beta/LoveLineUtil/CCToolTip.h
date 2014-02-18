/*
	CREATED: Nov. 3, 2001 Carl Corcoran
*/


#pragma once


#include <Windows.h>
#include <commctrl.h>
#include "CCString.h"
#include "global.h"


class CCEXPORT CCToolTip
{
public:

    CCToolTip();
    ~CCToolTip();

    HRESULT Create(HINSTANCE hInstance, HWND hParent);
    HRESULT AddTool(HWND hWnd, CCString Text, LPARAM lParam);

private:

    HWND hOwner;
    HWND hWnd;

};


