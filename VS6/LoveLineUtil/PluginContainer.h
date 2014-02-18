

#pragma once


#include "CCControl.h"


//////////////////////////////////////////////////////////////////////////////
class CCEXPORT PluginContainer : public CCControl
{
public:
	PluginContainer();
	~PluginContainer();

    HRESULT Create(HWND hParent, HINSTANCE hInstance, RECT prect, RECT orect);

    void SetChild(HWND hWnd);
    HWND GetChild();

    void Show(BOOL);
	HRESULT Destroy();

private:

    static LRESULT CALLBACK ContainerWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    HWND hChild;// There will be a single child window in a container.
	WCHAR wszClass[MAX_PATH];
};


