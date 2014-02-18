

#include "stdafx.h"
#include "P_Features.h"
#include "..\\LoveLineUtil\\LLUtils.h"
#include "Resource.h"


#define COLUMN_TITLE        0
#define COLUMN_DESCRIPTION  1
#define COLUMN_FEATURE_ID   2

#define COLUMN_DATE         0
#define COLUMN_TIME         1
#define COLUMN_GUESTS       2 
#define COLUMN_LINK_ID      3


extern HINSTANCE g_hInstance;


Plugin_Features::Plugin_Features()
{
    this->pPlugins = NULL;
    this->pCore = NULL;
}


Plugin_Features::~Plugin_Features()
{
}


LPARAM CALLBACK Plugin_Features::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Plugin_Features* pThis = (Plugin_Features*)GetWindowLongPtr(hWnd, GWL_USERDATA);

    switch(uMsg)
    {
    case WM_NOTIFY:
        {
            NMLISTVIEW* pnml;
            NMITEMACTIVATE* pnmia;
            int iColumn;
            if((iColumn = pThis->cListFeatures.IsSortNotify(lParam)) != -1)
            {
                pThis->cListFeatures.SortTextAutomatically(iColumn);
                return TRUE;
            }

            if((iColumn = pThis->cListLinks.IsSortNotify(lParam)) != -1)
            {
                if(iColumn == COLUMN_TIME)
                {
                    pThis->cListLinks.SortByLParam(pThis, Plugin_Features::SortEpisodeProc);
                }
                else
                {
                    pThis->cListLinks.SortTextAutomatically(iColumn);
                }
                return TRUE;
            }

            if(pnmia = pThis->cListLinks.IsActivatedNotify(lParam))
            {
                pThis->OnPlay();
                return 0;
            }

            if(pnml = pThis->cListFeatures.IsSelectedNotify(lParam))
            {
                // The user has selected an item from the feature list.
                // Now we have to fill in the "links" list.
                pThis->cListFeatures.SetSelectionMark(pnml->iItem);
                pThis->OnSelectFeature();
                return 0;
            }

            break;
        }
    case WM_COMMAND:
        {
            UINT uId = (UINT)((DWORD)wParam & 0xFFFF);

            NMHDR* pnmh = (NMHDR*)lParam;

            if(HIWORD(wParam) == EN_CHANGE && uId == pThis->cEditSearch)
            {
                pThis->OnSearchChange();
                return TRUE;
            }

            if(uId == pThis->cAddNow)
            {
                // On AddNow
                pThis->OnAddNow();
                return 0;
            }
            if(uId == pThis->cNewNow)
            {
                // On NewNow
                pThis->OnNewNow();
                return 0;
            }
            if(uId == pThis->cEditFeature)
            {
                // On Edit Feature.
                pThis->OnEditFeature();
                return 0;
            }
            if(uId == pThis->cNewFeature)
            {
                // On New Feature.
                pThis->OnNewFeature();
                return 0;
            }
            if(uId == pThis->cRemoveFeature)
            {
                // On Remove Feature
                pThis->OnRemoveFeature();
                return 0;
            }
            if(uId == pThis->cRemoveLink)
            {
                // On Remove Link.
                pThis->OnRemoveLink();
                return 0;
            }
            if(uId == pThis->cPlay)
            {
                // On play.
                pThis->OnPlay();
                return 0;
            }

            break;
        }
    case WM_SIZE:
        {
            pThis->cAddNow.Size();
            pThis->cNewNow.Size();
            pThis->cGroupFeatures.Size();
            pThis->cGroupLinks.Size();
            pThis->cStaticSearch.Size();
            pThis->cEditSearch.Size();
            pThis->cListFeatures.Size();
            pThis->cNewFeature.Size();
            pThis->cRemoveFeature.Size();
            pThis->cEditFeature.Size();
            pThis->cEditResults.Size();
            pThis->cPlay.Size();
            pThis->cRemoveLink.Size();
            pThis->cListLinks.Size();
            return 0;
        }
    case WM_CREATE:
        {
            CREATESTRUCT* pcs = (CREATESTRUCT*)lParam;
            pThis = (Plugin_Features*)pcs->lpCreateParams;
            SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)pThis);
            return 0;
        }
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


