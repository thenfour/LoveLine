

#include "P_Episode.h"
#include "..\\LoveLineUtil\\LLUtils.h"


#define SORT_IN 1
#define SORT_OUT 2
#define SORT_LENGTH 3


#define COLUMN_LINK_TITLE    1
#define COLUMN_LINK_PREVIEW  0
#define COLUMN_LINK_TIME     2
#define COLUMN_LINK_ID       3

#define COLUMN_COMMERCIAL_PREVIEW  0
#define COLUMN_COMMERCIAL_IN       1
#define COLUMN_COMMERCIAL_OUT      2
#define COLUMN_COMMERCIAL_LENGTH   3
#define COLUMN_COMMERCIAL_ID       4


Plugin_Episode::Plugin_Episode()
{
    this->pPlugins = NULL;
    this->pCore = NULL;
}


Plugin_Episode::~Plugin_Episode()
{
}


LPARAM CALLBACK Plugin_Episode::WndProc(
    HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Plugin_Episode* pThis = (Plugin_Episode*)GetWindowLongPtr(hWnd, GWL_USERDATA);

    switch(uMsg)
    {
    case WM_NOTIFY:
        {
            NMHDR* pnmh = (NMHDR*)lParam;
            NMITEMACTIVATE* pia = NULL;
            NMLISTVIEW* plv = NULL;
            int iColumn;

            if(plv = pThis->cListLinks.IsSelectedNotify(lParam))
            {
                // Play the link.
                pThis->cListLinks.SetSelectionMark(plv->iItem);
                pThis->_OnPlayLink();
                return 0;
            }

            if((iColumn = pThis->cListCommercials.IsSortNotify(lParam)) != -1)
            {
                switch(iColumn)
                {
                case COLUMN_COMMERCIAL_PREVIEW:
                    pThis->cListCommercials.SortByLParam(pThis, Plugin_Episode::SortProc);
                    return 0;
                case COLUMN_COMMERCIAL_IN:
                    pThis->dwSortBy = SORT_IN;
                    pThis->iColumn = iColumn;
                    pThis->cListCommercials.SortById(pThis, Plugin_Episode::SortCommercialProc);
                    return 0;
                case COLUMN_COMMERCIAL_OUT:
                    pThis->dwSortBy = SORT_OUT;
                    pThis->iColumn = iColumn;
                    pThis->cListCommercials.SortById(pThis, Plugin_Episode::SortCommercialProc);
                    return 0;
                case COLUMN_COMMERCIAL_LENGTH:
                    pThis->dwSortBy = SORT_LENGTH;
                    pThis->iColumn = iColumn;
                    pThis->cListCommercials.SortById(pThis, Plugin_Episode::SortCommercialProc);
                    return 0;
                default:
                    pThis->cListCommercials.SortTextAutomatically(iColumn);
                    return 0;
                }

                return 0;
            }

            if((iColumn = pThis->cListLinks.IsSortNotify(lParam)) != -1)
            {
                switch(iColumn)
                {
                case COLUMN_LINK_PREVIEW:
                case COLUMN_LINK_TIME:
                    pThis->cListLinks.SortByLParam(pThis, Plugin_Episode::SortProc);
                    return 0;
                case COLUMN_LINK_ID:
                case COLUMN_LINK_TITLE:
                default:
                    pThis->cListLinks.SortTextAutomatically(iColumn);
                    return 0;
                }
                return 0;
            }

            break;
        }
    case WM_COMMAND:
        {
            UINT uId = (UINT)((DWORD)wParam & 0xFFFF);

            if(uId == pThis->cCancel)
            {
                // On cancel
                pThis->_OnCancel();
                return 0;
            }

            if(uId == pThis->cBrowse)
            {
                // On browse
                pThis->_OnBrowse();
                return 0;
            }

            if(uId == pThis->cSave)
            {
                // On save
                pThis->_OnSave();
                return 0;
            }

            if(uId == pThis->cEdit)
            {
                // On Edit
                pThis->_OnEdit();
                return 0;
            }

            if(uId == pThis->cRemoveCommercial)
            {
                // On remove commercial
                pThis->_OnRemoveCommercial();
                return 0;
            }

            if(uId == pThis->cRemoveLink)
            {
                // On remove link
                pThis->_OnRemoveLink();
                return 0;
            }

            return TRUE;
        }
    case WM_SIZE:
        {
            pThis->cCancel.Size();
            pThis->cEdit.Size();
            pThis->cEpisodeInfo.Size();
            pThis->cListCommercials.Size();
            pThis->cListLinks.Size();
            pThis->cRemoveCommercial.Size();
            pThis->cRemoveLink.Size();
            pThis->cSave.Size();
            pThis->cStaticDate.Size();
            pThis->cStaticDescription.Size();
            pThis->cStaticFile.Size();
            pThis->cStaticGuests.Size();
            pThis->cBrowse.Size();
            pThis->cDate.Size();
            pThis->cGuests.Size();
            pThis->cFile.Size();
            pThis->cDescription.Size();
            return 0;
        }
    case WM_CREATE:
        {
            CREATESTRUCT* pcs = (CREATESTRUCT*)lParam;
            pThis = (Plugin_Episode*)pcs->lpCreateParams;
            SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)pThis);
            return 0;
        }
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


