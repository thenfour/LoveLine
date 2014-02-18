

#pragma once


#include <windows.h>
#include <stdio.h>


#ifndef CCEXPORT
    #define CCEXPORT __declspec(dllexport)
#endif


#ifndef SAFE_RELEASE
    #define SAFE_RELEASE(a)             { if(a) (a)->Release(); (a) = 0;}
#endif


/*

    If we're building FREE, then forget about the debug stuff.  #define it out.

    odlf(), MsgBoxf(), and odsf() all take a variable number of arguments, which
    is why I've disabled that error C4001 in global.h.

*/
#ifdef NDEBUG

    #define MsgBoxf(a)
    #define odsf(a)

#else


/*
	MessageBox Formatted
*/
void CCEXPORT __cdecl MsgBoxf(PCWSTR wszString, ...);


/*
	OutputDebugString FORMATTED (should have been included with everything)
*/
void CCEXPORT __cdecl odsf(PCWSTR wszFunction, PCWSTR wszString, ...);
void CCEXPORT _odsf(PCWSTR wszFunction, PCWSTR wszString, va_list argptr);


/*
	OutputDebugString RECTANGLE - this function just makes it easy
	to output the dimentions of a rectangle in the format:
	
	rcHwnd = (34,23)-(23,25)
*/
void CCEXPORT odsr(PCWSTR wszFunction, PCWSTR wszString, PRECT prc);


#endif//ifdef NDEBUG

