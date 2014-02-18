

#include "LLApp.h"
#include <shlwapi.h>
#include <commctrl.h>

#define DEFEXT L"mp3"

#include "..\\LoveLineUtil\\global.h"
#include "..\\LoveLineUtil\\CCString.h"


#include "resource.h"


#define ID_TIMER_RECORD_TOTAL           0x00000001
#define ID_TIMER_RECORD_PROGRESS        0x00000002
#define ID_TIMER_ENCODE_TOTAL           WM_APP + 1
#define ID_TIMER_ENCODE_PROGRESS        WM_APP + 2


#define ENCODE_BUFFER_SIZE 5// In seconds


#define MIN_LENGTH     2
#define MAX_LENGTH     4233600      // 49 days
#define MIN_SAMPLERATE 1000
#define MAX_SAMPLERATE 131072      // 128k
#define MIN_BITRATE    1000
#define MAX_BITRATE    104857600   // 100 Mbps


void FormatSeconds(CCString* ps, DWORD dw)
{
    // HHH:MM:SS
    DWORD dwHrs;
    DWORD dwMin;

    // 2 divides, 2 multiplies, 2 minuses
    dwHrs = dw / 3600;
    dw -= dwHrs * 3600;
    dwMin = dw / 60;
    dw -= dwMin * 60;

    ps->FormatW(L"%d:%02d:%02d", dwHrs, dwMin, dw);
}


/*
    Selects a string in a combo box (for use with drop-down lists)
*/
HRESULT SelectString(HWND hDlg, UINT uId, PCWSTR wsz)
{
    HWND hCombo = GetDlgItem(hDlg, uId);
    int nItem;

    // Get the ID of the string.
    nItem = SendMessage(hCombo, CB_SELECTSTRING, (WPARAM)-1, (LPARAM)wsz);
    if(nItem == CB_ERR)
    {
        return E_FAIL;
    }

    // Select the ID.
    SendMessage(hCombo, CB_SETCURSEL, (WPARAM)nItem, (LPARAM)0);
    
    return S_OK;
}

HFONT __CreateFont(PCWSTR wszFontName, int nSize, BOOL bBold)
{
    HFONT hFont;

    NONCLIENTMETRICS ncm = {0};
    ncm.cbSize = sizeof(ncm);
    SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);

    LOGFONT TitleLogFont = ncm.lfMessageFont;
    TitleLogFont.lfWeight = bBold == TRUE ? FW_BOLD : 0;
    lstrcpy(TitleLogFont.lfFaceName, wszFontName);

    HDC hdc = GetDC(NULL); //gets the screen DC
    TitleLogFont.lfHeight = 0 - GetDeviceCaps(hdc, LOGPIXELSY) * nSize / 72;
    hFont = CreateFontIndirect(&TitleLogFont);
    ReleaseDC(NULL, hdc);

    return hFont;
}


LLMainInitVars::LLMainInitVars()
{
    this->FileName = L"";
    this->bStartRecording = FALSE;
    this->nSeconds = 7800;
    this->nBitDepth = 16;
    this->nBitRate = 28800;
    this->nSampleRate = 22050;
    this->nChannels = 1;
    this->bDeleteFile = FALSE;
    this->bExitAutomatically = FALSE;
    this->hDlg = NULL;
    this->hCourier = __CreateFont(L"Courier", 10, FALSE);
    this->hVerdana = __CreateFont(L"Verdana Bold", 12, TRUE);
    this->hTitle = __CreateFont(L"Arial", 18, TRUE);
    this->hEncodingThread = NULL;
    this->hEncodingCancel = CreateEvent(NULL, FALSE, FALSE, NULL);

    this->bIsRecording = FALSE;
}


LLMainInitVars::~LLMainInitVars()
{
    if(this->hVerdana) DeleteObject(this->hVerdana);
    if(this->hCourier) DeleteObject(this->hCourier);
}


/*
    BitRate:
        8, 16, or 24.

    SampleRate:
        1000 < x < 100000

    Channels
        1 or 2

    Seconds
        Greater than 0.
*/


LLApp::LLApp()
{
    this->hInstance = 0;
}


LLApp::~LLApp()
{
}


