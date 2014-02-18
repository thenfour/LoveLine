

#include "LLSlider.h"
#include "Windowsx.h"
#include "LLUtils.h"


#define STATUS_Y 18
#define BOOKMARK_Y 14
#define SHIFT_SPEED_FACTOR 10
#define X_SIZE 10

LLSlider::LLSlider()
{
    this->dwLength = 0;
    this->dwPos = 0;
    this->pBookmarks = 0;
    this->pCommercials = 0;
    this->sStatusText = L"Ready.";
    this->Highlight.dwIndex = 0;
    this->Highlight.dwType = LLS_NONE;
}


LLSlider::~LLSlider()
{
    this->ClearBookmarks();
    this->ClearCommercials();
	DestroyWindow(this->hWnd);

    DeleteObject(this->hBookmarkHandle);
    DeleteObject(this->hCommercialSelected);
    DeleteObject(this->hCommercialHandle);
    DeleteObject(this->hFlatLine);
    DeleteObject(this->hCursor);
    DeleteObject(this->hHighlight);
    DeleteObject(this->hCommercial);
    DeleteObject(this->hBackground);
    DeleteObject(this->hBookmark);
    DeleteObject(this->hDelete);

    UnregisterClassW(this->sClass, this->hInstance);
}


HRESULT LLSlider::Create(HWND hParent, HINSTANCE hInstance, RECT prect, RECT orect)
{
	WNDCLASSW wc;

    this->hFlatLine = CreateSolidBrush(RGB(0, 150, 255));
    this->hCursor = CreateSolidBrush(RGB(0, 100, 255));
    this->hHighlight = CreateSolidBrush(RGB(255, 0, 0));
    this->hBackground = CreateSolidBrush(RGB(255,255,255));

    this->hBookmark = CreateSolidBrush(RGB(255, 168, 168));
    this->hBookmarkHandle = CreateSolidBrush(RGB(255, 0, 0));

    this->hCommercial = CreateSolidBrush(        RGB(212,254,209));
    this->hCommercialSelected = CreateSolidBrush(RGB(85,254,73));
    this->hCommercialHandle = CreateSolidBrush(  RGB(255,0,0));

    this->hDelete = CreatePen(PS_SOLID, 4, RGB(255,0,0));

    this->hInstance  = hInstance;

	CopyRect(&this->orect, &orect);
	CopyRect(&this->prect, &prect);

	this->hParent = hParent;
	this->dwStyle = WS_CHILD | WS_VISIBLE;

    this->sClass.FromNewGuid();

	wc.cbClsExtra= 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = this->hBackground;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = NULL;
	wc.hInstance = hInstance;
    wc.lpfnWndProc = LLSlider::WndProc;
	wc.lpszClassName = this->sClass;
	wc.lpszMenuName = 0;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	RegisterClassW(&wc);

    this->Highlight.dwType = LLS_NONE;
    SetRect(&this->rcHighlight, 0, 0, 0, 0);

	this->hWnd = CreateWindowExW(0, this->sClass, L"", this->dwStyle,
		0, 0, 1, 1,
		(HWND)this->hParent,
		(HMENU)0, hInstance, this);

    this->SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));

	return S_OK;
}


/*
    This function needs to delete each commercial one by one
    because applications should be allowed to enumerate commercials
    during the operation (they'll be given the LLS_REMOVECOMMERCIAL
    notification DURING the process).
*/
HRESULT LLSlider::ClearCommercials()
{
    while(this->pCommercials)
    {
        this->RemoveCommercial(0);
    }

    return S_OK;
}


HRESULT LLSlider::AddCommercial(DWORD dwIn, DWORD dwOut, PVOID pParam, GUID id)
{
    this->CheckBounds(&dwIn);
    this->CheckBounds(&dwOut);

    SLIDER_COMMERCIAL_LIST_ITEM* pNew = new SLIDER_COMMERCIAL_LIST_ITEM;

    pNew->id = id;
    pNew->dwIn = dwIn;
    pNew->dwOut = dwOut;
    pNew->pParam = pParam;
    pNew->pNext = this->pCommercials;
    pNew->pPrevious = NULL;

    if(this->pCommercials) this->pCommercials->pPrevious = pNew;
    this->pCommercials = pNew;

    this->Invalidate();

    return S_OK;
}


HRESULT LLSlider::SetCommercial(int i, DWORD dwIn, DWORD dwOut, PVOID pParam, GUID id)
{
    this->CheckBounds(&dwIn);
    this->CheckBounds(&dwOut);

    SLIDER_COMMERCIAL_LIST_ITEM* p = this->_GetCommercial(i);
    LSN_CHANGECOMMERCIAL_STRUCT s;

    if(p == NULL)
    {
        return E_FAIL;
    }

    s.New.id = id;
    s.New.dwIn = dwIn;
    s.New.dwOut = dwOut;
    s.New.pParam = pParam;
    s.Old.id = p->id;
    s.Old.dwIn = p->dwIn;
    s.Old.dwOut = p->dwOut;
    s.Old.pParam = p->pParam;

    p->id = id;
    p->dwIn = dwIn;
    p->dwOut = dwOut;
    p->pParam = pParam;

    this->SendParentNotification(LSN_CHANGECOMMERCIAL, &s);

    this->Invalidate();

    return S_OK;
}


HRESULT LLSlider::RemoveCommercial(int i)
{
    SLIDER_COMMERCIAL_LIST_ITEM* p = this->_GetCommercial(i);

    if(p == NULL)
    {
        return E_FAIL;
    }

    if(p->pPrevious)
    {
        p->pPrevious->pNext = p->pNext;
    }

    if(p->pNext)
    {
        p->pNext->pPrevious = p->pPrevious;
    }

    if(this->pCommercials == p)
    {
        this->pCommercials = NULL;
    }

    delete p;

    this->Invalidate();

    return S_OK;
}


