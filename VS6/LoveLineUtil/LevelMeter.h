/*
    Create();
    SetFormat();
    Unpause();
*/


#pragma once


#include <windows.h>
#include "mmtimer.h"
#include "AudioLevels.h"
#include "buffer.h"


// Events are received in Thread B, so synchronize that buffer
class CLevelMeterEvents
{
public:
    virtual bool LM_SetExtents(long lmin, long lmax) = 0;
    virtual bool LM_SetValue(long pos) = 0;
};


/*
    Specialized class to handle audio buffers.  It's thread safe.

    NewData() always resets the buffer and the next read will start at the beginning
    of the buffer.
*/
class CCEXPORT CLevelMeterBuffer
{
public:
    CLevelMeterBuffer();
    ~CLevelMeterBuffer();

    void SetGranularity(size_t n);// returned lenghts are always rounded down to this granularity.

    void NewData(const BYTE* p, size_t dwSize);
    BYTE* Lock(size_t* dwSize);// dwSize is OUT - EACH CALL TO THIS **MUST** BE FOLLOWED BY A MATCHING SINGLE CALL TO UNLOCK()
    void Unlock();

private:
    size_t m_nGranularity;
    CBuffer<BYTE> m_buf;
    size_t m_nCursor;
    size_t m_nBuf;// amount of data we have stuck in the buffer.
    CRITICAL_SECTION m_cs;
};


class CCEXPORT CLevelMeter : public CMMTimerCallback
{
public:
    CLevelMeter();
    ~CLevelMeter();

    bool Create(CLevelMeterEvents* pEvents, WAVEFORMATEX& pwfx);
    bool Destroy();

    bool Unpause();
    bool Pause();

    /*
        use this to send the most updated waveform data.  We will
        copy the data to our own little structure and start feeding off it.
    */
    bool NewData(const BYTE* pData, size_t dwSize);

    // Methods of CMMTimerCallback
    void OnTime(class CMMTimer* pTimer, void* pUser);

private:
    CLevelMeterEvents* m_pCallback;
    CAudioLevels m_l;
    CMMTimer m_t;

    CLevelMeterBuffer m_buf;

    DWORD m_dwLastTick;
};


