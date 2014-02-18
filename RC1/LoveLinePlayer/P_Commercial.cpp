

#include "P_Commercial.h"
#include "..\\LoveLineUtil\\LLUtils.h"


#define COLUMN_PREVIEW 0
#define COLUMN_IN 1
#define COLUMN_OUT 2
#define COLUMN_SIZE 3
#define COLUMN_SEGMENTLENGTH 4
#define COLUMN_ID 5


Plugin_Commercial::Plugin_Commercial()
{
    this->pCore = NULL;
    this->pPlugins = NULL;
}


Plugin_Commercial::~Plugin_Commercial()
{
}


LPARAM CALLBACK Plugin_Commercial::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Plugin_Commercial* pThis = (Plugin_Commercial*)GetWindowLongPtr(hWnd, GWL_USERDATA);

    switch(uMsg)
    {
    case WM_COMMAND:
        {
            if(LOWORD(wParam) == pThis->cRemove)
            {
                pThis->OnRemove();
                return 0;
            }
            if(LOWORD(wParam) == pThis->cEdit)
            {
                pThis->OnEdit();
                return 0;
            }
            if(LOWORD(wParam) == pThis->cAdd)
            {
                pThis->OnAdd();
                return 0;
            }
            break;
        }
    case WM_SIZE:
        {
            pThis->cRemove.Size();
            pThis->cAdd.Size();
            pThis->cEdit.Size();
            pThis->cRanges.Size();
            return 0;
        }
    case WM_CREATE:
        {
            CREATESTRUCT* pcs = (CREATESTRUCT*)lParam;
            pThis = (Plugin_Commercial*)pcs->lpCreateParams;
            SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)pThis);
            return 0;
        }
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


void CALLBACK Plugin_Commercial::NotificationProc(PVOID pUser, DWORD dwMsg, PVOID pParams)
{
    Plugin_Commercial* pThis = (Plugin_Commercial*)pUser;
    switch(dwMsg)
    {
    case LL_DBUPDATE:
        {
            LL_DBUPDATE_STRUCT* p = (LL_DBUPDATE_STRUCT*)pParams;

            if(p->dwType & DBUPDATE_SEGMENT)
            {
                pThis->Refresh();
            }
            break;
        }
    case LL_SEEKEPISODE:
        {
            break;
        }
    case LL_ATTACH:
        {
            LL_ATTACH_STRUCT* pas = (LL_ATTACH_STRUCT*)pParams;
            StandardPlugins* psp = (StandardPlugins*)pas->pUserData;
            pThis = (Plugin_Commercial*)psp->pCommercial;

            pThis->pPlugins = psp;
            pThis->pCore = pas->pCore;

            pThis->Create();

            pas->hWnd = pThis->cMainWindow;
            pas->pUserData = pThis;

            break;
        }
    case LL_INITIALIZE:
        {
            pThis->Refresh();
            break;
        }
    case LL_DETACH:
        {
            pThis->Destroy();
            break;
        }
    }

    return;
}


HRESULT Plugin_Commercial::Create()
{
    HINSTANCE hInstance;
    RECT orect;
    RECT prect;

    /*
        Create our windows
    */

    hInstance = GetModuleHandle(NULL);

    SetRect(&prect, 0, 0, 100, 100);
    SetRect(&orect, 0, 0, 0, 0);
    this->cMainWindow.Create(L"Plugin_Commercial",
        Plugin_Commercial::WndProc, hInstance, prect, orect, this);

    // The segment list
    LIST_COLUMN c[10];
    SetRect(&prect, 0, 0, 100, 100);
    SetRect(&orect, 15, 15, -15, -42);
    this->cRanges.Create(this->cMainWindow, hInstance, prect, orect);
    c[COLUMN_PREVIEW].Set(L"Preview", LIST_LEFT, 100);
    c[COLUMN_IN].Set(L"Start", LIST_LEFT, 100);
    c[COLUMN_OUT].Set(L"End", LIST_LEFT, 100);
    c[COLUMN_SIZE].Set(L"Size", LIST_LEFT, 100);
    c[COLUMN_SEGMENTLENGTH].Set(L"Segment Length", LIST_LEFT, 100);
    c[COLUMN_ID].Set(L"Index", LIST_LEFT, 100);
    
    this->cRanges.ShowHeader(6, c);
    this->cImages.CreateEmpty(PREVIEW_SIZEX, PREVIEW_SIZEY, ILC_COLORDDB);
    this->cRanges.SetSmallImageList(&this->cImages);
    this->cRanges.SetStyle(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_CHILD | WS_VISIBLE);
    this->cRanges.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);

    // Buttons
    SetRect(&prect, 0, 100, 0, 100);
    SetRect(&orect, 15, -36, 90, -15);
    this->cAdd.Create(L"Add", this->cMainWindow, hInstance, prect, orect);

    SetRect(&prect, 0, 100, 0, 100);
    SetRect(&orect, 96, -36, 171, -15);
    this->cRemove.Create(L"Remove", this->cMainWindow, hInstance, prect, orect);

    SetRect(&prect, 0, 100, 0, 100);
    SetRect(&orect, 177, -36, 252, -15);
    this->cEdit.Create(L"Edit", this->cMainWindow, hInstance, prect, orect);

    return S_OK;
}


