

#include "stdafx.h"
#include "P_Library.h"
#include "..\\LoveLineUtil\\LLUtils.h"
#include "Resource.h"
#include <shlwapi.h>


#define COLUMN_DATE     0
#define COLUMN_GUESTS   1
#define COLUMN_FILENAME 2
#define COLUMN_GUID     3

#define COLUMN_TITLE    0
#define COLUMN_TIME     1
#define COLUMN_FEATURE_ID 2

#define COLUMN_NEW_DATE       0
#define COLUMN_NEW_FILENAME   1

#define COLUMN_STALE_DATE       0
#define COLUMN_STALE_FILENAME   2
#define COLUMN_STALE_GUESTS     1
#define COLUMN_STALE_ID         3


extern HINSTANCE g_hInstance;


Plugin_Library::Plugin_Library()
{
    this->pPlugins = NULL;
    this->pCore = NULL;
}


Plugin_Library::~Plugin_Library()
{
}


LPARAM CALLBACK Plugin_Library::WndProc(
    HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Plugin_Library* pThis = (Plugin_Library*)GetWindowLongPtr(hWnd, GWL_USERDATA);

    switch(uMsg)
    {
    case WM_NOTIFY:
        {
            NMHDR* pnmh = (NMHDR*)lParam;
            NMITEMACTIVATE* pia = NULL;
            NMLISTVIEW* plv = NULL;
            int iColumn;

            if((iColumn = pThis->cListEpisodes.IsSortNotify(lParam)) != -1)
            {
                if(iColumn == COLUMN_DATE)
                {
                    pThis->cListEpisodes.SortByLParam(pThis, Plugin_Library::CompareFunc);
                }
                else
                {
                    pThis->cListEpisodes.SortTextAutomatically(iColumn);
                }

                return TRUE;
            }

            if((iColumn = pThis->cListLinks.IsSortNotify(lParam)) != -1)
            {
                if(iColumn == COLUMN_TIME)
                {
                    pThis->cListLinks.SortByLParam(pThis, Plugin_Library::CompareFunc);
                }
                else
                {
                    pThis->cListLinks.SortTextAutomatically(iColumn);
                }

                return TRUE;
            }

            if(plv = pThis->cListEpisodes.IsSelectedNotify(lParam))
            {
                pThis->cListEpisodes.SetSelectionMark(plv->iItem);
                pThis->OnSelect();
                return TRUE;
            }

            if(pia = pThis->cListEpisodes.IsActivatedNotify(lParam))
            {
                pThis->cListEpisodes.SetSelectionMark(pia->iItem);
                pThis->OnPlay();
                return TRUE;
            }

            if(pia = pThis->cListLinks.IsActivatedNotify(lParam))
            {
                pThis->cListLinks.SetSelectionMark(pia->iItem);
                pThis->OnPlayLink();
                return TRUE;
            }

            break;
        }
    case WM_COMMAND:
        {
            UINT uId = (UINT)((DWORD)wParam & 0xFFFF);
            if(uId == pThis->cAddFile)
            {
                pThis->OnAddFile();
                return TRUE;
            }
            if(uId == pThis->cRemove)
            {
                pThis->OnRemove();
                return TRUE;
            }
            if(uId == pThis->cPlay)
            {
                // On play.
                pThis->OnPlay();
            }

            break;
        }
    case WM_SIZE:
        {
            pThis->cStaticDate.Size();
            pThis->cStaticLinks.Size();
            pThis->cStaticGuests.Size();
            pThis->cStaticFile.Size();
            pThis->cStaticDescription.Size();
            pThis->cDate.Size();
            pThis->cGuests.Size();
            pThis->cFile.Size();
            pThis->cDescription.Size();
            pThis->cAddFile.Size();
            pThis->cRemove.Size();
            pThis->cPlay.Size();
            pThis->cGroupEpisodes.Size();
            pThis->cGroupEpisodeDetails.Size();
            pThis->cListEpisodes.Size();
            pThis->cListLinks.Size();
            return 0;
        }
    case WM_CREATE:
        {
            CREATESTRUCT* pcs = (CREATESTRUCT*)lParam;
            pThis = (Plugin_Library*)pcs->lpCreateParams;
            SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)pThis);
            return 0;
        }
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