void CALLBACK Plugin_Features::NotificationProc(PVOID pUser, DWORD dwMsg, PVOID pParams)
{
    Plugin_Features* pThis = (Plugin_Features*)pUser;

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
            LoadListViewSettings(L"Features_Features", &pThis->cListFeatures, &pThis->pCore->db, &pThis->pCore->Log);
            LoadListViewSettings(L"Features_Links", &pThis->cListLinks, &pThis->pCore->db, &pThis->pCore->Log);

            pThis->Refresh();
            break;
        }
    case LL_DBUPDATE:
        {
            LL_DBUPDATE_STRUCT* ps = (LL_DBUPDATE_STRUCT*)pParams;

            if(ps->dwType & (DBUPDATE_FEATURE | DBUPDATE_LINK))
            {
                pThis->Refresh();
            }
            break;
        }
    case LL_ATTACH:
        {
            LL_ATTACH_STRUCT* pas = (LL_ATTACH_STRUCT*)pParams;
            StandardPlugins* psp = (StandardPlugins*)pas->pUserData;
            pThis = (Plugin_Features*)psp->pFeatures;

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


HRESULT Plugin_Features::Create()
{
    HINSTANCE hInstance;
    RECT orect;
    RECT prect;
    LIST_COLUMN c[10];

    hInstance = GetModuleHandle(NULL);

    SetRect(&prect, 0, 0, 100, 100);
    SetRect(&orect, 0, 0, 0, 0);

    this->cMainWindow.Create(L"Plugin_Features",
        Plugin_Features::WndProc, hInstance, prect, orect, this);

    // The "Features" Group box
    SetRect(&prect, 0, 0, 50, 100);
    SetRect(&orect, 6, 6, -3, -6);
    this->cGroupFeatures.CreateGroup(
        L"Features", this->cMainWindow, hInstance, prect, orect);

    // The "Links" group box
    SetRect(&prect, 50, 0, 100, 100);
    SetRect(&orect, 3, 6, -6, -6);
    this->cGroupLinks.CreateGroup(
        L"Links", this->cMainWindow, hInstance, prect, orect);

    // "Search" static box
    SetRect(&prect, 0, 0, 0, 0);
    SetRect(&orect, 15, 26, 72, 42);
    this->cStaticSearch.Create(L"Search", this->cMainWindow, hInstance, prect, orect);

    // The "Search" edit box
    SetRect(&prect, 0, 0, 50, 0);
    SetRect(&orect, 72, 21, -12, 42);
    this->cEditSearch.Create(this->cMainWindow, hInstance, prect, orect);
    this->cEditSearch.SetReadOnly(FALSE);

    // Features ListView
    SetRect(&prect, 0, 0, 50, 100);
    SetRect(&orect, 15, 48, -12, -42);
    this->cListFeatures.Create(this->cMainWindow, hInstance, prect, orect);

    c[COLUMN_TITLE].Set(L"Name", LIST_LEFT, 100);
    c[COLUMN_DESCRIPTION].Set(L"Description", LIST_LEFT, 100);
    c[COLUMN_FEATURE_ID].Set(L"ID", LIST_LEFT, 100);
    this->cListFeatures.ShowHeader(3, c);

    this->cListFeatures.SetStyle(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_CHILD | WS_VISIBLE);
    this->cListFeatures.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);

    // The 3 buttons on the bottom left
    SetRect(&prect, 0, 100, 0, 100);
    SetRect(&orect, 15, -36, 90, -15);
    this->cNewFeature.Create(L"New...", this->cMainWindow, hInstance, prect, orect);

    SetRect(&prect, 0, 100, 0, 100);
    SetRect(&orect, 177, -36, 252, -15);
    this->cRemoveFeature.Create(L"Remove", this->cMainWindow, hInstance, prect, orect);

    SetRect(&prect, 0, 100, 0, 100);
    SetRect(&orect, 96, -36, 171, -15);
    this->cEditFeature.Create(L"Edit...", this->cMainWindow, hInstance, prect, orect);

    // The "Results" edit box
    SetRect(&prect, 50, 0, 100, 0);
    SetRect(&orect, 12, 21, -15, 42);
    this->cEditResults.Create(this->cMainWindow, hInstance, prect, orect);

    // Links ListView
    SetRect(&prect, 50, 0, 100, 100);
    SetRect(&orect, 12, 48, -15, -69);
    this->cListLinks.Create(this->cMainWindow, hInstance, prect, orect);

    c[COLUMN_DATE].Set(L"Date", LIST_LEFT, 100);
    c[COLUMN_TIME].Set(L"Time", LIST_LEFT, 100);
    c[COLUMN_GUESTS].Set(L"Guests", LIST_LEFT, 100);
    c[COLUMN_LINK_ID].Set(L"ID", LIST_LEFT, 100);
    this->cListLinks.ShowHeader(4, c);

    this->cListLinks.SetStyle(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_CHILD | WS_VISIBLE);
    this->cListLinks.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);

    // The 4 buttons on the bottom right
    SetRect(&prect, 50, 100, 50, 100);
    SetRect(&orect, 12, -63, 87, -42);
    this->cNewNow.Create(L"New Now", this->cMainWindow, hInstance, prect, orect);

    SetRect(&prect, 50, 100, 50, 100);
    SetRect(&orect, 93, -63, 168, -42);
    this->cAddNow.Create(L"Add Now", this->cMainWindow, hInstance, prect, orect);

    SetRect(&prect, 50, 100, 50, 100);
    SetRect(&orect, 12, -36, 87, -15);
    this->cPlay.Create(L"Play", this->cMainWindow, hInstance, prect, orect);

    SetRect(&prect, 50, 100, 50, 100);
    SetRect(&orect, 93, -36, 168, -15);
    this->cRemoveLink.Create(L"Remove", this->cMainWindow, hInstance, prect, orect);

    this->cImages.CreateEmpty(ICON_SIZEX, ICON_SIZEY, ILC_COLORDDB);
    this->cImages.AddBitmapFromResource(&this->iEpisodeImage, g_hInstance, IDB_EPISODE);
    this->cImages.AddBitmapFromResource(&this->iFeatureImage, g_hInstance, IDB_FEATURE);

    this->cListFeatures.SetSmallImageList(&this->cImages);
    this->cListLinks.SetSmallImageList(&this->cImages);

    this->sSearchString = L"";

    return S_OK;
}


