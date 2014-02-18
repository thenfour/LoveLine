

#pragma once


#include "CCControl.h"
#include "CCString.h"
#include "Global.h"


struct SLIDER_COMMERCIAL_LIST_ITEM
{
    DWORD Out()
    {
        return max(this->dwIn, this->dwOut);
    }

    DWORD* _Out()
    {
        if(this->dwIn < this->dwOut) return &this->dwOut;
        return &this->dwIn;
    }

    DWORD* _In()
    {
        if(this->dwIn > this->dwOut) return &this->dwOut;
        return &this->dwIn;
    }

    DWORD In()
    {
        return min(this->dwIn, this->dwOut);
    }

    DWORD Len()
    {
        return this->Out() - this->In();
    }

    PVOID pParam;
    GUID id;
    DWORD dwIn;
    DWORD dwOut;

    SLIDER_COMMERCIAL_LIST_ITEM* pNext;
    SLIDER_COMMERCIAL_LIST_ITEM* pPrevious;
};


struct SLIDER_BOOKMARK_LIST_ITEM
{
    PVOID pParam;
    GUID id;
    CCString sName;
    DWORD dwPos;

    SLIDER_BOOKMARK_LIST_ITEM* pNext;
    SLIDER_BOOKMARK_LIST_ITEM* pPrevious;
};


struct SLIDER_COMMERCIAL
{
    PVOID pParam;
    GUID id;
    DWORD dwIn;
    DWORD dwOut;
};


struct SLIDER_BOOKMARK
{
    PVOID pParam;
    GUID id;
    CCString sName;
    DWORD dwPos;
};


/*
    When something's highlighted, it needs to be stored somehow.
    These are values that describe WHAT is highlighted.
*/
#define LLS_NONE          0x00000000
#define LLS_CURSOR        0x00000001
#define LLS_BOOKMARK      0x00000002
#define LLS_COMMERCIALIN  0x00000003
#define LLS_COMMERCIALOUT 0x00000004


struct LLSHighlight
{
    DWORD dwType;
    DWORD dwIndex;// What index of that object is it.
};


// Notifications:
#define LSN_FIRST              0x00001000

#define LSN_STARTDRAGGING      LSN_FIRST + 0x00000010
#define LSN_ENDDRAGGING        LSN_FIRST + 0x00000011
#define LSN_DRAGMOVE           LSN_FIRST + 0x00000016// Sent when the user moves anything (cursor, etc... but is not sent when SetPos() is called.

#define LSN_REMOVECOMMERCIAL   LSN_FIRST + 0x00000012// Sent AFTER it was deleted.
#define LSN_CHANGECOMMERCIAL   LSN_FIRST + 0x00000013// Sent AFTER its changed.

#define LSN_CHANGEBOOKMARK     LSN_FIRST + 0x00000014// Sent AFTER its changed.
#define LSN_REMOVEBOOKMARK     LSN_FIRST + 0x00000015// Sent AFTER its removed.

#define LSN_SETPOS             LSN_FIRST + 0x00000017// Sent every time SetPos() is called.  No lparam.

struct LSN_DRAG_STRUCT
{
    NMHDR hdr;
    DWORD dwNewPosition;
    LLSHighlight DragObject;
};


struct LSN_REMOVECOMMERCIAL_STRUCT
{
    NMHDR hdr;
    SLIDER_COMMERCIAL Commercial;
};


struct LSN_CHANGECOMMERCIAL_STRUCT
{
    NMHDR hdr;
    SLIDER_COMMERCIAL Old;
    SLIDER_COMMERCIAL New;
};


struct LSN_REMOVEBOOKMARK_STRUCT
{
    NMHDR hdr;
    SLIDER_BOOKMARK Bookmark;
};


struct LSN_CHANGEBOOKMARK_STRUCT
{
    NMHDR hdr;
    SLIDER_BOOKMARK Old;
    SLIDER_BOOKMARK New;
};


//////////////////////////////////////////////////////////////////////////////
class CCEXPORT LLSlider : public CCControl
{
public:
	LLSlider();
	~LLSlider();

    HRESULT Create(HWND hParent, HINSTANCE hInstance, RECT prect, RECT orect);

    DWORD GetPos();
    void  SetPos(DWORD dw);
    void  SetLength(DWORD dwLen);

