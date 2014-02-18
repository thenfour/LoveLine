

#pragma once


#include <windows.h>
#include "global.h"


//A unique name for a class...
#define PROGRESSBAR_CLASS L"4ea0d3dd-4fd1-43fd-b9bb-13d2153ac800"


class CCProgressBar;


typedef void (CALLBACK* PROGRESSBARPROC)(LPVOID pUser, CCProgressBar* pBar, PWSTR Text);


class CCEXPORT CCProgressBar
{
public:
	CCProgressBar();
	~CCProgressBar();

	int Create(PROGRESSBARPROC pProc, PVOID pUser, HWND hParent, HINSTANCE hInstance, int x, int y, int w, int h);
    int Destroy();

	DWORD GetPos();
	void Refresh();
	DWORD SetPos(DWORD dwNew);
	DWORD GetMax();
	DWORD GetMin();
	DWORD SetMax(DWORD dwNew);
	DWORD SetMin(DWORD dwMin);
	static void CALLBACK DefaultProc(PVOID pUser, CCProgressBar* pBar, PWSTR Text);

private:

	HBRUSH hBackground;
	HBRUSH hComplete;//the color of the completed portion.
	HFONT hFont;

	DWORD dwPos;
	DWORD dwMin;
	DWORD dwMax;

	HWND hParent;
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HWND hWnd;

	PROGRESSBARPROC pProc;
	LPVOID pUserData;
	WCHAR OldText[1024];

	void OnPaint();

	void CorrectBounds();
};


