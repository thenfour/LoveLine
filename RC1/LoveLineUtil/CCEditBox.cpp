

#include "CCEditBox.h"


CCEditBox::CCEditBox()
{
}


CCEditBox::~CCEditBox()
{
}


HRESULT CCEditBox::CreateMultiline(HWND hParent, HINSTANCE, RECT prect, RECT orect)
{
    CopyRect(&this->prect, &prect);
    CopyRect(&this->orect, &orect);
    this->hParent = hParent;
    this->hInstance = hInstance;

    this->dwStyle = WS_CHILDWINDOW | 
        ES_READONLY | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | 
        WS_VSCROLL | WS_HSCROLL;
    DWORD dwExStyle = WS_EX_CLIENTEDGE;

    this->hWnd = CreateWindowExW(dwExStyle, 
        L"EDIT", L"", 
        this->dwStyle, 0, 0, 1, 1, hParent, (HMENU)this->id,
        hInstance, NULL);

    //this->Size();
    this->SetFont(0, 0);

    this->AddStyle(WS_VISIBLE);

    return S_OK;
}


HRESULT CCEditBox::Create(HWND hParent, HINSTANCE hInstance, RECT prect, RECT orect)
{
    CopyRect(&this->prect, &prect);
    CopyRect(&this->orect, &orect);
    this->hParent = hParent;
    this->hInstance = hInstance;

    this->dwStyle = WS_CHILDWINDOW | ES_READONLY;
    DWORD dwExStyle = WS_EX_CLIENTEDGE;

    this->hWnd = CreateWindowExW(dwExStyle, 
        L"EDIT", L"", 
        this->dwStyle, 0, 0, 1, 1, hParent, (HMENU)this->id,
        hInstance, NULL);

    this->Size();
    this->SetFont(0, 0);

    this->AddStyle(WS_VISIBLE);

    return S_OK;

}


HRESULT CCEditBox::Append(PCWSTR wszText)
{
    int nStart = GetWindowTextLength(this->hWnd);
    SendMessage(this->hWnd, EM_SETSEL, nStart, nStart);
    SendMessage(this->hWnd, EM_REPLACESEL, NULL, (LPARAM)wszText);

    return S_OK;
}


HRESULT CCEditBox::SetText(PCWSTR wsz)
{
    SetWindowTextW(this->hWnd, wsz);
    return S_OK;
}


HRESULT CCEditBox::GetText(PWSTR wsz, DWORD cchMax)
{
    GetWindowTextW(this->hWnd, wsz, cchMax);
    return S_OK;
}


HRESULT CCEditBox::SetMultiline(BOOL b)
{
    DWORD dwStyle = this->GetStyle();

    if(b == TRUE)
    {
        dwStyle |= (ES_WANTRETURN | ES_MULTILINE);
    }
    else
    {
        dwStyle &= ~(ES_WANTRETURN | ES_MULTILINE);
    }

    this->SetStyle(dwStyle);

    return S_OK;
}


HRESULT CCEditBox::SetReadOnly(BOOL b)
{
    SendMessage(this->hWnd, EM_SETREADONLY, b, 0);
    return S_OK;
}


