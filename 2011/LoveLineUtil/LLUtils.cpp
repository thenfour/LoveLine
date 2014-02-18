

#include "pch.h"
#include "LLUtils.h"
#include "Resource.h"
#include "shlobj.h"


extern HINSTANCE g_hInstance;


/*
    First we look at the filename.  It can be in the form
    of YYYYMMDD or YYMMDD.
*/
void DateFromFileName(CCString sFileName, SYSTEMTIME* pTime)
{
    CCString s = sFileName;

    int i = 0;
    int nDigit = 0;
    WCHAR wcDigit;
    CCString Digits;

    sFileName.Path_StripToFilename();

    // Get all the digits.
    while(1)
    {
        wcDigit = s[i];
        if(wcDigit == 0) break;

        if(wcDigit >= L'0' && wcDigit <= L'9')
        {
            // It's a digit; append it.
            Digits += wcDigit;
        }

        i++;
    }

    // Determine whether the date is 8-digit format
    // or 6-digit format.
    // It's possible that there are other digits in the filename
    // that aren't relevant, so we will have to determine this by
    // a little simple AI.

    // If there aren't even 6 digits, use the filetime.
    if(Digits.len() < 6)
    {
        _GetFileTime(sFileName, pTime);
        return;
    }

    if(Digits.len() == 6) 
    {
        // We know it's 6-digit format.
        Decode6DigitFormat(Digits, pTime);
        return;
    }

    // The year must be between 1996 and 2018.
    // Because of this if the first 2 digits are above 18, it's 8-digit format.
    // Otherwise it's 6-digit format.
    CCString sFormat;
    int nFormat;
    Digits.Left(2, sFormat);
    nFormat = sFormat.ToInt(10);

    if(nFormat > 18)
    {
        // We know it's 8-digit format.
        Decode8DigitFormat(Digits, pTime);
        return;
    }

    // We know it's 6-digit format.
    Decode6DigitFormat(Digits, pTime);
    return;
}


HRESULT _GetFileTime(CCString sFileName, SYSTEMTIME* pTime)
{
    WIN32_FILE_ATTRIBUTE_DATA fad;
    ZeroMemory(pTime, sizeof(SYSTEMTIME));

    if(GetFileAttributesEx(sFileName, GetFileExInfoStandard, &fad) == 0)
    {
        // failed.
        return E_FAIL;
    }

    FileTimeToSystemTime(&fad.ftCreationTime, pTime);

    return S_OK;
}


void Decode8DigitFormat(CCString s, SYSTEMTIME* pTime)
{
    CCString Year;
    CCString Month;
    CCString Day;
    FILETIME ft;

    s.Left(4, Year);
    s.Mid(4, 2, Month);
    s.Mid(6, 2, Day);

    pTime->wDay = Day.ToInt(10);
    pTime->wMonth = Month.ToInt(10);
    pTime->wYear= Year.ToInt(10);
    pTime->wDayOfWeek = 0;// This can be calculated by converting this to a FILETIME struct and then back to a SYSTEMTIME.
    pTime->wHour = 0;
    pTime->wMinute = 0;
    pTime->wSecond = 0;
    pTime->wMilliseconds = 0;

    SystemTimeToFileTime(pTime, &ft);
    FileTimeToSystemTime(&ft, pTime);

    return;
}


void Decode6DigitFormat(CCString s, SYSTEMTIME* pTime)
{
    CCString Year;
    CCString Month;
    CCString Day;
    FILETIME ft;

    s.Left(2, Year);
    s.Mid(2, 2, Month);
    s.Mid(4, 2, Day);

    pTime->wDay = Day.ToInt(10);
    pTime->wMonth = Month.ToInt(10);
    pTime->wYear= Year.ToInt(10);
    pTime->wDayOfWeek = 0;// This can be calculated by converting this to a FILETIME struct and then back to a SYSTEMTIME.
    pTime->wHour = 0;
    pTime->wMinute = 0;
    pTime->wSecond = 0;
    pTime->wMilliseconds = 0;

    // Because we only had two digits for the year, adjust it.
    // The only valid dates are 1996 and 2018.
    // We'll assume that if the year is under 80 we'll use the 2000's.
    // Otherwise we'll add 1900.
    if(pTime->wYear < 80)
    {
        pTime->wYear += 2000;
    }
    else
    {
        pTime->wYear += 1900;
    }

    SystemTimeToFileTime(pTime, &ft);
    FileTimeToSystemTime(&ft, pTime);

    return;
}