void CALLBACK Plugin_Episode::NotificationProc(
    PVOID pUser, DWORD dwMsg, PVOID pParams)
{
    Plugin_Episode* pThis = (Plugin_Episode*)pUser;

    switch(dwMsg)
    {
    case LL_DBUPDATE:
        {
            LL_DBUPDATE_STRUCT* ps = (LL_DBUPDATE_STRUCT*)pParams;
            pThis->Refresh();
            break;
        }
    case LL_SEEKEPISODE:
        {
            LL_SEEKEPISODE_STRUCT* ps = (LL_SEEKEPISODE_STRUCT*)pParams;
            pThis->Refresh();
            break;
        }
    case LL_ATTACH:
        {
            LL_ATTACH_STRUCT* pas = (LL_ATTACH_STRUCT*)pParams;
            StandardPlugins* psp = (StandardPlugins*)pas->pUserData;
            pThis = (Plugin_Episode*)psp->pEpisode;

            pThis->pPlugins = psp;
            pThis->pCore = pas->pCore;

            pThis->Create();

            pas->hWnd = pThis->cMainWindow;
            pas->pUserData = pThis;

            break;
        }
    case LL_INITIALIZE:
        {
            /*
                Load Settings
            */
            LoadListViewSettings(L"Episode_Commercials", &pThis->cListCommercials, &pThis->pCore->db, &pThis->pCore->Log);
            LoadListViewSettings(L"Episode_Links", &pThis->cListLinks, &pThis->pCore->db, &pThis->pCore->Log);
            break;
        }
    case LL_DETACH:
        {

            /*
                Save Settings
            */
            SaveListViewSettings(L"Episode_Commercials", &pThis->cListCommercials, &pThis->pCore->db, &pThis->pCore->Log);
            SaveListViewSettings(L"Episode_Links", &pThis->cListLinks, &pThis->pCore->db, &pThis->pCore->Log);

            pThis->Destroy();
            break;
        }
    }

    return;
}