HRESULT Plugin_Features::Destroy()
{
    /*
        Save settings.
    */
    SaveListViewSettings(L"Features_Features", &this->cListFeatures, &this->pCore->db, &this->pCore->Log);
    SaveListViewSettings(L"Features_Links", &this->cListLinks, &this->pCore->db, &this->pCore->Log);

    this->cAddNow.Destroy();
    this->cNewNow.Destroy();
    this->cGroupFeatures.Destroy();
    this->cGroupLinks.Destroy();
    this->cStaticSearch.Destroy();
    this->cEditSearch.Destroy();
    this->cListFeatures.Destroy();
    this->cNewFeature.Destroy();
    this->cRemoveFeature.Destroy();
    this->cEditFeature.Destroy();
    this->cEditResults.Destroy();
    this->cPlay.Destroy();
    this->cRemoveLink.Destroy();
    this->cListLinks.Destroy();

    return S_OK;
}


HRESULT CALLBACK Plugin_Features::EnumFeaturesProc(LLFeature* pFeature, PVOID pUser)
{
    Plugin_Features* pThis = (Plugin_Features*)pUser;
    int iItem;
    LPARAM lParam = FALSE;
    CCString sGuid;
    CCString sUprFeature;

    // Only add it if it matches our search string
    if(pThis->sSearchString.len() != 0)
    {
        sUprFeature = pFeature->Text;
        sUprFeature.upr();

        if(wcsstr(sUprFeature, pThis->sSearchString) == NULL)
        {
            sUprFeature = pFeature->Title;
            sUprFeature.upr();

            if(wcsstr(sUprFeature, pThis->sSearchString) == NULL)
            {
                return S_OK;
            }
        }
    }

    sGuid.FromGuid(&pFeature->guid);

    if(pThis->nFeatureSelected != -1)
    {
        if(sGuid.cmp(pThis->sidFeatureSelected) == 0)
        {
            lParam = TRUE;
        }
    }

    pThis->cListFeatures.AddItem(&iItem, lParam, pThis->iFeatureImage);
    pThis->cListFeatures.AddItemText(iItem, COLUMN_TITLE, pFeature->Title);
    pThis->cListFeatures.AddItemText(iItem, COLUMN_DESCRIPTION, pFeature->Text);
    pThis->cListFeatures.AddItemText(iItem, COLUMN_FEATURE_ID, sGuid);

    return S_OK;
}


HRESULT Plugin_Features::Refresh()
{
    // Remember the ID of the currently selected feature so we can
    // select it again.
    this->nFeatureSelected = this->cListFeatures.GetSelectionMark();

    if(this->nFeatureSelected != -1)
    {
        this->cListFeatures.GetItemText(this->nFeatureSelected, COLUMN_FEATURE_ID, &this->sidFeatureSelected);
    }

    this->cListFeatures.Clear();
    this->cListLinks.Clear();

    // Fill the list with features.
    this->pCore->db.EnumFeatures(Plugin_Features::EnumFeaturesProc, this, &this->pCore->Log);

    this->cListFeatures.SimulateColumnClick(this->cListFeatures.GetLastColumnClicked());

    // If the enumerator found a match with this->sidFeatureSelected,
    // it will fill the LPARAM with TRUE.
    // So to select it, we just have to find the LPARAM that's set to TRUE.
    this->cListFeatures.SelectAnItem(this->cListFeatures.FindLParam(TRUE));

    return S_OK;
}


