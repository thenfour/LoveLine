

#include "stdafx.h"
#include "P_WavUtils.h"
#include "Resource.h"
#include "../LoveLineUtil/wavreader.h"
#include "../LoveLineUtil/mp3writer.h"
#include <shlobj.h>
#include <process.h>


#define COLUMN_DATE     0
#define COLUMN_GUESTS   1
#define COLUMN_FILENAME 2
#define COLUMN_GUID     3

#define COLUMN_TITLE    0
#define COLUMN_TIME     1
#define COLUMN_FEATURE_ID 2

#define WAVUTILS_COMMERCIALS L"WavUtils_Commercials"
#define WAVUTILS_COMPRESS    L"WavUtils_Compress"
#define WAVUTILS_LENGTH      L"WavUtils_Length"
#define WAVUTILS_PATH        L"WavUtils_Path"
#define WAVUTILS_FILENAME    L"WavUtils_Filename"
#define WAVUTILS_BITRATE     L"WavUtils_Bitrate"

#define LISTVIEW_SETTINGS_WAVUTILS_EPISODES L"WavUtils_Episodes"
#define LISTVIEW_SETTINGS_WAVUTILS_LINKS    L"WavUtils_Links"

#define WAVUTILS_BUFFER_MS   30000


bool PWU_IsValidBitRate(DWORD dw)
{
    switch(dw)
    {
    case 16:
    case 24:
    case 32:
    case 48:
    case 56:
    case 64:
    case 80:
    case 96:
    case 112:
    case 128:
    case 160:
    case 192:
    case 224:
    case 256:
    case 320:
        return true;
    }

    return false;
}


class CVerticalSpacer
{
public:
    CVerticalSpacer() :
        top(m_top),
        bottom(m_bottom),
        m_top(0),
        m_bottom(0),
        m_last_control(UNKNOWN)
    {
    }

    const int& top;
    const int& bottom;

    static const int _StaticToEditSpacer = 3;
    static const int _StaticHeight = 12;
    static const int _EditToStaticSpacer = 6;
    static const int _EditHeight = 23;

    // starts the thing off with a static control
    void Static(int nTop = -1)
    {
        if(nTop != -1)
        {
            // starting over.
            m_top = nTop;
        }
        else
        {
            m_top = bottom + _EditToStaticSpacer;
        }

        m_bottom = top + _StaticHeight;
        m_last_control = STATIC;
    }

    void Edit(int nTop = -1)
    {
        if(nTop != -1)
        {
            // starting over.
            m_top = nTop;
        }
        else
        {
            m_top = bottom + _StaticToEditSpacer;
        }

        m_bottom = top + _EditHeight;
        m_last_control = EDIT;
    }

private:
    int m_top;
    int m_bottom;

    enum last_control_t
    {
        UNKNOWN,
        EDIT,
        STATIC
    };

    last_control_t m_last_control;
};


extern HINSTANCE g_hInstance;


Plugin_WavUtils::Plugin_WavUtils()
{
    m_pCore = NULL;
    m_pPlugins = NULL;
    m_hThread = 0;
}


Plugin_WavUtils::~Plugin_WavUtils()
{
}


LPARAM CALLBACK Plugin_WavUtils::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Plugin_WavUtils* pThis = (Plugin_WavUtils*)GetWindowLongPtr(hWnd, GWL_USERDATA);

    switch(uMsg)
    {
    case WM_NOTIFY:
        {
            NMHDR* pnmh = (NMHDR*)lParam;
            NMITEMACTIVATE* pia = NULL;
            NMLISTVIEW* plv = NULL;
            int iColumn;

            if((iColumn = pThis->m_cEpisodeList.IsSortNotify(lParam)) != -1)
            {
                if(iColumn == COLUMN_DATE)
                {
                    pThis->m_cEpisodeList.SortByLParam(pThis, Plugin_WavUtils::CompareFunc);
                }
                else
                {
                    pThis->m_cEpisodeList.SortTextAutomatically(iColumn);
                }

                return TRUE;
            }

            if((iColumn = pThis->m_cListLinks.IsSortNotify(lParam)) != -1)
            {
                if(iColumn == COLUMN_TIME)
                {
                    pThis->m_cListLinks.SortByLParam(pThis, Plugin_WavUtils::CompareFunc);
                }
                else
                {
                    pThis->m_cListLinks.SortTextAutomatically(iColumn);
                }

                return TRUE;
            }

            if(plv = pThis->m_cEpisodeList.IsSelectedNotify(lParam))
            {
                pThis->m_cEpisodeList.SetSelectionMark(plv->iItem);
                pThis->OnSelect();
                return TRUE;
            }

            break;
        }
    case WM_COMMAND:
        {
            UINT uId = LOWORD(wParam);

            if(uId == pThis->m_cBrowse.GetId())
            {
                pThis->OnBrowse();
                return TRUE;
            }
            else if(uId == pThis->m_cMP3.GetId())
            {
                // toggle the old bitrate thingy
                if(pThis->m_cMP3.GetCheck())
                {
                    pThis->m_cBitrate.SetReadOnly(FALSE);
                }
                else
                {
                    pThis->m_cBitrate.SetReadOnly(TRUE);
                }
            }
            else if(uId == pThis->m_cLegend.GetId())
            {
                DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_LEGEND), pThis->m_cMainWindow, Plugin_WavUtils::_LegendProc);
                return TRUE;
            }
            else if(uId == pThis->m_cStart.GetId())
            {
                pThis->OnStart();
                return TRUE;
            }
            else if(uId == pThis->m_cClear.GetId())
            {
                pThis->m_cLog.SetText(L"");
                return TRUE;
            }
            else if(uId == pThis->m_cCancel.GetId())
            {
                SetEvent(pThis->m_hCancelEvent);
                return TRUE;
            }

            break;
        }
    case WM_SIZE:
        {
            //pThis->m_cMainWindow.Size();
            pThis->m_cEpisodeList.Size();
            pThis->m_cGroupEpisodes.Size();
            pThis->m_cListLinks.Size();
            pThis->m_cGroupEpisodeDetails.Size();
            pThis->m_cCommercials.Size();
            pThis->m_cOutput.Size();
            pThis->m_cBrowse.Size();
            pThis->m_cSegments.Size();
            pThis->m_cBitrate.Size();
            pThis->m_cStart.Size();
            pThis->m_cFilename.Size();
            pThis->m_cLegend.Size();
            pThis->m_cMP3.Size();

            pThis->m_csFilename.Size();
            pThis->m_csOutput.Size();
            pThis->m_csSegments.Size();
            pThis->m_csBitrate.Size();
            pThis->m_cProgress.Size();
            pThis->m_cLog.Size();
            pThis->m_cClear.Size();
            pThis->m_cCancel.Size();
            return 0;
        }
    case WM_CREATE:
        {
            CREATESTRUCT* pcs = (CREATESTRUCT*)lParam;
            pThis = (Plugin_WavUtils*)pcs->lpCreateParams;
            SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)pThis);
            return 0;
        }
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


