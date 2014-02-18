

#include "pch.h"
#include "global.h"
#include "LevelMeter.h"


#define LM_DELAY 50// milliseconds


CLevelMeter::CLevelMeter() :
    m_pCallback(0),
    m_dwLastTick(0)
{
}


CLevelMeter::~CLevelMeter()
{
    Destroy();
}


bool CLevelMeter::Create(CLevelMeterEvents* pCallback, WAVEFORMATEX& pwfx)
{
    bool r = false;
    long lmin, lmax;

    if(m_t.Create(this, 0, 100, false))
    {
        m_pCallback = pCallback;
        m_l.SetFormat(pwfx);
        m_l.SetReturnExtents();
        m_l.GetReturnExtents(&lmin, &lmax);
        m_pCallback->LM_SetExtents(lmin, lmax);

        m_buf.SetGranularity(pwfx.nBlockAlign);

        r = true;
    }

    return r;
}


bool CLevelMeter::Destroy()
{
    m_t.Destroy();
    return true;
}


bool CLevelMeter::Unpause()
{
    m_dwLastTick = 0;
    m_t.Unpause();
    return true;
}


bool CLevelMeter::Pause()
{
    m_t.Pause();
    return true;
}

void CLevelMeter::OnTime(class CMMTimer* pTimer, void* pUser)
{
    size_t nMSElapsed = 0;
    size_t nSize = 0;
    BYTE* pData = 0;
    long lLevel = 0;

    if(m_dwLastTick)
    {
        nMSElapsed = GetTickCount() - m_dwLastTick;
        nSize = m_l.MillisecondsToBytes(nMSElapsed);

        pData = m_buf.Lock(&nSize);
        if(pData)
        {

            // analyze it
            m_l.GetLevel(pData, nSize, &lLevel);

            // send callback
            m_pCallback->LM_SetValue(lLevel);
        }

        m_buf.Unlock();
    }

    m_dwLastTick = GetTickCount();

    long pos = 0;
}


bool CLevelMeter::NewData(const BYTE* pData, size_t dwSize)
{
    m_buf.NewData(pData, dwSize);
    return true;
}


//////////////////////////////////////////////////////////////////////////////


CLevelMeterBuffer::CLevelMeterBuffer()
{
    m_nGranularity = 1;
    m_nCursor = 0;
    m_nBuf = 0;
    InitializeCriticalSection(&m_cs);
}


CLevelMeterBuffer::~CLevelMeterBuffer()
{
    DeleteCriticalSection(&m_cs);
}


void CLevelMeterBuffer::SetGranularity(size_t n)
{
    m_nGranularity = n;
}


void CLevelMeterBuffer::NewData(const BYTE* p, size_t dwSize)
{
    EnterCriticalSection(&m_cs);

    if(m_buf.Realloc(dwSize))
    {
        if(m_buf.Lock())
        {
            memset(m_buf.locked_value, 0xcb, m_buf.GetSize());

            memcpy(m_buf.locked_value, p, dwSize);
            m_nCursor = 0;
            m_nBuf = dwSize;

            m_buf.Unlock();
        }
    }

    LeaveCriticalSection(&m_cs);
}


// read FROM the cursor TO either:
// 1) end of the buffer
// 2) dwSize
// whichever is less.
BYTE* CLevelMeterBuffer::Lock(size_t* dwSize)
{
    BYTE* r = 0;

    EnterCriticalSection(&m_cs);

    if(m_nCursor >= m_nBuf)
    {
        // cannot read past end of buffer.
        *dwSize = 0;
        r = 0;
    }
    else
    {
        if(m_buf.Lock())
        {
            size_t nAmountToRead = min(*dwSize, m_nBuf-m_nCursor);
            nAmountToRead = GranulizeRoundDown<size_t>(nAmountToRead, m_nGranularity);
            r = m_buf.locked_value + m_nCursor;
            m_nCursor += nAmountToRead;
            *dwSize = nAmountToRead;
        }
    }

    return r;
}


void CLevelMeterBuffer::Unlock()
{
    m_buf.Unlock();
    LeaveCriticalSection(&m_cs);
}