/*
    Command line options:
    -F Filename           Filename to initialize recorder with (required with -R)
                          Default filename is the date and time YYYY-MM-DD(HH-MM-SS).wmv
    -R                    Start Recording automatically (default NO)
    -D                    Delete the intermediate files after encoding (default NO)
    -Q Bitrate            Bit rate for encoded output. (default 28800)
    -E                    Exit Automatically. (default NO)
    -L Seconds            How many seconds to record (default 7800)
    -B BitDepth           Bitdepth 8/16 (default 16)
    -S Sample Rate        Sample Rate (default 22050)
    -C Channels (1 or 2)  Channels (default 1)
    -?                    Display help
*/
int LLApp::Entry(HINSTANCE hInstance, PCWSTR wszCmdLine)
{
    PWSTR*  argv = NULL;
    int     argc = 0;
    int     iArg = 0;
    int     n;
    int     nPage = 0;
    PCWSTR  wszArg = NULL;
    BOOL    bExit = FALSE;// Exit the program before initialization?  Used with the -? option.

    WCHAR   wszThisModule[MAX_PATH];

    LLMainInitVars Vars;
    this->hInstance = hInstance;
    Vars.pThis = this;

    // Parse the command line

    argv = CommandLineToArgvW(wszCmdLine, &argc);
    if(argv == NULL)
    {
        return 1;
    }

    while(1)
    {
        // Look at the current arg.
        wszArg = argv[iArg];

        if(wcslen(wszArg) == 2)
        {
            if(wszArg[0] == '/' || wszArg[0] == '-')
            {
                // It's properly formed.
                switch(wszArg[1])
                {
                    case 'F':
                    case 'f':
                        iArg++;
                        Vars.FileName.cpy(argv[iArg]);
                        break;
                    case 'R':
                    case 'r':
                        Vars.bStartRecording = TRUE;
                        break;
                    case 'E':
                    case 'e':
                        Vars.bExitAutomatically = TRUE;
                        break;
                    case 'L':
                    case 'l':
                        iArg++;
                        n = wcstol(argv[iArg], 0, 10);
                        if(n >= MIN_LENGTH && n <= MAX_LENGTH) Vars.nSeconds = n;
                        break;
                    case 'B':
                    case 'b':
                        iArg++;
                        n = wcstol(argv[iArg], 0, 10);
                        if(n == 8 || n == 16 || n == 24) Vars.nBitDepth = n;
                        break;
                    case 'Q':
                    case 'q':
                        iArg++;
                        n = wcstol(argv[iArg], 0, 10);
                        if(n >= MIN_BITRATE && n <= MAX_BITRATE) Vars.nBitRate = n;
                        break;
                    case 'S':
                    case 's':
                        iArg++;
                        n = wcstol(argv[iArg], 0, 10);
                        if(n >= MIN_SAMPLERATE && n <= MAX_SAMPLERATE) Vars.nSampleRate = n;
                        break;
                    case 'D':
                    case 'd':
                        Vars.bDeleteFile = TRUE;
                        break;
                    case 'C':
                    case 'c':
                        iArg++;
                        n = wcstol(argv[iArg], 0, 10);
                        if(n > 0 && n < 3) Vars.nChannels = n;
                        break;
                    case '?':
                        MessageBox(0,
                            L"Command line options:\n"
                            L"-F Filename           Filename to initialize recorder with (required with -R)\n"
                            L"                      Default filename is the date and time YYYY-MM-DD(HH-MM-SS).wmv\n"
                            L"-R                    Start Recording automatically (default NO)\n"
                            L"-D                    Delete the intermediate files after encoding (default NO)\n"
                            L"-E                    Exit automatically after recording (default NO)\n"
                            L"-L Seconds            How many seconds to record (default 7800)\n"
                            L"-S Sample Rate        Sample Rate (default 22050)\n"
                            L"-C Channels (1 or 2)  Channels (default 1)\n"
                            L"-B BitDepth           Bitdepth 8/16 (default 16)\n"
                            L"-Q Bitrate            Bit rate for encoded output. (default 28800)\n"
                            L"-?                    Display help\n",
                            L"LoveLine Recorder",
                            MB_OK);

                        bExit = TRUE;
                        break;
                }
            }
        }

        iArg ++;
        if(iArg == argc) break;
    }

    GlobalFree(argv);

    if(bExit == TRUE) return 0;

    // Grab the default filename if it's not specified already.
    SYSTEMTIME st;
    WCHAR wszTime[MAX_PATH];

    GetLocalTime(&st);
    GetModuleFileName(hInstance, wszThisModule, MAX_PATH);

    swprintf(wszTime, L"..\\Media\\%04d-%02d-%02d(%02d-%02d-%02d)." DEFEXT,
        st.wYear, st.wMonth, st.wDay,
        st.wHour, st.wMinute, st.wSecond);

    PathRemoveFileSpec(wszThisModule);
    PathAppend(wszThisModule, wszTime);
    Vars.FileName = wszThisModule;

    PROPSHEETHEADER psh;
    PROPSHEETPAGE psp;
    HPROPSHEETPAGE hPages[10];
    PROPSHEETPAGE* ppsp = NULL;

    /*
        Welcome page
    */
    psp.dwSize =        sizeof(psp);
    psp.dwFlags =       PSP_DEFAULT | PSP_HIDEHEADER;
    psp.hInstance =     hInstance;
    psp.lParam =        (LPARAM) &Vars; //The shared data structure
    psp.pfnDlgProc =    LLApp::WelcomeProc;
    psp.pszTemplate =   MAKEINTRESOURCE(IDD_WELCOME);

    hPages[nPage++] =         CreatePropertySheetPage(&psp);

    /*
        FileName page.
    */
	psp.dwFlags =			PSP_DEFAULT | PSP_USEHEADERTITLE | PSP_USEHEADERSUBTITLE;
	psp.pszHeaderTitle =	L"Recording Options";
	psp.pszHeaderSubTitle =	L"Please specify these recording options.";
	psp.pszTemplate =		MAKEINTRESOURCE(IDD_FILENAME);
    psp.pfnDlgProc =		LLApp::FileProc;

	hPages[nPage++] =				CreatePropertySheetPage(&psp);

    /*
        Options page.
    */
	psp.dwFlags =			PSP_DEFAULT | PSP_USEHEADERTITLE | PSP_USEHEADERSUBTITLE;
	psp.pszHeaderTitle =	L"Recording Options";
	psp.pszHeaderSubTitle =	L"Please specify these recording options.";
	psp.pszTemplate =		MAKEINTRESOURCE(IDD_OPTIONS);
    psp.pfnDlgProc =		LLApp::OptionsProc;

	hPages[nPage++] =				CreatePropertySheetPage(&psp);

    /*
        Bit Rate page.
    */
	psp.dwFlags =			PSP_DEFAULT | PSP_USEHEADERTITLE | PSP_USEHEADERSUBTITLE;
	psp.pszHeaderTitle =	L"Encoding Options";
	psp.pszHeaderSubTitle =	L"Please specify these encoding options.";
	psp.pszTemplate =		MAKEINTRESOURCE(IDD_BITRATE);
    psp.pfnDlgProc =		LLApp::BitRateProc;

	hPages[nPage++] =				CreatePropertySheetPage(&psp);

    /*
        Ready page.
    */
	psp.dwFlags =			PSP_DEFAULT | PSP_USEHEADERTITLE | PSP_USEHEADERSUBTITLE;
	psp.pszHeaderTitle =	L"Ready to record";
	psp.pszHeaderSubTitle =	L"";
	psp.pszTemplate =		MAKEINTRESOURCE(IDD_READY);
    psp.pfnDlgProc =		LLApp::ReadyProc;

	hPages[nPage++] =				CreatePropertySheetPage(&psp);

    /*
        Record page.
    */
	psp.dwFlags =			PSP_DEFAULT | PSP_USEHEADERTITLE | PSP_USEHEADERSUBTITLE;
	psp.pszHeaderTitle =	L"Recording";
	psp.pszHeaderSubTitle =	L"";
	psp.pszTemplate =		MAKEINTRESOURCE(IDD_RECORD);
    psp.pfnDlgProc =		LLApp::RecordProc;

	hPages[nPage++] =				CreatePropertySheetPage(&psp);

    /*
        Encode page.
    */
	psp.dwFlags =			PSP_DEFAULT | PSP_USEHEADERTITLE | PSP_USEHEADERSUBTITLE;
	psp.pszHeaderTitle =	L"Encoding";
	psp.pszHeaderSubTitle =	L"";
	psp.pszTemplate =		MAKEINTRESOURCE(IDD_ENCODE);
    psp.pfnDlgProc =		LLApp::EncodeProc;

	hPages[nPage++] =				CreatePropertySheetPage(&psp);

    /*
        Completed page
    */
    psp.dwSize =        sizeof(psp);
    psp.dwFlags =       PSP_DEFAULT | PSP_HIDEHEADER;
    psp.pfnDlgProc =    LLApp::CompletedProc;
    psp.pszTemplate =   MAKEINTRESOURCE(IDD_COMPLETED);

    hPages[nPage++] =         CreatePropertySheetPage(&psp);

    /*
        Display the stuff...
    */
    psh.dwSize =         sizeof(psh);
    psh.hInstance =      this->hInstance;
    psh.hwndParent =     NULL;
    psh.phpage =         hPages;
    psh.dwFlags =        PSH_WIZARD97 | PSH_WATERMARK | PSH_HEADER;
    psh.pszbmWatermark = MAKEINTRESOURCE(IDB_WATERMARK);
    psh.pszbmHeader =    MAKEINTRESOURCE(IDB_HEADER);
    psh.nStartPage =     0;
    psh.nPages =         nPage;

    PropertySheet(&psh);

    return 0;
}


