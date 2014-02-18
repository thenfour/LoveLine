

#include "LLPlayerCore.h"
#include "LLPlayerApp.h"
#include <shlwapi.h>


extern HINSTANCE g_hInstance;


LLPlayerCore::LLPlayerCore()
{
    ZeroMemory(&this->idCurrentEpisode, sizeof(GUID));
    this->pPlugins = NULL;
}


LLPlayerCore::~LLPlayerCore()
{
}


HRESULT LLPlayerCore::SetPlugins(LL_PLUGIN_INFORMATION* pPlugins)
{
    this->pPlugins = pPlugins;
    return S_OK;
}


HRESULT LLPlayerCore::Initialize(LLPlayerApp* pOwner, CCSplash* pSplash)
{
    if(pSplash) pSplash->SetStatusText(L"Initializing Player Core");

    RECT orect = {0,0,1,1};
    RECT prect = {0,0,0,0};

    this->MediaNotificationWindow.Create(
        L"LLPlayerCore Media Notification Window",
        LLPlayerCore::WndProc, g_hInstance, prect, orect, 0, this, 0);

    GetModuleFileName(GetModuleHandle(NULL), this->BinPath.GetBuffer(MAX_PATH), MAX_PATH);
    this->BinPath.Path_StripToPath();

    this->AppRoot = this->BinPath;
    this->AppRoot.Path_Append(L"..");
    this->DatabasePath = this->AppRoot;
    this->DatabasePath.Path_Append(L"Database");
    this->MediaPath  = this->AppRoot;
    this->MediaPath.Path_Append(L"Media");

    this->pOwner = pOwner;

    this->DatabaseName = this->DatabasePath;
    this->DatabaseName.Path_Append(L"db.xml");

    this->Player.SetNotificationWindow(this->MediaNotificationWindow);

    if(pSplash) pSplash->SetStatusText(L"Initializing LoveLine Database");

    if(FAILED(this->db.Open(this->DatabaseName, &this->Log)))
    {
        return E_FAIL;
    }

    return S_OK;
}


HRESULT LLPlayerCore::SeekEpisode(GUID idEpisode, DWORD dwMS)
{
    LLEpisode e;
    CCString sFullPath;
    LL_SEEKEPISODE_STRUCT se;
    BOOL bIsPlaying;

    bIsPlaying = this->Player.IsPlaying();

    if(memcmp(&idEpisode, &this->idCurrentEpisode, sizeof(GUID)) != 0)
    {
        /*
            This is only necessary when requesting to move to a DIFFERENT
            episode.
        */

        if(FAILED(this->db.GetEpisode(idEpisode, &e, &this->Log)))
        {
            return E_FAIL;
        }

        // Stop the player first.
        if(FAILED(this->Player.Stop(&this->Log)))
        {
            return E_FAIL;
        }

        this->SendPluginMessage(LL_STOP, 0);

        sFullPath = this->GetFullMediaPath(e.FileName);

        if(FAILED(this->Player.Open(sFullPath, &this->Log)))
        {
            return E_FAIL;
        }

        this->Player.GetLength(&this->dwLength, &this->Log);

        this->idCurrentEpisode = idEpisode;

        se.guidEpisode = idEpisode;
        this->SendPluginMessage(LL_SEEKEPISODE, &se);
    }

    if(FAILED(this->SeekCurrentEpisode(dwMS)))
    {
        return E_FAIL;
    }

    if(bIsPlaying == TRUE)
    {
        if(FAILED(this->Player.Play(&this->Log)))
        {
            return E_FAIL;
        }
    }

    return S_OK;
}


HRESULT LLPlayerCore::SeekCurrentEpisode(DWORD dwMS)
{
    if(FAILED(this->Player.Seek(dwMS, &this->Log)))
    {
        return E_FAIL;
    }

    this->dwCurrentPos = dwMS;

    LL_SEEKCURRENT_STRUCT sc;
    sc.dwMS = dwMS;
    this->SendPluginMessage(LL_SEEKCURRENT, &sc);

    return S_OK;
}


HRESULT LLPlayerCore::Uninitialize()
{
    DWORD dwPos;

    if(SUCCEEDED(this->Player.GetPos(&dwPos, &this->Log)))
    {
        this->Player.Stop(&this->Log);
        this->SendPluginMessage(LL_STOP, 0);
        this->Player.Close();

        this->db.SetLastPosition(this->idCurrentEpisode, dwPos, &this->Log);
        this->db.Close();
    }

    this->MediaNotificationWindow.Destroy();

    return S_OK;
}


HRESULT LLPlayerCore::SendPluginMessage(DWORD dwMsg, PVOID pParam)
{
    LL_PLUGIN_INFORMATION* p = this->pPlugins;

    if(dwMsg == LL_DBUPDATE)
    {
        // If the database is updated, let's make sure our current episode
        // is still good.
        if(this->IsCurrentEpisodeValid() == FALSE)
        {
            this->Player.Stop(&this->Log);
            ZeroMemory(&this->idCurrentEpisode, sizeof(GUID));
        }
    }

    while(p)
    {
        p->pNotify(p->pUser, dwMsg, pParam);
        p = p->pNext;
    }

    return S_OK;
}


CCString LLPlayerCore::GetMediaPath()
{
    return this->MediaPath;
}


CCString LLPlayerCore::GetDatabasePath()
{
    return this->DatabasePath;
}


CCString LLPlayerCore::GetBinPath()
{
    return this->BinPath;
}


CCString LLPlayerCore::GetAppRoot()
{
    return this->AppRoot;
}


CCString LLPlayerCore::GetDatabaseName()
{
    return this->DatabaseName;
}


CCString LLPlayerCore::GetFullMediaPath(CCString sFileName)
{
    CCString s;

    if(sFileName.Path_FileExists() == TRUE)
    {
        return sFileName;
    }

    s = this->MediaPath;
    s.Path_Append(sFileName);

    return s;
}


HRESULT LLPlayerCore::GetCurrentEpisode(GUID* p)
{
    memcpy(p, &this->idCurrentEpisode, sizeof(GUID));
    return S_OK;
}


DWORD LLPlayerCore::GetLength()
{
    return this->dwLength;
}


LRESULT CALLBACK LLPlayerCore::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LLPlayerCore* pThis = (LLPlayerCore*)GetWindowLong(hWnd, GWL_USERDATA);

    switch(uMsg)
    {
    case CCM_COMPLETE:
        {
            LL_SEEKEPISODE_STRUCT p;
            p.guidEpisode = pThis->idCurrentEpisode;
            pThis->SendPluginMessage(LL_COMPLETE, &p);
            break;
        }
    case WM_CREATE:
        {
            CREATESTRUCT* pcs = (CREATESTRUCT*)lParam;
            SetWindowLong(hWnd, GWL_USERDATA, (LONG)pcs->lpCreateParams);
            pThis = (LLPlayerCore*)pcs->lpCreateParams;
            break;
        }
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


BOOL LLPlayerCore::IsCurrentEpisodeValid()
{
    RPC_STATUS status;
    if(UuidIsNil(&this->idCurrentEpisode, &status) == TRUE) return FALSE;

    LLEpisode e;
    if(FAILED(this->db.GetEpisode(this->idCurrentEpisode, &e, &this->Log)))
    {
        return FALSE;
    }

    return TRUE;
}