int LLSlider::GetCommercialCount()
{
    SLIDER_COMMERCIAL_LIST_ITEM* p = this->pCommercials;
    int i = 0;

    while(p)
    {
        i++;
        p = p->pNext;
    }

    return i;
}


HRESULT LLSlider::GetCommercial(int i, SLIDER_COMMERCIAL* pCommercial)
{
    SLIDER_COMMERCIAL_LIST_ITEM* p = this->_GetCommercial(i);

    if(p == NULL) return E_FAIL;

    pCommercial->id = p->id;
    pCommercial->dwIn = p->dwIn;
    pCommercial->dwOut = p->dwOut;
    pCommercial->pParam = p->pParam;

    return S_OK;
}


HRESULT LLSlider::ClearBookmarks()
{
    while(this->pBookmarks)
    {
        this->RemoveBookmark(0);
    }

    return S_OK;
}

void LLSlider::CheckBounds(DWORD* pdw)
{
    if(*pdw > this->dwLength) *pdw = this->dwLength;
}

HRESULT LLSlider::AddBookmark(DWORD dwIn, CCString Text, PVOID pParam, GUID id)
{
    SLIDER_BOOKMARK_LIST_ITEM* pNew = new SLIDER_BOOKMARK_LIST_ITEM;

    this->CheckBounds(&dwIn);

    pNew->id = id;
    pNew->dwPos = dwIn;
    pNew->pParam = pParam;
    pNew->sName = Text;
    pNew->pNext = this->pBookmarks;
    pNew->pPrevious = NULL;

    if(this->pBookmarks) this->pBookmarks->pPrevious = pNew;
    this->pBookmarks = pNew;

    this->Invalidate();

    return S_OK;
}


HRESULT LLSlider::SetBookmark(int i, DWORD dwIn, CCString Text, PVOID pParam, GUID id)
{
    this->CheckBounds(&dwIn);

    SLIDER_BOOKMARK_LIST_ITEM* p = this->_GetBookmark(i);
    LSN_CHANGEBOOKMARK_STRUCT s;

    if(p == NULL)
    {
        return E_FAIL;
    }

    s.New.id = id;
    s.New.dwPos = dwIn;
    s.New.pParam = pParam;
    s.New.sName = Text;
    s.Old.id = p->id;
    s.Old.dwPos = p->dwPos;
    s.Old.pParam = p->pParam;
    s.Old.sName = p->sName;

    p->id = id;
    p->dwPos = dwIn;
    p->sName = Text;
    p->pParam = pParam;

    this->SendParentNotification(LSN_CHANGEBOOKMARK, &s);

    this->Invalidate();

    return S_OK;
}


HRESULT LLSlider::RemoveBookmark(int i)
{
    SLIDER_BOOKMARK_LIST_ITEM* p = this->_GetBookmark(i);

    if(p == NULL)
    {
        return E_FAIL;
    }

    if(p->pPrevious)
    {
        p->pPrevious->pNext = p->pNext;
    }

    if(p->pNext)
    {
        p->pNext->pPrevious = p->pPrevious;
    }

    if(this->pBookmarks == p)
    {
        this->pBookmarks = NULL;
    }

    delete p;

    this->Invalidate();

    return S_OK;
}


int LLSlider::GetBookmarkCount()
{
    SLIDER_BOOKMARK_LIST_ITEM* p = this->pBookmarks;
    int i = 0;

    while(p)
    {
        i++;
        p = p->pNext;
    }

    return i;
}


HRESULT LLSlider::GetBookmark(int i, SLIDER_BOOKMARK* pCommercial)
{
    SLIDER_BOOKMARK_LIST_ITEM* p = this->_GetBookmark(i);

    if(p == NULL) return E_FAIL;

    pCommercial->id = p->id;
    pCommercial->dwPos = p->dwPos;
    pCommercial->sName = p->sName;
    pCommercial->pParam = p->pParam;

    return S_OK;
}


SLIDER_COMMERCIAL_LIST_ITEM* LLSlider::_GetCommercial(int i)
{
    SLIDER_COMMERCIAL_LIST_ITEM* p = this->pCommercials;

    for(;i>0;i--)
    {
        if(p == NULL) return NULL;

        p = p->pNext;
    }

    return p;
}


SLIDER_BOOKMARK_LIST_ITEM* LLSlider::_GetBookmark(int i)
{
    SLIDER_BOOKMARK_LIST_ITEM* p = this->pBookmarks;

    for(;i>0;i--)
    {
        if(p == NULL) return NULL;

        p = p->pNext;
    }

    return p;
}


DWORD LLSlider::GetPos()
{
    return this->dwPos;
}


void LLSlider::SetPos(DWORD dw)
{
    RECT rcCursor;

    if(dw > this->dwLength) dw = this->dwLength;
    this->dwPos = dw;

    // We don't want to invalidate the whole damn thing just because the cursor changed.
    this->CalculateCursorRect(&rcCursor);
    this->Invalidate(&rcCursor);
    this->Invalidate(&this->rcCursor);

    this->SendParentNotification(LSN_SETPOS, NULL);
}


