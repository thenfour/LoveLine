

#include "CCListBox.h"
#include <stdio.h>

void LIST_COLUMNORDER::Set(int nIndex, int nSize)
{
    this->nIndex = nIndex;
    this->nSize = nSize;
}


//////////////////////////////////////////////////////////////////////////////
int CCListBox::GetSelectionMark()
{
    return ListView_GetSelectionMark(this->hWnd);
}


//////////////////////////////////////////////////////////////////////////////
LPARAM CCListBox::GetItemLParam(int iItem)
{
    LVITEMW lvi = {0};
    lvi.mask = LVIF_PARAM;
    lvi.iItem = iItem;
    lvi.iSubItem = 0;
    lvi.lParam = -1;
    ListView_GetItem(this->hWnd, &lvi);

    return lvi.lParam;
}


//////////////////////////////////////////////////////////////////////////////
int CCListBox::FindLParam(LPARAM pCriteria)
{
    int ir;

    LVFINDINFOW lvfi;
    lvfi.flags = LVFI_PARAM;
    lvfi.lParam = pCriteria;

    ir = ListView_FindItem(this->hWnd, -1, &lvfi);

    return ir;
}

//////////////////////////////////////////////////////////////////////////////
HRESULT CCListBox::SelectAnItem(int iItem)
{
    LVITEMW lvi = {0};
    lvi.iItem = iItem;
    lvi.iSubItem = 0;
    lvi.mask = LVIF_STATE;
    lvi.state = LVIS_SELECTED;
    lvi.stateMask = LVIS_SELECTED;

    if(ListView_SetItem(this->hWnd, &lvi) == FALSE)
    {
        return E_FAIL;
    }

    ListView_SetSelectionMark(this->hWnd, iItem);

    return S_OK;
}


HRESULT CCListBox::SetSmallImageList(CCImageList* pil)
{
    SendMessage(this->hWnd, LVM_SETIMAGELIST, LVSIL_SMALL, (LPARAM)pil->GethImageList());
    this->pSmall = pil;

    return S_OK;
}


HRESULT CCListBox::SetBigImageList(CCImageList* pil)
{
    SendMessage(this->hWnd, LVM_SETIMAGELIST, LVSIL_NORMAL, (LPARAM)pil->GethImageList());
    this->pSmall = pil;

    return S_OK;
}


HRESULT CCListBox::SetStateImageList(CCImageList* pil)
{
    SendMessage(this->hWnd, LVM_SETIMAGELIST, LVSIL_STATE, (LPARAM)pil->GethImageList());
    this->pSmall = pil;

    return S_OK;
}


//////////////////////////////////////////////////////////////////////////////
int CCListBox::GetItemCount()
{
	return SendMessage(this->hWnd, LVM_GETITEMCOUNT, 0, 0);
}


//////////////////////////////////////////////////////////////////////////////
HRESULT CCListBox::GetItemText(int iItem, int iColumn, CCString* pOut)
{
    int nLen;

    LV_ITEM lvi;
    lvi.iSubItem = iColumn;
    lvi.cchTextMax = 1024;
    lvi.pszText = pOut->GetBuffer(1024);
    
    nLen = SendMessage(this->hWnd, LVM_GETITEMTEXT, iItem, (LPARAM)&lvi);

    //lvi.pszText = pOut->GetBuffer(nLen);
    //lvi.cchTextMax = nLen;

    //ListView_GetItemText(this->hWnd, iItem, iColumn, pOut->GetBuffer(nLen + 1), nLen + 1);

    return S_OK;
}


//////////////////////////////////////////////////////////////////////////////
int CCListBox::GetColumnCount()
{
    int r = 0;
    LVCOLUMNW col;
    col.mask = LVCF_WIDTH;

    while(ListView_GetColumn(this->hWnd, r, &col) != FALSE)
    {
        r++;
    }

    return r;
}




//////////////////////////////////////////////////////////////////////////////
HRESULT CCListBox::GetColumnText(int iCol, CCString* pOut)
{
    LVCOLUMNW col;
    col.mask = LVCF_TEXT;
    col.pszText = pOut->GetBuffer(MAX_PATH);
    col.cchTextMax = MAX_PATH;

    if(ListView_GetColumn(this->hWnd, iCol, &col) == FALSE)
    {
        return E_FAIL;
    }

    return S_OK;
}



//////////////////////////////////////////////////////////////////////////////
HRESULT CCListBox::SizeColumns()
{
    int nCol = 0;

    while(
        ListView_SetColumnWidth(this->hWnd,
        nCol++,
        LVSCW_AUTOSIZE_USEHEADER)
        == TRUE
        );

    return S_OK;
}