HRESULT SaveFileDlg(HWND hWnd, CCString* FileName,
    int nFilters, FILE_FILTER* pFilters, CCString DefExt, PCWSTR wszInitialDir)
{
    OPENFILENAME of;
    WCHAR* wszFilter = NULL;

    wszFilter = ConvertFileFilter(nFilters, pFilters);

    ZeroMemory(&of, sizeof(of));
    of.lStructSize = sizeof(of);
    of.hInstance = GetModuleHandle(NULL);
    of.hwndOwner = hWnd;
    of.lpstrFilter = wszFilter;
    of.nFilterIndex = 0;
    of.lpstrFile = FileName->GetBuffer(MAX_PATH);
    of.nMaxFile = MAX_PATH;
    of.Flags = OFN_ENABLESIZING;
    of.lpstrDefExt = DefExt;

    if(wszInitialDir)
    {
        of.lpstrInitialDir = wszInitialDir;
    }

    if(GetSaveFileName(&of) == 0)
    {
        //The command either failed, or they hit cancel.
        free(wszFilter);
        *FileName = L"";
        return E_FAIL;
    }

    free(wszFilter);
    return S_OK;
}


HRESULT OpenFileDlg(HWND hWnd, CCString* FileName, 
    int nFilters, FILE_FILTER* pFilters, PCWSTR wszInitialDir)
{
    OPENFILENAME of;
    WCHAR* wszFilter = NULL;

    wszFilter = ConvertFileFilter(nFilters, pFilters);

    ZeroMemory(&of, sizeof(of));
    of.lStructSize = sizeof(of);
    of.hInstance = GetModuleHandle(NULL);
    of.hwndOwner = hWnd;
    of.lpstrFilter = wszFilter;
    of.nFilterIndex = 0;
    of.lpstrFile = FileName->GetBuffer(MAX_PATH);
    of.nMaxFile = MAX_PATH;
    of.Flags = OFN_ENABLESIZING | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    of.lpstrDefExt = L"";

    if(wszInitialDir)
    {
        of.lpstrInitialDir = wszInitialDir;
    }

    if(GetOpenFileName(&of) == 0)
    {
        //The command either failed, or they hit cancel.
        free(wszFilter);
        *FileName = L"";
        return E_FAIL;
    }

    free(wszFilter);
    return S_OK;
}


PWSTR ConvertFileFilter(int nFilters, FILE_FILTER* pFilters)
{
    WCHAR* wszFilter = NULL;
    DWORD dwFilterLen = 0;
    DWORD dwPos = 0;
    int i;

    // Construct the filter string.
    for(i=0;i<nFilters;i++)
    {
        dwFilterLen += sizeof(WCHAR) *
            (pFilters[i].Filter.len() +
            pFilters[i].Name.len() +
            2);
    }

    dwFilterLen += sizeof(WCHAR);// For the final terminator.

    wszFilter = (WCHAR*)malloc(dwFilterLen);

    for(i=0;i<nFilters;i++)
    {
        wcscpy(wszFilter + dwPos, pFilters[i].Name);
        dwPos += pFilters[i].Name.len() + 1;
        wcscpy(wszFilter + dwPos, pFilters[i].Filter);
        dwPos += pFilters[i].Filter.len() + 1;
    }

    *(wszFilter + dwPos) = 0;// The final terminator.

    return wszFilter;
}


// Converts a systemtime struct to a date
// Like Mon, Nov. 11, 2001
HRESULT LLFormatDate(SYSTEMTIME* pst, CCString* pFormat)
{
    if(GetDateFormat(LOCALE_USER_DEFAULT, NULL, pst, 
        L"ddd',' MMM'.' dd',' yyyy",
        pFormat->GetBuffer(1024), 1024) == 0)
    {
        *pFormat = L"(unknown date)";
    }

    return S_OK;
}