void LLSlider::Draw(PAINTSTRUCT* ps)
{
    HDC hdc = ps->hdc;
    RECT* rc = &ps->rcPaint;
    SLIDER_BOOKMARK_LIST_ITEM* pb;
    SLIDER_COMMERCIAL_LIST_ITEM* pc;
    int i;
    BOOL bSelected;

    /*
        Calculate all the rectangles
    */
    GetClientRect(this->hWnd, &this->rcWindow);
    CopyRect(&this->rcBookmarkFrame, &this->rcWindow);
    CopyRect(&this->rcSliderFrame, &this->rcWindow);
    CopyRect(&this->rcStatusFrame, &this->rcWindow);

    // Slider area
    this->rcSliderFrame.bottom -= STATUS_Y;
    this->rcSliderFrame.top += BOOKMARK_Y;
    CopyRect(&this->rcSlider, &this->rcSliderFrame);
    InflateRect(&this->rcSlider, -2, -2);

    // Bookmark Area
    this->rcBookmarkFrame.bottom = BOOKMARK_Y;
    CopyRect(&this->rcBookmark, &this->rcBookmarkFrame);
    InflateRect(&this->rcBookmark, -2, -2);

    // Status Area
    this->rcStatusFrame.top = this->rcStatusFrame.bottom - STATUS_Y;
    CopyRect(&this->rcStatus, &this->rcStatusFrame);
    InflateRect(&this->rcStatus, -2, -2);


    // Draw the background.
    FillRect(hdc, rc, this->hBackground);

    // Draw commercials.
    pc = this->pCommercials;
    i = 0;

    while(pc)
    {
        // Draw the commercial.
        bSelected = FALSE;
        if(i == this->Highlight.dwIndex &&
            (this->Highlight.dwType == LLS_COMMERCIALIN ||
            this->Highlight.dwType == LLS_COMMERCIALOUT))
        {
            bSelected = TRUE;
        }
        this->DrawCommercial(pc, ps, bSelected);
        pc = pc->pNext;
        i++;
    }

    // Draw the blue line.
    this->DrawBlueLine(ps);

    // Draw the bookmarks.
    pb = this->pBookmarks;
    i = 0;

    while(pb)
    {
        bSelected = FALSE;
        if(i == this->Highlight.dwIndex && this->Highlight.dwType == LLS_BOOKMARK)
        {
            bSelected = TRUE;
        }
        this->DrawBookmark(pb, ps, bSelected);
        pb = pb->pNext;
        i++;
    }

    // Draw the cursor.
    this->DrawCursor(ps);

    // Draw the status text.
    this->DrawStatus(ps);

    DrawEdge(ps->hdc, &this->rcBookmarkFrame, EDGE_SUNKEN, BF_RECT);
    DrawEdge(ps->hdc, &this->rcStatusFrame, EDGE_SUNKEN, BF_RECT);
    DrawEdge(ps->hdc, &this->rcSliderFrame, EDGE_SUNKEN, BF_RECT);
}


void LLSlider::CalculateCursorRect(RECT* prc)
{
    *prc = this->rcSlider;

    prc->left = this->TimeToPixels(this->dwPos) - 5;
    prc->right = prc->left + 10;
}


void LLSlider::DrawCursor(PAINTSTRUCT* ps)
{
    this->rcCursor = this->rcSlider;

    this->rcCursor.left = this->TimeToPixels(this->dwPos);
    this->rcCursor.right = this->rcCursor.left;

    InflateRect(&this->rcCursor, 4, -2);
    FillRect(ps->hdc, &this->rcCursor, this->hCursor);

    InflateRect(&this->rcCursor, -3, 2);
    FillRect(ps->hdc, &this->rcCursor, this->hFlatLine);

    InflateRect(&this->rcCursor, 4, 2);
}


DWORD LLSlider::TimeToPixels(DWORD dwMS)
{
    DWORD dw;
    int nWidth = this->rcSlider.right - this->rcSlider.left;

    if(this->dwLength == 0) return 0;

    dw = (nWidth * dwMS) / this->dwLength;

    return dw + GetSystemMetrics(SM_CXEDGE);
}


void LLSlider::SetLength(DWORD dwLen)
{
    this->dwLength = dwLen;
    this->Invalidate();
}


DWORD LLSlider::PixelsToTime(int dwPixels)
{
    DWORD dw;
    int nWidth = this->rcSlider.right - this->rcSlider.left;

    if(nWidth == 0) return 0;
    if(dwPixels < 0) return 0;

    dw = (dwPixels * this->dwLength) / nWidth;

    if(dw > this->dwLength) dw = this->dwLength;
    return dw;
}


void LLSlider::DrawBookmark(SLIDER_BOOKMARK_LIST_ITEM* p, PAINTSTRUCT* ps, BOOL bSelected)
{
    POINT pt[3];
    HRGN hRgn = NULL;
    int nHeight = this->rcBookmark.bottom - this->rcBookmark.top;
    RECT rc = this->rcSlider;
    rc.left = this->TimeToPixels(p->dwPos);
    rc.right = rc.left + 1;
    HBRUSH h = bSelected == FALSE ? this->hBookmark : this->hBookmarkHandle;

    pt[0].x = rc.left;
    pt[0].y = this->rcBookmark.bottom;
    pt[1].x = rc.left - nHeight;
    pt[1].y = this->rcBookmark.top;
    pt[2].x = rc.left + nHeight;
    pt[2].y = this->rcBookmark.top;

    FillRect(ps->hdc, &rc, h);

    hRgn = CreatePolygonRgn(pt, 3, WINDING);

    FillRgn(ps->hdc, hRgn, h);

    DeleteObject(hRgn);

    // And if they're deleting us, draw a big X in the middle of us.
    if(bSelected == TRUE && this->bDeleting == TRUE)
    {
        RECT rcDelete;
        SelectObject(ps->hdc, this->hDelete);
        rcDelete.left = this->TimeToPixels(p->dwPos);
        rcDelete.right = rcDelete.left;
        rcDelete.top = (this->rcSlider.bottom + this->rcSlider.top) / 2;
        rcDelete.bottom = rcDelete.top;

        InflateRect(&rcDelete, X_SIZE, X_SIZE);

        MoveToEx(ps->hdc, rcDelete.left, rcDelete.top, NULL);
        LineTo(ps->hdc, rcDelete.right, rcDelete.bottom);

        MoveToEx(ps->hdc, rcDelete.right, rcDelete.top, NULL);
        LineTo(ps->hdc, rcDelete.left, rcDelete.bottom);
    }
}