void CALLBACK Plugin_Library::NotificationProc(
    PVOID pUser, DWORD dwMsg, PVOID pParams)
{
    Plugin_Library* pThis = (Plugin_Library*)pUser;

    switch(dwMsg)
    {
    case LL_SETFOCUS:
        {
            break;
        }
    case LL_INITIALIZE:
        {
            /*
                Load Settings
            */
            LoadListViewSettings(L"Library_Episodes", &pThis->cListEpisodes, &pThis->pCore->db, &pThis->pCore->Log);
            LoadListViewSettings(L"Library_Links", &pThis->cListLinks, &pThis->pCore->db, &pThis->pCore->Log);

            /*
                Here we determine if there are new or stale episodes.
            */
            pThis->ScoutEpisodes();
            pThis->OnRefresh();
            break;
        }
    case LL_DBUPDATE:
        {
            LL_DBUPDATE_STRUCT* pus = (LL_DBUPDATE_STRUCT*)pParams;
            if(pus->dwType & (DBUPDATE_EPISODE | DBUPDATE_LINK | DBUPDATE_FEATURE))
            {
                pThis->OnRefresh();
            }
            break;
        }
    case LL_ATTACH:
        {
            LL_ATTACH_STRUCT* pas = (LL_ATTACH_STRUCT*)pParams;
            StandardPlugins* psp = (StandardPlugins*)pas->pUserData;
            pThis = (Plugin_Library*)psp->pLibrary;

            pThis->pPlugins = psp;
            pThis->pCore = pas->pCore;

            pThis->Create();

            pas->hWnd = pThis->cMainWindow;
            pas->pUserData = pThis;

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


HRESULT Plugin_Library::Create()
{
    HINSTANCE hInstance;
    RECT orect;
    RECT prect;
    LIST_COLUMN c[10];

    hInstance = GetModuleHandle(NULL);

    SetRect(&prect, 0, 0, 100, 100);
    SetRect(&orect, 0, 0, 0, 0);

    this->cMainWindow.Create(L"Plugin_Library",
        Plugin_Library::WndProc, hInstance, prect, orect, this);

    // The "Episodes" Group box
    SetRect(&prect, 0, 0, 50, 100);
    SetRect(&orect, 6, 6, -3, -6);
    this->cGroupEpisodes.CreateGroup(
        L"Episodes", this->cMainWindow, hInstance, prect, orect);

    // The "Episode Details" group box
    SetRect(&prect, 50, 0, 100, 100);
    SetRect(&orect, 3, 6, -6, -6);
    this->cGroupEpisodeDetails.CreateGroup(
        L"Episode Details", this->cMainWindow, hInstance, prect, orect);

    // Episode ListView
    SetRect(&prect, 0, 0, 50, 100);
    SetRect(&orect, 15, 21, -12, -42);
    this->cListEpisodes.Create(this->cMainWindow, hInstance, prect, orect);

    c[COLUMN_DATE].Set(L"Date", LIST_LEFT, 100);
    c[COLUMN_GUESTS].Set(L"Guests", LIST_LEFT, 100);
    c[COLUMN_FILENAME].Set(L"FileName", LIST_LEFT, 100);
    c[COLUMN_GUID].Set(L"ID", LIST_LEFT, 100);
    this->cListEpisodes.ShowHeader(4, c);

    this->cListEpisodes.SetStyle(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_CHILD | WS_VISIBLE);
    this->cListEpisodes.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);

    // Add File, Remove, and Play buttons
    SetRect(&prect, 0, 100, 0, 100);
    SetRect(&orect, 15, -36, 90, -15);
    this->cAddFile.Create(L"Add File...", this->cMainWindow, hInstance, prect, orect);

    SetRect(&prect, 0, 100, 0, 100);
    SetRect(&orect, 96, -36, 171, -15);
    this->cRemove.Create(L"Remove", this->cMainWindow, hInstance, prect, orect);

    SetRect(&prect, 0, 100, 0, 100);
    SetRect(&orect, 177, -36, 252, -15);
    this->cPlay.Create(L"Play", this->cMainWindow, hInstance, prect, orect);

    // Static boxes on the right side
    SetRect(&prect, 50, 0, 50, 0);
    SetRect(&orect, 12, 26, 72, 43);
    this->cStaticDate.Create(L"Date", this->cMainWindow, hInstance, prect, orect);

    // Description
    SetRect(&prect, 50, 0, 50, 35);
    SetRect(&orect, 12, 53, 72, 0);
    this->cStaticDescription.Create(L"Description", this->cMainWindow, hInstance, prect, orect);

    // Guests
    SetRect(&prect, 50, 35, 50, 35);
    SetRect(&orect, 12, 11, 72, 26);
    this->cStaticGuests.Create(L"Guests", this->cMainWindow, hInstance, prect, orect);

    // File
    SetRect(&prect, 50, 35, 50, 35);
    SetRect(&orect, 12, 38, 72, 54);
    this->cStaticFile.Create(L"File", this->cMainWindow, hInstance, prect, orect);

    // Links
    SetRect(&prect, 50, 35, 50, 35);
    SetRect(&orect, 12, 60, 72, 77);
    this->cStaticLinks.Create(L"Links", this->cMainWindow, hInstance, prect, orect);

    // Links ListView
    SetRect(&prect, 50, 35, 100, 100);
    SetRect(&orect, 12, 77, -15, -15);
    this->cListLinks.Create(this->cMainWindow, hInstance, prect, orect);

    c[COLUMN_TITLE].Set(L"Feature Name", LIST_LEFT, 100);
    c[COLUMN_TIME].Set(L"Time", LIST_LEFT, 100);
    c[COLUMN_FEATURE_ID].Set(L"ID", LIST_LEFT, 100);
    this->cListLinks.ShowHeader(3, c);

    this->cListLinks.SetStyle(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_CHILD | WS_VISIBLE);
    this->cListLinks.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);

    // The Edit boxes
    SetRect(&prect, 50, 0, 100, 0);
    SetRect(&orect, 72, 21, -15, 42);
    this->cDate.Create(this->cMainWindow, hInstance, prect, orect);

    SetRect(&prect, 50, 0, 100, 35);
    SetRect(&orect, 72, 48, -15, 0);
    this->cDescription.Create(this->cMainWindow, hInstance, prect, orect);

    SetRect(&prect, 50, 35, 100, 35);
    SetRect(&orect, 72, 6, -15, 27);
    this->cGuests.Create(this->cMainWindow, hInstance, prect, orect);

    SetRect(&prect, 50, 35, 100, 35);
    SetRect(&orect, 72, 33, -15, 54);
    this->cFile.Create(this->cMainWindow, hInstance, prect, orect);

    // The image list and stuff.
    this->cImages.CreateEmpty(ICON_SIZEX, ICON_SIZEY, ILC_COLORDDB);
    this->cListEpisodes.SetSmallImageList(&this->cImages);
    this->cListLinks.SetSmallImageList(&this->cImages);

    this->cImages.AddBitmapFromResource(&this->iImageEpisode, g_hInstance, IDB_EPISODE);
    this->cImages.AddBitmapFromResource(&this->iImageLink, g_hInstance, IDB_FEATURE);

    return S_OK;
}


HRESULT Plugin_Library::Destroy()
{
    /*
        Save Settings
    */
    SaveListViewSettings(L"Library_Episodes", &this->cListEpisodes, &this->pCore->db, &this->pCore->Log);
    SaveListViewSettings(L"Library_Links", &this->cListLinks, &this->pCore->db, &this->pCore->Log);

    this->cStaticDate.Destroy();
    this->cStaticLinks.Destroy();
    this->cStaticGuests.Destroy();
    this->cStaticFile.Destroy();
    this->cStaticDescription.Destroy();
    this->cDate.Destroy();
    this->cGuests.Destroy();
    this->cFile.Destroy();
    this->cDescription.Destroy();
    this->cAddFile.Destroy();
    this->cRemove.Destroy();
    this->cPlay.Destroy();
    this->cGroupEpisodes.Destroy();
    this->cGroupEpisodeDetails.Destroy();
    this->cListEpisodes.Destroy();
    this->cListLinks.Destroy();
    this->cMainWindow.Destroy();
    this->cImages.Clear();

    return S_OK;
}


void Plugin_Library::OnAddFile()
{
    LLEpisode e;
    SYSTEMTIME st;
    CCString sFileName;
    FILE_FILTER pFilters[5];

    // Get the name of a file to add
    pFilters[0].Set(L"Media Files (*.wm?;*.mp3;*.wav)", L"*.wm?;*.mp3;*.wav");
    pFilters[1].Set(L"All Files (*.*)", L"*.*");
    if(FAILED(OpenFileDlg(this->cMainWindow, &sFileName, 2, pFilters, this->pCore->GetMediaPath())))
    {
        // Canceled.
        return;
    }

    // Fill in the episode struct.
    e.Description = L"";
    DateFromFileName(sFileName, &st);
    e.dwDay = st.wDay;
    e.dwMonth = st.wMonth;
    e.dwYear = st.wYear;
    e.FileName = sFileName;
    e.FileName.Path_AbsoluteToRelative(this->pCore->GetMediaPath());
    e.Guests = L"";

    if(FAILED(this->pCore->db.AddEpisode(&e, &this->pCore->Log)))
    {
        // Error!
        return;
    }

    LL_DBUPDATE_STRUCT ms;
    ms.dwOperation = DBUPDATE_NEW;
    ms.dwType = DBUPDATE_EPISODE;
    this->pCore->SendPluginMessage(LL_DBUPDATE, &ms);
}


void Plugin_Library::OnRemove()
{
    int iSel;
    CCString sID;
    GUID guid;

    // Figure out which one is selected.
    iSel = this->cListEpisodes.GetSelectionMark();
    if(iSel == -1) return;

    this->cListEpisodes.GetItemText(iSel, COLUMN_GUID, &sID);
    sID.ToGuid(&guid);

    if(FAILED(this->pCore->db.RemoveEpisode(guid, &this->pCore->Log)))
    {
        return;
    }

    LL_DBUPDATE_STRUCT ms;
    ms.dwOperation = DBUPDATE_REMOVE;
    ms.dwType = DBUPDATE_EPISODE;
    this->pCore->SendPluginMessage(LL_DBUPDATE, &ms);
}


void Plugin_Library::OnRefresh()
{
    int iSel;
    LLTIME llTime = NULL;

    iSel = this->cListEpisodes.GetSelectionMark();
    if(iSel != -1)
    {
        llTime = this->cListEpisodes.GetItemLParam(iSel);
    }

    this->cListEpisodes.Clear();
    this->cListLinks.Clear();

    this->pCore->db.EnumEpisodes(Plugin_Library::EnumEpisodesProc, this,
        &this->pCore->Log);

    int iCol = this->cListEpisodes.GetLastColumnClicked();
    if(iCol != -1)
    {
        this->cListEpisodes.SimulateColumnClick(iCol);
    }

    // Select the correct episode.
    if(iSel != -1)
    {
        iSel = this->cListEpisodes.FindLParam(llTime);
        if(iSel != -1)
        {
            this->cListEpisodes.SelectAnItem(iSel);
        }
    }
}


HRESULT CALLBACK Plugin_Library::EnumEpisodesProc(LLEpisode* pEpisode, PVOID pUser)
{
    Plugin_Library* pThis = (Plugin_Library*)pUser;
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

    pThis->cListEpisodes.AddItem(&id, llTime, pThis->iImageEpisode);
    pThis->cListEpisodes.AddItemText(id, COLUMN_DATE, sTime);
    pThis->cListEpisodes.AddItemText(id, COLUMN_GUESTS, pEpisode->Guests);
    pThis->cListEpisodes.AddItemText(id, COLUMN_FILENAME, pEpisode->FileName);

    sTime.FromGuid(&pEpisode->guid);
    pThis->cListEpisodes.AddItemText(id, COLUMN_GUID, sTime);

    return S_OK;
}


void Plugin_Library::OnPlayLink()
{
    CCString sGuid;
    GUID guid;
    LLLink l;
    int nSel = this->cListLinks.GetSelectionMark();

    if(nSel == -1) return;

    // Get the guid
    this->cListLinks.GetItemText(nSel, COLUMN_FEATURE_ID, &sGuid);
    sGuid.ToGuid(&guid);

    // Get the link object
    if(FAILED(this->pCore->db.GetLink(guid, &l, &this->pCore->Log)))
    {
        return;
    }

    // And seek to the link.
    this->pCore->SeekEpisode(l.guidEpisode, l.dwPos);

    return;
}


void Plugin_Library::OnPlay()
{
    CCString sGuid;
    GUID guid;
    int nSel = this->cListEpisodes.GetSelectionMark();

    if(nSel == -1) return;

    // Get the filename from the list
    this->cListEpisodes.GetItemText(nSel, COLUMN_GUID, &sGuid);
    sGuid.ToGuid(&guid);

    this->pCore->SeekEpisode(guid, 0);

    return;
}


void Plugin_Library::OnSelect()
{
    // Update all the controls on the screen, including
    // the links list.
    int iSel = 0;
    CCString sID;
    CCString Date;
    GUID guid;
    LLEpisode e;

    this->cListLinks.Clear();

    iSel = this->cListEpisodes.GetSelectionMark();
    if(iSel == -1) return;

    this->cListEpisodes.GetItemText(iSel, COLUMN_GUID, &sID);

    sID.ToGuid(&guid);
    if(FAILED(this->pCore->db.GetEpisode(guid, &e, &this->pCore->Log)))
    {
        return;
    }

    // Fill in the feature list
    if(FAILED(this->pCore->db.EnumFeatures(e.guid, Plugin_Library::EnumFeaturesProc,
        this, &this->pCore->Log)))
    {
        // don't care if we couldn't get features.
    }

    int iCol = this->cListLinks.GetLastColumnClicked();
    if(iCol != -1)
    {
        this->cListLinks.SimulateColumnClick(iCol);
    }

    LLFormatDate(&e, &Date);
    this->cDate.SetText(Date);

    this->cGuests.SetText(e.Guests);
    this->cDescription.SetText(e.Description);
    this->cFile.SetText(e.FileName);

    return;
}


int CALLBACK Plugin_Library::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    return lParam1 - lParam2;
}