void CALLBACK Plugin_WavUtils::NotificationProc(PVOID pUser, DWORD dwMsg, PVOID pParams)
{
    Plugin_WavUtils* pThis = (Plugin_WavUtils*)pUser;
    switch(dwMsg)
    {
    case LL_DBUPDATE:
        {
            LL_DBUPDATE_STRUCT* pus = (LL_DBUPDATE_STRUCT*)pParams;
            if(pus->dwType & (DBUPDATE_EPISODE | DBUPDATE_LINK | DBUPDATE_FEATURE))
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
            pThis = (Plugin_WavUtils*)psp->pWavUtils;

            pThis->m_pPlugins = psp;
            pThis->m_pCore = pas->pCore;

            pThis->Create();

            pas->hWnd = pThis->m_cMainWindow;
            pas->pUserData = pThis;

            break;
        }
    case LL_INITIALIZE:
        {
            SaveListViewSettings(LISTVIEW_SETTINGS_WAVUTILS_EPISODES, &pThis->m_cEpisodeList, &pThis->m_pCore->db, &pThis->m_pCore->Log);
            SaveListViewSettings(LISTVIEW_SETTINGS_WAVUTILS_LINKS, &pThis->m_cListLinks, &pThis->m_pCore->db, &pThis->m_pCore->Log);

            CCString s;

            if(FAILED(pThis->m_pCore->db.GetSetting(WAVUTILS_PATH, &s, &pThis->m_pCore->Log)))
            {
                s = L"";
            }
            pThis->m_cOutput.SetText(s);

            if(FAILED(pThis->m_pCore->db.GetSetting(WAVUTILS_LENGTH, &s, &pThis->m_pCore->Log)))
            {
                s = L"4";
            }
            pThis->m_cSegments.SetText(s);

            if(FAILED(pThis->m_pCore->db.GetSetting(WAVUTILS_FILENAME, &s, &pThis->m_pCore->Log)))
            {
                s = L"%Y-%m-%d\\%n.wav";
            }
            pThis->m_cFilename.SetText(s);

            if(FAILED(pThis->m_pCore->db.GetSetting(WAVUTILS_BITRATE, &s, &pThis->m_pCore->Log)))
            {
                s = L"32";
            }
            pThis->m_cBitrate.SetText(s);

            if(FAILED(pThis->m_pCore->db.GetSetting(WAVUTILS_COMMERCIALS, &s, &pThis->m_pCore->Log)))
            {
                s = L"1";
            }
            pThis->m_cCommercials.SetCheck((!s.cmp(L"1")) ? true : false);

            if(FAILED(pThis->m_pCore->db.GetSetting(WAVUTILS_COMPRESS, &s, &pThis->m_pCore->Log)))
            {
                s = L"1";
            }
            bool bChecked = (!s.cmp(L"1")) ? true : false;
            pThis->m_cMP3.SetCheck(bChecked);
            if(bChecked)
            {
                pThis->m_cBitrate.SetReadOnly(FALSE);
            }
            else
            {
                pThis->m_cBitrate.SetReadOnly(TRUE);
            }

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


HRESULT Plugin_WavUtils::Create()
{
    RECT prect, orect;
    HINSTANCE hInstance = GetModuleHandle(NULL);
    LIST_COLUMN c[10];

    CVerticalSpacer v;

    // Create controls.
    SetRect(&prect, 0, 0, 100, 100);
    SetRect(&orect, 0, 0, 0, 0);
    m_cMainWindow.Create(L"Plugin_WavUtils", Plugin_WavUtils::WndProc, GetModuleHandle(NULL), prect, orect, this);

    // The "Episodes" Group box
    SetRect(&prect, 0, 0, 50, 100);
    SetRect(&orect, 6, 6, -3, -6);
    m_cGroupEpisodes.CreateGroup(
        L"Episodes", m_cMainWindow, hInstance, prect, orect);

    // Episode ListView
    SetRect(&prect, 0, 0, 50, 70);
    SetRect(&orect, 15, 21, -12, 0);
    m_cEpisodeList.Create(m_cMainWindow, hInstance, prect, orect);

    c[COLUMN_DATE].Set(L"Date", LIST_LEFT, 100);
    c[COLUMN_GUESTS].Set(L"Guests", LIST_LEFT, 100);
    c[COLUMN_FILENAME].Set(L"FileName", LIST_LEFT, 100);
    c[COLUMN_GUID].Set(L"ID", LIST_LEFT, 100);
    m_cEpisodeList.ShowHeader(4, c);

    m_cEpisodeList.SetStyle(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_CHILD | WS_VISIBLE);
    m_cEpisodeList.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);

    // The RIGHT group box
    SetRect(&prect, 50, 0, 100, 100);
    SetRect(&orect, 3, 6, -6, -6);
    this->m_cGroupEpisodeDetails.CreateGroup(
        L"", this->m_cMainWindow, hInstance, prect, orect);

    // Links ListView
    SetRect(&prect, 0, 70, 50, 100);
    SetRect(&orect, 15, 6, -12, -15);
    this->m_cListLinks.Create(this->m_cMainWindow, hInstance, prect, orect);

    c[COLUMN_TITLE].Set(L"Feature Name", LIST_LEFT, 100);
    c[COLUMN_TIME].Set(L"Time", LIST_LEFT, 100);
    c[COLUMN_FEATURE_ID].Set(L"ID", LIST_LEFT, 100);
    this->m_cListLinks.ShowHeader(3, c);

    this->m_cListLinks.SetStyle(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_CHILD | WS_VISIBLE);
    this->m_cListLinks.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);

    // Commercials button
    v.Edit(21);
    SetRect(&prect, 50, 0, 75, 0);
    SetRect(&orect, 15, v.top, 0, v.bottom);
    m_cCommercials.CreateCheck(L"Extract commercials?", m_cMainWindow, prect, orect);

    // Output to MP3 button
    SetRect(&prect, 75, 0, 100, 0);
    SetRect(&orect, 0, v.top, -12, v.bottom);
    m_cMP3.CreateCheck(L"Compress?", m_cMainWindow, prect, orect);

    // m_csOutput
    v.Static();
    SetRect(&prect, 50, 0, 100, 0);
    SetRect(&orect, 15, v.top, -32, v.bottom);
    m_csOutput.Create(L"Output directory", m_cMainWindow, hInstance, prect, orect);

    // m_cOutput
    v.Edit();
    SetRect(&prect, 50, 0, 100, 0);
    SetRect(&orect, 15, v.top, -32, v.bottom);
    m_cOutput.Create(m_cMainWindow, hInstance, prect, orect, true);
    m_cOutput.SetReadOnly(FALSE);

    // m_cBrowse
    SetRect(&prect, 100, 0, 100, 0);
    SetRect(&orect, -32, v.top, -12, v.bottom);
    m_cBrowse.Create(L"...", m_cMainWindow, 0, prect, orect);

    // m_csSegments
    v.Static();
    SetRect(&prect, 50, 0, 75, 0);
    SetRect(&orect, 15, v.top, 0, v.bottom);
    m_csSegments.Create(L"Segment length (in seconds)", m_cMainWindow, hInstance, prect, orect);

    // m_csBitrate
    SetRect(&prect, 75, 0, 100, 0);
    SetRect(&orect, 0, v.top, -12, v.bottom);
    m_csBitrate.Create(L"Bitrate (in kbps)", m_cMainWindow, hInstance, prect, orect);

    // m_cSegments
    v.Edit();
    SetRect(&prect, 50, 0, 75, 0);
    SetRect(&orect, 15, v.top, 0, v.bottom);
    m_cSegments.Create(m_cMainWindow, hInstance, prect, orect, true);
    m_cSegments.SetReadOnly(FALSE);

    // m_cBitrate
    SetRect(&prect, 75, 0, 100, 0);
    SetRect(&orect, 0, v.top, -12, v.bottom);
    m_cBitrate.Create(m_cMainWindow, hInstance, prect, orect, true);
    m_cBitrate.SetReadOnly(FALSE);

    // m_csFilename
    v.Static();
    SetRect(&prect, 50, 0, 100, 0);
    SetRect(&orect, 15, v.top, -12, v.bottom);
    m_csFilename.Create(L"File naming", m_cMainWindow, hInstance, prect, orect);

    // m_cFilename
    v.Edit();
    SetRect(&prect, 50, 0, 100, 0);
    SetRect(&orect, 15, v.top, -32, v.bottom);
    m_cFilename.Create(m_cMainWindow, hInstance, prect, orect, true);
    m_cFilename.SetReadOnly(FALSE);

    SetRect(&prect, 100, 0, 100, 0);
    SetRect(&orect, -32, v.top, -12, v.bottom);
    m_cLegend.Create(L"Legend", m_cMainWindow, hInstance, prect, orect);

    // m_cStart
    v.Edit();
    SetRect(&prect, 50, 0, 75, 0);
    SetRect(&orect, 15, v.top, -3, v.bottom);
    m_cStart.Create(L"Start", m_cMainWindow, hInstance, prect, orect);

    // m_cCancel
    SetRect(&prect, 75, 0, 100, 0);
    SetRect(&orect, 3, v.top, -12, v.bottom);
    m_cCancel.Create(L"Cancel", m_cMainWindow, hInstance, prect, orect);
    m_cCancel.SetActive(FALSE);

    // m_cProgress
    v.Edit();
    SetRect(&prect, 50, 0, 100, 0);
    SetRect(&orect, 15, v.top, -93, v.bottom);
    m_cProgress.Create(m_cMainWindow, hInstance, prect, orect, true);
    m_cProgress.SetReadOnly(TRUE);

    // m_cClear
    SetRect(&prect, 100, 0, 100, 0);
    SetRect(&orect, -87, v.top, -12, v.bottom);
    m_cClear.Create(L"Clear log", m_cMainWindow, hInstance, prect, orect);

    // m_cLog
    v.Edit();
    SetRect(&prect, 50, 0, 100, 100);
    SetRect(&orect, 15, v.top, -12, -15);
    m_cLog.CreateMultiline(m_cMainWindow, hInstance, prect, orect, true);
    m_cLog.SetReadOnly(TRUE);

    // The image list and stuff.
    this->m_cImages.CreateEmpty(ICON_SIZEX, ICON_SIZEY, ILC_COLORDDB);
    this->m_cEpisodeList.SetSmallImageList(&this->m_cImages);
    this->m_cListLinks.SetSmallImageList(&this->m_cImages);

    this->m_cImages.AddBitmapFromResource(&this->m_iImageEpisode, g_hInstance, IDB_EPISODE);
    this->m_cImages.AddBitmapFromResource(&this->m_iImageLink, g_hInstance, IDB_FEATURE);

    m_hCancelEvent = CreateEvent(0, FALSE, FALSE, 0);

    return S_OK;
}


HRESULT Plugin_WavUtils::Destroy()
{
    // Save list view settings.
    SaveListViewSettings(LISTVIEW_SETTINGS_WAVUTILS_EPISODES, &m_cEpisodeList, &m_pCore->db, &m_pCore->Log);
    SaveListViewSettings(LISTVIEW_SETTINGS_WAVUTILS_LINKS, &m_cListLinks, &m_pCore->db, &m_pCore->Log);

    CCString s;

    s.FromWindow(m_cOutput);
    m_pCore->db.SetSetting(WAVUTILS_PATH, s, &m_pCore->Log);// path

    s.FromWindow(m_cSegments);
    m_pCore->db.SetSetting(WAVUTILS_LENGTH, s, &m_pCore->Log);// segment length

    s.FromWindow(m_cBitrate);
    m_pCore->db.SetSetting(WAVUTILS_BITRATE, s, &m_pCore->Log);// segment length

    s.FromWindow(m_cFilename);
    m_pCore->db.SetSetting(WAVUTILS_FILENAME, s, &m_pCore->Log);// filename

    s.FromInt(m_cCommercials.GetCheck() ? 1 : 0, 10);
    m_pCore->db.SetSetting(WAVUTILS_COMMERCIALS, s, &m_pCore->Log);// extract commercials

    s.FromInt(m_cMP3.GetCheck() ? 1 : 0, 10);
    m_pCore->db.SetSetting(WAVUTILS_COMPRESS, s, &m_pCore->Log);// extract commercials

    m_cEpisodeList.Destroy();
    m_cGroupEpisodes.Destroy();
    m_cListLinks.Destroy();
    m_cGroupEpisodeDetails.Destroy();
    m_cCommercials.Destroy();
    m_cMP3.Destroy();
    m_cOutput.Destroy();
    m_cBrowse.Destroy();
    m_cSegments.Destroy();
    m_cBitrate.Destroy();
    m_csBitrate.Destroy();
    m_cStart.Destroy();
    m_cMainWindow.Destroy();
    m_cLegend.Destroy();

    CloseHandle(m_hCancelEvent);

    return S_OK;
}


HRESULT Plugin_WavUtils::Refresh()
{
    int iSel;
    LLTIME llTime = NULL;

    iSel = this->m_cEpisodeList.GetSelectionMark();
    if(iSel != -1)
    {
        llTime = this->m_cEpisodeList.GetItemLParam(iSel);
    }

    this->m_cEpisodeList.Clear();
    this->m_cListLinks.Clear();

    this->m_pCore->db.EnumEpisodes(Plugin_WavUtils::EnumEpisodesProc, this,
        &this->m_pCore->Log);

    int iCol = this->m_cEpisodeList.GetLastColumnClicked();
    if(iCol != -1)
    {
        this->m_cEpisodeList.SimulateColumnClick(iCol);
    }

    // Select the correct episode.
    if(iSel != -1)
    {
        iSel = this->m_cEpisodeList.FindLParam(llTime);
        if(iSel != -1)
        {
            this->m_cEpisodeList.SelectAnItem(iSel);
        }
    }

    return S_OK;
}


HRESULT CALLBACK Plugin_WavUtils::EnumEpisodesProc(LLEpisode* pEpisode, PVOID pUser)
{
    Plugin_WavUtils* pThis = (Plugin_WavUtils*)pUser;
    int id;
    LLTIME llTime;
    CCString sTime;
    SYSTEMTIME st;

    // Add the list.
    llTime = MAKELLTIME(pEpisode->dwYear, pEpisode->dwMonth, pEpisode->dwDay);
    st.wDay = (WORD)pEpisode->dwDay;
    st.wMonth = (WORD)pEpisode->dwMonth;
    st.wYear = (WORD)pEpisode->dwYear;
    LLFormatDate(&st, &sTime);

    pThis->m_cEpisodeList.AddItem(&id, llTime, pThis->m_iImageEpisode);
    pThis->m_cEpisodeList.AddItemText(id, COLUMN_DATE, sTime);
    pThis->m_cEpisodeList.AddItemText(id, COLUMN_GUESTS, pEpisode->Guests);
    pThis->m_cEpisodeList.AddItemText(id, COLUMN_FILENAME, pEpisode->FileName);

    sTime.FromGuid(&pEpisode->guid);
    pThis->m_cEpisodeList.AddItemText(id, COLUMN_GUID, sTime);

    return S_OK;
}


HRESULT CALLBACK Plugin_WavUtils::EnumFeaturesProc(LLLink* pl, LLFeature* pf, PVOID pUser)
{
    Plugin_WavUtils* pThis = (Plugin_WavUtils*)pUser;
    int i;
    CCString sGuid;
    CCString sTime;

    sGuid.FromGuid(&pl->guid);
    LLFormatMilliseconds(pl->dwPos, &sTime);

    pThis->m_cListLinks.AddItem(&i, pl->dwPos, pThis->m_iImageLink);
    pThis->m_cListLinks.AddItemText(i, COLUMN_TITLE, pf->Title);
    pThis->m_cListLinks.AddItemText(i, COLUMN_TIME, sTime);
    pThis->m_cListLinks.AddItemText(i, COLUMN_FEATURE_ID, sGuid);
    return S_OK;
}


int CALLBACK Plugin_WavUtils::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    return lParam1 - lParam2;
}


