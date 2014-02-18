

#include "pch.h"
#include "global.h"
#include "mmtimer.h"


CMMTimer::CMMTimer() :
    m_hTimer(0),
    m_pUser(0),
    m_pCallback(0),
    m_Resolution(0),
    m_bPaused(false)
{
}


CMMTimer::~CMMTimer()
{
    Destroy();
}


bool CMMTimer::Create(CMMTimerCallback* pCallback, void* pUser, unsigned long dwMS, bool bInitiallyRunning)
{
    TIMECAPS tc;
    bool r = false;

    m_pCallback = pCallback;
    m_pUser = pUser;

    if(timeGetDevCaps(&tc, sizeof(tc)) == TIMERR_NOERROR) 
    {
        m_Resolution = min(max(tc.wPeriodMin, 1), tc.wPeriodMax);

        if(timeBeginPeriod(m_Resolution) == TIMERR_NOERROR)
        {
            m_bPaused = !bInitiallyRunning;

            m_hTimer = timeSetEvent(dwMS, m_Resolution, CMMTimer::TimeProc,
                reinterpret_cast<DWORD_PTR>(this), TIME_PERIODIC | TIME_CALLBACK_FUNCTION);

            if(m_hTimer != NULL)
            {
                r = true;
            }
        }
    }

    return r;
}


bool CMMTimer::Destroy()
{
    if(m_hTimer)
    {
        timeKillEvent(m_hTimer);
        timeEndPeriod(m_Resolution);
        m_hTimer = 0;
        m_pCallback = 0;
        m_pUser = 0;
        m_bPaused = false;
    }
    return true;
}


void CMMTimer::Pause()
{
    m_bPaused = true;
}


void CMMTimer::Unpause()
{
    m_bPaused = false;
}


void CALLBACK CMMTimer::TimeProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
    if(dwUser)
    {
        CMMTimer* pThis = reinterpret_cast<CMMTimer*>(dwUser);

        if(!pThis->m_bPaused)
        {
            pThis->m_pCallback->OnTime(pThis, pThis->m_pUser);
        }
    }

    return;
}