HRESULT CALLBACK Plugin_Library::EnumFeaturesProc(LLLink* pl, LLFeature* pf, PVOID pUser)
{
    Plugin_Library* pThis = (Plugin_Library*)pUser;
    int i;
    CCString sGuid;
    CCString sTime;

    sGuid.FromGuid(&pl->guid);
    LLFormatMilliseconds(pl->dwPos, &sTime);

    pThis->cListLinks.AddItem(&i, pl->dwPos, pThis->iImageLink);
    pThis->cListLinks.AddItemText(i, COLUMN_TITLE, pf->Title);
    pThis->cListLinks.AddItemText(i, COLUMN_TIME, sTime);
    pThis->cListLinks.AddItemText(i, COLUMN_FEATURE_ID, sGuid);
    return S_OK;
}


void Plugin_Library::ScoutEpisodes()
{
    DialogBoxParam(g_hInstance, MAKEINTRESOURCE(IDD_NEWFOUND),
        this->cMainWindow, Plugin_Library::NewEpisodesProc, (LPARAM)this);

    DialogBoxParam(g_hInstance, MAKEINTRESOURCE(IDD_STALEFOUNDX),
        this->cMainWindow, Plugin_Library::StaleEpisodesProc, (LPARAM)this);
}


/*
    Called by the new episodes found window proc to fill in the list.
    Returns TRUE if there were episodes.  Returns FALSE to close the
    dialog automatically and exit.
*/
BOOL Plugin_Library::_CheckForNewEpisodes(HWND hWnd)
{
    // Make sure the episodes are in the database.
    CCString sMediaPath = this->pCore->GetMediaPath();
    HANDLE hFind;
    WIN32_FIND_DATA wfd;
    BOOL bFound = FALSE;
	PWSTR wszExt;

    sMediaPath.Path_Append(L"*.*");

    hFind = FindFirstFile(sMediaPath, &wfd);
    if(hFind == INVALID_HANDLE_VALUE)
    {
        this->pCore->Log.AddEntry(GetLastError(), L"Could not scout for new episodes.");
        return FALSE;
    }

    while(1)
    {
        if((wcscmp(wfd.cFileName, L".") != 0) && (wcscmp(wfd.cFileName, L"..") != 0))
        {
			wszExt = PathFindExtension(wfd.cFileName);
			if(wcsicmp(wszExt, L".mp3") == 0 || wcsicmp(wszExt, L".wma") == 0)
			{
				if(this->pCore->db.IsFileEnlisted(wfd.cFileName) == FALSE)
				{
					// Add it to the list.
					int i;
					SYSTEMTIME st;
					CCString Date;

					DateFromFileName(wfd.cFileName, &st);
					LLFormatDate(&st, &Date);

					this->cNewListEpisodes.AddItem(&i, 0, this->iImageEpisode);
					this->cNewListEpisodes.AddItemText(i, COLUMN_NEW_DATE, Date);
					this->cNewListEpisodes.AddItemText(i, COLUMN_NEW_FILENAME, wfd.cFileName);
					bFound = TRUE;
				}
			}
        }

        if(FindNextFile(hFind, &wfd) == 0)
        {
            break;
        }
    }

    FindClose(hFind);

    return bFound;
}