void Plugin_WavUtils::OnSelect()
{
    // Update all the controls on the screen, including
    // the links list.
    int iSel = 0;
    CCString sID;
    CCString Date;
    GUID guid;
    LLEpisode e;

    m_cListLinks.Clear();

    iSel = this->m_cEpisodeList.GetSelectionMark();
    if(iSel == -1) return;

    this->m_cEpisodeList.GetItemText(iSel, COLUMN_GUID, &sID);

    sID.ToGuid(&guid);
    if(FAILED(this->m_pCore->db.GetEpisode(guid, &e, &this->m_pCore->Log)))
    {
        return;
    }

    // Fill in the feature list
    if(FAILED(this->m_pCore->db.EnumFeatures(e.guid, Plugin_WavUtils::EnumFeaturesProc,
        this, &this->m_pCore->Log)))
    {
        // don't care if we couldn't get features.
    }

    int iCol = this->m_cListLinks.GetLastColumnClicked();
    if(iCol != -1)
    {
        this->m_cListLinks.SimulateColumnClick(iCol);
    }

    return;
}


void Plugin_WavUtils::OnBrowse()
{
    CCString sNewPath;

    if(S_OK == LLBrowseForFolder(this->m_cMainWindow, sNewPath))
    {
        m_cOutput.SetText(sNewPath);
    }

    return;
}


