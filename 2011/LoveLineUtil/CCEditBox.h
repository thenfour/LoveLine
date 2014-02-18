

#pragma once


#include "CCControl.h"
#include "global.h"


/*
    Allocate 64k at a time
*/
#define LOCAL_EDIT_BUFFER 64000


//////////////////////////////////////////////////////////////////////////////
class CCEXPORT CCEditBox : public CCControl
{
public:
	CCEditBox();
	~CCEditBox();

	HRESULT Create(HWND hParent, HINSTANCE, RECT prect, RECT orect, bool Border = true);
	HRESULT CreateMultiline(HWND hParent, HINSTANCE, RECT prect, RECT orect, bool Border = true);

    HRESULT SetText(PCWSTR wsz);
    HRESULT GetText(PWSTR wsz, DWORD cchMax);

    HRESULT Append(PCWSTR);

    HRESULT SetMultiline(BOOL);
    HRESULT SetReadOnly(BOOL);

private:

};


