

#include "pch.h"
#include "PluginContainer.h"
#include <stdio.h>


void PluginContainer::SetChild(HWND hWnd)
{
    this->hChild = hWnd;
}


HWND PluginContainer::GetChild()
{
    return this->hChild;
}


void PluginContainer::Show(BOOL b)
{
    if(b == TRUE)
    {
        ShowWindow(this->hWnd, SW_SHOWNORMAL);
        ShowWindow(this->hChild, SW_SHOWNORMAL);
    }
    else
    {
        ShowWindow(this->hWnd, SW_HIDE);
        ShowWindow(this->hChild, SW_HIDE);
    }
}


//////////////////////////////////////////////////////////////////////////////
PluginContainer::~PluginContainer()
{
}

//////////////////////////////////////////////////////////////////////////////
PluginContainer::PluginContainer()
{
    this->hChild = 0;
	this->wszClass[0]=0;
}

//////////////////////////////////////////////////////////////////////////////
HRESULT PluginContainer::Destroy()
{
	DestroyWindow(this->hWnd);
	this->hWnd = 0;

	UnregisterClassW(this->wszClass, this->hInstance);
	this->wszClass[0]=0;

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
HRESULT PluginContainer::Create(HWND hParent, HINSTANCE hInstance, RECT prect, RECT orect)
{
	WNDCLASSW wc;

    this->hInstance  = hInstance;

	CopyRect(&this->orect, &orect);
	CopyRect(&this->prect, &prect);

	this->hParent = hParent;
	this->dwStyle = WS_CHILD;

 	swprintf(this->wszClass, L"PluginContainer 0x%X", this->GetId());

	wc.cbClsExtra= 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = NULL;
	wc.hInstance = hInstance;
    wc.lpfnWndProc = PluginContainer::ContainerWndProc;
	wc.lpszClassName = this->wszClass;
	wc.lpszMenuName = 0;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	RegisterClassW(&wc);

	this->hWnd = CreateWindowExW(0, this->wszClass, L"", this->dwStyle,
		0, 0, 1, 1,
		(HWND)this->hParent,
		(HMENU)0, hInstance, this);

	return S_OK;
}



LRESULT CALLBACK PluginContainer::ContainerWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PluginContainer* pThis = (PluginContainer*) GetWindowLong(hWnd, GWL_USERDATA);

    switch(uMsg)
    {
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
            return 0;
        }
    case WM_SIZE:
        {
			if(pThis->GetChild())
			{
				MoveWindow(pThis->GetChild(), 0, 0, 
					LOWORD(lParam), HIWORD(lParam), TRUE);
			}
            return 0;
        }
    case WM_CREATE:
        {
            CREATESTRUCT * pcs = (CREATESTRUCT*)lParam;
            pThis = (PluginContainer*)pcs->lpCreateParams;
            SetWindowLong(hWnd, GWL_USERDATA, (LONG)pThis);
            return 0;
        }
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}




