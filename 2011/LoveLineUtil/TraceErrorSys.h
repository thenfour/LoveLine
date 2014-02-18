

#pragma once


#include "CCString.h"
#include <windows.h>

#ifndef CCEXPORT
#define CCEXPORT __declspec(dllexport)
#endif


/*
    This function will stick the error text in the buffer.

    Return value is the size of the buffer needed.  szBuf
    can be NULL if needed.
*/
void CCEXPORT TraceErrorSys(CCString* pBuf, HRESULT hr);

void CCEXPORT odssys(CCString Function, CCString Header, HRESULT hr);