/*
    IDC_TITLE
*/
INT_PTR CALLBACK LLApp::WelcomeProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LLMainInitVars* pVars = (LLMainInitVars*)GetWindowLongPtr(hWnd, GWL_USERDATA);

	switch (uMsg)
	{
	case WM_INITDIALOG:
		{
            pVars = (LLMainInitVars*)((LPPROPSHEETPAGE)lParam)->lParam;
			SetWindowLong(hWnd, GWL_USERDATA, (DWORD_PTR)pVars);

			HWND hControl = GetDlgItem(hWnd, IDC_TITLE);
            SendMessage(hControl, WM_SETFONT, (WPARAM)pVars->hVerdana, (LPARAM)TRUE);

            // Stick a minimize box on the title bar.
            HWND hPropSheet = GetParent(hWnd);
            DWORD dwStyle = GetWindowLong(hPropSheet, GWL_STYLE);
            dwStyle |= WS_MINIMIZEBOX;
            SetWindowLong(hPropSheet, GWL_STYLE, dwStyle);

			break;
		}
	case WM_NOTIFY:
		{
		    LPNMHDR lpnm = (LPNMHDR) lParam;

		    switch (lpnm->code)
		    {
            case PSN_KILLACTIVE:
                break;
		    case PSN_SETACTIVE: //Enable the Next button	 
		    	PropSheet_SetWizButtons(GetParent(hWnd), PSWIZB_NEXT);

                // If we are to automatically start recording, head immediately to the
                // record page.
                if(pVars->bStartRecording == TRUE)
                {
                    SetWindowLong(hWnd, DWL_MSGRESULT, IDD_RECORD);
                }

                return TRUE;
		    case PSN_WIZNEXT:
		    	//Handle a Next button click here
		    	break;
		    }

            break;
		}
	}
	return 0;
}


/*
    IDC_FILENAME
    IDC_DELETE
    IDC_LENGTH
    IDC_AUTOEXIT

    IDC_BROWSE
*/
INT_PTR CALLBACK LLApp::FileProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LLMainInitVars* pVars = (LLMainInitVars*)GetWindowLongPtr(hWnd, GWL_USERDATA);

	switch (uMsg)
	{
    case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
            case IDC_BROWSE:
                {
                    CCString FileName;
                    OPENFILENAME of;

                    ZeroMemory(&of, sizeof(of));
                    of.lStructSize = sizeof(of);
                    of.hInstance = pVars->pThis->hInstance;
                    of.hwndOwner = hWnd;
                    of.lpstrFilter = 
                        L"Windows Media(r) Audio Files (*.wma)\0"
                        L"*.wma\0"
                        L"MP3 Files (*.mp3)\0"
                        L"*.mp3\0"
                        L"All Files (*.*)\0"
                        L"*.*\0\0";
                    of.nFilterIndex = 0;
                    of.lpstrFile = FileName.GetBuffer(MAX_PATH);
                    of.nMaxFile = MAX_PATH;
                    of.Flags = OFN_ENABLESIZING;
                    of.lpstrDefExt = DEFEXT;

                    if(GetSaveFileName(&of) == 0)
                    {
                        //The command either failed, or they hit cancel.
                        return TRUE;
                    }

                    pVars->FileName = FileName;
                    SetDlgItemText(hWnd, IDC_FILENAME, (PCWSTR)pVars->FileName);
                    return TRUE;
                }
            }
            break;
        }
	case WM_INITDIALOG:
		{ 
            pVars = (LLMainInitVars*)((LPPROPSHEETPAGE)lParam)->lParam;
			SetWindowLong(hWnd, GWL_USERDATA, (DWORD_PTR)pVars);
			
            SetDlgItemText(hWnd, IDC_FILENAME, (PCWSTR)pVars->FileName);
            SetDlgItemInt(hWnd, IDC_LENGTH, pVars->nSeconds, TRUE);
            CheckDlgButton(hWnd, IDC_AUTOEXIT, pVars->bExitAutomatically == TRUE ? BST_CHECKED : BST_UNCHECKED);
            CheckDlgButton(hWnd, IDC_DELETE, pVars->bDeleteFile == TRUE ? BST_CHECKED : BST_UNCHECKED);

			break;
		}
	case WM_NOTIFY:
		{
		    LPNMHDR lpnm = (LPNMHDR) lParam;

		    switch (lpnm->code)
		    {
            case PSN_KILLACTIVE:
                {
                    int n;
                    BOOL bSuccess;
                    HWND hFileName = GetDlgItem(hWnd, IDC_FILENAME);

                    /*
                        Grab the user input values, validate them and determine
                        whether we can go on with the next page or not.
                    */

                    // Length....
                    n = GetDlgItemInt(hWnd, IDC_LENGTH, &bSuccess, TRUE);
                    if(bSuccess == FALSE)
                    {
                        MessageBox(hWnd, L"Invalid input for recording length",
                            L"LoveLine Recorder", MB_OK | MB_ICONASTERISK);
                        SetWindowLong(hWnd, DWL_MSGRESULT, TRUE);
                        return TRUE;
                    }
                    if(n < MIN_LENGTH || n > MAX_LENGTH)
                    {
                        MessageBox(hWnd, L"Valid values for length are between 2 seconds and 49 days",
                            L"LoveLine Recorder", MB_OK | MB_ICONASTERISK);
                        SetWindowLong(hWnd, DWL_MSGRESULT, TRUE);
                        return TRUE;
                    }

                    pVars->nSeconds = n;

                    // Auto exit.
                    pVars->bExitAutomatically = IsDlgButtonChecked(hWnd, IDC_AUTOEXIT);

                    // Delete intermediate file automatically.
                    pVars->bDeleteFile = IsDlgButtonChecked(hWnd, IDC_DELETE);

                    // Filename
                    n = GetWindowTextLength(hFileName);
                    GetWindowText(hFileName, pVars->FileName.GetBuffer(n + 2), n + 2);

                    SetWindowLong(hWnd, DWL_MSGRESULT, FALSE);
                    return TRUE;
                }
		    case PSN_SETACTIVE: //Enable the Next button	 
		    	PropSheet_SetWizButtons(GetParent(hWnd), PSWIZB_NEXT);
		    	break;
		    case PSN_WIZNEXT:
		    	//Handle a Next button click here
		    	break;
		    }

            break;
		}
	}
	return 0;
}