INT_PTR CALLBACK Plugin_Library::NewEpisodesProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Plugin_Library* pThis = (Plugin_Library*)GetWindowLongPtr(hWnd, GWL_USERDATA);

    switch(uMsg)
    {
    case WM_DESTROY:
        {
            pThis->cNewListEpisodes.Destroy();
            return TRUE;
        }
    case WM_CLOSE:
        {
            EndDialog(hWnd, 0);
            return TRUE;
        }
    case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
            case IDC_NOACTION:
                {
                    EndDialog(hWnd, 0);
                    return TRUE;
                }
            case IDC_ADDALL:
                {
                    pThis->_AddAllEpisodes(hWnd);
                    EndDialog(hWnd, 0);
                    return TRUE;
                }
            }
            break;
        }
    case WM_INITDIALOG:
        {
            SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)lParam);
            pThis = (Plugin_Library*)lParam;

            // Get information about IDC_LIST1
            HWND hList = GetDlgItem(hWnd, IDC_LIST1);
            RECT rcControl;
            GetControlRect(hList, &rcControl);

            // Create the list
            LIST_COLUMN c[10];
            RECT prect;

            SetRect(&prect, 0, 0, 0, 0);

            c[COLUMN_NEW_DATE].Set(L"Date", LIST_LEFT, 100);
            c[COLUMN_NEW_FILENAME].Set(L"FileName", LIST_LEFT, 100);

            pThis->cNewListEpisodes.Create(hWnd, g_hInstance, prect, rcControl);
            pThis->cNewListEpisodes.SetStyle(LVS_SHAREIMAGELISTS | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_CHILD | WS_VISIBLE);
            pThis->cNewListEpisodes.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);

            pThis->cNewListEpisodes.SetSmallImageList(&pThis->cImages);
            pThis->cNewListEpisodes.ShowHeader(2, c);

            pThis->cNewListEpisodes.Size();

            DestroyWindow(hList);

            // Check for new episodes.
            if(pThis->_CheckForNewEpisodes(hWnd) == 0)
            {
                EndDialog(hWnd, 0);
            }

            return TRUE;
        }
    }

    return FALSE;
}