    BOOL IsDragging(LLSHighlight* pWhat);

    /*
        Returns NULL if the specified position is not inside a commercial.
        Otherwise, the index of the specified commercial is returned.
    */
    int HitTest(DWORD dwPos);

    HRESULT ClearCommercials();
    HRESULT AddCommercial(DWORD dwIn, DWORD dwOut, PVOID pParam, GUID id);
    HRESULT SetCommercial(int i, DWORD dwIn, DWORD dwOut, PVOID pParam, GUID id);
    HRESULT RemoveCommercial(int i);
    int     GetCommercialCount();
    HRESULT GetCommercial(int i, SLIDER_COMMERCIAL* pCommercial);

    HRESULT ClearBookmarks();
    HRESULT AddBookmark(DWORD dwIn, CCString Text, PVOID pParam, GUID id);
    HRESULT SetBookmark(int i, DWORD dwIn, CCString Text, PVOID pParam, GUID id);
    HRESULT RemoveBookmark(int i);
    int     GetBookmarkCount();
    HRESULT GetBookmark(int i, SLIDER_BOOKMARK* pCommercial);

private:

    /*
        This returns a pointer to the DWORD position that is described
        by pHighlight.  For instance, if pHighlight.dwType == LLS_CURSOR,
        this returns &this->dwPos.
    */
    DWORD* _GetPosition(LLSHighlight* pHighlight);

    /*
        This will set the cursor at the highlighted position.
    */
    void SetCursorOnHighlight();

    HBRUSH hFlatLine;
    HBRUSH hCursor;
    HBRUSH hHighlight;
    HBRUSH hCommercial;
    HBRUSH hBackground;
    HBRUSH hBookmark;

    HBRUSH hBookmarkHandle;
    HBRUSH hCommercialSelected;
    HBRUSH hCommercialHandle;

    HPEN hDelete;

    SLIDER_COMMERCIAL_LIST_ITEM* _GetCommercial(int i);
    SLIDER_BOOKMARK_LIST_ITEM* _GetBookmark(int i);

    void SendParentNotification(DWORD dwMsg, PVOID lParam);

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    /*
        Drawing stuff... This should all happen in a WM_PAINT message.
        These functions all take into account the invalid rectangle in the
        PAINTSTRUCT struct for optimization.
    */
    CCString sStatusText;

    // The highlight describes what's being highlighted - this also
    // is valid for whent he user is dragging an object.
    LLSHighlight Highlight;
    DWORD dwOldPos;// NOT pixels.
    BOOL bDeleting;// When an item is being dragged off to be deleted this will be TRUE.
    DWORD* pdw;// This is a pointer to the value that's being dragged.

    POINT pOldPos;// Old MOUSE coords.

    RECT rcCursor;
    RECT rcWindow;
    RECT rcStatus;
    RECT rcBookmark;
    RECT rcSlider;
    RECT rcStatusFrame;
    RECT rcBookmarkFrame;
    RECT rcSliderFrame;

    RECT rcHighlight;// Rectangle that surrounds the last known highlighted region.
    void InvalidateHighlight();// Invalidates only the highlighted rectangle.

    BOOL bIsDragging;
    void CalculateHighlight(POINT* p);
    void CalculateHighlightRect();// Based on this->Highlight.

    void Draw(PAINTSTRUCT* ps);
    void DrawBlueLine(PAINTSTRUCT* ps);
    void DrawCommercial(SLIDER_COMMERCIAL_LIST_ITEM* p, PAINTSTRUCT* ps, BOOL bSelected);
    void CalculateCursorRect(RECT* prc);// This just approximates.
    void DrawCursor(PAINTSTRUCT* ps);
    void DrawBookmark(SLIDER_BOOKMARK_LIST_ITEM* p, PAINTSTRUCT* ps, BOOL bSelected);
    void DrawStatus(PAINTSTRUCT* ps);
    DWORD TimeToPixels(DWORD dwMS);
    DWORD PixelsToTime(int dwPixels);

	CCString sClass;

    DWORD dwLength;
    DWORD dwPos;

    void CheckBounds(DWORD* pdw);

    SLIDER_COMMERCIAL_LIST_ITEM* pCommercials;
    SLIDER_BOOKMARK_LIST_ITEM* pBookmarks;
};