void Plugin_Features::OnEditFeature()
{
    CCString sTitle;
    CCString sDesc;
    CCString sGuid;
    int nSel;
    LL_DBUPDATE_STRUCT s;
    LLFeature f;

    // Get the selected feature.
    nSel = this->cListFeatures.GetSelectionMark();

    if(nSel == -1)
    {
        return;
    }

    this->cListFeatures.GetItemText(nSel, COLUMN_FEATURE_ID, &sGuid);
    this->cListFeatures.GetItemText(nSel, COLUMN_TITLE, &sTitle);
    this->cListFeatures.GetItemText(nSel, COLUMN_DESCRIPTION, &sDesc);

    if(FAILED(GetNewFeatureInfo(this->cMainWindow, &sTitle, &sDesc)))
    {
        return;
    }

    sGuid.ToGuid(&f.guid);
    f.Text = sDesc;
    f.Title = sTitle;

    // Now change the feature in the database.
    if(FAILED(this->pCore->db.SetFeature(&f, &this->pCore->Log)))
    {
        this->pCore->Log.AddEntry(L"Could not change the feature.");
        return;
    }

    // Send the message to everyone saying we've updated the database.
    s.dwOperation = DBUPDATE_CHANGED;
    s.dwType = DBUPDATE_FEATURE;
    this->pCore->SendPluginMessage(LL_DBUPDATE, &s);
}


void Plugin_Features::OnNewFeature()
{
    CCString Title = "";
    CCString Desc = "";

    // Show a new feature dialog.
    if(FAILED(GetNewFeatureInfo(this->cMainWindow, &Title, &Desc)))
    {
        return;
    }

    LLFeature f;
    f.Text = Desc;
    f.Title = Title;

    if(FAILED(this->pCore->db.AddFeature(&f, &this->pCore->Log)))
    {
        this->pCore->Log.AddEntry(L"Could not add the new entry titled \"%s\"", (PCWSTR)Title);
        return;
    }

    // Send the message to everyone saying we've updated the database.
    LL_DBUPDATE_STRUCT s;
    s.dwOperation = DBUPDATE_NEW;
    s.dwType = DBUPDATE_FEATURE;
    this->pCore->SendPluginMessage(LL_DBUPDATE, &s);
}


void Plugin_Features::OnRemoveFeature()
{
    // First figure out exactly which feature they're trying to remove.
    int iSel = this->cListFeatures.GetSelectionMark();
    CCString sGuid;
    GUID id;

    if(iSel == -1) return;

    this->cListFeatures.GetItemText(iSel, COLUMN_FEATURE_ID, &sGuid);

    sGuid.ToGuid(&id);

    if(FAILED(this->pCore->db.RemoveFeature(id, &this->pCore->Log)))
    {
        this->pCore->Log.AddEntry(L"Could not remove the specified feature.");
        return;
    }

    LL_DBUPDATE_STRUCT s;
    s.dwOperation = DBUPDATE_REMOVE;
    s.dwType = DBUPDATE_FEATURE;
    this->pCore->SendPluginMessage(LL_DBUPDATE, &s);
}


void Plugin_Features::OnRemoveLink()
{
    int iSel = this->cListLinks.GetSelectionMark();
    CCString sGuid;
    GUID id;

    if(iSel == -1) return;

    this->cListLinks.GetItemText(iSel, COLUMN_LINK_ID, &sGuid);

    sGuid.ToGuid(&id);

    if(FAILED(this->pCore->db.RemoveLink(id, &this->pCore->Log)))
    {
        this->pCore->Log.AddEntry(L"Could not remove the specified link.");
        return;
    }


    LL_DBUPDATE_STRUCT s;
    s.dwOperation = DBUPDATE_REMOVE;
    s.dwType = DBUPDATE_LINK;
    this->pCore->SendPluginMessage(LL_DBUPDATE, &s);
}


void Plugin_Features::OnPlay()
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
    this->pCore->Player.Play(&this->pCore->Log);
}