/*
    IDC_OPTIONS
*/
INT_PTR CALLBACK LLApp::ReadyProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LLMainInitVars* pVars = (LLMainInitVars*)GetWindowLongPtr(hWnd, GWL_USERDATA);

	switch (uMsg)
	{
	case WM_INITDIALOG:
		{
            pVars = (LLMainInitVars*)((LPPROPSHEETPAGE)lParam)->lParam;
			SetWindowLong(hWnd, GWL_USERDATA, (DWORD_PTR)pVars);

            SendDlgItemMessage(hWnd, IDC_OPTIONS, WM_SETFONT, (WPARAM)pVars->hCourier, TRUE);

			break;
		}
	case WM_NOTIFY:
		{
		    LPNMHDR lpnm = (LPNMHDR) lParam;

		    switch (lpnm->code)
		    {
		    case PSN_SETACTIVE: //Enable the Next button
                {
		    	    PropSheet_SetWizButtons(GetParent(hWnd), PSWIZB_NEXT | PSWIZB_BACK);

                    // Format a string with the options.
                    CCString s;
                    CCString sExit;
                    CCString sChannels;
                    CCString sLength;

                    FormatSeconds(&sLength, pVars->nSeconds);

                    sChannels = pVars->nChannels == 1 ? L"Mono" : L"Stereo";
                    sExit = pVars->bExitAutomatically == TRUE ?
                        L"The recorder will exit automatically \r\n"
                        L"after recording has finished."
                        :
                        L"A summary will be displayed after \r\n"
                        L"recording has finished.";

                    float fkhz = (float)pVars->nSampleRate / 1000;
                    float fkbps = (float)pVars->nBitRate / 1000;
                    PWSTR wsz = s.GetBuffer(1024);

                    swprintf(wsz, 
                        L"RECORD OPTIONS\r\n"
                        L"    Sample rate    %.1f khz\r\n"
                        L"    Channels       %s (%d)\r\n"
                        L"    Bit depth      %d\r\n\r\n"
                        L"ENCODE OPTIONS\r\n"
                        L"    Bit rate       %d bps (%.0f kbps)\r\n"
                        L"    Name           %s\r\n"
                        L"    Length         %s (%d seconds)\r\n\r\n"
                        L"%s\r\n",
                        fkhz,
                        (PCWSTR)sChannels, pVars->nChannels,
                        pVars->nBitDepth,
                        pVars->nBitRate, fkbps,
                        (PCWSTR)pVars->FileName,
                        (PCWSTR)sLength, pVars->nSeconds,
                        (PCWSTR)sExit
                        );


                    SetDlgItemText(hWnd, IDC_OPTIONS, wsz);

                    /*
                        RECORD OPTIONS
                            Sample Rate  44.1 khz
                            Channels     Stereo (2)
                            Bit depth    16

                        ENCODE OPTIONS
                            Bit Rate  22050 bps (~22 kbps)
                            Name      c:\\adflasdf
                            Length    2h 5m

                        The recorder will exit
                        automatically when recording stops.
                    */

		    	    break;
                }
		    case PSN_WIZNEXT:
		    	//Handle a Next button click here
		    	break;
		    case PSN_RESET:
		    	//Handle a Cancel button click, if necessary
		    	break;
		    }

            break;
		}
	}
	return 0;
}


/*
    IDC_TITLE
*/
INT_PTR CALLBACK LLApp::CompletedProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LLMainInitVars* pVars = (LLMainInitVars*)GetWindowLongPtr(hWnd, GWL_USERDATA);

	switch (uMsg)
	{
	case WM_INITDIALOG:
		{ 
            pVars = (LLMainInitVars*)((LPPROPSHEETPAGE)lParam)->lParam;
			SetWindowLong(hWnd, GWL_USERDATA, (DWORD_PTR)pVars);
			
			HWND hControl = GetDlgItem(hWnd, IDC_TITLE);
            SendMessage(hControl, WM_SETFONT, (WPARAM)pVars->hVerdana, (LPARAM)TRUE);
			
            hControl = GetDlgItem(hWnd, IDC_LOG);
            SendMessage(hControl, WM_SETFONT, (WPARAM)pVars->hCourier, (LPARAM)TRUE);
            pVars->Log.SetWindow(hControl);
			break;
		}
	case WM_NOTIFY:
		{
		    LPNMHDR lpnm = (LPNMHDR) lParam;

		    switch (lpnm->code)
		    {
		    case PSN_SETACTIVE: //Enable the Next button	 
		    	PropSheet_SetWizButtons(GetParent(hWnd), PSWIZB_FINISH);
		    	break;
		    case PSN_WIZNEXT:
		    	//Handle a Next button click here
		    	break;
		    case PSN_WIZBACK:
		    	//Handle a Back button click here
		    	break;
		    case PSN_RESET:
		    	//Handle a Cancel button click, if necessary
		    	break;
		    }

            break;
		}
	}
	return 0;
}


/*
    IDC_SAMPLERATE
    IDC_BITDEPTH
    IDC_CHANNELS
*/
INT_PTR CALLBACK LLApp::OptionsProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LLMainInitVars* pVars = (LLMainInitVars*)GetWindowLongPtr(hWnd, GWL_USERDATA);

	switch (uMsg)
	{
	case WM_INITDIALOG:
		{
            pVars = (LLMainInitVars*)((LPPROPSHEETPAGE)lParam)->lParam;
			SetWindowLong(hWnd, GWL_USERDATA, (DWORD_PTR)pVars);

            /*
                Set up the sample rate box
                11025
                22050
                44100
                48000
            */
            CCString s;

            s.FromInt(11025, 10);
            SendMessage(GetDlgItem(hWnd, IDC_SAMPLERATE), CB_ADDSTRING, 0, (LPARAM)(PCWSTR)s);
            s.FromInt(22050, 10);
            SendMessage(GetDlgItem(hWnd, IDC_SAMPLERATE), CB_ADDSTRING, 0, (LPARAM)(PCWSTR)s);
            s.FromInt(44100, 10);
            SendMessage(GetDlgItem(hWnd, IDC_SAMPLERATE), CB_ADDSTRING, 0, (LPARAM)(PCWSTR)s);
            s.FromInt(48000, 10);
            SendMessage(GetDlgItem(hWnd, IDC_SAMPLERATE), CB_ADDSTRING, 0, (LPARAM)(PCWSTR)s);

            s.FromInt(pVars->nSampleRate, 10);
            SelectString(hWnd, IDC_SAMPLERATE, s);

            /*
                Set up the bit depth box.
            */
            s.FromInt(8, 10);
            SendMessage(GetDlgItem(hWnd, IDC_BITDEPTH), CB_ADDSTRING, 0, (LPARAM)(PCWSTR)s);
            s.FromInt(16, 10);
            SendMessage(GetDlgItem(hWnd, IDC_BITDEPTH), CB_ADDSTRING, 0, (LPARAM)(PCWSTR)s);
            s.FromInt(24, 10);
            SendMessage(GetDlgItem(hWnd, IDC_BITDEPTH), CB_ADDSTRING, 0, (LPARAM)(PCWSTR)s);

            s.FromInt(pVars->nBitDepth, 10);
            SelectString(hWnd, IDC_BITDEPTH, s);

            /*
                Set up the channels box.
            */
            s.FromInt(1, 10);
            SendMessage(GetDlgItem(hWnd, IDC_CHANNELS), CB_ADDSTRING, 0, (LPARAM)(PCWSTR)s);
            s.FromInt(2, 10);
            SendMessage(GetDlgItem(hWnd, IDC_CHANNELS), CB_ADDSTRING, 0, (LPARAM)(PCWSTR)s);

            s.FromInt(pVars->nChannels, 10);
            SelectString(hWnd, IDC_CHANNELS, s);

			break;
		}
	case WM_NOTIFY:
		{
		    LPNMHDR lpnm = (LPNMHDR) lParam;

		    switch (lpnm->code)
		    {
            case PSN_KILLACTIVE:
                {
                    int n;
                    BOOL bSuccess;

                    /*
                        Grab the user input values, validate them and determine
                        whether we can go on with the next page or not.
                    */

                    // Sample rate....
                    n = GetDlgItemInt(hWnd, IDC_SAMPLERATE, &bSuccess, TRUE);
                    if(bSuccess == FALSE)
                    {
                        MessageBox(hWnd, L"Invalid input for sample rate",
                            L"LoveLine Recorder", MB_OK | MB_ICONASTERISK);
                        SetWindowLong(hWnd, DWL_MSGRESULT, TRUE);
                        return TRUE;
                    }
                    if(n < MIN_SAMPLERATE || n > MAX_SAMPLERATE)
                    {
                        MessageBox(hWnd, L"The sample rate is out of range",
                            L"LoveLine Recorder", MB_OK | MB_ICONASTERISK);
                        SetWindowLong(hWnd, DWL_MSGRESULT, TRUE);
                        return TRUE;
                    }

                    pVars->nSampleRate = n;

                    // Bit depth....
                    n = GetDlgItemInt(hWnd, IDC_BITDEPTH, &bSuccess, TRUE);
                    if(bSuccess == FALSE)
                    {
                        MessageBox(hWnd, L"Invalid input for bit depth",
                            L"LoveLine Recorder", MB_OK | MB_ICONASTERISK);
                        SetWindowLong(hWnd, DWL_MSGRESULT, TRUE);
                        return TRUE;
                    }
                    if(n != 8 && n != 16 && n != 24)
                    {
                        MessageBox(hWnd, L"The bit depth invalid",
                            L"LoveLine Recorder", MB_OK | MB_ICONASTERISK);
                        SetWindowLong(hWnd, DWL_MSGRESULT, TRUE);
                        return TRUE;
                    }

                    pVars->nBitDepth = n;

                    // Channels....
                    n = GetDlgItemInt(hWnd, IDC_CHANNELS, &bSuccess, TRUE);
                    if(bSuccess == FALSE)
                    {
                        MessageBox(hWnd, L"Invalid input for sample rate",
                            L"LoveLine Recorder", MB_OK | MB_ICONASTERISK);
                        SetWindowLong(hWnd, DWL_MSGRESULT, TRUE);
                        return TRUE;
                    }
                    if(n != 1 && n != 2)
                    {
                        MessageBox(hWnd, L"Channels must be set to either 1 or 2",
                            L"LoveLine Recorder", MB_OK | MB_ICONASTERISK);
                        SetWindowLong(hWnd, DWL_MSGRESULT, TRUE);
                        return TRUE;
                    }

                    pVars->nChannels = n;

                    SetWindowLong(hWnd, DWL_MSGRESULT, FALSE);
                    return TRUE;
                }
		    case PSN_SETACTIVE: //Enable the Next button	 
		    	PropSheet_SetWizButtons(GetParent(hWnd), PSWIZB_BACK | PSWIZB_NEXT);
		    	break;
		    case PSN_WIZNEXT:
		    	//Handle a Next button click here
		    	break;
		    case PSN_WIZBACK:
		    	//Handle a Back button click here
		    	break;
		    case PSN_RESET:
		    	//Handle a Cancel button click, if necessary
		    	break;
		    }

            break;
		}
	}

	return 0;
}