HRESULT Plugin_Episode::Create()
{
    HINSTANCE hInstance;
    RECT orect;
    RECT prect;

    hInstance = GetModuleHandle(NULL);

    SetRect(&prect, 0, 0, 100, 100);
    SetRect(&orect, 0, 0, 0, 0);

    this->cMainWindow.Create(L"Plugin_Episode",
        Plugin_Episode::WndProc, hInstance, prect, orect, this);

    // The "Episode Info" Group box
    SetRect(&prect, 0, 0, 100, 0);
    SetRect(&orect, 6, 6, -6, 141);
    this->cEpisodeInfo.CreateGroup(
        L"Episode Info", this->cMainWindow, hInstance, prect, orect);

    // The Edit boxes
    SetRect(&prect, 0, 0, 60, 0);
    SetRect(&orect, 69, 21, -6, 42);
    this->cDate.Create(this->cMainWindow, hInstance, prect, orect);

    SetRect(&prect, 0, 0, 60, 0);
    SetRect(&orect, 69, 48, -6, 69);
    this->cGuests.Create(this->cMainWindow, hInstance, prect, orect);

    SetRect(&prect, 0, 0, 60, 0);
    SetRect(&orect, 69, 75, -33, 96);
    this->cFile.Create(this->cMainWindow, hInstance, prect, orect);

    // The static boxes
    SetRect(&prect, 0, 0, 0, 0);
    SetRect(&orect, 15, 26, 69, 38);
    this->cStaticDate.Create(L"Date", this->cMainWindow, hInstance, prect, orect);

    SetRect(&prect, 0, 0, 0, 0);
    SetRect(&orect, 15, 53, 69, 65);
    this->cStaticGuests.Create(L"Guests", this->cMainWindow, hInstance, prect, orect);

    SetRect(&prect, 0, 0, 0, 0);
    SetRect(&orect, 15, 80, 69, 92);
    this->cStaticFile.Create(L"File", this->cMainWindow, hInstance, prect, orect);

    // Description stuff
    SetRect(&prect, 60, 0, 100, 0);
    SetRect(&orect, 0, 21, -15, 33);
    this->cStaticDescription.Create(L"Description", this->cMainWindow, hInstance, prect, orect);

    SetRect(&prect, 60, 0, 100, 0);
    SetRect(&orect, 0, 42, -15, 123);
    this->cDescription.Create(this->cMainWindow, hInstance, prect, orect);

    // Buttons on the top section.
    SetRect(&prect, 0, 0, 0, 0);
    SetRect(&orect, 15, 102, 90, 123);
    this->cEdit.Create(L"Edit", this->cMainWindow, hInstance, prect, orect);

    SetRect(&prect, 0, 0, 0, 0);
    SetRect(&orect, 96, 102, 171, 123);
    this->cCancel.Create(L"Cancel", this->cMainWindow, hInstance, prect, orect);

    SetRect(&prect, 0, 0, 0, 0);
    SetRect(&orect, 177, 102, 252, 123);
    this->cSave.Create(L"Save", this->cMainWindow, hInstance, prect, orect);

    SetRect(&prect, 60, 0, 60, 0);
    SetRect(&orect, -27, 75, -6, 96);
    this->cBrowse.Create(L"...", this->cMainWindow, hInstance, prect, orect);

    // Bottom stuff
    LIST_COLUMN c[10];

    SetRect(&prect, 0, 0, 50, 100);
    SetRect(&orect, 6, 150, -3, -33);
    this->cListCommercials.Create(this->cMainWindow, hInstance, prect, orect);

    c[COLUMN_COMMERCIAL_IN].Set(L"In", LIST_LEFT, 100);
    c[COLUMN_COMMERCIAL_OUT].Set(L"Out", LIST_LEFT, 100);
    c[COLUMN_COMMERCIAL_ID].Set(L"ID", LIST_LEFT, 100);
    c[COLUMN_COMMERCIAL_LENGTH].Set(L"Length", LIST_LEFT, 100);
    c[COLUMN_COMMERCIAL_PREVIEW].Set(L"Preview", LIST_LEFT, PREVIEW_SIZEX);
    this->cListCommercials.ShowHeader(5, c);
    this->cListCommercials.SetStyle(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_CHILD | WS_VISIBLE);
    this->cListCommercials.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);

    SetRect(&prect, 50, 0, 100, 100);
    SetRect(&orect, 3, 150, -6, -33);
    this->cListLinks.Create(this->cMainWindow, hInstance, prect, orect);

    c[COLUMN_LINK_PREVIEW].Set(L"Preview", LIST_LEFT, PREVIEW_SIZEX);
    c[COLUMN_LINK_TITLE].Set(L"Title", LIST_LEFT, 100);
    c[COLUMN_LINK_ID].Set(L"ID", LIST_LEFT, 100);
    c[COLUMN_LINK_TIME].Set(L"Time", LIST_LEFT, 100);
    this->cListLinks.ShowHeader(4, c);
    this->cListLinks.SetStyle(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_CHILD | WS_VISIBLE);
    this->cListLinks.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);

    // Buttons on the bottom
    SetRect(&prect, 0, 100, 0, 100);
    SetRect(&orect, 6, -27, 81, -6);
    this->cRemoveCommercial.Create(L"Remove", this->cMainWindow, hInstance, prect, orect);

    SetRect(&prect, 50, 100, 50, 100);
    SetRect(&orect, 6, -27, 81, -6);
    this->cRemoveLink.Create(L"Remove", this->cMainWindow, hInstance, prect, orect);

    this->cImages.CreateEmpty(PREVIEW_SIZEX, PREVIEW_SIZEY, ILC_COLORDDB);
    this->cListCommercials.SetSmallImageList(&this->cImages);
    this->cListLinks.SetSmallImageList(&this->cImages);

    this->_SetControlsReadOnly();

    return S_OK;
}


