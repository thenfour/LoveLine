/*
    SetFormat();
    SetReturnExtents()
    ...
    GetLevel();
*/

#pragma once


#include <windows.h>


class CCEXPORT CAudioLevels
{
public:
    CAudioLevels();
    ~CAudioLevels();

    void SetFormat(WAVEFORMATEX& x);
    void SetReturnExtents(long minimum, long maximum);
    void SetReturnExtents();// sets to the default - the most optimized based on the format.
    void GetReturnExtents(long* pmin, long* pmax);

    void GetLevel(BYTE* pData, DWORD dwSize, long* pLevel);

    size_t MillisecondsToBytes(DWORD dwMS);

private:
    WAVEFORMATEX m_wfx;
    long m_min;
    long m_max;

    long m_natural_max;// the natural maximum value of data.

    // calculated from m_wfx on SetFormat() for optimization
    float m_nAvgBytesPerMillisecond;

    template<typename T>
    T _GetLevel(T* pData, size_t nElements, T nMax, bool bShift)
    {
        T r = 0;

        //odsf(L"_GetLevel()", L"{%04d, %04d, %04d, %04d}", (int)pData[0], (int)pData[1], (int)pData[2], (int)pData[3]);

        while(nElements)
        {
            T el = *pData;
            if(bShift) el += nMax;
            el = el < 0 ? -el : el;// absolute value
            // if it's still negative, it's because we can't make it positive. (like -128 in a signed 8-bit).
            if(el < 0)
            {
                return nMax;
            }
            if(el > r) r = el;
            pData++;
            nElements --;
        }

        return r;
    }
};



