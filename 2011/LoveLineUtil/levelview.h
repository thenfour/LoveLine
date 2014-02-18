/*
    This is similar to a progress bar in that it just displays a bar of varying lengths
*/


#pragma once


#include <windows.h>
#include "CCControl.h"
#include "simpleimage.h"


#define LVM_SETRANGE (WM_APP+1)// wParam = min, lParam = max;
#define LVM_SETPOS   (WM_APP+2)// lParam = pos

// runs in GUI thread
class CCEXPORT CLevelView : public CCControl
{
public:
    CLevelView();
    ~CLevelView();

    void Create(HWND parent, RECT& prect, RECT& orect);

    void SetExtents(long lmin, long lmax);
    void GetExtents(long* pmin, long* pmax);

    void GetPos(long* lpos);
    void SetPos(long lpos);

private:
    long m_min;
    long m_max;
    long m_pos;

    void _Destroy();

    void _Draw(HDC dc, RECT*);

    CSimpleImage m_yellow_on;
    CSimpleImage m_yellow_off;
    CSimpleImage m_red_on;
    CSimpleImage m_red_off;
    CSimpleImage m_green_on;
    CSimpleImage m_green_off;

    HBRUSH m_hbr;// background.

    // Our double-buffer... buffer.
    HBITMAP m_hBuffer;// gets recreated every OnSize()
    HDC m_dc;// For keepin around for the long haul too.

    static LPARAM CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

    // more OnSize()-updated stuff:
    int m_nGreen;// number of green LEDs
    int m_nYellow;
    int m_nRed;
    int m_nTotal;// number of LEDs total
    int m_nOffsetX;
    int m_nOffsetY;
};