/*
    IDC_BITRATE
*/
INT_PTR CALLBACK LLApp::BitRateProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LLMainInitVars* pVars = (LLMainInitVars*)GetWindowLongPtr(hWnd, GWL_USERDATA);

	switch (uMsg)
	{
	case WM_INITDIALOG:
		{ 
            pVars = (LLMainInitVars*)((LPPROPSHEETPAGE)lParam)->lParam;
			SetWindowLong(hWnd, GWL_USERDATA, (DWORD_PTR)pVars);

            //SetDlgItemInt(hWnd, IDC_BITRATE, pVars->nBitRate, TRUE);
            CCString s;

            /*
                Set up bit rate values.
            */
            s.FromInt(6000, 10);
            SendMessage(GetDlgItem(hWnd, IDC_BITRATE), CB_ADDSTRING, 0, (LPARAM)(PCWSTR)s);
            s.FromInt(11000, 10);
            SendMessage(GetDlgItem(hWnd, IDC_BITRATE), CB_ADDSTRING, 0, (LPARAM)(PCWSTR)s);
            s.FromInt(16000, 10);
            SendMessage(GetDlgItem(hWnd, IDC_BITRATE), CB_ADDSTRING, 0, (LPARAM)(PCWSTR)s);
            s.FromInt(20000, 10);
            SendMessage(GetDlgItem(hWnd, IDC_BITRATE), CB_ADDSTRING, 0, (LPARAM)(PCWSTR)s);
            s.FromInt(28800, 10);
            SendMessage(GetDlgItem(hWnd, IDC_BITRATE), CB_ADDSTRING, 0, (LPARAM)(PCWSTR)s);
            s.FromInt(32000, 10);
            SendMessage(GetDlgItem(hWnd, IDC_BITRATE), CB_ADDSTRING, 0, (LPARAM)(PCWSTR)s);
            s.FromInt(56000, 10);
            SendMessage(GetDlgItem(hWnd, IDC_BITRATE), CB_ADDSTRING, 0, (LPARAM)(PCWSTR)s);
            s.FromInt(64000, 10);
            SendMessage(GetDlgItem(hWnd, IDC_BITRATE), CB_ADDSTRING, 0, (LPARAM)(PCWSTR)s);
            s.FromInt(96000, 10);
            SendMessage(GetDlgItem(hWnd, IDC_BITRATE), CB_ADDSTRING, 0, (LPARAM)(PCWSTR)s);
            s.FromInt(128000, 10);
            SendMessage(GetDlgItem(hWnd, IDC_BITRATE), CB_ADDSTRING, 0, (LPARAM)(PCWSTR)s);

            s.FromInt(pVars->nBitRate, 10);
            SelectString(hWnd, IDC_BITRATE, s);

			break;
		}
	case WM_NOTIFY:
		{
		    LPNMHDR lpnm = (LPNMHDR) lParam;

		    switch (lpnm->code)
		    {
            case PSN_KILLACTIVE:
                {
                    int n;
                    BOOL bSuccess;

                    /*
                        Grab the user input values, validate them and determine
                        whether we can go on with the next page or not.
                    */
                    n = GetDlgItemInt(hWnd, IDC_BITRATE, &bSuccess, TRUE);
                    if(bSuccess == FALSE)
                    {
                        MessageBox(hWnd, L"Invalid input for bit rate",
                            L"LoveLine Recorder", MB_OK | MB_ICONASTERISK);
                        SetWindowLong(hWnd, DWL_MSGRESULT, TRUE);
                        return TRUE;
                    }
                    if(n < MIN_BITRATE || n > MAX_BITRATE)
                    {
                        MessageBox(hWnd, L"The sample rate is out of range",
                            L"LoveLine Recorder", MB_OK | MB_ICONASTERISK);
                        SetWindowLong(hWnd, DWL_MSGRESULT, TRUE);
                        return TRUE;
                    }

                    pVars->nBitRate = n;

                    SetWindowLong(hWnd, DWL_MSGRESULT, FALSE);
                    break;
                }
		    case PSN_SETACTIVE: //Enable the Next button	 
		    	PropSheet_SetWizButtons(GetParent(hWnd), PSWIZB_BACK | PSWIZB_NEXT);
		    	break;
		    case PSN_WIZNEXT:
		    	//Handle a Next button click here
		    	break;
		    case PSN_WIZBACK:
		    	//Handle a Back button click here
		    	break;
		    case PSN_RESET:
		    	//Handle a Cancel button click, if necessary
		    	break;
		    }

            break;
		}
	}

	return 0;
}


