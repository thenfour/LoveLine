/*
	CREATED: Nov. 3, 2001 Carl Corcoran
*/


#pragma once


#include <Windows.h>
#include <commctrl.h>
#include "CCString.h"
#include "global.h"


class CCEXPORT CCImageList
{
public:
	CCImageList();
	~CCImageList();

    // For dwColor, refer to image list stuff like:
    // ILC_COLOR24, ILC_COLOR16, etc...
    HRESULT CreateEmpty(int nSizeX, int nSizeY, DWORD dwColor);

    // cMask can be CLR_NONE or CLR_DEFAULT or a color.
    HRESULT CreateFromResource(HINSTANCE hInstance, UINT uResource, int cx, COLORREF cMask);
    HRESULT CreateFromFile(CCString FileName, int cx, COLORREF cMask);

    HRESULT Clear();
    HRESULT GetImageInfo(int i, IMAGEINFO* pii);

    HIMAGELIST GethImageList();

    HRESULT Draw(int i, HDC dc, int x, int y);

    HRESULT AddBitmapFromHandle(int* pn, HBITMAP hbm);
    HRESULT AddBitmapFromFile(int* pn, CCString sFileName);

    // cMask can be CLR_NONE or CLR_DEFAULT or a color.
    HRESULT AddBitmapFromFileMasked(int* pn, CCString sFileName, COLORREF cMask);
    HRESULT AddBitmapFromResource(int* pn, HINSTANCE hInstance, UINT uResource);

    // cMask can be CLR_NONE or CLR_DEFAULT or a color.
    HRESULT AddBitmapFromResourceMasked(int* pn, HINSTANCE hInstance, UINT uResource, COLORREF cMask);

    HRESULT AddIconFromFile(int* pn, CCString sFileName);
    HRESULT AddIconFromResource(int* pn, HINSTANCE hInstance, UINT uResource);

    HRESULT RemoveImage(int n);
    int GetCount();

private:

    HIMAGELIST hImageList;

};


