

#pragma once


#include <windows.h>
#include "global.h"


class CCEXPORT CAudioReaderBuffer
{
public:
    CAudioReaderBuffer();
    ~CAudioReaderBuffer();

    int GetLength();
    PBYTE LockBuffer();
    void UnlockBuffer();

    HRESULT AddBytes(PBYTE pBuffer, int nSize);
    HRESULT Clear();

private:

    CRITICAL_SECTION m_cs;

    int nGoodPortion;// Size of USED portion of the buffer.

    int nBufferLen;
    PBYTE pBuffer;

};