HRESULT Plugin_Episode::Destroy()
{
    this->cCancel.Destroy();
    this->cEdit.Destroy();
    this->cEpisodeInfo.Destroy();
    this->cListCommercials.Destroy();
    this->cListLinks.Destroy();
    this->cRemoveCommercial.Destroy();
    this->cRemoveLink.Destroy();
    this->cSave.Destroy();
    this->cStaticDate.Destroy();
    this->cStaticDescription.Destroy();
    this->cStaticFile.Destroy();
    this->cStaticGuests.Destroy();
    this->cBrowse.Destroy();
    this->cDate.Destroy();
    this->cGuests.Destroy();
    this->cFile.Destroy();
    this->cDescription.Destroy();

    this->cMainWindow.Destroy();
    return S_OK;
}


HRESULT Plugin_Episode::Refresh()
{
    LLEpisode e;
    CCString s;

    // All current changes will be lost when this happens.
    this->_SetControlsReadOnly();

    this->cListCommercials.Clear();
    this->cListLinks.Clear();
    this->cImages.Clear();

    // Load defaults.
    this->cDate.SetText(L"(no episode)");
    this->cGuests.SetText(L"");
    this->cDescription.SetText(L"");
    this->cFile.SetText(L"");

    // Get the episode.
    this->pCore->GetCurrentEpisode(&e.guid);
    if(FAILED(this->pCore->db.GetEpisode(e.guid, &e, &this->pCore->Log)))
    {
        // FAILED!  This could happen when
        // the user deletes the currently playing episode.
         return E_FAIL;
    }

    // Fill in stuff.
    LLFormatDate(&e, &s);
    this->cDate.SetText(s);
    this->cFile.SetText(e.FileName);
    this->cGuests.SetText(e.Guests);
    this->cDescription.SetText(e.Description);

    // Fill in links
    if(FAILED(this->pCore->db.EnumFeatures(e.guid, Plugin_Episode::EnumBookmarks,
        this, &this->pCore->Log)))
    {
        //return E_FAIL;
    }

    // Fill in commercials
    if(FAILED(this->pCore->db.EnumCommercials(e.guid, Plugin_Episode::EnumCommercials,
        this, &this->pCore->Log)))
    {
        //return E_FAIL;
    }

    this->cListLinks.SimulateColumnClick(this->cListLinks.GetLastColumnClicked());
    this->cListCommercials.SimulateColumnClick(this->cListCommercials.GetLastColumnClicked());

    return S_OK;
}


void Plugin_Episode::_SetControlsReadOnly()
{
    this->cDate.SetReadOnly(TRUE);
    this->cGuests.SetReadOnly(TRUE);
    this->cDescription.SetReadOnly(TRUE);
    this->cFile.SetReadOnly(TRUE);

    this->cBrowse.SetActive(FALSE);
    this->cEdit.SetActive(TRUE);
    this->cSave.SetActive(FALSE);
    this->cCancel.SetActive(FALSE);
}


