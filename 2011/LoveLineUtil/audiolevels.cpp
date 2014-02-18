

#include "pch.h"
#include "Global.h"
#include "audiolevels.h"


CAudioLevels::CAudioLevels() :
    m_min(0),
    m_max(100),
    m_nAvgBytesPerMillisecond(0),
    m_natural_max(0)
{
}


CAudioLevels::~CAudioLevels()
{
}


void CAudioLevels::SetFormat(WAVEFORMATEX& x)
{
    RtlCopyMemory(&m_wfx, &x, sizeof(x));
    m_nAvgBytesPerMillisecond = ((float)m_wfx.nAvgBytesPerSec) / 1000;
    m_natural_max = ((1 << m_wfx.wBitsPerSample) / 2)-1;
}


void CAudioLevels::SetReturnExtents(long minimum, long maximum)
{
    m_min = minimum;
    m_max = maximum;
}


void CAudioLevels::GetLevel(BYTE* pData, DWORD dwSize, long* pLevel)
{
    signed __int32 nNaturalLevel = 0;

    switch(m_wfx.wBitsPerSample)
    {
    case 8:
        {
            nNaturalLevel = _GetLevel<signed __int8>((signed __int8*)pData, dwSize, 0x7F, true);
            break;
        }
    case 16:
        {
            //nNaturalLevel = _GetLevel<signed __int16>((signed __int16*)pData, dwSize/2, 0x7FFF, false);
            nNaturalLevel = _GetLevel<signed __int16>((signed __int16*)pData, 16, 0x7FFF, false);
            break;
        }
    case 32:
        {
            nNaturalLevel = _GetLevel<signed __int32>((signed __int32*)pData, dwSize/4, 0x7FFFFFFF, false);
            break;
        }
    }

    // convert natural level to plevel.
    long lRange = m_max - m_min;
    *pLevel = MulDiv(nNaturalLevel, lRange, m_natural_max);// plevel is now a value proportional to the range.
    *pLevel += m_min;

    if(*pLevel < 0)
    {
        int a = 0;
    }

    return;
}


void CAudioLevels::SetReturnExtents()
{
    m_min = 0;
    m_max = m_natural_max;
}


void CAudioLevels::GetReturnExtents(long* pmin, long* pmax)
{
    if(pmin) *pmin = m_min;
    if(pmax) *pmax = m_max;
}


size_t CAudioLevels::MillisecondsToBytes(DWORD dwMS)
{
    return (size_t)(m_nAvgBytesPerMillisecond * dwMS);
}