void LLSlider::DrawBlueLine(PAINTSTRUCT* ps)
{
    RECT rcBlueLine = this->rcSlider;
    rcBlueLine.top = (rcBlueLine.bottom + rcBlueLine.top) / 2;
    rcBlueLine.bottom = rcBlueLine.top + 1;

    FillRect(ps->hdc, &rcBlueLine, this->hFlatLine);
}


void LLSlider::DrawStatus(PAINTSTRUCT* ps)
{
    SelectObject(ps->hdc, (HFONT)GetStockObject(DEFAULT_GUI_FONT));

    TextOut(ps->hdc, this->rcStatus.left, this->rcStatus.top, this->sStatusText, this->sStatusText.len());
}

void LLSlider::DrawCommercial(SLIDER_COMMERCIAL_LIST_ITEM* p, PAINTSTRUCT* ps, BOOL bSelected)
{
    HBRUSH h = bSelected == TRUE ? this->hCommercialSelected : this->hCommercial;
    RECT rc = this->rcSlider;
    int nTemp;
    DWORD dwHighlightType = this->Highlight.dwType;
    rc.left = this->TimeToPixels(p->In());
    rc.right = this->TimeToPixels(p->Out());
    if(rc.left > rc.right)
    {
        nTemp = rc.left;
        rc.left = rc.right;
        rc.right = nTemp;
    }

    FillRect(ps->hdc, &rc, h);

    if(bSelected == TRUE)
    {
        if(this->bIsDragging == TRUE)
        {
            // We may have to swap out and in because the user may have dragged the
            // handle over the other one, swapping them around.
            if(this->pdw == p->_In())
            {
                dwHighlightType = LLS_COMMERCIALIN;
            }
            else
            {
                dwHighlightType = LLS_COMMERCIALOUT;
            }
        }

        // Figure out whether its the in or out thing...
        if(dwHighlightType == LLS_COMMERCIALIN)
        {
            rc.right = this->TimeToPixels(p->In()) + 1;
            rc.left = this->TimeToPixels(p->In()) - 1;
            FillRect(ps->hdc, &rc, this->hCommercialHandle);
        }
        else
        {
            rc.left = this->TimeToPixels(p->Out()) - 1;
            rc.right = this->TimeToPixels(p->Out()) + 1;
            FillRect(ps->hdc, &rc, this->hCommercialHandle);
            }

        // And if they're deleting us, draw a big X in the middle of us.
        if(this->bDeleting == TRUE)
        {
            RECT rcDelete;
            SelectObject(ps->hdc, this->hDelete);
            rcDelete.left = (this->TimeToPixels(p->Out()) + this->TimeToPixels(p->In())) / 2;
            rcDelete.right = rcDelete.left;
            rcDelete.top = (this->rcSlider.bottom + this->rcSlider.top) / 2;
            rcDelete.bottom = rcDelete.top;

            InflateRect(&rcDelete, X_SIZE, X_SIZE);

            MoveToEx(ps->hdc, rcDelete.left, rcDelete.top, NULL);
            LineTo(ps->hdc, rcDelete.right, rcDelete.bottom);

            MoveToEx(ps->hdc, rcDelete.right, rcDelete.top, NULL);
            LineTo(ps->hdc, rcDelete.left, rcDelete.bottom);
        }
    }
}