void Plugin_WavUtils::OnStart()
{
    if(m_hThread)
    {
        // impossible because of the UI.
    }
    else
    {
        m_hThread = (HANDLE)_beginthread(Plugin_WavUtils::_StartThread, 0, this);
    }
}


/*
    This starts running in a new thread - _StartThread() calls this function (to get rid of the pThis)
*/
void Plugin_WavUtils::_Start2()
{
    CCString sFilename;
    CCString sFullFilename;
    CCString sGuid;
    GUID guid;

    CoInitialize(0);

    m_cStart.SetActive(FALSE);
    m_cCancel.SetActive(TRUE);

    m_dwTick = GetTickCount();

    m_bExtractCommercials = m_cCommercials.GetCheck();
    m_bCompress = m_cMP3.GetCheck();
    m_sOutputDir.FromWindow(m_cOutput);
    m_sFilenaming.FromWindow(m_cFilename);;
    m_nSegment = 1;
    m_dwWritten = 0;
    m_dwRead = 0;

    CCString sBitrate;
    sBitrate.FromWindow(m_cBitrate);
    m_nBitrate = sBitrate.ToInt(10);

    if(!PWU_IsValidBitRate(m_nBitrate))
    {
        MessageBox(m_cMainWindow, L"Invalid bit rate.", L"Error", MB_OK | MB_ICONEXCLAMATION);
    }
    else
    {
        CCString sSegmentLen;
        sSegmentLen.FromWindow(m_cSegments);
        int nSegmentLen = sSegmentLen.ToInt(10);
        if(nSegmentLen < 0)
        {
            MessageBox(m_cMainWindow, L"Invalid segment length.", L"Error", MB_OK | MB_ICONEXCLAMATION);
        }
        else
        {
            m_dwSegmentLength = ((DWORD)nSegmentLen) * 1000;

            int nSel = m_cEpisodeList.GetSelectionMark();
            if(nSel == -1)
            {
                MessageBox(m_cMainWindow, L"Please select an episode.", L"Error", MB_OK | MB_ICONEXCLAMATION);
            }
            else
            {
                // Get the filename from the list
                m_cEpisodeList.GetItemText(nSel, COLUMN_GUID, &sGuid);
                sGuid.ToGuid(&guid);

                if(FAILED(m_pCore->db.GetEpisode(guid, &m_Episode, &m_pCore->Log)))
                {
                    m_cLog.Append(L"Failed to get information data.  Check the log for more details.\r\n");
                }
                else
                {
                    sFullFilename = m_pCore->GetFullMediaPath(m_Episode.FileName);

                    if(FAILED(m_Source.Open(sFullFilename, &m_wfx, &m_pCore->Log)))
                    {
                        m_cLog.Append(L"Failed to open the source file.  Check the log for more details.\r\n");
                    }
                    else
                    {
                        _SetupFilenameInfo();

                        if(!_GetCommercialInfo())
                        {
                            m_cLog.Append(L"Failed to get commercial data.  Check the log for more details.\r\n");
                        }
                        else
                        {
                            if(!_OpenNewFile())
                            {
                                m_cLog.Append(L"Failed to open the output file.  Check the log for more details.\r\n");
                            }
                            else
                            {
                                _Rip();
                            }
                        }

                        m_Output.Close(&m_pCore->Log);

                        if(m_bCompress)
                        {
                            _CompressOutput();
                        }
                        m_Source.Close(&m_pCore->Log);
                    }// Source.Open()
                }// Getepisode
            }// if(nSel
        }// if(nSegmentLen
    }// valid bitrate
    CCString sTimeElapsed;
    CCString sResults;
    m_dwTick = GetTickCount() - m_dwTick;
    LLFormatMilliseconds(m_dwTick, &sTimeElapsed);

    sResults.FormatW(L"Completed in %s.", (PCWSTR)sTimeElapsed);

    m_cStart.SetActive(TRUE);
    m_cCancel.SetActive(FALSE);
    m_cProgress.SetText(sResults);
    sResults.cat(L"\r\n");
    m_cLog.Append(sResults);

    CoUninitialize();

    m_hThread = 0;
}


