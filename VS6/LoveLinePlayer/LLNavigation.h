

#pragma once


#include <Windows.h>
#include "..\\LoveLineUtil\\CCButton.h"
#include "..\\LoveLineUtil\\Global.h"
#include "..\\LoveLineUtil\\LLDatabase.h"


#define MAX_NAV_SIZE 300


struct LLNavPos
{
    DWORD dwPos;
    GUID idEpisode;
};


class LLNavigation
{
public:

    LLNavigation();
    ~LLNavigation();

    HRESULT SaveSettings(LLDatabase* pdb);
    HRESULT LoadSettings(LLDatabase* pdb);

    HRESULT NavigateTo(DWORD dw, GUID id);
    HRESULT NavigateBack(DWORD* pdw, GUID* pid);
    HRESULT NavigateForward(DWORD* pdw, GUID* pid);

    /*
        This will just control whether they're disabled or not.
    */
    HRESULT SetButtons(CCButton* pBack, CCButton* pForward);

    BOOL CanNavigateForward();
    BOOL CanNavigateBackward();

private:

    HRESULT _UpdateButtons();

    CCButton* pBack;
    CCButton* pForward;

    int nCurrentPos;// Index of current position.  The latest position is 0.
    //The earliest is nPositions - 1.
    int nPositions;// How many positions are valid.

    LLNavPos Positions[MAX_NAV_SIZE];
};