void LLSlider::CalculateHighlight(POINT* p)
{
    LLSHighlight NewHighlight;
    NewHighlight.dwType = LLS_NONE;
    NewHighlight.dwIndex = 0;
    SLIDER_BOOKMARK_LIST_ITEM* pb = NULL;
    SLIDER_COMMERCIAL_LIST_ITEM* pc = NULL;
    SLIDER_BOOKMARK_LIST_ITEM* pBestBookmark = NULL;
    SLIDER_COMMERCIAL_LIST_ITEM* pBestCommercial = NULL;
    DWORD dwClosestDistance = (DWORD)-1;
    DWORD dwDistance;
    DWORD x = this->PixelsToTime(p->x);

    int i;

    /*
        If the cursor is in the bookmark area, only bookmarks can be highlighted.
        If the cursor is in the slider area, only commercials can be highlighted.
    */
    if(PtInRect(&this->rcSlider, *p) == TRUE)
    {
        CCString sLength;
        CCString sIn;
        CCString sOut;

        // We have a commercial situation on our hands.
        i = 0;
        pc = this->pCommercials;

        while(pc)
        {
            LLFormatMilliseconds(pc->In(), &sIn);
            LLFormatMilliseconds(pc->Out(), &sOut);
            LLFormatMilliseconds(pc->Len(), &sLength);

            // Determine whether either point is closer than dwClosestDistance
            dwDistance = abs(x - (int)pc->In());
            if(dwDistance < dwClosestDistance)
            {
                // the "In" point is closest.
                NewHighlight.dwType = LLS_COMMERCIALIN;
                NewHighlight.dwIndex = i;
                dwClosestDistance = dwDistance;
                this->sStatusText.FormatW(
                    L"Commercial (%s - %s) Length %s", (PCWSTR)sIn, (PCWSTR)sOut, (PCWSTR)sLength);
                pBestCommercial = pc;
            }

            dwDistance = abs(x - (int)pc->Out());
            if(dwDistance < dwClosestDistance)
            {
                // the "In" point is closest.
                NewHighlight.dwType = LLS_COMMERCIALOUT;
                NewHighlight.dwIndex = i;
                dwClosestDistance = dwDistance;
                this->sStatusText.FormatW(
                    L"Commercial (%s - %s) Length %s", (PCWSTR)sIn, (PCWSTR)sOut, (PCWSTR)sLength);
                pBestCommercial = pc;
            }

            pc = pc->pNext;
            i++;
        }
    }
    else if(PtInRect(&this->rcBookmark, *p) == TRUE)
    {
        // We have a bookmark situation on our hands.
        i = 0;
        pb = this->pBookmarks;
        CCString sPos;

        while(pb)
        {
            // Determine whether either point is closer than dwClosestDistance
            dwDistance = abs(x - (int)pb->dwPos);
            if(dwDistance < dwClosestDistance)
            {
                LLFormatMilliseconds(pb->dwPos, &sPos);
                // the "In" point is closest.
                NewHighlight.dwType = LLS_BOOKMARK;
                NewHighlight.dwIndex = i;
                dwClosestDistance = dwDistance;
                pBestBookmark = pb;
                this->sStatusText.FormatW(L"%s (%s)", (PCWSTR)pb->sName, (PCWSTR)sPos);
            }

            pb = pb->pNext;
            i++;
        }
    }

    // If this one is different than the one we already have,
    // we'll update.  Otherwise we'll just act as if nothing happened.
    if((NewHighlight.dwIndex != this->Highlight.dwIndex) ||
        (NewHighlight.dwType != this->Highlight.dwType))
    {
        // First invalidate the current highlighted region.
        this->InvalidateHighlight();

        // Change the highlight and the rect.
        this->Highlight.dwIndex = NewHighlight.dwIndex;
        this->Highlight.dwType = NewHighlight.dwType;

        this->CalculateHighlightRect();

        this->InvalidateHighlight();
    }
}


void LLSlider::CalculateHighlightRect()
{
    SLIDER_BOOKMARK_LIST_ITEM* pb = NULL;
    SLIDER_COMMERCIAL_LIST_ITEM* pc = NULL;
    int nBookmarkWidth = BOOKMARK_Y;

    if(this->Highlight.dwType == LLS_CURSOR)
    {
        this->CalculateCursorRect(&this->rcHighlight);
        return;
    }

    else if(this->Highlight.dwType == LLS_COMMERCIALIN || this->Highlight.dwType == LLS_COMMERCIALOUT)
    {
        // It's a commercial.
        pc = this->_GetCommercial(this->Highlight.dwIndex);

        if(pc)
        {
            this->rcHighlight.top = this->rcSlider.top;
            this->rcHighlight.bottom = this->rcSlider.bottom;
            this->rcHighlight.left = this->TimeToPixels(pc->In()) - 2;
            this->rcHighlight.right = this->TimeToPixels(pc->Out()) + 2;
        }
    }

    else if(this->Highlight.dwType == LLS_BOOKMARK)
    {
        // It's a bookmark.
        pb = this->_GetBookmark(this->Highlight.dwIndex);

        if(pb)
        {
            this->rcHighlight.top = this->rcBookmark.top;
            this->rcHighlight.bottom = this->rcSlider.bottom;
            this->rcHighlight.left = this->TimeToPixels(pb->dwPos) - nBookmarkWidth;
            this->rcHighlight.right = this->rcHighlight.left + nBookmarkWidth + nBookmarkWidth;
        }
    }
}


void LLSlider::InvalidateHighlight()
{
    this->Invalidate(&this->rcHighlight);
    this->Invalidate(&this->rcStatus);
}


