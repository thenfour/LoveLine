

#include "pch.h"
#include "CAudioReaderCallback.h"
#include "AudioReader.h"


CAudioReaderCallback::CAudioReaderCallback()
{
    this->pOwner = 0;
}


CAudioReaderCallback::~CAudioReaderCallback()
{
}


HRESULT STDMETHODCALLTYPE CAudioReaderCallback::QueryInterface(REFIID riid, void **ppOut)
{
    if(ppOut == NULL) return E_FAIL;

    if(riid == IID_IUnknown)
    {
        *ppOut = this;
        return S_OK;
    }

    else if(riid == IID_IWMReaderCallback)
    {
        *ppOut = static_cast<IWMReaderCallback*>(this);
        return S_OK;
    }

    else if(riid == IID_IWMReaderCallbackAdvanced)
    {
        *ppOut = static_cast<IWMReaderCallbackAdvanced*>(this);
        return S_OK;
    }

    *ppOut = NULL;
    return E_FAIL;
}


/*
    Because this isn't allocated by COM, the ref count
    has to always be 1.
*/
ULONG STDMETHODCALLTYPE CAudioReaderCallback::AddRef()
{
    return 1;
}


ULONG STDMETHODCALLTYPE CAudioReaderCallback::Release()
{
    return 1;
}


HRESULT CAudioReaderCallback::OnStatus(
    WMT_STATUS Status, HRESULT hr, WMT_ATTR_DATATYPE dwType,
    BYTE* pValue, PVOID pUser)
{
    switch(Status)
    {
    case WMT_END_OF_FILE:
        {
            if(this->pOwner)
            {
                this->pOwner->bAsyncEOF = TRUE;
            }
            return S_OK;
        }
    case WMT_OPENED:
        {
            this->pOwner = (AudioReader*)pUser;
            this->pOwner->hrAsync = hr;
            SetEvent(this->pOwner->hEventOpened);
            return S_OK;
        }
    }

    return S_OK;
}


HRESULT CAudioReaderCallback::OnSample(
    DWORD, QWORD qwTime, QWORD qwLen, DWORD dwFlags,
    INSSBuffer* pSample, PVOID pUser)
{
    HRESULT hr;
    PBYTE pBuffer = NULL;
    DWORD dwBufSize = 0;

    // Set the new cursor.
    this->pOwner->qwCursor = qwTime + qwLen;

    // Get the buffer to the data.
    if(FAILED(hr = pSample->GetBufferAndLength(&pBuffer, &dwBufSize)))
    {
        this->pOwner->hrAsync = E_FAIL;
        return S_OK;
    }

    // Create our own copy of it.
    if(FAILED(this->pOwner->Buffer.AddBytes(pBuffer, dwBufSize)))
    {
        this->pOwner->hrAsync = E_FAIL;
        return S_OK;
    }

    return S_OK;
}


HRESULT CAudioReaderCallback::AllocateForOutput(
    DWORD, DWORD cbBuffer, INSSBuffer** ppBuffer, PVOID pUser)
{
    return S_OK;
}


HRESULT CAudioReaderCallback::AllocateForStream(
    WORD wStreamNum, DWORD cbBuffer, INSSBuffer** ppBuffer, PVOID pUser)
{
    return S_OK;
}


HRESULT CAudioReaderCallback::OnOutputPropsChanged(
    DWORD dwOutputNum, WM_MEDIA_TYPE* pMediaType, PVOID pUser)
{
    return S_OK;
}


HRESULT CAudioReaderCallback::OnStreamSample(
    WORD, QWORD qwTime, QWORD qwLen, DWORD dwFlags,
    INSSBuffer* pSample, PVOID pUser)
{
    return S_OK;
}


HRESULT CAudioReaderCallback::OnStreamSelection(
    WORD wStreamCount, WORD* pStreamNumbers,
    WMT_STREAM_SELECTION* pSelections, PVOID pUser)
{
    return S_OK;
}


HRESULT CAudioReaderCallback::OnTime(QWORD qwCurrentTime,PVOID pUser)
{
    SetEvent(this->pOwner->hEventOnTime);
    return S_OK;
}