void Plugin_Episode::_SetControlsEdit()
{
    //this->cDate.SetReadOnly(FALSE);
    this->cDate.SetReadOnly(TRUE);

    this->cGuests.SetReadOnly(FALSE);
    this->cDescription.SetReadOnly(FALSE);
    this->cFile.SetReadOnly(FALSE);

    this->cBrowse.SetActive(TRUE);
    this->cEdit.SetActive(FALSE);
    this->cSave.SetActive(TRUE);
    this->cCancel.SetActive(TRUE);
}


void Plugin_Episode::_OnEdit()
{
    if(this->pCore->IsCurrentEpisodeValid() == FALSE) return;
    this->_SetControlsEdit();
}


void Plugin_Episode::_OnSave()
{
    // Gather the information from the boxes.
    LLEpisode e;
    this->pCore->GetCurrentEpisode(&e.guid);

    if(FAILED(this->pCore->db.GetEpisode(e.guid, &e, &this->pCore->Log)))
    {
        // Error....
        return;
    }

    e.Guests.FromWindow(this->cGuests);
    e.Description.FromWindow(this->cDescription);

    e.FileName.FromWindow(this->cFile);

    if(FAILED(this->pCore->db.SetEpisode(&e, &this->pCore->Log)))
    {
        this->pCore->Log.AddEntry(L"Failed to save the episode's data.");
    }

    LL_DBUPDATE_STRUCT ms;
    ms.dwOperation = DBUPDATE_CHANGED;
    ms.dwType = DBUPDATE_EPISODE;
    this->pCore->SendPluginMessage(LL_DBUPDATE, &ms);
}


void Plugin_Episode::_OnCancel()
{
    this->Refresh();
}


void Plugin_Episode::_OnRemoveCommercial()
{
    GUID id;
    CCString sGuid;
    int nSel;

    nSel = this->cListCommercials.GetSelectionMark();
    if(nSel == -1) return;

    this->cListCommercials.GetItemText(nSel, COLUMN_COMMERCIAL_ID, &sGuid);
    sGuid.ToGuid(&id);

    if(FAILED(this->pCore->db.RemoveCommercial(id, &this->pCore->Log)))
    {
        return;
    }

    LL_DBUPDATE_STRUCT ms;
    ms.dwOperation = DBUPDATE_REMOVE;
    ms.dwType = DBUPDATE_EPISODE;
    this->pCore->SendPluginMessage(LL_DBUPDATE, &ms);
}


void Plugin_Episode::_OnRemoveLink()
{
    GUID id;
    CCString sGuid;
    int nSel;

    nSel = this->cListLinks.GetSelectionMark();
    if(nSel == -1) return;

    this->cListLinks.GetItemText(nSel, COLUMN_LINK_ID, &sGuid);
    sGuid.ToGuid(&id);

    if(FAILED(this->pCore->db.RemoveLink(id, &this->pCore->Log)))
    {
        return;
    }

    LL_DBUPDATE_STRUCT ms;
    ms.dwOperation = DBUPDATE_REMOVE;
    ms.dwType = DBUPDATE_LINK;
    this->pCore->SendPluginMessage(LL_DBUPDATE, &ms);
}


HRESULT CALLBACK Plugin_Episode::EnumCommercials(LLCommercial* pc, PVOID pUser)
{
    Plugin_Episode* pThis = (Plugin_Episode*)pUser;
    int i;
    int iImage;
    HBITMAP hbm = NULL;
    CCString s;

    hbm = CreateCommercialPreviewImage(pThis->pCore->GetLength(), pc);

    pThis->cImages.AddBitmapFromHandle(&iImage, hbm);

    pThis->cListCommercials.AddItem(&i, pc->dwIn, iImage);

    LLFormatMilliseconds(max(pc->dwIn, pc->dwOut), &s);
    pThis->cListCommercials.AddItemText(i, COLUMN_COMMERCIAL_OUT, s);

    LLFormatMilliseconds(min(pc->dwIn, pc->dwOut), &s);
    pThis->cListCommercials.AddItemText(i, COLUMN_COMMERCIAL_IN, s);

    LLFormatMilliseconds(abs(pc->dwIn - pc->dwOut), &s);
    pThis->cListCommercials.AddItemText(i, COLUMN_COMMERCIAL_LENGTH, s);

    s.FromGuid(&pc->guid);
    pThis->cListCommercials.AddItemText(i, COLUMN_COMMERCIAL_ID, s);

    return S_OK;
}