bool Plugin_WavUtils::_OpenNewFile()
{
    bool r = false;
    CCString sOutputFilename;
    CCString sOutputFullpath;
    CCString sOutputDir;// needs to be different than m_sOutputDir if the filename includes dirs.
    int n = 0;

    _CreateFilename(sOutputFilename);

    sOutputFullpath = m_sOutputDir;
    sOutputFullpath.Path_Append(sOutputFilename);

    // Make sure the dir exists
    sOutputDir = sOutputFullpath;
    sOutputDir.Path_StripToPath();

    n = SHCreateDirectoryEx(m_cMainWindow, sOutputDir, 0);
    switch(n)
    {
    case ERROR_ALREADY_EXISTS:
    case ERROR_FILE_EXISTS:
    case ERROR_SUCCESS:
        {
            CCString s;
            CCString sMS;
            LLFormatMilliseconds(m_dwRead, &sMS);
            s.FormatW(L"[%s] Creating file: %s\r\n", (PCWSTR)sMS, PathFindFileName(sOutputFullpath));
            m_cLog.Append(s);

            if(SUCCEEDED(m_Output.Open(&m_wfx, sOutputFullpath, &m_pCore->Log)))
            {
                m_sCurrentWavFile = sOutputFullpath;
                r = true;
            }
            break;
        }
    case ERROR_BAD_PATHNAME:
        {
            CCString sErr;
            sErr.FormatW(L"Could not create directory %s: The path was set to a relative path.", (PCWSTR)sOutputDir);
            m_pCore->Log.AddEntry(L"%s", (PCWSTR)sErr);
            break;
        }
    case ERROR_FILENAME_EXCED_RANGE:
        {
            CCString sErr;
            sErr.FormatW(L"Could not create directory %s: Path name is too long.", (PCWSTR)sOutputDir);
            m_pCore->Log.AddEntry(L"%s", (PCWSTR)sErr);
            break;
        }
    case ERROR_CANCELLED:
        {
            CCString sErr;
            sErr.FormatW(L"Could not create directory %s: The operation was canceled.", (PCWSTR)sOutputDir);
            m_pCore->Log.AddEntry(L"%s", (PCWSTR)sErr);
            break;
        }
    default:
        {
            CCString sErr;
            sErr.FormatW(L"Could not create directory %s: Unknown error.", (PCWSTR)sOutputDir);
            m_pCore->Log.AddEntry(L"%s", (PCWSTR)sErr);
            break;
        }
    }

    return r;
}