HRESULT SystemTimeToLLTime(const SYSTEMTIME* pst, LLTIME* pTime)
{
    *pTime = MAKELLTIME(pst->wYear, pst->wMonth, pst->wDay);
    return S_OK;
}


HRESULT LLTimeToSystemTime(SYSTEMTIME* pst, const LLTIME pTime)
{
    FILETIME ft;

    pst->wDayOfWeek = 0;
    pst->wDay = DAYFROMLLTIME(pTime);
    pst->wHour = 0;
    pst->wMilliseconds = 0;
    pst->wMinute = 0;
    pst->wMonth = MONTHFROMLLTIME(pTime);
    pst->wSecond = 0;
    pst->wYear = YEARFROMLLTIME(pTime);

    SystemTimeToFileTime(pst, &ft);
    FileTimeToSystemTime(&ft, pst);

    return S_OK;
}


HRESULT LLFormatDate(LLEpisode* pEpisode, CCString* pFormat)
{
    SYSTEMTIME st;

    st.wDay = (WORD)pEpisode->dwDay;
    st.wMonth = (WORD)pEpisode->dwMonth;
    st.wYear = (WORD)pEpisode->dwYear;
    return LLFormatDate(&st, pFormat);
}


HRESULT LLFormatMilliseconds(DWORD dwMS, CCString* pSeconds)
{
    // There are    1000 ms/seconds
    // There are   60000 ms/minute
    // There are 3600000 ms/hour

    int nHours = dwMS / 3600000;
    dwMS -= nHours * 3600000;

    int nMinutes = dwMS / 60000;
    dwMS -= nMinutes * 60000;

    int nSeconds = dwMS / 1000;
    dwMS -= nSeconds * 1000;

    pSeconds->FormatW(L"%02d:%02d:%02d", nHours, nMinutes, nSeconds);

    return S_OK;
}


BOOL PtInSegment(DWORD dwPos, LLSegment* p)
{
    if(dwPos >= p->dwIn)
    {
        if(dwPos < p->dwOut)
        {
            return TRUE;
        }
    }

    return FALSE;
}


DWORD GetCommercialLength(LLDatabase* pdb, DWORD dwTime, CCLog* pLog)
{
    int nSegments = 0;
    LLSegment* pSegments = NULL;
    int i;

    if(FAILED(pdb->GetSegmentCount(&nSegments, pLog)))
    {
        pLog->AddEntry(L"Database Error while calculating commercial length.  Using default commercial length.");
        return MAKEMS(0,4,27);
    }

    pSegments = (LLSegment*)malloc(nSegments * sizeof(LLSegment));

    if(FAILED(pdb->GetSegmentData(pSegments, pLog)))
    {
        pLog->AddEntry(L"Database Error while calculating commercial length.  Using default commercial length.");
        free(pSegments);
        return MAKEMS(0,4,27);
    }

    for(i=0;i<nSegments;i++)
    {
        if(PtInSegment(dwTime, &pSegments[i]) == TRUE)
        {
            DWORD dw = pSegments[i].dwSize;
            free(pSegments);
            return dw;
        }
    }

    free(pSegments);

    return MAKEMS(0,4,27);
}


struct LLUTILS_NEWFEATUREINFO
{
    BOOL bOK;// TRUE when the user clicks OK.  False when they hit CANCEL.
    CCString* pTitle;
    CCString* pDesc;
};


INT_PTR CALLBACK _LLUtils_NewFeatureDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LLUTILS_NEWFEATUREINFO* pVars = (LLUTILS_NEWFEATUREINFO*)GetWindowLongPtr(hWnd, GWL_USERDATA);

    switch(uMsg)
    {
    case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
            case IDOK:
                {
                    int n;
                    HWND hTitle = GetDlgItem(hWnd, IDC_TITLE);
                    HWND hDesc = GetDlgItem(hWnd, IDC_DESCRIPTION);

                    n = GetWindowTextLength(hTitle);
                    GetWindowText(hTitle, pVars->pTitle->GetBuffer(n + 1), n + 1);

                    n = GetWindowTextLength(hDesc);
                    GetWindowText(hDesc, pVars->pDesc->GetBuffer(n + 1), n + 1);

                    pVars->bOK = TRUE;

                    EndDialog(hWnd, 0);
                    return TRUE;
                }
            case IDCANCEL:
                {
                    pVars->bOK = FALSE;

                    EndDialog(hWnd, 0);
                    return TRUE;
                }
            }
            break;
        }
    case WM_CLOSE:
        {
            EndDialog(hWnd, 0);
            return TRUE;
        }
    case WM_INITDIALOG:
        {
            pVars = (LLUTILS_NEWFEATUREINFO*)lParam;
            SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)pVars);

            HWND hTitle = GetDlgItem(hWnd, IDC_TITLE);
            HWND hDesc = GetDlgItem(hWnd, IDC_DESCRIPTION);

            SetWindowText(hTitle, *pVars->pTitle);
            SetWindowText(hDesc, *pVars->pDesc);

            SetFocus(hTitle);

            pVars->bOK = FALSE;
            return FALSE;
        }
    }

    return FALSE;
}