LRESULT CALLBACK LLSlider::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LLSlider* pThis = (LLSlider*)GetWindowLongPtr(hWnd, GWL_USERDATA);

    switch(uMsg)
    {
    case WM_MOUSELEAVE:
        {
            pThis->Highlight.dwType = LLS_NONE;
            pThis->sStatusText = L"Ready.";
            pThis->InvalidateHighlight();
            return TRUE;
        }
    case WM_SETCURSOR:
        {
            TRACKMOUSEEVENT tme;
            tme.cbSize = sizeof(tme);
            tme.dwFlags = TME_LEAVE;
            tme.dwHoverTime = 0;
            tme.hwndTrack = hWnd;
            TrackMouseEvent(&tme);
            break;
        }
    case WM_KEYDOWN:
        {
            switch(wParam)
            {
            case VK_ESCAPE:
                {
                    // If we were dragging, let's quit dragging, use the old
                    // value - basically canceling.
                    if(pThis->bIsDragging == TRUE)
                    {
                        ReleaseCapture();
                        LSN_DRAG_STRUCT s;
                        pThis->bIsDragging = FALSE;
                        pThis->bDeleting = FALSE;

                        DWORD* pdw = pThis->_GetPosition(&pThis->Highlight);

                        if(pdw)
                        {
                            *pdw = pThis->dwOldPos;
                        }

                        s.DragObject.dwIndex = pThis->Highlight.dwIndex;
                        s.DragObject.dwType = pThis->Highlight.dwType;
                        s.dwNewPosition = pThis->dwOldPos;

                        pThis->SendParentNotification(LSN_DRAGMOVE, &s);
                        pThis->SendParentNotification(LSN_ENDDRAGGING, &s);

                        pThis->Invalidate();
                    }

                    return 0;
                }
            }
            break;
        }
    case WM_MOUSEMOVE:
        {
            POINT p;
            p.x = GET_X_LPARAM(lParam);
            p.y = GET_Y_LPARAM(lParam);

            if(p.x == pThis->pOldPos.x)
            {
                if(p.y == pThis->pOldPos.y)
                {
                    // This is the same mousemove event as before - just exit.
                    return 0;
                }
            }

            pThis->pOldPos.x = p.x;
            pThis->pOldPos.y = p.y;

            if(pThis->bIsDragging == FALSE)
            {
                // Determine if the user has highlighted another item.
                pThis->CalculateHighlight(&p);
            }
            else
            {
                // The user is moving the mouse...
                // Determine the new position.
                DWORD dwNewPos = pThis->PixelsToTime(GET_X_LPARAM(lParam));
                int nDelta;

                if(pThis->pdw)
                {
                    /*
                        Adjust the dword now.
                        If the user is holding SHIFT, we slice the DELTA by 4
                        and adjust the mouse position too.
                    */
                    if(wParam & MK_SHIFT)
                    {
                        nDelta = (int)((dwNewPos - (*pThis->pdw))) / SHIFT_SPEED_FACTOR;
                        dwNewPos = (*pThis->pdw) + nDelta;

                        p.x = pThis->TimeToPixels(dwNewPos);
                        pThis->pOldPos.x = p.x;
                        ClientToScreen(pThis->hWnd, &p);
                        SetCursorPos(p.x, p.y);
                    }

                    *pThis->pdw = dwNewPos;

                    // If the cursor is out of our window then they are attempting
                    // to delete the item...
                    if(p.y < pThis->rcWindow.top || p.y > pThis->rcWindow.bottom)
                    {
                        pThis->bDeleting = TRUE;
                    }
                    else
                    {
                        pThis->bDeleting = FALSE;
                    }

                    CCString sTime;

                    switch(pThis->Highlight.dwType)
                    {
                    case LLS_CURSOR:
                        {
                            CCString sTime;
                            LLFormatMilliseconds(dwNewPos, &sTime);
                            pThis->sStatusText.FormatW(L"Cursor: %s", (PCWSTR)sTime);
                            //odsf(L"Slider::WndProc", L"%s", (PCWSTR)pThis->sStatusText);
                            break;
                        }
                    case LLS_COMMERCIALIN:
                    case LLS_COMMERCIALOUT:
                        {
                            SLIDER_COMMERCIAL_LIST_ITEM* pc;
                            pc = pThis->_GetCommercial(pThis->Highlight.dwIndex);

                            if(pc)
                            {
                                CCString sIn;
                                CCString sOut;
                                CCString sLength;
                                LLFormatMilliseconds(min(pc->dwIn, pc->dwOut), &sIn);
                                LLFormatMilliseconds(max(pc->dwIn, pc->dwOut), &sOut);
                                LLFormatMilliseconds(abs(pc->dwIn - pc->dwOut), &sLength);
                                pThis->sStatusText.FormatW(
                                    L"Commercial (%s - %s) Length %s", (PCWSTR)sIn, (PCWSTR)sOut, (PCWSTR)sLength);
                                //odsf(L"Slider::WndProc", L"%s", (PCWSTR)pThis->sStatusText);
                            }
                            break;
                        }
                    case LLS_BOOKMARK:
                        {
                            CCString sTime;
                            SLIDER_BOOKMARK_LIST_ITEM* pb;
                            pb = pThis->_GetBookmark(pThis->Highlight.dwIndex);

                            if(pb)
                            {
                                LLFormatMilliseconds(pb->dwPos, &sTime);
                                pThis->sStatusText.FormatW(L"%s (%s)", (PCWSTR)pb->sName, (PCWSTR)sTime);
                                //odsf(L"Slider::WndProc", L"%s", (PCWSTR)pThis->sStatusText);
                            }

                            break;
                        }
                    }

                    /*
                        Calculate a new highlight rect and invalidate it.
                    */
                    pThis->InvalidateHighlight();
                    pThis->CalculateHighlightRect();
                    pThis->InvalidateHighlight();

                    LSN_DRAG_STRUCT s;
                    s.DragObject.dwIndex = pThis->Highlight.dwIndex;
                    s.DragObject.dwType = pThis->Highlight.dwType;
                    s.dwNewPosition = dwNewPos;
                    pThis->SendParentNotification(LSN_DRAGMOVE, &s);

                }
            }

            break;
        }
    case WM_LBUTTONDOWN:
        {
            pThis->bDeleting = FALSE;

            // Start tracking.
            if(pThis->bIsDragging == FALSE)
            {
                POINT p;
                p.x = GET_X_LPARAM(lParam);
                p.y = GET_Y_LPARAM(lParam);

                // Here we move the cursor as long as we're within the
                // slider area.
                if(PtInRect(&pThis->rcSlider, p))
                {
                    pThis->pdw = &pThis->dwPos;

                    // Cursor to be moved.
                    SetFocus(pThis->hWnd);
                    SetCapture(pThis->hWnd);
                    pThis->bIsDragging = TRUE;

                    // Let's set up the highlight struct now.
                    pThis->dwOldPos = pThis->dwPos;
                    pThis->Highlight.dwIndex = 0;
                    pThis->Highlight.dwType = LLS_CURSOR;

                    LSN_DRAG_STRUCT s;
                    s.DragObject.dwIndex = pThis->Highlight.dwIndex;
                    s.DragObject.dwType = pThis->Highlight.dwType;
                    s.dwNewPosition = pThis->dwPos;
                    pThis->SendParentNotification(LSN_STARTDRAGGING, &s);

                    pThis->SetCursorOnHighlight();
                    pThis->InvalidateHighlight();

                    return 0;
                }

                // If we're in the bookmark area, we'll move the cursor
                // To the nearest bookmark.
                else if(PtInRect(&pThis->rcBookmark, p))
                {
                    DWORD* pdw = pThis->_GetPosition(&pThis->Highlight);

                    if(pdw)
                    {
                        pThis->SetPos(*pdw);

                        LSN_DRAG_STRUCT s;
                        s.DragObject.dwIndex = pThis->Highlight.dwIndex;
                        s.DragObject.dwType = pThis->Highlight.dwType;
                        s.dwNewPosition = *pdw;
                        pThis->SendParentNotification(LSN_STARTDRAGGING, &s);
                        pThis->SendParentNotification(LSN_DRAGMOVE, &s);
                        pThis->SendParentNotification(LSN_ENDDRAGGING, &s);

                        pThis->InvalidateHighlight();
                    }

                    return 0;
                }

                // The user's mouse is neither in the slider area or
                // the bookmark area - the left mouse button will have
                // no effect.
                return 0;
            }
            else
            {
                // This could be true here if the user is dragging with the
                // right mouse button.
            }
            break;
        }
    case WM_RBUTTONDOWN:
        {
            pThis->bDeleting = FALSE;

            if(pThis->bIsDragging == FALSE)
            {
                DWORD* pdw = pThis->_GetPosition(&pThis->Highlight);

                // If there is no selection, there will be no pdw - and
                // thus we should not start dragging.
                if(pdw)
                {
                    pThis->pdw = pdw;
                    pThis->dwOldPos = *pdw;
                    SetFocus(pThis->hWnd);
                    SetCapture(pThis->hWnd);
                    pThis->bIsDragging = TRUE;

                    LSN_DRAG_STRUCT s;
                    s.DragObject.dwIndex = pThis->Highlight.dwIndex;
                    s.DragObject.dwType = pThis->Highlight.dwType;
                    s.dwNewPosition = *pdw;
                    pThis->SetCursorOnHighlight();
                    pThis->SendParentNotification(LSN_STARTDRAGGING, &s);
                }
            }
            else
            {
            }
            break;
        }
    case WM_LBUTTONUP:
        {
            if(pThis->bIsDragging == TRUE)
            {
                DWORD* pdw = pThis->_GetPosition(&pThis->Highlight);

                ReleaseCapture();
                pThis->bIsDragging = FALSE;

                if(pdw)
                {
                    LSN_DRAG_STRUCT s;
                    s.DragObject.dwIndex = pThis->Highlight.dwIndex;
                    s.DragObject.dwType = pThis->Highlight.dwType;
                    s.dwNewPosition = *pdw;
                    pThis->SendParentNotification(LSN_ENDDRAGGING, &s);
                }

                pThis->Highlight.dwType = LLS_NONE;

                pThis->bDeleting = FALSE;
                POINT p;
                p.x = GET_X_LPARAM(lParam);
                p.y = GET_Y_LPARAM(lParam);
                pThis->CalculateHighlight(&p);

                //pThis->Invalidate();
            }
            else
            {
            }
            break;
        }
    case WM_RBUTTONUP:
        {
            if(pThis->bIsDragging == TRUE)
            {
                DWORD* pdw = pThis->_GetPosition(&pThis->Highlight);

                ReleaseCapture();
                pThis->bIsDragging = FALSE;

                /*
                    Now we changed a bookmark or commercial,
                    so let's send that notification.
                */
                LSN_CHANGECOMMERCIAL_STRUCT cs;
                LSN_CHANGEBOOKMARK_STRUCT bs;
                SLIDER_COMMERCIAL_LIST_ITEM* pc;
                SLIDER_BOOKMARK_LIST_ITEM* pb;

                if(pThis->Highlight.dwType == LLS_BOOKMARK)
                {
                    pb = pThis->_GetBookmark(pThis->Highlight.dwIndex);
                    if(pb)
                    {
                        // Did they change it or delete it?
                        if(pThis->bDeleting == TRUE)
                        {
                            LSN_REMOVEBOOKMARK_STRUCT s;
                            s.Bookmark.dwPos = pb->dwPos;
                            s.Bookmark.pParam = pb->pNext;
                            s.Bookmark.sName = pb->sName;
                            s.Bookmark.id = pb->id;
                            pThis->RemoveBookmark(pThis->Highlight.dwIndex);
                            pThis->SendParentNotification(LSN_REMOVEBOOKMARK, &s);
                            return 0;
                        }

                        bs.New.dwPos = pb->dwPos;
                        bs.New.id = pb->id;
                        bs.New.pParam = pb->pParam;
                        bs.New.sName = pb->sName;
                        bs.Old.dwPos = pThis->dwOldPos;
                        bs.Old.id = pb->id;
                        bs.Old.pParam = pb->pParam;
                        bs.Old.sName = pb->sName;
                        pThis->SendParentNotification(LSN_CHANGEBOOKMARK, &bs);
                    }
                }
                else
                {
                    // It HAS to be a commercial.
                    pc = pThis->_GetCommercial(pThis->Highlight.dwIndex);
                    if(pc)
                    {
                        // Did they change it or delete it?
                        if(pThis->bDeleting == TRUE)
                        {
                            LSN_REMOVECOMMERCIAL_STRUCT s;
                            s.Commercial.id = pc->id;
                            s.Commercial.dwIn = pc->dwIn;
                            s.Commercial.dwOut = pc->dwOut;
                            s.Commercial.pParam = pc->pParam;
                            pThis->RemoveCommercial(pThis->Highlight.dwIndex);
                            pThis->SendParentNotification(LSN_REMOVECOMMERCIAL, &s);
                            return 0;
                        }

                        cs.New.dwIn = pc->dwIn;
                        cs.New.dwOut = pc->dwOut;
                        cs.New.id = pc->id;
                        cs.New.pParam = pc->pParam;
                        cs.Old.dwIn = pc->dwIn;
                        cs.Old.dwOut = pc->dwOut;
                        cs.Old.id = pc->id;
                        cs.Old.pParam = pc->pParam;

                        if(pThis->Highlight.dwType == LLS_COMMERCIALIN)
                        {
                            cs.Old.dwIn = pThis->dwOldPos;
                        }

                        if(pThis->Highlight.dwType == LLS_COMMERCIALOUT)
                        {
                            cs.Old.dwOut = pThis->dwOldPos;
                        }

                        pThis->SendParentNotification(LSN_CHANGECOMMERCIAL, &cs);
                    }
                }

                if(pdw)
                {
                    LSN_DRAG_STRUCT s;
                    s.DragObject.dwIndex = pThis->Highlight.dwIndex;
                    s.DragObject.dwType = pThis->Highlight.dwType;
                    s.dwNewPosition = *pdw;
                    pThis->SendParentNotification(LSN_ENDDRAGGING, &s);
                }
                pThis->Highlight.dwType = LLS_NONE;
                pThis->bDeleting = FALSE;
                POINT p;
                p.x = GET_X_LPARAM(lParam);
                p.y = GET_Y_LPARAM(lParam);
                pThis->CalculateHighlight(&p);
                //pThis->Invalidate();
            }
            else
            {
            }
            break;
        }
    case WM_CREATE:
        {
            CREATESTRUCT* pcs = (CREATESTRUCT*)lParam;

            pThis = (LLSlider*)pcs->lpCreateParams;
            SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG)pThis);

            return 0;
        }
    case WM_PAINT:
        {
            //odsf(L"LLSlider::WM_PAINT", L"Painting...");
            PAINTSTRUCT ps;
            BeginPaint(hWnd, &ps);
            pThis->Draw(&ps);
            EndPaint(hWnd, &ps);
            return 0;
        }
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


