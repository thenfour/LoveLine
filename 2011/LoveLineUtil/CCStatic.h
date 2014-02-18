

#pragma once


#include "CCControl.h"
#include "Global.h"
#include "CCString.h"


class CCEXPORT CCStatic : public CCControl
{
public:

    HRESULT Create(CCString Text, HWND hParent, HINSTANCE hInstance, RECT prect, RECT orect);

};