HRESULT GetNewFeatureInfo(HWND hParent, CCString* pTitle, CCString* pDesc)
{
    LLUTILS_NEWFEATUREINFO Info;
    Info.bOK = FALSE;
    Info.pDesc = pDesc;
    Info.pTitle = pTitle;

    DialogBoxParam(g_hInstance, MAKEINTRESOURCE(IDD_NEWFEATURE),
        hParent, _LLUtils_NewFeatureDlgProc, (LPARAM)&Info);

    return Info.bOK == TRUE ? S_OK : E_FAIL;
}


inline int LLTimeToPixels(int nPixelSize, DWORD dwLength, DWORD dwPos)
{
    int n;
    if(dwPos > dwLength) return nPixelSize;
    if(dwLength == 0) return 0;
    n = (nPixelSize * dwPos) / dwLength;
    return n;
}


HBITMAP CreateCommercialPreviewImage(DWORD dwEpisodeLen, LLCommercial* pc)
{
    HBITMAP hbm;
    int nIn;
    RECT rc;
    int nOut;
    HDC dc;

    nIn = LLTimeToPixels(PREVIEW_SIZEX, dwEpisodeLen, pc->dwIn);
    nOut = LLTimeToPixels(PREVIEW_SIZEX, dwEpisodeLen, pc->dwOut);

    hbm = CreateBlankPreviewImage();
    if(hbm == 0)
    {
        __asm int 3;
    }

    dc = CreateCompatibleDC(NULL);
    if(dc == 0)
    {
        __asm int 3;
    }

    HBRUSH hCommercial = CreateSolidBrush(RGB(0,0,212));
    SelectObject(dc, hbm);

    SetRect(&rc, nIn, 0, nOut, PREVIEW_SIZEY);
    FillRect(dc, &rc, hCommercial);

    DeleteObject(hCommercial);
    DeleteDC(dc);

    return hbm;
}


HBITMAP CCEXPORT CreateCursorPreviewImage(DWORD dwEpisodeLen, LLLink* pl)
{
    HBITMAP hbm;
    int nIn;
    RECT rc;
    HDC dc;

    nIn = LLTimeToPixels(PREVIEW_SIZEX, dwEpisodeLen, pl->dwPos);

    hbm = CreateBlankPreviewImage();
    if(hbm == 0)
    {
        return NULL;
    }

    dc = CreateCompatibleDC(NULL);
    if(dc == 0)
    {
        return NULL;
    }

    HBRUSH hCursor = CreateSolidBrush(RGB(0, 100, 255));
    SelectObject(dc, hbm);

    SetRect(&rc, nIn-1, 0, nIn+1, PREVIEW_SIZEY);
    FillRect(dc, &rc, hCursor);

    DeleteObject(hCursor);
    DeleteDC(dc);

    return hbm;
}