BOOL LLSlider::IsDragging(LLSHighlight* pWhat)
{
    if(pWhat)
    {
        pWhat->dwIndex = this->Highlight.dwIndex;
        pWhat->dwType = this->Highlight.dwType;
    }

    return this->bIsDragging;
}


DWORD* LLSlider::_GetPosition(LLSHighlight* pHighlight)
{
    SLIDER_COMMERCIAL_LIST_ITEM* pc;
    SLIDER_BOOKMARK_LIST_ITEM* pb;

    if(pHighlight->dwType == LLS_CURSOR)
    {
        return &this->dwPos;
    }

    if(pHighlight->dwType == LLS_BOOKMARK)
    {
        pb = this->_GetBookmark(pHighlight->dwIndex);

        if(pb)
        {
            return &pb->dwPos;
        }
    }

    /*
        If the highlight is COMMERCIAL_IN, the actual value may be the OUT value
        if it's less than the in value.  So we have to adjust our value accordingly.
    */
    if(pHighlight->dwType == LLS_COMMERCIALIN)
    {
        pc = this->_GetCommercial(pHighlight->dwIndex);

        if(pc)
        {
            return pc->_In();
        }
    }

    if(pHighlight->dwType == LLS_COMMERCIALOUT)
    {
        pc = this->_GetCommercial(pHighlight->dwIndex);

        if(pc)
        {
            return pc->_Out();
        }
    }

    return NULL;
}


