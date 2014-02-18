

#pragma once


#include <windows.h>
#include <mmsystem.h>


// Equiv. to the timerproc
class CMMTimerCallback
{
public:
    virtual void OnTime(class CMMTimer* pTimer, void* pUser) = 0;
};





class CCEXPORT CMMTimer
{
public:
    CMMTimer();
    ~CMMTimer();

    bool Create(CMMTimerCallback* pCallback, void* pUser, unsigned long dwMS, bool bInitiallyRunning);
    bool Destroy();
    void Pause();
    void Unpause();

private:
    MMRESULT m_hTimer;
    void* m_pUser;
    CMMTimerCallback* m_pCallback;
    UINT m_Resolution;
    bool m_bPaused;

    static void CALLBACK TimeProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

};