HRESULT Plugin_Commercial::Destroy()
{
    this->cAdd.Destroy();
    this->cEdit.Destroy();
    this->cRemove.Destroy();
    this->cRanges.Destroy();
    this->cMainWindow.Destroy();
    return S_OK;
}


HRESULT Plugin_Commercial::Refresh()
{
    LLSegment* pSegments = NULL;
    LLCommercial c;
    DWORD dwLongestLen;
    int nSegments;
    int i;
    int iItem;
    CCString s;
    int iImage;
    HBITMAP hbm;

    this->cRanges.Clear();
    this->cImages.Clear();

    if(FAILED(this->pCore->db.GetSegmentCount(&nSegments, &this->pCore->Log)))
    {
        return E_FAIL;
    }

    // Allocate the segments.
    pSegments = (LLSegment*)malloc(nSegments * sizeof(LLSegment));

    if(FAILED(this->pCore->db.GetSegmentData(pSegments, &this->pCore->Log)))
    {
        free(pSegments);
        return E_FAIL;
    }

    dwLongestLen = 0;

    // Determine the size (the very largest OUT value)
    for(i=0;i<nSegments;i++)
    {
        if(pSegments[i].dwOut > dwLongestLen)
        {
            if(pSegments[i].dwOut == INFINITE)
            {
                dwLongestLen = pSegments[i].dwIn + 300000;// Add an extra 5 minutes to it
            }
            else
            {
                dwLongestLen = pSegments[i].dwOut;// normal
            }
        }
    }

    for(i=0;i<nSegments;i++)
    {
        c.dwIn = pSegments[i].dwIn;
        if(pSegments[i].dwOut == INFINITE)
        {
            c.dwOut = pSegments[i].dwIn + 300000;
        }
        else
        {
            c.dwOut = pSegments[i].dwOut;
        }

        hbm = CreateCommercialPreviewImage(dwLongestLen, &c);
        this->cImages.AddBitmapFromHandle(&iImage, hbm);

        this->cRanges.AddItem(&iItem, pSegments[i].dwSize, iImage);

        LLFormatMilliseconds(pSegments[i].dwIn, &s);
        this->cRanges.AddItemText(iItem, COLUMN_IN, s);

        if(pSegments[i].dwOut == INFINITE)
        {
            s = L"(Infinite)";
        }
        else
        {
            LLFormatMilliseconds(pSegments[i].dwOut, &s);
        }
        this->cRanges.AddItemText(iItem, COLUMN_OUT, s);

        LLFormatMilliseconds(pSegments[i].dwSize, &s);
        this->cRanges.AddItemText(iItem, COLUMN_SIZE, s);

        if(pSegments[i].dwOut == INFINITE)
        {
            s = L"(n/a)";
        }
        else
        {
            LLFormatMilliseconds(pSegments[i].dwOut - pSegments[i].dwIn, &s);
        }
        this->cRanges.AddItemText(iItem, COLUMN_SEGMENTLENGTH, s);

        s.FromInt(i, 10);
        this->cRanges.AddItemText(iItem, COLUMN_ID, s);
    }

    free(pSegments);

    return S_OK;
}