void LLSlider::SendParentNotification(DWORD dwMsg, PVOID lParam)
{
    NMHDR* pnmh = (NMHDR*)lParam;
    NMHDR nmh;

    if(pnmh)
    {
        pnmh->code = dwMsg;
        pnmh->hwndFrom = this->hWnd;
        pnmh->idFrom = this->id;
    }
    else
    {
        nmh.code = dwMsg;
        nmh.hwndFrom = this->hWnd;
        nmh.idFrom = this->id;
        pnmh = &nmh;
    }

    SendMessage(GetParent(this->hWnd), WM_NOTIFY, this->id, (LPARAM)pnmh);
}


int LLSlider::HitTest(DWORD dwPos)
{
    int i = -1;
    SLIDER_COMMERCIAL_LIST_ITEM* p = this->pCommercials;
    SLIDER_COMMERCIAL c;

    while(p)
    {
        i++;

        c.dwIn = p->In();
        c.dwOut = p->Out();

        // Determine if the position is in the commercial.
        if(dwPos >= c.dwIn && dwPos <= c.dwOut)
        {
            // it's between the two.
            return i;
        }

        p = p->pNext;
    }

    // We went through the whole list with nothing found; return -1.
    return -1;
}


void LLSlider::SetCursorOnHighlight()
{
    DWORD* pdw = this->_GetPosition(&this->Highlight);
    POINT p;

    if(pdw)
    {
        GetCursorPos(&p);
        ScreenToClient(this->hWnd, &p);
        p.x = this->TimeToPixels(*pdw);
        ClientToScreen(this->hWnd, &p);
        SetCursorPos(p.x, p.y);
    }
}