/*
    IDC_PROGRESS
    IDC_ELAPSED
    IDC_REMAINING
*/
INT_PTR CALLBACK LLApp::RecordProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LLMainInitVars* pVars = (LLMainInitVars*)GetWindowLongPtr(hWnd, GWL_USERDATA);
    static BOOL bErr = FALSE;

	switch (uMsg)
	{
    case WM_DESTROY:
        {
            pVars->pThis->StopRecording(pVars);
            return TRUE;
        }
	case WM_INITDIALOG:
		{
            pVars = (LLMainInitVars*)((LPPROPSHEETPAGE)lParam)->lParam;
			SetWindowLong(hWnd, GWL_USERDATA, (DWORD_PTR)pVars);
            pVars->hDlg = hWnd;

            bErr = FALSE;

            pVars->Log.SetWindow(GetDlgItem(hWnd, IDC_LOG));

            SendMessage(GetDlgItem(hWnd, IDC_LOG), WM_SETFONT, (WPARAM)pVars->hCourier, (LPARAM)TRUE);

            // This starts the recording and starts our timers.
            if(FAILED(pVars->pThis->StartRecording(pVars)))
            {
                // Display a message box stating that there was an error.
                MessageBox(hWnd, L"There was an error initializing sound capture",
                    L"LoveLine Recorder", MB_OK);

                // Now make sure we're stopped.
                bErr = TRUE;
                SendMessage(hWnd, WM_TIMER, ID_TIMER_RECORD_TOTAL, 0);
            }

            SendMessage(GetDlgItem(hWnd, IDC_RECORDING), WM_SETFONT, (WPARAM)pVars->hTitle, (LPARAM)TRUE);

            break;
		}
	case WM_NOTIFY:
		{
		    LPNMHDR lpnm = (LPNMHDR) lParam;

		    switch (lpnm->code)
		    {
		    case PSN_SETACTIVE: //Enable the Next button	 
                PropSheet_SetWizButtons(GetParent(hWnd), PSWIZB_NEXT);
		    	break;
		    case PSN_WIZNEXT:
                {
                    // Let's see whether we're still recording or not.
                    if(pVars->bIsRecording == TRUE)
                    {
                        int n;
                        n = MessageBox(hWnd,
                            L"Are you sure you want to start encoding?\n"
                            L"This will stop recording.",
                            L"LoveLine Recorder", MB_YESNO);

                        SetWindowLong(hWnd, DWL_MSGRESULT, 0);

                        if(n == IDNO)
                        {
                            SetWindowLong(hWnd, DWL_MSGRESULT, -1);
                            return TRUE;
                        }
                        // Stop the recorder and continue.
                        pVars->pThis->StopRecording(pVars);
                        return TRUE;
                    }
                    else
                    {
                        // The recorder isn't going; don't do anything.
                    }
		    	    break;
                }
		    case PSN_WIZBACK:
		    	//Handle a Back button click here
		    	break;
            case PSN_QUERYCANCEL:
                // Ask if they really want to quit recording or not
                return MessageBox(hWnd, L"Are you sure you want to cancel and exit?",
                    L"LoveLine Recorder", MB_YESNO)
                    == IDYES ? FALSE : TRUE;
            case PSN_RESET:
		    	//Handle a Cancel button click, if necessary
                pVars->pThis->StopRecording(pVars);
		    	break;
		    }

            break;
		}
    case WM_TIMER:
        {
            switch(wParam)
            {
            case ID_TIMER_RECORD_TOTAL:
                {
                    CCString Time;

                    // We're done recording.
                    pVars->pThis->StopRecording(pVars);

                    SendDlgItemMessage(hWnd, IDC_PROGRESS, PBM_SETPOS, pVars->nSeconds, 0);
                    FormatSeconds(&Time, pVars->nSeconds);
                    SetDlgItemText(hWnd, IDC_ELAPSED, Time);

                    FormatSeconds(&Time, 0);
                    SetDlgItemText(hWnd, IDC_REMAINING, Time);

                    // Enable the next button.
		    	    PropSheet_SetWizButtons(GetParent(hWnd), PSWIZB_NEXT);

                    // If bExitAutomatically is TRUE, go on to the next screen to start encoding.
                    if(pVars->bExitAutomatically == TRUE)
                    {
                        PropSheet_SetCurSelByID(GetParent(hWnd), IDD_ENCODE);
                    }

                    return TRUE;
                }
            case ID_TIMER_RECORD_PROGRESS:
                // Update the controls to reflect new time.
                {
                    DWORD dwElapsedSeconds;
                    DWORD dwRemainingSeconds;
                    CCString Time;

                    dwElapsedSeconds = (GetTickCount() - pVars->pThis->dwStartTick) / 1000;
                    dwRemainingSeconds = pVars->nSeconds - dwElapsedSeconds;

                    SendDlgItemMessage(hWnd, IDC_PROGRESS, PBM_SETPOS, dwElapsedSeconds, 0);

                    // Format the seconds and stuff.
                    FormatSeconds(&Time, dwElapsedSeconds);
                    SetDlgItemText(hWnd, IDC_ELAPSED, Time);

                    FormatSeconds(&Time, dwRemainingSeconds);
                    SetDlgItemText(hWnd, IDC_REMAINING, Time);

                    return TRUE;
                }
            }
        }
	}
	return 0;
}


/*
    This signals the recording process.  It's called when the record dialog
    is displayed.
*/
HRESULT LLApp::StartRecording(LLMainInitVars* pVars)
{
    WAVEFORMATEX Format;

    pVars->IntermediateFile = pVars->FileName;
    PathRenameExtension(pVars->IntermediateFile.GetBuffer(pVars->IntermediateFile.len() + 10), L".wav");

    pVars->Log.AddEntry(L"Attempting to start recording.");
    pVars->Log.Refresh();

    // Using the data from the dialogs, fill out a WAVEFORMATEX structure.
    Format.wFormatTag = WAVE_FORMAT_PCM;
    Format.cbSize = 0;
    Format.nChannels = pVars->nChannels;
    Format.nSamplesPerSec = pVars->nSampleRate;
    Format.wBitsPerSample = pVars->nBitDepth;
    Format.nBlockAlign = (pVars->nBitDepth / 8) * pVars->nChannels;
    Format.nAvgBytesPerSec = Format.nBlockAlign * Format.nSamplesPerSec;

    // Now initialize the capturer - we'll use a 2 second interval for data.
    if(FAILED(this->Capturer.Initialize(&Format, LLApp::CaptureDataProc, pVars, 4, &pVars->Log)))
    {
        pVars->Log.Refresh();
        return E_FAIL;
    }

    // Initialize the writer now.
    if(FAILED(this->Writer.Open(&Format, pVars->IntermediateFile, &pVars->Log)))
    {
        pVars->Log.Refresh();
        return E_FAIL;
    }

    if(FAILED(this->Capturer.Start(&pVars->Log)))
    {
        pVars->Log.Refresh();
        return E_FAIL;
    }

    // And we have to make sure we set two timers.
    // One for small increments (for the progress bar, etc...) and one for
    // the final "finished" signal.
    SetTimer(pVars->hDlg, ID_TIMER_RECORD_PROGRESS, 1000, NULL);
    SetTimer(pVars->hDlg, ID_TIMER_RECORD_TOTAL, pVars->nSeconds * 1000, NULL);

    this->dwStartTick = GetTickCount();

    /*
        Set the progress bar range
    */
    SendDlgItemMessage(pVars->hDlg, IDC_PROGRESS, PBM_SETRANGE32, 0, pVars->nSeconds);

    pVars->Log.AddEntry(L"Recording started successfully.");
    pVars->Log.Refresh();

    pVars->bIsRecording = TRUE;

    return S_OK;
}