void Plugin_Features::OnNewNow()
{
    DWORD dwPos;
    LLFeature f;
    LLLink l;
    CCString sDesc;
    CCString sTitle;

    if(this->pCore->IsCurrentEpisodeValid() == FALSE) return;

    // Remember the position now
    this->pCore->Player.GetPos(&dwPos, &this->pCore->Log);

    // Get a new feature
    if(FAILED(GetNewFeatureInfo(this->cMainWindow, &sTitle, &sDesc)))
    {
        // Don't worry about it.
        return;
    }

    // Add the feature.
    f.Text = sDesc;
    f.Title = sTitle;
    if(FAILED(this->pCore->db.AddFeature(&f, &this->pCore->Log)))
    {
        return;
    }

    // Now add a link to the feature.
    l.dwPos = dwPos;
    this->pCore->GetCurrentEpisode(&l.guidEpisode);
    l.guidFeature = f.guid;

    if(FAILED(this->pCore->db.AddLink(&l, &this->pCore->Log)))
    {
        this->pCore->Log.AddEntry(L"Successfully added the feature, but couldn't add a link to the feature.");
        return;
    }

    LL_DBUPDATE_STRUCT s;
    s.dwOperation = DBUPDATE_NEW;
    s.dwType = DBUPDATE_FEATURE | DBUPDATE_LINK;
    this->pCore->SendPluginMessage(LL_DBUPDATE, &s);
}


void Plugin_Features::OnAddNow()
{
    if(this->pCore->IsCurrentEpisodeValid() == FALSE) return;

    // Get the selected feature.
    CCString sGuid;
    LLFeature f;
    LLLink l;
    DWORD dwPos;
    GUID guid;
    int iSel;
    
    // Get player position
    this->pCore->Player.GetPos(&dwPos, &this->pCore->Log);

    // Get the selected ID and feature
    iSel = this->cListFeatures.GetSelectionMark();
    if(iSel == -1) return;

    this->cListFeatures.GetItemText(iSel, COLUMN_FEATURE_ID, &sGuid);

    sGuid.ToGuid(&guid);

    if(FAILED(this->pCore->db.GetFeature(guid, &f, &this->pCore->Log)))
    {
        return;
    }

    // Create a new link
    l.dwPos = dwPos;
    this->pCore->GetCurrentEpisode(&l.guidEpisode);
    l.guidFeature = f.guid;

    // Add it to the database.
    if(FAILED(this->pCore->db.AddLink(&l, &this->pCore->Log)))
    {
        return;
    }

    LL_DBUPDATE_STRUCT s;
    s.dwOperation = DBUPDATE_NEW;
    s.dwType = DBUPDATE_LINK;
    this->pCore->SendPluginMessage(LL_DBUPDATE, &s);

    return;
}


void Plugin_Features::OnSelectFeature()
{
    int iSel = this->cListFeatures.GetSelectionMark();
    CCString sGuid;
    CCString sTitle;
    GUID id;

    if(iSel == -1) return;

    this->cListFeatures.GetItemText(iSel, COLUMN_TITLE, &sTitle);
    this->cEditResults.SetText(sTitle);

    this->cListFeatures.GetItemText(iSel, COLUMN_FEATURE_ID, &sGuid);
    sGuid.ToGuid(&id);

    this->cListLinks.Clear();

    if(FAILED(this->pCore->db.EnumEpisodes(id, Plugin_Features::EnumEpisodesProc,
        this, &this->pCore->Log)))
    {
        this->pCore->Log.AddEntry(L"Could not enumerate episodes.");
    }

    this->cListLinks.SimulateColumnClick(this->cListLinks.GetLastColumnClicked());

    return;
}


HRESULT CALLBACK Plugin_Features::EnumEpisodesProc(LLLink* pl, LLEpisode* pe, PVOID pUser)
{
    Plugin_Features* pThis = (Plugin_Features*)pUser;

    // Add this episode to the list.
    CCString Date;
    CCString Time;
    CCString Guid;

    LLFormatDate(pe, &Date);
    LLFormatMilliseconds(pl->dwPos, &Time);
    Guid.FromGuid(&pl->guid);

    int i;
    pThis->cListLinks.AddItem(&i, pl->dwPos, pThis->iEpisodeImage);
    pThis->cListLinks.AddItemText(i, COLUMN_GUESTS, pe->Guests);
    pThis->cListLinks.AddItemText(i, COLUMN_TIME, Time);
    pThis->cListLinks.AddItemText(i, COLUMN_DATE, Date);
    pThis->cListLinks.AddItemText(i, COLUMN_LINK_ID, Guid);

    return S_OK;
}


int CALLBACK Plugin_Features::SortEpisodeProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    return lParam1 - lParam2;
}


void Plugin_Features::OnSearchChange()
{
    // Change the search string.
    this->sSearchString.FromWindow(this->cEditSearch);
    this->sSearchString.upr();

    this->Refresh();
}