//////////////////////////////////////////////////////////////////////////////
void LIST_COLUMN::Set(PCWSTR wszTitle, DWORD Orientation, int Size)
{
	wcscpy(this->Title, wszTitle);
	this->Orientation = Orientation;
	this->Size = Size;
}



//////////////////////////////////////////////////////////////////////////////
CCListBox::CCListBox()
{
    this->nLastColumnClicked = -1;
    this->pSmall = NULL;
    this->pBig = NULL;
    this->pState = NULL;
	this->nColumns = 0;
}



//////////////////////////////////////////////////////////////////////////////
CCListBox::~CCListBox()
{
}


//////////////////////////////////////////////////////////////////////////////
HRESULT CCListBox::Create(HWND hParent, HINSTANCE hInstance, RECT prect, RECT orect)
{
	CopyRect(&this->orect, &orect);
	CopyRect(&this->prect, &prect);
    this->hInstance = hInstance;
	this->hParent = hParent;
	this->dwStyle = WS_CHILD | LVS_SHOWSELALWAYS | WS_TABSTOP | 
		LVS_REPORT;

	this->hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, 
        WC_LISTVIEWW, L"x", this->dwStyle,
		0, 0, 10, 10, 
		hParent, (HMENU)this->GetId(), hInstance, 0);

	ListView_SetExtendedListViewStyle(this->hWnd, LVS_EX_FULLROWSELECT);

	//this->Size();
    this->nLastColumnClicked = -1;

    this->AddStyle(WS_VISIBLE);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////////