void Plugin_Library::_AddAllEpisodes(HWND hWnd)
{
    int i;
    int iTotal = this->cNewListEpisodes.GetItemCount();
    CCString FileName;
    LLEpisode e;
    LL_DBUPDATE_STRUCT ms;
    SYSTEMTIME st;
    e.Description = L"";
    e.Guests = L"";

    for(i=0;i<iTotal;i++)
    {
        this->cNewListEpisodes.GetItemText(i, COLUMN_NEW_FILENAME, &FileName);
        DateFromFileName(FileName, &st);
        e.dwDay = st.wDay;
        e.dwMonth = st.wMonth;
        e.dwYear = st.wYear;
        e.FileName = FileName;
        if(SUCCEEDED(this->pCore->db.AddEpisode(&e, &this->pCore->Log)))
        {
            ms.dwOperation = DBUPDATE_NEW;
            ms.dwType = DBUPDATE_EPISODE;
            this->pCore->SendPluginMessage(LL_DBUPDATE, &ms);
        }
    }
}


INT_PTR CALLBACK Plugin_Library::StaleEpisodesProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Plugin_Library* pThis = (Plugin_Library*)GetWindowLongPtr(hWnd, GWL_USERDATA);

    switch(uMsg)
    {
    case WM_DESTROY:
        {
            pThis->cStaleListEpisodes.Destroy();
            return TRUE;
        }
    case WM_CLOSE:
        {
            EndDialog(hWnd, 0);
            return TRUE;
        }
    case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
            case IDC_NOACTION:
                {
                    EndDialog(hWnd, 0);
                    return TRUE;
                }
            case IDC_REMOVEALL:
                {
                    pThis->_RemoveAllEpisodes(hWnd);
                    EndDialog(hWnd, 0);
                    return TRUE;
                }
            }
            break;
        }
    case WM_INITDIALOG:
        {
            SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)lParam);
            pThis = (Plugin_Library*)lParam;

            // Get information about IDC_LIST1
            HWND hList = GetDlgItem(hWnd, IDC_LIST1);
            RECT rcControl;
            GetControlRect(hList, &rcControl);

            // Create the list
            LIST_COLUMN c[10];
            RECT prect;

            SetRect(&prect, 0, 0, 0, 0);

            c[COLUMN_STALE_DATE].Set(L"Date", LIST_LEFT, 100);
            c[COLUMN_STALE_FILENAME].Set(L"FileName", LIST_LEFT, 100);
            c[COLUMN_STALE_GUESTS].Set(L"Guests", LIST_LEFT, 100);
            c[COLUMN_STALE_ID].Set(L"ID", LIST_LEFT, 100);

            pThis->cStaleListEpisodes.Create(hWnd, g_hInstance, prect, rcControl);
            pThis->cStaleListEpisodes.SetStyle(LVS_SHAREIMAGELISTS | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_CHILD | WS_VISIBLE);
            pThis->cStaleListEpisodes.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);
            pThis->cStaleListEpisodes.ShowHeader(4, c);

            pThis->cStaleListEpisodes.Size();

            pThis->cStaleListEpisodes.SetSmallImageList(&pThis->cImages);

            DestroyWindow(hList);

            // Check for new episodes.
            if(pThis->_CheckForStaleEpisodes(hWnd) == 0)
            {
                EndDialog(hWnd, 0);
            }

            return TRUE;
        }
    }

    return FALSE;
}