/*
    This is called in a different thread than the UI so I have to be careful in here.
*/
HRESULT CALLBACK LLApp::CaptureDataProc(PBYTE pData, DWORD dwSize, PVOID pUser)
{
    HRESULT hr;
    LLMainInitVars* pVars = (LLMainInitVars*)pUser;
    LLApp* pThis = pVars->pThis;

    //odsf(L"LLApp::CaptureData", L"Writing %d bytes of data", dwSize);

    if(FAILED(hr = pThis->Writer.WriteBits(pData, dwSize, &pVars->Log)))
    {
        pVars->Log.Refresh();
        return E_FAIL;
    }

    return S_OK;
}


HRESULT LLApp::StopRecording(LLMainInitVars* pVars)
{
    // Also stop the timers.
    KillTimer(pVars->hDlg, ID_TIMER_RECORD_TOTAL);
    KillTimer(pVars->hDlg, ID_TIMER_RECORD_PROGRESS);

    this->Capturer.Stop(&pVars->Log);
    this->Capturer.Uninitialize();

    this->Writer.Close(&pVars->Log);

    pVars->Log.AddEntry(L"Audio capture stopped successfully.");
    pVars->Log.Refresh();

    pVars->bIsRecording = FALSE;

    return S_OK;
}


DWORD WINAPI LLApp::EncodeThreadProc(PVOID pUser)
{
    CoInitialize(NULL);

    LLMainInitVars* pVars = (LLMainInitVars*)pUser;
    WAVEFORMATEX wfx;

    HRESULT hr;
    PBYTE pBuf = NULL;
    DWORD dwBufSize;
    DWORD br;// bytes read.
    DWORD dwWaveSize;
    DWORD dwCurrentSize;// How much we've read so far (used for progress)
    DWORD nPercent;

    pVars->bIsEncoding = TRUE;

    if(FAILED(pVars->pThis->Reader.Open(pVars->IntermediateFile, FALSE, &pVars->Log)))
    {
        pVars->Log.Refresh();
        PostMessage(pVars->hDlg, ID_TIMER_ENCODE_TOTAL, 0, 0);
        CoUninitialize();
        pVars->bIsEncoding = FALSE;
        return E_FAIL;
    }

    pVars->pThis->Reader.GetFormat(&wfx);

    if(FAILED(pVars->pThis->WMAWriter.Open(&wfx, pVars->FileName, pVars->nBitRate, FALSE, &pVars->Log)))
    {
        pVars->Log.Refresh();
        pVars->pThis->Reader.Close();
        PostMessage(pVars->hDlg, ID_TIMER_ENCODE_TOTAL, 0, 0);
        CoUninitialize();
        pVars->bIsEncoding = FALSE;
        return E_FAIL;
    }

    dwBufSize = ENCODE_BUFFER_SIZE * wfx.nAvgBytesPerSec;
    pBuf = (PBYTE)malloc(dwBufSize);
    if(pBuf == NULL)
    {
        pVars->Log.AddEntry(L"Couldn't allocate memory for encoding.");
        pVars->Log.Refresh();

        pVars->pThis->Reader.Close();
        pVars->pThis->WMAWriter.Close(&pVars->Log);
        PostMessage(pVars->hDlg, ID_TIMER_ENCODE_TOTAL, 0, 0);
        CoUninitialize();
        pVars->bIsEncoding = FALSE;
        return E_FAIL;
    }

    dwWaveSize = pVars->pThis->Reader.GetDataSize();

    dwCurrentSize = 0;

    pVars->pThis->dwStartTick = GetTickCount();
    pVars->Log.AddEntry(L"Encoding started successfully.");
    pVars->Log.Refresh();

    DWORD dwTick = GetTickCount();

    while(1)
    {
        // See if we are supposed to cancel.
        if(WaitForSingleObject(pVars->hEncodingCancel, 0) == WAIT_OBJECT_0)
        {
            free(pBuf);
            pVars->pThis->Reader.Close();
            pVars->pThis->WMAWriter.Close(&pVars->Log);
            PostMessage(pVars->hDlg, ID_TIMER_ENCODE_TOTAL, 0, 0);
            pVars->Log.AddEntry(L"Encoding cancelled successfully.");
            pVars->Log.Refresh();
            CoUninitialize();
            pVars->bIsEncoding = FALSE;
            return 0;
        }

        if((GetTickCount() - dwTick) > 1000)
        {
            nPercent = (int)(((double)dwCurrentSize / (double)dwWaveSize) * 100);
            PostMessage(pVars->hDlg, ID_TIMER_ENCODE_PROGRESS, 0, nPercent);
            dwTick = GetTickCount();
            //odsf(L"EncodeThreadProc", L"dwCurrentSize (%d 0x%08x) dwWaveSize (%d 0x%08x) nPercent %d",
            //    dwCurrentSize, dwCurrentSize, dwWaveSize, dwWaveSize, nPercent);
        }

        // Encode some.
        hr = pVars->pThis->Reader.Read(pBuf, dwBufSize, &br, &pVars->Log);
        if(FAILED(hr) || (br != dwBufSize))
        {
            // This is probably a EOF so don't log it.
            if(hr != ERROR_HANDLE_EOF)
            {
                pVars->Log.Refresh();
            }
            
            // Now either we failed reading (severed) or we're at the end of the file.
            // Let's complete the process and exit
            if(FAILED(pVars->pThis->WMAWriter.WriteBits(pBuf, br, &pVars->Log)))
            {
                pVars->Log.Refresh();
            }

            free(pBuf);
            pVars->pThis->Reader.Close();
            pVars->pThis->WMAWriter.Close(&pVars->Log);
            PostMessage(pVars->hDlg, ID_TIMER_ENCODE_TOTAL, 0, 0);
            pVars->Log.AddEntry(L"Encoding completed successfully.");
            pVars->Log.Refresh();

            // Delete the input file.
            if(pVars->bDeleteFile == TRUE)
            {
                DeleteFile(pVars->IntermediateFile);
            }

            CoUninitialize();
            pVars->bIsEncoding = FALSE;
            return 0;
        }

        dwCurrentSize += br;

        if(FAILED(pVars->pThis->WMAWriter.WriteBits(pBuf, br, &pVars->Log)))
        {
            pVars->Log.Refresh();
        }
    }

    // This is no-mans land.
    return 0;
}


