

#pragma once


#include <Windows.h>
#include "CCString.h"
#include "global.h"


class CCEXPORT CCLog
{
public:
    CCLog();
    ~CCLog();

    void SetWindow(HWND hEdit);
    void SetFile(CCString FileName);
    void AddEntry(HRESULT hr, CCString FormatString, ...);
    void AddEntry(CCString FormatString, ...);
    void _AddEntry(CCString FormatString);
    void Refresh();

private:

    void EnsureFileOpened();
    HANDLE hFile;
    BOOL bFileError;// TRUE when we couldn't open the file.

    HWND hEdit;
    CCString HiddenText;// This will be appended to Log when Refresh() is called.
    CCString Log;
};


