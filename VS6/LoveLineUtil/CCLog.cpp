

#include "pch.h"
#include "CCLog.h"
#include "TraceErrorSys.h"


CCLog::CCLog()
{
    this->hEdit = 0;
    this->HiddenText = L"";
    this->Log = L"";
    this->hFile = NULL;
    this->bFileError = FALSE;
}


CCLog::~CCLog()
{
    if(this->hFile)
    {
        CloseHandle(this->hFile);
    }
}


void CCLog::EnsureFileOpened()
{
    CCString sModuleName;
    CCString sLogName;

    // Here we make sure that our file is opened and if not,
    // We'll open it and add a header entry.

    if(this->hFile != 0) return;
    if(this->bFileError == TRUE) return;

    // Open the file.
    GetModuleFileName(GetModuleHandle(NULL), sModuleName.GetBuffer(MAX_PATH), MAX_PATH);
    sLogName = sModuleName;
    sModuleName.Path_StripToFilename();
    sModuleName.cat(L".log");

    sLogName.Path_StripToPath();
    sLogName.Path_Append(L"..\\Log");
    sLogName.Path_Append(sModuleName);

    this->hFile = CreateFile(sLogName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_ALWAYS, NULL, NULL);

    if(this->hFile == INVALID_HANDLE_VALUE || this->hFile == 0)
    {
        // Error.
        this->bFileError = TRUE;
        this->hFile = NULL;
        this->AddEntry(L"Error opening the log file \"%s\"", (PCWSTR)sLogName);
    }

    SetFilePointer(this->hFile, 0, 0, FILE_END);

    CCString sHeader;
    DWORD br;

    sHeader = 
        L"\r\n--------------------------------------------------------\r\n"
        L"        Log file opened successfully.\r\n"
        L"--------------------------------------------------------\r\n";
    WriteFile(this->hFile, (PCWSTR)sHeader, sHeader.len() * sizeof(WCHAR),
        &br, NULL);
}


void CCLog::_AddEntry(CCString s)
{
    //
    //   [ Oct 31 22:19.13 ] asdfasdfasdfasdfasdfasdfasdf
    //
    CCString Date;
    CCString Time;
    CCString NewEntry;

    // Make sure that our log is open.
    this->EnsureFileOpened();

    SYSTEMTIME st;
    GetLocalTime(&st);
    GetDateFormat(LOCALE_USER_DEFAULT, NULL, &st, 
        L"MMM dd", Date.GetBuffer(1024), 1024);

    GetTimeFormat(LOCALE_USER_DEFAULT, TIME_FORCE24HOURFORMAT, &st,
        L"hh':'mm'.'ss", Time.GetBuffer(1024), 1024);

    NewEntry.FormatW(L"[%s %s] %s\r\n", (PCWSTR)Date, (PCWSTR)Time, (PCWSTR)s);
    this->HiddenText.cat(NewEntry);

    // Write it to teh file
    if(this->hFile != NULL)
    {
        DWORD br;
        WriteFile(this->hFile, (PCWSTR)NewEntry, NewEntry.len() * sizeof(WCHAR),
            &br, NULL);
    }

    // Write it to debug output
    OutputDebugString((PCWSTR)NewEntry);

    if(this->hEdit)
    {
        int nStart = GetWindowTextLength(this->hEdit);
        SendMessage(this->hEdit, EM_SETSEL, nStart, nStart);
        SendMessage(this->hEdit, EM_REPLACESEL, NULL, (LPARAM)((PCWSTR)NewEntry));
    }
}


/*
    Puts the hidden text in the log and updates the edit box.
*/
void CCLog::Refresh()
{
    if(this->HiddenText.len() == 0) return;

    int nLen = GetWindowTextLength(this->hEdit);

    // Set the limit text to allow the text to be in there
    SendMessage(this->hEdit, EM_LIMITTEXT, nLen + this->HiddenText.len() + 10, 0); 
    // Then put the cursor at the end
    SendMessage(this->hEdit, EM_SETSEL, nLen, nLen);
    // And paste the new data in there.
    SendMessage(this->hEdit, EM_REPLACESEL, FALSE, (LPARAM)(PCWSTR)this->HiddenText);

    // And finally update our own text buffer.
    this->Log.cat(this->HiddenText);
    this->HiddenText = L"";
}


void CCLog::SetWindow(HWND hEdit)
{
    this->hEdit = hEdit;
    SetWindowText(hEdit, this->Log);
}


void CCLog::SetFile(CCString FileName)
{
}


void CCLog::AddEntry(HRESULT hr, CCString FormatString, ...)
{
    CCString NewEntry;
    CCString Err;
    va_list args;

    TraceErrorSys(&Err, hr);

    va_start(args, FormatString);
    NewEntry.FormatvW(FormatString, args);
    va_end(args);

    NewEntry.cat(Err);

    this->AddEntry(NewEntry);
}


void CCLog::AddEntry(CCString FormatString, ...)
{
    CCString NewEntry;
    va_list args;

    va_start(args, FormatString);
    NewEntry.FormatvW(FormatString, args);
    va_end(args);

    this->_AddEntry(NewEntry);
}