HRESULT LLApp::StartEncoding(LLMainInitVars* pVars)
{
    pVars->Log.AddEntry(L"Attempting to start encoding.");

    // Set the range of the progress bar to percent now.
    SendMessage(GetDlgItem(pVars->hDlg, IDC_PROGRESS), PBM_SETRANGE32, 0, 100);
    SendMessage(GetDlgItem(pVars->hDlg, IDC_PROGRESS), PBM_SETPOS, 0, 0);

    SetDlgItemText(pVars->hDlg, IDC_RECORDING, L"Encoding");

    DWORD dwId = 0;

    pVars->hEncodingThread = CreateThread(NULL, 0, LLApp::EncodeThreadProc,
        pVars, 0, &dwId);

    return S_OK;
}


HRESULT LLApp::StopEncoding(LLMainInitVars* pVars)
{
    if(pVars->hEncodingThread == NULL) return S_OK;

    // Set the cancel event and wait for the thread to stop.
    SetEvent(pVars->hEncodingCancel);

    // Because the encoding thead may want to use some window messages,
    // let's do a little message loop.
    MSG msg;
    while(WaitForSingleObject(pVars->hEncodingThread, 0) != WAIT_OBJECT_0)
    {
        if(PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE))
        {
            GetMessage(&msg, 0, 0, 0);
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    CloseHandle(pVars->hEncodingThread);
    pVars->hEncodingThread = NULL;
    pVars->bIsEncoding = FALSE;

    return S_OK;
}


/*
    IDC_PROGRESS
    IDC_ELAPSED
    IDC_REMAINING
*/
INT_PTR CALLBACK LLApp::EncodeProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LLMainInitVars* pVars = (LLMainInitVars*)GetWindowLongPtr(hWnd, GWL_USERDATA);
    static BOOL bErr = FALSE;

	switch (uMsg)
	{
    case WM_DESTROY:
        {
            pVars->pThis->StopEncoding(pVars);
            return TRUE;
        }
	case WM_INITDIALOG:
		{
            pVars = (LLMainInitVars*)((LPPROPSHEETPAGE)lParam)->lParam;
			SetWindowLong(hWnd, GWL_USERDATA, (DWORD_PTR)pVars);
            pVars->hDlg = hWnd;

            bErr = FALSE;

            pVars->Log.SetWindow(GetDlgItem(hWnd, IDC_LOG));

            SendMessage(GetDlgItem(hWnd, IDC_LOG), WM_SETFONT, (WPARAM)pVars->hCourier, (LPARAM)TRUE);

            // This starts the encoding.
            if(FAILED(pVars->pThis->StartEncoding(pVars)))
            {
                // Display a message box stating that there was an error.
                MessageBox(hWnd, L"There was an error initializing encoding.",
                    L"LoveLine Recorder", MB_OK);

                // Now make sure we're stopped.
                bErr = TRUE;
                SendMessage(hWnd, WM_TIMER, ID_TIMER_ENCODE_TOTAL, 0);
            }

            SendMessage(GetDlgItem(hWnd, IDC_ENCODING), WM_SETFONT, (WPARAM)pVars->hTitle, (LPARAM)TRUE);

            break;
		}
	case WM_NOTIFY:
		{
		    LPNMHDR lpnm = (LPNMHDR) lParam;

		    switch (lpnm->code)
		    {
		    case PSN_SETACTIVE: //Enable the Next button	 
                PropSheet_SetWizButtons(GetParent(hWnd), PSWIZB_NEXT);
		    	break;
		    case PSN_WIZNEXT:
                // Let's see whether we're still recording or not.
                if(pVars->bIsEncoding == TRUE)
                {
                    int n;
                    n = MessageBox(hWnd,
                        L"Are you sure you want to stop encoding?",
                        L"LoveLine Recorder", MB_YESNO);

                    if(n == IDNO)
                    {
                        SetWindowLong(hWnd, DWL_MSGRESULT, -1);
                        return TRUE;
                    }
                    else
                    {
                        SetWindowLong(hWnd, DWL_MSGRESULT, 0);
                    }

                    // Stop the recorder and continue.
                    pVars->pThis->StopEncoding(pVars);
                }
                else
                {
                    // The encoder isn't going; don't do anything.
                }

                return TRUE;
		    case PSN_WIZBACK:
		    	//Handle a Back button click here
		    	break;
            case PSN_QUERYCANCEL:
                // Ask if they really want to quit recording or not
                return MessageBox(hWnd, L"Are you sure you want to stop encoding?",
                    L"LoveLine Recorder", MB_YESNO)
                    == IDYES ? FALSE : TRUE;
            case PSN_RESET:
		    	//Handle a Cancel button click, if necessary
                pVars->pThis->StopEncoding(pVars);
		    	break;
		    }

            break;
		}
    case ID_TIMER_ENCODE_TOTAL:
        {
            CCString Time;

            DWORD dwSeconds = (GetTickCount() - pVars->pThis->dwStartTick) / 1000;

            // We're done encoding.
            pVars->pThis->StopEncoding(pVars);

            SendDlgItemMessage(hWnd, IDC_PROGRESS, PBM_SETPOS, 100, 0);
            FormatSeconds(&Time, dwSeconds);
            SetDlgItemText(hWnd, IDC_ELAPSED, Time);

            FormatSeconds(&Time, 0);
            SetDlgItemText(hWnd, IDC_REMAINING, Time);

            // Enable the next button.
		    PropSheet_SetWizButtons(GetParent(hWnd), PSWIZB_NEXT);
            pVars->bIsRecording = FALSE;

            // If bExitAutomatically is TRUE, exit.  Otherwise just show the NEXT button.
            if(pVars->bExitAutomatically == TRUE)
            {
                DestroyWindow(GetParent(hWnd));
            }

            return TRUE;
        }

    case ID_TIMER_ENCODE_PROGRESS:
        {
            // Update the controls to reflect new time.  lParam is the percentage (0-100)
            DWORD dwElapsedSeconds;
            DWORD dwRemainingSeconds;
            DWORD dwTotalSeconds;
            CCString Time;

            dwElapsedSeconds = (GetTickCount() - pVars->pThis->dwStartTick) / 1000;

            if(lParam == 0)
            {
                dwTotalSeconds = dwElapsedSeconds;
            }
            else
            {
                dwTotalSeconds = (100 * dwElapsedSeconds) / lParam;
            }

            dwRemainingSeconds = dwTotalSeconds - dwElapsedSeconds;

            SendDlgItemMessage(hWnd, IDC_PROGRESS, PBM_SETPOS, lParam, 0);

            // Format the seconds and stuff.
            FormatSeconds(&Time, dwElapsedSeconds);
            SetDlgItemText(hWnd, IDC_ELAPSED, Time);

            FormatSeconds(&Time, dwRemainingSeconds);
            SetDlgItemText(hWnd, IDC_REMAINING, Time);

            return TRUE;
        }
	}
	return 0;
}


