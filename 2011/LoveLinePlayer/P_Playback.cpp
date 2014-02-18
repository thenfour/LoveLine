

#include "stdafx.h"
#include "P_Playback.h"
#include "P_Features.h"
#include "..\\LoveLineUtil\\LLUtils.h"
#include "resource.h"


#define TIMER_SLIDER    0x00000001
#define TIMER_INTERVAL  333


Plugin_Playback::Plugin_Playback()
{
    this->pPlugins = NULL;
    this->pCore = NULL;
    this->bUseNavSeek = TRUE;
    this->bIsDragging = FALSE;
}

static const DWORD HotKeySeekAmountMS = 5000;


Plugin_Playback::~Plugin_Playback()
{
}


enum HotKeyID
{
	HotKeyID_Play = 1000,
	HotKeyID_Stop = 1001,
	HotKeyID_Back = 1002,
	HotKeyID_Forward = 1003
};

LPARAM CALLBACK Plugin_Playback::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Plugin_Playback* pThis = (Plugin_Playback*)GetWindowLongPtr(hWnd, GWL_USERDATA);

    switch(uMsg)
    {
    case WM_TIMER:
        {
            if(wParam == TIMER_SLIDER)
            {
                // This is the slider timer.  Update the slider position
                // and the LCD display.

                // determine if we have hit a commercial or not.
                DWORD dwMS;
                int iComm = 0;
                SLIDER_COMMERCIAL c;

                pThis->pCore->Player.GetPos(&dwMS, &pThis->pCore->Log);
                iComm = pThis->cSlider.HitTest(dwMS);

                if(iComm != -1)
                {
                    pThis->cSlider.GetCommercial(iComm, &c);

                    // Seek to the dwOut param + 1 (so we're OUT of the commercial)
                    pThis->bUseNavSeek = FALSE;
                    pThis->pCore->SeekCurrentEpisode(max(c.dwIn, c.dwOut) + 1);
                    return 0;
                }

                pThis->UpdateSliderPositionAndLCD();
                return 0;
            }
            break;
        }
    case WM_NOTIFY:
        {
            NMHDR* pnmh = (NMHDR*)lParam;

            // Check for volume notifications
            if(pnmh->hwndFrom == pThis->cVol)
            {
                if(pnmh->code == CCF_POSCHANGED)
                {
                    pThis->pCore->Player.SetVolume(pThis->cVol.GetPos(), &pThis->pCore->Log);
                    return 0;
                }
            }

            // Check for slider notifications
            if(pnmh->hwndFrom == pThis->cSlider)
            {
                switch(pnmh->code)
                {
                case LSN_DRAGMOVE:
                    {
                        // The user has specifically moved something
                        // to a new location.
                        LSN_DRAG_STRUCT* s = (LSN_DRAG_STRUCT*)pnmh;
                        pThis->bUseNavSeek = FALSE;
                        pThis->pCore->SeekCurrentEpisode(s->dwNewPosition);
                        return 0;
                    }
                case LSN_STARTDRAGGING:
                    {
                        // The user has started to drag a handle.
                        // When this happens, we want the timer to NOT
                        // update stuff...
						pThis->dwLastPos = ((LSN_DRAG_STRUCT*)pnmh)->dwNewPosition;
                        pThis->bIsDragging = TRUE;
                        pThis->StopTimer();
                        return 0;
                    }
                case LSN_ENDDRAGGING:
                    {
                        LSN_DRAG_STRUCT* s = (LSN_DRAG_STRUCT*)pnmh;

                        pThis->bIsDragging = FALSE;

                        /*
                            If the user was dragging something, the play position
                            is most likely going to be wrong.  So we'll set the
                            play position to the correct position now unless
                            they were dragging the cursor.
                        */
                        if(s->DragObject.dwType != LLS_CURSOR)
                        {
                            pThis->pCore->SeekCurrentEpisode(pThis->cSlider.GetPos());
                        }
                        else
                        {
                            GUID id;
                            pThis->pCore->GetCurrentEpisode(&id);
                            pThis->Nav.NavigateTo(pThis->dwLastPos, id);
                            pThis->Nav.NavigateTo(pThis->cSlider.GetPos(), id);
                        }

                        pThis->StartTimer();
                        return 0;
                    }
                case LSN_REMOVEBOOKMARK:
                    {
                        LSN_REMOVEBOOKMARK_STRUCT* p = (LSN_REMOVEBOOKMARK_STRUCT*)lParam;
                        if(SUCCEEDED(
                            pThis->pCore->db.RemoveLink(
                            p->Bookmark.id, &pThis->pCore->Log)))
                        {
                            LL_DBUPDATE_STRUCT s;
                            s.dwOperation = DBUPDATE_REMOVED;
                            s.dwType = DBUPDATE_LINK;
                            pThis->pCore->SendPluginMessage(LL_DBUPDATE, &s);
                        }
                        return 0;
                    }
                case LSN_REMOVECOMMERCIAL:
                    {
                        LSN_REMOVECOMMERCIAL_STRUCT* p = (LSN_REMOVECOMMERCIAL_STRUCT*)lParam;
                        if(SUCCEEDED(
                            pThis->pCore->db.RemoveCommercial(
                            p->Commercial.id, &pThis->pCore->Log)))
                        {
                            LL_DBUPDATE_STRUCT s;
                            s.dwOperation = DBUPDATE_REMOVED;
                            s.dwType = DBUPDATE_COMMERCIAL;
                            pThis->pCore->SendPluginMessage(LL_DBUPDATE, &s);
                        }
                        return 0;
                    }
                case LSN_CHANGECOMMERCIAL:
                    {
                        LSN_CHANGECOMMERCIAL_STRUCT* pc = (LSN_CHANGECOMMERCIAL_STRUCT*)lParam;
                        LLCommercial c;

                        if(FAILED(pThis->pCore->db.GetCommercial(pc->New.id, &c, &pThis->pCore->Log)))
                        {
                            pThis->pCore->Log.AddEntry(L"The commercial that was modified wasn't found in the database - this is an application bug.");
                            return 0;
                        }

                        c.dwIn = pc->New.dwIn;
                        c.dwOut = pc->New.dwOut;

                        if(FAILED(pThis->pCore->db.SetCommercial(&c, &pThis->pCore->Log)))
                        {
                            pThis->pCore->Log.AddEntry(L"There was an error updating the commercial that the user modified.");
                            return 0;
                        }

                        LL_DBUPDATE_STRUCT s;
                        s.dwOperation = DBUPDATE_CHANGED;
                        s.dwType = DBUPDATE_COMMERCIAL;
                        pThis->pCore->SendPluginMessage(LL_DBUPDATE, &s);

                        return 0;
                    }
                case LSN_CHANGEBOOKMARK:
                    {
                        LSN_CHANGEBOOKMARK_STRUCT* pb = (LSN_CHANGEBOOKMARK_STRUCT*)lParam;
                        LLLink l;

                        // Update the bookmark in the database.
                        if(FAILED(pThis->pCore->db.GetLink(pb->New.id, &l, &pThis->pCore->Log)))
                        {
                            pThis->pCore->Log.AddEntry(L"The link that was modified wasn't found in the database - this is an application bug.");
                            return 0;
                        }

                        l.dwPos = pb->New.dwPos;

                        if(FAILED(pThis->pCore->db.SetLink(&l, &pThis->pCore->Log)))
                        {
                            pThis->pCore->Log.AddEntry(L"There was an error updating the link that the user modified.");
                            return 0;
                        }

                        LL_DBUPDATE_STRUCT s;
                        s.dwOperation = DBUPDATE_CHANGED;
                        s.dwType = DBUPDATE_LINK;
                        pThis->pCore->SendPluginMessage(LL_DBUPDATE, &s);

                        return 0;
                    }
                }
            }
            break;
        }
    case WM_COMMAND:
        {
            UINT uId = (UINT)((DWORD)wParam & 0xFFFF);
            if(uId == pThis->cBack)
            {
                DWORD dw;
                GUID id;

                if(SUCCEEDED(pThis->Nav.NavigateBack(&dw, &id)))
                {
                    pThis->bUseNavSeek = FALSE;
                    pThis->pCore->SeekEpisode(id, dw);
                }

                return 0;
            }
            if(uId == pThis->cNewBookmark)
            {
              // new bookmark.
              pThis->pPlugins->pFeatures->DoNewNow();
              return 0;
            }
            if(uId == pThis->cForward)
            {
                DWORD dw;
                GUID id;

                if(SUCCEEDED(pThis->Nav.NavigateForward(&dw, &id)))
                {
                    pThis->bUseNavSeek = FALSE;
                    pThis->pCore->SeekEpisode(id, dw);
                }

                return 0;
            }
            if(uId == pThis->cAddCommercial)
            {
                // On Add Commercial.
                pThis->OnAddCommercial();
                return 0;
            }
            if(uId == pThis->cPlay)
            {
                // On play.
                pThis->pCore->Player.Play(&pThis->pCore->Log);
                pThis->pCore->SendPluginMessage(LL_PLAY, 0);
                return 0;
            }
            if(uId == pThis->cStop)
            {
                // On play.
                pThis->pCore->Player.Stop(&pThis->pCore->Log);
                pThis->pCore->SendPluginMessage(LL_STOP, 0);
                return 0;
            }

            break;
        }
    case WM_SIZE:
        {
            pThis->cBack.Size();
            pThis->cForward.Size();
            pThis->cNewBookmark.Size();
            pThis->cLCD.Size();
            pThis->cStaticDate.Size();
            pThis->cStaticGuests.Size();
            pThis->cStaticFilename.Size();
            pThis->cPlay.Size();
            pThis->cStop.Size();
            pThis->cVol.Size();
            pThis->cSlider.Size();
            pThis->cAddCommercial.Size();
            return 0;
        }
    case WM_CREATE:
        {
            CREATESTRUCT* pcs = (CREATESTRUCT*)lParam;
            pThis = (Plugin_Playback*)pcs->lpCreateParams;
            SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)pThis);

						RegisterHotKey(hWnd, HotKeyID_Play, MOD_ALT, VK_UP);
						RegisterHotKey(hWnd, HotKeyID_Stop, MOD_ALT, VK_DOWN);
						RegisterHotKey(hWnd, HotKeyID_Back, MOD_ALT, VK_LEFT);
						RegisterHotKey(hWnd, HotKeyID_Forward, MOD_ALT, VK_RIGHT);

            return 0;
        }
		case WM_DESTROY:
			{
				UnregisterHotKey(hWnd, HotKeyID_Play);
				UnregisterHotKey(hWnd, HotKeyID_Stop);
				UnregisterHotKey(hWnd, HotKeyID_Back);
				UnregisterHotKey(hWnd, HotKeyID_Forward);
				return 0;
			}
		case WM_HOTKEY:
			{
				switch(wParam)
				{
				case HotKeyID_Play:
          pThis->pCore->Player.Play(&pThis->pCore->Log);
          pThis->pCore->SendPluginMessage(LL_PLAY, 0);
          return 0;
				case HotKeyID_Stop:
          pThis->pCore->Player.Stop(&pThis->pCore->Log);
          pThis->pCore->SendPluginMessage(LL_STOP, 0);
          return 0;
				case HotKeyID_Back:
					{
						DWORD position = 0;
						if(SUCCEEDED(pThis->pCore->Player.GetPos(&position, &pThis->pCore->Log)))
						{
							if(position > HotKeySeekAmountMS)
							{
								pThis->pCore->SeekCurrentEpisode(position - HotKeySeekAmountMS);
							}
						}
						return 0;
					}
				case HotKeyID_Forward:
					{
						DWORD position = 0;
						if(SUCCEEDED(pThis->pCore->Player.GetPos(&position, &pThis->pCore->Log)))
						{
							DWORD newPosition = position + HotKeySeekAmountMS;
							if(newPosition < pThis->pCore->GetLength())
							{
								pThis->pCore->SeekCurrentEpisode(newPosition);
							}
						}
						return 0;
					}
				}
			}
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void CALLBACK Plugin_Playback::NotificationProc(PVOID pUser, DWORD dwMsg, PVOID pParams)
{
    Plugin_Playback* pThis = (Plugin_Playback*)pUser;

    switch(dwMsg)
    {
    case LL_COMPLETE:
        {
            LL_SEEKEPISODE_STRUCT* lse = (LL_SEEKEPISODE_STRUCT*)pParams;

            // Advance to the next episode automatically.
            GUID idNew;
            LLEpisode ThisEpisode;

            if(pThis->bIsDragging == FALSE)
            {
                if(FAILED(pThis->pCore->db.GetEpisode(
                    lse->guidEpisode, &ThisEpisode, &pThis->pCore->Log)))
                {
                    //
                    return;
                }

                if(FAILED(pThis->pCore->db.GetNextEpisode(
                    MAKELLTIME(ThisEpisode.dwYear, ThisEpisode.dwMonth, ThisEpisode.dwDay),
                    &idNew, &pThis->pCore->Log)))
                {
                    // Gracefully cancel....
                    return;
                }

                // Seek to that episode.
                pThis->pCore->SeekEpisode(idNew, 0);
            }

            break;
        }
    case LL_SEEKEPISODE:
        {
            LL_SEEKEPISODE_STRUCT* lse = (LL_SEEKEPISODE_STRUCT*)pParams;
            pThis->Refresh();
            break;
        }
    case LL_SEEKCURRENT:
        {
            LL_SEEKCURRENT_STRUCT* lsc = (LL_SEEKCURRENT_STRUCT*)pParams;
			GUID id;
			pThis->pCore->GetCurrentEpisode(&id);
			pThis->UpdateNav(lsc->dwMS, id);
            pThis->NotifySeekCurrent();
            break;
        }
    case LL_SETFOCUS:
        {
            break;
        }
    case LL_INITIALIZE:
        {
            // Grab the last saved position and travel there..
            GUID guid;
            DWORD dwPos;

            if(FAILED(pThis->pCore->db.GetLastPosition(
                &guid, &dwPos, &pThis->pCore->Log)))
            {
                // Use defaults.
                dwPos = 0;
                ZeroMemory(&guid, sizeof(GUID));
            }

            // Load settings and set them up.
            CCString s;

            if(SUCCEEDED(
                pThis->pCore->db.GetSetting(
                L"Playback_IsPlaying", &s, &pThis->pCore->Log)))
            {
                if(s.ToInt(10) == TRUE) pThis->pCore->Player.Play(&pThis->pCore->Log);
            }

            if(SUCCEEDED(
                pThis->pCore->db.GetSetting(
                L"Playback_Volume", &s, &pThis->pCore->Log)))
            {
                pThis->pCore->Player.SetVolume(s.ToInt(10), &pThis->pCore->Log);
                pThis->cVol.SetPos(s.ToInt(10));
            }

            pThis->pCore->SeekEpisode(guid, dwPos);
            break;
        }
    case LL_DBUPDATE:
        {
            // The database has changed.  Pretty much we have to 
            // force the display to be updated.
            pThis->Refresh();
            break;
        }
    case LL_ATTACH:
        {
            LL_ATTACH_STRUCT* pas = (LL_ATTACH_STRUCT*)pParams;
            StandardPlugins* psp = (StandardPlugins*)pas->pUserData;
            pThis = (Plugin_Playback*)psp->pPlayback;

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


void Plugin_Playback::UpdateNav(DWORD dwMS, GUID id)
{
    // There are times when we don't register this with the navigator.
    // - When the seek was brought on by a FORWARD or BACK command
    // - When the seek was brought on by a commercial skip
    // - When the seek is caused by the user dragging the mouse.
    if(this->bUseNavSeek == TRUE)
    {
        this->pCore->GetCurrentEpisode(&id);
		this->pCore->Player.GetPos(&dwMS, &this->pCore->Log);
        this->Nav.NavigateTo(dwMS, id);
    }
    else
    {
        this->bUseNavSeek = TRUE;
    }
}


HRESULT Plugin_Playback::Create()
{
    HINSTANCE hInstance;
    RECT orect;
    RECT prect;

    hInstance = GetModuleHandle(NULL);

    SetRect(&prect, 0, 0, 100, 100);
    SetRect(&orect, 0, 0, 0, 0);

    this->cMainWindow.Create(L"Plugin_Playback",
        Plugin_Playback::WndProc, hInstance, prect, orect, this);
#define VOL_X 40
    // Make the static boxes
    SetRect(&prect, 0, 0, 100, 0);
    SetRect(&orect, 15, 15, -100 - VOL_X, 55);
    this->cStaticDate.Create(L"(no episode)", this->cMainWindow, hInstance, prect, orect);
    this->cStaticDate.SetFont(L"Arial", 18);

    // The LCD thing...
    SetRect(&prect, 100, 0, 100, 0);
    SetRect(&orect, -100 - VOL_X, 15, -VOL_X, 55);
    this->cLCD.Create(L"--:--:--", this->cMainWindow, hInstance, prect, orect);
    this->cLCD.SetFont(L"Courier", 18);
    this->sLastLCDText = L"--:--:--";

    // Guests static box
    SetRect(&prect, 0, 0, 100, 0);
    SetRect(&orect, 15, 55, -VOL_X, 70);
    this->cStaticGuests.Create(L"(no episode)", this->cMainWindow, hInstance, prect, orect);
    this->cStaticGuests.SetFont(L"Verdana", 12);

    // Filename static box
    SetRect(&prect, 0, 0, 100, 0);
    SetRect(&orect, 15, 70, -VOL_X, 85);
    this->cStaticFilename.Create(L".eoeukai", this->cMainWindow, hInstance, prect, orect);
    this->cStaticFilename.SetFont(L"Verdana", 12);

#define BUTTON_X 54
#define BUTTON_Y 54
#define SETORECT(n) \
    { \
        SetRect(&orect, \
            15 + (BUTTON_X * ((n)-1)),  \
            85,  \
            15 + (BUTTON_X * (n)),  \
            85 + BUTTON_Y); \
    }

    // Play, Stop
    SetRect(&prect, 0, 0, 0, 0);
    SETORECT(1);
    this->cStop.Create(L"Stop", this->cMainWindow, hInstance, prect, orect);
    this->cStop.SetBitmap(hInstance, IDB_STOP);

    SetRect(&prect, 0, 0, 0, 0);
    SETORECT(2);
    this->cPlay.Create(L"Play", this->cMainWindow, hInstance, prect, orect);
    this->cPlay.SetBitmap(hInstance, IDB_PLAY);

    // Add commercial
    SetRect(&prect, 0, 0, 0, 0);
    SETORECT(3);
    this->cAddCommercial.Create(L"Add Commercial", this->cMainWindow, hInstance, prect, orect);
    this->cAddCommercial.SetBitmap(hInstance, IDB_COMMERCIAL);

    // Forward, Back
    SetRect(&prect, 0, 0, 0, 0);
    SETORECT(4);
    this->cBack.Create(L"Back", this->cMainWindow, hInstance, prect, orect);
    this->cBack.SetBitmap(hInstance, IDB_BACK);

    SetRect(&prect, 0, 0, 0, 0);
    SETORECT(5);
    this->cForward.Create(L"Forward", this->cMainWindow, hInstance, prect, orect);
    this->cForward.SetBitmap(hInstance, IDB_FORWARD);

    SetRect(&prect, 0, 0, 0, 0);
    SETORECT(7);
    this->cNewBookmark.Create(L"New\r\nbookmark\r\nhere", this->cMainWindow, hInstance, prect, orect);

    // Volume fader
    SetRect(&prect, 100, 0, 100, 50);
    SetRect(&orect, -VOL_X, 0, 0, 0);
    this->cVol.Create(this->cMainWindow, hInstance, prect, orect);
    this->cVol.SetBounds(0, 10000);
    this->cVol.SetPos(10000);

    // The slider
    SetRect(&prect, 0, 50, 100, 100);
    SetRect(&orect, 0, 0, 0, 0);
    this->cSlider.Create(this->cMainWindow, hInstance, prect, orect);

    // The navigation system.
    this->Nav.SetButtons(&this->cBack, &this->cForward);
    this->bUseNavSeek = TRUE;

    return S_OK;
}


HRESULT Plugin_Playback::Destroy()
{
    // Save settings.
    CCString s;
    DWORD dw;

    s.FromInt(this->pCore->Player.IsPlaying(), 10);
    this->pCore->db.SetSetting(L"Playback_IsPlaying", s, &this->pCore->Log);

    this->pCore->Player.GetVolume(&dw, &this->pCore->Log);
    s.FromInt(dw, 10);
    this->pCore->db.SetSetting(L"Playback_Volume", s, &this->pCore->Log);

    this->cVol.Destroy();
    this->cStaticDate.Destroy();
    this->cStaticFilename.Destroy();
    this->cStaticGuests.Destroy();
    this->cNewBookmark.Destroy();
    this->cPlay.Destroy();
    this->cStop.Destroy();
    this->cSlider.Destroy();
    this->cBack.Destroy();
    this->cForward.Destroy();
    this->cLCD.Destroy();
    this->cAddCommercial.Destroy();

    return S_OK;
}


HRESULT Plugin_Playback::Refresh()
{
    GUID idEpisode;
    CCString sDate;
    CCString sTime;
    DWORD dwLen;
    LLEpisode e;

    this->pCore->GetCurrentEpisode(&idEpisode);

    // Stop our timer so the slider doesn't update anymore.
    this->StopTimer();

    // We have to clear our slider and fill it in with stuff.
    this->cSlider.ClearBookmarks();
    this->cSlider.ClearCommercials();

    if(this->pCore->IsCurrentEpisodeValid() == FALSE)
    {
        // The episode is invalid - let's load up the defaults.
        this->cLCD.SetText(L"--:--:--");
        this->cSlider.SetLength(0);
        this->cStaticDate.SetText(L"(No episode)");
        this->cStaticGuests.SetText(L"n/a");
        return S_OK;
    }

    this->pCore->db.GetEpisode(idEpisode, &e, &this->pCore->Log);

    // Format the date and stick it in teh static box.
    LLFormatDate(&e, &sDate);

    this->cStaticDate.SetText(sDate);
    this->cStaticGuests.SetText(e.Guests);
    this->cStaticFilename.SetText(e.FileName);

    // Set our slider and LCD displays.
    if(this->bIsDragging == FALSE)
    {
        pCore->Player.GetLength(&dwLen, &this->pCore->Log);
        this->cSlider.SetLength(dwLen);
        pCore->Player.GetPos(&dwLen, &this->pCore->Log);
        this->cSlider.SetPos(dwLen);

        LLFormatMilliseconds(dwLen, &sTime);
        this->cLCD.SetText(sTime);
        // Start our timer back up again.
        this->StartTimer();
    }

    // Now fill the slider with the bookmarks and commercials.
    if(FAILED(this->pCore->db.EnumCommercials(
        idEpisode, Plugin_Playback::EnumCommercials, this, &this->pCore->Log)))
    {
        this->pCore->Log.AddEntry(L"Could not get commercial objects from the database.");
    }

    if(FAILED(this->pCore->db.EnumFeatures(
        idEpisode, Plugin_Playback::EnumBookmarks, this, &this->pCore->Log)))
    {
        this->pCore->Log.AddEntry(L"Could not get feature objects from the database.");
    }

    return S_OK;
}


/*
    Stops the slider timer.  This timer tells the slider to update itself.
*/
void Plugin_Playback::StopTimer()
{
    KillTimer(this->cMainWindow, TIMER_SLIDER);
}


/*
    Starts the slider timer.  When the slider timer is started, this
    plugin's window proc will update the slider every second.
*/
void Plugin_Playback::StartTimer()
{
    // Make sure it's not started already...
    this->StopTimer();

    SetTimer(this->cMainWindow, TIMER_SLIDER, TIMER_INTERVAL, NULL);
}


void Plugin_Playback::NotifySeekCurrent()
{
    // Once again, update our controls.
    this->UpdateSliderPositionAndLCD();
}


void Plugin_Playback::UpdateSliderPositionAndLCD()
{
    DWORD dwMS;
    CCString sTime;

    if(this->bIsDragging == FALSE)
    {
        this->pCore->Player.GetPos(&dwMS, &this->pCore->Log);

        this->cSlider.SetPos(dwMS);
        LLFormatMilliseconds(dwMS, &sTime);
        if(sTime.cmp(this->sLastLCDText) != 0)
        {
            this->sLastLCDText = sTime;
            this->cLCD.SetText(sTime);
        }
    }
}


void Plugin_Playback::OnAddCommercial()
{
    LLCommercial c;

    this->pCore->Player.GetPos(&c.dwIn, &this->pCore->Log);

    c.dwOut = c.dwIn + GetCommercialLength(&this->pCore->db, c.dwIn, &this->pCore->Log);

    this->pCore->GetCurrentEpisode(&c.guidEpisode);
    if(FAILED(this->pCore->db.AddCommercial(&c, &this->pCore->Log)))
    {
        return;
    }

    LL_DBUPDATE_STRUCT ms;
    ms.dwOperation = DBUPDATE_NEW;
    ms.dwType = DBUPDATE_COMMERCIAL;
    this->pCore->SendPluginMessage(LL_DBUPDATE, &ms);

    this->cSlider.AddCommercial(c.dwIn, c.dwOut, 0, c.guid);
}


HRESULT CALLBACK Plugin_Playback::EnumCommercials(LLCommercial* pc, PVOID pUser)
{
    Plugin_Playback* pThis = (Plugin_Playback*)pUser;

    // Add a new commercial to the database.
    pThis->cSlider.AddCommercial(pc->dwIn, pc->dwOut, 0, pc->guid);

    return S_OK;
}


HRESULT CALLBACK Plugin_Playback::EnumBookmarks(LLLink* pl, LLFeature* pf, PVOID pUser)
{
    Plugin_Playback* pThis = (Plugin_Playback*)pUser;

    // Add a new bookmark to the database.
    pThis->cSlider.AddBookmark(pl->dwPos, pf->Title, 0, pl->guid);

    return S_OK;
}