HBITMAP CreateLinkPreviewImage(DWORD dwEpisodeLen, LLLink* pl)
{
    HBITMAP hbm;
    int nIn;
    RECT rc;
    HDC dc;
    POINT p[4];// For the cool triangle.

    nIn = LLTimeToPixels(PREVIEW_SIZEX, dwEpisodeLen, pl->dwPos);

    hbm = CreateBlankPreviewImage();
    if(hbm == 0)
    {
        return NULL;
    }

    dc = CreateCompatibleDC(NULL);
    if(dc == 0)
    {
        return NULL;
    }

    HBRUSH hLink = CreateSolidBrush(RGB(255,0,0));
    SelectObject(dc, hbm);

    SetRect(&rc, nIn, 0, nIn+1, PREVIEW_SIZEY);
    FillRect(dc, &rc, hLink);

    p[0].x = nIn;
    p[0].y = PREVIEW_HANDLESIZE;
    p[1].x = nIn - PREVIEW_HANDLESIZE;
    p[1].y = 0;
    p[2].x = nIn + PREVIEW_HANDLESIZE + 1;
    p[2].y = 0;
    p[3].x = nIn + 1;
    p[3].y = PREVIEW_HANDLESIZE;
    HRGN hTriangle = CreatePolygonRgn(p, 4, WINDING);
    FillRgn(dc, hTriangle, hLink);
    DeleteObject(hTriangle);

    DeleteObject(hLink);
    DeleteDC(dc);

    return hbm;
}


HBITMAP CreateBlankPreviewImage()
{
    HDC dc;
    RECT rc;
    HBITMAP hbm;

    HBRUSH hBackground = CreateSolidBrush(RGB(212,230,255));
    HBRUSH hBlueLine = CreateSolidBrush(RGB(0, 150, 255));

    /*
        Create a cute commercial bitmap
    */
    HDC hScreenDC = GetDC(NULL);

    hbm = CreateCompatibleBitmap(hScreenDC, PREVIEW_SIZEX, PREVIEW_SIZEY);
    if(!hbm)
    {
        return NULL;
    }

    ReleaseDC(NULL, hScreenDC);

    dc = CreateCompatibleDC(NULL);
    if(!dc)
    {
        return NULL;
    }
    SelectObject(dc, hbm);

    // Draw on the bitmap
    SetRect(&rc, 0, 0, PREVIEW_SIZEX, PREVIEW_SIZEY);
    FillRect(dc, &rc, hBackground);
    SetRect(&rc, 0, PREVIEW_SIZEY/2, PREVIEW_SIZEX, PREVIEW_SIZEY/2 +1);
    FillRect(dc, &rc, hBlueLine);

    DeleteDC(dc);
    DeleteObject(hBlueLine);
    DeleteObject(hBackground);

    return hbm;
}


HRESULT GetControlRect(HWND hControl, RECT* prc)
{
    POINT* p1 = (POINT*)prc;
    POINT* p2 = ((POINT*)prc) + 1;

    GetWindowRect(hControl, prc);

    ScreenToClient(GetParent(hControl), p1);
    ScreenToClient(GetParent(hControl), p2);
    return S_OK;
}


HRESULT LoadListViewSettings(CCString SettingName, CCListBox* pl, LLDatabase* pdb, CCLog* pLog)
{
    int nColumns = pl->GetColumnCount();
    int nSize;
    LIST_COLUMNORDER* po = NULL;
    CCString sModifiedSettingName;
    CCString sVal;

    // Do the last column selected.
    sModifiedSettingName = SettingName;
    sModifiedSettingName += L"_SortColumn";
    if(SUCCEEDED(pdb->GetSetting(sModifiedSettingName, &sVal, pLog)))
    {
        pl->SimulateColumnClick(sVal.ToInt(10));
    }

    // Do the order and size array.
    sModifiedSettingName = SettingName;
    sModifiedSettingName += L"_ColumnSettings";

    nSize = sizeof(LIST_COLUMNORDER) * nColumns;
    po = (LIST_COLUMNORDER*)malloc(nSize);
    //if(po == NULL) return E_FAIL;

    if(SUCCEEDED(pdb->GetSetting(sModifiedSettingName, &sVal, pLog)))
    {
        if(sVal.ToBinary(po, nSize) == nSize)
        {
            pl->SetColumnIndexesAndSizes(po);
        }
    }

	free(po);

    return S_OK;
}


