

#pragma once


#include "CCControl.h"
#include "global.h"


//////////////////////////////////////////////////////////////////////////////
class CCEXPORT CCButton : public CCControl
{
public:
	CCButton();
	~CCButton();

	HRESULT Create(PCWSTR, HWND, HINSTANCE, RECT prect, RECT orect);
	HRESULT CreateGroup(PCWSTR, HWND, HINSTANCE, RECT prect, RECT orect);
	HRESULT CreateCheck(PCWSTR Title, HWND hParent, const RECT& prect, const RECT& orect);
	HRESULT Click();
    HRESULT SetBitmap(HINSTANCE hInstance, UINT uResid);
    HRESULT SetActive(BOOL b);

    bool GetCheck();
    void SetCheck(bool b);

private:

    HBITMAP hBitmap;
};