HRESULT CCListBox::AddItem(int* pId, LPARAM lParam, int nImage)
{
	LVITEM lvitem;
	int i;

    ZeroMemory(&lvitem, sizeof(lvitem));
	lvitem.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;
    lvitem.iItem = MAXLONG;//if there are more than this, who cares if it's out of order.
    lvitem.iSubItem = 0; 
    lvitem.state = 0; 
    lvitem.stateMask = 0; 
    lvitem.pszText = 0;
    lvitem.cchTextMax = 0;
    lvitem.iImage = nImage;
    lvitem.lParam = lParam;

	i = ListView_InsertItem(this->GethWnd(), &lvitem);

	if(pId) *pId = i;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////////
HRESULT CCListBox::AddItemText(int nId, int nColumn, CCString Label)
{
	ListView_SetItemText(this->GethWnd(), nId, nColumn, (PWSTR)(PCWSTR)Label);
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////////
HRESULT CCListBox::Clear()
{
    //this->nLastColumnClicked = -1;
	ListView_DeleteAllItems(this->GethWnd());
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////////
HRESULT CCListBox::ShowHeader(int nParts, LIST_COLUMN * Strings)
{
	LVCOLUMNW col;
	col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	this->nColumns = nParts;

	int i;
	for (i = 0 ; i <nParts; i++)
    {
		col.pszText = Strings[i].Title;
		col.cx=Strings[i].Size;
		col.fmt = Strings[i].Orientation;
		col.iSubItem = i;

		ListView_InsertColumn(this->GethWnd(), i, &col);
	}

    // Delete all columns to the right of these, just to make sure we're done.
	while(ListView_DeleteColumn(this->hWnd, i) == TRUE)
    {
	}

	return 0;
}


int CCListBox::GetItemState(int iItem)
{
    int nState;

    nState = SendMessage(this->hWnd, LVM_GETITEMSTATE, iItem, LVIS_STATEIMAGEMASK);

    return (nState >> 12);
}


HRESULT CCListBox::SetItemState(int iItem, int nState)
{
    LVITEM lvi;
    lvi.stateMask = LVIS_STATEIMAGEMASK;
    lvi.state = (nState & 15) << 12;

    SendMessage(this->hWnd, LVM_SETITEMSTATE, iItem, (LPARAM)&lvi);

    return S_OK;
}


void CCListBox::SetExtendedListViewStyle(DWORD dw)
{
    SendMessage(this->hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dw);
}


DWORD CCListBox::GetExtendedListViewStyle()
{
    return SendMessage(this->hWnd, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
}


NMLISTVIEW* CCListBox::IsSelectedNotify(LPARAM lParam)
{
    NMHDR* pnmh = (NMHDR*)lParam;
    NMLISTVIEW* plv = NULL;

    if(pnmh->code == LVN_ITEMCHANGED)
    {
        if(pnmh->idFrom == this->id)
        {
            // Now determine if the change was a state change.
            plv = (NMLISTVIEW*)lParam;

            if(plv->uChanged & LVIF_STATE)
            {
                // The state changed.
                if(plv->uNewState & LVIS_SELECTED)
                {
                    return plv;
                }
            }
        }
    }

    return NULL;
}


NMITEMACTIVATE* CCListBox::IsActivatedNotify(LPARAM lParam)
{
    NMHDR* pnmh = (NMHDR*)lParam;
    NMITEMACTIVATE* plv = NULL;

    if(pnmh->code == LVN_ITEMACTIVATE)
    {
        if(pnmh->idFrom == this->id)
        {
            // Now determine if the change was a state change.
            return (NMITEMACTIVATE*)lParam;
        }
    }

    return NULL;
}


int CCListBox::IsSortNotify(LPARAM lParam)
{
    NMHDR* pnmh = (NMHDR*)lParam;
    NMLISTVIEW* plv = NULL;

    if(pnmh->code == LVN_COLUMNCLICK)
    {
        if(pnmh->idFrom == this->id)
        {
            plv = (NMLISTVIEW*)lParam;
            this->nLastColumnClicked = plv->iSubItem;
            return plv->iSubItem;
        }
    }

    return -1;
}


void CCListBox::SortById(PVOID pUser, PFNLVCOMPARE pfnCompare)
{
    ListView_SortItemsEx(this->hWnd, pfnCompare, pUser);
}


void CCListBox::SortByLParam(PVOID pUser, PFNLVCOMPARE pfnCompare)
{
    ListView_SortItems(this->hWnd, pfnCompare, pUser);
}


void CCListBox::SortTextAutomatically(int iCol)
{
    this->iCol = iCol;
    ListView_SortItemsEx(this->hWnd, CCListBox::TextCompareFunc, this);
}

int CALLBACK CCListBox::TextCompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    CCListBox* pThis = (CCListBox*)lParamSort;
    CCString Text1, Text2;

    pThis->GetItemText(lParam1, pThis->iCol, &Text1);
    pThis->GetItemText(lParam2, pThis->iCol, &Text2);

    return Text1.cmp(Text2);
}


int CCListBox::SetSelectionMark(int i)
{
    return ListView_SetSelectionMark(this->hWnd, i);
}


void CCListBox::SimulateColumnClick(int iColumn)
{
    if(iColumn == -1) return;

    NMLISTVIEW nmlv;
    nmlv.hdr.code = LVN_COLUMNCLICK;
    nmlv.hdr.hwndFrom = this->hWnd;
    nmlv.hdr.idFrom = this->id;

    nmlv.iItem = -1;
    nmlv.iSubItem = iColumn;
    nmlv.lParam = 0;
    nmlv.ptAction.x = 0;
    nmlv.ptAction.y = 0;
    nmlv.uChanged = 0;
    nmlv.uNewState = 0;
    nmlv.uOldState = 0;

    SendMessage(this->hParent, WM_NOTIFY, this->id, (LPARAM)&nmlv);
}


int CCListBox::GetLastColumnClicked()
{
    return this->nLastColumnClicked;
}


int CCListBox::GetColumnIndexesAndSizes(LIST_COLUMNORDER* p)
{
    int nColumns = this->GetColumnCount();
    int i;
    int* pOrder = NULL;

    // Get column orders.
    pOrder = new int[nColumns];
    if(pOrder == 0) return -1;

    ListView_GetColumnOrderArray(this->hWnd, nColumns, pOrder);

    // Now get the column sizes and stick ALL the info in the outgoing struct
    for(i=0;i<nColumns;i++)
    {
        if(p) p[i].Set(pOrder[i], ListView_GetColumnWidth(this->hWnd, i));
    }

    delete [] pOrder;

    return nColumns;
}


int CCListBox::SetColumnIndexesAndSizes(LIST_COLUMNORDER* p)
{
    int nColumns = this->GetColumnCount();
    int i;
    int* pOrder = NULL;

    if(p == 0) return -1;

    // Set up the order array - this will be filled in during the sizing loop
    // and later put into action.
    pOrder = new int[nColumns];
    if(pOrder == 0) return -1;

    for(i=0;i<nColumns;i++)
    {
        ListView_SetColumnWidth(this->hWnd, i, p[i].nSize);
        pOrder[i] = p[i].nIndex;
    }

    ListView_SetColumnOrderArray(this->hWnd, nColumns, pOrder);

    delete [] pOrder;

    return nColumns;
}