/*
    Three factors break up the episode into smaller segments:
    1) Commercials
    2) Buffered-reads
    3) Output file maximum lengths

    So we need to just read segment by segment until the reader is out of data.
*/
bool Plugin_WavUtils::_Rip()
{
    bool r = false;
    BYTE* pBuffer = 0;
    DWORD dwCommercial = 0;
    DWORD dwBuffer = 0;
    DWORD dwOutput = 0;
    HRESULT hr = 0;

    DWORD dwNextSegment = 0;// what we're determining below.
    DWORD dwBytesRead = 0;
    DWORD dwMSRead = 0;
    DWORD dwMSToRead = 0;

    CCString sProgress;
    CCString sLog;

    while(1)
    {
        /*
            get next segment time.  We will read from m_dwSize -> next segment.
            The next segment time could be defined as the MS offset after m_dwSize at which we STOP reading.

            1) Commercials.  A segment ends when either a commercial ENDS or BEGINS.  When we're reading
               inside a commercial, it will not be written to the output WAV file (the check will be made
               later).
            2) Buffered-reads.  A segment ends at the granularity level.
            3) Output file maximum lengths.  Same thing.

            If no next-segment could be determined, we read the output file maximum length.  If that doesnt
            exist, then we read the buffer size until we're done.
        */
        _GetNextCommercialSegment(dwCommercial);
        _GetNextOutputSegment(dwOutput);
        _GetNextBufferSegment(dwBuffer);

        dwNextSegment = min(dwCommercial, dwOutput);
        dwNextSegment = min(dwNextSegment, dwBuffer);

        // We now have the next segment
        dwMSToRead = dwNextSegment - m_dwRead;
        hr = m_Source.Read(dwMSToRead, &m_pCore->Log);// read the audio data from the source
        m_Source.LockAndFlush(&dwBytesRead, &pBuffer, &m_pCore->Log);// get a pointer to locked data.

        dwMSRead = _BytesToMS(dwBytesRead);
        odsf(L"_Rip()", L"Requested %d ms, actually read %d bytes (%d ms)", dwMSToRead, dwBytesRead, dwMSRead);

        if(dwBytesRead == 0)
        {
            /*
                If we didn't read ANYTHING, that probably means the segment was too short for the reader.
                In that case, who cares... if it was too small for the reader, it was too small
                to care about...

                So, the solution is to just "pretend" like we processed the segment and move on.
            */
            if(m_bExtractCommercials)
            {
                if(!_PtBetweenInCommercial(m_dwRead, m_dwRead + dwMSToRead))
                {
                    m_dwWritten += dwMSToRead;
                }
            }
            else
            {
                m_dwWritten += dwMSToRead;
            }

            // Fake that we read the data.
            m_dwRead += dwMSToRead;
        }
        else
        {
            if(m_bExtractCommercials)
            {
                if(!_PtBetweenInCommercial(m_dwRead, m_dwRead + dwMSRead))
                {
                    m_Output.WriteBits(pBuffer, dwBytesRead, &m_pCore->Log);
                    m_dwWritten += dwMSRead;
                }
            }
            else
            {
                m_Output.WriteBits(pBuffer, dwBytesRead, &m_pCore->Log);
                m_dwWritten += dwMSRead;
            }

            LLFormatMilliseconds(m_dwRead, &sProgress);
            m_cProgress.SetText(sProgress);

            m_dwRead += dwMSRead;
        }
        
        m_Source.Unlock();

        if(WaitForSingleObject(m_hCancelEvent, 0) == WAIT_OBJECT_0)
        {
            break;
        }

        // Determine if we have to start a new file
        if(m_dwSegmentLength)
        {
            if(m_dwWritten >= m_dwSegmentLength)
            {
                // Create a new file.
                odsf(L"Rip()", L"Creating new file because written: %06d >= %06d", m_dwWritten, m_dwSegmentLength);

                m_Output.Close(&m_pCore->Log);

                // Convert the file to mp3.
                if(m_bCompress)
                {
                    _CompressOutput();
                }

                m_dwWritten = 0;
                m_nSegment ++;

                _OpenNewFile();
            }
        }

        if(ERROR_END_OF_MEDIA == hr)
        {
            // end of media
            break;
        }
    }

    return r;
}


