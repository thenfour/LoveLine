

#include "LLNavigation.h"


LLNavigation::LLNavigation()
{
    this->nCurrentPos = 0;
    this->pBack = 0;
    this->pForward = 0;
}


LLNavigation::~LLNavigation()
{
    // Make sure the array is deleted
}


HRESULT LLNavigation::SaveSettings(LLDatabase* pdb)
{
    return S_OK;
}


HRESULT LLNavigation::LoadSettings(LLDatabase* pdb)
{
    return S_OK;
}


/*
    This one is a little bit complicated.  If the current position isn't
    position ZERO, then we have to shift everything up and purge all those
    other forward positions.
*/
HRESULT LLNavigation::NavigateTo(DWORD dw, GUID id)
{
    if(dw == this->Positions[this->nCurrentPos].dwPos)
    {
        if(id == this->Positions[this->nCurrentPos].idEpisode)
        {
            return S_OK;
        }
    }

    if(this->nPositions == 0)
    {
        this->nPositions = 1;
        this->Positions[0].dwPos = dw;
        this->Positions[0].idEpisode = id;
        this->nCurrentPos = 0;
        this->_UpdateButtons();
        return S_OK;
    }

    this->nPositions = this->nPositions - this->nCurrentPos + 1;

    if(this->nPositions == MAX_NAV_SIZE)
    {
        this->nPositions --;
    }

    memmove(
        &this->Positions[1], 
        &this->Positions[this->nCurrentPos],
        (this->nPositions-1) * sizeof(LLNavPos));

    this->nCurrentPos = 0;
    this->Positions[0].dwPos = dw;
    this->Positions[0].idEpisode = id;

    this->_UpdateButtons();

    return S_OK;
}


HRESULT LLNavigation::NavigateBack(DWORD* pdw, GUID* pid)
{
    if(this->CanNavigateBackward() == FALSE) return E_FAIL;

    this->nCurrentPos ++;
    if(pdw) *pdw = this->Positions[this->nCurrentPos].dwPos;
    if(pid) *pid = this->Positions[this->nCurrentPos].idEpisode;

    this->_UpdateButtons();

    return S_OK;
}


HRESULT LLNavigation::NavigateForward(DWORD* pdw, GUID* pid)
{
    if(this->CanNavigateForward() == FALSE) return E_FAIL;

    this->nCurrentPos --;
    if(pdw) *pdw = this->Positions[this->nCurrentPos].dwPos;
    if(pid) *pid = this->Positions[this->nCurrentPos].idEpisode;

    this->_UpdateButtons();

    return S_OK;
}


HRESULT LLNavigation::SetButtons(CCButton* pBack, CCButton* pForward)
{
    if(pBack) this->pBack = pBack;
    if(pForward) this->pForward = pForward;
    this->_UpdateButtons();
    return S_OK;
}


BOOL LLNavigation::CanNavigateForward()
{
    if(this->nCurrentPos == 0)
    {
        return FALSE;
    }

    return TRUE;
}


BOOL LLNavigation::CanNavigateBackward()
{
    if(this->nCurrentPos >= this->nPositions-1)
    {
        return FALSE;
    }

    return TRUE;
}


HRESULT LLNavigation::_UpdateButtons()
{
    if(this->pBack)
    {
        this->pBack->SetActive(this->CanNavigateBackward() == TRUE ? TRUE : FALSE);
    }

    if(this->pForward)
    {
        this->pForward->SetActive(this->CanNavigateForward() == TRUE ? TRUE : FALSE);
    }

    return S_OK;
}




