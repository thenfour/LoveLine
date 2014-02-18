

#include "pch.h"
#include "global.h"
#include "levelview.h"
#include "resource.h"


#define WC_LEVELVIEW    L"LevelView"


CLevelView::CLevelView() :
    m_min(0),
    m_max(100),
    m_pos(0),
    m_dc(0),
    m_hBuffer(0),
    m_nGreen(0),
    m_nYellow(0),
    m_nRed(0),
    m_nTotal(0),
    m_hbr(0),
    m_nOffsetX(0),
    m_nOffsetY(0)
{
}


CLevelView::~CLevelView()
{
    _Destroy();
}


void CLevelView::_Destroy()
{
    m_yellow_on.Free();
    m_yellow_off.Free();
    m_red_on.Free();
    m_red_off.Free();
    m_green_on.Free();
    m_green_off.Free();

    if(m_dc) DeleteDC(m_dc);
    m_dc = 0;

    if(m_hbr) DeleteObject(m_hbr);
    m_hbr = 0;

    if(m_hBuffer) DeleteObject(m_hBuffer);
    m_hBuffer = 0;
}


void CLevelView::Create(HWND hParent, RECT& prect, RECT& orect)
{
    m_hbr = CreateSolidBrush(RGB(40,63,47));

    // Register the class.
    WNDCLASS wc = {0};
    wc.hInstance = g_hInstance;
    wc.hbrBackground = m_hbr;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpfnWndProc = CLevelView::WndProc;
    wc.lpszClassName = WC_LEVELVIEW;
    RegisterClass(&wc);

	CopyRect(&this->orect, &orect);
	CopyRect(&this->prect, &prect);

	this->hParent = hParent;
	this->dwStyle = WS_CHILD;

    m_yellow_on.Load(g_hInstance, MAKEINTRESOURCE(IDB_YELLOWON));
    m_yellow_off.Load(g_hInstance, MAKEINTRESOURCE(IDB_YELLOWOFF));
    m_red_on.Load(g_hInstance, MAKEINTRESOURCE(IDB_REDON));
    m_red_off.Load(g_hInstance, MAKEINTRESOURCE(IDB_REDOFF));
    m_green_on.Load(g_hInstance, MAKEINTRESOURCE(IDB_GREENON));
    m_green_off.Load(g_hInstance, MAKEINTRESOURCE(IDB_GREENOFF));

    this->hWnd = CreateWindowExW(0, WC_LEVELVIEW, L"", this->dwStyle, 0, 0, 1, 1,
		hParent, (HMENU)this->GetId(), 0, this);

    m_dc = CreateCompatibleDC(NULL);

	this->Size();

    this->Show();
}


void CLevelView::SetExtents(long lmin, long lmax)
{
    m_min = lmin;
    m_max = lmax;
}


void CLevelView::GetExtents(long* pmin, long* pmax)
{
    if(pmin) *pmin = m_min;
    if(pmax) *pmax = m_max;
}


void CLevelView::GetPos(long* lpos)
{
    if(lpos) *lpos = m_pos;
}


void CLevelView::SetPos(long lpos)
{
    m_pos = lpos;
    Invalidate();
}


void CLevelView::_Draw(HDC dc, RECT* pArea)
{
    int nLEDs = 0;
    int n = 0;
    int nStep = m_red_on.m_bm.bmWidth;
    int x = 0;
    RECT rcClient;
    GetClientRect(this->hWnd, &rcClient);

    nLEDs = MulDiv(m_pos-m_min, m_nTotal, m_max-m_min);

    FillRect(m_dc, &rcClient, m_hbr);

    // draw green LEDs
    for(n=0;n<m_nGreen;n++)
    {
        if(nLEDs)
        {
            nLEDs --;
            m_green_on.Blt(m_dc, x, 0);
        }
        else
        {
            m_green_off.Blt(m_dc, x, 0);
        }

        x += nStep;
    }
    for(n=0;n<m_nYellow;n++)
    {
        if(nLEDs)
        {
            nLEDs --;
            m_yellow_on.Blt(m_dc, x, 0);
        }
        else
        {
            m_yellow_off.Blt(m_dc, x, 0);
        }

        x += nStep;
    }
    for(n=0;n<m_nRed;n++)
    {
        if(nLEDs)
        {
            nLEDs --;
            m_red_on.Blt(m_dc, x, 0);
        }
        else
        {
            m_red_off.Blt(m_dc, x, 0);
        }

        x += nStep;
    }

    // Flip() it to the foreground.
    BitBlt(dc, m_nOffsetX, m_nOffsetY, rcClient.right, rcClient.bottom, m_dc, 0, 0, SRCCOPY);
}


LPARAM CALLBACK CLevelView::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    CLevelView* pThis = reinterpret_cast<CLevelView*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch(uMsg)
    {
    case WM_SIZE:
        {
            RECT rcClient;

            GetClientRect(hWnd, &rcClient);

            HDC dcScreen = GetDC(0);

            if(pThis->m_hBuffer) DeleteObject(pThis->m_hBuffer);
            pThis->m_hBuffer = CreateCompatibleBitmap(dcScreen, rcClient.right, rcClient.bottom);
            ReleaseDC(0, dcScreen);

            SelectObject(pThis->m_dc, pThis->m_hBuffer);

            pThis->m_nTotal = rcClient.right / pThis->m_red_on.m_bm.bmWidth;
            pThis->m_nRed = 1;
            pThis->m_nYellow = pThis->m_nTotal / 3;
            pThis->m_nGreen = pThis->m_nTotal - (pThis->m_nRed + pThis->m_nYellow);

            int nLEDAreaWidth = pThis->m_red_on.m_bm.bmWidth * pThis->m_nTotal;
            int nLEDAreaHeight = pThis->m_red_on.m_bm.bmHeight;
            pThis->m_nOffsetX = (rcClient.right - nLEDAreaWidth) / 2;
            pThis->m_nOffsetY = (rcClient.bottom - nLEDAreaHeight) / 2;

            return 0;
        }
    case LVM_SETRANGE:
        {
            pThis->SetExtents((long)wParam, (long)lParam);
            return 0;
        }
    case LVM_SETPOS:
        {
            pThis->SetPos((long)lParam);
            return 0;
        }
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            BeginPaint(hWnd, &ps);
            pThis->_Draw(ps.hdc, &ps.rcPaint);
            EndPaint(hWnd, &ps);
            return 0;
        }
    case WM_CREATE:
        {
            CREATESTRUCT* pcs = (CREATESTRUCT*)lParam;
            pThis = (CLevelView*)pcs->lpCreateParams;
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
            return 0;
        }
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}