void Plugin_WavUtils::_CompressOutput()
{
    bool bSuccess = false;
    WAVReader src;
    MP3Writer dest;
    CCString sDestFile = m_sCurrentWavFile;
    sDestFile.Path_RenameExtension(L".mp3");
    if(FAILED(src.Open(m_sCurrentWavFile, FALSE, &m_pCore->Log)))
    {
        m_pCore->Log.AddEntry(L"Failed to open the output file for reading.");
    }
    else
    {
        // open the mp3
        WAVEFORMATEX wfx = {0};
        src.GetFormat(&wfx);
        if(FAILED(dest.Open(&wfx, sDestFile, m_nBitrate, FALSE, &m_pCore->Log)))
        {
            m_pCore->Log.AddEntry(L"Failed to open the output MP3 file for writing.");
        }
        else
        {
            const DWORD bufsize = 1000000;
            DWORD br = 0;
            char* buf = 0;

            buf = new char[bufsize];
            if(!buf)
            {
                m_pCore->Log.AddEntry(L"Out of memory.");
            }
            else
            {
                m_cLog.Append(L"Compressing file.");
                while(1)
                {
                    if(WaitForSingleObject(m_hCancelEvent, 0) == WAIT_OBJECT_0)
                    {
                        SetEvent(m_hCancelEvent);
                        m_cLog.Append(L"Canceled.\r\n");
                        break;
                    }
                    m_cLog.Append(L".");
                    src.Read(buf, bufsize, &br, &m_pCore->Log);
                    if(FAILED(dest.WriteBits(buf, br, &m_pCore->Log)))
                    {
                        m_pCore->Log.AddEntry(L"Failed writing %d bytes to output mp3.", br);
                    }
                    if(br != bufsize)
                    {
                        bSuccess = true;
                        break;
                    }
                }

                delete [] buf;
                buf = 0;
                m_cLog.Append(L"finished.\r\n");
            }

            dest.Close(&m_pCore->Log);
        }

        src.Close();

        // Now delete the source file.
        if(bSuccess)
        {
            DeleteFile(m_sCurrentWavFile);
        }
    }
}


DWORD Plugin_WavUtils::_BytesToMS(DWORD bytes)
{
    return (bytes * 1000) / m_wfx.nAvgBytesPerSec;
}


bool Plugin_WavUtils::_GetCommercialInfo()
{
    m_Commercials.clear();
    m_pCore->db.EnumCommercials(m_Episode.guid, Plugin_WavUtils::EnumCommercials, this, &m_pCore->Log);
    return true;
}


HRESULT CALLBACK Plugin_WavUtils::EnumCommercials(LLCommercial* pc, PVOID pUser)
{
    Plugin_WavUtils* pThis = (Plugin_WavUtils*)pUser;
    pThis->m_Commercials.push_back(LLCommercial(*pc));
    return S_OK;
}


/*
    Determines if the midpoint between two time offsets is in a commercial.
*/
bool Plugin_WavUtils::_PtBetweenInCommercial(DWORD dwStart, DWORD dwEnd)
{
    bool r = false;
    LLCommercial* pc = 0;
    std::vector<LLCommercial>::iterator it = m_Commercials.begin();

    unsigned __int64 qwActual = dwStart + dwEnd;
    DWORD dwActual = (DWORD)(qwActual/2);

    while(it != m_Commercials.end())
    {
        pc = &(*it);
        if(dwActual >= pc->dwIn)
        {
            if(dwActual < pc->dwOut)
            {
                r = true;
                break;
            }
        }

        it++;
    }

    return r;
}


void Plugin_WavUtils::_GetNextCommercialSegment(DWORD& dw)
{
    LLCommercial* pc = 0;
    std::vector<LLCommercial>::iterator it = m_Commercials.begin();

    dw = (DWORD)-1;

    while(it != m_Commercials.end())
    {
        pc = &(*it);

        if((pc->dwIn > m_dwRead) && (pc->dwIn < dw))
        {
            dw = pc->dwIn;
        }

        if((pc->dwOut > m_dwRead) && (pc->dwOut < dw))
        {
            dw = pc->dwOut;
        }

        it++;
    }

    if(dw == (DWORD)-1)
    {
        _GetNextBufferSegment(dw);
    }
}