HRESULT SaveListViewSettings(CCString SettingName, CCListBox* pl, LLDatabase* pdb, CCLog* pLog)
{
    int nColumns = pl->GetColumnCount();
    int nSize;
    LIST_COLUMNORDER* po = NULL;
    CCString sModifiedSettingName;
    CCString sVal;

    // Do the last column selected.
    sModifiedSettingName = SettingName;
    sModifiedSettingName += L"_SortColumn";
    sVal.FromInt(pl->GetLastColumnClicked(), 10);
    pdb->SetSetting(sModifiedSettingName, sVal, pLog);

    // Do the order and size array.
    sModifiedSettingName = SettingName;
    sModifiedSettingName += L"_ColumnSettings";

    nSize = sizeof(LIST_COLUMNORDER) * nColumns;
    po = (LIST_COLUMNORDER*)malloc(nSize);
    if(po == NULL) return E_FAIL;

    if(pl->GetColumnIndexesAndSizes(po) == -1)
	{
		free(po);
		return E_FAIL;
	}

    sVal.FromBinary(po, nSize);
    pdb->SetSetting(sModifiedSettingName, sVal, pLog);

	free(po);
    return S_OK;
}


struct LLUTILS_SEGMENTDATADLG
{
    BOOL bOK;// TRUE when the user clicks OK.  False when they hit CANCEL.
    LLSegment* ps;
};


INT_PTR CALLBACK _LLUtils_SegmentDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LLUTILS_SEGMENTDATADLG* pVars = (LLUTILS_SEGMENTDATADLG*)GetWindowLongPtr(hWnd, GWL_USERDATA);

    switch(uMsg)
    {
    case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
            case IDC_INFINITY:
                {
                    BOOL b = FALSE;
                    if(IsDlgButtonChecked(hWnd, IDC_INFINITY) == BST_CHECKED)
                    {
                        b = TRUE;
                    }
                    SendMessage(GetDlgItem(hWnd, IDC_OUTHOURS), EM_SETREADONLY, b, 0);
                    SendMessage(GetDlgItem(hWnd, IDC_OUTMINUTES), EM_SETREADONLY, b, 0);
                    SendMessage(GetDlgItem(hWnd, IDC_OUTSECONDS), EM_SETREADONLY, b, 0);
                    return TRUE;
                }
            case IDOK:
                {
                    pVars->bOK = TRUE;

                    // Grab values.
                    pVars->ps->dwIn = MAKEMS(
                        GetDlgItemInt(hWnd, IDC_INHOURS, NULL, FALSE),
                        GetDlgItemInt(hWnd, IDC_INMINUTES, NULL, FALSE),
                        GetDlgItemInt(hWnd, IDC_INSECONDS, NULL, FALSE));

                    if(IsDlgButtonChecked(hWnd, IDC_INFINITY) == BST_CHECKED)
                    {
                        pVars->ps->dwOut = INFINITE;
                    }
                    else
                    {
                        pVars->ps->dwOut = MAKEMS(
                            GetDlgItemInt(hWnd, IDC_OUTHOURS, NULL, FALSE),
                            GetDlgItemInt(hWnd, IDC_OUTMINUTES, NULL, FALSE),
                            GetDlgItemInt(hWnd, IDC_OUTSECONDS, NULL, FALSE));
                    }

                    pVars->ps->dwSize = MAKEMS(
                        GetDlgItemInt(hWnd, IDC_COMMERCIALHOURS, NULL, FALSE),
                        GetDlgItemInt(hWnd, IDC_COMMERCIALMINUTES, NULL, FALSE),
                        GetDlgItemInt(hWnd, IDC_COMMERCIALSECONDS, NULL, FALSE));

                    // Fix bounds.
                    if(pVars->ps->dwOut < pVars->ps->dwIn)
                    {
                        DWORD dwTemp;
                        dwTemp = pVars->ps->dwIn;
                        pVars->ps->dwIn = pVars->ps->dwOut;
                        pVars->ps->dwOut = dwTemp;
                    }

                    EndDialog(hWnd, 0);
                    return TRUE;
                }
            case IDCANCEL:
                {
                    pVars->bOK = FALSE;
                    EndDialog(hWnd, 0);
                    return TRUE;
                }
            }
            break;
        }
    case WM_CLOSE:
        {
            EndDialog(hWnd, 0);
            return TRUE;
        }
    case WM_INITDIALOG:
        {
            pVars = (LLUTILS_SEGMENTDATADLG*)lParam;
            SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)pVars);

            CCString s;
            s.FromInt(HOURSFROMMS(pVars->ps->dwIn), 10);
            SetWindowText(GetDlgItem(hWnd, IDC_INHOURS), s);

            s.FromInt(MINUTESFROMMS(pVars->ps->dwIn), 10);
            SetWindowText(GetDlgItem(hWnd, IDC_INMINUTES), s);

            s.FromInt(SECONDSFROMMS(pVars->ps->dwIn), 10);
            SetWindowText(GetDlgItem(hWnd, IDC_INSECONDS), s);

            if(pVars->ps->dwOut == INFINITE)
            {
                CheckDlgButton(hWnd, IDC_INFINITY, BST_CHECKED);
                // Make edit boxes invalid.
                SendMessage(GetDlgItem(hWnd, IDC_OUTHOURS), EM_SETREADONLY, TRUE, 0);
                SendMessage(GetDlgItem(hWnd, IDC_OUTMINUTES), EM_SETREADONLY, TRUE, 0);
                SendMessage(GetDlgItem(hWnd, IDC_OUTSECONDS), EM_SETREADONLY, TRUE, 0);
            }
            else
            {
                CheckDlgButton(hWnd, IDC_INFINITY, BST_UNCHECKED);
                SendMessage(GetDlgItem(hWnd, IDC_OUTHOURS), EM_SETREADONLY, FALSE, 0);
                SendMessage(GetDlgItem(hWnd, IDC_OUTMINUTES), EM_SETREADONLY, FALSE, 0);
                SendMessage(GetDlgItem(hWnd, IDC_OUTSECONDS), EM_SETREADONLY, FALSE, 0);

                s.FromInt(HOURSFROMMS(pVars->ps->dwOut), 10);
                SetWindowText(GetDlgItem(hWnd, IDC_OUTHOURS), s);

                s.FromInt(MINUTESFROMMS(pVars->ps->dwOut), 10);
                SetWindowText(GetDlgItem(hWnd, IDC_OUTMINUTES), s);

                s.FromInt(SECONDSFROMMS(pVars->ps->dwOut), 10);
                SetWindowText(GetDlgItem(hWnd, IDC_OUTSECONDS), s);
            }


            s.FromInt(HOURSFROMMS(pVars->ps->dwSize), 10);
            SetWindowText(GetDlgItem(hWnd, IDC_COMMERCIALHOURS), s);

            s.FromInt(MINUTESFROMMS(pVars->ps->dwSize), 10);
            SetWindowText(GetDlgItem(hWnd, IDC_COMMERCIALMINUTES), s);

            s.FromInt(SECONDSFROMMS(pVars->ps->dwSize), 10);
            SetWindowText(GetDlgItem(hWnd, IDC_COMMERCIALSECONDS), s);

            pVars->bOK = FALSE;
            return TRUE;
        }
    }

    return FALSE;
}