void Plugin_Commercial::OnAdd()
{
    LLSegment s;
    s.dwIn = 0;
    s.dwOut = 0;
    s.dwSize = 0;

    if(FAILED(GetSegmentDataDlg(&s, this->cMainWindow)))
    {
        return;
    }

    // Add it to the database and the list.
    LLSegment* p = NULL;
    int n;

    if(FAILED(this->pCore->db.GetSegmentCount(&n, &this->pCore->Log)))
    {
        this->pCore->Log.AddEntry(L"New Segment: Error setting segment data");
        return;
    }

    p = (LLSegment*)malloc(sizeof(LLSegment) * (n + 1));

    if(FAILED(this->pCore->db.GetSegmentData(p, &this->pCore->Log)))
    {
        this->pCore->Log.AddEntry(L"New Segment: Error setting segment data");
        free(p);
        return;
    }

    p[n].dwIn = s.dwIn;
    p[n].dwOut = s.dwOut;
    p[n].dwSize = s.dwSize;
    n++;

    if(FAILED(this->pCore->db.SetSegmentData(n, p, &this->pCore->Log)))
    {
        this->pCore->Log.AddEntry(L"New Segment: Error setting segment data");
        free(p);
        return;
    }

    free(p);

    LL_DBUPDATE_STRUCT us;
    us.dwOperation = DBUPDATE_CHANGED;
    us.dwType = DBUPDATE_SEGMENT;
    this->pCore->SendPluginMessage(LL_DBUPDATE, &us);

    return;
}


void Plugin_Commercial::OnRemove()
{
    int iSegment;
    int iItem = this->cRanges.GetSelectionMark();
    if(iItem == -1) return;

    CCString s;

    this->cRanges.GetItemText(iItem, COLUMN_ID, &s);
    iSegment = s.ToInt(10);

    // Delete that one.
    LLSegment* p = NULL;
    int nSegments;

    if(FAILED(this->pCore->db.GetSegmentCount(&nSegments, &this->pCore->Log)))
    {
        return;
    }

    p = (LLSegment*)malloc(sizeof(LLSegment) * nSegments);

    if(FAILED(this->pCore->db.GetSegmentData(p, &this->pCore->Log)))
    {
        free(p);
        return;
    }

    // Close the gap.
    PVOID pFrom = &p[iSegment + 1];
    PVOID pTo = &p[iSegment];
    int nLen = (nSegments - (iSegment+1)) * sizeof(LLSegment);
    memmove(pTo, pFrom, nLen);

    nSegments --;

    if(FAILED(this->pCore->db.SetSegmentData(nSegments, p, &this->pCore->Log)))
    {
        free(p);
        return;
    }

    free(p);

    LL_DBUPDATE_STRUCT us;
    us.dwOperation = DBUPDATE_CHANGED;
    us.dwType = DBUPDATE_SEGMENT;
    this->pCore->SendPluginMessage(LL_DBUPDATE, &us);

    return;
}


void Plugin_Commercial::OnEdit()
{
    CCString s;
    LLSegment NewSegment;
    int iSegment;
    int iItem = this->cRanges.GetSelectionMark();

    if(iItem == -1) return;


    this->cRanges.GetItemText(iItem, COLUMN_ID, &s);
    iSegment = s.ToInt(10);

    // Delete that one.
    LLSegment* p = NULL;
    int nSegments;

    if(FAILED(this->pCore->db.GetSegmentCount(&nSegments, &this->pCore->Log)))
    {
        return;
    }

    p = (LLSegment*)malloc(sizeof(LLSegment) * nSegments);

    if(FAILED(this->pCore->db.GetSegmentData(p, &this->pCore->Log)))
    {
        free(p);
        return;
    }

    NewSegment.dwIn = p[iSegment].dwIn;
    NewSegment.dwOut = p[iSegment].dwOut;
    NewSegment.dwSize = p[iSegment].dwSize;

    if(FAILED(GetSegmentDataDlg(&NewSegment, this->cMainWindow)))
    {
        free(p);
        return;
    }

    p[iSegment].dwIn = NewSegment.dwIn;
    p[iSegment].dwOut = NewSegment.dwOut;
    p[iSegment].dwSize = NewSegment.dwSize;

    if(FAILED(this->pCore->db.SetSegmentData(nSegments, p, &this->pCore->Log)))
    {
        free(p);
        return;
    }

    LL_DBUPDATE_STRUCT us;
    us.dwOperation = DBUPDATE_CHANGED;
    us.dwType = DBUPDATE_SEGMENT;
    this->pCore->SendPluginMessage(LL_DBUPDATE, &us);

    free(p);
    return;
}







