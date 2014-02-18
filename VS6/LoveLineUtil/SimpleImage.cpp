

#include "pch.h"
#include "simpleimage.h"


CSimpleImage::CSimpleImage()
{
    m_hbm = 0;
    m_dc = 0;
}


CSimpleImage::~CSimpleImage()
{
    Free();
}


HRESULT CSimpleImage::Load(HINSTANCE hInstance, PCWSTR szResource)
{
    Free();

    m_hbm = (HBITMAP)LoadImage(hInstance, szResource, IMAGE_BITMAP, 0, 0, 0);

    if(!m_hbm)
    {
        return E_FAIL;
    }

    HDC dcScreen = GetDC(0);
    m_dc = CreateCompatibleDC(dcScreen);
    ReleaseDC(0, dcScreen);

    SelectObject(m_dc, m_hbm);

    if(0 == GetObject(m_hbm, sizeof(m_bm), &m_bm))
    {
        return E_FAIL;
    }

    return S_OK;
}


HRESULT CSimpleImage::Load(PCWSTR wszFile)
{
    Free();

    m_hbm = (HBITMAP)LoadImage(0, wszFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    if(!m_hbm)
    {
        odsf(L"CPotImage::Load", L"Failed!");
        return E_FAIL;
    }

    HDC dcScreen = GetDC(0);
    m_dc = CreateCompatibleDC(dcScreen);
    ReleaseDC(0, dcScreen);

    SelectObject(m_dc, m_hbm);

    if(0 == GetObject(m_hbm, sizeof(m_bm), &m_bm))
    {
        return E_FAIL;
    }

    return S_OK;
}


HRESULT CSimpleImage::Blt(HDC dc, int x, int y)
{
    if(0 == BitBlt(dc, x, y, m_bm.bmWidth+x, m_bm.bmHeight+y, m_dc, 0, 0, SRCCOPY))
    {
        return E_FAIL;
    }

    return S_OK;
}


HRESULT CSimpleImage::Free()
{
    if(m_dc) { DeleteDC(m_dc); m_dc = 0; }
    if(m_hbm) { DeleteObject(m_hbm); m_hbm = 0; }
    return S_OK;
}


HRESULT CSimpleImage::Blt(HDC dc, int xDest, int yDest, int xSrc, int ySrc, int cx, int cy)
{
    if(!BitBlt(dc, xDest, yDest, cx, cy, m_dc, xSrc, ySrc, SRCCOPY))
    {
        return E_FAIL;
    }

    return S_OK;
}