HRESULT GetSegmentDataDlg(IN OUT LLSegment* ps, HWND hParent)
{
    LLUTILS_SEGMENTDATADLG Vars;
    Vars.ps = ps;
    Vars.bOK = FALSE;

    DialogBoxParam(g_hInstance, MAKEINTRESOURCE(IDD_NEWSEGMENT), hParent,
        _LLUtils_SegmentDlgProc, (LPARAM)&Vars);

    if(Vars.bOK == FALSE) return E_FAIL;

    return S_OK;
}


HRESULT CCEXPORT LLBrowseForFolder(HWND hWnd, CCString& sOut)
{
    BROWSEINFO bi = {0};
    IMalloc* pMalloc = 0;
    LPITEMIDLIST pidl = 0;
    HRESULT hr = S_FALSE;
    WCHAR wszPath[MAX_PATH];

    bi.hwndOwner = hWnd;
    bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
    pidl = SHBrowseForFolder(&bi);

    if(pidl)
    {
        if(TRUE == SHGetPathFromIDList(pidl, wszPath))
        {
            sOut = wszPath;
            hr = S_OK;
        }

        SHGetMalloc(&pMalloc);
        if(pMalloc)
        {
            pMalloc->Free(pidl);
            SAFE_RELEASE(pMalloc);
        }
    }

    return hr;
}


