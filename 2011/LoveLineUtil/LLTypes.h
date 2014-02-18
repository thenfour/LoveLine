

#pragma once


#include <windows.h>


/*
    LLTIME stuff.

+-----------------------------+----------------+--------------+
|  Year                       |  Month         | Day          |
+-----------------------------+----------------+--------------+

    Year  16 bits
    Month  8 bits
    Day    8 bits

*/
typedef DWORD LLTIME;

#define MAKELLTIME(year, month, day) \
    ((LLTIME)(((year) << 16) | ((month) << 8) | (day) ))

#define YEARFROMLLTIME(lltime) ((WORD)((lltime) & 0xFFFF0000 >> 16))
#define MONTHFROMLLTIME(lltime) ((BYTE)((lltime) & 0x0000FF00 >> 8))
#define DAYFROMLLTIME(lltime) ((BYTE)((lltime) & 0x000000FF))