HRESULT CALLBACK Plugin_Episode::EnumBookmarks(LLLink* pl, LLFeature* pf, PVOID pUser)
{
    Plugin_Episode* pThis = (Plugin_Episode*)pUser;
    int i;
    CCString s;
    HBITMAP hbm = NULL;
    int iImage;

    hbm = CreateLinkPreviewImage(pThis->pCore->GetLength(), pl);

    pThis->cImages.AddBitmapFromHandle(&iImage, hbm);

    pThis->cListLinks.AddItem(&i, pl->dwPos, iImage);

    s.FromGuid(&pl->guid);
    pThis->cListLinks.AddItemText(i, COLUMN_LINK_ID, s);

    LLFormatMilliseconds(pl->dwPos, &s);
    pThis->cListLinks.AddItemText(i, COLUMN_LINK_TIME, s);

    pThis->cListLinks.AddItemText(i, COLUMN_LINK_TITLE, pf->Title);

    return S_OK;
}


int CALLBACK Plugin_Episode::SortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    return lParam1 - lParam2;
}


int CALLBACK Plugin_Episode::SortCommercialProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    Plugin_Episode* pThis = (Plugin_Episode*)lParamSort;
    LLCommercial c1;
    LLCommercial c2;
    CCString sId1;
    CCString sId2;

    pThis->cListCommercials.GetItemText(lParam1, COLUMN_COMMERCIAL_ID, &sId1);
    pThis->cListCommercials.GetItemText(lParam2, COLUMN_COMMERCIAL_ID, &sId2);

    sId1.ToGuid(&c1.guid);
    sId2.ToGuid(&c2.guid);

    // Get the commercials.
    pThis->pCore->db.GetCommercial(c1.guid, &c1, &pThis->pCore->Log);
    pThis->pCore->db.GetCommercial(c2.guid, &c2, &pThis->pCore->Log);

    switch(pThis->iColumn)
    {
    case COLUMN_COMMERCIAL_IN:
        return min(c2.dwIn, c2.dwOut) - min(c1.dwIn, c1.dwOut);
    case COLUMN_COMMERCIAL_OUT:
        return max(c2.dwIn, c2.dwOut) - max(c1.dwIn, c1.dwOut);
    case COLUMN_COMMERCIAL_LENGTH:
        return abs(c2.dwIn - c2.dwOut) - abs(c1.dwIn - c1.dwOut);
    }

    return 0;
}


void Plugin_Episode::_OnPlayLink()
{
    // Determine what's selected.
    int iSel;
    CCString sGuid;
    GUID idLink;
    LLLink l;

    iSel = this->cListLinks.GetSelectionMark();

    if(iSel == -1)
    {
        return;
    }

    this->cListLinks.GetItemText(iSel, COLUMN_LINK_ID, &sGuid);
    sGuid.ToGuid(&idLink);

    // Grab the link from the database.
    if(FAILED(this->pCore->db.GetLink(idLink, &l, &this->pCore->Log)))
    {
        return;
    }

    this->pCore->SeekEpisode(l.guidEpisode, l.dwPos);
}


void Plugin_Episode::_OnBrowse()
{
    CCString FileName;
    FILE_FILTER pFilters[2];
    pFilters[0].Set(L"Media Files (*.wm?;*.mp3;*.wav)", L"*.wm?;*.mp3;*.wav");
    pFilters[1].Set(L"All Files (*.*)", L"*.*");

    if(FAILED(OpenFileDlg(
        this->cMainWindow, &FileName, 2, pFilters, this->pCore->GetMediaPath())))
    {
        // Cancel...
        return;
    }

    FileName.Path_AbsoluteToRelative(this->pCore->GetMediaPath());
    this->cFile.SetText(FileName);
}



