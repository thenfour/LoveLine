

#pragma once


#include "Global.h"
#include "CCString.h"
#include "LLDatabase.h"
#include "LLTypes.h"
#include "CCListBox.h"


/*
    Preview Image sizes.
*/
#define PREVIEW_SIZEX 80
#define PREVIEW_SIZEY 25
#define PREVIEW_HANDLESIZE 4

#define ICON_SIZEX 16
#define ICON_SIZEY 16

/*
    Pass in a valid FULL filename (file should exist on disk)
*/
void CCEXPORT DateFromFileName(CCString s, SYSTEMTIME* pTime);

HRESULT CCEXPORT _GetFileTime(CCString sFileName, SYSTEMTIME* pTime);
void CCEXPORT Decode8DigitFormat(CCString s, SYSTEMTIME* pTime);
void CCEXPORT Decode6DigitFormat(CCString s, SYSTEMTIME* pTime);


class FILE_FILTER
{
public:
    void Set(CCString Name, CCString Filter)
    {
        this->Name = Name;
        this->Filter = Filter;
    }

    CCString Name;
    CCString Filter;
};


HRESULT CCEXPORT SaveFileDlg(
    HWND hWnd, CCString* FileName,
    int nFilters, FILE_FILTER* pFilters, CCString DefExt, PCWSTR wszInitialDir);

HRESULT CCEXPORT OpenFileDlg(
    HWND hWnd, CCString* FileName,
    int nFilters, FILE_FILTER* pFilters, PCWSTR wszInitialDir);


HRESULT CCEXPORT LLBrowseForFolder(HWND, CCString& sOut);


PWSTR CCEXPORT ConvertFileFilter(int nFilters, FILE_FILTER* pFilters);


HRESULT CCEXPORT LLFormatDate(SYSTEMTIME* pst, CCString* pFormat);
HRESULT CCEXPORT LLFormatDate(LLEpisode* pEpisode, CCString* pFormat);
HRESULT CCEXPORT LLFormatMilliseconds(DWORD dwMS, CCString* pSeconds);


HRESULT CCEXPORT SystemTimeToLLTime(const SYSTEMTIME* pst, LLTIME* pTime);
HRESULT CCEXPORT LLTimeToSystemTime(SYSTEMTIME* pst, const LLTIME pTime);



/*
    Returns the length, in ms, of the typical commercial at the specified
    time.
*/
DWORD CCEXPORT GetCommercialLength(LLDatabase* pdb, DWORD dwTime, CCLog* pLog);

HRESULT CCEXPORT GetNewFeatureInfo(HWND hParent, CCString* pTitle, CCString* pDesc);

HBITMAP CCEXPORT CreateCommercialPreviewImage(DWORD dwEpisodeLen, LLCommercial* pc);
HBITMAP CCEXPORT CreateLinkPreviewImage(DWORD dwEpisodeLen, LLLink* pl);
HBITMAP CCEXPORT CreateCursorPreviewImage(DWORD dwEpisodeLen, LLLink* pl);
HBITMAP CCEXPORT CreateBlankPreviewImage();

/*
    Puts dimentions in prc that you could pass to CreateWindow().
*/
HRESULT CCEXPORT GetControlRect(HWND hControl, RECT* prc);

HRESULT CCEXPORT LoadListViewSettings(CCString SettingName, CCListBox* pl, LLDatabase* pdb, CCLog* pLog);
HRESULT CCEXPORT SaveListViewSettings(CCString SettingName, CCListBox* pl, LLDatabase* pdb, CCLog* pLog);


#define MAKEMS(h,m,s) (((h)*3600000) + ((m)*60000) + ((s)*1000) )
#define HOURSFROMMS(ms) ((int)((ms) / 3600000))
#define MINUTESFROMMS(ms) (((ms) / 60000) % 60)
#define SECONDSFROMMS(ms) (((ms) / 1000) % 60)

HRESULT CCEXPORT GetSegmentDataDlg(IN OUT LLSegment* ps, HWND hParent);
BOOL CCEXPORT PtInSegment(DWORD dwPos, LLSegment* p);