HRESULT CALLBACK Plugin_Library::EnumStaleEpisodesProc(LLEpisode* pEpisode, PVOID pUser)
{
    Plugin_Library* pThis = (Plugin_Library*)pUser;
    CCString FullPath;

    // Get the full path
    FullPath = pThis->pCore->GetMediaPath();
    FullPath.Path_Append(pEpisode->FileName);

    // If it doesn't exist, put it in the list.
    if(FullPath.Path_FileExists() == FALSE)
    {
        int i;
        CCString s;

        pThis->cStaleListEpisodes.AddItem(&i, 0, pThis->iImageEpisode);

        s.FromGuid(&pEpisode->guid);
        pThis->cStaleListEpisodes.AddItemText(i, COLUMN_STALE_ID, s);

        pThis->cStaleListEpisodes.AddItemText(i, COLUMN_STALE_FILENAME, pEpisode->FileName);

        LLFormatDate(pEpisode, &s);
        pThis->cStaleListEpisodes.AddItemText(i, COLUMN_STALE_DATE, s);

        pThis->cStaleListEpisodes.AddItemText(i, COLUMN_STALE_GUESTS, pEpisode->Guests);
    }

    return S_OK;
}


BOOL Plugin_Library::_CheckForStaleEpisodes(HWND hWnd)
{
    this->pCore->db.EnumEpisodes(Plugin_Library::EnumStaleEpisodesProc, this, &this->pCore->Log);
    if(this->cStaleListEpisodes.GetItemCount() > 0) return TRUE;
    return FALSE;
}


void Plugin_Library::_RemoveAllEpisodes(HWND hWnd)
{
    int i;
    int iTotal = this->cStaleListEpisodes.GetItemCount();
    CCString s;
    GUID guid;
    LL_DBUPDATE_STRUCT ms;

    for(i=0;i<iTotal;i++)
    {
        this->cStaleListEpisodes.GetItemText(i, COLUMN_STALE_ID, &s);
        s.ToGuid(&guid);

        if(SUCCEEDED(this->pCore->db.RemoveEpisode(guid, &this->pCore->Log)))
        {
            ms.dwOperation = DBUPDATE_REMOVE;
            ms.dwType = DBUPDATE_EPISODE;
            this->pCore->SendPluginMessage(LL_DBUPDATE, &ms);
        }
    }
}




