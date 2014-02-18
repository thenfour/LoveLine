

#include "CCImageList.h"


CCImageList::CCImageList()
{
    this->hImageList = NULL;
}


CCImageList::~CCImageList()
{
    ImageList_Destroy(this->hImageList);
}


HRESULT CCImageList::CreateEmpty(int nSizeX, int nSizeY, DWORD dwColor)
{
    this->hImageList = ImageList_Create(nSizeX, nSizeY, dwColor, 0, 10);

    if(this->hImageList == NULL) return E_FAIL;

    return S_OK;
}


HRESULT CCImageList::CreateFromResource(
    HINSTANCE hInstance, UINT uResource, int cx, COLORREF cMask)
{
    this->hImageList = ImageList_LoadImage(
        hInstance, MAKEINTRESOURCE(uResource),
        cx, 10, cMask, IMAGE_BITMAP, 0);

    if(this->hImageList == NULL) return E_FAIL;

    return S_OK;
}


HRESULT CCImageList::CreateFromFile(CCString FileName, int cx, COLORREF cMask)
{
    this->hImageList = ImageList_LoadImage(
        NULL, (PCWSTR)FileName,
        cx, 10, cMask, IMAGE_BITMAP, LR_LOADFROMFILE);

    if(this->hImageList == NULL) return E_FAIL;

    return S_OK;
}


HIMAGELIST CCImageList::GethImageList()
{
    return this->hImageList;
}


HRESULT CCImageList::AddBitmapFromFile(int* pn, CCString sFileName)
{
    HBITMAP hBitmap;
    int i;

    hBitmap = (HBITMAP)LoadImage(NULL, (PCWSTR)sFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    if(hBitmap == NULL) return E_FAIL;

    i = ImageList_Add(this->hImageList, hBitmap, NULL);
    DeleteObject(hBitmap);

    if(pn) *pn = i;

    if(i == -1) return E_FAIL;

    return S_OK;
}


HRESULT CCImageList::AddBitmapFromFileMasked(int* pn, CCString sFileName, COLORREF cMask)
{
    HBITMAP hBitmap;
    int i;

    hBitmap = (HBITMAP)LoadImage(NULL, (PCWSTR)sFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    if(hBitmap == NULL) return E_FAIL;

    i = ImageList_AddMasked(this->hImageList, hBitmap, cMask);
    DeleteObject(hBitmap);

    if(pn) *pn = i;

    if(i == -1) return E_FAIL;

    return S_OK;
}


HRESULT CCImageList::AddBitmapFromResource(int* pn, HINSTANCE hInstance, UINT uResource)
{
    HBITMAP hBitmap;
    int i;

    hBitmap = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(uResource),
        IMAGE_BITMAP, 0, 0, 0);

    if(hBitmap == NULL) return E_FAIL;

    i = ImageList_Add(this->hImageList, hBitmap, NULL);
    DeleteObject(hBitmap);

    if(pn) *pn = i;

    if(i == -1) return E_FAIL;

    return S_OK;
}


HRESULT CCImageList::AddBitmapFromResourceMasked(
    int* pn, HINSTANCE hInstance, UINT uResource, COLORREF cMask)
{
    HBITMAP hBitmap;
    int i;

    hBitmap = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(uResource),
        IMAGE_BITMAP, 0, 0, 0);

    if(hBitmap == NULL) return E_FAIL;

    i = ImageList_AddMasked(this->hImageList, hBitmap, cMask);
    DeleteObject(hBitmap);

    if(pn) *pn = i;

    if(i == -1) return E_FAIL;

    return S_OK;
}


HRESULT CCImageList::AddIconFromFile(int* pn, CCString sFileName)
{
    HICON hIcon;
    int i;

    hIcon = (HICON)LoadImage(NULL, (PCWSTR)sFileName,
        IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

    if(hIcon == NULL) return E_FAIL;

    i = ImageList_AddIcon(this->hImageList, hIcon);
    DestroyIcon(hIcon);

    if(pn) *pn = i;

    if(i == -1) return E_FAIL;

    return S_OK;
}


HRESULT CCImageList::AddIconFromResource(int* pn, HINSTANCE hInstance, UINT uResource)
{
    HICON hIcon;
    int i;

    hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(uResource),
        IMAGE_ICON, 0, 0, 0);

    if(hIcon == NULL) return E_FAIL;

    i = ImageList_AddIcon(this->hImageList, hIcon);
    DestroyIcon(hIcon);

    if(pn) *pn = i;

    if(i == -1) return E_FAIL;

    return S_OK;
}


HRESULT CCImageList::RemoveImage(int n)
{
    return ImageList_Remove(this->hImageList, n) == 0 ? E_FAIL : S_OK;
}


int CCImageList::GetCount()
{
    return ImageList_GetImageCount(this->hImageList);
}


HRESULT CCImageList::GetImageInfo(int i, IMAGEINFO* pii)
{
    ImageList_GetImageInfo(this->hImageList, i, pii);
    return E_FAIL;
}


HRESULT CCImageList::Draw(int i, HDC dc, int x, int y)
{
    int n;

    n = ImageList_Draw(this->hImageList, i, dc, x, y, ILD_NORMAL);

    return n == 0 ? E_FAIL : S_OK;
}


HRESULT CCImageList::AddBitmapFromHandle(int* pn, HBITMAP hbm)
{
    int n = ImageList_Add(this->hImageList, hbm, NULL);
    DeleteObject(hbm);
    if(pn) *pn = n;
    return n == -1 ? E_FAIL : S_OK;
}


HRESULT CCImageList::Clear()
{
    return ImageList_RemoveAll(this->hImageList);
}

