

#include "pch.h"
#include "CAudioReaderBuffer.h"


#define BUFFER_GRANULARITY 128000


CAudioReaderBuffer::CAudioReaderBuffer()
{
    InitializeCriticalSection(&m_cs);
    this->pBuffer = NULL;
    this->nBufferLen = 0;
    this->nGoodPortion = 0;
}


CAudioReaderBuffer::~CAudioReaderBuffer()
{
    if(this->pBuffer)
    {
        free(this->pBuffer);
    }

    DeleteCriticalSection(&m_cs);

    this->pBuffer = NULL;
    this->nBufferLen = 0;
    this->nGoodPortion = 0;
}


HRESULT CAudioReaderBuffer::Clear()
{
    this->nGoodPortion = 0;
    return S_OK;
}


int CAudioReaderBuffer::GetLength()
{
    return this->nGoodPortion;
}


PBYTE CAudioReaderBuffer::LockBuffer()
{
    EnterCriticalSection(&m_cs);
    return this->pBuffer;
}

void CAudioReaderBuffer::UnlockBuffer()
{
    LeaveCriticalSection(&m_cs);
}


HRESULT CAudioReaderBuffer::AddBytes(PBYTE pBuffer, int nSize)
{
    LockBuffer();

    // Reallocate the buffer if necessary.
    int nNewUsedSize = this->nGoodPortion + nSize;

    if(nNewUsedSize > this->nBufferLen)
    {
        this->nBufferLen = ((nNewUsedSize + BUFFER_GRANULARITY) / BUFFER_GRANULARITY) * BUFFER_GRANULARITY;
        this->pBuffer = (PBYTE)realloc(this->pBuffer, this->nBufferLen);

        if(this->pBuffer == NULL)
        {
            this->nBufferLen = 0;
            UnlockBuffer();
            return E_FAIL;
        }
    }

    // Now the buffer is guaranteed to be large enough.
    memcpy(this->pBuffer + this->nGoodPortion, pBuffer, nSize);
    this->nGoodPortion += nSize;

    UnlockBuffer();

    return S_OK;
}