void Plugin_WavUtils::_GetNextBufferSegment(DWORD& dw)
{
    dw = ((WAVUTILS_BUFFER_MS + m_dwRead) / WAVUTILS_BUFFER_MS) * WAVUTILS_BUFFER_MS;
}


/*
    dw want m_dwWritten to be the granularity of m_dwSegmentLength.
*/
void Plugin_WavUtils::_GetNextOutputSegment(DWORD& dw)
{
    if(m_dwSegmentLength)
    {
        //dw = ((m_dwSegmentLength + m_dwRead) / m_dwSegmentLength) * m_dwSegmentLength;
        dw = ((m_dwSegmentLength + m_dwWritten) / m_dwSegmentLength) * m_dwSegmentLength;
        // dw is now what we would like m_dwWritten to BE after the read.
        dw -= m_dwWritten;
        dw += m_dwRead;
    }
    else
    {
        _GetNextBufferSegment(dw);
    }
}


bool LLGetDateFormat(IN const SYSTEMTIME* pTime, IN PCWSTR wszFormat, OUT CCString& s)
{
    bool r = false;
    int nSize = 0;

    nSize = GetDateFormat(LOCALE_USER_DEFAULT, 0, pTime, wszFormat, 0, 0);
    if(nSize)
    {
        GetDateFormat(LOCALE_USER_DEFAULT, 0, pTime, wszFormat, s.GetBuffer(nSize+1), nSize+1);
    }

    return r;
}


void Plugin_WavUtils::_SetupFilenameInfo()
{
    SYSTEMTIME s;
    s.wDay = (WORD)m_Episode.dwDay;
    s.wMonth = (WORD)m_Episode.dwMonth;
    s.wYear = (WORD)m_Episode.dwYear;

    LLGetDateFormat(&s, L"dd", m_fi.d);
    LLGetDateFormat(&s, L"dddd", m_fi.D);
    LLGetDateFormat(&s, L"MM", m_fi.m);
    LLGetDateFormat(&s, L"MMMM", m_fi.M);
    LLGetDateFormat(&s, L"yy", m_fi.y);
    LLGetDateFormat(&s, L"yyyy", m_fi.Y);

    m_fi.f = m_Episode.FileName;
    m_fi.F = m_Episode.FileName;
    m_fi.F.Truncate(m_fi.F.Path_FindExtension());
    m_fi.e = PathFindExtension(m_fi.f);
    m_fi.g = m_Episode.Guests;
}


/*
        %e   - The original file extension without the period.
        %f   - The complete original filename
        %m   - Month, like "08"
        %M   - Month, like "August"
        %d   - Day, like "14"
        %D   - Day, like "Sunday"
        %y   - Year, like 02
        %Y   - Year, like 2002
        %g   - Guests
*/
bool Plugin_WavUtils::_CreateFilename(CCString& s)
{
    PCWSTR wszSource = (PCWSTR)this->m_sFilenaming;
    WCHAR ch;

    while(ch = (*wszSource))
    {
        if(ch == '%')
        {
            wszSource ++;
            ch = *wszSource;
            if(ch)
            {
                switch(ch)
                {
                case 'n':
                    {
                        CCString sSegment;
                        sSegment.FormatW(L"%02d", m_nSegment);
                        s.cat(sSegment);
                        break;
                    }
                case 'e':
                    s.cat(m_fi.e);
                    break;
                case 'f':
                    s.cat(m_fi.f);
                    break;
                case 'F':
                    s.cat(m_fi.F);
                    break;
                case 'm':
                    s.cat(m_fi.m);
                    break;
                case 'M':
                    s.cat(m_fi.M);
                    break;
                case 'd':
                    s.cat(m_fi.d);
                    break;
                case 'D':
                    s.cat(m_fi.D);
                    break;
                case 'y':
                    s.cat(m_fi.y);
                    break;
                case 'Y':
                    s.cat(m_fi.Y);
                    break;
                case 'g':
                    s.cat(m_fi.g);
                    break;
                default:
                    s.cat(ch);
                    break;
                }
            }
            else
            {
                s.cat(ch);
                break;
            }
        }
        else
        {
            s.cat(ch);
        }

        wszSource++;
    }

    return true;
}


void Plugin_WavUtils::_StartThread(void* pThis)
{
    ((Plugin_WavUtils*)pThis)->_Start2();
}


INT_PTR CALLBACK Plugin_WavUtils::_LegendProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
        {
            HWND hText = GetDlgItem(hDlg, IDC_LEGENDTEXT);
            CCString s;
            s = L"%e   - The original file extension without the period.\r\n";
            s.cat(L"%f   - The complete original filename\r\n");
            s.cat(L"%m   - Month, like \"08\"\r\n");
            s.cat(L"%M   - Month, like \"August\"\r\n");
            s.cat(L"%d   - Day, like \"14\"\r\n");
            s.cat(L"%D   - Day, like \"Sunday\"\r\n");
            s.cat(L"%y   - Year, like 02\r\n");
            s.cat(L"%Y   - Year, like 2002\r\n");
            s.cat(L"%g   - Guests\r\n");

            SetWindowText(hText, s);
            return TRUE;
        }
    case WM_CLOSE:
        {
            EndDialog(hDlg, 0);
            return TRUE;
        }
    }
    return FALSE;
}

